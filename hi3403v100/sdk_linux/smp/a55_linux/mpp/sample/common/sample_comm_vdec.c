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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/prctl.h>
#include <limits.h>
#include "sample_comm.h"

#define SEND_STREAM_CNT 5
#define VDEC_SECOND 1000000
#define SAMPLE_VDEC_LOW_DELAY_LINE_CNT 16

static ot_vb_src g_vdec_vb_src = OT_VB_SRC_MOD;
static td_bool g_vdec_line_ldy_en = TD_FALSE;

static ot_vb_pool g_pic_vb_pool[OT_VB_MAX_POOLS] = { [0 ... (OT_VB_MAX_POOLS - 1)] = OT_VB_INVALID_POOL_ID };
static ot_vb_pool g_tmv_vb_pool[OT_VB_MAX_POOLS] = { [0 ... (OT_VB_MAX_POOLS - 1)] = OT_VB_INVALID_POOL_ID };

td_void sample_comm_vdec_print_chn_status(td_s32 chn, ot_vdec_chn_status status)
{
    printf("\033[0;33m ---------------------------------------------------------------\033[0;39m\n");
    printf("\033[0;33m chn:%d, type:%d, start:%d, decode_frames:%d, left_pics:%d, left_bytes:%d, "
        "left_frames:%d, recv_frames:%d  \033[0;39m\n",
        chn, (status).type, (status).is_started,  (status).dec_stream_frames, (status).left_decoded_frames,
        (status).left_stream_bytes, (status).left_stream_frames, (status).recv_stream_frames);
    printf("\033[0;33m format_err:%d,    pic_size_err_set:%d,  stream_unsprt:%d,  pack_err:%d, "
        "set_pic_size_err:%d,  ref_err_set:%d,  pic_buf_size_err_set:%d  \033[0;39m\n",
        (status).dec_err.format_err, (status).dec_err.set_pic_size_err, (status).dec_err.stream_unsupport,
        (status).dec_err.pack_err, (status).dec_err.set_protocol_num_err, (status).dec_err.set_ref_num_err,
        (status).dec_err.set_pic_buf_size_err);
    printf("\033[0;33m -----------------------------------------------------------------\033[0;39m\n");
    return;
}

td_bool sample_comm_vdec_get_lowdelay_en(td_void)
{
    return g_vdec_line_ldy_en;
}
td_void sample_comm_vdec_set_lowdelay_en(td_bool enable)
{
    g_vdec_line_ldy_en = enable;
}

td_s32 sample_comm_vdec_init_user_vb_pool(td_u32 chn_num, sample_vdec_buf *vdec_buf,
    sample_vdec_attr *sample_vdec, td_u32 array_len)
{
    td_u32 i;
    ot_vb_pool_cfg vb_pool_cfg;
    for (i = 0; (i < chn_num) && (i < array_len) && (i < OT_VB_MAX_POOLS); i++) {
        if ((vdec_buf[i].pic_buf_size != 0) && (sample_vdec[i].frame_buf_cnt != 0)) {
            (td_void)memset_s(&vb_pool_cfg, sizeof(ot_vb_pool_cfg), 0, sizeof(ot_vb_pool_cfg));
            vb_pool_cfg.blk_size = vdec_buf[i].pic_buf_size;
            vb_pool_cfg.blk_cnt = sample_vdec[i].frame_buf_cnt;
            vb_pool_cfg.remap_mode = OT_VB_REMAP_MODE_NONE;
            g_pic_vb_pool[i] = ss_mpi_vb_create_pool(&vb_pool_cfg);
            if (g_pic_vb_pool[i] == OT_VB_INVALID_POOL_ID) {
                return TD_FAILURE;
            }
        }
        if (vdec_buf[i].tmv_buf_size != 0) {
            (td_void)memset_s(&vb_pool_cfg, sizeof(ot_vb_pool_cfg), 0, sizeof(ot_vb_pool_cfg));
            vb_pool_cfg.blk_size = vdec_buf[i].tmv_buf_size;
            vb_pool_cfg.blk_cnt = sample_vdec[i].sample_vdec_video.ref_frame_num + 1;
            vb_pool_cfg.remap_mode = OT_VB_REMAP_MODE_NONE;
            g_tmv_vb_pool[i] = ss_mpi_vb_create_pool(&vb_pool_cfg);
            if (g_tmv_vb_pool[i] == OT_VB_INVALID_POOL_ID) {
                return TD_FAILURE;
            }
        }
    }
    return TD_SUCCESS;
}

td_void sample_comm_handle_init_vb_fail(td_s32 idx)
{
    td_s32 ret;
    td_s32 i = idx;
    for (; (i >= 0) && (i < OT_VB_MAX_POOLS); i--) {
        if (g_pic_vb_pool[i] != OT_VB_INVALID_POOL_ID) {
            ret = ss_mpi_vb_destroy_pool(g_pic_vb_pool[i]);
            if (ret != TD_SUCCESS) {
                printf("vb destroy pool %d fail!\n", g_pic_vb_pool[i]);
            }
            g_pic_vb_pool[i] = OT_VB_INVALID_POOL_ID;
        }
        if (g_tmv_vb_pool[i] != OT_VB_INVALID_POOL_ID) {
            ret = ss_mpi_vb_destroy_pool(g_tmv_vb_pool[i]);
            if (ret != TD_SUCCESS) {
                printf("vb destroy pool %d fail!\n", g_tmv_vb_pool[i]);
            }
            g_tmv_vb_pool[i] = OT_VB_INVALID_POOL_ID;
        }
    }
    return;
}

