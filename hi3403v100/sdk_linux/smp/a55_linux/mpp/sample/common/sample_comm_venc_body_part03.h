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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART03_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART03_H_

            h264_cvbr->long_term_max_bit_rate = 1024 * 3 + 3072 * frame_rate / 30; /* 1024 3 3072 30 is a number */
            h264_cvbr->long_term_min_bit_rate = 1024 * 2; /* 1024 2 is a number */
            break;
        case PIC_3840X2160:
            h264_cvbr->max_bit_rate = 1024 * 8 + 5120 * frame_rate / 30; /* 1024 8 5120 30 is a number */
            h264_cvbr->long_term_max_bit_rate = 1024 * 5 + 5120 * frame_rate / 30; /* 1024 5 5120 30 is a number */
            h264_cvbr->long_term_min_bit_rate = 1024 * 3; /* 1024 3 is a number */
            break;
        case PIC_4000X3000:
            h264_cvbr->max_bit_rate = 1024 * 12 + 5120 * frame_rate / 30; /* 1024 12 5120 30 is a number */
            h264_cvbr->long_term_max_bit_rate = 1024 * 10 + 5120 * frame_rate / 30; /* 1024 10 5120 30 is a number */
            h264_cvbr->long_term_min_bit_rate = 1024 * 4; /* 1024 4 is a number */
            break;
        case PIC_7680X4320:
            h264_cvbr->max_bit_rate = 1024 * 24 + 5120 * frame_rate / 30; /* 1024 24 5120 30 is a number */
            h264_cvbr->long_term_max_bit_rate = 1024 * 15 + 5120 * frame_rate / 30; /* 1024 15 5120 30 is a number */
            h264_cvbr->long_term_min_bit_rate = 1024 * 5; /* 1024 5 is a number */
            break;
        default:
            h264_cvbr->max_bit_rate = 1024 * 24 + 2048 * frame_rate / 30; /* 1024 24 2048 30 is a number */
            h264_cvbr->long_term_max_bit_rate = 1024 * 15 + 2048 * frame_rate / 30; /* 1024 15 2048 30 is a number */
            h264_cvbr->long_term_min_bit_rate = 1024 * 5; /* 1024 5 is a number */
            break;
    }
}

static td_void sample_comm_venc_h264_cvbr_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 stats_time,
    td_u32 frame_rate, ot_pic_size size)
{
    ot_venc_h264_cvbr    h264_cvbr;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H264_CVBR;
    h264_cvbr.gop         = gop;
    h264_cvbr.stats_time    = stats_time;
    h264_cvbr.src_frame_rate  = frame_rate;
    h264_cvbr.dst_frame_rate = frame_rate;
    h264_cvbr.long_term_stats_time = 1;
    h264_cvbr.short_term_stats_time = stats_time;

    sample_comm_venc_set_h264_cvbr_bit_rate(&h264_cvbr, frame_rate, size);

    venc_chn_attr->rc_attr.h264_cvbr = h264_cvbr;
}

static td_void sample_comm_venc_h265_cvbr_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 stats_time,
    td_u32 frame_rate, ot_pic_size size)
{
    ot_venc_h265_cvbr    h265_cvbr;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H265_CVBR;
    h265_cvbr.gop         = gop;
    h265_cvbr.stats_time    = stats_time;
    h265_cvbr.src_frame_rate  = frame_rate;
    h265_cvbr.dst_frame_rate = frame_rate;
    h265_cvbr.long_term_stats_time = 1;
    h265_cvbr.short_term_stats_time = stats_time;

    sample_comm_venc_set_h265_cvbr_bit_rate(&h265_cvbr, frame_rate, size);

    venc_chn_attr->rc_attr.h265_cvbr = h265_cvbr;
}

