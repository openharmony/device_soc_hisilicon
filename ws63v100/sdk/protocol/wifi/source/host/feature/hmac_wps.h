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
 * Description: hmac_wps
 */

#ifndef __HMAC_WPS_H__
#define __HMAC_WPS_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "frw_ext_if.h"
#include "hmac_vap.h"
#include "mac_device_ext.h"
#include "hmac_device.h"
#include "wlan_types_common.h"
#include "hmac_feature_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    MAC_P2P_IE = 0,
    MAC_WPS_IE = 1,
    MAC_IE_BUTT
} mac_ie_type_enum;
typedef osal_u32 mac_ie_type_enum_uint32;

typedef struct {
    mac_ie_type_enum_uint32 ie_type;
    osal_u32 oui;
    osal_u32 oui_type;
}mac_ie_oui_stru;

/*****************************************************************************
  2 函数声明
*****************************************************************************/
typedef osal_void (*hmac_scan_del_wps_ie_cb)(hmac_vap_stru *hmac_vap, osal_u32 *len, osal_u8 *ie, osal_u8 del_wps_ie);

#ifdef CONTROLLER_CUSTOMIZATION
osal_u32 hmac_wps_init(osal_void);
osal_void hmac_wps_deinit(osal_void);
#endif

static osal_u32 hmac_wps_init_weakref(osal_void) __attribute__ ((weakref("hmac_wps_init"), used));
static osal_void hmac_wps_deinit_weakref(osal_void) __attribute__ ((weakref("hmac_wps_deinit"), used));

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_wps.h */
