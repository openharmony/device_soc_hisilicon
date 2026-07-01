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

#include "sample_comm.h"
#include "sdk_module_init.h"

#define DEBUG_SAVE_FILE 0
#define PIC_SIZE   PIC_1080P
#define BIG_STREAM_SIZE     PIC_1080P
#define SMALL_STREAM_SIZE   PIC_D1_NTSC
#define DEFAULT_WAIT_TIME   20000
#define VB_MAX_NUM     10
#define SAMPLE_COMPOSITE_MAX_BUFFER_LEN (4000 * 1024)
#define SAMPLE_COMPOSITE_HELP_ARG_LEN 2

#define ENTER_ASCII 10

#define SAMPLE_VDEC_MAX_NUM   1

#define VI_DEV_ID       0
#define VI_PIPE_ID      0
#define VENC_CHN_ID     0
#define VDEC_CHN_ID     0
#define VO_CHN_ID       0
#define VPSS_CHN_DEC_ID 0
#define VPSS_CHN_0      0
#define VPSS_CHN_1      1
#define VPSS_CHN_2      2
#define VO_LAYER        0
#define VPSS_GRP        0
#define VPSS_GRP_DEC    1
#define VPSS_CHN_NUM    2
#define CHN_NUM_MAX    2

#define VI_VB_YUV_CNT    6
#define VPSS_VB_YUV_CNT  11

#define DEFAULT_VDEC_WAIT_TIME (-1)

typedef struct {
    ot_size            max_size;
    ot_pixel_format    pixel_format;
    ot_size            output_size[OT_VPSS_MAX_PHYS_CHN_NUM];
    ot_compress_mode   compress_mode[OT_VPSS_MAX_PHYS_CHN_NUM];
    td_bool            enable[OT_VPSS_MAX_PHYS_CHN_NUM];
} sample_composite_vpss_chn_attr;

typedef struct {
    td_bool            chn_num;
    ot_payload_type    type[SAMPLE_VDEC_MAX_NUM];
    ot_size            size[SAMPLE_VDEC_MAX_NUM];
    td_u32             ref_frame_num[SAMPLE_VDEC_MAX_NUM];
    td_u32             display_frame_num[SAMPLE_VDEC_MAX_NUM];
    td_bool            frame_buf_cnt[SAMPLE_VDEC_MAX_NUM];
    td_bool            composite_dec_en[SAMPLE_VDEC_MAX_NUM];
} sample_composite_vdec_chn_attr;

typedef struct {
    td_u32            valid_num;
    td_u64            blk_size[OT_VB_MAX_COMMON_POOLS];
    td_u32            blk_cnt[OT_VB_MAX_COMMON_POOLS];
    td_u32            supplement_config;
} sample_composite_vb_attr;

typedef struct {
    ot_size          enc_size[VPSS_CHN_NUM];
    ot_pic_size      enc_pic_size[VPSS_CHN_NUM];
    ot_payload_type  payload[CHN_NUM_MAX];
    sample_vi_cfg    vi_cfg;
    sample_vo_cfg    vo_cfg;
    ot_size          vi_size;
    td_s32           vdec_chn_num;
    ot_venc_chn      venc_chn[CHN_NUM_MAX];
    sample_composite_vpss_chn_attr vpss_param[VPSS_CHN_NUM];
    sample_composite_vdec_chn_attr vdec_param;
    sample_composite_vb_attr       vb_attr;
} sample_composite_param;

static td_bool g_send_multi_frame_signal = TD_FALSE;
static td_bool g_stop_get_stream_signal = TD_FALSE;
static pthread_t g_send_multi_frame_thread = 0;
static pthread_t g_get_stream_thread = 0;
FILE *g_dbg_file = TD_NULL;

/******************************************************************************
* function : show usage
******************************************************************************/
void sample_composite_usage(const char *prg_nm)
{
    printf("Usage : %s [index] \n", prg_nm);
    printf("index:\n");
    printf("\t  0) Composite venc + vdec.\n");
    printf("\t  1) Composite venc + save stream.\n");

    return;
}

static td_bool g_sample_venc_exit = TD_FALSE;

/******************************************************************************
* function : to process abnormal case
******************************************************************************/
td_void sample_composite_handle_sig(td_s32 signo)
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
    printf("\t 1) smart p.\n");

    c[0] = sample_venc_getchar();
    if (c[0] == 'e') {
        return TD_FAILURE;
    } else if ((c[0] == ENTER_ASCII) || ((c[0] != '0') && (c[0] != '1'))) {
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

static td_void get_vb_attr(const ot_size *vi_size, const sample_composite_vpss_chn_attr *vpss_chn_attr,
    sample_composite_vb_attr *vb_attr)
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
}

static td_void get_default_vo_cfg(sample_vo_cfg *vo_cfg, ot_size enc_size)
{
    sample_comm_vo_get_def_config(vo_cfg);

    vo_cfg->intf_sync  = OT_VO_OUT_1080P50;
    vo_cfg->disp_rect.x  = 0;
    vo_cfg->disp_rect.y  = 0;
    vo_cfg->disp_rect.width = enc_size.width;
    vo_cfg->disp_rect.height = enc_size.height;
    vo_cfg->image_size.width = enc_size.width;
    vo_cfg->image_size.height = enc_size.height;
    vo_cfg->vo_mode   = VO_MODE_1MUX;
    vo_cfg->bg_color  = COLOR_RGB_BLACK;
}

