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
 * Description:   HAL PMU CPU DRIVER.
 *
 * Create: 2020-01-15
 */

#include "hal_reg_config.h"
#include "soc_osal.h"
#include "systick.h"
#include "debug_print.h"
#include "panic.h"

#define CHECK_FREQUENCY                         1000

uint32_t g_pmu_check_reg = 0;
uint32_t g_pmu_check_reg_value = 0;
uint32_t g_pmu_check_reg_bit = 0;

void hal_reg_status_check_timeout(uint32_t addr, uint16_t offset, switch_type_t on, uint32_t timeout)
{
    uint64_t time = uapi_systick_get_ms();
    uint64_t check_frq = CHECK_FREQUENCY;

    g_pmu_check_reg = addr;
    g_pmu_check_reg_bit = offset;
    while (reg16_getbit((void *)((uintptr_t)addr), offset) != on) {
        check_frq--;
        if (check_frq == 0) {
            if ((time + timeout) < uapi_systick_get_ms()) {
                g_pmu_check_reg_value = readl((void *)((uintptr_t)g_pmu_check_reg));
                PRINT("pmu status timeout!! pmu addr is %x, with value %x\r\n", g_pmu_check_reg, g_pmu_check_reg_value);
                panic(PANIC_PMU_LDO, g_pmu_check_reg);
            } else {
                check_frq = CHECK_FREQUENCY;
            }
        }
    }

    g_pmu_check_reg = 0;
    g_pmu_check_reg_value = 0;
    g_pmu_check_reg_bit = 0;
}

void hal_reg_config_bit(uint32_t addr, switch_type_t on, REG16_POS position)
{
    unsigned int irq_sts = osal_irq_lock();
    if (on != 0) {
        reg16_setbit ((void *)((uintptr_t) addr), position);
    } else {
        reg16_clrbit ((void *)((uintptr_t) addr), position);
    }
    osal_irq_restore(irq_sts);
}
