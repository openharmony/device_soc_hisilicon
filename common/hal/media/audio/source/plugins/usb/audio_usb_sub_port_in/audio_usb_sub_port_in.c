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

#include "audio_usb_sub_port_in.h"
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
#include <alsa/asoundlib.h>


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
#define MODULE_NAME "AudioPrimarySubPortIn"

#define MAX_INPUT_PORT_CNT          OT_AI_DEV_MAX_NUM
#define MAX_INPUT_PORT_TRACK_CNT    OT_AI_MAX_CHN_NUM

#define MAX_KEY_STR_LEN 10

#define PORT_ID_MODULE_SHIFT    16
#define PORT_ID_DEVICE_SHIFT    8

static inline unsigned int GetPortId(unsigned int module, unsigned int device, unsigned int channel)
{
    return ((unsigned int)(((module) << PORT_ID_MODULE_SHIFT) | ((device) << PORT_ID_DEVICE_SHIFT) | (channel)));
}

/* count of audio frame in Buffer */
#define AUDIO_FRAME_NUM_IN_BUF 30

/* sample per frame for all encoder(aacplus:2048) */
#define AUDIO_AUDIO_POINT_NUM 1024

#define AUDIO_FRAME_CACHED_DEPTH 5

#define AUDIO_INPUT_VOL 30

#define AUDIO_HAL_FRAME_COUNT 1024

static const float VOLUME_PERCENT_MIN_IN = 0.0f;
static const float VOLUME_PERCENT_MAX_IN = 100.0f;

const unsigned long long TIME_CONVERSION_US_S = 1000000ULL; /* us to s */
const unsigned long long TIME_CONVERSION_NS_US = 1000ULL; /* ns to us */

static enum AudioFormat g_audioInFormats[] = { AUDIO_FORMAT_TYPE_PCM_16_BIT };

