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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <limits.h>

#include "sample_comm.h"
#include "sdk_module_init.h"
#include "audio_aac_adp.h"
#include "audio_dl_adp.h"
#include "ot_resample.h"
#if defined(OT_VQE_USE_STATIC_MODULE_REGISTER)
#include "ot_vqe_register.h"
#endif

static ot_payload_type g_payload_type = OT_PT_AAC;
static td_bool g_aio_resample  = TD_FALSE;
static td_bool g_user_get_mode  = TD_FALSE;
static td_bool g_ao_volume_ctrl = TD_FALSE;
static ot_audio_sample_rate g_in_sample_rate  = OT_AUDIO_SAMPLE_RATE_BUTT;
static ot_audio_sample_rate g_out_sample_rate = OT_AUDIO_SAMPLE_RATE_BUTT;
/* 0: close, 1: record, 2: talk, 3: talkv2 */
static td_u32 g_ai_vqe_type = 1;
ot_ai_record_vqe_cfg g_ai_vqe_record_attr = { 0 };
ot_ai_talk_vqe_cfg g_ai_vqe_talk_attr = { 0 };
ot_ai_talk_vqe_v2_cfg g_ai_vqe_talk_v2_attr = { 0 };

static void *g_res_handle = TD_NULL;
static td_s16 *g_res_out_buf = TD_NULL;

static td_bool g_sample_audio_exit = TD_FALSE;

#define sample_dbg(ret) \
    do { \
        printf("ret = %#x, fuc:%s, line:%d\n", ret, __FUNCTION__, __LINE__); \
    } while (0)

#define sample_res_check_null_ptr(ptr) \
    do { \
        if ((td_u8*)(ptr) == TD_NULL) { \
            printf("ptr is TD_NULL,fuc:%s,line:%d\n", __FUNCTION__, __LINE__); \
            return TD_FAILURE; \
        } \
    } while (0)

#define OT_RES_LIB_NAME "libvqe_res.so"

/* function : PT number to string */
static char *sample_audio_pt2_str(ot_payload_type type)
{
    if (type == OT_PT_G711A) {
        return "g711a";
    } else if (type == OT_PT_G711U) {
        return "g711u";
    } else if (type == OT_PT_ADPCMA) {
        return "adpcm";
    } else if (type == OT_PT_G726) {
        return "g726";
    } else if (type == OT_PT_LPCM) {
        return "pcm";
    } else if (type == OT_PT_AAC) {
        return "aac";
    } else {
        return "data";
    }
}

typedef struct {
    td_bool start;
    td_s32  ai_dev;
    td_s32  ai_chn;
    ot_audio_sample_rate in_sample;
    ot_audio_sample_rate out_sample;
    td_u32 per_frame;
    FILE *fd;
    pthread_t ai_pid;
} ot_sample_ai_to_ext_res;

typedef td_void *(*ot_resample_create_callback)(td_s32 in_rate, td_s32 out_rate, td_s32 chn_num);
typedef td_s32 (*ot_resample_process_callback)(td_void *instance, const td_s16 *in_buf, td_s32 in_samples,
    td_s16 *out_buf);
typedef td_void (*ot_resample_destroy_callback)(td_void *instance);
typedef td_s32 (*ot_resample_get_max_output_num_callback)(td_void *instance, td_s32 in_samples);

typedef struct {
    td_void *lib_handle;
    ot_resample_create_callback resample_create_callback;
    ot_resample_process_callback resample_process_callback;
    ot_resample_destroy_callback resample_destroy_callback;
    ot_resample_get_max_output_num_callback resample_get_max_output_num_callback;
} ot_sample_res_fun;

static ot_sample_ai_to_ext_res   g_sample_ai_ext_res[OT_AI_DEV_MAX_NUM * OT_AI_MAX_CHN_NUM];
static ot_sample_res_fun     g_sample_res_fun = {0};

static td_u32 sample_audio_get_resample_multi(const ot_sample_ai_to_ext_res *ai_ctrl)
{
    td_u32 mulit = 1;

    if (ai_ctrl->in_sample % ai_ctrl->out_sample == 0) {
        mulit = ai_ctrl->in_sample / ai_ctrl->out_sample;
        if (ai_ctrl->per_frame % mulit == 0) {
            mulit = 1;
        }
    }
    return mulit;
}

static td_bool sample_audio_resample_direct(const ot_sample_ai_to_ext_res *ai_ctrl, const ot_audio_frame *frame)
{
    td_u32 out_sample = g_sample_res_fun.resample_process_callback(g_res_handle, (td_s16*)frame->virt_addr[0],
        ai_ctrl->per_frame, g_res_out_buf);
    if (fwrite(g_res_out_buf, 2, out_sample, ai_ctrl->fd) != (size_t)out_sample) { /* 2:byte */
        printf("%s: fwrite failed.\n", __FUNCTION__);
        return TD_FALSE;
    }
    return TD_TRUE;
}

static td_void sample_audio_ai_to_res_proc(ot_sample_ai_to_ext_res *ai_ctrl, ot_audio_frame *frame,
    td_s16 *cache, td_u16 *cache_count, td_u16 cache_sample)
{
    td_u32 proc_frame;
    td_u32 out_sample;
    td_u32 cur_sample;
    td_u32 ret;
    td_u32 mulit = 1;

    mulit = sample_audio_get_resample_multi(ai_ctrl);
    /* send frame to encoder */
    if (mulit != 1) {
        if (*cache_count != 0) {
            if (cache_sample < *cache_count) {
                return;
            }
            ret = memcpy_s(cache + (*cache_count), (cache_sample - *cache_count) * sizeof(td_s16),
                frame->virt_addr[0], (mulit - *cache_count) * sizeof(td_s16));
            if (ret != EOK) {
                printf("%s: memcpy_s failed, ret:0x%x.\n", __FUNCTION__, ret);
                return;
            }
            out_sample = g_sample_res_fun.resample_process_callback(g_res_handle, cache, mulit, g_res_out_buf);
            (td_void)fwrite(g_res_out_buf, 2, out_sample, ai_ctrl->fd); /* 2:byte */
        }

        proc_frame = ai_ctrl->per_frame - (mulit - *cache_count);
        proc_frame = proc_frame - proc_frame % mulit;
        out_sample = g_sample_res_fun.resample_process_callback(g_res_handle, (td_s16*)(frame->virt_addr[0] +
            (mulit - *cache_count) * sizeof(td_s16)), proc_frame, g_res_out_buf);
        (td_void)fwrite(g_res_out_buf, 2, out_sample, ai_ctrl->fd); /* 2:byte */

        cur_sample = proc_frame + (mulit - *cache_count);
        if ((ai_ctrl->per_frame - cur_sample) != 0) {
            ret = memcpy_s(cache, cache_sample * sizeof(td_s16),
                frame->virt_addr[0] + cur_sample * sizeof(td_s16), (ai_ctrl->per_frame - cur_sample) * sizeof(td_s16));
            if (ret != EOK) {
                printf("%s: memcpy_s failed, ret:0x%x.\n", __FUNCTION__, ret);
                return;
            }
        }
        *cache_count = ai_ctrl->per_frame - cur_sample;
    } else {
        if (sample_audio_resample_direct(ai_ctrl, frame) != TD_TRUE) {
            return;
        }
    }

    if (fflush(ai_ctrl->fd) != 0) {
        printf("%s: fflush failed.\n", __FUNCTION__);
    }
}

