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

#include "fb_composition.h"
#include <vector>
#include <memory>
#include <cerrno>
#include "hitrace_meter.h"
#include "display_adapter.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
int32_t FbComposition::FbFresh(int fd, HdiLayer &clientlayer, int &fence)
{
    DISPLAY_LOGD();
    DisplayFrameInfo fbFrameInfo;
    fbFrameInfo.rect.x = 0;
    fbFrameInfo.rect.y = 0;
    fbFrameInfo.rect.w = clientlayer.GetCurrentBuffer()->GetWidth();
    fbFrameInfo.rect.h = clientlayer.GetCurrentBuffer()->GetHeight();
    fbFrameInfo.inFence = clientlayer.GetAcquireFenceFd();
    fbFrameInfo.stride = clientlayer.GetCurrentBuffer()->GetStride();
    fbFrameInfo.bufaddr = clientlayer.GetCurrentBuffer()->GetMemHandle();
    fbFrameInfo.format = PIXEL_FMT_RGBA_8888;
    HitraceScoped trace(HITRACE_TAG_GRAPHIC_AGP, "fbfresh");
    if (DisplayAdapter::GetInstance()->FbFresh(fd, fbFrameInfo) != 0) {
        DISPLAY_LOGE(" HIFB_REFRESH_FRAMEINFO Error! %{public}d", errno);
    }
    fence = fbFrameInfo.outFence;
    return DISPLAY_SUCCESS;
}

FbComposition::FbComposition(const std::vector<int> &fbs)
{
    DISPLAY_LOGD();
    fds_ = fbs;
}

FbComposition::~FbComposition()
{
    DISPLAY_LOGD();
}

int32_t FbComposition::Init()
{
    DISPLAY_LOGD();
    return HdiComposition::Init();
}

int32_t FbComposition::SetLayers(std::vector<HdiLayer*> &layers, HdiLayer &clientLayer)
{
    DISPLAY_LOGD();
    mCompLayers.clear();
    mCompLayers.push_back(&clientLayer);
    return DISPLAY_SUCCESS;
}

int32_t FbComposition::Apply(bool modeSet)
{
    (void)modeSet;
    DISPLAY_LOGD("mCompLayers size %{public}zu", mCompLayers.size());
    for (uint32_t i = 0; i < mCompLayers.size(); i++) {
        int fence = -1;
        HdiLayer *layer = mCompLayers[i];
        HitraceScoped trace(HITRACE_TAG_GRAPHIC_AGP, "fb apply");
        int ret = FbFresh(fds_[i], *layer, fence);
        layer->SetReleaseFence(fence);
        DISPLAY_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_LOGE("fb fresh failed"));
    }
    return DISPLAY_SUCCESS;
}
} // OHOS
} // HDI
} // DISPLAY
