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

#include "fb_display.h"
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cerrno>
#include <securec.h>
#include <linux/fb.h>
#include "display_common.h"
#include "hdi_gfx_composition.h"
#include "fb_composition.h"
#include "vsync/sorft_vsync.h"
#include "hdi_gfx_composition.h"
#include "display_adapter.h"

namespace OHOS {
namespace HDI {
namespace DISPLAY {
std::unique_ptr<HdiLayer> FbDisplay::CreateHdiLayer(LayerType type)
{
    DISPLAY_LOGD();
    auto layer = HdiDisplay::CreateHdiLayer(type);
    layer->SetReleaseFence(-1); // the fd display will return the current buffer fence.
    return layer;
}

FbDisplay::FbDisplay(std::vector<int> &fds)
{
    DISPLAY_LOGD();
    deviceFds_ = fds;
}

FbDisplay::~FbDisplay()
{
    DISPLAY_LOGD();
}

int32_t FbDisplay::Init()
{
    int ret;
    int deviceFd = deviceFds_[0];
    struct fb_var_screeninfo varInfo;
    const uint32_t defaultFreshRate = 60 * 1000;
    ret = HdiDisplay::Init();
    DISPLAY_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_LOGE("init failed"));
    ret = DisplayAdapter::GetInstance()->Ioctl(deviceFd, FBIOGET_VSCREENINFO, &varInfo);
    DISPLAY_CHK_RETURN((ret < 0), DISPLAY_FAILURE, DISPLAY_LOGE("failed to get screen information"));
    DisplayModeInfo mode;
    mode.freshRate = defaultFreshRate;
    mode.width = varInfo.xres;
    mode.height = varInfo.yres;
    mode.id = 0;
    modes_.push_back(mode);
    DISPLAY_LOGD("xres : %{public}d yres : %{public}d fps : %{public}d", varInfo.xres, varInfo.yres, mode.freshRate);

    displayCapability_.phyWidth = varInfo.xres;
    displayCapability_.phyHeight = varInfo.yres;
    displayCapability_.type = DISP_INTF_PANEL;
    mActiveModeId = 0;
    auto preComp = std::make_unique<HdiGfxComposition>();
    DISPLAY_CHK_RETURN((preComp == nullptr), DISPLAY_FAILURE,
        DISPLAY_LOGE("can not new HdiGfxComposition errno %{public}d", errno));
    ret = preComp->Init();
    DISPLAY_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_LOGE("can not init HdiGfxComposition"));

    auto postComp = std::make_unique<FbComposition>(deviceFds_);
    DISPLAY_CHK_RETURN((postComp == nullptr), DISPLAY_FAILURE,
        DISPLAY_LOGE("can not new HdiDrmComposition errno %{public}d", errno));
    ret = postComp->Init();
    DISPLAY_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_LOGE("can not init HdiDrmComposition"));
    mComposer = std::make_unique<HdiComposer>(std::move(preComp), std::move(postComp));
    mClientLayer->SetReleaseFence(-1);
    return DISPLAY_SUCCESS;
}

int32_t FbDisplay::GetDisplayCapability(DisplayCapability *info)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((info == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("inof is nullptr"));
    *info = displayCapability_;
    return DISPLAY_SUCCESS;
}

int32_t FbDisplay::GetDisplaySupportedModes(uint32_t *num, DisplayModeInfo *modes)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((num == NULL), DISPLAY_NULL_PTR, DISPLAY_LOGE("num and modes is nullptr"));
    DISPLAY_CHK_RETURN((*num < 0), DISPLAY_FAILURE, DISPLAY_LOGE("the num is invalid"));
    if (modes == NULL) {
        *num = modes_.size();
        return DISPLAY_SUCCESS;
    }
    if (*num > modes_.size()) {
        *num = modes_.size();
    }
    memcpy_s(modes, sizeof(DisplayModeInfo) * (*num), modes_.data(), sizeof(DisplayModeInfo) * (*num));
    return DISPLAY_SUCCESS;
}

int32_t FbDisplay::GetDisplayMode(uint32_t *modeId)
{
    DISPLAY_LOGD();
    *modeId = mActiveModeId;
    if (mActiveModeId == static_cast<uint32_t>(INVALID_MODE_ID)) {
        DISPLAY_LOGE("now has not active mode");
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

int32_t FbDisplay::SetDisplayMode(uint32_t modeId)
{
    DISPLAY_LOGD();
    if (modeId < modes_.size()) {
        mActiveModeId = modeId;
    } else {
        DISPLAY_LOGE("the modeId is invalid");
    }
    return DISPLAY_SUCCESS;
}

int32_t FbDisplay::GetDisplayPowerStatus(DispPowerStatus *status)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((status == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("the status is nullptr"));
    *status = mPowerstatus;
    return DISPLAY_SUCCESS;
}

int32_t FbDisplay::SetDisplayPowerStatus(DispPowerStatus status)
{
    DISPLAY_LOGD();
    mPowerstatus = status;
    return DISPLAY_SUCCESS;
}

int32_t FbDisplay::GetDisplayBacklight(uint32_t *value)
{
    DISPLAY_LOGD();
    return DISPLAY_NOT_SUPPORT;
}

int32_t FbDisplay::SetDisplayBacklight(uint32_t value)
{
    DISPLAY_LOGD();
    return DISPLAY_NOT_SUPPORT;
}

int32_t FbDisplay::RegDisplayVBlankCallback(VBlankCallback cb, void *data)
{
    DISPLAY_LOGD("the VBlankCallback %{public}p ", cb);
    std::shared_ptr<VsyncCallBack> vsyncCb = std::make_shared<VsyncCallBack>(cb, nullptr);
    SorftVsync::GetInstance().ReqesterVBlankCb(vsyncCb);
    return DISPLAY_SUCCESS;
}

bool FbDisplay::IsConnected()
{
    DISPLAY_LOGD();
    return true;
}

int32_t FbDisplay::SetDisplayVsyncEnabled(bool enabled)
{
    DISPLAY_LOGD("enable %{public}d", enabled);
    SorftVsync::GetInstance().EnableVsync(enabled);
    return DISPLAY_SUCCESS;
}
} // namespace OHOS
} // namespace HDI
} // namespace DISPLAY