static td_void sample_comm_venc_h264_avbr_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 stats_time,
    td_u32 frame_rate, ot_pic_size size)
{
    ot_venc_h264_avbr h264_avbr;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H264_AVBR;
    h264_avbr.gop = gop;
    h264_avbr.stats_time = stats_time;
    h264_avbr.src_frame_rate = frame_rate;
    h264_avbr.dst_frame_rate = frame_rate;
    switch (size) {
        case PIC_360P:
            h264_avbr.max_bit_rate = 1024 * 1 + 1024 * frame_rate / 30; /* 1024 1024 30 is a number */
            break;

        case PIC_D1_NTSC:
            h264_avbr.max_bit_rate = 1024 * frame_rate / 30; /* 1024 30 is a number */
            break;

        case PIC_720P:
            h264_avbr.max_bit_rate = 1024 * 2 + 1024 * frame_rate / 30; /* 1024 2 1024 30 is a number */
            break;

        case PIC_1080P:
            h264_avbr.max_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;

        case PIC_2592X1944:
            h264_avbr.max_bit_rate = 1024 * 3 + 3072 * frame_rate / 30; /* 1024 3 3072 30 is a number */
            break;

        case PIC_3840X2160:
            h264_avbr.max_bit_rate = 1024 * 5 + 5120 * frame_rate / 30; /* 1024 5 5120 30 is a number */
            break;

        case PIC_4000X3000:
            h264_avbr.max_bit_rate = 1024 * 10 + 5120 * frame_rate / 30; /* 1024 10 5120 30 is a number */
            break;

        case PIC_7680X4320:
            h264_avbr.max_bit_rate = 1024 * 20 + 5120 * frame_rate / 30; /* 1024 20 5120 30 is a number */
            break;

        default:
            h264_avbr.max_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;
    }

    venc_chn_attr->rc_attr.h264_avbr = h264_avbr;
}

static td_void sample_comm_venc_h265_avbr_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 stats_time,
    td_u32 frame_rate, ot_pic_size size)
{
    ot_venc_h265_avbr h265_avbr;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H265_AVBR;
    h265_avbr.gop = gop;
    h265_avbr.stats_time = stats_time;
    h265_avbr.src_frame_rate = frame_rate;
    h265_avbr.dst_frame_rate = frame_rate;

    switch (size) {
        case PIC_D1_NTSC:
            h265_avbr.max_bit_rate = 1024 * frame_rate / 30; /* 1024 is a number 30 is a number */
            break;

        case PIC_720P:
            h265_avbr.max_bit_rate = 1024 * 2 + 1024 * frame_rate / 30; /* 1024 2 1024 30 is a number */
            break;

        case PIC_1080P:
            h265_avbr.max_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;

        case PIC_2592X1944:
            h265_avbr.max_bit_rate = 1024 * 3 + 3072 * frame_rate / 30; /* 1024 3 3072 30 is a number */
            break;

        case PIC_3840X2160:
            h265_avbr.max_bit_rate = 1024 * 5 + 5120 * frame_rate / 30; /* 1024 5 5120 30 is a number */
            break;

        case PIC_4000X3000:
            h265_avbr.max_bit_rate = 1024 * 10 + 5120 * frame_rate / 30; /* 1024 10 5120 30 is a number */
            break;

        case PIC_7680X4320:
            h265_avbr.max_bit_rate = 1024 * 20 + 5120 * frame_rate / 30; /* 1024 20 5120 30 is a number */
            break;

        default:
            h265_avbr.max_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;
    }
    venc_chn_attr->rc_attr.h265_avbr = h265_avbr;
}

static td_void sample_comm_venc_mjpeg_vbr_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 stats_time,
    td_u32 frame_rate, ot_pic_size size)
{
    ot_venc_mjpeg_vbr mjpeg_vbr;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_MJPEG_VBR;
    mjpeg_vbr.stats_time = stats_time;
    mjpeg_vbr.src_frame_rate = frame_rate;
    mjpeg_vbr.dst_frame_rate = 5; /* 5 is a number */

    switch (size) {
        case PIC_360P:
            mjpeg_vbr.max_bit_rate = 1024 * 3 + 1024 * frame_rate / 30; /* 1024 3 1024 30 is a number */
            break;

        case PIC_D1_NTSC:
            mjpeg_vbr.max_bit_rate = 1024 + 1024 * frame_rate / 30; /* 1024 30 is a number */
            break;

        case PIC_720P:
            mjpeg_vbr.max_bit_rate = 1024 * 3 + 1024 * frame_rate / 30;  /* 1024 3 30 is a number */
            break;

        case PIC_1080P:
            mjpeg_vbr.max_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;

        case PIC_2592X1944:
            mjpeg_vbr.max_bit_rate = 1024 * 20 + 3072 * frame_rate / 30; /* 1024 20 3072 30 is a number */
            break;

        case PIC_3840X2160:
            mjpeg_vbr.max_bit_rate = 1024 * 25 + 5120 * frame_rate / 30; /* 1024 25 5120 30 is a number */
            break;

        case PIC_4000X3000:
            mjpeg_vbr.max_bit_rate = 1024 * 30 + 5120 * frame_rate / 30; /* 1024 30 5120 30 is a number */
            break;

        case PIC_7680X4320:
            mjpeg_vbr.max_bit_rate = 1024 * 40 + 5120 * frame_rate / 30; /* 1024 40 5120 30 is a number */
            break;

        default:
            mjpeg_vbr.max_bit_rate = 1024 * 5 + 2048 * frame_rate / 30; /* 1024 5 2048 30 is a number */
            break;
    }

    venc_chn_attr->rc_attr.mjpeg_vbr = mjpeg_vbr;
}

