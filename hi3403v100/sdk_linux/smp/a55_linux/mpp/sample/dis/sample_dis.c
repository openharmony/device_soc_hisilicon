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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#ifdef OT_GYRODIS_SUPPORT
#include "sample_gyro_dis.h"
#include "sample_dis_two_pipe.h"
#endif

#include "sample_comm.h"
#include "ss_mpi_isp.h"
#include "sdk_module_init.h"

typedef enum {
    SAMPLE_DIS_GME_TYPE_4DOF = 0,
    SAMPLE_DIS_GME_TYPE_6DOF,
    SAMPLE_DIS_GME_TYPE_BUTT
} dis_gme_type;

ot_vi_pipe g_vi_pipe = 0;
ot_vi_pipe g_send_vi_pipe = 3;
ot_vi_chn  g_vi_chn = 0;

ot_vpss_grp g_vpss_grp = 0;
ot_vpss_chn g_vpss_chn = 0;

ot_vo_chn g_vo_chn = 0;
ot_venc_chn g_venc_chn = 0;
static td_u32 g_dis_sample_signal_flag = 0;

td_bool g_dis_save_stream = TD_TRUE;
td_bool g_dis_send_data = TD_FALSE;

static sample_comm_venc_chn_param g_venc_chn_param = {
    .frame_rate           = 30,
    .stats_time           = 1,
    .gop                  = 60,
    .venc_size            = {1920, 1080},
    .size                 = PIC_1080P,
    .profile              = 0,
    .is_rcn_ref_share_buf = TD_FALSE,
    .gop_attr             = {
        .gop_mode = OT_VENC_GOP_MODE_NORMAL_P,
        .normal_p = {2},
    },
    .type                 = OT_PT_H265,
    .rc_mode              = SAMPLE_RC_CBR,
};

td_void sample_dis_set_send_data_statue(td_bool send_data)
{
    g_dis_send_data = send_data;
}

td_bool sample_dis_get_send_data_statue(td_void)
{
    return g_dis_send_data;
}

td_bool sample_dis_get_save_stream(void)
{
    return g_dis_save_stream;
}

td_void sample_dis_set_save_stream(td_bool save_stream)
{
    g_dis_save_stream = save_stream;
}

td_void sample_dis_set_venc_chn_size(ot_size img_size)
{
    g_venc_chn_param.venc_size = img_size;
}

/* function : show usage */
static td_void sample_dis_usage(td_char *argv_name)
{
    printf("Usage : %s <index> <intf>\n", argv_name);
    printf("index:\n");
    printf("\t 0)DIS-4DOF_GME.VI-VO VENC.\n");
    printf("\t 1)DIS-6DOF_GME.VI-VO VENC.\n");
#ifdef OT_GYRODIS_SUPPORT
    printf("\t 2)DIS_GYRO(IPC).VI-VO VENC.\n");
    printf("\t 3)DIS_GYRO(DV).VI-VO VENC.\n");
    printf("\t 4)DIS_GYRO and LDCV2 SWITCH. VI-VO VENC. LDCV2+DIS_GYRO -> LDCV2 -> LDCV2+DIS_GYRO.\n");
    printf("\t 5)DIS_GYRO and LDCV3 SWITCH. VI-VO VENC. LDCV2+DIS_GYRO -> LDCV3 -> LDCV2+DIS_GYRO.\n");
    printf("\t 6)DIS_GYRO(IPC). TWO SENSOR.\n");
    printf("\t 7)DIS_GYRO(TWO PIPE). ONE WITH DIS, ONE WITHOUT DIS.\n");
    printf("\t 8)DIS_GYRO dis send data.Usage : %s <index> <width> <height>\n", argv_name);
#endif
    printf("intf:\n");
    printf("\t 0) vo HDMI output, default.\n");
    printf("\t 1) vo BT1120 output.\n");
    return;
}

/* function : Get param by different sensor */
static td_s32 sample_dis_get_param_by_sensor(sample_sns_type sns_type, ot_dis_cfg *dis_cfg, ot_dis_attr *dis_attr)
{
    td_s32 ret = TD_SUCCESS;

    if (dis_cfg == NULL  || dis_attr == NULL) {
        return TD_FAILURE;
    }

    dis_cfg->frame_rate = 30; /* 30 fps default frame_rate */

    return ret;
}

