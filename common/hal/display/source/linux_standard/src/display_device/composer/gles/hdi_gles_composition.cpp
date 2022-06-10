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
#include "hdi_gles_composition.h"
#include "gles_gfx_image.h"
#include "display_mock_surface.h"
#include "display_utils.h"
#include "window.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
static const char g_vertexShader[] = "attribute vec4 vertexPosition;   \n"
    "attribute vec2 textureCoord;   \n"
    "varying vec2 textureCoordVar;     \n"
    "void main()                  \n"
    "{                            \n"
    "   vec2 zeroToOne = vertexPosition.xy / vec2(1024, 600);\n"
    "   vec2 zeroToTwo = zeroToOne * 2.0;\n"
    "   vec2 clipSpace = zeroToTwo - 1.0;\n"
    "   gl_Position = vec4(clipSpace * vec2(1, -1), 0, 1);\n"
    "   textureCoordVar = textureCoord;  \n"
    "}                            \n";

static const char g_textureFragmentShaderRgba[] = "precision mediump float; \n"
    "varying vec2 textureCoordVar;                            \n"
    "uniform sampler2D textureSampler;                        \n"
    "void main()                                         \n"
    "{                                                   \n"
    "  gl_FragColor = texture2D( textureSampler, textureCoordVar );\n"
    "}                                                   \n";

/* Declare common fragment shader uniforms */
#define FRAGMENT_CONVERT_YUV                                        \
    "  gl_FragColor.r = y + 1.59602678 * v;\n"                      \
        "  gl_FragColor.g = y - 0.39176229 * u - 0.81296764 * v;\n" \
        "  gl_FragColor.b = y + 2.01723214 * u;\n"                  \
        "  gl_FragColor.a = 1.0;\n"                                 \
        " } \n"

static const char g_textureFragmentFhaderYuv[] = "precision mediump float;\n"
    "uniform sampler2D s_texureY;\n"
    "uniform sampler2D s_texureU;\n"
    "uniform sampler2D s_texureV;\n"
    "varying vec2 textureCoordVar;\n"
    "void main() {\n"
    "  float y = 1.16438356 * (texture2D(s_texureY, textureCoordVar).x - 0.0625);\n"
    "  float u = texture2D(s_texureU, textureCoordVar).x - 0.5;\n"
    "  float v = texture2D(s_texureV, textureCoordVar).x - 0.5;\n"
    "  gl_FragColor.r = y + 1.59602678 * v;\n"
    "  gl_FragColor.g = y - 0.39176229 * u - 0.81296764 * v;\n"
    "  gl_FragColor.b = y + 2.01723214 * u;\n"
    "  gl_FragColor.a = 1.0;\n"
    "} \n";

static const char g_clearfragmentShader[] = "precision mediump float;\n"
    "void main()\n"
    "{\n"
    "   gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);\n"
    "}\n";

int32_t HdiGlesComposition::InitProgram()
{
    bool ret;
    // init the rgba program
    programRgba_ = std::make_unique<GlesProgram>();
    DISPLAY_CHK_RETURN((programRgba_ == nullptr), DISPLAY_FAILURE, DISPLAY_LOGE("can not create program for rgba"));
    ret = programRgba_->Init(g_vertexShader, g_textureFragmentShaderRgba);
    DISPLAY_CHK_RETURN((!ret), DISPLAY_FAILURE, DISPLAY_LOGE("Failed to init rgba program"));
    
    programRgba_->Use();
    
    programRgba_->SetUniform("textureSampler", 0);

    // init the yuv program
    programYuv_ = std::make_unique<GlesProgram>();
    DISPLAY_CHK_RETURN((programYuv_ == nullptr), DISPLAY_FAILURE, DISPLAY_LOGE("can not create program for rgba"));
    ret = programYuv_->Init(g_vertexShader, g_textureFragmentFhaderYuv);
    DISPLAY_CHK_RETURN((!ret), DISPLAY_FAILURE, DISPLAY_LOGE("Failed to init yuv program"));
    
    programYuv_->Use();
    
    programYuv_->SetUniform("s_texureY", GlesGfxImage::INDEX_Y_TEXTURE);
    
    programYuv_->SetUniform("s_texureU", GlesGfxImage::INDEX_U_TEXTURE);
    
    programYuv_->SetUniform("s_texureV", GlesGfxImage::INDEX_V_TEXTURE);
    
    // init the clear program
    programClear_ = std::make_unique<GlesProgram>();
    DISPLAY_CHK_RETURN((programClear_ == nullptr), DISPLAY_FAILURE, DISPLAY_LOGE("can not create program for rgba"));
    programClear_->Init(g_vertexShader, g_clearfragmentShader);
    DISPLAY_CHK_RETURN((!ret), DISPLAY_FAILURE, DISPLAY_LOGE("Failed to init clear program"));
    
    programClear_->Use();

    return DISPLAY_SUCCESS;
}