static td_void sample_comm_venc_h264_vbr_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 stats_time,
    td_u32 frame_rate, ot_pic_size size)
{
    ot_venc_h264_vbr h264_vbr;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H264_VBR;
    h264_vbr.gop = gop;
    h264_vbr.stats_time = stats_time;
    h264_vbr.src_frame_rate = frame_rate;
    h264_vbr.dst_frame_rate = frame_rate;
    switch (size) {
        case PIC_360P:
            h264_vbr.max_bit_rate = 1024 * 1 + 1024 * frame_rate / 30; /* 1024 30 is a number */
            break;

        case PIC_D1_NTSC:
            h264_vbr.max_bit_rate = 1024 * frame_rate / 30; /* 1024 30 is a number */
            break;

        case PIC_720P:
            h264_vbr.max_bit_rate = 1024 * 2 + 1024 * frame_rate / 30; /* 1024 2 30 is a number */
            break;

        case PIC_1080P:
            h264_vbr.max_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;

        case PIC_2592X1944:
            h264_vbr.max_bit_rate = 1024 * 3 + 3072 * frame_rate / 30; /* 1024 3 3072 30 is a number */
            break;

        case PIC_3840X2160:
            h264_vbr.max_bit_rate = 1024 * 5 + 5120 * frame_rate / 30; /* 1024 5 5120 30 is a number */
            break;

        case PIC_4000X3000:
            h264_vbr.max_bit_rate = 1024 * 10 + 5120 * frame_rate / 30; /* 1024 10 5120 30 is a number */
            break;

        case PIC_7680X4320:
            h264_vbr.max_bit_rate = 1024 * 20 + 5120 * frame_rate / 30; /* 1024 20 5120 30 is a number */
            break;

        default:
            h264_vbr.max_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;
    }

    venc_chn_attr->rc_attr.h264_vbr = h264_vbr;
}

static td_void sample_comm_venc_h265_vbr_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 stats_time,
    td_u32 frame_rate, ot_pic_size size)
{
    ot_venc_h265_vbr h265_vbr;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H265_VBR;
    h265_vbr.gop = gop;
    h265_vbr.stats_time = stats_time;
    h265_vbr.src_frame_rate = frame_rate;
    h265_vbr.dst_frame_rate = frame_rate;
    switch (size) {
        case PIC_D1_NTSC:
            h265_vbr.max_bit_rate = 1024 * frame_rate / 30; /* 1024 is a number 30 is a number */
            break;

        case PIC_720P:
            h265_vbr.max_bit_rate = 1024 * 2 + 1024 * frame_rate / 30; /* 1024 2 1024 30 is a number */
            break;

        case PIC_1080P:
            h265_vbr.max_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;

        case PIC_2592X1944:
            h265_vbr.max_bit_rate = 1024 * 3 + 3072 * frame_rate / 30; /* 1024 3 is 3072 30 is a number */
            break;

        case PIC_3840X2160:
            h265_vbr.max_bit_rate = 1024 * 5 + 5120 * frame_rate / 30; /* 1024 5 5120 30 is a number */
            break;

        case PIC_4000X3000:
            h265_vbr.max_bit_rate = 1024 * 10 + 5120 * frame_rate / 30; /* 1024 10 5120 30 is a number */
            break;

        case PIC_7680X4320:
            h265_vbr.max_bit_rate = 1024 * 20 + 5120 * frame_rate / 30; /* 1024 20 5120 30 is a number */
            break;

        default:
            h265_vbr.max_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;
    }
    venc_chn_attr->rc_attr.h265_vbr = h265_vbr;
}

static td_void sample_comm_venc_mjpeg_fixqp_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 frame_rate)
{
    ot_venc_mjpeg_fixqp mjpege_fixqp;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_MJPEG_FIXQP;
    mjpege_fixqp.qfactor = 75; /* 75 is a number */
    mjpege_fixqp.src_frame_rate = frame_rate;
    mjpege_fixqp.dst_frame_rate = frame_rate;

    venc_chn_attr->rc_attr.mjpeg_fixqp = mjpege_fixqp;
}

