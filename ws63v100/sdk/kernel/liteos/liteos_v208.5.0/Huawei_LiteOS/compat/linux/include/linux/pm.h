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
 */
#ifndef _LINUX_PM_H
#define _LINUX_PM_H

#include "linux/list.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct pm_message {
    int event;
} dev_pm_message_t;

struct dev_pm_info {
    dev_pm_message_t power_state;
    LOS_DL_LIST      entry;
};

#define OS_EVENT_SUSPEND_FLAG        0x0002
#define OS_EVENT_RESUME_FLAG         0x0010

#define OS_SUSPEND_INIT    ((struct pm_message){ .event = OS_EVENT_SUSPEND_FLAG, })
#define OS_RESUME_INIT     ((struct pm_message){ .event = OS_EVENT_RESUME_FLAG, })

int dpm_suspend_start(dev_pm_message_t state);
void dpm_resume_end(dev_pm_message_t state);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

