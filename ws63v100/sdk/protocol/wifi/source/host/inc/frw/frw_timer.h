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
 * Description: Header file of frw_timer.c.
 * Author:
 * Create: 2020-01-01
 */

#ifndef __FRW_TIMER_H__
#define __FRW_TIMER_H__

/*****************************************************************************
    其他头文件包含
*****************************************************************************/
#include "oal_timer.h"
#include "frw_ext_if.h"
#include "mac_vap_ext.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_FRW_TIMER_H
/*****************************************************************************
    宏定义
*****************************************************************************/
#define FRW_TIMER_ENTRY_MAGIC_NUM 0xDEADBEEF /* 填充给注册的timer，防止被调用处memset清除后导致timer异常 */
/*****************************************************************************
    函数声明
*****************************************************************************/
osal_void frw_timer_init(osal_u32 delay, oal_timer_func func, osal_ulong arg);
osal_void frw_timer_exit(osal_void);
osal_s32 frw_timer_timeout_proc(hmac_vap_stru *hmac_vap, frw_msg *msg);
#ifdef _PRE_WLAN_FEATURE_WS73
osal_void frw_timer_timeout_proc_event_etc(uintptr_t arg);
#else
osal_void frw_timer_timeout_proc_event_etc(unsigned long arg);
#endif
osal_void frw_timer_exit_clean(osal_u32 core_id);
#ifdef _PRE_WLAN_FEATURE_POWERSAVE
osal_void frw_timer_stop_all_timer(osal_void);
osal_void frw_timer_restart_all_timer(osal_void);
#endif
/*****************************************************************************
    OTHERS定义
*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of frw_timer.h */
