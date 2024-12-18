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
 * Description: MAC/SLP interface implementation.
 */
#ifndef HAL_SLP_ROM_H
#define HAL_SLP_ROM_H
#ifdef _PRE_WLAN_FEATURE_SLP
#include "osal_types.h"

/*****************************************************************************
  1 其他头文件包含
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
  函数声明
******************************************************************************/
osal_void hal_set_slp_tx_gpio(osal_u32 val);
osal_void hal_set_slp_rx_gpio(osal_u32 val);
osal_u32 hal_get_slp_tx_gpio(osal_void);
osal_u32 hal_get_slp_rx_gpio(osal_void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
#endif
