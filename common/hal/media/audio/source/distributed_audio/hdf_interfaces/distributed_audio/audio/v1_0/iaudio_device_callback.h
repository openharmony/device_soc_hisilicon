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

 /**
  * @addtogroup HdiAudio
  * @{
  *
  * @brief Provides unified APIs for audio services to access audio drivers.
  *
  * An audio service can obtain an audio driver object or agent and then call APIs provided by this object or agent to
  * access different types of audio devices based on the audio IDs, thereby obtaining audio information,
  * subscribing to or unsubscribing from audio data, enabling or disabling an audio,
  * setting the audio data reporting mode, and setting audio options such as the accuracy and measurement range.
  *
  * @version 1.0
  */

#ifndef OHOS_HDI_IAUDIO_DEVICE_CALLBACK_H
#define OHOS_HDI_IAUDIO_DEVICE_CALLBACK_H

#include <vector>
#include "media_info.h"

namespace OHOS {
    namespace HDI {
        namespace DistributedAudio {
            namespace Audio {
                namespace V1_0 {
                    class IAudioManagerDeviceChangeCallback {
                        public:
                        virtual ~IAudioManagerDeviceChangeCallback() = default;
                        virtual void OnDeviceChange(const AudioDeviceInfo &deviceInfo) = 0;
                    };
                }
            }
        }
    }
}
#endif