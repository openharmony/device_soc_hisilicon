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
#include "audio_adapter.h"

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

#define CONFIG_CHANNEL_COUNT  2 // two channels

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

static int32_t AudioAdapterInitPortCapability(const struct AudioHwAdapter *hwAdapter)
{
    CHK_NULL_RETURN(hwAdapter->capability, MEDIA_HAL_INVALID_PARAM, "Input param capability is null");
    CHK_NULL_RETURN(hwAdapter->adapterDescriptor.ports, MEDIA_HAL_INVALID_PARAM, "Input param ports is null");
    CHK_NULL_RETURN(hwAdapter->portPlugin, MEDIA_HAL_INVALID_PARAM, "Input param portPlugin is null");
    CHK_NULL_RETURN(hwAdapter->portPlugin->GetPortPluginCapability, MEDIA_HAL_INVALID_PARAM,
        "Input param GetPortPluginCapability is null");

    if (hwAdapter->adapterDescriptor.portNum > MAX_ADAPTER_PORTS_NUM) {
        return MEDIA_HAL_INVALID_PARAM;
    }

    int32_t ret;
    uint32_t deviceId = 0;
    struct AudioPort port = {};
    struct AudioPortCapability *capability = NULL;
    for (uint32_t portIndex = 0; portIndex < hwAdapter->adapterDescriptor.portNum; portIndex++) {
        if (hwAdapter->adapterDescriptor.ports[portIndex].dir == PORT_IN) {
            deviceId = hwAdapter->adapterDescriptor.ports[portIndex].portId;
            capability = &(hwAdapter->capability[portIndex]);
            port = hwAdapter->adapterDescriptor.ports[portIndex];
        } else if (hwAdapter->adapterDescriptor.ports[portIndex].dir == PORT_OUT) {
            deviceId = hwAdapter->adapterDescriptor.ports[portIndex].portId;
            capability = &(hwAdapter->capability[portIndex]);
            port = hwAdapter->adapterDescriptor.ports[portIndex];
        } else if (hwAdapter->adapterDescriptor.ports[portIndex].dir == PORT_OUT_IN) {
            deviceId = hwAdapter->adapterDescriptor.ports[portIndex].portId;
            capability = &(hwAdapter->capability[portIndex]);
            port = hwAdapter->adapterDescriptor.ports[portIndex];
        } else {
            MEDIA_HAL_LOGE(MODULE_NAME, "port index %u direction %d is invalid",
                portIndex, hwAdapter->adapterDescriptor.ports[portIndex].dir);
            return MEDIA_HAL_INVALID_PARAM;
        }
        ret = hwAdapter->portPlugin->GetPortPluginCapability(&port, capability);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "audio adapter get port capability failed");
            return ret;
        }
    }

    CHK_NULL_RETURN(capability, MEDIA_HAL_ERR, "audio adapter capability is null");

    capability->deviceId = deviceId;
    MEDIA_HAL_LOGD(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static int32_t AudioAdapterOpenPort(struct AudioHwAdapter *hwAdapter)
{
    MEDIA_HAL_LOCK(hwAdapter->mutex);
    if (hwAdapter->portOpened) {
        MEDIA_HAL_LOGE(MODULE_NAME, "port has opened");
        MEDIA_HAL_UNLOCK(hwAdapter->mutex);
        return MEDIA_HAL_OK;
    }

    int32_t ret = hwAdapter->portPlugin->Open(&hwAdapter->portHandle);
    if (ret != MEDIA_HAL_OK || hwAdapter->portHandle == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "portPlugin Open failed: 0x%x", ret);
        MEDIA_HAL_UNLOCK(hwAdapter->mutex);
        return ret;
    }
    hwAdapter->portOpened = true;
    MEDIA_HAL_UNLOCK(hwAdapter->mutex);

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

int32_t AudioAdapterClosePort(struct AudioAdapter *adapter)
{
    CHK_NULL_RETURN(adapter, MEDIA_HAL_INVALID_PARAM, "input param adapter null");

    struct AudioHwAdapter *hwAdapter = (struct AudioHwAdapter *)adapter;
    MEDIA_HAL_LOCK(hwAdapter->mutex);
    if (!hwAdapter->portOpened) {
        MEDIA_HAL_UNLOCK(hwAdapter->mutex);
        MEDIA_HAL_LOGE(MODULE_NAME, "port not opened");
        return MEDIA_HAL_ERR;
    }

    int32_t ret = hwAdapter->portPlugin->Close(hwAdapter->portHandle);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_UNLOCK(hwAdapter->mutex);
        MEDIA_HAL_LOGE(MODULE_NAME, "portPlugin Close failed: 0x%x", ret);
        return ret;
    }
    hwAdapter->portOpened = false;
    MEDIA_HAL_UNLOCK(hwAdapter->mutex);
    return MEDIA_HAL_OK;
}

