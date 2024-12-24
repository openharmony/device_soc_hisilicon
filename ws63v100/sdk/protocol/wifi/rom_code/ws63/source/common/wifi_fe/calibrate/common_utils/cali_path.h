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
 * Description: 校准path相关
 */

#ifndef __HAL_CALI_PATH_H__
#define __HAL_CALI_PATH_H__

#include "cali_outer_os_if.h"
#include "cali_outer_hal_if.h"
#include "cali_base_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
osal_void cali_set_up_logen_rf_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
osal_void cali_recover_logen_rf_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
// rx dc的射频通路建立
osal_void cali_set_up_rx_dc_rf_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
// rx dc的射频通路恢复
osal_void cali_recover_rx_dc_rf_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
// tx dc的射频通路建立
osal_void cali_set_up_tx_dc_rf_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
// tx dc的射频通路恢复
osal_void cali_recover_tx_dc_rf_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
osal_void cali_set_up_txpwr_rf_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
osal_void cali_recover_txpwr_rf_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
osal_void cali_set_up_rc_rf_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
osal_void cali_recover_rc_rf_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
// IQ 校准通路
osal_void cali_set_up_tx_iq_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
osal_void cali_set_up_rx_iq_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
osal_void cali_recover_tx_iq_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
osal_void cali_recover_rx_iq_path(const hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 bw, osal_u8 chain);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
