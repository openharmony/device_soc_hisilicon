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
 * Description: APF mac header.
 * Author:
 * Create: 2022-10-14
 */

#ifndef MAC_APF_H
#define MAC_APF_H

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "frw_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_APF_H

#ifdef _PRE_WLAN_FEATURE_APF
/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define APF_PROGRAM_MAX_LEN 512

/*****************************************************************************
  3 枚举定义
*****************************************************************************/
typedef enum {
    APF_SET_FILTER_CMD,
    APF_GET_FILTER_CMD,
    APF_FILTER_CMD_BUFF
} mac_apf_cmd_type_enum;
typedef osal_u8 mac_apf_cmd_type_uint8;

typedef enum {
    OSAL_SWITCH_OFF  = 0,
    OSAL_SWITCH_ON   = 1,
    OSAL_SWITCH_BUTT
} osal_switch_enum;

typedef struct {
    mac_apf_cmd_type_uint8 cmd_type;
    osal_u16 program_len;
    osal_u8 *program;
} mac_apf_filter_cmd_stru;

#endif /* end of _PRE_WLAN_FEATURE_APF */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of mac_apf.h */