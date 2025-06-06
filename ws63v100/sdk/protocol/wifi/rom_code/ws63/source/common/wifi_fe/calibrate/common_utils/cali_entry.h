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
 * Description: 校准入口/上层处理
 */

#ifndef __CALI_ENTRY_H__
#define __CALI_ENTRY_H__

#include "cali_outer_os_if.h"
#include "cali_outer_hal_if.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

osal_void cali_offline_cali_one_rf_channel(hal_device_stru *device);
osal_void cali_offline_cali_entry(osal_void);

osal_void cali_offline_cali_prepare(hal_device_stru *device);
osal_void cali_offline_cali_finish(hal_device_stru *device);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
