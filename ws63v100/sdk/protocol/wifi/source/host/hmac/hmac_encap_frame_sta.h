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
 * 文 件 名   : hmac_encap_frame_sta.h
 * 生成日期   : 2013年6月28日
 * 功能描述   : hmac_encap_frame_sta.c 的头文件
 */

#ifndef HMAC_ENCAP_FRAME_STA_H
#define HMAC_ENCAP_FRAME_STA_H


/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "hmac_vap.h"
#include "hmac_device.h"

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
extern osal_u32 hmac_mgmt_encap_asoc_req_sta_etc(hmac_vap_stru *hmac_vap, osal_u8 *req_frame,
    osal_u8 *curr_bssid, osal_u8 *dest_addr);
extern osal_u16  hmac_mgmt_encap_auth_req_etc(hmac_vap_stru *hmac_vap, osal_u8 *mgmt_frame);
extern osal_u16  hmac_mgmt_encap_auth_req_seq3_etc(hmac_vap_stru *hmac_vap, osal_u8 *mgmt_frame,
    osal_u8 *mac_hrd);
extern hmac_scanned_bss_info* hmac_vap_get_scan_bss_info(hmac_vap_stru *hmac_vap);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_encap_frame_sta.h */
