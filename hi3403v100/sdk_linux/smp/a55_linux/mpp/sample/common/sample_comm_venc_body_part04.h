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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART04_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART04_H_

        case PIC_3840X2160:
            h264_cbr.bit_rate = 1024 * 5 + 5120 * frame_rate / 30; /* 1024 5 5120 30 is a number */
            break;

        case PIC_4000X3000:
            h264_cbr.bit_rate = 1024 * 12 + 5120 * frame_rate / 30; /* 1024 12 5120 30 is a number */
            break;

        case PIC_7680X4320:
            h264_cbr.bit_rate = 1024 * 10 + 5120 * frame_rate / 30; /* 1024 10 5120 30 is a number */
            break;

        default:
            h264_cbr.bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2  2048 30 is a number */
            break;
    }

    venc_chn_attr->rc_attr.h264_cbr = h264_cbr;
}

static td_void sample_comm_venc_h265_cbr_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 stats_time,
    td_u32 frame_rate, ot_pic_size size)
{
    ot_venc_h265_cbr h265_cbr;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H265_CBR;
    h265_cbr.gop = gop;
    h265_cbr.stats_time = stats_time;     /* stream rate statics time(s) */
    h265_cbr.src_frame_rate = frame_rate; /* input (vi) frame rate */
    h265_cbr.dst_frame_rate = frame_rate; /* target frame rate */
    switch (size) {
        case PIC_D1_NTSC:
            h265_cbr.bit_rate = 1024 * frame_rate / 30; /* 1024 is a number 30 is a number */
            break;

        case PIC_720P:
            h265_cbr.bit_rate = 1024 * 2 + 1024 * frame_rate / 30; /* 1024 2 1024 30 is a number */
            break;

        case PIC_1080P:
            h265_cbr.bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;

        case PIC_2592X1944:
            h265_cbr.bit_rate = 1024 * 3 + 3072 * frame_rate / 30; /* 1024 3 3072 30 is a number */
            break;

        case PIC_3840X2160:
            h265_cbr.bit_rate = 1024 * 5 + 5120 * frame_rate / 30; /* 1024 5 5120 30 is a number */
            break;

        case PIC_4000X3000:
            h265_cbr.bit_rate = 1024 * 10 + 5120 * frame_rate / 30; /* 1024 10 5120 30 is a number */
            break;

        case PIC_7680X4320:
            h265_cbr.bit_rate = 1024 * 20 + 5120 * frame_rate / 30; /* 1024 20 5120 30 is a number */
            break;

        default:
            h265_cbr.bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;
    }
    venc_chn_attr->rc_attr.h265_cbr = h265_cbr;
}

static td_s32 sample_comm_venc_jpeg_param_init(ot_venc_chn_attr *venc_chn_attr)
{
    ot_venc_jpeg_attr jpeg_attr;
    jpeg_attr.dcf_en = TD_FALSE;
    jpeg_attr.mpf_cfg.large_thumbnail_num = 0;
    jpeg_attr.recv_mode = OT_VENC_PIC_RECV_SINGLE;

    venc_chn_attr->venc_attr.jpeg_attr = jpeg_attr;

    return TD_SUCCESS;
}


