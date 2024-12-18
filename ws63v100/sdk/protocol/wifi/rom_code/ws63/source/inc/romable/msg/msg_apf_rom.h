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
 * Description: Header file of msg_apf_rom
 */

#ifndef MSG_APF_ROM_H
#define MSG_APF_ROM_H

#include "oal_types_device_rom.h"

#define APF_PROGRAM_MAX_LEN 512

#ifdef _PRE_WLAN_FEATURE_APF
typedef struct {
    oal_bool_enum_uint8 is_enabled;
    oal_bool_enum_uint8 rsv; // 保留，4字节对齐
    osal_u16 program_len;
    osal_u32 install_timestamp;
    osal_u8 program[APF_PROGRAM_MAX_LEN];
} mac_apf_stru;
#endif
#endif