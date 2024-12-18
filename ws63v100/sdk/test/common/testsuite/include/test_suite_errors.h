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

#ifndef LIB_CMD_TEST_SUITE_ERRORS_H
#define LIB_CMD_TEST_SUITE_ERRORS_H

typedef enum {
    TEST_SUITE_OK = 0,
    TEST_SUITE_UNKNOWN_FUNCTION = -1,
    TEST_SUITE_ERROR_TIMED_OUT = -2,
    TEST_SUITE_ERROR_BAD_PARAMS = -3,
    TEST_SUITE_ERROR_PIN_ALLOCATION = -4,
    TEST_SUITE_ERROR_PIN_DEALLOCATION = -5,
    TEST_SUITE_ERROR_UNEXPECTED_INTERRUPT = -6,
    TEST_SUITE_ALLOC_FAILED = -7,
    TEST_SUITE_CONFIG_FAILED = -8,
    TEST_SUITE_TEST_FAILED = -254,
} test_suite_error_e;

#endif
