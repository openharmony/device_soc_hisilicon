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

#include <generated/autoconf.h>
#include <linux/kernel.h>
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
#include <linux/version.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <asm/cacheflush.h>
#ifndef CONFIG_64BIT
#include <linux/highmem.h>
#include <asm/pgtable.h>
#endif
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/time.h>
#include <linux/dma-mapping.h>
#include "securec.h"
#include "ot_osal.h"
#include "allocator.h"
#include "mmz_comm.h"
#include "media_mem.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0)
#include <linux/libnvdimm.h>
#endif

OSAL_LIST_HEAD(g_mmz_list);
OSAL_LIST_HEAD(g_map_mmz_list);

int anony = 0;
static bool mem_process_isolation = true;
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
static DEFINE_SEMAPHORE(g_mmz_lock);
#else
static DEFINE_SEMAPHORE(g_mmz_lock, 1);
#endif

module_param(anony, int, S_IRUGO);
module_param(mem_process_isolation, bool, S_IRUGO);

#define MMZ_SETUP_CMDLINE_LEN     256
#define MMZ_ALLOCATOR_NAME_LEN    32

#ifndef MODULE

static char __initdata g_setup_zones[MMZ_SETUP_CMDLINE_LEN] = {'\0'};
static int __init parse_kern_cmdline(char *line)
{
    if (strncpy_s(g_setup_zones, sizeof(g_setup_zones), line, sizeof(g_setup_zones) - 1) != EOK) {
        osal_trace("%s - strncpy_s failed!\n", __FUNCTION__);
        return -1;
    }
    return 1;
}
__setup("mmz=", parse_kern_cmdline);

static char __initdata g_setup_allocator[MMZ_ALLOCATOR_NAME_LEN];
static int __init parse_kern_allocator(char *line)
{
    if (strncpy_s(g_setup_allocator, sizeof(g_setup_allocator), line, sizeof(g_setup_allocator) - 1) != EOK) {
        osal_trace("%s - strncpy_s failed!\n", __FUNCTION__);
        return -1;
    }
    return 1;
}
__setup("mmz_allocator=", parse_kern_allocator);

static char __initdata g_mmap_zones[MMZ_SETUP_CMDLINE_LEN] = {'\0'};
__setup("map_mmz=", parse_kern_cmdline);

#else
static char g_setup_zones[MMZ_SETUP_CMDLINE_LEN] = {'\0'};
static char g_mmap_zones[MMZ_SETUP_CMDLINE_LEN] = {'\0'};
static char g_setup_allocator[MMZ_ALLOCATOR_NAME_LEN] = "ot"; /* default setting */
module_param_string(mmz, g_setup_zones, MMZ_SETUP_CMDLINE_LEN, 0600);
module_param_string(map_mmz, g_mmap_zones, MMZ_SETUP_CMDLINE_LEN, 0600);
module_param_string(mmz_allocator, g_setup_allocator, MMZ_ALLOCATOR_NAME_LEN, 0600);
MODULE_PARM_DESC(mmz, "mmz_allocator=allocator mmz=name,0,start,size,type,eqsize:[others] map_mmz=start,size:[others]");
#endif

static struct mmz_allocator g_the_allocator;

int ot_mmz_get_anony(void)
{
    return anony;
}

bool media_mem_is_check_pid(void)
{
    return mem_process_isolation;
}

struct osal_list_head *ot_mmz_get_mmz_list(void)
{
    return &g_mmz_list;
}

ot_mmz_t *ot_mmz_create(const char *name,
                        unsigned long gfp,
                        unsigned long phys_start,
                        unsigned long nbytes)
{
    ot_mmz_t *p = NULL;

    mmz_trace_func();

    if (name == NULL) {
        osal_trace(KERN_ERR "%s: 'name' should not be NULL!", __FUNCTION__);
        return NULL;
    }

    p = kmalloc(sizeof(ot_mmz_t) + 1, GFP_KERNEL);
    if (p == NULL) {
        osal_trace(KERN_ERR "%s: System OOM!\n", __func__);
        return NULL;
    }

    (void)memset_s(p, sizeof(ot_mmz_t) + 1, 0, sizeof(ot_mmz_t) + 1);

    if (strncpy_s(p->name, OT_MMZ_NAME_LEN, name, OT_MMZ_NAME_LEN - 1) != EOK) {
        osal_trace("%s - strncpy_s failed!\n", __FUNCTION__);
        kfree(p);
        return NULL;
    }
    p->gfp = gfp;
    p->phys_start = phys_start;
    p->nbytes = nbytes;

    OSAL_INIT_LIST_HEAD(&p->list);
    OSAL_INIT_LIST_HEAD(&p->mmb_list);

    p->destructor = kfree;

    return p;
}
EXPORT_SYMBOL(ot_mmz_create);

