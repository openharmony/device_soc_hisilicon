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
 * Description: header file for alg_trc.c
 * Create: 2022-12-15
 */

#ifndef ALG_AUTORATE_H
#define ALG_AUTORATE_H

/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#include "alg_main_rom.h"
#include "alg_autorate_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_AUTORATE
osal_u32 alg_ar_probe_mcs_prepare_next_probe_info_cb(alg_autorate_user_stru *user_info,
    oal_bool_enum_uint8 *continue_cur_stage_probe, alg_autorate_probe_direction_enum_uint8 probe_direction,
    osal_u32 *ret);
osal_u32 alg_ar_probe_result_update_proc_cb(alg_autorate_user_stru *user_info, hal_tx_dscr_ctrl_one_param *tx_dscr,
    osal_u32 *ret);
osal_u32 alg_ar_probe_result_analysis_proc_cb(alg_autorate_user_stru *user_info, hal_tx_dscr_ctrl_one_param *tx_dscr,
    oal_bool_enum_uint8 *continue_probe);
osal_void alg_autorate_dev_param_cfg(mac_alg_cfg_enum_uint16 alg_cfg, osal_u32 value);
osal_u32 alg_ar_probe_finish_update_info_cb_etc(alg_autorate_user_stru *user_info, osal_u8 update_bestrate);
#endif /* #ifdef _PRE_WLAN_FEATURE_AUTORATE */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of alg_autorate.h */
