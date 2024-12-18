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
 * Description: header for cali rate pow lut
 */
#ifndef __FE_UPDATE_POW_H__
#define __FE_UPDATE_POW_H__
#include "cali_outer_hal_if.h"
#include "power_ppa_ctrl_spec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

// 根据校准数据中的ppa_code刷新某一tpc_code的ppa_code
osal_u16 cali_get_adjusted_ppa_code(const hal_device_stru *device, osal_u8 cali_subband, osal_u8 tpc_code);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif