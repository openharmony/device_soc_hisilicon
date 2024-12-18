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
 * Description: Provide HAL drv rtc \n
 *
 * History: \n
 * 2023-03-02, Create file. \n
 */
#include "common_def.h"
#include "errcode.h"
#include "hal_rtc.h"

uintptr_t g_rtc_comm_regs = NULL;
uintptr_t g_rtc_regs[CONFIG_RTC_MAX_NUM] = {NULL};

errcode_t hal_rtc_regs_init(rtc_index_t index)
{
    if (unlikely(rtc_porting_base_addr_get(index) == 0)) {
        return ERRCODE_RTC_REG_ADDR_INVALID;
    }

    g_rtc_regs[index] = rtc_porting_base_addr_get(index);
    g_rtc_comm_regs = rtc_porting_comm_addr_get();

    return ERRCODE_SUCC;
}

void hal_rtc_regs_deinit(rtc_index_t index)
{
    g_rtc_regs[index] = NULL;
}