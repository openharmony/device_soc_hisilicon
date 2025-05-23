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

#ifndef __ALG_ANTI_INTERFERENCE_H__
#define __ALG_ANTI_INTERFERENCE_H__

#ifdef _PRE_WLAN_FEATURE_ANTI_INTERF

/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#include "hmac_alg_if.h"
#include "msg_alg_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/* 弱干扰免疫参数 */
#define ALG_ANTI_INF_RSSI_TH_DEFAULT_VAL       (-95)   /* 弱干扰免疫RSSI门限的初始默认值 */
#define ALG_ANTI_INF_RSSI_TH_MAX_VAL           (-10)   /* 弱干扰免疫RSSI门限最大值 */
#define ALG_ANTI_INF_RSSI_TH_MIN_VAL           (-128)   /* 弱干扰免疫RSSI门限最小值 */
#define ALG_ANTI_INF_UNLOCK_TX_TH_DEFAULT_VAL (-107)   /* 动态agc unlock tx门限初始默认值 */
#define ALG_ANTI_INF_UNLOCK_RX_TH_DEFAULT_VAL (-107)   /* 动态agc unlock rx门限初始默认值 */
#define ALG_ANTI_INF_MAX_DUR_VAL              32767     /* 本BSS的duration门限(us) */
#define ALG_ANTI_INF_OBSS_MAX_DUR_VAL         13000     /* 其他BSS的duration门限(us) */
/******************************************************************************
  2 函数声明
******************************************************************************/
osal_s32 alg_anti_intf_init(osal_void);
osal_void alg_anti_intf_exit(osal_void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* #ifdef _PRE_WLAN_FEATURE_ANTI_INTERF */
#endif /* end of alg_anti_interference.h */