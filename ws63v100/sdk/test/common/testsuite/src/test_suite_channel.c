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

#include "common_def.h"
#include "test_suite_channel.h"

static test_suite_channel_funcs_t *g_test_suite_channel_funcs = NULL;

static void test_suite_channel_register_funcs(test_suite_channel_funcs_t *funcs)
{
    if (funcs == NULL) {
        return;
    }
    g_test_suite_channel_funcs = funcs;
}

static void test_suite_channel_unregister_funcs(void)
{
    g_test_suite_channel_funcs = NULL;
}

void test_suite_channel_init(test_suite_channel_funcs_t *funcs)
{
    test_suite_channel_register_funcs(funcs);
    if (g_test_suite_channel_funcs == NULL) {
        return;
    }
    g_test_suite_channel_funcs->init();
}

void test_suite_channel_deinit(void)
{
    if (g_test_suite_channel_funcs == NULL) {
        return;
    }
    g_test_suite_channel_funcs->deinit();
    test_suite_channel_unregister_funcs();
}

test_suite_channel_funcs_t *test_suite_channel_get_funcs(void)
{
    return g_test_suite_channel_funcs;
}
