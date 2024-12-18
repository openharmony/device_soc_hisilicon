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
 * Description: hmac_keep_alive
 * Author:
 * Create: 2022-09-22
 */
#ifndef __HMAC_KEEP_ALIVE_H__
#define __HMAC_KEEP_ALIVE_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "oal_ext_if.h"
#include "mac_vap_ext.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_KEEP_ALIVE_H

osal_s32 hmac_config_keepalive_sync_timestamp(hmac_vap_stru *hmac_vap, frw_msg *msg);
osal_s32 hmac_config_set_keepalive_mode(hmac_vap_stru *hmac_vap, frw_msg *msg);
osal_s32 hmac_config_common_debug(hmac_vap_stru *hmac_vap, frw_msg *msg);
osal_u32 hmac_config_get_ap_user_aging_time(osal_void);
osal_s32 hmac_config_set_ap_user_aging_time(hmac_vap_stru *hmac_vap, frw_msg *msg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_keep_alive.h */

