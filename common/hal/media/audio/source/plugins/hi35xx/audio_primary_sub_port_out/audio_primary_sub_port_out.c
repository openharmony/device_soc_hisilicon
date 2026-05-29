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

#include <math.h>
#include <pthread.h>
#ifdef DRM_OPTION_ENABLE
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#endif
#ifndef __HuaweiLite__
#include <sys/syscall.h>
#endif
#include "securec.h"
/* SDK include */
#include "ss_mpi_audio.h"
#include "ot_common_aio.h"

#include "media_hal_common.h"
#include "audio_port_plugin_common.h"
#include "audio_effect_manager.h"

#include "audio_primary_sub_port_out.h"

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
#define TIME_S_TO_US_SCALE 1000000
#define TIME_US_TO_NS_SCALE 1000
#define NO_WAIT  0
#ifdef DRM_OPTION_ENABLE
#define DRM_IOCTL_BASE 'd'
#define DRM_COMMAND_BASE 0x40
#define DRM_HI3403V100_AUDIO_SET_CONFIG 2
#define SMART_DRM_AUDIO_DEFAULT_TMDS_CLK 148500
#define SMART_DRM_AUDIO_BIT_DEPTH_16 16
#define SMART_DRM_AUDIO_INTF_I2S 0
#define SMART_DRM_CARD_PATH "/dev/dri/card0"
#endif
#define AUDIO_VQE_AGC_NOISE_FLOOR (-50)
#define AUDIO_VQE_AGC_TARGET_LEVEL (-40)
#define AUDIO_JOIN3(prefix, middle, suffix) prefix##middle##suffix
#define AUDIO_AO_VQE_MASK_NR AUDIO_JOIN3(OT_AO_VQE_MASK_, A, NR)
#define AUDIO_VQE_CFG_NR(obj) ((obj)->a##nr_cfg)
#define AUDIO_VQE_NR_NOISE_DB_THR 30
#define AUDIO_PLAY_COMMON_NR_ENABLE_KEY "ao.common:a" "nrenable"
#define AUDIO_PLAY_NR_USR_MODE_KEY "ao.a" "nr:usrmode"
#define AUDIO_PLAY_NR_INTENSITY_KEY "ao.a" "nr:nrintensity"
#define AUDIO_PLAY_NR_NOISE_DB_THR_KEY "ao.a" "nr:noisedbthr"
#define AUDIO_PLAY_NR_SPPRO_SWITCH_KEY "ao.a" "nr:spproswitch"
#define AUDIO_PLAY_NR_RESERVED_KEY "ao.a" "nr:reserved"

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
    enum VqeScene vqeScene;
};

struct AudioSubPortOut {
    bool opened;
    bool mute;
    ot_aio_attr aoSourceAttr;
    ot_audio_dev aoDeviceId;
    ot_audio_dev aoHDMIDeviceId;
    bool aoEnabled;
    struct AudioSubPortOutTrack *hwOutputPortTrack[MAX_OUTPUT_PORT_TRACK_CNT];
    pthread_mutex_t mutex;
};

struct AudioSubPortOutMng {
    bool used;
    int32_t chnID;
    struct AudioSubPortOut *hwOutputPort;
};

#ifdef DRM_OPTION_ENABLE
struct SmartDrmAudioFrame {
    uint32_t enable;
    uint32_t sampleRate;
    uint32_t bitDepth;
    uint32_t channels;
    uint32_t soundIntf;
    uint32_t tmdsClk;
    uint64_t pcmAddr;
    uint32_t pcmBytes;
    uint32_t reserved;
};

#define DRM_IOCTL_HI3403V100_AUDIO_SET_CONFIG \
    _IOWR(DRM_IOCTL_BASE, DRM_COMMAND_BASE + DRM_HI3403V100_AUDIO_SET_CONFIG, struct SmartDrmAudioFrame)
#endif

static struct AudioSubPortOutMng g_audioHwOutputPorts[MAX_OUTPUT_PORT_CNT];
static bool g_audioOutputMngInited = false;
static pthread_mutex_t g_audioOutputMngLock = PTHREAD_MUTEX_INITIALIZER;

