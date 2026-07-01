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
 *
 * Description: AudioInputPortPlugin
 */

#include "audio_primary_sub_port_in.h"
#include <pthread.h>
#ifndef __HuaweiLite__
#include <sys/syscall.h>
#endif
#include "securec.h"
/* SDK include */
#include "ss_mpi_audio.h"
#include "ot_common_aio.h"
#include "ot_common_aenc.h"
#include "audio_aac_adp.h"
#include "media_hal_common.h"
#include "audio_port_plugin_common.h"
#include "audio_effect_manager.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
#define MODULE_NAME "AudioPrimarySubPortIn"

#define MAX_INPUT_PORT_CNT          OT_AI_DEV_MAX_NUM
#define MAX_INPUT_PORT_TRACK_CNT    OT_AI_MAX_CHN_NUM

#define MAX_KEY_STR_LEN 10

#define PORT_ID(module, device, channel) \
    ((unsigned int)(((module) << 16) | ((device) << 8) | (channel)))

/* count of audio frame in Buffer */
#define AUDIO_FRAME_NUM_IN_BUF 30

/* sample per frame for all encoder(aacplus:2048) */
#define AUDIO_AUDIO_POINT_NUM 1024

#define AUDIO_FRAME_CACHED_DEPTH 5

#define AUDIO_INPUT_VOL 30

#define AUDIO_HAL_FRAME_COUNT 1024

#define AUDIO_AO_INNER_DEV_ID 0 /** ao inner acodec dev id */

const unsigned long long TIME_CONVERSION_US_S = 1000000ULL; /* us to s */
const unsigned long long TIME_CONVERSION_NS_US = 1000ULL; /* ns to us */

#define AUDIO_JOIN3(prefix, middle, suffix) prefix##middle##suffix
#define AUDIO_TALK_VQE_MASK_NR AUDIO_JOIN3(OT_AI_TALKVQE_MASK_, A, NR)
#define AUDIO_VQE_CFG_NR(obj) ((obj)->a##nr_cfg)
#define AUDIO_VQE_NR_NOISE_DB_THR 45
#define AUDIO_VQE_NR_INTENSITY 15
#define AUDIO_TALK_COMMON_NR_ENABLE_KEY "ai.talk.common:a" "nrenable"
#define AUDIO_TALK_NR_USR_MODE_KEY "ai.talk.a" "nr:usrmode"
#define AUDIO_TALK_NR_NOISE_DB_THR_KEY "ai.talk.a" "nr:noisedbthr"
#define AUDIO_TALK_NR_INTENSITY_KEY "ai.talk.a" "nr:nrintensity"
#define AUDIO_TALK_NR_SPPRO_SWITCH_KEY "ai.talk.a" "nr:spproswitch"

#define AUDIO_EQ_GAIN_100HZ_DEFAULT (-10)
#define AUDIO_EQ_GAIN_200HZ_DEFAULT (-3)
#define AUDIO_EQ_GAIN_250HZ_DEFAULT (-2)
#define AUDIO_EQ_GAIN_350HZ_DEFAULT (-2)
#define AUDIO_EQ_GAIN_500HZ_DEFAULT (-1)
#define AUDIO_EQ_GAIN_800HZ_DEFAULT (-1)
#define AUDIO_EQ_GAIN_1200HZ_DEFAULT (-2)
#define AUDIO_EQ_GAIN_2500HZ_DEFAULT (-5)
#define AUDIO_EQ_GAIN_4000HZ_DEFAULT (-6)
#define AUDIO_EQ_GAIN_8000HZ_DEFAULT (-8)


static enum AudioFormat g_audioInFormats[] = { AUDIO_FORMAT_TYPE_PCM_16_BIT };

typedef struct {
    /* track impl */
    AudioPortTrack common;
    bool mute;
    float gain;
    uint32_t currentChannelId;
    uint64_t frames;
    struct PortPluginAttr inputAttr;
    struct AudioTimeStamp aiTimeStamp;
    /* MPP LIB */
    ot_audio_dev aiDeviceId;
    ot_aio_attr aiAttr;
    ot_ai_chn  aiChn;
    ot_mpp_chn stSrcChn;
    ot_mpp_chn stDestChn;
    /* talk vqe ao info */
    ot_audio_dev refAoDevId;
    ot_ai_chn refAoChnId;
    enum VqeScene vqeScene;
} SubPortTrackInContext;

typedef struct {
    bool opened;
    ot_audio_dev aiDeviceId;
    SubPortTrackInContext *portTrackInCtx[MAX_INPUT_PORT_TRACK_CNT];
    pthread_mutex_t mutex;
} SubPortInContext;

typedef struct {
    bool used;
    int32_t chnID;
    SubPortInContext *portInCtx;
} SubPortInMng;

static SubPortInMng g_subPortIn[MAX_INPUT_PORT_CNT];
static bool g_subPortInMngInited = false;
static pthread_mutex_t g_subPortInMngLock = PTHREAD_MUTEX_INITIALIZER;

static void AudioSubPortInMngInit(void)
{
    MEDIA_HAL_LOCK(g_subPortInMngLock);
    if (!g_subPortInMngInited) {
        for (int32_t i = 0; i < MAX_INPUT_PORT_CNT; i++) {
            g_subPortIn[i].used = false;
            g_subPortIn[i].chnID = i;
        }
        g_subPortInMngInited = true;
    }
    MEDIA_HAL_UNLOCK(g_subPortInMngLock);
    int32_t ret = MediaSystemInit();
    if (ret != 0) {
        MEDIA_HAL_LOGW(MODULE_NAME, "MediaSystemInit failed ret%d", ret);
    }
}