typedef struct {
    /* track impl */
    AudioPortTrack common;
    bool mute;
    float volume;
    float gain;
    uint32_t currentChannelId;
    uint64_t frames;
    struct PortPluginAttr inputAttr;
    struct AudioTimeStamp aiTimeStamp;
    /* MPP LIB */
    ot_audio_dev aiDeviceId;
    snd_pcm_t *ai_pcm_handle;
    snd_mixer_t *ai_mixer_handle;
    snd_mixer_elem_t *ai_elem_vol;
    long aiMinVol, aiMaxVol;
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
    snd_pcm_t *ai_pcm_handle;
    snd_mixer_t *ai_mixer_handle;
    snd_mixer_elem_t *ai_elem_vol;
    long aiMinVol, aiMaxVol;
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

static int32_t AudioUsbSubPortInGetFreeTrack(SubPortInContext *portInCtx)
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

static int32_t AudioUsbSubPortInFreeTrack(SubPortInContext *portInCtx,
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

static int32_t AudioUsbSubPortInMngFreeChn(int32_t chnID)
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
            audioData[AUDIO_STEREO_SOUND_MODE_CHN_CNT * i + framesize + j] = audioLeft[i + j];
        }
    }

    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortInGetCapability(const struct AudioPort *port, struct AudioPortCapability *capability)
{
    MEDIA_HAL_UNUSED(port);
    capability->sampleRateMasks = 0;
    capability->sampleRateMasks |= AUDIO_SAMPLE_RATE_MASK_48000;
    capability->sampleRateMasks |= AUDIO_SAMPLE_RATE_MASK_96000;
    capability->hardwareMode = true;
    capability->formats = g_audioInFormats;
    capability->subPortsNum = 0;
    capability->subPorts = NULL;

    capability->channelCount = AUDIO_STEREO_SOUND_MODE_CHN_CNT;
    MEDIA_HAL_LOGD(MODULE_NAME, "usb support stereo");
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortInGetFrameCount(AudioHandle trackHandle, uint64_t *count)
{
    MEDIA_HAL_UNUSED(trackHandle);

    *count = AUDIO_HAL_FRAME_COUNT;
    MEDIA_HAL_LOGI(MODULE_NAME, "FrameCount :%llu", *count);
    return MEDIA_HAL_OK;
}

static int32_t AiInit(SubPortInContext *portInCtx, ot_aio_attr aiAttr)
{
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
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
    portTrackInCtx->common.GetFrameCount = AudioUsbSubPortInGetFrameCount;
    portTrackInCtx->common.GetLatency = NULL;
    portTrackInCtx->common.EnableTrack = AudioUsbSubPortInEnableTrack;
    portTrackInCtx->common.PauseTrack = NULL;
    portTrackInCtx->common.ResumeTrack = NULL;
    portTrackInCtx->common.FlushTrack = NULL;
    portTrackInCtx->common.DisableTrack = AudioUsbSubPortInDisableTrack;
    portTrackInCtx->common.AcquireFrame = AudioUsbSubPortInAcquireFrame;
    portTrackInCtx->common.SendFrame = NULL;
    portTrackInCtx->common.GetPosition = AudioUsbSubPortInGetPosition;
    portTrackInCtx->common.SetMute = AudioUsbSubPortInSetMute;
    portTrackInCtx->common.GetMute = AudioUsbSubPortInGetMute;
    portTrackInCtx->common.SetVolume = AudioUsbSubPortInSetVolume;
    portTrackInCtx->common.GetVolume = AudioUsbSubPortInGetVolume;
    portTrackInCtx->common.Invoke = AudioUsbSubPortInInvoke;

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
}

int32_t AudioUsbSubPortInSetMute(AudioHandle trackHandle, bool mute)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");

    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    if (mute) { // mute set input volume to 0
        snd_mixer_selem_set_capture_volume_all(portTrackInCtx->ai_elem_vol, 0);
    } else {  // unmute set input volume to last volume
        long volRange = portTrackInCtx->aiMaxVol - portTrackInCtx->aiMinVol;
        long vol = portTrackInCtx->aiMinVol + volRange * portTrackInCtx->volume / 100;
        snd_mixer_selem_set_capture_volume_all(portTrackInCtx->ai_elem_vol, vol);
    }
    portTrackInCtx->mute = mute;
    MEDIA_HAL_LOGI(MODULE_NAME, "set mute %d", mute);
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortInGetMute(AudioHandle trackHandle, bool *mute)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    *mute = portTrackInCtx->mute;
    MEDIA_HAL_LOGE(MODULE_NAME, "get mute %d", *mute);
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortInSetVolume(AudioHandle trackHandle, float volume)
{
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;
    int32_t ret;
    if (volume < VOLUME_PERCENT_MIN_IN || volume > VOLUME_PERCENT_MAX_IN) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioUsbSubPortInSetVolume, volume:%f err, it should [%f,%f]", volume,
                       VOLUME_PERCENT_MIN_IN, VOLUME_PERCENT_MAX_IN);
        return MEDIA_HAL_INVALID_PARAM;
    }
    const float epsinon = 0.00001;
    if ((volume >= -epsinon) && (volume <= epsinon)) {
        ot_audio_fade fade = {};
        portTrackInCtx->volume = volume;
        snd_mixer_selem_set_capture_volume_all(portTrackInCtx->ai_elem_vol, 0);
        return MEDIA_HAL_OK;
    }

    long vol = portTrackInCtx->aiMinVol + (portTrackInCtx->aiMaxVol - portTrackInCtx->aiMinVol) * volume / 100;
    ret = snd_mixer_selem_set_capture_volume_all(portTrackInCtx->ai_elem_vol, vol);
    if (ret < 0) {
        MEDIA_HAL_LOGI(MODULE_NAME, "SetVolume failed: %s\n", snd_strerror(ret));
        return ret;
    }
    MEDIA_HAL_LOGE(MODULE_NAME, "AudioUsbSubPortInSetVolume SetVolume: %d (raw=%ld)\n", volume, vol);
    portTrackInCtx->volume = volume;
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortInGetVolume(AudioHandle trackHandle, float *volume)
{
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    *volume = (float)portTrackInCtx->volume;
    return MEDIA_HAL_OK;
}

