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
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "sample_comm.h"
#include "ss_mpi_snap.h"
#include "sdk_module_init.h"

#define check_digit(x) ((x) >= '0' && (x) <= '9')

static volatile sig_atomic_t g_sig_flag = 0;

static sample_sns_type g_sns_type = SENSOR0_TYPE;
static sample_vi_cfg g_vi_cfg;
static ot_vi_pipe g_video_pipe = 0;
static ot_vi_pipe g_snap_pipe = 1;

static sample_vo_cfg g_vo_cfg = {
    .vo_dev            = SAMPLE_VO_DEV_UHD,
    .vo_intf_type      = OT_VO_INTF_HDMI,
    .intf_sync         = OT_VO_OUT_1080P30,
    .bg_color          = COLOR_RGB_BLACK,
    .pix_format        = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420,
    .disp_rect         = {0, 0, 1920, 1080},
    .image_size        = {1920, 1080},
    .vo_part_mode      = OT_VO_PARTITION_MODE_SINGLE,
    .dis_buf_len       = 3, /* 3: def buf len for single */
    .dst_dynamic_range = OT_DYNAMIC_RANGE_SDR8,
    .vo_mode           = VO_MODE_1MUX,
    .compress_mode     = OT_COMPRESS_MODE_NONE,
};

static sample_comm_venc_chn_param g_venc_chn_param = {
    .frame_rate           = 30, /* 30 is a number */
    .stats_time           = 1,  /* 1 is a number */
    .gop                  = 30, /* 30 is a number */
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

static ot_snap_attr g_norm_snap_attr = {
    .snap_type = OT_SNAP_TYPE_NORM,
    .load_ccm_en = TD_TRUE,
    .norm_attr = {
        .frame_cnt         = 2, /* snap 2 frames */
        .repeat_send_times = 1,
        .zsl_en            = TD_FALSE,
    },
};

static td_void sample_get_char(td_void)
{
    if (g_sig_flag == 1) {
        return;
    }

    sample_pause();
}

static td_void sample_vi_get_default_vb_config(ot_size *size, ot_vb_cfg *vb_cfg)
{
    ot_vb_calc_cfg calc_cfg;
    ot_pic_buf_attr buf_attr;

    (td_void)memset_s(vb_cfg, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg));
    vb_cfg->max_pool_cnt = 128; /* 128 blks */

    /* default YUV pool: SP420 + compress_seg */
    buf_attr.width         = size->width;
    buf_attr.height        = size->height;
    buf_attr.align         = OT_DEFAULT_ALIGN;
    buf_attr.bit_width     = OT_DATA_BIT_WIDTH_8;
    buf_attr.pixel_format  = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    buf_attr.compress_mode = OT_COMPRESS_MODE_SEG;
    ot_common_get_pic_buf_cfg(&buf_attr, &calc_cfg);

    vb_cfg->common_pool[0].blk_size = calc_cfg.vb_size;
    vb_cfg->common_pool[0].blk_cnt  = 10; /* 10 blks */

    /* default raw pool: raw12bpp + compress_line */
    buf_attr.pixel_format  = OT_PIXEL_FORMAT_RGB_BAYER_12BPP;
    buf_attr.compress_mode = OT_COMPRESS_MODE_LINE;
    ot_common_get_pic_buf_cfg(&buf_attr, &calc_cfg);
    vb_cfg->common_pool[1].blk_size = calc_cfg.vb_size;
    vb_cfg->common_pool[1].blk_cnt  = 10; /* 10 blks */
}

#ifdef SAMPLE_MEM_SHARE_ENABLE
td_void sample_snap_init_mem_share(td_void)
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

