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
 * Description: UPG alloc memory functions source file
 */

#include <stddef.h>
#include <stdint.h>
#include "common_def.h"
#include "upg_common.h"
#include "upg_porting.h"

#include "upg_alloc.h"

STATIC bool upg_is_flash_addr(const void *addr)
{
    uintptr_t start_addr = upg_get_flash_base_addr();
    uintptr_t end_addr = start_addr + upg_get_flash_size();
    if (start_addr <= (uintptr_t)(addr) && (uintptr_t)(addr) < end_addr) {
        return true;
    }
    return false;
}

void *upg_malloc(uint32_t size)
{
    if (upg_get_func_list()->malloc != NULL) {
        return upg_get_func_list()->malloc(size);
    }
    return NULL;
}

void upg_free(void *addr)
{
    if (addr == NULL || upg_is_flash_addr(addr)) { /* 使用总线方式直接读取时，flash地址不需要释放 */
        return;
    }
    if (upg_get_func_list()->free != NULL) {
        return upg_get_func_list()->free(addr);
    }
    return;
}
