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
 * Description: oam log printing interface
 */

#ifndef __WLAN_UTIL_COMMON_H__
#define __WLAN_UTIL_COMMON_H__

#include "wlan_util_common_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* ****************************************************************************
 函 数 名  : oal_bit_get_num_one_byte
 功能描述  : 获取单字节中的bit1的个数
**************************************************************************** */
static inline osal_u8 oal_bit_get_num_one_byte(osal_u8 byte)
{
    osal_u8 byte_data = byte;

    byte_data = (byte_data & 0x55) + ((byte_data >> 1) & 0x55); /* 0x55掩码, 1移位数 */
    byte_data = (byte_data & 0x33) + ((byte_data >> 2) & 0x33); /* 0x33掩码, 2移位数 */
    byte_data = (byte_data & 0x0F) + ((byte_data >> 4) & 0x0F); /* 0x0F掩码, 4移位数 */

    return byte_data;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of wlan_util_common.h */
