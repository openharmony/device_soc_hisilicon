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

#ifndef DISPLAY_ADPT_HDMI_H
#define DISPLAY_ADPT_HDMI_H

#include "display_adapt_tde_include.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HI_ERR_TDE_INVALID_HANDLE OT_ERR_TDE_INVALID_HANDLE

typedef ot_tde_mb_surface TDE2_MB_S;
typedef ot_tde_mb_opt TDE2_MBOPT_S;
typedef ot_tde_pattern_fill_opt TDE2_PATTERN_FILL_OPT_S;
typedef ot_tde_rotate_angle TDE_ROTATE_ANGLE_E;

#define TDE2_COLOR_FMT_RGB444   OT_TDE_COLOR_FORMAT_RGB444
#define TDE2_COLOR_FMT_RGB555   OT_TDE_COLOR_FORMAT_RGB555
#define TDE2_COLOR_FMT_ARGB4444 OT_TDE_COLOR_FORMAT_ARGB4444
#define TDE2_COLOR_FMT_RGB565   OT_TDE_COLOR_FORMAT_RGB565
#define TDE2_COLOR_FMT_ARGB1555 OT_TDE_COLOR_FORMAT_ARGB1555
#define TDE2_COLOR_FMT_RGB888   OT_TDE_COLOR_FORMAT_RGB888
#define TDE2_COLOR_FMT_ARGB8888 OT_TDE_COLOR_FORMAT_ARGB8888

typedef ot_tde_opt TDE2_OPT_S;

#define TDE2_COLOR_FMT_RGB444       OT_TDE_COLOR_FORMAT_RGB444
#define TDE2_COLOR_FMT_BGR444       OT_TDE_COLOR_FORMAT_BGR444
#define TDE2_COLOR_FMT_RGB555       OT_TDE_COLOR_FORMAT_RGB555
#define TDE2_COLOR_FMT_BGR555       OT_TDE_COLOR_FORMAT_BGR555
#define TDE2_COLOR_FMT_RGB565       OT_TDE_COLOR_FORMAT_RGB565
#define TDE2_COLOR_FMT_BGR565       OT_TDE_COLOR_FORMAT_BGR565
#define TDE2_COLOR_FMT_RGB888       OT_TDE_COLOR_FORMAT_RGB888
#define TDE2_COLOR_FMT_BGR888       OT_TDE_COLOR_FORMAT_BGR888
#define TDE2_COLOR_FMT_ARGB4444     OT_TDE_COLOR_FORMAT_ARGB4444
#define TDE2_COLOR_FMT_ABGR4444     OT_TDE_COLOR_FORMAT_ABGR4444
#define TDE2_COLOR_FMT_RGBA4444     OT_TDE_COLOR_FORMAT_RGBA4444
#define TDE2_COLOR_FMT_BGRA4444     OT_TDE_COLOR_FORMAT_BGRA4444
#define TDE2_COLOR_FMT_ARGB1555     OT_TDE_COLOR_FORMAT_ARGB1555
#define TDE2_COLOR_FMT_ABGR1555     OT_TDE_COLOR_FORMAT_ABGR1555
#define TDE2_COLOR_FMT_RGBA1555     OT_TDE_COLOR_FORMAT_RGBA1555
#define TDE2_COLOR_FMT_BGRA1555     OT_TDE_COLOR_FORMAT_BGRA1555
#define TDE2_COLOR_FMT_ARGB8565     OT_TDE_COLOR_FORMAT_ARGB8565
#define TDE2_COLOR_FMT_ABGR8565     OT_TDE_COLOR_FORMAT_ABGR8565
#define TDE2_COLOR_FMT_RGBA8565     OT_TDE_COLOR_FORMAT_RGBA8565
#define TDE2_COLOR_FMT_BGRA8565     OT_TDE_COLOR_FORMAT_BGRA8565
#define TDE2_COLOR_FMT_ARGB8888     OT_TDE_COLOR_FORMAT_ARGB8888
#define TDE2_COLOR_FMT_ABGR8888     OT_TDE_COLOR_FORMAT_ABGR8888
#define TDE2_COLOR_FMT_RGBA8888     OT_TDE_COLOR_FORMAT_RGBA8888
#define TDE2_COLOR_FMT_BGRA8888     OT_TDE_COLOR_FORMAT_BGRA8888
#define TDE2_COLOR_FMT_RABG8888     OT_TDE_COLOR_FORMAT_RABG8888
#define TDE2_COLOR_FMT_CLUT1        OT_TDE_COLOR_FORMAT_CLUT1
#define TDE2_COLOR_FMT_CLUT2        OT_TDE_COLOR_FORMAT_CLUT2
#define TDE2_COLOR_FMT_CLUT4        OT_TDE_COLOR_FORMAT_CLUT4
#define TDE2_COLOR_FMT_CLUT8        OT_TDE_COLOR_FORMAT_CLUT8
#define TDE2_COLOR_FMT_ACLUT44      OT_TDE_COLOR_FORMAT_ACLUT44
#define TDE2_COLOR_FMT_ACLUT88      OT_TDE_COLOR_FORMAT_ACLUT88
#define TDE2_COLOR_FMT_A1           OT_TDE_COLOR_FORMAT_A1
#define TDE2_COLOR_FMT_A8           OT_TDE_COLOR_FORMAT_A8
#define TDE2_COLOR_FMT_YCBCR888     OT_TDE_COLOR_FORMAT_YCbCr888
#define TDE2_COLOR_FMT_AYCBCR888   OT_TDE_COLOR_FORMAT_AYCbCr8888
#define TDE2_COLOR_FMT_YCBCR422     OT_TDE_COLOR_FORMAT_YCbCr422
#define TDE2_COLOR_FMT_PKGVYUY      OT_TDE_COLOR_FORMAT_PKGVYUY
#define TDE2_COLOR_FMT_BYTE         OT_TDE_COLOR_FORMAT_BYTE
#define TDE2_COLOR_FMT_HALFWORD     OT_TDE_COLOR_FORMAT_HALFWORD
#define TDE2_COLOR_FMT_JPG_YCBCR400MBP    OT_TDE_COLOR_FORMAT_JPG_YCbCr400MBP
#define TDE2_COLOR_FMT_JPG_YCBCR422MBHP   OT_TDE_COLOR_FORMAT_JPG_YCbCr422MBHP
#define TDE2_COLOR_FMT_JPG_YCBCR422MBVP   OT_TDE_COLOR_FORMAT_JPG_YCbCr422MBVP
#define TDE2_COLOR_FMT_MP1_YCBCR420MBP    OT_TDE_COLOR_FORMAT_MP1_YCbCr420MBP
#define TDE2_COLOR_FMT_MP2_YCBCR420MBP    OT_TDE_COLOR_FORMAT_MP2_YCbCr420MBP
#define TDE2_COLOR_FMT_MP2_YCBCR420MBI    OT_TDE_COLOR_FORMAT_MP2_YCbCr420MBI
#define TDE2_COLOR_FMT_JPG_YCBCR420MBP    OT_TDE_COLOR_FORMAT_JPG_YCbCr420MBP
#define TDE2_COLOR_FMT_JPG_YCBCR444MBP    OT_TDE_COLOR_FORMAT_JPG_YCbCr444MBP
#define TDE2_COLOR_FMT_MAX                OT_TDE_COLOR_FORMAT_MAX
typedef ot_tde_color_format TDE2_COLOR_FMT_E;

