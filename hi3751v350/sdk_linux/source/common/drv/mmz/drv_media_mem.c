/*
 * Copyright (C) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/list.h>
#include <asm/cacheflush.h>
#include <linux/time.h>
#include <asm/setup.h>
#include <linux/dma-mapping.h>
#include <asm/memory.h>

#ifndef CONFIG_64BIT
#include <asm/highmem.h>
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 0)
#include <linux/cma.h>
#include <linux/dma-buf.h>
#include <linux/dma-map-ops.h>
#include <linux/dma-heap.h>
#endif

#include <linux/hisilicon/hisi_iommu.h>
#include <asm/tlbflush.h>
#include <asm/pgtable.h>
#include <linux/seq_file.h>
#include <linux/err.h>
#include "securec.h"
#include "drv_media_mem.h"

#define DEFAULT_ALLOC  0
#define SLAB_ALLOC     1
#define EQ_BLOCK_ALLOC 2

#define LOW_TO_HIGH 0
#define HIGH_TO_LOW 1

#define mmz_trace(s, params...) \
    do {                                        \
            hi_mmz_debug(s "\n", ##params);     \
    } while (0)
#define mmz_trace_func() mmz_trace("%s", __FILE__)

#define MMZ_GRAIN          PAGE_SIZE
#define mmz_bitmap_size(p) (mmz_align2(mmz_length2grain((p)->nbytes), 8) / 8)

#define mmz_get_bit(p, n) (((p)->bitmap[(n) / 8] >> ((n) & 0x7)) & 0x1)
#define mmz_set_bit(p, n) (p)->bitmap[(n) / 8] |= 1 << ((n) & 0x7)
#define mmz_clr_bit(p, n) (p)->bitmap[(n) / 8] &= ~(1 << ((n) & 0x7))

#define mmz_pos2phy_addr(p, n) ((p)->phys_start + (n) * MMZ_GRAIN)
#define mmz_phy_addr2pos(p, a) (((a) - (p)->phys_start) / MMZ_GRAIN)

#define mmz_align2low(x, g)   (((x) / (g)) * (g))
#define mmz_align2(x, g)      ((((x) + (g)-1) / (g)) * (g))
#define mmz_grain_align(x)    mmz_align2(x, MMZ_GRAIN)
#define mmz_length2grain(len) (mmz_grain_align(len) / MMZ_GRAIN)

char g_line[COMMAND_LINE_SIZE];
int g_mmz_print_level = MMZ_WARN_PRINT_LEVEL;

#define NAME_LEN_MAX 64

struct iommu_zone {
    unsigned int iova_start;
    unsigned int iova_end;
    unsigned int iova_align;
};

#define SPLIT_LINE \
        "--------------------------------------------------------------------------------------------------------\n"

#ifdef CONFIG_HISI_IOMMU
extern struct iommu_zone *hisi_get_iommu_zone(void);
#else
struct iommu_zone g_hisi_iommu;
static inline struct iommu_zone *hisi_get_iommu_zone(void)
{
    g_hisi_iommu.iova_start = 0x100000;
    g_hisi_iommu.iova_end = 0xffefffff;
    g_hisi_iommu.iova_align = 4096; /* 4096 表示对齐长度为4096 */

    return &g_hisi_iommu;
}
#endif

#define ZONE_MAX 64
struct mmz_iommu g_mmz_iommu;

LIST_HEAD(g_mmz_list);
DEFINE_SEMAPHORE(g_mmz_lock);

static int g_anony = 0;

module_param(g_anony, int, S_IRUGO);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
static void __dma_clear_buffer(struct ion_handle *handle);
#endif

static int _mmb_free(hil_mmb_t *mmb);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
static struct mmz_iommu *get_mmz_iommu_root(void)
{
    return &g_mmz_iommu;
}
#endif

static void hil_media_memory_zone_free(const void *addr)
{
    kfree(addr);
}

hil_mmz_t *hil_mmz_create(const char *name, HI_U32 gfp, HI_U32 zone_start, HI_U32 nbytes)
{
    hil_mmz_t *p = NULL;
    int ret;

    mmz_trace_func();
    if (name == NULL) {
        hi_mmz_error("'name' can not be zero!");
        return NULL;
    }

    p = kmalloc(sizeof(hil_mmz_t) + 1, GFP_KERNEL);
    if (p == NULL) {
        return NULL;
    }

    ret = memset_s(p, sizeof(hil_mmz_t) + 1, 0, sizeof(hil_mmz_t) + 1);
    if (ret != EOK) {
        hi_mmz_error("'memset fail\n");
        kfree(p);
        p = NULL;
        return NULL;
    }
    ret = strncpy_s(p->name, HIL_MAX_NAME_LEN, name, (HIL_MAX_NAME_LEN - 1));
    if (ret != EOK) {
        hi_mmz_error("strlncpy fail\n");
        kfree(p);
        p = NULL;
        return NULL;
    }

    p->gfp = gfp;
    p->zone_start = zone_start;
    p->nbytes = nbytes;

    INIT_LIST_HEAD(&p->list);

    p->destructor = hil_media_memory_zone_free;
    return p;
}

#ifndef DMABUF_FLUSH_CACHE
void flush_inner_cache(const void *viraddr, unsigned int len)
{
    if ((viraddr == NULL) || (len == 0)) {
        hi_mmz_warn("failed, viraddr:%pK len:0x%x!\n", viraddr, len);
        return;
    }

    mmz_flush_dcache_area((void *)viraddr, (size_t)len);
}

#ifndef CONFIG_64BIT
static void flush_outer(hil_mmb_t *mmb)
{
    struct scatterlist *sg = NULL;
    int i = 0;
    unsigned long size;
    struct sg_table *table = NULL;
    struct mmz_iommu *common = &g_mmz_iommu;

#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 0)
    table = hi_dma_buf_sgt(mmb->handle);
#else
    table = get_pages_from_buffer(common->client, mmb->handle, &size);
#endif
    if (table == NULL) {
        hi_mmz_warn("get pages failed!\n");
        return;
    }

    for_each_sg(table->sgl, sg, table->nents, i) {
        struct page *page = sg_page(sg);
        HI_U32 len = PAGE_ALIGN(sg->length);
        HI_U32 phys = __pfn_to_phys(page_to_pfn(page));

        outer_flush_range(phys, phys + len);
    }
}

/* just for A9 */
void flush_outer_cache_range(mmb_addr_t phyaddr, mmb_addr_t len, unsigned int iommu)
{
    hil_mmb_t *mmb = NULL;

    if (!iommu) {
        outer_flush_range(phyaddr, phyaddr + len);
        return;
    }

    mmb = hil_mmb_getby_phys(phyaddr, iommu);
    if (mmb == NULL) {
        hi_mmz_warn("invalid args!\n");
        return;
    }

    if (!mmb->iommu) {
        outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
    } else {
        flush_outer(mmb);
    }
}
#else
void flush_outer_cache_range(mmb_addr_t phyaddr, mmb_addr_t len, unsigned int iommu)
{
    (void)phyaddr;
    (void)len;
    (void)iommu;
    return;
}
#endif
#endif // DMABUF_FLUSH_CACHE

int hil_mmz_destroy(const hil_mmz_t *zone)
{
    if (zone == NULL) {
        return -1;
    }

    if (zone->destructor != NULL) {
        zone->destructor(zone);
        zone = NULL;
    }

    return 0;
}

int hil_mmz_register(hil_mmz_t *zone)
{
    if (zone == NULL) {
        return -1;
    }

    mmz_trace(HIL_MMZ_FMT, hil_mmz_fmt_arg(zone));

    down(&g_mmz_lock);
    list_add(&zone->list, &g_mmz_list);
    up(&g_mmz_lock);

    return 0;
}

