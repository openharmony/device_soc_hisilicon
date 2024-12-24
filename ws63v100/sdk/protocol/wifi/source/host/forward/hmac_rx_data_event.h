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
 * Description: the head file for hmac rx data msg process
 * Create: 2022-5-10
 */


#ifndef __HMAC_RX_DATA_EVENT_H__
#define __HMAC_RX_DATA_EVENT_H__

#include "mac_vap_ext.h"
#include "frw_msg_rom.h"
#include "oal_net.h"
#include "dmac_ext_if_device.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_RX_DATA_H

osal_s32 hmac_rx_data_event_adapt(hmac_vap_stru *hmac_vap, frw_msg *msg);
osal_s32 hmac_rx_process_data_msg(hmac_vap_stru *hmac_vap, frw_msg *msg);
osal_s32 hmac_rx_process_data_msg_process(hmac_vap_stru *hmac_vap, frw_msg *msg);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_rx_data_event.h */