ot_mmz_t *ot_mmz_create_v2(const char *name, const ot_mmz_create_attr *mmz_create_attr)
{
    ot_mmz_t *p = NULL;

    mmz_trace_func();
    if ((name == NULL) || (mmz_create_attr == NULL)) {
        osal_trace(KERN_ERR "%s: 'name' or mmz_attr can not be zero!", __FUNCTION__);
        return NULL;
    }

    p = kmalloc(sizeof(ot_mmz_t), GFP_KERNEL);
    if (p == NULL) {
        return NULL;
    }

    (void)memset_s(p, sizeof(ot_mmz_t), 0, sizeof(ot_mmz_t));
    if (strncpy_s(p->name, OT_MMZ_NAME_LEN, name, OT_MMZ_NAME_LEN - 1) != EOK) {
        osal_trace("%s - strncpy_s failed!\n", __FUNCTION__);
        kfree(p);
        return NULL;
    }
    p->gfp = mmz_create_attr->gfp;
    p->phys_start = mmz_create_attr->phys_start;
    p->nbytes = mmz_create_attr->nbytes;
    p->alloc_type = mmz_create_attr->alloc_type;
    p->block_align = mmz_create_attr->block_align;

    OSAL_INIT_LIST_HEAD(&p->list);
    OSAL_INIT_LIST_HEAD(&p->mmb_list);

    p->destructor = kfree;

    return p;
}

int ot_mmz_destroy(const ot_mmz_t *zone)
{
    if (zone == NULL) {
        return -1;
    }

    if (zone->destructor) {
        zone->destructor(zone);
    }
    return 0;
}
EXPORT_SYMBOL(ot_mmz_destroy);

static int _check_mmz(const ot_mmz_t *zone)
{
    ot_mmz_t *p = NULL;

    unsigned long new_start = zone->phys_start;
    unsigned long new_end = zone->phys_start + zone->nbytes;

    if (zone->nbytes == 0) {
        return -1;
    }

    if (!((new_start >= __pa((uintptr_t)high_memory)) ||
        ((new_start < (unsigned long)PHYS_OFFSET) && (new_end <= (unsigned long)PHYS_OFFSET)))) {
        osal_trace(KERN_ERR "ERROR: Conflict MMZ:\n");
        osal_trace(KERN_ERR OT_MMZ_FMT_S "\n", ot_mmz_fmt_arg(zone));
        osal_trace(KERN_ERR "MMZ conflict to kernel memory (0x%08lX, 0x%08lX)\n",
               (long unsigned int)PHYS_OFFSET,
               (long unsigned int)(__pa((uintptr_t)high_memory) - 1));
        return -1;
    }

    osal_list_for_each_entry(p, &g_mmz_list, list) {
        unsigned long start, end;
        start = p->phys_start;
        end   = p->phys_start + p->nbytes;
        if (new_start >= end) {
            continue;
        } else if (new_start < start && new_end <= start) {
            continue;
        } else {
        }

        osal_trace(KERN_ERR "ERROR: Conflict MMZ:\n");
        osal_trace(KERN_ERR "MMZ new:   " OT_MMZ_FMT_S "\n", ot_mmz_fmt_arg(zone));
        osal_trace(KERN_ERR "MMZ exist: " OT_MMZ_FMT_S "\n", ot_mmz_fmt_arg(p));
        osal_trace(KERN_ERR "Add new MMZ failed!\n");
        return -1;
    }

    return 0;
}

int ot_mmz_register(ot_mmz_t *zone)
{
    int ret;

    mmz_trace(1, OT_MMZ_FMT_S, ot_mmz_fmt_arg(zone));

    if (zone == NULL) {
        return -1;
    }

    down(&g_mmz_lock);

    if (strcmp(g_setup_allocator, "ot") == 0) {
        ret = _check_mmz(zone);
        if (ret) {
            up(&g_mmz_lock);
            return ret;
        }
    }

    OSAL_INIT_LIST_HEAD(&zone->mmb_list);

    osal_list_add(&zone->list, &g_mmz_list);

    up(&g_mmz_lock);

    return 0;
}

int ot_mmz_unregister(ot_mmz_t *zone)
{
    int losts = 0;
    ot_mmb_t *p = NULL;

    if (zone == NULL) {
        return -1;
    }

    mmz_trace_func();

    down(&g_mmz_lock);
    osal_list_for_each_entry(p, &zone->mmb_list, list) {
        osal_trace(KERN_WARNING "MB Lost: " OT_MMB_FMT_S "\n",
               ot_mmb_fmt_arg(p));
        losts++;
    }

    if (losts) {
        osal_trace(KERN_ERR "%d mmbs not free, mmz<%s> can not be unregistered!\n",
               losts, zone->name);
        up(&g_mmz_lock);
        return -1;
    }

    osal_list_del(&zone->list);
    up(&g_mmz_lock);

    return 0;
}

