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
 * Description:  LOG MEMORY REGION SETUP MODULE INTERFACE.
 * Author:
 * Create:  2018-10-15
 */

#ifndef NON_OS_LOG_MEMORY_REGION_H
#define NON_OS_LOG_MEMORY_REGION_H

#include <stdbool.h>

/**
 * @addtogroup connectivity_drivers_non_os_log
 * @{
 */
/**
 * @brief  Initialize the log memory region and set the alias region for the other cores
 */
void log_memory_region_init(void);

/**
 * @brief  Initialize the mass data memory region and set the alias region for the other cores
 */
void massdata_memory_region_init(void);

/**
 * Used to check whether the log area is initialized.
*/
bool log_memory_is_init(void);
/**
 * @}
 */
#endif
