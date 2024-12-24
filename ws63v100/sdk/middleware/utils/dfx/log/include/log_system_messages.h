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
 * Description:   system log messages
 * Author:
 * Create:
 */

#ifndef LOG_SYSTEM_MESSAGES_H
#define LOG_SYSTEM_MESSAGES_H

#include "log_common.h"

/**
 * @addtogroup connectivity_libs_log
 * @{
 */
typedef struct {
    uint32_t counter;  // !< Number of missed messages.
} log_system_missed_message_t;

typedef enum {
    LOG_SYSTEM_MISSED_MESSAGE_BUFFER,  // missed messages due to shared memeory being full
} log_system_message_id_t;

typedef enum {
    LOG_SYSTEM_MODULE_LOG,  // logging
} log_system_module_t;

/**
 * @}
 */
#endif