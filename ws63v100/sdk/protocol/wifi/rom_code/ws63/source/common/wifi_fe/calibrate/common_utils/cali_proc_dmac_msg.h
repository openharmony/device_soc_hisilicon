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
 * Description:
 */

#ifndef __CALI_PROC_DMAC_MSG_H__
#define __CALI_PROC_DMAC_MSG_H__

#include "dmac_ext_if_rom.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
osal_s32 dmac_hal_rf_fsm_sync_state(dmac_vap_stru *dmac_vap, frw_msg *msg);
osal_s32 dmac_hal_rf_work_entry(dmac_vap_stru *dmac_vap, frw_msg *msg);
osal_s32 dmac_config_hcc_send_ready_msg(dmac_vap_stru *dmac_vap, frw_msg *msg);
osal_u32 dmac_config_cali_data_report(dmac_vap_stru *dmac_vap);
osal_s32 dmac_cali_data_buffer_download(dmac_vap_stru *dmac_vap, frw_msg *msg);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