static td_s32 sample_comm_venc_mjpeg_param_init(ot_venc_chn_attr *venc_chn_attr,
    sample_comm_venc_chn_param *venc_create_chn_param)
{
    sample_rc rc_mode    = venc_create_chn_param->rc_mode;
    td_u32 stats_time    = venc_create_chn_param->stats_time;
    td_u32 frame_rate    = venc_create_chn_param->frame_rate;
    ot_pic_size size     = venc_create_chn_param->size;

    if (rc_mode == SAMPLE_RC_FIXQP) {
        sample_comm_venc_mjpeg_fixqp_param_init(venc_chn_attr, frame_rate);
    } else if (rc_mode == SAMPLE_RC_CBR) {
        sample_comm_venc_mjpeg_cbr_param_init(venc_chn_attr, stats_time, frame_rate, size);
    } else if ((rc_mode == SAMPLE_RC_VBR) || (rc_mode == SAMPLE_RC_AVBR)) {
        if (rc_mode == SAMPLE_RC_AVBR) {
            sample_print("mjpege not support AVBR, so change rcmode to VBR!\n");
        }
        sample_comm_venc_mjpeg_vbr_param_init(venc_chn_attr, stats_time, frame_rate, size);
    } else {
        sample_print("can't support other mode(%d) in this version!\n", rc_mode);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_comm_venc_h264_param_init(ot_venc_chn_attr *chn_attr, sample_comm_venc_chn_param *chn_param)
{
    sample_rc rc_mode = chn_param->rc_mode;
    td_u32 gop = chn_param->gop;
    td_u32 stats_time = chn_param->stats_time;
    td_u32 frame_rate = chn_param->frame_rate;
    ot_pic_size size = chn_param->size;

    chn_attr->venc_attr.h264_attr.frame_buf_ratio = SAMPLE_FRAME_BUF_RATIO_MIN;
    if (rc_mode == SAMPLE_RC_CBR) {
        sample_comm_venc_h264_cbr_param_init(chn_attr, gop, stats_time, frame_rate, size);
    } else if (rc_mode == SAMPLE_RC_FIXQP) {
        sample_comm_venc_h264_fixqp_param_init(chn_attr, gop, frame_rate);
    } else if (rc_mode == SAMPLE_RC_VBR) {
        sample_comm_venc_h264_vbr_param_init(chn_attr, gop, stats_time, frame_rate, size);
    } else if (rc_mode == SAMPLE_RC_AVBR) {
        sample_comm_venc_h264_avbr_param_init(chn_attr, gop, stats_time, frame_rate, size);
    } else if (rc_mode == SAMPLE_RC_CVBR) {
        sample_comm_venc_h264_cvbr_param_init(chn_attr, gop, stats_time, frame_rate, size);
    } else if (rc_mode == SAMPLE_RC_QVBR) {
        sample_comm_venc_h264_qvbr_param_init(chn_attr, gop, stats_time, frame_rate, size);
    } else if (rc_mode == SAMPLE_RC_QPMAP) {
        sample_comm_venc_h264_qpmap_param_init(chn_attr, gop, frame_rate, stats_time);
    } else {
        sample_print("%s,%d,rc_mode(%d) not support\n", __FUNCTION__, __LINE__, rc_mode);
        return TD_FAILURE;
    }
    chn_attr->venc_attr.h264_attr.rcn_ref_share_buf_en = chn_param->is_rcn_ref_share_buf;

    return TD_SUCCESS;
}

static td_s32 sample_comm_venc_h265_param_init(ot_venc_chn_attr *chn_attr,
    sample_comm_venc_chn_param *chn_param)
{
    sample_rc rc_mode = chn_param->rc_mode;
    td_u32 gop = chn_param->gop;
    td_u32 stats_time = chn_param->stats_time;
    td_u32 frame_rate = chn_param->frame_rate;
    ot_pic_size size = chn_param->size;

    chn_attr->venc_attr.h265_attr.frame_buf_ratio = SAMPLE_FRAME_BUF_RATIO_MIN;
    if (rc_mode == SAMPLE_RC_CBR) {
        sample_comm_venc_h265_cbr_param_init(chn_attr, gop, stats_time, frame_rate, size);
    } else if (rc_mode == SAMPLE_RC_FIXQP) {
        sample_comm_venc_h265_fixqp_param_init(chn_attr, gop, frame_rate);
    } else if (rc_mode == SAMPLE_RC_VBR) {
        sample_comm_venc_h265_vbr_param_init(chn_attr, gop, stats_time, frame_rate, size);
    } else if (rc_mode == SAMPLE_RC_AVBR) {
        sample_comm_venc_h265_avbr_param_init(chn_attr, gop, stats_time, frame_rate, size);
    } else if (rc_mode == SAMPLE_RC_CVBR) {
        sample_comm_venc_h265_cvbr_param_init(chn_attr, gop, stats_time, frame_rate, size);
    } else if (rc_mode == SAMPLE_RC_QVBR) {
        sample_comm_venc_h265_qvbr_param_init(chn_attr, gop, stats_time, frame_rate, size);
    } else if (rc_mode == SAMPLE_RC_QPMAP) {
        sample_comm_venc_h265_qpmap_param_init(chn_attr, gop, frame_rate, stats_time);
    } else {
        sample_print("%s,%d,rc_mode(%d) not support\n", __FUNCTION__, __LINE__, rc_mode);
        return TD_FAILURE;
    }
    chn_attr->venc_attr.h265_attr.rcn_ref_share_buf_en = chn_param->is_rcn_ref_share_buf;

    return TD_SUCCESS;
}

static td_void sample_comm_venc_set_gop_attr(ot_payload_type type, ot_venc_chn_attr *chn_attr,
    ot_venc_gop_attr *gop_attr)
{
    if (type == OT_PT_MJPEG || type == OT_PT_JPEG) {
        chn_attr->gop_attr.gop_mode = OT_VENC_GOP_MODE_NORMAL_P;
        chn_attr->gop_attr.normal_p.ip_qp_delta = 0;
    } else {
        chn_attr->gop_attr = *gop_attr;
        if ((gop_attr->gop_mode == OT_VENC_GOP_MODE_BIPRED_B) && (type == OT_PT_H264)) {
            if (chn_attr->venc_attr.profile == 0) {
                chn_attr->venc_attr.profile = 1;

                sample_print("H.264 base profile not support BIPREDB, so change profile to main profile!\n");
            }
        }
    }
}

static td_s32 sample_comm_venc_channel_param_init(sample_comm_venc_chn_param *chn_param, ot_venc_chn_attr *chn_attr)
{
    td_s32 ret;
    ot_venc_gop_attr *gop_attr = &chn_param->gop_attr;
    td_u32 profile = chn_param->profile;
    ot_payload_type type = chn_param->type;
    ot_size venc_size = chn_param->venc_size;

    chn_attr->venc_attr.type = type;
    chn_attr->venc_attr.max_pic_width = venc_size.width;
    chn_attr->venc_attr.max_pic_height = venc_size.height;
    chn_attr->venc_attr.pic_width = venc_size.width;   /* the picture width */
    chn_attr->venc_attr.pic_height = venc_size.height; /* the picture height */

    if (type == OT_PT_MJPEG || type == OT_PT_JPEG) {
        chn_attr->venc_attr.buf_size =
            OT_ALIGN_UP(venc_size.width, 16) * OT_ALIGN_UP(venc_size.height, 16) * 4; /* 16 4 is a number */
    } else {
        chn_attr->venc_attr.buf_size =
            OT_ALIGN_UP(venc_size.width * venc_size.height * 3 / 4, 64); /*  3  4 64 is a number */
    }
    chn_attr->venc_attr.profile = profile;
    chn_attr->venc_attr.is_by_frame = TD_TRUE; /* get stream mode is slice mode or frame mode? */

    if (gop_attr->gop_mode == OT_VENC_GOP_MODE_SMART_P) {
        chn_param->stats_time = gop_attr->smart_p.bg_interval / chn_param->gop;
    } else {
        chn_param->stats_time = 1;
    }

    switch (type) {
        case OT_PT_H265:
            ret = sample_comm_venc_h265_param_init(chn_attr, chn_param);
            break;

        case OT_PT_H264:
            ret = sample_comm_venc_h264_param_init(chn_attr, chn_param);
            break;

        case OT_PT_MJPEG:
            ret = sample_comm_venc_mjpeg_param_init(chn_attr, chn_param);
            break;

        case OT_PT_JPEG:
            ret = sample_comm_venc_jpeg_param_init(chn_attr);
            break;

        default:
            sample_print("can't support this type (%d) in this version!\n", type);
            return OT_ERR_VENC_NOT_SUPPORT;
    }

    sample_comm_venc_set_gop_attr(type, chn_attr, gop_attr);

    return ret;
}

td_s32 sample_comm_venc_create(ot_venc_chn venc_chn, sample_comm_venc_chn_param *chn_param)
{
    td_s32 ret;
    ot_venc_chn_attr venc_chn_attr;
    ot_pic_size size = chn_param->size;

    chn_param->frame_rate = 30; /* 30 is a number */
    chn_param->gop = 30; /* 30 is a number */

    if (sample_comm_sys_get_pic_size(size, &chn_param->venc_size) != TD_SUCCESS) {
        sample_print("get picture size failed!\n");
        return TD_FAILURE;
    }

    /* step 1:  create venc channel */
    if ((ret = sample_comm_venc_channel_param_init(chn_param, &venc_chn_attr)) != TD_SUCCESS) {
        sample_print("venc_channel_param_init failed!\n");
        return ret;
    }

    if ((ret = ss_mpi_venc_create_chn(venc_chn, &venc_chn_attr)) != TD_SUCCESS) {
        sample_print("ss_mpi_venc_create_chn [%d] failed with %#x! ===\n", venc_chn, ret);
        return ret;
    }

    if (chn_param->type == OT_PT_JPEG) {
        return TD_SUCCESS;
    }

    if ((ret = sample_comm_venc_close_reencode(venc_chn)) != TD_SUCCESS) {
        ss_mpi_venc_destroy_chn(venc_chn);
        return ret;
    }

    return TD_SUCCESS;
}

/*
 * function : start venc stream mode
 * note     : rate control parameter need adjust, according your case.
 */
td_s32 sample_comm_venc_start(ot_venc_chn venc_chn, sample_comm_venc_chn_param *chn_param)
{
    td_s32 ret;
    ot_venc_start_param start_param;

    /* step 1: create encode chnl */
    if ((ret = sample_comm_venc_create(venc_chn, chn_param)) != TD_SUCCESS) {
        sample_print("sample_comm_venc_create failed with%#x! \n", ret);
        return TD_FAILURE;
    }
    /* step 2:  start recv venc pictures */
    start_param.recv_pic_num = -1;
    if ((ret = ss_mpi_venc_start_chn(venc_chn, &start_param)) != TD_SUCCESS) {
        sample_print("ss_mpi_venc_start_recv_pic failed with%#x! \n", ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/* function : stop venc (stream mode-- H264, MJPEG) */
td_s32 sample_comm_venc_stop(ot_venc_chn venc_chn)
{
    td_s32 ret;

    /* stop venc chn */
    ret = ss_mpi_venc_stop_chn(venc_chn);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_venc_stop_chn vechn[%d] failed with %#x!\n", venc_chn, ret);
    }

    /* distroy venc channel */
    ret = ss_mpi_venc_destroy_chn(venc_chn);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_venc_destroy_chn vechn[%d] failed with %#x!\n", venc_chn, ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/* start snap */
td_s32 sample_comm_venc_snap_start(ot_venc_chn venc_chn, ot_size *size, td_bool support_dcf)
{
    td_s32 ret;
    ot_venc_chn_attr venc_chn_attr;
    ot_venc_start_param start_param;

    /* step 1:  create venc channel */
    venc_chn_attr.venc_attr.type = OT_PT_JPEG;
    venc_chn_attr.venc_attr.max_pic_width = size->width;
    venc_chn_attr.venc_attr.max_pic_height = size->height;
    venc_chn_attr.venc_attr.pic_width = size->width;
    venc_chn_attr.venc_attr.pic_height = size->height;
    venc_chn_attr.venc_attr.buf_size = size->width * size->height * 2; /* 2 is a number */
    venc_chn_attr.venc_attr.is_by_frame = TD_TRUE; /* get stream mode is field mode or frame mode */
    venc_chn_attr.venc_attr.profile = 0;
    venc_chn_attr.venc_attr.jpeg_attr.dcf_en = support_dcf;
    venc_chn_attr.venc_attr.jpeg_attr.mpf_cfg.large_thumbnail_num = 0;
    venc_chn_attr.venc_attr.jpeg_attr.recv_mode = OT_VENC_PIC_RECV_SINGLE;

    ret = ss_mpi_venc_create_chn(venc_chn, &venc_chn_attr);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_venc_create_chn [%d] failed with %#x!\n", venc_chn, ret);
        return ret;
    }

    ret = ss_mpi_venc_set_jpeg_enc_mode(venc_chn, OT_VENC_JPEG_ENC_SNAP);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_venc_set_jpeg_enc_mode faild with%#x!\n", ret);
        return TD_FAILURE;
    }

    start_param.recv_pic_num = -1;
    ret = ss_mpi_venc_start_chn(venc_chn, &start_param);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_venc_start_chn faild with%#x!\n", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/* start photo */
td_s32 sample_comm_venc_photo_start(ot_venc_chn venc_chn, ot_size *size, td_bool support_dcf)
{
    td_s32 ret;
    ot_venc_chn_attr venc_chn_attr;
    ot_venc_start_param start_param;

    /* step 1:  create venc channel */
    venc_chn_attr.venc_attr.type = OT_PT_JPEG;
    venc_chn_attr.venc_attr.max_pic_width = size->width;
    venc_chn_attr.venc_attr.max_pic_height = size->height;
    venc_chn_attr.venc_attr.pic_width = size->width;
    venc_chn_attr.venc_attr.pic_height = size->height;
    venc_chn_attr.venc_attr.buf_size = size->width * size->height * 2; /* 2 is a number */
    venc_chn_attr.venc_attr.is_by_frame = TD_TRUE; /* get stream mode is field mode or frame mode */
    venc_chn_attr.venc_attr.profile = 0;
    venc_chn_attr.venc_attr.jpeg_attr.dcf_en = support_dcf;
    venc_chn_attr.venc_attr.jpeg_attr.mpf_cfg.large_thumbnail_num = 0;
    venc_chn_attr.venc_attr.jpeg_attr.recv_mode = OT_VENC_PIC_RECV_SINGLE;

    ret = ss_mpi_venc_create_chn(venc_chn, &venc_chn_attr);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_venc_create_chn [%d] failed with %#x!\n", venc_chn, ret);
        return ret;
    }

    start_param.recv_pic_num = -1;
    ret = ss_mpi_venc_start_chn(venc_chn, &start_param);
    if (ret != TD_SUCCESS) {
        sample_print("mpi_venc_start_chn faild with%#x!\n", ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/* stop snap */
td_s32 sample_comm_venc_snap_stop(ot_venc_chn venc_chn)
{
    td_s32 ret;
    ret = ss_mpi_venc_stop_chn(venc_chn);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_venc_stop_chn vechn[%d] failed with %#x!\n", venc_chn, ret);
        return TD_FAILURE;
    }
    ret = ss_mpi_venc_destroy_chn(venc_chn);
    if (ret != TD_SUCCESS) {
        sample_print("ss_mpi_venc_destroy_chn vechn[%d] failed with %#x!\n", venc_chn, ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_save_snap_stream(ot_venc_stream stream, td_bool save_jpg, td_bool save_thm)
{
    td_s32 ret;
    char stream_file[FILE_NAME_LEN] = {0};
    FILE *file = TD_NULL;
    td_s32 fd = -1;

    if (snprintf_s(stream_file, FILE_NAME_LEN, FILE_NAME_LEN - 1, "snap_%d.jpg", g_snap_cnt) < 0) {
        free(stream.pack);
        stream.pack = NULL;
        return TD_FAILURE;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART04_H_ */
