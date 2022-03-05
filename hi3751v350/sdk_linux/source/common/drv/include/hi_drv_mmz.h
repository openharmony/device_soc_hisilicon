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

#ifndef __HI_DRV_MMZ_H__
#define __HI_DRV_MMZ_H__

#include <linux/kernel.h>
#include <linux/module.h>
#ifdef __KERNEL__
#include <linux/scatterlist.h>
#include <linux/delay.h>
#include <linux/seq_file.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#endif

#ifdef __KERNEL__
#include <linux/kthread.h>
#endif

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* **************************** Macro Definition ***************************** */
#define MMZ_OTHERS NULL
#define MMZ_SMMU   "iommu"
/* ************************** Structure Definition *************************** */
typedef struct {
    HI_U8 *pu8_start_vir_addr;
    HI_U32 start_phy_addr;
    HI_U32 size;
} mmz_buffer;

typedef struct {
    HI_U8 *pu8_start_vir_addr;
    HI_U32 start_smmu_addr;
    HI_U32 size;
} smmu_buffer;


// remove  hi_drv_mmz_query_buffer_ref function in this file
// remove  hi_drv_mmz_buffer_put function in this file
// remove  hi_drv_mmz_buffer_get function in this file
// remove  hi_drv_mmz_get_phy_by_virt function in this file
/* ********************* Global Variable declaration ************************* */
/* ****************************** API declaration **************************** */
/* alloc mmz memory, get physic address and map kernel-state address */
/* CNcomment:Apply for mmz memory, get the physical address, and map the kernel-mode address */
HI_S32 hi_drv_mmz_alloc_and_map(const char *name, char *mmzzonename, HI_U32 size, int align, mmz_buffer *mmz_buf);

/* unmap kernel-state address, release mmz memory */
/* CNcomment:Unmap the kernel mode address and release mmz memory */
HI_VOID hi_drv_mmz_unmap_and_release(mmz_buffer *mmz_buf);

/* Only alloc mmz memory, return physic address, but not map kernel-state address */
/* CNcomment:Only apply for mmz memory, return the physical address, and do not map the kernel-mode address */
HI_S32 hi_drv_mmz_alloc(const char *bufname, char *zone_name, HI_U32 size, int align, mmz_buffer *mmz_buf);

/* map kernel-state address after alloc mmz memory for cache, and flushing cache with hi_drv_mmz_flush */
/* CNcomment:After applying for mmz cacheable memory, perform kernel-mode address mapping, and use hi_drv_mmz_flush
for cache synchronization */
HI_S32 hi_drv_mmz_map_cache(mmz_buffer *mmz_buf);

/* flush cache data to memory, needed to call when map memory with hi_drv_mmz_map_cache */
/* CNcomment:When using hi_drv_mmz_map_cache, you need to actively call hi_drv_mmz_flush for cache data
synchronization */
HI_S32 hi_drv_mmz_flush(mmz_buffer *mmz_buf);
HI_S32 hi_drv_mmz_invalidate(mmz_buffer *mmz_buf);

/* alloc mmz memory, and map kernel-state address */
/* CNcomment:After applying for mmz memory, map the kernel state address */
HI_S32 hi_drv_mmz_map(mmz_buffer *mmz_buf);

/* unmap kernel-state address */
/* CNcomment:Unmap kernel-mode addresses */
HI_VOID hi_drv_mmz_unmap(mmz_buffer *mmz_buf);

/* release unmapped mmz memory */
/* CNcomment:After unmapped, or the mmz memory that has not been mapped in kernel mode is released */
HI_VOID hi_drv_mmz_release(mmz_buffer *mmz_buf);

// SMMU API
HI_S32 hi_drv_smmu_alloc_and_map(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, smmu_buffer *smmu_buf);

HI_VOID hi_drv_smmu_unmap_and_release(smmu_buffer *smmu_buf);

HI_S32 hi_drv_smmu_alloc(const HI_CHAR *bufname, HI_U32 size, int align, smmu_buffer *smmu_buf);

HI_S32 hi_drv_smmu_map_cache(smmu_buffer *smmu_buf);

HI_S32 hi_drv_smmu_flush(smmu_buffer *smmu_buf);
HI_S32 hi_drv_smmu_invalidate(smmu_buffer *smmu_buf);

HI_S32 hi_drv_smmu_map(smmu_buffer *smmu_buf);

HI_VOID hi_drv_smmu_unmap(smmu_buffer *smmu_buf);

HI_VOID hi_drv_smmu_release(smmu_buffer *smmu_buf);

/* map cma mem to smmu  */
HI_U32 hi_drv_mmz_map_to_smmu(mmz_buffer *mmz_buf);

/* unmap cma mem to smmu   */
HI_S32 hi_drv_mmz_unmap_from_smmu(smmu_buffer *smmu_buf);

