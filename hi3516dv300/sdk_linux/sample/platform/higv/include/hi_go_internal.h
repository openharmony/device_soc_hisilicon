/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HI_GO_INTERNAL_H
#define HI_GO_INTERNAL_H

#include "hi_go_comm.h"
#include "hi_go_bliter.h"
#include "hi_go_text.h"

#ifdef __cplusplus
extern "C" {
#endif

HI_S32 HI_GO_BlitEx(HI_HANDLE SrcSurface, const HI_RECT *pSrcRect, HI_HANDLE DstSurface, const HI_RECT *pDstRect,
    const HIGO_BLTOPT_S *pBlitOpt);

HI_S32 Handle_GetInstance(HI_HANDLE Handle, HI_VOID *phAddr, HI_U32 Model);

HI_S32 Handle_Alloc(HI_HANDLE *pHandle, HI_VOID *hAddr, HI_U32 Model);

HI_VOID Handle_Free(HI_HANDLE Handle);

HI_S32 Handle_AssignAlloc(HI_HANDLE Handle, HI_VOID *pAddr, HI_U32 Model);

HI_S32 Handle_AssignStaticArea(HI_HANDLE HandleMax);

HI_BOOL HIGO_IntersectRect(HI_REGION *pdrc, const HI_REGION *psrc1, const HI_REGION *psrc2);

HI_BOOL HIGO_UnionRect(HI_REGION *pdrc, const HI_REGION *psrc1, const HI_REGION *psrc2);

HI_BOOL HIGO_DoesIntersect(const HI_REGION *psrc1, const HI_REGION *psrc2);

HI_VOID HIGO_ADP_GfxSync(HI_VOID);

HI_S32 HI_GO_GetSurfaceClipRect(HI_HANDLE hSurface, HI_RECT *pRect);

HI_S32 HI_GO_SetLayerRotateMode(HI_HANDLE Layer, const HIGO_ROTATE_E RotateMode);

HI_S32 HI_GO_FreeWindowSurface(HI_HANDLE Window);

HI_S32 HI_GO_RebuildWindowSurface(HI_HANDLE Window);

HI_S32 HI_GO_CreateTextExByHandle(const HIGO_TEXT_INFO_S *pInfo, HI_HANDLE hFont);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
