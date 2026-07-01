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

#ifndef OHOS_HDI_IAUDIO_MANAGER_H
#define OHOS_HDI_IAUDIO_MANAGER_H

#include "audio_types.h"
#include "iaudio_adapter.h"

namespace OHOS {
namespace HDI {
namespace DistributedAudio {
namespace Audio {
namespace V1_0 {
class IAudioManagerDeviceChangeCallback;
class IAudioManager {
public:
    virtual ~IAudioManager() {}
    virtual int32_t GetAllAdapters(std::vector<AudioAdapterDescriptor> &descs) = 0;
    virtual int32_t LoadAdapter(const AudioAdapterDescriptor &desc,
        std::shared_ptr<IAudioAdapter> &adapter) = 0;
    virtual int32_t UnloadAdapter(const std::string &adapterName) = 0;
    virtual int32_t ReleaseAudioManagerObject() = 0;
    virtual void SetDeviceChangeCallback(const std::shared_ptr<IAudioManagerDeviceChangeCallback> &callback) = 0;
};
}
}
}
}
}
#endif
