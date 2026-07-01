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

#include "ot_type.h"
#include "sdk_module_param.h"
#include "securec.h"
#include "ot_osal_user.h"
#include "sdk_module_init.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

static td_s32 BASE_init(void)
{
    return ot_base_mod_init();
}

static td_s32 SYS_init(void)
{
    return ot_sys_mod_init();
}

static td_s32 RGN_init(void)
{
    return ot_rgn_mod_init();
}

static td_s32 ISP_init(void)
{
    return ot_isp_mod_init();
}

static td_s32 VI_init(void)
{
    return ot_vi_mod_init();
}

static td_s32 VPP_init(void)
{
    return ot_vpp_mod_init();
}

static td_s32 DIS_init(void)
{
    return ot_dis_mod_init();
}

static td_s32 GDC_init(void)
{
    gdc_module_params_s st_gdc_module_param;
    st_gdc_module_param.u32_max_gdc_job  = 32; /* 32 -- max job num */
    st_gdc_module_param.u32_max_gdc_task  = 64; /* 64 -- max task num */
    st_gdc_module_param.u32_max_gdc_node  = 64; /* 64 -- max node num */
    st_gdc_module_param.au32_gdc_en[0]   = 1;

    return ot_gdc_mod_init(&st_gdc_module_param);
}

static td_s32 VGS_init(void)
{
    vgs_module_params_s st_vgs_module_param;
    st_vgs_module_param.max_vgs_job  = 64;  /* 64 -- max job num */
    st_vgs_module_param.max_vgs_task = 200; /* 200 -- max task num */
    st_vgs_module_param.max_vgs_node = 200; /* 200 -- max node num */

    return ot_vgs_mod_init(&st_vgs_module_param);
}

static td_s32 VPSS_init(void)
{
    return ot_vpss_mod_init(NULL);
}

static td_s32 VO_init(void)
{
    return ot_vo_mod_init();
}

static td_s32 RC_init(void)
{
    return ot_rc_mod_init();
}

static td_s32 VENC_init(void)
{
    td_u32 max_chn = 64;
    return ot_venc_mod_init(max_chn);
}

static td_s32 VDEC_init(void)
{
    return ot_vdec_mod_init();
}

static td_s32 CHNL_init(void)
{
    return ot_chnl_mod_init();
}

static td_s32 VEDU_init(void)
{
    return ot_vedu_mod_init();
}

static td_s32 H264e_init(void)
{
    return ot_h264e_mod_init();
}

static td_s32 H265e_init(void)
{
    return ot_h265e_mod_init(NULL);
}

static td_s32 JPEGE_init(void)
{
    return ot_jpege_mod_init();
}

static td_s32 JPEGD_init(void)
{
    return ot_jpegd_mod_init();
}

static td_s32 VFMW_init(void)
{
    return ot_vfmw_mod_init(32); /* 32 */
}

static td_s32 AIAO_init(void)
{
    return ot_aiao_mod_init();
}

static td_s32 AI_init(void)
{
    return ot_ai_mod_init();
}

static td_s32 AO_init(void)
{
    return ot_ao_mod_init();
}

static td_s32 AENC_init(void)
{
    return ot_aenc_mod_init();
}

static td_s32 ADEC_init(void)
{
    return ot_adec_mod_init();
}

static td_s32 ACODEC_init(void)
{
    return ot_acodec_mod_init();
}

static td_s32 DPU_RECT_init(void)
{
    return ot_dpu_rect_mod_init();
}

static td_s32 DPU_MATCH_init(void)
{
    return ot_dpu_match_mod_init();
}

#if INIT_PQP == 0
static td_s32 SVP_NPU_init(void)
{
    return ot_svp_npu_mod_init();
}
#else
static td_s32 PQP_init(void)
{
    return ot_pqp_mod_init();
}
#endif

static td_s32 IVE_init(void)
{
    ive_module_params_s ive_mod_param;
    ive_mod_param.power_save_en = 1; /* 1 -- power save on */
    ive_mod_param.mod_node_num = 128; /* 128 -- max node num */
    return ot_ive_mod_init(&ive_mod_param);
}

static td_s32 MAU_init(void)
{
    return ot_mau_mod_init();
}

static td_void AUDIO_init(void)
{
    td_s32 ret;

    ret = AIAO_init();
    if (ret != 0) {
        printf("aiao init error.\n");
    }

    ret = AI_init();
    if (ret != 0) {
        printf("ai init error.\n");
    }

    ret = AO_init();
    if (ret != 0) {
        printf("ao init error.\n");
    }

    ret = AENC_init();
    if (ret != 0) {
        printf("aenc init error.\n");
    }

    ret = ADEC_init();
    if (ret != 0) {
        printf("adec init error.\n");
    }

    ret = ACODEC_init();
    if (ret != 0) {
        printf("acodec init error.\n");
    }
}

static td_s32 SENSOR_I2C_init()
{
    return ot_dev_init();
}

static td_s32 PWM_init()
{
    return ot_pwm_mod_init();
}

static td_s32 PIRIS_init()
{
    return ot_piris_mod_init();
}
#ifdef OT_GYRODIS_SUPPORT
static td_s32 GYRO_SPI_init()
{
    return ot_motionsensor_mod_init();
}

static td_s32 GYRO_DIS_init()
{
    return ot_gyrodis_mod_init();
}

static td_s32 MOTION_FUSION_init()
{
    return motionfusion_init();
}

