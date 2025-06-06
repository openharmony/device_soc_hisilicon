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
 * 文 件 名   : hmac_encap_frame.h
 * 生成日期   : 2013年6月28日
 * 功能描述   : hmac_encap_frame.c 的头文件
 */

#ifndef __HMAC_ENCAP_FRAME_H__
#define __HMAC_ENCAP_FRAME_H__


/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "hmac_user.h"
#include "mac_vap_ext.h"

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
extern osal_u16  hmac_mgmt_encap_deauth_etc(hmac_vap_stru *hmac_vap, osal_u8 *data,
    osal_u8 *da, osal_u16 err_code);
extern osal_u16  hmac_mgmt_encap_disassoc_etc(hmac_vap_stru *hmac_vap, osal_u8 *data,
    osal_u8 *da, osal_u16 err_code);
extern osal_u16  hmac_encap_sa_query_req_etc(hmac_vap_stru *hmac_vap, osal_u8 *data,
    osal_u8 *da, osal_u16 trans_id);
extern osal_u16  hmac_encap_sa_query_rsp_etc(hmac_vap_stru *hmac_vap,
    osal_u8 *hdr, osal_u8 *data);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_encap_frame.h */
