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
 * Description: Header file
 */

#ifndef MSG_SDP_ROM_H
#define MSG_SDP_ROM_H

#include "wlan_types.h"

typedef struct {
    osal_u8 is_init;
    osal_u8 tx_enable;
    osal_u8 publish_cnt;
    osal_u8 subscribe_cnt;
    osal_u8 pre_dw;
    osal_u8 rsv[2]; /* 保留2个字节 */
    osal_u8 timer_enable;
    osal_u16 init_period_ms;
    osal_u16 init_dw_ms;
    mac_channel_stru sw_chan;
} mac_sdp_init_info;

typedef struct {
    osal_u8 mac[WLAN_MAC_ADDR_LEN];
    osal_u8 peer_handle;
    osal_u8 type : 1; /* 1-add mac/handle; 0-clear mac/handle */
    osal_u8 rsv : 7; /* 7 bits */
} oal_sdp_peer_mac_handle;

#endif