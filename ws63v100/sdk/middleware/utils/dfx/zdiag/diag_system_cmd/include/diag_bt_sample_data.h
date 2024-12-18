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
 * Description: sample data
 * This file should be changed only infrequently and with great care.
 */
#ifndef DIAG_SAMPLE_DATA_H
#define DIAG_SAMPLE_DATA_H
#include <stdint.h>
#include "errcode.h"

errcode_t diag_sample_data_report_start(uint32_t transmit_id);
errcode_t diag_sample_data_report_stop(uint32_t transmit_id);
errcode_t diag_sample_data_report(uint32_t transmit_id, uint8_t *buf, uint32_t size);
#endif