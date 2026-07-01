 /*
 * Copyright (c) 2026 RKH Corp.
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

#include <math.h>
#include <pthread.h>
#ifndef __HuaweiLite__
#include <sys/syscall.h>
#endif
#include "securec.h"
/* SDK include */
#include "ss_mpi_audio.h"
#include "ot_common_aio.h"

#include "media_hal_common.h"
#include "audio_port_plugin_common.h"
#include <alsa/asoundlib.h>

#include "audio_usb_sub_port_out.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
#define MODULE_NAME "AudioSubPortOut"

#define MAX_OUTPUT_PORT_CNT          OT_AO_DEV_MAX_NUM
#define MAX_OUTPUT_PORT_TRACK_CNT    OT_AO_MAX_CHN_NUM

/* count of audio frame in Buffer */
#define AUDIO_FRAME_NUM_OUT_BUF 4
#define AUDIO_OUT_PTNUMPERFRM   1024
#define AUDIO_OUT_POINT_NUM_PER_FRAME 4096
#define TIME_S_TO_US_SCALE 1000000
#define TIME_US_TO_NS_SCALE 1000
#define NO_WAIT  0
#define PCM_WAIT_TIMEOUT_MS     100

struct AudioSubPortOutTrack {
    /* track impl */
    AudioPortTrack common;
    bool inited;
    bool started;
    bool paused;
    bool mute;
    float volume;
    uint64_t frames;
    int64_t firstFramePts;
    int64_t currentPts;
    struct PortPluginAttr inputAttr;
    /* MPP LIB */
    ot_aio_attr aoSourceAttr;
    ot_ao_chn aoChn;

    ot_audio_dev aoDeviceId;
    snd_pcm_t *ao_pcm_handle;
    snd_mixer_t *ao_mixer_handle;
    snd_mixer_elem_t *ao_elem_vol;
    long aoMinVol, aoMaxVol;
    enum VqeScene vqeScene;
};

struct AudioSubPortOut {
    bool opened;
    bool mute;
    ot_aio_attr aoSourceAttr;
    ot_audio_dev aoDeviceId;
    snd_pcm_t *ao_pcm_handle;
    snd_mixer_t *ao_mixer_handle;
    snd_mixer_elem_t *ao_elem_vol;
    long aoMinVol, aoMaxVol;
    bool aoEnabled;
    struct AudioSubPortOutTrack *hwOutputPortTrack[MAX_OUTPUT_PORT_TRACK_CNT];
    pthread_mutex_t mutex;
};

struct AudioSubPortOutMng {
    bool used;
    int32_t chnID;
    struct AudioSubPortOut *hwOutputPort;
};

static struct AudioSubPortOutMng g_audioHwOutputPorts[MAX_OUTPUT_PORT_CNT];
static bool g_audioOutputMngInited = false;
static pthread_mutex_t g_audioOutputMngLock = PTHREAD_MUTEX_INITIALIZER;

const int32_t VOLUME_DB_MIN = -121;
const int32_t VOLUME_DB_MAX = 6;
static const float VOLUME_PERCENT_MIN = 0.0f;
static const float VOLUME_PERCENT_MAX = 100.0f;
const int32_t VOLUME_PERCENT_BASE = 100;
const int32_t PERCENT_TO_GAIN_LOG_FACTOR = 10;

static void AudioRenderVolumePercentToDb(float volume, int32_t *db)
{
    double tmpDb = PERCENT_TO_GAIN_LOG_FACTOR * log((double)volume / VOLUME_PERCENT_BASE);
    if (tmpDb < VOLUME_DB_MIN) {
        tmpDb = VOLUME_DB_MIN;
    } else if (tmpDb > VOLUME_DB_MAX) {
        tmpDb = VOLUME_DB_MAX;
    }
    *db = (int32_t)tmpDb;
}

static void AudioOutputPortMngInit(void)
{
    MEDIA_HAL_LOCK(g_audioOutputMngLock);
    if (!g_audioOutputMngInited) {
        for (int32_t i = 0; i < MAX_OUTPUT_PORT_CNT; i++) {
            g_audioHwOutputPorts[i].used = false;
            g_audioHwOutputPorts[i].chnID = i;
        }
        g_audioOutputMngInited = true;
    }
    MEDIA_HAL_UNLOCK(g_audioOutputMngLock);
}

static int32_t AudioOutputPortMngGetFreeChn()
{
    MEDIA_HAL_LOCK(g_audioOutputMngLock);
    if (!g_audioOutputMngInited) {
        MEDIA_HAL_UNLOCK(g_audioOutputMngLock);
        return MEDIA_HAL_ERR;
    }
    for (int32_t i = 0; i < MAX_OUTPUT_PORT_CNT; i++) {
        MEDIA_HAL_LOGE(MODULE_NAME, "MAX_OUTPUT_PORT_CNT %d, g_audioHwOutputPorts[i].used = %d, chnID = %d",
            MAX_OUTPUT_PORT_CNT, g_audioHwOutputPorts[i].used, g_audioHwOutputPorts[i].chnID);
        if (!g_audioHwOutputPorts[i].used) {
            g_audioHwOutputPorts[i].used = true;
            int32_t chnID = g_audioHwOutputPorts[i].chnID;
            MEDIA_HAL_UNLOCK(g_audioOutputMngLock);
            return chnID;
        }
    }
    MEDIA_HAL_UNLOCK(g_audioOutputMngLock);
    MEDIA_HAL_LOGE(MODULE_NAME, "could get free audio Output channel");
    return MEDIA_HAL_ERR;
}

