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
 * Create: 2022-4-27
 */

#ifndef OAL_LINUX_MM_H
#define OAL_LINUX_MM_H

#include <linux/slab.h>
#include <linux/hardirq.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>
#include "oal_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef dma_addr_t  oal_dma_addr;

typedef enum {
    OAL_BIDIRECTIONAL = 0,
    OAL_TO_DEVICE = 1,
    OAL_FROM_DEVICE = 2,
    OAL_NONE = 3,
} oal_data_direction;
typedef osal_u8 oal_direction_uint8;

/*****************************************************************************
 功能描述  : 申请核心态的内存空间，并填充0。对于Linux操作系统而言，
             需要考虑中断上下文和内核上下文的不同情况(GFP_KERNEL和GFP_ATOMIC)。
 输入参数  : size: alloc mem size
 返 回 值  : alloc mem addr
*****************************************************************************/
OAL_STATIC OAL_INLINE osal_void* oal_memalloc(osal_u32 size)
{
    osal_s32   l_flags = GFP_KERNEL;
    osal_void   *mem_space;

    /* 不睡眠或在中断程序中标志置为GFP_ATOMIC */
    if ((in_interrupt() != 0) || (irqs_disabled() != 0)) {
        l_flags = GFP_ATOMIC;
    }

    if (unlikely(size == 0)) {
        return OAL_PTR_NULL;
    }

    mem_space = kmalloc(size, l_flags);
    if (mem_space == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    return mem_space;
}

/*****************************************************************************
 功能描述  : 释放核心态的内存空间。
 输入参数  : pbuf:需释放的内存地址
*****************************************************************************/
OAL_STATIC OAL_INLINE osal_void oal_free(osal_void *p_buf)
{
    kfree(p_buf);
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of oal_mm.h */

