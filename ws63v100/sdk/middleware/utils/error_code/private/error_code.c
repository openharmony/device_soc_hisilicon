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
 * Description:  error_code_e module implementation
 * Author:
 * Create: 2018-10-15
 */

#include "error_code.h"
#include "non_os.h"

static uint32_t g_error_code = 0;

void error_code_reset(void)
{
    g_error_code = 0;
}

void error_code_set(error_code_e id)
{
    non_os_enter_critical();
    g_error_code |= BIT(id);
    non_os_exit_critical();
}

void error_code_clear(error_code_e id)
{
    non_os_enter_critical();
    g_error_code &= (~BIT(id));
    non_os_exit_critical();
}

uint32_t error_code_get(void)
{
    return g_error_code;
}