td_void sample_comm_vdec_cal_vb_size(td_u32 chn_num, sample_vdec_attr *sample_vdec,
    td_u32 sample_vdec_arr_len, sample_vdec_buf *vdec_buf)
{
    td_u32 i;
    ot_pic_buf_attr buf_attr = { 0 };
    for (i = 0; (i < chn_num) && (i < sample_vdec_arr_len); i++) {
        buf_attr.align = 0;
        buf_attr.height = sample_vdec[i].height;
        buf_attr.width = sample_vdec[i].width;
        if (sample_vdec[i].type == OT_PT_H265) {
            buf_attr.bit_width = sample_vdec[i].sample_vdec_video.bit_width;
            buf_attr.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
            vdec_buf[i].pic_buf_size = ot_vdec_get_pic_buf_size(sample_vdec[i].type, &buf_attr);
            vdec_buf[i].tmv_buf_size =
                ot_vdec_get_tmv_buf_size(sample_vdec[i].type, sample_vdec[i].width, sample_vdec[i].height);
        } else if (sample_vdec[i].type == OT_PT_H264) {
            buf_attr.bit_width = sample_vdec[i].sample_vdec_video.bit_width;
            buf_attr.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
            vdec_buf[i].pic_buf_size = ot_vdec_get_pic_buf_size(sample_vdec[i].type, &buf_attr);
            if (sample_vdec[i].sample_vdec_video.dec_mode == OT_VIDEO_DEC_MODE_IPB) {
                vdec_buf[i].tmv_buf_size =
                    ot_vdec_get_tmv_buf_size(sample_vdec[i].type, sample_vdec[i].width, sample_vdec[i].height);
            }
        } else {
            buf_attr.bit_width = OT_DATA_BIT_WIDTH_8;
            buf_attr.pixel_format = sample_vdec[i].sample_vdec_picture.pixel_format;
            vdec_buf[i].pic_buf_size = ot_vdec_get_pic_buf_size(sample_vdec[i].type, &buf_attr);
        }
    }
    return;
}

td_s32 sample_comm_vdec_config_vb_pool(td_u32 chn_num, sample_vdec_attr *sample_vdec, td_u32 arr_len,
    sample_vdec_buf *vdec_buf, ot_vb_cfg *vb_conf)
{
    td_u32 i;
    td_u32 j;
    td_bool find_flag;
    td_s32 pos = 0;
    /* pic_buffer */
    for (j = 0; j < OT_VB_MAX_COMMON_POOLS; j++) {
        find_flag = TD_FALSE;
        for (i = 0; (i < chn_num) && (i < arr_len); i++) {
            if ((find_flag == TD_FALSE) && (vdec_buf[i].pic_buf_size != 0) && (vdec_buf[i].pic_buf_alloc == TD_FALSE)) {
                vb_conf->common_pool[j].blk_size = vdec_buf[i].pic_buf_size;
                vb_conf->common_pool[j].blk_cnt = sample_vdec[i].frame_buf_cnt;
                vdec_buf[i].pic_buf_alloc = TD_TRUE;
                find_flag = TD_TRUE;
                pos = j;
            }

            if ((find_flag == TD_TRUE) && (vdec_buf[i].pic_buf_alloc == TD_FALSE) &&
                (vb_conf->common_pool[j].blk_size == vdec_buf[i].pic_buf_size)) {
                vb_conf->common_pool[j].blk_cnt += sample_vdec[i].frame_buf_cnt;
                vdec_buf[i].pic_buf_alloc = TD_TRUE;
            }
        }
    }

    /* tmv_buffer */
    for (j = pos + 1; j < OT_VB_MAX_COMMON_POOLS; j++) {
        find_flag = TD_FALSE;
        for (i = 0; (i < chn_num) && (i < arr_len); i++) {
            if ((find_flag == TD_FALSE) && (vdec_buf[i].tmv_buf_size != 0) && (vdec_buf[i].tmv_buf_alloc == TD_FALSE)) {
                vb_conf->common_pool[j].blk_size = vdec_buf[i].tmv_buf_size;
                vb_conf->common_pool[j].blk_cnt = sample_vdec[i].sample_vdec_video.ref_frame_num + 1;
                vdec_buf[i].tmv_buf_alloc = TD_TRUE;
                find_flag = TD_TRUE;
                pos = j;
            }

            if ((find_flag == TD_TRUE) && (vdec_buf[i].tmv_buf_alloc == TD_FALSE) &&
                (vb_conf->common_pool[j].blk_size == vdec_buf[i].tmv_buf_size)) {
                vb_conf->common_pool[j].blk_cnt += sample_vdec[i].sample_vdec_video.ref_frame_num + 1;
                vdec_buf[i].tmv_buf_alloc = TD_TRUE;
            }
        }
    }
    vb_conf->max_pool_cnt = pos + 1;
    return i - 1;
}

