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
 * Description: auto-aggr probe algorithm rom.
 */

#ifndef ALG_AGGR_PROBE_ROM_H
#define ALG_AGGR_PROBE_ROM_H

#ifdef _PRE_WLAN_FEATURE_AUTOAGGR

/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#include "hal_device_rom.h"
#include "dmac_ext_if_rom.h"
#include "frw_msg_rom.h"
#include "dmac_alg_if_part_rom.h"
#include "alg_aggr_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/******************************************************************************
  2 宏定义
******************************************************************************/
#define alg_aggr_smooth_delta_per(_pst_aggr_dev_info, _avg_delta_per, _instant_delta_per) \
    (((osal_s32)(_avg_delta_per) * (osal_s32)((1U << (_pst_aggr_dev_info)->aggr_stat_shift) - 1) + \
       (_instant_delta_per)) / (osal_s32)(1U << (_pst_aggr_dev_info)->aggr_stat_shift))

#define alg_aggr_average_delta_per(delta_per, stat_cnt, instant_delta_per) \
    (((delta_per) * (stat_cnt) + (instant_delta_per)) / ((stat_cnt) + 1))

#define alg_aggr_pktnum1_smaller_pktnum2(_ul_pktno1, _ul_pktno2) \
    ((((_ul_pktno1) < (_ul_pktno2)) && ((_ul_pktno2) - (_ul_pktno1) < 0xFFF)) ||  \
     (((_ul_pktno1) > (_ul_pktno2)) && ((_ul_pktno1) - (_ul_pktno2) > 0xF00U)))

/******************************************************************************
  3 枚举定义
******************************************************************************/
/* 聚合探测方向 */
typedef enum {
    ALG_AGGR_PROBE_UP = 0,
    ALG_AGGR_PROBE_DOWN = 1,
    ALG_AGGR_PROBE_DIRECT_BUTT
} alg_aggr_direct_enum;

/* 聚合探测状态 */
typedef enum {
    ALG_AGGR_PROBE_STATE_IDLE = 0,   /* 聚合探测空闲状态 */
    ALG_AGGR_PROBE_STATE_START = 1,  /* 聚合探测启动状态 */

    ALG_AGGR_PROBE_STATE_BUTT
} alg_aggr_probe_state_enum;

/******************************************************************************
  4 STRUCT定义
******************************************************************************/

/******************************************************************************
  5 函数声明
******************************************************************************/
osal_u32  alg_aggr_tx_probe_type_process(dmac_user_stru *user, mac_tx_ctl_stru *cb, hal_tx_txop_alg_stru *txop_param,
    alg_aggr_user_info_stru *aggr_info);
osal_void alg_aggr_check_probe_direction(alg_aggr_stru *aggr_dev_info, alg_aggr_user_info_stru *aggr_info);
osal_void alg_aggr_basic_probe_sent_process(dmac_user_stru *dmac_user, alg_aggr_user_info_stru *aggr_info,
    mac_tx_ctl_stru *cb, hal_tx_dscr_ctrl_one_param *tx_dscr);
osal_void alg_aggr_get_probe_lock(alg_aggr_user_info_stru *aggr_info, osal_u16 user_lut_idx);

typedef osal_u32 (*alg_aggr_tx_probe_proc_cb)(dmac_user_stru *user, mac_tx_ctl_stru *cb,
    hal_tx_txop_alg_stru *txop_param, alg_aggr_user_info_stru *aggr_info, osal_u32 *ret);
typedef osal_u32 (*alg_aggr_probe_sent_cb)(dmac_user_stru *dmac_user, alg_aggr_user_info_stru *aggr_info,
    mac_tx_ctl_stru *cb, hal_tx_dscr_ctrl_one_param *tx_dscr);
typedef osal_u32 (*alg_aggr_probe_up_bytes_cb)(alg_aggr_user_info_stru *aggr_info, alg_aggr_stru *aggr_dev_info,
    hal_tx_dscr_ctrl_one_param *tx_dscr, osal_u32 *ret);
typedef osal_u32 (*alg_aggr_probe_up_updt_cb)(dmac_user_stru *dmac_user, alg_aggr_user_info_stru *aggr_info);
typedef osal_u32 (*alg_aggr_probe_down_updt_cb)(dmac_user_stru *dmac_user, alg_aggr_user_info_stru *aggr_info,
    alg_aggr_stru *aggr_dev_info, osal_u32 best_rate_kbps);
typedef osal_u32 (*alg_aggr_probe_up_proc_cb)(dmac_user_stru *dmac_user, osal_u32 best_rate_kbps,
    alg_aggr_user_info_stru *aggr_info, osal_u16 probe_per, hal_tx_dscr_ctrl_one_param *tx_dscr);
typedef osal_u32 (*alg_aggr_probe_down_proc_cb)(dmac_user_stru *dmac_user, osal_u32 best_rate_kbps,
    alg_aggr_user_info_stru *aggr_info, osal_u16 probe_per, alg_aggr_stru *aggr_dev_info);
typedef osal_u32 (*alg_aggr_chk_probe_down_cb)(alg_aggr_stru *aggr_dev_info, alg_aggr_user_info_stru *aggr_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* #ifdef _PRE_WLAN_FEATURE_AUTOAGGR */
#endif /* end of alg_aggr_probe_rom.h */
