/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

#ifndef __HI_MOD_PARAM__
#define __HI_MOD_PARAM__

#include "hi_type.h"
#include "hi_defines.h"

typedef struct hi_vgs_module_params_s {
    HI_U32 u32_max_vgs_job;
    HI_U32 u32_max_vgs_task;
    HI_U32 u32_max_vgs_node;
    HI_U32 au32_vgs_en[VGS_IP_NUM];
    HI_BOOL b_vgs_hdr_support;
    HI_BOOL b_vgs_exit_in_sys;
} vgs_module_params_s;

typedef struct hi_gdc_module_params_s {
    HI_U32 u32_max_gdc_job;
    HI_U32 u32_max_gdc_task;
    HI_U32 u32_max_gdc_node;
    HI_U32 au32_gdc_en[GDC_IP_NUM];
} gdc_module_params_s;

typedef struct hivdec_module_params_s {
    HI_U32 u32_vdec_max_chn_num;
    HI_BOOL b_vdec_hfr;
} vdec_module_params_s;

typedef struct hi_ive_module_params_s {
    HI_BOOL b_save_power_en;
    HI_U16  u16_ive_node_num;
    HI_U16  u16_rsv;
} ive_module_params_s;

typedef struct hi_svp_nnie_module_params_s {
    HI_BOOL b_save_power_en;
    HI_U16 u16_nnie_tsk_buf_num;
} svp_nnie_module_params_s;


typedef struct hi_h265e_module_params_s {
    HI_U32 u32_feature_enable;
} h265e_module_params_s;

typedef struct hi_venc_module_params_s {
    HI_U32 u32_venc_max_chn_num;
} venc_module_params_s;

typedef struct hi_vfmw_module_params_s {
    HI_S32 s32_vfmw_max_chn_num;
} vfmw_module_params_s;

typedef struct hi_acodec_module_params_s {
    HI_U32  u32_init_delay_timems;
} acodec_module_params_s;

#endif