const int32_t VOLUME_DB_MIN = -121;
const int32_t VOLUME_DB_MAX = 6;
static const float VOLUME_PERCENT_MIN = 0.0f;
static const float VOLUME_PERCENT_MAX = 300.0f;
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

int32_t AudioPrimarySubPortOutGetCapability(const struct AudioPort *port,
    struct AudioPortCapability *capability)
{
    MEDIA_HAL_UNUSED(port);
    CHK_NULL_RETURN(capability, MEDIA_HAL_INVALID_PARAM, "param capability null");
#if ((defined __HI3518EV300__) || (defined __HI3516EV200__))
    capability->channelCount = AUDIO_MONO_SOUND_MODE_CHN_CNT;
    MEDIA_HAL_LOGD(MODULE_NAME, "HI3518EV300 and HI3516EV200 only support mono.");
#else
    capability->channelCount = AUDIO_STEREO_SOUND_MODE_CHN_CNT;
    MEDIA_HAL_LOGD(MODULE_NAME, "HI3516DV300 hi3556v200 hi3559v200 support stereo.");
#endif
    return MEDIA_HAL_OK;
}

#ifdef DRM_OPTION_ENABLE
static int32_t AudioHdmiPathIoctlSet(bool enable, const struct PortPluginAttr *portAttr)
{
    struct SmartDrmAudioFrame frame = {
        .enable = enable ? 1 : 0,
        .sampleRate = portAttr != NULL ? portAttr->sampleRate : 0,
        .bitDepth = SMART_DRM_AUDIO_BIT_DEPTH_16,
        .channels = AUDIO_STEREO_SOUND_MODE_CHN_CNT,
        .soundIntf = SMART_DRM_AUDIO_INTF_I2S,
        .tmdsClk = SMART_DRM_AUDIO_DEFAULT_TMDS_CLK,
    };
    int32_t fd;
    int32_t ret;

    fd = open(SMART_DRM_CARD_PATH, O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "open %s failed, errno:%d(%s)", SMART_DRM_CARD_PATH, errno, strerror(errno));
        return MEDIA_HAL_ERR;
    }

    ret = ioctl(fd, DRM_IOCTL_HI3403V100_AUDIO_SET_CONFIG, &frame);
    if (ret < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "DRM audio ioctl failed, errno:%d(%s)", errno, strerror(errno));
        close(fd);
        return MEDIA_HAL_ERR;
    }

    close(fd);
    return MEDIA_HAL_OK;
}
#endif

static int32_t AoInit(ot_audio_dev aoDeviceId, ot_aio_attr *aoAttr)
{
    int32_t ret = ss_mpi_ao_set_pub_attr(aoDeviceId, aoAttr);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_set_pub_attr fail,ret:0x%x", ret);
        return ret;
    }
    ret = ss_mpi_ao_enable(aoDeviceId);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_enable err,ret=0x%x", ret);
        return ret;
    }
    MEDIA_HAL_LOGD(MODULE_NAME, "ss_mpi_ao_enable OK");
    ret = ss_mpi_ao_set_track_mode(aoDeviceId, OT_AUDIO_TRACK_NORMAL);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_set_track_mode err,ret=0x%x", ret);
        goto AO_DISABLE;
    }

    ret = AudioAcodecInit(aoAttr->sample_rate);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ADPT_ACODEC_Init failed %#x", ret);
        goto AO_DISABLE;
    }
    return MEDIA_HAL_OK;

AO_DISABLE:
    (void)ss_mpi_ao_disable(aoDeviceId);
    return ret;
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
    aoSourceAttr->point_num_per_frame = AUDIO_OUT_PTNUMPERFRM;
    aoSourceAttr->clk_share = 0;
    if (audioPortPin == PIN_OUT_HDMI) {
        aoSourceAttr->i2s_type = OT_AIO_I2STYPE_INNERHDMI;
    } else if (audioPortPin == PIN_OUT_SPEAKER) {
        aoSourceAttr->i2s_type = OT_AIO_I2STYPE_INNERCODEC;
    }
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
    if (audioPortPin == PIN_OUT_HDMI) {
#ifdef DRM_OPTION_ENABLE
        ret = AudioHdmiPathIoctlSet(true, portAttr);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "enable HDMI audio path by DRM ioctl failed");
            return ret;
        }
