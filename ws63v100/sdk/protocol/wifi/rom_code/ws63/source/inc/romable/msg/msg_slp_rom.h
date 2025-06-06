﻿/*
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
 * Description: Header file
 */

#ifndef MSG_SLP_ROM_H
#define MSG_SLP_ROM_H

#include "oal_mem_pool.h"
#include "oal_netbuf_common_rom.h"

typedef struct {
    /* host slp报文结构：CB(48byte) + MAC(36byte) + PLAYLOAD(依据实际情况而定)
     * 报文通过frw消息发送至device侧，再申请netbuf空间进行依次拷贝操作
     */
    osal_u8 slp_cts_data[OAL_MAX_CB_LEN + OAL_MAX_MAC_HDR_LEN];
    osal_u8 slp_rm_request_data[OAL_MAX_CB_LEN + OAL_MAX_MAC_HDR_LEN + WLAN_SMGMT_NETBUF_SIZE];
    osal_u8 slp_gpio_data[OAL_MAX_CB_LEN + OAL_MAX_MAC_HDR_LEN];
    osal_u16 slp_timer_val;
    osal_u8 rm_type : 1; /* 0:广播测距 1:单播测距 */
    osal_u8 resv : 7; /* 保留7bit对齐 */
    osal_u8 resv1;
} mac_slp_rm_stru;

#endif