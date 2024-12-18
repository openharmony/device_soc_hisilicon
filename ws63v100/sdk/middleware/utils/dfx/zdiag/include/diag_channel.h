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
 * Description: diag channel adapt api header file
 * This file should be changed only infrequently and with great care.
 */
#ifndef __SOC_ZDIAG_CHANNEL_H__
#define __SOC_ZDIAG_CHANNEL_H__

#include <stdint.h>
#include <stdbool.h>
#include "errcode.h"
#include "diag_config.h"
#include "diag_common.h"
#include "dfx_write_interface.h"

typedef enum {
    SOC_DIAG_CHANNEL_ATTR_NEED_RX_BUF = 0x1,
} soc_diag_channel_attribute_t;

typedef int32_t (*diag_channel_tx_hook)(void *fd, dfx_data_type_t data_type, uint8_t *data[],
                                        uint16_t len[], uint8_t cnt);
typedef errcode_t (*diag_channel_notify_hook)(uint32_t id, uint32_t data);

errcode_t soc_diag_channel_init(diag_channel_id_t id, uint32_t attribute);

errcode_t soc_diag_channel_set_tx_hook(diag_channel_id_t id, diag_channel_tx_hook hook);

errcode_t soc_diag_channel_set_notify_hook(diag_channel_id_t id, diag_channel_notify_hook hook);

errcode_t soc_diag_channel_rx_mux_data(diag_channel_id_t id, uint8_t *data, uint16_t size);

int32_t soc_diag_channel_rx_mux_char_data(diag_channel_id_t id, uint8_t *data, uint16_t size);

errcode_t soc_diag_channel_set_connect_hso_addr(diag_channel_id_t id, uint8_t hso_addr);
#endif
