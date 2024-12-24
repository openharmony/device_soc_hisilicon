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
 * Description: dfx soc log strategy
 * This file should be changed only infrequently and with great care.
 */

#ifndef SOC_LOG_STRATEGY_H
#define SOC_LOG_STRATEGY_H
#include "dfx_write_interface.h"
#include "errcode.h"
#define SOC_LOG_DEFAULT_LEVEL SOC_TRACE_LEVEL_INFO
#define SOC_LOG_INVALID_LINE 0xFFFFFFFF
#define SOC_LOG_DEFAULT_MODULE_ID 0xFFFF

typedef struct {
    uint8_t type;
    uint8_t level;
    uint16_t module_id;
    const char *fn_name;
    uint32_t line_num;
} soc_log_param_t;

typedef enum {
    SOC_LOG_TYPE_INVALID,
    SOC_LOG_TYPE_PRINT,
    SOC_LOG_TYPE_SIMPLE_PRINT,
    SOC_LOG_TYPE_KEY_TRACE,
} soc_log_type_t;

errcode_t soc_log_register_write_impl(dfx_write_data_interface_t *impl);
#endif // !SOC_LOG_STRATEGY_H
