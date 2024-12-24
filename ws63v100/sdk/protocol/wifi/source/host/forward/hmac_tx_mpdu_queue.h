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
 * Description: hmac_tx_mpdu_queue.c 的头文件
 * Date: 2022-5-13
 */

#ifndef __HMAC_TX_MPDU_QUEUE_H__
#define __HMAC_TX_MPDU_QUEUE_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "mac_vap_ext.h"
#include "mac_user_ext.h"
#include "oal_net.h"
#include "hal_common_ops.h"
#include "dmac_ext_if_device.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_TX_COMPLETE_ROM_H

osal_u32  hmac_tx_schedule(hal_to_dmac_device_stru *hal_device, osal_u8 ac_num);
osal_u32 hmac_tx_process_tid_queue(hal_to_dmac_device_stru *hal_device, hmac_vap_stru *hmac_vap,
    oal_netbuf_stru *netbuf, hmac_tid_stru *tid_queue, osal_u8 hal_q);
oal_netbuf_stru *hmac_tx_dequeue_first_mpdu(oal_netbuf_head_stru *netbuf_head);
osal_void hmac_vap_clear_tx_queue(hmac_vap_stru *hmac_vap);
#ifdef _PRE_WIFI_DEBUG
void pkt_trace_debug(oal_netbuf_stru *netbuf, osal_char *direct);
void pkt_drop_debug(const mac_tx_ctl_stru *tx_ctl, const osal_char *drop_reason);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
