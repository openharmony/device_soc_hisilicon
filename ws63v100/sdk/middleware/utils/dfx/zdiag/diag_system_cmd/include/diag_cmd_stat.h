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
 * Description: diag stat query cmd.
 */

#ifndef DIAG_CMD_STAT_H
#define DIAG_CMD_STAT_H

#include "errcode.h"
#include "diag.h"

#define DFX_STAT_ID_BASE_SYS    0x0557 /* SYS STAT packet ID range: [0xA000, 0xF000) */
#define DFX_STAT_ID_MAX_SYS     0x055E


typedef struct {
    uint32_t id;
} diag_dbg_stat_query_t;

errcode_t diag_cmd_stat_query(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option);

#endif
