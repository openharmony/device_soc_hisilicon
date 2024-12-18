/*
 *   Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: OS Abstract Layer.
 */

#include "soc_osal.h"
#include "osal_inner.h"

int osal_wait_init(osal_wait *wait)
{
    osal_unused(wait);
    return OSAL_NONEOS_DEFAULT_RET;
}

int osal_wait_timeout_interruptible(osal_wait *wait, osal_wait_condition_func func, const void *param, unsigned long ms)
{
    osal_unused(wait, func, param, ms);
    return OSAL_NONEOS_DEFAULT_RET;
}

void osal_wait_destroy(osal_wait *wait)
{
    osal_unused(wait);
}

void osal_wait_wakeup(osal_wait *wait)
{
    osal_unused(wait);
}
