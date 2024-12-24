/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description: DFR initialization and adapt interface implementation.
 */

#ifndef MSG_DFR_ROM_H
#define MSG_DFR_ROM_H

#include "hal_commom_ops_type_rom.h"
#include "wlan_spec_hh503_rom.h"
typedef struct {
    osal_u16 mem_total_cnt;   /* 本内存池一共有多少内存块 */
    osal_u16 mem_used_cnt;    /* 本内存池已用内存块 */
    osal_u16 reserved;        /* 保留2字节对齐 */
    osal_u32 mem_alloc_cnt;   /* 本内存池申请成功内存块统计 */
    osal_u32 mem_fail_cnt;    /* 本内存池申请成功内存块统计 */
    osal_u32 high_cnt;        /* 高优先级frw队列未被调用统计 */
    osal_u32 low_cnt;         /* 低优先级frw队列未被调用统计 */
    hal_rx_dscr_queue_header_stru rx_dscr_queue[HAL_RX_QUEUE_NUM];
    hal_tx_dscr_queue_header_stru tx_dscr_queue[HAL_TX_QUEUE_NUM];
} dfr_info_rsp_stru;

#endif