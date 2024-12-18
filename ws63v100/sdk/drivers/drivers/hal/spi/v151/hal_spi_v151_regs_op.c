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
 * Description: Provides V100 ssi register operation api \n
 *
 * History: \n
 * 2022-06-05, Create file. \n
 */
#include <stdint.h>
#include "common_def.h"
#include "hal_spi_v151_regs_op.h"

volatile uint32_t *hal_spi_v151_int_set_reg(spi_bus_t bus, spi_v151_int_reg_t reg)
{
    volatile uint32_t *reg_addr = NULL;
    switch (reg) {
        case SPI_INMAR_REG:
            reg_addr = &spis_v151_regs(bus)->spi_inmar;
            break;
        default:
            break;
    }
    return reg_addr;
}

volatile uint32_t *hal_spi_v151_int_get_reg(spi_bus_t bus, spi_v151_int_reg_t reg)
{
    volatile uint32_t *reg_addr = NULL;
    switch (reg) {
        case SPI_INMAR_REG:
            reg_addr = &spis_v151_regs(bus)->spi_inmar;
            break;
        case SPI_INSR_REG:
            reg_addr = &spis_v151_regs(bus)->spi_insr;
            break;
        case SPI_RAINSR_REG:
            reg_addr = &spis_v151_regs(bus)->spi_rainsr;
            break;
    }
    return reg_addr;
}
