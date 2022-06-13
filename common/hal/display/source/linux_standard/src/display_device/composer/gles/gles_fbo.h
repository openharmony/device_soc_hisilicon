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
#ifndef GLES_FBO_H
#define GLES_FBO_H
#include <cstdint>
#include <memory>
#include "display_type.h"
#include "gles_gfx_image.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class GlesFbo {
public:
    static std::unique_ptr<GlesFbo> Create(EGLDisplay display,
        const BufferHandle &bufferhandle); // before call it must call makecurrent ?
    bool Init(EGLDisplay display, const BufferHandle &bufferhandle);
    GLuint GetFbo();
    ~GlesFbo();

private:
    GLuint glTexture_;
    GLuint glFbo_ = 0;
    EGLImage eglImage_ = EGL_NO_IMAGE_KHR;
    EGLDisplay display_;
};
}
}
}
#endif // GLES_FBO_H