static td_s32 sample_audio_ai_to_res_init(ot_sample_ai_to_ext_res *ai_ctrl)
{
    ot_ai_chn_param ai_chn_para;
    td_s32 ret;
    td_s32 out_max_num;
    sample_res_check_null_ptr(g_sample_res_fun.resample_create_callback);
    sample_res_check_null_ptr(g_sample_res_fun.resample_process_callback);
    sample_res_check_null_ptr(g_sample_res_fun.resample_destroy_callback);
    sample_res_check_null_ptr(g_sample_res_fun.resample_get_max_output_num_callback);

    ret = ss_mpi_ai_get_chn_param(ai_ctrl->ai_dev, ai_ctrl->ai_chn, &ai_chn_para);
    if (ret != TD_SUCCESS) {
        printf("%s: Get ai chn param failed\n", __FUNCTION__);
        return ret;
    }

    ai_chn_para.usr_frame_depth = 30; /* 30:frame depth */

    ret = ss_mpi_ai_set_chn_param(ai_ctrl->ai_dev, ai_ctrl->ai_chn, &ai_chn_para);
    if (ret != TD_SUCCESS) {
        printf("%s: set ai chn param failed, ret=0x%x\n", __FUNCTION__, ret);
        return ret;
    }

    /* Create Resample. only support mono channel. */
    g_res_handle = g_sample_res_fun.resample_create_callback(ai_ctrl->in_sample, ai_ctrl->out_sample, 1);
    if (g_res_handle == TD_NULL) {
        printf("%s: ot_resampler_create failed, ret=0x%x\n", __FUNCTION__, ret);
        return TD_FAILURE;
    }

    out_max_num = g_sample_res_fun.resample_get_max_output_num_callback(g_res_handle, ai_ctrl->per_frame);
    if (out_max_num <= 0) {
        printf("get max output num failed\n");
        g_sample_res_fun.resample_destroy_callback(g_res_handle);
        g_res_handle = TD_NULL;
        return TD_FAILURE;
    }

    g_res_out_buf = malloc(out_max_num * sizeof(td_s16) + 2); /* 2:Reserved space */
    if (g_res_out_buf == TD_NULL) {
        printf("malloc failed\n");
        g_sample_res_fun.resample_destroy_callback(g_res_handle);
        g_res_handle = TD_NULL;
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

#ifndef CONFIG_USER_SPACE
static td_s32 sample_comm_audio_select(td_s32 ai_fd, fd_set *read_fds)
{
    td_s32 ret;
    struct timeval time_out_val;
    time_out_val.tv_sec = 1;
    time_out_val.tv_usec = 0;

    FD_ZERO(read_fds);
    FD_SET(ai_fd, read_fds);

    ret = select(ai_fd + 1, read_fds, TD_NULL, TD_NULL, &time_out_val);
    if (ret < 0) {
        return TD_FAILURE;
    } else if (ret == 0) {
        printf("%s: get ai frame select time out\n", __FUNCTION__);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}
#endif

static td_s32 sample_audio_ai_ext_res_one_frame(ot_sample_ai_to_ext_res *ai_ctrl,
    td_s16 *cache, td_u16 *cache_count, td_u32 cache_len)
{
    td_s32 ret;
    ot_audio_frame frame = {0};
    ot_aec_frame aec_frm = {0};

    (td_void)memset_s(&aec_frm, sizeof(ot_aec_frame), 0, sizeof(ot_aec_frame));
    ret = ss_mpi_ai_get_frame(ai_ctrl->ai_dev, ai_ctrl->ai_chn, &frame, &aec_frm, -1);
    if (ret != TD_SUCCESS) {
        return TD_SUCCESS;
    }

    sample_audio_ai_to_res_proc(ai_ctrl, &frame, cache, cache_count, cache_len);
    ret = ss_mpi_ai_release_frame(ai_ctrl->ai_dev, ai_ctrl->ai_chn, &frame, &aec_frm);
    if (ret != TD_SUCCESS) {
        printf("%s: ss_mpi_ai_release_frame(%d, %d), failed with %#x!\n", \
            __FUNCTION__, ai_ctrl->ai_dev, ai_ctrl->ai_chn, ret);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_audio_ai_ext_res_prepare(ot_sample_ai_to_ext_res *ai_ctrl, td_s32 *ai_fd)
{
    td_s32 ret = sample_audio_ai_to_res_init(ai_ctrl);
    if (ret != TD_SUCCESS) {
        printf("%s: sample_audio_ai_to_res_param_set failed\n", __FUNCTION__);
        return TD_FAILURE;
    }

#ifndef CONFIG_USER_SPACE
    *ai_fd = ss_mpi_ai_get_fd(ai_ctrl->ai_dev, ai_ctrl->ai_chn);
    if (*ai_fd < 0) {
        printf("%s: get ai fd failed\n", __FUNCTION__);
        return TD_FAILURE;
    }
#else
    ot_unused(ai_fd);
#endif
    return TD_SUCCESS;
}

typedef struct {
    ot_sample_ai_to_ext_res *ai_ctrl;
    td_s32 ai_fd;
    fd_set *read_fds;
    td_s16 *cache;
    td_u16 *cache_count;
    td_u32 cache_len;
} sample_audio_ai_ext_res_ctx;

static td_s32 sample_audio_ai_ext_res_process_once(const sample_audio_ai_ext_res_ctx *res_ctx)
{
#ifndef CONFIG_USER_SPACE
    td_s32 ret = sample_comm_audio_select(res_ctx->ai_fd, res_ctx->read_fds);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (!FD_ISSET(res_ctx->ai_fd, res_ctx->read_fds)) {
        return TD_SUCCESS;
    }
#else
    ot_unused(res_ctx);
#endif
    return sample_audio_ai_ext_res_one_frame(res_ctx->ai_ctrl, res_ctx->cache, res_ctx->cache_count,
        res_ctx->cache_len);
}

/* function : get frame from Ai, send it to Resampler */
void *sample_comm_audio_ai_ext_res_proc(void *parg)
{
    td_s32 ret;
    ot_sample_ai_to_ext_res *ai_ctrl = (ot_sample_ai_to_ext_res *)parg;
    td_s32 ai_fd = -1;
    fd_set read_fds;
    td_u16 cache_count = 0;
    td_s16 cache[16]; /* 16:Max 64 / 8 * 2. */
    sample_audio_ai_ext_res_ctx res_ctx;

    if (sample_audio_ai_ext_res_prepare(ai_ctrl, &ai_fd) != TD_SUCCESS) {
        return TD_NULL;
    }
    res_ctx.ai_ctrl = ai_ctrl;
    res_ctx.ai_fd = ai_fd;
    res_ctx.read_fds = &read_fds;
    res_ctx.cache = cache;
    res_ctx.cache_count = &cache_count;
    res_ctx.cache_len = sizeof(cache) / sizeof(td_s16);

    while (ai_ctrl->start) {
        ret = sample_audio_ai_ext_res_process_once(&res_ctx);
        if (ret != TD_SUCCESS) {
            break;
        }
    }

    ai_ctrl->start = TD_FALSE;
    free(g_res_out_buf);
    g_res_out_buf = TD_NULL;
    g_sample_res_fun.resample_destroy_callback(g_res_handle);
    g_res_handle = TD_NULL;

    return TD_NULL;
}

/* function : Create the thread to get frame from ai and send to aenc */
td_s32 sample_comm_audio_creat_trd_ai_ext_res(ot_aio_attr *aio_attr, ot_audio_dev ai_dev,
    ot_ai_chn ai_chn, ot_audio_sample_rate out_sample_rate, FILE *res_fd)
{
    ot_sample_ai_to_ext_res *ai_to_ext_res = TD_NULL;

    if ((ai_dev >= OT_AI_DEV_MAX_NUM) || (ai_dev < 0) ||
        (ai_chn >= OT_AI_MAX_CHN_NUM) || (ai_chn < 0)) {
        printf("%s: ai_dev = %d, ai_chn = %d error.\n", __FUNCTION__, ai_dev, ai_chn);
        return TD_FAILURE;
    }

    ai_to_ext_res = &g_sample_ai_ext_res[ai_dev * OT_AI_MAX_CHN_NUM + ai_chn];
    ai_to_ext_res->ai_dev = ai_dev;
    ai_to_ext_res->ai_chn = ai_chn;
    ai_to_ext_res->in_sample = aio_attr->sample_rate;
    ai_to_ext_res->out_sample = out_sample_rate;
    ai_to_ext_res->per_frame = aio_attr->point_num_per_frame;
    ai_to_ext_res->fd = res_fd;
    ai_to_ext_res->start = TD_TRUE;
    pthread_create(&ai_to_ext_res->ai_pid, 0, sample_comm_audio_ai_ext_res_proc, ai_to_ext_res);

    return TD_SUCCESS;
}

/* function : Destory the thread to get frame from ai and send to extern resamler */
td_s32 sample_comm_audio_destroy_trd_ai_ext_res(ot_audio_dev ai_dev, ot_ai_chn ai_chn)
{
    ot_sample_ai_to_ext_res *ai_to_ext_res = TD_NULL;

    if ((ai_dev >= OT_AI_DEV_MAX_NUM) || (ai_dev < 0) ||
        (ai_chn >= OT_AI_MAX_CHN_NUM) || (ai_chn < 0)) {
        printf("%s: ai_dev = %d, ai_chn = %d error.\n", __FUNCTION__, ai_dev, ai_chn);
        return TD_FAILURE;
    }

    ai_to_ext_res = &g_sample_ai_ext_res[ai_dev * OT_AI_MAX_CHN_NUM + ai_chn];
    if (ai_to_ext_res->start) {
        ai_to_ext_res->start = TD_FALSE;
        pthread_join(ai_to_ext_res->ai_pid, 0);
    }

    if (ai_to_ext_res->fd != TD_NULL) {
        fclose(ai_to_ext_res->fd);
        ai_to_ext_res->fd = TD_NULL;
    }

    return TD_SUCCESS;
}

/* function : DeInit resamle functions */
static td_void sample_audio_deinit_ext_res_fun(td_void)
{
    td_s32 ret;

#if defined(OT_VQE_USE_STATIC_MODULE_REGISTER)
    ret = memset_s(&g_sample_res_fun, sizeof(ot_sample_res_fun), 0, sizeof(ot_sample_res_fun));
    if (ret != EOK) {
        printf("%s: g_sample_res_fun init failed with %d!\n", __FUNCTION__, ret);
        return;
    }
#else
    if (g_sample_res_fun.lib_handle != TD_NULL) {
        audio_dlclose(g_sample_res_fun.lib_handle);
        ret = memset_s(&g_sample_res_fun, sizeof(ot_sample_res_fun), 0, sizeof(ot_sample_res_fun));
        if (ret != EOK) {
            printf("%s: g_sample_res_fun init failed with %d!\n", __FUNCTION__, ret);
            return;
        }
    }
#endif
}

/* function : Init resamle functions */
static td_s32 sample_audio_init_ext_res_fun(td_void)
{
    ot_sample_res_fun sample_res_fun = {0};

    sample_audio_deinit_ext_res_fun();

#if defined(OT_VQE_USE_STATIC_MODULE_REGISTER)
    sample_res_fun.resample_create_callback = ot_resample_create;
    sample_res_fun.resample_process_callback = ot_resample_process;
    sample_res_fun.resample_destroy_callback = ot_resample_destroy;
    sample_res_fun.resample_get_max_output_num_callback = ot_resample_get_max_output_num;
#else
    td_s32 ret;
    ret = audio_dlopen(&(sample_res_fun.lib_handle), OT_RES_LIB_NAME);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "load resample lib fail!\n");
        return TD_FAILURE;
    }

    ret = audio_dlsym((td_void**)&(sample_res_fun.resample_create_callback), sample_res_fun.lib_handle,
        "ot_resample_create");
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "find symbol error!\n");
        goto dysym_fail;
    }

    ret = audio_dlsym((td_void**)&(sample_res_fun.resample_process_callback), sample_res_fun.lib_handle,
        "ot_resample_process");
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "find symbol error!\n");
        goto dysym_fail;
    }

    ret = audio_dlsym((td_void**)&(sample_res_fun.resample_destroy_callback), sample_res_fun.lib_handle,
        "ot_resample_destroy");
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "find symbol error!\n");
        goto dysym_fail;
    }

    ret = audio_dlsym((td_void**)&(sample_res_fun.resample_get_max_output_num_callback), sample_res_fun.lib_handle,
        "ot_resample_get_max_output_num");
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "find symbol error!\n");
        goto dysym_fail;
    }