static int32_t AudioSubPortInMngGetFreeChn(void)
{
    MEDIA_HAL_LOCK(g_subPortInMngLock);
    if (!g_subPortInMngInited) {
        MEDIA_HAL_UNLOCK(g_subPortInMngLock);
        return MEDIA_HAL_ERR;
    }
    for (int32_t i = 0; i < MAX_INPUT_PORT_CNT; i++) {
        if (!g_subPortIn[i].used) {
            g_subPortIn[i].used = true;
            int32_t chnID = g_subPortIn[i].chnID;
            MEDIA_HAL_UNLOCK(g_subPortInMngLock);
            return chnID;
        }
    }
    MEDIA_HAL_UNLOCK(g_subPortInMngLock);
    MEDIA_HAL_LOGE(MODULE_NAME, "could get free audio Input channel");
    return MEDIA_HAL_ERR;
}

static int32_t AudioPrimarySubPortInGetFreeTrack(SubPortInContext *portInCtx)
{
    MEDIA_HAL_LOCK(portInCtx->mutex);
    for (int32_t i = 0; i < MAX_INPUT_PORT_TRACK_CNT; i++) {
        if (portInCtx->portTrackInCtx[i] == NULL) {
            MEDIA_HAL_UNLOCK(portInCtx->mutex);
            return i;
        }
    }
    MEDIA_HAL_UNLOCK(portInCtx->mutex);
    MEDIA_HAL_LOGE(MODULE_NAME, "could get free audio Input track");
    return MEDIA_HAL_ERR;
}

static int32_t AudioPrimarySubPortInFreeTrack(SubPortInContext *portInCtx,
    SubPortTrackInContext *portTrackInCtx)
{
    MEDIA_HAL_LOCK(portInCtx->mutex);
    for (int32_t i = 0; i < MAX_INPUT_PORT_TRACK_CNT; i++) {
        if (portInCtx->portTrackInCtx[i] == portTrackInCtx) {
            portInCtx->portTrackInCtx[i] = NULL;
            MEDIA_HAL_UNLOCK(portInCtx->mutex);
            return MEDIA_HAL_OK;
        }
    }
    MEDIA_HAL_UNLOCK(portInCtx->mutex);
    MEDIA_HAL_LOGE(MODULE_NAME, "could not found audio Input track");
    return MEDIA_HAL_ERR;
}

static int32_t AudioPrimarySubPortInMngFreeChn(int32_t chnID)
{
    MEDIA_HAL_LOCK(g_subPortInMngLock);
    if (!g_subPortInMngInited) {
        MEDIA_HAL_UNLOCK(g_subPortInMngLock);
        return MEDIA_HAL_ERR;
    }
    for (int32_t i = 0; i < MAX_INPUT_PORT_CNT; i++) {
        if (g_subPortIn[i].chnID == chnID) {
            g_subPortIn[i].used = false;
            MEDIA_HAL_UNLOCK(g_subPortInMngLock);
            return MEDIA_HAL_OK;
        }
    }
    MEDIA_HAL_UNLOCK(g_subPortInMngLock);
    MEDIA_HAL_LOGE(MODULE_NAME, "freehandle input audio Input handle invalid : %d", chnID);
    return MEDIA_HAL_ERR;
}

static void GetRecordVqeCommonParam(ot_ai_record_vqe_cfg *recordVqeAttr)
{
    td_bool drcEnable = false;
    td_bool hdrEnable = false;
    td_bool hpfEnable = false;
    td_bool rnrEnable = false;
    td_bool agcEnable = false;
    td_bool eqEnable = false;
    td_s32 tmpValue = 0;

    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.common:drcenable", 0, &drcEnable);
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.common:hdrenable", 0, &hdrEnable);
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.common:hpfenable", 0, &hpfEnable);
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.common:rnrenable", 0, &rnrEnable);
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.common:agcenable", 0, &agcEnable);
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.common:eqenable", 0, &eqEnable);

    recordVqeAttr->open_mask = (drcEnable ? OT_AI_RECORDVQE_MASK_DRC : 0) |
        (hdrEnable ? OT_AI_RECORDVQE_MASK_HDR : 0) | (hpfEnable ? OT_AI_RECORDVQE_MASK_HPF : 0) |
        (rnrEnable ? OT_AI_RECORDVQE_MASK_RNR : 0) | (agcEnable ? OT_AI_RECORDVQE_MASK_AGC : 0) |
        (eqEnable ? OT_AI_RECORDVQE_MASK_EQ : 0);

    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.common:framesample", AUDIO_AUDIO_POINT_NUM,
        &recordVqeAttr->frame_sample);
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.common:workstate", 0, &tmpValue);
    recordVqeAttr->work_state = (ot_vqe_work_state)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.common:recordtype", 0, &tmpValue);
    recordVqeAttr->record_type = (ot_vqe_record_type)tmpValue;
}

static void GetRecordVqeFilterParam(ot_ai_record_vqe_cfg *recordVqeAttr)
{
    td_s32 tmpValue = 0;

    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.drc:usrmode", 0, &recordVqeAttr->drc_cfg.usr_mode);
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.rnr:usrmode", 0, &recordVqeAttr->rnr_cfg.usr_mode);
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.hdr:usrmode", 0, &recordVqeAttr->hdr_cfg.usr_mode);
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.hpf:usrmode", 1, &recordVqeAttr->hpf_cfg.usr_mode);
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.hpf:freq", OT_AUDIO_HPF_FREQ_80, &tmpValue);
    recordVqeAttr->hpf_cfg.hpf_freq = (ot_audio_hpf_freq)tmpValue;
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.agc:usrmode", 0, &recordVqeAttr->agc_cfg.usr_mode);
}