/* get smmu addr addr by kvirt   */
HI_S32 hi_drv_smmu_get_smmu_addr_by_virt(smmu_buffer *smmu_buf);

/* get the page table base address and the write or read address when smmu incurs an error */
/*
pt_addr: the page table base address
         moudules need to configure it to smmu common registers SMMU_SCB_TTBR and SMMU_CB_TTBR

err_rd_addr: If an error occurs, the smmu hardware will provide this address and
             modules can read data which is always 0 from this address
             moudules need to set it to the smmu common register SMMU_ERR_RDADDR

err_wr_addr: If an error occurs, the smmu hardware provides this address and
             modules can write data to this address so as to avoid writing other module's address
             moudules need to set it to their smmu common register SMMU_ERR_WRADDR.
*/
HI_VOID hi_drv_smmu_get_page_table_addr(HI_U32 *pt_addr, HI_U32 *err_rd_addr, HI_U32 *err_wr_addr);

/* increment refcount for nosec smmu buffer */
HI_S32 hi_drv_smmu_buffer_get(smmu_buffer *smmu_buf);

/* decrement refcount for nosec smmu buffer  */
HI_S32 hi_drv_smmu_buffer_put(smmu_buffer *p_smmu_buf);

/* alloc sec cma mem */
HI_S32 hi_drv_secmmz_alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, mmz_buffer *secmmz_buf);

/* free sec cma mem */
HI_S32 hi_drv_secmmz_release(const mmz_buffer *secmmz_buf);

/* alloc sec sys mem  */
HI_S32 hi_drv_secsmmu_alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, smmu_buffer *sec_smmu_buf);

/* free sec sys mem */
HI_S32 hi_drv_secsmmu_release(const smmu_buffer *sec_smmu_buf);

/* normal smmu map to sec smmu */
HI_S32 hi_drv_smmu_map_to_sec_smmu(HI_U32 non_secsmmu, smmu_buffer *sec_smmu_buf);
/* normal smmu unmap from sec smmu  */
HI_S32 hi_drv_smmu_unmap_from_sec_smmu(const smmu_buffer *sec_smmu_buf);

/* normal cma mem map to sec smmu */
HI_S32 hi_drv_mmz_map_to_sec_smmu(const mmz_buffer *mmz_buf, smmu_buffer *sec_smmu_buf);

/* normal cma mem unmap from sec smmu */
HI_S32 hi_drv_mmz_unmap_from_sec_smmu(const smmu_buffer *sec_smmu_buf);
/* increment refcount for sec mmz buffer */
HI_S32 hi_drv_secmmz_buffer_get(mmz_buffer *secmmz_buf);

/* decrement refcount for sec mmz buffer  */
HI_S32 hi_drv_secmmz_buffer_put(mmz_buffer *secmmz_buf);
/* increment refcount for sec smmu buffer */
HI_S32 hi_drv_secsmmu_buffer_get(smmu_buffer *sec_smmu_buf);

/* decrement refcount for sec smmu buffer  */
HI_S32 hi_drv_secsmmu_buffer_put(smmu_buffer *sec_smmu_buf);

/* query smmu buffer refcount */
HI_S32 hi_drv_smmu_query_buffer_ref(smmu_buffer *smmu_buf, HI_U32 *ref);

/* query sec-mmz buffer refcount */
HI_S32 hi_drv_secmmz_query_buffer_ref(mmz_buffer *secmmz_buf, HI_U32 *ref);

/* query sec-smmu buffer refcount */
HI_S32 hi_drv_secsmmu_query_buffer_ref(smmu_buffer *sec_smmu_buf, HI_U32 *ref);

HI_S32 hi_drv_secsmmu_get_smmu(const smmu_buffer *sec_smmu_buf, smmu_buffer *smmu_buf);

/* query where the iommu addr come from
 * smmu_buf: input, smmu address should be input
 * source: output, 0: iommu addr from mmz driver
 *                 1: iommu addr from other driver such as  ion/gpu ..
 *                -1: iommu addr is illegal
 * return val:
 *            0: exec successfully
 *           !0: exec failed
 *
 * when return val is !0,the source should be ignored.
 * */
HI_S32 hi_drv_smmu_query_buffer_source(smmu_buffer *smmu_buf, HI_S32 *source);

/* query where the sec_iommu addr come from
 * sec_smmu: input, smmu address should be input
 * source: output, 0: sec_iommu point buffer from mmz driver
                   1: sec_iommu point buffer don't from mmz driver
                  -1: sec_iommu is illegal
 * return val:
 *          0: exec successfully
 *         !0: exec failed
 *
 * when return val is !0,the source should be ignored.
 * */
HI_S32 hi_drv_sec_smmu_query_buffer_source(unsigned int sec_smmu, HI_S32 *source);

int hi_drv_mmz_init(void);

void hi_drv_mmz_exit(void);

HI_S32 drv_mmz_mod_init(HI_VOID);

HI_VOID drv_mmz_mod_exit(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