#endif

    (td_void)memcpy_s(&g_sample_res_fun, sizeof(g_sample_res_fun), &sample_res_fun, sizeof(sample_res_fun));

    return TD_SUCCESS;

#if !defined(OT_VQE_USE_STATIC_MODULE_REGISTER)
dysym_fail:
    audio_dlclose(sample_res_fun.lib_handle);
    sample_res_fun.lib_handle = TD_NULL;
    return TD_FAILURE;
#endif
}

static td_void sample_audio_exit_proc(td_void)
{
    td_u32 dev_id;
    td_u32 chn_id;

    for (dev_id = 0; dev_id < OT_AI_DEV_MAX_NUM; dev_id++) {
        for (chn_id = 0; chn_id < OT_AI_MAX_CHN_NUM; chn_id++) {
            if (sample_comm_audio_destroy_trd_ai_ext_res(dev_id, chn_id) != TD_SUCCESS) {
                printf("%s: sample_comm_audio_destroy_trd_ai_ext_res(%d,%d) failed!\n", __FUNCTION__,
                    dev_id, chn_id);
            }
        }
    }
    sample_comm_audio_destory_all_trd();
    sample_comm_sys_exit();
}

static int smaple_audio_getchar(td_void)
{
    int c;

    if (g_sample_audio_exit == TD_TRUE) {
        sample_audio_exit_proc();
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
        exit(-1);
    }

    c = getchar();

    if (g_sample_audio_exit == TD_TRUE) {
        sample_audio_exit_proc();
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
        exit(-1);
    }

    return c;
}

/* function : open aenc file */
static FILE *sample_audio_open_aenc_file(ot_aenc_chn ae_chn, ot_payload_type type)
{
    FILE *fd = TD_NULL;
    td_char asz_file_name[FILE_NAME_LEN] = {0};
    td_s32 ret;

    /* create file for save stream */
#ifdef __LITEOS__
    ret = snprintf_s(asz_file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1,
        "/sharefs/audio_chn%d.%s", ae_chn, sample_audio_pt2_str(type));
#else
    ret = snprintf_s(asz_file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1,
        "audio_chn%d.%s", ae_chn, sample_audio_pt2_str(type));
#endif
    if (ret < 0) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "get aenc file name failed");
        return TD_NULL;
    }

    if (asz_file_name[0] == '\0') {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "aenc file name is NULL");
        return TD_NULL;
    }

    if (strlen(asz_file_name) > (FILE_NAME_LEN - 1)) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "aenc file name extra long");
        return TD_NULL;
    }

    fd = fopen(asz_file_name, "w+");
    if (fd == NULL) {
        printf("%s: open file %s failed\n", __FUNCTION__, asz_file_name);
        return NULL;
    }
    printf("open stream file:\"%s\" for aenc ok\n", asz_file_name);
    return fd;
}

/* function : Open ExtResample File */
static FILE *sample_audio_open_res_file(ot_ai_chn ai_chn)
{
    FILE *fd = TD_NULL;
    td_char asz_file_name[FILE_NAME_LEN] = {0};
    td_s32 ret;

    /* create file for save stream */
#ifdef __LITEOS__
    ret = snprintf_s(asz_file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1, "/sharefs/res_chn%d.pcm", ai_chn);
#else
    ret = snprintf_s(asz_file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1, "res_chn%d.pcm", ai_chn);
#endif
    if (ret < 0) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "get res file name failed");
        return TD_NULL;
    }

    if (asz_file_name[0] == '\0') {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "res file name is NULL");
        return TD_NULL;
    }

    if (strlen(asz_file_name) > (FILE_NAME_LEN - 1)) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "res file name extra long");
        return TD_NULL;
    }

    fd = fopen(asz_file_name, "w+");
    if (fd == TD_NULL) {
        printf("%s: open file %s failed\n", __FUNCTION__, asz_file_name);
        return TD_NULL;
    }
    printf("open stream file:\"%s\" for resample ok\n", asz_file_name);
    return fd;
}

/* function : open adec file */
static FILE *sample_audio_open_adec_file(ot_adec_chn ad_chn, ot_payload_type type)
{
    FILE *fd = TD_NULL;
    td_char asz_file_name[FILE_NAME_LEN] = {0};
    td_s32 ret;
    td_char path[PATH_MAX] = {0};

    /* create file for save stream */
#ifdef __LITEOS__
    ret = snprintf_s(asz_file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1,
        "/sharefs/audio_chn%d.%s", ad_chn, sample_audio_pt2_str(type));
#else
    ret = snprintf_s(asz_file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1,
        "audio_chn%d.%s", ad_chn, sample_audio_pt2_str(type));
#endif
    if (ret < 0) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "get adec file name failed");
        return TD_NULL;
    }

    if (asz_file_name[0] == '\0') {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "adec file name is NULL");
        return TD_NULL;
    }

    if (strlen(asz_file_name) > (FILE_NAME_LEN - 1)) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "adec file name extra long");
        return TD_NULL;
    }

    if (realpath(asz_file_name, path) == TD_NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "adec file name realpath fail");
        return TD_NULL;
    }

    fd = fopen(path, "rb");
    if (fd == NULL) {
        printf("%s: open file %s failed\n", __FUNCTION__, asz_file_name);
        return NULL;
    }
    printf("open stream file:\"%s\" for adec ok\n", asz_file_name);
    return fd;
}

static td_void sample_audio_set_ai_vqe_param(sample_comm_ai_vqe_param *ai_vqe_param,
    ot_audio_sample_rate out_sample_rate, td_bool resample_en, td_void *ai_vqe_attr, td_u32 ai_vqe_type)
{
    ai_vqe_param->out_sample_rate = out_sample_rate;
    ai_vqe_param->resample_en = resample_en;
    ai_vqe_param->ai_vqe_attr = ai_vqe_attr;
    ai_vqe_param->ai_vqe_type = ai_vqe_type;
}

static td_void sample_audio_adec_ao_init_param(ot_aio_attr *aio_attr, ot_audio_dev *ao_dev)
{
    aio_attr->sample_rate  = OT_AUDIO_SAMPLE_RATE_48000;
    aio_attr->bit_width    = OT_AUDIO_BIT_WIDTH_16;
    aio_attr->work_mode    = OT_AIO_MODE_I2S_MASTER;
    aio_attr->snd_mode     = OT_AUDIO_SOUND_MODE_STEREO;
    aio_attr->expand_flag  = 0;
    aio_attr->frame_num    = 30; /* 30:frame num */
    if (g_payload_type == OT_PT_AAC) {
        aio_attr->point_num_per_frame = AACLC_SAMPLES_PER_FRAME;
    } else {
        aio_attr->point_num_per_frame = SAMPLE_AUDIO_POINT_NUM_PER_FRAME;
    }
    aio_attr->chn_cnt      = 2; /* 2:chn num */
#ifdef OT_ACODEC_TYPE_INNER
    *ao_dev = SAMPLE_AUDIO_INNER_AO_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_INNERCODEC;
#else
    *ao_dev = SAMPLE_AUDIO_EXTERN_AO_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_EXTERN;
#endif
    g_aio_resample = TD_FALSE;
    g_in_sample_rate  = OT_AUDIO_SAMPLE_RATE_BUTT;
    g_out_sample_rate = OT_AUDIO_SAMPLE_RATE_BUTT;
}

td_void sample_audio_adec_ao_inner(ot_audio_dev ao_dev, ot_ao_chn ao_chn, ot_adec_chn ad_chn)
{
    td_s32 ret;
    FILE *fd = NULL;

    ret = sample_comm_audio_ao_bind_adec(ao_dev, ao_chn, ad_chn);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        return;
    }

    fd = sample_audio_open_adec_file(ad_chn, g_payload_type);
    if (fd == TD_NULL) {
        sample_dbg(TD_FAILURE);
        goto adec_ao_err0;
    }

    ret = sample_comm_audio_creat_trd_file_adec(ad_chn, fd);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        fclose(fd);
        fd = TD_NULL;
        goto adec_ao_err0;
    }

    printf("bind adec:%d to ao(%d,%d) ok \n", ad_chn, ao_dev, ao_chn);

    printf("\nplease press twice ENTER to exit this sample\n");
    smaple_audio_getchar();
    smaple_audio_getchar();

    ret = sample_comm_audio_destory_trd_file_adec(ad_chn);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

adec_ao_err0:
    ret = sample_comm_audio_ao_unbind_adec(ao_dev, ao_chn, ad_chn);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

    return;
}

