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
 * Description: get cpup function transfer
 */
#ifndef DFX_CPUP_H
#define DFX_CPUP_H

#include <stdint.h>
#include "errcode.h"
#include "dfx_os_st.h"

uint32_t dfx_cpup_get_all_usage(uint16_t max_num, dfx_cpup_item_usage_info_t *cpup_info, uint32_t mode, uint16_t flag);

void dfx_cpup_reset(void);
void dfx_cpup_stop(void);
void dfx_cpup_start(void);
#endif