static td_s32 MOTION_SENSOR_MNG_init()
{
    return ot_motionsensor_mng_init();
}

static td_s32 MOTION_SENSOR_CHIP_init()
{
    return ot_motionsensor_chip_init();
}

static td_s32 GYRO_init()
{
    td_s32 ret;

    ret = GYRO_SPI_init();
    if (ret != 0) {
        printf("spi init error.\n");
    }

    ret = GYRO_DIS_init();
    if (ret != 0) {
        printf("gyrodis init error.\n");
    }

    ret = MOTION_FUSION_init();
    if (ret != 0) {
        printf("mfusion init error.\n");
    }

    ret = MOTION_SENSOR_MNG_init();
    if (ret != 0) {
        printf("msensor_mng init error.\n");
    }

    ret = MOTION_SENSOR_CHIP_init();
    if (ret != 0) {
        printf("msensor_chip init error.\n");
    }

    return ret;
}
#endif

static td_s32 basic_mod_init()
{
    td_s32 ret;
    ret = osal_init();
    if (ret != 0) {
        printf("osal_init init error.\n");
    }

    ret = BASE_init();
    if (ret != 0) {
        printf("base init error.\n");
    }

    ret = SYS_init();
    if (ret != 0) {
        printf("sys init error.\n");
    }

    ret = CHNL_init();
    if (ret != 0) {
        printf("chnl init error.\n");
    }
    return ret;
}

static td_s32 common_mod_init()
{
    td_s32 ret;
    ret = RGN_init();
    if (ret != 0) {
        printf("rgn init error.\n");
    }

    ret = GDC_init();
    if (ret != 0) {
        printf("gdc init error.\n");
    }

    ret = VGS_init();
    if (ret != 0) {
        printf("vgs init error.\n");
    }

    ret = DIS_init();
    if (ret != 0) {
        printf("vgs init error.\n");
    }

    ret = VPP_init();
    if (ret != 0) {
        printf("vi init error.\n");
    }
    return ret;
}

static td_void svp_mod_init()
{
    td_s32 ret;
    ret = IVE_init();
    if (ret != 0) {
        printf("ive init error.\n");
    }

    ret = DPU_RECT_init();
    if (ret != 0) {
        printf("dpu rect init error.\n");
    }

    ret = DPU_MATCH_init();
    if (ret != 0) {
        printf("dpu match init error.\n");
    }

    ret = MAU_init();
    if (ret != 0) {
        printf("mau init error.\n");
    }

#if INIT_PQP == 0
    ret = SVP_NPU_init();
    if (ret != 0) {
        printf("svp npu init error.\n");
    }
#else
    ret = PQP_init();
    if (ret != 0) {
        printf("pqp init error.\n");
    }
#endif
}

static td_s32 business_mod_init()
{
    td_s32 ret;
    ret = VI_init();
    if (ret != 0) {
        printf("vi init error.\n");
    }

    ret = ISP_init();
    if (ret != 0) {
        printf("isp init error.\n");
    }

    ret = VPSS_init();
    if (ret != 0) {
        printf("vpss init error.\n");
    }

    ret = VO_init();
    if (ret != 0) {
        printf("vo init error.\n");
    }

    ret = VEDU_init();
    if (ret != 0) {
        printf("vedu init error.\n");
    }

    ret = RC_init();
    if (ret != 0) {
        printf("rc init error.\n");
    }

    ret = VENC_init();
    if (ret != 0) {
        printf("venc init error.\n");
    }

    ret = H264e_init();
    if (ret != 0) {
        printf("H264e init error.\n");
    }

    ret = H265e_init();
    if (ret != 0) {
        printf("H265e init error.\n");
    }

    ret = JPEGE_init();
    if (ret != 0) {
        printf("jpege init error.\n");
    }
    return ret;
}

static td_s32 business_mod_init1()
{
    td_s32 ret;

    ret = JPEGD_init();
    if (ret != 0) {
        printf("jpegd init error.\n");
    }

    ret = VFMW_init();
    if (ret != 0) {
        printf("vfmw init error.\n");
    }

    ret = VDEC_init();
    if (ret != 0) {
        printf("vdec init error.\n");
    }

    svp_mod_init();

    AUDIO_init();

    return ret;
}

static td_s32 business_mod_init2()
{
    td_s32 ret;

    ret = SENSOR_I2C_init();
    if (ret != 0) {
        printf("sensor i2c init error.\n");
    }

    ret = PWM_init();
    if (ret != 0) {
        printf("pwm init error.\n");
    }

    ret = PIRIS_init();
    if (ret != 0) {
        printf("piris init error.\n");
    }

    return ret;
}

td_void SDK_init(void)
{
    td_s32 ret;

    ret = basic_mod_init();
    if (ret != 0) {
        printf("basic_mod_init error.\n");
        return;
    }

    ret = common_mod_init();
    if (ret != 0) {
        printf("common_mod_init error.\n");
        return;
    }

    ret = business_mod_init();
    if (ret != 0) {
        printf("business_mod_init error.\n");
        return;
    }

    ret = business_mod_init1();
    if (ret != 0) {
        printf("basic_mod_init error.\n");
        return;
    }

    ret = business_mod_init2();
    if (ret != 0) {
        printf("business_mod_init2 error.\n");
        return;
    }
#ifdef OT_GYRODIS_SUPPORT
    ret = GYRO_init();
    if (ret != 0) {
        printf("gyro_dis init error.\n");
    }
#endif
    printf("SDK init ok...\n");
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
