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
 * Description: The Header File For oal_netbuf_data.c
 * Create: 2022-04-14
 */

#ifndef __OAL_NETBUF_DATA_H__
#define __OAL_NETBUF_DATA_H__
#include "oal_netbuf_ext.h"
#include "dmac_ext_if_device.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

static INLINE__ osal_u8 *oal_netbuf_tx_data(const oal_netbuf_stru *netbuf)
{
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb_const(netbuf);
    return oal_netbuf_header(netbuf) + tx_ctl->frame_header_length;
}

static INLINE__ const osal_u8 *oal_netbuf_tx_data_const(const oal_netbuf_stru *netbuf)
{
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb_const(netbuf);
    return oal_netbuf_header(netbuf) + tx_ctl->frame_header_length;
}

static INLINE__ osal_u8 *oal_netbuf_rx_data(const oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb_const(netbuf);
    return oal_netbuf_header(netbuf) + rx_ctl->mac_header_len;
}

static INLINE__ const osal_u8 *oal_netbuf_rx_data_const(const oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb_const(netbuf);
    return oal_netbuf_header_const(netbuf) + rx_ctl->mac_header_len;
}

static INLINE__ osal_u16 oal_netbuf_rx_frame_len(oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    return rx_ctl->frame_len;
}

static INLINE__ osal_u16 oal_netbuf_rx_header_len(oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    return rx_ctl->mac_header_len;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