/* function : to process abnormal case */
static void sample_dis_handle_sig(td_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        g_dis_sample_signal_flag = 1;
    }
}

static td_s32 sample_dis_gme_enable(sample_sns_type sns_type, dis_gme_type gme_type)
{
    td_s32 ret;
    ot_dis_cfg dis_cfg = {0};
    ot_dis_attr dis_attr = {0};

    dis_cfg.motion_level = OT_DIS_MOTION_LEVEL_NORM;
    dis_cfg.crop_ratio = 80; /* 80 sample crop ratio */
    dis_cfg.buf_num = 10; /* 10 sample buf num */
    dis_cfg.frame_rate = 30; /* 30 sample frame rate */
    dis_cfg.camera_steady = TD_FALSE;

    if (gme_type == SAMPLE_DIS_GME_TYPE_4DOF) {
        dis_cfg.scale              = TD_FALSE;
        dis_cfg.pdt_type           = OT_DIS_PDT_TYPE_DV;
        dis_cfg.mode              = OT_DIS_MODE_4_DOF_GME;
    } else {
        dis_cfg.scale              = TD_TRUE;
        dis_cfg.pdt_type           = OT_DIS_PDT_TYPE_IPC;
        dis_cfg.mode              = OT_DIS_MODE_6_DOF_GME;
    }

    dis_attr.enable = TD_TRUE;
    dis_attr.moving_subject_level = 0;
    dis_attr.rolling_shutter_coef = 0;
    dis_attr.timelag  = 0;
    dis_attr.still_crop = TD_FALSE;
    dis_attr.hor_limit = 512; /* 512 sample hor_limit */
    dis_attr.ver_limit = 512; /* 512 sample ver_limit */
    dis_attr.gdc_bypass = TD_FALSE;
    dis_attr.strength = 1024; /* 1024 sample strength */

    ret = sample_dis_get_param_by_sensor(sns_type, &dis_cfg, &dis_attr);
    if (ret != TD_SUCCESS) {
        sample_print("sample_dis_get_param_by_sensor failed.ret:0x%x !\n", ret);
        return TD_FAILURE;
    }
    ret = ss_mpi_vi_set_chn_dis_cfg(g_vi_pipe, g_vi_chn, &dis_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("set dis config failed.ret:0x%x !\n", ret);
        return TD_FAILURE;
    }

    ret = ss_mpi_vi_set_chn_dis_attr(g_vi_pipe, g_vi_chn, &dis_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set dis attr failed.ret:0x%x !\n", ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_void sample_dis_pause(td_void)
{
    if (g_dis_sample_signal_flag == 0) {
        getchar();
    }
}

static td_s32 sample_dis_gme_change()
{
    td_s32 ret;
    ot_dis_attr dis_attr = {0};

    printf("\nplease hit the Enter key to Disable DIS!\n");
    sample_dis_pause();

    ret = ss_mpi_vi_get_chn_dis_attr(g_vi_pipe, g_vi_chn, &dis_attr);
    if (ret != TD_SUCCESS) {
        sample_print("get dis attr failed.ret:0x%x !\n", ret);
        return TD_FAILURE;
    }

    dis_attr.enable = TD_FALSE;
    ret = ss_mpi_vi_set_chn_dis_attr(g_vi_pipe, g_vi_chn, &dis_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set dis attr failed.ret:0x%x !\n", ret);
        return TD_FAILURE;
    }

    printf("\nplease hit the Enter key to enable DIS!\n");
    sample_dis_pause();

    dis_attr.enable = TD_TRUE;
    ret = ss_mpi_vi_set_chn_dis_attr(g_vi_pipe, g_vi_chn, &dis_attr);
    if (ret != TD_SUCCESS) {
        sample_print("set dis attr failed.ret:0x%x !\n", ret);
        return TD_FAILURE;
    }

    printf("\nplease hit the Enter key to exit!\n");
    sample_dis_pause();
    return TD_SUCCESS;
}

static td_s32 sample_dis_init_sys_vb(const ot_size *img_size)
{
    ot_vb_cfg vb_cfg = {0};
    ot_pic_buf_attr buf_attr = {0};
    td_u32 blk_size;
    td_s32 ret;
    td_bool save_stream = sample_dis_get_save_stream();

    buf_attr.width = img_size->width;
    buf_attr.height = img_size->height;
    buf_attr.align = 0;
    buf_attr.bit_width = OT_DATA_BIT_WIDTH_8;
    buf_attr.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_422;
    buf_attr.compress_mode = OT_COMPRESS_MODE_SEG;

    blk_size = ot_common_get_pic_buf_size(&buf_attr);
    vb_cfg.max_pool_cnt = 128; /* 128 max pool cnt */
    vb_cfg.common_pool[0].blk_size = blk_size;
    vb_cfg.common_pool[0].blk_cnt = 20; /* 20 normal blk cnt */

    vb_cfg.common_pool[1].blk_size = blk_size;
    vb_cfg.common_pool[1].blk_cnt = 4; /* 4 bayer 16bpp blk cnt */

    if (save_stream == TD_FALSE) {
        vb_cfg.common_pool[0].blk_cnt = 20; /* 20 normal blk cnt */
        vb_cfg.common_pool[1].blk_cnt = 12; /* 12 bayer 16bpp blk cnt */
    }

    ret = sample_comm_sys_init_with_vb_supplement(&vb_cfg, OT_VB_SUPPLEMENT_BNR_MOT_MASK);
    if (ret != TD_SUCCESS) {
        sample_print("init sys fail.ret:0x%x !\n", ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_dis_start_vpss(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn, const ot_size *img_size)
{
    ot_vpss_grp_attr vpss_grp_attr = {0};
    ot_vpss_chn_attr vpss_chn_attr[OT_VPSS_MAX_PHYS_CHN_NUM] = {0};
    td_bool chn_enable[OT_VPSS_MAX_PHYS_CHN_NUM] = {0};
    td_s32 ret;

    vpss_grp_attr.max_width = img_size->width;
    vpss_grp_attr.max_height = img_size->height;
    vpss_grp_attr.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    vpss_grp_attr.dynamic_range = OT_DYNAMIC_RANGE_SDR8;
    vpss_grp_attr.frame_rate.src_frame_rate = -1;
    vpss_grp_attr.frame_rate.dst_frame_rate = -1;

    chn_enable[0] = TD_TRUE;
    vpss_chn_attr[0].width = img_size->width;
    vpss_chn_attr[0].height = img_size->height;
    vpss_chn_attr[0].chn_mode = OT_VPSS_CHN_MODE_USER;
    vpss_chn_attr[0].compress_mode = OT_COMPRESS_MODE_NONE;
    vpss_chn_attr[0].dynamic_range = OT_DYNAMIC_RANGE_SDR8;
    vpss_chn_attr[0].pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    vpss_chn_attr[0].video_format = OT_VIDEO_FORMAT_LINEAR;
    vpss_chn_attr[0].frame_rate.src_frame_rate = -1;
    vpss_chn_attr[0].frame_rate.dst_frame_rate = -1;
    vpss_chn_attr[0].depth = 1;
    vpss_chn_attr[0].mirror_en = TD_FALSE;
    vpss_chn_attr[0].flip_en = TD_FALSE;
    vpss_chn_attr[0].aspect_ratio.mode = OT_ASPECT_RATIO_NONE;

    ret = sample_common_vpss_start(vpss_grp, chn_enable, &vpss_grp_attr, vpss_chn_attr, OT_VPSS_MAX_PHYS_CHN_NUM);
    if (ret != TD_SUCCESS) {
        sample_print("start vpss failed. ret: 0x%x !\n", ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_dis_start_vi(const sample_vi_cfg *vi_cfg)
{
    ot_vi_vpss_mode_type mode_type = OT_VI_ONLINE_VPSS_OFFLINE;
    ot_isp_nr_attr isp_nr_attr = {0};
    td_s32 ret;
    td_bool save_stream = sample_dis_get_save_stream();
    if (save_stream == TD_FALSE || g_dis_send_data == TD_TRUE) {
        mode_type = OT_VI_OFFLINE_VPSS_OFFLINE;
    }

    ret = sample_comm_vi_set_vi_vpss_mode(mode_type, OT_VI_VIDEO_MODE_NORM);
    if (ret != TD_SUCCESS) {
        sample_print("set vi failed.ret:0x%x !\n", ret);
        return TD_FAILURE;
    }
    ret = sample_comm_vi_start_vi(vi_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("start vi failed.ret:0x%x !\n", ret);
        return TD_FAILURE;
    }

    if (g_dis_send_data == TD_FALSE) {
        ret = ss_mpi_isp_get_nr_attr(g_vi_pipe, &isp_nr_attr);
        if (ret != TD_SUCCESS) {
            sample_print("get nr attr failed.ret:0x%x !\n", ret);
            return TD_FAILURE;
        }

        isp_nr_attr.en = TD_FALSE;
        ret = ss_mpi_isp_set_nr_attr(g_vi_pipe, &isp_nr_attr);
        if (ret != TD_SUCCESS) {
            sample_print("set nr attr failed.ret:0x%x !\n", ret);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 sample_dis_start_venc(td_void)
{
    td_s32 ret;
    sample_comm_venc_chn_param venc_chnl_param = {0};

    ret = sample_comm_venc_get_gop_attr(OT_VENC_GOP_MODE_NORMAL_P, &venc_chnl_param.gop_attr);
    if (ret != TD_SUCCESS) {
        sample_print("venc get Gop attr failed. ret: 0x%x !\n", ret);
        return TD_FAILURE;
    }

    venc_chnl_param.rc_mode = SAMPLE_RC_CBR;
    venc_chnl_param.type = OT_PT_H265;
    venc_chnl_param.profile = 0;
    venc_chnl_param.size = PIC_1080P;
    if (g_dis_send_data == TD_FALSE) {
        ret = sample_comm_venc_start(g_venc_chn, &venc_chnl_param);
    } else {
        ret = sample_comm_venc_start(g_venc_chn, &g_venc_chn_param);
    }
    if (ret != TD_SUCCESS) {
        sample_print("start venc failed. ret: 0x%x !\n", ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_dis_bind(const sample_vo_cfg *vo_cfg)
{
    td_s32 ret;
    td_bool save_stream = sample_dis_get_save_stream();
    ret = sample_comm_vpss_bind_vo(g_vpss_grp, g_vpss_chn, vo_cfg->vo_dev, g_vo_chn);
    if (ret != TD_SUCCESS) {
        sample_print("vo bind vpss failed. ret: 0x%x !\n", ret);
        return TD_FAILURE;
    }

    if (g_dis_send_data == TD_TRUE) {
        ret = sample_comm_vi_bind_vpss(g_send_vi_pipe, g_vi_chn, g_vpss_grp, g_vpss_chn);
    } else {
        ret = sample_comm_vi_bind_vpss(g_vi_pipe, g_vi_chn, g_vpss_grp, g_vpss_chn);
    }
    if (ret != TD_SUCCESS) {
        sample_print("vi bind vpss failed. ret: 0x%x !\n", ret);
        return TD_FAILURE;
    }

    if (save_stream == TD_TRUE) {
        ret = sample_comm_vpss_bind_venc(g_vpss_grp, g_vpss_chn, g_venc_chn);
        if (ret != TD_SUCCESS) {
            sample_print("vpss bind venc failed. ret: 0x%x !\n", ret);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_void sample_dis_unbind(const sample_vo_cfg *vo_cfg)
{
    td_bool save_stream = sample_dis_get_save_stream();
    if (save_stream == TD_TRUE) {
        sample_comm_vpss_un_bind_venc(g_vpss_grp, g_vpss_chn, g_venc_chn);
    }
    if (g_dis_send_data == TD_TRUE) {
        sample_comm_vi_un_bind_vpss(g_send_vi_pipe, g_vi_chn, g_vpss_grp, g_vpss_chn);
    } else {
        sample_comm_vi_un_bind_vpss(g_vi_pipe, g_vi_chn, g_vpss_grp, g_vpss_chn);
    }
    sample_comm_vpss_un_bind_vo(g_vpss_grp, g_vpss_chn, vo_cfg->vo_dev, g_vo_chn);
}

/* define SAMPLE_MEM_SHARE_ENABLE, when use tools to dump YUV/RAW. */
#ifdef SAMPLE_MEM_SHARE_ENABLE
td_void sample_dis_init_mem_share(td_void)
{
    td_u32 i;
    ot_vb_common_pools_id pools_id = {0};

    if (ss_mpi_vb_get_common_pool_id(&pools_id) != TD_SUCCESS) {
        sample_print("get common pool_id failed!\n");
        return;
    }
    for (i = 0; i < pools_id.pool_cnt; ++i) {
        ss_mpi_vb_pool_share_all(pools_id.pool[i]);
    }
}
#endif

td_s32 sample_dis_start_sample(sample_vi_cfg *vi_cfg, sample_vo_cfg *vo_cfg, ot_size *img_size)
{
    td_bool chn_enable[OT_VPSS_MAX_PHYS_CHN_NUM] = {TD_TRUE};
    td_bool save_stream = sample_dis_get_save_stream();
    /* step 1: init SYS and common VB */
    if (sample_dis_init_sys_vb(img_size) != TD_SUCCESS) {
        return TD_FAILURE;
    }

#ifdef SAMPLE_MEM_SHARE_ENABLE
    sample_dis_init_mem_share();
#endif

    /* step 2: start VI */
    if (sample_dis_start_vi(vi_cfg) != TD_SUCCESS) {
        goto sys_exit;
    }

    /* step 3:  start VPSS */
    if (sample_dis_start_vpss(g_vpss_grp, g_vpss_chn, img_size) != TD_SUCCESS) {
        goto vi_stop;
    }

    /* step 4:  start VO */
    if (sample_comm_vo_start_vo(vo_cfg) != TD_SUCCESS) {
        sample_print("start vo failed!\n");
        goto vpss_stop;
    }

    if (save_stream == TD_TRUE) {
        /* step 5:  start VENC */
        if (sample_dis_start_venc() != TD_SUCCESS) {
            goto vo_stop;
        }
    }

    /* step 6:  start bind */
    if (sample_dis_bind(vo_cfg) != TD_SUCCESS) {
        goto venc_stop;
    }

    if (save_stream == TD_TRUE) {
        /* step 7: stream VENC process -- get stream, then save it to file. */
        if (sample_comm_venc_start_get_stream(&g_venc_chn, 1) != TD_SUCCESS) {
            sample_print("venc start get stream failed!\n");
            goto unbind;
        }
    }

    return TD_SUCCESS;

unbind:
    sample_dis_unbind(vo_cfg);
venc_stop:
    if (save_stream == TD_TRUE) {
        sample_comm_venc_stop(g_venc_chn);
    }
vo_stop:
    sample_comm_vo_stop_vo(vo_cfg);
vpss_stop:
    sample_common_vpss_stop(g_vpss_grp, chn_enable, OT_VPSS_MAX_PHYS_CHN_NUM);
vi_stop:
    sample_comm_vi_stop_vi(vi_cfg);
sys_exit:
    sample_comm_sys_exit();
    return TD_FAILURE;
}

td_void sample_dis_stop_sample_without_sys_exit(sample_vi_cfg *vi_cfg, sample_vo_cfg *vo_cfg)
{
    td_bool chn_enable[OT_VPSS_MAX_PHYS_CHN_NUM] = {0};
    td_bool save_stream = sample_dis_get_save_stream();
    chn_enable[0] = TD_TRUE;

    if (save_stream == TD_TRUE) {
        sample_comm_venc_stop_get_stream(1);
    }
    sample_dis_unbind(vo_cfg);
    if (save_stream == TD_TRUE) {
        sample_comm_venc_stop(g_venc_chn);
    }
    sample_comm_vo_stop_vo(vo_cfg);
    sample_common_vpss_stop(g_vpss_grp, chn_enable, OT_VPSS_MAX_PHYS_CHN_NUM);
    sample_comm_vi_stop_vi(vi_cfg);
}

td_void sample_dis_stop_sample(sample_vi_cfg *vi_cfg, sample_vo_cfg *vo_cfg)
{
    sample_dis_stop_sample_without_sys_exit(vi_cfg, vo_cfg);
    sample_comm_sys_exit();
}

static td_s32 sample_dis_gme(ot_vo_intf_type vo_intf_type, dis_gme_type gme_type)
{
    td_s32 ret;
    ot_size img_size;
    sample_vi_cfg vi_cfg;
    sample_vo_cfg vo_cfg = {0};
    sample_dis_set_save_stream(TD_TRUE);

    if (gme_type != SAMPLE_DIS_GME_TYPE_4DOF && gme_type != SAMPLE_DIS_GME_TYPE_6DOF) {
        sample_print("wrong gme_type %d!\n", gme_type);
        return TD_FAILURE;
    }

    /* step 1:  get sensors information and vo config */
    sample_comm_vi_get_default_vi_cfg(SENSOR0_TYPE, &vi_cfg);
    sample_comm_vo_get_def_config(&vo_cfg);
    vo_cfg.vo_intf_type = vo_intf_type;

    /* step 2:  get input size */
    sample_comm_vi_get_size_by_sns_type(vi_cfg.sns_info.sns_type, &img_size);

    /* step 3: start VI-VO-VENC */
    ret = sample_dis_start_sample(&vi_cfg, &vo_cfg, &img_size);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* step 4: set DIS config & attribute */
    if (sample_dis_gme_enable(vi_cfg.sns_info.sns_type, gme_type) != TD_SUCCESS) {
        goto exit;
    }

    /* step 5: dis enable disable */
    if (sample_dis_gme_change() != TD_SUCCESS) {
        goto exit;
    }

exit:
    /* exit process */
    sample_dis_stop_sample(&vi_cfg, &vo_cfg);
    return TD_SUCCESS;
}

static td_s32 sample_dis_proc(td_char *argv_name, char sample_index_char, ot_vo_intf_type vo_intf_type,
    ot_size *input_size)
{
    td_s32 ret = TD_FAILURE;
    switch (sample_index_char) {
        case '0':
            ret = sample_dis_gme(vo_intf_type, SAMPLE_DIS_GME_TYPE_4DOF);
            break;
        case '1':
            ret = sample_dis_gme(vo_intf_type, SAMPLE_DIS_GME_TYPE_6DOF);
            break;

#ifdef OT_GYRODIS_SUPPORT
        case '2':
            ret = sample_dis_ipc_gyro(vo_intf_type);
            break;
        case '3':
            ret = sample_dis_dv_gyro(vo_intf_type);
            break;
        case '4':
            ret = sample_dis_gyro_switch(vo_intf_type, OT_LDC_V2);
            break;
        case '5':
            ret = sample_dis_gyro_switch(vo_intf_type, OT_LDC_V3);
            break;
        case '6':
            ret = sample_dis_gyro_two_sensor(vo_intf_type);
            break;
        case '7':
            ret = sample_dis_gyro_demo(vo_intf_type);
            break;
        case '8':
            ret = sample_dis_send(vo_intf_type, input_size);
            break;
#endif
        default:
            sample_print("the index is invalid!\n");
            sample_dis_usage(argv_name);
            return TD_FAILURE;
    }
    return ret;
}

static td_s32 sample_dis_parse_optional_args(int argc, char *argv[],
    ot_vo_intf_type *vo_intf_type, ot_size *input_size)
{
    if (argc == 4) { /* 4 : argv num */
        input_size->width = atoi(argv[2]); /* index: 2 */
        input_size->height = atoi(argv[3]); /* index: 3 */
    }
    if (argc != 3) { /* 3 argv num */
        return TD_SUCCESS;
    }
    if ((strlen(argv[2]) != 1)) { /* 2 intf */
        sample_dis_usage(argv[0]);
        return TD_FAILURE;
    }
    switch (*argv[2]) { /* 2 intf */
        case '0':
            break;
        case '1':
            *vo_intf_type = OT_VO_INTF_BT1120;
            break;
        default:
            sample_print("the index is invalid!\n");
            sample_dis_usage(argv[0]);
            return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static void sample_dis_print_exit(td_s32 ret)
{
    if (g_dis_sample_signal_flag == 1) {
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
        exit(-1);
    }

    if (ret == TD_SUCCESS) {
        sample_print("program exit normally!\n");
    } else {
        sample_print("program exit abnormally!\n");
    }
}

#ifdef __LITEOS__
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    td_s32 ret;
    ot_size input_size = {3840, 2160};
    ot_vo_intf_type vo_intf_type = OT_VO_INTF_HDMI;
    if ((argc < 2) || (argc > 4) || (strlen(argv[1]) != 1)) { /* 2 4 argv num */
        sample_dis_usage(argv[0]);
        return TD_FAILURE;
    }
    g_dis_sample_signal_flag = 0;
#ifndef __LITEOS__
    sample_sys_signal(&sample_dis_handle_sig);
#endif
#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif

    ret = sample_dis_parse_optional_args(argc, argv, &vo_intf_type, &input_size);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = sample_dis_proc(argv[0], *argv[1], vo_intf_type, &input_size);
    sample_dis_print_exit(ret);

#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif
    return ret;
}
