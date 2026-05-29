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
#include <linux/module.h>
#include "ot_osal.h"
#include "osal_mmz.h"

#include "ot_debug.h"
#include "ot_common.h"
#include "mm_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

OSAL_PHYS_ADDR_TYPE cmpi_mmz_malloc(const char *mmz_name, const char *buf_name, unsigned int size, td_bool kernel_only)
{
    ot_mmb_t *mmb = NULL;
    ot_mmz_alloc_para_in para_in = {0};

    para_in.mmz_name = mmz_name;
    para_in.buf_name = buf_name;
    para_in.size = size;
    para_in.kernel_only = kernel_only;

    mmb = ot_mmb_alloc(&para_in);
    if (mmb == NULL) {
        osal_trace(KERN_WARNING "mmz malloc failed!\n");
        return 0;
    }

    return ot_mmb_phys(mmb);
}
EXPORT_SYMBOL(cmpi_mmz_malloc);

OSAL_PHYS_ADDR_TYPE cmpi_mmz_malloc_fix_addr(const char *buf_name, OSAL_PHYS_ADDR_TYPE start, unsigned int size,
    td_bool kernel_only)
{
    ot_mmb_t *mmb = NULL;
    ot_mmz_alloc_para_in  para_in = {0};
    ot_mmz_alloc_para_out para_out = {0};

    para_in.mmz_name = NULL;
    para_in.buf_name = buf_name;
    para_in.start = start;
    para_in.size = size;
    para_in.kernel_only = kernel_only;

    mmb = ot_mmb_alloc_v3(&para_in, &para_out);
    if (mmb == NULL) {
        osal_trace(KERN_WARNING "mmz malloc fix addr failed!\n");
        return TD_NULL;
    }

    return ot_mmb_phys(mmb);
}
EXPORT_SYMBOL(cmpi_mmz_malloc_fix_addr);

void cmpi_mmz_free(OSAL_PHYS_ADDR_TYPE phys_addr, void *virt_addr)
{
    if (virt_addr != NULL) {
        ot_mmb_t *mmb = ot_mmb_getby_kvirt(virt_addr);

        if (mmb != NULL) {
            ot_mmb_unmap(mmb);
        }
    }

    if (phys_addr != 0) {
        ot_mmb_freeby_phys((unsigned int)phys_addr);
    }
}
EXPORT_SYMBOL(cmpi_mmz_free);

void *cmpi_remap_cached(OSAL_PHYS_ADDR_TYPE phys_addr, unsigned int size)
{
    unsigned int diff;
    OSAL_PHYS_ADDR_TYPE page_phy;
    unsigned char *page_addr;
    unsigned int page_size;

    /* the mmap address should align with page */
    page_phy = (OSAL_PHYS_ADDR_TYPE)(phys_addr & 0xfffffffffffff000ULL);
    diff = (unsigned int)(phys_addr - page_phy);

    /* the mmap size should be multiples of 1024 */
    page_size = ((size + diff - 1) & 0xfffff000UL) + 0x1000;

    page_addr = ot_mmf_map2kern_cache((unsigned long)page_phy, page_size);
    if (page_addr == NULL) {
        osal_printk("phy(0x%lx) size(%u) map cache err!\n", (unsigned long)page_phy, page_size);
        return NULL;
    }

    return (page_addr + diff);
}
EXPORT_SYMBOL(cmpi_remap_cached);

void *cmpi_remap_nocache(OSAL_PHYS_ADDR_TYPE phys_addr, unsigned int size)
{
    unsigned int diff;
    OSAL_PHYS_ADDR_TYPE page_phy;
    unsigned char *page_addr;
    unsigned int page_size;

    /* the mmap address should align with page */
    page_phy = (OSAL_PHYS_ADDR_TYPE)(phys_addr & 0xfffffffffffff000ULL);
    diff = (unsigned int)(phys_addr - page_phy);

    /* the mmap size should be multiples of 1024 */
    page_size = ((size + diff - 1) & 0xfffff000UL) + 0x1000;

    page_addr = ot_mmf_map2kern_nocache((unsigned long)page_phy, page_size);
    if (page_addr == NULL) {
        osal_printk("phy(0x%lx) size(%u) map nocache err!\n", (unsigned long)page_phy, page_size);
        return NULL;
    }

    return (page_addr + diff);
}
EXPORT_SYMBOL(cmpi_remap_nocache);

void cmpi_unmap(void *virt_addr)
{
    ot_mmf_unmap(virt_addr);
}
EXPORT_SYMBOL(cmpi_unmap);

static int cmpi_mmz_malloc_map(const cmpi_mmz_malloc_param *param, td_bool cached)
{
    ot_mmb_t *mmb = NULL;
    ot_mmz_alloc_para_in para_in = {0};

    if (param == NULL || param->phys_addr == NULL || param->virt_addr == NULL) {
        return TD_FAILURE;
    }

    para_in.mmz_name = param->mmz_name;
    para_in.buf_name = param->buf_name;
    para_in.size = param->len;
    para_in.kernel_only = param->kernel_only;

    mmb = ot_mmb_alloc(&para_in);
    if (mmb == NULL) {
        osal_trace(KERN_WARNING "mmz malloc failed!\n");
        return TD_FAILURE;
    }

    /* the buffer allocated by mmz is 4k align */
    if (cached == TD_TRUE) {
        *param->virt_addr = ot_mmb_map2kern_cached(mmb);
    } else {
        *param->virt_addr = ot_mmb_map2kern(mmb);
    }
    if (*param->virt_addr == NULL) {
        ot_mmb_free(mmb);
        osal_trace(KERN_WARNING "mmz remap %s failed!\n", cached == TD_TRUE ? "cache" : "no cache");
        return TD_FAILURE;
    }
    *param->phys_addr = ot_mmb_phys(mmb);

    return TD_SUCCESS;
}

int cmpi_mmz_malloc_nocache(const cmpi_mmz_malloc_param *param)
{
    return cmpi_mmz_malloc_map(param, TD_FALSE);
}
EXPORT_SYMBOL(cmpi_mmz_malloc_nocache);

int cmpi_mmz_malloc_cached(const cmpi_mmz_malloc_param *param)
{
    return cmpi_mmz_malloc_map(param, TD_TRUE);
}
EXPORT_SYMBOL(cmpi_mmz_malloc_cached);

int cmpi_check_mmz_phy_addr(OSAL_PHYS_ADDR_TYPE phy_addr, unsigned int len)
{
    /* if address in mmz of current system */
    if (ot_is_phys_in_mmz((unsigned int)phy_addr, len)) {
        /* if in other system */
        if (ot_map_mmz_check_phys((unsigned int)phy_addr, len)) {
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}
EXPORT_SYMBOL(cmpi_check_mmz_phy_addr);

int cmpi_invalid_cache_byaddr(void *virt_addr, OSAL_PHYS_ADDR_TYPE phys_addr, unsigned int len)
{
    return ot_mmb_invalid_cache_byaddr(virt_addr, phys_addr, len);
}

td_void *cmpi_mmz_phys_to_handle(td_phys_addr_t phys_addr)
{
    ot_mmb_t *mmb = NULL;
    td_ulong offset = 0;
    mmb = ot_mmb_getby_phys_2(phys_addr, &offset);
    if (mmb == NULL) {
        osal_trace(KERN_WARNING "get mmb failed!\n");
    }
    return (td_void *)mmb;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

