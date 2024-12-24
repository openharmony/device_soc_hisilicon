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
 * Description: Provides systick porting template \n
 *
 * History: \n
 * 2022-08-01， Create file. \n
 */
#include "hal_systick_ws63.h"
#include "systick_porting.h"
#include "platform_core.h"
#define HAL_SYSTICK_SPEED  32000

static uintptr_t const g_systick_base_addr =  (uintptr_t)SYSTICK_BASE_ADDR;

uintptr_t systick_porting_base_addr_get(void)
{
    return g_systick_base_addr;
}

void systick_port_cali_xclk(void)
{
}

static uint32_t g_systick_clock = HAL_SYSTICK_SPEED;

uint32_t systick_clock_get(void)
{
    return g_systick_clock;
}

void systick_clock_set(uint32_t clock)
{
    g_systick_clock = clock;
}