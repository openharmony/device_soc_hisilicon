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

#include <stdint.h>
#include "common_def.h"
#include "test_suite_channel.h"
#include "test_suite_console.h"
#include "test_suite_log.h"
#include "test_suite_task.h"
#include "test_suite_functions_processor.h"

#if defined(CONFIG_TESTSUITE_IPC)
#include "test_suite_ipc.h"
#elif defined(CONFIG_TESTSUITE_UART)
#include "test_suite_uart.h"
#endif
#include "test_suite.h"

void uapi_test_suite_init(void)
{
#if defined(CONFIG_TESTSUITE_IPC)
    test_suite_channel_init(test_suite_ipc_funcs_get());
#elif defined(CONFIG_TESTSUITE_UART)
    test_suite_channel_init(test_suite_uart_funcs_get());
#endif
    test_suite_console_init();
    test_suite_log_get_channel_funcs();
    test_suite_function_init();
    test_suite_task_init();
}

void uapi_test_suite_deinit(void)
{
    test_suite_console_disable();
    test_suite_channel_deinit();
    test_suite_function_deinit();
    test_suite_task_destroy();
}

errcode_t uapi_test_suite_add_function(char *name, char *description, test_suite_function_callback_t func)
{
    return test_suite_function_add_func(name, description, func);
}
