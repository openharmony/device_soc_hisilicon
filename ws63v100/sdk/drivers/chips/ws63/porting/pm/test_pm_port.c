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
 * Description: Test pm port source. \n
 *
 * History: \n
 * 2023-11-13, Create file. \n
 */

#include "pm_porting.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "test_pm_port.h"

static int test_pm_port_enter_low_power(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    pm_port_enter_lowpower();

    return TEST_SUITE_OK;
}

static int test_pm_port_exit_low_power(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    pm_port_exit_lowpower();

    return TEST_SUITE_OK;
}

void add_pm_port_test_case(void)
{
    uapi_test_suite_add_function("test_pm_port_enter_low_power", "Params: none", test_pm_port_enter_low_power);
    uapi_test_suite_add_function("test_pm_port_exit_low_power", "Params: none", test_pm_port_exit_low_power);
}
