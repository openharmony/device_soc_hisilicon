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
    ot_tde_single_src singleSrc = { 0 };

    singleSrc.src_surface = pstSrc;
    singleSrc.dst_surface = pstDst;
    singleSrc.src_rect = pstSrcRect;
    singleSrc.dst_rect = pstDstRect;

    return ss_tde_quick_deflicker(s32Handle, &singleSrc);
}

int32_t HI_TDE2_BitmapMaskRop(int32_t s32Handle, TDE2_SURFACE_S *pstBackGround,
    TDE2_RECT_S *pstBackGroundRect, TDE2_SURFACE_S *pstForeGround,
    TDE2_RECT_S *pstForeGroundRect, TDE2_SURFACE_S *pstMask,
    TDE2_RECT_S *pstMaskRect, TDE2_SURFACE_S *pstDst,
    TDE2_RECT_S *pstDstRect, TDE2_ROP_CODE_E ropColor,
    TDE2_ROP_CODE_E ropAlpha)
{
    ot_tde_triple_src tripleSrc = { 0 };

    tripleSrc.bg_surface = pstBackGround;
    tripleSrc.fg_surface = pstForeGround;
    tripleSrc.dst_surface = pstDst;
    tripleSrc.mask_surface = pstMask;
    tripleSrc.bg_rect = pstBackGroundRect;
    tripleSrc.fg_rect = pstForeGroundRect;
    tripleSrc.mask_rect = pstMaskRect;
    tripleSrc.dst_rect = pstDstRect;

    return ss_tde_bitmap_mask_rop(s32Handle, &tripleSrc, ropColor, ropAlpha);
}

int32_t HI_TDE2_BitmapMaskBlend(int32_t s32Handle, TDE2_SURFACE_S *pstBackGround,
    TDE2_RECT_S *pstBackGroundRect, TDE2_SURFACE_S *pstForeGround,
    TDE2_RECT_S *pstForeGroundRect, TDE2_SURFACE_S *pstMask,
    TDE2_RECT_S *pstMaskRect, TDE2_SURFACE_S *pstDst,
    TDE2_RECT_S *pstDstRect, uint8_t u8Alpha, TDE2_ALUCMD_E enBlendMode)
{
    ot_tde_triple_src tripleSrc = { 0 };

    tripleSrc.bg_surface = pstBackGround;
    tripleSrc.fg_surface = pstForeGround;
    tripleSrc.dst_surface = pstDst;
    tripleSrc.mask_surface = pstMask;
    tripleSrc.bg_rect = pstBackGroundRect;
    tripleSrc.fg_rect = pstForeGroundRect;
    tripleSrc.mask_rect = pstMaskRect;
    tripleSrc.dst_rect = pstDstRect;

    return ss_tde_bitmap_mask_blend(s32Handle, &tripleSrc, u8Alpha, enBlendMode);
}

#ifdef __cplusplus
}
#endif
#endif // DISPLAY_ADPT_TDE_H
