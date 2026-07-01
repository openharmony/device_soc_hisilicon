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

#ifndef OHOS_HDI_TYPES_H
#define OHOS_HDI_TYPES_H

#include <string>
#include <vector>
#include <memory>

namespace OHOS {
    namespace HDI {
        namespace DistributedAudio {
            namespace Audioext {
                namespace V1_0 {
                    enum PortOperationMode {
                        NORMAL_MODE = 0,
                        MMAP_MODE = 1,
                    };
                    struct AudioParameter {
                        uint32_t format;
                        uint32_t channelCount;
                        uint32_t sampleRate;
                        uint32_t period;
                        uint32_t frameSize;
                        uint32_t streamUsage;
                        enum PortOperationMode renderFlags;
                        enum PortOperationMode capturerFlags;
                        std::string ext;
                    };
                    struct AudioData {
                        struct AudioParameter param;
                        std::vector<int8_t> data;
                    };
                    struct DAudioEvent {
                        int32_t type;
                        std::string content;
                    };
                    struct CurrentTime {
                        long tvSec;
                        long tvNSec;
                    };
                }
            }
        }
    }
}
#endif