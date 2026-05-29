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
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <inttypes.h>
#include "log.h"
#include "frame_cache.h"
#include "ot_camera.h"
#include "sample_comm.h"
#include "sample_venc.h"
#include "uvc_media.h"
#include "uvc.h"

#define SAMPLE_UVC_MAX_STREAM_NAME_LEN 64

#define SAMPLE_RETURN_CONTINUE  1
#define SAMPLE_RETURN_BREAK     2

static pthread_t g_venc_thread_pid;
static sample_venc_getstream_para g_get_stream_para;

typedef struct {
    FILE *file[OT_VENC_MAX_CHN_NUM];
    td_s32 venc_fd[OT_VENC_MAX_CHN_NUM];
    td_s32 maxfd;
    td_u32 picture_cnt[OT_VENC_MAX_CHN_NUM];
    ot_venc_chn venc_chn;
    td_char file_postfix[10]; /* 10 :file_postfix number */
    td_s32 chn_total;
    ot_payload_type pay_load_type[OT_VENC_MAX_CHN_NUM];
} sample_comm_venc_stream_proc_info;

static ot_payload_type change_to_mpp_format(uint32_t fcc)
{
    ot_payload_type t;

    switch (fcc) {
        case VIDEO_IMG_FORMAT_YUYV:
        case VIDEO_IMG_FORMAT_MJPEG:
            t = OT_PT_MJPEG;
            break;

        case VIDEO_IMG_FORMAT_H264:
            t = OT_PT_H264;
            break;

        default:
            t = OT_PT_MJPEG;
            break;
    }

    return t;
}

static ot_pic_size change_to_mpp_wh(int width)
{
    ot_pic_size s;

    switch (width) {
        case 640:           /* 640: width */
            s = PIC_360P;   /* 640 x 360 */
            break;
        case 1280:          /* 1280: width */
            s = PIC_720P;
            break;
        case 1920:          /* 1920: width */
            s = PIC_1080P;
            break;
        case 3840:          /* 3840: width */
            s = PIC_3840X2160;
            break;
        default:
            s = PIC_720P;
            break;
    }

    return s;
}

static td_void set_config_format(ot_payload_type *format, int idx)
{
    encoder_property property;
    sample_uvc_get_encoder_property(&property);

    format[idx] = change_to_mpp_format(property.format);
}

static td_void set_config_wh(ot_pic_size *wh, int idx)
{
    encoder_property property;
    sample_uvc_get_encoder_property(&property);

    wh[idx] = change_to_mpp_wh(property.width);
}

td_void set_user_config_format(ot_payload_type *format, ot_pic_size *wh, int *c)
{
    set_config_format(format, 0);
    set_config_wh(wh, 0);
    *c = 1;
}

int is_channel_yuv(int channel)
{
    encoder_property property;
    sample_uvc_get_encoder_property(&property);

    if ((channel == 1) && (property.format == VIDEO_IMG_FORMAT_YUYV)) {
        return 1;
    }

    return 0;
}

ot_pixel_format sample_comm_venc_get_pixel_format(td_void)
{
    encoder_property property;
    sample_uvc_get_encoder_property(&property);

    if (property.format == VIDEO_IMG_FORMAT_YUYV) {
        return OT_PIXEL_FORMAT_YVU_SEMIPLANAR_422;
    } else {
        return OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    }
}

static td_s32 sample_comm_venc_save_data(ot_venc_stream *stream, frame_node_t *fnode)
{
    td_u32 i;
    ot_venc_pack *data = TD_NULL;
    unsigned char *s = TD_NULL;
    unsigned int data_len;
    unsigned int copy_size;

    uvc_cache_t *uvc_cache = uvc_cache_get();

    fnode->used = 0;

    for (i = 0; i < stream->pack_cnt; ++i) {
        data = &stream->pack[i];
        s = data->addr + data->offset;
        data_len = data->len - data->offset;
        copy_size = data_len < (fnode->length - fnode->used) ? data_len : (fnode->length - fnode->used);

        if (copy_size > 0) {
            (td_void)memcpy_s(fnode->mem + fnode->used, copy_size, s, copy_size);
            fnode->used += copy_size;
        }
    }

    put_node_to_queue(uvc_cache->ok_queue, fnode);

    return TD_SUCCESS;
}

