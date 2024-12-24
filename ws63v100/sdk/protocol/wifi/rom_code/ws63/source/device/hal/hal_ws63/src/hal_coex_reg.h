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
 * Description: Header file for hal_coex_reg.c.
 */

#ifndef HAL_COEX_REG_H
#define HAL_COEX_REG_H

/*****************************************************************************
  其他头文件包含
*****************************************************************************/
#include "oal_ext_if_rom.h"
#include "hal_coex_reg_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_BTCOEX

#undef THIS_FILE_ID
#define THIS_FILE_ID DIAG_FILE_ID_WIFI_DEVICE_HAL_COEX_REG_H

#undef THIS_MOD_ID
#define THIS_MOD_ID DIAG_MOD_ID_WIFI_DEVICE
osal_void hal_btcoex_init_device(osal_void);
osal_u32 hal_btcoex_init(const hal_to_dmac_device_stru *hal_device);
osal_u32 hal_btcoex_sw_preempt_init(hal_to_dmac_device_stru *hal_device);
osal_bool hal_btcoex_get_bt_run(osal_void);

#ifdef _PRE_BSLE_GATEWAY
osal_void hal_bsle_set_priority(osal_void);
#endif

#endif /* #ifdef _PRE_WLAN_FEATURE_BTCOEX */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