static td_void get_default_vpss_chn_attr(ot_size vi_size, ot_size enc_size[], td_s32 len,
    sample_composite_vpss_chn_attr *vpss_chan_attr)
{
    td_s32 i;
    td_u32 max_width;
    td_u32 max_height;

    (td_void)memset_s(vpss_chan_attr, sizeof(sample_composite_vpss_chn_attr), 0,
        sizeof(sample_composite_vpss_chn_attr));

    max_width = vi_size.width;
    max_height = vi_size.height;

    for (i = 0; (i < len) && (i < OT_VPSS_MAX_PHYS_CHN_NUM); i++) {
        vpss_chan_attr->output_size[i].width  = enc_size[i].width;
        vpss_chan_attr->output_size[i].height = enc_size[i].height;
        vpss_chan_attr->compress_mode[i]      = OT_COMPRESS_MODE_NONE;
        vpss_chan_attr->enable[i]             = TD_TRUE;

        max_width = MAX2(max_width, enc_size[i].width);
        max_height = MAX2(max_height, enc_size[i].height);
    }

    vpss_chan_attr->max_size.width  = max_width;
    vpss_chan_attr->max_size.height = max_height;
    vpss_chan_attr->pixel_format    = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;

    return;
}

static td_void get_default_vdec_chn_attr(ot_size enc_size[], td_s32 len, ot_payload_type payload[],
    td_s32 pt_len, sample_composite_vdec_chn_attr *vdec_chan_attr)
{
    td_s32 i;

    for (i = 0; (i < len) && (i < SAMPLE_VDEC_MAX_NUM) && (i < pt_len); i++) {
        vdec_chan_attr->type[i] = payload[i];
        vdec_chan_attr->size[i].width = enc_size[i].width;
        vdec_chan_attr->size[i].height = enc_size[i].height;
        vdec_chan_attr->ref_frame_num[i]       = 6;   /* 6 : shvc ref num */
        vdec_chan_attr->display_frame_num[i]   = 1;   /* 1 : display_frame_num */
        vdec_chan_attr->frame_buf_cnt[i]       = 6;   /* 6 : frame_buf_cnt */
    }

    vdec_chan_attr->chn_num = len > SAMPLE_VDEC_MAX_NUM ? SAMPLE_VDEC_MAX_NUM : len;
}

