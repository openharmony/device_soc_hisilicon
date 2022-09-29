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
#include "display_gralloc_private.h"
#include <cerrno>
#include <sys/mman.h>
#include "allocator_manager.h"
#include "securec.h"
#include "display_common.h"
#ifdef __cplusplus
extern "C" {
#endif
using namespace OHOS::HDI::DISPLAY;

bool Convert2PrivateHandle(const BufferHandle *handle, PrivateBufferHandle *privateHandle)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN(((handle == nullptr) || privateHandle == nullptr), false, DISPLAY_LOGE("the handle is null"));
    memcpy_s(&(privateHandle->hdl), sizeof(BufferHandle), handle, sizeof(BufferHandle));
    privateHandle->attrFd = handle->fd;
    privateHandle->attrOffset = handle->size;
    privateHandle->attrSize = BUFFER_PRIV_SIZE;
    return true;
}


#ifdef __cplusplus
}
#endif