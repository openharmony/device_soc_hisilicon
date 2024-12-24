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
 * Description: osal adapt atomic source file.
 */

#include "osal_adapt.h"

int osal_adapt_atomic_init(osal_atomic *atomic)
{
    osal_atomic_set(atomic, 0);
    return OSAL_SUCCESS;
}

void osal_adapt_atomic_destroy(osal_atomic *atomic)
{
    osal_atomic_set(atomic, 0);
}

int osal_adapt_atomic_read(osal_atomic *atomic)
{
    return osal_atomic_read(atomic);
}

void osal_adapt_atomic_set(osal_atomic *atomic, int val)
{
    osal_atomic_set(atomic, val);
}

int osal_adapt_atomic_inc_return(osal_atomic *atomic)
{
    return osal_atomic_inc_return(atomic);
}

int osal_adapt_atomic_dec_return(osal_atomic *atomic)
{
    return osal_atomic_dec_return(atomic);
}

void osal_adapt_atomic_inc(osal_atomic *atomic)
{
    osal_atomic_inc(atomic);
}

void osal_adapt_atomic_dec(osal_atomic *atomic)
{
    osal_atomic_dec(atomic);
}
void osal_adapt_atomic_add(osal_atomic *atomic, int val)
{
    osal_atomic_add(atomic, val);
}