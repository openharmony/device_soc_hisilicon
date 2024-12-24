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
 * limitations under the License. \n
 *
 * Description: Provides AT report source \n
 */

#include "at_product.h"
#include "at_channel.h"
#include "at_process.h"
#include "at_base.h"

void uapi_at_report(const char* str)
{
    if (str == NULL) {
        return;
    }

    uint16_t channel_id = at_proc_get_current_channel_id();
    at_write_func_t func = at_channel_get_write_func(channel_id);
    if (func != NULL) {
        func((char*)str);
    }

    at_log_normal(str, strlen(str), 0);
}

void uapi_at_report_to_single_channel(at_channel_id_t channel_id, const char* str)
{
    if (str == NULL) {
        return;
    }

    at_write_func_t func = at_channel_get_write_func(channel_id);
    if (func != NULL) {
        func((char*)str);
    }

    at_log_normal(str, strlen(str), 0);
}
