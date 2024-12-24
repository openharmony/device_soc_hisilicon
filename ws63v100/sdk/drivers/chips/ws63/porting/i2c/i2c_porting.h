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
 * Description: Provides i2c port template \n
 *
 * History: \n
 * 2022-08-15， Create file. \n
 */
#ifndef I2C_PORTING_H
#define I2C_PORTING_H

#include <stdint.h>
#include <stdbool.h>
#include "platform_core.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define I2C_BUS_MAX_NUM I2C_BUS_MAX_NUMBER

#define I2C_0_SCL_PIN_CTRL_REG 0x4400D048
#define I2C_0_SDA_PIN_CTRL_REG 0x4400D044
#define I2C_1_SCL_PIN_CTRL_REG 0x4400D040
#define I2C_1_SDA_PIN_CTRL_REG 0x4400D03C
#define PIN_CTRL_MODE_2 0x2

/**
 * @brief  Get the base address of a specified I2C.
 * @param  [in]  bus The bus index of I2C.
 * @return The base address of specified I2C.
 */
uintptr_t i2c_porting_base_addr_get(i2c_bus_t bus);

/**
 * @brief  Set the bus clock of specified i2c.
 * @param  [in]  bus The I2C bus. see @ref i2c_bus_t
 * @param  [in]  clock The bus clock of specified I2C.
 */
void i2c_port_set_clock_value(i2c_bus_t bus, uint32_t clock);

/**
 * @brief  Get the bus clock of specified i2c.
 * @param  [in]  bus The I2C bus. see @ref i2c_bus_t
 * @return The bus clock of specified I2C.
 */
uint32_t i2c_port_get_clock_value(i2c_bus_t bus);

/**
 * @brief  Register the interrupt of I2C.
 */
void i2c_port_register_irq(i2c_bus_t bus);

/**
 * @brief  Unregister the interrupt of I2C.
 */
void i2c_port_unregister_irq(i2c_bus_t bus);

/**
 * @brief  I2C lock.
 * @param [in]  bus The bus index of I2C.
 * @return The irq lock number of I2C.
 */
uint32_t i2c_porting_lock(i2c_bus_t bus);

/**
 * @brief  I2C unlock.
 * @param [in]  bus The bus index of I2C.
 * @param [in]  irq_sts The irq lock number of I2C.
 */
void i2c_porting_unlock(i2c_bus_t bus, uint32_t irq_sts);

#ifdef TEST_SUITE
/**
 * @brief  Init i2c pin for test.
 */
void i2c_port_test_i2c_init_pin(void);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
