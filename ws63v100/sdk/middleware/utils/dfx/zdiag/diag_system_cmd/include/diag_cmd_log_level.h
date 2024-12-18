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
 * Description: diag set log lvl
 */

#ifndef DIAG_CMD_SET_LOG_LEVEL_H
#define DIAG_CMD_SET_LOG_LEVEL_H

#include "errcode.h"
#include "diag.h"

typedef struct {
    uint32_t core;
    uint32_t log_lvl;
} diag_log_lvl_data_t;

errcode_t diag_cmd_set_log_level(uint16_t cmd_id, uint8_t *cmd_param, uint16_t cmd_param_size,
    diag_option_t *option);
#endif // !DIAG_CMD_SET_LOG_LEVEL_H