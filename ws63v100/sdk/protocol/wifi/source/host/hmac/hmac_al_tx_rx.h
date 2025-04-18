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
 * Description: hmac always tx or rx
 * Author:
 */

#ifndef __HMAC_AL_TX_RX_H__
#define __HMAC_AL_TX_RX_H__

#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "hmac_device.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

osal_u32 hmac_config_send_qos_to_queue(hmac_vap_stru *hmac_vap, osal_u32 len, osal_u8 type);
oal_netbuf_stru* hmac_config_create_al_tx_packet(osal_u32 size, mac_rf_payload_enum_uint8 payload_flag);
osal_s32 hmac_al_tx_complete_event_handler(hmac_vap_stru *hmac_vap, frw_msg *msg);
osal_s32 hmac_config_set_always_rx(hmac_vap_stru *hmac_vap, frw_msg *msg);
osal_s32 hmac_config_set_always_tx(hmac_vap_stru *hmac_vap, frw_msg *msg);
osal_void hmac_set_al_tx_frame_type_switch(osal_u8 val);
osal_void hmac_set_al_tx_protocol(osal_u8 val);
osal_void hmac_set_al_tx_mcs(osal_u32 val);
osal_u8 hmac_get_al_tx_frame_type_switch(osal_void);
osal_u32 hmac_get_al_tx_mcs(osal_void);
osal_u8 hmac_get_al_tx_protocol(osal_void);
osal_s32 hmac_config_get_rx_ppdu_info(hmac_vap_stru *hmac_vap, frw_msg *msg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
#endif
