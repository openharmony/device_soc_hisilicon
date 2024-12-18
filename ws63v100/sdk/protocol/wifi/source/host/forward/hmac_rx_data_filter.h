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
 * Description: the head file for hmac rx data filter
 * Create: 2022-5-11
 */

#ifndef __HMAC_RX_DATA_FILTER_H__
#define __HMAC_RX_DATA_FILTER_H__

#include "mac_vap_ext.h"
#include "oal_net.h"
#include "dmac_ext_if_device.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

osal_void hmac_rx_data_user_is_null(hmac_vap_stru *hmac_vap, mac_ieee80211_frame_stru *frame_hdr);
dmac_rx_frame_ctrl_enum_uint8 hmac_rx_deal_frame_filter(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    dmac_rx_ctl_stru *cb_ctrl, oal_netbuf_stru *netbuf);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

