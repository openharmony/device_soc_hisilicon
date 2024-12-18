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
 * Description: diag romable
 * This file should be changed only infrequently and with great care.
 */
#ifndef __DIAG_ROM_API_H__
#define __DIAG_ROM_API_H__
#include "diag.h"
typedef errcode_t (*diag_report_sys_msg_handler)(uint32_t module_id, uint32_t msg_id, const uint8_t *buf,
    uint16_t buf_size, uint8_t level);

typedef struct {
    diag_report_sys_msg_handler report_sys_msg;
} diag_rom_api_t;

void diag_rom_api_register(const diag_rom_api_t *api);
#endif