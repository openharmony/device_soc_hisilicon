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
 * Description: last dump
 * This file should be changed only infrequently and with great care.
 */

#ifndef LAST_DUMP_H
#define LAST_DUMP_H
#include <stdint.h>

void dfx_last_dump_start(uint32_t file_num);
void dfx_last_dump_data(const char *name, uintptr_t addr, uint32_t size);
void dfx_last_word_send(uint8_t *buf, uint32_t size);
void dfx_last_dump_end(uint32_t file_num);

#endif // !LAST_DUMP_H