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

#ifndef HDI_VIDEO_COMPOSITION_H
#define HDI_VIDEO_COMPOSITION_H
#include <unordered_map>
#include "display_layer.h"
#include "hdi_composer.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class HdiVideoComposition : public HdiComposition {
public:
    int32_t Init(void) override;
    int32_t SetLayers(std::vector<HdiLayer *> &layers, HdiLayer &clientLayer) override;
    int32_t Apply(bool modeSet) override;
    ~HdiVideoComposition() override {}

private:
    static constexpr const char *VO_LAYER_LIB_NAME = "libdisplay_layer_video.z.so";
    static constexpr const char *VO_LAYER_FUNC_INIT = "LayerInitialize";
    static constexpr const char *VO_LAYER_FUNC_DEINIT = "LayerUninitialize";

    int32_t VideoModuleInit(void);
    bool CanHandle(HdiLayer &hdiLayer);
    void InitGfxSurface(ISurface &surface, HdiLayerBuffer &buffer);
    uint32_t CreateVideoLayerForHdiLayer(HdiLayer &hdiLayer);
    uint32_t GetVoLayerId(HdiLayer &hdiLayer);
    void CLoseUnUsedLayer(std::vector<HdiLayer *> &layers);
    std::unordered_map<uint32_t, uint32_t> layerMaps_;
    LayerFuncs *layerFuncs_ = nullptr;
    void *videoModule_ = nullptr;
};
} // namespace OHOS
} // namespace HDI
} // namespace DISPLAY

#endif // HDI_VIDEO_COMPOSITION_H