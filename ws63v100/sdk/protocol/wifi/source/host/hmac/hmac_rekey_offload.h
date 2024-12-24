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
 * 文 件 名   : hmac_rekey_offload.h
 * 生成日期   : 2022年10月14日
 * 功能描述   : hmac_rekey_offload.c的头文件
 */
#ifndef __HMAC_REKEY_OFFLOAD_H__
#define __HMAC_REKEY_OFFLOAD_H__
/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "frw_ext_if.h"
#include "hmac_vap.h"
#include "msg_wow_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID DIAG_FILE_ID_WIFI_HOST_REKEY_ARP_OFFLOAD_H

#undef THIS_MOD_ID
#define THIS_MOD_ID DIAG_MOD_ID_WIFI_HOST


osal_void hmac_rekey_offload_get_param(mac_rekey_offload_param_sync *rekey_param);

static osal_u32 hmac_rekey_offload_init_weakref(osal_void) __attribute__ ((weakref("hmac_rekey_offload_init"), used));
static osal_void hmac_rekey_offload_deinit_weakref(osal_void)
    __attribute__ ((weakref("hmac_rekey_offload_deinit"), used));

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif