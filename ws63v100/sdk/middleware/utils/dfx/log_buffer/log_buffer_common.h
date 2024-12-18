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
 * Description:  LOG BUFFER COMMON DEFINITIONS.
 * Author:
 * Create:  2018-10-15
 */

#ifndef NON_OS_LOG_BUFFER_COMMON_H
#define NON_OS_LOG_BUFFER_COMMON_H
#include "std_def.h"

/**
 * @addtogroup connectivity_drivers_non_os_log
 * @{
 */
/**
 * @brief  Log message information header.
 */
typedef struct {
    uint32_t time_us; /**< time when the logging was generated */
    uint16_t length;  /**< log message length */

#ifdef FEATURE_PLT_LB_CHECK
    uint16_t _magic;
#endif
} __attribute__((packed)) log_buffer_header_t;

#ifdef FEATURE_PLT_LB_CHECK
#define lb_magic_set(h)   (h)->_magic = 0x8F5C
#define lb_magic_check(h) (0x8F5C == (h)->_magic)
#else
#define lb_magic_set(h)
#define lb_magic_check(h) 1
#endif

/**
 * @}
 */
#endif
