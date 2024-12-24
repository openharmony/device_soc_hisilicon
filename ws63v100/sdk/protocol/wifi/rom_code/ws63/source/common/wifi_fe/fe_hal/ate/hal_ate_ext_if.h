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
 * Description: hal ate interface header
 */

#ifndef __HAL_ATE_EXT_IF_H__
#define __HAL_ATE_EXT_IF_H__
#include "fe_rf_dev.h"
#include "hal_ate_spec.h"
#include "fe_hal_phy_if.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
typedef enum {
    ATE_TRX_TYPE_TX,
    ATE_TRX_TYPE_RX,
    ATE_TRX_TYPE_BUTT
} ate_trx_type_enum;


osal_void hal_ate_reset_rf_cali_reg_val(hal_rf_dev *rf_dev);
osal_void hal_ate_recover_rf_cali_path(hal_rf_dev *rf_dev);
osal_s32 hal_ate_get_mode_map_val(osal_u8 mode_idx, osal_u8 *band, osal_u8 *bandwidth);
const osal_u8 *hal_ate_get_channel_list(osal_u8 band, osal_u8 *ch_list_len);
osal_s32 hal_ate_get_protocol_and_rate(osal_u8 cfg_type, osal_u8 rate_val, osal_u8 *rate_idx, osal_u8 *protocol_mode);
osal_void hal_ate_rf_cali_set_pa_ppa(osal_u8 freq, osal_u8 cali_subband_num, osal_u32 reg_val);
osal_u32 hal_ate_rf_get_vdet_pa_ppa_code(osal_u8 idx);
fe_hal_phy_al_tx_para_stru hal_ate_rf_get_vdet_al_tx_param(osal_u8 rf_id);
osal_void hal_ate_open_sample_gen(osal_u8 node);
osal_void hal_ate_close_sample_gen(osal_void);
osal_void hal_ate_config_rf_tx_status(osal_u8 tx_local_en);
osal_void hal_ate_config_rf_rx_status(osal_u8 rx_local_en);
osal_void hal_ate_config_rf_rx_radar_status(osal_u8 rx_local_en);
osal_void hal_ate_config_rf_trx_gain_default(osal_void);
osal_void hal_ate_cfg_tone(cali_single_tone_stru single_tone_cfg);
osal_void hal_ate_update_al_tx_param(osal_void);
#ifdef _PRE_WLAN_ATE_EDA_TEST
osal_void hal_ate_init_read_reg_all(osal_void);
#endif
#if _PRE_WLAN_ATE_EDA_TEST == 4
osal_void hal_ate_read_sample_data(osal_void);
#endif
#if _PRE_WLAN_ATE_EDA_TEST == 5 || _PRE_WLAN_ATE_EDA_TEST == 7
osal_void hal_ate_write_sample_data(osal_void);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif // __HAL_ATE_EXT_IF_H__