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

#include "sample_comm.h"
#include "sample_ipc.h"
#include "securec.h"
#include "sdk_module_init.h"

#define BIG_STREAM_SIZE     PIC_3840X2160
#define SMALL_STREAM_SIZE   PIC_1080P

#define VI_VB_YUV_CNT    6
#define VPSS_VB_YUV_CNT  8

#define ENTER_ASCII 10

#define VB_MAX_NUM     10

#define CHN_NUM_MAX    2

typedef struct {
    ot_size            max_size;
    ot_pixel_format    pixel_format;
    ot_size            output_size[OT_VPSS_MAX_PHYS_CHN_NUM];
    ot_compress_mode   compress_mode[OT_VPSS_MAX_PHYS_CHN_NUM];
    td_bool            enable[OT_VPSS_MAX_PHYS_CHN_NUM];
} sample_venc_vpss_chn_attr;

typedef struct {
    td_u32 valid_num;
    td_u64 blk_size[OT_VB_MAX_COMMON_POOLS];
    td_u32 blk_cnt[OT_VB_MAX_COMMON_POOLS];
    td_u32 supplement_config;
} sample_venc_vb_attr;

typedef struct {
    ot_vpss_chn vpss_chn[CHN_NUM_MAX];
    ot_venc_chn venc_chn[CHN_NUM_MAX];
} sample_venc_vpss_chn;

/******************************************************************************
* function : show usage
******************************************************************************/
static td_void sample_venc_usage(td_char *s_prg_nm)
{
    printf("usage : %s [index] [options]\n", s_prg_nm);
    printf("index:\n");
    printf("\t  0) normal           :H.265e@1080P@30fps + h264e@D1@30fps.\n");
    printf("\t  1) qpmap            :H.265e@1080P@30fps + h264e@1080P@30fps.\n");
    printf("\t  2) intra_refresh    :H.265e@1080P@30fps(row) + h264e@1080P@30fps(column).\n");
    printf("\t  3) roi_bg_frame_rate:H.265e@1080P@30fps + H.264@1080P@30fps.\n");
    printf("\t  4) debreath_effect  :H.265e@1080P@30fps(enable) + H.265e@1080P@30fps(disable).\n");
    printf("\t  5) roi_set          :Mjpege@1080P@30fps(user set roi info by API).\n");
    printf("\t  6) roimap           :Mjpege@1080P@30fps(user customize every region).\n");
    printf("options: not necessary\n");
    printf("\t  0) not save heif file.\n");
    printf("\t  1) save heif file.\n");
}

static td_bool g_sample_venc_exit = TD_FALSE;

/******************************************************************************
* function : to process abnormal case
******************************************************************************/
static td_void sample_venc_handle_sig(td_s32 signo)
{
    if (g_sample_venc_exit == TD_TRUE) {
        return;
    }

    if (signo == SIGINT || signo == SIGTERM) {
        g_sample_venc_exit = TD_TRUE;
    }
}

static td_s32 sample_venc_getchar()
{
    td_s32 c;
    if (g_sample_venc_exit == TD_TRUE) {
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
        return 'e';
    }

    c = getchar();

    if (g_sample_venc_exit == TD_TRUE) {
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
        return 'e';
    }

    return c;
}

static td_void sample_venc_flush_input(td_void)
{
    td_s32 c = 0;

    while ((c != ENTER_ASCII) && (c != 'e') && (c != EOF)) {
        c = sample_venc_getchar();
    }
}

static td_s32 get_gop_mode(ot_venc_gop_mode *gop_mode)
{
    td_s32 c[2] = {'\0'}; /* 2: len */

begin_get:
    printf("please input choose gop mode!\n");
    printf("\t 0) normal p.\n");
    printf("\t 1) dual p.\n");
    printf("\t 2) smart p.\n");

    c[0] = sample_venc_getchar();
    if (c[0] == 'e') {
        return TD_FAILURE;
    } else if ((c[0] == ENTER_ASCII) || ((c[0] != '0') && (c[0] != '1') && (c[0] != '2'))) {
        sample_print("invalid input! please try again.\n");
        sample_venc_flush_input();
        goto begin_get;
    }

    c[1] = sample_venc_getchar();
    if (c[1] != ENTER_ASCII) {
        sample_print("invalid input! please try again.\n");
        sample_venc_flush_input();
        goto begin_get;
    }
    switch (c[0]) {
        case '0':
            *gop_mode = OT_VENC_GOP_MODE_NORMAL_P;
            break;

        case '1':
            *gop_mode = OT_VENC_GOP_MODE_DUAL_P;
            break;

        case '2':
            *gop_mode = OT_VENC_GOP_MODE_SMART_P;
            break;

        default:
            sample_print("input invalid!\n");
            goto begin_get;
    }

    return TD_SUCCESS;
}

static td_void print_rc_mode(ot_payload_type type)
{
    printf("please input choose rc mode!\n");
    printf("\t c) cbr.\n");
    printf("\t v) vbr.\n");
    if (type != OT_PT_MJPEG) {
        printf("\t a) avbr.\n");
        printf("\t x) cvbr.\n");
        printf("\t q) qvbr.\n");
    }
    printf("\t f) fix_qp\n");
}

static td_s32 get_rc_mode(ot_payload_type type, sample_rc *rc_mode)
{
    td_s32 c[2] = {'\0'}; /* 2: len */

begin_get:
    if (type != OT_PT_JPEG) {
        print_rc_mode(type);

        c[0] = sample_venc_getchar();
        if (c[0] == 'e') {
            return TD_FAILURE;
        } else if ((c[0] == ENTER_ASCII) || ((c[0] != 'c') && (c[0] != 'v') && (c[0] != 'a') && (c[0] != 'x') &&
            (c[0] != 'q') && (c[0] != 'f'))) {
            sample_print("invalid input! please try again.\n");
            sample_venc_flush_input();
            goto begin_get;
        }
        c[1] = sample_venc_getchar();
        if (c[1] != ENTER_ASCII) {
            sample_print("invalid input! please try again.\n");
            sample_venc_flush_input();
            goto begin_get;
        }
        switch (c[0]) {
            case 'c':
                *rc_mode = SAMPLE_RC_CBR;
                break;

            case 'v':
                *rc_mode = SAMPLE_RC_VBR;
                break;

            case 'a':
                *rc_mode = SAMPLE_RC_AVBR;
                break;

            case 'x':
                *rc_mode = SAMPLE_RC_CVBR;
                break;

            case 'q':
                *rc_mode = SAMPLE_RC_QVBR;
                break;

            case 'f':
                *rc_mode = SAMPLE_RC_FIXQP;
                break;

            default:
                sample_print("input invalid!\n");
                goto begin_get;
        }
    }

    return TD_SUCCESS;
}

static td_s32 get_intra_refresh_mode(ot_venc_intra_refresh_mode *intra_refresh_mode)
{
    td_s32 c[2] = {'\0'}; /* 2: len */

begin_get:
    printf("please input choose intra refresh mode!\n");
    printf("\t r) row.\n");
    printf("\t c) column.\n");

    c[0] = sample_venc_getchar();
    if (c[0] == 'e') {
        return TD_FAILURE;
    } else if ((c[0] == ENTER_ASCII) || ((c[0] != 'c') && (c[0] != 'r'))) {
        sample_print("invalid input! please try again.\n");
        sample_venc_flush_input();
        goto begin_get;
    }
    c[1] = sample_venc_getchar();
    if (c[1] != ENTER_ASCII) {
        sample_print("invalid input! please try again.\n");
        sample_venc_flush_input();
        goto begin_get;
    }
    switch (c[0]) {
        case 'r':
            *intra_refresh_mode = OT_VENC_INTRA_REFRESH_ROW;
            break;

        case 'c':
            *intra_refresh_mode = OT_VENC_INTRA_REFRESH_COLUMN;
            break;

        default:
            sample_print("input invalid!\n");
            goto begin_get;
    }

    return TD_SUCCESS;
}