int32_t AudioAdapterInitAllPorts(struct AudioAdapter *adapter)
{
    CHK_FAILED_RETURN(AudioManagerIsValidAdapter(adapter), true, MEDIA_HAL_INVALID_PARAM, "adapter is invalid");

    struct AudioHwAdapter *hwAdapter = (struct AudioHwAdapter *)adapter;

    int32_t ret = AudioAdapterInitPortCapability(hwAdapter);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioAdapterInitPortCapability failed");
        return MEDIA_HAL_ERR;
    }

    ret = AudioAdapterOpenPort(hwAdapter);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioAdapterOpenPort failed");
        return MEDIA_HAL_ERR;
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static int32_t AudioAdapterGetFreeChn(struct AudioAdapter *adapter, bool capture, uint32_t *chnId)
{
    CHK_NULL_RETURN(adapter, MEDIA_HAL_INVALID_PARAM, "input param adapter null");

    struct AudioHwAdapter *hwAdapter = (struct AudioHwAdapter *)adapter;
    MEDIA_HAL_LOCK(hwAdapter->mutex);
    if (capture) {
        for (uint32_t i = 0; i < MAX_CAPTURE_INSTANCE_NUM; i++) {
            if (!hwAdapter->capture[i].used) {
                hwAdapter->capture[i].used = true;
                hwAdapter->capture[i].chnId = i;
                MEDIA_HAL_UNLOCK(hwAdapter->mutex);
                *chnId = hwAdapter->capture[i].chnId;
                return MEDIA_HAL_OK;
            }
        }
    } else {
        for (uint32_t i = 0; i < MAX_RENDER_INSTANCE_NUM; i++) {
            if (!hwAdapter->render[i].used) {
                hwAdapter->render[i].used = true;
                hwAdapter->render[i].chnId = i;
                MEDIA_HAL_UNLOCK(hwAdapter->mutex);
                *chnId = hwAdapter->render[i].chnId;
                return MEDIA_HAL_OK;
            }
        }
    }
    MEDIA_HAL_UNLOCK(hwAdapter->mutex);
    MEDIA_HAL_LOGE(MODULE_NAME, "not get %s free channel", capture ? "capture" : "render");
    return MEDIA_HAL_ERR;
}

