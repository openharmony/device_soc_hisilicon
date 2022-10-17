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

#include "hdi_video_composition.h"
#include <set>
#include <cinttypes>
#include <dlfcn.h>

namespace OHOS {
namespace HDI {
namespace DISPLAY {
int32_t HdiVideoComposition::VideoModuleInit(void)
{
    DISPLAY_LOGD();
    videoModule_ = dlopen(VO_LAYER_LIB_NAME, RTLD_NOW | RTLD_NOLOAD);
    if (videoModule_ != nullptr) {
        DISPLAY_LOGI("Module '%{public}s' already loaded", VO_LAYER_LIB_NAME);
    } else {
        DISPLAY_LOGI("Loading module '%{public}s'", VO_LAYER_LIB_NAME);
        videoModule_ = dlopen(VO_LAYER_LIB_NAME, RTLD_NOW);
        if (videoModule_ == nullptr) {
            DISPLAY_LOGE("Failed to load module: %{public}s", dlerror());
            return DISPLAY_FAILURE;
        }
    }

    using InitFunc = int32_t (*)(LayerFuncs **funcs);
    InitFunc func = reinterpret_cast<InitFunc>(dlsym(videoModule_, VO_LAYER_FUNC_INIT));
    if (func == nullptr) {
        DISPLAY_LOGE("Failed to lookup %{public}s function: %s", VO_LAYER_FUNC_INIT, dlerror());
        dlclose(videoModule_);
        videoModule_ = nullptr;
        return DISPLAY_FAILURE;
    }
    return func(&layerFuncs_);
}

uint32_t HdiVideoComposition::CreateVideoLayerForHdiLayer(HdiLayer &hdiLayer)
{
    uint32_t voLayerId;
    DISPLAY_LOGD("layerId %{public}d", hdiLayer.GetId());
    IRect rect = hdiLayer.GetLayerDisplayRect();
    LayerInfo layerInfo = { rect.w, rect.h, LAYER_TYPE_OVERLAY, 8, PIXEL_FMT_YCRCB_420_P }; // the bpp of yuv is 8
    int ret = layerFuncs_->CreateLayer(0, &layerInfo, &voLayerId);
    DISPLAY_CHK_RETURN(((ret != DISPLAY_SUCCESS) || (voLayerId == INVALIDE_LAYER_ID)), INVALIDE_LAYER_ID,
        DISPLAY_LOGE("can not create video layer"));
    layerMaps_[hdiLayer.GetId()] = voLayerId;
    DISPLAY_LOGD("map the voLayerId %{public}d to layerId %{public}d", voLayerId, hdiLayer.GetId());
    return voLayerId;
}

uint32_t HdiVideoComposition::GetVoLayerId(HdiLayer &hdiLayer)
{
    DISPLAY_LOGD("layerId %{public}d", hdiLayer.GetId());
    uint32_t voLayerId = INVALIDE_LAYER_ID;
    if (layerMaps_.find(hdiLayer.GetId()) != layerMaps_.end()) {
        voLayerId = layerMaps_[hdiLayer.GetId()];
    }
    return voLayerId;
}

int32_t HdiVideoComposition::Init(void)
{
    DISPLAY_LOGD();
    int ret = VideoModuleInit();
    DISPLAY_CHK_RETURN(((ret != DISPLAY_SUCCESS) || (layerFuncs_ == nullptr)), DISPLAY_FAILURE,
        DISPLAY_LOGE("can not init the video module"));
    return DISPLAY_SUCCESS;
}

int32_t HdiVideoComposition::SetLayers(std::vector<HdiLayer*> &layers, HdiLayer &clientLayer)
{
    DISPLAY_LOGD();
    mCompLayers.clear();
    std::vector<HdiLayer*> matchedLayers;
    for (auto layer : layers) {
        if ((layer != nullptr) && CanHandle(*layer)) {
            matchedLayers.push_back(layer);
        }
    }
    CLoseUnUsedLayer(matchedLayers);
    // make sure has the videoLayer for the matched layer
    for (auto layer : matchedLayers) {
        if ((GetVoLayerId(*layer) != INVALIDE_LAYER_ID) ||
            (CreateVideoLayerForHdiLayer(*layer) != INVALIDE_LAYER_ID)) {
            layer->SetLayerCompositionType(COMPOSITION_VIDEO);
            mCompLayers.push_back(layer);
        }
    }
    return DISPLAY_SUCCESS;
}

int32_t HdiVideoComposition::Apply(bool modeSet)
{
    DISPLAY_LOGD();
    for (auto hdiLayer : mCompLayers) {
        if (hdiLayer == nullptr) {
            DISPLAY_LOGE("the composer layer is nullptr");
            continue;
        }
        uint32_t voLayerId = GetVoLayerId(*hdiLayer);
        layerFuncs_->SetLayerSize(0, voLayerId, const_cast<IRect *>(&(hdiLayer->GetLayerDisplayRect())));
        layerFuncs_->SetLayerCrop(0, voLayerId, const_cast<IRect *>(&(hdiLayer->GetLayerCrop())));
        layerFuncs_->SetLayerZorder(0, voLayerId, hdiLayer->GetZorder());
        layerFuncs_->SetTransformMode(0, voLayerId, hdiLayer->GetTransFormType());
        // now the display_layer has no fence
        layerFuncs_->SetLayerBuffer(0, voLayerId, &(hdiLayer->GetCurrentBuffer()->mHandle),
            hdiLayer->GetAcquireFenceFd());
        hdiLayer->SetReleaseFence(-1); // todo check the fence whether use the last fence.
    }
    return DISPLAY_SUCCESS;
}

void HdiVideoComposition::CLoseUnUsedLayer(const std::vector<HdiLayer*> &layers)
{
    DISPLAY_LOGD();
    std::vector<uint32_t> neededRemove;
    for (auto iter = layerMaps_.begin(); iter != layerMaps_.end(); iter++) {
        bool found = false;
        for (auto layer : layers) {
            if (iter->first == layer->GetId()) {
                found = true;
            }
        }
        if (!found) {
            DISPLAY_LOGD("close the layer Id %{public}d", iter->second);
            layerFuncs_->CloseLayer(0, iter->second);
            neededRemove.push_back(iter->first);
            break;
        }
    }
    for (auto id : neededRemove) {
        DISPLAY_LOGD("erase the layer Id %{public}d", id);
        layerMaps_.erase(id);
    }
}

bool HdiVideoComposition::CanHandle(HdiLayer &hdiLayer)
{
    static std::set<PixelFormat> formats = {
        PIXEL_FMT_YUV_422_I,    /* *< YUV422 interleaved format */
        PIXEL_FMT_YCBCR_422_SP, /* *< YCBCR422 semi-planar format */
        PIXEL_FMT_YCRCB_422_SP, /* *< YCRCB422 semi-planar format */
        PIXEL_FMT_YCBCR_420_SP, /* *< YCBCR420 semi-planar format */
        PIXEL_FMT_YCRCB_420_SP, /* *< YCRCB420 semi-planar format */
        PIXEL_FMT_YCBCR_422_P,  /* *< YCBCR422 planar format */
        PIXEL_FMT_YCRCB_422_P,  /* *< YCRCB422 planar format */
        PIXEL_FMT_YCBCR_420_P,  /* *< YCBCR420 planar format */
        PIXEL_FMT_YCRCB_420_P,  /* *< YCRCB420 planar format */
        PIXEL_FMT_YUYV_422_PKG, /* *< YUYV422 packed format */
        PIXEL_FMT_UYVY_422_PKG, /* *< UYVY422 packed format */
        PIXEL_FMT_YVYU_422_PKG, /* *< YVYU422 packed format */
        PIXEL_FMT_VYUY_422_PKG, /* *< VYUY422 packed format */
    };

    if (hdiLayer.GetType() == LAYER_TYPE_SIDEBAND) {
        DISPLAY_LOGD("is sideband");
        return false;
    }

    if (hdiLayer.GetCurrentBuffer() == nullptr) {
        DISPLAY_LOGD("has no buffer");
        return false;
    }

    if (formats.find(static_cast<PixelFormat>(hdiLayer.GetCurrentBuffer()->GetFormat())) == formats.end()) {
        return false;
    }
    return true;
}
}
}
}