static td_void get_vb_attr(const ot_size *vi_size, const sample_venc_vpss_chn_attr *vpss_chn_attr,
    sample_venc_vb_attr *vb_attr)
{
    td_s32 i;
    ot_pic_buf_attr pic_buf_attr = {0};

    vb_attr->valid_num = 0;

    // vb for vi-vpss
    pic_buf_attr.width  = vi_size->width;
    pic_buf_attr.height = vi_size->height;
    pic_buf_attr.align = OT_DEFAULT_ALIGN;
    pic_buf_attr.bit_width = OT_DATA_BIT_WIDTH_8;
    pic_buf_attr.pixel_format = OT_PIXEL_FORMAT_YUV_SEMIPLANAR_422;
    pic_buf_attr.compress_mode = OT_COMPRESS_MODE_NONE;
    vb_attr->blk_size[vb_attr->valid_num] = ot_common_get_pic_buf_size(&pic_buf_attr);
    vb_attr->blk_cnt[vb_attr->valid_num] = VI_VB_YUV_CNT;
    vb_attr->valid_num++;

    // vb for vpss-venc(big stream)
    if (vb_attr->valid_num >= OT_VB_MAX_COMMON_POOLS) {
        return;
    }

    for (i = 0; i < OT_VPSS_MAX_PHYS_CHN_NUM && vb_attr->valid_num < OT_VB_MAX_COMMON_POOLS; i++) {
        if (vpss_chn_attr->enable[i] == TD_TRUE) {
            pic_buf_attr.width = vpss_chn_attr->output_size[i].width;
            pic_buf_attr.height = vpss_chn_attr->output_size[i].height;
            pic_buf_attr.align = OT_DEFAULT_ALIGN;
            pic_buf_attr.bit_width = OT_DATA_BIT_WIDTH_8;
            pic_buf_attr.pixel_format = vpss_chn_attr->pixel_format;
            pic_buf_attr.compress_mode = vpss_chn_attr->compress_mode[i];
            if (pic_buf_attr.compress_mode == OT_COMPRESS_MODE_SEG_COMPACT) {
                ot_vb_calc_cfg calc_cfg = {0};
                ot_common_get_vpss_compact_seg_buf_size(&pic_buf_attr, &calc_cfg);
                vb_attr->blk_size[vb_attr->valid_num] = calc_cfg.vb_size;
            } else {
                vb_attr->blk_size[vb_attr->valid_num] = ot_common_get_pic_buf_size(&pic_buf_attr);
            }
            vb_attr->blk_cnt[vb_attr->valid_num] = VPSS_VB_YUV_CNT;

            vb_attr->valid_num++;
        }
    }

    vb_attr->supplement_config = OT_VB_SUPPLEMENT_JPEG_MASK | OT_VB_SUPPLEMENT_BNR_MOT_MASK;
}

static td_void get_default_vpss_chn_attr(ot_size *vi_size, ot_size enc_size[], td_s32 len,
    sample_venc_vpss_chn_attr *vpss_chan_attr)
{
    td_s32 i;
    td_u32 max_width;
    td_u32 max_height;

    if (memset_s(vpss_chan_attr, sizeof(sample_venc_vpss_chn_attr), 0, sizeof(sample_venc_vpss_chn_attr)) != EOK) {
        printf("vpss chn attr call memset_s error\n");
        return;
    }

    max_width = vi_size->width;
    max_height = vi_size->height;

    for (i = 0; (i < len) && (i < OT_VPSS_MAX_PHYS_CHN_NUM); i++) {
        vpss_chan_attr->output_size[i].width = enc_size[i].width;
        vpss_chan_attr->output_size[i].height = enc_size[i].height;
        vpss_chan_attr->compress_mode[i] = (i == 0) ? OT_COMPRESS_MODE_SEG_COMPACT : OT_COMPRESS_MODE_NONE;
        vpss_chan_attr->enable[i] = TD_TRUE;

        max_width = MAX2(max_width, enc_size[i].width);
        max_height = MAX2(max_height, enc_size[i].height);
    }

    vpss_chan_attr->max_size.width = max_width;
    vpss_chan_attr->max_size.height = max_height;
    vpss_chan_attr->pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;

    return;
}

static td_s32 sample_venc_sys_init(sample_venc_vb_attr *vb_attr)
{
    td_u32 i;
    td_s32 ret;
    ot_vb_cfg vb_cfg = {0};

    if (vb_attr->valid_num > OT_VB_MAX_COMMON_POOLS) {
        sample_print("sample_venc_sys_init vb valid num(%d) too large than OT_VB_MAX_COMMON_POOLS(%d)!\n",
            vb_attr->valid_num, OT_VB_MAX_COMMON_POOLS);
        return TD_FAILURE;
    }

    for (i = 0; i < vb_attr->valid_num; i++) {
        vb_cfg.common_pool[i].blk_size = vb_attr->blk_size[i];
        vb_cfg.common_pool[i].blk_cnt = vb_attr->blk_cnt[i];
    }

    vb_cfg.max_pool_cnt = vb_attr->valid_num;

    if (vb_attr->supplement_config == 0) {
        ret = sample_comm_sys_init(&vb_cfg);
    } else {
        ret = sample_comm_sys_init_with_vb_supplement(&vb_cfg, vb_attr->supplement_config);
    }

    if (ret != TD_SUCCESS) {
        sample_print("sample_venc_sys_init failed!\n");
    }

    return ret;
}

static td_s32 sample_venc_vi_init(sample_vi_cfg *vi_cfg)
{
    td_s32 ret;

    ret = sample_comm_vi_start_vi(vi_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vi_start_vi failed: 0x%x\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

static td_void sample_venc_vi_deinit(sample_vi_cfg *vi_cfg)
{
    sample_comm_vi_stop_vi(vi_cfg);
}

static td_s32 sample_venc_vpss_init(ot_vpss_grp vpss_grp, sample_venc_vpss_chn_attr *vpss_chan_cfg)
{
    td_s32 ret;
    ot_vpss_chn vpss_chn;
    ot_vpss_grp_attr grp_attr = { 0 };
    ot_vpss_chn_attr chn_attr[OT_VPSS_MAX_PHYS_CHN_NUM] = {0};

    grp_attr.max_width = vpss_chan_cfg->max_size.width;
    grp_attr.max_height = vpss_chan_cfg->max_size.height;
    grp_attr.nr_en = TD_FALSE;
    grp_attr.dei_mode = OT_VPSS_DEI_MODE_OFF;
    grp_attr.pixel_format = vpss_chan_cfg->pixel_format;
    grp_attr.frame_rate.src_frame_rate = -1;
    grp_attr.frame_rate.dst_frame_rate = -1;

    for (vpss_chn = 0; vpss_chn < OT_VPSS_MAX_PHYS_CHN_NUM; vpss_chn++) {
        if (vpss_chan_cfg->enable[vpss_chn] == 1) {
            chn_attr[vpss_chn].width = vpss_chan_cfg->output_size[vpss_chn].width;
            chn_attr[vpss_chn].height = vpss_chan_cfg->output_size[vpss_chn].height;
            chn_attr[vpss_chn].chn_mode = OT_VPSS_CHN_MODE_USER;
            chn_attr[vpss_chn].compress_mode = vpss_chan_cfg->compress_mode[vpss_chn];
            chn_attr[vpss_chn].pixel_format = vpss_chan_cfg->pixel_format;
            chn_attr[vpss_chn].frame_rate.src_frame_rate = -1;
            chn_attr[vpss_chn].frame_rate.dst_frame_rate = -1;
            chn_attr[vpss_chn].depth = 0;
            chn_attr[vpss_chn].mirror_en = 0;
            chn_attr[vpss_chn].flip_en = 0;
            chn_attr[vpss_chn].aspect_ratio.mode = OT_ASPECT_RATIO_NONE;
        }
    }

    ret = sample_common_vpss_start(vpss_grp, vpss_chan_cfg->enable, &grp_attr, chn_attr, OT_VPSS_MAX_PHYS_CHN_NUM);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
    }

    return ret;
}

static td_void sample_venc_vpss_deinit(ot_vpss_grp vpss_grp, sample_venc_vpss_chn_attr *vpss_chan_cfg)
{
    td_s32 ret;

    ret = sample_common_vpss_stop(vpss_grp, vpss_chan_cfg->enable, OT_VPSS_MAX_PHYS_CHN_NUM);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
    }
}

static td_s32 sample_venc_init_param(ot_size *enc_size, td_s32 chn_num_max, ot_size *vi_size,
    sample_venc_vpss_chn_attr *vpss_param)
{
    td_s32 i;
    td_s32 ret;
    ot_pic_size pic_size[CHN_NUM_MAX] = {BIG_STREAM_SIZE, SMALL_STREAM_SIZE};

    for (i = 0; i < chn_num_max && i < CHN_NUM_MAX; i++) {
        ret = sample_comm_sys_get_pic_size(pic_size[i], &enc_size[i]);
        if (ret != TD_SUCCESS) {
            sample_print("sample_comm_sys_get_pic_size failed!\n");
            return ret;
        }
    }

    // get vpss param
    get_default_vpss_chn_attr(vi_size, enc_size, CHN_NUM_MAX, vpss_param);

    return 0;
}

static td_void sample_venc_set_video_param(sample_comm_venc_chn_param *chn_param,
    ot_venc_gop_attr gop_attr, td_s32 chn_num_max, td_bool qp_map)
{
    td_u32 profile[CHN_NUM_MAX] = {0, 0};
    td_bool share_buf_en = TD_TRUE;
    ot_pic_size pic_size[CHN_NUM_MAX] = {BIG_STREAM_SIZE, SMALL_STREAM_SIZE};
    ot_payload_type payload[CHN_NUM_MAX] = {OT_PT_H265, OT_PT_H264};
    sample_rc rc_mode = 0;

    if (qp_map) {
        rc_mode = SAMPLE_RC_QPMAP;
    } else {
        if (get_rc_mode(payload[0], &rc_mode) != TD_SUCCESS) {
            return;
        }
    }

    if (chn_num_max < 2) { /* 2: chn_param array len */
        sample_print("chn_num_max  %d not enough! should > 2\n", chn_num_max);
        return;
    }

    /* encode h.265 */
    chn_param[0].gop_attr = gop_attr;
    chn_param[0].type = payload[0];
    chn_param[0].size = pic_size[0];
    chn_param[0].rc_mode = rc_mode;
    chn_param[0].profile = profile[0];
    chn_param[0].is_rcn_ref_share_buf = share_buf_en;

    /* encode h.264 */
    chn_param[1].gop_attr = gop_attr;
    chn_param[1].type = payload[1];
    chn_param[1].size = pic_size[1];
    chn_param[1].rc_mode = rc_mode;
    chn_param[1].profile = profile[1];
    chn_param[1].is_rcn_ref_share_buf = share_buf_en;
}