/*
 * Alpha blending command. You can set parameters or select Porter or Duff.
 * pixel = (source * fs + destination * fd),
 * sa = source alpha,
 * da = destination alpha
 */
#define TDE2_BLENDCMD_NONE       OT_TDE_BLEND_CMD_NONE
#define TDE2_BLENDCMD_CLEAR      OT_TDE_BLEND_CMD_CLEAR
#define TDE2_BLENDCMD_SRC        OT_TDE_BLEND_CMD_SRC
#define TDE2_BLENDCMD_SRCOVER    OT_TDE_BLEND_CMD_SRCOVER
#define TDE2_BLENDCMD_DSTOVER    OT_TDE_BLEND_CMD_DSTOVER
#define TDE2_BLENDCMD_SRCIN      OT_TDE_BLEND_CMD_SRCIN
#define TDE2_BLENDCMD_DSTIN      OT_TDE_BLEND_CMD_DSTIN
#define TDE2_BLENDCMD_SRCOUT     OT_TDE_BLEND_CMD_SRCOUT
#define TDE2_BLENDCMD_DSTOUT     OT_TDE_BLEND_CMD_DSTOUT
#define TDE2_BLENDCMD_SRCATOP    OT_TDE_BLEND_CMD_SRCATOP
#define TDE2_BLENDCMD_DSTATOP    OT_TDE_BLEND_CMD_DSTATOP
#define TDE2_BLENDCMD_ADD        OT_TDE_BLEND_CMD_ADD
#define TDE2_BLENDCMD_XOR        OT_TDE_BLEND_CMD_XOR
#define TDE2_BLENDCMD_DST        OT_TDE_BLEND_CMD_DST
#define TDE2_BLENDCMD_CONFIG     OT_TDE_BLEND_CMD_CONFIG
#define TDE2_BLENDCMD_BUTT       OT_TDE_BLEND_CMD_MAX
typedef ot_tde_blend_cmd TDE2_BLENDCMD_E;