static void GetRecordVqeEqParam(ot_ai_record_vqe_cfg *recordVqeAttr)
{
    td_s32 tmpValue = 0;

    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.eq:gaindb100hz",
        AUDIO_EQ_GAIN_100HZ_DEFAULT, &tmpValue);
    recordVqeAttr->eq_cfg.gain_db[0x0] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.eq:gaindb200hz",
        AUDIO_EQ_GAIN_200HZ_DEFAULT, &tmpValue);
    recordVqeAttr->eq_cfg.gain_db[0x1] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.eq:gaindb250hz",
        AUDIO_EQ_GAIN_250HZ_DEFAULT, &tmpValue);
    recordVqeAttr->eq_cfg.gain_db[0x2] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.eq:gaindb350hz",
        AUDIO_EQ_GAIN_350HZ_DEFAULT, &tmpValue);
    recordVqeAttr->eq_cfg.gain_db[0x3] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.eq:gaindb500hz",
        AUDIO_EQ_GAIN_500HZ_DEFAULT, &tmpValue);
    recordVqeAttr->eq_cfg.gain_db[0x4] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.eq:gaindb800hz",
        AUDIO_EQ_GAIN_800HZ_DEFAULT, &tmpValue);
    recordVqeAttr->eq_cfg.gain_db[0x5] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.eq:gaindb1.2khz",
        AUDIO_EQ_GAIN_1200HZ_DEFAULT, &tmpValue);
    recordVqeAttr->eq_cfg.gain_db[0x6] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.eq:gaindb2.5khz",
        AUDIO_EQ_GAIN_2500HZ_DEFAULT, &tmpValue);
    recordVqeAttr->eq_cfg.gain_db[0x7] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.eq:gaindb4khz",
        AUDIO_EQ_GAIN_4000HZ_DEFAULT, &tmpValue);
    recordVqeAttr->eq_cfg.gain_db[0x8] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.record.eq:gaindb8khz",
        AUDIO_EQ_GAIN_8000HZ_DEFAULT, &tmpValue);
    recordVqeAttr->eq_cfg.gain_db[0x9] = (int8_t)tmpValue;
}

static int32_t GetRecordVqeParam(ot_ai_record_vqe_cfg *recordVqeAttr)
{
    if (memset_s(recordVqeAttr, sizeof(ot_ai_record_vqe_cfg), 0, sizeof(ot_ai_record_vqe_cfg)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s ot_ai_record_vqe_cfg failed");
        return MEDIA_HAL_ERR;
    }

    GetRecordVqeCommonParam(recordVqeAttr);
    GetRecordVqeFilterParam(recordVqeAttr);
    GetRecordVqeEqParam(recordVqeAttr);

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static void SetRecordVqeWorkSampleRate(SubPortTrackInContext *portTrackInCtx, ot_ai_record_vqe_cfg *recordVqeAttr)
{
    switch (portTrackInCtx->aiAttr.sample_rate) {
        case OT_AUDIO_SAMPLE_RATE_48000:
            recordVqeAttr->work_sample_rate = OT_AUDIO_SAMPLE_RATE_48000;
            break;
        case OT_AUDIO_SAMPLE_RATE_16000:
            recordVqeAttr->work_sample_rate = OT_AUDIO_SAMPLE_RATE_16000;
            break;
        default:
            recordVqeAttr->work_sample_rate = OT_AUDIO_SAMPLE_RATE_48000;
            MEDIA_HAL_LOGW(MODULE_NAME, "VQE SampleRate only support 16K or 48K, default use 48K.");
            break;
    }
}

static void SetRecordVqeChannelNum(SubPortTrackInContext *portTrackInCtx, ot_ai_record_vqe_cfg *recordVqeAttr)
{
    if (portTrackInCtx->aiAttr.snd_mode == OT_AUDIO_SOUND_MODE_MONO) {
        recordVqeAttr->in_chn_num = AUDIO_MONO_SOUND_MODE_CHN_CNT;
        recordVqeAttr->out_chn_num = AUDIO_MONO_SOUND_MODE_CHN_CNT;
    } else {
        recordVqeAttr->in_chn_num = AUDIO_STEREO_SOUND_MODE_CHN_CNT;
        recordVqeAttr->out_chn_num = AUDIO_STEREO_SOUND_MODE_CHN_CNT;
    }
}

static int32_t ApplyRecordVqeAttr(SubPortTrackInContext *portTrackInCtx, ot_ai_record_vqe_cfg *recordVqeAttr)
{
    int32_t ret = ss_mpi_ai_set_record_vqe_attr(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn, recordVqeAttr);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_set_record_vqe_attr fail, ret:%x", ret);
        return ret;
    }
    ret = ss_mpi_ai_enable_vqe(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_enable_vqe fail, ret:%x", ret);
        return ret;
    }

    return MEDIA_HAL_OK;
}

static int32_t AudioPrimarySubPortInEnableRecordVqe(SubPortTrackInContext *portTrackInCtx)
{
    int32_t ret = SS_CONFACCESS_Init(PDT_INIPARAM, PDT_INIPARAM_PATH);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_CONFACCESS_Init failed");
        return MEDIA_HAL_ERR;
    }

    ot_ai_record_vqe_cfg recordVqeAttr = {};
    ret = GetRecordVqeParam(&recordVqeAttr);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "GetRecordVqeParam failed: 0x%x", ret);
        SS_CONFACCESS_Deinit(PDT_INIPARAM);
        return ret;
    }
    if (SS_CONFACCESS_Deinit(PDT_INIPARAM) != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_CONFACCESS_Deinit failed");
        return MEDIA_HAL_ERR;
    }

    SetRecordVqeWorkSampleRate(portTrackInCtx, &recordVqeAttr);
    SetRecordVqeChannelNum(portTrackInCtx, &recordVqeAttr);

    ret = ApplyRecordVqeAttr(portTrackInCtx, &recordVqeAttr);
    if (ret != MEDIA_HAL_OK) {
        return ret;
    }

    portTrackInCtx->vqeScene = VQE_SCENE_AI_RECORD;
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static void GetTalkVqeCommonParam(ot_ai_talk_vqe_cfg *talkVqeAttr, bool refAoEnable)
{
    td_bool hpfEnable = false;
    td_bool noiseReductionEnable = false;
    td_bool aecEnable = false;
    td_bool agcEnable = false;
    td_bool eqEnable = false;
    td_s32 tmpValue = 0;

    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.talk.common:hpfenable", 0, &hpfEnable);
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, AUDIO_TALK_COMMON_NR_ENABLE_KEY, 0,
        &noiseReductionEnable);
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.talk.common:aecenable", 0, &aecEnable);
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.talk.common:agcenable", 0, &agcEnable);
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.talk.common:eqenable", 0, &eqEnable);

    aecEnable = refAoEnable;
    talkVqeAttr->open_mask = (hpfEnable ? OT_AI_TALKVQE_MASK_HPF : 0) |
        (noiseReductionEnable ? AUDIO_TALK_VQE_MASK_NR : 0) | (aecEnable ? OT_AI_TALKVQE_MASK_AEC : 0) |
        (agcEnable ? OT_AI_TALKVQE_MASK_AGC : 0) | (eqEnable ? OT_AI_TALKVQE_MASK_EQ : 0);

    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.talk.common:worksamplerate", OT_AUDIO_SAMPLE_RATE_8000,
        &talkVqeAttr->work_sample_rate);
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.talk.common:framesample", AUDIO_AUDIO_POINT_NUM,
        &talkVqeAttr->frame_sample);
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.talk.common:workstate", 0, &tmpValue);
    talkVqeAttr->work_state = (ot_vqe_work_state)tmpValue;
}

