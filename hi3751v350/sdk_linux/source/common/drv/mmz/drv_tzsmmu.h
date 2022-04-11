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

#ifndef __DRV_TZSMMU_H
#define __DRV_TZSMMU_H

#include <linux/hisilicon/tee_smmu.h>
#include "hi_drv_mmz.h"
#include "hi_type.h"

u32 secmem_alloc(mmb_addr_t phyaddr, unsigned int iommu);
u32 secmem_free(mmb_addr_t sec_addr, unsigned int iommu);

u32 secmem_map_to_secsmmu(HI_U32 phyaddr, int iommu);
int secmem_unmap_from_secsmmu(HI_U32 sec_addr, int iommu);


int hi_tee_agent_end(void);

int sec_mem_get(HI_U32 sec_addr, int iommu);
int sec_mem_put(HI_U32 sec_addr, int iommu);
int sec_mem_buf_query_ref(HI_U32 sec_addr, int iommu, HI_U32 *ref);

#endif
