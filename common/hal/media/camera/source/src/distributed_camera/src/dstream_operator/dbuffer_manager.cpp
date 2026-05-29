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

#include "dbuffer_manager.h"
#include "distributed_hardware_log.h"
#include "constants.h"

#define TAG "DBufferMgr"

namespace OHOS {
namespace DistributedHardware {

DBufferManager::DBufferManager()
{
    std::unique_lock<std::mutex> l(lock_);
    for (uint32_t i = 0; i < BUFFER_QUEUE_SIZE; i++) {
        std::shared_ptr<HalBuffer> buffer = std::make_shared<HalBuffer>();
        int32_t size = MAX_SUPPORT_WIDTH * MAX_SUPPORT_HEIGHT * 3 / 4;
        void *virAddr = malloc(size);
        if (virAddr == nullptr) {
            DHLOGE("malloc failed.");
            break;
        }
        buffer->virAddr = virAddr;
        buffer->size = size;
        buffer->flag = i;
        idleList_.emplace_back(buffer);
        DHLOGI("HalBuffer emplace.:%d size:%d", i, size);
    }
}

DBufferManager::~DBufferManager()
{
    std::unique_lock<std::mutex> l(lock_);
    while (!idleList_.empty()) {
        std::shared_ptr<HalBuffer> buffer = idleList_.front();
        free(buffer->virAddr);
        idleList_.pop_front();
    }
    while (!busyList_.empty()) {
        std::shared_ptr<HalBuffer> buffer = busyList_.front();
        free(buffer->virAddr);
        busyList_.pop_front();
    }
}


std::shared_ptr<HalBuffer> DBufferManager::AcquireBuffer()
{
    std::unique_lock<std::mutex> l(lock_);

    if (!idleList_.empty()) {
        auto it = idleList_.front();
        busyList_.push_front(it);
        idleList_.pop_front();
        return it;
    }
    DHLOGE("AcquireBuffer failed");
    return nullptr;
}

RetCode DBufferManager::ShutterBuffer(std::shared_ptr<HalBuffer>& buffer)
{
    std::unique_lock<std::mutex> l(lock_);
    std::list<std::shared_ptr<HalBuffer>>::iterator it = busyList_.begin();
    for (; it != busyList_.end(); it++) {
        if ((*it)->virAddr != buffer->virAddr) {
            continue;
        }
        idleList_.push_front(*it);
        busyList_.erase(it);
        return Ret::RC_OK;
    }
    DHLOGE("Buffer :%x not found, cannot Shutter buffer.", buffer->virAddr);
    return Ret::RC_ERROR;
}

} // namespace DistributedHardware
} // namespace OHOS
