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
* Description: hmac power head
* Create: 2020-7-5
*/

#ifndef __HMAC_POWER_H__
#define __HMAC_POWER_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "hmac_vap.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  10 函数声明
*****************************************************************************/
osal_void hmac_pow_set_vap_tx_power(hmac_vap_stru *hmac_vap, hal_pow_set_type_enum_uint8 type);
osal_s32 hmac_power_set_rate_power_offset(hmac_vap_stru *hmac_vap, const frw_msg *msg);
extern osal_s32 hmac_config_host_dev_init_etc(hmac_vap_stru *hmac_vap, frw_msg *msg);
extern osal_s32 hmac_config_host_dev_exit_etc(hmac_vap_stru *hmac_vap, frw_msg *msg);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_power.h */