static int32_t AudioOutputPortGetFreeTrack(struct AudioSubPortOut *hwAudioOutputPort)
{
    MEDIA_HAL_LOCK(hwAudioOutputPort->mutex);
    for (int32_t i = 0; i < MAX_OUTPUT_PORT_TRACK_CNT; i++) {
        if (hwAudioOutputPort->hwOutputPortTrack[i] == NULL) {
            MEDIA_HAL_UNLOCK(hwAudioOutputPort->mutex);
            return i;
        }
    }
    MEDIA_HAL_UNLOCK(hwAudioOutputPort->mutex);
    MEDIA_HAL_LOGE(MODULE_NAME, "could get free audio Output track");
    return MEDIA_HAL_ERR;
}

static int32_t AudioOutputPortFreeTrack(struct AudioSubPortOut *hwAudioOutputPort,
    struct AudioSubPortOutTrack *hwOutputPortTrack)
{
    MEDIA_HAL_LOCK(hwAudioOutputPort->mutex);
    for (int32_t i = 0; i < MAX_OUTPUT_PORT_TRACK_CNT; i++) {
        if (hwAudioOutputPort->hwOutputPortTrack[i] == hwOutputPortTrack) {
            hwAudioOutputPort->hwOutputPortTrack[i] = NULL;
            MEDIA_HAL_UNLOCK(hwAudioOutputPort->mutex);
            return MEDIA_HAL_OK;
        }
    }
    MEDIA_HAL_UNLOCK(hwAudioOutputPort->mutex);
    MEDIA_HAL_LOGE(MODULE_NAME, "could not found audio Output track");
    return MEDIA_HAL_ERR;
}

static int32_t AudioOutputPortMngFreeChn(int32_t chnID)
{
    MEDIA_HAL_LOCK(g_audioOutputMngLock);
    if (!g_audioOutputMngInited) {
        MEDIA_HAL_UNLOCK(g_audioOutputMngLock);
        return MEDIA_HAL_ERR;
    }
    for (int32_t i = 0; i < MAX_OUTPUT_PORT_CNT; i++) {
        if (g_audioHwOutputPorts[i].chnID == chnID) {
            g_audioHwOutputPorts[i].used = false;
            MEDIA_HAL_UNLOCK(g_audioOutputMngLock);
            return MEDIA_HAL_OK;
        }
    }
    MEDIA_HAL_UNLOCK(g_audioOutputMngLock);
    MEDIA_HAL_LOGE(MODULE_NAME, "freehandle input audio Output handle invalid : %d", chnID);
    return MEDIA_HAL_ERR;
}

int32_t AudioUsbSubPortOutGetCapability(const struct AudioPort *port,
    struct AudioPortCapability *capability)
{
    MEDIA_HAL_UNUSED(port);
    CHK_NULL_RETURN(capability, MEDIA_HAL_INVALID_PARAM, "param capability null");

    capability->channelCount = AUDIO_STEREO_SOUND_MODE_CHN_CNT;
    MEDIA_HAL_LOGD(MODULE_NAME, "usb support stereo.");
    return MEDIA_HAL_OK;
}

static int32_t AoInit(ot_audio_dev aoDeviceId, ot_aio_attr *aoAttr)
{
    // todo zhp
    MEDIA_HAL_LOGE(MODULE_NAME, "AoInit success");
    return MEDIA_HAL_OK;
}

static int32_t AoSourceAttrConfig(
    struct PortPluginAttr *portAttr,
    ot_aio_attr *aoSourceAttr,
    enum AudioPortPin audioPortPin)
{
    if (memset_s(aoSourceAttr, sizeof(ot_aio_attr), 0x0, sizeof(ot_aio_attr)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s: hwOutputPort aoAttr failed");
        return MEDIA_HAL_ERR;
    }
    if (!AudioConvertSampleRateToHAL(portAttr->sampleRate, &aoSourceAttr->sample_rate)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ConvertSampleRateToHAL sample_rate: %d failed", portAttr->sampleRate);
        return MEDIA_HAL_ERR;
    }
    aoSourceAttr->bit_width     = OT_AUDIO_BIT_WIDTH_16;
    aoSourceAttr->work_mode     = OT_AIO_MODE_I2S_MASTER;
    if (!AudioConvertChannelToHAL(portAttr->channelCount, &aoSourceAttr->snd_mode)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ConvertChannelToHAL channelCount: %u failed", portAttr->channelCount);
        return MEDIA_HAL_ERR;
    }
    aoSourceAttr->chn_cnt = portAttr->channelCount;
    aoSourceAttr->expand_flag = 0;
    aoSourceAttr->frame_num = AUDIO_FRAME_NUM_OUT_BUF;
    aoSourceAttr->point_num_per_frame = AUDIO_OUT_POINT_NUM_PER_FRAME;
    aoSourceAttr->clk_share = 0;

    return MEDIA_HAL_OK;
}

static int32_t ConfigAndEnableAO(
    AudioHandle portHandle,
    struct PortPluginAttr *portAttr,
    enum AudioPortPin audioPortPin)
{
    struct AudioSubPortOut *hwPortCtx = (struct AudioSubPortOut *)portHandle;
    int32_t ret = AoSourceAttrConfig(portAttr, &hwPortCtx->aoSourceAttr, audioPortPin);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ConfigAoSourceAttr failed");
        return ret;
    }
    ot_aio_attr aoAttr = hwPortCtx->aoSourceAttr;

