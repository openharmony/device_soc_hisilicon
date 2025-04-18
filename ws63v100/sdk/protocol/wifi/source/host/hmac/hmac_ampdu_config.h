﻿/*
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
  文 件 名   : hmac_ampdu_config.h
  生成日期   : 2022年9月21日
  功能描述   : hmac_ampdu_config.c 的头文件
 */

#ifndef __HMAC_AMPDU_CONFIG_H__
#define __HMAC_AMPDU_CONFIG_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "hmac_user.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_AMPDU_CONFIG_H

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
  10 inline函数定义
*****************************************************************************/

/*****************************************************************************
  11 函数声明
*****************************************************************************/
#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
osal_s32 hmac_config_set_ampdu_tx_hw_on(hmac_vap_stru *hmac_vap, mac_cfg_ampdu_tx_on_param_stru *ampdu_tx_on);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_ampdu_config.h */