#endif
        ret = AoInit(hwPortCtx->aoHDMIDeviceId, &aoAttr);
    } else if (audioPortPin == PIN_OUT_SPEAKER) {
        ret = AoInit(hwPortCtx->aoDeviceId, &aoAttr);
    }
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AoInit failed %#x", ret);
        if (audioPortPin == PIN_OUT_HDMI) {
#ifdef DRM_OPTION_ENABLE
            (void)AudioHdmiPathIoctlSet(false, portAttr);
#endif
        }
        return ret;
    }
    hwPortCtx->aoEnabled = true;
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortOutOpen(AudioHandle *portHandle)
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
    hwOutputPort->aoDeviceId = portIndex; // 0:speaker, 1:hdmi

    portIndex = AudioOutputPortMngGetFreeChn();
    if (portIndex == -1) {
        MEDIA_HAL_LOGE(MODULE_NAME, "can not found free Output Port");
        return MEDIA_HAL_INVALID_PARAM;
    }
    hwOutputPort->aoHDMIDeviceId = portIndex; // 1:hdmi
    pthread_mutex_init(&hwOutputPort->mutex, NULL);
    hwOutputPort->opened = true;
    *portHandle = hwOutputPort;
    MEDIA_HAL_LOGD(MODULE_NAME, "hwOutputPort open SUCCESS");
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortOutClose(AudioHandle portHandle)
{
    struct AudioSubPortOut *hwPortCtx = (struct AudioSubPortOut *)portHandle;
    CHK_NULL_RETURN(hwPortCtx, MEDIA_HAL_INVALID_PARAM, "input param portHandle null");
    CHK_FAILED_RETURN(hwPortCtx->opened, true, MEDIA_HAL_ERR, "not inited");
    int32_t ret;
    if (hwPortCtx->aoEnabled) {
        ret = ss_mpi_ao_disable(hwPortCtx->aoDeviceId);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "aoDeviceId ss_mpi_ao_disable fail,ret:0x%x", ret);
        }
        ret = ss_mpi_ao_disable(hwPortCtx->aoHDMIDeviceId);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "aoHDMIDeviceId ss_mpi_ao_disable fail,ret:0x%x", ret);
        }
#ifdef DRM_OPTION_ENABLE
        (void)AudioHdmiPathIoctlSet(false, NULL);
