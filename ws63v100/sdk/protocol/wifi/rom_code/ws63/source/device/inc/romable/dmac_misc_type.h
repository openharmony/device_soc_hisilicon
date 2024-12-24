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
 * Description: Header file of device and host frw_msg public interface.
 */

#ifndef DMAC_MISC_TYPE_H
#define DMAC_MISC_TYPE_H

#include "osal_types.h"
#include "oal_types_device_rom.h"
#define osal_likely(x) __builtin_expect(!!(x), 1)
#define osal_unlikely(x) __builtin_expect(!!(x), 0)

static INLINE__ osal_u32 oal_return_addr(osal_void)
{
    return (osal_u32)__builtin_return_address(0);
}

#define osal_bit(nr) (1UL << (nr))

#ifndef OSAL_WRITE_BITS /* 与oal_plat_type.h重定义，先屏蔽后整改 */
#define OSAL_WRITE_BITS

static INLINE__ osal_void osal_write_bits(osal_u32 *data, osal_u32 val, osal_u32 bits, osal_u32 pos)
{
    (*data) &= ~((((osal_u32)1 << bits) - 1) << pos);
    (*data) |= ((val & (((osal_u32)1 << bits) - 1)) << pos);
}
#endif

#define osal_array_size(_array)  (sizeof(_array) / sizeof((_array)[0]))

#define unref_param(P)  ((P) = (P))

#ifndef NEWLINE
#define NEWLINE                 "\r\n"
#endif

static INLINE__ osal_u16 osal_makeu16(osal_u32 a, osal_u32 b)
{
    return ((osal_u16)(((osal_u8)a) | (((osal_u16)((osal_u8)b)) << 8))); /* 左移8位作为高8位 */
}

#endif // __DMAC_MISC_TYPE__

