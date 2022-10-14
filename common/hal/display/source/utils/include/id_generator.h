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
#ifndef _ID_GENERATOR_H_
#define _ID_GENERATOR_H_
#include <string>
#include <unordered_set>
#include <climits>
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class IdGenerator {
public:
    IdGenerator(std::string name);
    bool GetNext(uint32_t &id);
    void Release(uint32_t id);
private:
    std::unordered_set<uint32_t> usedIds_;
    std::string name_;
    uint32_t nextId_ = 0;
};
} // OHOS
} // HDI
} // DISPLAY

#endif // _ID_GENERATOR_H_