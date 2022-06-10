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
#ifndef EGL_EXT_FUNCTION_H
#define EGL_EXT_FUNCTION_H
#include <cstdint>
#include <mutex>
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

class EglExtFunction {
public:
    static EglExtFunction getInstance()
    {
        static EglExtFunction instance;
        static std::once_flag once;
        std::call_once(once, [&]() { instance.Init(); });
        return instance;
    }

    bool Init(); // user must call the interface before using
    EGLImageKHR eglCreateImageKHR(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer,
        const EGLint *attrib_list);
    EGLBoolean eglDestroyImageKHR(EGLDisplay dpy, EGLImageKHR image);
    void glEGLImageTargetTexture2DOES(EGLenum target, GLeglImageOES image);
    void EGLImageTargetTexture2D(GLenum target, GLeglImageOES image);

private:
    PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHRFunc_ = nullptr;
    PFNEGLDESTROYIMAGEKHRPROC eglDestroyImageKHRFunc_ = nullptr;
    PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOESFunc_ = nullptr;
};

#endif // EGL_EXT_FUNCTION_H