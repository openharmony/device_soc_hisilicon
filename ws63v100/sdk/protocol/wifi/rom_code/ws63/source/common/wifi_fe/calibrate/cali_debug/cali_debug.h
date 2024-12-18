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
 * Description: 校准debug相关
 */

#ifndef __CALI_DEBUG_H__
#define __CALI_DEBUG_H__

#include "dmac_ext_if_rom.h"
#include "cali_base_def.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_RF_CALI_DEBUG
#define SINGLE_TONE_TEST_TYPE_OFFSET 10 // 小于10的部分给功率 其他给IQ
#define SINGLE_TONE_TEST_TYPE_OFFSET2 20
#define SINGLE_TONE_TEST_TYPE_OFFSET3 30
osal_s32 dmac_config_single_tone(dmac_vap_stru *dmac_vap, frw_msg *msg);
osal_s32 dmac_config_txdc_step(dmac_vap_stru *dmac_vap, frw_msg *msg);

osal_void cali_txpwr_step_debug(hal_device_stru *device,
    wlan_channel_band_enum_uint8 freq, cali_bandwidth_enum_uint8 rf_cali_bandwidth, osal_u32 data);
osal_void cali_tx_dc_step_debug(const hal_device_stru *device,
    wlan_channel_band_enum_uint8 freq, cali_bandwidth_enum_uint8 rf_cali_bandwidth, osal_u32 data);
osal_void cali_rx_dc_step_debug(const hal_device_stru *device,
    wlan_channel_band_enum_uint8 freq, cali_bandwidth_enum_uint8 rf_cali_bandwidth, osal_u32 data);
osal_void cali_rc_step_debug(const hal_device_stru *device,
    wlan_channel_band_enum_uint8 freq, cali_bandwidth_enum_uint8 rf_cali_bandwidth, osal_u32 data);
osal_void cali_logen_step_debug(hal_device_stru *device,
    wlan_channel_band_enum_uint8 band, cali_bandwidth_enum_uint8 bw, osal_u32 data);
#endif // #ifdef _PRE_WLAN_RF_CALI_DEBUG

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