static void GetTalkVqeFilterParam(ot_ai_talk_vqe_cfg *talkVqeAttr)
{
    td_s32 tmpValue = 0;

    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.talk.hpf:usrmode", 0, &talkVqeAttr->hpf_cfg.usr_mode);
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.talk.hpf:freq", OT_AUDIO_HPF_FREQ_120, &tmpValue);
    talkVqeAttr->hpf_cfg.hpf_freq = (ot_audio_hpf_freq)tmpValue;

    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, AUDIO_TALK_NR_USR_MODE_KEY, 0,
        &AUDIO_VQE_CFG_NR(talkVqeAttr).usr_mode);
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, AUDIO_TALK_NR_NOISE_DB_THR_KEY,
        AUDIO_VQE_NR_NOISE_DB_THR, &tmpValue);
    AUDIO_VQE_CFG_NR(talkVqeAttr).noise_db_threshold = (int16_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, AUDIO_TALK_NR_INTENSITY_KEY,
        AUDIO_VQE_NR_INTENSITY, &tmpValue);
    AUDIO_VQE_CFG_NR(talkVqeAttr).nr_intensity = (int16_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, AUDIO_TALK_NR_SPPRO_SWITCH_KEY, 0, &tmpValue);
    AUDIO_VQE_CFG_NR(talkVqeAttr).music_probe_switch = (int8_t)tmpValue;
}

static void GetTalkVqeAecAgcParam(ot_ai_talk_vqe_cfg *talkVqeAttr, bool refAoEnable)
{
    if (refAoEnable) {
        SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.talk.aec:usrmode", 0, &talkVqeAttr->aec_cfg.usr_mode);
    }
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ai.talk.agc:usrmode", 0, &talkVqeAttr->agc_cfg.usr_mode);
}

