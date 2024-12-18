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
 * Description: sample data adapt
 */

#ifndef SAMPLE_DATA_ADAPT_H
#define SAMPLE_DATA_ADAPT_H

#include "diag.h"
errcode_t diag_cmd_sample_data(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option);
void zdiag_adapt_sample_data_report(uint8_t *sdt_buf, uint16_t sdt_buf_size);
#endif