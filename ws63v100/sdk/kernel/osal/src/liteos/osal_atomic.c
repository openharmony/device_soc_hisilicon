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
 * Description: atomic
 *
 * Create: 2021-12-16
 */

#include <los_memory.h>
#include "soc_osal.h"
#include "osal_errno.h"
#include "los_atomic.h"
#include "securec.h"
#include "osal_inner.h"

int osal_atomic_read(osal_atomic *atomic)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    return LOS_AtomicRead((Atomic *)&(atomic->counter));
}

void osal_atomic_set(osal_atomic *atomic, int i)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }

    LOS_AtomicSet((Atomic *)&(atomic->counter), i);
}

int osal_atomic_inc_return(osal_atomic *atomic)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    return LOS_AtomicIncRet((Atomic *)&(atomic->counter));
}

int osal_atomic_dec_return(osal_atomic *atomic)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    return LOS_AtomicDecRet((Atomic *)&(atomic->counter));
}

void osal_atomic_inc(osal_atomic *atomic)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }

    LOS_AtomicInc((Atomic *)&(atomic->counter));
}

void osal_atomic_dec(osal_atomic *atomic)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }

    LOS_AtomicDec((Atomic *)&(atomic->counter));
}

void osal_atomic_add(osal_atomic *atomic, int count)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }

    LOS_AtomicAdd((Atomic *)&(atomic->counter), count);
}

void osal_atomic_sub(osal_atomic *atomic, unsigned int count)
{
    LOS_AtomicSub((Atomic *)&(atomic->counter), count);
}

int osal_atomic_add_return(osal_atomic *atomic, int count)
{
    return LOS_AtomicAdd((Atomic *)&(atomic->counter), count);
}

int osal_atomic_dec_and_test(osal_atomic *atomic)
{
    return (LOS_AtomicDecRet((Atomic *)&(atomic->counter)) == 0);
}