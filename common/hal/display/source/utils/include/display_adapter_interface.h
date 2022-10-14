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

#ifndef DISPLAY_ADAPTER_INTERFACE_H
#define DISPLAY_ADAPTER_INTERFACE_H
#include <cstdint>
#include <sys/types.h>
#include "display_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    IRect rect;
    unsigned int stride;
    unsigned long bufaddr;
    int format;
    int inFence;
    int outFence;
} DisplayFrameInfo;

typedef struct {
    int32_t (*OpenDevice)(const char *path, int flags, mode_t mode);
    int32_t (*CloseDevice)(int32_t devFd);
    int32_t (*Ioctl)(int32_t devFd, uint32_t cmd, void *args);
    int32_t (*FbGetDmaBuffer)(int32_t devFd);
    int32_t (*FbFresh)(int32_t devFd, DisplayFrameInfo *frame);
} DisplayAdapterFuncs;

int32_t DisplayAdapaterInitialize(DisplayAdapterFuncs **funcs);
int32_t DisplayAdapaterUninitialize(DisplayAdapterFuncs *funcs);

#ifdef __cplusplus
}
#endif
#endif // FB_ADAPTER_INTERFACE_H