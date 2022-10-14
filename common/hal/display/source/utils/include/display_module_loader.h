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
#ifndef DISPLAY_MODULE_LOADER_H
#define DISPLAY_MODULE_LOADER_H

#include <dlfcn.h>
#include <memory>
#include <string>
#include <display_common.h>
namespace OHOS {
namespace HDI {
namespace DISPLAY {
class DisplayModuleLoader {
public:
    static std::unique_ptr<DisplayModuleLoader> Create(const std::string pathName)
    {
        void *handle = dlopen(pathName.c_str(), RTLD_NOW | RTLD_NOLOAD);
        if (handle == nullptr) {
            handle = dlopen(pathName.c_str(), RTLD_NOW);
            if (handle == nullptr) {
                return nullptr;
            }
        }
        return std::make_unique<DisplayModuleLoader>(handle);
    }

    explicit DisplayModuleLoader(void *handle) : handle_(handle) {}

    void *GetSymbol(const std::string symbol)
    {
        return dlsym(handle_, symbol.c_str());
    }

    ~DisplayModuleLoader()
    {
        if (handle_ != nullptr) {
            dlclose(handle_);
            handle_ = nullptr;
        }
    }

private:
    void *handle_;
};
} // namespace DISPLAY
} // namespace HDI
} // namespace OHOS
#endif // DISPLAY_MODULE_LOADER_H