static SubPortInContext *AllocateAndInitPortContext(int32_t portId)
{
    SubPortInContext *portInCtx = (SubPortInContext *)malloc(sizeof(SubPortInContext));
    if (portInCtx == NULL) {
        MEDIA_HAL_LOGI(MODULE_NAME, "malloc failed");
        return NULL;
    }

    if (memset_s(portInCtx, sizeof(SubPortInContext), 0, sizeof(SubPortInContext)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        free(portInCtx);
        return NULL;
    }

    portInCtx->aiDeviceId = portId;
    return portInCtx;
}

static int32_t ConfigureAndOpenPcm(SubPortInContext *portInCtx, int card_usb)
{
    char pcmDevice[32];
    int snprintfRet = snprintf_s(pcmDevice, sizeof(pcmDevice), sizeof(pcmDevice) - 1, "hw:%d,0", card_usb);
    if (snprintfRet < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s failed");
        return MEDIA_HAL_ERR;
    }
    int pcm = snd_pcm_open(&(portInCtx->ai_pcm_handle), pcmDevice, SND_PCM_STREAM_CAPTURE, 0);
    if (pcm < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snd_pcm_open failed");
        return MEDIA_HAL_ERR;
    }
    return MEDIA_HAL_OK;
}

static int32_t ConfigureAndOpenMixer(SubPortInContext *portInCtx, int card_usb)
{
    int32_t ret = snd_mixer_open(&(portInCtx->ai_mixer_handle), 0);
    if (ret < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snd_mixer_open failed: %s\n", snd_strerror(ret));
        return ret;
    }
    char ctlDevice[32];
    int snprintfRet = snprintf_s(ctlDevice, sizeof(ctlDevice), sizeof(ctlDevice) - 1, "hw:%d", card_usb);
    if (snprintfRet < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s failed");
        snd_mixer_close(portInCtx->ai_mixer_handle);
        return MEDIA_HAL_ERR;
    }
    ret = snd_mixer_attach(portInCtx->ai_mixer_handle, ctlDevice);
    if (ret < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snd_mixer_attach failed: %s\n", snd_strerror(ret));
        snd_mixer_close(portInCtx->ai_mixer_handle);
        return ret;
    }

    snd_mixer_selem_register(portInCtx->ai_mixer_handle, NULL, NULL);
    snd_mixer_load(portInCtx->ai_mixer_handle);
    return MEDIA_HAL_OK;
}

static void SetupVolumeControl(SubPortInContext *portInCtx)
{
    // get element "Master" or "PCM" with volume control
    snd_mixer_elem_t *elem = NULL;
    for (elem = snd_mixer_first_elem(portInCtx->ai_mixer_handle); elem; elem = snd_mixer_elem_next(elem)) {
        if (snd_mixer_selem_is_active(elem)) {
            const char *name = snd_mixer_selem_get_name(elem);
            MEDIA_HAL_LOGE(MODULE_NAME, "snd_mixer_selem_get_name name=%s\n", name);
            if ((strcmp(name, "Mic") == 0) && snd_mixer_selem_has_capture_volume(elem)) {
                portInCtx->ai_elem_vol = elem;
                break;
            }
        }
    }

    if (!(portInCtx->ai_elem_vol)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Cannot find suitable mixer element (Master/PCM)\n");
        // maybe some device not support volume control
        return;
    }

    // get volume range
    snd_mixer_selem_get_capture_volume_range(portInCtx->ai_elem_vol, &portInCtx->aiMinVol, &portInCtx->aiMaxVol);
    MEDIA_HAL_LOGE(MODULE_NAME, "Volume range: [%ld, %ld]\n", portInCtx->aiMinVol, portInCtx->aiMaxVol);
}

int32_t AudioUsbSubPortInOpen(AudioHandle *portHandle, int card_usb)
{
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "input param portHandle null");
    int32_t ret;
    if (!g_subPortInMngInited) {
        AudioSubPortInMngInit();
    }

    int32_t portId = AudioSubPortInMngGetFreeChn();
    CHK_COND_RETURN(portId == MEDIA_HAL_INVALID_CHN_ID, MEDIA_HAL_ERR, "no free AI device");

    SubPortInContext *portInCtx = AllocateAndInitPortContext(portId);
    if (portInCtx == NULL) {
        ret = MEDIA_HAL_NO_MEM;
        goto FREE_CHN;
    }

    ret = ConfigureAndOpenPcm(portInCtx, card_usb);
    if (ret != MEDIA_HAL_OK) {
        goto FREE;
    }

    ret = ConfigureAndOpenMixer(portInCtx, card_usb);
    if (ret != MEDIA_HAL_OK) {
        goto FREE;
    }

    SetupVolumeControl(portInCtx);

    pthread_mutex_init(&portInCtx->mutex, NULL);
    portInCtx->opened = true;

    *portHandle = portInCtx;
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;

FREE:
    free(portInCtx);
FREE_CHN:
    (void)AudioUsbSubPortInMngFreeChn(portId);
    return ret;
}

