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
 * Description: Provides HAL SPI \n
 *
 * History: \n
 * 2022-08-12, Create file. \n
 */
#include "common_def.h"
#include "hal_spi.h"

uintptr_t g_hal_spis_regs[SPI_BUS_MAX_NUM] = { 0 };

errcode_t hal_spi_regs_init(void)
{
    for (int32_t i = 0; i < SPI_BUS_MAX_NUM; i++) {
        if (spi_porting_base_addr_get(i) == 0) {
            return ERRCODE_UART_REG_ADDR_INVALID;
        }
        g_hal_spis_regs[i] = spi_porting_base_addr_get(i);
    }
    return ERRCODE_SUCC;
}