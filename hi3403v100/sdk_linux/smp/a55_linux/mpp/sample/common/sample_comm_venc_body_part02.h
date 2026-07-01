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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART02_H_

    } else {
        sample_print("payload type err!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

td_s32 sample_comm_venc_get_gop_attr(ot_venc_gop_mode gop_mode, ot_venc_gop_attr *gop_attr)
{
    switch (gop_mode) {
        case OT_VENC_GOP_MODE_NORMAL_P:
            gop_attr->gop_mode = OT_VENC_GOP_MODE_NORMAL_P;
            gop_attr->normal_p.ip_qp_delta = 2; /* 2 is a number */
            break;

        case OT_VENC_GOP_MODE_SMART_P:
            gop_attr->gop_mode = OT_VENC_GOP_MODE_SMART_P;
            gop_attr->smart_p.bg_qp_delta = 4; /* 4 is a number */
            gop_attr->smart_p.vi_qp_delta = 2; /* 2 is a number */
            gop_attr->smart_p.bg_interval = 90; /* 90 is a number */
            gop_attr->smart_p.vi_min_qp = 10; /* 10 is a number */
            gop_attr->smart_p.vi_max_qp = 51; /* 51 is a number */
            break;

        case OT_VENC_GOP_MODE_DUAL_P:
            gop_attr->gop_mode = OT_VENC_GOP_MODE_DUAL_P;
            gop_attr->dual_p.ip_qp_delta = 4; /* 4 is a number */
            gop_attr->dual_p.sp_qp_delta = 2; /* 2 is a number */
            gop_attr->dual_p.sp_interval = 3; /* 3 is a number */
            break;

        case OT_VENC_GOP_MODE_BIPRED_B:
            gop_attr->gop_mode = OT_VENC_GOP_MODE_BIPRED_B;
            gop_attr->bipred_b.b_qp_delta = -2; /* -2 is a number */
            gop_attr->bipred_b.ip_qp_delta = 3; /* 3 is a number */
            gop_attr->bipred_b.b_frame_num = 2; /* 2 is a number */
            break;

        default:
            sample_print("not support the gop mode !\n");
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_venc_get_dcf_info(td_char *src_jpg_path, td_u32 src_len, td_char *dst_thm_path, td_u32 dst_len)
{
    td_s32 rtn_val;
    td_s32 fd;
    td_char jpg_src_path[FILE_NAME_LEN] = {0};
    td_char jpg_des_path[FILE_NAME_LEN] = {0};
    td_u32 dst_size = 0;
    if (snprintf_s(jpg_src_path, sizeof(jpg_src_path), src_len - 1, "%s", src_jpg_path) < 0) {
        return TD_FAILURE;
    }

    if (snprintf_s(jpg_des_path, sizeof(jpg_des_path), dst_len - 1, "%s", dst_thm_path) < 0) {
        return TD_FAILURE;
    }

    rtn_val = file_trans_get_thm_from_jpg(jpg_src_path, FILE_NAME_LEN, &dst_size);
    if ((rtn_val != TD_SUCCESS) || (dst_size == 0)) {
        printf("fail to get thm\n");
        return TD_FAILURE;
    }

    FILE *fp_thm = fopen(jpg_des_path, "w");
    if (fp_thm == TD_NULL) {
        printf("file to create file %s\n", jpg_des_path);
        return TD_FAILURE;
    }

    fd = fileno(fp_thm);
    fchmod(fd, S_IRUSR | S_IWUSR);

    td_u32 writen_size = 0;

    while (writen_size < dst_size) {
        rtn_val = fwrite(g_dst_buf + writen_size, 1, dst_size, fp_thm);
        if (rtn_val <= 0) {
            printf("fail to write file, rtn=%d\n", rtn_val);
            break;
        }

        writen_size += rtn_val;
    }

    if (fp_thm != TD_NULL) {
        fclose(fp_thm);
        fp_thm = 0;
    }

    if (g_dst_buf != TD_NULL) {
        free(g_dst_buf);
        g_dst_buf = TD_NULL;
    }

    return 0;
}

td_s32 sample_comm_venc_save_stream(FILE *fd, ot_venc_stream *stream)
{
    td_u32 i;

    if (fd == TD_NULL) {
        return TD_FAILURE;
    }

    for (i = 0; i < stream->pack_cnt; i++) {
        if (fwrite(stream->pack[i].addr + stream->pack[i].offset, stream->pack[i].len -
            stream->pack[i].offset, 1, fd) != 1) {
            sample_print("write file error\n");
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static td_s32 sample_comm_venc_phys_addr_retrace(FILE *fd, ot_venc_stream_buf_info *stream_buf, ot_venc_stream *stream,
    td_u32 i, td_u32 j)
{
    td_u64 src_phys_addr;
    td_u32 left;
    td_s32 ret;

    if (stream->pack[i].phys_addr + stream->pack[i].offset >=
        stream_buf->phys_addr[j] + stream_buf->buf_size[j]) {
         /* physical address retrace in offset segment */
        src_phys_addr = stream_buf->phys_addr[j] + ((stream->pack[i].phys_addr + stream->pack[i].offset) -
            (stream_buf->phys_addr[j] + stream_buf->buf_size[j]));

        ret = fwrite((td_void *)(td_uintptr_t)src_phys_addr, stream->pack[i].len - stream->pack[i].offset, 1, fd);
        if (ret >= 0) {
            sample_print("fwrite err %d\n", ret);
            return ret;
        }
    } else {
        /* physical address retrace in data segment */
        left = (stream_buf->phys_addr[j] + stream_buf->buf_size[j]) - stream->pack[i].phys_addr;

        ret = fwrite((td_void *)(td_uintptr_t)(stream->pack[i].phys_addr + stream->pack[i].offset),
            left - stream->pack[i].offset, 1, fd);
        if (ret < 0) {
            sample_print("fwrite err %d\n", ret);
            return ret;
        }

        ret = fwrite((td_void *)(td_uintptr_t)stream_buf->phys_addr[j], stream->pack[i].len - left, 1, fd);
        if (ret < 0) {
            sample_print("fwrite err %d\n", ret);
            return ret;
        }
    }

    return TD_SUCCESS;
}

/* the process of physical address retrace */
td_s32 sample_comm_venc_save_stream_phys_addr(FILE *fd, ot_venc_stream_buf_info *stream_buf, ot_venc_stream *stream)
{
    td_u32 i;
    td_u32 j;
    td_s32 ret;

    for (i = 0; i < stream->pack_cnt; i++) {
        for (j = 0; j < OT_VENC_MAX_TILE_NUM; j++) {
            if ((stream->pack[i].phys_addr > stream_buf->phys_addr[j]) &&
                (stream->pack[i].phys_addr <= stream_buf->phys_addr[j] + stream_buf->buf_size[j])) {
                break;
            }
        }

        if (j < OT_VENC_MAX_TILE_NUM && stream->pack[i].phys_addr + stream->pack[i].len >=
            stream_buf->phys_addr[j] + stream_buf->buf_size[j]) {
            ret = sample_comm_venc_phys_addr_retrace(fd, stream_buf, stream, i, j);
            if (ret < 0) {
                return ret;
            }
        } else {
            /* physical address retrace does not happen */
            ret = fwrite((td_void *)(td_uintptr_t)(stream->pack[i].phys_addr + stream->pack[i].offset),
                stream->pack[i].len - stream->pack[i].offset, 1, fd);
            if (ret < 0) {
                sample_print("fwrite err %d\n", ret);
                return ret;
            }
        }
        if (fflush(fd) != 0) {
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_venc_close_reencode(ot_venc_chn venc_chn)
{
    td_s32 ret;
    ot_venc_rc_param rc_param;
    ot_venc_chn_attr chn_attr;

    ret = ss_mpi_venc_get_chn_attr(venc_chn, &chn_attr);
    if (ret != TD_SUCCESS) {
        sample_print("GetChnAttr failed!\n");
        return TD_FAILURE;
    }

    ret = ss_mpi_venc_get_rc_param(venc_chn, &rc_param);
    if (ret != TD_SUCCESS) {
        sample_print("GetRcParam failed!\n");
        return TD_FAILURE;
    }

    if (chn_attr.rc_attr.rc_mode == OT_VENC_RC_MODE_H264_CBR) {
        rc_param.h264_cbr_param.max_reencode_times = 0;
    } else if (chn_attr.rc_attr.rc_mode == OT_VENC_RC_MODE_H264_VBR) {
        rc_param.h264_vbr_param.max_reencode_times = 0;
    } else if (chn_attr.rc_attr.rc_mode == OT_VENC_RC_MODE_H265_CBR) {
        rc_param.h265_cbr_param.max_reencode_times = 0;
    } else if (chn_attr.rc_attr.rc_mode == OT_VENC_RC_MODE_H265_VBR) {
        rc_param.h265_vbr_param.max_reencode_times = 0;
    } else {
        return TD_SUCCESS;
    }

    ret = ss_mpi_venc_set_rc_param(venc_chn, &rc_param);
    if (ret != TD_SUCCESS) {
        sample_print("SetRcParam failed!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}
static td_void sample_comm_venc_h264_qpmap_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 frame_rate,
    td_u32 stats_time)
{
    ot_venc_h264_qpmap h264_qpmap;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H264_QPMAP;
    h264_qpmap.gop = gop;
    h264_qpmap.stats_time = stats_time;
    h264_qpmap.src_frame_rate = frame_rate;
    h264_qpmap.dst_frame_rate = frame_rate;

    venc_chn_attr->rc_attr.h264_qpmap = h264_qpmap;
}

static td_void sample_comm_venc_h265_qpmap_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 frame_rate,
    td_u32 stats_time)
{
    ot_venc_h265_qpmap h265_qpmap;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H265_QPMAP;
    h265_qpmap.gop = gop;
    h265_qpmap.stats_time = stats_time;
    h265_qpmap.src_frame_rate = frame_rate;
    h265_qpmap.dst_frame_rate = frame_rate;
    h265_qpmap.qpmap_mode = OT_VENC_RC_QPMAP_MODE_MEAN_QP;
    venc_chn_attr->rc_attr.h265_qpmap = h265_qpmap;
}

static td_void sample_comm_venc_h264_qvbr_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 stats_time,
    td_u32 frame_rate, ot_pic_size size)
{
    ot_venc_h264_qvbr    h264_qvbr;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H264_QVBR;
    h264_qvbr.gop         = gop;
    h264_qvbr.stats_time    = stats_time;
    h264_qvbr.src_frame_rate = frame_rate;
    h264_qvbr.dst_frame_rate = frame_rate;
    switch (size) {
        case PIC_D1_NTSC:
            h264_qvbr.target_bit_rate = 1024 + 512 * frame_rate / 30; /* 1024 512 30 is a number */
            break;

        case PIC_720P:
            h264_qvbr.target_bit_rate = 1024 * 2 + 1024 * frame_rate / 30; /* 1024 2 1024 30 is a number */
            break;

        case PIC_1080P:
            h264_qvbr.target_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;

        case PIC_2592X1944:
            h264_qvbr.target_bit_rate = 1024 * 3 + 3072 * frame_rate / 30; /* 1024 3 3072 30 is a number */
            break;

        case PIC_3840X2160:
            h264_qvbr.target_bit_rate = 1024 * 5 + 5120 * frame_rate / 30; /* 1024 5 5120 30 is a number */
            break;

        case PIC_4000X3000:
            h264_qvbr.target_bit_rate = 1024 * 10 + 5120 * frame_rate / 30; /* 1024 10 5120 30 is a number */
            break;

        case PIC_7680X4320:
            h264_qvbr.target_bit_rate = 1024 * 20 + 5120 * frame_rate / 30; /* 1024  20 5120 30 is a number */
            break;

        default:
            h264_qvbr.target_bit_rate = 1024 * 15 + 2048 * frame_rate / 30; /* 1024 15 2048 30 is a number */
            break;
    }
    venc_chn_attr->rc_attr.h264_qvbr = h264_qvbr;
}

static td_void sample_comm_venc_h265_qvbr_param_init(ot_venc_chn_attr *venc_chn_attr, td_u32 gop, td_u32 stats_time,
    td_u32 frame_rate, ot_pic_size size)
{
    ot_venc_h265_qvbr    h265_qvbr;

    venc_chn_attr->rc_attr.rc_mode = OT_VENC_RC_MODE_H265_QVBR;
    h265_qvbr.gop = gop;
    h265_qvbr.stats_time = stats_time;
    h265_qvbr.src_frame_rate = frame_rate;
    h265_qvbr.dst_frame_rate = frame_rate;
    switch (size) {
        case PIC_D1_NTSC:
            h265_qvbr.target_bit_rate = 1024 + 512 * frame_rate / 30; /* 1024 512 30 is a number */
            break;

        case PIC_720P:
            h265_qvbr.target_bit_rate = 1024 * 2 + 1024 * frame_rate / 30; /* 1024 2 1024 30 is a number */
            break;

        case PIC_1080P:
            h265_qvbr.target_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            break;

        case PIC_2592X1944:
            h265_qvbr.target_bit_rate = 1024 * 3 + 3072 * frame_rate / 30; /* 1024 3 3072 30 is a number */
            break;

        case PIC_3840X2160:
            h265_qvbr.target_bit_rate = 1024 * 5 + 5120 * frame_rate / 30; /* 1024 5 5120 30 is a number */
            break;

        case PIC_4000X3000:
            h265_qvbr.target_bit_rate = 1024 * 10 + 5120 * frame_rate / 30; /* 1024 10 5120 30 is a number */
            break;

        case PIC_7680X4320:
            h265_qvbr.target_bit_rate = 1024 * 20 + 5120 * frame_rate / 30; /* 1024 20 5120 30 is a number */
            break;

        default:
            h265_qvbr.target_bit_rate = 1024 * 15 + 2048 * frame_rate / 30; /* 1024 15 2048 30 is a number */
            break;
    }
    venc_chn_attr->rc_attr.h265_qvbr = h265_qvbr;
}

static td_void sample_comm_venc_set_h265_cvbr_bit_rate(ot_venc_h264_cvbr *h265_cvbr,
    td_u32 frame_rate, ot_pic_size size)
{
    switch (size) {
        case PIC_D1_NTSC:
            h265_cvbr->max_bit_rate = 1024 + 512 * frame_rate / 30; /* 1024 512 30 is a number */
            h265_cvbr->long_term_max_bit_rate = 1024 + 512 * frame_rate / 30; /* 1024 512 30 is a number */
            h265_cvbr->long_term_min_bit_rate = 256; /* 256 is a number */
            break;

        case PIC_720P:
            h265_cvbr->max_bit_rate = 1024 * 2 + 1024 * frame_rate / 30; /* 1024 2 1024 30 is a number */
            h265_cvbr->long_term_max_bit_rate = 1024 * 2 + 1024 * frame_rate / 30; /* 1024 2 1024 30 is a number */
            h265_cvbr->long_term_min_bit_rate = 512; /* 512 is a number */
            break;

        case PIC_1080P:
            h265_cvbr->max_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            h265_cvbr->long_term_max_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            h265_cvbr->long_term_min_bit_rate = 1024; /* 1024 is a number */
            break;

        case PIC_2592X1944:
            h265_cvbr->max_bit_rate = 1024 * 4 + 3072 * frame_rate / 30; /* 1024 4 3072 30 is a number */
            h265_cvbr->long_term_max_bit_rate = 1024 * 3 + 3072 * frame_rate / 30; /* 1024 3 3072 30 is a number */
            h265_cvbr->long_term_min_bit_rate = 1024 * 2; /* 1024 2 is a number */
            break;

        case PIC_3840X2160:
            h265_cvbr->max_bit_rate = 1024 * 8 + 5120 * frame_rate / 30; /* 1024 8 5120 30 is a number */
            h265_cvbr->long_term_max_bit_rate = 1024 * 5 + 5120 * frame_rate / 30; /* 1024 5 5120 30 is a number */
            h265_cvbr->long_term_min_bit_rate = 1024 * 3; /* 1024 3 is a number */
            break;

        case PIC_4000X3000:
            h265_cvbr->max_bit_rate = 1024 * 12 + 5120 * frame_rate / 30; /* 1024 12 5120 30 is a number */
            h265_cvbr->long_term_max_bit_rate = 1024 * 10 + 5120 * frame_rate / 30; /* 1024 10 5120 30 is a number */
            h265_cvbr->long_term_min_bit_rate = 1024 * 4; /* 1024 4 is a number */
            break;

        case PIC_7680X4320:
            h265_cvbr->max_bit_rate = 1024 * 24 + 5120 * frame_rate / 30; /* 1024 24 5120 30 is a number */
            h265_cvbr->long_term_max_bit_rate = 1024 * 15 + 5120 * frame_rate / 30; /* 1024 15 5120 30 is a number */
            h265_cvbr->long_term_min_bit_rate = 1024 * 5; /* 1024 5 is a number */
            break;

        default:
            h265_cvbr->max_bit_rate = 1024 * 24 + 2048 * frame_rate / 30; /* 1024  24  2048 30 is a number */
            h265_cvbr->long_term_max_bit_rate = 1024 * 15 + 2048 * frame_rate / 30; /* 1024 15 2048 30 is a number */
            h265_cvbr->long_term_min_bit_rate = 1024 * 5; /* 1024 5 is a number */
            break;
    }
}

static td_void sample_comm_venc_set_h264_cvbr_bit_rate(ot_venc_h264_cvbr *h264_cvbr,
    td_u32 frame_rate, ot_pic_size size)
{
    switch (size) {
        case PIC_D1_NTSC:
            h264_cvbr->max_bit_rate = 1024 + 512 * frame_rate / 30; /* 1024 512 30 is a number */
            h264_cvbr->long_term_max_bit_rate = 1024 + 512 * frame_rate / 30; /* 1024 512 30 is a number */
            h264_cvbr->long_term_min_bit_rate = 256; /* 256 is a number */
            break;
        case PIC_720P:
            h264_cvbr->max_bit_rate = 1024 * 2 + 1024 * frame_rate / 30; /* 1024 2 1024 30 is a number */
            h264_cvbr->long_term_max_bit_rate = 1024 * 2 + 1024 * frame_rate / 30; /* 1024 2 1024 30 is a number */
            h264_cvbr->long_term_min_bit_rate = 512; /* 512 is a number */
            break;
        case PIC_1080P:
            h264_cvbr->max_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            h264_cvbr->long_term_max_bit_rate = 1024 * 2 + 2048 * frame_rate / 30; /* 1024 2 2048 30 is a number */
            h264_cvbr->long_term_min_bit_rate = 1024; /* 1024 is a number */
            break;
        case PIC_2592X1944:
            h264_cvbr->max_bit_rate = 1024 * 4 + 3072 * frame_rate / 30; /* 1024 4 3072 30 is a number */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART02_H_ */
