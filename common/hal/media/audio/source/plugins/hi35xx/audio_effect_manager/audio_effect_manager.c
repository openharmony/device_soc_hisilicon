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
 * Audio effect manager
 */

#include "securec.h"

#include "audio_primary_sub_port_in.h"
#include "audio_primary_sub_port_out.h"

#include "media_hal_common.h"

#include "audio_effect_manager.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MODULE_NAME "AudioEffectManager"

#define CUR_PARAM_MAX_SIZE 50

typedef struct {
    bool enable;
    int32_t deviceId;
    int32_t channelId;
    AudioPortTrack *trackHandle;
    enum AudioPortDirection dir;
} AudioPortInfo;

/* input and output port */
static AudioPortInfo g_audioPortInfos[] = {
    {
        .enable = false,
        .deviceId = -1,
        .channelId = -1,
        .trackHandle = NULL,
        .dir = PORT_IN,
    },
    {
        .enable = false,
        .deviceId = -1,
        .channelId = -1,
        .trackHandle = NULL,
        .dir = PORT_OUT,
    },
};

static bool BothEnable(void)
{
    for (uint32_t i = 0; i < ARRAY_SIZE(g_audioPortInfos); i++) {
        if (!g_audioPortInfos[i].enable) {
            return false;
        }
    }

    return true;
}

static bool BothDisable(void)
{
    for (uint32_t i = 0; i < ARRAY_SIZE(g_audioPortInfos); i++) {
        if (g_audioPortInfos[i].enable) {
            return false;
        }
    }

    return true;
}

static void RecordPortInfo(AudioHandle trackHandle, int32_t devId, int32_t chnId, enum AudioPortDirection dir)
{
    for (uint32_t i = 0; i < ARRAY_SIZE(g_audioPortInfos); i++) {
        if (dir == g_audioPortInfos[i].dir) {
            g_audioPortInfos[i].enable = true;
            g_audioPortInfos[i].deviceId = devId;
            g_audioPortInfos[i].channelId = chnId;
            g_audioPortInfos[i].trackHandle = trackHandle;
        }
    }
}

static void DeletePortInfo(enum AudioPortDirection dir)
{
    for (uint32_t i = 0; i < ARRAY_SIZE(g_audioPortInfos); i++) {
        if (dir == g_audioPortInfos[i].dir) {
            g_audioPortInfos[i].enable = false;
            g_audioPortInfos[i].deviceId = -1;
            g_audioPortInfos[i].channelId = -1;
            g_audioPortInfos[i].trackHandle = NULL;
        }
    }
}

static void GetPortInfo(int32_t *devId, int32_t *chnId, AudioPortTrack **trackHandle, enum AudioPortDirection dir)
{
    for (uint32_t i = 0; i < ARRAY_SIZE(g_audioPortInfos); i++) {
        if (dir == g_audioPortInfos[i].dir) {
            *devId = g_audioPortInfos[i].deviceId;
            *chnId = g_audioPortInfos[i].channelId;
            *trackHandle = g_audioPortInfos[i].trackHandle;
        }
    }
}

static int32_t SetDeviceAndChannelId(AudioPortTrack *trackHandle, int32_t deviceId, int32_t channelId)
{
    char keyValuePair[CUR_PARAM_MAX_SIZE] = { 0 };
    int32_t ret = sprintf_s(keyValuePair, sizeof(keyValuePair), "%s = %d %s = %d", "deviceId", deviceId,
        "channelId", channelId);
    if (ret == MEDIA_HAL_ERR) {
        MEDIA_HAL_LOGE(MODULE_NAME, "splice key = value string failed: %d", ret);
        return MEDIA_HAL_ERR;
    }

    struct InvokeAttr paramAttr = {};
    paramAttr.request = (void *)keyValuePair;
    if (trackHandle == NULL || trackHandle->Invoke == NULL) {
        return MEDIA_HAL_ERR;
    }
    ret = trackHandle->Invoke(trackHandle, INVOKE_ID_SET_EXTRA_PARAMS, &paramAttr);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "audio input port set extra param failed: 0x%x", ret);
        return ret;
    }
    return MEDIA_HAL_OK;
}

int32_t AudioEffectEnable(AudioHandle trackHandle, int32_t devId, int32_t chnId, enum AudioPortDirection dir)
{
    if (BothEnable()) {
        MEDIA_HAL_LOGI(MODULE_NAME, "input and output port both enabled");
        return MEDIA_HAL_OK;
    }

    /* 1. record trackHandle/devId/chnId */
    RecordPortInfo(trackHandle, devId, chnId, dir);

    if (!BothEnable()) {
        MEDIA_HAL_LOGI(MODULE_NAME, "%s port have enabled wait until the other port is enabled",
            (dir == PORT_IN) ? "input" : "output");
        return MEDIA_HAL_OK;
    }

    /* 2. transfer output port devId/chnId to input port, then enable talk vqe */
    int32_t aiDeviceId = -1;
    int32_t aiChannelId = -1;
    AudioPortTrack *aiTrackHandle = NULL;
    GetPortInfo(&aiDeviceId, &aiChannelId, &aiTrackHandle, PORT_IN);

    int32_t aoDeviceId = -1;
    int32_t aoChannelId = -1;
    AudioPortTrack *aoTrackHandle = NULL;
    GetPortInfo(&aoDeviceId, &aoChannelId, &aoTrackHandle, PORT_OUT);

    int32_t ret = SetDeviceAndChannelId(aiTrackHandle, aoDeviceId, aoChannelId);
    if (ret != MEDIA_HAL_OK) {
        return ret;
    }
    enum VqeScene vqeScene = VQE_SCENE_AI_TALK;
    struct InvokeAttr vqeAttr = {};
    vqeAttr.request = &vqeScene;
    ret = aiTrackHandle->Invoke(aiTrackHandle, INVOKE_ID_CONFIG_VQE, &vqeAttr);
    if (ret != MEDIA_HAL_OK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "audio input port config vqe failed: 0x%x", ret);
        return ret;
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

int32_t AudioEffectDisable(AudioHandle trackHandle, enum AudioPortDirection dir)
{
    MEDIA_HAL_UNUSED(trackHandle);

    /* 1. delete trackHandle/devId/chnId */
    DeletePortInfo(dir);

    if (BothDisable()) {
        MEDIA_HAL_LOGI(MODULE_NAME, "input and output port both enabled");
        return MEDIA_HAL_OK;
    }

    /* 2. ai talk vqe alg, need reference the stream of ao, we need notify module ai if modules ao have been disabled */
    if (dir == PORT_OUT) {
        int32_t aiDeviceId = -1;
        int32_t aiChannelId = -1;
        AudioPortTrack *aiTrackHandle = NULL;
        GetPortInfo(&aiDeviceId, &aiChannelId, &aiTrackHandle, PORT_IN);

        /* 3. both device id and channel id are invalid, means modules ao have been disabled */
        int32_t aoDeviceId = -1;
        int32_t aoChannelId = -1;

        int32_t ret = SetDeviceAndChannelId(aiTrackHandle, aoDeviceId, aoChannelId);
        if (ret != MEDIA_HAL_OK) {
            return ret;
        }
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
