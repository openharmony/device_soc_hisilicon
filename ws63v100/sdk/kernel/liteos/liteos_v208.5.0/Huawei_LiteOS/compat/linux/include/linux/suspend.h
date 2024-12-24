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
#ifndef _LINUX_SUSPEND_H
#define _LINUX_SUSPEND_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef int suspend_state_t;

#define PM_SUSPEND_ON           ((suspend_state_t)0)
#define PM_SUSPEND_FREEZE       ((suspend_state_t)1)
#define PM_SUSPEND_STANDBY      ((suspend_state_t)2)
#define PM_SUSPEND_MEM          ((suspend_state_t)3)
#define PM_SUSPEND_MIN          PM_SUSPEND_FREEZE
#define PM_SUSPEND_MAX          ((suspend_state_t)4)

int pm_suspend(suspend_state_t state);
void pm_resume(void);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#endif