static int32_t AudioAdapterFreeChn(struct AudioAdapter *adapter, uint32_t chnId, bool capture)
{
    CHK_NULL_RETURN(adapter, MEDIA_HAL_INVALID_PARAM, "input param adapter null");

    struct AudioHwAdapter *hwAdapter = (struct AudioHwAdapter *)adapter;
    MEDIA_HAL_LOCK(hwAdapter->mutex);
    if (capture) {
        if (chnId >= MAX_CAPTURE_INSTANCE_NUM) {
            MEDIA_HAL_LOGE(MODULE_NAME, "Can't free capture invaild chnId: %u", chnId);
            MEDIA_HAL_UNLOCK(hwAdapter->mutex);
            return MEDIA_HAL_ERR;
        }
        hwAdapter->capture[chnId].used = false;
        hwAdapter->capture[chnId].chnId = 0xFFFFFFFF;
        MEDIA_HAL_UNLOCK(hwAdapter->mutex);
        return MEDIA_HAL_OK;
    } else {
        if (chnId >= MAX_RENDER_INSTANCE_NUM) {
            MEDIA_HAL_LOGE(MODULE_NAME, "Can't free render invaild chnId: %u", chnId);
            MEDIA_HAL_UNLOCK(hwAdapter->mutex);
            return MEDIA_HAL_ERR;
        }
        hwAdapter->render[chnId].used = false;
        hwAdapter->render[chnId].chnId = 0xFFFFFFFF;
        MEDIA_HAL_UNLOCK(hwAdapter->mutex);
        return MEDIA_HAL_OK;
    }
}


static int32_t CheckHdmiStatus(void)
{
#ifdef DRM_OPTION_ENABLE
    struct SmartDrmHdmiSinkStatus sinkStatus = {};
    int32_t fd;
    int32_t ret;

    fd = open(SMART_DRM_CARD_PATH, O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "open %s failed, errno:%d(%s)", SMART_DRM_CARD_PATH, errno, strerror(errno));
        return false;
    }

    ret = ioctl(fd, DRM_IOCTL_HI3403V100_HDMI_GET_SINK_STATUS, &sinkStatus);
    if (ret < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "DRM HDMI sink status ioctl failed, errno:%d(%s)", errno, strerror(errno));
        close(fd);
        return false;
    }
    close(fd);

    if (sinkStatus.connected) {
        MEDIA_HAL_LOGI(MODULE_NAME, "HDMI is connected by DRM");
        return true;
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "HDMI is not connected by DRM");
    return false;
#else
    ot_hdmi_sink_capability sinkCap = {0};
    int32_t ret = MEDIA_HAL_OK;

    ret = ss_mpi_hdmi_get_sink_capability(OT_HDMI_ID_0, &sinkCap);
    if (ret == MEDIA_HAL_OK) {
        if (sinkCap.is_connected) {
            MEDIA_HAL_LOGI(MODULE_NAME, "HDMI is connected");
            return true;
        } else {
            MEDIA_HAL_LOGI(MODULE_NAME, "HDMI is not connected");
            return false;
        }
    } else {
        MEDIA_HAL_LOGE(MODULE_NAME, "Failed to get HDMI sink capability, ret=%d", ret);
        return false;
    }
#endif
}

static void SetupRenderFunctionPointers(struct AudioHwRender *hwRender)
{
    hwRender->common.control.Start = AudioRenderStart;
    hwRender->common.control.Stop = AudioRenderStop;
    hwRender->common.control.Pause = AudioRenderPause;
    hwRender->common.control.Resume = AudioRenderResume;
    hwRender->common.control.Flush = AudioRenderFlush;
    hwRender->common.attr.GetFrameSize = AudioRenderGetFrameSize;
    hwRender->common.attr.GetFrameCount = AudioRenderGetFrameCount;
    hwRender->common.attr.SetSampleAttributes = AudioRenderSetSampleAttributes;
    hwRender->common.attr.GetSampleAttributes = AudioRenderGetSampleAttributes;
    hwRender->common.attr.GetCurrentChannelId = AudioRenderGetCurrentChannelId;
    hwRender->common.scene.CheckSceneCapability = AudioRenderCheckSceneCapability;
    hwRender->common.scene.SelectScene = AudioRenderSelectScene;
    hwRender->common.volume.SetMute = AudioRenderSetMute;
    hwRender->common.volume.GetMute = AudioRenderGetMute;
    hwRender->common.volume.SetVolume = AudioRenderSetVolume;
    hwRender->common.volume.GetVolume = AudioRenderGetVolume;
    hwRender->common.volume.GetGainThreshold = AudioRenderGetGainThreshold;
    hwRender->common.volume.GetGain = AudioRenderGetGain;
    hwRender->common.volume.SetGain = AudioRenderSetGain;
    hwRender->common.GetLatency = AudioRenderGetLatency;
    hwRender->common.RenderFrame = AudioRenderRenderFrame;
    hwRender->common.GetRenderPosition = AudioRenderGetRenderPosition;
    hwRender->common.SetRenderSpeed = AudioRenderSetRenderSpeed;
    hwRender->common.GetRenderSpeed = AudioRenderGetRenderSpeed;
    hwRender->common.SetChannelMode = AudioRenderSetChannelMode;
    hwRender->common.GetChannelMode = AudioRenderGetChannelMode;
}