    MEDIA_HAL_LOGE(MODULE_NAME, "aoAttr.bit_width: %d, snd_mode: %d, sample_rate: %d, chn_cnt: %d",
        aoAttr.bit_width, aoAttr.snd_mode, aoAttr.sample_rate, aoAttr.chn_cnt);
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames = 4096;
    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(hwPortCtx->ao_pcm_handle, params);
    snd_pcm_hw_params_set_access(hwPortCtx->ao_pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    // todo zhp
    snd_pcm_hw_params_set_format(hwPortCtx->ao_pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(hwPortCtx->ao_pcm_handle, params, aoAttr.chn_cnt);
    snd_pcm_hw_params_set_rate_near(hwPortCtx->ao_pcm_handle, params, &(aoAttr.sample_rate), 0);
    snd_pcm_hw_params_set_period_size_near(hwPortCtx->ao_pcm_handle, params, &frames, 0);
    snd_pcm_hw_params(hwPortCtx->ao_pcm_handle, params);
    snd_pcm_hw_params_free(params);

    MEDIA_HAL_LOGE(MODULE_NAME, "aoAttr.frame_num: %d, point_num_per_frame: %d, period_size: %d",
                   aoAttr.frame_num, aoAttr.point_num_per_frame, frames);

    ret = AoInit(hwPortCtx->aoDeviceId, &aoAttr);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AoInit failed %#x", ret);
        return ret;
    }
    hwPortCtx->aoEnabled = true;
    return MEDIA_HAL_OK;
}

static int32_t InitMixerAndVolumeControl(struct AudioSubPortOut *hwOutputPort, int card_usb)
{
    int ret = snd_mixer_open(&(hwOutputPort->ao_mixer_handle), 0);
    if (ret < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snd_mixer_open failed: %s\n", snd_strerror(ret));
        return ret;
    }
    char ctlDevice[32];
    int snprintfRet = snprintf_s(ctlDevice, sizeof(ctlDevice), sizeof(ctlDevice) - 1, "hw:%d", card_usb);
    if (snprintfRet < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s failed");
        snd_mixer_close(hwOutputPort->ao_mixer_handle);
        return MEDIA_HAL_ERR;
    }
    ret = snd_mixer_attach(hwOutputPort->ao_mixer_handle, ctlDevice);
    if (ret < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snd_mixer_attach failed: %s\n", snd_strerror(ret));
        snd_mixer_close(hwOutputPort->ao_mixer_handle);
        return ret;
    }

    snd_mixer_selem_register(hwOutputPort->ao_mixer_handle, NULL, NULL);
    snd_mixer_load(hwOutputPort->ao_mixer_handle);

    // get element "Master" or "PCM" with volume control
    snd_mixer_elem_t *elem = NULL;
    for (elem = snd_mixer_first_elem(hwOutputPort->ao_mixer_handle); elem; elem = snd_mixer_elem_next(elem)) {
        if (snd_mixer_selem_is_active(elem)) {
            const char *name = snd_mixer_selem_get_name(elem);
            if (((strcmp(name, "PCM") == 0) || (strcmp(name, "Master") == 0)) &&
                snd_mixer_selem_has_playback_volume(elem)) {
                hwOutputPort->ao_elem_vol = elem;
                break;
            }
        }
    }

    if (!(hwOutputPort->ao_elem_vol)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Cannot find suitable mixer element (Master/PCM)\n");
        snd_mixer_close(hwOutputPort->ao_mixer_handle);
        // maybe some device not support volume control
        return MEDIA_HAL_OK;
    }

    // get volume range
    snd_mixer_selem_get_playback_volume_range(
        hwOutputPort->ao_elem_vol,
        &hwOutputPort->aoMinVol,
        &hwOutputPort->aoMaxVol);
    MEDIA_HAL_LOGE(MODULE_NAME, "Volume range: [%ld, %ld]\n", hwOutputPort->aoMinVol, hwOutputPort->aoMaxVol);

    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortOutOpen(AudioHandle *portHandle, int card_usb)
{
    struct AudioSubPortOut *hwOutputPort = NULL;
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "input param portHandle null");
    if (!g_audioOutputMngInited) {
        AudioOutputPortMngInit();
    }
    int32_t portIndex = AudioOutputPortMngGetFreeChn();
    if (portIndex == -1) {
        MEDIA_HAL_LOGE(MODULE_NAME, "can not found free Output Port");
        return MEDIA_HAL_INVALID_PARAM;
    }
    hwOutputPort = (struct AudioSubPortOut *)malloc(sizeof(struct AudioSubPortOut));
    if (hwOutputPort == NULL) {
        (void)AudioOutputPortMngFreeChn(portIndex);
        return MEDIA_HAL_NO_MEM;
    }
    if (memset_s(hwOutputPort, sizeof(struct AudioSubPortOut), 0, sizeof(struct AudioSubPortOut)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s AudioSubPortOut failed");
    }

    hwOutputPort->aoDeviceId = portIndex;
    char pcmDevice[32];
    int snprintfRet = snprintf_s(pcmDevice, sizeof(pcmDevice), sizeof(pcmDevice) - 1, "hw:%d,0", card_usb);
    if (snprintfRet < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s failed");
        return MEDIA_HAL_ERR;
    }
    int pcm = snd_pcm_open(&(hwOutputPort->ao_pcm_handle), pcmDevice, SND_PCM_STREAM_PLAYBACK, 0);
    if (pcm < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snd_pcm_open failed");
        return MEDIA_HAL_ERR;
    }
    MEDIA_HAL_LOGE(MODULE_NAME, "snd_pcm_open success");

    int ret = InitMixerAndVolumeControl(hwOutputPort, card_usb);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "InitMixerAndVolumeControl failed");
        return ret;
    }