td_s32 sample_composite_sys_init(sample_composite_vb_attr *vb_attr)
{
    td_s32 i;
    td_s32 ret;
    ot_vb_cfg vb_cfg = {0};

    if (vb_attr->valid_num > OT_VB_MAX_COMMON_POOLS) {
        sample_print("sample_composite_sys_init vb valid num(%d) too large than OT_VB_MAX_COMMON_POOLS(%d)!\n",
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
        sample_print("sample_composite_sys_init failed!\n");
    }

    return ret;
}

td_s32 sample_composite_vi_init(sample_vi_cfg *vi_cfg)
{
    td_s32 ret;

    ret = sample_comm_vi_start_vi(vi_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vi_start_vi failed: 0x%x\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

td_void sample_composite_vi_deinit(sample_vi_cfg *vi_cfg)
{
    sample_comm_vi_stop_vi(vi_cfg);
}

td_s32 sample_composite_vpss_init(ot_vpss_grp vpss_grp, sample_composite_vpss_chn_attr *vpss_chan_cfg)
{
    td_s32 ret;
    ot_vpss_chn vpss_chn;
    ot_vpss_grp_attr grp_attr = {0};
    ot_vpss_chn_attr chn_attr[OT_VPSS_MAX_PHYS_CHN_NUM] = {0};

    grp_attr.max_width  = vpss_chan_cfg->max_size.width;
    grp_attr.max_height = vpss_chan_cfg->max_size.height;
    grp_attr.nr_en = TD_FALSE;
    grp_attr.dei_mode = OT_VPSS_DEI_MODE_OFF;
    grp_attr.pixel_format = vpss_chan_cfg->pixel_format;
    grp_attr.frame_rate.src_frame_rate = -1;
    grp_attr.frame_rate.dst_frame_rate = -1;

    for (vpss_chn = 0; vpss_chn < OT_VPSS_MAX_PHYS_CHN_NUM; vpss_chn++) {
        if (vpss_chan_cfg->enable[vpss_chn] == 1) {
            chn_attr[vpss_chn].width                   = vpss_chan_cfg->output_size[vpss_chn].width;
            chn_attr[vpss_chn].height                  = vpss_chan_cfg->output_size[vpss_chn].height;
            chn_attr[vpss_chn].chn_mode                = OT_VPSS_CHN_MODE_USER;
            chn_attr[vpss_chn].compress_mode           = vpss_chan_cfg->compress_mode[vpss_chn];
            chn_attr[vpss_chn].pixel_format            = vpss_chan_cfg->pixel_format;
            chn_attr[vpss_chn].frame_rate.src_frame_rate = -1;
            chn_attr[vpss_chn].frame_rate.dst_frame_rate = -1;
            chn_attr[vpss_chn].depth                     = 2;    /* 2 : user mode */
            chn_attr[vpss_chn].mirror_en                 = 0;
            chn_attr[vpss_chn].flip_en                   = 0;
            chn_attr[vpss_chn].aspect_ratio.mode         = OT_ASPECT_RATIO_NONE;
        }
    }

    ret = sample_common_vpss_start(vpss_grp, vpss_chan_cfg->enable, &grp_attr, chn_attr, OT_VPSS_MAX_PHYS_CHN_NUM);
    if (ret != TD_SUCCESS) {
        sample_print("start vpss failed with %#x!\n", ret);
    }

    return ret;
}

td_void sample_composite_vpss_deinit(ot_vpss_grp vpss_grp, sample_composite_vpss_chn_attr *vpss_chan_cfg)
{
    td_s32 ret;

    ret = sample_common_vpss_stop(vpss_grp, vpss_chan_cfg->enable, OT_VPSS_MAX_PHYS_CHN_NUM);
    if (ret != TD_SUCCESS) {
        sample_print("failed with %#x!\n", ret);
    }
}

td_s32 sample_composite_vdec_init(sample_composite_vdec_chn_attr *vdec_chan_cfg)
{
    td_s32 ret = TD_SUCCESS;
    td_s32 i;
    sample_vdec_attr vdec_attr[SAMPLE_VDEC_MAX_NUM];
    ot_vdec_chn_param vdec_chn_param = {0};

    for (i = 0; (i < vdec_chan_cfg->chn_num) && (i < SAMPLE_VDEC_MAX_NUM); i++) {
        (td_void)memset_s(&vdec_attr[i], sizeof(vdec_attr), 0, sizeof(sample_vdec_attr));
        vdec_attr[i].type                           = vdec_chan_cfg->type[i];
        vdec_attr[i].width                          = vdec_chan_cfg->size[i].width;
        vdec_attr[i].height                         = vdec_chan_cfg->size[i].height;
        vdec_attr[i].mode                            = OT_VDEC_SEND_MODE_FRAME;
        vdec_attr[i].sample_vdec_video.dec_mode      = OT_VIDEO_DEC_MODE_IP;
        vdec_attr[i].sample_vdec_video.bit_width     = OT_DATA_BIT_WIDTH_8;
        vdec_attr[i].sample_vdec_video.ref_frame_num = vdec_chan_cfg->ref_frame_num[i];
        vdec_attr[i].display_frame_num               = vdec_chan_cfg->display_frame_num[i];
        vdec_attr[i].frame_buf_cnt                   = vdec_chan_cfg->frame_buf_cnt[i];

        ret = sample_comm_vdec_init_vb_pool(vdec_chan_cfg->chn_num, &vdec_attr[i], SAMPLE_VDEC_MAX_NUM);
        if (ret != TD_SUCCESS) {
            sample_print("init mod common vb fail for %#x!\n", ret);
            return ret;
        }

        ret = sample_comm_vdec_start(vdec_chan_cfg->chn_num, &vdec_attr[i], SAMPLE_VDEC_MAX_NUM);
        if (ret != TD_SUCCESS) {
            sample_print("start Vdec fail for %#x!\n", ret);
            sample_comm_vdec_exit_vb_pool();
            return ret;
        }

        if (ss_mpi_vdec_get_chn_param(VDEC_CHN_ID, &vdec_chn_param) == TD_SUCCESS) {
            vdec_chn_param.video_param.quick_mark_mode = OT_QUICK_MARK_NONE;
            vdec_chn_param.video_param.composite_dec_en = vdec_chan_cfg->composite_dec_en[i];
            ret = ss_mpi_vdec_set_chn_param(VDEC_CHN_ID, &vdec_chn_param);
            if (ret != TD_SUCCESS) {
                sample_print("vdec chn %d set chn param (composite dec = %d)failed! ret = 0x%x\n", VDEC_CHN_ID,
                    vdec_chan_cfg->composite_dec_en[i], ret);
            }
        } else {
            sample_print("vdec chn %d get chn param failed!\n", VDEC_CHN_ID);
        }
    }

    return ret;
}

td_void sample_composite_vdec_deinit(sample_composite_vdec_chn_attr *vdec_chan_cfg)
{
    sample_comm_vdec_stop(vdec_chan_cfg->chn_num);
    sample_comm_vdec_exit_vb_pool();
}

td_s32 sample_composite_vo_init(sample_vo_cfg *vo_cfg)
{
    td_s32 ret;

    ret = sample_comm_vo_start_vo(vo_cfg);
    if (ret != TD_SUCCESS) {
        sample_print("sample_comm_vo_start_vo failed! ret = 0x%x\n", ret);
    }

    return ret;
}

td_void sample_composite_vo_deinit(sample_vo_cfg *vo_cfg)
{
    sample_comm_vo_stop_vo(vo_cfg);
}

static td_void set_venc_composite_enable(td_void)
{
    td_s32 ret;
    ot_venc_chn_config chn_cfg = {0};

    ret = ss_mpi_venc_get_chn_config(VENC_CHN_ID, &chn_cfg);
    if (ret == TD_SUCCESS) {
        chn_cfg.composite_enc_en = TD_TRUE;
        chn_cfg.mosaic_en = TD_TRUE;
        chn_cfg.quality_level = 1;
        ret = ss_mpi_venc_set_chn_config(VENC_CHN_ID, &chn_cfg);
        if (ret != TD_SUCCESS) {
            sample_print("Venc set chn config (composite encode = %d) failed! ret = 0x%x\n",
                chn_cfg.composite_enc_en, ret);
        }
    } else {
        sample_print("Venc get chn config failed! ret = 0x%x\n", ret);
    }
}

static td_s32 sample_composite_venc_init(ot_pic_size enc_pic_size)
{
    td_s32 ret;
    ot_venc_chn      venc_chn[1] = {VENC_CHN_ID};   // just use venc chn 0
    td_u32           profile[1]  = {0};
    ot_payload_type  payload[1]  = {OT_PT_H265};
    ot_venc_gop_mode gop_mode;
    ot_venc_gop_attr gop_attr;
    sample_rc        rc_mode;
    td_bool          is_rcn_ref_share = TD_TRUE;
    sample_comm_venc_chn_param venc_create_param = {0};

    if (get_rc_mode(payload[0], &rc_mode) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (get_gop_mode(&gop_mode) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    ret = sample_comm_venc_get_gop_attr(gop_mode, &gop_attr);
    if (ret != TD_SUCCESS) {
        sample_print("Venc Get GopAttr for %#x!\n", ret);
        return TD_FAILURE;
    }

    set_venc_composite_enable();

    venc_create_param.type                  = payload[0];
    venc_create_param.size                  = enc_pic_size;
    venc_create_param.rc_mode               = rc_mode;
    venc_create_param.profile               = profile[0];
    venc_create_param.is_rcn_ref_share_buf  = is_rcn_ref_share;
    venc_create_param.gop_attr              = gop_attr;

    ret = sample_comm_venc_start(venc_chn[0], &venc_create_param);
    if (ret != TD_SUCCESS) {
        sample_print("Venc Start failed for %#x!\n", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void sample_composite_venc_deinit(td_void)
{
    sample_comm_venc_stop(VENC_CHN_ID);
}

static td_void config_mosaic_info(ot_venc_mosaic_info *mosaic_info, td_phys_addr_t phys_addr)
{
    mosaic_info->mode = OT_VENC_MOSAIC_MODE_MAP;
    mosaic_info->blk_size   = OT_MOSAIC_BLK_SIZE_64;
    mosaic_info->map_param.valid = TD_TRUE;
    mosaic_info->map_param.phys_addr = phys_addr;
    mosaic_info->map_param.specified_yuv_en = TD_FALSE;
    mosaic_info->map_param.pixel_yuv.data_y = 255; // 255: yuv data
    mosaic_info->map_param.pixel_yuv.data_u = 255; // 255: yuv data
    mosaic_info->map_param.pixel_yuv.data_v = 255; // 255: yuv data
}

static td_u32 venc_trans_blk_size(ot_mosaic_blk_size blk_size)
{
    td_u32 size;

    switch (blk_size) {
        case OT_MOSAIC_BLK_SIZE_4:
            size = 4; // 4: blk size
            break;

        case OT_MOSAIC_BLK_SIZE_8:
            size = 8; // 8: blk size
            break;

        case OT_MOSAIC_BLK_SIZE_16:
            size = 16; // 16: blk size
            break;

        case OT_MOSAIC_BLK_SIZE_32:
            size = 32; // 32: blk size
            break;

        case OT_MOSAIC_BLK_SIZE_64:
            size = 64; // 64: blk size
            break;

        case OT_MOSAIC_BLK_SIZE_128:
            size = 128; // 128: blk size
            break;

        default:
            size = 0;
            break;
    }

    return size;
}

static td_void composite_send_multi_frame(td_phys_addr_t phys_addr)
{
    td_s32 ret;
    td_s32 ret0;
    td_s32 ret1;
    ot_video_frame_info mosaic_frm;
    ot_video_frame_info ori_frm;
    ot_venc_multi_frame_info multi_frm;
    td_u32 max_time_ref;

    ret0 = ss_mpi_vpss_get_chn_frame(VPSS_GRP, VPSS_CHN_0, &mosaic_frm, DEFAULT_WAIT_TIME);
    ret1 = ss_mpi_vpss_get_chn_frame(VPSS_GRP, VPSS_CHN_1, &ori_frm, DEFAULT_WAIT_TIME);

    max_time_ref = MAX2(mosaic_frm.video_frame.time_ref, ori_frm.video_frame.time_ref);
    if (mosaic_frm.video_frame.time_ref < max_time_ref) {
        ret0 = ss_mpi_vpss_release_chn_frame(VPSS_GRP, VPSS_CHN_0, &mosaic_frm);
        ret0 = ss_mpi_vpss_get_chn_frame(VPSS_GRP, VPSS_CHN_0, &mosaic_frm, DEFAULT_WAIT_TIME);
        sample_print("VPSS_CHN0 is lags behind others. 0x%x\n", ret0);
    }

    if (ori_frm.video_frame.time_ref < max_time_ref) {
        ret1 = ss_mpi_vpss_release_chn_frame(VPSS_GRP, VPSS_CHN_1, &ori_frm);
        ret1 = ss_mpi_vpss_get_chn_frame(VPSS_GRP, VPSS_CHN_1, &ori_frm, DEFAULT_WAIT_TIME);
        sample_print("VPSS_CHN2 is lags behind others. 0x%x\n", ret1);
    }

    if (ret0 == TD_SUCCESS && ret1 == TD_SUCCESS) {
        multi_frm.frame[0] = mosaic_frm;
        multi_frm.frame[1] = ori_frm;
        multi_frm.frame_num = 2; /* 2: frame num */
        config_mosaic_info(&multi_frm.mosaic_info, phys_addr);
        ret = ss_mpi_venc_send_multi_frame(VENC_CHN_ID, &multi_frm, DEFAULT_WAIT_TIME);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_venc_send_multi_frame Failed! Error(%#x)\n", ret);
        }
    }

    if (ret0 == TD_SUCCESS) {
        ss_mpi_vpss_release_chn_frame(VPSS_GRP, VPSS_CHN_0, &mosaic_frm);
    }

    if (ret1 == TD_SUCCESS) {
        ss_mpi_vpss_release_chn_frame(VPSS_GRP, VPSS_CHN_1, &ori_frm);
    }
}

static td_void *sample_composite_send_multi_frame_proc(td_void *p)
{
    td_s32 ret;
    td_u32 map_size;
    td_u32 stride;
    td_u32 blk_size;
    td_phys_addr_t phys_addr;
    td_void *virt_addr = TD_NULL;
    ot_venc_chn_attr attr;

    ss_mpi_venc_get_chn_attr(VENC_CHN_ID, &attr);

    blk_size = venc_trans_blk_size(OT_MOSAIC_BLK_SIZE_64);
    stride = ot_venc_get_mosaic_map_stride(attr.venc_attr.pic_width, blk_size);
    map_size = ot_venc_get_mosaic_map_size(attr.venc_attr.pic_width, attr.venc_attr.pic_height, blk_size);

    ret = ss_mpi_sys_mmz_alloc(&phys_addr, &virt_addr, "mosaic_map", TD_NULL, map_size);
    if (ret != TD_SUCCESS) {
        sample_print("alloc mosaic map failed.\n");
        return TD_NULL;
    }

    (td_void)memset_s(virt_addr, map_size, 0, map_size);
    (td_void)memset_s(virt_addr, stride, 0xff, stride);
    (td_void)memset_s(virt_addr + stride * 2, stride, 0xff, stride); /* 2: stride num */

    while (g_send_multi_frame_signal == TD_FALSE) {
        composite_send_multi_frame(phys_addr);
    }

    ret = ss_mpi_sys_mmz_free(phys_addr, virt_addr);
    if (ret != TD_SUCCESS) {
        sample_print("free mosaic map failed.\n");
    }

    return TD_NULL;
}

td_u32 sample_composite_copy_stream(const ot_venc_stream *stream, td_u8 *buffer, td_u32 max_len)
{
    td_s32 i;
    td_u32 len = 0;

    for (i = 0; i < stream->pack_cnt; i++) {
        td_u32 pack_len = stream->pack[i].len - stream->pack[i].offset;
        if (len + pack_len > max_len) {
            sample_print("sample_composite_copy_stream: buffer overflow!\n");
            break;
        }

        if (memcpy_s(buffer + len, max_len - len, stream->pack[i].addr + stream->pack[i].offset, pack_len) != EOK) {
            sample_print("sample_composite_copy_stream: memcpy_s return failed! des len = %d, copylen = %d\n",
                max_len - len, pack_len);
        }
        len += pack_len;
    }

    return len;
}

#if DEBUG_SAVE_FILE
static td_void open_stream_file(td_void)
{
    td_s32 fd;
    td_char file_name[FILE_NAME_LEN];
    td_char real_file_name[FILE_PATH_LEN];

    if (snprintf_s(file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1, "./") < 0) {
        return;
    }

    if (realpath(file_name, real_file_name) == TD_NULL) {
        sample_print("chn %d stream file path error\n", VENC_CHN_ID);
        return;
    }

    if (snprintf_s(real_file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1, "stream_composite.h265") < 0) {
        return;
    }

    g_dbg_file = fopen(real_file_name, "wb");
    if (!g_dbg_file) {
        sample_print("open file[%s] failed!\n", real_file_name);
        return;
    }
    fd = fileno(g_dbg_file);
    fchmod(fd, S_IRUSR | S_IWUSR);
}

static td_void close_stream_file(td_void)
{
    if (g_dbg_file) {
        fclose(g_dbg_file);
        g_dbg_file = TD_NULL;
    }
}

static td_void save_stream(ot_venc_stream *venc_stream)
{
    (void)sample_comm_venc_save_stream(g_dbg_file, venc_stream);
}
#else
#define open_stream_file()
#define close_stream_file()
#define save_stream(a)
#endif

static td_s32 sample_composite_stream_init(ot_venc_stream *venc_stream, td_u8 **buffer, td_u32 max_buffer_len)
{
    if (max_buffer_len == 0 || max_buffer_len > SAMPLE_COMPOSITE_MAX_BUFFER_LEN) {
        sample_print("invalid stream buffer len %u!\n", max_buffer_len);
        return TD_FAILURE;
    }

    *buffer = (td_u8 *)malloc(max_buffer_len);
    if (*buffer == TD_NULL) {
        sample_print("mallic failed!\n");
        return TD_FAILURE;
    }

    /*******************************************************
     step 0 : malloc corresponding number of pack nodes.
    *******************************************************/
    (td_void)memset_s(venc_stream, sizeof(*venc_stream), 0, sizeof(ot_venc_stream));
    venc_stream->pack = (ot_venc_pack *)malloc(sizeof(ot_venc_pack) * 32); /* 32: size */
    if (venc_stream->pack == TD_NULL) {
        sample_print("malloc stream pack failed!\n");
        free(*buffer);
        *buffer = TD_NULL;
        return TD_FAILURE;
    }

    open_stream_file();
    return TD_SUCCESS;
}

static td_bool sample_composite_process_stream(ot_venc_stream *venc_stream, td_u8 *buffer, td_u32 max_buffer_len)
{
    td_u32 len;
    ot_vdec_stream vdec_stream;

    /*******************************************************
     step 2.0 : call mpi to get one-frame stream
    *******************************************************/
    venc_stream->pack_cnt = 30; /* 30: size */
    if (ss_mpi_venc_get_stream(VENC_CHN_ID, venc_stream, DEFAULT_WAIT_TIME) != TD_SUCCESS) {
        return TD_FALSE;
    }

    /*******************************************************
     step 2.1 : save frame to file
    *******************************************************/
    len = sample_composite_copy_stream(venc_stream, buffer, max_buffer_len);

    vdec_stream.addr = buffer;
    vdec_stream.len  = len;
    vdec_stream.need_display = 1;
    vdec_stream.end_of_frame  = 1;
    vdec_stream.end_of_stream = 0;
    vdec_stream.pts = venc_stream->pack[0].pts;
    ss_mpi_vdec_send_stream(VDEC_CHN_ID, &vdec_stream, DEFAULT_VDEC_WAIT_TIME);

    save_stream(venc_stream);

    /*******************************************************
     step 2.2 : release stream
     *******************************************************/
    if (ss_mpi_venc_release_stream(VENC_CHN_ID, venc_stream) != TD_SUCCESS) {
        sample_print("ss_mpi_venc_release_stream failed!\n");
        return TD_FALSE;
    }

    return TD_TRUE;
}

static td_void sample_composite_stream_deinit(ot_venc_stream *venc_stream, td_u8 **buffer)
{
    /*******************************************************
     step 3 : free pack nodes
    *******************************************************/
    free(venc_stream->pack);
    venc_stream->pack = NULL;

    /*******************************************************
    * step 4 : close save-file
    *******************************************************/
    close_stream_file();

    if (*buffer != NULL) {
        free(*buffer);
        *buffer = NULL;
    }
}

static td_void *sample_composite_get_stream_proc(td_void *p)
{
    ot_venc_stream venc_stream;
    td_u8 *buffer = TD_NULL;
    const td_u32 max_buffer_len = SAMPLE_COMPOSITE_MAX_BUFFER_LEN;

    (td_void)p;
    if (sample_composite_stream_init(&venc_stream, &buffer, max_buffer_len) != TD_SUCCESS) {
        return NULL;
    }

    printf("join SAMPLE_COMPOSITE_GetStreamProc!\n");
    /******************************************
     step 1:  Start to get streams of each channel.
    ******************************************/
    while (g_stop_get_stream_signal == TD_FALSE) {
        if (sample_composite_process_stream(&venc_stream, buffer, max_buffer_len) != TD_TRUE) {
            break;
        }
    }

    sample_composite_stream_deinit(&venc_stream, &buffer);
    sample_print("SAMPLE_COMPOSITE_GetStreamProc End!\n");
    return NULL;
}

static td_void sample_composite_stop_thread_sendframe(td_void)
{
    g_send_multi_frame_signal = TD_TRUE;
    if (g_send_multi_frame_thread != 0) {
        pthread_join(g_send_multi_frame_thread, TD_NULL);
        g_send_multi_frame_thread = 0;
    }
}

static td_void sample_composite_stop_thread_stream(td_void)
{
    g_stop_get_stream_signal = TD_TRUE;
    if (g_get_stream_thread != 0) {
        pthread_join(g_get_stream_thread, TD_NULL);
        g_get_stream_thread = 0;
    }
}

static td_void sample_composite_stop_thread(td_void)
{
    sample_composite_stop_thread_sendframe();
    sample_composite_stop_thread_stream();
}

static td_void sample_composite_start_thread(td_void)
{
    g_send_multi_frame_signal = TD_FALSE;
    g_stop_get_stream_signal = TD_FALSE;
    pthread_create(&g_send_multi_frame_thread, 0, sample_composite_send_multi_frame_proc, TD_NULL);
    pthread_create(&g_get_stream_thread, 0, sample_composite_get_stream_proc, TD_NULL);
}

static td_void sample_composite_init_composite_param(sample_composite_param *param)
{
    param->enc_pic_size[0] = PIC_1080P; /* 0, idx */
    param->enc_pic_size[1] = PIC_1080P; /* 1, idx */

    param->payload[0] = OT_PT_H265;
    param->vdec_chn_num = 1;
}

static td_s32 sample_composite_init_vi_vpss(sample_composite_param *param)
{
    td_s32 i;
    td_s32 ret;
    sample_sns_type sns_type = SENSOR0_TYPE;

    for (i = 0; i < VPSS_CHN_NUM; i++) {
        ret = sample_comm_sys_get_pic_size(param->enc_pic_size[i], &param->enc_size[i]);
        if (ret != TD_SUCCESS) {
            sample_print("sample_comm_sys_get_pic_size failed!\n");
            return ret;
        }
    }

    // get vi param
    sample_comm_vi_get_default_vi_cfg(sns_type, &(param->vi_cfg));

    // get vpss param
    get_default_vpss_chn_attr(param->vi_cfg.dev_info.dev_attr.in_size,
        param->enc_size, VPSS_CHN_NUM, &param->vpss_param[0]);
    get_default_vpss_chn_attr(param->vi_cfg.dev_info.dev_attr.in_size,
        param->enc_size, param->vdec_chn_num, &param->vpss_param[1]);

    // get vdec param
    get_default_vdec_chn_attr(param->enc_size, param->vdec_chn_num, param->payload, 1, &param->vdec_param);
    param->vdec_param.composite_dec_en[0] = TD_TRUE;   /* open composite decode */

    // get vo param
    get_default_vo_cfg(&param->vo_cfg, param->enc_size[0]);

    /******************************************
      step 1: init sys alloc common vb
    ******************************************/
    get_vb_attr(&(param->vi_cfg.dev_info.dev_attr.in_size), &param->vpss_param[0], &param->vb_attr);

    if ((ret = sample_composite_sys_init(&param->vb_attr)) != TD_SUCCESS) {
        return ret;
    }

    return TD_SUCCESS;
}

static td_void sample_composite_start_and_stop(td_void)
{
    /******************************************
     create work thread
    ******************************************/
    sample_composite_start_thread();

    printf("please press twice ENTER to exit this sample\n");
    (td_void)getchar();

    if (g_sample_venc_exit != TD_TRUE) {
        (td_void)getchar();
    }

    /******************************************
     exit process
    ******************************************/
    sample_composite_stop_thread();

    sample_composite_venc_deinit();
}


static td_s32 sample_composite_vi_vpss_init(sample_composite_param *param)
{
    td_s32 ret;

    if (sample_composite_init_vi_vpss(param) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if ((ret = sample_composite_vi_init(&param->vi_cfg)) != TD_SUCCESS) {
        goto EXIT_SYS_STOP;
    }

    if ((ret = sample_composite_vpss_init(VPSS_GRP, &param->vpss_param[0])) != TD_SUCCESS) {
        goto EXIT_VI_STOP;
    }

    if ((ret = sample_composite_vpss_init(VPSS_GRP_DEC, &param->vpss_param[1])) != TD_SUCCESS) {
        goto EXIT_VPSS_STOP_0;
    }

    return TD_SUCCESS;

EXIT_VPSS_STOP_0:
    sample_composite_vpss_deinit(VPSS_GRP, &param->vpss_param[0]);
EXIT_VI_STOP:
    sample_composite_vi_deinit(&param->vi_cfg);
EXIT_SYS_STOP:
    sample_comm_sys_exit();

    return ret;
}
/******************************************************************************
* function :  venc_vdec_vo
******************************************************************************/
td_s32 sample_composite_venc_vdec_vo(td_void)
{
    td_s32 ret;

    sample_composite_param param = {0};
    sample_composite_init_composite_param(&param);

    if ((ret = sample_composite_vi_vpss_init(&param)) != TD_SUCCESS) {
        return ret;
    }

    if ((ret = sample_composite_vdec_init(&param.vdec_param)) != TD_SUCCESS) {
        goto EXIT_VPSS_STOP_1;
    }

    if ((ret = sample_composite_vo_init(&param.vo_cfg)) != TD_SUCCESS) {
        goto EXIT_VDEC_STOP;
    }

    if ((ret = sample_comm_vi_bind_vpss(VI_PIPE_ID, VI_DEV_ID, VPSS_GRP, 0)) != TD_SUCCESS) {
        sample_print("VI Bind VPSS err for %#x!\n", ret);
        goto EXIT_VO_STOP;
    }

    if ((ret = sample_comm_vdec_bind_vpss(VDEC_CHN_ID, VPSS_GRP_DEC)) != TD_SUCCESS) {
        sample_print("VDEC Bind VPSS err for %#x!\n", ret);
        goto EXIT_VI_VPSS_UNBIND;
    }

    if ((ret = sample_comm_vpss_bind_vo(VPSS_GRP_DEC, VPSS_CHN_DEC_ID, VO_LAYER, VO_CHN_ID)) != TD_SUCCESS) {
        sample_print("VPSS Bind VO err for %#x!\n", ret);
        goto EXIT_VDEC_VPSS_UNBIND;
    }

    /******************************************
        start stream venc
    ******************************************/
    if ((ret = sample_composite_venc_init(param.enc_pic_size[0])) != TD_SUCCESS) {
        goto EXIT_VPSS_VO_UNBIND;
    }

    sample_composite_start_and_stop();

EXIT_VPSS_VO_UNBIND:
    sample_comm_vpss_un_bind_vo(VPSS_GRP_DEC, VPSS_CHN_0, VO_LAYER, VO_CHN_ID);
EXIT_VDEC_VPSS_UNBIND:
    sample_comm_vdec_un_bind_vpss(VDEC_CHN_ID, VPSS_GRP_DEC);
EXIT_VI_VPSS_UNBIND:
    sample_comm_vi_un_bind_vpss(VI_PIPE_ID, VI_DEV_ID, VPSS_GRP, VPSS_CHN_0);
EXIT_VO_STOP:
    sample_composite_vo_deinit(&param.vo_cfg);
EXIT_VDEC_STOP:
    sample_composite_vdec_deinit(&param.vdec_param);
EXIT_VPSS_STOP_1:
    sample_composite_vpss_deinit(VPSS_GRP_DEC, &param.vpss_param[1]);
    sample_composite_vpss_deinit(VPSS_GRP, &param.vpss_param[0]);
    sample_composite_vi_deinit(&param.vi_cfg);
    sample_comm_sys_exit();

    return ret;
}

static td_void sample_composite_waiting_to_exit(td_void)
{
    g_send_multi_frame_signal = TD_FALSE;
    pthread_create(&g_send_multi_frame_thread, 0, sample_composite_send_multi_frame_proc, TD_NULL);

    printf("please press twice ENTER to exit this sample\n");
    (td_void)getchar();

    if (g_sample_venc_exit != TD_TRUE) {
        (td_void)getchar();
    }

    /******************************************
     exit process
    ******************************************/
    sample_composite_stop_thread_sendframe();
    sample_comm_venc_stop_get_stream(1); // 1: chn num
}

static td_void sample_composite_file_init(sample_composite_param *param)
{
    param->enc_pic_size[0] = PIC_1080P; /* 0: idx */
    param->enc_pic_size[1] = PIC_1080P; /* 1: idx */
}

static td_s32 sample_composite_file_init_vi_vpss(sample_composite_param *param)
{
    td_s32 i;
    td_s32 ret;
    sample_sns_type sns_type = SENSOR0_TYPE;

    for (i = 0; i < VPSS_CHN_NUM; i++) {
        ret = sample_comm_sys_get_pic_size(param->enc_pic_size[i], &param->enc_size[i]);
        if (ret != TD_SUCCESS) {
            sample_print("sample_comm_sys_get_pic_size failed!\n");
            return ret;
        }
    }

    // get vi param
    sample_comm_vi_get_default_vi_cfg(sns_type, &(param->vi_cfg));

    // get vpss param
    get_default_vpss_chn_attr(param->vi_cfg.dev_info.dev_attr.in_size,
        param->enc_size, VPSS_CHN_NUM, &param->vpss_param[0]);

    /******************************************
      step 1: init sys alloc common vb
    ******************************************/
    get_vb_attr(&(param->vi_cfg.dev_info.dev_attr.in_size), &param->vpss_param[0], &param->vb_attr);

    if ((ret = sample_composite_sys_init(&param->vb_attr)) != TD_SUCCESS) {
        return ret;
    }

    return TD_SUCCESS;
}
/******************************************************************************
* function :  vi-vpss-venc-file
******************************************************************************/
td_s32 sample_composite_venc_file(td_void)
{
    td_s32           ret;

    sample_composite_param param =  {0};
    sample_composite_file_init(&param);

    if (sample_composite_file_init_vi_vpss(&param) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if ((ret = sample_composite_vi_init(&param.vi_cfg)) != TD_SUCCESS) {
        goto EXIT_SYS_STOP;
    }

    if ((ret = sample_composite_vpss_init(VPSS_GRP, &param.vpss_param[0])) != TD_SUCCESS) {
        goto EXIT_VI_STOP;
    }

    if ((ret = sample_comm_vi_bind_vpss(VI_PIPE_ID, VI_DEV_ID, VPSS_GRP, 0)) != TD_SUCCESS) {
        sample_print("VI Bind VPSS err for %#x!\n", ret);
        goto EXIT_VPSS_STOP_0;
    }

   /******************************************
    start stream venc
    ******************************************/
    if ((ret = sample_composite_venc_init(param.enc_pic_size[0])) != TD_SUCCESS) {
        goto EXIT_VI_VPSS_UNBIND;
    }

    /******************************************
     create work thread
    ******************************************/
    if ((ret = sample_comm_venc_start_get_stream(param.venc_chn, 1)) != TD_SUCCESS) {
        sample_print("Start Venc failed!\n");
        goto EXIT_VENC_STOP;
    }

    sample_composite_waiting_to_exit();

EXIT_VENC_STOP:
    sample_composite_venc_deinit();
EXIT_VI_VPSS_UNBIND:
    sample_comm_vi_un_bind_vpss(VI_PIPE_ID, VI_DEV_ID, VPSS_GRP, VPSS_CHN_0);
EXIT_VPSS_STOP_0:
    sample_composite_vpss_deinit(VPSS_GRP, &param.vpss_param[0]);
EXIT_VI_STOP:
    sample_composite_vi_deinit(&param.vi_cfg);
EXIT_SYS_STOP:
    sample_comm_sys_exit();

    return ret;
}

static td_s32 sample_composite_parse_index(td_s32 argc, td_char *argv[], td_u32 *index)
{
    if (argc != 2) { /* 2:argc num */
        sample_composite_usage(argv[0]);
        return TD_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        sample_composite_usage(argv[0]);
        return TD_SUCCESS;
    }

    if ((strlen(argv[1]) != 1) || (argv[1][0] < '0') || (argv[1][0] > '9')) {
        sample_composite_usage(argv[0]);
        return TD_SUCCESS;
    }

    *index = atoi(argv[1]);
    return TD_SUCCESS;
}

static td_s32 sample_composite_run(td_u32 index, const td_char *argv_name)
{
    switch (index) {
        case 0: /* 0:case num */
            return sample_composite_venc_vdec_vo();
        case 1: /* 1:case num */
            return sample_composite_venc_file();
        default:
            sample_composite_usage(argv_name);
            return TD_FAILURE;
    }
}

static void sample_composite_print_exit(td_s32 ret)
{
    if (ret == TD_SUCCESS) {
        printf("program exit normally!\n");
    } else {
        printf("program exit abnormally!\n");
    }
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

    ret = sample_composite_parse_index(argc, argv, &index);
    if (ret != TD_SUCCESS || !strncmp(argv[1], "-h", SAMPLE_COMPOSITE_HELP_ARG_LEN) || strlen(argv[1]) != 1 ||
        argv[1][0] < '0' || argv[1][0] > '9') {
        return ret;
    }

#ifndef __LITEOS__
    sample_sys_signal(sample_composite_handle_sig);
#endif

#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif

    ret = sample_composite_run(index, argv[0]);
    sample_composite_print_exit(ret);

#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif

#ifdef __LITEOS__
    return ret;
#else
    exit(ret);
#endif
}