int hil_mmz_unregister(hil_mmz_t *zone)
{
    int losts = 0;
    hil_mmb_t *p = NULL;
    struct rb_node *n = NULL;

    if (zone == NULL) {
        return -1;
    }

    mmz_trace_func();

    down(&g_mmz_lock);
    for (n = rb_first(&zone->root); n; n = rb_next(n)) {
        if (zone->iommu) {
            p = rb_entry(n, hil_mmb_t, s_node);
        } else {
            p = rb_entry(n, hil_mmb_t, node);
        }

        hi_mmz_warn("Lost: " HIL_MMB_FMT "\n", hil_mmb_fmt_arg(p));
        losts++;
    }

    if (losts) {
        hi_mmz_error("  %d mmbs not free, mmz<%s> can not be deregistered!\n", losts, zone->name);
        up(&g_mmz_lock);
        return -1;
    }

    list_del(&zone->list);
    hil_mmz_destroy(zone);
    up(&g_mmz_lock);

    return 0;
}

#if !(HI_PROC_SUPPORT == 0)
static void dump_mem_head(const hil_mmz_t *p, const int zone_cnt)
{
    struct rb_node *n = NULL;
    char *mem_type = NULL;
    char *smmu_name = "SMMU";
    char *phy_name = "DDR";
    unsigned int number = 0;

    if (p->iommu) {
        mem_type = smmu_name;
    } else {
        mem_type = phy_name;
    }

    hi_mmz_debug(SPLIT_LINE);
    hi_mmz_debug("|                   %s           |  ID  | ZONE  |  KVIRT  |  FLAGS  |"
                 "  LENGTH(KB)  |       NAME        |\n", mem_type);
    hi_mmz_debug(SPLIT_LINE);
    for (n = rb_first(&p->root); n; n = rb_next(n)) {
        number++;
    }
    hi_mmz_debug("|ZONE[%d]: (0x%08x, 0x%08x)   %d         %d        0x%08x      %-10u   \"%s%-14s|\n", zone_cnt,
                 p->zone_start, p->zone_start + p->nbytes - 1, number,
                 p->iommu, p->gfp, p->nbytes / SZ_1K, p->name, "\"");
}

static void dump_mem_statistics(const hil_mmz_t *p, const int zone_cnt, int block_number, const unsigned int used_size)
{
    int total_size = p->nbytes / 1024; /* 1024 1M = 1024 Bytes */
    unsigned int free_size = total_size - used_size;

    hi_mmz_debug(SPLIT_LINE);
    hi_mmz_debug("|%-102s|\n", "Summary:");
    hi_mmz_debug(SPLIT_LINE);
    if (p->iommu) {
        hi_mmz_debug("|  SMMU Total Size  |    Iommu Used     |     Idle     |  Zone Number  |"
                     "   BLock Number                 |\n");
    } else {
        hi_mmz_debug("|  MMZ Total Size  |     CMA Used      |     Idle     |  Zone Number  |"
                     "   BLock Number                 |\n");
    }
    hi_mmz_debug(SPLIT_LINE);
    hi_mmz_debug("|       %d%-8s       %d%-8s          %d%-8s         %d              %d                |\n",
                 total_size / 1024, "MB", used_size / 1024, "MB", /* 1024 1M = 1024 Bytes */
                 free_size / 1024, "MB", zone_cnt, block_number); /* 1024 1M = 1024 Bytes */
    hi_mmz_debug(SPLIT_LINE);
    hi_mmz_debug("\n");
}

static unsigned int dump_mem_zone(const hil_mmz_t *p, int *block_number)
{
    hil_mmb_t *mmb = NULL;
    struct rb_node *n = NULL;
    unsigned int used_size = 0;
    int block_number_tmp = 0;

    for (n = rb_first(&p->root); n; n = rb_next(n)) {
        if (p->iommu) {
            mmb = rb_entry(n, hil_mmb_t, s_node);
            if (!mmb->iommu) {
                continue;
            }
        } else {
            mmb = rb_entry(n, hil_mmb_t, node);
        }
        hi_mmz_debug("|" HIL_MMB_FMT "|\n", hil_mmb_fmt_arg(mmb));
        used_size += mmb->length / 1024; /* 1024 1M = 1024 Bytes */
        block_number_tmp++;
    }

    *block_number += block_number_tmp;
    return used_size;
}
#endif

static void dump_mem(void)
{
#if !(HI_PROC_SUPPORT == 0)

    int zone_cnt = 0;
    int block_number;
    hil_mmz_t *p;
    unsigned int used_size;

    list_for_each_entry(p, &g_mmz_list, list) {
#ifndef HI_SMMU_SUPPORT
        if (p->iommu) {
            continue;
        }
#endif
        dump_mem_head(p, zone_cnt);
        block_number = 0;
        used_size = dump_mem_zone(p, &block_number);
        dump_mem_statistics(p, zone_cnt, block_number, used_size);
        zone_cnt++;
    }
#endif
}

#ifdef HI_ADVCA_FUNCTION_RELEASE
#define mmz_dump_stack()
#else
#define mmz_dump_stack()  dump_stack()
#endif

hil_mmb_t *hil_mmb_getby_phys(HI_U32 addr, HI_U32 iommu)
{
    hil_mmz_t *zone = NULL;
    hil_mmb_t *mmb = NULL;

    if (addr == 0) {
        hi_mmz_error("err args\n");
        if (g_mmz_print_level > MMZ_WARN_PRINT_LEVEL) {
            mmz_dump_stack();
        }
        return NULL;
    }

    down(&g_mmz_lock);
    list_for_each_entry(zone, &g_mmz_list, list) {
        struct rb_node *n = NULL;

        if (zone->iommu != iommu) {
            continue;
        }

        for (n = rb_first(&zone->root); n; n = rb_next(n)) {
            hil_mmb_t *m = NULL;
            if (!iommu) {
                m = rb_entry(n, hil_mmb_t, node);
                if ((m->phys_addr <= addr) && (addr < (m->phys_addr + m->length))) {
                    mmb = m;
                    goto result;
                }
            } else {
                m = rb_entry(n, hil_mmb_t, s_node);
                if ((m->iommu_addr <= addr) && (addr < (m->iommu_addr + m->length))) {
                    mmb = m;
                    goto result;
                }
            }
        }
    }
result:
    if (mmb == NULL) {
        if (iommu) {
            hi_mmz_warn("smmu:0x%x err args\n", addr);
        } else {
            hi_mmz_warn("phys:0x%x err args\n", addr);
        }

        if (g_mmz_print_level > MMZ_WARN_PRINT_LEVEL) {
            mmz_dump_stack();
            dump_mem();
        }
    }

    up(&g_mmz_lock);
    return mmb;
}

hil_mmb_t *hil_mmb_getby_sec_addr(HI_U32 sec_addr, HI_U32 iommu)
{
    hil_mmz_t *zone = NULL;
    hil_mmb_t *mmb = NULL;

    if (sec_addr == 0) {
        hi_mmz_warn("err args\n");
        return NULL;
    }

    down(&g_mmz_lock);
    list_for_each_entry(zone, &g_mmz_list, list) {
        struct rb_node *n = NULL;

        for (n = rb_first(&zone->root); n; n = rb_next(n)) {
            hil_mmb_t *m = NULL;
            if (!iommu) {
                /* is sec_addr is phys_addr, so it just in ddr zone */
                m = rb_entry(n, hil_mmb_t, node);
                if (m->phys_addr <= sec_addr && sec_addr < (m->phys_addr + m->length)) {
                    mmb = m;
                    goto result;
                }
            } else {
                /* if sec_addr is sec_smmu, then it maybe both in ddr zone and smmu zone */
                if (zone->iommu) {
                    m = rb_entry(n, hil_mmb_t, s_node);
                } else {
                    m = rb_entry(n, hil_mmb_t, node);
                }
                if (m->sec_smmu != 0 && m->sec_smmu <= sec_addr && sec_addr < (m->sec_smmu + m->length)) {
                    mmb = m;
                    goto result;
                }
            }
        }
    }
result:
    up(&g_mmz_lock);
    if (mmb == NULL) {
        if (iommu) {
            hi_mmz_warn("smmu:0x%x err args\n", sec_addr);
        } else {
            hi_mmz_warn("phys:0x%x err args\n", sec_addr);
        }
    }
    return mmb;
}

