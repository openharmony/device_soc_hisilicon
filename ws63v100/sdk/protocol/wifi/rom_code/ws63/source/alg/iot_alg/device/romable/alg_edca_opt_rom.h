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
 * Description: algorithm EDCA
 */
#ifndef ALG_EDCA_OPT_ROM_H
#define ALG_EDCA_OPT_ROM_H

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT
/*****************************************************************************
  1 其他头文件包含
******************************************************************************/
#include "dmac_alg_if_part_rom.h"
#include "dmac_ext_if_rom.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
typedef struct {
    alg_edca_det_ac_info_stru edca_det_ac_info[WLAN_WME_AC_BUTT];
    osal_u32 *rom;
} alg_dmac_edca_opt_stru;
typedef osal_u32 (*alg_edca_opt_tx_complete_process_cb)(dmac_user_stru *dmac_user, oal_dmac_netbuf_stru *buf,
    hal_tx_dscr_ctrl_one_param *tx_dscr_param);
typedef osal_u32 (*alg_edca_opt_dev_init_cb)(hal_to_dmac_device_stru *hal_device);
typedef osal_u32 (*alg_edca_opt_dev_exit_cb)(hal_to_dmac_device_stru *hal_device);
/*****************************************************************************
  10 函数声明
******************************************************************************/
osal_void alg_edca_init(osal_void);
osal_void alg_edca_exit(osal_void);
osal_s32 dmac_get_edca_ac_stat(dmac_vap_stru *dmac_vap, frw_msg *msg);
osal_s32 dmac_clear_edca_all_ac_stat(dmac_vap_stru *dmac_vap, frw_msg *msg);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
#endif
