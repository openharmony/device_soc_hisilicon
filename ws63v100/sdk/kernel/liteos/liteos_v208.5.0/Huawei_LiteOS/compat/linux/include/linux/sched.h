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
#ifndef _LINUX_SCHED_H
#define _LINUX_SCHED_H

#include "linux/kernel.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define cond_resched() do {} while (0)
#define signal_pending(x) (0)

/**
 * @ingroup sched
 * @brief Delay a task.
 *
 * @par Description:
 * This API is used to delay the execution of the current task. The task is able to be scheduled
 * after that it is delayed for a specified number of Ticks.
 *
 * @attention
 * <ul>
 * <li>The task fails to be delayed if it is being delayed during interrupt processing or it is locked.</li>
 * </ul>
 *
 * @param  timeout [IN] Type #signed long, Number of Ticks for which the task is delayed(unit: Tick).
 *
 * @retval #0   The task is successfully delayed.
 * @retval #>0  The remaining ticks, it means that the task is unsuccessfully delayed.
 *
 * @par Dependency:
 * <ul><li>sched.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 */
signed long schedule_timeout(signed long timeout);

#define schedule_timeout_uninterruptible(t) schedule_timeout(t)

#define schedule_timeout_interruptible(t) schedule_timeout(t)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LINUX_SCHED_H */
