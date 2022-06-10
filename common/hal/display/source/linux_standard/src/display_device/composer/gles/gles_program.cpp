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
#include <gles_program.h>
#include "display_common.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
bool GlesProgram::Init(const std::string &vertexSharderSrc, const std::string &fragmentSharderSrc)
{
    DISPLAY_LOGD();
    bool ret = false;
    // init sharder
    auto vertexSharder = std::make_unique<GlesSharder>(GL_VERTEX_SHADER, vertexSharderSrc);
    ret = vertexSharder->Init();
    DISPLAY_CHK_RETURN((!ret), false, DISPLAY_LOGE("Failed to init vertex sharder"));
    auto fragmentSharder = std::make_unique<GlesSharder>(GL_FRAGMENT_SHADER, fragmentSharderSrc);
    ret = fragmentSharder->Init();
    DISPLAY_CHK_RETURN((!ret), false, DISPLAY_LOGE("Failed to init fragment sharder"));
    // init program
    handle_ = glCreateProgram();
    DISPLAY_CHK_RETURN((handle_ == 0), false, DISPLAY_LOGE("Failed to create program"));

    glAttachShader(handle_, vertexSharder->Get());
    glAttachShader(handle_, fragmentSharder->Get());
    glLinkProgram(handle_);
    vertexSharder_ = std::move(vertexSharder);
    fragmentSharder_ = std::move(fragmentSharder);
    GLint linked = 0;
    glGetProgramiv(handle_, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        DISPLAY_LOGE("Failed to link program");
        GLint logLen = 0;
        glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 1) {
            char *log = new char[logLen + 1]();
            glGetProgramInfoLog(handle_, logLen + 1, nullptr, log);
            DISPLAY_LOGE("link program log:");
            DISPLAY_LOGE("%{public}s", log);
            DISPLAY_LOGE("link program log end");
            delete[] log;
        }
        return false;
    }

    return true;
}

GlesProgram::~GlesProgram()
{
    if (vertexSharder_ != nullptr) {
        glDetachShader(handle_, vertexSharder_->Get());
    }
    if (fragmentSharder_ != nullptr) {
        glDetachShader(handle_, fragmentSharder_->Get());
    }
    glDeleteProgram(handle_);
}

GLuint GlesProgram::Get() const
{
    return handle_;
}

void GlesProgram::Use() const
{
    DISPLAY_LOGD();
    glUseProgram(handle_);
}

void GlesProgram::SetUniform(const GLchar *name, GLint value)
{
    DISPLAY_LOGD();
    GLint location = glGetUniformLocation(handle_, name);
    if (location < 0) {
        DISPLAY_LOGE("can not find the uniform %{public}s", name);
        return;
    }
    glUniform1i(location, value);
}
}
}
}