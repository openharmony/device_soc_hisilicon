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
#include "framebuffer_allocator.h"
#include <fcntl.h>
#include <cerrno>
#include <securec.h>
#include <linux/fb.h>
#include "display_common.h"
#include "display_adapter.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
int32_t FramebufferAllocator::SetFdFormatAndVirtualRes(struct fb_var_screeninfo varInfo)
{
    int ret;
    DISPLAY_LOGD();
    varInfo.reserved[0] = 0;
    varInfo.reserved[1] = 0;
    varInfo.reserved[2] = 0; // init reserved[2] is 0
    varInfo.xoffset = 0;
    varInfo.yoffset = 0;
    varInfo.activate = FB_ACTIVATE_NOW;

    varInfo.bits_per_pixel = 32; // 32bit
    varInfo.transp.length = 8; // 8 bits
    varInfo.red.length = 8; // 8 bits
    varInfo.green.length = 8; // 8 bits
    varInfo.blue.length = 8; // 8 bits

    varInfo.transp.offset = 24; // offset 24 of a channel
    varInfo.red.offset = 0; // offset 16 of red channel
    varInfo.green.offset = 8; // offset 8 of green channel
    varInfo.blue.offset = 16; // offset 16 of blue channel

    varInfo.xres_virtual = varInfo.xres;
    varInfo.yres_virtual = varInfo.yres * FB_BUFFERS_NUM;

    // set framebuffer vary info
    ret = DisplayAdapter::GetInstance()->Ioctl(deviceFd_, FBIOPUT_VSCREENINFO, &varInfo);
    DISPLAY_CHK_RETURN((ret < 0), DISPLAY_FAILURE, DISPLAY_LOGE("FBIOPUT_VSCREENINFO failed errno : %d", errno));
    return DISPLAY_SUCCESS;
}

int32_t FramebufferAllocator::InitFb()
{
    int ret;
    DISPLAY_LOGD();
    ret = DisplayAdapter::GetInstance()->Ioctl(deviceFd_, FBIOGET_FSCREENINFO, &fixInfo_);
    DISPLAY_CHK_RETURN((ret < 0), DISPLAY_FAILURE, DISPLAY_LOGE("FBIOGET_FSCREENINFO failed errno : %d", errno));

    ret = DisplayAdapter::GetInstance()->Ioctl(deviceFd_, FBIOGET_VSCREENINFO, &varInfo_);
    DISPLAY_CHK_RETURN((ret < 0), DISPLAY_FAILURE, DISPLAY_LOGE("FBIOGET_VSCREENINFO failed errno : %d", errno));

    ret = SetFdFormatAndVirtualRes(varInfo_);
    DISPLAY_CHK_RETURN((ret < 0), DISPLAY_FAILURE, DISPLAY_LOGE(" failed to set fb format errno : %d", errno));

    // update the variable screen information
    ret = DisplayAdapter::GetInstance()->Ioctl(deviceFd_, FBIOGET_VSCREENINFO, &varInfo_);
    DISPLAY_CHK_RETURN((ret < 0), DISPLAY_FAILURE, DISPLAY_LOGE("FBIOGET_VSCREENINFO failed errno : %d", errno));

    size_t frameBufferSize = fixInfo_.line_length * varInfo_.yres_virtual;

    frameBufferSize_ = frameBufferSize;
    DISPLAY_CHK_RETURN((varInfo_.yres == 0), DISPLAY_FAILURE, DISPLAY_LOGE("the yres is zero"));
    buffersNum_ = varInfo_.yres_virtual / varInfo_.yres;
    bufferSize_ = fixInfo_.line_length * varInfo_.yres;
    smemStart_ = fixInfo_.smem_start;
    dmaBufferFb_ = DisplayAdapter::GetInstance()->FbGetDmaBuffer(deviceFd_);
    DISPLAY_LOGD("the dmabuffer fd of framebuffer is %{public}d", dmaBufferFb_);
    for (uint32_t i = 0; i < bufferSize_; i++) {
        freeBuffers_.push(smemStart_ + i * bufferSize_);
    }
    DISPLAY_LOGD();
    return DISPLAY_SUCCESS;
}

