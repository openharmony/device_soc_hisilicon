/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
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
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/list.h>
#include <asm/cacheflush.h>
#include <linux/version.h>
#include "securec.h"
#include "allocator.h"
#include "mmz_comm.h"
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0)
#include <linux/libnvdimm.h>
#endif

long long g_ot_max_malloc_size = 0x40000000UL;        /* 1GB */


static int calculate_fixed_region(unsigned long end, unsigned long start,
    unsigned long *fixed_start, unsigned long *fixed_len, unsigned long len)
{
    unsigned long blank_len;
    int ret = 0;

    blank_len = end - start;
    if ((blank_len < *fixed_len) && (blank_len >= len)) {
        *fixed_len = blank_len;
        *fixed_start = start;
        mmz_trace(MMZ_INFO_LEVEL, "fixed_region: start=0x%08lX, len=%luKB\n",
            *fixed_start, *fixed_len / SZ_1K);
        ret = 1;
    }
    return ret;
}

static unsigned long find_fit_region(unsigned long *region_len,
    ot_mmz_t *mmz, unsigned long size, unsigned long align)
{
    unsigned long start;
    unsigned long len;
    unsigned long fixed_start = 0;
    unsigned long fixed_len = -1;
    ot_mmb_t *p = NULL;

    align = mmz_grain_align(align);
    if (align == 0) {
        align = MMZ_GRAIN;
    }
    start = mmz_align2(mmz->phys_start, align);
    len = mmz_grain_align(size);

    list_for_each_entry(p, &mmz->mmb_list, list) {
        ot_mmb_t *next = NULL;
        mmz_trace(MMZ_INFO_LEVEL, "p->phys_addr=0x%08lX p->length = %luKB \t", p->phys_addr, p->length / SZ_1K);
        next = list_entry(p->list.next, typeof(*p), list);
        mmz_trace(MMZ_INFO_LEVEL, ",next = 0x%08lX\n\n", next->phys_addr);
        /*
         * if p is the first entry or not.
         */
        if (list_first_entry(&mmz->mmb_list, typeof(*p), list) == p) {
            (void)calculate_fixed_region(p->phys_addr, start, &fixed_start, &fixed_len, len);
        }
        start = mmz_align2((p->phys_addr + p->length), align);
        /* if aglin is larger than mmz->nbytes, it would trigger the BUG_ON */
        /* if we have to alloc after the last node.  */
        if (osal_list_is_last(&p->list, &mmz->mmb_list)) {
            if (calculate_fixed_region(mmz->phys_start + mmz->nbytes, start, &fixed_start, &fixed_len, len) == 1) {
                break;
            }

            if (fixed_len != (unsigned long)-1) {
                goto out;
            }
            fixed_start = 0;
            mmz_trace(MMZ_INFO_LEVEL, "fixed_region: start=0x%08lX, len=%luKB\n", fixed_start, fixed_len / SZ_1K);
            goto out;
        }
        /* blank is too small */
        if ((start + len) > next->phys_addr) {
            mmz_trace(MMZ_INFO_LEVEL, "start=0x%08lX ,len=%lu,next=0x%08lX\n", start, len, next->phys_addr);
            continue;
        }

        (void)calculate_fixed_region(next->phys_addr, start, &fixed_start, &fixed_len, len);
    }

    if ((mmz_grain_align(start + len) <= (mmz->phys_start + mmz->nbytes)) &&
        (start >= mmz->phys_start) && (start < (mmz->phys_start + mmz->nbytes))) {
        fixed_start = start;
    } else {
        fixed_start = 0;
    }

    mmz_trace(MMZ_INFO_LEVEL, "fixed_region: start=0x%08lX, len=%luKB\n", fixed_start, len / SZ_1K);
out:
    *region_len = len;
    return fixed_start;
}

