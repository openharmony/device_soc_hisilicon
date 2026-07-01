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
 */

#ifndef DISPLAY_ADPT_TDE_INCLUDE_H
#define DISPLAY_ADPT_TDE_INCLUDE_H

#include "ss_mpi_tde.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef ot_tde_alpha_blending TDE2_ALUCMD_E;
typedef ot_tde_rect TDE2_RECT_S;

/* Structure of the bitmap information set by customers */
typedef ot_tde_surface TDE2_SURFACE_S;

typedef ot_tde_rop_mode TDE2_ROP_CODE_E;

int32_t HI_TDE2_QuickDeflicker(int32_t s32Handle, TDE2_SURFACE_S *pstSrc, TDE2_RECT_S *pstSrcRect,
    TDE2_SURFACE_S *pstDst, TDE2_RECT_S *pstDstRect)
{
    (void)(s32Handle);
    (void)(pstSrc);
    (void)(pstSrcRect);
    (void)(pstDst);
    (void)(pstDstRect);
    printf("TDE not supported quick deflicker\n");
    return -1;
}

int32_t HI_TDE2_BitmapMaskRop(int32_t s32Handle, TDE2_SURFACE_S *pstBackGround,
    TDE2_RECT_S *pstBackGroundRect, TDE2_SURFACE_S *pstForeGround,
    TDE2_RECT_S *pstForeGroundRect, TDE2_SURFACE_S *pstMask,
    TDE2_RECT_S *pstMaskRect, TDE2_SURFACE_S *pstDst,
    TDE2_RECT_S *pstDstRect, TDE2_ROP_CODE_E ropColor,
    TDE2_ROP_CODE_E ropAlpha)
{
    (void)(s32Handle);
    (void)(pstBackGround);
    (void)(pstBackGroundRect);
    (void)(pstForeGround);
    (void)(pstForeGroundRect);
    (void)(pstMask);
    (void)(pstMaskRect);
    (void)(pstDst);
    (void)(pstDstRect);
    (void)(ropColor);
    (void)(ropAlpha);
    printf("TDE not supported bitmap mask rop\n");
    return -1;
}

int32_t HI_TDE2_BitmapMaskBlend(int32_t s32Handle, TDE2_SURFACE_S *pstBackGround,
    TDE2_RECT_S *pstBackGroundRect, TDE2_SURFACE_S *pstForeGround,
    TDE2_RECT_S *pstForeGroundRect, TDE2_SURFACE_S *pstMask,
    TDE2_RECT_S *pstMaskRect, TDE2_SURFACE_S *pstDst,
    TDE2_RECT_S *pstDstRect, uint8_t u8Alpha, TDE2_ALUCMD_E enBlendMode)
{
    (void)(s32Handle);
    (void)(pstBackGround);
    (void)(pstBackGroundRect);
    (void)(pstForeGround);
    (void)(pstForeGroundRect);
    (void)(pstMask);
    (void)(pstMaskRect);
    (void)(pstDst);
    (void)(pstDstRect);
    (void)(u8Alpha);
    (void)(enBlendMode);
    printf("TDE not supported bitmap mask blend\n");
    return -1;
}

#ifdef __cplusplus
}
#endif
#endif // DISPLAY_ADPT_TDE_H