#endif
        hwPortCtx->aoEnabled = false;
    }
    ret = AudioOutputPortMngFreeChn(hwPortCtx->aoDeviceId);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioOutputPortMngFreeChn failed");
    }
    ret = AudioOutputPortMngFreeChn(hwPortCtx->aoHDMIDeviceId);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioOutputPortMngFreeChn failed");
    }
    AudioAcodecDeInit();
    pthread_mutex_destroy(&hwPortCtx->mutex);
    hwPortCtx->opened = false;
    free(hwPortCtx);
    hwPortCtx = NULL;

    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortOutGetLatency(AudioHandle trackHandle, uint32_t *latencyMs)
{
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_NULL_RETURN(latencyMs, MEDIA_HAL_INVALID_PARAM, "input param buffer null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, true, MEDIA_HAL_ERR, "not started yet");
    int32_t ret = ss_mpi_ao_get_chn_delay(hwPortTrackCtx->aoDeviceId, hwPortTrackCtx->aoChn, latencyMs);
    if (ret != MEDIA_HAL_OK) {
        *latencyMs = 0;
        return MEDIA_HAL_ERR;
    }
    return MEDIA_HAL_OK;
}

static void GetPlayVqeAgcParam(ot_ao_vqe_cfg *aoVqeAttr)
{
    td_s32 tmpValue = 0;

    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.agc:usrmode", 0, &aoVqeAttr->agc_cfg.usr_mode);
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.agc:noisesupswitch", 0, &tmpValue);
    aoVqeAttr->agc_cfg.noise_suppress_switch = (int16_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.agc:adjustspeed", 0, &tmpValue);
    aoVqeAttr->agc_cfg.adjust_speed = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.agc:improvesnr", 0, &tmpValue);
    aoVqeAttr->agc_cfg.improve_snr = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.agc:maxgain", 0, &tmpValue);
    aoVqeAttr->agc_cfg.max_gain = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.agc:noisefloor", AUDIO_VQE_AGC_NOISE_FLOOR, &tmpValue);
    aoVqeAttr->agc_cfg.noise_floor = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.agc:outputmode", 0, &tmpValue);
    aoVqeAttr->agc_cfg.output_mode = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.agc:targetlevel", AUDIO_VQE_AGC_TARGET_LEVEL, &tmpValue);
    aoVqeAttr->agc_cfg.target_level = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.agc:usehighpassfilt", 0, &tmpValue);
    aoVqeAttr->agc_cfg.use_hpf = (int8_t)tmpValue;
}

static void GetPlayVqeNrHpfParam(ot_ao_vqe_cfg *aoVqeAttr)
{
    td_s32 tmpValue = 0;

    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, AUDIO_PLAY_NR_USR_MODE_KEY, 0,
        &AUDIO_VQE_CFG_NR(aoVqeAttr).usr_mode);
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, AUDIO_PLAY_NR_INTENSITY_KEY, 0, &tmpValue);
    AUDIO_VQE_CFG_NR(aoVqeAttr).nr_intensity = (int16_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, AUDIO_PLAY_NR_NOISE_DB_THR_KEY,
        AUDIO_VQE_NR_NOISE_DB_THR, &tmpValue);
    AUDIO_VQE_CFG_NR(aoVqeAttr).noise_db_threshold = (int16_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, AUDIO_PLAY_NR_SPPRO_SWITCH_KEY, 0, &tmpValue);
    AUDIO_VQE_CFG_NR(aoVqeAttr).music_probe_switch = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, AUDIO_PLAY_NR_RESERVED_KEY, 0,
        &AUDIO_VQE_CFG_NR(aoVqeAttr).reserved);
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.hpf:usrmode", 0, &aoVqeAttr->hpf_cfg.usr_mode);
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.hpf:freq", OT_AUDIO_HPF_FREQ_80, &tmpValue);
    aoVqeAttr->hpf_cfg.hpf_freq = (ot_audio_hpf_freq)tmpValue;
}

static void GetPlayVqeEqParam(ot_ao_vqe_cfg *aoVqeAttr)
{
    td_s32 tmpValue = 0;

    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.eq:gaindb100hz", 0, &tmpValue);
    aoVqeAttr->eq_cfg.gain_db[0x0] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.eq:gaindb200hz", 0, &tmpValue);
    aoVqeAttr->eq_cfg.gain_db[0x1] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.eq:gaindb250hz", 0, &tmpValue);
    aoVqeAttr->eq_cfg.gain_db[0x2] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.eq:gaindb350hz", 0, &tmpValue);
    aoVqeAttr->eq_cfg.gain_db[0x3] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.eq:gaindb500hz", 0, &tmpValue);
    aoVqeAttr->eq_cfg.gain_db[0x4] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.eq:gaindb800hz", 0, &tmpValue);
    aoVqeAttr->eq_cfg.gain_db[0x5] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.eq:gaindb1.2khz", 0, &tmpValue);
    aoVqeAttr->eq_cfg.gain_db[0x6] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.eq:gaindb2.5khz", 0, &tmpValue);
    aoVqeAttr->eq_cfg.gain_db[0x7] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.eq:gaindb4khz", 0, &tmpValue);
    aoVqeAttr->eq_cfg.gain_db[0x8] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.eq:gaindb8khz", 0, &tmpValue);
    aoVqeAttr->eq_cfg.gain_db[0x9] = (int8_t)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.eq:reserved", 0, &aoVqeAttr->eq_cfg.reserved);
}

