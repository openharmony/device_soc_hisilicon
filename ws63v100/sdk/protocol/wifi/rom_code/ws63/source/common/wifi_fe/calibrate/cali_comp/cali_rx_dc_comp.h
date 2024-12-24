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
 * Description: rx dc校准补偿
 */

#ifndef __CALI_RX_DC_COMP_H__
#define __CALI_RX_DC_COMP_H__

#include "cali_outer_os_if.h"
#include "cali_outer_hal_if.h"
#include "fe_rf_dev.h"
#include "cali_base_def.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

osal_void cali_rx_dc_comp(const hal_rf_dev *rf_dev, wlan_channel_band_enum_uint8 band, osal_u8 chn_idx,
    cali_bandwidth_enum_uint8 rf_cali_bandwidth);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
