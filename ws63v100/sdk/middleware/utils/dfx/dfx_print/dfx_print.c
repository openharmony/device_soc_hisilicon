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
 * Description: dfx print
 */
#ifdef CONFIG_DFX_SUPPORT_PRINT
#include <stdarg.h>
#include "dfx_print.h"

static uint8_t g_print_level = DFX_PRINT_LEVEL_ERROR;
uint8_t dfx_print_get_level(void)
{
    return g_print_level;
}
void dfx_print_set_level(uint8_t level)
{
    if (level < DFX_PRINT_LEVEL_MAX) {
        g_print_level =  level;
    }
}

#endif