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
#ifndef GLES_PROGRAM_H
#define GLES_PROGRAM_H
#include <memory>
#include "gles_sharder.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class GlesProgram {
public:
    GlesProgram() {}
    ~GlesProgram();
    GLuint Get() const;
    bool Init(const std::string &vertexSharderSrc, const std::string &fragmentSharderSrc);
    void Use() const;
    void SetUniform(const GLchar *name, GLint value);

private:
    std::unique_ptr<GlesSharder> vertexSharder_;
    std::unique_ptr<GlesSharder> fragmentSharder_;
    GLuint handle_ = 0;
};
}
}
}
#endif // GLES_PROGRAM_H