typedef ot_tde_blend_opt TDE2_BLEND_OPT_S;

typedef ot_tde_fill_color TDE2_FILLCOLOR_S;

#define TDE2_OUTALPHA_FROM_NORM         OT_TDE_OUT_ALPHA_FROM_NORM
#define TDE2_OUTALPHA_FROM_BACKGROUND   OT_TDE_OUT_ALPHA_FROM_BG
#define TDE2_OUTALPHA_FROM_FOREGROUND   OT_TDE_OUT_ALPHA_FROM_FG
#define TDE2_OUTALPHA_FROM_GLOBALALPHA  OT_TDE_OUT_ALPHA_FROM_GLOBALALPHA
#define TDE2_OUTALPHA_FROM_BUTT         OT_TDE_OUT_ALPHA_FROM_MAX

#define TDE2_ALUCMD_NONE                OT_TDE_ALPHA_BLENDING_NONE
#define TDE2_ALUCMD_BLEND               OT_TDE_ALPHA_BLENDING_BLEND
#define TDE2_ALUCMD_ROP                 OT_TDE_ALPHA_BLENDING_ROP
#define TDE2_ALUCMD_COLORIZE            OT_TDE_ALPHA_BLENDING_COLORIZE
#define TDE2_ALUCMD_BUTT                OT_TDE_ALPHA_BLENDING_MAX

#define TDE2_FILTER_MODE_BOTH           OT_TDE_FILTER_MODE_BOTH

/* Definition of ROP codes */
#define TDE2_ROP_BLACK          OT_TDE_ROP_BLACK
#define TDE2_ROP_NOTMERGEPEN    OT_TDE_ROP_NOTMERGEPEN
#define TDE2_ROP_MASKNOTPEN     OT_TDE_ROP_MASKNOTPEN
#define TDE2_ROP_NOTCOPYPEN     OT_TDE_ROP_NOTCOPYPEN
#define TDE2_ROP_MASKPENNOT     OT_TDE_ROP_MASKPENNOT
#define TDE2_ROP_NOT            OT_TDE_ROP_NOT
#define TDE2_ROP_XORPEN         OT_TDE_ROP_XORPEN
#define TDE2_ROP_NOTMASKPEN     OT_TDE_ROP_NOTMASKPEN
#define TDE2_ROP_MASKPEN        OT_TDE_ROP_MASKPEN
#define TDE2_ROP_NOTXORPEN      OT_TDE_ROP_NOTXORPEN
#define TDE2_ROP_NOP            OT_TDE_ROP_NOP
#define TDE2_ROP_MERGENOTPEN    OT_TDE_ROP_MERGENOTPEN
#define TDE2_ROP_COPYPEN        OT_TDE_ROP_COPYPEN
#define TDE2_ROP_MERGEPENNOT    OT_TDE_ROP_MERGEPENNOT
#define TDE2_ROP_MERGEPEN       OT_TDE_ROP_MERGEPEN
#define TDE2_ROP_WHITE          OT_TDE_ROP_WHITE
#define TDE2_ROP_BUTT           OT_TDE_ROP_MAX

int32_t HI_TDE2_QuickCopy(int32_t s32Handle, TDE2_SURFACE_S *pstSrc, TDE2_RECT_S *pstSrcRect,
    TDE2_SURFACE_S *pstDst, TDE2_RECT_S *pstDstRect)
{
    ot_tde_single_src singleSrc = { 0 };

    singleSrc.src_surface = pstSrc;
    singleSrc.dst_surface = pstDst;
    singleSrc.src_rect = pstSrcRect;
    singleSrc.dst_rect = pstDstRect;

    return ss_tde_quick_copy(s32Handle, &singleSrc);
}

int32_t HI_TDE2_QuickFill(int32_t s32Handle, TDE2_SURFACE_S *pstDst, TDE2_RECT_S *pstDstRect,
    td_u32 u32FillData)
{
    ot_tde_none_src noneSrc = { 0 };

    noneSrc.dst_surface = pstDst;
    noneSrc.dst_rect = pstDstRect;

    return ss_tde_quick_fill(s32Handle, &noneSrc, u32FillData);
}