td_s32 sample_comm_vdec_init_vb_pool(td_u32 chn_num, sample_vdec_attr *sample_vdec, td_u32 arr_len)
{
    ot_vb_cfg vb_conf;
    td_s32 i;
    td_s32 ret;
    sample_vdec_buf vdec_buf[OT_VDEC_MAX_CHN_NUM];

    check_null_ptr_return(sample_vdec);
    if (arr_len > OT_VDEC_MAX_CHN_NUM) {
        printf("sample_vdec_attr array len Invalid \n");
        return TD_FAILURE;
    }
    if (memset_s(vdec_buf, sizeof(vdec_buf), 0, sizeof(sample_vdec_buf) * OT_VDEC_MAX_CHN_NUM) != EOK) {
        return TD_FAILURE;
    }
    if (memset_s(&vb_conf, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg)) != EOK) {
        return TD_FAILURE;
    }

    sample_comm_vdec_cal_vb_size(chn_num, sample_vdec, arr_len, vdec_buf);

    i = sample_comm_vdec_config_vb_pool(chn_num, sample_vdec, arr_len, vdec_buf, &vb_conf);

    if (g_vdec_vb_src == OT_VB_SRC_MOD) {
        ss_mpi_vb_exit_mod_common_pool(OT_VB_UID_VDEC);
        check_return(ss_mpi_vb_set_mod_pool_cfg(OT_VB_UID_VDEC, &vb_conf), "vb set mod pool config");
        ret = ss_mpi_vb_init_mod_common_pool(OT_VB_UID_VDEC);
        if (ret != TD_SUCCESS) {
            printf("vb exit mod common pool fail for 0x%x\n", ret);
            ss_mpi_vb_exit_mod_common_pool(OT_VB_UID_VDEC);
            return TD_FAILURE;
        }
    } else if (g_vdec_vb_src == OT_VB_SRC_USER) {
        if (sample_comm_vdec_init_user_vb_pool(chn_num, &vdec_buf[0], &sample_vdec[0], OT_VDEC_MAX_CHN_NUM) !=
            TD_SUCCESS) {
            goto fail;
        }
    }

    return TD_SUCCESS;

fail:
    sample_comm_handle_init_vb_fail(i);
    return TD_FAILURE;
}

td_void sample_comm_vdec_exit_user_vb_pool(td_void)
{
    td_s32 i;
    td_s32 ret;
    for (i = OT_VB_MAX_POOLS - 1; i >= 0; i--) {
        if (g_pic_vb_pool[i] != OT_VB_INVALID_POOL_ID) {
            ret = ss_mpi_vb_destroy_pool(g_pic_vb_pool[i]);
            if (ret != TD_SUCCESS) {
                printf("vb destroy pool %d fail!\n", g_pic_vb_pool[i]);
            }
            g_pic_vb_pool[i] = OT_VB_INVALID_POOL_ID;
        }
        if (g_tmv_vb_pool[i] != OT_VB_INVALID_POOL_ID) {
            ret = ss_mpi_vb_destroy_pool(g_tmv_vb_pool[i]);
            if (ret != TD_SUCCESS) {
                printf("vb destroy pool %d fail!\n", g_tmv_vb_pool[i]);
            }
            g_tmv_vb_pool[i] = OT_VB_INVALID_POOL_ID;
        }
    }
    return;
}

td_void sample_comm_vdec_exit_vb_pool(td_void)
{
    if (g_vdec_vb_src == OT_VB_SRC_MOD) {
        ss_mpi_vb_exit_mod_common_pool(OT_VB_UID_VDEC);
    } else if (g_vdec_vb_src == OT_VB_SRC_USER) {
        sample_comm_vdec_exit_user_vb_pool();
    }

    return;
}

td_void sample_comm_vdec_send_h264_frame_process(td_s32 *read_len, td_u8 *buf,
    vdec_thread_param *thread_param, td_s32 used_bytes)
{
    td_s32 i;
    td_bool find_start = TD_FALSE;
    td_bool find_end = TD_FALSE;
    td_bool new_pic;
    /* H264 frame start marker */
    if (*read_len > thread_param->min_buf_size) {
        sample_print("chn %d read_len %d is bigger than buf_size %u!\n", thread_param->chn_id, *read_len,
            thread_param->min_buf_size);
        return;
    }
    for (i = 0; i < *read_len - 8; i++) { /* 8:h264 frame start code length */
        int tmp = buf[i + 3] & 0x1F; /* 3:index  0x1F:frame start marker */
        new_pic = (buf[i] == 0 && buf[i + 1] == 0 && buf[i + 2] == 1 && /* 1 2:index */
            (((tmp == 0x5 || tmp == 0x1) && ((buf[i + 4] & 0x80) == 0x80)) || /* 4:index 0x5 0x80:frame start mark */
            (tmp == 20 && (buf[i + 7] & 0x80) == 0x80))); /* 20 0x1 0x80:frame start marker 7:index */
        if (new_pic == TD_TRUE) {
            find_start = TD_TRUE;
            i += 8; /* 8:h264 frame start code length */
            break;
        }
    }

    for (; i < *read_len - 8; i++) { /* 8:h264 frame start code length */
        int tmp = buf[i + 3] & 0x1F; /* 3:index  0x1F:frame start marker */
        new_pic = (buf[i] == 0 && buf[i + 1] == 0 && buf[i + 2] == 1 && /* 1 2:index */
            (tmp == 15 || tmp == 7 || tmp == 8 || tmp == 6 || /* 15 7 8 6:frame start marker */
            ((tmp == 5 || tmp == 1) && ((buf[i + 4] & 0x80) == 0x80)) || /* 4:index 5 0x80:frame start marker */
            (tmp == 20 && (buf[i + 7] & 0x80) == 0x80))); /* 7:index 20 0x80:frame start marker */
        if (new_pic == TD_TRUE) {
            find_end = TD_TRUE;
            break;
        }
    }

    if (i > 0) {
        *read_len = i;
    }
    if (find_start == TD_FALSE) {
        sample_print("chn %d can not find H264 start code! read_len %d, used_bytes %d!\n", thread_param->chn_id,
            *read_len, used_bytes);
    }
    if (find_end == TD_FALSE) {
        *read_len = i + 8; /* 8:h264 frame start code length */
    }
    return;
}

