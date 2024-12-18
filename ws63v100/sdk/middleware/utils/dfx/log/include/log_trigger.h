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
 * Description: LOG TRIGGER MODULE INTERRFACE
 * Author:
 * Create:
 */

#ifndef SRC_LIB_LOG_PRIVATE_LOG_TRIGGER_H
#define SRC_LIB_LOG_PRIVATE_LOG_TRIGGER_H

/**
 * @addtogroup connectivity_libs_log
 * @{
 */
#include "core.h"
#define CORE_LOGGING APPS
typedef void (*log_trigger_callback_t)(void);

/**
 * @brief  Ensure the log reader will be triggered.
 * This function should be called when the logger has detected it has written to an empty buffer.
 */
void log_trigger(void);

/**
 * @brief  Ensure the log trigger should be registered.
 * This function should be called when the log module init.
 * @param  callback: should match the type<log_trigger_callback_t>.
 */
void register_log_trigger(log_trigger_callback_t callback);

/**
 * @brief  trigger ipc to inform A core to save data from share mem in flash
 */
#if (BTH_WITH_SMART_WEAR == NO)
#if (CORE == BT)
void massdata_trigger(void *pay_i, uint8_t core, uint8_t type);
#endif
#endif

/**
 * @}
 */
#endif
