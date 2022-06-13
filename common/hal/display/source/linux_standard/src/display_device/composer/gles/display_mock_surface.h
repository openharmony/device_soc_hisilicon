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
#ifndef DISPLAY_MOCK_SURFACE
#define DISPLAY_MOCK_SURFACE
#include <queue>
#include <unordered_map>
#include "hardware_buffer.h"
#include "surface.h"
#include "hdi_layer.h"
#include "display_gralloc.h"

namespace OHOS {
class DisplayMockSurface : public Surface {
public:
    bool Init(uint32_t width, uint32_t height);
    bool AllocBuffer(AllocInfo &info);

    ~DisplayMockSurface() = default;

    bool IsConsumer() const;
    sptr<IBufferProducer> GetProducer() const;

    GSError RequestBuffer(sptr<SurfaceBuffer> &buffer, int32_t &fence, BufferRequestConfig &config);

    GSError CancelBuffer(sptr<SurfaceBuffer> &buffer);

    GSError FlushBuffer(sptr<SurfaceBuffer> &buffer, int32_t fence, BufferFlushConfig &config);

    GSError AcquireBuffer(sptr<SurfaceBuffer> &buffer, int32_t &fence, int64_t &timestamp, Rect &damage);
    GSError ReleaseBuffer(sptr<SurfaceBuffer> &buffer, int32_t fence);

    GSError AttachBuffer(sptr<SurfaceBuffer> &buffer);

    GSError DetachBuffer(sptr<SurfaceBuffer> &buffer);

    uint32_t GetQueueSize();
    GSError SetQueueSize(uint32_t queueSize);

    GSError SetDefaultWidthAndHeight(int32_t width, int32_t height);
    int32_t GetDefaultWidth();
    int32_t GetDefaultHeight();

    GSError SetDefaultUsage(uint32_t usage);
    uint32_t GetDefaultUsage();

    GSError SetUserData(const std::string &key, const std::string &val);
    std::string GetUserData(const std::string &key);

    GSError GetName(std::string &name);

    GSError RegisterConsumerListener(sptr<IBufferConsumerListener> &listener);
    GSError RegisterConsumerListener(IBufferConsumerListenerClazz *listener);
    GSError RegisterReleaseListener(OnReleaseFunc func);
    GSError UnregisterConsumerListener();

    uint64_t GetUniqueId() const;

    void Dump(std::string &result) const;

    GSError CleanCache();
    void AcquireBufferHandle(HDI::DISPLAY::HdiLayer &layer);
    void SetReleaseFence(int32_t fence);

private:
    static constexpr const uint32_t BUFFER_NUM = 3;
    std::queue<OHOS::sptr<OHOS::SurfaceBuffer>> hardwareBufferQueue_;
    sptr<OHOS::SurfaceBuffer> curentPresentBuffer_ = nullptr;
    GrallocFuncs *grallocFuncs_ = nullptr;
    uint32_t width_ = 0;
    uint32_t height_ = 0;
};
} // OHOS

#endif // DISPLAY_MOCK_SURFACE