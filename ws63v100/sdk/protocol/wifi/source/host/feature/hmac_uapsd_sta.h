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
 * Description: hmac_uapsd_sta的头文件
 * Create: 2023-3-3
 */
#ifndef HMAC_UAPSD_STA_H
#define HMAC_UAPSD_STA_H

#include "mac_vap_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef osal_void (*hmac_uapsd_sta_set_qos_info_cb)(hmac_vap_stru *hmac_vap, osal_u8 *buffer);
typedef mac_cfg_uapsd_sta_stru* (*hmac_uapsd_sta_get_uapsd_info_cb)(osal_u8 vap_id);

static osal_u32 hmac_uapsd_sta_init_weakref(osal_void) __attribute__ ((weakref("hmac_uapsd_sta_init"), used));
static osal_void hmac_uapsd_sta_deinit_weakref(osal_void)
    __attribute__ ((weakref("hmac_uapsd_sta_deinit"), used));

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif