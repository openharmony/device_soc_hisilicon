/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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
#ifndef __SDK_MOD_PARAM__
#define __SDK_MOD_PARAM__

#include "ot_type.h"
#include "ot_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ot_vgs_module_params {
    td_u32 max_vgs_job;
    td_u32 max_vgs_task;
    td_u32 max_vgs_node;
} vgs_module_params_s;

typedef struct ot_gdc_module_params_s {
    td_u32 u32_max_gdc_job;
    td_u32 u32_max_gdc_task;
    td_u32 u32_max_gdc_node;
    td_u32 au32_gdc_en[OT_GDC_IP_NUM];
} gdc_module_params_s;

typedef struct hivdec_module_params_s {
    td_u32 u32_vdec_max_chn_num;
    td_bool b_vdec_hfr;
} vdec_module_params_s;

typedef struct ot_ive_module_params_s {
    td_u16 mod_node_num;
    td_u8 power_save_en;
} ive_module_params_s;

typedef struct ot_svp_nnie_module_params_s {
    td_bool b_save_power_en;
    td_u16 u16_nnie_tsk_buf_num;
} svp_nnie_module_params_s;


typedef struct ot_h265e_module_params_s {
    td_u32 u32_feature_enable;
} h265e_module_params_s;

typedef struct ot_venc_module_params_s {
    td_u32 u32_venc_max_chn_num;
} venc_module_params_s;

typedef struct ot_vfmw_module_params_s {
    td_s32 s32_vfmw_max_chn_num;
} vfmw_module_params_s;

typedef struct ot_acodec_module_params_s {
    td_u32  u32_init_delay_timems;
}acodec_module_params_s;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SDK_MOD_PARAM__ */
