/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
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

#ifndef DISTRIBUTED_CAMERA_BUFFER_MANAGER_H
#define DISTRIBUTED_CAMERA_BUFFER_MANAGER_H

#include <list>
#include <mutex>
#include <memory>
#include "constants.h"
#include "hal_camera.h"

namespace OHOS {
namespace DistributedHardware {

class DBufferManager {
public:
    DBufferManager();
    ~DBufferManager();

public:
    std::shared_ptr<HalBuffer> AcquireBuffer();

    RetCode ShutterBuffer(std::shared_ptr<HalBuffer>& buffer);

private:
    std::mutex lock_;
    std::list<std::shared_ptr<HalBuffer>> idleList_ = {};
    std::list<std::shared_ptr<HalBuffer>> busyList_ = {};
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // DISTRIBUTED_CAMERA_BUFFER_MANAGER_H