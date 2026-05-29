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
#include <math.h>
#ifdef DRM_OPTION_ENABLE
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#endif
#include "audio_internal.h"
#include "audio_port_plugin.h"

#ifndef DRM_OPTION_ENABLE
#define ENABLE_H8
#if defined(ENABLE_H8)
#include "ot_common_hdmi.h"
#include "ss_mpi_hdmi.h"
#else
#include "hi_comm_hdmi.h"
#include "mpi_hdmi.h"
#endif
#endif

#define CONFIG_OUT_LATENCY_MS  100 // unit: ms
#define CONFIG_HDMI_OUT_CONNECT  200
#define CONFIG_HDMI_OUT_DISCONNECT  201
#ifdef DRM_OPTION_ENABLE
#define DRM_IOCTL_BASE 'd'
#define DRM_COMMAND_BASE 0x40
#define DRM_HI3403V100_HDMI_GET_SINK_STATUS 3
#define SMART_DRM_CARD_PATH "/dev/dri/card0"

struct SmartDrmHdmiSinkStatus {
    uint32_t connected;
    uint32_t sinkHasAudio;
    uint32_t connectorStatus;
    uint32_t reserved;
};

#define DRM_IOCTL_HI3403V100_HDMI_GET_SINK_STATUS \
    _IOWR(DRM_IOCTL_BASE, DRM_COMMAND_BASE + DRM_HI3403V100_HDMI_GET_SINK_STATUS, struct SmartDrmHdmiSinkStatus)
#endif

// 1 buffer: 8000(8kHz sample rate) * 2(bytes, PCM_16_BIT) * 1(channel)
// 1 frame: 1024(sample) * 2(bytes, PCM_16_BIT) * 1(channel)
#define CONFIG_FRAME_SIZE      (1024 * 2 * 1)
#define CONFIG_FRAME_COUNT     ((8000 * 2 * 1 + (CONFIG_FRAME_SIZE - 1)) / CONFIG_FRAME_SIZE)
#define STERO_TO_MONO_FACTOR 2

int32_t AudioRenderStart(AudioHandle handle)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    if (hwRender->started) {
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderStart, have started");
        return MEDIA_HAL_OK;
    }
    int32_t ret = hwRender->trackHandle->EnableTrack(hwRender->trackHandle);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPortTrack EnableTrack failed: 0x%x", ret);
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return ret;
    }
    ret = hwRender->trackHandle->SetVolume(hwRender->trackHandle, AUDIO_DEFAULT_VOLUME);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPortTrack SetVolume failed: 0x%x", ret);
    }
    hwRender->started = true;
    hwRender->paused = false;
#ifdef ENABALE_VQE
    if (hwRender->attrs.type != AUDIO_IN_MEDIA) {
        MEDIA_HAL_LOGI(MODULE_NAME, "unsupported audio category: %d", hwRender->attrs.type);
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return MEDIA_HAL_OK;
    }
    struct InvokeAttr invokeAttr = {};
    enum VqeScene vqeScene = VQE_SCENE_AO_MUSIC; // The playback scenario needs to be differentiated.
    invokeAttr.request = &vqeScene;
    ret = hwRender->trackHandle->Invoke(hwRender->trackHandle, INVOKE_ID_CONFIG_VQE, &invokeAttr);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPortTrack Invoke INVOKE_ID_CONFIG_VQE failed: 0x%x", ret);
    }
#endif
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderStop(AudioHandle handle)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    if (!hwRender->started) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderStop, not started");
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return MEDIA_HAL_ERR;
    }
    int32_t ret = hwRender->trackHandle->DisableTrack(hwRender->trackHandle);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPortTrack DisableTrack failed 0x%x", ret);
    }
    hwRender->started = false;
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return ret;
}