static int32_t GetPlayVqeParam(ot_ao_vqe_cfg *aoVqeAttr)
{
    if (memset_s(aoVqeAttr, sizeof(ot_ao_vqe_cfg), 0, sizeof(ot_ao_vqe_cfg)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
        return MEDIA_HAL_ERR;
    }

    td_bool hpfEnable = false;
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.common:hpfenable", 0, &hpfEnable);
    td_bool noiseReductionEnable = false;
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, AUDIO_PLAY_COMMON_NR_ENABLE_KEY, 0,
        &noiseReductionEnable);
    td_bool agcEnable = false;
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.common:agcenable", 0, &agcEnable);
    td_bool eqEnable = false;
    SS_CONFACCESS_GetBool(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.common:eqenable", 0, &eqEnable);

    aoVqeAttr->open_mask = (hpfEnable ? OT_AO_VQE_MASK_HPF : 0) | (noiseReductionEnable ? AUDIO_AO_VQE_MASK_NR : 0) |
        (agcEnable ? OT_AO_VQE_MASK_AGC : 0) | (eqEnable ? OT_AO_VQE_MASK_EQ : 0);

    td_s32 tmpValue = 0;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.common:workstate", 1, &tmpValue);
    aoVqeAttr->work_state = (ot_vqe_work_state)tmpValue;
    SS_CONFACCESS_GetInt(PDT_INIPARAM, AUDIO_VQE_MODULE, "ao.common:framesample",
        AUDIO_OUT_PTNUMPERFRM, &aoVqeAttr->frame_sample);

    GetPlayVqeAgcParam(aoVqeAttr);
    GetPlayVqeNrHpfParam(aoVqeAttr);
    GetPlayVqeEqParam(aoVqeAttr);

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}


static int32_t AudioOutputPortEnableVqe(struct AudioSubPortOutTrack *audioOutputTrack)
{
    CHK_NULL_RETURN(audioOutputTrack, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_FAILED_RETURN(audioOutputTrack->started, true, MEDIA_HAL_ERR, "not started");
    CHK_FAILED_RETURN(audioOutputTrack->aoSourceAttr.chn_cnt, 1, MEDIA_HAL_ERR, "not support");

    int32_t ret = SS_CONFACCESS_Init(PDT_INIPARAM, PDT_INIPARAM_PATH);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_CONFACCESS_Init failed");
        return MEDIA_HAL_ERR;
    }

    ot_ao_vqe_cfg aoVqeAttr = {};
    ret = GetPlayVqeParam(&aoVqeAttr);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "GetPlayVqeParam failed: 0x%x", ret);
        SS_CONFACCESS_Deinit(PDT_INIPARAM);
        return ret;
    }
    ret = SS_CONFACCESS_Deinit(PDT_INIPARAM);
    if (ret != TD_SUCCESS) {
        MEDIA_HAL_LOGE(MODULE_NAME, "SS_CONFACCESS_Deinit failed");
        return MEDIA_HAL_ERR;
    }

    switch (audioOutputTrack->aoSourceAttr.sample_rate) {
        case OT_AUDIO_SAMPLE_RATE_16000:
            aoVqeAttr.work_sample_rate = OT_AUDIO_SAMPLE_RATE_16000;
            break;
        case OT_AUDIO_SAMPLE_RATE_8000:
            aoVqeAttr.work_sample_rate = OT_AUDIO_SAMPLE_RATE_8000;
            break;
        default:
            aoVqeAttr.work_sample_rate = OT_AUDIO_SAMPLE_RATE_8000;
            MEDIA_HAL_LOGW(MODULE_NAME, "VQE SampleRate only support 16K or 8K, default use 8K.");
            break;
    }

    ret = ss_mpi_ao_set_vqe_attr(audioOutputTrack->aoDeviceId, audioOutputTrack->aoChn, &aoVqeAttr);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_set_vqe_attr fail, ret:%x", ret);
        return ret;
    }
    ret = ss_mpi_ao_enable_vqe(audioOutputTrack->aoDeviceId, audioOutputTrack->aoChn);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_enable_vqe fail, ret:%x", ret);
        return ret;
    }
    audioOutputTrack->vqeScene = VQE_SCENE_AO_MUSIC;
    MEDIA_HAL_LOGI(MODULE_NAME, "EnableVqe out");
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortOutEnableTrack(AudioHandle trackHandle)
{
    int32_t ret;
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, false, MEDIA_HAL_OK, "started yet");
    ret = ss_mpi_ao_enable_chn(hwPortTrackCtx->aoDeviceId, hwPortTrackCtx->aoChn);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_enable_chn left ret=0x%x", ret);
        return ret;
    }
    hwPortTrackCtx->started = true;

    if (hwPortTrackCtx->inputAttr.type == AUDIO_IN_COMMUNICATION) {
        ret = AudioEffectEnable(&hwPortTrackCtx->common, hwPortTrackCtx->aoDeviceId, hwPortTrackCtx->aoChn, PORT_OUT);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "AudioEffectEnable failed: 0x%x", ret);
        }
    }

    MEDIA_HAL_LOGD(MODULE_NAME, "ss_mpi_ao_enable_chn out");
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortOutPauseTrack(AudioHandle trackHandle)
{
    int32_t ret;
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, true, MEDIA_HAL_ERR, "not started yet");
    ret = ss_mpi_ao_pause_chn(hwPortTrackCtx->aoDeviceId, hwPortTrackCtx->aoChn);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_disable_chn  err:0x%x ", ret);
        return ret;
    }
    hwPortTrackCtx->paused = true;
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortOutResumeTrack(AudioHandle trackHandle)
{
    int32_t ret;
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_FAILED_RETURN(hwPortTrackCtx->paused, true, MEDIA_HAL_ERR, "not paused yet");
    ret = ss_mpi_ao_resume_chn(hwPortTrackCtx->aoDeviceId, hwPortTrackCtx->aoChn);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_resume_chn  err:0x%x ", ret);
        return ret;
    }
    hwPortTrackCtx->paused = false;
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortOutFlushTrack(AudioHandle trackHandle)
{
    int32_t ret;
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, true, MEDIA_HAL_ERR, "not started yet");
    ret = ss_mpi_ao_clr_chn_buf(hwPortTrackCtx->aoDeviceId, hwPortTrackCtx->aoChn);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_clr_chn_buf  err:0x%x ", ret);
    }
    return ret;
}