static int32_t HwRenderInit(const struct AudioHwAdapter *hwAdapter, struct AudioHwRender *hwRender,
    uint32_t chnId, bool steroToMono)
{
    struct PortPluginAttr portAttr = {};
    portAttr.type = hwRender->attrs.type;
    portAttr.sampleRate = hwRender->attrs.sampleRate;
    portAttr.channelCount = hwRender->attrs.channelCount;
    portAttr.audioFormat = hwRender->attrs.format;

    if (hwRender->deviceDesc.pins == PIN_OUT_HDMI) {
        bool isConnected = CheckHdmiStatus();
        if (!isConnected) {
            MEDIA_HAL_LOGE(MODULE_NAME, "HDMI is not connected, can not init render");
            return MEDIA_HAL_ERR;
        }
    }

    int32_t ret = hwAdapter->portPlugin->CreateTrack(hwAdapter->portHandle, hwRender->deviceDesc.pins,
        &portAttr, (AudioHandle *)&hwRender->trackHandle);
    if (ret != MEDIA_HAL_OK || hwRender->trackHandle == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "portPlugin CreateTrack failed: 0x%x", ret);
        return ret;
    }

    SetupRenderFunctionPointers(hwRender);

    hwRender->started = false;
    hwRender->paused = false;
    hwRender->steroToMono = steroToMono;
    hwRender->chnId = chnId;
    pthread_mutex_init(&hwRender->mutex, NULL);
    return MEDIA_HAL_OK;
}

int32_t AudioAdapterCreateRender(struct AudioAdapter *adapter, const struct AudioDeviceDescriptor *desc,
    const struct AudioSampleAttributes *attrs, struct AudioRender **render)
{
    CHK_FAILED_RETURN(AudioManagerIsValidAdapter(adapter), true, MEDIA_HAL_INVALID_PARAM, "adapter is invalid");
    CHK_NULL_RETURN(desc, MEDIA_HAL_INVALID_PARAM, "input param desc null");
    CHK_NULL_RETURN(attrs, MEDIA_HAL_INVALID_PARAM, "input param attrs null");
    CHK_NULL_RETURN(render, MEDIA_HAL_INVALID_PARAM, "input param render null");

    int32_t ret;
    struct AudioHwAdapter *hwAdapter = (struct AudioHwAdapter *)adapter;

    uint32_t channelId = 0;
    ret = AudioAdapterGetFreeChn(adapter, false, &channelId);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioAdapterGetFreeChn failed");
        return MEDIA_HAL_NO_MEM;
    }
    struct AudioHwRender *hwRender = (struct AudioHwRender *)malloc(sizeof(struct AudioHwRender));
    if (hwRender == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc failed");
        ret = MEDIA_HAL_NO_MEM;
        goto FREE_CHN;
    }
    if (memset_s(hwRender, sizeof(struct AudioHwRender), 0, sizeof(struct AudioHwRender)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }

    if (memcpy_s(&hwRender->deviceDesc, sizeof(struct AudioDeviceDescriptor),
        desc, sizeof(struct AudioDeviceDescriptor)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memcpy_s hwRender->deviceDesc failed");
    }

    if (memcpy_s(&hwRender->attrs, sizeof(struct AudioSampleAttributes),
        attrs, sizeof(struct AudioSampleAttributes)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memcpy_s hwRender->attrs failed");
    }

    bool steroToMono = false;
    /* plguin support transform stero to mono  */
    if (hwRender->attrs.channelCount == CONFIG_CHANNEL_COUNT && hwAdapter->capability->channelCount == 1) {
        hwRender->attrs.channelCount = hwAdapter->capability->channelCount;
        steroToMono = true;
    }
    ret = HwRenderInit(hwAdapter, hwRender, channelId, steroToMono);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "InitHwRender failed");
        goto FREE_HWRENDER;
    }
    *render = &hwRender->common;
    return MEDIA_HAL_OK;

FREE_HWRENDER:
    free(hwRender);
FREE_CHN:
    (void)AudioAdapterFreeChn(adapter, channelId, false);
    return ret;
}

