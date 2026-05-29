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
 * Description: acodec
 */

#include "audio_port_plugin_common.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "ot_acodec.h"
#include "ot_type.h"
#include "ot_common_aio.h"
#ifdef __LINUX__
#include "ot_osal_user.h"
#endif
#include "audio_types.h"
#include "media_hal_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
#define MODULE_NAME "AudioPortPluginCommon"
#define ACODEC_FILE "/dev/acodec"

#ifdef __LINUX__
#define AUDIO_OSAL_OPEN OSAL_OPEN
#define AUDIO_OSAL_CLOSE OSAL_CLOSE
#define AUDIO_OSAL_IOCTL OSAL_IOCTL
#else
#define AUDIO_OSAL_OPEN open
#define AUDIO_OSAL_CLOSE close
#define AUDIO_OSAL_IOCTL ioctl
#endif

typedef struct {
    ot_audio_sample_rate audioSampleRate;
    ot_acodec_fs acodecFs;
} FsSelect;

static const FsSelect g_fsSelectTable[] = {
    {OT_AUDIO_SAMPLE_RATE_8000, OT_ACODEC_FS_8000},
    {OT_AUDIO_SAMPLE_RATE_11025, OT_ACODEC_FS_11025},
    {OT_AUDIO_SAMPLE_RATE_12000, OT_ACODEC_FS_12000},
    {OT_AUDIO_SAMPLE_RATE_16000, OT_ACODEC_FS_16000},
    {OT_AUDIO_SAMPLE_RATE_22050, OT_ACODEC_FS_22050},
    {OT_AUDIO_SAMPLE_RATE_24000, OT_ACODEC_FS_24000},
    {OT_AUDIO_SAMPLE_RATE_32000, OT_ACODEC_FS_32000},
    {OT_AUDIO_SAMPLE_RATE_44100, OT_ACODEC_FS_44100},
    {OT_AUDIO_SAMPLE_RATE_48000, OT_ACODEC_FS_48000},
    {OT_AUDIO_SAMPLE_RATE_64000, OT_ACODEC_FS_64000},
    {OT_AUDIO_SAMPLE_RATE_96000, OT_ACODEC_FS_96000},
};

static const ot_audio_sample_rate g_audioSampleRateTable[] = {
    OT_AUDIO_SAMPLE_RATE_8000,
    OT_AUDIO_SAMPLE_RATE_11025,
    OT_AUDIO_SAMPLE_RATE_12000,
    OT_AUDIO_SAMPLE_RATE_16000,
    OT_AUDIO_SAMPLE_RATE_22050,
    OT_AUDIO_SAMPLE_RATE_24000,
    OT_AUDIO_SAMPLE_RATE_32000,
    OT_AUDIO_SAMPLE_RATE_44100,
    OT_AUDIO_SAMPLE_RATE_48000,
    OT_AUDIO_SAMPLE_RATE_64000,
    OT_AUDIO_SAMPLE_RATE_96000,
};

static int32_t I2sFsSel(const ot_audio_sample_rate enSampleRate, ot_acodec_fs *i2sFsSel)
{
    size_t num = sizeof(g_fsSelectTable) / sizeof(g_fsSelectTable[0]);
    bool isMatch = false;
    ot_acodec_fs i2sFs = OT_ACODEC_FS_BUTT;
    for (size_t i = 0; i < num; i++) {
        if (enSampleRate == g_fsSelectTable[i].audioSampleRate) {
            isMatch = true;
            i2sFs = g_fsSelectTable[i].acodecFs;
            break;
        }
    }

    if (!isMatch) {
        MEDIA_HAL_LOGE(MODULE_NAME, "input sample rate not match, sample rate is:%d", enSampleRate);
        return MEDIA_HAL_ERR;
    }
    *i2sFsSel = i2sFs;
    return MEDIA_HAL_OK;
}

int32_t AudioAcodecInit(ot_audio_sample_rate enSampleRate)
{
    int32_t fdAcodec = -1;
    ot_acodec_fs i2sFsSel = 0;

    fdAcodec = AUDIO_OSAL_OPEN(ACODEC_FILE, O_RDWR);
    if (fdAcodec < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "can't open Acodec:%s", ACODEC_FILE);
        return MEDIA_HAL_ERR;
    }

    int32_t ret = I2sFsSel(enSampleRate, &i2sFsSel);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "I2sFsSel failed:%d", ret);
        AUDIO_OSAL_CLOSE(fdAcodec);
        return ret;
    }
    if (AUDIO_OSAL_IOCTL(fdAcodec, OT_ACODEC_SET_I2S1_FS, &i2sFsSel)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "set acodec sample rate failed");
        AUDIO_OSAL_CLOSE(fdAcodec);
        return MEDIA_HAL_ERR;
    }
    ot_acodec_mixer inputMode = OT_ACODEC_MIXER_IN_D;
    if (AUDIO_OSAL_IOCTL(fdAcodec, OT_ACODEC_SET_MIXER_MIC, &inputMode)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "select acodec input_mode failed");
        AUDIO_OSAL_CLOSE(fdAcodec);
        return MEDIA_HAL_ERR;
    }
    AUDIO_OSAL_CLOSE(fdAcodec);
    return MEDIA_HAL_OK;
}

int32_t AudioAcodecDeInit(void)
{
    return MEDIA_HAL_OK;
}

