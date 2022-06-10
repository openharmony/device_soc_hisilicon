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
#ifndef GLES_SHARDER_H
#define GLES_SHARDER_H
#include <string>
#include "GLES/gl.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class GlesSharder {
public:
    GlesSharder(GLenum type, const std::string &src);
    ~GlesSharder();
    bool Init();
    GLuint Get() const;

private:
    bool compile();
    GLuint handle_ = 0;
    GLenum type_ = 0;
    std::string source_;
};
}
}
}
#endif // GLES_SHARDER_H