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
 * Description: Primary sub port out header
 */

#ifndef AUDIO_PRIMARY_SUB_PORT_OUT_H
#define AUDIO_PRIMARY_SUB_PORT_OUT_H

#include "audio_port_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

/* sub port out api */
int32_t AudioPrimarySubPortOutOpen(AudioHandle *portHandle);
int32_t AudioPrimarySubPortOutClose(AudioHandle portHandle);
int32_t AudioPrimarySubPortOutGetCapability(const struct AudioPort *port, struct AudioPortCapability *capability);
int32_t AudioPrimarySubPortOutCreateTrack(AudioHandle portHandle, AudioHandle *trackHandle,
    struct PortPluginAttr *portAttr, enum AudioPortPin audioPortPin);
int32_t AudioPrimarySubPortOutDestroyTrack(AudioHandle portHandle, AudioHandle trackHandle);

#ifdef __cplusplus
}
#endif

#endif