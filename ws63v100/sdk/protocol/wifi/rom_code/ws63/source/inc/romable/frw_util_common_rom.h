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
 * Description: netbuf enum type head file.
 */

#ifndef FRW_UTIL_COMMON_ROM_H
#define FRW_UTIL_COMMON_ROM_H

#include "osal_types.h"
#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif
#endif

/* 32位寄存器最大长度 */
#define EXT_TIME_US_MAX_LEN (0xFFFFFFFF)
#define USEC_TO_MSEC        1000                 /* s -> ms -> us 转换 */

typedef struct {
    osal_slong i_sec;
    osal_slong i_usec;
} osal_time_us_stru;

/* 获取从_ul_start到_ul_end的时间差 */
static INLINE__ osal_u32 osal_get_runtime(osal_u32 _start, osal_u32 _end)
{
    return ((_start > _end) ? (EXT_TIME_US_MAX_LEN - _start + _end + 1) : (_end - _start));
}

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif
#endif /* end of frw_util_common_rom.h */