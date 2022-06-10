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
#include "gl_egl_state.h"
#include <cstdint>

#include "display_common.h"
#include "display_bytrace.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
static bool CheckEglExtension(const char *extensions, const char *extension)
{
    size_t extlen = strlen(extension);
    const char *end = extensions + strlen(extensions);
    constexpr char CHARACTER_WHITESPACE = ' ';
    constexpr const char *CHARACTER_STRING_WHITESPACE = " ";
    while (extensions < end) {
        size_t n = 0;

        /* Skip whitespaces, if any */
        if (*extensions == CHARACTER_WHITESPACE) {
            extensions++;
            continue;
        }

        n = strcspn(extensions, CHARACTER_STRING_WHITESPACE);
        /* Compare strings */
        if (n == extlen && strncmp(extension, extensions, n) == 0) {
            return true; /* Found */
        }
        extensions += n;
    }
    /* Not found */
    return false;
}

static EGLDisplay GetPlatformEglDisplay(EGLenum platform, void *native_display, const EGLint *attrib_list)
{
    using GetPlatformDisplayExt = PFNEGLGETPLATFORMDISPLAYEXTPROC;
    constexpr const char *EGL_EXT_PLATFORM_WAYLAND = "EGL_EXT_platform_wayland";
    constexpr const char *EGL_KHR_PLATFORM_WAYLAND = "EGL_KHR_platform_wayland";
    constexpr const char *EGL_GET_PLATFORM_DISPLAY_EXT = "eglGetPlatformDisplayEXT";
    static GetPlatformDisplayExt eglGetPlatformDisplayExt = NULL;

    if (!eglGetPlatformDisplayExt) {
        const char *extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
        if (extensions && (CheckEglExtension(extensions, EGL_EXT_PLATFORM_WAYLAND) ||
            CheckEglExtension(extensions, EGL_KHR_PLATFORM_WAYLAND))) {
            eglGetPlatformDisplayExt = (GetPlatformDisplayExt)eglGetProcAddress(EGL_GET_PLATFORM_DISPLAY_EXT);
        }
    }

    if (eglGetPlatformDisplayExt) {
        return eglGetPlatformDisplayExt(platform, native_display, attrib_list);
    }

    return eglGetDisplay((EGLNativeDisplayType)native_display);
}

bool GlEglState::Init(const EGLint surfaceType)
{
    uint32_t ret;
    DISPLAY_LOGD();
    eglDisplay_ = GetPlatformEglDisplay(EGL_PLATFORM_OHOS_KHR, EGL_DEFAULT_DISPLAY, nullptr);
    DISPLAY_CHK_RETURN((eglDisplay_ == EGL_NO_DISPLAY), false,
        DISPLAY_LOGE("can not get eglDisplay glerror : %{puiblic}d", eglGetError()));

    EGLint major = 0, minor = 0;
    ret = eglInitialize(eglDisplay_, &major, &minor);
    DISPLAY_CHK_RETURN((ret == EGL_FALSE), false,
        DISPLAY_LOGE("egl initialize failed glerror : %{public}d", eglGetError()));

    ret = eglBindAPI(EGL_OPENGL_ES_API);
    DISPLAY_CHK_RETURN((ret == EGL_FALSE), false,
        DISPLAY_LOGE("egl bind OpenGL ES API failed glerror : %{public}d", eglGetError()));

    EGLint outNum;
    EGLint configAttr[] = {EGL_SURFACE_TYPE, surfaceType, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8,
                           EGL_ALPHA_SIZE, 8, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE};
    ret = eglChooseConfig(eglDisplay_, configAttr, &eglConfig_, 1, &outNum);
    DISPLAY_CHK_RETURN(((ret == EGL_FALSE) || (outNum < 1)), false,
        DISPLAY_LOGE("Failed to choose egl config glerror %{public}d", eglGetError()));

    constexpr EGLint EGL_CONTEXT_CLIENT_VERSION_NUM = 2;
    static const EGLint contextAttr[] = {
        EGL_CONTEXT_CLIENT_VERSION, EGL_CONTEXT_CLIENT_VERSION_NUM,
        EGL_NONE
    };
    eglContext_ = eglCreateContext(eglDisplay_, eglConfig_, EGL_NO_CONTEXT, contextAttr);
    DISPLAY_CHK_RETURN((ret == EGL_FALSE), false,
        DISPLAY_LOGE("Failed to create egl context glerror : %{public}d", eglGetError()));
    return true;
}

bool GlEglState::Init(uint32_t width, uint32_t height)
{
    if (!Init(EGL_PBUFFER_BIT)) {
        DISPLAY_LOGE("Init failed");
    }
    EGLint attribs[] = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};
    eglSurface_ = eglCreatePbufferSurface(eglDisplay_, eglConfig_, attribs);
    if (eglSurface_ == EGL_NO_SURFACE) {
        DISPLAY_LOGE("Failed to create eglsurface %{public}x", eglGetError());
    }
    return true;
}

bool GlEglState::Init(NativeWindowType window)
{
    if (window == nullptr) {
        return false;
    }
    if (!Init(EGL_WINDOW_BIT)) {
        DISPLAY_LOGE("Init failed");
        return false;
    }
    nativeWindow_ = window;
    CreateEglSurface(nativeWindow_);
    return true;
}

void GlEglState::MakeCurrent()
{
    DISPLAY_LOGD();
    if (!eglMakeCurrent(eglDisplay_, eglSurface_, eglSurface_, eglContext_)) {
        DISPLAY_LOGE("can not make cuurent glerror %{public}d", eglGetError());
    }
}

void GlEglState::SwapBuffer()
{
    DisplayBytrace trace = DisplayBytrace("SwapBuffer");
    DISPLAY_LOGD();
    if (!eglSwapBuffers(eglDisplay_, eglSurface_)) {
        DISPLAY_LOGE("Failed to SwapBuffers on surface %{public}p glerror %{public}d", eglSurface_, eglGetError());
    }
}

void GlEglState::CreateEglSurface(EGLNativeWindowType nativeWindow)
{
    uint32_t ret = eglMakeCurrent(eglDisplay_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (!ret) {
        DISPLAY_LOGW("Failed to make current glerror %{public}x", eglGetError());
    }
    eglSurface_ = eglCreateWindowSurface(eglDisplay_, eglConfig_, nativeWindow, nullptr);
    if (eglSurface_ == EGL_NO_SURFACE) {
        DISPLAY_LOGE("Failed to create eglsurface %{public}x", eglGetError());
    }
}
}
}
}