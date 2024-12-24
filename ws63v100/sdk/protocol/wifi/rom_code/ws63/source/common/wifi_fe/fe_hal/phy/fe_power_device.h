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
 * Description: 校准涉及到的phy功能接口
 * Date: 2022-10-19
 */

#ifndef __FE_POWER_DEVICE_H__
#define __FE_POWER_DEVICE_H__

#include "cali_outer_hal_if.h"
#include "cali_complex_num.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
osal_u8 fe_hal_mac_get_pdet_en(const hal_device_stru *device);
osal_u32 fe_hal_mac_get_pdet(const hal_device_stru *device);
osal_u8 fe_hal_mac_get_rpt_tx_tpc(const hal_device_stru *device);
osal_void fe_hal_online_cali_get_pdet_val(hal_tx_complete_event_stru *tx_comp_event);
osal_void fe_hal_phy_set_single_ppa_lut(const hal_device_stru *device, osal_u16 upc_code, osal_u16 index);
osal_u32 fe_hal_phy_get_single_ppa_lut(const hal_device_stru *device, osal_u16 index);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif  // __CALI_PHY_IF_H__