int32_t AudioAcodecSetVol(int32_t s32Vol)
{
    int32_t fdAcodec = -1;
    int32_t inputVol;
    fdAcodec = AUDIO_OSAL_OPEN(ACODEC_FILE, O_RDWR);
    if (fdAcodec < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "can't open Acodec :%s", ACODEC_FILE);
        return MEDIA_HAL_ERR;
    }
    inputVol = s32Vol;
    if (AUDIO_OSAL_IOCTL(fdAcodec, OT_ACODEC_SET_INPUT_VOLUME, &inputVol)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "set acodec micin volume failed");
        AUDIO_OSAL_CLOSE(fdAcodec);
        return MEDIA_HAL_ERR;
    }
    AUDIO_OSAL_CLOSE(fdAcodec);
    return MEDIA_HAL_OK;
}

int32_t AudioAcodecGetVol(int32_t *s32Vol)
{
    if (s32Vol == NULL) {
        return MEDIA_HAL_INVALID_PARAM;
    }
    int32_t fdAcodec = -1;
    int32_t inputVol = 0;
    fdAcodec = AUDIO_OSAL_OPEN(ACODEC_FILE, O_RDWR);
    if (fdAcodec < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "can't open Acodec:%s", ACODEC_FILE);
        return MEDIA_HAL_ERR;
    }
    if (AUDIO_OSAL_IOCTL(fdAcodec, OT_ACODEC_GET_INPUT_VOLUME, &inputVol)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "set acodec micin volume failed");
        AUDIO_OSAL_CLOSE(fdAcodec);
        return MEDIA_HAL_ERR;
    }
    *s32Vol = inputVol;
    AUDIO_OSAL_CLOSE(fdAcodec);
    return MEDIA_HAL_OK;
}

static int32_t SetAudioAcodecMute(bool isMute)
{
    int32_t fdAcodec = -1;
    fdAcodec = AUDIO_OSAL_OPEN(ACODEC_FILE, O_RDWR);
    if (fdAcodec < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "can't open Acodec:%s", ACODEC_FILE);
        return MEDIA_HAL_ERR;
    }

    unsigned int muteCtrl = isMute ? 0x1 : 0x0;
    if (AUDIO_OSAL_IOCTL(fdAcodec, OT_ACODEC_SET_MICL_MUTE, &muteCtrl)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "OT_ACODEC_SET_MICL_MUTE failed");
        AUDIO_OSAL_CLOSE(fdAcodec);
        return MEDIA_HAL_ERR;
    }
    if (AUDIO_OSAL_IOCTL(fdAcodec, OT_ACODEC_SET_DACL_MUTE, &muteCtrl)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "OT_ACODEC_SET_MICL_MUTE failed");
        AUDIO_OSAL_CLOSE(fdAcodec);
        return MEDIA_HAL_ERR;
    }
    if (AUDIO_OSAL_IOCTL(fdAcodec, OT_ACODEC_SET_MICR_MUTE, &muteCtrl)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "OT_ACODEC_SET_MICR_MUTE failed");
        AUDIO_OSAL_CLOSE(fdAcodec);
        return MEDIA_HAL_ERR;
    }
    if (AUDIO_OSAL_IOCTL(fdAcodec, OT_ACODEC_SET_DACR_MUTE, &muteCtrl)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "OT_ACODEC_SET_DACR_MUTE failed");
        AUDIO_OSAL_CLOSE(fdAcodec);
        return MEDIA_HAL_ERR;
    }
    AUDIO_OSAL_CLOSE(fdAcodec);
    return MEDIA_HAL_OK;
}

int32_t AudioAcodecMute(void)
{
    MEDIA_HAL_LOGI(MODULE_NAME, "set audio mute");
    return SetAudioAcodecMute(true);
}

int32_t AudioAcodecUnMute(void)
{
    MEDIA_HAL_LOGI(MODULE_NAME, "set audio unmute");
    return SetAudioAcodecMute(false);
}

bool AudioConvertSampleRateToHAL(int32_t sampleRate, ot_audio_sample_rate *enSamplerate)
{
    size_t num = sizeof(g_audioSampleRateTable) / sizeof(g_audioSampleRateTable[0]);
    bool isMatch = false;
    ot_audio_sample_rate audioSampleRate = OT_AUDIO_SAMPLE_RATE_BUTT;
    for (size_t i = 0; i < num; i++) {
        if (sampleRate == (int32_t)g_audioSampleRateTable[i]) {
            isMatch = true;
            audioSampleRate = g_audioSampleRateTable[i];
            break;
        }
    }

    if (isMatch == false) {
        MEDIA_HAL_LOGE(MODULE_NAME, "input sample rate not match, sample rate is:%d", sampleRate);
        return false;
    }

    *enSamplerate = audioSampleRate;
    return true;
}

bool AudioConvertChannelToHAL(int32_t channel, ot_audio_snd_mode *enSoundMode)
{
    if (enSoundMode == NULL) {
        return false;
    }
    switch (channel) {
        case 1:
            *enSoundMode = OT_AUDIO_SOUND_MODE_MONO;
            break;
        case 2: /* 2: audio convert channel */
            *enSoundMode = OT_AUDIO_SOUND_MODE_STEREO;
            break;
        default:
            MEDIA_HAL_LOGW(MODULE_NAME, "Invalid channel: %d", channel);
            return false;
    }
    return true;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
