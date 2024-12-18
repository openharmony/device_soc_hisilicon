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
 * Description: frw adapter
 */

#ifndef __FRW_COMMON_H
#define __FRW_COMMON_H

#include "osal_types.h"
#include "frw_msg_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
osal_void cfg_msg_init(osal_u8 *data, osal_u32 data_len, osal_u8 *rsp, osal_u32 rsp_buf_len, frw_msg *cfg_info);
osal_s32 send_cfg_to_device(osal_u8 vap_id, osal_u16 msg_id, frw_msg *cfg_info, osal_bool sync);
osal_s32 send_sync_cfg_to_host(osal_u8 vap_id, osal_u16 msg_id, frw_msg *cfg_info);

osal_void frw_event_process_all_event_etc(osal_ulong data);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