static unsigned long find_fixed_region_from_highaddr(unsigned long *region_len,
                                                     const ot_mmz_t *mmz, unsigned long size, unsigned long align)
{
    int j;
    unsigned int i;
    unsigned long fixed_start = 0;
    unsigned long fixed_len = ~1;

    mmz_trace_func();

    i = mmz_length2grain(mmz->nbytes);

    while (i > 0) {
        unsigned int scan_i;
        unsigned long start;
        unsigned long len;
        unsigned long start_highaddr;

        if (mmz_get_bit(mmz, i)) {
            i--;
            continue;
        }

        len = 0;
        start_highaddr = mmz_pos2phy_addr(mmz, i);
        for (scan_i = i; scan_i > 0; scan_i--) {
            if (mmz_get_bit(mmz, scan_i)) {
                break;
            }

            len += MMZ_GRAIN;
        }

        if (len >= size) {
            j = mmz_phy_addr2pos(mmz, mmz_align2low(start_highaddr - size, align));
            start = mmz_pos2phy_addr(mmz, j);
            if ((start_highaddr - len <= start) && (start <= start_highaddr - size)) {
                fixed_len = len;
                fixed_start = start;
                break;
            }

            mmz_trace(1, "fixed_region: start=0x%08lX, len=%luKB",
                      fixed_start, fixed_len / SZ_1K);
        }
        i = (scan_i > 0) ? (scan_i - 1) : 0;
    }

    *region_len = fixed_len;

    return fixed_start;
}

static ot_mmb_t *ot_mmz_mmb_alloc(const ot_mmz_alloc_para_in *para_in)
{
    ot_mmz_t *mmz = NULL;
    unsigned long start;
    unsigned long region_len;
    unsigned long fixed_start = 0;
    unsigned long fixed_len = ~1;
    ot_mmz_t *fixed_mmz = NULL;

    const char *mmz_name = para_in->mmz_name;
    unsigned long size = para_in->size;
    unsigned long align = para_in->align;
    unsigned long gfp = para_in->gfp;
    const ot_mmz_t *user_mmz = para_in->user_mmz;

    mmz_trace_func();
    if ((size == 0) || (size > g_ot_max_malloc_size)) {
        return NULL;
    }
    if (align == 0) {
        align = MMZ_GRAIN;
    }

    size = mmz_grain_align(size);

    mmz_trace(1, "size=%luKB, align=%lu", size / SZ_1K, align);

    begin_list_for_each_mmz(mmz, gfp, mmz_name)

    if ((user_mmz != NULL) && (user_mmz != mmz)) {
        continue;
    }

    start = find_fit_region(&region_len, mmz, size, align);
    if ((fixed_len > region_len) && (start != 0)) {
        fixed_len = region_len;
        fixed_start = start;
        fixed_mmz = mmz;
    }
    end_list_for_each_mmz()

    if (fixed_mmz == NULL) {
        return NULL;
    }

    return do_mmb_alloc(para_in, fixed_mmz, fixed_start, size);
}

static ot_mmb_t *ot_mmz_mmb_alloc_v2(const ot_mmz_alloc_para_in *para_in)
{
    ot_mmz_t *mmz = NULL;

    unsigned long start = 0;
    unsigned long region_len = 0;

    unsigned long fixed_start = 0;
    unsigned long fixed_len = ~1;
    ot_mmz_t *fixed_mmz = NULL;

    const char *mmz_name = para_in->mmz_name;
    unsigned long size = para_in->size;
    unsigned long align = para_in->align;
    unsigned long gfp = para_in->gfp;
    unsigned int order = para_in->order;
    const ot_mmz_t *user_mmz = para_in->user_mmz;

    if ((size == 0) || (size > 0x40000000UL)) {
        return NULL;
    }
    if (align == 0) {
        align = 1;
    }

    size = mmz_grain_align(size);

    mmz_trace(1, "size=%luKB, align=%lu", size / SZ_1K, align);

    begin_list_for_each_mmz(mmz, gfp, mmz_name)

    if ((user_mmz != NULL) && (user_mmz != mmz)) {
        continue;
    }

    if (mmz->alloc_type == SLAB_ALLOC) {
        size = mmz_align_power2(size);
    } else if (mmz->alloc_type == EQ_BLOCK_ALLOC) {
        size = mmz_align2(size, mmz->block_align);
    }

    if (order == LOW_TO_HIGH) {
        start = find_fit_region(&region_len, mmz, size, align);
    } else if (order == HIGH_TO_LOW) {
        start = find_fixed_region_from_highaddr(&region_len, mmz, size, align);
    }

    if ((fixed_len > region_len) && (start != 0)) {
        fixed_len = region_len;
        fixed_start = start;
        fixed_mmz = mmz;
    }

    end_list_for_each_mmz()

    if (fixed_mmz == NULL) {
        return NULL;
    }

    return do_mmb_alloc(para_in, fixed_mmz, fixed_start, size);
}

