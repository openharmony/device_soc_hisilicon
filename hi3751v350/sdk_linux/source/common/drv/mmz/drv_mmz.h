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

#ifndef __MEDIA_MEM_INTF_H__
#define __MEDIA_MEM_INTF_H__

#include <linux/version.h>

#include "hi_type.h"

typedef hi_u32 mmb_addr_t;

#define MMB_ADDR_INVALID (0)

mmb_addr_t new_mmb(const char *name, int size, unsigned int align, const char *zone_name);
void delete_mmb(mmb_addr_t addr, unsigned int iommu);
void *remap_mmb(mmb_addr_t addr, unsigned int iommu);
void *remap_mmb_cached(mmb_addr_t addr, unsigned int iommu);
#ifndef DMABUF_FLUSH_CACHE
int unmap_mmb(const void *mapped_addr);
#else
int unmap_mmb(void *mapped_addr);
#endif
mmb_addr_t cma_mapto_smmu(mmb_addr_t addr, int iommu);
int cma_unmapfrom_smmu(mmb_addr_t addr, int iommu);
mmb_addr_t get_phyaddr_byvirt(const void *mapped_addr, int iommu);
void flush_cache_kern(mmb_addr_t phyaddr, const void *viraddr, mmb_addr_t len, unsigned int iommu);
struct sg_table *get_meminfo(hi_u32 addr, hi_u32 iommu, hi_u32 *size, hi_u32 *base);
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
mmb_addr_t get_sec_smmu_by_nosmmu(HI_U32 nonsmmu);
mmb_addr_t get_sec_smmu_by_phys(HI_U32 phys_addr);
mmb_addr_t get_phys_by_secsmmu(HI_U32 sec_smmu);
mmb_addr_t get_nonsecsmmu_by_secsmmu(HI_U32 sec_smmu);
#endif

int mmb_buf_get(HI_U32 addr, HI_U32 iommu);
int mmb_buf_put(HI_U32 addr, HI_U32 iommu);

int mmb_buf_ref_query(HI_U32 addr, HI_U32 iommu, HI_U32 *ref);
int query_buffer_source(HI_U32 iommu_addr, HI_S32 *source);
int query_secure_buffer_source(HI_U32 sec_smmu, HI_S32 *source);
int dma_buf_export_fd(unsigned int phyaddr, unsigned int iommu);
#endif

