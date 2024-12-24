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
 * Description: Header file of HAL ANT SEL ROM external public interfaces .
 */

#ifndef HAL_ANT_SEL_ROM_H
#define HAL_ANT_SEL_ROM_H

#include "osal_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

osal_void hal_set_ant_sel_mac_tx_prot(osal_u8 ant_sel);
osal_void hal_set_ant_sel_mac_tx_bar(osal_u8 ant_sel);
osal_void hal_set_ant_sel_mac_tx_cf_end(osal_u8 ant_sel);
osal_void hal_set_ant_sel_mac_tx_beacon(osal_u8 ant_sel);
osal_void hal_set_ant_sel_mac_tx_coex(osal_u8 ant_sel);
osal_void hal_set_ant_sel_mac_tx_one_pkt(osal_u8 ant_sel);
osal_void hal_set_ant_sel_mac_rx(osal_u8 ant_sel);
osal_void hal_set_ant_sel_phy_enable(osal_u8 phy_enable);
osal_void hal_set_ant_sel_phy_cfg_man(osal_u8 cfg_man);
osal_void hal_set_ant_sel_phy_cfg_value(osal_u8 ant_sel);
osal_u32 hal_get_ant_sel_phy_enable(osal_void);
osal_u32 hal_get_ant_sel_phy_cfg_man(osal_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hal_ant_sel_rom.h */