static int mmb_add_to_rbtree(hil_mmb_t *mmb, hil_mmz_t *mmz, int iommu)
{
    struct rb_node **p = &mmz->root.rb_node;
    struct rb_node *parent = NULL;
    hil_mmb_t *entry = NULL;

    if (iommu) {
        while (*p) {
            parent = *p;
            entry = rb_entry(parent, hil_mmb_t, s_node);
            if (mmb->iommu_addr < entry->iommu_addr) {
                p = &(*p)->rb_left;
            } else if (mmb->iommu_addr > entry->iommu_addr) {
                p = &(*p)->rb_right;
            } else {
                hi_mmz_warn("buffer already found.\n");
                BUG();
                return HI_FAILURE;
            }
        }
        rb_link_node(&mmb->s_node, parent, p);
        rb_insert_color(&mmb->s_node, &mmz->root);
    } else {
        while (*p) {
            parent = *p;
            entry = rb_entry(parent, hil_mmb_t, node);
            if (mmb->phys_addr < entry->phys_addr) {
                p = &(*p)->rb_left;
            } else if (mmb->phys_addr > entry->phys_addr) {
                p = &(*p)->rb_right;
            } else {
                hi_mmz_warn("buffer already found.\n");
                BUG();
                return HI_FAILURE;
            }
        }

        rb_link_node(&mmb->node, parent, p);
        rb_insert_color(&mmb->node, &mmz->root);
    }
    return HI_SUCCESS;
}

#define  MMZ_NODE_FIRST_MATCHED    1
#define  MMZ_NODE_LAST_MATCHED     0

/*
 * first : 1,find the first node which matches the requirement
 *         0,find the last node which matches the requirement
 */
static hil_mmz_t *list_for_each_mmz(HI_U32 gfp, const char *mmz_name, int first)
{
    hil_mmz_t *p = NULL;
    hil_mmz_t *mmz = NULL;

    list_for_each_entry(p, &g_mmz_list, list) {
        if (gfp == 0 ? 0 : p->gfp != gfp) {
            continue;
        }
        if (mmz_name != NULL) {
            if ((*mmz_name != '\0') && strncmp(mmz_name, p->name, HIL_MAX_NAME_LEN)) {
                continue;
            }
        }
        if (mmz_name == NULL && g_anony == 1) {
            if (strncmp("anonymous", p->name, HIL_MAX_NAME_LEN)) {
                continue;
            }
        }

        mmz = p;
        if (first) {
            break;
        }
    }
    return mmz;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
static int real_page_buffer_alloc_and_map(hil_mmb_t *mmb, HI_U32 size, HI_U32 align, int iommu,
                                          unsigned int heap_id_mask)
{
    struct mmz_iommu *common = get_mmz_iommu_root();
    struct ion_client *client = common->client;
    struct iommu_map_format *format = NULL;
    const unsigned int flags = 0;
    int ret = 0;

    /* alloc real page buffer via ion interface */
    mmb->handle = ion_alloc(client, size, align, heap_id_mask, flags);
    if (IS_ERR(mmb->handle)) {
        hi_mmz_warn("mem alloc failed !\n");
        return -1;  /* -1, error */
    }
    __dma_clear_buffer(mmb->handle);

    if (iommu) {
        format = kzalloc(sizeof(struct iommu_map_format), GFP_KERNEL);
        if (format == NULL) {
            hi_mmz_warn("no mem!\n");
            goto err;
        }
        /* map real page buffer to smmu space */
        ret = ion_map_iommu(client, mmb->handle, format);
        if (ret) {
            hi_mmz_warn("alloc iommu failed!\n");
            kfree(format);
            goto err;
        }

        mmb->phys_addr = MMB_ADDR_INVALID;
        mmb->iommu_addr = format->iova_start;
        mmb->length = format->iova_size;
        kfree(format);
        format = NULL;
    } else {
        size_t len = 0;
        unsigned long phys = 0;
        /* get cma phy address and len of real page buffer */
        ret = ion_phys(client, mmb->handle, &phys, &len);
        if (ret) {
            hi_mmz_error("cannot get phys_addr!\n");
            goto err;
        }

        mmb->phys_addr = (HI_U32)phys;
        mmb->iommu_addr = MMB_ADDR_INVALID;
        mmb->length = (HI_U32)len;
    }

    mmb->client = client;
    return 0;  /* 0, success */
err:
    ion_free(client, mmb->handle);
    return -1;  /* -1, error */
}
#else
static int real_page_buffer_alloc_and_map(hil_mmb_t *mmb, HI_U32 size, HI_U32 align, int iommu,
                                          struct dma_heap *heap)
{
    dma_addr_t iova_start;
    unsigned int fd_flags = O_RDWR | O_CLOEXEC;
    unsigned int heap_flags = 0; /* 0: Currently no heap flags */

    mmb->handle = hi_dma_buf_alloc(heap, size, fd_flags, heap_flags);

    if (IS_ERR(mmb->handle)) {
        hi_mmz_warn("mem alloc failed !\n");
        return -1;  /* -1, error */
    }

    if (iommu) {
        iova_start = dma_buf_map_iommu(mmb->handle);
        if (!iova_start) {
            hi_mmz_warn("alloc iommu failed!\n");
            goto err;
        }

        mmb->phys_addr = MMB_ADDR_INVALID;
        mmb->iommu_addr = iova_start;
    } else {
        unsigned long phys = 0;
        /* get cma phy address and len of real page buffer */
        phys = dma_buf_phys(mmb->handle);
        if (!phys) {
            hi_mmz_error("cannot get phys_addr!\n");
            goto err;
        }

        mmb->phys_addr = (HI_U32)phys;
        mmb->iommu_addr = MMB_ADDR_INVALID;
    }
    mmb->length = mmb->handle->size;

    return 0;  /* 0, success */
err:
    dma_buf_put(mmb->handle);
    return -1;  /* -1, error */
}
#endif

static int hil_mmb_init(hil_mmb_t *mmb, hil_mmz_t *mmz, unsigned int size, unsigned int align, int flag)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
    unsigned int heap_id_mask;
#else
    struct dma_heap *heap;
#endif
    int iommu;
    int ret;

    iommu = mmz->iommu;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
    heap_id_mask = mmz->heap_id_mask;
    ret = real_page_buffer_alloc_and_map(mmb, size, align, iommu, heap_id_mask);
#else
    heap = mmz->heap;
    ret = real_page_buffer_alloc_and_map(mmb, size, align, iommu, heap);
#endif
    if (ret) {
        hi_mmz_warn("real_page_buffer_alloc_and_map fail\n");
        return ret;
    }

    mmb->zone = mmz;
    mmb->iommu = iommu;
    mmb->phy_ref++;
    mmb->flag = flag; /* used when release */
    mmb->kdata = NULL;
    mmb->sec_smmu = MMB_ADDR_INVALID; /* for secure smmu */
    spin_lock_init(&mmb->u_lock);

    if (flag == HI_USER_ALLOC) {
        mmb->owner_id = current->tgid;
    } else {
        mmb->owner_id = 0;
    }
    INIT_LIST_HEAD(&mmb->ulist);

    return 0;
}

