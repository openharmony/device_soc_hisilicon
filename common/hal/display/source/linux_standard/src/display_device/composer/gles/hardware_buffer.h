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
#ifndef HARDWARE_BUFFER_H
#define HARDWARE_BUFFER_H
#include "surface_buffer.h"
namespace OHOS {
class HardwareBuffer : public OHOS::SurfaceBuffer {
public:
    static OHOS::sptr<OHOS::SurfaceBuffer> Create(const BufferHandle &handle);
    BufferHandle *GetBufferHandle() const override;
    explicit HardwareBuffer(const BufferHandle &handle);
    virtual ~HardwareBuffer();

    virtual int32_t GetWidth() const override;
    virtual int32_t GetHeight() const override;
    virtual int32_t GetStride() const override;
    virtual int32_t GetSurfaceBufferWidth() const override;
    virtual int32_t GetSurfaceBufferHeight() const override;
    virtual SurfaceColorGamut GetSurfaceBufferColorGamut() const override;
    virtual GSError SetSurfaceBufferWidth(int32_t width) override;
    virtual GSError SetSurfaceBufferHeight(int32_t height) override;
    virtual GSError SetSurfaceBufferColorGamut(SurfaceColorGamut colorGamut) override;
    virtual int32_t GetFormat() const override;
    virtual int64_t GetUsage() const override;
    virtual uint64_t GetPhyAddr() const override;
    virtual int32_t GetKey() const override;
    virtual void *GetVirAddr() const override;
    virtual int GetFileDescriptor() const override;
    virtual uint32_t GetSize() const override;
    virtual GSError SetInt32(uint32_t key, int32_t value) override;
    virtual GSError GetInt32(uint32_t key, int32_t &value) override;
    virtual GSError SetInt64(uint32_t key, int64_t value) override;
    virtual GSError GetInt64(uint32_t key, int64_t &value) override;
    virtual sptr<EglData> GetEglData() const override;

    // support ipc data
    virtual GSError ExtraGet(std::string key, int32_t &value) const override;
    virtual GSError ExtraGet(std::string key, int64_t &value) const override;
    virtual GSError ExtraGet(std::string key, double &value) const override;
    virtual GSError ExtraGet(std::string key, std::string &value) const override;
    virtual GSError ExtraSet(std::string key, int32_t value) override;
    virtual GSError ExtraSet(std::string key, int64_t value) override;
    virtual GSError ExtraSet(std::string key, double value) override;
    virtual GSError ExtraSet(std::string key, std::string value) override;
    void SetAcquireFence(int32_t fd);
    int32_t GetAcquireFence() const;
    void SetReleaseFence(int32_t fd);
    int32_t GetReleaseFence() const;

private:
    BufferHandle *handle_ = nullptr;
    int32_t acquireFence_ = -1;
    int32_t relesaeFence_ = -1;
};
}
#endif // HARDWARE_BUFFER_H