static unsigned long adjust_start_addr(unsigned long begin, unsigned long end, unsigned long mark, unsigned long len)
{
    unsigned long start_addr;

    if ((begin + len) > mark) {
        start_addr = begin;
    } else if ((mark + len) > end) {
        start_addr = end - len;
    } else {
        start_addr = mark;
    }

    return start_addr;
}

typedef struct {
    unsigned long start;
    unsigned long size;
    unsigned long align;
    int *error_code;
} mmz_region_search_ctx;

static unsigned long find_region_from_head(ot_mmz_t *mmz, const ot_mmb_t *cur, const mmz_region_search_ctx *ctx)
{
    unsigned long region_start = mmz_align2(mmz->phys_start, ctx->align);
    if ((region_start + ctx->size) <= cur->phys_addr) {
        *(ctx->error_code) = MMZ_REGION_FREE;
        return adjust_start_addr(region_start, cur->phys_addr, ctx->start, ctx->size);
    }

    *(ctx->error_code) = MMZ_REGION_NOT_ENOUGH;
    mmz_trace(MMZ_INFO_LEVEL, "Find from head fail, mmz_start=0x%lX, pos=0x%lX, mmb_begin=0x%lX\n",
        mmz->phys_start, ctx->start, cur->phys_addr);
    return 0;
}

static unsigned long find_region_from_tail(unsigned long region_start, unsigned long mmz_end,
    unsigned long start, unsigned long size, int *error_code)
{
    if ((region_start + size) <= mmz_end) {
        *error_code = MMZ_REGION_FREE;
        return adjust_start_addr(region_start, mmz_end, start, size);
    }

    *error_code = MMZ_REGION_NOT_ENOUGH;
    mmz_trace(MMZ_INFO_LEVEL, "Find from tail failed, mmb_start=0x%08lX, pos=0x%08lX, mmz_end=0x%08lX\n\n",
        region_start, start, mmz_end);
    return 0;
}

static unsigned long find_region_from_middle(unsigned long region_start, unsigned long region_end,
    unsigned long start, unsigned long size, int *error_code)
{
    if ((region_end - region_start) >= size) {
        *error_code = MMZ_REGION_FREE;
        return adjust_start_addr(region_start, region_end, start, size);
    }

    *error_code = MMZ_REGION_NOT_ENOUGH;
    mmz_trace(MMZ_INFO_LEVEL, "Find from middle fail, pre_end=0x%08lX, pos=0x%08lX, nxt_start=0x%08lX\n\n",
        region_start, start, region_end);
    return 0;
}

static int check_fixed_region_range(const ot_mmz_t *mmz, unsigned long start, int *error_code)
{
    unsigned long mmz_start;
    unsigned long mmz_end;

    if (mmz == NULL) {
        *error_code = MMZ_REGION_OUT_RANGE;
        return -1;
    }
    mmz_start = mmz->phys_start;
    mmz_end = mmz->phys_start + mmz->nbytes;

    if (start < mmz_start || start >= mmz_end) {
        *error_code = MMZ_REGION_OUT_RANGE;
        return -1;
    }
    return 0;
}