static td_void sample_set_venc_vpss_chn(sample_venc_vpss_chn *venc_vpss_chn)
{
    td_s32 i;

    for (i = 0; i < CHN_NUM_MAX; i++) {
        venc_vpss_chn->vpss_chn[i] = i;
        venc_vpss_chn->venc_chn[i] = i;
    }
}

static td_void sample_venc_unbind_vpss_stop(ot_vpss_grp vpss_grp, const sample_venc_vpss_chn *venc_vpss_chn)
{
    td_s32 i;

    for (i = 0; i < CHN_NUM_MAX; i++) {
        sample_comm_vpss_un_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[i], venc_vpss_chn->venc_chn[i]);
        sample_comm_venc_stop(venc_vpss_chn->venc_chn[i]);
    }
}

static td_void sample_venc_stop(const sample_venc_vpss_chn *venc_vpss_chn)
{
    td_s32 i;

    for (i = 0; i < CHN_NUM_MAX; i++) {
        sample_comm_venc_stop(venc_vpss_chn->venc_chn[i]);
    }
}

static td_s32 sample_venc_normal_start_encode(ot_vpss_grp vpss_grp, sample_venc_vpss_chn *venc_vpss_chn)
{
    td_s32 ret;
    ot_venc_gop_mode gop_mode;
    ot_venc_gop_attr gop_attr;
    sample_comm_venc_chn_param chn_param[CHN_NUM_MAX] = {0};
    sample_comm_venc_chn_param *h265_chn_param = TD_NULL;
    sample_comm_venc_chn_param *h264_chn_param = TD_NULL;

    if (get_gop_mode(&gop_mode) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if ((ret = sample_comm_venc_get_gop_attr(gop_mode, &gop_attr)) != TD_SUCCESS) {
        sample_print("Venc Get GopAttr for %#x!\n", ret);
        return ret;
    }

    sample_venc_set_video_param(chn_param, gop_attr, CHN_NUM_MAX, TD_FALSE);

    /* encode h.265 */
    h265_chn_param = &(chn_param[0]);
    if ((ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[0], h265_chn_param)) != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        return ret;
    }

    ret = sample_comm_vpss_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[0], venc_vpss_chn->venc_chn[0]);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vpss_bind_venc failed for %#x!\n", ret);
        goto EXIT_VENC_H265_STOP;
    }

    /* encode h.264 */
    h264_chn_param = &(chn_param[1]);
    if ((ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[1], h264_chn_param)) != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        goto EXIT_VENC_H265_UnBind;
    }

    ret = sample_comm_vpss_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[1], venc_vpss_chn->venc_chn[1]);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vpss_bind_venc failed for %#x!\n", ret);
        goto EXIT_VENC_H264_STOP;
    }

    /******************************************
     stream save process
    ******************************************/
    if ((ret = sample_comm_venc_start_get_stream(venc_vpss_chn->venc_chn, CHN_NUM_MAX)) != TD_SUCCESS) {
        sample_print("Start Venc failed!\n");
        goto EXIT_VENC_H264_UnBind;
    }

    return TD_SUCCESS;

EXIT_VENC_H264_UnBind:
    sample_comm_vpss_un_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[1], venc_vpss_chn->venc_chn[1]);
EXIT_VENC_H264_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[1]);
EXIT_VENC_H265_UnBind:
    sample_comm_vpss_un_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[0], venc_vpss_chn->venc_chn[0]);
EXIT_VENC_H265_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[0]);

    return ret;
}

static td_void sample_venc_exit_process()
{
    printf("please press twice ENTER to exit this sample\n");
    (td_void)getchar();

    if (g_sample_venc_exit != TD_TRUE) {
        (td_void)getchar();
    }
    sample_comm_venc_stop_get_stream(CHN_NUM_MAX);
}

/******************************************************************************
* function :  H.265e@1080P@30fps + h264e@D1@30fps
******************************************************************************/
static td_s32 sample_venc_normal(td_void)
{
    td_s32 ret;
    sample_sns_type sns_type = SENSOR0_TYPE;
    sample_vi_cfg vi_cfg;
    ot_size enc_size[CHN_NUM_MAX];
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    const ot_vpss_grp vpss_grp = 0;
    sample_venc_vpss_chn_attr vpss_param;
    sample_venc_vb_attr vb_attr = {0};
    sample_venc_vpss_chn venc_vpss_chn = {0};

    sample_set_venc_vpss_chn(&venc_vpss_chn);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    /******************************************
      step 0: related parameter ready
    ******************************************/
    ret = sample_venc_init_param(enc_size, CHN_NUM_MAX, &(vi_cfg.dev_info.dev_attr.in_size), &vpss_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /******************************************
      step 1: init sys alloc common vb
    ******************************************/
    get_vb_attr(&(vi_cfg.dev_info.dev_attr.in_size), &vpss_param, &vb_attr);

    if ((ret = sample_venc_sys_init(&vb_attr)) != TD_SUCCESS) {
        sample_print("Init SYS err for %#x!\n", ret);
        return ret;
    }

    if ((ret = sample_venc_vi_init(&vi_cfg)) != TD_SUCCESS) {
        sample_print("Init VI err for %#x!\n", ret);
        goto EXIT_SYS_STOP;
    }

    if ((ret = sample_venc_vpss_init(vpss_grp, &vpss_param)) != TD_SUCCESS) {
        sample_print("Init VPSS err for %#x!\n", ret);
        goto EXIT_VI_STOP;
    }

    if ((ret = sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0)) != TD_SUCCESS) {
        sample_print("VI Bind VPSS err for %#x!\n", ret);
        goto EXIT_VPSS_STOP;
    }

    if ((ret = sample_venc_normal_start_encode(vpss_grp, &venc_vpss_chn)) != TD_SUCCESS) {
        goto EXIT_VI_VPSS_UNBIND;
    }

    /******************************************
     exit process
    ******************************************/
    sample_venc_exit_process();
    sample_venc_unbind_vpss_stop(vpss_grp, &venc_vpss_chn);

EXIT_VI_VPSS_UNBIND:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0);
EXIT_VPSS_STOP:
    sample_venc_vpss_deinit(vpss_grp, &vpss_param);
EXIT_VI_STOP:
    sample_venc_vi_deinit(&vi_cfg);
EXIT_SYS_STOP:
    sample_comm_sys_exit();

    return ret;
}

static td_s32 sample_venc_qpmap_start_encode(ot_size *enc_size, td_s32 chn_num_max,
    ot_vpss_grp vpss_grp, sample_venc_vpss_chn *venc_vpss_chn)
{
    td_s32 ret;
    ot_venc_gop_mode gop_mode;
    ot_venc_gop_attr gop_attr;
    sample_comm_venc_chn_param chn_param[CHN_NUM_MAX] = {0};
    sample_comm_venc_chn_param *h265_chn_param = TD_NULL;
    sample_comm_venc_chn_param *h264_chn_param = TD_NULL;
    ot_unused(chn_num_max);

    if (get_gop_mode(&gop_mode) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if ((ret = sample_comm_venc_get_gop_attr(gop_mode, &gop_attr)) != TD_SUCCESS) {
        sample_print("Venc Get GopAttr for %#x!\n", ret);
        return ret;
    }

    sample_venc_set_video_param(chn_param, gop_attr, CHN_NUM_MAX, TD_TRUE);

    /* encode h.265 */
    h265_chn_param = &(chn_param[0]);
    ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[0], h265_chn_param);
    if (ret != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        return ret;
    }

    /* encode h.264 */
    h264_chn_param = &(chn_param[1]);
    ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[1], h264_chn_param);
    if (ret != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        goto EXIT_VENC_H265_STOP;
    }

    ret = sample_comm_venc_qpmap_send_frame(vpss_grp, venc_vpss_chn->vpss_chn,
        venc_vpss_chn->venc_chn, CHN_NUM_MAX, enc_size);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_venc_qpmap_send_frame failed for %#x!\n", ret);
        goto EXIT_VENC_H264_STOP;
    }

    /******************************************
     stream save process
    ******************************************/
    if ((ret = sample_comm_venc_start_get_stream(venc_vpss_chn->venc_chn, CHN_NUM_MAX)) != TD_SUCCESS) {
        sample_print("Start Venc failed!\n");
        goto EXIT_VENC_H264_STOP;
    }

    return TD_SUCCESS;

EXIT_VENC_H264_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[1]);
EXIT_VENC_H265_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[0]);

    return ret;
}