hil_mmb_t *hil_mmb_alloc(const char *name, HI_U32 size, HI_U32 align, const char *mmz_name, int flag)
{
    hil_mmb_t *mmb = NULL;
    hil_mmz_t *mmz = NULL;
    const char *mmb_name = (name == NULL) ? "<null>" : name;
    int iommu;
    int ret;
    const unsigned int gfp = 0;  /* gfp is always 0 in later version */

    down(&g_mmz_lock);

    mmz = list_for_each_mmz(gfp, mmz_name, MMZ_NODE_LAST_MATCHED);
    if (mmz == NULL) {
        hi_mmz_warn("can't find zone:%s\n", mmz_name);
        goto err_exit;
    }
    iommu = mmz->iommu;

    mmb = kmalloc(sizeof(hil_mmb_t), GFP_KERNEL);
    if (mmb == NULL) {
        goto err_exit;
    }
    ret = memset_s(mmb, sizeof(hil_mmb_t), 0, sizeof(hil_mmb_t));
    if (ret != 0) {
        hi_mmz_error("memset fail");
        goto handle_err;
    }

    ret = strncpy_s(mmb->name, HIL_MAX_NAME_LEN, mmb_name, HIL_MAX_NAME_LEN - 1);
    if (ret != EOK) {
        hi_mmz_warn("strncpy fail\n");
        goto handle_err;
    }

    ret = hil_mmb_init(mmb, mmz, size, align, flag);
    if (ret != EOK) {
        hi_mmz_warn("hil_mmb_init failed\n");
        goto handle_err;
    }

    ret  = mmb_add_to_rbtree(mmb, mmz, iommu);
    if (ret != HI_SUCCESS) {
        hi_mmz_warn("add to rbtree failed!\n");
        goto handle_err;
    }

    up(&g_mmz_lock);
    return mmb;

handle_err:
    kfree(mmb);
    mmb = NULL;
err_exit:
    up(&g_mmz_lock);
    return NULL;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
static void __dma_clear_buffer(struct ion_handle *handle)
{
    struct scatterlist *sg = NULL;
    int i = 0;
    HI_U32 size;
    struct sg_table *table = NULL;
    struct mmz_iommu *common = &g_mmz_iommu;
    unsigned long len;

    table = get_pages_from_buffer(common->client, handle, &len);
    if (table == NULL) {
        hi_mmz_warn("get pages failed!\n");
        return;
    }

    size = (HI_U32)len;
    size = PAGE_ALIGN(size);
    for_each_sg(table->sgl, sg, table->nents, i) {
        struct page *page = sg_page(sg);
        HI_U32 length = PAGE_ALIGN(sg->length);
#ifdef CONFIG_64BIT
        void *ptr = page_address(page);
        mmz_flush_dcache_area(ptr, length);
#else
        HI_U32 phys = __pfn_to_phys(page_to_pfn(page));

        if (PageHighMem(page)) {
            while (length > 0) {
                void *ptr = kmap_atomic(page);

                mmz_flush_dcache_area(ptr, PAGE_SIZE);
                __kunmap_atomic(ptr);
                page++;
                length -= PAGE_SIZE;
            }
        } else {
            void *ptr = page_address(page);
            mmz_flush_dcache_area(ptr, length);
        }
        outer_flush_range(phys, phys + length);
#endif
    }
}
#endif

#ifndef DMABUF_FLUSH_CACHE
static void *_map2kern(const hil_mmb_t *mmb, int cached)
{
    struct scatterlist *sg = NULL;
    int i = 0;
    int j = 0;
    void *vaddr = NULL;
    pgprot_t pgprot;
    int npages;
    HI_U32 size;
    struct sg_table *table = NULL;
    struct page **pages = NULL;
    struct page **tmp = NULL;
    struct mmz_iommu *common = &g_mmz_iommu;
    unsigned long len;

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
    table = get_pages_from_buffer(common->client, mmb->handle, &len);
#else
    table = hi_dma_buf_sgt(mmb->handle);
    len = mmb->length;
#endif
    if (table == NULL) {
        hi_mmz_warn("get pages failed!\n");
        return NULL;
    }
    size = (HI_U32)len;
    npages = PAGE_ALIGN(size) / PAGE_SIZE;
    pages = vmalloc(sizeof(struct page *) * npages);
    tmp = pages;

    if (pages == HI_NULL) {
        hi_mmz_warn("no mem!\n");
        return NULL;
    }
    pgprot = PAGE_KERNEL_EXEC;

    if (!cached) {
        pgprot = pgprot_writecombine(PAGE_KERNEL_EXEC);
    }

    for_each_sg(table->sgl, sg, table->nents, i) {
        int npages_this_entry = PAGE_ALIGN(sg->length) / PAGE_SIZE;
        struct page *page = sg_page(sg);
        for (j = 0; j < npages_this_entry; j++) {
            *(tmp++) = page++;
        }
    }
    vaddr = vmap(pages, npages, VM_MAP, pgprot);
    vfree(pages);
    return vaddr;
}
#endif

static void *_mmb_map2kern(hil_mmb_t *mmb, int cached)
{
    struct mmb_kdata *kdata = NULL;

    kdata = mmb->kdata;
    if (kdata != NULL) {
        if (kdata->map_cached != cached) {
            hi_mmz_warn("mmb<%s> already kernel-mapped one cache attr, now another cache "
                        "attr re-mapped requested. But the first remap is returned\n", mmb->name);
        }
        mmb->phy_ref++;
        mmb->map_ref++;
        kdata->kmap_ref++;
        return kdata->kvirt;
    }
    kdata = kmalloc(sizeof(struct mmb_kdata), GFP_KERNEL | __GFP_ZERO);
    if (kdata == NULL) {
        hi_mmz_warn("remap failed!\n");
        return NULL;
    }
#ifndef DMABUF_FLUSH_CACHE
    kdata->kvirt = _map2kern(mmb, cached);
#else
    kdata->kvirt = dma_buf_vmap(mmb->handle);
#endif
    if (kdata->kvirt == NULL) {
        hi_mmz_warn("remap failed in ion!\n");
        kfree(kdata);
        return NULL;
    }
    kdata->map_cached = cached;
    kdata->kmap_ref++;

    mmb->kdata = kdata;
    mmb->phy_ref++;
    mmb->map_ref++;

    return kdata->kvirt;
}

void *hil_mmb_map2kern(hil_mmb_t *mmb)
{
    void *p = NULL;

    if (mmb == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    p = _mmb_map2kern(mmb, 0);
    up(&g_mmz_lock);

    return p;
}

void *hil_mmb_map2kern_cached(hil_mmb_t *mmb)
{
    void *p = NULL;

    if (mmb == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    p = _mmb_map2kern(mmb, 1);
    up(&g_mmz_lock);

    return p;
}

#ifndef DMABUF_FLUSH_CACHE
int hil_mmb_unmap(hil_mmb_t *mmb, const void *addr)
{
    struct mmb_kdata *kdata = NULL;
    mmb_addr_t phyaddr;

    if (mmb == NULL) {
        return -1;
    }
    down(&g_mmz_lock);

    kdata = mmb->kdata;
    if (kdata == NULL) {
        hi_mmz_warn("cannot find userdata!\n");
        up(&g_mmz_lock);
        return HI_FAILURE;
    }
    if (mmb->iommu) {
        phyaddr = mmb->iommu_addr;
    } else {
        phyaddr = mmb->phys_addr;
    }

    if (kdata->map_cached) {
        up(&g_mmz_lock);
#ifndef CONFIG_64BIT
        __cpuc_flush_dcache_area((void *)kdata->kvirt, (size_t)mmb->length);
        flush_outer_cache_range(phyaddr, mmb->length, mmb->iommu);
#else
        __flush_dcache_area((void *)kdata->kvirt, (size_t)mmb->length);
#endif
        down(&g_mmz_lock);
    }

    kdata->kmap_ref--;
    if (!kdata->kmap_ref) {
        vunmap(kdata->kvirt);
        kfree(kdata);
        mmb->kdata = NULL;
    }

    mmb->map_ref--;
    mmb->phy_ref--;

    if ((mmb->phy_ref == 0) && (mmb->map_ref == 0) && (mmb->cma_smmu_ref == 0) && (mmb->sec_smmu_ref == 0)) {
        hil_mmb_free(mmb);
        mmb = NULL;
    }

    up(&g_mmz_lock);
    return 0;
}
#endif

static int _mmb_free(hil_mmb_t *mmb)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
    if (mmb->iommu) {
        ion_unmap_iommu(mmb->client, mmb->handle);
    }

    ion_free(mmb->client, mmb->handle);
#else
    if (mmb->iommu) {
        dma_buf_unmap_iommu(mmb->iommu_addr, mmb->handle);
    }

    dma_buf_put(mmb->handle);
#endif

    if (mmb->iommu) {
        rb_erase(&mmb->s_node, &mmb->zone->root);
    } else {
        rb_erase(&mmb->node, &mmb->zone->root);
    }
    kfree(mmb);

    return 0;
}

int hil_mmb_free(hil_mmb_t *mmb)
{
    struct mmb_udata *p = NULL;
    struct mmb_udata *q = NULL;
    hil_mmz_t *zone = NULL;
    int found = 0;

    if (mmb == NULL) {
        return HI_FAILURE;
    }

    list_for_each_entry(zone, &g_mmz_list, list) {
        struct rb_node *n = NULL;

        for (n = rb_first(&zone->root); n; n = rb_next(n)) {
            hil_mmb_t *m = NULL;
            if (zone->iommu) {
                m = rb_entry(n, hil_mmb_t, s_node);
                if (mmb == m) {
                    found = 1;
                    break;
                }
            } else {
                m = rb_entry(n, hil_mmb_t, node);
                if (mmb == m) {
                    found = 1;
                    break;
                }
            }
        }
    }

    if (found == 0) {
        return HI_FAILURE;
    }

    if (mmb->phy_ref > 0) {
        mmb->phy_ref--;
    }

    if ((mmb->map_ref) || (mmb->phy_ref) || (mmb->cma_smmu_ref) || (mmb->sec_smmu_ref)) {
        hi_mmz_debug("name=%s, mmz(0x%x) smmu(0x%x) is still used: "
                     "phy_ref:%d map_ref:%d smmu_ref:%d sec_smmu_ref:0x%d\n",
                     mmb->name, mmb->phys_addr, mmb->iommu_addr,
                     mmb->phy_ref, mmb->map_ref, mmb->cma_smmu_ref, mmb->sec_smmu_ref);
        return 0;
    }
    spin_lock(&mmb->u_lock);
    if (!list_empty(&mmb->ulist)) {
        /*
         * if we are in exception mode like killing process by ctrl+c
         * the udata in mmb maybe not free, we free them there.
         * And unmap is called normally, udata has been free before and
         * this branch do not get in.
         */
        list_for_each_entry_safe(p, q, &mmb->ulist, list) {
            list_del(&p->list);
            kfree(p);
        }
    }
    spin_unlock(&mmb->u_lock);
    _mmb_free(mmb);
    mmb = NULL;

    return 0;
}

hil_mmb_t *hil_mmb_getby_kvirt(const void *virt)
{
    hil_mmb_t *mmb = NULL;
    hil_mmz_t *zone = NULL;

    if (virt == NULL) {
        hi_mmz_warn("virt:%pK err args\n", virt);
        return NULL;
    }

    down(&g_mmz_lock);
    list_for_each_entry(zone, &g_mmz_list, list) {
        struct rb_node *n;
        for (n = rb_first(&zone->root); n; n = rb_next(n)) {
            hil_mmb_t *m = NULL;

            if (zone->iommu) {
                m = rb_entry(n, hil_mmb_t, s_node);
            } else {
                m = rb_entry(n, hil_mmb_t, node);
            }
            if (m->kdata == NULL) {
                continue;
            }
            if (((uintptr_t)(m->kdata->kvirt) <= (uintptr_t)virt) &&
                ((uintptr_t)virt < ((uintptr_t)m->kdata->kvirt + m->length))) {
                mmb = m;
                goto end;
            }
        }
    }
end:
    up(&g_mmz_lock);

    if (mmb == NULL) {
        hi_mmz_warn("virt:%pK cannot find mem\n", virt);
    }

    return mmb;
}

hil_mmb_t *hil_mmbinfo_getby_kvirt(const void *virt)
{
    hil_mmb_t *mmb = NULL;
    hil_mmz_t *zone = NULL;

    if (virt == NULL) {
        hi_mmz_warn("virt:%pK err args\n", virt);
        return NULL;
    }

    down(&g_mmz_lock);
    list_for_each_entry(zone, &g_mmz_list, list) {
        struct rb_node *n;
        for (n = rb_first(&zone->root); n; n = rb_next(n)) {
            hil_mmb_t *m = NULL;

            if (zone->iommu) {
                m = rb_entry(n, hil_mmb_t, s_node);
            } else {
                m = rb_entry(n, hil_mmb_t, node);
            }
            if (m->kdata == NULL) {
                continue;
            }
            if (((uintptr_t)(m->kdata->kvirt) <= (uintptr_t)virt) &&
                ((uintptr_t)(m->kdata->kvirt + m->length) >= (uintptr_t)virt)) {
                mmb = m;
                goto end;
            }
        }
    }
end:
    up(&g_mmz_lock);
    if (mmb == NULL) {
        hi_mmz_warn("virt:%pK cannot find mem\n", virt);
    }

    return mmb;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
static int hil_mmb_alloc_iommu(hil_mmb_t *mmb)
{
    struct iommu_map_format *format;
    struct mmz_iommu *common = &g_mmz_iommu;
    int ret;

    format = kmalloc(sizeof(struct iommu_map_format), GFP_KERNEL | __GFP_ZERO);
    if (format == NULL) {
        hi_mmz_warn("no mem!\n");
        return -1;  /* -1, error */
    }
    ret = ion_map_iommu(common->client, mmb->handle, format);
    if (ret) {
        hi_mmz_warn("alloc iommu failed!\n");
        kfree(format);
        return ret;
    }

    mmb->iommu_addr = format->iova_start;
    kfree(format);
    format = NULL;

    return ret;
}
#else
static int hil_mmb_alloc_iommu(hil_mmb_t *mmb)
{
    int ret;

    if (mmb->handle == NULL) {
        hi_mmz_warn("err args!\n");
        return MMB_ADDR_INVALID;
    }

    ret = dma_buf_map_iommu(mmb->handle);
    if (!ret) {
        hi_mmz_warn("dma_buf_map_iommu failed\n");
        return MMB_ADDR_INVALID;
    }

    mmb->iommu_addr = ret;

    return 0;
}
#endif

mmb_addr_t hil_mmb_cma_mapto_iommu(mmb_addr_t addr, int iommu)
{
    hil_mmb_t *mmb = NULL;
    hil_mmz_t *mmz = NULL;
    const HI_U32 gfp = 0; /* gfp is 0 in later version */
    int ret;
    mmb_addr_t offset;

    if (iommu) {
        hi_mmz_warn("err args, iommu must be 0, and addr must be cma phy_addr\n");
        return MMB_ADDR_INVALID;
    }

    mmb = hil_mmb_getby_phys((HI_U32)addr, 0);
    if (mmb == NULL) {
        hi_mmz_warn("err args!\n");
        return MMB_ADDR_INVALID;
    }

    down(&g_mmz_lock);
    if (mmb->iommu_addr != MMB_ADDR_INVALID) {
        hi_mmz_warn("It's already mapped to iommu.\n");
        mmb->cma_smmu_ref++;
        mmb->phy_ref++;
        up(&g_mmz_lock);
        return mmb->iommu_addr;
    }
    offset = addr - mmb->phys_addr;

    ret = hil_mmb_alloc_iommu(mmb);
    if (ret) {
        hi_mmz_warn("hil_mmb_alloc_iommu failed!\n");
        goto err;
    }

    mmb->cma_smmu_ref++;
    mmb->phy_ref++;

    mmz = list_for_each_mmz(gfp, "iommu", MMZ_NODE_LAST_MATCHED);
    if (mmz == NULL) {
        hi_mmz_warn("cannot find iommu zone!\n");
        goto err;
    }

    ret = mmb_add_to_rbtree(mmb, mmz, 1);
    if (ret != HI_SUCCESS) {
        hi_mmz_warn("cannot find iommu zone!\n");
        goto err;
    }

    up(&g_mmz_lock);
    return (mmb->iommu_addr + offset);

err:
    up(&g_mmz_lock);
    return MMB_ADDR_INVALID;
}

int hil_mmb_cma_unmapfrom_iommu(mmb_addr_t addr, int iommu)
{
    hil_mmb_t *mmb = NULL;
    hil_mmz_t *mmz = NULL;
    const HI_U32 gfp = 0;

    if (!iommu) {
        hi_mmz_warn("err args, iommu must be 1, and addr must be cma phy_addr\n");
        return HI_FAILURE;
    }

    mmb = hil_mmb_getby_phys((HI_U32)addr, 1);
    if (mmb == NULL || mmb->handle == NULL) {
        hi_mmz_warn("err args!\n");
        return HI_FAILURE;
    }

    down(&g_mmz_lock);
    if (mmb->iommu_addr != MMB_ADDR_INVALID) {
        mmb->cma_smmu_ref--;
        mmb->phy_ref--;
    }

    if (mmb->cma_smmu_ref) {
        up(&g_mmz_lock);
        return HI_SUCCESS;
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
    ion_unmap_iommu(mmb->client, mmb->handle);
#else
    dma_buf_unmap_iommu(mmb->iommu_addr, mmb->handle);
#endif
    mmz = list_for_each_mmz(gfp, "iommu", MMZ_NODE_LAST_MATCHED);
    if (mmz == NULL) {
        hi_mmz_warn("cannot find iommu zone!\n");
        up(&g_mmz_lock);
        return HI_FAILURE;
    }

    mmb->iommu_addr = MMB_ADDR_INVALID;
    rb_erase(&mmb->s_node, &mmz->root);

    if ((mmb->phy_ref == 0) && (mmb->map_ref == 0) && (mmb->cma_smmu_ref == 0) && (mmb->sec_smmu_ref == 0)) {
        hil_mmb_free(mmb);
    }
    up(&g_mmz_lock);

    return HI_SUCCESS;
}


struct sg_table *hil_get_meminfo(const hil_mmb_t *mmb)
{
    unsigned long size;
    struct sg_table *table = NULL;
    struct mmz_iommu *common = &g_mmz_iommu;

    if (mmb == NULL) {
        hi_mmz_error("invalid params!\n");
        return NULL;
    }
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
    table = get_pages_from_buffer(common->client, mmb->handle, &size);
#else
    table = hi_dma_buf_sgt(mmb->handle);
#endif
    if (table == NULL) {
        hi_mmz_warn("get pages failed!\n");
        return NULL;
    }
    return table;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
int sec_mmb_get(u32 addr, int iommu, u32 sec_smmu)
{
    hil_mmb_t *mmb;

    mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
    if (mmb == NULL) {
        hi_mmz_warn("err args!\n");
        return HI_FAILURE;
    }
    down(&g_mmz_lock);

    if (!mmb->sec_smmu_ref) {
        mmb->sec_smmu = sec_smmu;
    }

    mmb->sec_smmu_ref++;
    mmb->phy_ref++;

    up(&g_mmz_lock);
    return HI_SUCCESS;
}

int sec_mmb_put(u32 addr, int iommu)
{
    hil_mmb_t *mmb = NULL;
    int ref;

    mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
    if (mmb == NULL) {
        hi_mmz_warn("err args, addr:0x%x iommu:0x%x!\n", addr, iommu);
        return HI_FAILURE;
    }
    down(&g_mmz_lock);

    if (!mmb->sec_smmu_ref || !mmb->phy_ref) {
        hi_mmz_warn("wrong operation.mmb->sec_smmu_ref :%d mmb->phy_ref: %d \n",
                    mmb->sec_smmu_ref, mmb->phy_ref);
        up(&g_mmz_lock);
        return HI_FAILURE;
    }

    mmb->sec_smmu_ref--;
    mmb->phy_ref--;
    if (!mmb->sec_smmu_ref) {
        mmb->sec_smmu = MMB_ADDR_INVALID;
    }

    ref = mmb->sec_smmu_ref;
    if ((mmb->phy_ref == 0) && (mmb->map_ref == 0) && (mmb->cma_smmu_ref == 0) && (mmb->sec_smmu_ref == 0)) {
        hil_mmb_free(mmb);
    }

    up(&g_mmz_lock);
    return ref;
}

int sec_mmb_query_ref(HI_U32 sec_addr, int iommu, HI_U32 *ref)
{
    hil_mmb_t *mmb = NULL;

    if (ref == NULL) {
        hi_mmz_warn("ref should not be null!\n");
        return HI_FAILURE;
    }

    mmb = hil_mmb_getby_sec_addr(sec_addr, iommu);
    if (mmb == NULL) {
        hi_mmz_warn("err args!\n");
        return HI_FAILURE;
    }
    down(&g_mmz_lock);
    *ref = mmb->sec_smmu_ref;
    up(&g_mmz_lock);

    return HI_SUCCESS;
}

int set_sec_mmb_flag(u32 addr, int iommu)
{
    hil_mmb_t *mmb;

    mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
    if (mmb == NULL) {
        hi_mmz_warn("err args!\n");
        return HI_FAILURE;
    }
    down(&g_mmz_lock);
    mmb->sec_flag = 1;
    up(&g_mmz_lock);
    return HI_SUCCESS;
}

int clr_sec_mmb_flag(u32 addr, int iommu)
{
    hil_mmb_t *mmb;

    mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
    if (mmb == NULL) {
        hi_mmz_warn("err args!\n");
        return HI_FAILURE;
    }
    down(&g_mmz_lock);
    mmb->sec_flag = 0;
    up(&g_mmz_lock);
    return HI_SUCCESS;
}

/* return 0 : normal mem ; return 1 :sec mem */
int is_sec_mem(u32 addr, int iommu)
{
    hil_mmb_t *mmb;
    int ret;

    mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
    if (mmb == NULL) {
        hi_mmz_warn("err args!\n");
        return HI_FAILURE;
    }

    down(&g_mmz_lock);
    ret = mmb->sec_flag;
    up(&g_mmz_lock);

    return ret;
}

int sec_delay_release_for_mem(u32 addr, int iommu)
{
    hil_mmb_t *mmb;

    mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
    if (mmb == NULL) {
        hi_mmz_warn("err args!\n");
        return HI_FAILURE;
    }

    down(&g_mmz_lock);
    if ((mmb->phy_ref == 0) && (mmb->map_ref == 0) && (mmb->cma_smmu_ref == 0) && (mmb->sec_smmu_ref == 0)) {
        hil_mmb_free(mmb);
    }
    up(&g_mmz_lock);

    return 0;
}
#endif

int mmb_get(u32 addr, int iommu)
{
    hil_mmb_t *mmb;

    mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
    if (mmb == NULL) {
        hi_mmz_warn("err args!\n");
        return HI_FAILURE;
    }

    down(&g_mmz_lock);
    mmb->phy_ref++;
    up(&g_mmz_lock);

    return HI_SUCCESS;
}

int mmb_put(u32 addr, int iommu)
{
    hil_mmb_t *mmb;

    mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
    if (mmb == NULL) {
        hi_mmz_warn("err args!\n");
        return HI_FAILURE;
    }

    down(&g_mmz_lock);
    if (mmb->phy_ref > 0) {
        mmb->phy_ref--;
    } else {
        hi_mmz_warn("phy_ref is wrong:ref:%d\n", mmb->phy_ref);
        up(&g_mmz_lock);
        return HI_FAILURE;
    }

    if (!mmb->phy_ref && !mmb->map_ref && !mmb->cma_smmu_ref && !mmb->sec_smmu_ref) {
        hil_mmb_free(mmb);
    }

    up(&g_mmz_lock);
    return HI_SUCCESS;
}

int mmb_ref_query(u32 addr, int iommu, u32 *ref)
{
    hil_mmb_t *mmb = NULL;

    if (ref == NULL) {
        hi_mmz_warn("ref should not be null!\n");
        return HI_FAILURE;
    }

    mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
    if (mmb == NULL) {
        hi_mmz_warn("err args!\n");
        return HI_FAILURE;
    }
    down(&g_mmz_lock);
    *ref = mmb->phy_ref;
    up(&g_mmz_lock);

    return HI_SUCCESS;
}

int mem_source_query(u32 iommu_addr, int *source)
{
    phys_addr_t phys;
    hil_mmb_t *mmb = NULL;
    hil_mmz_t *zone = NULL;
    hil_mmb_t *m = NULL;

    if (source == NULL || !iommu_addr) {
        hi_mmz_warn("source or iommu_addr should not be null!\n");
        return HI_FAILURE;
    }

    phys = hisi_iommu_domain_iova_to_phys(iommu_addr);
    if (!phys) {
        /* iommu_addr is illegal */
        *source = -1;
        goto out;
    }

    down(&g_mmz_lock);
    list_for_each_entry(zone, &g_mmz_list, list) {
        struct rb_node *n = NULL;

        if (zone->iommu != 1) {
            continue;
        }

        for (n = rb_first(&zone->root); n; n = rb_next(n)) {
            m = NULL;
            m = rb_entry(n, hil_mmb_t, s_node);
            if ((m->iommu_addr <= iommu_addr) && (iommu_addr < (m->iommu_addr + m->length))) {
                mmb = m;
                break;
            }
        }
    }
    if (mmb == NULL) {
        *source = 1; /* the iommu_addr from other source such ion */
    } else {
        *source = 0; /* the iommu_addr from mmz driver   */
    }

    up(&g_mmz_lock);

out:
    return HI_SUCCESS;
}
int sec_mem_source_query(u32 sec_iommu, int *source)
{
    hil_mmb_t *mmb = NULL;
    hil_mmz_t *zone = NULL;

    if (source == NULL) {
        return HI_FAILURE;
    }
    if (!sec_iommu) {
        hi_mmz_warn("sec_iommu:0x%x err args\n", sec_iommu);
        *source = -1;
        return HI_FAILURE;
    }

    down(&g_mmz_lock);
    list_for_each_entry(zone, &g_mmz_list, list) {
        struct rb_node *n;
        for (n = rb_first(&zone->root); n; n = rb_next(n)) {
            hil_mmb_t *m = NULL;

            if (zone->iommu) {
                m = rb_entry(n, hil_mmb_t, s_node);
            } else {
                m = rb_entry(n, hil_mmb_t, node);
            }

            if ((m->sec_smmu <= sec_iommu) && (sec_iommu < (m->sec_smmu + m->length))) {
                mmb = m;
                goto end;
            } else {
                continue;
            }
        }
    }
end:
    up(&g_mmz_lock);

    if (mmb == NULL) {
        *source = 1; /* the sec_smmu point buffer don't from mmz driver */
    } else {
        *source = 0; /* the sec_smmu  point buffer from mmz driver   */
    }

    return HI_SUCCESS;
}

static hil_mmz_t *hil_mmz_find(HI_U32 gfp, const char *mmz_name)
{
    hil_mmz_t *mmz = NULL;

    down(&g_mmz_lock);
    mmz = list_for_each_mmz(gfp, mmz_name, MMZ_NODE_FIRST_MATCHED);
    up(&g_mmz_lock);

    return mmz;
}

/*
 * name,gfp,phys_start,nbytes,alloc_type;
 * All param in hex mode, except name.
 */
static int media_mem_parse_cmdline(char *s)
{
    hil_mmz_t *zone = NULL;
    char *line = NULL;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
    struct ion_platform_heap *heap = NULL;
#else
    struct dma_heap *heap;
    struct cma *cma;
#endif
    int ret;

    line = strsep(&s, ":");
    while (line != NULL) {
        int i;
        char *argv[6]; /* 6 表示保存参数的字符串长度 */

        /*
         * We got 4 args in "line", formatted "argv[0], argv[1], argv[2], argv[3], argv[4]".
         * eg: "<mmz_name>, <gfp>, <phys_start_addr>, <size>, <alloc_type>"
         * For more convenient, "hard code" are used such as "arg[0]", i.e.
         */
        for (i = 0; (argv[i] = strsep(&line, ",")) != NULL;) {
            if (++i == ARRAY_SIZE(argv)) {
                break;
            }
        }
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
        heap = hisi_get_cma_heap(argv[0]);
        if (heap == NULL) {
            hi_mmz_warn("can't get cma zone info:%s\n", argv[0]);
            continue;
        }
#else
        heap = get_heap_by_name(CMA_HEAP_NAME);
        cma = dev_get_cma_area(NULL);
        if (heap == NULL || cma == NULL) {
            hi_mmz_warn("can't get cma zone info:%s\n", argv[0]);
            continue;
        }
#endif

        if (i == 4) { /* 4 表示第5个参数 */
            zone = hil_mmz_create("null", 0, 0, 0);
            if (zone == NULL) {
                continue;
            }
            ret = strncpy_s(zone->name, HIL_MAX_NAME_LEN, argv[0], (HIL_MAX_NAME_LEN - 1));
            if (ret != EOK) {
                hi_mmz_error("strncpy_s fail\n");
                hil_mmz_destroy(zone);
                return HI_FAILURE;
            }
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
            zone->gfp = 0;
            zone->zone_start = heap->base;
            zone->nbytes = heap->size;
            zone->heap_id_mask = 1 << (heap->id);
#else
            zone->gfp = 0;
            zone->zone_start = cma_get_base(cma);
            zone->nbytes = cma_get_size(cma);
            zone->heap = heap;
#endif
            /* all cma zones share one root,all mmb from cma zones are added to this root. */
            zone->root = RB_ROOT;
            zone->iommu = 0;
        } else {
            hi_mmz_error("MMZ: your parameter num is not correct!\n");
            continue;
        }

        if (hil_mmz_register(zone)) {
            hi_mmz_warn("Add MMZ failed: " HIL_MMZ_FMT "\n", hil_mmz_fmt_arg(zone));
            hil_mmz_destroy(zone);
        }

        zone = NULL;
    }
    return 0;
}

#define MAX_MMZ_INFO_LEN (64 * 1024)

#if !(HI_PROC_SUPPORT == 0)
static void mmz_proc_head(struct seq_file *m, const hil_mmz_t *p, const int zone_cnt)
{
    struct rb_node *n = NULL;
    char *mem_type = NULL;
    char *smmu_name = "SMMU";
    char *phy_name = "DDR";
    unsigned int number = 0;

    if (p->iommu) {
        mem_type = smmu_name;
    } else {
        mem_type = phy_name;
    }

    seq_puts(m, SPLIT_LINE);
    seq_printf(m, "|                   %s           |  ID  | ZONE  |  KVIRT  |  FLAGS  |"
               "  LENGTH(KB)  |       NAME        |\n", mem_type);
    seq_puts(m, SPLIT_LINE);

    for (n = rb_first(&p->root); n; n = rb_next(n)) {
        number++;
    }

    seq_printf(m, "|ZONE[%d]: (0x%08x, 0x%08x)   %d         %d        0x%08x      %-10u   \"%s%-14s|\n",
               zone_cnt, p->zone_start, p->zone_start + p->nbytes - 1, number, p->iommu, p->gfp,
               p->nbytes / SZ_1K, p->name, "\"");
}

static int mmz_proc_zone(struct seq_file *m, const hil_mmz_t *p, unsigned int *used_size, int *block_number)
{
    hil_mmb_t *mmb = NULL;
    struct rb_node *n = NULL;
    unsigned int used_size_tmp = 0;
    int block_number_tmp = 0;

    for (n = rb_first(&p->root); n; n = rb_next(n)) {
        if (p->iommu) {
            mmb = rb_entry(n, hil_mmb_t, s_node);
            if (!mmb->iommu) {
                continue;
            }
        } else {
            mmb = rb_entry(n, hil_mmb_t, node);
        }
        seq_printf(m, "|" HIL_MMB_FMT "|\n", hil_mmb_fmt_arg(mmb));
        if (m->count > MAX_MMZ_INFO_LEN) {
            hi_mmz_error("mmz_info_buff overflow(0x%x), more than 20k data!\n", (unsigned int)m->count);
            return -1;  /* -1, proc error */
        }
        used_size_tmp += mmb->length / 1024; /* 1024 表示1MB = 1024Bytes */
        block_number_tmp++;
    }

    *used_size += used_size_tmp;
    *block_number += block_number_tmp;

    return 0;  /* 0, proc success */
}

static void mmz_proc_statistics(struct seq_file *m, const hil_mmz_t *p, const int zone_cnt,
                                int block_number, const unsigned int used_size)
{
    int total_size = p->nbytes / 1024; /* 1024 表示1MB = 1024Bytes */
    unsigned int free_size = total_size - used_size;

    seq_puts(m, SPLIT_LINE);
    seq_printf(m, "|%-102s|\n", "Summary:");
    seq_puts(m, SPLIT_LINE);
    if (p->iommu) {
        seq_puts(m, "|  SMMU Total Size  |    Iommu Used     |     Idle     |  Zone Number  |"
                 "   BLock Number                 |\n");
    } else {
        seq_puts(m, "|  MMZ Total Size  |     CMA Used      |     Idle     |  Zone Number  |"
                 "   BLock Number                 |\n");
    }

    seq_puts(m, SPLIT_LINE);
    seq_printf(m, "|       %d%-8s       %d%-8s          %d%-8s         %d              %d                |\n",
               total_size / 1024, "MB", used_size / 1024, "MB", /* 1024 表示1MB = 1024Bytes */
               free_size / 1024, "MB", zone_cnt, block_number); /* 1024 表示1MB = 1024Bytes */
    seq_puts(m, SPLIT_LINE);
    seq_puts(m, "\n");
}
#endif

int mmz_read_proc(struct seq_file *m, void *v)
{
#if !(HI_PROC_SUPPORT == 0)
    int zone_cnt = 0;
    int block_number;
    hil_mmz_t *p = NULL;
    unsigned int used_size;
    int ret;

    if (m == NULL) {
        return HI_FAILURE;
    }
    down(&g_mmz_lock);

    /* Collect all mmb info into mmz_info_buff */
    list_for_each_entry(p, &g_mmz_list, list) {
#ifndef HI_SMMU_SUPPORT
        if (p->iommu) {
            continue;
        }
#endif

        mmz_proc_head(m, p, zone_cnt);
        if (m->count > MAX_MMZ_INFO_LEN) {
            hi_mmz_error("mmz_info_buff overflow(0x%x), more than 20k data!\n", (unsigned int)m->count);
            break;
        }

        block_number = 0;
        used_size = 0;
        ret = mmz_proc_zone(m, p, &used_size, &block_number);
        if (ret) {
            hi_mmz_error("mmz_proc_zone failed!\n");
            break;
        }

        mmz_proc_statistics(m, p, zone_cnt, block_number, used_size);
        if (m->count > MAX_MMZ_INFO_LEN) {
            hi_mmz_error("mmz_info_buff overflow(0x%x), more than 20k data!\n", (unsigned int)m->count);
            break;
        }
        zone_cnt++;
    }

    up(&g_mmz_lock);

#endif
    return 0;
}

#define MMZ_SETUP_CMDLINE_LEN 256
static char __initdata setup_zones[MMZ_SETUP_CMDLINE_LEN] = "ddr,0,0,160M";

static void mmz_exit_check(void)
{
    hil_mmz_t *p = NULL;

    mmz_trace_func();
    for (p = hil_mmz_find(0, NULL); p != NULL; p = hil_mmz_find(0, NULL)) {
        hil_mmz_unregister(p);
    }
}

static int mmz_zone_init(void)
{
    char *s = NULL;
    char *p = NULL;
    char *q = NULL;
    int ret;

    ret = strncpy_s(g_line, COMMAND_LINE_SIZE, saved_command_line, COMMAND_LINE_SIZE - 1);
    if (ret != EOK) {
        hi_mmz_error("strncpy_s fail\n");
        return HI_FAILURE;
    }

    q = strstr(g_line, "mmz=");
    if (q != NULL) {
        s = strsep(&q, "=");
        if (s != NULL) {
            p = strsep(&q, " ");
        }
        if (p != NULL) {
            ret = strncpy_s(setup_zones, MMZ_SETUP_CMDLINE_LEN, p, (MMZ_SETUP_CMDLINE_LEN - 1));
            if (ret != EOK) {
                hi_mmz_error("strncpy_s fail\n");
                return HI_FAILURE;
            }
        }
    }

    ret = media_mem_parse_cmdline(setup_zones);

    return ret;
}

#ifdef HI_SMMU_SUPPORT
static int iommu_zone_init(void)
{
    hil_mmz_t *zone = NULL;
    struct iommu_zone *iommu_zone;
#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 0)
    struct dma_heap *heap;
#endif
    int ret;

    iommu_zone = hisi_get_iommu_zone();
    if (iommu_zone == NULL) {
        hi_mmz_warn("iommu zone can not be used!\n");
        return HI_FAILURE;
    }

    zone = hil_mmz_create("null", 0, 0, 0);
    if (zone == NULL) {
        hi_mmz_warn("iommu zone created failed,iommu zone may not be used!\n");
        return HI_FAILURE;
    }
    ret = strcpy_s(zone->name, HIL_MAX_NAME_LEN, "iommu");
    if (ret != EOK) {
        hi_mmz_error("strncpy_s fail\n");
        hil_mmz_destroy(zone);
        return HI_FAILURE;
    }
    zone->gfp = 0;
    zone->zone_start = iommu_zone->iova_start;
    zone->nbytes = iommu_zone->iova_end - iommu_zone->iova_start + 1;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
    zone->heap_id_mask = ION_HEAP_SYSTEM_MASK;
#else
    heap = get_heap_by_name(SYSTEM_HEAP_NAME);
    if (heap == NULL) {
        hi_mmz_error("get dma heap failed!\n");
        hil_mmz_destroy(zone);
        return HI_FAILURE;
    }

    zone->heap = heap;
#endif
    zone->root = RB_ROOT;
    zone->iommu = 1;
    if (hil_mmz_register(zone)) {
        hi_mmz_warn("Add MMZ failed: " HIL_MMZ_FMT "\n", hil_mmz_fmt_arg(zone));
        hil_mmz_destroy(zone);
    }

    return HI_SUCCESS;
}
#endif // HI_SMMU_SUPPORT

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
static int client_init(void)
{
    struct mmz_iommu *common = &g_mmz_iommu;

    common->client = hisi_ion_client_create("cma-iommu");
    if (common->client == NULL) {
        hi_mmz_warn("ion client is created failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
#endif

static int g_init_done;
int hi_drv_mmz_init(void)
{
    int ret;

    if (g_init_done) {
        return 0;
    }
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
    ret = client_init();
    if (ret) {
        hi_mmz_warn("client init is failed!\n");
        return HI_FAILURE;
    }
#endif

    ret = mmz_zone_init();
    if (ret) {
        hi_mmz_warn("mmz zone init is failed!\n");
        return HI_FAILURE;
    }

#ifdef HI_SMMU_SUPPORT
    ret = iommu_zone_init();
    if (ret) {
        hi_mmz_warn("iommu zone init is failed!\n");
        return HI_FAILURE;
    }
#endif

    g_init_done = 1;
    return HI_SUCCESS;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
void hi_drv_mmz_exit(void)
{
    struct mmz_iommu *common = &g_mmz_iommu;

    mmz_exit_check();
    g_init_done = 0;
    hisi_ion_client_destroy(common->client);

    return;
}
#else
void hi_drv_mmz_exit(void)
{
    mmz_exit_check();
    g_init_done = 0;

    return;
}
#endif

EXPORT_SYMBOL(hi_drv_mmz_init);
EXPORT_SYMBOL(hi_drv_mmz_exit);
