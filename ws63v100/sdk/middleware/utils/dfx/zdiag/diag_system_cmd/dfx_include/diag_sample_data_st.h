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
 * Description: sample data st
 * This file should be changed only infrequently and with great care.
 */
#ifndef DIAG_SAMPLE_DATA_ST_H
#define DIAG_SAMPLE_DATA_ST_H
#include <stdint.h>

typedef struct {
    uint32_t flag;
    uint32_t transmit_id;
} diag_sample_data_cmd_t;

typedef struct {
    uint32_t ret;
    uint32_t flag;
    uint32_t transmit_id;
} diag_sample_data_ind_t;

typedef struct {
    uint32_t transmit_id;
    uint32_t ret;
    uint32_t offset;
    uint32_t size;
    uint32_t crc;
    uint8_t data[0];
} diag_sample_data_reply_pkt_t;

#endif