int32_t AudioAdapterDestroyRender(struct AudioAdapter *adapter, struct AudioRender *render)
{
    CHK_FAILED_RETURN(AudioManagerIsValidAdapter(adapter), true, MEDIA_HAL_INVALID_PARAM, "adapter null");
    CHK_NULL_RETURN(render, MEDIA_HAL_INVALID_PARAM, "input param render null");

    struct AudioHwAdapter *hwAdapter = (struct AudioHwAdapter *)adapter;
    struct AudioHwRender *hwRender = (struct AudioHwRender *)render;
    int32_t ret = AudioAdapterFreeChn(adapter, hwRender->chnId, false);
    if (ret == -1) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioAdapterFreeChn failed");
        return MEDIA_HAL_ERR;
    }
    if (hwRender->started) {
        (void)AudioRenderStop(render);
    }

    ret = hwAdapter->portPlugin->DestroyTrack(hwAdapter->portHandle, (AudioHandle)hwRender->trackHandle);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "portPlugin DestroyTrack failed: 0x%x", ret);
    }
    hwRender->trackHandle = NULL;
    pthread_mutex_destroy(&hwRender->mutex);
    free(hwRender);
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static int32_t HwCaptureInit(const struct AudioHwAdapter *hwAdapter,
    uint32_t chnId, struct AudioHwCapture *hwCapture)
{
    struct PortPluginAttr portAttr = {};
    portAttr.type = hwCapture->attrs.type;
    portAttr.sampleRate   = hwCapture->attrs.sampleRate;
    portAttr.channelCount = hwCapture->attrs.channelCount;
    portAttr.audioFormat  = hwCapture->attrs.format;

    int32_t ret = hwAdapter->portPlugin->CreateTrack(hwAdapter->portHandle, hwCapture->deviceDesc.pins,
        &portAttr, (AudioHandle *)&hwCapture->trackHandle);
    if (ret != MEDIA_HAL_OK || hwCapture->trackHandle == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "portPlugin CreateTrack failed: 0x%x", ret);
        return ret;
    }

    hwCapture->common.control.Start = AudioCaptureStart;
    hwCapture->common.control.Stop = AudioCaptureStop;
    hwCapture->common.control.Pause = AudioCapturePause;
    hwCapture->common.control.Resume = AudioCaptureResume;
    hwCapture->common.control.Flush = AudioCaptureFlush;
    hwCapture->common.attr.GetFrameSize = AudioCaptureGetFrameSize;
    hwCapture->common.attr.GetFrameCount = AudioCaptureGetFrameCount;
    hwCapture->common.attr.SetSampleAttributes = AudioCaptureSetSampleAttributes;
    hwCapture->common.attr.GetSampleAttributes = AudioCaptureGetSampleAttributes;
    hwCapture->common.attr.GetCurrentChannelId = AudioCaptureGetCurrentChannelId;
    hwCapture->common.scene.CheckSceneCapability = AudioCaptureCheckSceneCapability;
    hwCapture->common.scene.SelectScene = AudioCaptureSelectScene;
    hwCapture->common.volume.SetMute = AudioCaptureSetMute;
    hwCapture->common.volume.GetMute = AudioCaptureGetMute;
    hwCapture->common.volume.SetVolume = AudioCaptureSetVolume;
    hwCapture->common.volume.GetVolume = AudioCaptureGetVolume;
    hwCapture->common.volume.GetGainThreshold = AudioCaptureGetGainThreshold;
    hwCapture->common.volume.GetGain = AudioCaptureGetGain;
    hwCapture->common.volume.SetGain = AudioCaptureSetGain;
    hwCapture->common.CaptureFrame = AudioCaptureCaptureFrame;
    hwCapture->common.GetCapturePosition = AudioCaptureGetCapturePosition;
    hwCapture->chnId = chnId;
    pthread_mutex_init(&hwCapture->mutex, NULL);
    return MEDIA_HAL_OK;
}

