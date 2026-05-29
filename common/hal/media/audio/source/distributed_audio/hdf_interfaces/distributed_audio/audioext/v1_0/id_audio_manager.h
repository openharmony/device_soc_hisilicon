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

#ifndef OHOS_HDI_ID_AUDIOMANAGER_H
#define OHOS_HDI_ID_AUDIOMANAGER_H

#include <string>
#include "types.h"
#include "id_audio_callback.h"

namespace OHOS {
namespace HDI {
namespace DistributedAudio {
namespace Audioext {
namespace V1_0 {
class IDAudioManager {
public:
    virtual ~IDAudioManager() {}
    virtual int32_t RegisterAudioDevice(const std::string &adpName, int32_t devId, const std::string &capability,
        const std::shared_ptr<IDAudioCallback> &callbackObj) = 0;
    
    virtual int32_t UnRegisterAudioDevice(const std::string &adpName, int32_t devId) = 0;
    
    virtual int32_t NotifyEvent(const std::string &adpName, int32_t devId, const DAudioEvent &event) = 0;
};
}
}
}
}
}
#endif