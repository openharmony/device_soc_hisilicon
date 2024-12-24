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
 * Description: osal atomic source file.
 */

#include "soc_osal.h"
#include "osal_inner.h"

void osal_atomic_set(osal_atomic *atomic, int i)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    unsigned int irq = osal_irq_lock();
    atomic->counter = i;
    osal_irq_restore(irq);
}

int osal_atomic_inc_return(osal_atomic *atomic)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    unsigned int irq = osal_irq_lock();
    atomic->counter += 1;
    osal_irq_restore(irq);
    return atomic->counter;
}

int osal_atomic_read(osal_atomic *atomic)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    int value;
    unsigned int irq = osal_irq_lock();
    value = atomic->counter;
    osal_irq_restore(irq);
    return value;
}

void osal_atomic_inc(osal_atomic *atomic)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    unsigned int irq = osal_irq_lock();
    atomic->counter += 1;
    osal_irq_restore(irq);
}

void osal_atomic_dec(osal_atomic *atomic)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    unsigned int irq = osal_irq_lock();
    atomic->counter -= 1;
    osal_irq_restore(irq);
}

void osal_atomic_add(osal_atomic *atomic, int count)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    unsigned int irq = osal_irq_lock();
    atomic->counter += count;
    osal_irq_restore(irq);
}
