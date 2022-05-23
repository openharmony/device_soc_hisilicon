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

#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/mm.h>
#include <linux/memblock.h>

#include "hi_osal.h"
#include "hi_module.h"
#include "securec.h"

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
#ifndef CONFIG_64BIT
#include <mach/io.h>
#endif
#endif

unsigned int g_kmalloc_used[HI_ID_BUTT] = {0};
unsigned int g_vmalloc_used[HI_ID_BUTT] = {0};

unsigned int osal_get_kmalloc_used(unsigned int module_id)
{
    if (module_id >= HI_ID_BUTT) {
        return 0;
    }

    return g_kmalloc_used[module_id];
}

unsigned int osal_get_vmalloc_used(unsigned int module_id)
{
    if (module_id >= HI_ID_BUTT) {
        return 0;
    }

    return g_vmalloc_used[module_id];
}

static unsigned int osal_convert_gfp_flag(unsigned int osal_gfp_flag)
{
    unsigned int gfp_flag;

    switch (osal_gfp_flag) {
        case OSAL_GFP_KERNEL:
            gfp_flag = GFP_KERNEL;
            break;

        case OSAL_GFP_ATOMIC:
            gfp_flag = GFP_ATOMIC;
            break;

        case OSAL_GFP_DMA:
            gfp_flag = GFP_DMA;
            break;

        default:
            gfp_flag = GFP_KERNEL;
            break;
    }

    return gfp_flag;
}

void *osal_kmalloc(unsigned int module_id, unsigned long size, unsigned int osal_gfp_flag)
{
    unsigned int gfp_flag;

    if (module_id >= HI_ID_BUTT) {
        printk("osal_kmalloc module id(%ud) is wrong\n", module_id);
        osal_dump_stack();
        return NULL;
    }

    g_kmalloc_used[module_id]++;
    gfp_flag = osal_convert_gfp_flag(osal_gfp_flag);
    return kmalloc(size, gfp_flag);
}
EXPORT_SYMBOL(osal_kmalloc);

void osal_kfree(unsigned int module_id, const void *addr)
{
    if (module_id >= HI_ID_BUTT) {
        printk("osal_kfree module id(%ud) is wrong\n", module_id);
        osal_dump_stack();
        return;
    }

    if (addr != NULL) {
        g_kmalloc_used[module_id]--;
        kfree(addr);
    }
    return;
}
EXPORT_SYMBOL(osal_kfree);

void *osal_vmalloc(unsigned int module_id, unsigned long size)
{
    if (module_id >= HI_ID_BUTT) {
        printk("osal_vmalloc module id(%ud) is wrong\n", module_id);
        osal_dump_stack();
        return NULL;
    }

    g_vmalloc_used[module_id]++;

    return vmalloc(size);
}

EXPORT_SYMBOL(osal_vmalloc);

void osal_vfree(unsigned int module_id, const void *addr)
{
    if (module_id >= HI_ID_BUTT) {
        printk("osal_vfree module id(%ud) is wrong\n", module_id);
        osal_dump_stack();
        return;
    }

    if (addr != NULL) {
        g_vmalloc_used[module_id]--;
        vfree(addr);
    }
    return;
}

EXPORT_SYMBOL(osal_vfree);

void *osal_ioremap(unsigned long phys_addr, unsigned long size)
{
    return ioremap(phys_addr, size);
}
EXPORT_SYMBOL(osal_ioremap);

void *osal_ioremap_nocache(unsigned long phys_addr, unsigned long size)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
    return ioremap(phys_addr, size);
#else
    return ioremap_nocache(phys_addr, size);
#endif
}
EXPORT_SYMBOL(osal_ioremap_nocache);

void *osal_ioremap_cached(unsigned long phys_addr, unsigned long size)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
    return ioremap_cached(phys_addr, size);
#else
    return ioremap_cache(phys_addr, size);
#endif
}
EXPORT_SYMBOL(osal_ioremap_cached);

