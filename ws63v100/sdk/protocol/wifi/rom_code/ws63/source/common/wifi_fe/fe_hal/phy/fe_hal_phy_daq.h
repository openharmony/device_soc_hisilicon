﻿/*
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
 * Description: thruput test header file.
 */

#ifndef __FE_HAL_PHY_DAQ_H__
#define __FE_HAL_PHY_DAQ_H__
#ifdef _PRE_WLAN_FEATURE_DAQ
#ifdef _PRE_WLAN_ATE
#include "hal_common_ops_rom.h"
#include "hal_ext_if_device.h"
#else
#include "hal_common_ops.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#define HAL_DAQ_PHY_SAMPLE_MSG_ADDR 0x680000
/* ============== PHY 数采接口 起始 ============ */
osal_void hal_sample_free_msg(osal_void);
osal_u32 hal_sample_alloc_msg(osal_u16 data_len);
void hal_config_chip_test_set_diag_param_phy_recovery(hal_to_dmac_device_mac_test_stru *hal_mac_device);
void hal_config_chip_test_set_diag_param_phy_begin(hal_to_phy_test_diag_stru *param,
    hal_to_dmac_device_mac_test_stru *hal_mac_device);
oal_bool_enum_uint8 hal_config_chip_test_set_diag_param_phy_query(hal_to_dmac_device_mac_test_stru *hal_mac_device);
osal_u32 hal_config_chip_test_set_diag_param_phy_save_data(osal_u8 *data, osal_u32 data_len,
    hal_to_dmac_device_mac_test_stru *hal_mac_device);
/* ============== PHY 数采接口 结束 ============ */
osal_void hal_config_diag_int_switch(osal_u8 flag);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif  /* _PRE_WLAN_FEATURE_DAQ */
#endif  /* __FE_HAL_PHY_DAQ_H__ */
