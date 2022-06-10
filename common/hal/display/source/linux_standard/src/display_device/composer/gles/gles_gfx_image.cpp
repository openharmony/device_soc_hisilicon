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
#include "gles_gfx_image.h"
#include "window.h"
#include "egl_ext_function.h"
#include "display_common.h"
#include "hardware_buffer.h"
#include "display_bytrace.h"
#include "display_utils.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
GlesGfxImage::GlesGfxImage(EGLDisplay display) : eglDisplay_(display)
{
    DISPLAY_LOGD();
}

GlesGfxImage::~GlesGfxImage()
{
    DISPLAY_LOGD();
    for (uint32_t i = 0; i < TEXTURE_COUNT; i++) {
        if (textureId_[i] != 0) {
            glDeleteTextures(1, &textureId_[i]);
        }
    }
    if (eglImageKHr_ != EGL_NO_IMAGE_KHR) {
        EglExtFunction::getInstance().eglDestroyImageKHR(eglDisplay_, eglImageKHr_);
    }
}

std::unique_ptr<GlesGfxImage> GlesGfxImage::Creat(EGLDisplay display, const BufferHandle &bufferHandle)
{
    DISPLAY_LOGD();
    DisplayBytrace trace("GlesGfxImage::Creat");
    auto image = std::make_unique<GlesGfxImage>(display);
    if (image->Init(display, bufferHandle)) {
        return image;
    }
    DISPLAY_LOGE("Falied to int gfx image");
    return nullptr;
}

bool GlesGfxImage::Init(EGLDisplay display, const BufferHandle &bufferHandle)
{
    DISPLAY_LOGD();
    eglDisplay_ = display;
    if (DisplayUtils::IsYuv(static_cast<PixelFormat>(bufferHandle.format))) {
        return CreateImageYuv(bufferHandle);
    } else {
        return CreateImageRgb(bufferHandle);
    }
}

EGLImage GlesGfxImage::CreateEglImage(EGLDisplay display, const BufferHandle &bufferhandle)
{
    DISPLAY_LOGD();
    OHOS::sptr<OHOS::SurfaceBuffer> SurfaceBuffer = OHOS::HardwareBuffer::Create(bufferhandle);
    if (SurfaceBuffer == nullptr) {
        DISPLAY_LOGE("Failed to create surface buffer");
        return EGL_NO_IMAGE_KHR;
    }
    struct NativeWindowBuffer *nativeWindowBuffer = CreateNativeWindowBufferFromSurfaceBuffer(&SurfaceBuffer);
    EGLImageKHR img = EglExtFunction::getInstance().eglCreateImageKHR(display, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_OHOS,
        nativeWindowBuffer, 0);
    if (img == EGL_NO_IMAGE_KHR) {
        DISPLAY_LOGE("Failed to create Image Khr");
    }
    DestoryNativeWindowBuffer(nativeWindowBuffer);
    return img;
}

EGLBoolean GlesGfxImage::DestroyEglImage(EGLDisplay display, EGLImageKHR image)
{
    return EglExtFunction::getInstance().eglDestroyImageKHR(display, image);
}


void GlesGfxImage::CreateTextures(uint32_t num)
{
    DISPLAY_LOGD("CreateTextures");
    if (num > TEXTURE_COUNT) {
        DISPLAY_LOGE("the number is error");
        num = TEXTURE_COUNT;
    }

    for (int i = 0; i < num; i++) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // todo wether need ?
        glGenTextures(1, &textureId_[i]);
        glBindTexture(GL_TEXTURE_2D, textureId_[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}

bool GlesGfxImage::CreateImageRgb(const BufferHandle &bufferHandle)
{
    DISPLAY_LOGD();
    CreateTextures(1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId_[INDEX_RGB_TEXTURE]);
    OHOS::sptr<OHOS::SurfaceBuffer> SurfaceBuffer = OHOS::HardwareBuffer::Create(bufferHandle);
    if (SurfaceBuffer == nullptr) {
        DISPLAY_LOGE("Failed to create surface buffer");
        return false;
    }
    struct NativeWindowBuffer *nativeWindowBuffer = CreateNativeWindowBufferFromSurfaceBuffer(&SurfaceBuffer);
    EGLImageKHR img = EglExtFunction::getInstance().eglCreateImageKHR(eglDisplay_, EGL_NO_CONTEXT,
        EGL_NATIVE_BUFFER_OHOS, nativeWindowBuffer, 0);
    if (img == EGL_NO_IMAGE_KHR) {
        DISPLAY_LOGE("Failed to create Image Khr");
    }
    EglExtFunction::getInstance().glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, img);
    eglImageKHr_ = img;

    DestoryNativeWindowBuffer(nativeWindowBuffer);
    return true;
}

bool GlesGfxImage::CreateImageYuv(const BufferHandle &bufferHandle)
{
    DISPLAY_LOGD("CreateImageYuv");
    const uint32_t textureCount = 3;
    constexpr const uint32_t UV_SCALE = 2;
    if (bufferHandle.virAddr == nullptr) {
        DISPLAY_LOGE("Failed to create yuv image for the virtual address is nullptr");
        return false;
    }

    uint8_t *yData = static_cast<uint8_t *>(bufferHandle.virAddr);
    uint8_t *uData = yData + (bufferHandle.width * bufferHandle.height);                       // reduce the width
    uint8_t *vData = uData + (bufferHandle.width * bufferHandle.height) / UV_SCALE / UV_SCALE; // reduce the width
    CreateTextures(textureCount);
    // for Y
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId_[INDEX_Y_TEXTURE]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, bufferHandle.width, bufferHandle.height, 0, GL_LUMINANCE,
        GL_UNSIGNED_BYTE, yData);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId_[INDEX_U_TEXTURE]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, bufferHandle.width / UV_SCALE, bufferHandle.height / UV_SCALE, 0,
        GL_LUMINANCE, GL_UNSIGNED_BYTE, uData);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureId_[INDEX_V_TEXTURE]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, bufferHandle.width / UV_SCALE, bufferHandle.height / UV_SCALE, 0,
        GL_LUMINANCE, GL_UNSIGNED_BYTE, vData);
    return true;
}
}
}
}