static td_void sample_venc_qpmap_exit_process()
{
    printf("please press twice ENTER to exit this sample\n");
    (td_void)getchar();

    if (g_sample_venc_exit != TD_TRUE) {
        (td_void)getchar();
    }

    sample_comm_venc_stop_send_qpmap_frame();
    sample_comm_venc_stop_get_stream(CHN_NUM_MAX);
}

static td_s32 sample_venc_qpmap(td_void)
{
    td_s32 ret;
    sample_sns_type sns_type = SENSOR0_TYPE;
    sample_vi_cfg vi_cfg;
    ot_size enc_size[CHN_NUM_MAX];
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    const ot_vpss_grp vpss_grp = 0;
    sample_venc_vpss_chn_attr vpss_param;
    sample_venc_vb_attr vb_attr = {0};
    sample_venc_vpss_chn venc_vpss_chn = {0};

    sample_set_venc_vpss_chn(&venc_vpss_chn);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    /******************************************
      step 0: related parameter ready
    ******************************************/
    ret = sample_venc_init_param(enc_size, CHN_NUM_MAX, &(vi_cfg.dev_info.dev_attr.in_size), &vpss_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /******************************************
      step 1: init sys alloc common vb
    ******************************************/
    get_vb_attr(&(vi_cfg.dev_info.dev_attr.in_size), &vpss_param, &vb_attr);

    if ((ret = sample_venc_sys_init(&vb_attr)) != TD_SUCCESS) {
        sample_print("Init SYS err for %#x!\n", ret);
        return ret;
    }

    if ((ret = sample_venc_vi_init(&vi_cfg)) != TD_SUCCESS) {
        sample_print("Init VI err for %#x!\n", ret);
        goto EXIT_SYS_STOP;
    }

    if ((ret = sample_venc_vpss_init(vpss_grp, &vpss_param)) != TD_SUCCESS) {
        sample_print("Init VPSS err for %#x!\n", ret);
        goto EXIT_VI_STOP;
    }

    if ((ret = sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0)) != TD_SUCCESS) {
        sample_print("VI Bind VPSS err for %#x!\n", ret);
        goto EXIT_VPSS_STOP;
    }

    /******************************************
    start stream venc
    ******************************************/
    if ((ret = sample_venc_qpmap_start_encode(enc_size, CHN_NUM_MAX, vpss_grp, &venc_vpss_chn)) != TD_SUCCESS) {
        goto EXIT_VI_VPSS_UNBIND;
    }

    /******************************************
     exit process
    ******************************************/
    sample_venc_qpmap_exit_process();
    sample_venc_stop(&venc_vpss_chn);

EXIT_VI_VPSS_UNBIND:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0);
EXIT_VPSS_STOP:
    sample_venc_vpss_deinit(vpss_grp, &vpss_param);
EXIT_VI_STOP:
    sample_venc_vi_deinit(&vi_cfg);
EXIT_SYS_STOP:
    sample_comm_sys_exit();

    return ret;
}

static td_s32 sample_venc_vpss_bind_venc(ot_vpss_grp vpss_grp, ot_vpss_chn vpss_chn, ot_venc_chn venc_chn)
{
    td_s32 ret;

    ret = sample_comm_vpss_bind_venc(vpss_grp, vpss_chn, venc_chn);
    if (ret != TD_SUCCESS) {
        sample_print("call sample_comm_vpss_bind_venc vpss grp %d, vpss chn %d, venc chn %d, ret =  %#x!\n",
            vpss_grp, vpss_chn, venc_chn, ret);
        return ret;
    }

    return TD_SUCCESS;
}

static td_void sample_venc_intra_refresh_param_init(ot_venc_intra_refresh_mode intra_refresh_mode,
    ot_venc_intra_refresh *intra_refresh)
{
    intra_refresh->enable = TD_TRUE;
    intra_refresh->mode = intra_refresh_mode;
    if (intra_refresh_mode == OT_VENC_INTRA_REFRESH_ROW) {
        intra_refresh->refresh_num = 5; /* 5: refresh num */
    } else {
        intra_refresh->refresh_num = 6; /* 6: refresh num */
    }
    intra_refresh->request_i_qp = 30; /* 30: request num */
}

static td_s32 sample_venc_set_intra_refresh(ot_venc_chn venc_chn, ot_venc_intra_refresh_mode intra_refresh_mode)
{
    td_s32 ret;
    ot_venc_intra_refresh intra_refresh = {0};

    if ((ret = ss_mpi_venc_get_intra_refresh(venc_chn, &intra_refresh)) != TD_SUCCESS) {
        sample_print("Get Intra Refresh failed for %#x!\n", ret);
        return ret;
    }

    sample_venc_intra_refresh_param_init(intra_refresh_mode, &intra_refresh);

    if ((ret = ss_mpi_venc_set_intra_refresh(venc_chn, &intra_refresh)) != TD_SUCCESS) {
        sample_print("Set Intra Refresh failed for %#x!\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_venc_get_gop_default_attr(ot_venc_gop_attr *gop_attr)
{
    td_s32 ret;
    ot_venc_gop_mode gop_mode;

    if (get_gop_mode(&gop_mode) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    ret = sample_comm_venc_get_gop_attr(gop_mode, gop_attr);
    if (ret != TD_SUCCESS) {
        sample_print("Venc get gop default attr for mode %d failed return %#x!\n", gop_mode, ret);
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 sample_venc_intra_refresh_set_video_param(ot_venc_intra_refresh_mode *intra_refresh_mode,
    sample_comm_venc_chn_param *chn_param, td_s32 len)
{
    td_s32 ret;
    ot_venc_gop_attr gop_attr;

    if ((ret = sample_comm_venc_get_gop_default_attr(&gop_attr)) != TD_SUCCESS) {
        return ret;
    }

    if (get_intra_refresh_mode(intra_refresh_mode) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if (len > CHN_NUM_MAX) {
        sample_print("the num of venc_create_param is beyond CHN_NUM_MAX !\n");
        return TD_FAILURE;
    }
    sample_venc_set_video_param(chn_param, gop_attr, CHN_NUM_MAX, TD_FALSE);

    return TD_SUCCESS;
}

static td_s32 sample_venc_intra_refresh_start_encode(ot_vpss_grp vpss_grp, sample_venc_vpss_chn *venc_vpss_chn)
{
    td_s32 ret;
    ot_venc_intra_refresh_mode intra_refresh_mode;
    sample_comm_venc_chn_param chn_param[CHN_NUM_MAX] = {0};
    sample_comm_venc_chn_param *h265_chn_param = TD_NULL;
    sample_comm_venc_chn_param *h264_chn_param = TD_NULL;

    ret = sample_venc_intra_refresh_set_video_param(&intra_refresh_mode, chn_param, CHN_NUM_MAX);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /* encode h.265 */
    h265_chn_param = &(chn_param[0]);
    if ((ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[0], h265_chn_param)) != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        return ret;
    }

    ret = sample_venc_vpss_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[0], venc_vpss_chn->venc_chn[0]);
    if (ret != TD_SUCCESS) {
        goto EXIT_VENC_H265_STOP;
    }

    /* set intra refresh mode for chn 0 */
    if ((ret = sample_venc_set_intra_refresh(venc_vpss_chn->venc_chn[0], intra_refresh_mode)) != TD_SUCCESS) {
        goto EXIT_VENC_H265_UnBind;
    }

    /* encode h.264 */
    h264_chn_param = &(chn_param[1]);
    if ((ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[1], h264_chn_param)) != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        goto EXIT_VENC_H265_UnBind;
    }

    ret = sample_venc_vpss_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[1], venc_vpss_chn->venc_chn[1]);
    if (ret != TD_SUCCESS) {
        goto EXIT_VENC_H264_STOP;
    }

    /* set intra refresh mode for chn 1 */
    if ((ret = sample_venc_set_intra_refresh(venc_vpss_chn->venc_chn[1], intra_refresh_mode)) != TD_SUCCESS) {
        goto EXIT_VENC_H264_UnBind;
    }

    /******************************************
     stream save process
    ******************************************/
    if ((ret = sample_comm_venc_start_get_stream(venc_vpss_chn->venc_chn, CHN_NUM_MAX)) != TD_SUCCESS) {
        sample_print("Start Venc failed!\n");
    }

    return ret;

EXIT_VENC_H264_UnBind:
    sample_comm_vpss_un_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[1], venc_vpss_chn->venc_chn[1]);
EXIT_VENC_H264_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[1]);
EXIT_VENC_H265_UnBind:
    sample_comm_vpss_un_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[0], venc_vpss_chn->venc_chn[0]);
EXIT_VENC_H265_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[0]);

    return ret;
}