static int32_t GetTalkVqeParam(ot_ai_talk_vqe_cfg *talkVqeAttr, bool refAoEnable)
{
    if (memset_s(talkVqeAttr, sizeof(ot_ai_talk_vqe_cfg), 0, sizeof(ot_ai_talk_vqe_cfg)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s ot_ai_talk_vqe_cfg failed");
        return MEDIA_HAL_ERR;
    }

    GetTalkVqeCommonParam(talkVqeAttr, refAoEnable);
    GetTalkVqeFilterParam(talkVqeAttr);
    GetTalkVqeAecAgcParam(talkVqeAttr, refAoEnable);

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static int32_t AudioPrimarySubPortInEnableTalkVqe(SubPortTrackInContext *portTrackInCtx)
{
    MEDIA_HAL_LOGI(MODULE_NAME, "refAoDevId: %d, refAoChnId: %d", portTrackInCtx->refAoDevId,
        portTrackInCtx->refAoChnId);

    int32_t ret = SS_CONFACCESS_Init(PDT_INIPARAM, PDT_INIPARAM_PATH);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_CONFACCESS_Init failed");
        return MEDIA_HAL_ERR;
    }

    bool refAoEnable = (portTrackInCtx->refAoDevId != -1 && portTrackInCtx->refAoChnId != -1) ? true : false;
    ot_ai_talk_vqe_cfg talkVqeAttr = {};
    ret = GetTalkVqeParam(&talkVqeAttr, refAoEnable);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "GetTalkVqeParam failed: 0x%x", ret);
        SS_CONFACCESS_Deinit(PDT_INIPARAM);
        return ret;
    }
    ret = SS_CONFACCESS_Deinit(PDT_INIPARAM);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_CONFACCESS_Deinit failed");
        return MEDIA_HAL_ERR;
    }
    /* if VQE is enabled for multiple times, it needs to be disabled and then enabled */
    if (portTrackInCtx->vqeScene != VQE_SCENE_NONE) {
        ret = ss_mpi_ai_disable_vqe(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_disable_vqe fail, ret:%x", ret);
            return ret;
        }
    }

    ret = ss_mpi_ai_set_talk_vqe_attr(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn,
        portTrackInCtx->refAoDevId, portTrackInCtx->refAoChnId, &talkVqeAttr);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_set_talk_vqe_attr fail, ret:%x", ret);
        return ret;
    }
    ret = ss_mpi_ai_enable_vqe(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_enable_vqe fail, ret:%x", ret);
        return ret;
    }

    portTrackInCtx->vqeScene = VQE_SCENE_AI_TALK;
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static int32_t AudioPrimarySubPortInSetExtraParam(SubPortTrackInContext *portTrackInCtx,
    const struct InvokeAttr *invokeAttr)
{
    char aoDeviceIdKey[MAX_KEY_STR_LEN] = { 0 };
    char aoChanneIdKey[MAX_KEY_STR_LEN] = { 0 };
    int32_t aoDeviceId = -1;
    int32_t aoChannelId = -1;

    int32_t ret = sscanf_s((const char *)invokeAttr->request, "%s = %d %s = %d", aoDeviceIdKey, MAX_KEY_STR_LEN,
        &aoDeviceId, aoChanneIdKey, MAX_KEY_STR_LEN, &aoChannelId);
    if (ret == MEDIA_HAL_ERR) {
        MEDIA_HAL_LOGI(MODULE_NAME, "sscanf_s failed");
        return ret;
    }

    portTrackInCtx->refAoDevId = aoDeviceId;
    portTrackInCtx->refAoChnId = aoChannelId;

    /* when the AO exits, disable the talk vqe AEC algorithm */
    if ((aoDeviceId == -1 && aoChannelId == -1) && portTrackInCtx->vqeScene == VQE_SCENE_AI_TALK) {
        ret = AudioPrimarySubPortInEnableTalkVqe(portTrackInCtx);
        CHK_FAILED_RETURN(ret, MEDIA_HAL_OK, ret, "disable talk vqe aec algorithm failed");
        MEDIA_HAL_LOGI(MODULE_NAME, "when the ref ao exits, disable the talk vqe aec algorithm success");
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "%s = %d, %s = %d", aoDeviceIdKey, aoDeviceId, aoChanneIdKey, aoChannelId);
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

int32_t DualMono2Stereo(char *audioData, char *audioLeft,
    char *audioRight, uint32_t audioLen)
{
    if (audioData == NULL || audioLeft == NULL || audioRight == NULL) {
        return -1;
    }
    uint32_t i;
    uint32_t j;
    uint32_t framesize = AUDIO_STEREO_SOUND_MODE_CHN_CNT;
    uint32_t signleDataLen = audioLen / AUDIO_STEREO_SOUND_MODE_CHN_CNT;

    for (i = 0; i < signleDataLen; i += framesize) {
        for (j = 0; j < framesize; j++) {
            audioData[AUDIO_STEREO_SOUND_MODE_CHN_CNT * i + j] = audioLeft[i + j];
            audioData[AUDIO_STEREO_SOUND_MODE_CHN_CNT * i + framesize + j] = audioRight[i + j];
        }
    }

    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortInGetCapability(const struct AudioPort *port, struct AudioPortCapability *capability)
{
    MEDIA_HAL_UNUSED(port);
    capability->sampleRateMasks = 0;
    capability->sampleRateMasks |= AUDIO_SAMPLE_RATE_MASK_8000;
    capability->sampleRateMasks |= AUDIO_SAMPLE_RATE_MASK_12000;
    capability->sampleRateMasks |= AUDIO_SAMPLE_RATE_MASK_11025;
    capability->sampleRateMasks |= AUDIO_SAMPLE_RATE_MASK_16000;
    capability->sampleRateMasks |= AUDIO_SAMPLE_RATE_MASK_22050;
    capability->sampleRateMasks |= AUDIO_SAMPLE_RATE_MASK_24000;
    capability->sampleRateMasks |= AUDIO_SAMPLE_RATE_MASK_32000;
    capability->sampleRateMasks |= AUDIO_SAMPLE_RATE_MASK_44100;
    capability->sampleRateMasks |= AUDIO_SAMPLE_RATE_MASK_48000;
    capability->sampleRateMasks |= AUDIO_SAMPLE_RATE_MASK_64000;
    capability->sampleRateMasks |= AUDIO_SAMPLE_RATE_MASK_96000;
    capability->hardwareMode = true;
    capability->formats = g_audioInFormats;
    capability->subPortsNum = 0;
    capability->subPorts = NULL;
#if ((defined __HI3518EV300__) || (defined __HI3516EV200__))
    capability->channelCount = AUDIO_MONO_SOUND_MODE_CHN_CNT;
    MEDIA_HAL_LOGD(MODULE_NAME, "HI3518EV300 and HI3516EV200 only support mono");
#else
    capability->channelCount = AUDIO_STEREO_SOUND_MODE_CHN_CNT;
    MEDIA_HAL_LOGD(MODULE_NAME, "HI3516DV300 hi3556v200 hi3559v200 support stereo");
#endif
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortInGetFrameCount(AudioHandle trackHandle, uint64_t *count)
{
    MEDIA_HAL_UNUSED(trackHandle);

    *count = AUDIO_HAL_FRAME_COUNT;
    MEDIA_HAL_LOGI(MODULE_NAME, "FrameCount :%llu", *count);
    return MEDIA_HAL_OK;
}

static int32_t AiInit(SubPortInContext *portInCtx, ot_aio_attr aiAttr)
{
    int32_t ret = ss_mpi_ai_set_pub_attr(portInCtx->aiDeviceId, &aiAttr);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_set_pub_attr fail,ret:%x", ret);
        return ret;
    }
    ret = AudioAcodecSetVol(AUDIO_INPUT_VOL);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioAcodecSetVol failed ret:%x", ret);
    }
    ret = ss_mpi_ai_enable(portInCtx->aiDeviceId);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_enable fail,ret:%x", ret);
        return ret;
    }

    td_u32 chnCnt = aiAttr.chn_cnt >> ((td_u32)(aiAttr.snd_mode < 0 ? 0 : aiAttr.snd_mode));
    for (td_s32 i = 0; i < (td_s32)chnCnt; i++) {
        ret = ss_mpi_ai_enable_chn(portInCtx->aiDeviceId, i);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_enable_chn fail,ret:%x", ret);
            goto AI_DISABLE;
        }
    }

    ret = AudioAcodecInit(aiAttr.sample_rate);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ADPT_ACODEC_Init failed %#x", ret);
        goto AI_DISABLE;
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;