/* function : file -> adec -> ao */
td_s32 sample_audio_adec_ao(td_void)
{
    td_s32 ret;
    td_u32 ao_chn_cnt;
    td_u32 adec_chn_cnt;
    ot_audio_dev ao_dev;
    ot_aio_attr aio_attr;
    const ot_ao_chn ao_chn = 0;
    const ot_adec_chn ad_chn = 0;

    sample_audio_adec_ao_init_param(&aio_attr, &ao_dev);

    adec_chn_cnt = aio_attr.chn_cnt >> ((td_u32)aio_attr.snd_mode);
    ret = sample_comm_audio_start_adec(adec_chn_cnt, g_payload_type);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto adec_ao_err3;
    }

    ao_chn_cnt = aio_attr.chn_cnt;
    ret = sample_comm_audio_start_ao(ao_dev, ao_chn_cnt, &aio_attr, g_in_sample_rate, g_aio_resample);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto adec_ao_err2;
    }

    ret = sample_comm_audio_cfg_acodec(&aio_attr);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto adec_ao_err1;
    }

    sample_audio_adec_ao_inner(ao_dev, ao_chn, ad_chn);

adec_ao_err1:
    ret = sample_comm_audio_stop_ao(ao_dev, ao_chn_cnt, g_aio_resample);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

adec_ao_err2:
    ret = sample_comm_audio_stop_adec(ad_chn);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

adec_ao_err3:
    return ret;
}

static td_void sample_audio_ai_aenc_init_param(ot_aio_attr *aio_attr, ot_audio_dev *ai_dev, ot_audio_dev *ao_dev)
{
    aio_attr->sample_rate  = OT_AUDIO_SAMPLE_RATE_48000;
    aio_attr->bit_width    = OT_AUDIO_BIT_WIDTH_16;
    aio_attr->work_mode    = OT_AIO_MODE_I2S_MASTER;
    aio_attr->snd_mode     = OT_AUDIO_SOUND_MODE_STEREO;
    aio_attr->expand_flag  = 0;
    aio_attr->frame_num    = 30; /* 30:frame num */
    if (g_payload_type == OT_PT_AAC) {
        aio_attr->point_num_per_frame = AACLC_SAMPLES_PER_FRAME;
    } else {
        aio_attr->point_num_per_frame = SAMPLE_AUDIO_POINT_NUM_PER_FRAME;
    }
    aio_attr->chn_cnt      = 2; /* 2:chn num */
#ifdef OT_ACODEC_TYPE_INNER
    *ai_dev = SAMPLE_AUDIO_INNER_AI_DEV;
    *ao_dev = SAMPLE_AUDIO_INNER_AO_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_INNERCODEC;
#else
    *ai_dev = SAMPLE_AUDIO_EXTERN_AI_DEV;
    *ao_dev = SAMPLE_AUDIO_EXTERN_AO_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_EXTERN;
#endif

    g_aio_resample = TD_FALSE;
    g_in_sample_rate  = OT_AUDIO_SAMPLE_RATE_BUTT;
    g_out_sample_rate = OT_AUDIO_SAMPLE_RATE_BUTT;
}

static td_s32 sample_audio_aenc_bind_ai(ot_audio_dev ai_dev, td_u32 aenc_chn_cnt)
{
    td_s32 ret;
    td_u32 i;
    td_u32 j;
    ot_ai_chn ai_chn;
    ot_aenc_chn ae_chn;

    for (i = 0; i < aenc_chn_cnt; i++) {
        ae_chn = i;
        ai_chn = i;

        if (g_user_get_mode == TD_TRUE) {
            ret = sample_comm_audio_creat_trd_ai_aenc(ai_dev, ai_chn, ae_chn);
        } else {
            ret = sample_comm_audio_aenc_bind_ai(ai_dev, ai_chn, ae_chn);
        }
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
            goto aenc_bind_err;
        }
        printf("ai(%d,%d) bind to aenc_chn:%d ok!\n", ai_dev, ai_chn, ae_chn);
    }
    return TD_SUCCESS;

aenc_bind_err:
    for (j = 0; j < i; j++) {
        ae_chn = j;
        ai_chn = j;

        if (g_user_get_mode == TD_TRUE) {
            sample_comm_audio_destory_trd_ai(ai_dev, ai_chn);
        } else {
            sample_comm_audio_aenc_unbind_ai(ai_dev, ai_chn, ae_chn);
        }
    }
    return TD_FAILURE;
}

static td_void sample_audio_aenc_unbind_ai(ot_audio_dev ai_dev, td_u32 aenc_chn_cnt)
{
    td_s32 ret;
    td_u32 i;
    ot_ai_chn ai_chn;
    ot_aenc_chn ae_chn;

    for (i = 0; i < aenc_chn_cnt; i++) {
        ae_chn = i;
        ai_chn = i;

        if (g_user_get_mode == TD_TRUE) {
            ret = sample_comm_audio_destory_trd_ai(ai_dev, ai_chn);
            if (ret != TD_SUCCESS) {
                sample_dbg(ret);
            }
        } else {
            ret = sample_comm_audio_aenc_unbind_ai(ai_dev, ai_chn, ae_chn);
            if (ret != TD_SUCCESS) {
                sample_dbg(ret);
            }
        }
    }
}

static td_s32 sample_audio_adec_bind_aenc(td_u32 adec_chn_cnt)
{
    td_s32 ret;
    td_u32 i;
    td_u32 j;
    ot_aenc_chn ae_chn;
    ot_adec_chn ad_chn;
    FILE *fd = NULL;

    for (i = 0; i < adec_chn_cnt; i++) {
        ae_chn = i;
        ad_chn = i;

        fd = sample_audio_open_aenc_file(ad_chn, g_payload_type);
        if (fd == TD_NULL) {
            sample_dbg(TD_FAILURE);
            for (j = 0; j < i; j++) {
                sample_comm_audio_destory_trd_aenc_adec(j);
            }
            return TD_FAILURE;
        }

        ret = sample_comm_audio_creat_trd_aenc_adec(ae_chn, ad_chn, fd);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
            fclose(fd);
            fd = TD_NULL;
            for (j = 0; j < i; j++) {
                sample_comm_audio_destory_trd_aenc_adec(j);
            }
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static td_void sample_audio_adec_unbind_aenc(td_u32 adec_chn_cnt)
{
    td_s32 ret;
    td_u32 i;
    ot_adec_chn ad_chn;

    for (i = 0; i < adec_chn_cnt; i++) {
        ad_chn = i;
        ret = sample_comm_audio_destory_trd_aenc_adec(ad_chn);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
        }
    }
}

static td_s32 sample_audio_ao_bind_adec(td_u32 adec_chn_cnt, ot_audio_dev ao_dev)
{
    td_s32 ret;
    td_u32 i;
    td_u32 j;
    ot_ao_chn ao_chn;
    ot_adec_chn ad_chn;

    for (i = 0; i < adec_chn_cnt; i++) {
        ao_chn = i;
        ad_chn = i;
        ret = sample_comm_audio_ao_bind_adec(ao_dev, ao_chn, ad_chn);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
            for (j = 0; j < i; j++) {
                sample_comm_audio_ao_unbind_adec(ao_dev, j, j);
            }
            return TD_FAILURE;
        }

        printf("bind adec:%d to ao(%d,%d) ok \n", ad_chn, ao_dev, ao_chn);
    }

    return TD_SUCCESS;
}

static td_void sample_audio_ao_unbind_adec(td_u32 adec_chn_cnt, ot_audio_dev ao_dev)
{
    td_s32 ret;
    td_u32 i;
    ot_ao_chn ao_chn;
    ot_adec_chn ad_chn;

    for (i = 0; i < adec_chn_cnt; i++) {
        ao_chn = i;
        ad_chn = i;
        ret = sample_comm_audio_ao_unbind_adec(ao_dev, ao_chn, ad_chn);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
        }
    }
}

static td_void sample_audio_ai_aenc_inner(td_bool send_adec, ot_aio_attr *aio_attr, ot_audio_dev ao_dev)
{
    td_s32 ret;
    td_u32 i;
    ot_adec_chn ad_chn;
    td_u32 ao_chn_cnt;
    td_u32 adec_chn_cnt;

    /* step 5: start adec & ao. ( if you want ) */
    if (send_adec == TD_TRUE) {
        adec_chn_cnt = aio_attr->chn_cnt >> ((td_u32)aio_attr->snd_mode);
        ret = sample_comm_audio_start_adec(adec_chn_cnt, g_payload_type);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
            return;
        }

        ao_chn_cnt = aio_attr->chn_cnt;
        ret = sample_comm_audio_start_ao(ao_dev, ao_chn_cnt, aio_attr, g_in_sample_rate, g_aio_resample);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
            goto ai_aenc_err2;
        }

        ret = sample_audio_adec_bind_aenc(adec_chn_cnt);
        if (ret != TD_SUCCESS) {
            goto ai_aenc_err1;
        }

        ret = sample_audio_ao_bind_adec(adec_chn_cnt, ao_dev);
        if (ret != TD_SUCCESS) {
            goto ai_aenc_err0;
        }
    }

    printf("\nplease press twice ENTER to exit this sample\n");
    smaple_audio_getchar();
    smaple_audio_getchar();

    /* step 6: exit the process */
    if (send_adec == TD_TRUE) {
        sample_audio_ao_unbind_adec(adec_chn_cnt, ao_dev);

ai_aenc_err0:
        sample_audio_adec_unbind_aenc(adec_chn_cnt);

ai_aenc_err1:
        ret = sample_comm_audio_stop_ao(ao_dev, ao_chn_cnt, g_aio_resample);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
        }