    pthread_mutex_init(&hwOutputPort->mutex, NULL);
    hwOutputPort->opened = true;
    *portHandle = hwOutputPort;
    MEDIA_HAL_LOGE(MODULE_NAME, "hwOutputPort open SUCCESS");
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortOutClose(AudioHandle portHandle)
{
    struct AudioSubPortOut *hwPortCtx = (struct AudioSubPortOut *)portHandle;
    CHK_NULL_RETURN(hwPortCtx, MEDIA_HAL_INVALID_PARAM, "input param portHandle null");
    CHK_FAILED_RETURN(hwPortCtx->opened, true, MEDIA_HAL_ERR, "not inited");
    int32_t ret;

    ret = AudioOutputPortMngFreeChn(hwPortCtx->aoDeviceId);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioOutputPortMngFreeChn failed");
    }
    snd_mixer_close(hwPortCtx->ao_mixer_handle);
    snd_pcm_drain(hwPortCtx->ao_pcm_handle);
    snd_pcm_close(hwPortCtx->ao_pcm_handle);
    pthread_mutex_destroy(&hwPortCtx->mutex);
    hwPortCtx->opened = false;
    free(hwPortCtx);
    hwPortCtx = NULL;

    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortOutGetLatency(AudioHandle trackHandle, uint32_t *latencyMs)
{
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_NULL_RETURN(latencyMs, MEDIA_HAL_INVALID_PARAM, "input param buffer null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, true, MEDIA_HAL_ERR, "not started yet");

    #define AUDIO_LATENCY_MS 20
    *latencyMs = AUDIO_LATENCY_MS;
    return MEDIA_HAL_OK;
}


int32_t AudioUsbSubPortOutEnableTrack(AudioHandle trackHandle)
{
    int32_t ret;
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, false, MEDIA_HAL_OK, "started yet");

    ret = snd_pcm_prepare(hwPortTrackCtx->ao_pcm_handle);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snd_pcm_prepare left ret=0x%x", ret);
        return ret;
    }

    hwPortTrackCtx->started = true;

    MEDIA_HAL_LOGD(MODULE_NAME, "snd_pcm_prepare out");
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortOutPauseTrack(AudioHandle trackHandle)
{
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, true, MEDIA_HAL_ERR, "not started yet");
    snd_pcm_state_t state = snd_pcm_state(hwPortTrackCtx->ao_pcm_handle);
    MEDIA_HAL_LOGI(MODULE_NAME, "Current PCM state: %d", state);

    if (state == SND_PCM_STATE_RUNNING) {
        int ret = snd_pcm_pause(hwPortTrackCtx->ao_pcm_handle, 1);
        if (ret == 0) {
            MEDIA_HAL_LOGI(MODULE_NAME, "USB Audio paused.");
            return MEDIA_HAL_OK;
        }
        // zhp todo
        // if pause failed, try drain and prepare
        if (ret < 0) {
            MEDIA_HAL_LOGE(MODULE_NAME, "snd_pcm_pause(1) failed: %s", snd_strerror(ret));
            ret = snd_pcm_drain(hwPortTrackCtx->ao_pcm_handle);
            if (ret < 0) {
                snd_pcm_recover(hwPortTrackCtx->ao_pcm_handle, ret, 0);
                snd_pcm_drop(hwPortTrackCtx->ao_pcm_handle);
            }
            snd_pcm_prepare(hwPortTrackCtx->ao_pcm_handle);
            return MEDIA_HAL_OK;
        }
    } else {
        MEDIA_HAL_LOGW(MODULE_NAME, "PCM not in running state, ignore pause");
    }

    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortOutResumeTrack(AudioHandle trackHandle)
{
    int32_t ret;
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");

    // todo zhp
    snd_pcm_state_t state = snd_pcm_state(hwPortTrackCtx->ao_pcm_handle);
    MEDIA_HAL_LOGI(MODULE_NAME, "Current PCM state: %d", state);

    ret = snd_pcm_pause(hwPortTrackCtx->ao_pcm_handle, 0);
    if (ret == 0) {
        MEDIA_HAL_LOGI(MODULE_NAME, "USB Audio resumed.");
        return 0;
    }
    if (ret < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snd_pcm_pause(0) failed: %s", snd_strerror(ret));
        MEDIA_HAL_LOGW(MODULE_NAME, "Pause not supported, trying recover...");
        ret = snd_pcm_prepare(hwPortTrackCtx->ao_pcm_handle);
        if (ret < 0) {
            snd_pcm_recover(hwPortTrackCtx->ao_pcm_handle, ret, 0);
            return MEDIA_HAL_ERR;
        }

        return MEDIA_HAL_OK;
    }
    snd_pcm_recover(hwPortTrackCtx->ao_pcm_handle, ret, 0);
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortOutFlushTrack(AudioHandle trackHandle)
{
    int32_t ret;
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, true, MEDIA_HAL_ERR, "not started yet");
    snd_pcm_state_t state = snd_pcm_state(hwPortTrackCtx->ao_pcm_handle);
    if (state == SND_PCM_STATE_RUNNING) {
        snd_pcm_drop(hwPortTrackCtx->ao_pcm_handle);
    }
    // setup pcm to prepare state
    ret = snd_pcm_prepare(hwPortTrackCtx->ao_pcm_handle);
    if (ret < 0) {
        snd_pcm_recover(hwPortTrackCtx->ao_pcm_handle, ret, 0);
    }
    return MEDIA_HAL_OK;
}

static int32_t AudioUsbSubPortOutTrackSetMute(AudioHandle trackHandle, bool mute)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");