int32_t AudioAdapterCreateCapture(struct AudioAdapter *adapter, const struct AudioDeviceDescriptor *desc,
    const struct AudioSampleAttributes *attrs, struct AudioCapture **capture)
{
    CHK_FAILED_RETURN(AudioManagerIsValidAdapter(adapter), true, MEDIA_HAL_INVALID_PARAM, "adapter null");
    CHK_NULL_RETURN(desc, MEDIA_HAL_INVALID_PARAM, "input param desc null");
    CHK_NULL_RETURN(attrs, MEDIA_HAL_INVALID_PARAM, "input param attrs null");
    CHK_NULL_RETURN(capture, MEDIA_HAL_INVALID_PARAM, "input param capture null");

    int32_t ret;
    struct AudioHwAdapter *hwAdapter = (struct AudioHwAdapter *)adapter;

    uint32_t channelId = 0;
    ret = AudioAdapterGetFreeChn(adapter, true, &channelId);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioAdapterGetFreeChn failed");
        return MEDIA_HAL_NO_MEM;
    }
    struct AudioHwCapture *hwCapture = (struct AudioHwCapture *)malloc(sizeof(struct AudioHwCapture));
    if (hwCapture == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc AudioHwCapture failed");
        ret = MEDIA_HAL_NO_MEM;
        goto FREE_CHN;
    }
    if (memset_s(hwCapture, sizeof(struct AudioHwCapture), 0, sizeof(struct AudioHwCapture)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s AudioHwCapture failed");
    }
    if (memcpy_s(&hwCapture->deviceDesc, sizeof(struct AudioDeviceDescriptor),
        desc, sizeof(struct AudioDeviceDescriptor)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memcpy_s hwCapture->deviceDesc failed");
    }
    if (memcpy_s(&hwCapture->attrs, sizeof(struct AudioSampleAttributes),
        attrs, sizeof(struct AudioSampleAttributes)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memcpy_s hwCapture->attrs failed");
    }
    ret = HwCaptureInit(hwAdapter, channelId, hwCapture);
    if (ret != MEDIA_HAL_OK || hwCapture->trackHandle == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "InitHwCapture failed");
        goto FREE_HWCAPTURE;
    }
    *capture = &hwCapture->common;
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
FREE_HWCAPTURE:
    free(hwCapture);
FREE_CHN:
    (void)AudioAdapterFreeChn(adapter, channelId, true);
    return ret;
}

int32_t AudioAdapterDestroyCapture(struct AudioAdapter *adapter, struct AudioCapture *capture)
{
    CHK_FAILED_RETURN(AudioManagerIsValidAdapter(adapter), true, MEDIA_HAL_INVALID_PARAM, "adapter null");
    CHK_NULL_RETURN(capture, MEDIA_HAL_INVALID_PARAM, "input param capture null");

    struct AudioHwAdapter *hwAdapter = (struct AudioHwAdapter *)adapter;
    struct AudioHwCapture *hwCapture = (struct AudioHwCapture *)capture;
    int32_t ret = AudioAdapterFreeChn(adapter, hwCapture->chnId, true);
    if (ret == -1) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioAdapterFreeChn failed");
        return MEDIA_HAL_INVALID_PARAM;
    }
    if (hwCapture->started) {
        (void)AudioCaptureStop(hwCapture);
    }
    ret = hwAdapter->portPlugin->DestroyTrack(hwAdapter->portHandle, hwCapture->trackHandle);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "portPlugin DestroyTrack failed: 0x%x", ret);
    }
    hwCapture->trackHandle = NULL;
    pthread_mutex_destroy(&hwCapture->mutex);
    free(hwCapture);
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

