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
#ifndef FRAMEBUFFER_ALLOCATOR_H
#define FRAMEBUFFER_ALLOCATOR_H
#include <queue>
#include <linux/fb.h>
#include "allocator.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class FramebufferAllocator : public Allocator {
public:
    int32_t Init() override;
    int32_t Allocate(const BufferInfo &bufferInfo, BufferHandle &handle) override;
    void* Mmap(BufferHandle &handle) override;
    int32_t Unmap(BufferHandle &handle) override;
    int32_t FreeMem(BufferHandle *handle) override;
    ~FramebufferAllocator() override;
private:
    static constexpr uint32_t FB_BUFFERS_NUM = 3;
    static constexpr const char *FBDEV_PATH = "/dev/graphics/fb0";

    int32_t SetFdFormatAndVirtualRes(struct fb_var_screeninfo varInfo);
    int32_t InitFb();
    int32_t GetDmaBuffer();

    struct fb_fix_screeninfo fixInfo_;
    struct fb_var_screeninfo varInfo_;
    // framebuffer information
    uint32_t bufferSize_ = 0;
    uint32_t buffersNum_ = 0;
    uint32_t frameBufferSize_ = 0;
    unsigned long smemStart_ = 0;
    int32_t dmaBufferFb_ = -1;
    int32_t deviceFd_ = -1;
    std::queue<unsigned long> freeBuffers_;
};
}
}
}
#endif // FRAMEBUFFER_ALLOCATOR_H