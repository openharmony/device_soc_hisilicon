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
 * Description: rf customize rssi comp 头文件
 */
#ifndef _FE_RF_CUSTOMIZE_RSSI_COMP_H_
#define _FE_RF_CUSTOMIZE_RSSI_COMP_H_

#include "osal_types.h"
#include "dmac_ext_if_rom.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#define FE_CUST_RSSI_COMP_2G_NUM  3  /* RSSI数量 与band划分对应 */
typedef struct {
    osal_s8 rssi_comp[FE_CUST_RSSI_COMP_2G_NUM];
    osal_u8 select_subband; /* 设置或获取rssi_comp对应的subband标志位 */
} fe_custom_rssi_comp_2g_stru;

typedef struct {
    fe_custom_rssi_comp_2g_stru rssi_comp_2g;
} fe_custom_rssi_stru;

osal_s32 fe_custom_get_rssi_comp(osal_u8 band, osal_s8 *rssi_comp, osal_u8 subband);
osal_s32 fe_custom_process_rssi_comp(dmac_vap_stru *dmac_vap, frw_msg *msg);
#endif  // _PRE_PLAT_FEATURE_CUSTOMIZE
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif  // _FE_RF_CUSTOMIZE_RSSI_COMP_H_