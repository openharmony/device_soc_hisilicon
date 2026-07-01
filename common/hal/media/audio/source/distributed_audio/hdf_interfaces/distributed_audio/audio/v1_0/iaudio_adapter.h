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

#ifndef OHOS_HDI_IAUDIO_ADAPTER_H
#define OHOS_HDI_IAUDIO_ADAPTER_H

#include <vector>
#include "audio_types.h"
#include "iaudio_callback.h"
#include "iaudio_render.h"
#include "iaudio_capture.h"

namespace OHOS {
namespace HDI {
namespace DistributedAudio {
namespace Audio {
namespace V1_0 {
class IAudioAdapter {
public:
    virtual ~IAudioAdapter() {}
    virtual int32_t InitAllPorts() = 0;
    virtual int32_t CreateRender(const AudioDeviceDescriptor &desc,
        const AudioSampleAttributes &attrs, std::shared_ptr<IAudioRender> &render) = 0;
    virtual int32_t DestroyRender(const AudioDeviceDescriptor &desc) = 0;
    virtual int32_t CreateCapture(const AudioDeviceDescriptor &desc, const AudioSampleAttributes &attrs,
        std::shared_ptr<IAudioCapture> &capture) = 0;
    virtual int32_t DestroyCapture(const AudioDeviceDescriptor &desc) = 0;
    virtual int32_t GetPortCapability(const AudioPort &port, AudioPortCapability &capability) = 0;
    virtual int32_t SetPassthroughMode(const AudioPort &port, AudioPortPassthroughMode mode) = 0;
    virtual int32_t GetPassthroughMode(const AudioPort &port, AudioPortPassthroughMode &mode) = 0;
    virtual int32_t GetDeviceStatus(AudioDeviceStatus &status) = 0;
    virtual int32_t UpdateAudioRoute(const AudioRoute &route, int32_t &routeHandle) = 0;
    virtual int32_t ReleaseAudioRoute(int32_t routeHandle) = 0;
    virtual int32_t SetMicMute(bool mute) = 0;
    virtual int32_t GetMicMute(bool &mute) = 0;
    virtual int32_t SetVoiceVolume(float volume) = 0;
    virtual int32_t SetExtraParams(AudioExtParamKey key, const std::string &condition, const std::string &value) = 0;
    virtual int32_t GetExtraParams(AudioExtParamKey key, const std::string &condition, std::string &value) = 0;
    virtual int32_t RegExtraParamObserver(const std::shared_ptr<IAudioCallback> &audioCallback, int8_t cookie) = 0;
};
}
}
}
}
}
#endif