static int32_t AudioPrimarySubPortOutTrackSetMute(AudioHandle trackHandle, bool mute)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");

    struct AudioSubPortOutTrack *hwOutputPortTrack = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_FAILED_RETURN(hwOutputPortTrack->inited, true, MEDIA_HAL_ERR, "track not init");

    MEDIA_HAL_LOGI(MODULE_NAME, "set mute:%d success", mute);
    return MEDIA_HAL_OK;
}

static int32_t AudioPrimarySubPortOutTrackGetMute(AudioHandle trackHandle, bool *mute)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");

    struct AudioSubPortOutTrack *hwOutputPortTrack = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_FAILED_RETURN(hwOutputPortTrack->inited, true, MEDIA_HAL_ERR, "track not init");

    MEDIA_HAL_LOGI(MODULE_NAME, "Get mute success, current state is %s", *mute == true ? "mute" : "unmute");
    return MEDIA_HAL_OK;
}

static int32_t AudioPrimarySubPortOutTrackSetVolume(AudioHandle trackHandle, float volume)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    struct AudioSubPortOutTrack *hwOutputPortTrack = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_FAILED_RETURN(hwOutputPortTrack->inited, true, MEDIA_HAL_ERR, "track not init");

    int32_t tmpValume;
    int32_t ret;
    if (volume < VOLUME_PERCENT_MIN || volume > VOLUME_PERCENT_MAX) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderSetVolume, volume:%f err, it should [%f,%f]", volume,
                       VOLUME_PERCENT_MIN, VOLUME_PERCENT_MAX);
        return MEDIA_HAL_INVALID_PARAM;
    }
    const float EPSINON = 0.00001;
    if ((volume >= -EPSINON) && (volume <= EPSINON)) {
        ot_audio_fade fade = {};
        hwOutputPortTrack->volume = volume;
        ret = ss_mpi_ao_set_mute(hwOutputPortTrack->aoDeviceId, true, &fade);
        return ret;
    }

    AudioRenderVolumePercentToDb(volume, &tmpValume);
    if ((hwOutputPortTrack->volume >= -EPSINON) && (hwOutputPortTrack->volume <= EPSINON)) {
        ss_mpi_ao_set_mute(hwOutputPortTrack->aoDeviceId, false, NULL);
    }
    hwOutputPortTrack->volume = volume;
    ret = ss_mpi_ao_set_volume(hwOutputPortTrack->aoDeviceId, tmpValume);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_set_volume s32Ret=0x%x", ret);
    }
    return ret;
}