int32_t HdiGlesComposition::Init(uint32_t width, uint32_t height)
{
    DISPLAY_LOGD();
    int32_t ret;
    OHOS::sptr<OHOS::DisplayMockSurface> mockSurface = new DisplayMockSurface();
    DISPLAY_CHK_RETURN((mockSurface == nullptr), false, DISPLAY_LOGE("can not create client surface"));
    if (!mockSurface->Init(width, height)) {
        DISPLAY_LOGE("Failed to init client surface");
        return DISPLAY_FAILURE;
    }
    clientSurface_ = mockSurface;

    nativeWindow_ = CreateNativeWindowFromSurface(&clientSurface_);
    DISPLAY_CHK_RETURN((nativeWindow_ == nullptr), DISPLAY_FAILURE, DISPLAY_LOGE("Failed create native window"));

    eglState_ = std::make_unique<GlEglState>();
    DISPLAY_CHK_RETURN((eglState_ == nullptr), DISPLAY_FAILURE, DISPLAY_LOGE("Failed to create eglState"));
    if (!eglState_->Init(nativeWindow_)) { // todo should get the resolustion
        DISPLAY_LOGE("can not init EglState");
    }
    eglState_->MakeCurrent(); // todo need ?
    ret = InitProgram();
    glViewport(0, 0, width, height);
    DISPLAY_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_LOGE("can not init program"));
    return DISPLAY_SUCCESS;
}

bool HdiGlesComposition::CanHandle(HdiLayer &hdiLayer)
{
    DISPLAY_LOGD();
    return true;
}

int32_t HdiGlesComposition::SetLayers(std::vector<HdiLayer *> &layers, HdiLayer &clientLayer)
{
    DISPLAY_LOGD("layers size %{public}d", layers.size());
    mClientLayer = &clientLayer;
    mCompLayers.clear();
    for (auto &layer : layers) {
        if (CanHandle(*layer)) {
            if ((layer->GetCompositionType() != COMPOSITION_VIDEO) &&
                (layer->GetCompositionType() != COMPOSITION_CURSOR)) {
                layer->SetDeviceSelect(COMPOSITION_DEVICE);
            } else {
                layer->SetDeviceSelect(layer->GetCompositionType());
            }
            mCompLayers.push_back(layer);
        }
    }
    DISPLAY_LOGD("composer layers size %{public}d", mCompLayers.size());
    return DISPLAY_SUCCESS;
}

int32_t HdiGlesComposition::Apply(bool modeSet)
{
    int32_t ret;
    eglState_->MakeCurrent();
    DISPLAY_LOGD("composer layers size %{public}d", mCompLayers.size());
    DisplayBytrace trace("gles apply");
    clientSurface_->SetReleaseFence(mClientLayer->GetReleaseFenceFd());
    for (uint32_t i = 0; i < mCompLayers.size(); i++) {
        HdiLayer *layer = mCompLayers[i];
        CompositionType compType = layer->GetDeviceSelect();
        switch (compType) {
            case COMPOSITION_VIDEO:
                ret = ClearLayer(*layer);
                DISPLAY_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE,
                    DISPLAY_LOGE("clear layer %{public}d failed", i));
                break;
            case COMPOSITION_DEVICE:
                ret = DrawLayer(*layer);
                DISPLAY_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE,
                    DISPLAY_LOGE("blit layer %{public}d failed ", i));
                break;
            default:
                DISPLAY_LOGE("the gfx composition can not surpport the type %{public}d", compType);
                break;
        }
    }
    glFinish(); // now wo no to use fence
    eglState_->SwapBuffer();
    clientSurface_->AcquireBufferHandle(*mClientLayer);
    
    return DISPLAY_SUCCESS;
}