int32_t AudioRenderPause(AudioHandle handle)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    if (!hwRender->started) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderPause, not started");
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return MEDIA_HAL_ERR;
    }
    if (hwRender->paused) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderPause, current paused");
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return MEDIA_HAL_OK;
    }
    int32_t ret = hwRender->trackHandle->PauseTrack(hwRender->trackHandle);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPortTrack PuaseTrack failed 0x%x", ret);
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return ret;
    }
    hwRender->paused = true;
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderResume(AudioHandle handle)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    if (!hwRender->started) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderResume, not started");
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return MEDIA_HAL_ERR;
    }
    if (!hwRender->paused) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderPause, current in play");
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return MEDIA_HAL_OK;
    }
    int32_t ret = hwRender->trackHandle->ResumeTrack(hwRender->trackHandle);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPortTrack ResumeTrack failed 0x%x", ret);
    }
    hwRender->paused = false;
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderFlush(AudioHandle handle)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    if (!hwRender->started) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderFlush, not started");
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return MEDIA_HAL_ERR;
    }
    int32_t ret = hwRender->trackHandle->FlushTrack(hwRender->trackHandle);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPortTrack FlushTrack failed 0x%x", ret);
    }
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderGetFrameSize(AudioHandle handle, uint64_t *size)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");
    CHK_NULL_RETURN(size, MEDIA_HAL_INVALID_PARAM, "input param size null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    *size = CONFIG_FRAME_SIZE;
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderGetFrameCount(AudioHandle handle, uint64_t *count)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");
    CHK_NULL_RETURN(count, MEDIA_HAL_INVALID_PARAM, "input param count null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    *count = CONFIG_FRAME_COUNT;
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderSetSampleAttributes(AudioHandle handle, const struct AudioSampleAttributes *attrs)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");
    CHK_NULL_RETURN(attrs, MEDIA_HAL_INVALID_PARAM, "input param attrs null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    hwRender->attrs.format = attrs->format;
    hwRender->attrs.sampleRate = attrs->sampleRate;
    hwRender->attrs.channelCount = attrs->channelCount;
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderGetSampleAttributes(AudioHandle handle, struct AudioSampleAttributes *attrs)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");
    CHK_NULL_RETURN(attrs, MEDIA_HAL_INVALID_PARAM, "input param attrs null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    attrs->format = hwRender->attrs.format;
    attrs->sampleRate = hwRender->attrs.sampleRate;
    attrs->channelCount = hwRender->attrs.channelCount;
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderGetCurrentChannelId(AudioHandle handle, uint32_t *channelId)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");
    CHK_NULL_RETURN(channelId, MEDIA_HAL_INVALID_PARAM, "input param channelId null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderCheckSceneCapability(AudioHandle handle, const struct AudioSceneDescriptor *scene,
    bool *supported)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");
    CHK_NULL_RETURN(scene, MEDIA_HAL_INVALID_PARAM, "input param scene null");
    CHK_NULL_RETURN(supported, MEDIA_HAL_INVALID_PARAM, "input param supported null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    *supported = false;
    if (scene->desc.portId == 0 && scene->desc.pins == PIN_OUT_SPEAKER) {
        *supported = true;
    }
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderSelectScene(AudioHandle handle, const struct AudioSceneDescriptor *scene)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");
    CHK_NULL_RETURN(scene, MEDIA_HAL_INVALID_PARAM, "input param scene null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    if (scene->desc.portId != 0 || scene->desc.pins != PIN_OUT_SPEAKER) {
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return MEDIA_HAL_INVALID_PARAM;
    }
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    MEDIA_HAL_LOGV(MODULE_NAME, "select scene %d/%d success", scene->desc.portId, scene->desc.pins);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderSetMute(AudioHandle handle, bool mute)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    int32_t ret = hwRender->trackHandle->SetMute(hwRender->trackHandle, mute);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPortTrack SetMute failed: 0x%x", ret);
    }

    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return ret;
}

int32_t AudioRenderGetMute(AudioHandle handle, bool *mute)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");
    CHK_NULL_RETURN(mute, MEDIA_HAL_INVALID_PARAM, "input param mute null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    int32_t ret = hwRender->trackHandle->GetMute(hwRender->trackHandle, mute);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPortTrack GetMute failed: 0x%x", ret);
    }

    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderSetVolume(AudioHandle handle, float volume)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    if (!(hwRender->started)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderSetVolume, not started");
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return MEDIA_HAL_ERR;
    }
    int32_t ret = hwRender->trackHandle->SetVolume(hwRender->trackHandle, volume);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPortTrack SetVolume failed: 0x%x", ret);
    }

    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return ret;
}

