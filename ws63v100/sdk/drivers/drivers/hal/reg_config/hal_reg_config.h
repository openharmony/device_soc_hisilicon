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
 * Description:   HAL PMU CPU DRIVER HEADER.
 *
 * Create: 2020-01-15
 */

#ifndef SRC_DRIVERS_HAL_REG_CONFIG_H
#define SRC_DRIVERS_HAL_REG_CONFIG_H

#include "chip_io.h"

/** @defgroup connectivity_drivers_hal_reg_config REG Config
  * @ingroup  connectivity_drivers_hal
  * @{
  */
/**
 * @brief  Check register value
 * @param  addr Register addr
 * @param  offset Register bit offset
 * @param  on Bit set/clear
 * @param  timeout Check timeout value
 * @return None
 */
void hal_reg_status_check_timeout(uint32_t addr, uint16_t offset, switch_type_t on, uint32_t timeout);

/**
 * @brief  Set register value
 * @param  addr Register addr
 * @param  position Register bit offset
 * @param  on Bit set/clear
 * @return None
 */
void hal_reg_config_bit(uint32_t addr, switch_type_t on, REG16_POS position);

/**
  * @}
  */
#endif