ai_aenc_err2:
        for (i = 0; i < adec_chn_cnt; i++) {
            ad_chn = i;
            ret = sample_comm_audio_stop_adec(ad_chn);
            if (ret != TD_SUCCESS) {
                sample_dbg(ret);
            }
        }
    }
}

/*
 * function : ai -> aenc -> file
 *                       -> adec -> ao
 */
td_s32 sample_audio_ai_aenc(td_void)
{
    td_s32 ret;
    ot_audio_dev ai_dev;
    ot_audio_dev ao_dev;
    td_u32 ai_chn_cnt;
    td_u32 aenc_chn_cnt;
    td_bool send_adec = TD_TRUE;
    ot_aio_attr aio_attr = {0};
    sample_comm_ai_vqe_param ai_vqe_param = {0};

    sample_audio_ai_aenc_init_param(&aio_attr, &ai_dev, &ao_dev);

    /* step 1: start ai */
    ai_chn_cnt = aio_attr.chn_cnt;
    sample_audio_set_ai_vqe_param(&ai_vqe_param, g_out_sample_rate, g_aio_resample, TD_NULL, 0);
    ret = sample_comm_audio_start_ai(ai_dev, ai_chn_cnt, &aio_attr, &ai_vqe_param, -1);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto ai_aenc_err6;
    }

    /* step 2: config audio codec */
    ret = sample_comm_audio_cfg_acodec(&aio_attr);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto ai_aenc_err5;
    }

    /* step 3: start aenc */
    aenc_chn_cnt = aio_attr.chn_cnt >> ((td_u32)aio_attr.snd_mode);
    ret = sample_comm_audio_start_aenc(aenc_chn_cnt, &aio_attr, g_payload_type);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto ai_aenc_err5;
    }

    /* step 4: aenc bind ai chn */
    ret = sample_audio_aenc_bind_ai(ai_dev, aenc_chn_cnt);
    if (ret != TD_SUCCESS) {
        goto ai_aenc_err4;
    }

    sample_audio_ai_aenc_inner(send_adec, &aio_attr, ao_dev);

    sample_audio_aenc_unbind_ai(ai_dev, aenc_chn_cnt);

ai_aenc_err4:
    ret = sample_comm_audio_stop_aenc(aenc_chn_cnt);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

ai_aenc_err5:
    ret = sample_comm_audio_stop_ai(ai_dev, ai_chn_cnt, g_aio_resample, TD_FALSE);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

ai_aenc_err6:
    return ret;
}

static td_void sample_audio_ai_to_ext_resample_init_param(ot_aio_attr *aio_attr, ot_audio_dev *ai_dev)
{
    aio_attr->sample_rate  = OT_AUDIO_SAMPLE_RATE_16000;
    aio_attr->bit_width    = OT_AUDIO_BIT_WIDTH_16;
    aio_attr->work_mode    = OT_AIO_MODE_I2S_MASTER;
    aio_attr->snd_mode     = OT_AUDIO_SOUND_MODE_MONO;
    aio_attr->expand_flag  = 0;
    aio_attr->frame_num    = 30; /* 30:frame num */
    aio_attr->point_num_per_frame = SAMPLE_AUDIO_POINT_NUM_PER_FRAME;
    aio_attr->chn_cnt      = 1;

#ifdef OT_ACODEC_TYPE_INNER
    *ai_dev = SAMPLE_AUDIO_INNER_AI_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_INNERCODEC;
#else
    *ai_dev = SAMPLE_AUDIO_EXTERN_AI_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_EXTERN;
#endif

    g_aio_resample = TD_FALSE;
    g_out_sample_rate = OT_AUDIO_SAMPLE_RATE_24000;
}

static td_s32 sample_audio_start_ai_resample(td_u32 ai_chn_cnt, ot_audio_dev ai_dev, ot_aio_attr *aio_attr)
{
    td_s32 ret;
    td_u32 i;
    td_u32 j;
    ot_ai_chn ai_chn;
    FILE *fd = TD_NULL;

    for (i = 0; i < ai_chn_cnt; i++) {
        ai_chn = i;

        fd = sample_audio_open_res_file(ai_chn);
        if (fd == TD_NULL) {
            sample_dbg(TD_FAILURE);
            for (j = 0; j < i; j++) {
                sample_comm_audio_destroy_trd_ai_ext_res(ai_dev, j);
            }
            return TD_FAILURE;
        }

        ret = sample_comm_audio_creat_trd_ai_ext_res(aio_attr, ai_dev, ai_chn, g_out_sample_rate, fd);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
            fclose(fd);
            fd = TD_NULL;
            for (j = 0; j < i; j++) {
                sample_comm_audio_destroy_trd_ai_ext_res(ai_dev, j);
            }
            return TD_FAILURE;
        }

        printf("ai(%d,%d) extern resample ok!\n", ai_dev, ai_chn);
    }

    return TD_SUCCESS;
}

static td_void sample_audio_stop_ai_resample(td_u32 ai_chn_cnt, ot_audio_dev ai_dev)
{
    td_s32 ret;
    td_u32 i;
    ot_ai_chn ai_chn;

    for (i = 0; i < ai_chn_cnt; i++) {
        ai_chn = i;
        ret = sample_comm_audio_destroy_trd_ai_ext_res(ai_dev, ai_chn);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
        }
    }
}

/* function : Ai -> ExtResample -> file */
td_s32 sample_audio_ai_to_ext_resample(td_void)
{
    td_s32 ret;
    ot_audio_dev ai_dev;
    td_u32 ai_chn_cnt;
    ot_aio_attr aio_attr = {0};
    sample_comm_ai_vqe_param ai_vqe_param = {0};

    sample_audio_ai_to_ext_resample_init_param(&aio_attr, &ai_dev);

    /* step 1: start Ai, disable inner resample. */
    ai_chn_cnt = aio_attr.chn_cnt;
    sample_audio_set_ai_vqe_param(&ai_vqe_param, g_out_sample_rate, g_aio_resample, TD_NULL, 0);
    ret = sample_comm_audio_start_ai(ai_dev, ai_chn_cnt, &aio_attr, &ai_vqe_param, -1);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto ai_res_err3;
    }

    /* step 2: config audio codec */
    ret = sample_comm_audio_cfg_acodec(&aio_attr);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto ai_res_err2;
    }

    /* step 3: enable extern resample. */
    ret = sample_audio_init_ext_res_fun();
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto ai_res_err2;
    }

    /* step 4: Create thread to resample. */
    ret = sample_audio_start_ai_resample(ai_chn_cnt, ai_dev, &aio_attr);
    if (ret != TD_SUCCESS) {
        goto ai_res_err1;
    }

    printf("\nplease press twice ENTER to exit this sample\n");
    smaple_audio_getchar();
    smaple_audio_getchar();

    /* step 5: exit the process */
    sample_audio_stop_ai_resample(ai_chn_cnt, ai_dev);

ai_res_err1:
    sample_audio_deinit_ext_res_fun();

ai_res_err2:
    ret = sample_comm_audio_stop_ai(ai_dev, ai_chn_cnt, g_aio_resample, TD_FALSE);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

ai_res_err3:
    return ret;
}

static td_void sample_audio_ai_ao_init_param(ot_aio_attr *aio_attr, ot_audio_dev *ai_dev, ot_audio_dev *ao_dev)
{
    aio_attr->sample_rate   = OT_AUDIO_SAMPLE_RATE_48000;
    aio_attr->bit_width     = OT_AUDIO_BIT_WIDTH_16;
    aio_attr->work_mode     = OT_AIO_MODE_I2S_MASTER;
    aio_attr->snd_mode      = OT_AUDIO_SOUND_MODE_STEREO;
    aio_attr->expand_flag   = 0;
    aio_attr->frame_num     = 30; /* 30:frame num */
    aio_attr->point_num_per_frame = AACLC_SAMPLES_PER_FRAME;
    aio_attr->chn_cnt       = 2; /* 2:chn num */
#ifdef OT_ACODEC_TYPE_INNER
    *ai_dev = SAMPLE_AUDIO_INNER_AI_DEV;
    *ao_dev = SAMPLE_AUDIO_INNER_AO_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_INNERCODEC;
#else
    *ai_dev = SAMPLE_AUDIO_EXTERN_AI_DEV;
    *ao_dev = SAMPLE_AUDIO_EXTERN_AO_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_EXTERN;
#endif

    g_aio_resample = TD_FALSE;
    /* config ao resample attr if needed */
    if (g_aio_resample == TD_TRUE) {
        /* ai 48k -> 32k */
        g_out_sample_rate = OT_AUDIO_SAMPLE_RATE_32000;

        /* ao 32k -> 48k */
        g_in_sample_rate  = OT_AUDIO_SAMPLE_RATE_32000;
    } else {
        g_in_sample_rate  = OT_AUDIO_SAMPLE_RATE_BUTT;
        g_out_sample_rate = OT_AUDIO_SAMPLE_RATE_BUTT;
    }

    /* resample and anr should be user get mode */
    g_user_get_mode = (g_aio_resample == TD_TRUE) ? TD_TRUE : TD_FALSE;
}

