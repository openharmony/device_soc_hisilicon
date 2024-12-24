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
/**
 * @defgroup clock
 * @ingroup linux
 */
#ifndef _LINUX_CLOCK_H
#define _LINUX_CLOCK_H

#include "los_typedef.h"

/**
 * @ingroup clock
 * @brief get the system time(ns)
 *
 * @par Description:
 * This API is used to get the system time by read the value of system timer.
 * @attention
 * <ul>
 * <li>system should init the system timer first.</li>
 * </ul>
 *
 * @param None
 *
 * @retval unsigned long long   the system time(ns).
 * @par Dependency:
 * <ul><li>clock.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V200R003C00
 */
extern unsigned long long sched_clock(void);

#endif
