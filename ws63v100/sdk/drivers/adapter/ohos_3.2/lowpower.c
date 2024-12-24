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
 * Description: Provides lowpower driver source \n
 *
 * History: \n
 * 2023-10-18， Create file. \n
 */
#include "common_def.h"
#include "iot_errno.h"
#include "lowpower.h"

unsigned int LpcInit(void)
{
    // uapi_pm_init uapi_clocks_init uapi_pm_vote_init已经在系统启动时适配
    return IOT_SUCCESS;
}

unsigned int LpcSetType(LpcType type)
{
    unused(type);
    // 低功耗睡眠模式是各业务投票决定，不能简单的设置睡眠状态
    // 睡眠投票接口：uapi_pm_add_sleep_veto
    // 睡眠去投票接口：uapi_pm_remove_sleep_veto
    return IOT_SUCCESS;
}