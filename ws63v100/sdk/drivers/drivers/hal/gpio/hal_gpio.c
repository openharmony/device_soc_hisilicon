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
 * Description: Provides HAL gpio \n
 *
 * History: \n
 * 2022-07-26, Create file. \n
 */

#include <stdint.h>
#include "hal_gpio.h"

uintptr_t g_gpios_regs[GPIO_CHANNEL_MAX_NUM] = { 0 };

uintptr_t hal_gpio_base_addrs_get(uint32_t i)
{
    return gpio_porting_base_addr_get((gpio_channel_t)i);
}

void hal_gpio_regs_init(void)
{
    for (uint32_t i = 0; i < GPIO_CHANNEL_MAX_NUM; i++) {
        g_gpios_regs[i] = hal_gpio_base_addrs_get(i);
    }
}