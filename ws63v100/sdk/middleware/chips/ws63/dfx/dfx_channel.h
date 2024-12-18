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
 * Description: dfx uart channel
 * This file should be changed only infrequently and with great care.
 */
#ifndef DFX_CHANNEL_H
#define DFX_CHANNEL_H

#include <stdint.h>
#include "errcode.h"

void diag_uart_rx_proc(uint8_t *buffer, uint16_t length);
errcode_t diag_register_channel(void);
#endif