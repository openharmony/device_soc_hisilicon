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
 * Description: Header file of the DMAC external public interface.
 * Create: 2022-2-19
 */

#ifndef __DMAC_EXT_IF_DEVICE_H__
#define __DMAC_EXT_IF_DEVICE_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "dmac_ext_if_device_rom.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 宏定义
*****************************************************************************/

#define DMAC_TX_QUEUE_AGGR_DEPTH 2

/*****************************************************************************
  3 枚举定义
*****************************************************************************/

#ifdef _PRE_WLAN_DFT_STAT
typedef enum {
    OAM_STATS_PHY_NODE_TX_CNT = 1,
    OAM_STATS_PHY_NODE_RX_OK_CNT,
    OAM_STATS_PHY_NODE_SYN_CNT,
    OAM_STATS_PHY_NODE_OFDM_SYN_ERR_CNT,
    OAM_STATS_PHY_NODE_AGC_LOCK_CNT,
    OAM_STATS_PHY_NODE_11B_HDR_ERR_CNT,
    OAM_STATS_PHY_NODE_OFDM_HDR_ERR_CNT,
    OAM_STATS_PHY_NODE_UNSUPPORTED_CNT,
    OAM_STATS_PHY_NODE_CARRIER_LOSE_CNT,
    OAM_STATS_PHY_NODE_LDPC_FAIL_CNT,
    OAM_STATS_PHY_NODE_RX_TIMEOUT_CNT,
    OAM_STATS_PHY_NODE_HI_PIN_UNLOCK_CNT,
    OAM_STATS_PHY_NODE_LO_PIN_UNLOCK_CNT,
    OAM_STATS_PHY_NODE_INIT_BW_LARGER_CNT,
    OAM_STATS_PHY_NODE_INIT_BW_SMALLER_CNT,
    OAM_STATS_PHY_NODE_INIT_BW_EQUAL_CNT,
    OAM_STATS_PHY_NODE_BUTT
} oam_stats_phy_node_idx_enum;
typedef osal_u8 oam_stats_phy_node_idx_enum_uint8;
#endif

typedef struct {
    osal_u8 query_sta_addr[WLAN_MAC_ADDR_LEN];
} dmac_query_request_event;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of dmac_ext_if_device.h */
