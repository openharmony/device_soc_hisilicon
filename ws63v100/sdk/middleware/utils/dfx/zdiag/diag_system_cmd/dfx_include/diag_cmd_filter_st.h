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
 * Description: dfx filter st
 * This file should be changed only infrequently and with great care.
 */

#ifndef DIAG_CMD_FILTER_ST_H
#define DIAG_CMD_FILTER_ST_H

#include <stdint.h>

typedef struct {
    uint16_t module_id;     /* Module ID, when MSG is SYS or USR, mark the subcategories under the two categories */
    uint8_t is_dest_module; /* Is it the target module */
    uint8_t switch_code;    /* Turns the configuration on or off */
} diag_cmd_msg_cfg_req_stru_t;

#endif