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

#ifndef FB_COMPOSITION_H
#define FB_COMPOSITION_H
#include <memory>
#include <vector>
#include "hdi_composer.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class FbComposition : public HdiComposition {
public:
    explicit FbComposition(std::vector<int> &fbs);
    virtual ~FbComposition();
    int32_t Init();
    int32_t SetLayers(std::vector<HdiLayer *> &layers, HdiLayer &clientLayer);
    int32_t Apply(bool modeSet);
private:
    int32_t FbFresh(int fd, HdiLayer &clientlayer, int &fence);
    std::vector<int> fds_;
};
} // namespace DISPLAY
} // namespace HDI
} // namespace OHOS
#endif // FB_COMPOSITION_H