AI_DISABLE:
    (void)ss_mpi_ai_disable(portInCtx->aiDeviceId);
    return ret;
}

static bool AudioConvertAudioFormatToHAL(enum AudioFormat format, uint32_t *ptNumPerFrm)
{
    if (ptNumPerFrm == NULL) {
        return false;
    }
    switch (format) {
        case AUDIO_FORMAT_TYPE_PCM_16_BIT:
        case AUDIO_FORMAT_TYPE_AAC_LC:
        case AUDIO_FORMAT_TYPE_AAC_LD:
        case AUDIO_FORMAT_TYPE_AAC_ELD:
        case AUDIO_FORMAT_TYPE_AAC_HE_V1:
        case AUDIO_FORMAT_TYPE_AAC_HE_V2:
            *ptNumPerFrm = AUDIO_AAC_PCM_PTNUMPERFRM;
            break;
        case AUDIO_FORMAT_TYPE_G711A:
        case AUDIO_FORMAT_TYPE_G711U:
        case AUDIO_FORMAT_TYPE_G726:
            *ptNumPerFrm = AUDIO_G711_G726_PTNUMPERFRM;
            break;
        default:
            MEDIA_HAL_LOGW(MODULE_NAME, "Invalid audio format: %d", format);
            return false;
    }
    return true;
}

static int32_t AiAttrConfig(const struct PortPluginAttr *inputAttr, ot_aio_attr *aiAttr)
{
    if (memset_s(aiAttr, sizeof(ot_aio_attr), 0x0, sizeof(ot_aio_attr)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s: portInCtx aiAttr failed");
        return MEDIA_HAL_ERR;
    }
    if (!AudioConvertSampleRateToHAL(inputAttr->sampleRate, &(aiAttr->sample_rate))) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ConvertSampleRateToHAL sample_rate: %d failed", inputAttr->sampleRate);
        return MEDIA_HAL_ERR;
    }
    aiAttr->bit_width     = OT_AUDIO_BIT_WIDTH_16;
    aiAttr->work_mode     = OT_AIO_MODE_I2S_MASTER;
    if (!AudioConvertChannelToHAL(inputAttr->channelCount, &(aiAttr->snd_mode))) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ConvertChannelToHAL channelCount: %u failed ", inputAttr->channelCount);
        return MEDIA_HAL_ERR;
    }
    aiAttr->expand_flag      = 0;
    aiAttr->frame_num      = AUDIO_FRAME_NUM_IN_BUF;
    if (!AudioConvertAudioFormatToHAL(inputAttr->audioFormat, &(aiAttr->point_num_per_frame))) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ConvertCodecFormatToHAL codecFormat: %d failed ", inputAttr->audioFormat);
        return MEDIA_HAL_ERR;
    }
    aiAttr->chn_cnt      = inputAttr->channelCount;
    aiAttr->clk_share      = 1;
    aiAttr->i2s_type      = OT_AIO_I2STYPE_INNERCODEC;
    return MEDIA_HAL_OK;
}

static void SubPortTrackInInit(SubPortTrackInContext *portTrackInCtx, const struct PortPluginAttr *inputAttr,
    enum AudioPortPin audioPortPin)
{
    portTrackInCtx->inputAttr = *inputAttr;

    portTrackInCtx->common.audioPin = audioPortPin;
    portTrackInCtx->common.GetFrameCount = AudioPrimarySubPortInGetFrameCount;
    portTrackInCtx->common.GetLatency = NULL;
    portTrackInCtx->common.EnableTrack = AudioPrimarySubPortInEnableTrack;
    portTrackInCtx->common.PauseTrack = NULL;
    portTrackInCtx->common.ResumeTrack = NULL;
    portTrackInCtx->common.FlushTrack = NULL;
    portTrackInCtx->common.DisableTrack = AudioPrimarySubPortInDisableTrack;
    portTrackInCtx->common.AcquireFrame = AudioPrimarySubPortInAcquireFrame;
    portTrackInCtx->common.SendFrame = NULL;
    portTrackInCtx->common.GetPosition = AudioPrimarySubPortInGetPosition;
    portTrackInCtx->common.SetMute = AudioPrimarySubPortInSetMute;
    portTrackInCtx->common.GetMute = AudioPrimarySubPortInGetMute;
    portTrackInCtx->common.SetVolume = AudioPrimarySubPortInSetVolume;
    portTrackInCtx->common.GetVolume = AudioPrimarySubPortInGetVolume;
    portTrackInCtx->common.Invoke = AudioPrimarySubPortInInvoke;

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
}

