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

#ifndef AUDIO_PRIMARY_SUB_PORT_IN_H
#define AUDIO_PRIMARY_SUB_PORT_IN_H

#include "audio_port_plugin.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of __cplusplus */

/* sub port in track api */
int32_t AudioUsbSubPortInGetFrameCount(AudioHandle trackHandle, uint64_t *count);

int32_t AudioUsbSubPortInEnableTrack(AudioHandle trackHandle);
int32_t AudioUsbSubPortInDisableTrack(AudioHandle trackHandle);

int32_t AudioUsbSubPortInAcquireFrame(AudioHandle trackHandle, uint8_t *buffer,
    uint64_t requestBytes, uint64_t *replyBytes);

int32_t AudioUsbSubPortInGetPosition(AudioHandle trackHandle,
    uint64_t *frames, struct AudioTimeStamp *time);

int32_t AudioUsbSubPortInSetMute(AudioHandle trackHandle, bool mute);
int32_t AudioUsbSubPortInGetMute(AudioHandle trackHandle, bool *mute);

int32_t AudioUsbSubPortInSetVolume(AudioHandle trackHandle, float volume);
int32_t AudioUsbSubPortInGetVolume(AudioHandle trackHandle, float *volume);

int32_t AudioUsbSubPortInInvoke(AudioHandle trackHandle, enum InvokeID invokeID, struct InvokeAttr *invokeAttr);

/* sub port in api */
int32_t AudioUsbSubPortInOpen(AudioHandle *portHandle, int card_usb);
int32_t AudioUsbSubPortInClose(AudioHandle portHandle);

int32_t AudioUsbSubPortInGetCapability(const struct AudioPort *port, struct AudioPortCapability *capability);

int32_t AudioUsbSubPortInCreateTrack(AudioHandle portHandle, AudioHandle *trackHandle,
    struct PortPluginAttr *inputAttr, enum AudioPortPin audioPortPin);
int32_t AudioUsbSubPortInDestroyTrack(AudioHandle portHandle, AudioHandle trackHandle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* AUDIO_INPUT_PORT_PLUGIN_IMPL_H */
