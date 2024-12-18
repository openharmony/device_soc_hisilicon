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
 * Description: get cpup function
 */
#ifndef DIAG_CMD_GET_CPUP_H
#define DIAG_CMD_GET_CPUP_H

#include "errcode.h"
#include "diag.h"

#define CPUP_DIAG_REPORT_CNT    30
#define CPUP_MAX_IRQ_CNT        90

errcode_t diag_cmd_get_cpup(uint16_t cmd_id, uint8_t *cmd_param, uint16_t cmd_param_size, diag_option_t *option);
errcode_t diag_cmd_set_cpup_enable(uint16_t cmd_id, uint8_t *cmd_param, uint16_t cmd_param_size,
    diag_option_t *option);
#endif // !DIAG_CMD_GET_CPUP_H