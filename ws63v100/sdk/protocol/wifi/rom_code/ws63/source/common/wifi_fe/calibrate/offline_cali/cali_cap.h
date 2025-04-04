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
 * Description: cap code计算
 */

#ifndef __CALI_CAP_H__
#define __CALI_CAP_H__
/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "cali_outer_hal_if.h"
#include "cali_base_def.h"
#include "cali_accum_data.h"
#include "fe_rf_dev.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 宏定义
*************************************************************************/

/*****************************************************************************
  3 枚举定义
*****************************************************************************/
typedef enum {
    CALI_CAP_PATH_PWR = 0,
    CALI_CAP_PATH_LOGEN = 1,

    CALI_CAP_PATH_BUTT = 2
} cali_cap_path_enum;
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
    osal_u8                      cap_path_type;          /* 校准CAP的通路类型,0:功率校准通路 1:logen校准 */
    wlan_channel_band_enum_uint8 cali_band;              /* 频段 */
    osal_u8                      cali_subband_num;       /* CAP CODE配置频点 */
    osal_u8                      reserved;
    osal_u16                     cap_code_len;           /* CAP CODE长度 */
    osal_s16                     accum_threshold;        /* 累加上报上限值 */
    osal_s32                     vdet_offset;            /* 无单音时的DC值 */
} cali_cap_stru;
/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/
/* 函数指针定义 WRITE_UPC_FUNC_PTR */
typedef osal_void (*p_upc_cfg_func)(const hal_rf_dev *, osal_u8, osal_u8, osal_u16);
/*****************************************************************************
  10 函数声明
*****************************************************************************/
osal_u8 cali_calc_cap_code(const hal_device_stru *device, cali_cap_stru *cap_cali_param,
    cali_accum_para_stru *accum_param, p_upc_cfg_func set_cap_func, osal_u8 *code);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif