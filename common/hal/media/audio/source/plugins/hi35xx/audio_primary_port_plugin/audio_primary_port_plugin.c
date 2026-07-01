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
 * Primary port plugin
 */

#include "audio_primary_port_plugin.h"

#include <stdlib.h>
#include "securec.h"

#include "audio_primary_sub_port_in.h"
#include "audio_primary_sub_port_out.h"

#include "media_hal_common.h"
#include "audio_port_plugin_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MODULE_NAME "AudioPrimaryPortPlugin"

typedef struct {
    /* sub port track context */
    AudioHandle subPortTrackHandle;
} PrimaryPortTrackContext;

typedef struct {
    /* port context */
    bool isOpened;
    /* sub port context */
    AudioHandle subPortInHandle;
    AudioHandle subPortOutHandle;
} PrimaryPortContext;

static int32_t PrimaryPortGetPortPluginCapability(const struct AudioPort *port,
    struct AudioPortCapability *capability)
{
    CHK_NULL_RETURN(port, MEDIA_HAL_INVALID_PARAM, "Input param port is null");
    CHK_NULL_RETURN(capability, MEDIA_HAL_INVALID_PARAM, "Input param capability is null");

    int32_t ret;
    if (port->dir == PORT_IN) {
        ret = AudioPrimarySubPortInGetCapability(port, capability);
        CHK_FAILED_RETURN(ret, MEDIA_HAL_OK, ret, "AudioPrimarySubPortInGetCapability failed");
    } else if (port->dir == PORT_OUT) {
        ret = AudioPrimarySubPortOutGetCapability(port, capability);
        CHK_FAILED_RETURN(ret, MEDIA_HAL_OK, ret, "AudioPrimarySubPortOutGetCapability failed");
    } else {
        MEDIA_HAL_LOGE(MODULE_NAME, "port direction: 0x%x is invalid", port->dir);
        return MEDIA_HAL_INVALID_PARAM;
    }

    MEDIA_HAL_LOGD(MODULE_NAME, "%s get capability success", port->portName);
    return MEDIA_HAL_OK;
}