static td_s32 sample_comm_venc_save_stream(ot_payload_type type,
    FILE *fd, ot_venc_stream *stream, frame_node_t *fnode)
{
    td_s32 ret = TD_FAILURE;
#if (UVC_SAVE_FILE == 1)
    td_s32 i;
#endif

    if ((type == OT_PT_H264) || (type == OT_PT_MJPEG) || (type == OT_PT_H265)) {
        ret = sample_comm_venc_save_data(stream, fnode);
    }

#if (UVC_SAVE_FILE == 1)
    for (i = 0; i < stream->pack_cnt; ++i) {
        (void)fwrite(
            stream->pack[i].addr + stream->pack[i].offset, stream->pack[i].len - stream->pack[i].offset, 1, fd);
        (void)fflush(fd);
    }
#endif

    return ret;
}

/* function : get stream from each channels and save them */
#if (UVC_SAVE_FILE == 1)
static td_s32 sample_venc_open_file_not_jpg(td_u32 chn_index, ot_payload_type pay_load_type,
    char *sz_file_postfix, FILE **file)
{
    td_s32 ret;
    td_char asz_file_name[SAMPLE_UVC_MAX_STREAM_NAME_LEN];

    if (pay_load_type != OT_PT_JPEG) {
        ret = snprintf_s(asz_file_name, SAMPLE_UVC_MAX_STREAM_NAME_LEN,
            SAMPLE_UVC_MAX_STREAM_NAME_LEN - 1, "output/stream_chn%d%s", chn_index, sz_file_postfix);
        if (ret < 0) {
            sample_print("get stream name err!\n");
            return TD_FAILURE;
        }

        *file = fopen(asz_file_name, "wb");
        if (*file == TD_NULL) {
            sample_print("open file[%s] failed!\n", asz_file_name);
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static td_s32 sample_venc_open_file_jpg(td_u32 chn_index, ot_payload_type pay_load_type,
    td_u32 picture_cnt, char *sz_file_postfix, FILE **file)
{
    td_s32 ret;
    td_char asz_file_name[SAMPLE_UVC_MAX_STREAM_NAME_LEN];

    if (pay_load_type == OT_PT_JPEG) {
        ret = snprintf_s(asz_file_name, SAMPLE_UVC_MAX_STREAM_NAME_LEN,
            SAMPLE_UVC_MAX_STREAM_NAME_LEN - 1, "output/stream_chn%d_%d%s",
            chn_index, picture_cnt, sz_file_postfix);
        if (ret < 0) {
            sample_print("get stream name err!\n");
            return TD_FAILURE;
        }

        *file = fopen(asz_file_name, "wb");
        if (*file == TD_NULL) {
            sample_print("open file err!\n");
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}
#endif

static td_s32 sample_venc_get_stream(td_u32 chn_index, uvc_cache_t *uvc_cache, ot_venc_stream *stream)
{
    td_s32 ret;
    ot_venc_chn_status stat;

    ret = ss_mpi_venc_query_status(chn_index, &stat);
    if (ret != TD_SUCCESS) {
        sample_print("venc query status chn[%d] failed with %#x!\n", chn_index, ret);
        return SAMPLE_RETURN_BREAK;
    }

    if (stat.cur_packs == 0) {
        return SAMPLE_RETURN_CONTINUE;
    }

    /* step 2.3 : malloc corresponding number of pack nodes. */
    stream->pack = (ot_venc_pack *)malloc(sizeof(ot_venc_pack) * stat.cur_packs);
    if (stream->pack == TD_NULL) {
        sample_print("malloc stream pack failed!\n");
        return SAMPLE_RETURN_BREAK;
    }

    /* step 2.4 : call mpi to get one-frame stream */
    stream->pack_cnt = stat.cur_packs;
    ret = ss_mpi_venc_get_stream(chn_index, stream, TD_TRUE);
    if (ret != TD_SUCCESS) {
        free(stream->pack);
        stream->pack = TD_NULL;
        sample_print("venc get stream failed with %#x!\n", ret);
        return SAMPLE_RETURN_BREAK;
    }

    return TD_SUCCESS;
}

static td_s32 sample_venc_get_stream_from_one_channl(td_u32 chn_index, uvc_cache_t *uvc_cache,
    sample_comm_venc_stream_proc_info *stream_proc_info)
{
    td_s32 ret;
    ot_venc_stream stream = {0};
    frame_node_t *fnode = TD_NULL;

    get_node_from_queue(uvc_cache->free_queue, &fnode);
    if (fnode == TD_NULL) {
        return SAMPLE_RETURN_CONTINUE;
    }

    ret = sample_venc_get_stream(chn_index, uvc_cache, &stream);
    if (ret != TD_SUCCESS) {
        put_node_to_queue(uvc_cache->free_queue, fnode);
        return ret;
    }

    /* step 2.5 : save frame to file */
#if (UVC_SAVE_FILE == 1)
    ret = sample_venc_open_file_jpg(chn_index, stream_proc_info->pay_load_type[chn_index],
        stream_proc_info->picture_cnt[chn_index], stream_proc_info->file_postfix,
        &stream_proc_info->file[chn_index]);
    if (ret != TD_SUCCESS) {
        sample_print("sample_venc_open_file_jpg failed!\n");
        free(stream.pack);
        stream.pack = TD_NULL;
        put_node_to_queue(uvc_cache->free_queue, fnode);
        return ret;
    }
#endif

    ret = sample_comm_venc_save_stream(stream_proc_info->pay_load_type[chn_index],
        stream_proc_info->file[chn_index], &stream, fnode);
    if (ret != TD_SUCCESS) {
        free(stream.pack);
        stream.pack = TD_NULL;
        sample_print("save stream failed!\n");
        return SAMPLE_RETURN_BREAK;
    }

    /* step 2.6 : release stream */
    ret = ss_mpi_venc_release_stream(chn_index, &stream);
    if (ret != TD_SUCCESS) {
        sample_print("venc release stream failed!\n");
        free(stream.pack);
        stream.pack = TD_NULL;
        return SAMPLE_RETURN_BREAK;
    }

    /* step 2.7 : free pack nodes */
    free(stream.pack);
    stream.pack = TD_NULL;
    stream_proc_info->picture_cnt[chn_index]++;
    if (stream_proc_info->pay_load_type[chn_index] == OT_PT_JPEG) {
        fclose(stream_proc_info->file[chn_index]);
    }

    return TD_SUCCESS;
}

static td_void sample_venc_save_stream(uvc_cache_t *uvc_cache, fd_set *read_fds,
    sample_venc_getstream_para *para, sample_comm_venc_stream_proc_info *stream_proc_info)
{
    td_s32 ret;
    td_u32 i;

    for (i = 0; i < stream_proc_info->chn_total; i++) {
        if (FD_ISSET(stream_proc_info->venc_fd[i], read_fds)) {
            stream_proc_info->venc_chn = para->venc_chn[i];
            ret = sample_venc_get_stream_from_one_channl(i, uvc_cache, stream_proc_info);
            if (ret == SAMPLE_RETURN_CONTINUE) {
                continue;
            } else if (ret == SAMPLE_RETURN_BREAK) {
                break;
            }
        }
    }
}

static td_s32 sample_venc_prepare_get_stream(sample_comm_venc_stream_proc_info *stream_proc_info,
    sample_venc_getstream_para *para)
{
    td_s32 i;
    td_s32 ret;
    ot_venc_chn_attr venc_chn_attr;

    for (i = 0; i < stream_proc_info->chn_total; i++) {
        /* decide the stream file name, and open file to save stream */
        ret = ss_mpi_venc_get_chn_attr(para->venc_chn[i], &venc_chn_attr);
        if (ret != TD_SUCCESS) {
            sample_print("venc get chn attr chn[%d] failed with %#x!\n", para->venc_chn[i], ret);
            return TD_FAILURE;
        }
        stream_proc_info->pay_load_type[i] = venc_chn_attr.venc_attr.type;

        ret = sample_comm_venc_get_file_postfix(stream_proc_info->pay_load_type[i],
            stream_proc_info->file_postfix, sizeof(stream_proc_info->file_postfix));
        if (ret != TD_SUCCESS) {
            sample_print("sample_comm_venc_get_file_postfix [%d] failed with %#x!\n",
                venc_chn_attr.venc_attr.type, ret);
            return TD_FAILURE;
        }
#if (UVC_SAVE_FILE == 1)
        ret = sample_venc_open_file_not_jpg(i, stream_proc_info->pay_load_type[i],
            stream_proc_info->file_postfix, &stream_proc_info->file[i]);
        if (ret != TD_SUCCESS) {
            return TD_FAILURE;
        }
#endif
        /* set venc fd. */
        stream_proc_info->venc_fd[i] = ss_mpi_venc_get_fd(i);
        if (stream_proc_info->venc_fd[i] < 0) {
            sample_print("venc get fd failed with %#x!\n", stream_proc_info->venc_fd[i]);
            return TD_FAILURE;
        }
        if (stream_proc_info->maxfd <= stream_proc_info->venc_fd[i]) {
            stream_proc_info->maxfd = stream_proc_info->venc_fd[i];
        }
    }

    return TD_SUCCESS;
}

td_void *sample_comm_venc_get_venc_stream_proc(td_void *p)
{
    td_s32 i;
    sample_venc_getstream_para *para;
    fd_set read_fds;
    td_s32 ret;
    sample_comm_venc_stream_proc_info stream_proc_info = {0};

    uvc_cache_t *uvc_cache = uvc_cache_get();

    para = (sample_venc_getstream_para *)p;
    stream_proc_info.chn_total = para->cnt;

    if (uvc_cache == TD_NULL) {
        return TD_NULL;
    }

    /* step 1:  check & prepare save-file & venc-fd */
    if (stream_proc_info.chn_total >= OT_VENC_MAX_CHN_NUM) {
        sample_print("input count invalid\n");
        return TD_NULL;
    }

    ret = sample_venc_prepare_get_stream(&stream_proc_info, para);
    if (ret != TD_SUCCESS) {
        return TD_NULL;
    }

    /* step 2:  start to get streams of each channel. */
    while (para->thread_start == TD_TRUE) {
        FD_ZERO(&read_fds);
        for (i = 0; i < stream_proc_info.chn_total; i++) {
            FD_SET(stream_proc_info.venc_fd[i], &read_fds);
        }
        sample_venc_save_stream(uvc_cache, &read_fds, para, &stream_proc_info);
    }
    /* step 3 : close save-file */
#if (UVC_SAVE_FILE == 1)
    for (i = 0; i < stream_proc_info.chn_total; i++) {
        if (stream_proc_info.pay_load_type[i] != OT_PT_JPEG) {
            fclose(stream_proc_info.file[i]);
        }
    }
#endif
    return TD_NULL;
}

td_s32 sample_comm_venc_start_get_stream(ot_venc_chn venc_chn[], td_s32 cnt)
{
    td_s32 i;

    g_get_stream_para.thread_start = TD_TRUE;
    g_get_stream_para.cnt = cnt;
    for (i = 0; i < cnt; ++i) {
        g_get_stream_para.venc_chn[i] = venc_chn[i];
    }

    return pthread_create(&g_venc_thread_pid, 0,
        sample_comm_venc_get_venc_stream_proc, (td_void *)&g_get_stream_para);
}

td_s32 sample_comm_venc_stop_get_stream(td_void)
{
    if (g_get_stream_para.thread_start == TD_TRUE) {
        g_get_stream_para.thread_start = TD_FALSE;
        pthread_join(g_venc_thread_pid, 0);
    }
    return TD_SUCCESS;
}
