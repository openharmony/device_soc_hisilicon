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
#ifndef __SDK_MOD_INIT__
#define __SDK_MOD_INIT__

#include "ot_type.h"
#include "ot_defines.h"
#include "sdk_module_param.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INIT_PQP 0

int ot_base_mod_init(void);
int ot_sys_mod_init(void);
int ot_rgn_mod_init(void);
int ot_isp_mod_init(void);
int ot_vi_mod_init(void);
int ot_vpp_mod_init(void);
int ot_dis_mod_init(void);
int ot_gdc_mod_init(void* args);
td_s32 ot_vgs_mod_init(vgs_module_params_s *mod_params);
int ot_vpss_mod_init(void *args);
int ot_vo_mod_init(void);
int  ot_rc_mod_init(void);
td_s32 ot_venc_mod_init(td_u32 max_chn);
int ot_chnl_mod_init(void);
int ot_vedu_mod_init(void);
int ot_h264e_mod_init(void);
int ot_h265e_mod_init(void *args);
int ot_jpege_mod_init(void);
int ot_aiao_mod_init(void);
int ot_ai_mod_init(void);
int ot_ao_mod_init(void);
int ot_aenc_mod_init(void);
int ot_adec_mod_init(void);
int ot_acodec_mod_init(void);
int ot_dpu_rect_mod_init(void);
int ot_dpu_match_mod_init(void);
int ot_svp_npu_mod_init(void);
int ot_pqp_mod_init(void);
td_s32 ot_ive_mod_init(ive_module_params_s *ive_mod_param);
int ot_mau_mod_init(void);
int ot_vdec_mod_init(void);
int ot_jpegd_mod_init(void);
int ot_vfmw_mod_init(td_s32 chn_num);
int ot_dev_init(void);
int ot_pwm_mod_init(void);
int ot_piris_mod_init(void);
#ifdef OT_GYRODIS_SUPPORT
int ot_motionsensor_mod_init(void);
int motionfusion_init(td_void);
int ot_motionsensor_mng_init(td_void);
int ot_gyrodis_mod_init(td_void);
int ot_motionsensor_chip_init(td_void);
#endif

void ot_base_mod_exit(void);
void ot_sys_mod_exit(void);
void ot_isp_mod_exit(void);
void ot_vi_mod_exit(void);
void ot_vpp_mod_exit(void);
void ot_rgn_mod_exit(void);
void ot_gdc_mod_exit(void);
void ot_dis_mod_exit(void);
void ot_vgs_mod_exit(void);
void ot_vpss_mod_exit(void);
void ot_vo_mod_exit(void);
void ot_rc_mod_exit(void);
void ot_venc_mod_exit(void);
void ot_chnl_mod_exit(void);
void ot_h264e_mod_exit(void);
void ot_h265e_mod_exit(void);
void ot_jpege_mod_exit(void);
void ot_aiao_mod_exit(void);
void ot_ai_mod_exit(void);
void ot_ao_mod_exit(void);
void ot_aenc_mod_exit(void);
void ot_adec_mod_exit(void);
void ot_acodec_mod_exit(void);
void ot_vedu_mod_exit(void);
void ot_dpu_match_mod_exit(void);
void ot_dpu_rect_mod_exit(void);
void ot_svp_npu_mod_exit(void);
void ot_pqp_mod_exit(void);
void ot_ive_mod_exit(void);
void ot_mau_mod_exit(void);
void ot_dev_exit(void);
void ot_pwm_mod_exit(void);
void ot_piris_mod_exit(void);
#ifdef OT_GYRODIS_SUPPORT
void ot_motionsensor_mod_exit(void);
void motionfusion_exit(td_void);
void ot_motionsensor_mng_exit(td_void);
void ot_gyrodis_mod_exit(td_void);
void ot_motionsensor_chip_exit(td_void);
#endif

td_void SDK_init(void);
td_void SDK_exit(void);
void ot_vdec_mod_exit(void);
void ot_jpegd_mod_exit(void);
void ot_vfmw_mod_exit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SDK_MOD_INIT__ */
