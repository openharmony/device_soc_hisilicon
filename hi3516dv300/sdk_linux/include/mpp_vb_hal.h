/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

#ifndef __MPP_VB_HAL__
#define __MPP_VB_HAL__

#include "hi_type.h"
#include "hi_comm_vb.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define HI_VBS_INVALID_BUF_HANDLE ((HI_U64)-1)

typedef struct {
    HI_S32 sharefd;
    HI_U32 size;
    HI_U64 phy;
    HI_U64 handle;  /* buffer identification, must be unique in current process. invalid value is
                       HI_VBS_INVALID_BUF_HANDLE */
    HI_BOOL filled; /* if buffer is filled with data, or a empty buffer */
} VBSBufferInfo;

VB_POOL HI_VBS_CreateExtPool(HI_U64 blkSize, HI_U32 blkCnt);
HI_S32 HI_VBS_DestroyPool(VB_POOL poolId);

HI_S32 HI_VBS_AddBufToExtPool(VB_POOL poolId, VBSBufferInfo *bufInfo);
HI_S32 HI_VBS_DeleteBufFromExtPool(VB_BLK blockId);

HI_S32 HI_VBS_BufferToVB(VB_POOL poolId, HI_U64 bufHandle);
HI_U64 HI_VBS_VBToBuffer(HI_S32 blockId);

HI_S32 HI_VBS_ShowServerInfo(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
