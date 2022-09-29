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
#include "allocator.h"
#include <cinttypes>
#include "display_common.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
void Allocator::DumpBufferHandle(BufferHandle &handle) const
{
    DISPLAY_LOGD("size %{public}d, width %{public}d height %{public}d stride %{public}d fd %{public}d "
        "format %{public}d phy 0x%{public}" PRIx64 " usage 0x%{public}" PRIx64 "  viraddr 0x%{public}p",
        handle.size, handle.width, handle.height, handle.stride, handle.fd, handle.format, handle.phyAddr,
        handle.usage, handle.virAddr);
}

bool Allocator::IsYuv(PixelFormat format)
{
    switch (format) {
        case PIXEL_FMT_YCBCR_420_P:
        case PIXEL_FMT_YUV_422_I:
        case PIXEL_FMT_YCBCR_422_SP:
        case PIXEL_FMT_YCRCB_422_SP:
        case PIXEL_FMT_YCBCR_420_SP:
        case PIXEL_FMT_YCRCB_420_SP:
        case PIXEL_FMT_YCBCR_422_P:
        case PIXEL_FMT_YCRCB_422_P:
        case PIXEL_FMT_YCRCB_420_P:
        case PIXEL_FMT_YUYV_422_PKG:
        case PIXEL_FMT_UYVY_422_PKG:
        case PIXEL_FMT_YVYU_422_PKG:
        case PIXEL_FMT_VYUY_422_PKG:
            return true;
        default:
            return false;
    }
}

uint32_t Allocator::UpdatePixelInfo(BufferInfo &bufferInfo)
{
    DISPLAY_LOGD("format is %{public}d", bufferInfo.format_);
    if (IsYuv(bufferInfo.format_)) {
        bufferInfo.bitsPerPixel_ = 8; // 8 bits per pixel
    } else {
        switch (bufferInfo.format_) {
            case PIXEL_FMT_RGBX_8888:
            case PIXEL_FMT_RGBA_8888:
            case PIXEL_FMT_BGRX_8888:
            case PIXEL_FMT_BGRA_8888:
                bufferInfo.bitsPerPixel_ = 32; // 32 bits per pixel
                break;
            case PIXEL_FMT_RGB_888:
                bufferInfo.bitsPerPixel_ = 24; // 24 bits per pixel
                break;
            case PIXEL_FMT_RGB_565:
            case PIXEL_FMT_BGRX_4444:
            case PIXEL_FMT_BGRA_4444:
            case PIXEL_FMT_RGBA_4444:
            case PIXEL_FMT_RGBX_4444:
            case PIXEL_FMT_BGRX_5551:
            case PIXEL_FMT_BGRA_5551:
                bufferInfo.bitsPerPixel_ = 16; // 16 bits per pixel
                break;
            default:
                DISPLAY_LOGE("the format can not support %{public}d", bufferInfo.format_);
                return DISPLAY_NOT_SUPPORT;
        }
    }

    bufferInfo.bytesPerPixel_ = bufferInfo.bitsPerPixel_ / BITS_PER_BYTES;
    return DISPLAY_SUCCESS;
}

int32_t Allocator::UpdateRGBStrideAndSize(BufferInfo &bufferInfo)
{
    bufferInfo.widthStride_ = AlignUp(bufferInfo.width_, WIDTH_ALIGN);
    bufferInfo.heightStride_ = AlignUp(bufferInfo.height_, HEIGHT_ALIGN);
    bufferInfo.size_ = bufferInfo.widthStride_ * bufferInfo.heightStride_ * bufferInfo.bytesPerPixel_;
    return DISPLAY_SUCCESS;
}

