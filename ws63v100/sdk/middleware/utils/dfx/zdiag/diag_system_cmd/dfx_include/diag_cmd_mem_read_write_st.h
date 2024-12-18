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
 * Description: dfx mem read write st
 * This file should be changed only infrequently and with great care.
 */

#ifndef DIAG_CMD_MEM_READ_WRITE_ST_H
#define DIAG_CMD_MEM_READ_WRITE_ST_H

#include <stdint.h>

typedef struct {
    uintptr_t start_addr;
    uint32_t cnt;
} mem_read_cmd_t;

typedef struct {
    uintptr_t start_addr;
    uint32_t size;
} mem_read_ind_head_t;

typedef struct {
    mem_read_ind_head_t head;
    uint32_t data[16];
} mem_read32_ind_t;

typedef struct {
    mem_read_ind_head_t head;
    uint16_t data[32];
} mem_read16_ind_t;

typedef struct {
    mem_read_ind_head_t head;
    uint8_t data[64];
} mem_read8_ind_t;

typedef struct {
    uintptr_t start_addr;
    uint32_t val;
} mem_write_cmd_t;

typedef struct {
    uint32_t ret;
} mem_write_ind_t;
#endif