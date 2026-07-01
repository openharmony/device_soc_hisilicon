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

#include <stdio.h>
#include "ot_type.h"
#include "osal_mmz.h"
#include "ot_osal_user.h"
#include "sdk_module_init.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

static td_void BASE_exit(void)
{
    ot_base_mod_exit();
}

static td_void SYS_exit(void)
{
    ot_sys_mod_exit();
}

static td_void ISP_exit(void)
{
    ot_isp_mod_exit();
}

static td_void VI_exit(void)
{
    ot_vi_mod_exit();
}

static td_void VPP_exit(void)
{
    ot_vpp_mod_exit();
}

static td_void RGN_exit(void)
{
    ot_rgn_mod_exit();
}

static td_void GDC_exit(void)
{
    ot_gdc_mod_exit();
}

static td_void DIS_exit(void)
{
    ot_dis_mod_exit();
}

static td_void VGS_exit(void)
{
    ot_vgs_mod_exit();
}

static td_void VPSS_exit(void)
{
    ot_vpss_mod_exit();
}

static td_void VO_exit(void)
{
    ot_vo_mod_exit();
}

static td_void RC_exit(void)
{
    ot_rc_mod_exit();
}

static td_void VENC_exit(void)
{
    ot_venc_mod_exit();
}

static td_void VDEC_exit(void)
{
    return ot_vdec_mod_exit();
}

static td_void VFMW_exit(void)
{
    return ot_vfmw_mod_exit();
}

static td_void JPEGD_exit(void)
{
    return ot_jpegd_mod_exit();
}

static td_void CHNL_exit(void)
{
    ot_chnl_mod_exit();
}

static td_void H264e_exit(void)
{
    ot_h264e_mod_exit();
}

static td_void H265e_exit(void)
{
    ot_h265e_mod_exit();
}

static td_void JPEGE_exit(void)
{
    ot_jpege_mod_exit();
}

static td_void AIAO_exit(void)
{
    ot_aiao_mod_exit();
}

static td_void AI_exit(void)
{
    ot_ai_mod_exit();
}

static td_void AO_exit(void)
{
    ot_ao_mod_exit();
}

static td_void AENC_exit(void)
{
    ot_aenc_mod_exit();
}

static td_void ADEC_exit(void)
{
    ot_adec_mod_exit();
}

static td_void ACODEC_exit(void)
{
    ot_acodec_mod_exit();
}

static td_void VEDU_exit(void)
{
    ot_vedu_mod_exit();
}

static td_void DPU_MATCH_exit(void)
{
    ot_dpu_match_mod_exit();
}

static td_void DPU_RECT_exit(void)
{
    ot_dpu_rect_mod_exit();
}

#if INIT_PQP == 0
static td_void SVP_NPU_exit(void)
{
    ot_svp_npu_mod_exit();
}
#else
static td_void PQP_exit(void)
{
    ot_pqp_mod_exit();
}
#endif

static td_void IVE_exit(void)
{
    ot_ive_mod_exit();
}

static td_void MAU_exit(void)
{
    ot_mau_mod_exit();
}

static td_void AUDIO_exit(void)
{
    ACODEC_exit();
    ADEC_exit();
    AENC_exit();
    AO_exit();
    AI_exit();
    AIAO_exit();
}

static td_void SENSOR_I2C_exit()
{
    ot_dev_exit();
}

static td_void PWM_exit()
{
    ot_pwm_mod_exit();
}

static td_void PIRIS_exit()
{
    ot_piris_mod_exit();
}
#ifdef OT_GYRODIS_SUPPORT
static td_void GYRO_SPI_exit()
{
    ot_motionsensor_mod_exit();
}

static td_void GYRO_DIS_exit()
{
    ot_gyrodis_mod_exit();
}

static td_void MOTION_FUSION_exit()
{
    motionfusion_exit();
}

static td_void MOTION_SENSOR_MNG_exit()
{
    ot_motionsensor_mng_exit();
}

static td_void MOTION_SENSOR_CHIP_exit()
{
    ot_motionsensor_chip_exit();
}

static td_void GYRO_exit()
{
    MOTION_SENSOR_CHIP_exit();
    MOTION_SENSOR_MNG_exit();
    MOTION_FUSION_exit();
    GYRO_DIS_exit();
    GYRO_SPI_exit();
}
#endif

td_void SDK_exit(void)
{
#ifdef OT_GYRODIS_SUPPORT
    GYRO_exit();
#endif
    PIRIS_exit();
    PWM_exit();
    SENSOR_I2C_exit();

    AUDIO_exit();
#if INIT_PQP == 0
    SVP_NPU_exit();
#else
    PQP_exit();
#endif
    MAU_exit();
    DPU_MATCH_exit();
    DPU_RECT_exit();
    IVE_exit();

    VDEC_exit();
    VFMW_exit();
    JPEGD_exit();

    JPEGE_exit();
    H265e_exit();
    H264e_exit();
    VENC_exit();
    RC_exit();
    VEDU_exit();
    VO_exit();
    VPSS_exit();
    ISP_exit();
    VI_exit();

    VPP_exit();
    DIS_exit();
    VGS_exit();
    GDC_exit();
    RGN_exit();
    CHNL_exit();
    SYS_exit();
    BASE_exit();
    osal_exit();

    printf("SDK exit ok...\n");
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
