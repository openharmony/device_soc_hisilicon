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

#ifndef HAL_ONE_PKT_ROM_H
#define HAL_ONE_PKT_ROM_H

#include "osal_types.h"
#include "hal_device_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

osal_void hh503_irq_one_pkt_done_isr(hal_device_stru *hal_device, osal_u8 data);
typedef osal_u32 (*hal_wait_one_packet_done_cb)(osal_u16 timeout);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __HAL_ONE_PKT_H__ */