int32_t Allocator::UpdateYuvStrideAndSize(BufferInfo &bufferInfo)
{
    int32_t ret = DISPLAY_NOT_SUPPORT;
    constexpr uint32_t UV_DIV_420 = 2;
    DISPLAY_LOGD();
    bufferInfo.widthStride_ = AlignUp(bufferInfo.width_, WIDTH_ALIGN);
    bufferInfo.heightStride_ = AlignUp(bufferInfo.height_, HEIGHT_ALIGN);
    switch (bufferInfo.format_) {
        case PIXEL_FMT_YCBCR_420_P:
        case PIXEL_FMT_YCBCR_422_SP:
        case PIXEL_FMT_YCRCB_422_SP:
        case PIXEL_FMT_YCBCR_420_SP:
        case PIXEL_FMT_YCRCB_420_SP:
        case PIXEL_FMT_YCRCB_420_P:
            bufferInfo.heightStride_ = (bufferInfo.heightStride_ + bufferInfo.heightStride_ / UV_DIV_420);
            ret = DISPLAY_SUCCESS;
            break;
        case PIXEL_FMT_YCBCR_422_P:
        case PIXEL_FMT_YCRCB_422_P:
            bufferInfo.heightStride_ = (bufferInfo.heightStride_ + bufferInfo.heightStride_);
            ret = DISPLAY_SUCCESS;
            break;
        default:
            break;
    }
    bufferInfo.size_ = bufferInfo.widthStride_ *  bufferInfo.heightStride_;
    return ret;
}

int32_t Allocator::UpdateStrideAndSize(BufferInfo &bufferInfo)
{
    if (IsYuv(bufferInfo.format_)) {
        return UpdateYuvStrideAndSize(bufferInfo);
    } else {
        return UpdateRGBStrideAndSize(bufferInfo);
    }
}

int32_t Allocator::ConvertToBufferInfo(BufferInfo &bufferInfo, const AllocInfo &info)
{
    DISPLAY_LOGD();
    bufferInfo.width_ = info.width;
    bufferInfo.height_ = info.height;
    bufferInfo.format_ = info.format;
    bufferInfo.usage_ = info.usage;
    int32_t ret = UpdatePixelInfo(bufferInfo);
    DISPLAY_CHK_RETURN(
        (ret != DISPLAY_SUCCESS), DISPLAY_NOT_SUPPORT, DISPLAY_LOGE("failed to update pixel information"));
    ret = UpdateStrideAndSize(bufferInfo);
    DISPLAY_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_NOT_SUPPORT, DISPLAY_LOGE("failed to update stride and size"));
    return DISPLAY_SUCCESS;
}

int32_t Allocator::Init()
{
    DISPLAY_LOGD();
    return DISPLAY_SUCCESS;
}

int32_t Allocator::AllocMem(const AllocInfo &info, BufferHandle **handle)
{
    int ret;
    DISPLAY_LOGD();
    BufferInfo bufferInfo;
    ret = ConvertToBufferInfo(bufferInfo, info);
    DISPLAY_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_NOT_SUPPORT, DISPLAY_LOGE("failed to convert to bufferInfo"));

    BufferHandle *priBuffer = (BufferHandle *)malloc(sizeof(BufferHandle));
    DISPLAY_CHK_RETURN((priBuffer == nullptr), DISPLAY_NOT_SUPPORT, DISPLAY_LOGE("can not alloc memory"));
    (void)memset_s(priBuffer, sizeof(BufferHandle), 0, sizeof(BufferHandle));
    priBuffer->fd = -1;
    priBuffer->width = bufferInfo.width_;
    priBuffer->height = bufferInfo.height_;
    priBuffer->stride = bufferInfo.widthStride_ * bufferInfo.bytesPerPixel_;
    priBuffer->size = bufferInfo.size_;
    priBuffer->format = bufferInfo.format_;
    priBuffer->usage = bufferInfo.usage_;

    ret = Allocate(bufferInfo, *priBuffer);
    DumpBufferHandle(*priBuffer);
    if (ret != DISPLAY_SUCCESS) {
        DISPLAY_LOGE("AllocMem failed");
        free(priBuffer);
        priBuffer = nullptr;
        return ret;
    }
    *handle = priBuffer;
    return DISPLAY_SUCCESS;
}

