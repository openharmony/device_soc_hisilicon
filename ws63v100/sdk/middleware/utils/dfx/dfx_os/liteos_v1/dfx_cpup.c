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
 * Description: dfx cpup
 * This file should be changed only infrequently and with great care.
 */

#include "dfx_cpup.h"
#include "errcode.h"
#include "dfx_os_st.h"
#include "common_def.h"
#include "los_cpup.h"

uint32_t dfx_cpup_get_all_usage(uint16_t max_num, dfx_cpup_item_usage_info_t *cpup_info, uint32_t mode, uint16_t flag)
{
#ifdef LOSCFG_KERNEL_CPUP
    return LOS_AllCpuUsage(max_num, (CPUP_INFO_S *)cpup_info, mode, flag);
#else
    unused(max_num);
    unused(cpup_info);
    unused(mode);
    unused(flag);
    return (uint32_t)-1;
#endif
}

void dfx_cpup_reset(void)
{
#ifdef LOSCFG_KERNEL_CPUP
    LOS_CpupReset();
#endif
}

void dfx_cpup_stop(void)
{
#ifdef LOSCFG_KERNEL_CPUP
    LOS_CpupStop();
#endif
}

void dfx_cpup_start(void)
{
#ifdef LOSCFG_KERNEL_CPUP
    LOS_CpupStart();
#endif
}