/******************************************************************************
* function : intra_refresh:H.265e@1080P@30fps(row) + h264e@1080P@30fps(column).
******************************************************************************/
static td_s32 sample_venc_intra_refresh(td_void)
{
    td_s32 ret;
    sample_sns_type sns_type = SENSOR0_TYPE;
    sample_vi_cfg vi_cfg;
    ot_size enc_size[CHN_NUM_MAX];
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    const ot_vpss_grp vpss_grp = 0;
    sample_venc_vpss_chn_attr vpss_param;
    sample_venc_vb_attr vb_attr = {0};
    sample_venc_vpss_chn venc_vpss_chn = {0};

    sample_set_venc_vpss_chn(&venc_vpss_chn);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    /******************************************
      step 0: related parameter ready
    ******************************************/
    ret = sample_venc_init_param(enc_size, CHN_NUM_MAX, &(vi_cfg.dev_info.dev_attr.in_size), &vpss_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /******************************************
      step 1: init sys alloc common vb
    ******************************************/
    get_vb_attr(&(vi_cfg.dev_info.dev_attr.in_size), &vpss_param, &vb_attr);

    if ((ret = sample_venc_sys_init(&vb_attr)) != TD_SUCCESS) {
        sample_print("Init SYS err for %#x!\n", ret);
        return ret;
    }

    if ((ret = sample_venc_vi_init(&vi_cfg)) != TD_SUCCESS) {
        sample_print("Init VI err for %#x!\n", ret);
        goto EXIT_SYS_STOP;
    }

    if ((ret = sample_venc_vpss_init(vpss_grp, &vpss_param)) != TD_SUCCESS) {
        sample_print("Init VPSS err for %#x!\n", ret);
        goto EXIT_VI_STOP;
    }

    if ((ret = sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0)) != TD_SUCCESS) {
        sample_print("VI Bind VPSS err for %#x!\n", ret);
        goto EXIT_VPSS_STOP;
    }

    /******************************************
    start stream venc
    ******************************************/
    ret = sample_venc_intra_refresh_start_encode(vpss_grp, &venc_vpss_chn);
    if (ret != TD_SUCCESS) {
        goto EXIT_VI_VPSS_UNBIND;
    }

    /******************************************
     exit process
    ******************************************/
    sample_venc_exit_process();
    sample_venc_unbind_vpss_stop(vpss_grp, &venc_vpss_chn);

EXIT_VI_VPSS_UNBIND:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0);
EXIT_VPSS_STOP:
    sample_venc_vpss_deinit(vpss_grp, &vpss_param);
EXIT_VI_STOP:
    sample_venc_vi_deinit(&vi_cfg);
EXIT_SYS_STOP:
    sample_comm_sys_exit();

    return ret;
}

static td_void sample_venc_roi_attr_init(ot_venc_roi_attr *roi_attr)
{
    roi_attr->is_abs_qp = TD_TRUE;
    roi_attr->enable = TD_TRUE;
    roi_attr->qp = 30; /* 30: qp value */
    roi_attr->idx = 0;
    roi_attr->rect.x = 64; /* 64: rect.x value */
    roi_attr->rect.y = 64; /* 64: rect.y value */
    roi_attr->rect.height = 256; /* 256: rect.height value */
    roi_attr->rect.width = 256; /* 256: rect.width value */
}

static td_void sample_venc_roi_bg_frame_rate_init(ot_venc_roi_bg_frame_rate *roi_bg_frame_rate)
{
    roi_bg_frame_rate->src_frame_rate = 30; /* 30: src_frame_rate value */
    roi_bg_frame_rate->dst_frame_rate = 15; /* 15: dst_frame_rate value */
}

static td_s32 sample_venc_set_roi_attr(ot_venc_chn venc_chn)
{
    td_s32 ret;
    ot_venc_roi_attr roi_attr;
    ot_venc_roi_bg_frame_rate roi_bg_frame_rate;

    if ((ret = ss_mpi_venc_get_roi_attr(venc_chn, 0, &roi_attr)) != TD_SUCCESS) {   /* 0: roi index */
        sample_print("chn %d Get Roi Attr failed for %#x!\n", venc_chn, ret);
        return ret;
    }

    sample_venc_roi_attr_init(&roi_attr);

    if ((ret = ss_mpi_venc_set_roi_attr(venc_chn, &roi_attr)) != TD_SUCCESS) {
        sample_print("chn %d Set Roi Attr failed for %#x!\n", venc_chn, ret);
        return ret;
    }

    if ((ret = ss_mpi_venc_get_roi_bg_frame_rate(venc_chn, &roi_bg_frame_rate)) != TD_SUCCESS) {
        sample_print("chn %d Get Roi BgFrameRate failed for %#x!\n", venc_chn, ret);
        return ret;
    }

    sample_venc_roi_bg_frame_rate_init(&roi_bg_frame_rate);

    if ((ret = ss_mpi_venc_set_roi_bg_frame_rate(venc_chn, &roi_bg_frame_rate)) != TD_SUCCESS) {
        sample_print("chn %d Set Roi BgFrameRate failed for %#x!\n", venc_chn, ret);
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 sample_venc_roi_bg_start_encode(ot_vpss_grp vpss_grp, sample_venc_vpss_chn *venc_vpss_chn)
{
    td_s32 ret;
    ot_venc_gop_attr gop_attr;
    sample_comm_venc_chn_param chn_param[CHN_NUM_MAX] = {0};
    sample_comm_venc_chn_param *h265_chn_param = TD_NULL;
    sample_comm_venc_chn_param *h264_chn_param = TD_NULL;

    if ((ret = sample_comm_venc_get_gop_default_attr(&gop_attr)) != TD_SUCCESS) {
        return ret;
    }

    sample_venc_set_video_param(chn_param, gop_attr, CHN_NUM_MAX, TD_FALSE);

    /* encode h.265 */
    h265_chn_param = &(chn_param[0]);
    if ((ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[0], h265_chn_param)) != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        return ret;
    }

    ret = sample_venc_vpss_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[0], venc_vpss_chn->venc_chn[0]);
    if (ret != TD_SUCCESS) {
        goto EXIT_VENC_H265_STOP;
    }

    /* set roi bg frame rate for chn 0 */
    if ((ret = sample_venc_set_roi_attr(venc_vpss_chn->venc_chn[0])) != TD_SUCCESS) {
        goto EXIT_VENC_H265_UnBind;
    }

    /* encode h.264 */
    h264_chn_param = &(chn_param[1]);
    if ((ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[1], h264_chn_param)) != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        goto EXIT_VENC_H265_UnBind;
    }

    ret = sample_venc_vpss_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[1], venc_vpss_chn->venc_chn[1]);
    if (ret != TD_SUCCESS) {
        goto EXIT_VENC_H264_STOP;
    }

    /* set roi bg frame rate for chn 1 */
    if ((ret = sample_venc_set_roi_attr(venc_vpss_chn->venc_chn[1])) != TD_SUCCESS) {
        goto EXIT_VENC_H264_UnBind;
    }

    /******************************************
     stream save process
    ******************************************/
    if ((ret = sample_comm_venc_start_get_stream(venc_vpss_chn->venc_chn, CHN_NUM_MAX)) != TD_SUCCESS) {
        sample_print("Start Venc failed!\n");
        goto EXIT_VENC_H264_UnBind;
    }

    return ret;

EXIT_VENC_H264_UnBind:
    sample_comm_vpss_un_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[1], venc_vpss_chn->venc_chn[1]);
EXIT_VENC_H264_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[1]);
EXIT_VENC_H265_UnBind:
    sample_comm_vpss_un_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[0], venc_vpss_chn->venc_chn[0]);
EXIT_VENC_H265_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[0]);

    return ret;
}

/******************************************************************************
* function : roi_bg_frame_rate:H.265e@1080P@30fps + H.264@1080P@30fps.
******************************************************************************/
static td_s32 sample_venc_roi_bg(td_void)
{
    td_s32 ret;
    sample_sns_type sns_type = SENSOR0_TYPE;
    sample_vi_cfg vi_cfg;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    const ot_vpss_grp vpss_grp = 0;
    sample_venc_vpss_chn_attr vpss_param;
    sample_venc_vb_attr vb_attr = {0};
    ot_size enc_size[CHN_NUM_MAX];
    sample_venc_vpss_chn venc_vpss_chn = {0};

    sample_set_venc_vpss_chn(&venc_vpss_chn);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    /******************************************
      step 0: related parameter ready
    ******************************************/
    ret = sample_venc_init_param(enc_size, CHN_NUM_MAX, &(vi_cfg.dev_info.dev_attr.in_size), &vpss_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /******************************************
      step 1: init sys alloc common vb
    ******************************************/
    get_vb_attr(&(vi_cfg.dev_info.dev_attr.in_size), &vpss_param, &vb_attr);

    if ((ret = sample_venc_sys_init(&vb_attr)) != TD_SUCCESS) {
        sample_print("Init SYS err for %#x!\n", ret);
        return ret;
    }

    if ((ret = sample_venc_vi_init(&vi_cfg)) != TD_SUCCESS) {
        sample_print("Init VI err for %#x!\n", ret);
        goto EXIT_SYS_STOP;
    }

    if ((ret = sample_venc_vpss_init(vpss_grp, &vpss_param)) != TD_SUCCESS) {
        sample_print("Init VPSS err for %#x!\n", ret);
        goto EXIT_VI_STOP;
    }

    if ((ret = sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0)) != TD_SUCCESS) {
        sample_print("VI Bind VPSS err for %#x!\n", ret);
        goto EXIT_VPSS_STOP;
    }

    /******************************************
    start stream venc
    ******************************************/
    if ((ret = sample_venc_roi_bg_start_encode(vpss_grp, &venc_vpss_chn)) != TD_SUCCESS) {
        goto EXIT_VI_VPSS_UNBIND;
    }

    /******************************************
     exit process
    ******************************************/
    sample_venc_exit_process();
    sample_venc_unbind_vpss_stop(vpss_grp, &venc_vpss_chn);

EXIT_VI_VPSS_UNBIND:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0);
EXIT_VPSS_STOP:
    sample_venc_vpss_deinit(vpss_grp, &vpss_param);
