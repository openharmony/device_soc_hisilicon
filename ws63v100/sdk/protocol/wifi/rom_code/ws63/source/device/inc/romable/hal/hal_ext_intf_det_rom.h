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
 * Description: hal tx.
 */

#ifndef HAL_EXT_INTF_DET_ROM_H
#define HAL_EXT_INTF_DET_ROM_H
/*****************************************************************************
    1 头文件包含
*****************************************************************************/
#include "hal_device_rom.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_INTF_DET
osal_void hal_set_counter1_clear(osal_void);
osal_void hal_set_mac_clken(oal_bool_enum_uint8 wctrl_enable);
osal_void hal_chan_measure_start(hal_chan_measure_stru *meas);
osal_void hh503_get_txrx_frame_time(hal_ch_mac_statics_stru *ch_statics);
osal_void hh503_irq_channel_statistics_isr(hal_device_stru *hal_device, osal_u8 data);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hal_ext_intf_det_rom.h */