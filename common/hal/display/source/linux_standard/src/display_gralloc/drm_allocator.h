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
#ifndef DRM_ALLOCATOR_H
#define DRM_ALLOCATOR_H
#include <string>
#include "allocator.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class DrmAllocator : public Allocator {
public:
    int32_t Init() override;
    int32_t Allocate(const BufferInfo &bufferInfo, BufferHandle &handle) override;
    ~DrmAllocator() override;
private:
    static constexpr const char* FILE_PATH = "/dev/dri/card0";
    uint64_t GetPhysicalAddr(int primeFd);
    int32_t drmFd_ = -1;
};
}
}
}
#endif // DRM_ALLOCATOR_H