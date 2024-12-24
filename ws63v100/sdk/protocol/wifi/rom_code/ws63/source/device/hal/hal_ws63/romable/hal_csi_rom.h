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
 * Description: Header file for hal_csi_rom.c.
 */

#ifndef HAL_CSI_ROM_H
#define HAL_CSI_ROM_H

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "osal_types.h"
#include "wlan_types_rom.h"
#include "hal_device_rom.h"
#include "hal_common_ops_rom.h"
#include "hh503_phy_reg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  宏定义
*****************************************************************************/

#undef THIS_FILE_ID
#define THIS_FILE_ID DIAG_FILE_ID_WIFI_DEVICE_HAL_CSI_ROM_H

#undef THIS_MOD_ID
#define THIS_MOD_ID DIAG_MOD_ID_WIFI_DEVICE

#ifdef _PRE_WLAN_FEATURE_CSI
typedef osal_u32 (*hh503_irq_csi_rx_complete_isr_cb)(void);
typedef osal_u32 (*hal_csi_keep_mac_pa_open_cb)(void);

/*****************************************************************************
  函数声明
*****************************************************************************/
osal_void hh503_irq_csi_rx_complete_isr(hal_device_stru *hal_device, osal_u8 data);
osal_bool hal_csi_is_open(void);
osal_u32 hh503_csi_get_blk_num_cnt(osal_void);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hal_csi.h */