int32_t AudioPrimarySubPortInSetMute(AudioHandle trackHandle, bool mute)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");

    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    int32_t ret;
    if (mute) {
        ret = AudioAcodecMute();
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "AudioAcodecMute,ret:%x", ret);
            return ret;
        }
    } else {
        ret = AudioAcodecUnMute();
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "AudioAcodecUnMute,ret:%x", ret);
            return ret;
        }
    }
    portTrackInCtx->mute = mute;
    MEDIA_HAL_LOGI(MODULE_NAME, "set mute %d", mute);
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortInGetMute(AudioHandle trackHandle, bool *mute)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    *mute = portTrackInCtx->mute;
    MEDIA_HAL_LOGE(MODULE_NAME, "get mute %d", *mute);
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortInSetVolume(AudioHandle trackHandle, float volume)
{
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    MEDIA_HAL_UNUSED(portTrackInCtx);
    int32_t ret = AudioAcodecSetVol((td_s32)volume);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioAcodecSetVol,ret:%x", ret);
        return ret;
    }
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortInGetVolume(AudioHandle trackHandle, float *volume)
{
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    MEDIA_HAL_UNUSED(portTrackInCtx);
    td_s32 vol;
    int32_t ret = AudioAcodecGetVol(&vol);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioAcodecGetVol,ret:%x", ret);
        return ret;
    }
    *volume = (float)vol;
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortInOpen(AudioHandle *portHandle)
{
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "input param portHandle null");
    int32_t ret;
    if (!g_subPortInMngInited) {
        AudioSubPortInMngInit();
    }

    int32_t portId = AudioSubPortInMngGetFreeChn();
    CHK_COND_RETURN(portId == MEDIA_HAL_INVALID_CHN_ID, MEDIA_HAL_ERR, "no free AI device");

    SubPortInContext *portInCtx = (SubPortInContext *)malloc(sizeof(SubPortInContext));
    if (portInCtx == NULL) {
        MEDIA_HAL_LOGI(MODULE_NAME, "malloc failed");
        ret = MEDIA_HAL_NO_MEM;
        goto FREE_CHN;
    }

    if (memset_s(portInCtx, sizeof(SubPortInContext), 0, sizeof(SubPortInContext)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        ret = MEDIA_HAL_ERR;
        goto FREE;
    }

    portInCtx->aiDeviceId = portId;
    pthread_mutex_init(&portInCtx->mutex, NULL);
    portInCtx->opened = true;

    *portHandle = portInCtx;
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;

FREE:
    free(portInCtx);
FREE_CHN:
    (void)AudioPrimarySubPortInMngFreeChn(portId);
    return ret;
}

int32_t AudioPrimarySubPortInClose(AudioHandle portHandle)
{
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "input param portHandle null");
    SubPortInContext *portInCtx = (SubPortInContext *)portHandle;

    (void)AudioPrimarySubPortInMngFreeChn(portInCtx->aiDeviceId);

    free(portInCtx);

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortInCreateTrack(AudioHandle portHandle, AudioHandle *trackHandle,
    struct PortPluginAttr *inputAttr, enum AudioPortPin audioPortPin)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "input param portHandle null");
    SubPortInContext *portInCtx = (SubPortInContext *)portHandle;

    int32_t trackIndex = AudioPrimarySubPortInGetFreeTrack(portInCtx);
    if (trackIndex == -1) {
        MEDIA_HAL_LOGE(MODULE_NAME, "can not found free Input Port track");
        return MEDIA_HAL_INVALID_PARAM;
    }

    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)malloc(sizeof(SubPortTrackInContext));
    if (portTrackInCtx == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc SubPortTrackInContext failed");
        return MEDIA_HAL_NO_MEM;
    }
    if (memset_s(portTrackInCtx, sizeof(SubPortTrackInContext), 0,
        sizeof(SubPortTrackInContext)) != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s SubPortTrackInContext failed");
    }

    SubPortTrackInInit(portTrackInCtx, inputAttr, audioPortPin);

    int32_t ret = AiAttrConfig(inputAttr, &(portTrackInCtx->aiAttr));
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AiAttrConfig %#x", ret);
        goto FREE_TRACK_IN;
    }
    ret = AiInit(portInCtx, portTrackInCtx->aiAttr);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AiInit failed %#x", ret);
        goto FREE_TRACK_IN;
    }
    
    portTrackInCtx->aiDeviceId = portInCtx->aiDeviceId;
    portTrackInCtx->aiChn = trackIndex;
    portTrackInCtx->refAoDevId = -1;
    portTrackInCtx->refAoChnId = -1;
    portTrackInCtx->vqeScene = VQE_SCENE_NONE;
    portTrackInCtx->currentChannelId = PORT_ID(OT_ID_AI, 0, (uint32_t)portTrackInCtx->aiChn);
    portInCtx->portTrackInCtx[trackIndex] = portTrackInCtx;
    *trackHandle = &portTrackInCtx->common;

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;

FREE_TRACK_IN:
    free(portTrackInCtx);
    return ret;
}

