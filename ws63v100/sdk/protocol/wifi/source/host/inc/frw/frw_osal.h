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
 * Description: wifi dmac os adapt layer
 * Author:
 * Create: 2021-08-05
 */

#ifndef __FRW_OSAL_H__
#define __FRW_OSAL_H__

#include "osal_types.h"
#include "oal_types.h"
#include "osal_adapt.h"
#if defined(_PRE_OS_VERSION_LINUX) && defined(_PRE_OS_VERSION) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include<linux/interrupt.h>
#endif
/* 禁止所有中断 */
static inline osal_u32 frw_osal_irq_lock(osal_void)
{
#if defined(_PRE_OS_VERSION_LINUX) && defined(_PRE_OS_VERSION) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    local_irq_disable();
    return OAL_SUCC;
#else
    return osal_adapt_irq_lock();
#endif
}

/* 恢复中断 */
static inline osal_void frw_osal_irq_restore(osal_u32 irq_status)
{
#if defined(_PRE_OS_VERSION_LINUX) && defined(_PRE_OS_VERSION) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    unref_param(irq_status);
    local_irq_enable();
    return;
#else
    return osal_adapt_irq_restore(irq_status);
#endif
}

#endif // endif __FRW_OSAL_H__

