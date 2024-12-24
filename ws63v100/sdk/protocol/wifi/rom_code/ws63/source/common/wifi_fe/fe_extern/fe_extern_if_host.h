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
#ifndef __FE_EXTERN_IF_HOST_H__
#define __FE_EXTERN_IF_HOST_H__

#include "mac_vap_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

osal_void online_cali_init(hal_to_dmac_device_stru *hal_device);
osal_s32 online_cali_tx_complete_dyn_process(hmac_vap_stru *hmac_vap, frw_msg *msg);
osal_void cali_pow_initialize_tx_power(hal_to_dmac_device_stru *hal_to_dmac_device);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif