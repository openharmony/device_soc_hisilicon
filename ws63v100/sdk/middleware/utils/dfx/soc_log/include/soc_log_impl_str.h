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
 * Description: dfx soc log impl str
 * This file should be changed only infrequently and with great care.
 */

#ifndef SOC_LOG_IMPL_STR_H
#define SOC_LOG_IMPL_STR_H
#include <stdint.h>
#include "errcode.h"
#include "dfx_write_interface.h"

typedef int32_t (*dfx_soc_log_output_handler)(const uint8_t *data,  uint32_t len);

errcode_t dfx_serial_get_write_data_impl(dfx_write_data_interface_t *impl);
void dfx_register_soc_log_output_handler(dfx_soc_log_output_handler handler);

#endif
