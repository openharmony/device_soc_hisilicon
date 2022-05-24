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
#include <iremote_proxy.h>
#include "display_layer.h"
#include "idisplay_layer.h"
#include "display_common.h"
using namespace OHOS::HDI::Display::V1_0;
static OHOS::sptr<IDisplayLayer> g_layerService = nullptr;
constexpr const char *DISPLAY_LAYER_SERVICE_NAME = "hdi_video_layer_service";

static int32_t InitDisplayIfNeed(void)
{
    DISPLAY_LOGD();
    if (g_layerService == nullptr) {
        g_layerService = IDisplayLayer::Get(DISPLAY_LAYER_SERVICE_NAME);
        if (g_layerService == nullptr) {
            DISPLAY_LOGE("get layer service failed");
            return DISPLAY_FAILURE;
        }
        int32_t ret = g_layerService->InitDisplay(0);
        if (ret != DISPLAY_SUCCESS) {
            DISPLAY_LOGE("init display fail, ret=%{public}d",  ret);
            return DISPLAY_FAILURE;
        }
    }
    return DISPLAY_SUCCESS;
}

static int32_t DeInitDisplay(void)
{
    if (g_layerService != nullptr) {
        return g_layerService->DeinitDisplay(0);
    }
    return DISPLAY_SUCCESS;
}


static int32_t CreateLayer(uint32_t devId, const LayerInfo *layerInfo, uint32_t *layerId)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((layerInfo == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("layerInfo is nullptr"));
    DISPLAY_CHK_RETURN((layerId == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("layerId is nullptr"));
    LayerInfo info = *layerInfo;
    int32_t ret = InitDisplayIfNeed();
    DISPLAY_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_LOGE("display init failed"));
    return g_layerService->CreateLayer(devId, info, *layerId);
}

static int32_t CloseLayer(uint32_t devId, uint32_t layerId)
{
    DISPLAY_LOGD();
    g_layerService->CloseLayer(devId, layerId);
    return DISPLAY_SUCCESS;
}

static int32_t SetLayerSize(uint32_t devId, uint32_t layerId, IRect *rect)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((rect == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("rect is nullptr"));
    g_layerService->SetLayerRect(devId, layerId, *rect);
    return DISPLAY_SUCCESS;
}

static int32_t SetLayerCrop(uint32_t devId, uint32_t layerId, IRect *rect)
{
    DISPLAY_LOGD();
    return DISPLAY_SUCCESS;
}

static int32_t SetLayerZorder(uint32_t devId, uint32_t layerId, uint32_t zorder)
{
    DISPLAY_LOGD();
    g_layerService->SetLayerZorder(devId, layerId, zorder);
    return DISPLAY_SUCCESS;
}

static int32_t SetTransformMode(uint32_t devId, uint32_t layerId, TransformType type)
{
    DISPLAY_LOGD();
    g_layerService->SetTransformMode(devId, layerId, type);
    return DISPLAY_SUCCESS;
}

static int32_t SetLayerBuffer(uint32_t devId, uint32_t layerId, const BufferHandle *buffer, int32_t fence)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((buffer == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("buffer is nullptr"));
    g_layerService->SetLayerBuffer(devId, layerId, *buffer, fence);
    return DISPLAY_SUCCESS;
}
extern "C" {
int32_t LayerInitialize(LayerFuncs **funcs)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((funcs == nullptr), DISPLAY_FAILURE, DISPLAY_LOGE("funcs is nullptr"));
    LayerFuncs *lFunc = (LayerFuncs *)calloc(1, sizeof(LayerFuncs));
    if (lFunc == nullptr) {
        DISPLAY_LOGE("can not calloc LayerFuncs");
        return DISPLAY_FAILURE;
    }
    lFunc->CreateLayer = CreateLayer;
    lFunc->CloseLayer = CloseLayer;
    lFunc->SetLayerSize = SetLayerSize;
    lFunc->SetLayerCrop = SetLayerCrop;
    lFunc->SetLayerZorder = SetLayerZorder;
    lFunc->SetTransformMode = SetTransformMode;
    lFunc->SetLayerBuffer = SetLayerBuffer;
    *funcs = lFunc;
    DISPLAY_LOGD("LayerInitialize success");
    return DISPLAY_SUCCESS;
}

int32_t LayerUninitialize(LayerFuncs *funcs)
{
    DISPLAY_CHK_RETURN((funcs == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("funcs is nullptr"));
    DISPLAY_LOGD();
    free(funcs);
    int32_t ret = DeInitDisplay();
    DISPLAY_CHK_RETURN((ret != DISPLAY_SUCCESS), ret, DISPLAY_LOGE("uninitialize failed"));
    return DISPLAY_SUCCESS;
}
}