int32_t Allocator::Allocate(const BufferInfo &bufferInfo, BufferHandle &handle)
{
    DISPLAY_LOGE("AllocMem do not implement");
    return DISPLAY_NOT_SUPPORT;
}

int32_t Allocator::FreeMem(BufferHandle *handle)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((handle == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("buffer is null"));
    if (handle->fd >= 0) {
        DISPLAY_LOGD("release the fd is %{public}d", handle->fd);
        close(handle->fd);
        handle->fd = -1;
    }
    const uint32_t reserveFds = handle->reserveFds;
    for (uint32_t i = 0; i < reserveFds; i++) {
        if (handle->reserve[i] >= 0) {
            DISPLAY_LOGD("release the fd is %{public}d", handle->reserve[i]);
            close(handle->reserve[i]);
            handle->reserve[i] = -1;
        }
    }
    free(handle);
    return DISPLAY_SUCCESS;
}

int32_t Allocator::DmaBufferSync(const BufferHandle &handle, uint64_t flag)
{
    if (handle.fd < 0) {
        DISPLAY_LOGW("dmabuffer fd is invalid can not sync");
        return DISPLAY_SUCCESS;
    }

    struct dma_buf_sync syncPrm = {0};
    errno_t eok = memset_s(&syncPrm, sizeof(syncPrm), 0, sizeof(syncPrm));
    DISPLAY_CHK_RETURN((eok != EOK), DISPLAY_PARAM_ERR, DISPLAY_LOGE("dma buffer sync memset_s failed"));
    if (handle.usage & HBM_USE_CPU_WRITE) {
        syncPrm.flags |= DMA_BUF_SYNC_WRITE;
    }
    if (handle.usage & HBM_USE_CPU_READ) {
        syncPrm.flags |= DMA_BUF_SYNC_READ;
    }
    const int32_t retryMaxCount = 6;
    int32_t retryCount = 0;
    for (retryCount = 0; retryCount < retryMaxCount; retryCount++) {
        int ret = ioctl(handle.fd, DMA_BUF_IOCTL_SYNC, &syncPrm);
        if ((ret != -EAGAIN) && (ret != -EINTR)) {
            break;
        }
    }

    if (retryCount >= retryMaxCount) {
        DISPLAY_LOGE("sync failed");
        return DISPLAY_SYS_BUSY;
    }
    return DISPLAY_SUCCESS;
}

int32_t Allocator::InvalidateCache(BufferHandle &handle)
{
    DISPLAY_LOGD();
    return DmaBufferSync(handle, DMA_BUF_SYNC_END);
}

int32_t Allocator::FlushCache(BufferHandle &handle)
{
    DISPLAY_LOGD();
    return DmaBufferSync(handle, DMA_BUF_SYNC_START);
}

void *Allocator::Mmap(BufferHandle &handle)
{
    DISPLAY_LOGD();
    void *virAddr = nullptr;
    if (handle.virAddr != nullptr) {
        DISPLAY_LOGW("the buffer has virtual addr");
        return handle.virAddr;
    }
    virAddr = mmap(nullptr, handle.size, PROT_READ | PROT_WRITE, MAP_SHARED, handle.fd, 0);
    if (virAddr == static_cast<void*>(MAP_FAILED)) {
        DISPLAY_LOGE("mmap failed errno %{public}d, fd : %{public}d", errno, handle.fd);
        handle.virAddr = nullptr;
        return nullptr;
    }
    handle.virAddr = virAddr;
    return virAddr;
}

int32_t Allocator::Unmap(BufferHandle &handle)
{
    if (handle.virAddr == nullptr) {
        DISPLAY_LOGE("virAddr is nullptr , has not map the buffer");
        return DISPLAY_PARAM_ERR;
    }
    int ret = munmap(handle.virAddr, handle.size);
    if (ret != 0) {
        DISPLAY_LOGE("munmap failed err: %{public}d", errno);
        return DISPLAY_FAILURE;
    }
    handle.virAddr = nullptr;
    return DISPLAY_SUCCESS;
}
} // namespace DISPLAY
} // namespace HDI
} // namespace OHOS