ot_mmb_t *ot_mmb_alloc(const ot_mmz_alloc_para_in *para_in)
{
    ot_mmb_t *mmb = NULL;

    if (para_in == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    if (g_the_allocator.mmb_alloc != NULL) {
        mmb = g_the_allocator.mmb_alloc(para_in);
    }
    up(&g_mmz_lock);

    return mmb;
}
EXPORT_SYMBOL(ot_mmb_alloc);

ot_mmb_t *ot_mmb_alloc_v2(const ot_mmz_alloc_para_in *para_in)
{
    ot_mmb_t *mmb = NULL;

    if (para_in == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    if (g_the_allocator.mmb_alloc_v2 != NULL) {
        mmb = g_the_allocator.mmb_alloc_v2(para_in);
    }
    up(&g_mmz_lock);

    return mmb;
}
EXPORT_SYMBOL(ot_mmb_alloc_v2);

ot_mmb_t *ot_mmb_alloc_v3(const ot_mmz_alloc_para_in *para_in, ot_mmz_alloc_para_out *para_out)
{
    ot_mmb_t *mmb = NULL;

    if (para_in == NULL || para_out == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    if (g_the_allocator.mmb_alloc_v3 != NULL) {
        mmb = g_the_allocator.mmb_alloc_v3(para_in, para_out);
    }
    up(&g_mmz_lock);

    return mmb;
}
EXPORT_SYMBOL(ot_mmb_alloc_v3);

ot_mmb_t *ot_mmb_alloc_in(const ot_mmz_alloc_para_in *para_in)
{
    ot_mmb_t *mmb = NULL;

    if (para_in == NULL || para_in->user_mmz == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    if (g_the_allocator.mmb_alloc != NULL) {
        mmb = g_the_allocator.mmb_alloc(para_in);
    }
    up(&g_mmz_lock);

    return mmb;
}

ot_mmb_t *ot_mmb_alloc_in_v2(const ot_mmz_alloc_para_in *para_in)
{
    ot_mmb_t *mmb = NULL;

    if (para_in == NULL || para_in->user_mmz == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    if (g_the_allocator.mmb_alloc_v2 != NULL) {
        mmb = g_the_allocator.mmb_alloc_v2(para_in);
    }
    up(&g_mmz_lock);

    return mmb;
}

void *ot_mmb_map2kern(ot_mmb_t *mmb)
{
    void *p = NULL;

    if (mmb == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    if (g_the_allocator.mmb_map2kern != NULL) {
        p = g_the_allocator.mmb_map2kern(mmb, 0);
    }
    up(&g_mmz_lock);

    return p;
}
EXPORT_SYMBOL(ot_mmb_map2kern);

/* mmf: media-memory fragment */
void *ot_mmf_map2kern_nocache(unsigned long phys, int len)
{
    void *virt = g_the_allocator.mmf_map(phys, len, 0);
    if (virt != NULL) {
        return virt;
    }

    return NULL;
}
EXPORT_SYMBOL(ot_mmf_map2kern_nocache);

void *ot_mmf_map2kern_cache(unsigned long phys, int len)
{
    void *virt = g_the_allocator.mmf_map(phys, len, 1);
    if (virt != NULL) {
        return virt;
    }

    return NULL;
}
EXPORT_SYMBOL(ot_mmf_map2kern_cache);

void ot_mmf_unmap(void *virt)
{
    if (g_the_allocator.mmf_unmap != NULL) {
        g_the_allocator.mmf_unmap(virt);
    }
}
EXPORT_SYMBOL(ot_mmf_unmap);

void *ot_mmb_map2kern_cached(ot_mmb_t *mmb)
{
    void *p = NULL;

    if (mmb == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    if (g_the_allocator.mmb_map2kern != NULL) {
        p = g_the_allocator.mmb_map2kern(mmb, 1);
    }
    up(&g_mmz_lock);

    return p;
}
EXPORT_SYMBOL(ot_mmb_map2kern_cached);

int ot_mmb_flush_dcache_byaddr(void *kvirt,
                               unsigned long phys_addr,
                               unsigned long length)
{
    if (kvirt == NULL) {
        return -EINVAL;
    }
    /*
     * Use flush range to instead flush_cache_all,
     * because flush_cache_all only flush local cpu.
     * And on_each_cpu macro cannot used to flush
     * all cpus with irq disabled.
     */
#ifdef CONFIG_64BIT
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
    __flush_dcache_area(kvirt, length);
#else
    dcache_clean_inval_poc((unsigned long)kvirt, (unsigned long)kvirt + length);
#endif
#else
    /*
     * dmac_map_area is invalid in kernel,
     * arm9 is not supported yet
     */
    __cpuc_flush_dcache_area(kvirt, length);
#endif

#ifdef CONFIG_CACHE_L2X0
    /* flush l2 cache, use paddr */
    /*
     * if length > L2 cache size, then this interface
     * will call <outer_flush_all>
     */
    outer_flush_range(phys_addr, phys_addr + length);
#else
    osal_unused(phys_addr);
#endif

    return 0;
}
EXPORT_SYMBOL(ot_mmb_flush_dcache_byaddr);

int ot_mmb_invalid_cache_byaddr(void *kvirt,
                                unsigned long phys_addr,
                                unsigned long length)
{
    osal_unused(phys_addr);
    if (kvirt == NULL) {
        return -EINVAL;
    }

#ifdef CONFIG_64BIT
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
    __flush_dcache_area(kvirt, length);
#else
    dcache_clean_inval_poc((unsigned long)kvirt, (unsigned long)kvirt + length);
#endif
#else
    /*
     * dmac_map_area is invalid in kernel,
     * arm9 is not supported yet
     */
    __cpuc_flush_dcache_area(kvirt, length);
#endif
    return 0;
}
EXPORT_SYMBOL(ot_mmb_invalid_cache_byaddr);

int ot_mmb_unmap(ot_mmb_t *mmb)
{
    int ref;

    if ((mmb == NULL) || (g_the_allocator.mmb_unmap == NULL)) {
        return -1;
    }

    down(&g_mmz_lock);

    ref = g_the_allocator.mmb_unmap(mmb);

    up(&g_mmz_lock);

    return ref;
}
EXPORT_SYMBOL(ot_mmb_unmap);

int ot_mmb_get(ot_mmb_t *mmb)
{
    int ref;

    if (mmb == NULL) {
        return -1;
    }
    down(&g_mmz_lock);

    if (mmb->flags & OT_MMB_RELEASED)
        osal_trace(KERN_WARNING "ot_mmb_get: amazing, mmb<%s> is released!\n", mmb->name);
    ref = ++mmb->phy_ref;

    up(&g_mmz_lock);

    return ref;
}

int ot_mmb_put(ot_mmb_t *mmb)
{
    int ref;

    if (mmb == NULL) {
        return -1;
    }

    down(&g_mmz_lock);

    if (mmb->flags & OT_MMB_RELEASED) {
        osal_trace(KERN_WARNING "ot_mmb_put: amazing, mmb<%s> is released!\n", mmb->name);
    }

    ref = --mmb->phy_ref;

    if ((mmb->flags & OT_MMB_RELEASED) && (mmb->phy_ref == 0) && (mmb->map_ref == 0)) {
        if (g_the_allocator.mmb_free != NULL) {
            g_the_allocator.mmb_free(mmb);
        }
    }

    up(&g_mmz_lock);

    return ref;
}

int ot_mmb_free(ot_mmb_t *mmb)
{
    mmz_trace_func();

    if (mmb == NULL) {
        return -1;
    }

    mmz_trace(1, OT_MMB_FMT_S, ot_mmb_fmt_arg(mmb));
    down(&g_mmz_lock);

    if (mmb->flags & OT_MMB_RELEASED) {
        osal_trace(KERN_WARNING "ot_mmb_free: amazing, mmb<%s> has been released, but is still in use!\n", mmb->name);
        up(&g_mmz_lock);
        return 0;
    }

    if (mmb->phy_ref > 0) {
        osal_trace(KERN_WARNING "ot_mmb_free: free mmb<%s> delayed for which ref-count is %d!\n",
               mmb->name, mmb->map_ref);
        mmb->flags |= OT_MMB_RELEASED;
        up(&g_mmz_lock);
        return 0;
    }

    if (mmb->flags & OT_MMB_MAP2KERN) {
        osal_trace(KERN_WARNING "free mmb<%s> delayed for which is kernel-mapped to 0x%pK with map_ref %d!\n",
               mmb->name, mmb->kvirt, mmb->map_ref);
        mmb->flags |= OT_MMB_RELEASED;
        up(&g_mmz_lock);
        return 0;
    }
    if (g_the_allocator.mmb_free != NULL) {
        g_the_allocator.mmb_free(mmb);
    }
    up(&g_mmz_lock);
    return 0;
}
EXPORT_SYMBOL(ot_mmb_free);

static ot_mmb_t *match_mmb_by_phys_addr(unsigned long addr)
{
    ot_mmz_t *zone = NULL;

    list_for_each_entry(zone, &g_mmz_list, list) {
        ot_mmb_t *mmb = NULL;
        list_for_each_entry(mmb, &zone->mmb_list, list) {
            if (mmb->phys_addr == addr) {
                return mmb;
            }
        }
    }
    return NULL;
}

ot_mmb_t *ot_mmb_getby_phys(unsigned long addr)
{
    ot_mmb_t *p = NULL;
    down(&g_mmz_lock);
    p = match_mmb_by_phys_addr(addr);
    up(&g_mmz_lock);
    return p;
}
EXPORT_SYMBOL(ot_mmb_getby_phys);

static pte_t *usr_virt_to_pte(unsigned long virt)
{
    pgd_t *pgd = NULL;
    pud_t *pud = NULL;
    pmd_t *pmd = NULL;
    p4d_t *p4d = NULL;

    pgd = pgd_offset(current->mm, virt);
    if (pgd_none(*pgd)) {
        osal_trace("osal_trace: not mapped in pgd!\n");
        return NULL;
    }

    p4d = p4d_offset(pgd, virt);
    if (p4d_none(*p4d)) {
        osal_trace("osal_trace: not mapped in p4d!\n");
        return NULL;
    }

    pud = pud_offset(p4d, virt);
    if (pud_none(*pud)) {
        osal_trace("osal_trace: not mapped in pud!\n");
        return NULL;
    }

    pmd = pmd_offset(pud, virt);
    if (pmd_none(*pmd)) {
        osal_trace("osal_trace: not mapped in pmd!\n");
        return NULL;
    }
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
    return pte_offset_map(pmd, virt);
#else
    return pte_offset_kernel(pmd, virt);
#endif
}

static unsigned int usr_pte_cacheable(const pte_t *pte)
{
#ifdef CONFIG_64BIT
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
    if (((pte_val(*pte) & PTE_ATTRINDX_MASK) == PTE_ATTRINDX(MT_NORMAL)) ||
       ((pte_val(*pte) & PTE_ATTRINDX_MASK) == PTE_ATTRINDX(MT_NORMAL_TAGGED))) {
#else
    if (pte_val(*pte) & (1 << 4)) { /* 4: cacheable flag of 64-bit linux is bit 4 */
#endif
#else
    if (pte_val(*pte) & (1 << 3)) { /* 3: cacheable flag of 32-bit linux is bit 3 */
#endif
        return 1;
    }

    return 0;
}

static int check_usr_virt_addr(unsigned long virt)
{
    if (virt & 0x3) {
        osal_trace("invalid virt addr 0x%08lx[not 4 bytes align]\n", virt);
        return -1;
    }

    if (virt >= PAGE_OFFSET) {
        osal_trace("invalid user space virt addr 0x%08lx\n", virt);
        return -1;
    }
    return 0;
}

static unsigned long usr_pte_to_phys(pte_t *pte, unsigned long virt)
{
    unsigned long page_addr = (unsigned long)((pte_val(*pte) & PHYS_MASK) & PAGE_MASK);
    unsigned long page_offset = virt & ~PAGE_MASK;
    unsigned long phys_addr = page_addr | page_offset;

    /*
     * phys_addr: the lowest bit indicates its cache attribute
     * 1: cacheable
     * 0: uncacheable
     */
    return phys_addr | usr_pte_cacheable(pte);
}

unsigned long usr_virt_to_phys(unsigned long virt)
{
    pte_t *pte = NULL;
    unsigned long phys_addr;

    if (check_usr_virt_addr(virt) != 0) {
        return 0;
    }

    pte = usr_virt_to_pte(virt);
    if (pte == NULL) {
        return 0;
    }
    if (pte_none(*pte)) {
        osal_trace("osal_trace: not mapped in pte!\n");
        pte_unmap(pte);
        return 0;
    }

    phys_addr = usr_pte_to_phys(pte, virt);
    pte_unmap(pte);
    return phys_addr;
}
EXPORT_SYMBOL(usr_virt_to_phys);

static ot_mmb_t *match_mmb_by_kvirt(const void *virt, unsigned long *out_offset)
{
    ot_mmz_t *zone = NULL;

    list_for_each_entry(zone, &g_mmz_list, list) {
        ot_mmb_t *mmb = NULL;
        list_for_each_entry(mmb, &zone->mmb_list, list) {
            const char *mmb_start = (const char *)mmb->kvirt;
            const char *virt_addr = (const char *)virt;
            if ((mmb_start <= virt_addr) && ((mmb_start + mmb->length) > virt_addr)) {
                *out_offset = (unsigned long)((const char *)virt - (const char *)mmb->kvirt);
                return mmb;
            }
        }
    }
    return NULL;
}

static ot_mmb_t *match_mmb_by_phys_range(unsigned long addr, unsigned long *out_offset)
{
    ot_mmz_t *zone = NULL;

    list_for_each_entry(zone, &g_mmz_list, list) {
        ot_mmb_t *mmb = NULL;
        list_for_each_entry(mmb, &zone->mmb_list, list) {
            if ((mmb->phys_addr <= addr) && ((mmb->length + mmb->phys_addr) > addr)) {
                *out_offset = addr - mmb->phys_addr;
                return mmb;
            }
        }
    }
    return NULL;
}

ot_mmb_t *ot_mmb_getby_kvirt(const void *virt)
{
    ot_mmb_t *p = NULL;
    unsigned long out_offset;

    if (virt == NULL) {
        return NULL;
    }
    down(&g_mmz_lock);
    p = match_mmb_by_kvirt(virt, &out_offset);
    up(&g_mmz_lock);

    mmz_trace(1, "Outoffset %lu \n", out_offset);

    return p;
}
EXPORT_SYMBOL(ot_mmb_getby_kvirt);

ot_mmb_t *ot_mmb_getby_phys_2(unsigned long addr, unsigned long *out_offset)
{
    ot_mmb_t *p = NULL;

    down(&g_mmz_lock);
    p = match_mmb_by_phys_range(addr, out_offset);
    up(&g_mmz_lock);
    return p;
}
EXPORT_SYMBOL(ot_mmb_getby_phys_2);

ot_mmb_t *media_mem_get_mmb_by_handle(const void *mem_handle)
{
    ot_mmb_t *mmb = NULL;
    ot_mmz_t *mmz = NULL;

    if (mem_handle == NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    list_for_each_entry(mmz, &g_mmz_list, list) {
        list_for_each_entry(mmb, &mmz->mmb_list, list) {
            if (mmb == mem_handle) {
                ++mmb->phy_ref;
                up(&g_mmz_lock);
                return mmb;
            }
        }
    }
    up(&g_mmz_lock);
    return NULL;
}

ot_mmb_t *media_mem_get_mmb_by_phys(unsigned long phys_addr)
{
    ot_mmb_t *mmb = NULL;
    ot_mmz_t *mmz = NULL;

    down(&g_mmz_lock);
    list_for_each_entry(mmz, &g_mmz_list, list) {
        list_for_each_entry(mmb, &mmz->mmb_list, list) {
            if ((mmb->phys_addr <= phys_addr) && (phys_addr < mmb->phys_addr + mmb->length)) {
                ++mmb->phy_ref;
                up(&g_mmz_lock);
                return mmb;
            }
        }
    }
    up(&g_mmz_lock);
    return NULL;
}

/* [phys_addr, phys_addr + size] must in mmb range */
ot_mmb_t *media_mem_get_mmb_by_phys_and_size(unsigned long phys_addr, unsigned long size)
{
    ot_mmb_t *mmb = NULL;
    ot_mmz_t *mmz = NULL;
    unsigned long addr_end = phys_addr + size;

    if (size == 0 || addr_end < phys_addr) {
        return NULL;
    }

    down(&g_mmz_lock);
    list_for_each_entry(mmz, &g_mmz_list, list) {
        list_for_each_entry(mmb, &mmz->mmb_list, list) {
            if ((mmb->phys_addr <= phys_addr) && (addr_end <= mmb->phys_addr + mmb->length)) {
                ++mmb->phy_ref;
                up(&g_mmz_lock);
                return mmb;
            }
        }
    }
    up(&g_mmz_lock);
    return NULL;
}

void media_mem_put_mmb(ot_mmb_t *mmb)
{
    (void)ot_mmb_put(mmb);
}

void media_mem_clear_pid_in_all_mmb(int pid)
{
    ot_mmz_t *mmz = NULL;
    ot_mmb_t *mmb = NULL;
    int pos;

    down(&g_mmz_lock);
    list_for_each_entry(mmz, &g_mmz_list, list) {
        list_for_each_entry(mmb, &mmz->mmb_list, list) {
            pos = find_pid_in_mmb(mmb, pid);
            /* is not the process that alloc the buffer */
            if (pos > 0) {
                clear_pid_in_mmb(mmb, pos);
            }
        }
    }
    up(&g_mmz_lock);
}

ot_mmz_t *ot_mmz_find(unsigned long gfp, const char *mmz_name)
{
    ot_mmz_t *p = NULL;

    down(&g_mmz_lock);
    begin_list_for_each_mmz(p, gfp, mmz_name)
    up(&g_mmz_lock);
    return p;
    end_list_for_each_mmz()
    up(&g_mmz_lock);

    return NULL;
}
EXPORT_SYMBOL(ot_mmz_find);

unsigned long ot_mmz_get_phys(const char *zone_name)
{
    ot_mmz_t *zone = NULL;

    zone = ot_mmz_find(0, zone_name);
    if (zone != NULL) {
        return zone->phys_start;
    }

    return 0;
}
EXPORT_SYMBOL(ot_mmz_get_phys);

static int _check_map_mmz(const ot_mmz_t *zone)
{
    ot_mmz_t *p = NULL;

    unsigned long new_start = zone->phys_start;
    unsigned long new_end = zone->phys_start + zone->nbytes;

    if (zone->nbytes == 0) {
        return -1;
    }

    osal_list_for_each_entry(p, &g_map_mmz_list, list) {
        unsigned long start, end;
        start = p->phys_start;
        end   = p->phys_start + p->nbytes;

        if (new_start >= end) {
            continue;
        } else if ((new_start < start) && (new_end <= start)) {
            continue;
        } else {
        }

        osal_trace(KERN_ERR "ERROR: Conflict MMZ:\n");
        osal_trace(KERN_ERR "MMZ new:   " OT_MMZ_FMT_S "\n", ot_mmz_fmt_arg(zone));
        osal_trace(KERN_ERR "MMZ exist: " OT_MMZ_FMT_S "\n", ot_mmz_fmt_arg(p));
        osal_trace(KERN_ERR "Add new MMZ failed!\n");
        return -1;
    }

    return 0;
}

int ot_map_mmz_register(ot_mmz_t *zone)
{
    int ret;

    mmz_trace(1, OT_MMZ_FMT_S, ot_mmz_fmt_arg(zone));

    if (zone == NULL) {
        return -1;
    }

    down(&g_mmz_lock);

    if (strcmp(g_setup_allocator, "ot") == 0) {
        ret = _check_mmz(zone);
        if (ret != 0) {
            up(&g_mmz_lock);
            return ret;
        }
    }

    ret = _check_map_mmz(zone);
    if (ret) {
        up(&g_mmz_lock);
        return ret;
    }

    OSAL_INIT_LIST_HEAD(&zone->mmb_list);

    osal_list_add(&zone->list, &g_map_mmz_list);

    up(&g_mmz_lock);

    return 0;
}

int ot_map_mmz_unregister(ot_mmz_t *zone)
{
    int losts = 0;
    ot_mmb_t *p = NULL;

    if (zone == NULL) {
        return -1;
    }

    mmz_trace_func();

    down(&g_mmz_lock);
    osal_list_for_each_entry(p, &zone->mmb_list, list) {
        osal_trace(KERN_WARNING "MB Lost: " OT_MMB_FMT_S "\n",
               ot_mmb_fmt_arg(p));
        losts++;
    }

    if (losts) {
        osal_trace(KERN_ERR "%d mmbs not free, mmz<%s> can not be unregistered!\n",
               losts, zone->name);
        up(&g_mmz_lock);
        return -1;
    }

    osal_list_del(&zone->list);
    up(&g_mmz_lock);

    return 0;
}

static int map_mmz_init(char *s)
{
    ot_mmz_t *zone = NULL;
    char *line = NULL;

    if (s[0] == '\0') {
        return 0;
    }

    while ((line = strsep(&s, ":")) != NULL) {
        int i;
        char *argv[2]; /* 2: map mmz has two arguments */

        for (i = 0; (argv[i] = strsep(&line, ",")) != NULL;) {
            if (++i == ARRAY_SIZE(argv)) {
                break;
            }
        }

        if (i == 2) { /* 2: had parse two args */
            zone = ot_mmz_create("null", 0, 0, 0);
            if (zone == NULL) {
                continue;
            }
            zone->phys_start = _strtoul_ex(argv[0], NULL, 0);
            zone->nbytes = _strtoul_ex(argv[1], NULL, 0);
        } else {
            osal_trace(KERN_ERR "error parameters\n");
            return -EINVAL;
        }

        if (ot_map_mmz_register(zone)) {
            osal_trace(KERN_WARNING "Add MMZ failed: " OT_MMZ_FMT_S "\n", ot_mmz_fmt_arg(zone));
            ot_mmz_destroy(zone);
        }
        zone = NULL;
    }

    return 0;
}

static void map_mmz_exit(void)
{
    ot_mmz_t *pmmz = NULL;
    struct osal_list_head *p = NULL;
    struct osal_list_head *n = NULL;

    mmz_trace_func();

    osal_list_for_each_safe(p, n, &g_map_mmz_list) {
        pmmz = list_entry(p, ot_mmz_t, list);
        osal_trace(KERN_WARNING "MMZ force removed: " OT_MMZ_FMT_S "\n",
               ot_mmz_fmt_arg(pmmz));
        ot_map_mmz_unregister(pmmz);
        ot_mmz_destroy(pmmz);
    }
}

int ot_map_mmz_check_phys(unsigned long addr_start, unsigned long addr_len)
{
    ot_mmz_t *p = NULL;
    unsigned long addr_end = addr_start + addr_len;
    unsigned long temp_start, temp_end;

    if ((addr_len > 0) && (addr_end > addr_start)) {
        osal_list_for_each_entry(p, &g_map_mmz_list, list) {
            temp_start = p->phys_start;
            temp_end   = p->phys_start + p->nbytes;
            if ((addr_start >= temp_start) && (addr_end <= temp_end)) {
                return 0;
            }
        }
    }

    error_mmz("ERROR: MMAP ADDR: 0x%lx-0x%lx\n", addr_start, addr_end);

    return -1;
}
EXPORT_SYMBOL(ot_map_mmz_check_phys);

int ot_vma_check(unsigned long vm_start, unsigned long vm_end)
{
    struct vm_area_struct *pvma1 = NULL;
    struct vm_area_struct *pvma2 = NULL;

    pvma1 = find_vma(current->mm, vm_start);
    if (pvma1 == NULL) {
        osal_trace(KERN_ERR "ERROR: pvma1 is null\n");
        return -1;
    }

    pvma2 = find_vma(current->mm, vm_end - 1);
    if (pvma2 == NULL) {
        osal_trace(KERN_ERR "ERROR: pvma2 is null\n");
        return -1;
    }

    if (pvma1 != pvma2) {
        osal_trace(KERN_ERR "ERROR: pvma1:[0x%lx,0x%lx) and pvma2:[0x%lx,0x%lx) are not equal\n",
            pvma1->vm_start, pvma1->vm_end, pvma2->vm_start, pvma2->vm_end);
        return -1;
    }

    if (!(pvma1->vm_flags & VM_WRITE)) {
        osal_trace(KERN_ERR "ERROR vma flag:0x%lx\n", pvma1->vm_flags);
        return -1;
    }

    if (pvma1->vm_start > vm_start) {
        osal_trace("cannot find corresponding vma, vm[%lx, %lx], user range[%lx,%lx]\n",
            pvma1->vm_start, pvma1->vm_end, vm_start, vm_end);
        return -1;
    }

    return 0;
}
EXPORT_SYMBOL(ot_vma_check);

int ot_is_phys_in_mmz(unsigned long addr_start, unsigned long addr_len)
{
    ot_mmz_t *p = NULL;
    unsigned long addr_end = addr_start + addr_len;
    unsigned long temp_start, temp_end;

    if ((addr_len > 0) && (addr_end > addr_start)) {
        osal_list_for_each_entry(p, &g_mmz_list, list) {
            temp_start = p->phys_start;
            temp_end   = p->phys_start + p->nbytes;
            if ((addr_start >= temp_start) && (addr_end <= temp_end)) {
                return 0;
            }
        }
    }

    return -1;
}
EXPORT_SYMBOL(ot_is_phys_in_mmz);

int ot_mmb_check_mem_share(const ot_mmb_t *mmb)
{
    return ot_mmb_check_mem_share_with_pid(mmb, osal_get_current_tgid());
}
EXPORT_SYMBOL(ot_mmb_check_mem_share);

int ot_mmb_check_mem_share_with_pid(const ot_mmb_t *mmb, int pid)
{
    if (mmb == NULL) {
        return -EINVAL;
    }
    if (mmb->kernel_only == 1) {
        error_mmz("mmb used only in kernel!\n");
        return -EPERM;
    }
    if (mmb->check_pid == 0) {
        /* default share */
        return 0;
    }
    if (find_pid_in_mmb(mmb, pid) < 0) {
        error_mmz("mmb is not shared with pid(%d)!\n", pid);
        return -EPERM;
    }
    return 0;
}
EXPORT_SYMBOL(ot_mmb_check_mem_share_with_pid);

int ot_mmz_check_phys_addr(unsigned long phys_addr, unsigned long size)
{
    ot_mmb_t *mmb = NULL;
    int ret;

    /* if allocated by mmz of current system */
    mmb = media_mem_get_mmb_by_phys_and_size(phys_addr, size);
    if (mmb == NULL) {
        /* if in other system */
        if (ot_map_mmz_check_phys(phys_addr, size) < 0) {
            return -EINVAL;
        }
        return 0;
    }
    /* mem share check */
    ret = ot_mmb_check_mem_share(mmb);
    media_mem_put_mmb(mmb);
    return ret;
}
EXPORT_SYMBOL(ot_mmz_check_phys_addr);

int ot_mmz_get_mem_process_isolation(void)
{
    return mem_process_isolation;
}
EXPORT_SYMBOL(ot_mmz_get_mem_process_isolation);

int ot_mmb_flush_dcache_byaddr_safe(void *kvirt,
                                     unsigned long phys_addr,
                                     unsigned long length)
{
    int ret;
    struct mm_struct *mm = current->mm;

    if (kvirt == NULL) {
        return -EINVAL;
    }

    mmz_map_down(mm);

    if (ot_vma_check((unsigned long)(uintptr_t)kvirt, (unsigned long)(uintptr_t)kvirt + length)) {
        mmz_map_up(mm);
        return -EPERM;
    }

    ret = ot_mmb_flush_dcache_byaddr(kvirt, phys_addr, length);

    mmz_map_up(mm);

    return ret;
}
EXPORT_SYMBOL(ot_mmb_flush_dcache_byaddr_safe);

#define MEDIA_MEM_NAME  "media-mem"

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT

int mmz_seq_show(struct osal_proc_dir_entry *sfile)
{
    ot_mmz_t *p = NULL;
    unsigned int zone_number = 0;
    unsigned int block_number = 0;
    unsigned int used_size = 0;
    unsigned int free_size;
    unsigned int mmz_total_size = 0;

    osal_seq_printf(sfile, " anony=%d mmz_allocator=%s mem_process_isolation=%d\n\n",
        anony, g_setup_allocator, mem_process_isolation);
    mmz_trace_func();

    down(&g_mmz_lock);
    list_for_each_entry(p, &g_mmz_list, list) {
        ot_mmb_t *mmb = NULL;
        ot_mmb_t *temp_mmb = NULL;
        osal_seq_printf(sfile, "+---ZONE: " OT_MMZ_FMT_S "\n", ot_mmz_fmt_arg(p));
        mmz_total_size += p->nbytes / 1024; /* 1024: 1KByte = 1024Byte */
        ++zone_number;

        list_for_each_entry(mmb, &p->mmb_list, list) {
            if (temp_mmb != NULL && ((mmb)->phys_addr > mmz_grain_align((temp_mmb)->phys_addr + (temp_mmb)->length))) {
                osal_seq_printf(sfile, "   *-MMB: " OT_MMB_FMT_S "\n", ot_mmb_fmt_arg(mmb));
            } else {
                osal_seq_printf(sfile, "   |-MMB: " OT_MMB_FMT_S "\n", ot_mmb_fmt_arg(mmb));
            }
            temp_mmb = mmb;
            used_size += mmb->length / 1024; /* 1024: 1KByte = 1024Byte */
            ++block_number;
        }
    }

    if (mmz_total_size != 0) {
        free_size = mmz_total_size - used_size;
        osal_seq_printf(sfile, "\n---MMZ_USE_INFO:\n total size=%dKB(%dMB),"
            "used=%dKB(%dMB + %dKB),remain=%dKB(%dMB + %dKB),"
            "zone_number=%d,block_number=%d\n",
            mmz_total_size, mmz_total_size / 1024, /* 1024: 1MByte = 1024KByte */
            used_size, used_size / 1024, used_size % 1024, /* 1024: 1MByte = 1024KByte */
            free_size, free_size / 1024, free_size % 1024, /* 1024: 1MByte = 1024KByte */
            zone_number, block_number);
        mmz_total_size = 0;
        zone_number = 0;
        block_number = 0;
    }
    up(&g_mmz_lock);

    return 0;
}

static int __init media_mem_proc_init(void)
{
    osal_proc_entry_t *proc = NULL;

    proc = osal_create_proc_entry(MEDIA_MEM_NAME, NULL);
    if (proc == NULL) {
        osal_trace(KERN_ERR "Create mmz proc fail!\n");
        return -1;
    }
    proc->read = mmz_seq_show;

    return 0;
}

static void __exit media_mem_proc_exit(void)
{
    osal_remove_proc_entry(MEDIA_MEM_NAME, NULL);
}

#endif /* CONFIG_OT_PROC_SHOW_SUPPORT */

/* this function is used by osal_init.c */
int mem_check_module_param(void)
{
    if (anony != 1) {
        osal_trace("The module param \"anony\" should only be 1 which is %d\n", anony);
        return -1;
    }
    return 0;
}

static void mmz_exit_check(void)
{
    ot_mmz_t *pmmz = NULL;
    struct osal_list_head *p = NULL;
    struct osal_list_head *n = NULL;

    mmz_trace_func();

    osal_list_for_each_safe(p, n, &g_mmz_list) {
        pmmz = list_entry(p, ot_mmz_t, list);
        osal_trace(KERN_WARNING "MMZ force removed: " OT_MMZ_FMT_S "\n",
               ot_mmz_fmt_arg(pmmz));
        ot_mmz_unregister(pmmz);
        ot_mmz_destroy(pmmz);
    }
}

int __init media_mem_init(void)
{
    int ret = 0;

    osal_trace(KERN_INFO "Media Memory Zone Manager\n");

    if (anony != 1) {
        osal_trace("The module param \"anony\" should only be 1 which is %d\n", anony);
        return -EINVAL;
    }

    if (strcmp(g_setup_allocator, "cma") == 0) {
#ifdef CONFIG_CMA
        (void)cma_allocator_setopt(&g_the_allocator);
#else
        pr_err("cma is not enabled in kernel, please check!\n");
        return -EINVAL;
#endif
    } else if (strcmp(g_setup_allocator, "ot") == 0) {
        (void)ot_allocator_setopt(&g_the_allocator);
    } else {
        osal_trace("The module param \"g_setup_allocator\" should be \"cma\" or \"ot\", which is \"%s\"\n",
                g_setup_allocator);
        mmz_exit_check();
        return -EINVAL;
    }

    ret = g_the_allocator.init(g_setup_zones);
    if (ret != 0) {
        mmz_exit_check();
        return ret;
    }

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    media_mem_proc_init();
#endif

    mmz_userdev_init();

    map_mmz_init(g_mmap_zones);

    return 0;
}

#ifdef MODULE
void __exit media_mem_exit(void)
{
    map_mmz_exit();

    mmz_userdev_exit();

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    media_mem_proc_exit();
#endif

    mmz_exit_check();
}
#else
subsys_initcall(media_mem_init);
#endif
