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
 * Description: Provides watchdog port template \n
 *
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include "hal_watchdog_v151.h"
#include "watchdog_porting.h"
#include "watchdog.h"
#include "tcxo.h"
#include "platform_core_rom.h"
#include "chip_io.h"
#include "soc_osal.h"

uintptr_t g_watchdog_base_addr = (uintptr_t)CHIP_WDT_BASE_ADDRESS;
static uint32_t g_watchdog_clock = CONFIG_WDT_CLOCK;

void watchdog_port_register_hal_funcs(void)
{
    hal_watchdog_register_funcs(hal_watchdog_v151_funcs_get());
}

void watchdog_port_unregister_hal_funcs(void)
{
    hal_watchdog_unregister_funcs();
}

void watchdog_port_register_irq(void)
{
    return;
}

void watchdog_turnon_clk(void)
{
    return;
}

void watchdog_turnoff_clk(void)
{
    return;
}

void watchdog_port_set_clock(uint32_t wdt_clock)
{
    uint32_t irq_sts = osal_irq_lock();
    g_watchdog_clock = wdt_clock;
    osal_irq_restore(irq_sts);
}

uint32_t watchdog_port_get_clock(void)
{
    return g_watchdog_clock;
}

void irq_wdt_handler(void)
{
    hal_watchdog_v151_irq_handler(0);
}