EXIT_VI_STOP:
    sample_venc_vi_deinit(&vi_cfg);
EXIT_SYS_STOP:
    sample_comm_sys_exit();

    return ret;
}

static td_s32 sample_venc_set_debreath_effect(ot_venc_chn venc_chn, td_bool enable)
{
    td_s32 ret;
    ot_venc_debreath_effect debreath_effect;

    if ((ret = ss_mpi_venc_get_debreath_effect(venc_chn, &debreath_effect)) != TD_SUCCESS) {
        sample_print("Get debreath_effect failed for %#x!\n", ret);
        return ret;
    }

    if (enable) {
        debreath_effect.enable = TD_TRUE;
        debreath_effect.strength0 = 3; /* 3 : param */
        debreath_effect.strength1 = 20; /* 20 : param */
    } else {
        debreath_effect.enable = TD_FALSE;
    }

    if ((ret = ss_mpi_venc_set_debreath_effect(venc_chn, &debreath_effect)) != TD_SUCCESS) {
        sample_print("Set debreath_effect failed for %#x!\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 sample_venc_debreath_effect_start_encode(ot_vpss_grp vpss_grp, sample_venc_vpss_chn *venc_vpss_chn)
{
    td_s32 ret;
    ot_venc_gop_attr gop_attr;
    sample_comm_venc_chn_param chn_param[CHN_NUM_MAX] = {0};
    sample_comm_venc_chn_param *h265_chn_param = TD_NULL;
    sample_comm_venc_chn_param *h264_chn_param = TD_NULL;

    if ((ret = sample_comm_venc_get_gop_default_attr(&gop_attr)) != TD_SUCCESS) {
        return ret;
    }

    sample_venc_set_video_param(chn_param, gop_attr, CHN_NUM_MAX, TD_FALSE);

    /* encode h.265 */
    h265_chn_param = &(chn_param[0]);
    if ((ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[0], h265_chn_param)) != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        return ret;
    }

    ret = sample_venc_vpss_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[0], venc_vpss_chn->venc_chn[0]);
    if (ret != TD_SUCCESS) {
        goto EXIT_VENC_H265_STOP;
    }

    /* set intra refresh mode for chn 0 */
    if ((ret = sample_venc_set_debreath_effect(venc_vpss_chn->venc_chn[0], TD_TRUE)) != TD_SUCCESS) {
        goto EXIT_VENC_H265_UnBind;
    }

    /* encode h.264 */
    h264_chn_param = &(chn_param[1]);
    if ((ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[1], h264_chn_param)) != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        goto EXIT_VENC_H265_UnBind;
    }

    ret = sample_venc_vpss_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[1], venc_vpss_chn->venc_chn[1]);
    if (ret != TD_SUCCESS) {
        goto EXIT_VENC_H264_STOP;
    }

    /* set intra refresh mode for chn 1 */
    if ((ret = sample_venc_set_debreath_effect(venc_vpss_chn->venc_chn[1], TD_FALSE)) != TD_SUCCESS) {
        goto EXIT_VENC_H264_UnBind;
    }

    /******************************************
     stream save process
    ******************************************/
    if ((ret = sample_comm_venc_start_get_stream(venc_vpss_chn->venc_chn, CHN_NUM_MAX)) != TD_SUCCESS) {
        sample_print("Start Venc failed!\n");
        goto EXIT_VENC_H264_UnBind;
    }

    return TD_SUCCESS;

EXIT_VENC_H264_UnBind:
    sample_comm_vpss_un_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[1], venc_vpss_chn->venc_chn[1]);
EXIT_VENC_H264_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[1]);
EXIT_VENC_H265_UnBind:
    sample_comm_vpss_un_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[0], venc_vpss_chn->venc_chn[0]);
EXIT_VENC_H265_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[0]);

    return ret;
}

static td_s32 sample_venc_debreath_effect(td_void)
{
    td_s32 ret;
    sample_sns_type sns_type = SENSOR0_TYPE;
    sample_vi_cfg vi_cfg;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    const ot_vpss_grp vpss_grp = 0;
    sample_venc_vpss_chn_attr vpss_param;
    sample_venc_vb_attr vb_attr = {0};
    ot_size enc_size[CHN_NUM_MAX];
    sample_venc_vpss_chn venc_vpss_chn = {0};

    sample_set_venc_vpss_chn(&venc_vpss_chn);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);
    /******************************************
      step 0: related parameter ready
    ******************************************/
    ret = sample_venc_init_param(enc_size, CHN_NUM_MAX, &(vi_cfg.dev_info.dev_attr.in_size), &vpss_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /******************************************
      step 1: init sys alloc common vb
    ******************************************/
    get_vb_attr(&(vi_cfg.dev_info.dev_attr.in_size), &vpss_param, &vb_attr);

    if ((ret = sample_venc_sys_init(&vb_attr)) != TD_SUCCESS) {
        sample_print("Init SYS err for %#x!\n", ret);
        return ret;
    }

    if ((ret = sample_venc_vi_init(&vi_cfg)) != TD_SUCCESS) {
        sample_print("Init VI err for %#x!\n", ret);
        goto EXIT_SYS_STOP;
    }

    if ((ret = sample_venc_vpss_init(vpss_grp, &vpss_param)) != TD_SUCCESS) {
        sample_print("Init VPSS err for %#x!\n", ret);
        goto EXIT_VI_STOP;
    }

    if ((ret = sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0)) != TD_SUCCESS) {
        sample_print("VI Bind VPSS err for %#x!\n", ret);
        goto EXIT_VPSS_STOP;
    }

    /******************************************
    start stream venc
    ******************************************/
    ret = sample_venc_debreath_effect_start_encode(vpss_grp, &venc_vpss_chn);
    if (ret != TD_SUCCESS) {
        goto EXIT_VI_VPSS_UNBIND;
    }

    /******************************************
     exit process
    ******************************************/
    sample_venc_exit_process();
    sample_venc_unbind_vpss_stop(vpss_grp, &venc_vpss_chn);

EXIT_VI_VPSS_UNBIND:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0);
EXIT_VPSS_STOP:
    sample_venc_vpss_deinit(vpss_grp, &vpss_param);
EXIT_VI_STOP:
    sample_venc_vi_deinit(&vi_cfg);
EXIT_SYS_STOP:
    sample_comm_sys_exit();

    return ret;
}

static td_void sample_venc_set_jpeg_param(sample_comm_venc_chn_param *venc_create_param,
    ot_venc_gop_attr gop_attr, td_s32 chn_num_max)
{
    td_u32 profile[CHN_NUM_MAX] = {0, 0};
    ot_pic_size pic_size[CHN_NUM_MAX] = {BIG_STREAM_SIZE, SMALL_STREAM_SIZE};
    td_bool share_buf_en = TD_FALSE;
    ot_payload_type payload[CHN_NUM_MAX] = {OT_PT_MJPEG, OT_PT_MJPEG};
    sample_rc rc_mode = 0;

    if (get_rc_mode(payload[0], &rc_mode) != TD_SUCCESS) {
        return;
    }
    if (chn_num_max < 2) { /* 2: venc_create_param array len */
        sample_print("chn_num_max  %d not enough! should > 2\n", chn_num_max);
        return;
    }

    venc_create_param[0].gop_attr                  = gop_attr;
    venc_create_param[0].type                      = payload[0];
    venc_create_param[0].size                      = pic_size[0];
    venc_create_param[0].rc_mode                   = rc_mode;
    venc_create_param[0].profile                   = profile[0];
    venc_create_param[0].is_rcn_ref_share_buf      = share_buf_en;

    venc_create_param[1].gop_attr                  = gop_attr;
    venc_create_param[1].type                      = payload[1];
    venc_create_param[1].size                      = pic_size[1];
    venc_create_param[1].rc_mode                   = rc_mode;
    venc_create_param[1].profile                   = profile[1];
    venc_create_param[1].is_rcn_ref_share_buf      = share_buf_en;
}

static td_void sample_venc_mjpeg_roi_param_init(ot_venc_jpeg_roi_attr *roi_param)
{
    roi_param->idx = 0;
    roi_param->enable = TD_TRUE;
    roi_param->level = 0;
    roi_param->rect.x = 0;
    roi_param->rect.y = 0;
    roi_param->rect.width = 1280; /* 1280: rect.width value */
    roi_param->rect.height = 720; /* 720: rect.height value */
}

