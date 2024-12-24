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
 * Description: Header file for cali_iq_debug.c.
 */

#ifndef __CALI_IQ_DEBUG_H__
#define __CALI_IQ_DEBUG_H__

#include "cali_outer_os_if.h"
#include "cali_outer_hal_if.h"
#include "cali_base_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_CALI_IQ_DEBUG
#ifdef _PRE_WLAN_RF_CALI_DEBUG
enum {
    CALI_TRX_IQ_PREPARE = 0,
    CALI_TRX_IQ_RF_PATH = 1,
    CALI_TRX_IQ_PHY_PATH = 2,
    CALI_TRX_IQ_PLL = 3,
    CALI_TRX_IQ_COMP = 4,
    CALI_TRX_IQ_CALI = 5,
    CALI_TRX_IQ_FINISH = 6,
    CALI_TRX_IQ_RESET_RF_PATH = 7,
    CALI_TRX_IQ_RESET_PHY_PATH = 8,
    CALI_TRX_IQ_FFT_REPORT = 9,

    CALI_TRX_IQ_BUTT
};
osal_void cali_trx_iq_debug(hal_device_stru *device, wlan_channel_band_enum_uint8 band,
    cali_bandwidth_enum_uint8 rf_cali_bandwidth, osal_u32 cfg);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of __CALI_IQ_DEBUG_H__ */