int32_t HI_TDE2_QuickResize(int32_t s32Handle, TDE2_SURFACE_S *pstSrc, TDE2_RECT_S *pstSrcRect,
    TDE2_SURFACE_S *pstDst, TDE2_RECT_S *pstDstRect)
{
    ot_tde_single_src singleSrc = { 0 };

    singleSrc.src_surface = pstSrc;
    singleSrc.dst_surface = pstDst;
    singleSrc.src_rect = pstSrcRect;
    singleSrc.dst_rect = pstDstRect;

    return ss_tde_quick_resize(s32Handle, &singleSrc);
}

int32_t HI_TDE2_Bitblit(int32_t s32Handle, TDE2_SURFACE_S *pstBackGround, TDE2_RECT_S *pstBackGroundRect,
    TDE2_SURFACE_S *pstForeGround, TDE2_RECT_S *pstForeGroundRect,
    TDE2_SURFACE_S *pstDst, TDE2_RECT_S *pstDstRect, TDE2_OPT_S *pstOpt)
{
    ot_tde_double_src doubleSrc = { 0 };

    doubleSrc.bg_surface = pstBackGround;
    doubleSrc.fg_surface = pstForeGround;
    doubleSrc.dst_surface = pstDst;
    doubleSrc.bg_rect = pstBackGroundRect;
    doubleSrc.fg_rect = pstForeGroundRect;
    doubleSrc.dst_rect = pstDstRect;

    return ss_tde_bit_blit(s32Handle, &doubleSrc, pstOpt);
}


int32_t HI_TDE2_SolidDraw(int32_t s32Handle,
    const TDE2_SURFACE_S *pstForeGround, const TDE2_RECT_S *pstForeGroundRect, const TDE2_SURFACE_S *pstDst,
    const TDE2_RECT_S *pstDstRect, const TDE2_FILLCOLOR_S *pstFillColor, const TDE2_OPT_S *pstOpt)
{
    ot_tde_single_src single = { 0 };
    single.src_surface = (ot_tde_surface *)pstForeGround;
    single.dst_surface = (ot_tde_surface *)pstDst;
    single.src_rect = (ot_tde_rect *)pstForeGroundRect;
    single.dst_rect = (ot_tde_rect *)pstDstRect;
    return ss_tde_solid_draw(s32Handle, &single, pstFillColor, pstOpt);
}

int32_t HI_TDE2_MbBlit(int32_t s32Handle, TDE2_MB_S *pstMB, TDE2_RECT_S *pstMbRect,
    TDE2_SURFACE_S *pstDst, TDE2_RECT_S *pstDstRect, TDE2_MBOPT_S *pstMbOpt)
{
    ot_tde_mb_src mbSrc = { 0 };

    mbSrc.mb_surface = pstMB;
    mbSrc.dst_surface = pstDst;
    mbSrc.src_rect = pstMbRect;
    mbSrc.dst_rect = pstDstRect;

    return ss_tde_mb_blit(s32Handle, &mbSrc, pstMbOpt);
}

int32_t HI_TDE2_Rotate(int32_t s32Handle, TDE2_SURFACE_S *pstSrc, TDE2_RECT_S *pstSrcRect,
    TDE2_SURFACE_S *pstDst, TDE2_RECT_S *pstDstRect, TDE_ROTATE_ANGLE_E enRotateAngle)
{
    ot_tde_single_src singleSrc = { 0 };

    singleSrc.src_surface = pstSrc;
    singleSrc.dst_surface = pstDst;
    singleSrc.src_rect = pstSrcRect;
    singleSrc.dst_rect = pstDstRect;

    return ss_tde_rotate(s32Handle, &singleSrc, enRotateAngle);
}

int32_t HI_TDE2_PatternFill(int32_t s32Handle, TDE2_SURFACE_S *pstBackGround,
    TDE2_RECT_S *pstBackGroundRect, TDE2_SURFACE_S *pstForeGround,
    TDE2_RECT_S *pstForeGroundRect, TDE2_SURFACE_S *pstDst,
    TDE2_RECT_S *pstDstRect, TDE2_PATTERN_FILL_OPT_S *pstOpt)
{
    ot_tde_double_src doubleSrc = { 0 };

    doubleSrc.bg_surface = pstBackGround;
    doubleSrc.fg_surface = pstForeGround;
    doubleSrc.dst_surface = pstDst;
    doubleSrc.bg_rect = pstBackGroundRect;
    doubleSrc.fg_rect = pstForeGroundRect;
    doubleSrc.dst_rect = pstDstRect;

    return ss_tde_pattern_fill(s32Handle, &doubleSrc, pstOpt);
}

#ifdef __cplusplus
}
#endif
#endif // DISPLAY_ADPT_TDE_H
