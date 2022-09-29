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
#include "dmabufferheap_allocator.h"
#include <cerrno>
#include "display_common.h"
#include "dmabuf_alloc.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
int32_t DmaBufferHeapAllocator::Init()
{
    DISPLAY_LOGD();
    static constexpr const char *HEAP_NAME = "system";
    deviceFd_ = DmabufHeapOpen(HEAP_NAME);
    if (deviceFd_ < 0) {
        DISPLAY_LOGE("Failed to open dmabufferheap errno %{public}d", errno);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

int32_t DmaBufferHeapAllocator::Allocate(const BufferInfo &bufferInfo, BufferHandle &handle)
{
    DISPLAY_LOGD();
    DmabufHeapBuffer buffer;
    buffer.heapFlags = 0;
    buffer.size = bufferInfo.size_ + BUFFER_PRIV_SIZE;
    DmabufHeapBufferAlloc(deviceFd_, &buffer);
    handle.fd = buffer.fd;
    return DISPLAY_SUCCESS;
}

DmaBufferHeapAllocator::~DmaBufferHeapAllocator()
{
    DISPLAY_LOGD();
    if (deviceFd_ >= 0) {
        DmabufHeapClose(deviceFd_);
        deviceFd_ = -1;
    }
}
} // namespace DISPLAY
} // namespace HDI
} // namespace OHOS