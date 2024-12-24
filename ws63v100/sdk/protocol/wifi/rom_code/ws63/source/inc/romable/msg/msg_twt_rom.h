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
 * Description: Header file
 */

#ifndef MSG_TWT_ROM_H
#define MSG_TWT_ROM_H
#include "osal_types.h"
#include "mac_frame_common_rom.h"
#include "wlan_resource_common_rom.h"

typedef struct {
    osal_u8 twt_session_status;
    osal_u16 user_idx;
    osal_u8 twt_ps_pause;
} mac_d2hd_twt_cfg_stru;

typedef struct {
    osal_u32 cfg_type;
    mac_d2hd_twt_cfg_stru twt_cfg;
} mac_d2hd_twt_sync_info_stru;

/* 修改参数 */
typedef struct {
    mac_twt_update_source_enum_uint8 update_source;
    osal_u16 user_idx;
    osal_u8 resv;
    mac_cfg_twt_stru twt_cfg;
} mac_ctx_update_twt_stru;

#endif