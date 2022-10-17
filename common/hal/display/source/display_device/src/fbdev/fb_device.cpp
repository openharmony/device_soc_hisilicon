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

#include "fb_device.h"
#include <cerrno>
#include <fcntl.h>
#include <memory>
#include <securec.h>
#include <string>
#include "display_common.h"
#include "fb_display.h"
#include "display_adapter.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
std::shared_ptr<HdiDeviceInterface> FbDevice::Create()
{
    constexpr const char *FBDEV_PATH = "/dev/graphics/fb0";
    std::unique_ptr<DisplayAdapter>& adapter = DisplayAdapter::GetInstance();
    if (adapter == nullptr) {
        DISPLAY_LOGE("can not get display adapter the fbdevice need it");
        return nullptr;
    }
    int fd = adapter->OpenDevice(FBDEV_PATH, O_RDWR, 0);
    if (fd < 0) {
        DISPLAY_LOGE("failed to open fbdev %{public}s", FBDEV_PATH);
        return nullptr;
    }

    auto device = std::make_shared<FbDevice>(fd);
    return device;
}

FbDevice::FbDevice(int fd)
{
    DISPLAY_LOGD();
    deviceFd_ = fd;
}

FbDevice::~FbDevice()
{
    DISPLAY_LOGD();
    if (deviceFd_ > 0) {
        close(deviceFd_);
        deviceFd_ = -1;
    }
}

std::unordered_map<uint32_t, std::shared_ptr<HdiDisplay>> FbDevice::DiscoveryDisplay()
{
    DISPLAY_LOGD();
    return displays_;
}

int32_t FbDevice::Init()
{
    DISPLAY_LOGD();
    std::vector<int> fds = {deviceFd_};
    std::shared_ptr<HdiDisplay> display = std::make_shared<FbDisplay>(fds);
    int ret = display->Init();
    DISPLAY_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_LOGE("display init failed"));
    displays_.emplace(display->GetId(), std::move(display));
    return DISPLAY_SUCCESS;
}

void FbDevice::DeInit()
{
    DISPLAY_LOGD();
    displays_.clear();
}
} // namespace DISPLAY
} // namespace HDI
} // namespace OHOS
