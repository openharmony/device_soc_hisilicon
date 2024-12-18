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
 * Description: hmac_dnb_sta.c的头文件
 * Date: 2022-10-18
 */

#ifndef __HMAC_DNB_STA_H__
#define __HMAC_DNB_STA_H__

#include "mac_vap_ext.h"
#include "osal_types.h"
#include "frw_msg_rom.h"
#include "mac_frame.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HMAC_MAX_HE_LTF_NUM 4

typedef osal_void (*hmac_set_dnb_cap_cb)(hmac_vap_stru *hmac_vap, osal_u8 **buffer);

static osal_u32 hmac_sta_dnb_init_weakref(osal_void) __attribute__ ((weakref("hmac_sta_dnb_init"), used));
static osal_void hmac_sta_dnb_deinit_weakref(osal_void) __attribute__ ((weakref("hmac_sta_dnb_deinit"), used));

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
