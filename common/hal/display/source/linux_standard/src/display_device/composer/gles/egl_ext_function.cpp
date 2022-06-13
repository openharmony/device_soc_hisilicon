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

#include "egl_ext_function.h"
#include "display_common.h"

bool EglExtFunction::Init()
{
    eglCreateImageKHRFunc_ = reinterpret_cast<PFNEGLCREATEIMAGEKHRPROC>(eglGetProcAddress("eglCreateImageKHR"));
    if (eglCreateImageKHRFunc_ == nullptr) {
        DISPLAY_LOGE("Failed to get address of eglCreateImageKHR");
        return false;
    }

    eglDestroyImageKHRFunc_ = reinterpret_cast<PFNEGLDESTROYIMAGEKHRPROC>(eglGetProcAddress("eglDestroyImageKHR"));
    if (eglDestroyImageKHRFunc_ == nullptr) {
        DISPLAY_LOGE("Failed to get address of eglDestroyImageKHR");
        return false;
    }

    glEGLImageTargetTexture2DOESFunc_ =
        reinterpret_cast<PFNGLEGLIMAGETARGETTEXTURE2DOESPROC>(eglGetProcAddress("glEGLImageTargetTexture2DOES"));
    if (glEGLImageTargetTexture2DOESFunc_ == nullptr) {
        DISPLAY_LOGE("Failed to get address of glEGLImageTargetTexture2DOES");
        return false;
    }
    return true;
}

EGLImageKHR EglExtFunction::eglCreateImageKHR(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer,
    const EGLint *attrib_list)
{
    DISPLAY_LOGD("eglCreateImageKHR");
    return eglCreateImageKHRFunc_(dpy, ctx, target, buffer, attrib_list);
}

void EglExtFunction::glEGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image)
{
    DISPLAY_LOGD("glEGLImageTargetTexture2DOES");
    glEGLImageTargetTexture2DOESFunc_(target, image);
}

EGLBoolean EglExtFunction::eglDestroyImageKHR(EGLDisplay dpy, EGLImageKHR image)
{
    DISPLAY_LOGD("eglDestroyImageKHR");
    DISPLAY_CHK_RETURN(((dpy == EGL_NO_DISPLAY) || (image == EGL_NO_IMAGE_KHR)),
        false, DISPLAY_LOGE("the input parameter is invalide"));
    return eglDestroyImageKHRFunc_(dpy, image);
}