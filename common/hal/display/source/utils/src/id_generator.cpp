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

#include "id_generator.h"
#include <climits>
#include "display_common.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
IdGenerator::IdGenerator(std::string name) : name_(name)
{
    DISPLAY_LOGI("name : %{public}s", name.c_str());
}

bool IdGenerator::GetNext(uint32_t &id)
{
    DISPLAY_LOGI();
    if (usedIds_.size() > UINT_MAX) {
        DISPLAY_LOGE("all id have been used");
        return false;
    }

    while (usedIds_.find(nextId_) != usedIds_.end()) {
        nextId_++;
    }

    id = nextId_++;
    usedIds_.emplace(id);
    return true;
}

void IdGenerator::Release(uint32_t id)
{
    DISPLAY_LOGI();
    usedIds_.erase(id);
}
}
}
}