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

#include "sorft_vsync.h"
#include <chrono>
#include <sys/time.h>
#include "display_common.h"

namespace OHOS {
namespace HDI {
namespace DISPLAY {
SorftVsync::SorftVsync() {}

int32_t SorftVsync::Init()
{
    thread_ = std::make_unique<std::thread>([this]() { WorkThread(); });
    DISPLAY_CHK_RETURN((thread_ == nullptr), DISPLAY_FAILURE, DISPLAY_LOGE("can not create thread"));
    running_ = true;
    return DISPLAY_SUCCESS;
}

SorftVsync &SorftVsync::GetInstance()
{
    static SorftVsync instance;
    static std::once_flag once;
    std::call_once(once, [&]() {
        int ret = instance.Init();
        if (ret != DISPLAY_SUCCESS) {
            DISPLAY_LOGE("Vsync Worker Init failed");
        }
    });
    return instance;
}

SorftVsync::~SorftVsync()
{
    DISPLAY_LOGD();
    {
        std::lock_guard<std::mutex> lg(mutext_);
        running_ = false;
    }
    condition_.notify_one();
    if (thread_ != nullptr) {
        thread_->join();
    }
}

bool SorftVsync::CheckRuning()
{
    std::unique_lock<std::mutex> ul(mutext_);
    condition_.wait(ul, [this]() { return (enable_ || !running_); });
    return running_;
}

void SorftVsync::EnableVsync(bool enable)
{
    DISPLAY_LOGD();
    {
        std::lock_guard<std::mutex> lg(mutext_);
        enable_ = enable;
    }
    condition_.notify_one();
}

void SorftVsync::WorkThread()
{
    DISPLAY_LOGD();
    unsigned int seq = 0;
    uint64_t time = 0;
    const int SIXTY_HZ_OF_TIME = 16666;
    struct timespec ts;
    while (CheckRuning()) {
        // wait the vblank
        usleep(SIXTY_HZ_OF_TIME);
        seq++;
        if (callback_ != nullptr) {
            clock_gettime(CLOCK_MONOTONIC, &ts);
            time = ts.tv_nsec;
            callback_->Vsync(seq, time);
        } else {
            DISPLAY_LOGE("the callbac is nullptr");
        }
    }
}

void SorftVsync::ReqesterVBlankCb(std::shared_ptr<VsyncCallBack> &cb)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN_NOT_VALUE((cb == nullptr), DISPLAY_LOGE("the VBlankCallback is nullptr "));
    callback_ = cb;
}
} // namespace OHOS
} // namespace HDI
} // namespace DISPLAY
