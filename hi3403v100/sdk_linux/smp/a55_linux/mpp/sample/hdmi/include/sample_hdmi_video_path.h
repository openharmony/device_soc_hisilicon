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

#ifndef SAMPLE_HDMI_VIDEO_PATH_H
#define SAMPLE_HDMI_VIDEO_PATH_H

#include <stdio.h>
#include <signal.h>
#include "ss_mpi_vo.h"
#include "ot_common_vo.h"
#include "sample_comm.h"
#include "sample_hdmi.h"

typedef enum {
    SAMPLE_MEDIA_STOP,
    SAMPLE_MEDIA_RUN
} sample_media_status;

typedef enum {
    AUDIO_CODEC_INNER,
    AUDIO_CODEC_TLV320,
    AUDIO_CODEC_HDMI,
    AUDIO_CODEC_TW2865,
    AUDIO_CODEC_BUTT
} audio_codec_type;

typedef struct {
    td_bool start;
    pthread_t aenc_pid;
    td_s32 ae_chn;
    td_s32 ad_chn;
    FILE *pfd;
    td_bool send_ad_chn;
} sample_aenc;

typedef struct {
    td_bool start;
    td_s32 ai_dev;
    td_s32 ai_chn;
    td_s32 aenc_chn;
    td_s32 ao_dev;
    td_s32 ao_chn;
    td_bool send_aenc;
    td_bool send_ao;
    pthread_t ai_pid;
} sample_ai;

typedef struct {
    td_bool start;
    td_s32 ad_chn;
    FILE *pfd;
    pthread_t ad_pid;
} sample_adec;

typedef struct {
    ot_audio_dev ao_dev;
    td_bool start;
    pthread_t ao_pid;
} sample_ao;

typedef struct {
    ot_hdmi_id hdmi;
} hdmi_args;

typedef struct {
    td_char *src_path;
    td_u32 sample_rate;
    td_u32 chnl_num;
    td_char *src_path_new;
    td_u32 sample_rate_new;
    td_u32 chnl_num_new;
    ot_audio_dev ao_dev;
} aduio_input;

typedef struct {
    ot_vo_intf_sync vo_sync;
    ot_pic_size pic_size;
} pic_size_param;

typedef struct {
    ot_vo_intf_sync intf_sync;
    ot_hdmi_video_format hdmi_fmt;
} vo_hdmi_fmt;

td_void sample_hdmi_start(ot_vo_intf_sync intf_sync);
td_void sample_hdmi_media_set(td_bool run);
td_s32 sample_hdmi_vdec_vpss_vo_start(ot_vo_intf_sync intf_sync);
td_s32 sample_hdmi_vdec_vpss_vo_stop();
td_s32 sample_hdmi_fmt_change(ot_vo_intf_sync intf_sync, td_bool rgb_enable);
td_s32 sample_hdmi_start_audio(td_void);
td_s32 sample_hdmi_stop_audio();
td_s32 sample_hdmi_set_audio_sample_rate(td_u32 sample_rate);
td_s32 sample_hdmi_set_audio_chnl_num(td_u32 chnl_num);
td_s32 sample_hdmi_audio_reset();

#endif

