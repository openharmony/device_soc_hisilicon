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
 * Description: Header file for hal_phy_reg_rom.c.
 */

#ifndef __HAL_PHY_REG_ROM_H__
#define __HAL_PHY_REG_ROM_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "osal_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_DAQ
/*****************************************************************************
   宏定义
*****************************************************************************/

/*****************************************************************************
  STRUCT定义
*****************************************************************************/

/*****************************************************************************
  函数声明
*****************************************************************************/
void hal_set_sample_cfg_2_cfg_debug_sample_by_len(osal_u32 cfg_debug_sample_by_len);
void hal_set_sample_cfg_0_cfg_debug_sample_end_mode(osal_u32 cfg_debug_sample_end_mode);
void hal_set_sample_cfg_0_cfg_debug_sample_start_mode(osal_u32 cfg_debug_sample_start_mode);
void hal_set_sample_cfg_2_cfg_with_sample_delay_en(osal_u32 cfg_with_sample_delay_en);
void hal_set_sample_cfg_2_cfg_debug_sample_delay(osal_u32 cfg_debug_sample_delay);
void hal_set_sample_cfg_2_cfg_with_mac_tsf_en(osal_u32 cfg_with_mac_tsf_en);
void hal_set_sample_cfg_2_cfg_with_mac_info_en(osal_u32 cfg_with_mac_info_en);
void hal_set_sample_cfg_0_cfg_debug_sample_en(osal_u32 cfg_debug_sample_en);
void hal_set_wlbb_clr_sample_clear_sample_done(osal_u32 clear_sample_done);
osal_void hal_set_sample_cfg_0_cfg_phy_data_sample_cg_bps(osal_u32 cfg_phy_data_sample_cg_bps);
osal_void hal_set_sample_cfg_0_cfg_phy_data_sample_clk_en(osal_u32 cfg_phy_data_sample_clk_en);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hal_phy_reg_rom.h */
