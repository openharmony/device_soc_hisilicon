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
#ifndef ALLOCATOR_MANAGER_H
#define ALLOCATOR_MANAGER_H
#include <memory>
#include "allocator.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class AllocatorManager {
public:
    static AllocatorManager& GetInstance()
    {
        static AllocatorManager instance;
        return instance;
    }
    int32_t Init();
    int32_t DeInit();
    Allocator* GetAllocator(uint64_t usage);

private:
    bool init_ = false;
    std::shared_ptr<Allocator> frameBufferAllocator_ = nullptr;
    std::shared_ptr<Allocator> allocator_ = nullptr;
};
}
}
}
#endif  // ALLOCATOR_MANAGER_H