int32_t AudioUsbSubPortInClose(AudioHandle portHandle)
{
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "input param portHandle null");
    SubPortInContext *portInCtx = (SubPortInContext *)portHandle;

    (void)AudioUsbSubPortInMngFreeChn(portInCtx->aiDeviceId);
    snd_mixer_close(portInCtx->ai_mixer_handle);
    snd_pcm_drain(portInCtx->ai_pcm_handle);
    snd_pcm_close(portInCtx->ai_pcm_handle);
    free(portInCtx);

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static SubPortTrackInContext *AllocateAndInitTrackContext(
    struct PortPluginAttr *inputAttr,
    enum AudioPortPin audioPortPin)
{
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)malloc(sizeof(SubPortTrackInContext));
    if (portTrackInCtx == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc SubPortTrackInContext failed");
        return NULL;
    }
    if (memset_s(portTrackInCtx, sizeof(SubPortTrackInContext), 0, sizeof(SubPortTrackInContext)) != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s SubPortTrackInContext failed");
    }
    // todo zhp
    SubPortTrackInInit(portTrackInCtx, inputAttr, audioPortPin);
    return portTrackInCtx;
}

static int32_t ConfigurePcmParams(snd_pcm_t *pcmHandle, ot_aio_attr *aiAttr)
{
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames = 4096;
    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(pcmHandle, params);
    snd_pcm_hw_params_set_access(pcmHandle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    // todo zhp
    snd_pcm_hw_params_set_format(pcmHandle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(pcmHandle, params, aiAttr->chn_cnt);
    snd_pcm_hw_params_set_rate_near(pcmHandle, params, &(aiAttr->sample_rate), 0);
    snd_pcm_hw_params_set_period_size_near(pcmHandle, params, &frames, 0);
    snd_pcm_hw_params(pcmHandle, params);
    snd_pcm_hw_params_free(params);
    return MEDIA_HAL_OK;
}

static void SetupTrackContext(SubPortTrackInContext *trackCtx, SubPortInContext *portCtx, int32_t trackIndex)
{
    trackCtx->aiDeviceId = portCtx->aiDeviceId;
    trackCtx->aiChn = trackIndex;
    trackCtx->ai_pcm_handle = portCtx->ai_pcm_handle;
    trackCtx->ai_mixer_handle = portCtx->ai_mixer_handle;
    trackCtx->ai_elem_vol = portCtx->ai_elem_vol;
    trackCtx->aiMinVol = portCtx->aiMinVol;
    trackCtx->aiMaxVol = portCtx->aiMaxVol;
    trackCtx->refAoDevId = -1;
    trackCtx->refAoChnId = -1;
    trackCtx->vqeScene = VQE_SCENE_NONE;
    trackCtx->currentChannelId = GetPortId(OT_ID_AI, 0, (uint32_t)trackCtx->aiChn);
    portCtx->portTrackInCtx[trackIndex] = trackCtx;
}

int32_t AudioUsbSubPortInCreateTrack(AudioHandle portHandle, AudioHandle *trackHandle,
    struct PortPluginAttr *inputAttr, enum AudioPortPin audioPortPin)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "input param portHandle null");
    SubPortInContext *portInCtx = (SubPortInContext *)portHandle;

    int32_t trackIndex = AudioUsbSubPortInGetFreeTrack(portInCtx);
    if (trackIndex == -1) {
        MEDIA_HAL_LOGE(MODULE_NAME, "can not found free Input Port track");
        return MEDIA_HAL_INVALID_PARAM;
    }

    SubPortTrackInContext *portTrackInCtx = AllocateAndInitTrackContext(inputAttr, audioPortPin);
    if (portTrackInCtx == NULL) {
        return MEDIA_HAL_NO_MEM;
    }

    int32_t ret = AiAttrConfig(inputAttr, &(portTrackInCtx->aiAttr));
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AiAttrConfig %#x", ret);
        goto FREE_TRACK_IN;
    }
    MEDIA_HAL_LOGE(MODULE_NAME, "aiAttr.bit_width: %d, snd_mode: %d, sample_rate: %d, chn_cnt: %d",
        portTrackInCtx->aiAttr.bit_width, portTrackInCtx->aiAttr.snd_mode,
        portTrackInCtx->aiAttr.sample_rate, portTrackInCtx->aiAttr.chn_cnt);

    ret = ConfigurePcmParams(portInCtx->ai_pcm_handle, &(portTrackInCtx->aiAttr));
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ConfigurePcmParams failed %#x", ret);
        goto FREE_TRACK_IN;
    }

    // do nothing
    ret = AiInit(portInCtx, portTrackInCtx->aiAttr);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AiInit failed %#x", ret);
        goto FREE_TRACK_IN;
    }

    SetupTrackContext(portTrackInCtx, portInCtx, trackIndex);
    *trackHandle = &portTrackInCtx->common;

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;

