/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef OHOS_AUDIO_CAPTURE_INTERFACE_IMPL_BASE_H
#define OHOS_AUDIO_CAPTURE_INTERFACE_IMPL_BASE_H

#include <string>

#include "iaudio_capture.h"
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
    CAPTURE_STATUS_OPEN = 0,
    CAPTURE_STATUS_CLOSE,
    CAPTURE_STATUS_START,
    CAPTURE_STATUS_STOP,
    CAPTURE_STATUS_PAUSE,
} AudioCaptureStatus;

class AudioCaptureInterfaceImplBase : public IAudioCapture {
public:
    AudioCaptureInterfaceImplBase() = default;
    virtual ~AudioCaptureInterfaceImplBase() = default;

    virtual const AudioDeviceDescriptor &GetCaptureDesc() = 0;
    virtual void SetAttrs(const std::string &adpName, const AudioDeviceDescriptor &desc,
        const AudioSampleAttributes &attrs, const std::shared_ptr<IDAudioCallback> &callback) = 0;
};
} // V1_0
} // Audio
} // Distributedaudio
} // HDI
} // OHOS
#endif // OHOS_AUDIO_CAPTURE_INTERFACE_IMPL_BASE_H