static td_s32 sample_snap_sys_init(ot_vi_vpss_mode_type mode_type, ot_vi_video_mode video_mode,
    sample_sns_type sns_type)
{
    td_s32 ret;
    ot_size size;
    ot_vb_cfg vb_cfg;
    td_u32 supplement_config;

    sample_comm_vi_get_size_by_sns_type(sns_type, &size);
    sample_vi_get_default_vb_config(&size, &vb_cfg);

    supplement_config = OT_VB_SUPPLEMENT_BNR_MOT_MASK | OT_VB_SUPPLEMENT_JPEG_MASK;
    ret = sample_comm_sys_init_with_vb_supplement(&vb_cfg, supplement_config);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

#ifdef SAMPLE_MEM_SHARE_ENABLE
    sample_snap_init_mem_share();
#endif

    ret = sample_comm_vi_set_vi_vpss_mode(mode_type, video_mode);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_snap_start_vpss(ot_vpss_grp grp, ot_size *in_size)
{
    ot_vpss_grp_attr grp_attr;
    ot_vpss_chn_attr chn_attr;
    td_bool chn_enable[OT_VPSS_MAX_PHYS_CHN_NUM] = {TD_TRUE, TD_FALSE, TD_FALSE, TD_FALSE};

    sample_comm_vpss_get_default_grp_attr(&grp_attr);
    grp_attr.max_width  = in_size->width;
    grp_attr.max_height = in_size->height;
    if (grp == 1) {
        grp_attr.nr_attr.nr_type = OT_VPSS_NR_TYPE_SNAP_NORM;
    }
    sample_comm_vpss_get_default_chn_attr(&chn_attr);
    chn_attr.width  = in_size->width;
    chn_attr.height = in_size->height;

    return sample_common_vpss_start(grp, chn_enable, &grp_attr, &chn_attr, OT_VPSS_MAX_PHYS_CHN_NUM);
}

static td_void sample_snap_stop_vpss(ot_vpss_grp grp)
{
    td_bool chn_enable[OT_VPSS_MAX_PHYS_CHN_NUM] = {TD_TRUE, TD_FALSE, TD_FALSE, TD_FALSE};

    sample_common_vpss_stop(grp, chn_enable, OT_VPSS_MAX_PHYS_CHN_NUM);
}

static td_s32 sample_snap_start_venc(ot_venc_chn venc_chn[], td_u32 max_venc_chn_num, td_u32 chn_num)
{
    td_s32 i;
    td_s32 ret;
    ot_size venc_size;

    if (chn_num > max_venc_chn_num) {
        return TD_FAILURE;
    }

    sample_comm_vi_get_size_by_sns_type(g_sns_type, &venc_size);
    g_venc_chn_param.venc_size.width  = venc_size.width;
    g_venc_chn_param.venc_size.height = venc_size.height;
    g_venc_chn_param.size = sample_comm_sys_get_pic_enum(&venc_size);

    for (i = 0; i < (td_s32)chn_num; i++) {
        ret = sample_comm_venc_start(venc_chn[i], &g_venc_chn_param);
        if (ret != TD_SUCCESS) {
            goto exit;
        }
    }

    ret = sample_comm_venc_start_get_stream(venc_chn, chn_num);
    if (ret != TD_SUCCESS) {
        goto exit;
    }

    return TD_SUCCESS;

exit:
    for (i = i - 1; i >= 0; i--) {
        sample_comm_venc_stop(venc_chn[i]);
    }
    return TD_FAILURE;
}

static td_void sample_snap_stop_venc(const ot_venc_chn venc_chn[], td_u32 max_venc_chn_num, td_u32 chn_num)
{
    td_u32 i;

    if (chn_num > max_venc_chn_num) {
        return;
    }

    sample_comm_venc_stop_get_stream(chn_num);

    for (i = 0; i < chn_num; i++) {
        sample_comm_venc_stop(venc_chn[i]);
    }
}

static td_s32 sample_snap_start_vo(sample_vo_mode vo_mode)
{
    g_vo_cfg.vo_mode = vo_mode;

    return sample_comm_vo_start_vo(&g_vo_cfg);
}

static td_void sample_snap_stop_vo(td_void)
{
    sample_comm_vo_stop_vo(&g_vo_cfg);
}

static td_s32 sample_snap_start_venc_and_vo(const ot_vpss_grp vpss_grp[], td_u32 grp_num)
{
    td_u32 i;
    td_s32 ret;
    ot_size venc_size;
    const sample_vo_mode vo_mode = VO_MODE_1MUX;
    const ot_vpss_chn vpss_chn = 0;
    const ot_vo_layer vo_layer = 0;
    const ot_vo_chn vo_chn[4] = {0, 1, 2, 3};     /* 4: max chn num, 0/1/2/3 chn id */
    ot_venc_chn venc_chn[4] = {0, 1, 2, 3}; /* 4: max chn num, 0/1/2/3 chn id */

    ret = sample_snap_start_vo(vo_mode);
    if (ret != TD_SUCCESS) {
        goto start_vo_failed;
    }

    sample_comm_vi_get_size_by_sns_type(g_sns_type, &venc_size);
    ret = sample_snap_start_venc(venc_chn, 4, 1); /* 4: max venc chn num */
    if (ret != TD_SUCCESS) {
        goto start_venc_failed;
    }

    ret = sample_comm_venc_snap_start(venc_chn[1], &venc_size, TD_TRUE);
    if (ret != TD_SUCCESS) {
        goto start_venc_snap_failed;
    }

    for (i = 0; i < grp_num; i++) {
        if (i == 0) {
            sample_comm_vpss_bind_vo(vpss_grp[i], vpss_chn, vo_layer, vo_chn[i]);
        }
        sample_comm_vpss_bind_venc(vpss_grp[i], vpss_chn, venc_chn[i]);
    }

    return TD_SUCCESS;

start_venc_snap_failed:
    sample_snap_stop_venc(venc_chn, 4, 1); /* 4: max venc chn num */
start_venc_failed:
    sample_snap_stop_vo();
start_vo_failed:
    return TD_FAILURE;
}

static td_void sample_snap_stop_venc_and_vo(const ot_vpss_grp vpss_grp[], td_u32 grp_num)
{
    td_u32 i;
    const ot_vpss_chn vpss_chn = 0;
    const ot_vo_layer vo_layer = 0;
    const ot_vo_chn vo_chn[4] = {0, 1, 2, 3};     /* 4: max chn num, 0/1/2/3 chn id */
    const ot_venc_chn venc_chn[4] = {0, 1, 2, 3}; /* 4: max chn num, 0/1/2/3 chn id */

    for (i = 0; i < grp_num; i++) {
        if (i == 0) {
            sample_comm_vpss_un_bind_vo(vpss_grp[i], vpss_chn, vo_layer, vo_chn[i]);
        }
        sample_comm_vpss_un_bind_venc(vpss_grp[i], vpss_chn, venc_chn[i]);
    }

    sample_snap_stop_venc(venc_chn, 4, 1); /* 4: max venc chn num */
    sample_comm_venc_snap_stop(venc_chn[1]);
    sample_snap_stop_vo();
}

static td_s32 sample_snap_start_vi_route(void)
{
    td_s32 ret;

    sample_comm_vi_get_default_vi_cfg(g_sns_type, &g_vi_cfg);
    g_vi_cfg.bind_pipe.pipe_num = 2; /* 2: double pipe */
    g_vi_cfg.bind_pipe.pipe_id[0] = 0;
    g_vi_cfg.bind_pipe.pipe_id[1] = 1;

    (td_void)memcpy_s(&g_vi_cfg.pipe_info[1], sizeof(sample_vi_pipe_info),
        &g_vi_cfg.pipe_info[0], sizeof(sample_vi_pipe_info));

    g_vi_cfg.pipe_info[1].pipe_need_start = TD_FALSE;
    g_vi_cfg.pipe_info[1].isp_need_run = TD_FALSE;

    ret = sample_comm_vi_start_vi(&g_vi_cfg);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return TD_SUCCESS;
}

static td_void sample_snap_stop_vi_route(void)
{
    sample_comm_vi_stop_vi(&g_vi_cfg);
}

static td_s32 sample_snap_start_vpss_venc_vo(td_void)
{
    td_s32 ret;
    const ot_vi_chn vi_chn = 0;
    const ot_vpss_grp vpss_grp[2] = {0, 1};
    const td_u32 grp_num = 2;
    const ot_vpss_chn vpss_chn = 0;
    ot_size in_size;

    sample_comm_vi_bind_vpss(g_video_pipe, vi_chn, vpss_grp[0], vpss_chn);
    sample_comm_vi_bind_vpss(g_snap_pipe, vi_chn, vpss_grp[1], vpss_chn);
    sample_comm_vi_get_size_by_sns_type(g_sns_type, &in_size);
    ret = sample_snap_start_vpss(vpss_grp[0], &in_size);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = sample_snap_start_vpss(vpss_grp[1], &in_size);
    if (ret != TD_SUCCESS) {
        goto start_vpss_failed;
    }

    ret = sample_snap_start_venc_and_vo(vpss_grp, grp_num);
    if (ret != TD_SUCCESS) {
        goto start_venc_vo_failed;
    }

    return TD_SUCCESS;

start_venc_vo_failed:
    sample_snap_stop_vpss(vpss_grp[1]);
start_vpss_failed:
    sample_snap_stop_vpss(vpss_grp[0]);
    return ret;
}

static td_void sample_snap_stop_vpss_venc_vo(td_void)
{
    const ot_vpss_grp vpss_grp[2] = {0, 1};
    const td_u32 grp_num = 2;
    const ot_vi_chn vi_chn = 0;
    const ot_vpss_chn vpss_chn = 0;

    sample_snap_stop_venc_and_vo(vpss_grp, grp_num);
    sample_snap_stop_vpss(vpss_grp[1]);
    sample_snap_stop_vpss(vpss_grp[0]);
    sample_comm_vi_un_bind_vpss(g_snap_pipe, vi_chn, vpss_grp[1], vpss_chn);
    sample_comm_vi_un_bind_vpss(g_video_pipe, vi_chn, vpss_grp[0], vpss_chn);
}

static td_s32 sample_snap_start_snap(td_void)
{
    td_s32 ret;
    const ot_venc_chn venc_chn = 1; /* 1: snap venc */

    ret = ss_mpi_snap_set_pipe_attr(g_snap_pipe, &g_norm_snap_attr);
    if (ret != TD_SUCCESS) {
        printf("ss_mpi_snap_set_pipe_attr failed, ret: 0x%x\n", ret);
        return TD_FAILURE;
    }

    ret = ss_mpi_snap_enable_pipe(g_snap_pipe);
    if (ret != TD_SUCCESS) {
        printf("ss_mpi_snap_enable_pipe failed, ret: 0x%x\n", ret);
        return TD_FAILURE;
    }

    printf("=======press Enter key to trigger=====\n");
    sample_get_char();

    ret = ss_mpi_snap_trigger_pipe(g_snap_pipe);
    if (ret != TD_SUCCESS) {
        printf("ss_mpi_snap_trigger_pipe failed, ret: 0x%x\n", ret);
        goto exit;
    }

    ret = sample_comm_venc_snap_process(venc_chn, g_norm_snap_attr.norm_attr.frame_cnt, TD_TRUE, TD_TRUE);
    if (ret != TD_SUCCESS) {
        printf("snap venc process failed!\n");
        goto exit;
    }

    printf("snap success!\n");
    sample_get_char();

exit:
    ss_mpi_snap_disable_pipe(g_snap_pipe);
    return ret;
}

static td_s32 sample_snap_double_pipe_offline(td_void)
{
    td_s32 ret;
    const ot_vi_vpss_mode_type mode_type = OT_VI_OFFLINE_VPSS_OFFLINE;
    const ot_vi_video_mode video_mode = OT_VI_VIDEO_MODE_NORM;

    ret = sample_snap_sys_init(mode_type, video_mode, g_sns_type);
    if (ret != TD_SUCCESS) {
        goto sys_init_failed;
    }

    ret = sample_snap_start_vi_route();
    if (ret != TD_SUCCESS) {
        goto start_vi_route_failed;
    }

    ret = sample_snap_start_vpss_venc_vo();
    if (ret != TD_SUCCESS) {
        goto start_vpss_venc_vo_failed;
    }

    ret = sample_snap_start_snap();
    if (ret != TD_SUCCESS) {
        goto start_snap_failed;
    }

    sample_get_char();

start_snap_failed:
    sample_snap_stop_vpss_venc_vo();
start_vpss_venc_vo_failed:
    sample_snap_stop_vi_route();
start_vi_route_failed:
    sample_comm_sys_exit();
sys_init_failed:
    return ret;
}

static td_void sample_snap_usage(char *prg_name)
{
    printf("usage : %s <index> \n", prg_name);
    printf("index:\n");
    printf("\t(0) double pipe normal snap capture.\n");
}

static td_void sample_snap_handle_sig(td_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        g_sig_flag = 1;
    }
}

