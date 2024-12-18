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
 * Description: algorithm antanne interference head
 */

#ifndef __ALG_RTS_H__
#define __ALG_RTS_H__

#ifdef _PRE_WLAN_FEATURE_RTS

/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#include "hmac_alg_if.h"
#include "hal_alg_rts.h"
#include "msg_alg_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/******************************************************************************
  2 函数声明
******************************************************************************/
osal_s32 alg_host_rts_init(osal_void);
osal_s32 alg_host_rts_exit(osal_void);
osal_u32 alg_rts_para_sync_fill(alg_param_sync_stru *data);
osal_u32 alg_rts_config_threshold(hmac_vap_stru *hmac_vap, osal_u32 rts_threshold);
osal_u32 alg_rts_set_host_param(mac_ioctl_alg_param_stru *alg_param);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* #ifdef _PRE_WLAN_FEATURE_RTS */
#endif /* end of alg_rts.h */