static td_s32 sample_audio_ao_bind_ai(ot_audio_dev ai_dev, ot_ai_chn ai_chn,
    ot_audio_dev ao_dev, ot_ao_chn ao_chn)
{
    td_s32 ret;
    if (g_user_get_mode == TD_TRUE) {
        ret = sample_comm_audio_creat_trd_ai_ao(ai_dev, ai_chn, ao_dev, ao_chn);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
            return TD_FAILURE;
        }
    } else {
        ret = sample_comm_audio_ao_bind_ai(ai_dev, ai_chn, ao_dev, ao_chn);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
            return TD_FAILURE;
        }
    }

    printf("ai(%d,%d) bind to ao(%d,%d) ok\n", ai_dev, ai_chn, ao_dev, ao_chn);
    return TD_SUCCESS;
}

static td_void sample_audio_ao_unbind_ai(ot_audio_dev ai_dev, ot_ai_chn ai_chn,
    ot_audio_dev ao_dev, ot_ao_chn ao_chn)
{
    td_s32 ret;
    if (g_user_get_mode == TD_TRUE) {
        ret = sample_comm_audio_destory_trd_ai(ai_dev, ai_chn);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
        }
    } else {
        ret = sample_comm_audio_ao_unbind_ai(ai_dev, ai_chn, ao_dev, ao_chn);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
        }
    }
}

static td_void sample_audio_ai_ao_inner(ot_audio_dev ai_dev, ot_ai_chn ai_chn,
    ot_audio_dev ao_dev, ot_ao_chn ao_chn)
{
    td_s32 ret;

    /* bind AI to AO channel */
    ret = sample_audio_ao_bind_ai(ai_dev, ai_chn, ao_dev, ao_chn);
    if (ret != TD_SUCCESS) {
        return;
    }

    if (g_ao_volume_ctrl == TD_TRUE) {
        ret = sample_comm_audio_creat_trd_ao_vol_ctrl(ao_dev);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
            goto ai_ao_err0;
        }
    }

    printf("\nplease press twice ENTER to exit this sample\n");
    smaple_audio_getchar();
    smaple_audio_getchar();

    if (g_ao_volume_ctrl == TD_TRUE) {
        ret = sample_comm_audio_destory_trd_ao_vol_ctrl(ao_dev);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
        }
    }

ai_ao_err0:
    sample_audio_ao_unbind_ai(ai_dev, ai_chn, ao_dev, ao_chn);
}

/* function : ai -> ao(with fade in/out and volume adjust) */
td_s32 sample_audio_ai_ao(td_void)
{
    td_s32 ret;
    td_u32 ai_chn_cnt;
    td_u32 ao_chn_cnt;
    ot_audio_dev ai_dev;
    ot_audio_dev ao_dev;
    const ot_ai_chn ai_chn = 0;
    const ot_ao_chn ao_chn = 0;
    ot_aio_attr aio_attr = {0};
    sample_comm_ai_vqe_param ai_vqe_param = {0};

    sample_audio_ai_ao_init_param(&aio_attr, &ai_dev, &ao_dev);

    /* enable AI channel */
    ai_chn_cnt = aio_attr.chn_cnt;
    sample_audio_set_ai_vqe_param(&ai_vqe_param, g_out_sample_rate, g_aio_resample, TD_NULL, 0);
    ret = sample_comm_audio_start_ai(ai_dev, ai_chn_cnt, &aio_attr, &ai_vqe_param, -1);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto ai_ao_err3;
    }

    /* enable AO channel */
    ao_chn_cnt = aio_attr.chn_cnt;
    ret = sample_comm_audio_start_ao(ao_dev, ao_chn_cnt, &aio_attr, g_in_sample_rate, g_aio_resample);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto ai_ao_err2;
    }

    /* config internal audio codec */
    ret = sample_comm_audio_cfg_acodec(&aio_attr);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto ai_ao_err1;
    }

    sample_audio_ai_ao_inner(ai_dev, ai_chn, ao_dev, ao_chn);

ai_ao_err1:
    ret = sample_comm_audio_stop_ao(ao_dev, ao_chn_cnt, g_aio_resample);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

ai_ao_err2:
    ret = sample_comm_audio_stop_ai(ai_dev, ai_chn_cnt, g_aio_resample, TD_FALSE);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

ai_ao_err3:
    return ret;
}

static td_void sample_audio_ai_to_ao_sys_chn_init_param(ot_aio_attr *aio_attr, ot_audio_dev *ai_dev,
    ot_audio_dev *ao_dev)
{
    aio_attr->sample_rate  = OT_AUDIO_SAMPLE_RATE_48000;
    aio_attr->bit_width    = OT_AUDIO_BIT_WIDTH_16;
    aio_attr->work_mode    = OT_AIO_MODE_I2S_MASTER;
    aio_attr->snd_mode     = OT_AUDIO_SOUND_MODE_MONO;
    aio_attr->expand_flag  = 0;
    aio_attr->frame_num    = 30; /* 30:frame num */
    aio_attr->point_num_per_frame = AACLC_SAMPLES_PER_FRAME;
    aio_attr->chn_cnt      = 1; /* 1:chn num */
#ifdef OT_ACODEC_TYPE_INNER
    *ai_dev = SAMPLE_AUDIO_INNER_AI_DEV;
    *ao_dev = SAMPLE_AUDIO_INNER_AO_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_INNERCODEC;
#else
    *ai_dev = SAMPLE_AUDIO_EXTERN_AI_DEV;
    *ao_dev = SAMPLE_AUDIO_EXTERN_AO_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_EXTERN;
#endif

    g_aio_resample = TD_FALSE;
    /* config ao resample attr if needed */
    if (g_aio_resample == TD_TRUE) {
        /* ai 48k -> 32k */
        g_out_sample_rate = OT_AUDIO_SAMPLE_RATE_32000;

        /* ao 32k -> 48k */
        g_in_sample_rate  = OT_AUDIO_SAMPLE_RATE_32000;
    } else {
        g_in_sample_rate  = OT_AUDIO_SAMPLE_RATE_BUTT;
        g_out_sample_rate = OT_AUDIO_SAMPLE_RATE_BUTT;
    }

    /* resample and anr should be user get mode */
    g_user_get_mode = (g_aio_resample == TD_TRUE) ? TD_TRUE : TD_FALSE;
}

static td_void sample_audio_ai_to_ao_sys_chn_inner(ot_audio_dev ai_dev, ot_ai_chn ai_chn,
    ot_audio_dev ao_dev)
{
    td_s32 ret;

    /* bind AI to AO channel */
    ret = sample_comm_audio_creat_trd_ai_ao(ai_dev, ai_chn, ao_dev, OT_AO_SYS_CHN_ID);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        return;
    }
    printf("ai(%d,%d) bind to ao(%d,%d) ok\n", ai_dev, ai_chn, ao_dev, OT_AO_SYS_CHN_ID);

    printf("\nplease press twice ENTER to exit this sample\n");
    smaple_audio_getchar();
    smaple_audio_getchar();

    ret = sample_comm_audio_destory_trd_ai(ai_dev, ai_chn);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }
}

/* function : ai -> ao (sys chn) */
td_s32 sample_audio_ai_to_ao_sys_chn(td_void)
{
    td_s32 ret;
    td_u32 ai_chn_cnt;
    td_u32 ao_chn_cnt;
    ot_audio_dev ai_dev;
    ot_audio_dev ao_dev;
    const ot_ai_chn ai_chn = 0;
    ot_aio_attr aio_attr = {0};
    sample_comm_ai_vqe_param ai_vqe_param = {0};

    sample_audio_ai_to_ao_sys_chn_init_param(&aio_attr, &ai_dev, &ao_dev);

    /* enable AI channel */
    ai_chn_cnt = aio_attr.chn_cnt;
    sample_audio_set_ai_vqe_param(&ai_vqe_param, g_out_sample_rate, g_aio_resample, TD_NULL, 0);
    ret = sample_comm_audio_start_ai(ai_dev, ai_chn_cnt, &aio_attr, &ai_vqe_param, -1);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto ai_ao_sys_chn_err3;
    }

    /* enable AO channel */
    ao_chn_cnt = aio_attr.chn_cnt;
    ret = sample_comm_audio_start_ao(ao_dev, ao_chn_cnt, &aio_attr, g_in_sample_rate, g_aio_resample);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto ai_ao_sys_chn_err2;
    }

    /* config internal audio codec */
    ret = sample_comm_audio_cfg_acodec(&aio_attr);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto ai_ao_sys_chn_err1;
    }

    sample_audio_ai_to_ao_sys_chn_inner(ai_dev, ai_chn, ao_dev);

ai_ao_sys_chn_err1:
    ret = sample_comm_audio_stop_ao(ao_dev, ao_chn_cnt, g_aio_resample);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

ai_ao_sys_chn_err2:
    ret = sample_comm_audio_stop_ai(ai_dev, ai_chn_cnt, g_aio_resample, TD_FALSE);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

ai_ao_sys_chn_err3:
    return ret;
}

