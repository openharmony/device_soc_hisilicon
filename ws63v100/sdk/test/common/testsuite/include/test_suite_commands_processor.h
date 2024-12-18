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
 */

#ifndef TEST_SUITE_COMMANDS_PROCESSOR_H
#define TEST_SUITE_COMMANDS_PROCESSOR_H

#include <stdint.h>
#include "common_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup test_common_testsuite_cmd_processor Commands Processor
 * @ingroup  test_common_testsuite
 * @{
 */

/**
 * @if Eng
 * @brief  Prase test suite commands.
 * @param  [in]  commands user input commands over char process.
 * @else
 * @brief  命令解析。
 * @param  [in]  commands 经过单字节处理之后的输入命令集。
 * @endif
 */
void test_suite_commands_prase(char *commands);

/**
 * @if Eng
 * @brief  Timer callback function.
 * @param  [in]  data timer id.
 * @else
 * @brief  定时器回调函数。
 * @param  [in]  data 定时器id.
 * @endif
 */
void test_suite_commands_timeout_callback(uintptr_t data);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
