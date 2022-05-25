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

#ifndef FB_DEVICE_H
#define FB_DEVICE_H
#include <memory>
#include <unordered_map>
#include "hdi_device_common.h"
#include "hdi_device_interface.h"
#include "hdi_display.h"
#include "fb_display.h"
#include "display_module_loader.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class FbDevice : public HdiDeviceInterface {
public:
    static std::shared_ptr<HdiDeviceInterface> Create();
    explicit FbDevice(int fd);
    virtual ~FbDevice() override;
    virtual int32_t Init() override;
    virtual void DeInit() override;
    std::unordered_map<uint32_t, std::shared_ptr<HdiDisplay>> DiscoveryDisplay() override;

private:
    IdMapPtr<HdiDisplay> displays_;
    int deviceFd_ = -1;
};
} // namespace DISPLAY
} // namespace HDI
} // namespace OHOS

#endif // FB_DEVICE_H