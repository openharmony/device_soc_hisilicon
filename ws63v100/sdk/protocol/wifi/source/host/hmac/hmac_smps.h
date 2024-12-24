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
 * 文 件 名   : hmac_smps.h
 * 生成日期   :  2014年4月17日
 * 功能描述   : hmac_smps.c 的头文件
 */

#ifndef __HMAC_SMPS_H__
#define __HMAC_SMPS_H__

#ifdef _PRE_WLAN_FEATURE_SMPS

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "oal_ext_if.h"
#include "hmac_main.h"
#include "oam_ext_if.h"
#include "mac_resource_ext.h"
#include "dmac_ext_if_hcm.h"
#include "mac_device_ext.h"
#include "mac_vap_ext.h"
#include "hmac_vap.h"
#include "mac_user_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_SMPS_H
/*****************************************************************************
  2 宏定义
*****************************************************************************/


/*****************************************************************************
  3 枚举定义
*****************************************************************************/


/*****************************************************************************
  4 全局变量声明
*****************************************************************************/


/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/
extern osal_u32 hmac_smps_update_user_status(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user);

osal_u8 hmac_smps_get_hal_mode(osal_u8 dev_smps_mode);

osal_void hmac_smps_send_action(hmac_vap_stru *hmac_vap, wlan_mib_mimo_power_save_enum_uint8 smps_mode,
    oal_bool_enum_uint8 bool_code);

osal_u32 hmac_check_smps_field(hmac_vap_stru *hmac_vap, osal_u8 *payload, osal_u32 msg_len,
    hmac_user_stru *hmac_user);

osal_u32 hmac_mgmt_rx_smps_frame(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, const osal_u8 *frame_payload);

extern osal_s32 hmac_config_set_smps_mode(hmac_vap_stru *hmac_vap, frw_msg *msg);

osal_u8 hmac_user_get_smps_mode(const hmac_vap_stru *hmac_vap, const hmac_user_stru *hmac_user);

/*****************************************************************************
 功能描述 : 填写SM Power Save字段
*****************************************************************************/
static inline osal_u8 hmac_calc_smps_field(osal_u8 smps)
{
    if (smps == WLAN_MIB_MIMO_POWER_SAVE_DYNAMIC) {
        return MAC_SMPS_DYNAMIC_MODE;
    } else if (smps == WLAN_MIB_MIMO_POWER_SAVE_STATIC) {
        return MAC_SMPS_STATIC_MODE;
    }

    return MAC_SMPS_MIMO_MODE;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of _PRE_WLAN_FEATURE_SMPS */

#endif /* end of hmac_smps.h */