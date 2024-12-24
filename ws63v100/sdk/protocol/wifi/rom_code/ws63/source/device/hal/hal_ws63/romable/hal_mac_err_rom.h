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
 * Description: Header file for hal_mac_err_rom.c.
 */

#ifndef HAL_MAC_ERR_ROM_H
#define HAL_MAC_ERR_ROM_H

#include "osal_types.h"
#include "hal_device_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef osal_void (*hh503_cb_irq_mac_err)(hal_device_stru *hal_device, hal_error_state_stru *error_state);

osal_void hh503_dump_rx_vector_mac_err(osal_void);
osal_void hh503_dump_tx_vector_mac_err(osal_void);
osal_void hh503_irq_mac_err_cb(hal_device_stru *hal_device, hal_error_state_stru *error_state);
osal_void hh503_clear_mac_error_int_status(const hal_error_state_stru *status);
osal_void hal_mac_error_msg_report(hal_to_dmac_device_stru *hal_device,
    hal_mac_error_type_enum_uint8 error_type);

osal_void hh503_irq_mac_error_isr(hal_device_stru *hal_dev, osal_u8 data);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hal_mac_err_rom.h */

