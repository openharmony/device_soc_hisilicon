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
 * Description: The Header File For HMAC Tx Wi-Fi Data.
 * Create: 2022-04-14
 */

#ifndef __HMAC_TX_ENCAP_H__
#define __HMAC_TX_ENCAP_H__
#include "mac_vap_ext.h"
#include "mac_user_ext.h"
#include "oal_netbuf_ext.h"
#include "dmac_ext_if_hcm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

osal_u32 hmac_tx_encap_etc(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf);
osal_u32 hmac_tx_send_encap_data(hmac_vap_stru *hmac_vap, oal_netbuf_stru *netbuf);
osal_u32 hmac_tx_send_encap_mgmt(hmac_vap_stru *hmac_vap, oal_netbuf_stru *netbuf);
osal_u32 hmac_tx_set_addresses_sta(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    const mac_tx_ctl_stru *tx_ctl, mac_ether_header_stru *ether_hdr, mac_ieee80211_qos_htc_frame_addr4_stru *hdr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
