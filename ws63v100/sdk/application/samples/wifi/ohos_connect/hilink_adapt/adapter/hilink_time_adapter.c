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
 * Description: 时间适配层接口cmsis2实现源文件（此文件为DEMO，需集成方适配修改）
 */

#include "hilink_time_adapter.h"
#include "cmsis_os2.h"
#include "hilink_sal_defines.h"

#ifndef MS_PER_SECOND
#define MS_PER_SECOND   1000
#endif
#ifndef US_PER_MS
#define US_PER_MS       1000
#endif

int HILINK_GetOsTime(HiLinkTimeval *time)
{
    if (time == NULL) {
        HILINK_SAL_WARN("invalid param\r\n");
        return HILINK_SAL_PARAM_INVALID;
    }

    if (osKernelGetTickFreq() == 0) {
        HILINK_SAL_CRIT("invalid tick freq\r\n");
        return HILINK_SAL_PARAM_INVALID;
    }

    uint64_t ms = ((uint64_t)osKernelGetTickCount() * MS_PER_SECOND / osKernelGetTickFreq());
    time->sec = ms / MS_PER_SECOND;
    time->usec = ms % MS_PER_SECOND * US_PER_MS;

    return HILINK_SAL_OK;
}

int HILINK_GetUtcTime(HiLinkTimeval *time)
{
    (void)time;
    return HILINK_SAL_NOT_SUPPORT;
}