td_void sample_comm_vdec_send_mpeg4_frame_process(td_s32 *read_len, td_u8 *buf,
    vdec_thread_param *thread_param, td_s32 used_bytes)
{
    td_s32 i;
    td_bool find_start = TD_FALSE;
    td_bool find_end = TD_FALSE;
    /* MPEG4 frame start marker */
    if (*read_len > thread_param->min_buf_size) {
        sample_print("chn %d read_len %d is bigger than buf_size %u!\n", thread_param->chn_id, *read_len,
            thread_param->min_buf_size);
        return;
    }
    for (i = 0; i < *read_len - 4; i++) { /* 4:mpeg4 frame start code length */
        if (buf[i] == 0 && buf[i + 1] == 0 &&
            buf[i + 2] == 1 && buf[i + 3] == 0xB6) { /* 0xB6:frame start marker  2 3:index */
            find_start = TD_TRUE;
            i += 4; /* 4:mpeg4 frame start code length */
            break;
        }
    }

    for (; i < *read_len - 4; i++) { /* 4:mpeg4 frame start code length */
        if (buf[i] == 0 && buf[i + 1] == 0 &&
            buf[i + 2] == 1 && buf[i + 3] == 0xB6) { /* 2 3:index 0xB6:frame start marker */
            find_end = TD_TRUE;
            break;
        }
    }

    if (i > 0) {
        *read_len = i;
    }
    if (find_start == TD_FALSE) {
        sample_print("chn %d can not find MPEG4 start code! read_len %d, used_bytes %d!\n", thread_param->chn_id,
            *read_len, used_bytes);
    }
    if (find_end == TD_FALSE) {
        *read_len = i + 4; /* 4:mpeg4 frame start code length */
    }
    return;
}

td_void sample_comm_vdec_send_h265_frame_process(td_s32 *read_len, td_u8 *buf,
    vdec_thread_param *thread_param, td_s32 used_bytes)
{
    td_s32 i;
    td_bool find_start = TD_FALSE;
    td_bool find_end = TD_FALSE;
    /* H265 frame start marker */
    td_bool new_pic;
    if (*read_len > thread_param->min_buf_size) {
        sample_print("chn %d read_len %d is bigger than buf_size %u!\n", thread_param->chn_id, *read_len,
            thread_param->min_buf_size);
        return;
    }
    for (i = 0; i < *read_len - 6; i++) { /* 6:h265 frame start code length */
        td_u32 tmp = (buf[i + 3] & 0x7E) >> 1; /* 0x7E:frame start marker 3:index */
        new_pic = (buf[i + 0] == 0 && buf[i + 1] == 0 && buf[i + 2] == 1 && /* 1 2:index */
            (tmp <= 21) && ((buf[i + 5] & 0x80) == 0x80)); /* 5:index 21 0x80:frame start marker */

        if (new_pic) {
            find_start = TD_TRUE;
            i += 6; /* 6:h265 frame start code length */
            break;
        }
    }

    for (; i < *read_len - 6; i++) { /* 6:h265 frame start code length */
        td_u32 tmp = (buf[i + 3] & 0x7E) >> 1; /* 0x7E:frame start marker 3:index */
        new_pic = (buf[i + 0] == 0 && buf[i + 1] == 0 && buf[i + 2] == 1 && /* 1 2:index */
            (tmp == 32 || tmp == 33 || tmp == 34 || tmp == 39 || tmp == 40 || /* 32 33 34 39 40:frame start marker */
            ((tmp <= 21) && (buf[i + 5] & 0x80) == 0x80))); /* 5:index 21 0x80:frame start marker */

        if (new_pic) {
            find_end = TD_TRUE;
            break;
        }
    }
    if (i > 0) {
        *read_len = i;
    }

    if (find_start == TD_FALSE) {
        sample_print("chn %d can not find H265 start code! read_len %d, used_bytes %d!\n", thread_param->chn_id,
            *read_len, used_bytes);
    }
    if (find_end == TD_FALSE) {
        *read_len = i + 6; /* 6:h265 frame start code length */
    }
    return;
}

td_u32 sample_comm_vdec_send_jpeg_frame_process(td_s32 *read_len, td_u8 *buf,
    vdec_thread_param *thread_param, td_s32 used_bytes)
{
    td_s32 i;
    td_u32 len;
    td_u32 start = 0;
    td_bool find_start = TD_FALSE;
    if (*read_len > thread_param->min_buf_size) {
        sample_print("chn %d read_len %d is bigger than buf_size %u!\n", thread_param->chn_id, *read_len,
            thread_param->min_buf_size);
        return start;
    }
    /* JPEG frame start marker */
    for (i = 0; i < *read_len - 1; i++) {
        if (buf[i] == 0xFF && buf[i + 1] == 0xD8) { /* 0xFF 0xD8:frame start marker */
            start = i;
            find_start = TD_TRUE;
            i = i + 2; /* 2:offset */
            break;
        }
    }

    for (; i < *read_len - 3; i++) { /* 3:jpeg frame start code length */
        if ((buf[i] == 0xFF) && (buf[i + 1] & 0xF0) == 0xE0) { /* 0xFF 0xF0 0xE0:frame start marker */
            len = (buf[i + 2] << 8) + buf[i + 3]; /* 2 3:index  8:left shift length */
            i += 1 + len;
        } else {
            break;
        }
    }

    for (; i < *read_len - 1; i++) {
        if (buf[i] == 0xFF && buf[i + 1] == 0xD9) { /* 0xFF 0xD9:frame start marker */
            break;
        }
    }
    *read_len = i + 2; /* 2:offset */

    if (find_start == TD_FALSE) {
        sample_print("chn %d can not find JPEG start code! read_len %d, used_bytes %d!\n", thread_param->chn_id,
            *read_len, used_bytes);
    }
    return start;
}