static unsigned long find_fixed_region_in_mmb_list(ot_mmz_t *mmz, unsigned long start, unsigned long size,
    unsigned long align, int *error_code)
{
    unsigned long region_start;
    unsigned long region_end;
    unsigned long mmz_end = mmz->phys_start + mmz->nbytes;
    mmz_region_search_ctx search_ctx = {start, size, align, error_code};
    ot_mmb_t *cur = NULL;
    ot_mmb_t *next = NULL;

    if (osal_list_empty(&mmz->mmb_list) != 0) {
        *error_code = MMZ_REGION_FREE;
        region_start = mmz_align2(mmz->phys_start, align);
        return adjust_start_addr(region_start, mmz_end, start, size);
    }

    list_for_each_entry(cur, &mmz->mmb_list, list) {
        if (start >= cur->phys_addr && start < (cur->phys_addr + cur->length)) {
            *error_code = MMZ_REGION_BUSY;
            return 0;
        }

        if ((list_first_entry(&mmz->mmb_list, typeof(*cur), list) == cur) &&
            (start < cur->phys_addr)) {
            return find_region_from_head(mmz, cur, &search_ctx);
        }

        region_start = mmz_align2((cur->phys_addr + cur->length), align);
        if (osal_list_is_last(&cur->list, &mmz->mmb_list)) {
            return find_region_from_tail(region_start, mmz_end, start, size, error_code);
        }

        next = list_entry(cur->list.next, typeof(*cur), list);
        region_end = next->phys_addr;
        if (start < region_end) {
            return find_region_from_middle(region_start, region_end, start, size, error_code);
        }
    }

    mmz_trace(MMZ_INFO_LEVEL, "Invalid: mmz_start=0x%08lX, pos=0x%08lX, mmz_end=0x%08lX\n",
        mmz->phys_start, start, mmz_end);

    *error_code = MMZ_REGION_OUT_RANGE;
    return 0;
}

static unsigned long find_fixed_region(ot_mmz_t *mmz, unsigned long start, unsigned long size,
    unsigned long align, int *error_code)
{
    if (check_fixed_region_range(mmz, start, error_code) != 0) {
        return 0;
    }
    return find_fixed_region_in_mmb_list(mmz, start, size, align, error_code);
}

static ot_mmb_t *ot_mmz_mmb_alloc_v3(const ot_mmz_alloc_para_in *para_in, ot_mmz_alloc_para_out *para_out)
{
    ot_mmz_t *mmz = NULL;
    unsigned long fixed_start = 0;

    const char *mmz_name = para_in->mmz_name;
    unsigned long start = para_in->start;
    unsigned long size = para_in->size;
    unsigned long align = para_in->align;
    unsigned long gfp = para_in->gfp;

    if ((size == 0) || (size > g_ot_max_malloc_size)) {
        return NULL;
    }

    align = mmz_grain_align(align);
    if (align == 0) {
        align = MMZ_GRAIN;
    }

    size = mmz_align2(size, align);
    start &= ~(align - 1);

    mmz_trace(MMZ_INFO_LEVEL, "start= %lx size=%lx, align=%lx", start, size, align);

    begin_list_for_each_mmz(mmz, gfp, mmz_name)

    fixed_start = find_fixed_region(mmz, start, size, align, &(para_out->erro_num));
    if (para_out->erro_num != MMZ_REGION_OUT_RANGE) {
        break;
    }

    end_list_for_each_mmz()

    if (mmz == NULL || fixed_start == 0) {
        mmz_trace(MMZ_INFO_LEVEL, "find fixed region failed. fixed_start= %lx", fixed_start);
        return NULL;
    }

    return do_mmb_alloc(para_in, mmz, fixed_start, size);
}