    struct AudioSubPortOutTrack *hwOutputPortTrack = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_FAILED_RETURN(hwOutputPortTrack->inited, true, MEDIA_HAL_ERR, "track not init");
    if (mute) { // mute set output volume to 0
        snd_mixer_selem_set_playback_volume_all(hwOutputPortTrack->ao_elem_vol, 0);
    } else {  // unmute set output volume to last volume
        long volRange = hwOutputPortTrack->aoMaxVol - hwOutputPortTrack->aoMinVol;
        long vol = hwOutputPortTrack->aoMinVol + volRange * hwOutputPortTrack->volume / 100;
        snd_mixer_selem_set_playback_volume_all(hwOutputPortTrack->ao_elem_vol, vol);
    }
    hwOutputPortTrack->mute = mute;
    MEDIA_HAL_LOGI(MODULE_NAME, "set mute:%d success", mute);
    return MEDIA_HAL_OK;
}

static int32_t AudioUsbSubPortOutTrackGetMute(AudioHandle trackHandle, bool *mute)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");

    struct AudioSubPortOutTrack *hwOutputPortTrack = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_FAILED_RETURN(hwOutputPortTrack->inited, true, MEDIA_HAL_ERR, "track not init");
    *mute = hwOutputPortTrack->mute;
    MEDIA_HAL_LOGI(MODULE_NAME, "Get mute success, current state is %s", *mute == true ? "mute" : "unmute");
    return MEDIA_HAL_OK;
}

static int32_t AudioUsbSubPortOutTrackSetVolume(AudioHandle trackHandle, float volume)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    struct AudioSubPortOutTrack *hwOutputPortTrack = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_FAILED_RETURN(hwOutputPortTrack->inited, true, MEDIA_HAL_ERR, "track not init");

    int32_t tmpValume;
    int32_t ret;
    if (volume < VOLUME_PERCENT_MIN || volume > VOLUME_PERCENT_MAX) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioUsbSubPortOutTrackSetVolume, volume:%f err, it should [%f,%f]", volume,
                       VOLUME_PERCENT_MIN, VOLUME_PERCENT_MAX);
        return MEDIA_HAL_INVALID_PARAM;
    }
    const float epsinon = 0.00001;
    if ((volume >= -epsinon) && (volume <= epsinon)) {
        hwOutputPortTrack->volume = volume;
        snd_mixer_selem_set_playback_volume_all(hwOutputPortTrack->ao_elem_vol, 0);
        return MEDIA_HAL_OK;
    }

    long volRange = hwOutputPortTrack->aoMaxVol - hwOutputPortTrack->aoMinVol;
    long vol = hwOutputPortTrack->aoMinVol + volRange * volume / 100;
    ret = snd_mixer_selem_set_playback_volume_all(hwOutputPortTrack->ao_elem_vol, vol);
    if (ret < 0) {
        MEDIA_HAL_LOGI(MODULE_NAME, "SetVolume failed: %s\n", snd_strerror(ret));
        return ret;
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "SetVolume: %d (raw=%ld)\n", volume, vol);

    hwOutputPortTrack->volume = volume;

    return MEDIA_HAL_OK;
}

static int32_t AudioUsbSubPortOutTrackGetVolume(AudioHandle trackHandle, float *volume)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");

    struct AudioSubPortOutTrack *hwOutputPortTrack = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_FAILED_RETURN(hwOutputPortTrack->inited, true, MEDIA_HAL_ERR, "track not init");
    *volume = hwOutputPortTrack->volume;
    return MEDIA_HAL_OK;
}