static td_void ai_init_record_vqe_param(ot_ai_record_vqe_cfg *ai_vqe_record_attr)
{
    ai_vqe_record_attr->work_sample_rate = OT_AUDIO_SAMPLE_RATE_48000;
    ai_vqe_record_attr->frame_sample = AACLC_SAMPLES_PER_FRAME;
    ai_vqe_record_attr->work_state = OT_VQE_WORK_STATE_COMMON;
    ai_vqe_record_attr->in_chn_num = 2;  /* 2: chn num */
    ai_vqe_record_attr->out_chn_num = 2; /* 2: chn num */
    ai_vqe_record_attr->record_type = OT_VQE_RECORD_NORMAL;
    ai_vqe_record_attr->drc_cfg.usr_mode = TD_FALSE;
    ai_vqe_record_attr->rnr_cfg.usr_mode = TD_FALSE;
    ai_vqe_record_attr->hdr_cfg.usr_mode = TD_FALSE;
    ai_vqe_record_attr->hpf_cfg.usr_mode = TD_TRUE;
    ai_vqe_record_attr->hpf_cfg.hpf_freq = OT_AUDIO_HPF_FREQ_80;

    ai_vqe_record_attr->open_mask =
        OT_AI_RECORDVQE_MASK_DRC | OT_AI_RECORDVQE_MASK_HDR | OT_AI_RECORDVQE_MASK_HPF | OT_AI_RECORDVQE_MASK_RNR;
}

static td_void ai_init_talk_vqe_param(ot_ai_talk_vqe_cfg *ai_vqe_talk_attr)
{
    ai_vqe_talk_attr->work_sample_rate = OT_AUDIO_SAMPLE_RATE_16000;
    ai_vqe_talk_attr->frame_sample = AACLC_SAMPLES_PER_FRAME;
    ai_vqe_talk_attr->work_state = OT_VQE_WORK_STATE_COMMON;
    ai_vqe_talk_attr->agc_cfg.usr_mode = TD_FALSE;
    ai_vqe_talk_attr->aec_cfg.usr_mode = TD_FALSE;
    ai_vqe_talk_attr->anr_cfg.usr_mode = TD_FALSE;
    ai_vqe_talk_attr->hpf_cfg.usr_mode = TD_TRUE;
    ai_vqe_talk_attr->hpf_cfg.hpf_freq = OT_AUDIO_HPF_FREQ_150;

    ai_vqe_talk_attr->open_mask = OT_AI_TALKVQE_MASK_AGC | OT_AI_TALKVQE_MASK_ANR | OT_AI_TALKVQE_MASK_HPF;
}

static td_void ai_init_talk_vqe_v2_param(ot_ai_talk_vqe_v2_cfg *ai_vqe_talk_v2_attr)
{
    ai_vqe_talk_v2_attr->work_sample_rate = OT_AUDIO_SAMPLE_RATE_16000;
    ai_vqe_talk_v2_attr->frame_sample = AACLC_SAMPLES_PER_FRAME;
    ai_vqe_talk_v2_attr->work_state = OT_VQE_WORK_STATE_COMMON;
    ai_vqe_talk_v2_attr->in_chn_num = 2;  /* 2: chn num */
    ai_vqe_talk_v2_attr->out_chn_num = 2; /* 2: chn num */
    ai_vqe_talk_v2_attr->agc_cfg.usr_mode = TD_FALSE;
    ai_vqe_talk_v2_attr->pnr_cfg.usr_mode = TD_FALSE;

    ai_vqe_talk_v2_attr->open_mask = OT_AI_TALKVQEV2_MASK_AGC | OT_AI_TALKVQEV2_MASK_PNR | OT_AI_TALKVQEV2_MASK_WNR;
}

static td_void sample_audio_ai_vqe_process_ao_init_param(ot_aio_attr *aio_attr, ot_audio_dev *ai_dev,
    ot_audio_dev *ao_dev, td_void **ai_vqe_attr)
{
    aio_attr->sample_rate  = OT_AUDIO_SAMPLE_RATE_48000;
    aio_attr->bit_width    = OT_AUDIO_BIT_WIDTH_16;
    aio_attr->work_mode    = OT_AIO_MODE_I2S_MASTER;
    aio_attr->snd_mode     = OT_AUDIO_SOUND_MODE_STEREO;
    aio_attr->expand_flag  = 0;
    aio_attr->frame_num    = 30; /* 30:frame num */
    aio_attr->point_num_per_frame = AACLC_SAMPLES_PER_FRAME;
    aio_attr->chn_cnt      = 2; /* 2:chn num */

#ifdef OT_ACODEC_TYPE_INNER
    *ai_dev = SAMPLE_AUDIO_INNER_AI_DEV;
    *ao_dev = SAMPLE_AUDIO_INNER_AO_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_INNERCODEC;
#else
    *ai_dev = SAMPLE_AUDIO_EXTERN_AI_DEV;
    *ao_dev = SAMPLE_AUDIO_EXTERN_AO_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_EXTERN;
#endif

    g_aio_resample = TD_FALSE;
    g_in_sample_rate  = OT_AUDIO_SAMPLE_RATE_BUTT;
    g_out_sample_rate = OT_AUDIO_SAMPLE_RATE_BUTT;

    if (g_ai_vqe_type == 1) { /* 1:record vqe */
        ai_init_record_vqe_param(&g_ai_vqe_record_attr);
        *ai_vqe_attr = (td_void *)&g_ai_vqe_record_attr;
    } else if (g_ai_vqe_type == 2) { /* 2:talk vqe */
        aio_attr->sample_rate = OT_AUDIO_SAMPLE_RATE_16000;
        aio_attr->snd_mode = OT_AUDIO_SOUND_MODE_MONO;
        aio_attr->chn_cnt = 1;
        ai_init_talk_vqe_param(&g_ai_vqe_talk_attr);
        *ai_vqe_attr = (td_void *)&g_ai_vqe_talk_attr;
    } else if (g_ai_vqe_type == 3) { /* 3:talkv2 vqe */
        aio_attr->sample_rate = OT_AUDIO_SAMPLE_RATE_16000;
        ai_init_talk_vqe_v2_param(&g_ai_vqe_talk_v2_attr);
        *ai_vqe_attr = (td_void *)&g_ai_vqe_talk_v2_attr;
    } else {
        *ai_vqe_attr = TD_NULL;
    }
}

static td_s32 sample_audio_ao_bind_ai_multi_chn(ot_audio_dev ai_dev, const ot_aio_attr *ai_attr,
    ot_audio_dev ao_dev)
{
    td_s32 ret;
    td_u32 i;
    td_u32 j;
    td_u32 ai_chn_cnt = ai_attr->chn_cnt;

    for (i = 0; i < (ai_chn_cnt >> ((td_u32)ai_attr->snd_mode)); i++) {
        ret = sample_comm_audio_creat_trd_ai_ao(ai_dev, i, ao_dev, i);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
            for (j = 0; j < i; j++) {
                sample_comm_audio_destory_trd_ai(ai_dev, j);
            }
            return TD_FAILURE;
        }

        printf("bind ai(%d,%d) to ao(%d,%d) ok \n", ai_dev, i, ao_dev, i);
    }

    return TD_SUCCESS;
}

static td_void sample_audio_ao_unbind_ai_multi_chn(ot_audio_dev ai_dev, const ot_aio_attr *ai_attr)
{
    td_s32 ret;
    td_u32 i;
    td_u32 ai_chn_cnt = ai_attr->chn_cnt;

    for (i = 0; i < (ai_chn_cnt >> ((td_u32)ai_attr->snd_mode)); i++) {
        ret = sample_comm_audio_destory_trd_ai(ai_dev, i);
        if (ret != TD_SUCCESS) {
            sample_dbg(ret);
        }
    }
}

/* function : ai -> ao (with VQE) */
td_s32 sample_audio_ai_vqe_process_ao(td_void)
{
    td_s32 ret;
    td_u32 ai_chn_cnt;
    td_u32 ao_chn_cnt;
    ot_audio_dev ai_dev;
    ot_audio_dev ao_dev;
    ot_aio_attr aio_attr = {0};
    td_void *ai_vqe_attr = NULL;
    sample_comm_ai_vqe_param ai_vqe_param = {0};

    sample_audio_ai_vqe_process_ao_init_param(&aio_attr, &ai_dev, &ao_dev, &ai_vqe_attr);

    /* step 1: start ai */
    ai_chn_cnt = aio_attr.chn_cnt;
    sample_audio_set_ai_vqe_param(&ai_vqe_param, g_out_sample_rate, g_aio_resample, ai_vqe_attr, g_ai_vqe_type);
    ret = sample_comm_audio_start_ai(ai_dev, ai_chn_cnt, &aio_attr, &ai_vqe_param, ao_dev);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto vqe_err2;
    }

    /* step 2: start ao */
    ao_chn_cnt = aio_attr.chn_cnt;
    ret = sample_comm_audio_start_ao(ao_dev, ao_chn_cnt, &aio_attr, g_in_sample_rate, g_aio_resample);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto vqe_err1;
    }

    /* step 3: config audio codec */
    ret = sample_comm_audio_cfg_acodec(&aio_attr);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto vqe_err0;
    }

    /* step 4: ao bind ai chn */
    ret = sample_audio_ao_bind_ai_multi_chn(ai_dev, &aio_attr, ao_dev);
    if (ret != TD_SUCCESS) {
        goto vqe_err0;
    }

    printf("\nplease press twice ENTER to exit this sample\n");
    smaple_audio_getchar();
    smaple_audio_getchar();

    /* step 5: exit the process */
    sample_audio_ao_unbind_ai_multi_chn(ai_dev, &aio_attr);

vqe_err0:
    ret = sample_comm_audio_stop_ao(ao_dev, ao_chn_cnt, g_aio_resample);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

vqe_err1:
    ret = sample_comm_audio_stop_ai(ai_dev, ai_chn_cnt, g_aio_resample, TD_TRUE);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

vqe_err2:
    return ret;
}

