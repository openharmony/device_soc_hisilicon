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

#ifndef OHOS_HDI_ID_AUDIO_CALLBACK_H
#define OHOS_HDI_ID_AUDIO_CALLBACK_H

#include "types.h"

namespace OHOS {
    namespace HDI {
        namespace DistributedAudio {
            namespace Audioext {
                namespace V1_0 {
                    class IDAudioCallback {
                        public:
                        virtual ~IDAudioCallback() {}
                        virtual int32_t OpenDevice(const std::string &adpName, int32_t devId) = 0;

                        virtual int32_t CloseDevice(const std::string &adpName, int32_t devId) = 0;

                        virtual int32_t SetParameters(const std::string &adpName, int32_t devId,
                            const OHOS::HDI::DistributedAudio::Audioext::V1_0::AudioParameter &param) = 0;

                        virtual int32_t NotifyEvent(const std::string &adpName, int32_t devId,
                            const OHOS::HDI::DistributedAudio::Audioext::V1_0::DAudioEvent &event) = 0;

                        virtual int32_t WriteStreamData(const std::string &adpName, int32_t devId,
                            const OHOS::HDI::DistributedAudio::Audioext::V1_0::AudioData &data) = 0;

                        virtual int32_t ReadStreamData(const std::string &adpName, int32_t devId,
                            OHOS::HDI::DistributedAudio::Audioext::V1_0::AudioData &data) = 0;

                        virtual int32_t ReadMmapPosition(const std::string &adpName, int32_t devId, uint64_t &frames,
                            OHOS::HDI::DistributedAudio::Audioext::V1_0::CurrentTime &time) = 0;

                        virtual int32_t RefreshAshmemInfo(const std::string &adpName, int32_t devId,
                            int fd, int32_t ashmemLength, int32_t lengthPerTrans) = 0;
                    };
                }
            }
        }
    }
}
#endif