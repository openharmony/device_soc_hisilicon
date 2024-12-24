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
 * Description: Header file for wal_mfg_ccpriv.c.
 */
#ifndef WAL_MFG_CCPRIV_H
#define WAL_MFG_CCPRIV_H
#ifdef _PRE_WLAN_FEATURE_MFG_TEST

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "oal_ext_if.h"
#include "hmac_ext_if.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    EXT_AT_MFG_SET_XO_TRIM_COARSE,
    EXT_AT_MFG_SET_XO_TRIM_FINE,
    EXT_AT_MFG_GET_XO_TRIM,
    EXT_AT_MFG_SET_FACTOR,
    EXT_AT_MFG_GET_FACTOR,
    EXT_AT_MFG_SET_CALI_POWER,
    EXT_AT_MFG_GET_RSSI_OFFSET,
    EXT_AT_MFG_SET_CURVE_PARAM,
    EXT_AT_MFG_GET_CURVE_PARAM,
    EXT_AT_MFG_SET_LOW_CURVE_PARAM,
    EXT_AT_MFG_GET_LOW_CURVE_PARAM,
    EXT_AT_MFG_SET_RSSI_OFFSET,
    EXT_AT_MFG_SET_TAR_POWER,
    EXT_AT_MFG_GET_TEMP,
    EXT_AT_MFG_SET_EFUSE_POWER,
    EXT_AT_MFG_GET_EFUSE_POWER,
    EXT_AT_MFG_SET_EFUSE_RSSI_COMP,
    EXT_AT_MFG_GET_EFUSE_RSSI_COMP,
    EXT_AT_MFG_SET_EFUSE_CMU_XO_TRIM,
    EXT_AT_MFG_GET_EFUSE_CMU_XO_TRIM,
    EXT_AT_MFG_EFUSE_RCALDATA,
    EXT_AT_MFG_EFUSE_REMAIN,
    EXT_AT_MFG_SET_EFUSE_TEMP,
    EXT_AT_MFG_GET_EFUSE_TEMP,

    EXT_AT_MFG_BUTT
} ext_at_mfg_type_enum;

typedef struct {
    osal_char *cmd_name;
    osal_u32 type;
} wal_mfg_ccpriv_cmd_stru;

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif
#endif
#endif /* end of wal_mfg_ccpriv.h */