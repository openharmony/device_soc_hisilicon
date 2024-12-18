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
 * Description: Header file for hal_irq_rom.c.
 */

#ifndef HAL_BCN_ROM_H
#define HAL_BCN_ROM_H

#include "hal_device_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HAL_BCN_H

osal_void hh503_irq_wait_beacon_timeout_isr(hal_device_stru *hal_device, osal_u8 data);
#ifdef _PRE_WLAN_FEATURE_MAC_PARSE_TIM
osal_void hh503_irq_bcn_no_frm_isr(hal_device_stru *hal_device, osal_u8 data);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* HAL_BCN_H */