static void AudioRenderFillAudioFrame(const struct AudioSubPortOutTrack *hwPortTrack, const uint8_t *data,
    uint64_t len, ot_audio_frame *audioFrame)
{
    const uint32_t channels[OT_AUDIO_SOUND_MODE_BUTT] = {1, 2};
    audioFrame->bit_width = hwPortTrack->aoSourceAttr.bit_width;
    audioFrame->snd_mode = hwPortTrack->aoSourceAttr.snd_mode;
    audioFrame->time_stamp = 0;
    audioFrame->seq = (uint32_t)hwPortTrack->frames;
    audioFrame->phys_addr[0] = 0;
    audioFrame->phys_addr[1] = 0;
    audioFrame->pool_id[0] = 0;
    audioFrame->pool_id[1] = 0;
    if (audioFrame->snd_mode == OT_AUDIO_SOUND_MODE_MONO) {
        audioFrame->virt_addr[0] = (uint8_t*)data;
        audioFrame->virt_addr[1] = NULL;
        audioFrame->len = (uint32_t)len;
    } else if (audioFrame->snd_mode == OT_AUDIO_SOUND_MODE_STEREO) {
        audioFrame->virt_addr[0] = (uint8_t*)data;
        audioFrame->virt_addr[1] = (uint8_t*)data + len / channels[hwPortTrack->aoSourceAttr.snd_mode];
        audioFrame->len = (uint32_t)len / channels[hwPortTrack->aoSourceAttr.snd_mode];
    }
}

static uint8_t *ConvertToInterleaved(
    const uint8_t *buffer,
    uint64_t requestBytes,
    int bytesPerSample,
    uint64_t *framesCount)
{
    if (requestBytes == 0 || requestBytes > UINT32_MAX) {
        MEDIA_HAL_LOGE(MODULE_NAME, "invalid requestBytes: %llu", requestBytes);
        return NULL;
    }

    *framesCount = requestBytes / (bytesPerSample * AUDIO_STEREO_SOUND_MODE_CHN_CNT); // 每声道帧数
    uint8_t *interleavedBuf = malloc(requestBytes);
    if (!interleavedBuf) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc interleaved_buf failed");
        return NULL;
    }

    uint8_t *ch0 = (uint8_t *)buffer;
    uint8_t *ch1 = (uint8_t *)buffer + *framesCount * bytesPerSample;
    int bytesPerFrame = bytesPerSample * AUDIO_STEREO_SOUND_MODE_CHN_CNT;

    for (uint64_t i = 0; i < *framesCount; i++) {
        // 使用 safer memcpy alternative
        uint8_t *dstCh0 = interleavedBuf + i * bytesPerFrame;
        size_t dstSizeCh0 = requestBytes - i * bytesPerFrame;
        uint8_t *srcCh0 = ch0 + i * bytesPerSample;
        if (memcpy_s(dstCh0, dstSizeCh0, srcCh0, bytesPerSample) != EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "memcpy_s for ch0 failed");
            free(interleavedBuf);
            return NULL;
        }
        
        uint8_t *dstCh1 = interleavedBuf + i * bytesPerFrame + bytesPerSample;
        size_t dstSizeCh1 = requestBytes - (i * bytesPerFrame + bytesPerSample);
        uint8_t *srcCh1 = ch1 + i * bytesPerSample;
        if (memcpy_s(dstCh1, dstSizeCh1, srcCh1, bytesPerSample) != EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "memcpy_s for ch1 failed");
            free(interleavedBuf);
            return NULL;
        }
    }

    return interleavedBuf;
}

