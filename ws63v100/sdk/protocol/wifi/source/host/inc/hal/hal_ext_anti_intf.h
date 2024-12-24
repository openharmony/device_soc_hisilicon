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
 * Description: Header file for alg_intf_det.c
 */

#ifndef __HAL_EXT_ANTI_INTF_H__
#define __HAL_EXT_ANTI_INTF_H__
/*****************************************************************************
1 头文件包含
*****************************************************************************/
#include "osal_types.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
    2 函数声明
*****************************************************************************/
#ifdef _PRE_WLAN_FEATURE_ANTI_INTERF
osal_void hal_set_agc_unlock_min_th(osal_s8 tx_reg_val, osal_s8 rx_reg_val);
osal_void hal_set_nav_max_duration(osal_u16 bss_dur, osal_u32 obss_dur);
osal_void hal_get_nav_protect_times(osal_u32 *times);
osal_void hal_set_weak_intf_rssi_th(osal_s32 reg_val);
osal_void hal_get_cca_reg_th(osal_s8 *ac_reg_val, osal_u32 reg_num);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hal_ext_anti_intf.h */