int32_t FramebufferAllocator::Init()
{
    DISPLAY_LOGD();
    if (DisplayAdapter::GetInstance() == nullptr) {
        DISPLAY_LOGE("framebuffer allocator must depend on the display adapter");
        return DISPLAY_FAILURE;
    }
    deviceFd_ = DisplayAdapter::GetInstance()->OpenDevice(FBDEV_PATH, O_RDWR, 0);
    if (deviceFd_ < 0) {
        DISPLAY_LOGE("Failed to open fbdev %{public}d", deviceFd_);
        return DISPLAY_FAILURE;
    }
    return InitFb();
}

int32_t FramebufferAllocator::Allocate(const BufferInfo &bufferInfo, BufferHandle &handle)
{
    DISPLAY_LOGD();
    if (freeBuffers_.empty()) {
        DISPLAY_LOGE("has no memory");
        return DISPLAY_NOMEM;
    }
    unsigned long smemAddress = freeBuffers_.front();
    uint32_t offset = smemAddress - smemStart_;

    handle.phyAddr = smemAddress;
    handle.fd = DisplayAdapter::GetInstance()->FbGetDmaBuffer(deviceFd_);
    DISPLAY_LOGD("fd is %{public}d", handle.fd);
    // temp for use key to save offset
    handle.key = offset;
    handle.format = PIXEL_FMT_RGBA_8888;
    handle.width = bufferInfo.width_;
    handle.height = bufferInfo.height_;
    handle.stride = fixInfo_.line_length;
    handle.size = bufferSize_;
    handle.usage = bufferInfo.usage_;
    freeBuffers_.pop();
    return DISPLAY_SUCCESS;
}

FramebufferAllocator::~FramebufferAllocator()
{
    DISPLAY_LOGD();
    if (deviceFd_ > -1) {
        DisplayAdapter::GetInstance()->CloseDevice(deviceFd_);
        deviceFd_ = -1;
    }
}

void* FramebufferAllocator::Mmap(BufferHandle &handle)
{
    DISPLAY_LOGD();
    void *virAddr = nullptr;
    if (handle.virAddr != nullptr) {
        DISPLAY_LOGW("the buffer has virtual addr");
        return handle.virAddr;
    }

    if (handle.fd > -1) {
        virAddr = mmap(nullptr, frameBufferSize_, PROT_READ | PROT_WRITE, MAP_SHARED, handle.fd, 0);
    } else {
        virAddr = mmap(nullptr, frameBufferSize_, PROT_READ | PROT_WRITE, MAP_SHARED, deviceFd_, 0);
    }
    if (virAddr == MAP_FAILED) {
        DISPLAY_LOGE("failed to mmap errno %{public}d", errno);
        handle.virAddr = nullptr;
        return nullptr;
    }
    handle.virAddr = static_cast<uint8_t*>(virAddr) + handle.key;
    DISPLAY_LOGD("virAddr %{public}p handle.virAddr %{public}p", handle.virAddr, virAddr);

    return handle.virAddr;
}

int32_t FramebufferAllocator::Unmap(BufferHandle &handle)
{
    DISPLAY_LOGD();
    if (handle.virAddr == nullptr) {
        DISPLAY_LOGE("virAddr is nullptr , has not map the buffer");
        return DISPLAY_PARAM_ERR;
    }
    int ret = munmap(static_cast<uint8_t*>(handle.virAddr) - handle.key, frameBufferSize_);
    if (ret != 0) {
        DISPLAY_LOGE("munmap failed err: %{public}s", strerror(errno));
        return DISPLAY_FAILURE;
    }
    handle.virAddr = nullptr;
    return DISPLAY_SUCCESS;
}
int32_t FramebufferAllocator::FreeMem(BufferHandle *handle)
{
    DISPLAY_CHK_RETURN((handle == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("handle is nullptr"));
    freeBuffers_.push(handle->phyAddr);
    Allocator::FreeMem(handle);
    return DISPLAY_SUCCESS;
}
}
}
}