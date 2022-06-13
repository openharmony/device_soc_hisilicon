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
#include "gles_sharder.h"
#include "display_common.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
GlesSharder::GlesSharder(GLenum type, const std::string &src) : type_(type), source_(src)
{
    DISPLAY_LOGD();
}

GLuint GlesSharder::Get() const
{
    return handle_;
}

bool GlesSharder::compile()
{
    DISPLAY_LOGD();
    GLint compiled = 0;
    glCompileShader(handle_);
    glGetShaderiv(handle_, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        DISPLAY_LOGE("Failed to compile sharder");
        GLint logLen = 0;
        glGetShaderiv(handle_, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 1) {
            char *log = new char[logLen + 1]();
            glGetShaderInfoLog(handle_, logLen + 1, nullptr, log);
            DISPLAY_LOGE("sharder compile failed:");
            DISPLAY_LOGE("%{public}s", log);
            DISPLAY_LOGE("log information end");
            delete[] log;
            log = nullptr;
        }
        return false;
    }
    DISPLAY_LOGD("compiled success");
    return true;
}

bool GlesSharder::Init()
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN(((type_ != GL_VERTEX_SHADER) && (type_ != GL_FRAGMENT_SHADER)), false,
        DISPLAY_LOGE("the type is invalide"));
    handle_ = glCreateShader(type_);
    DISPLAY_CHK_RETURN((handle_ == 0), false,
        DISPLAY_LOGE("failed to create sharder glerror %{public}d", glGetError()));
    const GLchar *shaderSource = source_.c_str();
    glShaderSource(handle_, 1, &shaderSource, nullptr);
    return compile();
}

GlesSharder::~GlesSharder()
{
    if (handle_ != 0) {
        glDeleteShader(handle_);
    }
    DISPLAY_LOGD();
}
}
}
}