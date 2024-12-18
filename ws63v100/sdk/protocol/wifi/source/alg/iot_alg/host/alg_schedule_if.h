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
 * Description: schedule algorithm
 */

#ifndef __ALG_SCHEDULE_IF_H__
#define __ALG_SCHEDULE_IF_H__

/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#ifdef _PRE_WLAN_FEATURE_SCHEDULE
#include "msg_alg_rom.h"
#include "frw_msg_rom.h"
#include "mac_vap_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/******************************************************************************
  3 枚举定义
******************************************************************************/

/******************************************************************************
  4 STRUCT定义
******************************************************************************/

/******************************************************************************
  5 外部函数声明
******************************************************************************/
/* 业务调度对外接口函数 */
osal_s32 alg_schedule_init(osal_void);
osal_void alg_schedule_exit(osal_void);
osal_s32 alg_schedule_user_info_sync(hmac_vap_stru *hmac_vap, frw_msg *msg);
osal_u32 alg_schedule_config_param(hmac_vap_stru *hmac_vap, frw_msg *msg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* #ifdef _PRE_WLAN_FEATURE_SCHEDULE */
#endif /* end of alg_schedule_if.h */
