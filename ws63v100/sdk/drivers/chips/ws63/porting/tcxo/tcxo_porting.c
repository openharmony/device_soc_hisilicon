/**
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
 *
 * Description: Provides tcxo port \n
 *
 * History: \n
 * 2022-08-16， Create file. \n
 */

#include "tcxo_porting.h"
#include "hal_tcxo.h"
#include "hal_tcxo_v150.h"
#include "platform_core.h"

static uintptr_t const g_tcxo_base_addr = (uintptr_t)TCXO_COUNT_BASE_ADDR;

uintptr_t tcxo_porting_base_addr_get(void)
{
    return g_tcxo_base_addr;
}

static uint32_t g_tcxo_ticks_per_usec = TCXO_TICKS_PER_U_SECOND;

uint32_t tcxo_porting_ticks_per_usec_get(void)
{
    return g_tcxo_ticks_per_usec;
}

void tcxo_porting_ticks_per_usec_set(uint32_t ticks)
{
    g_tcxo_ticks_per_usec = ticks;
}