HdiGlesComposition::~HdiGlesComposition()
{
    DISPLAY_LOGD();
}

int32_t HdiGlesComposition::DrawLayer(HdiLayer &layer)
{
    HdiLayerBuffer *srcBuffer = layer.GetCurrentBuffer();
    DISPLAY_CHK_RETURN((srcBuffer == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("the srcbuffer is null"));
    DISPLAY_CHK_RETURN((srcBuffer->mHandle == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGD("the handle is nullptr"));
    layer.WaitAcquireFence();
    if (DisplayUtils::IsYuv(static_cast<PixelFormat>(srcBuffer->GetFormat()))) {
        DISPLAY_LOGD("draw yuv layer");
        return DrawLayer(*(srcBuffer->mHandle), layer.GetLayerDisplayRect(), *programYuv_);
    } else {
        DISPLAY_LOGD("draw rgba layer");
        return DrawLayer(*(srcBuffer->mHandle), layer.GetLayerDisplayRect(), *programRgba_);
    }
}

std::vector<float> HdiGlesComposition::GetVertexArrays(const IRect &displayRect)
{
    std::vector<float> vertexArrays;
    vertexArrays.push_back((float)(displayRect.x));
    vertexArrays.push_back((float)(displayRect.y));
    vertexArrays.push_back((float)(displayRect.x + displayRect.w));
    vertexArrays.push_back((float)(displayRect.y));
    vertexArrays.push_back((float)(displayRect.x + displayRect.w));
    vertexArrays.push_back((float)(displayRect.y + displayRect.h));
    vertexArrays.push_back((float)(displayRect.x));
    vertexArrays.push_back((float)(displayRect.y + displayRect.h));
    return vertexArrays;
}

int32_t HdiGlesComposition::ClearLayer(HdiLayer &layer)
{
    DisplayBytrace trace("ClearLayer");
    DISPLAY_LOGD();
    glDisable(GL_BLEND);
    auto vertexArrays = GetVertexArrays(layer.GetLayerDisplayRect());
    std::vector<float> textureArrays = {
        0.0f, 0.0f, // TexCoord 0
        1.0f, 0.0f, // TexCoord 3
        1.0f, 1.0f, // TexCoord 2
        0.0f, 1.0f, // TexCoord 1
    };
    programClear_->Use();
    glBindAttribLocation(programClear_->Get(), 0, "vertexPosition");
    glBindAttribLocation(programClear_->Get(), 1, "textureCoord");
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertexArrays.data());  // step 2
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, textureArrays.data()); // step 2
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    DisplayBytrace traceDraw("draw");
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // 4 points
    
    return DISPLAY_SUCCESS;
}

int32_t HdiGlesComposition::DrawLayer(const BufferHandle &bufferHandle, const IRect &displayRect, GlesProgram &program)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    DisplayBytrace trace("DrawLayer");
    auto img = GlesGfxImage::Creat(eglState_->GetDisplay(), bufferHandle);
    if (img == nullptr) {
        DISPLAY_LOGE("Failed to drawlayer imag is nullptr");
        return DISPLAY_FAILURE;
    }
    program.Use();
    auto vertexArrays = GetVertexArrays(displayRect);
    std::vector<float> textureArrays = {
        0.0f, 0.0f, // TexCoord 0
        1.0f, 0.0f, // TexCoord 3
        1.0f, 1.0f, // TexCoord 2
        0.0f, 1.0f, // TexCoord 1
    };
    glBindAttribLocation(program.Get(), 0, "vertexPosition");
    glBindAttribLocation(program.Get(), 1, "textureCoord");
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertexArrays.data());  // step 2
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, textureArrays.data()); // step 2
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    DisplayBytrace traceDraw("draw");
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // 4 points
    
    return DISPLAY_SUCCESS;
}
} // namespace OHOS
} // namespace HDI
} // namespace DISPLAY