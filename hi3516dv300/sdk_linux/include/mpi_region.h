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
#ifndef __MPI_REGION_H__
#define __MPI_REGION_H__

#include "hi_comm_region.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32 HI_MPI_RGN_Create(RGN_HANDLE Handle, const RGN_ATTR_S *pstRegion);
HI_S32 HI_MPI_RGN_Destroy(RGN_HANDLE Handle);
HI_S32 HI_MPI_RGN_GetAttr(RGN_HANDLE Handle, RGN_ATTR_S *pstRegion);
HI_S32 HI_MPI_RGN_SetAttr(RGN_HANDLE Handle, const RGN_ATTR_S *pstRegion);
HI_S32 HI_MPI_RGN_SetBitMap(RGN_HANDLE Handle, const BITMAP_S *pstBitmap);
HI_S32 HI_MPI_RGN_AttachToChn(RGN_HANDLE Handle, const MPP_CHN_S *pstChn, const RGN_CHN_ATTR_S *pstChnAttr);
HI_S32 HI_MPI_RGN_DetachFromChn(RGN_HANDLE Handle, const MPP_CHN_S *pstChn);
HI_S32 HI_MPI_RGN_SetDisplayAttr(RGN_HANDLE Handle, const MPP_CHN_S *pstChn, const RGN_CHN_ATTR_S *pstChnAttr);
HI_S32 HI_MPI_RGN_GetDisplayAttr(RGN_HANDLE Handle, const MPP_CHN_S *pstChn, RGN_CHN_ATTR_S *pstChnAttr);
HI_S32 HI_MPI_RGN_GetCanvasInfo(RGN_HANDLE Handle, RGN_CANVAS_INFO_S *pstCanvasInfo);
HI_S32 HI_MPI_RGN_UpdateCanvas(RGN_HANDLE Handle);
HI_S32 HI_MPI_RGN_BatchBegin(RGN_HANDLEGROUP *pu32Group, HI_U32 u32Num, const RGN_HANDLE handle[]);
HI_S32 HI_MPI_RGN_BatchEnd(RGN_HANDLEGROUP u32Group);
HI_S32 HI_MPI_RGN_GetFd(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
