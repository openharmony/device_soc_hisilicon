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
 */

#ifndef __HMAC_FRAG_H__
#define __HMAC_FRAG_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "hmac_main.h"
#include "hmac_tx_data.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_FRAG_H
/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define HMAC_FRAG_TIMEOUT   2000
#define HMAC_MAX_FRAG_SIZE  2500

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
typedef oal_netbuf_stru* (*hmac_defrag_start_cb)(hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf, osal_u32 hrdsize);
typedef osal_u32 (*hmac_frag_start_cb)(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf,
    mac_tx_ctl_stru *tx_ctl);
typedef osal_void (*hmac_user_clear_defrag_res_cb)(hmac_user_stru *hmac_user);
/*****************************************************************************
  10 函数声明
*****************************************************************************/
static osal_u32 hmac_frag_init_weakref(osal_void) __attribute__ ((weakref("hmac_frag_init"), used));
static osal_void hmac_frag_deinit_weakref(osal_void) __attribute__ ((weakref("hmac_frag_deinit"), used));
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_frag.h */