FREE_TRACK_IN:
    free(portTrackInCtx);
    return ret;
}

int32_t AudioUsbSubPortInEnableTrack(AudioHandle trackHandle)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    int32_t ret;
    ot_ai_chn_param stAiChnParam;
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;
    MEDIA_HAL_LOGI(MODULE_NAME, "AudioUsbSubPortInEnableTrack in");

    ret = snd_pcm_prepare(portTrackInCtx->ai_pcm_handle);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snd_pcm_prepare ret=0x%x", ret);
        return ret;
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "AudioUsbSubPortInEnableTrack out");
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortInAcquireFrame(AudioHandle trackHandle, uint8_t *buffer, uint64_t requestBytes,
    uint64_t *replyBytes)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;
    int32_t ret;
    long frames;
    int bytesPerSample = (portTrackInCtx->aiAttr.bit_width + 1);  // 0:8bit, 1:16bit
    int channels = (portTrackInCtx->aiAttr.snd_mode == OT_AUDIO_SOUND_MODE_STEREO) ? 2 : 1;
    int bytesPerFrame = bytesPerSample * channels;

    uint64_t remaining_bytes = requestBytes;
    *replyBytes = 0;

    #define PCM_WAIT_TIMEOUT_MS 100
    while (remaining_bytes > 0) {
        frames = snd_pcm_readi(portTrackInCtx->ai_pcm_handle, buffer, remaining_bytes / bytesPerFrame);
        if (frames > 0) {
            size_t bytesRead = frames * bytesPerFrame;
            remaining_bytes -= bytesRead;
            buffer += bytesRead;
            *replyBytes += bytesRead;
        } else if (frames == -EAGAIN) {
            MEDIA_HAL_LOGE(MODULE_NAME, "capture PCM readi -EAGAIN");
            snd_pcm_wait(portTrackInCtx->ai_pcm_handle, PCM_WAIT_TIMEOUT_MS);
        } else if (frames == -EBADFD) {
            MEDIA_HAL_LOGE(MODULE_NAME, "capture PCM bad state, preparing...");
            snd_pcm_prepare(portTrackInCtx->ai_pcm_handle);
            return MEDIA_HAL_ERR;
        } else {
            MEDIA_HAL_LOGE(MODULE_NAME, "capture err: %s", snd_strerror(frames));
            ret = snd_pcm_recover(portTrackInCtx->ai_pcm_handle, frames, 0);
            if (ret < 0) {
                MEDIA_HAL_LOGE(MODULE_NAME, "capture recover failed: %s", snd_strerror(ret));
                return MEDIA_HAL_ERR;
            }
        }
    }
    *replyBytes = requestBytes;
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortInGetPosition(AudioHandle trackHandle,
    uint64_t *frames, struct AudioTimeStamp *time)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    time->tvSec = portTrackInCtx->aiTimeStamp.tvSec;
    time->tvNSec = portTrackInCtx->aiTimeStamp.tvNSec;
    *frames = portTrackInCtx->frames;
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortInDisableTrack(AudioHandle trackHandle)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    snd_pcm_drain(portTrackInCtx->ai_pcm_handle);
    portTrackInCtx->refAoDevId = -1;
    portTrackInCtx->refAoChnId = -1;
    portTrackInCtx->vqeScene = VQE_SCENE_NONE;

    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortInDestroyTrack(AudioHandle portHandle, AudioHandle trackHandle)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "input param portHandle null");
    SubPortInContext *portInCtx = (SubPortInContext *)portHandle;
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    int32_t ret = AudioUsbSubPortInFreeTrack(portInCtx, portTrackInCtx);
    if (ret == -1) {
        MEDIA_HAL_LOGE(MODULE_NAME, "invalid trackHandle");
        return MEDIA_HAL_INVALID_PARAM;
    }

    free(portTrackInCtx);
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

int32_t AudioUsbSubPortInInvoke(AudioHandle trackHandle, enum InvokeID invokeID, struct InvokeAttr *invokeAttr)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    int32_t ret = MEDIA_HAL_OK;
    SubPortTrackInContext *portTrackInCtx = (SubPortTrackInContext *)trackHandle;

    // Currently no valid invokeID implemented
    MEDIA_HAL_LOGW(MODULE_NAME, "Invalid invokeID: %d", invokeID);
    return MEDIA_HAL_INVALID_PARAM;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
