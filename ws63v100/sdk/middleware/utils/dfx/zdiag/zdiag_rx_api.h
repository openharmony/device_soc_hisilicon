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
 * Description: diag rx
 * This file should be changed only infrequently and with great care.
 */
#ifndef __ZDIAG_RX_API_H__
#define __ZDIAG_RX_API_H__
#include "diag_common.h"
int32_t soc_diag_channel_rx_mux_char_data(diag_channel_id_t id, uint8_t *data, uint16_t size);
#endif