static td_s32 sample_venc_set_mjpeg_roi(ot_venc_chn venc_chn)
{
    td_s32 ret;
    const td_u32 idx = 0;
    ot_venc_jpeg_roi_attr roi_param;

    if ((ret = ss_mpi_venc_get_jpeg_roi_attr(venc_chn, idx, &roi_param)) != TD_SUCCESS) {
        sample_print("Get roi_param failed for %#x!\n", ret);
        return ret;
    }

    sample_venc_mjpeg_roi_param_init(&roi_param);

    if ((ret = ss_mpi_venc_set_jpeg_roi_attr(venc_chn, &roi_param)) != TD_SUCCESS) {
        sample_print("Set roi_param failed for %#x!\n", ret);
    }

    return ret;
}

static td_s32 sample_venc_mjpeg_roi_set_start_encode(ot_vpss_grp vpss_grp, sample_venc_vpss_chn *venc_vpss_chn)
{
    td_s32 ret;
    ot_venc_gop_attr gop_attr;
    sample_comm_venc_chn_param venc_create_param[CHN_NUM_MAX] = {0};
    sample_comm_venc_chn_param venc_chn0_param;
    sample_comm_venc_chn_param venc_chn1_param;

    if ((ret = sample_comm_venc_get_gop_attr(OT_VENC_GOP_MODE_NORMAL_P, &gop_attr)) != TD_SUCCESS) {
        return ret;
    }

    sample_venc_set_jpeg_param(venc_create_param, gop_attr, CHN_NUM_MAX);

    venc_chn0_param = venc_create_param[0];
    ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[0], &venc_chn0_param);
    if (ret != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        return ret;
    }

    ret = sample_venc_vpss_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[0], venc_vpss_chn->venc_chn[0]);
    if (ret != TD_SUCCESS) {
        goto EXIT_VENC_CHN0_STOP;
    }

    /* set intra refresh mode for chn 0 */
    if ((ret = sample_venc_set_mjpeg_roi(venc_vpss_chn->venc_chn[0])) != TD_SUCCESS) {
        goto EXIT_VENC_CHN_0_UnBind;
    }

    venc_chn1_param = venc_create_param[1];
    ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[1], &venc_chn1_param);
    if (ret != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        goto EXIT_VENC_CHN_0_UnBind;
    }

    ret = sample_venc_vpss_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[1], venc_vpss_chn->venc_chn[1]);
    if (ret != TD_SUCCESS) {
        goto EXIT_VENC_CHN_1_STOP;
    }

    /******************************************
     stream save process
    ******************************************/
    if ((ret = sample_comm_venc_start_get_stream(venc_vpss_chn->venc_chn, CHN_NUM_MAX)) != TD_SUCCESS) {
        goto EXIT_VENC_CHN_1_UnBind;
    }

    return TD_SUCCESS;

EXIT_VENC_CHN_1_UnBind:
    sample_comm_vpss_un_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[1], venc_vpss_chn->venc_chn[1]);
EXIT_VENC_CHN_1_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[1]);
EXIT_VENC_CHN_0_UnBind:
    sample_comm_vpss_un_bind_venc(vpss_grp, venc_vpss_chn->vpss_chn[0], venc_vpss_chn->venc_chn[0]);
EXIT_VENC_CHN0_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[0]);

    return ret;
}

static td_s32 sample_venc_mjpeg_roi_set(td_void)
{
    td_s32 ret;
    sample_sns_type sns_type = SENSOR0_TYPE;
    sample_vi_cfg vi_cfg;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    const ot_vpss_grp vpss_grp = 0;
    sample_venc_vpss_chn_attr vpss_param;
    sample_venc_vb_attr vb_attr = {0};
    ot_size enc_size[CHN_NUM_MAX];
    sample_venc_vpss_chn venc_vpss_chn = {0};

    sample_set_venc_vpss_chn(&venc_vpss_chn);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);

    /******************************************
      step 0: related parameter ready
    ******************************************/
    ret = sample_venc_init_param(enc_size, CHN_NUM_MAX, &(vi_cfg.dev_info.dev_attr.in_size), &vpss_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /******************************************
      step 1: init sys alloc common vb
    ******************************************/
    get_vb_attr(&(vi_cfg.dev_info.dev_attr.in_size), &vpss_param, &vb_attr);

    if ((ret = sample_venc_sys_init(&vb_attr)) != TD_SUCCESS) {
        return ret;
    }

    if ((ret = sample_venc_vi_init(&vi_cfg)) != TD_SUCCESS) {
        goto EXIT_SYS_STOP;
    }

    if ((ret = sample_venc_vpss_init(vpss_grp, &vpss_param)) != TD_SUCCESS) {
        goto EXIT_VI_STOP;
    }

    if ((ret = sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0)) != TD_SUCCESS) {
        goto EXIT_VPSS_STOP;
    }

    /******************************************
    start stream venc
    ******************************************/
    ret = sample_venc_mjpeg_roi_set_start_encode(vpss_grp, &venc_vpss_chn);
    if (ret != TD_SUCCESS) {
        goto EXIT_VI_VPSS_UNBIND;
    }

    /******************************************
     exit process
    ******************************************/
    sample_venc_exit_process();
    sample_venc_unbind_vpss_stop(vpss_grp, &venc_vpss_chn);

EXIT_VI_VPSS_UNBIND:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0);
EXIT_VPSS_STOP:
    sample_venc_vpss_deinit(vpss_grp, &vpss_param);
EXIT_VI_STOP:
    sample_venc_vi_deinit(&vi_cfg);
EXIT_SYS_STOP:
    sample_comm_sys_exit();

    return ret;
}

static td_void sample_venc_mjpeg_roi_attr_init(ot_venc_jpeg_roi_attr *roi_attr, td_s32 chn_num_max)
{
    if (chn_num_max < 2) {  /* 2: roi_attr array len */
        sample_print("roi_attr array len not enough, need 2, current %d!\n", chn_num_max);
        return;
    }
    roi_attr[0].enable = 1;
    roi_attr[0].idx = 0;
    roi_attr[0].rect.x = 0;
    roi_attr[0].rect.y = 0;
    roi_attr[0].rect.width = 160; /* 160: rect.width value */
    roi_attr[0].rect.height = 160; /* 160: rect.height value */
    roi_attr[0].level = 0;

    roi_attr[1].enable = 1;
    roi_attr[1].idx = 0;
    roi_attr[1].rect.x = 0;
    roi_attr[1].rect.y = 0;
    roi_attr[1].rect.width = 160; /* 160: rect.width value */
    roi_attr[1].rect.height = 160; /* 160: rect.height value */
    roi_attr[1].level = 0;
}

static td_s32 sample_venc_mjpeg_roimap_start_encode(ot_vpss_grp vpss_grp, ot_venc_jpeg_roi_attr *roi_attr,
    ot_size *enc_size, td_s32 chn_num_max, sample_venc_vpss_chn *venc_vpss_chn)
{
    td_s32 ret;
    ot_venc_gop_attr gop_attr;
    sample_comm_venc_chn_param venc_create_param[CHN_NUM_MAX] = {0};
    sample_comm_venc_chn_param venc_chn0_param;
    sample_comm_venc_chn_param venc_chn1_param;
    sample_venc_roimap_chn_info roimap_chn_info = {0};
    ot_unused(chn_num_max);

    if ((ret = sample_comm_venc_get_gop_attr(OT_VENC_GOP_MODE_NORMAL_P, &gop_attr)) != TD_SUCCESS) {
        sample_print("Venc Get GopAttr for %#x!\n", ret);
        return ret;
    }

    sample_venc_set_jpeg_param(venc_create_param, gop_attr, CHN_NUM_MAX);

    venc_chn0_param = venc_create_param[0];
    ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[0], &venc_chn0_param);
    if (ret != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        return ret;
    }

    venc_chn1_param = venc_create_param[1];
    ret = sample_comm_venc_start(venc_vpss_chn->venc_chn[1], &venc_chn1_param);
    if (ret != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        goto EXIT_VENC_CHN0_STOP;
    }

    roimap_chn_info.vpss_chn = venc_vpss_chn->vpss_chn;
    roimap_chn_info.venc_chn = venc_vpss_chn->venc_chn;
    roimap_chn_info.cnt = CHN_NUM_MAX;
    ret = sample_comm_venc_send_roimap_frame(vpss_grp, roimap_chn_info, enc_size, roi_attr);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_venc_qpmap_send_frame failed for %#x!\n", ret);
        goto EXIT_VENC_CHN1_STOP;
    }

    /******************************************
     stream save process
    ******************************************/
    if ((ret = sample_comm_venc_start_get_stream(venc_vpss_chn->venc_chn, CHN_NUM_MAX)) != TD_SUCCESS) {
        sample_print("Start Venc failed!\n");
        goto EXIT_VENC_CHN1_STOP;
    }

    return TD_SUCCESS;

EXIT_VENC_CHN1_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[1]);
EXIT_VENC_CHN0_STOP:
    sample_comm_venc_stop(venc_vpss_chn->venc_chn[0]);

    return ret;
}

