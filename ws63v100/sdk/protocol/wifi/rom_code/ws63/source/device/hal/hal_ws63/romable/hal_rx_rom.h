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
 * Description: Header file for hal_main.c.
 */

#ifndef HAL_RX_ROM_H
#define HAL_RX_ROM_H

#include "osal_types.h"
#include "hal_device_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

osal_void hh503_irq_drx_complete_isr(hal_device_stru *hal_device, osal_u8 data);
osal_void hh503_flush_rx_complete_irq(hal_device_stru *hal_dev, osal_u32 flush_mask);
osal_void hh503_irq_crx_complete_isr(hal_device_stru *hal_device, osal_u8 data);

oal_bool_enum_uint8 hal_is_hw_rx_queue_empty(const hal_to_dmac_device_stru *hal_device);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hal_rx */
