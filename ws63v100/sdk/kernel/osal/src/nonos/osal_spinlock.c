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

int osal_spin_lock_init(osal_spinlock *lock)
{
    osal_unused(lock);
    return 0;
}

void osal_spin_lock(osal_spinlock *lock)
{
    osal_unused(lock);
}

void osal_spin_unlock(osal_spinlock *lock)
{
    osal_unused(lock);
}
void osal_spin_lock_irqsave(osal_spinlock *lock, unsigned long *flags)
{
    osal_unused(lock, flags);
    (void)osal_irq_lock();
}

void osal_spin_unlock_irqrestore(osal_spinlock *lock, unsigned long *flags)
{
    osal_unused(lock, flags);
    osal_irq_unlock();
}

void osal_spin_lock_destroy(osal_spinlock *lock)
{
    osal_unused(lock);
}

