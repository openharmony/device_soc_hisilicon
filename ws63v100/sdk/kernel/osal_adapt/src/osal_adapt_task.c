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
 * Description: osal adapt task source file.
 */
#include "osal_adapt.h"


osal_task *osal_adapt_kthread_create(osal_kthread_handler thread, void *data,
    const char *name, unsigned int stack_size)
{
    return osal_kthread_create(thread, data, name, stack_size);
}

void osal_adapt_kthread_lock(void)
{
    osal_kthread_lock();
}

void osal_adapt_kthread_unlock(void)
{
    osal_kthread_unlock();
}

long osal_adapt_get_current_tid(void)
{
    return osal_get_current_tid();
}

int osal_adapt_kthread_should_stop(void)
{
    return osal_kthread_should_stop();
}

void osal_adapt_kthread_destroy(osal_task *task, unsigned int stop_flag)
{
    osal_kthread_destroy(task, stop_flag);
}

int osal_adapt_kthread_set_priority(osal_task *task, unsigned int priority)
{
    return osal_kthread_set_priority(task, priority);
}

int osal_adapt_workqueue_init(osal_workqueue *work, osal_workqueue_handler handler)
{
    return osal_workqueue_init(work, handler);
}

void osal_adapt_workqueue_destroy(osal_workqueue *work)
{
    osal_workqueue_destroy(work);
}

unsigned int osal_adapt_irq_lock(void)
{
    return osal_irq_lock();
}

void osal_adapt_irq_restore(unsigned int irq_status)
{
    osal_irq_restore(irq_status);
}

void osal_adapt_wait_destroy(osal_wait *wait)
{
    osal_wait_destroy(wait);
}

void osal_adapt_wait_wakeup(osal_wait *wait)
{
    osal_wait_wakeup(wait);
}