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

#ifndef AUDIO_PRIMARY_SUB_PORT_IN_H
#define AUDIO_PRIMARY_SUB_PORT_IN_H

#include "audio_port_plugin.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of __cplusplus */

/* sub port in track api */
int32_t AudioPrimarySubPortInGetFrameCount(AudioHandle trackHandle, uint64_t *count);

int32_t AudioPrimarySubPortInEnableTrack(AudioHandle trackHandle);
int32_t AudioPrimarySubPortInDisableTrack(AudioHandle trackHandle);

int32_t AudioPrimarySubPortInAcquireFrame(AudioHandle trackHandle, uint8_t *buffer,
    uint64_t requestBytes, uint64_t *replyBytes);

int32_t AudioPrimarySubPortInGetPosition(AudioHandle trackHandle,
    uint64_t *frames, struct AudioTimeStamp *time);

int32_t AudioPrimarySubPortInSetMute(AudioHandle trackHandle, bool mute);
int32_t AudioPrimarySubPortInGetMute(AudioHandle trackHandle, bool *mute);

int32_t AudioPrimarySubPortInSetVolume(AudioHandle trackHandle, float volume);
int32_t AudioPrimarySubPortInGetVolume(AudioHandle trackHandle, float *volume);

int32_t AudioPrimarySubPortInInvoke(AudioHandle trackHandle, enum InvokeID invokeID, struct InvokeAttr *invokeAttr);

/* sub port in api */
int32_t AudioPrimarySubPortInOpen(AudioHandle *portHandle);
int32_t AudioPrimarySubPortInClose(AudioHandle portHandle);

int32_t AudioPrimarySubPortInGetCapability(const struct AudioPort *port, struct AudioPortCapability *capability);

int32_t AudioPrimarySubPortInCreateTrack(AudioHandle portHandle, AudioHandle *trackHandle,
    struct PortPluginAttr *inputAttr, enum AudioPortPin audioPortPin);
int32_t AudioPrimarySubPortInDestroyTrack(AudioHandle portHandle, AudioHandle trackHandle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* AUDIO_INPUT_PORT_PLUGIN_IMPL_H */