static int32_t AudioPrimarySubPortOutTrackGetVolume(AudioHandle trackHandle, float *volume)
{
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_NULL_RETURN(volume, MEDIA_HAL_INVALID_PARAM, "input param volume null");

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

int32_t AudioPrimarySubPortOutSendFrame(AudioHandle trackHandle, const uint8_t *buffer,
    uint64_t requestBytes, uint64_t *replyBytes)
{
    int32_t ret;
    ot_ao_chn_state chnStatus = {};
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_NULL_RETURN(buffer, MEDIA_HAL_INVALID_PARAM, "input param buffer null");
    CHK_NULL_RETURN(replyBytes, MEDIA_HAL_INVALID_PARAM, "input param replyBytes null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, true, MEDIA_HAL_ERR, "not started yet");
    ret = ss_mpi_ao_query_chn_status(hwPortTrackCtx->aoDeviceId, hwPortTrackCtx->aoChn, &chnStatus);
    if (ret == MEDIA_HAL_OK && chnStatus.chn_free_num == 0) {
        *replyBytes = 0;
        return OT_ERR_AO_BUF_FULL;
    }
    ot_audio_frame audioFrame = {};
    AudioRenderFillAudioFrame(hwPortTrackCtx, (const uint8_t *)buffer, requestBytes, &audioFrame);
    ret = ss_mpi_ao_send_frame(hwPortTrackCtx->aoDeviceId, hwPortTrackCtx->aoChn, &audioFrame, NO_WAIT);
    if (ret == OT_ERR_AO_BUF_FULL) {
        *replyBytes = 0;
        return OT_ERR_AO_BUF_FULL;
    } else if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_send_frame failed ret: %x", ret);
        return ret;
    }
    *replyBytes = requestBytes;
    hwPortTrackCtx->frames++;
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortOutTrackGetPosition(AudioHandle trackHandle, uint64_t *frames,
    struct AudioTimeStamp *time)
{
    ot_ao_chn_state chnStatus = {};
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_NULL_RETURN(frames, MEDIA_HAL_INVALID_PARAM, "input param frames null");
    CHK_NULL_RETURN(time, MEDIA_HAL_INVALID_PARAM, "input param time null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, true, MEDIA_HAL_ERR, "not started yet");

    int32_t ret = ss_mpi_ao_query_chn_status(hwPortTrackCtx->aoDeviceId, hwPortTrackCtx->aoChn, &chnStatus);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_query_chn_status error,ret=0x%x", ret);
        return ret;
    }
    *frames = hwPortTrackCtx->frames - chnStatus.chn_busy_num;
    time->tvSec = hwPortTrackCtx->currentPts / TIME_S_TO_US_SCALE;
    time->tvNSec = (hwPortTrackCtx->currentPts % TIME_S_TO_US_SCALE) / TIME_US_TO_NS_SCALE;
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortOutDisableTrack(AudioHandle trackHandle)
{
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_FAILED_RETURN(hwPortTrackCtx->started, true, MEDIA_HAL_ERR, "not started yet");
    int32_t ret = ss_mpi_ao_disable_chn(hwPortTrackCtx->aoDeviceId, hwPortTrackCtx->aoChn);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_disable_chn AUDIO_LEFT_CHN err:0x%x ", ret);
    }
    hwPortTrackCtx->vqeScene = VQE_SCENE_NONE;
    hwPortTrackCtx->started = false;
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortOutDestroyTrack(AudioHandle portHandle, AudioHandle trackHandle)
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
    if (hwPortTrackCtx->started) {
        ret = ss_mpi_ao_disable_chn(hwPortTrackCtx->aoDeviceId, hwPortTrackCtx->aoChn);
        if (ret != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "ss_mpi_ao_disable_chn fail,ret:%x", ret);
        }
    }
    if (hwPortTrackCtx->inputAttr.type == AUDIO_IN_COMMUNICATION) {
        ret = AudioEffectDisable(&hwPortTrackCtx->common, PORT_OUT);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "AudioEffectDisable failed: 0x%x", ret);
        }
    }

    free(hwPortTrackCtx);
    return MEDIA_HAL_OK;
}

