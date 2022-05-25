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
#ifndef ALLOCATOR_H
#define ALLOCATOR_H
#include <cerrno>
#include <cstdint>
#include <sys/mman.h>
#include <linux/dma-buf.h>
#include "securec.h"
#include "display_type.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
template <typename T>
T AlignUp(T value, T align)
{
    if (align == 0) {
        return 0;
    }
    return (value + align - 1) / align * align;
}

template <typename T>
T DivRoundUp(T numerator, T denominator)
{
    if (denominator == 0) {
        return 0;
    }
    return (numerator + denominator - 1) / denominator;
}

struct BufferInfo {
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    uint32_t widthStride_ = 0;
    uint32_t heightStride_ = 0;
    uint32_t bitsPerPixel_ = 0;
    uint32_t bytesPerPixel_ = 0;
    uint32_t size_ = 0;
    uint64_t usage_ = 0;
    PixelFormat format_ = PIXEL_FMT_BUTT;
};

class Allocator {
public:
    virtual int32_t AllocMem(const AllocInfo &info, BufferHandle **handle);
    virtual int32_t Allocate(const BufferInfo &bufferInfo, BufferHandle &handle);
    virtual int32_t Init();
    virtual int32_t FreeMem(BufferHandle *handle);
    virtual void *Mmap(BufferHandle &handle);
    virtual int32_t Unmap(BufferHandle &handle);
    virtual int32_t InvalidateCache(BufferHandle &handle);
    virtual int32_t FlushCache(BufferHandle &handle);
    virtual ~Allocator() {}
    static constexpr uint32_t BITS_PER_BYTES = 8;
    static constexpr uint32_t HEIGHT_ALIGN = 2;
    static constexpr uint32_t WIDTH_ALIGN = 8;

private:
    int32_t DmaBufferSync(const BufferHandle &handle, uint64_t flag);
    uint32_t UpdatePixelInfo(BufferInfo &bufferInfo);
    int32_t UpdateStrideAndSize(BufferInfo &bufferInfo);
    int32_t UpdateRGBStrideAndSize(BufferInfo &bufferInfo);
    int32_t UpdateYuvStrideAndSize(BufferInfo &bufferInfo);
    int32_t ConvertToBufferInfo(BufferInfo &bufferInfo, const AllocInfo &info);
    bool IsYuv(PixelFormat format);
    void DumpBufferHandle(BufferHandle &handle) const;
};
} // namespace DISPLAY
} // namespace HDI
} // namespace OHOS
#endif // ALLOCATOR_H