static td_void sample_venc_mjpeg_roimap_exit_process()
{
    printf("please press twice ENTER to exit this sample\n");
    (td_void)getchar();

    if (g_sample_venc_exit != TD_TRUE) {
        (td_void)getchar();
    }

    sample_comm_venc_stop_send_roimap_frame();
    sample_comm_venc_stop_get_stream(CHN_NUM_MAX);
}

static td_s32 sample_venc_mjpeg_roimap(td_void)
{
    td_s32 ret;
    sample_sns_type sns_type = SENSOR0_TYPE;
    sample_vi_cfg vi_cfg;
    const ot_vi_pipe vi_pipe = 0;
    const ot_vi_chn vi_chn = 0;
    const ot_vpss_grp vpss_grp = 0;
    sample_venc_vpss_chn_attr vpss_param;
    sample_venc_vb_attr vb_attr = {0};
    ot_venc_jpeg_roi_attr roi_attr[CHN_NUM_MAX];
    ot_size enc_size[CHN_NUM_MAX];
    sample_venc_vpss_chn venc_vpss_chn = {0};

    sample_set_venc_vpss_chn(&venc_vpss_chn);
    sample_comm_vi_get_default_vi_cfg(sns_type, &vi_cfg);

    sample_venc_mjpeg_roi_attr_init(roi_attr, CHN_NUM_MAX);

    ret = sample_venc_init_param(enc_size, CHN_NUM_MAX, &(vi_cfg.dev_info.dev_attr.in_size), &vpss_param);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /******************************************
      step 1: init sys alloc common vb
    ******************************************/
    get_vb_attr(&(vi_cfg.dev_info.dev_attr.in_size), &vpss_param, &vb_attr);

    if ((ret = sample_venc_sys_init(&vb_attr)) != TD_SUCCESS) {
        return ret;
    }

    if ((ret = sample_venc_vi_init(&vi_cfg)) != TD_SUCCESS) {
        goto EXIT_SYS_STOP;
    }

    if ((ret = sample_venc_vpss_init(vpss_grp, &vpss_param)) != TD_SUCCESS) {
        goto EXIT_VI_STOP;
    }

    if ((ret = sample_comm_vi_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0)) != TD_SUCCESS) {
        goto EXIT_VPSS_STOP;
    }

    ret = sample_venc_mjpeg_roimap_start_encode(vpss_grp, roi_attr, enc_size, CHN_NUM_MAX, &venc_vpss_chn);
    if (ret != TD_SUCCESS) {
        goto EXIT_VI_VPSS_UNBIND;
    }

    /******************************************
     exit process
    ******************************************/
    sample_venc_mjpeg_roimap_exit_process();
    sample_venc_stop(&venc_vpss_chn);

EXIT_VI_VPSS_UNBIND:
    sample_comm_vi_un_bind_vpss(vi_pipe, vi_chn, vpss_grp, 0);
EXIT_VPSS_STOP:
    sample_venc_vpss_deinit(vpss_grp, &vpss_param);
EXIT_VI_STOP:
    sample_venc_vi_deinit(&vi_cfg);
EXIT_SYS_STOP:
    sample_comm_sys_exit();

    return ret;
}

static td_s32 sample_venc_choose_mode(td_u32 index)
{
    td_s32 ret;

    switch (index) {
        case 0: /* 0: mode 0 */
            ret = sample_venc_normal();
            break;

        case 1: /* 1: mode 1 */
            ret = sample_venc_qpmap();
            break;

        case 2: /* 2: mode 2 */
            ret = sample_venc_intra_refresh();
            break;

        case 3: /* 3: mode 3 */
            ret = sample_venc_roi_bg();
            break;

        case 4: /* 4: mode 4 */
            ret = sample_venc_debreath_effect();
            break;

        case 5: /* 5: mode 5 */
            ret = sample_venc_mjpeg_roi_set();
            break;

        case 6: /* 6: mode 6 */
            ret = sample_venc_mjpeg_roimap();
            break;

        default:
            printf("the index is invalid!\n");
            return TD_FAILURE;
    }

    return ret;
}

static td_s32 sample_venc_msg_proc_vb_pool_share(td_s32 pid)
{
    td_u32 i;
    ot_vb_common_pools_id pools_id = {0};

    if (ss_mpi_vb_get_common_pool_id(&pools_id) != TD_SUCCESS) {
        sample_print("get common pool_id failed!\n");
        return TD_FAILURE;
    }
    for (i = 0; i < pools_id.pool_cnt; ++i) {
        if (ss_mpi_vb_pool_share(pools_id.pool[i], pid) != TD_SUCCESS) {
            sample_print("vb pool share failed!\n");
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static td_void sample_venc_msg_proc_vb_pool_unshare(td_s32 pid)
{
    td_u32 i;
    ot_vb_common_pools_id pools_id = {0};

    if (ss_mpi_vb_get_common_pool_id(&pools_id) == TD_SUCCESS) {
        for (i = 0; i < pools_id.pool_cnt; ++i) {
            ss_mpi_vb_pool_unshare(pools_id.pool[i], pid);
        }
    }
}

static td_s32 sample_venc_ipc_msg_proc(const sample_ipc_msg_req_buf *msg_req_buf,
    td_bool *is_need_fb, sample_ipc_msg_res_buf *msg_res_buf)
{
    td_s32 ret;

    if (msg_req_buf == TD_NULL || is_need_fb == TD_NULL) {
        return TD_FAILURE;
    }

    /* need feedback default */
    *is_need_fb = TD_TRUE;

    switch ((sample_msg_type)msg_req_buf->msg_type) {
        case SAMPLE_MSG_TYPE_VB_POOL_SHARE_REQ: {
            if (msg_res_buf == TD_NULL) {
                return TD_FAILURE;
            }
            ret = sample_venc_msg_proc_vb_pool_share(msg_req_buf->msg_data.pid);
            msg_res_buf->msg_type = SAMPLE_MSG_TYPE_VB_POOL_SHARE_RES;
            msg_res_buf->msg_data.is_req_success = (ret == TD_SUCCESS) ? TD_TRUE : TD_FALSE;
            break;
        }
        case SAMPLE_MSG_TYPE_VB_POOL_UNSHARE_REQ: {
            if (msg_res_buf == TD_NULL) {
                return TD_FAILURE;
            }
            sample_venc_msg_proc_vb_pool_unshare(msg_req_buf->msg_data.pid);
            msg_res_buf->msg_type = SAMPLE_MSG_TYPE_VB_POOL_UNSHARE_RES;
            msg_res_buf->msg_data.is_req_success = TD_TRUE;
            break;
        }
        default: {
            printf("unsupported msg type(%ld)!\n", msg_req_buf->msg_type);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 sample_venc_parse_args(td_s32 argc, td_char *argv[], td_u32 *index)
{
    if (argc != 0x2 && argc != 0x3) { /* 2:arg num */
        sample_venc_usage(argv[0]);
        return TD_FAILURE;
    }
    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        sample_venc_usage(argv[0]);
        return TD_FAILURE;
    }
    if ((strlen(argv[1]) != 1) || (argv[1][0] < '0') || (argv[1][0] > '9')) {
        sample_venc_usage(argv[0]);
        return TD_FAILURE;
    }
    if (argc == 0x3) {
        td_s32 save_heif = atoi(argv[2]);
        if (save_heif != 0 && save_heif != 1) {
            sample_venc_usage(argv[0]);
            return TD_FAILURE;
        }
        sample_comm_venc_set_save_heif((save_heif == 1) ? TD_TRUE : TD_FALSE);
    }
    *index = atoi(argv[1]);
    return TD_SUCCESS;
}

static void sample_venc_print_exit(td_s32 ret)
{
    if (ret == TD_SUCCESS) {
        printf("program exit normally!\n");
    } else {
        printf("program exit abnormally!\n");
    }
}

static void sample_venc_prepare_run(void)
{
#ifndef __LITEOS__
    sample_sys_signal(sample_venc_handle_sig);
#endif

    if (sample_ipc_server_init(sample_venc_ipc_msg_proc) != TD_SUCCESS) {
        printf("sample_ipc_server_init failed!!!\n");
    }
#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif
}

static void sample_venc_finish_run(td_s32 ret)
{
    sample_ipc_server_deinit();
    sample_venc_print_exit(ret);
    sample_comm_venc_set_save_heif(TD_FALSE);
#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif
}

/******************************************************************************
* function    : main()
* description : video venc sample
******************************************************************************/
#ifdef __LITEOS__
td_s32 app_main(td_s32 argc, td_char *argv[])
#else
td_s32 main(td_s32 argc, td_char *argv[])
#endif
{
    td_s32 ret;
    td_u32 index;

    ret = sample_venc_parse_args(argc, argv, &index);
    if (ret != TD_SUCCESS)
        return ret;

    sample_venc_prepare_run();
    ret = sample_venc_choose_mode(index);
    sample_venc_finish_run(ret);
#ifdef __LITEOS__
    return ret;
#else
    exit(ret);
#endif
}
