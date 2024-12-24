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

#include <stdio.h>
#include <stdarg.h>
#include "securec.h"
#include "common_def.h"
#include "test_suite_channel.h"
#include "test_suite_log.h"

#define LOG_STRING_MAX_LENGTH 100

static test_suite_channel_funcs_t *g_test_suite_log_channel_funcs = NULL;
static int32_t g_test_suite_result;

void test_suite_log_get_channel_funcs(void)
{
    g_test_suite_log_channel_funcs = test_suite_channel_get_funcs();
}

void test_suite_log_char(char data)
{
    g_test_suite_log_channel_funcs->send_char(data);
}

void test_suite_log_string(const char *str)
{
    g_test_suite_log_channel_funcs->send(str);
}

void test_suite_log_stringf(const char *str, ...)
{
    static char s[LOG_STRING_MAX_LENGTH];
    va_list args;
    int32_t str_len;

    va_start(args, str);
    str_len = vsprintf_s(s, sizeof(s), str, args);
    va_end(args);

    if (str_len < 0) {
        return;
    }

    test_suite_log_string(s);
}

void test_suite_log_line(char *line)
{
    g_test_suite_log_channel_funcs->send_line(line);
}

void test_suite_log_set_test_result(int result)
{
    g_test_suite_result = result;
}

int test_suite_log_get_test_result(void)
{
    return g_test_suite_result;
}
