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
 * Description: 校准补偿相关配置
 */

#ifndef __CALI_COMP_CFG_H__
#define __CALI_COMP_CFG_H__

#include "cali_outer_os_if.h"
#include "cali_outer_hal_if.h"
#include "cali_spec.h"
#include "cali_base_def.h"
#include "fe_rf_dev.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 校准补偿函数指针定义 */
typedef osal_void (*cali_comp_func_ptr)(const hal_rf_dev *, wlan_channel_band_enum_uint8, osal_u8,
    cali_bandwidth_enum_uint8);

cali_comp_func_ptr cali_get_cali_comp_func(wlan_channel_band_enum_uint8 band, wlan_rf_cali_type_enum_uint8 cali_type);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
