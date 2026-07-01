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
#ifndef SVP_NPU_KERNEL_H
#define SVP_NPU_KERNEL_H

#include "ot_osal.h"
#include "ot_type.h"
#include "securec.h"
#include "npu_hal.h"
#include "mkp_npu.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct {
    struct osal_list_head busy_aicpu_list;
    struct osal_list_head wait_aicpu_list;
} svp_npu_aicpu_list;

typedef struct {
    td_void *check_info;
    td_bool *is_used;
    td_u16 stream_num;
    td_u16 report_num;
} svp_npu_device_process_info;

typedef struct {
    td_bool prof_under_process;
    td_bool prof_start;
    td_u32 prof_channels;
    td_u32 interval;
} svp_npu_process_prof_info;

typedef struct {
    td_phys_addr_t phys_addr;
    td_u32 total_size;
    td_u32 check_info_size;
    svp_npu_device_process_info info[SVP_NPU_DEVICE_BUTT];
    svp_npu_aicpu_list aicpu_list[SVP_NPU_PRIORITY_BUTT];
    osal_spinlock_t aicpu_spin_lock;
    osal_mutex_t aicpu_mutex;
    td_u32 total_stream_num;
    td_u32 query_timeout;
    osal_mutex_t *query_timeout_mutex;

    struct osal_list_head list[SVP_NPU_DEVICE_BUTT];

    struct osal_list_head model_list_head;
    td_u32 model_num;

    svp_npu_process_prof_info prof_info;
} svp_npu_process_info;

typedef struct {
    td_u16 max_task_node_num;
    td_u8 power_save_en;
    td_u8 rsv;
} svp_npu_mod_param;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* SVP_NPU_KERNEL_H */