int32_t AudioPrimarySubPortInEnableTrack(AudioHandle trackHandle)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    int32_t ret;
    ot_ai_chn_param stAiChnParam;
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;
    MEDIA_HAL_LOGI(MODULE_NAME, "ss_mpi_ai_enable_chn in");
    ret = ss_mpi_ai_enable_chn(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MEDIA_HAL_OK, "ss_mpi_ai_enable_chn fail,ret:%x", ret);
        return ret;
    }
    /* set chn frame depth */
    ret = ss_mpi_ai_get_chn_param(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn, &stAiChnParam);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_get_chn_param fail, ret:%x", ret);
        return ret;
    }
    stAiChnParam.usr_frame_depth = AUDIO_FRAME_CACHED_DEPTH;
    ret = ss_mpi_ai_set_chn_param(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn, &stAiChnParam);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_set_chn_param fail, ret:%x", ret);
        return ret;
    }

    if (portTrackInCtx->inputAttr.type == AUDIO_IN_COMMUNICATION) {
        ret = AudioEffectEnable(&portTrackInCtx->common, portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn, PORT_IN);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "AudioEffectEnable failed: 0x%x", ret);
        }
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "ss_mpi_ai_enable_chn out");
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortInAcquireFrame(AudioHandle trackHandle, uint8_t *buffer, uint64_t requestBytes,
    uint64_t *replyBytes)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    ot_audio_frame stAudioFrm = {};
    ot_aec_frame stAecFrm = {};
    /*-1: block get frame */
    int32_t ret = ss_mpi_ai_get_frame(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn, &stAudioFrm, &stAecFrm, -1);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_get_frame fail, ret:%x", ret);
        return ret;
    }
    CHK_NULL_RETURN(stAudioFrm.virt_addr[0], MEDIA_HAL_ERR, "ss_mpi_ai_get_frame failed");
    if (portTrackInCtx->aiAttr.snd_mode == OT_AUDIO_SOUND_MODE_STEREO) {
        CHK_NULL_RETURN(stAudioFrm.virt_addr[1], MEDIA_HAL_ERR, "ss_mpi_ai_get_frame failed");
    }
    *replyBytes = stAudioFrm.len * portTrackInCtx->aiAttr.chn_cnt;
    if (requestBytes < stAudioFrm.len * portTrackInCtx->aiAttr.chn_cnt) {
        (void)ss_mpi_ai_release_frame(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn, &stAudioFrm, &stAecFrm);
        MEDIA_HAL_LOGE(MODULE_NAME, "audio capture input buffer too small");
        return MEDIA_HAL_INVALID_PARAM;
    }
    if (portTrackInCtx->aiAttr.snd_mode == OT_AUDIO_SOUND_MODE_STEREO) {
        // The hardware does not support dual channels, so the mono data is copied to dual channels.
        DualMono2Stereo(buffer, (char *)stAudioFrm.virt_addr[0], (char *)stAudioFrm.virt_addr[1], *replyBytes);
    } else if (portTrackInCtx->aiAttr.snd_mode == OT_AUDIO_SOUND_MODE_MONO) {
        if (memcpy_s(buffer, requestBytes, stAudioFrm.virt_addr[0], stAudioFrm.len) != EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "memcpy_s ot_audio_frame failed");
            (void)ss_mpi_ai_release_frame(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn, &stAudioFrm, &stAecFrm);
            return MEDIA_HAL_ERR;
        }
    }
    portTrackInCtx->aiTimeStamp.tvSec = stAudioFrm.time_stamp / TIME_CONVERSION_US_S;
    portTrackInCtx->aiTimeStamp.tvNSec =
        (stAudioFrm.time_stamp - portTrackInCtx->aiTimeStamp.tvSec * TIME_CONVERSION_US_S) * TIME_CONVERSION_NS_US;
    portTrackInCtx->frames = stAudioFrm.seq;
    /* finally you must release the stream */
    ret = ss_mpi_ai_release_frame(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn, &stAudioFrm, &stAecFrm);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_release_frame faile, ret %x", ret);
        return ret;
    }
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortInGetPosition(AudioHandle trackHandle,
    uint64_t *frames, struct AudioTimeStamp *time)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    time->tvSec = portTrackInCtx->aiTimeStamp.tvSec;
    time->tvNSec = portTrackInCtx->aiTimeStamp.tvNSec;
    *frames = portTrackInCtx->frames;
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortInDisableTrack(AudioHandle trackHandle)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    MEDIA_HAL_LOGI(MODULE_NAME, "ss_mpi_ai_disable_chn in");
    int32_t ret = ss_mpi_ai_disable_chn(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_disable_chn fail,ret:%x", ret);
    }

    portTrackInCtx->refAoDevId = -1;
    portTrackInCtx->refAoChnId = -1;
    portTrackInCtx->vqeScene = VQE_SCENE_NONE;

    MEDIA_HAL_LOGI(MODULE_NAME, "ss_mpi_ai_disable_chn out");
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortInDestroyTrack(AudioHandle portHandle, AudioHandle trackHandle)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "input param portHandle null");
    SubPortInContext *portInCtx = (SubPortInContext *)portHandle;
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    int32_t ret = ss_mpi_ai_disable_vqe(portTrackInCtx->aiDeviceId, portTrackInCtx->aiChn);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_disable_vqe failed: 0x%x", ret);
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "ss_mpi_ai_disable in");
    ret = ss_mpi_ai_disable(portInCtx->aiDeviceId);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ai_disable fail,ret:0x%x", ret);
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "ss_mpi_ai_disable out");

    AudioAcodecDeInit();

    ret = AudioPrimarySubPortInFreeTrack(portInCtx, portTrackInCtx);
    if (ret == -1) {
        MEDIA_HAL_LOGE(MODULE_NAME, "invalid trackHandle");
        return MEDIA_HAL_INVALID_PARAM;
    }

    if (portTrackInCtx->inputAttr.type == AUDIO_IN_COMMUNICATION) {
        ret = AudioEffectDisable(&portTrackInCtx->common, PORT_IN);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "AudioEffectDisable failed: 0x%x", ret);
        }
    }

    free(portTrackInCtx);
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortInInvoke(AudioHandle trackHandle, enum InvokeID invokeID, struct InvokeAttr *invokeAttr)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    int32_t ret = MEDIA_HAL_OK;
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    switch (invokeID) {
        case INVOKE_ID_GET_CHANNEL_ID:
            *((uint32_t *)invokeAttr->request) = portTrackInCtx->currentChannelId;
            invokeAttr->replyBytes = (uint64_t)sizeof(uint32_t);
            break;
        case INVOKE_ID_CONFIG_VQE:
            if (*(enum VqeScene *)(invokeAttr->request) == VQE_SCENE_AI_RECORD) {
                ret = AudioPrimarySubPortInEnableRecordVqe(portTrackInCtx);
            } else if (*(enum VqeScene *)(invokeAttr->request) == VQE_SCENE_AI_TALK) {
                ret = AudioPrimarySubPortInEnableTalkVqe(portTrackInCtx);
            }
            return ret;
        case INVOKE_ID_SET_EXTRA_PARAMS:
            return AudioPrimarySubPortInSetExtraParam(portTrackInCtx, invokeAttr);
        default:
            MEDIA_HAL_LOGW(MODULE_NAME, "Invalid invokeID: %d", invokeID);
            return MEDIA_HAL_INVALID_PARAM;
    }
    return MEDIA_HAL_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