static td_void sample_register_sig_handler(td_void (*sig_handle)(td_s32))
{
    struct sigaction sa;

    (td_void)memset_s(&sa, sizeof(struct sigaction), 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handle;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, TD_NULL);
    sigaction(SIGTERM, &sa, TD_NULL);
}

#ifdef __LITEOS__
td_s32 app_main(td_s32 argc, td_char *argv[])
#else
td_s32 main(td_s32 argc, td_char *argv[])
#endif
{
    td_s32 ret;
    td_u32 index;

    if (argc != 2) { /* 2:arg num */
        sample_snap_usage(argv[0]);
        return TD_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        sample_snap_usage(argv[0]);
        return TD_FAILURE;
    }

    if (strlen(argv[1]) != 1 || !check_digit(argv[1][0])) {
        sample_snap_usage(argv[0]);
        return TD_FAILURE;
    }

#ifndef __LITEOS__
    sample_register_sig_handler(sample_snap_handle_sig);
#endif

#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif

    index = atoi(argv[1]);
    switch (index) {
        case 0:
            ret = sample_snap_double_pipe_offline();
            break;
        default:
            sample_snap_usage(argv[0]);
            ret = TD_FAILURE;
            break;
    }

    if ((ret == TD_SUCCESS) && (g_sig_flag == 0)) {
        printf("\033[0;32mprogram exit normally!\033[0;39m\n");
    } else {
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif

#ifdef __LITEOS__
    return ret;
#else
    exit(ret);
#endif
}
