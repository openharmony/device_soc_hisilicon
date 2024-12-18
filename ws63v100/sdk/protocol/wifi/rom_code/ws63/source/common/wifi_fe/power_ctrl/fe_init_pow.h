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
 * Description: header for cali init power
 */
#ifndef __FE_INIT_POW_H__
#define __FE_INIT_POW_H__
#include "cali_online_common.h"
#include "hal_common_ops_rom.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 校准模块支持的band */
#define WLAN_CALI_BAND_BUTT                  (WLAN_BAND_2G + 1)
osal_void cali_pow_initialize_tx_power(hal_to_dmac_device_stru *hal_to_dmac_device);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif  // __CALI_INIT_POWER_H__