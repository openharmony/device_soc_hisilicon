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

#ifndef DISPLAY_ADAPTER_H
#define DISPLAY_ADAPTER_H
#include <memory>
#include <mutex>
#include <string>
#include "display_adapter_interface.h"
#include "display_module_loader.h"

namespace OHOS {
namespace HDI {
namespace DISPLAY {
using AdapterInitFunc = int32_t (*)(DisplayAdapterFuncs **funcs);
using AdapterDeInitFunc = int32_t (*)(DisplayAdapterFuncs *funcs);

class DisplayAdapter {
public:
    static std::unique_ptr<DisplayAdapter> &GetInstance();
    virtual ~DisplayAdapter();

    int32_t Init();
    int32_t OpenDevice(const std::string &pathName, int32_t flags, mode_t mode);
    int32_t CloseDevice(int32_t devFd);
    int32_t Ioctl(int32_t devFd, uint32_t cmd, void *args);
    int32_t FbGetDmaBuffer(int32_t devFd);
    int32_t FbFresh(int32_t devFd, DisplayFrameInfo &frame);

private:
    static constexpr const char *LIB_NAME_ADAPTER = "libdisplay_adapter_impl.z.so";
    static constexpr const char *INIT_FUNCTION_NAME = "DisplayAdapaterInitialize";
    static constexpr const char *DEINIT_FUNCTION_NAME = "DisplayAdapaterUninitialize";

    std::unique_ptr<DisplayModuleLoader> loader_;
    DisplayAdapterFuncs *funcs_ = nullptr;
    AdapterInitFunc initFunc_ = nullptr;
    AdapterDeInitFunc deInitFunc_ = nullptr;
};
} // namespace DISPLAY
} // namespace HDI
} // namespace OHOS
#endif // DISPLAY_ADAPTER_INTERFACE_H