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
 * Description: osal adapt atomic timer file.
 */
#include "osal_adapt.h"

int osal_adapt_timer_init(osal_timer *timer, void *func, unsigned long data, unsigned int interval)
{
    timer->timer    = NULL;
    timer->handler  = func;
    timer->data     = data;
    timer->interval = interval;
    return osal_timer_init(timer);
}

unsigned int osal_adapt_jiffies_to_msecs(const unsigned int n)
{
    return osal_jiffies_to_msecs(n);
}

int osal_adapt_timer_destroy(osal_timer *timer)
{
    return osal_timer_destroy(timer);
}

unsigned long long osal_adapt_get_jiffies(void)
{
    return osal_get_jiffies();
}

int osal_adapt_timer_mod(osal_timer *timer, unsigned int interval)
{
    return osal_timer_mod(timer, interval);
}
