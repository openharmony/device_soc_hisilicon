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
#ifndef GL_EGL_STATE_H
#define GL_EGL_STATE_H
#include <cstdint>
#include "GLES/gl.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class GlEglState {
public:
    bool Init(NativeWindowType nativeWindow);
    bool Init(uint32_t width, uint32_t height);
    void SwapBuffer();
    void MakeCurrent();
    EGLDisplay GetDisplay()
    {
        return eglDisplay_;
    }

private:
    void CreateEglSurface(EGLNativeWindowType nativeWindow);
    bool Init(const EGLint surfaceType);
    NativeWindowType nativeWindow_ = 0;
    EGLDisplay eglDisplay_ = EGL_DEFAULT_DISPLAY;
    EGLSurface eglSurface_ = EGL_NO_SURFACE;
    EGLConfig eglConfig_ = EGL_NO_CONFIG_KHR;
    EGLContext eglContext_ = EGL_NO_CONTEXT;
};
}
}
}
#endif // GL_EGL_STATE_H