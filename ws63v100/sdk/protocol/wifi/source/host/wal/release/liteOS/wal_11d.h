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
 * Description: Header file for wal_11d.c.
 */

#ifndef __WAL_11D_H__
#define __WAL_11D_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "hmac_chan_mgmt.h"

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
typedef struct {
    td_char                    *pc_country;       /* 国家字符串 */
    mac_dfs_domain_enum_uint8   dfs_domain;       /* DFS 雷达标准 */
    td_u8                       reserved[3];      /* 3 BYTE保留字段 */
}wal_dfs_domain_entry_stru;

/*****************************************************************************
  8 UNION定义
*****************************************************************************/
/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/
/*****************************************************************************
  10 函数声明
*****************************************************************************/
td_u32 wal_regdomain_update(oal_net_device_stru *netdev, const td_char *country, td_u8 country_code_len);
#ifdef _PRE_WLAN_FEATURE_DFS
td_u32 wal_regdomain_update_for_dfs(oal_net_device_stru *netdev, td_char *country);
#endif
td_s32 wal_util_set_country_code(oal_net_device_stru *net_dev, osal_s8 *country_code);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif /* end of wal_11d.h */
