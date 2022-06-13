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

#ifndef GLES_GFX_IMAGE_H
#define GLES_GFX_IMAGE_H

#include <cstdint>
#include <memory>
#include "GLES/gl.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "display_type.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class GlesGfxImage {
public:
    explicit GlesGfxImage(EGLDisplay dpy);
    static std::unique_ptr<GlesGfxImage> Creat(EGLDisplay dps, const BufferHandle &bufferHandle);
    static EGLImage CreateEglImage(EGLDisplay display, const BufferHandle &bufferHandle);
    static EGLBoolean DestroyEglImage(EGLDisplay display, EGLImageKHR image);
    bool Init(EGLDisplay dps, const BufferHandle &bufferHandle);
    ~GlesGfxImage();
    static constexpr const uint32_t INDEX_RGB_TEXTURE = 0;
    static constexpr const uint32_t INDEX_Y_TEXTURE = 0;
    static constexpr const uint32_t INDEX_U_TEXTURE = 1;
    static constexpr const uint32_t INDEX_V_TEXTURE = 2;
private:
    bool CreateImageRgb(const BufferHandle &bufferHandle);
    bool CreateImageYuv(const BufferHandle &bufferHandle); // now sue texImag2d
    void CreateTextures(uint32_t num);

    static constexpr const uint32_t TEXTURE_COUNT = 3;

    EGLImageKHR eglImageKHr_ = EGL_NO_IMAGE_KHR;
    GLuint textureId_[TEXTURE_COUNT] = {0};
    EGLDisplay eglDisplay_;
};
}
}
}
#endif // GLES_GFX_IMAGE_H