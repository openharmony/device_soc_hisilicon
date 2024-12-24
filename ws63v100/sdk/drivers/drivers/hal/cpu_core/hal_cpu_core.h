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
 * Description:  HAL CPU CORE HEADER
 *
 * Create: 2018-10-15
 */

#ifndef HAL_CPU_CORE_H
#define HAL_CPU_CORE_H

#include <stdbool.h>

/** @defgroup connectivity_drivers_hal_cpu_core CPU Core
  * @ingroup  connectivity_drivers_hal
  * @{
  */

/**
 * @brief  Is bt core enabled
 * @return True if enabled, false otherwise
 */
bool hal_cpu_is_bt_enabled(void);
/**
  * @}
  */
#endif
