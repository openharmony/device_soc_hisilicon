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

#ifndef SORFT_VSYNC_H
#define SORFT_VSYNC_H

#include <thread>
#include <memory>
#include <mutex>
#include "hdi_display.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class SorftVsync {
public:
    SorftVsync();
    virtual ~SorftVsync();
    static SorftVsync &GetInstance();

    int32_t Init();
    void EnableVsync(bool enable);
    void WorkThread();
    void ReqesterVBlankCb(std::shared_ptr<VsyncCallBack> &cb);
    bool CheckRuning();

private:
    std::unique_ptr<std::thread> thread_;
    bool enable_ = false;
    std::mutex mutext_;
    std::condition_variable condition_;
    std::shared_ptr<VsyncCallBack> callback_;
    bool running_ = false;
};
} // namespace OHOS
} // namespace HDI
} // namespace DISPLAY

#endif // SORFT_VSYNC_H