td_u32 sample_comm_vdec_cut_frame(td_s32 *read_len, td_u8 *buf, vdec_thread_param *thread_param,
    td_s32 used_bytes, td_bool *end_of_stream)
{
    td_u32 start = 0;
    if ((thread_param->stream_mode == OT_VDEC_SEND_MODE_FRAME ||
        thread_param->stream_mode == OT_VDEC_SEND_MODE_COMPAT) && thread_param->type == OT_PT_H264) {
        sample_comm_vdec_send_h264_frame_process(read_len, buf, thread_param, used_bytes);
    } else if ((thread_param->stream_mode == OT_VDEC_SEND_MODE_FRAME ||
        thread_param->stream_mode == OT_VDEC_SEND_MODE_COMPAT) && thread_param->type == OT_PT_H265) {
        sample_comm_vdec_send_h265_frame_process(read_len, buf, thread_param, used_bytes);
    } else if ((thread_param->stream_mode == OT_VDEC_SEND_MODE_FRAME ||
        thread_param->stream_mode == OT_VDEC_SEND_MODE_COMPAT) && thread_param->type == OT_PT_MP4VIDEO) {
        sample_comm_vdec_send_mpeg4_frame_process(read_len, buf, thread_param, used_bytes);
    } else if ((thread_param->stream_mode == OT_VDEC_SEND_MODE_FRAME ||
        thread_param->stream_mode == OT_VDEC_SEND_MODE_COMPAT) && (thread_param->type == OT_PT_MJPEG ||
        thread_param->type == OT_PT_JPEG)) {
        start = sample_comm_vdec_send_jpeg_frame_process(read_len, buf, thread_param, used_bytes);
    } else {
        if ((*read_len != 0) && (*read_len < thread_param->min_buf_size)) {
            *end_of_stream = TD_TRUE;
        }
    }
    return start;
}

td_void sample_comm_vdec_send_stream_proc(vdec_thread_param *thread_param,
    ot_vdec_stream *stream, td_bool end_of_stream)
{
    td_s32 ret;
    td_s32 total_len = stream->len;
    td_s32 cur_len = 0;
    td_s32 i;
    td_u8 *base = stream->addr;
    td_s32 cnt = (thread_param->stream_mode == OT_VDEC_SEND_MODE_COMPAT) ? SEND_STREAM_CNT : 1;
    for (i = 0; i < cnt; i++) {
        stream->addr = base + cur_len;
        if (i == cnt - 1) {
            stream->len = total_len - cur_len;
            stream->end_of_frame = TD_TRUE;
            stream->end_of_stream = end_of_stream;
        } else {
            stream->len = total_len / cnt;
            cur_len += stream->len;
            stream->end_of_frame = TD_FALSE;
            stream->end_of_stream = TD_FALSE;
        }

        ret = ss_mpi_vdec_send_stream(thread_param->chn_id, stream, thread_param->milli_sec);
        while ((ret != TD_SUCCESS) && (thread_param->e_thread_ctrl == THREAD_CTRL_START)) {
            usleep(thread_param->interval_time);
            ret = ss_mpi_vdec_send_stream(thread_param->chn_id, stream, thread_param->milli_sec);
        }
    }
}

td_void sample_comm_vdec_handle_send_stream(vdec_thread_param *thread_param, ot_vdec_stream *stream,
    td_bool *end_of_stream, td_u64 *pts)
{
    td_u64 cur_time;

    while (thread_param->e_thread_ctrl == THREAD_CTRL_START) {
        ss_mpi_sys_get_cur_pts(&cur_time);
        if ((thread_param->last_time == 0) ||
            ((cur_time - thread_param->last_time) >= (VDEC_SECOND / thread_param->fps - thread_param->time_gap))) {
            sample_comm_vdec_send_stream_proc(thread_param, stream, *end_of_stream);
            *end_of_stream = TD_FALSE;
            *pts += thread_param->pts_increase;
            if (thread_param->last_time != 0) {
                thread_param->time_gap =
                    ((cur_time - thread_param->last_time) >= (VDEC_SECOND / thread_param->fps)) ?
                    (cur_time - thread_param->last_time - (VDEC_SECOND / thread_param->fps)) : 0;
                thread_param->time_gap = (thread_param->time_gap > (VDEC_SECOND / thread_param->fps)) ?
                    (VDEC_SECOND / thread_param->fps) : thread_param->time_gap;
            }

            thread_param->last_time = cur_time;
            break;
        } else {
            usleep(thread_param->interval_time);
        }
    }
    return;
}

