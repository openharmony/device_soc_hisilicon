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
#include "display_adapter.h"
#include <cstdarg>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
namespace OHOS {
namespace HDI {
namespace DISPLAY {
std::unique_ptr<DisplayAdapter> &DisplayAdapter::GetInstance()
{
    static std::unique_ptr<DisplayAdapter> instance = nullptr;
    static std::once_flag once;
    std::call_once(once, [&]() {
        auto adapter = std::make_unique<DisplayAdapter>();
        if (adapter != nullptr) {
            int ret = adapter->Init();
            if (ret == DISPLAY_SUCCESS) {
                instance = std::move(adapter);
            }
        }
    });
    return instance;
}

int32_t DisplayAdapter::Init()
{
    loader_ = DisplayModuleLoader::Create(LIB_NAME_ADAPTER);
    if (loader_ == nullptr) {
        DISPLAY_LOGE("can not create loader");
        return DISPLAY_FAILURE;
    }
    initFunc_ = reinterpret_cast<AdapterInitFunc>(loader_->GetSymbol(INIT_FUNCTION_NAME));
    if (initFunc_ == nullptr) {
        DISPLAY_LOGE("failed to get function %{public}s", INIT_FUNCTION_NAME);
        return DISPLAY_FAILURE;
    }
    deInitFunc_ = reinterpret_cast<AdapterDeInitFunc>(loader_->GetSymbol(DEINIT_FUNCTION_NAME));
    if (deInitFunc_ == nullptr) {
        DISPLAY_LOGE("failed to get function %{public}s", DEINIT_FUNCTION_NAME);
        return DISPLAY_FAILURE;
    }
    int ret = initFunc_(&funcs_);
    if ((ret != DISPLAY_SUCCESS) || (funcs_ == nullptr)) {
        DISPLAY_LOGE("failed to get display adapter functions");
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

DisplayAdapter::~DisplayAdapter()
{
    if ((funcs_ != nullptr) && (deInitFunc_ != nullptr)) {
        deInitFunc_(funcs_);
    }
}

int32_t DisplayAdapter::OpenDevice(const std::string &pathName, int32_t flags, mode_t mode)
{
    if (funcs_->OpenDevice != nullptr) {
        DISPLAY_LOGD("use the adapter open function");
        return funcs_->OpenDevice(pathName.c_str(), flags, mode);
    } else {
        return open(pathName.c_str(), flags, mode);
    }
}

int32_t DisplayAdapter::CloseDevice(int32_t devFd)
{
    if (funcs_->CloseDevice != nullptr) {
        DISPLAY_LOGD("use the adapter close function");
        return funcs_->CloseDevice(devFd);
    } else {
        return close(devFd);
    }
}

int32_t DisplayAdapter::Ioctl(int32_t devFd, uint32_t cmd, void *args)
{
    int32_t ret = -1;
    if (funcs_->Ioctl != nullptr) {
        DISPLAY_LOGD("use the adapter ioctl function");
        ret = funcs_->Ioctl(devFd, cmd, args);
    } else {
        ret = ioctl(devFd, cmd, args);
    }
    return ret;
}

int32_t DisplayAdapter::FbGetDmaBuffer(int32_t devFd)
{
    if (funcs_->FbGetDmaBuffer == nullptr) {
        DISPLAY_LOGE("has no function to get fb dmabuffer");
        return -1;
    }
    return funcs_->FbGetDmaBuffer(devFd);
}

int32_t DisplayAdapter::FbFresh(int32_t devFd, DisplayFrameInfo &frame)
{
    if (funcs_->FbFresh == nullptr) {
        DISPLAY_LOGE("has no function to fresh fb");
        return -1;
    }
    return funcs_->FbFresh(devFd, &frame);
}
} // namespace DISPLAY
} // namespace HDI
} // namespace OHOS