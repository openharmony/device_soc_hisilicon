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
 * Description: Implementation of the Time Adaptation Layer Interface. \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */

#include "app_call.h"
#include "hilink_time_adapter.h"

int HILINK_GetOsTime(HiLinkTimeval *time)
{
    return app_call1(APP_CALL_HILINK_GET_OS_TIME, int, HiLinkTimeval *, time);
}

int HILINK_GetUtcTime(HiLinkTimeval *time)
{
    return app_call1(APP_CALL_HILINK_GET_UTC_TIME, int, HiLinkTimeval *, time);
}