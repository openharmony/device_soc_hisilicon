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
 * Description: thruput test header file.
 */

#ifndef __HMAC_SAMPLE_DAQ_PHY_H__
#define __HMAC_SAMPLE_DAQ_PHY_H__

#ifdef _PRE_WLAN_FEATURE_DAQ
#include "hal_common_ops.h"
#include "diag_sample_data.h"
#include "mac_vap_ext.h"
#include "frw_msg_rom.h"

/* SOC CFG_SAMPLE_SEL 数采节点源 */
#define HMAC_SAMPLE_SOURCE_WLAN_PHY   0x1    /* 采集源：wlan phy */

/* ============== PHY 数采接口 起始 ============ */
void hmac_config_chip_test_set_diag_param_phy_recovery(void);
void hmac_config_chip_test_set_diag_param_phy_begin(hal_to_phy_test_diag_stru *param);
oal_bool_enum_uint8 hmac_config_chip_test_set_diag_param_phy_query(void);
osal_u32 hmac_sample_wlan_phy_data_enable(diag_wlan_phy_sample_cmd *sample_cmd, osal_u32 len);
/* ============== PHY 数采接口 结束 ============ */
/* phy 事件上报 */
osal_s32 hmac_complete_phy_event_rpt(hmac_vap_stru *hmac_vap, frw_msg *msg);

#endif
#endif /* __HMAC_SAMPLE_DAQ_PHY_H__ */
