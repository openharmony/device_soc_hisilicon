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
#include "display_gralloc.h"
#include <cerrno>
#include "allocator_manager.h"
#include "display_common.h"
using namespace OHOS::HDI::DISPLAY;
static int32_t AllocMem(const AllocInfo *info, BufferHandle **handle)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((info == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("info is nullptr"));
    DISPLAY_CHK_RETURN((handle == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("handle is nullptr"));
    return AllocatorManager::GetInstance().GetAllocator(info->usage)->AllocMem(*info, handle);
}

static void FreeMem(BufferHandle *handle)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN_NOT_VALUE((handle == nullptr), DISPLAY_LOGE("handle is nullptr"));
    AllocatorManager::GetInstance().GetAllocator(handle->usage)->FreeMem(handle);
}

static void *Mmap(BufferHandle *handle)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((handle == nullptr), nullptr, DISPLAY_LOGE("handle is nullptr"));
    return AllocatorManager::GetInstance().GetAllocator(handle->usage)->Mmap(*handle);
}

static int32_t Unmap(BufferHandle *handle)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((handle == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("handle is nullptr"));
    return AllocatorManager::GetInstance().GetAllocator(handle->usage)->Unmap(*handle);
}

static int32_t FlushCache(BufferHandle *handle)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((handle == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("handle is nullptr"));
    return AllocatorManager::GetInstance().GetAllocator(handle->usage)->FlushCache(*handle);
}

static int32_t InvalidateCache(BufferHandle *handle)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((handle == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("handle is nullptr"));
    return AllocatorManager::GetInstance().GetAllocator(handle->usage)->InvalidateCache(*handle);
}

extern "C" {
int32_t GrallocInitialize(GrallocFuncs **funcs)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN((funcs == nullptr), DISPLAY_PARAM_ERR, DISPLAY_LOGE("funcs is nullptr"));
    GrallocFuncs *grallocFuncs = (GrallocFuncs *)malloc(sizeof(GrallocFuncs));
    DISPLAY_CHK_RETURN((grallocFuncs == nullptr), DISPLAY_NULL_PTR, DISPLAY_LOGE("malloc failed"));
    (void)memset_s(grallocFuncs, sizeof(GrallocFuncs), 0, sizeof(GrallocFuncs));
    // initialize gralloc
    int ret = AllocatorManager::GetInstance().Init();
    if (ret != DISPLAY_SUCCESS) {
        DISPLAY_LOGE("failed to initialize allocator manager");
        free(grallocFuncs);
        grallocFuncs = nullptr;
        return DISPLAY_FAILURE;
    }
    grallocFuncs->AllocMem = AllocMem;
    grallocFuncs->FreeMem = FreeMem;
    grallocFuncs->Mmap = Mmap;
    grallocFuncs->Unmap = Unmap;
    grallocFuncs->InvalidateCache = InvalidateCache;
    grallocFuncs->FlushCache = FlushCache;
    *funcs = grallocFuncs;
    DISPLAY_LOGD("gralloc initialize success");
    return DISPLAY_SUCCESS;
}

int32_t GrallocUninitialize(GrallocFuncs *funcs)
{
    DISPLAY_LOGD();
    DISPLAY_CHK_RETURN(funcs == nullptr, DISPLAY_PARAM_ERR, DISPLAY_LOGE("funcs is nullptr"));
    free(funcs);
    AllocatorManager::GetInstance().DeInit();
    return DISPLAY_SUCCESS;
}
}