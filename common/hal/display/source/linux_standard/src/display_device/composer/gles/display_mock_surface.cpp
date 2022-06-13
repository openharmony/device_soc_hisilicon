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
#include "display_mock_surface.h"
#include "libsync.h"
#include "display_common.h"
#include "display_gralloc.h"
namespace OHOS {
bool DisplayMockSurface::AllocBuffer(AllocInfo &info)
{
    int32_t ret;
    BufferHandle *bufferHandle = nullptr;
    DISPLAY_LOGD();
    ret = grallocFuncs_->AllocMem(&info, &bufferHandle);
    DISPLAY_CHK_RETURN(((ret != DISPLAY_SUCCESS) || (bufferHandle == nullptr)), false,
        DISPLAY_LOGE("can not alloc buffer"));
    auto surfaceBuffer = HardwareBuffer::Create(*bufferHandle);
    hardwareBufferQueue_.push(surfaceBuffer);
    // the HardwareBuffer will hold the bufferhandle, now we can free the bufferhandle
    grallocFuncs_->FreeMem(bufferHandle);
    return true;
}

bool DisplayMockSurface::Init(uint32_t width, uint32_t height)
{
    int32_t ret;
    DISPLAY_LOGD();
    ret = GrallocInitialize(&grallocFuncs_);
    DISPLAY_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_LOGE("can not get gralloc function"));
    for (int i = 0; i < BUFFER_NUM; i++) {
        AllocInfo info;
        info.format = PIXEL_FMT_RGBA_8888;
        info.width = width;
        info.height = height;
        info.usage = HBM_USE_MEM_FB | HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE;
        AllocBuffer(info);
    }
    width_ = width;
    height_ = height;
    return true;
}

bool DisplayMockSurface::IsConsumer() const
{
    DISPLAY_LOGD();
    return false;
}

sptr<IBufferProducer> DisplayMockSurface::GetProducer() const
{
    DISPLAY_LOGD();
    return nullptr;
}

GSError DisplayMockSurface::RequestBuffer(sptr<SurfaceBuffer> &buffer, int32_t &fence, BufferRequestConfig &config)
{
    DISPLAY_LOGD();
    if (hardwareBufferQueue_.empty()) {
        DISPLAY_LOGE("has no buffer");
        return GSERROR_NO_BUFFER;
    }
    auto surfaceBuffer = hardwareBufferQueue_.front();
    if (surfaceBuffer == nullptr) {
        DISPLAY_LOGE("the buffer is nullptr");
        return GSERROR_NO_BUFFER;
    }
    HardwareBuffer *hardwareBuffer = static_cast<HardwareBuffer *>(surfaceBuffer.GetRefPtr());
    fence = hardwareBuffer->GetReleaseFence();
    // todo: now the ddk kernel can not support acquire fence, need fix it latter
    if (fence >= 0) {
        sync_wait(fence, FENCE_TIMEOUT);
        close(fence);
        fence = -1;
    }
    buffer = surfaceBuffer;
    hardwareBufferQueue_.pop();
    DISPLAY_LOGD("success");
    return GSERROR_OK;
}

GSError DisplayMockSurface::CancelBuffer(sptr<SurfaceBuffer> &buffer)
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}

GSError DisplayMockSurface::FlushBuffer(sptr<SurfaceBuffer> &buffer, int32_t fence, BufferFlushConfig &config)
{
    DISPLAY_LOGD();
    curentPresentBuffer_ = buffer;
    // todo fence
    if (fence >= 0) {
        close(fence);
        fence = -1;
    }
    HardwareBuffer *hardwareBuffer = static_cast<HardwareBuffer *>(buffer.GetRefPtr());
    DISPLAY_CHK_RETURN((hardwareBuffer == nullptr), GSERROR_NO_BUFFER, DISPLAY_LOGE("can not get the ptr"));

    return GSERROR_OK;
}

GSError DisplayMockSurface::AcquireBuffer(sptr<SurfaceBuffer> &buffer, int32_t &fence, int64_t &timestamp, Rect &damage)
{
    DISPLAY_LOGD();
    buffer = curentPresentBuffer_;
    fence = -1;
    return GSERROR_OK;
}

void DisplayMockSurface::AcquireBufferHandle(HDI::DISPLAY::HdiLayer &layer)
{
    DISPLAY_LOGD();
    if (curentPresentBuffer_ == nullptr) {
        DISPLAY_LOGE("cureent present buffer is nullptr");
        return;
    }
    layer.SetLayerBuffer(curentPresentBuffer_->GetBufferHandle(), -1);
}

void DisplayMockSurface::SetReleaseFence(int32_t fence)
{
    DISPLAY_LOGD("release fence %{public}d", fence);
    if (curentPresentBuffer_ == nullptr) {
        DISPLAY_LOGW("cureent present buffer is nullptr");
        return;
    }
    HardwareBuffer *buffer = reinterpret_cast<HardwareBuffer *>(curentPresentBuffer_.GetRefPtr());
    buffer->SetReleaseFence(fence);
    hardwareBufferQueue_.push(curentPresentBuffer_);
    curentPresentBuffer_ = nullptr;
}

GSError DisplayMockSurface::ReleaseBuffer(sptr<SurfaceBuffer> &buffer, int32_t fence)
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}

GSError DisplayMockSurface::AttachBuffer(sptr<SurfaceBuffer> &buffer)
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}
GSError DisplayMockSurface::DetachBuffer(sptr<SurfaceBuffer> &buffer)
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}

uint32_t DisplayMockSurface::GetQueueSize()
{
    DISPLAY_LOGD();
    return BUFFER_NUM;
}

GSError DisplayMockSurface::SetQueueSize(uint32_t queueSize)
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}

GSError DisplayMockSurface::SetDefaultWidthAndHeight(int32_t width, int32_t height)
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}

int32_t DisplayMockSurface::GetDefaultWidth()
{
    DISPLAY_LOGD();
    return width_;
}

int32_t DisplayMockSurface::GetDefaultHeight()
{
    DISPLAY_LOGD();
    return height_;
}

GSError DisplayMockSurface::SetDefaultUsage(uint32_t usage)
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}
uint32_t DisplayMockSurface::GetDefaultUsage()
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}

GSError DisplayMockSurface::SetUserData(const std::string &key, const std::string &val)
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}
std::string DisplayMockSurface::GetUserData(const std::string &key)
{
    DISPLAY_LOGD();
    return "";
}

GSError DisplayMockSurface::GetName(std::string &name)
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}

GSError DisplayMockSurface::RegisterConsumerListener(sptr<IBufferConsumerListener> &listener)
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}
GSError DisplayMockSurface::RegisterConsumerListener(IBufferConsumerListenerClazz *listener)
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}
GSError DisplayMockSurface::RegisterReleaseListener(OnReleaseFunc func)
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}
GSError DisplayMockSurface::UnregisterConsumerListener()
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}

uint64_t DisplayMockSurface::GetUniqueId() const
{
    DISPLAY_LOGD();
    return 0;
}

void DisplayMockSurface::Dump(std::string &result) const
{
    DISPLAY_LOGD();
}

GSError DisplayMockSurface::CleanCache()
{
    DISPLAY_LOGD();
    return GSERROR_OK;
}
}