static int32_t PrimaryPortOpen(AudioHandle *portHandle)
{
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "Input param portHandle is null");

    PrimaryPortContext *priPortCtx = (PrimaryPortContext *)malloc(sizeof(PrimaryPortContext));
    if (priPortCtx == NULL) {
        MEDIA_HAL_LOGI(MODULE_NAME, "malloc PrimaryPortContext failed");
        return MEDIA_HAL_NO_MEM;
    }
    if (memset_s(priPortCtx, sizeof(PrimaryPortContext), 0, sizeof(PrimaryPortContext)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s PrimaryPortContext failed");
        free(priPortCtx);
        return MEDIA_HAL_ERR;
    }
    int32_t ret = AudioPrimarySubPortInOpen(&priPortCtx->subPortInHandle);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPrimarySubPortInOpen failed ret 0x%x", ret);
        free(priPortCtx);
        return ret;
    }
    ret = AudioPrimarySubPortOutOpen(&priPortCtx->subPortOutHandle);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AudioPrimarySubPortOutOpen failed ret 0x%x", ret);
        free(priPortCtx);
        return ret;
    }
    priPortCtx->isOpened = true;
    *portHandle = priPortCtx;
    MEDIA_HAL_LOGD(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static bool AudioPluginIsValidPin(enum AudioPortPin pin, const char *pluginName)
{
    if (strcmp("primary", pluginName) == 0) {
        if (pin == PIN_IN_MIC || pin == PIN_OUT_SPEAKER || pin == PIN_OUT_HDMI) {
            return true;
        }
    }

    return false;
}

static int32_t PrimaryPortCreateTrack(AudioHandle portHandle, enum AudioPortPin audioPortPin,
    struct PortPluginAttr *portAttr, AudioHandle *trackHandle)
{
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "Input param portHandle is null");
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "Input param trackHandle is null");
    CHK_NULL_RETURN(portAttr, MEDIA_HAL_INVALID_PARAM, "Input param portAttr is null");

    if (!AudioPluginIsValidPin(audioPortPin, "primary")) {
        MEDIA_HAL_LOGE(MODULE_NAME, "pin 0x%x is invalid", audioPortPin);
        return MEDIA_HAL_INVALID_PARAM;
    }

    PrimaryPortContext *priPortCtx = (PrimaryPortContext *)portHandle;
    CHK_FAILED_RETURN(priPortCtx->isOpened, true, MEDIA_HAL_ERR, "port not open");

    int32_t ret;
    MEDIA_HAL_LOGI(MODULE_NAME, "audioPortPin=%d", audioPortPin);
    if (audioPortPin == PIN_IN_MIC) {
        ret = AudioPrimarySubPortInCreateTrack(priPortCtx->subPortInHandle,
                                               trackHandle, portAttr, audioPortPin);
        CHK_FAILED_RETURN(ret, MEDIA_HAL_OK, ret, "AudioPrimarySubPortInCreateTrack failed");
    } else {
        ret = AudioPrimarySubPortOutCreateTrack(priPortCtx->subPortOutHandle,
                                                trackHandle, portAttr, audioPortPin);
        CHK_FAILED_RETURN(ret, MEDIA_HAL_OK, ret, "AudioPrimarySubPortOutCreateTrack failed");
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static int32_t PrimaryPortDestroyTrack(AudioHandle portHandle, AudioHandle trackHandle)
{
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "Input param portHandle is null");
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "Input param trackHandle is null");

    PrimaryPortContext *priPortCtx = (PrimaryPortContext *)portHandle;
    CHK_FAILED_RETURN(priPortCtx->isOpened, true, MEDIA_HAL_ERR, "port not open");

    int32_t ret;
    AudioPortTrack *track = (AudioPortTrack *)trackHandle;
    if (track->audioPin == PIN_IN_MIC) {
        ret = AudioPrimarySubPortInDestroyTrack(priPortCtx->subPortInHandle, trackHandle);
        CHK_FAILED_RETURN(ret, MEDIA_HAL_OK, ret, "AudioPrimarySubPortInDestroyTrack failed");
    } else {
        ret = AudioPrimarySubPortOutDestroyTrack(priPortCtx->subPortOutHandle, trackHandle);
        CHK_FAILED_RETURN(ret, MEDIA_HAL_OK, ret, "AudioPrimarySubPortOutDestroyTrack failed");
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static int32_t PrimaryPortClose(AudioHandle portHandle)
{
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "Input param portHandle is null");
    PrimaryPortContext *priPortCtx = (PrimaryPortContext *)portHandle;
    CHK_FAILED_RETURN(priPortCtx->isOpened, true, MEDIA_HAL_ERR, "port not open");

    int32_t ret = AudioPrimarySubPortInClose(priPortCtx->subPortInHandle);
    CHK_FAILED_NO_RETURN(ret, MEDIA_HAL_OK, "AudioPrimarySubPortInClose failed");
    ret = AudioPrimarySubPortOutClose(priPortCtx->subPortOutHandle);
    CHK_FAILED_NO_RETURN(ret, MEDIA_HAL_OK, "AudioPrimarySubPortOutClose failed");

    priPortCtx->isOpened = false;
    free(priPortCtx);
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static AudioPortPlugin g_primaryPortPlugin = {
    .audioPortDir = PORT_OUT_IN,
    .audioPortPins = PIN_IN_MIC | PIN_OUT_SPEAKER,
    .GetPortPluginCapability = PrimaryPortGetPortPluginCapability,
    .Open = PrimaryPortOpen,
    .CreateTrack = PrimaryPortCreateTrack,
    .DestroyTrack = PrimaryPortDestroyTrack,
    .Close = PrimaryPortClose,
    .SetMute = NULL,
    .GetMute = NULL,
    .SetVolume = NULL,
    .GetVolume = NULL,
};

const AudioPortPlugin *GetPortPluginFuncs(AudioPluginType pluginType)
{
    if (pluginType == AUDIO_PLUGIN_BLUETOOTH) {
        return NULL;
    }
    return &g_primaryPortPlugin;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
