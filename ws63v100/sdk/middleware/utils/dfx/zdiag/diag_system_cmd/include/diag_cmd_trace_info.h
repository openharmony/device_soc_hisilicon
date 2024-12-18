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
 * Description:  Header file containing struct prototypes of trace info.
 * Create:       2023-01
 */
#ifndef __TRACE_INFO_H__
#define __TRACE_INFO_H__

#include "errcode.h"
#include "diag.h"

#define CYCLE_PER_US 64
#define CYCLE_HIGH_BITS 32

typedef enum {
    TRACE_INIT,
    TRACE_START,
    TRACE_STOP,
} trace_status_t;

void trace_send_data(uint16_t len, uint8_t *data);
errcode_t diag_cmd_trace_start(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option);
errcode_t diag_cmd_trace_stop(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option);
errcode_t diag_cmd_get_time_info(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size,
                                 diag_option_t *option);
errcode_t diag_cmd_set_time_info(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option);

#endif
