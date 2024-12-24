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
 * Description: HiLink log management \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */

#include "hilink_log_manage.h"
#include "hilink_call.h"

#include <stdio.h>

void HILINK_SetLogLevel(HiLinkLogLevel level)
{
    hilink_call1(HILINK_CALL_HILINK_SET_LOG_LEVEL, void, HiLinkLogLevel, level);
}

HiLinkLogLevel HILINK_GetLogLevel(void)
{
    return hilink_call0(HILINK_CALL_HILINK_GET_LOG_LEVEL, HiLinkLogLevel);
}