void osal_iounmap(volatile void *addr)
{
    iounmap(addr);
}
EXPORT_SYMBOL(osal_iounmap);

unsigned long osal_copy_from_user(void *to, const void *from, unsigned long n)
{
    return copy_from_user(to, from, n);
}
EXPORT_SYMBOL(osal_copy_from_user);

unsigned long osal_copy_to_user(void *to, const void *from, unsigned long n)
{
    return copy_to_user(to, from, n);
}
EXPORT_SYMBOL(osal_copy_to_user);

int osal_access_ok(int type, const void *addr, unsigned long size)
{
    uintptr_t uaddr = (uintptr_t)addr;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
    return access_ok(uaddr, size);
#else
    return access_ok(type, uaddr, size);
#endif
}
EXPORT_SYMBOL(osal_access_ok);

void *osal_phys_to_virt(unsigned long addr)
{
    return phys_to_virt(addr);
}
EXPORT_SYMBOL(osal_phys_to_virt);

unsigned long osal_virt_to_phys(const void *virt_addr)
{
    return virt_to_phys(virt_addr);
}
EXPORT_SYMBOL(osal_virt_to_phys);

/*
 * Maps @size from @phys_addr into contiguous kernel virtual space
 * Note:this function only support VM_MAP with PAGE_KERNEL flag
 * */
void *osal_blockmem_vmap(unsigned long phys_addr, unsigned long size)
{
    int ret = 0;
    unsigned int i = 0;
    unsigned int page_count;
    struct page **pages = NULL;
    void *vaddr = NULL;

    if ((phys_addr == 0) || (size == 0)) {
        printk("invalid vmap address: 0x%lX or size:%lu\n", phys_addr, size);
        return NULL;
    }

    page_count = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    pages = vmalloc(page_count * sizeof(struct page *));
    if (!pages) {
        printk("vmap malloc pages failed\n");
        return NULL;
    }

    ret = memset_s(pages, page_count * sizeof(struct page *), 0, page_count * sizeof(struct page *));
    if (ret != 0) {
        return NULL;
    }

    for (i = 0; i < page_count; i++) {
        pages[i] = phys_to_page(phys_addr + i * PAGE_SIZE);
    }

    vaddr = vmap(pages, page_count, VM_MAP, PAGE_KERNEL);
    if (!vaddr) {
        printk("vmap failed phys_addr:0x%lX, size:%lu\n", phys_addr, size);
    }

    vfree(pages);
    pages = NULL;

    return vaddr;
}
EXPORT_SYMBOL(osal_blockmem_vmap);

/*
 * Free the virtually contiguous memory area starting at @virt_addr
 * which was created from the phys_addr passed to osal_vunmap()
 * Must not be called in interrupt context.
 * */
void osal_blockmem_vunmap(const void *virt_addr)
{
    if (!virt_addr) {
        printk("vumap failed: virt_addr is NULL\n");
        return;
    }

    vunmap(virt_addr);
}
EXPORT_SYMBOL(osal_blockmem_vunmap);

/*
 * Free the reserved memory which has been defined in product
 **/
void osal_blockmem_free(unsigned long phys_addr, unsigned long size)
{
    unsigned int pfn_start;
    unsigned int pfn_end;

    if ((phys_addr == 0) || (size == 0)) {
        printk("Free block memory failed: phys_addr 0x%lX,size %lu\n",
            phys_addr, size);
        return;
    }

    pfn_start = __phys_to_pfn(phys_addr);
    pfn_end = __phys_to_pfn(phys_addr + size);

    for (; pfn_start < pfn_end; pfn_start++) {
        struct page *page = pfn_to_page(pfn_start);
        ClearPageReserved(page);
        init_page_count(page);
        __free_page(page);
        adjust_managed_page_count(page, 1); /* 1 block mem page count */
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
#ifndef CONFIG_ARCH_DISCARD_MEMBLOCK
    (void)memblock_free(phys_addr, size);
#endif
#endif
    return;
}
EXPORT_SYMBOL(osal_blockmem_free);