td_void sample_comm_vdec_send_stream_process(vdec_thread_param *thread_param, FILE *fp_strm,
    td_u8 *buf)
{
    td_bool end_of_stream;
    td_s32 used_bytes = 0;
    td_s32 read_len;
    td_u64 pts = thread_param->pts_init;
    td_u32 start;
    ot_vdec_stream stream;

    thread_param->last_time = 0;
    thread_param->time_gap = 0;
    while (thread_param->e_thread_ctrl != THREAD_CTRL_STOP) {
        if (thread_param->e_thread_ctrl == THREAD_CTRL_PAUSE) {
            sleep(1);
            continue;
        }

        end_of_stream = TD_FALSE;
        if (fseek(fp_strm, used_bytes, SEEK_SET) != 0) {
            break;
        }
        read_len = fread(buf, 1, thread_param->min_buf_size, fp_strm);
        if (read_len == 0) {
            if (thread_param->circle_send == TD_TRUE) {
                (td_void)memset_s(&stream, sizeof(ot_vdec_stream), 0, sizeof(ot_vdec_stream));
                stream.end_of_stream = TD_TRUE;
                ss_mpi_vdec_send_stream(thread_param->chn_id, &stream, -1);

                used_bytes = 0;
                fseek(fp_strm, 0, SEEK_SET);
                read_len = fread(buf, 1, thread_param->min_buf_size, fp_strm);
            } else {
                break;
            }
        }

        start = sample_comm_vdec_cut_frame(&read_len, buf, thread_param, used_bytes, &end_of_stream);

        stream.pts = pts;
        stream.addr = buf + start;
        stream.len = read_len;
        stream.end_of_frame = (thread_param->stream_mode == OT_VDEC_SEND_MODE_FRAME ||
            thread_param->stream_mode == OT_VDEC_SEND_MODE_COMPAT) ? TD_TRUE : TD_FALSE;
        stream.end_of_stream = end_of_stream;
        stream.need_display = 1;

        sample_comm_vdec_handle_send_stream(thread_param, &stream, &end_of_stream, &pts);
        used_bytes += read_len + start;
    }
    return;
}

