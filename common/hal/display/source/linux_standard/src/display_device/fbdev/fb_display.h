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

#ifndef FB_DISPLAY_H
#define FB_DISPLAY_H
#include <unordered_map>
#include <memory>
#include <vector>
#include "hdi_display.h"
#include "hdi_composer.h"
#include "fb_composition.h"

namespace OHOS {
namespace HDI {
namespace DISPLAY {
class FbDisplay : public HdiDisplay {
public:
    explicit FbDisplay(std::vector<int> &fds);
    virtual ~FbDisplay();
    int32_t Init() override;
    int32_t GetDisplayCapability(DisplayCapability *info) override;
    int32_t GetDisplaySupportedModes(uint32_t *num, DisplayModeInfo *modes) override;
    int32_t GetDisplayMode(uint32_t *modeId) override;
    int32_t SetDisplayMode(uint32_t modeId) override;
    int32_t GetDisplayPowerStatus(DispPowerStatus *status) override;
    int32_t SetDisplayPowerStatus(DispPowerStatus status) override;
    int32_t GetDisplayBacklight(uint32_t *value) override;
    int32_t SetDisplayBacklight(uint32_t value) override;
    virtual int32_t RegDisplayVBlankCallback(VBlankCallback cb, void *data) override;
    virtual bool IsConnected() override;
    virtual int32_t SetDisplayVsyncEnabled(bool enabled) override;
protected:
    std::unique_ptr<HdiLayer> CreateHdiLayer(LayerType type) override;
private:
    // the first fd is the master device fd
    std::vector<int> deviceFds_;
    std::vector<DisplayModeInfo> modes_;
    uint32_t mActiveModeId = INVALID_MODE_ID;
    DisplayCapability displayCapability_;
    DispPowerStatus mPowerstatus = POWER_STATUS_ON;
};
} // namespace OHOS
} // namespace HDI
} // namespace DISPLAY

#endif // HDI_DISPLAY_H