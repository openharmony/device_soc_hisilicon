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
 * Description: dfx connect st
 * This file should be changed only infrequently and with great care.
 */

#ifndef DIAG_CMD_CONNECT_ST_H
#define DIAG_CMD_CONNECT_ST_H
#include <stdint.h>

#define IMEI_SIZE 16
typedef struct {
    uint32_t major_minor_version;
    uint32_t revision_version;
    uint32_t build_version;
} ue_soft_version_t;
typedef struct {
    uint16_t version_v;
    uint16_t version_r;
    uint16_t version_c;
    uint16_t version_b;
    uint16_t version_spc;
    uint16_t reserved[3]; /* pad 3 item */
} ue_product_ver_t;

typedef struct {
    uint32_t rc; /* result code */
    char imei[IMEI_SIZE];
    ue_soft_version_t ue_soft_version;
    ue_product_ver_t build_version;
} diag_cmd_host_connect_cnf_stru_t;

#endif