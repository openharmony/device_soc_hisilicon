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
 * Description:   LOG REG DUMP MODULE
 * Author:
 * Create: 2020-09-15
 */

#ifndef LOG_REG_DUMP_H
#define LOG_REG_DUMP_H

#include "core.h"

#if (defined (BUILD_APPLICATION_STANDARD) || defined (TEST_SUITE))
#if (USE_COMPRESS_LOG_INSTEAD_OF_SDT_LOG == NO)
/**
 * @brief  Dump register address config check.
 */
void log_exception_dump_reg_check(void);

/**
 * @brief  Dump register to sdt.
 */
void log_exception_dump_reg(void);
#endif
#endif
#endif

