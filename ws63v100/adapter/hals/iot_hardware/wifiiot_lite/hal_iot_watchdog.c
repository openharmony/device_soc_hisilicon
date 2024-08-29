/*
# Copyright (C) 2024 HiHope Open Source Organization .
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
*/
#include "watchdog.h"
#include "iot_watchdog.h"

#define WATCHDOG_TRIGGER_MODE     WDT_MODE_INTERRUPT
#define WATCHDOG_TIMEOUT          8/*超时时间8s*/

void IoTWatchDogEnable(void)
{
    uapi_watchdog_init(WATCHDOG_TIMEOUT);
    uapi_watchdog_enable(WATCHDOG_TRIGGER_MODE);
}

void IoTWatchDogKick(void)
{
    uapi_watchdog_kick();
}

void IoTWatchDogDisable(void)
{
    uapi_watchdog_disable();
}
