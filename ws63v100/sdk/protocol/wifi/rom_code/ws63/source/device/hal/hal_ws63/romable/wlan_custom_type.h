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
 * Description: Header files shared by wlan_types.h and wlan_types_rom.h.
 * Author:
 */

#ifndef WLAN_CUSTOM_TYPES_BASE_H
#define WLAN_CUSTOM_TYPES_BASE_H

#include "td_base.h"
#include "osal_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


typedef enum {
    WLAN_OFDM_ACK_CTS_TYPE_24M = 0,
    WLAN_OFDM_ACK_CTS_TYPE_BUTT
} wlan_ofdm_ack_cts_type_enum;
typedef osal_u8 wlan_ofdm_ack_cts_type_enum_uint8;

typedef enum {
    WLAN_BAND_ASSEMBLE_20M                   = 0,
    WLAN_BAND_ASSEMBLE_SU_ER_242_TONE        = 0, /* 等同20M */
    WLAN_BAND_ASSEMBLE_SU_ER_106_TONE        = 1,
    WLAN_BAND_ASSEMBLE_40M                   = 2,
    WLAN_BAND_ASSEMBLE_40M_DUP               = 3,
    WLAN_BAND_ASSEMBLE_BUTT
} hal_channel_assemble_enum;
typedef osal_u8 hal_channel_assemble_enum_uint8;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of wlan_types.h */
