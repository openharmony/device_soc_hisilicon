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
#include "npu_hal.h"
#include "ot_osal.h"
#include "mkp_npu.h"

#define SVP_NPU_SMMU_TCU_OFFSET      0x10000
#define SVP_NPU_SMMU_TBU_OFFSET      0x0

/* CNcomment: 寄存器影射后的虚拟地址 */
static volatile td_void *g_svp_npu_smmu_reg[SVP_NPU_DEVICE_BUTT] = { TD_NULL };

td_s32 svm_smmu_power_on(void *base, unsigned int tcu_offset, unsigned int tbu_offset);

td_void hal_svp_npu_smmu_power_on(td_u32 device_id)
{
    (td_void)svm_smmu_power_on((td_void *)(g_svp_npu_smmu_reg[device_id]),
        SVP_NPU_SMMU_TCU_OFFSET, SVP_NPU_SMMU_TBU_OFFSET);
}
