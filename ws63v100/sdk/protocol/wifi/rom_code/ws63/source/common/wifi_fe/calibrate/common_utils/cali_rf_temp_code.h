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
 * Description: 温度码接口头文件
 */
#ifndef __CALI_RF_TEMP_CODE_H__
#define __CALI_RF_TEMP_CODE_H__
#include "hal_common_ops.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
osal_void rf_temp_code_config_comp_value(hal_to_dmac_device_stru *device,
    wlan_channel_band_enum_uint8 band, osal_bool is_init);
osal_void rf_temp_code_create_config_timer(hal_to_dmac_device_stru *device);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  // __CALI_RF_TEMP_CODE_H__
