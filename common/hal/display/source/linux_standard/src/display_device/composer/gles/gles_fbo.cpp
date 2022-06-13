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

#include "gles_fbo.h"
#include "gles_gfx_image.h"
#include "egl_ext_function.h"
#include "display_common.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
std::unique_ptr<GlesFbo> GlesFbo::Create(EGLDisplay display, const BufferHandle &bufferHandle)
{
    auto glesFbo = std::make_unique<GlesFbo>();
    if (glesFbo->Init(display, bufferHandle)) {
        return glesFbo;
    }
    DISPLAY_LOGD("Failed to Init Fbo");
    return nullptr;
}

bool GlesFbo::Init(EGLDisplay display, const BufferHandle &bufferhandle)
{
    display_ = display;
    eglImage_ = GlesGfxImage::CreateEglImage(display, bufferhandle);
    if (eglImage_ == EGL_NO_IMAGE_KHR) {
        DISPLAY_LOGE("Fbo Init Failed to Create Egl Image");
        return false;
    }
    glGenTextures(1, &glTexture_);
    glBindTexture(GL_TEXTURE_2D, glTexture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    EglExtFunction::getInstance().glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, eglImage_);
    glGenFramebuffers(1, &glFbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, glFbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glTexture_, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        DISPLAY_LOGE("Failed to create fbo");
        return false;
    }
    return true;
}

GLuint GlesFbo::GetFbo()
{
    DISPLAY_LOGD("GetFbo  %d", glFbo_);
    return glFbo_;
}


GlesFbo::~GlesFbo()
{
    DISPLAY_LOGD("~GlesFbo");
    if (glFbo_) {
        glDeleteFramebuffers(1, &glFbo_);
    }

    if (glTexture_) {
        glDeleteTextures(1, &glTexture_);
    }

    if (eglImage_) {
        GlesGfxImage::DestroyEglImage(display_, eglImage_);
    }
}
}
}
}