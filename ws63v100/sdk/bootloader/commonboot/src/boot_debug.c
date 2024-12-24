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
 * Description: debug
 *
 * Create: 2023-01-09
 */

#include "boot_debug.h"
#include "boot_def.h"
#include "boot_init.h"

void set_boot_status(hi_boot_start_status status)
{
    uapi_reg_write16(STATUS_DEBUG_REG_ADDR, status);
}

uint16_t get_boot_status(void)
{
    return uapi_reg_read_val16(STATUS_DEBUG_REG_ADDR);
}