int32_t AudioAdapterGetPortCapability(struct AudioAdapter *adapter, const struct AudioPort *port,
    struct AudioPortCapability *capability)
{
    CHK_FAILED_RETURN(AudioManagerIsValidAdapter(adapter), true, MEDIA_HAL_INVALID_PARAM, "adapter null");
    CHK_NULL_RETURN(port, MEDIA_HAL_INVALID_PARAM, "input param port null");
    CHK_NULL_RETURN(port->portName, MEDIA_HAL_INVALID_PARAM, "input param portName null");
    CHK_NULL_RETURN(capability, MEDIA_HAL_INVALID_PARAM, "input param capability null");

    if (port->dir != PORT_OUT || port->portId != 0 || strcmp(port->portName, "AOP") != 0) {
        return MEDIA_HAL_INVALID_PARAM;
    }
    capability->hardwareMode = true;
    capability->channelMasks = AUDIO_CHANNEL_STEREO;
    capability->channelCount = CONFIG_CHANNEL_COUNT;
    return MEDIA_HAL_OK;
}

int32_t AudioAdapterSetPassthroughMode(struct AudioAdapter *adapter, const struct AudioPort *port,
    enum AudioPortPassthroughMode mode)
{
    CHK_FAILED_RETURN(AudioManagerIsValidAdapter(adapter), true, MEDIA_HAL_INVALID_PARAM, "adapter null");
    CHK_NULL_RETURN(port, MEDIA_HAL_INVALID_PARAM, "input param port null");
    CHK_NULL_RETURN(port->portName, MEDIA_HAL_INVALID_PARAM, "input param portName null");

    if (port->dir != PORT_OUT || port->portId != 0 || strcmp(port->portName, "AOP") != 0) {
        return MEDIA_HAL_INVALID_PARAM;
    }
    struct AudioHwAdapter *hwAdapter = (struct AudioHwAdapter *)adapter;
    hwAdapter->mode = mode;
    return MEDIA_HAL_OK;
}

int32_t AudioAdapterGetPassthroughMode(struct AudioAdapter *adapter, const struct AudioPort *port,
    enum AudioPortPassthroughMode *mode)
{
    CHK_FAILED_RETURN(AudioManagerIsValidAdapter(adapter), true, MEDIA_HAL_INVALID_PARAM, "adapter null");
    CHK_NULL_RETURN(port, MEDIA_HAL_INVALID_PARAM, "input param port null");
    CHK_NULL_RETURN(port->portName, MEDIA_HAL_INVALID_PARAM, "input param portName null");
    CHK_NULL_RETURN(mode, MEDIA_HAL_INVALID_PARAM, "input param mode null");

    if (port->dir != PORT_OUT || port->portId != 0 || strcmp(port->portName, "AOP") != 0) {
        return MEDIA_HAL_INVALID_PARAM;
    }

    struct AudioHwAdapter *hwAdapter = (struct AudioHwAdapter *)adapter;
    *mode = hwAdapter->mode;
    return MEDIA_HAL_OK;
}