static void *ot_mmz_mmb_map2kern(ot_mmb_t *mmb, int cached)
{
    /*
      * already mapped? no need to remap again,
      * just return mmb's kernel virtual address.
      */
    if (mmb->flags & OT_MMB_MAP2KERN) {
        if ((!!cached * OT_MMB_MAP2KERN_CACHED) != (mmb->flags & OT_MMB_MAP2KERN_CACHED)) {
            osal_trace(KERN_ERR "mmb<%s> has been kernel-mapped as %s, can not be re-mapped as %s.",
                   mmb->name,
                   (mmb->flags & OT_MMB_MAP2KERN_CACHED) ? "cached" : "non-cached",
                   (cached) ? "cached" : "non-cached");
            return NULL;
        }

        mmb->map_ref++;

        return mmb->kvirt;
    }

    if (cached) {
        mmb->flags |= OT_MMB_MAP2KERN_CACHED;
        mmb->kvirt = ioremap_cache(mmb->phys_addr, mmb->length);
    } else {
        mmb->flags &= ~OT_MMB_MAP2KERN_CACHED;
        /* ioremap_wc has better performance */
        mmb->kvirt = ioremap_wc(mmb->phys_addr, mmb->length);
    }

    if (mmb->kvirt) {
        mmb->flags |= OT_MMB_MAP2KERN;
        mmb->map_ref++;
    } else {
        mmb->flags &= ~OT_MMB_MAP2KERN_CACHED;
    }

    return mmb->kvirt;
}

static void ot_mmz_mmb_free(ot_mmb_t *mmb)
{
    if (mmb->flags & OT_MMB_MAP2KERN_CACHED) {
#ifdef CONFIG_64BIT
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
    __flush_dcache_area((void *)mmb->kvirt, (size_t)mmb->length);
#else
    dcache_clean_inval_poc((unsigned long)mmb->kvirt, (unsigned long)mmb->kvirt + mmb->length);
#endif
#else
        __cpuc_flush_dcache_area((void *)mmb->kvirt, (size_t)mmb->length);
        outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
#endif
    }

    osal_list_del(&mmb->list);
    kfree(mmb);
}

static int ot_mmz_mmb_unmap(ot_mmb_t *mmb)
{
    int ref;

    if (mmb->flags & OT_MMB_MAP2KERN_CACHED) {
#ifdef CONFIG_64BIT
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
    __flush_dcache_area((void *)mmb->kvirt, (size_t)mmb->length);
#else
    dcache_clean_inval_poc((unsigned long)mmb->kvirt, (unsigned long)mmb->kvirt + mmb->length);
#endif
#else
        __cpuc_flush_dcache_area((void *)mmb->kvirt, (size_t)mmb->length);
        outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
#endif
    }

    if (mmb->flags & OT_MMB_MAP2KERN) {
        ref = --mmb->map_ref;
        if (mmb->map_ref != 0) {
            return ref;
        }
        iounmap(mmb->kvirt);
    }

    mmb->kvirt = NULL;
    mmb->flags &= ~OT_MMB_MAP2KERN;
    mmb->flags &= ~OT_MMB_MAP2KERN_CACHED;

    if ((mmb->flags & OT_MMB_RELEASED) && (mmb->phy_ref == 0)) {
        ot_mmz_mmb_free(mmb);
    }

    return 0;
}

static void *ot_mmz_mmf_map(phys_addr_t phys, int len, int cache)
{
    void *virt = NULL;
    if (cache) {
        virt = ioremap_cache(phys, len);
    } else {
        virt = ioremap_wc(phys, len);
    }

    return virt;
}

static void ot_mmz_mmf_unmap(void *virt)
{
    if (virt != NULL) {
        iounmap(virt);
    }
}

