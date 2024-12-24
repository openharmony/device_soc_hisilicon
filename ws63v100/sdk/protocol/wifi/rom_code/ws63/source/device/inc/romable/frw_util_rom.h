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
 * Description: oal util interface
 * Author:
 */

#ifndef FRW_UTIL_ROM_H
#define FRW_UTIL_ROM_H

#include "osal_types.h"
#include "systick.h"
#include "soc_osal.h"
#include "oal_types_device_rom.h"
#ifdef _PRE_OS_VERSION_NONOS
#include "serial_dw.h"
#define osal_printf debug_message
#else
#define osal_printf osal_printk
#endif

static INLINE__ osal_u32 frw_max(osal_u32 a, osal_u32 b)
{
    return a > b ? a : b;
}
static INLINE__ osal_u32 frw_min(osal_u32 a, osal_u32 b)
{
    return a < b ? a : b;
}

#ifndef _PRE_WLAN_FEATURE_WS73
/* 获取毫秒级时间戳  */
static INLINE__ osal_u64 osal_dmac_get_time_stamp_ms(osal_void)
{
    return uapi_systick_get_us() >> 10; /* 右移10位,用1024换算ms */
}
/* 获取高精度毫秒级时间戳 */
static INLINE__ osal_u64 osal_dmac_get_high_prec_ms(osal_void)
{
    return uapi_systick_get_us() / 1000; /* 用1000换算ms */
}
/* 获取31.25微秒级时间戳 */
static INLINE__ osal_u64 osal_dmac_get_time_stamp_ts(osal_void)
{
    return uapi_systick_get_us();
}

#else
/* 获取毫秒级时间戳  */
static INLINE__ osal_u64 osal_dmac_get_time_stamp_ms(osal_void)
{
    return uapi_get_time_us() >> 10; /* 右移10位,用1024换算ms */
}
/* 获取高精度毫秒级时间戳 */
static INLINE__ osal_u64 osal_dmac_get_high_prec_ms(osal_void)
{
    return uapi_get_time_us() / 1000; /* 用1000换算ms */
}
/* 获取31.25微秒级时间戳 */
static INLINE__ osal_u64 osal_dmac_get_time_stamp_ts(osal_void)
{
    return uapi_get_time_us();
}
#endif

osal_s32 osal_atoi(const osal_char *c_string);

#endif /* end of frw_util_rom.h */
