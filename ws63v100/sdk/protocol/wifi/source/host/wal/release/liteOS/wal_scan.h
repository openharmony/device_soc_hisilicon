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
 * Description: Header file for wal_scan.c.
 */

#ifndef __WAL_SCAN_H__
#define __WAL_SCAN_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "oal_ext_if.h"
#include "hmac_ext_if.h"
#include "wal_event.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 宏定义
*****************************************************************************/
/*****************************************************************************
  3 枚举定义
*****************************************************************************/
/*****************************************************************************
  4 全局变量声明
*****************************************************************************/
/*****************************************************************************
  5 消息头定义
*****************************************************************************/
/*****************************************************************************
  6 消息定义
*****************************************************************************/
/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
/*****************************************************************************
  8 UNION定义
*****************************************************************************/
/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/
/*****************************************************************************
  10 函数声明
*****************************************************************************/
td_void wal_inform_all_bss(const oal_net_device_stru *netdev, const hmac_bss_mgmt_stru *bss_mgmt, td_u8 vap_id);
td_u32 wal_start_scan_req(oal_net_device_stru *netdev, hmac_scan_stru *scan_mgmt);
td_u32 wal_force_scan_complete(oal_net_device_stru *netdev);
td_void wal_free_scan_mgmt_resource(hmac_scan_stru *scan_mgmt);
td_void wal_scan_report_ie_calc(const wal_scanned_bss_info_stru *scanned_bss_info, td_u32 *need_report_ie_len);
td_u32 is_p2p_scan_req(oal_cfg80211_scan_request_stru *request);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif /* end of wal_scan.h */
