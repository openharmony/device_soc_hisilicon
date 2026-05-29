/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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
 *
 * Description: muxer buffer manager interface
 */

#ifndef REC_BUFFER_MANAGER_H
#define REC_BUFFER_MANAGER_H
#include "ot_mw_type.h"
#include "ot_track_source.h"

td_s32 RecAllocBuf(const void *pBufArg, td_u32 bufSize, void **ppVmAddr);

td_s32 RecFreeBuf(const void *pBufArg, td_u32 bufSize, void *pVmAddr);

#endif
