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
 * Description: P2P initialization and adapt interface implementation.
 */

#ifndef MSG_P2P_ROM_H
#define MSG_P2P_ROM_H

#include "wlan_types_base_rom.h"
#include "oal_types_device_rom.h"

/* P2P noa中需要上报到device侧的信息 */
typedef struct {
    wlan_vap_mode_enum_uint8 vap_mode; /* vap模式 */
    oal_bool_enum_uint8 pause;
    osal_u8 reserved[2];
} p2p_info_report_stru;

#endif