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
 * Description: Header file for wal_vlwip.c
 * Create: 2019-08-04
 */

#ifndef __WAL_VLWIP_H__
#define __WAL_VLWIP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_VLWIP
typedef enum {
    WIFI_FILTER_NETIF_LWIP             = 0,
    WIFI_FILTER_NETIF_VLWIP            = 1,
    WIFI_FILTER_NETIF_BOTH             = 2,
    WIFI_FILTER_NETIF_BUTT
} wifi_filter_netif_enum;

typedef td_s32 (*driverif_input_func)(const struct netif *netif, struct pbuf *pbuf);
int uapi_vlwip_set_debug_callback(driverif_input_func cb);
td_u32 wal_vlwip_register_netdev(oal_net_device_stru *netdev);
td_void wal_vlwip_unregister_netdev(oal_net_device_stru *netdev);
td_u8 wal_vlwip_get_netif_type(td_void);
td_void hwal_packet_receive(oal_net_device_stru *netdev, oal_netbuf_stru *netbuf);

#endif

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif /* end __WAL_VLWIP_H__ */