int32_t AudioPrimarySubPortOutInvoke(AudioHandle trackHandle,
    enum InvokeID invokeID, struct InvokeAttr *invokeAttr)
{
    int32_t ret = MEDIA_HAL_OK;
    struct AudioSubPortOutTrack *hwPortTrackCtx = (struct AudioSubPortOutTrack *)trackHandle;
    CHK_NULL_RETURN(hwPortTrackCtx, MEDIA_HAL_INVALID_PARAM, "input param trackHandle null");
    CHK_NULL_RETURN(invokeAttr, MEDIA_HAL_INVALID_PARAM, "input param invokeAttr null");
    CHK_NULL_RETURN(invokeAttr->request, MEDIA_HAL_INVALID_PARAM, "input param request null");
    CHK_FAILED_RETURN(hwPortTrackCtx->inited, true, MEDIA_HAL_ERR, "not inited");
    switch (invokeID) {
        case INVOKE_ID_CONFIG_VQE:
            if (*(enum VqeScene *)(invokeAttr->request) == VQE_SCENE_AO_MUSIC) {
                ret = AudioOutputPortEnableVqe(hwPortTrackCtx);
            }
            return ret;
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
    portTrackOutCtx->common.GetLatency = AudioPrimarySubPortOutGetLatency;
    portTrackOutCtx->common.EnableTrack = AudioPrimarySubPortOutEnableTrack;
    portTrackOutCtx->common.DisableTrack = AudioPrimarySubPortOutDisableTrack;
    portTrackOutCtx->common.PauseTrack = AudioPrimarySubPortOutPauseTrack;
    portTrackOutCtx->common.ResumeTrack = AudioPrimarySubPortOutResumeTrack;
    portTrackOutCtx->common.FlushTrack = AudioPrimarySubPortOutFlushTrack;
    portTrackOutCtx->common.AcquireFrame = NULL;
    portTrackOutCtx->common.SendFrame = AudioPrimarySubPortOutSendFrame;
    portTrackOutCtx->common.GetPosition = AudioPrimarySubPortOutTrackGetPosition;
    portTrackOutCtx->common.SetMute = AudioPrimarySubPortOutTrackSetMute;
    portTrackOutCtx->common.GetMute = AudioPrimarySubPortOutTrackGetMute;
    portTrackOutCtx->common.SetVolume = AudioPrimarySubPortOutTrackSetVolume;
    portTrackOutCtx->common.GetVolume = AudioPrimarySubPortOutTrackGetVolume;
    portTrackOutCtx->common.Invoke = AudioPrimarySubPortOutInvoke;
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return;
}

int32_t AudioPrimarySubPortOutCreateTrack(AudioHandle portHandle, AudioHandle *trackHandle,
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
    SubPortTrackOutLoadImpl(hwOutputPortTrack, portAttr, audioPortPin);
    if (audioPortPin == PIN_OUT_HDMI) {
        hwOutputPortTrack->aoDeviceId = hwPortCtx->aoHDMIDeviceId;
    } else if (audioPortPin == PIN_OUT_SPEAKER) {
        hwOutputPortTrack->aoDeviceId = hwPortCtx->aoDeviceId;
    }
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
