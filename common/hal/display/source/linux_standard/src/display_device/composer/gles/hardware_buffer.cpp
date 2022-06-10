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
#include "hardware_buffer.h"
#include "buffer_handle_utils.h"
#include "display_common.h"
#include "display_type.h"
#include "securec.h"
namespace OHOS {
BufferHandle *CloneHandle(const BufferHandle *handle)
{
    if (handle == nullptr) {
        DISPLAY_LOGW("%{public}s handle is nullptr", __func__);
        return nullptr;
    }

    BufferHandle *newHandle = AllocateBufferHandle(handle->reserveFds, handle->reserveInts);
    if (newHandle == nullptr) {
        DISPLAY_LOGW("%{public}s AllocateBufferHandle failed, newHandle is nullptr", __func__);
        return nullptr;
    }

    if (handle->fd == -1) {
        newHandle->fd = handle->fd;
    } else {
        newHandle->fd = dup(handle->fd);
        if (newHandle->fd == -1) {
            DISPLAY_LOGW("CloneBufferHandle dup failed");
            FreeBufferHandle(newHandle);
            return nullptr;
        }
    }
    newHandle->width = handle->width;
    newHandle->stride = handle->stride;
    newHandle->height = handle->height;
    newHandle->size = handle->size;
    newHandle->format = handle->format;
    newHandle->usage = handle->usage;
    newHandle->phyAddr = handle->phyAddr;
    newHandle->key = handle->key;

    for (uint32_t i = 0; i < newHandle->reserveFds; i++) {
        newHandle->reserve[i] = dup(handle->reserve[i]);
        if (newHandle->reserve[i] == -1) {
            DISPLAY_LOGW("CloneBufferHandle dup reserveFds failed");
            FreeBufferHandle(newHandle);
            return nullptr;
        }
    }
    if (newHandle->reserveInts > 0) {
        if (memcpy_s(&newHandle->reserve[newHandle->reserveFds], sizeof(int32_t) * newHandle->reserveInts,
            &handle->reserve[handle->reserveFds], sizeof(int32_t) * handle->reserveInts) != EOK) {
            DISPLAY_LOGW("CloneBufferHandle memcpy_s failed");
            FreeBufferHandle(newHandle);
            return nullptr;
        }
    }
    return newHandle;
}

OHOS::sptr<OHOS::SurfaceBuffer> HardwareBuffer::Create(const BufferHandle &handle)
{
    OHOS::sptr<OHOS::SurfaceBuffer> surfaceBuffer = new HardwareBuffer(handle);
    return surfaceBuffer;
}

BufferHandle *HardwareBuffer::GetBufferHandle() const
{
    DISPLAY_LOGD();
    return handle_;
}

HardwareBuffer::HardwareBuffer(const BufferHandle &handle)
{
    handle_ = CloneHandle(&handle);
}

HardwareBuffer::~HardwareBuffer()
{
    DISPLAY_LOGD();
    FreeBufferHandle(handle_);
}

int32_t HardwareBuffer::GetWidth() const
{
    return 0;
}
int32_t HardwareBuffer::GetHeight() const
{
    return 0;
}
int32_t HardwareBuffer::GetStride() const
{
    return 0;
}

int32_t HardwareBuffer::GetSurfaceBufferWidth() const
{
    return 0;
}
int32_t HardwareBuffer::GetSurfaceBufferHeight() const
{
    return 0;
}
SurfaceColorGamut HardwareBuffer::GetSurfaceBufferColorGamut() const
{
    return COLOR_GAMUT_INVALID;
}
GSError HardwareBuffer::SetSurfaceBufferWidth(int32_t width)
{
    return GSERROR_OK;
}

GSError HardwareBuffer::SetSurfaceBufferHeight(int32_t height)
{
    return GSERROR_OK;
}
GSError HardwareBuffer::SetSurfaceBufferColorGamut(SurfaceColorGamut colorGamut)
{
    return GSERROR_OK;
}

int32_t HardwareBuffer::GetFormat() const
{
    return 0;
}
int64_t HardwareBuffer::GetUsage() const
{
    return 0;
}
uint64_t HardwareBuffer::GetPhyAddr() const
{
    return 0;
}
int32_t HardwareBuffer::GetKey() const
{
    return 0;
}
void *HardwareBuffer::GetVirAddr() const
{
    return nullptr;
}

int HardwareBuffer::GetFileDescriptor() const
{
    return -1;
}
uint32_t HardwareBuffer::GetSize() const
{
    return 0;
}
GSError HardwareBuffer::SetInt32(uint32_t key, int32_t value)
{
    return GSERROR_OK;
}
GSError HardwareBuffer::GetInt32(uint32_t key, int32_t &value)
{
    return GSERROR_OK;
}
GSError HardwareBuffer::SetInt64(uint32_t key, int64_t value)
{
    return GSERROR_OK;
}
GSError HardwareBuffer::GetInt64(uint32_t key, int64_t &value)
{
    return GSERROR_OK;
}
sptr<EglData> HardwareBuffer::GetEglData() const
{
    return nullptr;
}

// support ipc data
GSError HardwareBuffer::ExtraGet(std::string key, int32_t &value) const
{
    return GSERROR_OK;
}
GSError HardwareBuffer::ExtraGet(std::string key, int64_t &value) const
{
    return GSERROR_OK;
}
GSError HardwareBuffer::ExtraGet(std::string key, double &value) const
{
    return GSERROR_OK;
}
GSError HardwareBuffer::ExtraGet(std::string key, std::string &value) const
{
    return GSERROR_OK;
}
GSError HardwareBuffer::ExtraSet(std::string key, int32_t value)
{
    return GSERROR_OK;
}
GSError HardwareBuffer::ExtraSet(std::string key, int64_t value)
{
    return GSERROR_OK;
}
GSError HardwareBuffer::ExtraSet(std::string key, double value)
{
    return GSERROR_OK;
}
GSError HardwareBuffer::ExtraSet(std::string key, std::string value)
{
    return GSERROR_OK;
}

void HardwareBuffer::SetAcquireFence(int32_t fd)
{
    acquireFence_ = fd;
}

int32_t HardwareBuffer::GetAcquireFence() const
{
    return acquireFence_;
}

void HardwareBuffer::SetReleaseFence(int32_t fd)
{
    relesaeFence_ = fd;
}

int32_t HardwareBuffer::GetReleaseFence() const
{
    return relesaeFence_;
}
}