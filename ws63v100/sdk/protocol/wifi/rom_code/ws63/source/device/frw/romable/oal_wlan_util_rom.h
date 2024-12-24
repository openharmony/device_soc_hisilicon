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
 */

#ifndef OAL_WLAN_UTIL_ROM_H
#define OAL_WLAN_UTIL_ROM_H

#include "frw_util_rom.h"
#include "wlan_util_common_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 字节操作 */
static INLINE__ osal_u32 oal_rotl_w(osal_u32 val, osal_u32 bits, osal_u32 width)
{
    return (val << bits) | (val >> (width - bits));
}
static INLINE__ osal_u32 oal_rotl32(osal_u32 val, osal_u32 bits)
{
    return oal_rotl_w(val, bits, 32); /* 32 bits word */
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of oal_wlan_util_rom.h */