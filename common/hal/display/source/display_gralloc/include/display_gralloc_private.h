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

#ifndef DISPLAY_GRALLOC_PRIVATE_H
#define DISPLAY_GRALLOC_PRIVATE_H
#include "display_type.h"
#ifdef __cplusplus
extern "C" {
#endif
#define GRALLOC_NUM_FDS 1
#define GRALLOC_NUM_INTS ((sizeof(struct PrivBufferHandle) - sizeof(BufferHandle)) / sizeof(int) - GRALLOC_NUM_FDS)

#define INVALID_PIXEL_FMT 0

typedef struct {
    BufferHandle hdl;
    int attrFd;
    void* attrVirAddr;
    uint64_t attrPhyAddr;
    int attrSize;
    int attrOffset;
    int addr;
    uint32_t offset;
} PrivateBufferHandle;

#define BUFFER_PRIV_SIZE (4 * 1024)
bool Convert2PrivateHandle(const BufferHandle *handle, PrivateBufferHandle *privateHandle);
#ifdef __cplusplus
}
#endif

#endif