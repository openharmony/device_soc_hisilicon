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
 * Description: flash
 *
 * Create: 2023-01-09
 */

#include "boot_errcode.h"
#include "securec.h"
#include "boot_flash.h"

static flash_cmd_func g_flash_cmd_funcs;

flash_cmd_func *boot_get_flash_funcs(void)
{
    return &g_flash_cmd_funcs;
}

uint32_t boot_regist_flash_cmd(const flash_cmd_func *funcs)
{
    if (memcpy_s(&g_flash_cmd_funcs, sizeof(flash_cmd_func), funcs, sizeof(flash_cmd_func)) != EOK) {
        return ERRCODE_MEMCPY;
    }
    return ERRCODE_SUCC;
}