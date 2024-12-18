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
 * Description: 离线校准TX_DC
 */

#ifndef __CALI_TX_DC_H__
#define __CALI_TX_DC_H__

#include "osal_types.h"
#include "cali_outer_hal_if.h"
#include "cali_complex_num.h"
#include "cali_base_def.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

cali_complex_stru cali_tx_dc_calc_comp_val(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_complex_stru compensate, cali_complex_stru dc3);
osal_void cali_check_adjust_tx_dc_comp_val(cali_complex_stru *tx_dc_comp);
osal_void cali_data_save_tx_dc_val(const hal_device_stru *device,
    osal_u8 chn_idx, osal_u8 txdc_lvl_idx, cali_complex_stru *tx_dc_comp);
osal_void cali_tx_dc(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 cali_bandwidth, osal_u8 chain, osal_u8 chn_idx);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
