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
 * Description: Provides HAL drv timer \n
 *
 * History: \n
 * 2022-06-02, Create file. \n
 */
#include "common_def.h"
#include "errcode.h"
#include "hal_drv_timer.h"

uintptr_t g_timer_comm_regs = NULL;
uintptr_t g_timer_regs[CONFIG_TIMER_MAX_NUM] = {NULL};

errcode_t hal_timer_regs_init(timer_index_t index)
{
    if (timer_porting_base_addr_get(index) == 0) {
        return ERRCODE_TIMER_INVALID_REG_ADDR;
    }

    g_timer_regs[index] = timer_porting_base_addr_get(index);
    g_timer_comm_regs = timer_porting_comm_addr_get();

    return ERRCODE_SUCC;
}

void hal_timer_regs_deinit(timer_index_t index)
{
    g_timer_regs[index] = NULL;
}