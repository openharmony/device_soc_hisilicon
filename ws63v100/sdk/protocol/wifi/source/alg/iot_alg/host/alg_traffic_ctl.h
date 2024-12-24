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
 * Description: algorithm traffic control head
 */

#ifndef __ALG_TRAFFIC_CTL_H__
#define __ALG_TRAFFIC_CTL_H__

#ifdef _PRE_WLAN_FEATURE_TRAFFIC_CTL

/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#include "hmac_alg_if.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/******************************************************************************
  1 Traffic ctl 宏定义
******************************************************************************/
#define ALG_TRAFFIC_CTL_TIMEOUT                       100 /* 周期调整定时器周期时间 ms */
#define ALG_TRAFFIC_CTL_BUSY_CTRL_FLAG                OSAL_TRUE
#define ALG_TRAFFIC_CTL_MATH_SHIFT_SIZE               2 /* 平滑公式的平滑系数 */
#define ALG_TRAFFIC_CTL_MAX_FLOWCTL_CNT               0xFFFF /* 最长流控时间为10*100ms */

#define ALG_TRAFFIC_CTL_LOG_DEBUG_DEFAULT             OSAL_FALSE
/******************************************************************************
  2 函数声明
******************************************************************************/
osal_s32 alg_hmac_traffic_ctl_init(osal_void);
osal_void alg_hmac_traffic_ctl_exit(osal_void);
osal_u32 alg_traffic_ctl_get_mpdu_cnt(osal_void);
osal_void alg_traffic_ctl_bypass_tid_queue(osal_void);
osal_void alg_traffic_ctl_enqueue_tid_queue(osal_void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* #ifdef _PRE_WLAN_FEATURE_TRAFFIC_CTL */
#endif /* end of alg_traffic_ctl.h */