static int32_t WritePcmFrames(snd_pcm_t *pcmHandle, const uint8_t **buffer, uint64_t *remainingBytes, int bytesPerFrame)
{
    int32_t tryNum = 3;
    long frames;

    if (bytesPerFrame == 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "bytesPerFrame is 0, cannot perform division");
        return MEDIA_HAL_ERR;
    }

    while (*remainingBytes > 0) {
        frames = snd_pcm_writei(pcmHandle, *buffer, *remainingBytes / bytesPerFrame);
#if DEBUG
        snd_pcm_state_t state = snd_pcm_state(pcmHandle);
        MEDIA_HAL_LOGE(MODULE_NAME, "PCM state = %d, bytes_per_frame = %d", state, bytesPerFrame);
        MEDIA_HAL_LOGE(MODULE_NAME, "first 8 bytes: %02x %02x %02x %02x %02x %02x %02x %02x",
                       (*buffer)[0x00], (*buffer)[0x01], (*buffer)[0x02], (*buffer)[0x03],
                       (*buffer)[0x04], (*buffer)[0x05], (*buffer)[0x06], (*buffer)[0x07]);
#endif
        if (frames > 0) {
            size_t bytesWritten = frames * bytesPerFrame;
            *remainingBytes -= bytesWritten;
            *buffer += bytesWritten;
        } else if (frames == -EAGAIN) {
            MEDIA_HAL_LOGE(MODULE_NAME, "render PCM writei -EAGAIN");
            snd_pcm_wait(pcmHandle, PCM_WAIT_TIMEOUT_MS);
            tryNum--;
            if (tryNum == 0) {
                return MEDIA_HAL_OK;
            }
        } else if (frames == -EBADFD) {
            /* not #SND_PCM_STATE_PREPARED or #SND_PCM_STATE_RUNNING */
            MEDIA_HAL_LOGE(MODULE_NAME, "render PCM is not in the right state : %s", snd_strerror(frames));
            snd_pcm_prepare(pcmHandle);
            return MEDIA_HAL_ERR;
        } else {
            /* -ESTRPIPE: a suspend event occurred,
             * stream is suspended and waiting for an application recovery.
             * -EPIPE: an underrun occurred.
             */
            MEDIA_HAL_LOGE(MODULE_NAME, "err: %{public}s", snd_strerror(frames));
            int32_t ret = snd_pcm_recover(pcmHandle, frames, 0); // 0 for open render recover log.
            if (ret < 0) {
                MEDIA_HAL_LOGE(MODULE_NAME, "snd_pcm_recover failed: %s", snd_strerror(ret));
                return MEDIA_HAL_ERR;
            }
        }
    }
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortOutSendFrame(AudioHandle trackHandle, const uint8_t *buffer,
    uint64_t requestBytes, uint64_t *replyBytes)
{
    int32_t ret;
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_NULL_RETURN(buffer, MEDIA_HAL_INVALID_PARAM, "input param buffer null");
    CHK_NULL_RETURN(replyBytes, MEDIA_HAL_INVALID_PARAM, "input param replyBytes null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, true, MEDIA_HAL_ERR, "not started yet");

    int bytesPerSample = (hwPortTrackCtx->aoSourceAttr.bit_width + 1); // bit_width =0 8bit; bit_width =1 16bit.
    int channels = 1;
    if (hwPortTrackCtx->aoSourceAttr.snd_mode == OT_AUDIO_SOUND_MODE_STEREO) {
        channels = AUDIO_STEREO_SOUND_MODE_CHN_CNT;
    }
    int bytesPerFrame = bytesPerSample * channels;
    uint8_t *interleaved_buf = NULL;
    // 如果是立体声，需要转换为 interleaved 格式
    if (channels == AUDIO_STEREO_SOUND_MODE_CHN_CNT) {
        if (requestBytes == 0) {
            MEDIA_HAL_LOGE(MODULE_NAME, "requestBytes is 0, no need to malloc");
            return MEDIA_HAL_INVALID_PARAM;
        }
        uint64_t frames_count = 0;
        interleaved_buf = ConvertToInterleaved(buffer, requestBytes, bytesPerSample, &frames_count);
        if (!interleaved_buf) {
            return MEDIA_HAL_NO_MEM;
        }
        buffer = interleaved_buf;
    }
#if DEBUG
    static FILE *fp_debug = NULL;
    if (fp_debug == NULL) {
        fp_debug = fopen("/userdata/usb_out.pcm", "wb");
    }
    if (fp_debug) {
        fwrite(buffer, 1, requestBytes, fp_debug);
    }
#endif
    const uint8_t *bufferPtr = buffer;
    uint64_t remainingBytes = requestBytes;
    ret = WritePcmFrames(hwPortTrackCtx->ao_pcm_handle, &bufferPtr, &remainingBytes, bytesPerFrame);
    if (ret != MEDIA_HAL_OK) {
        return ret;
    }

    *replyBytes = requestBytes;
    hwPortTrackCtx->frames++;
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortOutTrackGetPosition(AudioHandle trackHandle, uint64_t *frames,
    struct AudioTimeStamp *time)
{
    // ot_ao_chn_state chnStatus = {};
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_NULL_RETURN(frames, MEDIA_HAL_INVALID_PARAM, "input param frames null");
    CHK_NULL_RETURN(time, MEDIA_HAL_INVALID_PARAM, "input param time null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, true, MEDIA_HAL_ERR, "not started yet");

    *frames = hwPortTrackCtx->frames - 0;
    time->tvSec = hwPortTrackCtx->currentPts / TIME_S_TO_US_SCALE;
    time->tvNSec = (hwPortTrackCtx->currentPts % TIME_S_TO_US_SCALE) / TIME_US_TO_NS_SCALE;
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortOutDisableTrack(AudioHandle trackHandle)
{
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, true, MEDIA_HAL_ERR, "not started yet");

    snd_pcm_drain(hwPortTrackCtx->ao_pcm_handle);
    hwPortTrackCtx->vqeScene = VQE_SCENE_NONE;
    hwPortTrackCtx->started = false;
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortOutDestroyTrack(AudioHandle portHandle, AudioHandle trackHandle)
{
    struct AudioSubPortOut *hwPortCtx = (struct AudioSubPortOut *)portHandle;
    CHK_NULL_RETURN(hwPortCtx, MEDIA_HAL_INVALID_PARAM, "input param portHandle null");
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    int32_t ret = AudioOutputPortFreeTrack(hwPortCtx, hwPortTrackCtx);
    if (ret == MEDIA_HAL_ERR) {
        MEDIA_HAL_LOGE(MODULE_NAME, "invalid trackHandle");
        return MEDIA_HAL_INVALID_PARAM;
    }

    free(hwPortTrackCtx);
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortOutInvoke(AudioHandle trackHandle,
    enum InvokeID invokeID, struct InvokeAttr *invokeAttr)
{
    int32_t ret = MEDIA_HAL_OK;
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_NULL_RETURN(invokeAttr, MEDIA_HAL_INVALID_PARAM, "input param invokeAttr null");
    CHK_NULL_RETURN(invokeAttr->request, MEDIA_HAL_INVALID_PARAM, "input param request null");
    CHK_FAILED_RETURN(hwPortTrackCtx->inited, true, MEDIA_HAL_ERR, "not inited");
    switch (invokeID) {
        case INVOKE_ID_GET_CHANNEL_ID:
            // Handle get channel id if needed
            break;
        case INVOKE_ID_CONFIG_VQE:
            // Handle config vqe if needed
            break;
        case INVOKE_ID_SET_EXTRA_PARAMS:
            // Handle set extra params if needed
            break;
        default:
            MEDIA_HAL_LOGW(MODULE_NAME, "Invalid invokeID: %d", invokeID);
            return MEDIA_HAL_INVALID_PARAM;
    }
    return MEDIA_HAL_OK;
}

static void SubPortTrackOutLoadImpl(struct AudioSubPortOutTrack *portTrackOutCtx,
    const struct PortPluginAttr *inputAttr, enum AudioPortPin audioPortPin)
{
    portTrackOutCtx->inputAttr = *inputAttr;

    portTrackOutCtx->common.audioPin = audioPortPin;
    portTrackOutCtx->common.GetFrameCount = NULL;
    portTrackOutCtx->common.GetLatency = AudioUsbSubPortOutGetLatency;
    portTrackOutCtx->common.EnableTrack = AudioUsbSubPortOutEnableTrack;
    portTrackOutCtx->common.DisableTrack = AudioUsbSubPortOutDisableTrack;
    portTrackOutCtx->common.PauseTrack = AudioUsbSubPortOutPauseTrack;
    portTrackOutCtx->common.ResumeTrack = AudioUsbSubPortOutResumeTrack;
    portTrackOutCtx->common.FlushTrack = AudioUsbSubPortOutFlushTrack;
    portTrackOutCtx->common.AcquireFrame = NULL;
    portTrackOutCtx->common.SendFrame = AudioUsbSubPortOutSendFrame;
    portTrackOutCtx->common.GetPosition = AudioUsbSubPortOutTrackGetPosition;
    portTrackOutCtx->common.SetMute = AudioUsbSubPortOutTrackSetMute;
    portTrackOutCtx->common.GetMute = AudioUsbSubPortOutTrackGetMute;
    portTrackOutCtx->common.SetVolume = AudioUsbSubPortOutTrackSetVolume;
    portTrackOutCtx->common.GetVolume = AudioUsbSubPortOutTrackGetVolume;
    portTrackOutCtx->common.Invoke = AudioUsbSubPortOutInvoke;
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return;
}

int32_t AudioUsbSubPortOutCreateTrack(AudioHandle portHandle, AudioHandle *trackHandle,
    struct PortPluginAttr *portAttr, enum AudioPortPin audioPortPin)
{
    int32_t trackIndex;
    struct AudioSubPortOutTrack *hwOutputPortTrack = NULL;
    struct AudioSubPortOut *hwPortCtx = (struct AudioSubPortOut *)portHandle;
    CHK_NULL_RETURN(hwPortCtx, MEDIA_HAL_INVALID_PARAM, "input param portHandle null");
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    trackIndex = AudioOutputPortGetFreeTrack(hwPortCtx);
    if (trackIndex == -1) {
        MEDIA_HAL_LOGE(MODULE_NAME, "can not found free Output Port track");
        return MEDIA_HAL_INVALID_PARAM;
    }

    if (!hwPortCtx->aoEnabled && (ConfigAndEnableAO(portHandle, portAttr, audioPortPin) != MEDIA_HAL_OK)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "config and anable AO failed");
        return MEDIA_HAL_INIT_FAIL;
    }

    hwOutputPortTrack = (struct AudioSubPortOutTrack *)malloc(sizeof(struct AudioSubPortOutTrack));
    if (hwOutputPortTrack == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc AudioSubPortOutTrack failed");
        return MEDIA_HAL_NO_MEM;
    }
    if (memset_s(hwOutputPortTrack, sizeof(struct AudioSubPortOutTrack), 0,
        sizeof(struct AudioSubPortOutTrack)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s AudioSubPortOutTrack failed");
    }
    hwOutputPortTrack->aoSourceAttr = hwPortCtx->aoSourceAttr;
    // todo zhp
    SubPortTrackOutLoadImpl(hwOutputPortTrack, portAttr, audioPortPin);
    hwOutputPortTrack->aoDeviceId = hwPortCtx->aoDeviceId;
    hwOutputPortTrack->ao_pcm_handle = hwPortCtx->ao_pcm_handle;
    hwOutputPortTrack->ao_mixer_handle = hwPortCtx->ao_mixer_handle;
    hwOutputPortTrack->ao_elem_vol = hwPortCtx->ao_elem_vol;
    hwOutputPortTrack->aoMinVol = hwPortCtx->aoMinVol;
    hwOutputPortTrack->aoMaxVol = hwPortCtx->aoMaxVol;
    hwOutputPortTrack->aoChn = trackIndex;
    hwOutputPortTrack->vqeScene = VQE_SCENE_NONE;
    hwOutputPortTrack->inited = true;
    hwPortCtx->hwOutputPortTrack[trackIndex] = hwOutputPortTrack;
    *trackHandle = &hwOutputPortTrack->common;
    return MEDIA_HAL_OK;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
