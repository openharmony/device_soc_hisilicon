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
 * Description:  calculate mips.
 *
 * Create:  2018-10-15
 */
#include "hal_mips.h"
#include "stdlib.h"

hal_exception_dump_mips_callback g_exception_mips_start_callback = NULL;
hal_exception_dump_mips_callback g_exception_mips_stop_callback = NULL;

/* register mips callback function */
void hal_register_mips_start_callback(hal_exception_dump_mips_callback callback)
{
    if (callback != NULL) {
        g_exception_mips_start_callback = callback;
    }
}

/* register mips callback function */
void hal_register_mips_stop_callback(hal_exception_dump_mips_callback callback)
{
    if (callback != NULL) {
        g_exception_mips_stop_callback = callback;
    }
}

/* start calculating ticks  */
void hal_calculate_mips_start(void)
{
    if (g_exception_mips_start_callback != NULL) {
        g_exception_mips_start_callback();
    }
}

/* stop calculating ticks  */
void hal_calculate_mips_stop(void)
{
    if (g_exception_mips_stop_callback != NULL) {
        g_exception_mips_stop_callback();
    }
}
