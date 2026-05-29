/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#ifndef OHOS_AUDIO_RENDER_INTERFACE_IMPL_BASE_H
#define OHOS_AUDIO_RENDER_INTERFACE_IMPL_BASE_H

#include <mutex>
#include <string>
#include <cmath>

#include "audio_types.h"
#include "iaudio_render.h"
#include "id_audio_manager.h"

namespace OHOS {
namespace HDI {
namespace DistributedAudio {
namespace Audio {
namespace V1_0 {
using OHOS::HDI::DistributedAudio::Audioext::V1_0::AudioData;
using OHOS::HDI::DistributedAudio::Audioext::V1_0::AudioParameter;
using OHOS::HDI::DistributedAudio::Audioext::V1_0::IDAudioCallback;
typedef enum {
    RENDER_STATUS_OPEN = 0,
    RENDER_STATUS_CLOSE,
    RENDER_STATUS_START,
    RENDER_STATUS_STOP,
    RENDER_STATUS_PAUSE,
} AudioRenderStatus;

class AudioRenderInterfaceImplBase : public IAudioRender {
public:
    AudioRenderInterfaceImplBase() = default;
    virtual ~AudioRenderInterfaceImplBase() = default;

    virtual const AudioDeviceDescriptor &GetRenderDesc() = 0;
    virtual void SetVolumeInner(const uint32_t vol) = 0;
    virtual void SetVolumeRangeInner(const uint32_t volMax, const uint32_t volMin) = 0;
    virtual uint32_t GetVolumeInner() = 0;
    virtual uint32_t GetMaxVolumeInner() = 0;
    virtual uint32_t GetMinVolumeInner() = 0;
    virtual void SetAttrs(const std::string &adpName, const AudioDeviceDescriptor &desc,
        const AudioSampleAttributes &attrs, const std::shared_ptr<IDAudioCallback> &callback) = 0;
};
} // V1_0
} // Audio
} // Distributedaudio
} // HDI
} // OHOS
#endif // OHOS_AUDIO_RENDER_INTERFACE_IMPL_BASE_H
