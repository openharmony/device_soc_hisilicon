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
#ifndef HDI_GLES_COMPOSITION_H
#define HDI_GLES_COMPOSITION_H
#include <cstdint>
#include <memory>
#include <unordered_map>
#include "hdi_composer.h"
#include "display_type.h"
#include "gl_egl_state.h"
#include "gles_fbo.h"
#include "display_bytrace.h"
#include "display_mock_surface.h"
#include "gles_gfx_image.h"
#include "gles_program.h"

namespace OHOS {
namespace HDI {
namespace DISPLAY {
class HdiGlesComposition : public HdiComposition {
public:
    int32_t Init(uint32_t width, uint32_t height);
    int32_t SetLayers(std::vector<HdiLayer *> &layers, HdiLayer &clientLayer);
    int32_t Apply(bool modeSet);
    virtual ~HdiGlesComposition();

private:
    int32_t DrawLayer(const BufferHandle &bufferHandle, const IRect &displayRect, GlesProgram &program);
    int32_t DrawLayer(HdiLayer &layer);
    int32_t ClearLayer(HdiLayer &layer);
    bool CanHandle(HdiLayer &hdiLayer);
    std::vector<float> GetVertexArrays(const IRect &displayRect);
    std::unordered_map<uint32_t, std::unique_ptr<GlesGfxImage>> imageMaps_;
    std::unordered_map<uint32_t, std::unique_ptr<GlesFbo>> glesFbo_;
    int32_t InitProgram();
    std::unique_ptr<GlEglState> eglState_ = nullptr;
    std::unique_ptr<GlesProgram> programRgba_ = nullptr;
    std::unique_ptr<GlesProgram> programYuv_ = nullptr;
    std::unique_ptr<GlesProgram> programClear_ = nullptr;
    OHOS::sptr<OHOS::DisplayMockSurface> clientSurface_ = nullptr;
    NativeWindowType nativeWindow_ = 0;

    HdiLayer *mClientLayer = nullptr;
};
} // namespace OHOS
} // namespace HDI
} // namespace DISPLAY

#endif // HDI_GLES_COMPOSITION_H