static int allocator_init_parse_args(ot_mmz_t **zone, int argc, char **argv)
{
    ot_mmz_t *mmz_zone = NULL;
    ot_mmz_create_attr mmz_create_attr = {0};
    if (argc == 4) { /* 4:Number of parameters */
        mmz_zone = ot_mmz_create("null", 0, 0, 0);
    } else if (argc == 6) { /* 6:Number of parameters */
        mmz_zone = ot_mmz_create_v2("null", &mmz_create_attr);
    } else {
        osal_trace(KERN_ERR "error parameters\n");
        return -EINVAL;
    }
    if (mmz_zone == NULL) {
        return 0;
    }
    if (strncpy_s(mmz_zone->name, OT_MMZ_NAME_LEN, argv[0], OT_MMZ_NAME_LEN - 1) != EOK) {
        osal_trace("%s - strncpy_s failed!\n", __FUNCTION__);
        ot_mmz_destroy(mmz_zone);
        return -1;
    }
    mmz_zone->gfp        = _strtoul_ex(argv[1], NULL, 0); /* 1: the second args */
    mmz_zone->phys_start = _strtoul_ex(argv[2], NULL, 0); /* 2: the third args */
    mmz_zone->nbytes     = _strtoul_ex(argv[3], NULL, 0); /* 3: the fourth args */
    if (mmz_zone->nbytes > g_ot_max_malloc_size) {
        g_ot_max_malloc_size = mmz_zone->nbytes;
    }
    if (argc == 6) { /* 6: had parse six args */
        mmz_zone->alloc_type  = _strtoul_ex(argv[4], NULL, 0); /* 4: the fifth args */
        mmz_zone->block_align = _strtoul_ex(argv[5], NULL, 0); /* 5: the sixth args */
    }
    *zone = mmz_zone;
    return 1;
}

static int ot_allocator_init(char *s)
{
    ot_mmz_t *zone = NULL;
    char *line = NULL;
    unsigned long phys_end;
    int ret;

    while ((line = strsep(&s, ":")) != NULL) {
        int i;
        char *argv[6]; /* 6: cmdline include 6 arguments */

        for (i = 0; (argv[i] = strsep(&line, ",")) != NULL;) {
            if (++i == ARRAY_SIZE(argv)) {
                break;
            }
        }

        ret = allocator_init_parse_args(&zone, i, argv);
        if (ret == 0) {
            continue;
        } else if (ret < 0) {
            return ret;
        }

        if (ot_mmz_register(zone)) {
            osal_trace(KERN_WARNING "Add MMZ failed: " OT_MMZ_FMT_S "\n", ot_mmz_fmt_arg(zone));
            ot_mmz_destroy(zone);
            return -1;
        }

        /* if phys_end is maximum value (ex, 0xFFFFFFFF 32bit) */
        phys_end = (zone->phys_start + zone->nbytes);

        if ((phys_end == 0) && (zone->nbytes >= PAGE_SIZE)) {
            /* reserve last PAGE_SIZE memory */
            zone->nbytes = zone->nbytes - PAGE_SIZE;
        }

        /* if phys_end exceed 0xFFFFFFFF (32bit), wrapping error */
        if ((zone->phys_start > phys_end) && (phys_end != 0)) {
            osal_trace(KERN_ERR "MMZ: parameter is not correct! Address exceeds 0xFFFFFFFF\n");
            ot_mmz_unregister(zone);
            ot_mmz_destroy(zone);
            return -1;
        }

        zone = NULL;
    }

    return 0;
}

int ot_allocator_setopt(struct mmz_allocator *allocator)
{
    if (allocator == NULL) {
        osal_trace(KERN_ERR "error parameters\n");
        return -1;
    }
    allocator->init = ot_allocator_init;
    allocator->mmb_alloc = ot_mmz_mmb_alloc;
    allocator->mmb_alloc_v2 = ot_mmz_mmb_alloc_v2;
    allocator->mmb_alloc_v3 = ot_mmz_mmb_alloc_v3;
    allocator->mmb_map2kern = ot_mmz_mmb_map2kern;
    allocator->mmb_unmap = ot_mmz_mmb_unmap;
    allocator->mmb_free = ot_mmz_mmb_free;
    allocator->mmf_map = ot_mmz_mmf_map;
    allocator->mmf_unmap = ot_mmz_mmf_unmap;
    return 0;
}
