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
 * Description: Provides hal watchdog \n
 *
 * History: \n
 * 2022-07-26, Create file. \n
 */
#include <stdint.h>
#include <stdio.h>
#include "hal_watchdog.h"

uintptr_t g_watchdog_regs;
hal_watchdog_funcs_t *g_hal_watchdogs_funcs = NULL;

errcode_t hal_watchdog_register_funcs(hal_watchdog_funcs_t *funcs)
{
    if (funcs == NULL) {
        return ERRCODE_INVALID_PARAM;
    }
    g_hal_watchdogs_funcs = funcs;
    return ERRCODE_SUCC;
}

errcode_t hal_watchdog_unregister_funcs(void)
{
    g_hal_watchdogs_funcs = NULL;
    return ERRCODE_SUCC;
}

hal_watchdog_funcs_t *hal_watchdog_get_funcs(void)
{
    return g_hal_watchdogs_funcs;
}

errcode_t hal_watchdog_regs_init(void)
{
    if (g_watchdog_base_addr == 0) {
        return ERRCODE_WDT_REG_ADDR_INVALID;
    }

    g_watchdog_regs = g_watchdog_base_addr;
    return ERRCODE_SUCC;
}

void hal_watchdog_regs_deinit(void)
{
    g_watchdog_regs = 0;
}