int32_t AudioRenderGetVolume(AudioHandle handle, float *volume)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");
    CHK_NULL_RETURN(volume, MEDIA_HAL_INVALID_PARAM, "input param volume null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    int32_t ret = hwRender->trackHandle->GetVolume(hwRender->trackHandle, volume);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPortTrack GetVolume failed: 0x%x", ret);
    }

    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return ret;
}

int32_t AudioRenderGetGainThreshold(AudioHandle handle, float *min, float *max)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");
    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    UNUSED(min);
    UNUSED(max);
    MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderGetGainThreshold not support now");
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderGetGain(AudioHandle handle, float *gain)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");
    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    UNUSED(gain);
    MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderGetGain not support now");
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderSetGain(AudioHandle handle, float gain)
{
    CHK_NULL_RETURN(handle, MEDIA_HAL_INVALID_PARAM, "input param handle null");
    struct AudioHwRender *hwRender = (struct AudioHwRender *)handle;
    MEDIA_HAL_LOCK(hwRender->mutex);
    UNUSED(gain);
    MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderSetGain not support now");
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderGetLatency(struct AudioRender *render, uint32_t *ms)
{
    CHK_NULL_RETURN(render, MEDIA_HAL_INVALID_PARAM, "input param render null");
    CHK_NULL_RETURN(ms, MEDIA_HAL_INVALID_PARAM, "input param ms null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)render;
    MEDIA_HAL_LOCK(hwRender->mutex);
    *ms = CONFIG_OUT_LATENCY_MS;
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderRenderFrame(struct AudioRender *render, const void *frame,
    uint64_t requestBytes, uint64_t *replyBytes)
{
    CHK_NULL_RETURN(render, MEDIA_HAL_INVALID_PARAM, "input param render null");
    CHK_NULL_RETURN(frame, MEDIA_HAL_INVALID_PARAM, "input param frame null");
    CHK_NULL_RETURN(replyBytes, MEDIA_HAL_INVALID_PARAM, "input param replyBytes null");

    int32_t ret;
    struct AudioHwRender *hwRender = (struct AudioHwRender *)render;
    MEDIA_HAL_LOCK(hwRender->mutex);
    if (!hwRender->started) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderRenderFrame, not started");
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return MEDIA_HAL_ERR;
    }
    if (hwRender->steroToMono) {
        uint64_t monoDataLen = requestBytes / STERO_TO_MONO_FACTOR;
        uint64_t monoReplyBytes = 0;
        ret = hwRender->trackHandle->SendFrame(hwRender->trackHandle, (const uint8_t *)frame, monoDataLen,
            &monoReplyBytes);
        *replyBytes = (monoDataLen == monoReplyBytes) ? requestBytes : 0;
    } else {
        ret = hwRender->trackHandle->SendFrame(hwRender->trackHandle, (const uint8_t *)frame, requestBytes, replyBytes);
    }
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return ret;
}

int32_t AudioRenderGetRenderPosition(struct AudioRender *render, uint64_t *frames, struct AudioTimeStamp *time)
{
    CHK_NULL_RETURN(render, MEDIA_HAL_INVALID_PARAM, "input param render null");
    CHK_NULL_RETURN(frames, MEDIA_HAL_INVALID_PARAM, "input param frames null");
    CHK_NULL_RETURN(time, MEDIA_HAL_INVALID_PARAM, "input param time null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)render;
    MEDIA_HAL_LOCK(hwRender->mutex);
    if (!hwRender->started) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioRenderGetRenderPosition, not started");
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return MEDIA_HAL_ERR;
    }
    int32_t ret = hwRender->trackHandle->GetPosition(hwRender->trackHandle, frames, time);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPortTrack GetPosition failed: 0x%x", ret);
    }
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return ret;
}

int32_t AudioRenderSetRenderSpeed(struct AudioRender *render, float speed)
{
    CHK_NULL_RETURN(render, MEDIA_HAL_INVALID_PARAM, "input param render null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)render;
    MEDIA_HAL_LOCK(hwRender->mutex);
    hwRender->speed = speed;
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderGetRenderSpeed(struct AudioRender *render, float *speed)
{
    CHK_NULL_RETURN(render, MEDIA_HAL_INVALID_PARAM, "input param render null");
    CHK_NULL_RETURN(speed, MEDIA_HAL_INVALID_PARAM, "input param speed null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)render;
    MEDIA_HAL_LOCK(hwRender->mutex);
    *speed = hwRender->speed;

#ifdef DRM_OPTION_ENABLE
    struct SmartDrmHdmiSinkStatus sinkStatus = {};
    int32_t fd = open(SMART_DRM_CARD_PATH, O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "open %s failed, errno:%d(%s)", SMART_DRM_CARD_PATH, errno, strerror(errno));
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return MEDIA_HAL_OK;
    }

    int32_t ret = ioctl(fd, DRM_IOCTL_HI3403V100_HDMI_GET_SINK_STATUS, &sinkStatus);
    if (ret < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "DRM HDMI sink status ioctl failed, errno:%d(%s)", errno, strerror(errno));
        close(fd);
        MEDIA_HAL_UNLOCK(hwRender->mutex);
        return MEDIA_HAL_OK;
    }
    close(fd);

    if (sinkStatus.connected) {
        MEDIA_HAL_LOGI(MODULE_NAME, "HDMI is connected");
        *speed = CONFIG_HDMI_OUT_CONNECT;
    } else {
        MEDIA_HAL_LOGI(MODULE_NAME, "HDMI is not connected");
        *speed = CONFIG_HDMI_OUT_DISCONNECT;
    }
#else
    ot_hdmi_sink_capability sinkCap = {0};
    int32_t ret = MEDIA_HAL_OK;
    MEDIA_HAL_LOGI(MODULE_NAME, "ss_mpi_hdmi_get_sink_capability enter");

    ret = ss_mpi_hdmi_get_sink_capability(OT_HDMI_ID_0, &sinkCap);
    if (ret == MEDIA_HAL_OK) {
        if (sinkCap.is_connected) {
            MEDIA_HAL_LOGI(MODULE_NAME, "HDMI is connected");
            *speed = CONFIG_HDMI_OUT_CONNECT;
        } else {
            MEDIA_HAL_LOGI(MODULE_NAME, "HDMI is not connected");
            *speed = CONFIG_HDMI_OUT_DISCONNECT;
        }
    }
#endif

    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderSetChannelMode(struct AudioRender *render, enum AudioChannelMode mode)
{
    CHK_NULL_RETURN(render, MEDIA_HAL_INVALID_PARAM, "input param render null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)render;
    MEDIA_HAL_LOCK(hwRender->mutex);
    hwRender->mode = mode;
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioRenderGetChannelMode(struct AudioRender *render, enum AudioChannelMode *mode)
{
    CHK_NULL_RETURN(render, MEDIA_HAL_INVALID_PARAM, "input param render null");
    CHK_NULL_RETURN(mode, MEDIA_HAL_INVALID_PARAM, "input param mode null");

    struct AudioHwRender *hwRender = (struct AudioHwRender *)render;
    MEDIA_HAL_LOCK(hwRender->mutex);
    *mode = hwRender->mode;
    MEDIA_HAL_UNLOCK(hwRender->mutex);
    return MEDIA_HAL_OK;
}