static td_void sample_comm_venc_h264_fixqp_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 frame_rate)
{
    ot_venc_h264_fixqp h264_fixqp;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H264_FIXQP;
    h264_fixqp.gop = gop;
    h264_fixqp.src_frame_rate = frame_rate;
    h264_fixqp.dst_frame_rate = frame_rate;
    h264_fixqp.i_qp = 25; /* 25 is a number */
    h264_fixqp.p_qp = 30; /* 30 is a number */
    h264_fixqp.b_qp = 32; /* 32 is a number */
    venc_chn_attr->rc_attr.h264_fixqp = h264_fixqp;
}

static td_void sample_comm_venc_h265_fixqp_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 frame_rate)
{
    ot_venc_h265_fixqp h265_fixqp;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H265_FIXQP;
    h265_fixqp.gop = gop;
    h265_fixqp.src_frame_rate = frame_rate;
    h265_fixqp.dst_frame_rate = frame_rate;
    h265_fixqp.i_qp = 25; /* 25 is a number */
    h265_fixqp.p_qp = 30; /* 30 is a number */
    h265_fixqp.b_qp = 32; /* 32 is a number */
    venc_chn_attr->rc_attr.h265_fixqp = h265_fixqp;
}

static td_void sample_comm_venc_mjpeg_cbr_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 stats_time,
    td_u32 frame_rate, ot_pic_size size)
{
    ot_venc_mjpeg_cbr mjpege_cbr;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_MJPEG_CBR;
    mjpege_cbr.stats_time = stats_time;
    mjpege_cbr.src_frame_rate = frame_rate;
    mjpege_cbr.dst_frame_rate = frame_rate;

    switch (size) {
        case PIC_360P:
            mjpege_cbr.bit_rate = 1024 * 3 + 1024 * frame_rate / 30; /* 1024 3 1024 30 is a number */
            break;

        case PIC_D1_NTSC:
            mjpege_cbr.bit_rate = 1024 + 1024 * frame_rate / 30; /* 1024 1024 30 is a number */
            break;

        case PIC_720P:
            mjpege_cbr.bit_rate = 1024 * 3 + 1024 * frame_rate / 30; /* 1024 3 1024 30 is a number */
            break;

        case PIC_1080P:
            mjpege_cbr.bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;

        case PIC_2592X1944:
            mjpege_cbr.bit_rate = 1024 * 20 + 3072 * frame_rate / 30; /* 1024 20 3072 30 is a number */
            break;

        case PIC_3840X2160:
            mjpege_cbr.bit_rate = 1024 * 25 + 5120 * frame_rate / 30; /* 1024 25 5120 30 is a number */
            break;

        case PIC_4000X3000:
            mjpege_cbr.bit_rate = 1024 * 30 + 5120 * frame_rate / 30; /* 1024 30 5120 30 is a number */
            break;

        case PIC_7680X4320:
            mjpege_cbr.bit_rate = 1024 * 40 + 5120 * frame_rate / 30; /* 1024 40 5120 30 is a number */
            break;

        default:
            mjpege_cbr.bit_rate = 1024 * 5 + 2048 * frame_rate / 30; /* 1024 5 2048 30 is a number */
            break;
    }

    venc_chn_attr->rc_attr.mjpeg_cbr = mjpege_cbr;
}

static td_void sample_comm_venc_h264_cbr_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 stats_time,
    td_u32 frame_rate, ot_pic_size size)
{
    ot_venc_h264_cbr h264_cbr;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H264_CBR;
    h264_cbr.gop = gop;
    h264_cbr.stats_time = stats_time; /* stream rate statics time(s) */
    h264_cbr.src_frame_rate = frame_rate; /* input (vi) frame rate */
    h264_cbr.dst_frame_rate = frame_rate; /* target frame rate */
    switch (size) {
        case PIC_360P:
            h264_cbr.bit_rate = 1024 * 1 + 1024 * frame_rate / 30; /* 1024 1024 30 is a number */
            break;

        case PIC_D1_NTSC:
            h264_cbr.bit_rate = 1024 * frame_rate / 30; /* 1024 is 30 is a number */
            break;

        case PIC_720P:
            h264_cbr.bit_rate = 1024 * 2 + 1024 * frame_rate / 30; /* 1024 2 1024 30 is a number */
            break;

        case PIC_1080P:
            h264_cbr.bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;

        case PIC_2592X1944:
            h264_cbr.bit_rate = 1024 * 3 + 3072 * frame_rate / 30; /* 1024 3 3072 30 is a number */
            break;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART03_H_ */