static td_void sample_audio_ai_hdmi_ao_init_param(ot_aio_attr *aio_attr, ot_audio_dev *ai_dev,
    ot_aio_attr *hdmi_ao_attr)
{
    aio_attr->sample_rate   = OT_AUDIO_SAMPLE_RATE_48000;
    aio_attr->bit_width     = OT_AUDIO_BIT_WIDTH_16;
    aio_attr->work_mode     = OT_AIO_MODE_I2S_MASTER;
    aio_attr->snd_mode      = OT_AUDIO_SOUND_MODE_STEREO;
    aio_attr->expand_flag   = 0;
    aio_attr->frame_num     = 30; /* 30:frame num */
    aio_attr->point_num_per_frame = AACLC_SAMPLES_PER_FRAME;
    aio_attr->chn_cnt       = 2; /* 2:chn num */
#ifdef OT_ACODEC_TYPE_INNER
    *ai_dev = SAMPLE_AUDIO_INNER_AI_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_INNERCODEC;
#else
    *ai_dev = SAMPLE_AUDIO_EXTERN_AI_DEV;
    aio_attr->clk_share  = 1;
    aio_attr->i2s_type   = OT_AIO_I2STYPE_EXTERN;
#endif

    hdmi_ao_attr->sample_rate   = OT_AUDIO_SAMPLE_RATE_48000;
    hdmi_ao_attr->bit_width     = OT_AUDIO_BIT_WIDTH_16;
    hdmi_ao_attr->work_mode     = OT_AIO_MODE_I2S_MASTER;
    hdmi_ao_attr->snd_mode      = OT_AUDIO_SOUND_MODE_STEREO;
    hdmi_ao_attr->expand_flag   = 0;
    hdmi_ao_attr->frame_num     = 30; /* 30:frame num */
    hdmi_ao_attr->point_num_per_frame = AACLC_SAMPLES_PER_FRAME;
    hdmi_ao_attr->chn_cnt       = 2; /* 2:chn num */
    hdmi_ao_attr->clk_share     = 1;
    hdmi_ao_attr->i2s_type      = OT_AIO_I2STYPE_INNERHDMI;

    g_in_sample_rate = aio_attr->sample_rate;
    g_aio_resample = TD_FALSE;
    /* resample should be user get mode */
    g_user_get_mode = (g_aio_resample == TD_TRUE) ? TD_TRUE : TD_FALSE;
}

/* function : ai -> ao(hdmi) */
td_s32 sample_audio_ai_hdmi_ao(td_void)
{
    td_s32 ret;
    td_u32 ai_chn_cnt;
    td_u32 ao_chn_cnt;
    ot_audio_dev ai_dev;
    ot_audio_dev ao_dev = SAMPLE_AUDIO_INNER_HDMI_AO_DEV;
    ot_aio_attr aio_attr = {0};
    ot_aio_attr hdmi_ao_attr = {0};
    sample_comm_ai_vqe_param ai_vqe_param = {0};

    sample_audio_ai_hdmi_ao_init_param(&aio_attr, &ai_dev, &hdmi_ao_attr);

    /* enable AI channel */
    ai_chn_cnt = aio_attr.chn_cnt;
    sample_audio_set_ai_vqe_param(&ai_vqe_param, OT_AUDIO_SAMPLE_RATE_BUTT, TD_FALSE, TD_NULL, 0);
    ret = sample_comm_audio_start_ai(ai_dev, ai_chn_cnt, &aio_attr, &ai_vqe_param, -1);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto hdmi_err2;
    }

    /* enable AO channel */
    ao_chn_cnt = hdmi_ao_attr.chn_cnt;
    ret = sample_comm_audio_start_ao(ao_dev, ao_chn_cnt, &hdmi_ao_attr, g_in_sample_rate, g_aio_resample);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto hdmi_err1;
    }

    /* config audio codec */
    ret = sample_comm_audio_cfg_acodec(&aio_attr);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
        goto hdmi_err0;
    }

    /* AI to AO channel */
    ret = sample_audio_ao_bind_ai_multi_chn(ai_dev, &aio_attr, ao_dev);
    if (ret != TD_SUCCESS) {
        goto hdmi_err0;
    }

    printf("\nplease press twice ENTER to exit this sample\n");
    smaple_audio_getchar();
    smaple_audio_getchar();

    sample_audio_ao_unbind_ai_multi_chn(ai_dev, &aio_attr);

hdmi_err0:
    ret = sample_comm_audio_stop_ao(ao_dev, hdmi_ao_attr.chn_cnt, TD_FALSE);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

hdmi_err1:
    ret = sample_comm_audio_stop_ai(ai_dev, ai_chn_cnt, TD_FALSE, TD_FALSE);
    if (ret != TD_SUCCESS) {
        sample_dbg(ret);
    }

hdmi_err2:
    return ret;
}

#if defined(OT_VQE_USE_STATIC_MODULE_REGISTER)
/* function : to register vqe module */
td_s32 sample_audio_register_vqe_module(td_void)
{
    td_s32 ret;
    ot_audio_vqe_register vqe_reg_cfg = {0};

    /* RecordVQE */
    vqe_reg_cfg.record_mod_cfg.handle = ot_vqe_record_get_handle();

    /* Resample */
    vqe_reg_cfg.resample_mod_cfg.handle = ot_vqe_resample_get_handle();

    /* TalkVQE */
    vqe_reg_cfg.hpf_mod_cfg.handle = ot_vqe_hpf_get_handle();
    vqe_reg_cfg.aec_mod_cfg.handle = ot_vqe_aec_get_handle();
    vqe_reg_cfg.agc_mod_cfg.handle = ot_vqe_agc_get_handle();
    vqe_reg_cfg.anr_mod_cfg.handle = ot_vqe_anr_get_handle();
    vqe_reg_cfg.eq_mod_cfg.handle = ot_vqe_eq_get_handle();

    /* TalkV2VQE */
    vqe_reg_cfg.talkv2_mod_cfg.handle = ot_vqe_talkv2_get_handle();

    ret = ss_mpi_audio_register_vqe_mod(&vqe_reg_cfg);
    if (ret != TD_SUCCESS) {
        printf("%s: register vqe module fail with ret = %d!\n", __FUNCTION__, ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}
#endif

td_void sample_audio_usage(td_void)
{
    printf("\n\n/usage:./sample_audio <index>/\n");
    printf("\tindex and its function list below\n");
    printf("\t0:  start AI to AO loop\n");
    printf("\t1:  send audio frame to AENC channel from AI, save them\n");
    printf("\t2:  read audio stream from file, decode and send AO\n");
    printf("\t3:  start AI(VQE process), then send to AO\n");
    printf("\t4:  start AI to AO(HDMI) loop\n");
    printf("\t5:  start AI to AO(sys_chn) loop\n");
    printf("\t6:  start AI, then send to resample, save it\n");
}

/* function : to process abnormal case */
static td_void sample_audio_handle_sig(td_s32 signo)
{
    if (g_sample_audio_exit == TD_TRUE) {
        return;
    }

    if ((signo == SIGINT) || (signo == SIGTERM)) {
        g_sample_audio_exit = TD_TRUE;
    }
}

static td_void main_inner(td_u32 index)
{
    switch (index) {
        case 0: { /* 0:ai->ao */
            sample_audio_ai_ao();
            break;
        }
        case 1: { /* 1:ai->aenc->adec->ao */
            sample_audio_ai_aenc();
            break;
        }
        case 2: { /* 2:file->adec->ao */
            sample_audio_adec_ao();
            break;
        }
        case 3: { /* 3:ai->ao vqe */
            sample_audio_ai_vqe_process_ao();
            break;
        }
        case 4: { /* 4:ai->ao hdmi */
            sample_audio_ai_hdmi_ao();
            break;
        }
        case 5: { /* 5:ai->ao synchn */
            sample_audio_ai_to_ao_sys_chn();
            break;
        }
        case 6: { /* 6:resample test */
            sample_audio_ai_to_ext_resample();
            break;
        }
        default: {
            break;
        }
    }
}

/* function : main */
#ifdef __LITEOS__
td_s32 app_main(int argc, char *argv[])
#else
td_s32 main(int argc, char *argv[])
#endif
{
    td_s32 ret;
    ot_vb_cfg vb_conf;
    td_u32 index;

    if (argc != 2) {  /* 2:argv num */
        sample_audio_usage();
        return TD_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        sample_audio_usage();
        return TD_FAILURE;
    }

    if ((strlen(argv[1]) != 1) ||
        (argv[1][0] < '0' || argv[1][0] > '6')) { /* 6:arg num */
        sample_audio_usage();
        return TD_FAILURE;
    }

    index = atoi(argv[1]);

    sample_sys_signal(&sample_audio_handle_sig);

#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif

#if defined(OT_VQE_USE_STATIC_MODULE_REGISTER)
    ret = sample_audio_register_vqe_module();
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
#endif
    ret = memset_s(&vb_conf, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg));
    if (ret != EOK) {
        printf("%s: vb_config init failed with %d!\n", __FUNCTION__, ret);
        return TD_FAILURE;
    }

    ret = sample_comm_sys_init(&vb_conf);
    if (ret != TD_SUCCESS) {
        printf("%s: system init failed with %d!\n", __FUNCTION__, ret);
        goto EXIT;
    }

    ss_mpi_aenc_aac_init();
    ss_mpi_adec_aac_init();

    main_inner(index);

    ss_mpi_aenc_aac_deinit();
    ss_mpi_adec_aac_deinit();

    sample_comm_sys_exit();

EXIT:
#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif
    return ret;
}
