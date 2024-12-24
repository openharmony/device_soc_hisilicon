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
 * Description: wal_psd
 * Create: 2023-02-28
 */

#ifndef __WAL_PSD_H__
#define __WAL_PSD_H__

#ifdef _PRE_WLAN_FEATURE_PSD
/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "oal_ext_if.h"
#ifndef _PRE_WLAN_FEATURE_WS73
#include "diag.h"
#else
#include "soc_zdiag.h"
#endif
#include "wal_ccpriv.h"
#include "hmac_psd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_PSD_H
#undef THIS_MOD_ID
#define THIS_MOD_ID DIAG_MOD_ID_WIFI_HOST

#define PSD_DEV_NAME "wlan0"

osal_void wal_psd_enable_callback(osal_u16 cmd_id, osal_void* cmd_param, osal_u16 cmd_param_size,
    DIAG_OPTION *option);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
#endif /* end of wal_psd.h */