td_s32 sample_comm_vdec_check_send_stream_param(vdec_thread_param *thread_param,
    td_char *c_stream_file, td_u32 arr_len)
{
    if (thread_param->c_file_path[0] == '\0' || thread_param->c_file_name[0] == '\0') {
        sample_print("chn %d stream file path or stream file name is NULL\n", thread_param->chn_id);
        return TD_FAILURE;
    }

    if (arr_len <= 1) {
        sample_print("chn %d arr length might be overflow\n", thread_param->chn_id);
        return TD_FAILURE;
    }

    if (snprintf_s(c_stream_file, arr_len, arr_len - 1, "%s/%s", thread_param->c_file_path,
        thread_param->c_file_name) < 0) {
        sample_print("chn %d config stream file failed!\n", thread_param->chn_id);
        return TD_FAILURE;
    }

    if (thread_param->min_buf_size <= 0) {
        sample_print("chn %d min_buf_size should greater than zero!\n", thread_param->chn_id);
        return TD_FAILURE;
    }

    if (thread_param->fps <= 0 || thread_param->fps > 300) { /* 0~300:frame rate limit */
        sample_print("chn %d fps should be [1, 300]!\n", thread_param->chn_id);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_comm_vdec_open_stream_file(const vdec_thread_param *thread_param,
    const td_char *stream_file, td_char *path, td_u32 path_len, FILE **fp_strm)
{
    td_char resolved_path[PATH_MAX] = {0};

    if (strlen(stream_file) >= PATH_MAX || path_len < PATH_MAX ||
        realpath(stream_file, resolved_path) == TD_NULL) {
        sample_print("chn %d Invalid stream path. Please check!\n", thread_param->chn_id);
        return TD_FAILURE;
    }
    if (strncpy_s(path, path_len, resolved_path, strlen(resolved_path)) != EOK) {
        sample_print("chn %d copy stream path failed!\n", thread_param->chn_id);
        return TD_FAILURE;
    }

    *fp_strm = fopen(path, "rb");
    if (*fp_strm == TD_NULL) {
        sample_print("chn %d can't open file %s in send stream thread!\n", thread_param->chn_id, stream_file);
        return TD_SUCCESS;
    }
    return TD_SUCCESS;
}

static td_void sample_comm_vdec_send_stream_end(const vdec_thread_param *thread_param)
{
    ot_vdec_stream stream;

    (td_void)memset_s(&stream, sizeof(ot_vdec_stream), 0, sizeof(ot_vdec_stream));
    stream.end_of_stream = TD_TRUE;
    ss_mpi_vdec_send_stream(thread_param->chn_id, &stream, -1);
}

td_void *sample_comm_vdec_send_stream(td_void *args)
{
    vdec_thread_param *thread_param = (vdec_thread_param *)args;
    FILE *fp_strm = TD_NULL;
    td_u8 *buf = TD_NULL;
    td_char c_stream_file[FILE_NAME_LEN];
    td_char path[PATH_MAX] = {0};

    prctl(PR_SET_NAME, "video_send_stream", 0, 0, 0);

    if (sample_comm_vdec_check_send_stream_param(thread_param, c_stream_file, FILE_NAME_LEN) != TD_SUCCESS) {
        return (td_void *)(TD_FAILURE);
    }

    if (sample_comm_vdec_open_stream_file(thread_param, c_stream_file, path, sizeof(path), &fp_strm) != TD_SUCCESS) {
        return (td_void *)(TD_FAILURE);
    }
    if (fp_strm == TD_NULL) {
        return (td_void *)TD_SUCCESS;
    }
    printf("\n \033[0;36m chn %d, stream file:%s, userbufsize: %d \033[0;39m\n", thread_param->chn_id,
        thread_param->c_file_name, thread_param->min_buf_size);

    buf = malloc(thread_param->min_buf_size);
    if (buf == TD_NULL) {
        sample_print("chn %d can't alloc %d in send stream thread!\n",
            thread_param->chn_id, thread_param->min_buf_size);
        goto end;
    }
    if (fflush(stdout) != 0) {
        goto end;
    }

    sample_comm_vdec_send_stream_process(thread_param, fp_strm, buf);

    sample_comm_vdec_send_stream_end(thread_param);

    printf("\033[0;35m chn %d send steam thread return ...  \033[0;39m\n", thread_param->chn_id);
    if (fflush(stdout) != 0) {
        sample_print("fflush stdout failed!\n");
    }
    if (buf != TD_NULL) {
        free(buf);
        buf = TD_NULL;
    }
end:
    if (fclose(fp_strm) != 0) {
        sample_print("fclose stream file failed!\n");
    }
    fp_strm = TD_NULL;
    return (td_void *)TD_SUCCESS;
}

td_void sample_comm_vdec_cmd_not_circle_send(td_u32 chn_num, vdec_thread_param *vdec_send,
    pthread_t *vdec_thread, td_u32 send_arr_len, td_u32 thread_arr_len)
{
    td_u32 i;
    td_s32 ret;
    ot_vdec_chn_status status;
    td_bool wait_done;
    printf("decoding..............");
    for (i = 0; (i < chn_num) && (i < send_arr_len) && (i < thread_arr_len); i++) {
        if (vdec_thread[i] != 0) {
            ret = pthread_join(vdec_thread[i], TD_NULL);
            if (ret == 0) {
                vdec_thread[i] = 0;
            }
        }
        vdec_thread[i] = 0;
        wait_done = TD_FALSE;
        while (wait_done != TD_TRUE) {
            ret = ss_mpi_vdec_query_status(vdec_send[i].chn_id, &status);
            if (ret != TD_SUCCESS) {
                printf("chn %d vdec query status fail!!\n", ret);
                return;
            }
            if ((status.left_stream_bytes == 0) && (status.left_stream_frames == 0)) {
                sample_comm_vdec_print_chn_status(vdec_send[i].chn_id, status);
                wait_done = TD_TRUE;
            }
            usleep(1000); /* 1000:Decoding wait time */
        }
    }
    printf("end!\n");
    return;
}

td_void sample_comm_vdec_start_send_stream(td_s32 chn_num, vdec_thread_param *vdec_send,
    pthread_t *vdec_thread, td_u32 send_arr_len, td_u32 thread_arr_len)
{
    td_u32 i;
    if ((vdec_send == TD_NULL) || (vdec_thread == TD_NULL)) {
        printf("vdec_send or vdec_thread can't be NULL!\n");
        return;
    }
    for (i = 0; (i < (td_u32)chn_num) && (i < send_arr_len) && (i < thread_arr_len); i++) {
        vdec_thread[i] = 0;
        pthread_create(&vdec_thread[i], 0, sample_comm_vdec_send_stream, (td_void *)&vdec_send[i]);
    }
}

td_void sample_comm_vdec_stop_send_stream(td_s32 chn_num, vdec_thread_param *vdec_send,
    pthread_t *vdec_thread, td_u32 send_arr_len, td_u32 thread_arr_len)
{
    td_u32 i;
    if ((vdec_send == TD_NULL) || (vdec_thread == TD_NULL)) {
        printf("vdec_send or vdec_thread can't be NULL!\n");
        return;
    }
    for (i = 0; (i < (td_u32)chn_num) && (i < send_arr_len) && (i < thread_arr_len); i++) {
        vdec_send[i].e_thread_ctrl = THREAD_CTRL_STOP;
        if (vdec_thread[i] != 0) {
            pthread_join(vdec_thread[i], TD_NULL);
            vdec_thread[i] = 0;
        }
        ss_mpi_vdec_stop_recv_stream(i);
    }
}

td_void sample_comm_vdec_config_attr(ot_vdec_chn_attr *chn_attr, sample_vdec_attr *sample_vdec)
{
    ot_pic_buf_attr buf_attr = { 0 };
    chn_attr->type = sample_vdec->type;
    chn_attr->mode = sample_vdec->mode;
    chn_attr->pic_width = sample_vdec->width;
    chn_attr->pic_height = sample_vdec->height;
    chn_attr->stream_buf_size = sample_vdec->width * sample_vdec->height;
    chn_attr->frame_buf_cnt = sample_vdec->frame_buf_cnt;
    buf_attr.align = 0;
    buf_attr.height = sample_vdec->height;
    buf_attr.width = sample_vdec->width;

    if (sample_vdec->type == OT_PT_H264 || sample_vdec->type == OT_PT_H265) {
        buf_attr.bit_width = sample_vdec->sample_vdec_video.bit_width;
        buf_attr.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
        chn_attr->video_attr.ref_frame_num = sample_vdec->sample_vdec_video.ref_frame_num;
        chn_attr->video_attr.temporal_mvp_en = 1;
        if ((sample_vdec->type == OT_PT_H264) &&
            (sample_vdec->sample_vdec_video.dec_mode != OT_VIDEO_DEC_MODE_IPB)) {
            chn_attr->video_attr.temporal_mvp_en = 0;
        }
        chn_attr->frame_buf_size = ot_vdec_get_pic_buf_size(chn_attr->type, &buf_attr);
    } else if (sample_vdec->type == OT_PT_JPEG || sample_vdec->type == OT_PT_MJPEG) {
        chn_attr->mode = OT_VDEC_SEND_MODE_FRAME;
        buf_attr.bit_width = OT_DATA_BIT_WIDTH_8;
        buf_attr.pixel_format = sample_vdec->sample_vdec_picture.pixel_format;
        chn_attr->frame_buf_size = ot_vdec_get_pic_buf_size(chn_attr->type, &buf_attr);
    }
    return;
}

td_s32 sample_comm_config_ldy_attr(td_s32 i, td_u32 height)
{
    ot_low_delay_info ldy_attr;
    if (g_vdec_line_ldy_en == TD_TRUE) {
        check_chn_return(ss_mpi_vdec_get_low_delay_attr(i, &ldy_attr), i, "ss_mpi_vdec_get_low_delay_attr");
        ldy_attr.enable = TD_TRUE;
        ldy_attr.line_cnt = SAMPLE_VDEC_LOW_DELAY_LINE_CNT;
        check_chn_return(ss_mpi_vdec_set_low_delay_attr(i, &ldy_attr), i, "ss_mpi_vdec_set_low_delay_attr");
        check_chn_return(ss_mpi_vdec_set_display_mode(i, OT_VIDEO_DISPLAY_MODE_PREVIEW), i,
            "ss_mpi_vdec_set_display_mode");
    }
    return TD_SUCCESS;
}

td_s32 sample_comm_vdec_start(td_s32 chn_num, sample_vdec_attr *sample_vdec, td_u32 arr_len)
{
    td_s32 i;
    td_s32 ret;
    ot_vdec_chn_attr chn_attr[OT_VDEC_MAX_CHN_NUM];
    ot_vdec_chn_pool pool;
    ot_vdec_chn_param chn_param;
    ot_vdec_mod_param mod_param;

    check_null_ptr_return(sample_vdec);
    if (arr_len > OT_VDEC_MAX_CHN_NUM) {
        sample_print("array size(%u) of chn_attr need < %u!\n", arr_len, OT_VDEC_MAX_CHN_NUM);
        return TD_FAILURE;
    }

    check_return(ss_mpi_vdec_get_mod_param(&mod_param), "vdec get mod param");
    mod_param.vb_src = g_vdec_vb_src;
    check_return(ss_mpi_vdec_set_mod_param(&mod_param), "vdec set mod param");

    for (i = 0; (i < chn_num) && (i < (td_s32)arr_len); i++) {
        sample_comm_vdec_config_attr(&chn_attr[i], &sample_vdec[i]);

        check_chn_return(ss_mpi_vdec_create_chn(i, &chn_attr[i]), i, "vdec create chn");

        if ((g_vdec_vb_src == OT_VB_SRC_USER) && (i < OT_VB_MAX_POOLS)) {
            pool.pic_vb_pool = g_pic_vb_pool[i];
            pool.tmv_vb_pool = g_tmv_vb_pool[i];
            check_chn_return(ss_mpi_vdec_attach_vb_pool(i, &pool), i, "vdec attach vb pool");
        }

        check_chn_return(ss_mpi_vdec_get_chn_param(i, &chn_param), i, "vdec get chn param");
        if (sample_vdec[i].type == OT_PT_H264 || sample_vdec[i].type == OT_PT_H265 ||
            sample_vdec[i].type == OT_PT_MP4VIDEO) {
            chn_param.video_param.dec_mode = sample_vdec[i].sample_vdec_video.dec_mode;
            chn_param.video_param.compress_mode = OT_COMPRESS_MODE_NONE;
            chn_param.video_param.video_format = OT_VIDEO_FORMAT_LINEAR;
            if (chn_param.video_param.dec_mode == OT_VIDEO_DEC_MODE_IPB) {
                chn_param.video_param.out_order = OT_VIDEO_OUT_ORDER_DISPLAY;
            } else {
                chn_param.video_param.out_order = OT_VIDEO_OUT_ORDER_DEC;
            }
            chn_param.video_param.slice_input_en = sample_comm_vdec_get_lowdelay_en();
        } else {
            chn_param.pic_param.pixel_format = sample_vdec[i].sample_vdec_picture.pixel_format;
            chn_param.pic_param.alpha = sample_vdec[i].sample_vdec_picture.alpha;
        }
        chn_param.display_frame_num = sample_vdec[i].display_frame_num;
        check_chn_return(ss_mpi_vdec_set_chn_param(i, &chn_param), i, "vdec set chn param");

        ret = sample_comm_config_ldy_attr(i, chn_attr[i].pic_height);
        if (ret != TD_SUCCESS) {
            return ret;
        }
        check_chn_return(ss_mpi_vdec_start_recv_stream(i), i, "vdec start recv stream");
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_vdec_stop(td_s32 chn_num)
{
    td_s32 i;

    for (i = 0; i < chn_num; i++) {
        check_chn_return(ss_mpi_vdec_stop_recv_stream(i), i, "vdec stop recv stream");
        check_chn_return(ss_mpi_vdec_destroy_chn(i), i, "vdec destroy chn");
    }

    return TD_SUCCESS;
}
