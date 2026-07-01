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
 * Audio effect manager header
 */

#ifndef AUDIO_EFFECT_MANAGER_H
#define AUDIO_EFFECT_MANAGER_H

#include "audio_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t AudioEffectEnable(AudioHandle trackHandle, int32_t devId, int32_t chnId, enum AudioPortDirection dir);
int32_t AudioEffectDisable(AudioHandle trackHandle, enum AudioPortDirection dir);

#ifdef __cplusplus
}
#endif

#endif