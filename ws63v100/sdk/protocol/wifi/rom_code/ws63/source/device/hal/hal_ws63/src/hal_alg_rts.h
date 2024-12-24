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
 * Description: Header file for hal_alg_txbf.c.
 */

#ifndef __HAL_ALG_RTS_H__
#define __HAL_ALG_RTS_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "hal_common_ops.h"
#include "hh503_phy_reg.h"
#include "wlan_spec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
   宏定义
*****************************************************************************/

/*****************************************************************************
  STRUCT定义
*****************************************************************************/

/*****************************************************************************
  函数声明
*****************************************************************************/
#ifdef _PRE_WLAN_FEATURE_RTS
osal_void hal_set_rts_rate_init(osal_void);
osal_void hal_set_rts_fail_tx_psdu_en(oal_bool_enum_uint8 cfg_rts_fail_tx_psdu_en);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hal_alg_rts.h */

