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

#include "display_gfx.h"
#include <securec.h>
#include "disp_common.h"
#include "display_adapt_fb.h"
#include "display_gfx_internal.h"

typedef struct {
    BlendType blendType;
    TDE2_BLENDCMD_E tdeBlendCmd;
} Tde2OptBlendCmdConvTable;

static TD_BOOL g_gfxInited = TD_FALSE;

static int32_t ConvertFormat(PixelFormat colorFmt, TDE2_COLOR_FMT_E *tdeColorFmt)
{
    static struct ColorFmtConvert fmtMapTable[] = {
        {PIXEL_FMT_RGB_444, TDE2_COLOR_FMT_RGB444},
        {PIXEL_FMT_RGBA_4444, TDE2_COLOR_FMT_ARGB4444},
        {PIXEL_FMT_RGB_555, TDE2_COLOR_FMT_RGB555},
        {PIXEL_FMT_RGB_565, TDE2_COLOR_FMT_RGB565},
        {PIXEL_FMT_RGBA_5551, TDE2_COLOR_FMT_ARGB1555},
        {PIXEL_FMT_RGB_888, TDE2_COLOR_FMT_RGB888},
        {PIXEL_FMT_RGBA_8888, TDE2_COLOR_FMT_ARGB8888},
    };

    int32_t len = (int32_t)(sizeof(fmtMapTable) / sizeof(fmtMapTable[0]));
    for (int32_t i = 0; i < len; i++) {
        if (fmtMapTable[i].pixFmt == colorFmt) {
            *tdeColorFmt = fmtMapTable[i].tdePixFmt;
            return DISPLAY_SUCCESS;
        }
    }
    return DISPLAY_FAILURE;
}

static int32_t Convert2TdeSurface(const ISurface *surface, TDE2_SURFACE_S *tdeSurface)
{
    if (ConvertFormat(surface->enColorFmt, &tdeSurface->color_format) != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: unsupport color format", __func__);
        return DISPLAY_FAILURE;
    }
    tdeSurface->phys_addr = surface->phyAddr;
    tdeSurface->height = surface->height < 0 ? 0 : surface->height;
    tdeSurface->width = surface->width < 0 ? 0 : surface->width;
    tdeSurface->stride = surface->stride < 0 ? 0 : surface->stride;
    tdeSurface->clut_phys_addr = surface->clutPhyAddr;
    tdeSurface->is_ycbcr_clut = surface->bYCbCrClut;
    tdeSurface->alpha_max_is_255 = surface->bAlphaMax255;
    tdeSurface->support_alpha_ex_1555 = surface->bAlphaExt1555;
    tdeSurface->alpha0 = surface->alpha0;
    tdeSurface->alpha1 = surface->alpha1;
    return DISPLAY_SUCCESS;
}

static void Convert2TdeRect(const IRect *src, TDE2_RECT_S *dest)
{
    dest->pos_x = src->x;
    dest->pos_y = src->y;
    dest->width = src->w < 0 ? 0 : src->w;
    dest->height = src->h < 0 ? 0 : src->h;
}

static void ConvertBelendOpt(BlendType type, TDE2_OPT_S *stOpt)
{
    Tde2OptBlendCmdConvTable convTable[] = {
        {BLEND_NONE, TDE2_BLENDCMD_NONE},       {BLEND_CLEAR, TDE2_BLENDCMD_CLEAR},
        {BLEND_SRC, TDE2_BLENDCMD_SRC},         {BLEND_SRCOVER, TDE2_BLENDCMD_SRCOVER},
        {BLEND_DSTOVER, TDE2_BLENDCMD_DSTOVER}, {BLEND_SRCIN, TDE2_BLENDCMD_SRCIN},
        {BLEND_DSTIN, TDE2_BLENDCMD_DSTIN},     {BLEND_SRCOUT, TDE2_BLENDCMD_SRCOUT},
        {BLEND_DSTOUT, TDE2_BLENDCMD_DSTOUT},   {BLEND_SRCATOP, TDE2_BLENDCMD_SRCATOP},
        {BLEND_DSTATOP, TDE2_BLENDCMD_DSTATOP}, {BLEND_ADD, TDE2_BLENDCMD_ADD},
        {BLEND_XOR, TDE2_BLENDCMD_XOR},         {BLEND_AKS, TDE2_BLENDCMD_SRCOVER},
        {BLEND_DST, TDE2_BLENDCMD_DST},         {BLEND_AKD, TDE2_BLENDCMD_SRCOVER}};

    stOpt->out_alpha_from = TDE2_OUTALPHA_FROM_NORM;
    if (type == BLEND_AKS) {
        stOpt->out_alpha_from = TDE2_OUTALPHA_FROM_FOREGROUND;
    } else if (type == BLEND_AKD) {
        stOpt->out_alpha_from = TDE2_OUTALPHA_FROM_BACKGROUND;
    }

    for (uint32_t index = 0; index < sizeof(convTable) / sizeof(convTable[0]); index++) {
        if (type == convTable[index].blendType) {
            stOpt->blend_opt.blend_cmd = convTable[index].tdeBlendCmd;
            return;
        }
    }
    stOpt->blend_opt.blend_cmd = TDE2_BLENDCMD_NONE;
}

static void ConvertTde2OptAlpha(const GfxOpt *opt, TDE2_OPT_S *stOpt)
{
    if (opt->enPixelAlpha || opt->enGlobalAlpha) {
        if (opt->enPixelAlpha) {
            stOpt->blend_opt.pixel_alpha_en = TD_TRUE;
        }
        stOpt->alpha_blending_cmd = TDE2_ALUCMD_BLEND;
        stOpt->out_alpha_from = TDE2_OUTALPHA_FROM_NORM;
        if (opt->blendType != BLEND_NONE) {
            ConvertBelendOpt(opt->blendType, stOpt);
        }
        stOpt->global_alpha = 255; /* 255, globleAlpha is 0xff */
        if (opt->enGlobalAlpha == TD_TRUE) {
            stOpt->blend_opt.global_alpha_en = TD_TRUE;
            stOpt->global_alpha = opt->globalAlpha > UINT8_MAX ? UINT8_MAX : opt->globalAlpha;
        }
        if (stOpt->blend_opt.blend_cmd == TDE2_BLENDCMD_SRCOVER) {
            stOpt->blend_opt.src1_alpha_premulti = TD_FALSE;
            stOpt->blend_opt.src2_alpha_premulti = TD_TRUE;
        } else if (stOpt->blend_opt.blend_cmd == TDE2_BLENDCMD_DSTOVER) {
            stOpt->blend_opt.src1_alpha_premulti = TD_TRUE;
            stOpt->blend_opt.src2_alpha_premulti = TD_FALSE;
        } else {
            stOpt->blend_opt.src1_alpha_premulti = TD_FALSE;
            stOpt->blend_opt.src2_alpha_premulti = TD_FALSE;
        }
    }
}

static void GenerateTde2Opt(const GfxOpt *opt, TDE2_OPT_S *stOpt)
{
    if (opt->enableRop) {
        stOpt->rop_color = (TDE2_ROP_CODE_E)opt->colorRopType;
        stOpt->rop_alpha = (TDE2_ROP_CODE_E)opt->alphaRopType;
    } else {
        ConvertTde2OptAlpha(opt, stOpt);
    }

    if (opt->enableScale) {
        stOpt->resize = TD_TRUE;
        stOpt->filter_mode = TDE2_FILTER_MODE_BOTH;
    }
}

static int32_t DoBlit(TDE2_SURFACE_S *srcSurface, TDE2_RECT_S *srcRect, TDE2_SURFACE_S *dstSurface,
                      TDE2_RECT_S *dstRect, GfxOpt *opt)
{
    TDE2_OPT_S stOpt;
    (void)memset_s(&stOpt, sizeof(TDE2_OPT_S), 0, sizeof(TDE2_OPT_S));
    int32_t hdl = ss_tde_begin_job();
    if (hdl == HI_ERR_TDE_INVALID_HANDLE) {
        HDF_LOGE("%s: ss_tde_begin_job fail", __func__);
        return DISPLAY_FAILURE;
    }
    int32_t ret = DISPLAY_FAILURE;
    if (opt == NULL) {
        ret = HI_TDE2_Bitblit(hdl, NULL, NULL, srcSurface, srcRect, dstSurface, dstRect, &stOpt);
    } else {
        GenerateTde2Opt(opt, &stOpt);
        ret = HI_TDE2_Bitblit(hdl, dstSurface, dstRect, srcSurface, srcRect, dstSurface, dstRect, &stOpt);
    }
    if (ret != DISPLAY_SUCCESS) {
        goto EXIT;
    }
    ret = ss_tde_end_job(hdl, TD_FALSE, TD_TRUE, TIME_OUT);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: ss_tde_end_job fail, ret 0x%x", __func__, ret);
        goto EXIT;
    }
    return ret;

EXIT:
    ss_tde_cancel_job(hdl);
    return ret;
}

static int32_t Blit(ISurface *srcSurface, IRect *srcRect, ISurface *dstSurface, IRect *dstRect, GfxOpt *opt)
{
    TDE2_SURFACE_S tdeSrcSurface;
    TDE2_SURFACE_S tdeDstSurface;
    TDE2_RECT_S tdeSrcRect;
    TDE2_RECT_S tdeDstRect;

    CHECK_NULLPOINTER_RETURN_VALUE(srcSurface, DISPLAY_NULL_PTR);
    CHECK_NULLPOINTER_RETURN_VALUE(srcRect, DISPLAY_NULL_PTR);
    CHECK_NULLPOINTER_RETURN_VALUE(dstSurface, DISPLAY_NULL_PTR);
    CHECK_NULLPOINTER_RETURN_VALUE(dstRect, DISPLAY_NULL_PTR);
    (void)memset_s(&tdeSrcSurface, sizeof(TDE2_SURFACE_S), 0, sizeof(TDE2_SURFACE_S));
    (void)memset_s(&tdeDstSurface, sizeof(TDE2_SURFACE_S), 0, sizeof(TDE2_SURFACE_S));
    if (Convert2TdeSurface(srcSurface, &tdeSrcSurface) != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: convert srcSurface to TDE surface failure", __func__);
        return DISPLAY_FAILURE;
    }
    if (Convert2TdeSurface(dstSurface, &tdeDstSurface) != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: convert dstSurface to TDE surface failure", __func__);
        return DISPLAY_FAILURE;
    }
    Convert2TdeRect(srcRect, &tdeSrcRect);
    Convert2TdeRect(dstRect, &tdeDstRect);
    int32_t ret = DoBlit(&tdeSrcSurface, &tdeSrcRect, &tdeDstSurface, &tdeDstRect, opt);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: Do TDE job failure", __func__);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

static int32_t DoFillRect(TDE2_SURFACE_S *surface, TDE2_RECT_S *rect, uint32_t color, GfxOpt *opt)
{
    int32_t ret;
    TDE2_OPT_S stOpt;

    (void)memset_s(&stOpt, sizeof(TDE2_OPT_S), 0, sizeof(TDE2_OPT_S));
    stOpt.alpha_blending_cmd = TDE2_ALUCMD_BLEND;
    stOpt.blend_opt.global_alpha_en = TD_TRUE;
    stOpt.global_alpha = opt->globalAlpha > UINT8_MAX ? UINT8_MAX : opt->globalAlpha;
    stOpt.out_alpha_from = TDE2_OUTALPHA_FROM_FOREGROUND;

    int32_t hdl = ss_tde_begin_job();
    if (hdl == HI_ERR_TDE_INVALID_HANDLE) {
        HDF_LOGE("%s: ss_tde_begin_job failed", __func__);
        return DISPLAY_FAILURE;
    }
    if (!opt->enGlobalAlpha) {
        ret = HI_TDE2_QuickFill(hdl, surface, rect, color);
        if (ret != DISPLAY_SUCCESS) {
            HDF_LOGE("%s: HI_TDE2_QuickFill failure, ret 0x%x", __func__, ret);
            ret = DISPLAY_FAILURE;
            goto EXIT;
        }
    } else {
        TDE2_FILLCOLOR_S fillColor;
        fillColor.color_format = surface->color_format;
        fillColor.color_value = color;
        ret = HI_TDE2_SolidDraw(hdl, surface, rect, surface, rect, &fillColor, &stOpt);
        if (ret != DISPLAY_SUCCESS) {
            HDF_LOGE("%s: HI_TDE2_SolidDraw failure, ret 0x%x", __func__, ret);
            ret = DISPLAY_FAILURE;
            goto EXIT;
        }
    }
    ret = ss_tde_end_job(hdl, TD_FALSE, TD_TRUE, TIME_OUT);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: ss_tde_end_job failure, ret 0x%x", __func__, ret);
        ret = DISPLAY_FAILURE;
        goto EXIT;
    }
    return ret;

EXIT:
    ss_tde_cancel_job(hdl);
    return ret;
}

static int32_t FillRect(ISurface *surface, IRect *rect, uint32_t color, GfxOpt *opt)
{
    int32_t ret;
    TDE2_SURFACE_S tdeSurface;
    TDE2_RECT_S tdeRect;

    CHECK_NULLPOINTER_RETURN_VALUE(surface, DISPLAY_NULL_PTR);
    CHECK_NULLPOINTER_RETURN_VALUE(rect, DISPLAY_NULL_PTR);
    CHECK_NULLPOINTER_RETURN_VALUE(opt, DISPLAY_NULL_PTR);
    (void)memset_s(&tdeSurface, sizeof(TDE2_SURFACE_S), 0, sizeof(TDE2_SURFACE_S));
    if (Convert2TdeSurface(surface, &tdeSurface) != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: convert surface to TDE surface failure", __func__);
        return DISPLAY_FAILURE;
    }
    Convert2TdeRect(rect, &tdeRect);
    ret = DoFillRect(&tdeSurface, &tdeRect, color, opt);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: do fill rect failure", __func__);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

static int32_t InitGfx(void)
{
    if (g_gfxInited == TD_TRUE) {
        HDF_LOGI("%s: gfx has inited", __func__);
        return DISPLAY_SUCCESS;
    }
    if (ss_tde_open() != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: TDEInit failure", __func__);
        return DISPLAY_FAILURE;
    }
    g_gfxInited = TD_TRUE;
    return DISPLAY_SUCCESS;
}

static int32_t DeinitGfx(void)
{
    if (g_gfxInited == TD_FALSE) {
        HDF_LOGI("%s: gfx has uninited", __func__);
        return DISPLAY_SUCCESS;
    }
    ss_tde_close();
    g_gfxInited = TD_FALSE;
    return DISPLAY_SUCCESS;
}

int32_t GfxInitialize(GfxFuncs **funcs)
{
    CHECK_NULLPOINTER_RETURN_VALUE(funcs, DISPLAY_NULL_PTR);
    GfxFuncs *gFuncs = (GfxFuncs *)malloc(sizeof(GfxFuncs));
    if (gFuncs == NULL) {
        HDF_LOGE("%s: gFuncs is null", __func__);
        return DISPLAY_NULL_PTR;
    }
    (void)memset_s(gFuncs, sizeof(GfxFuncs), 0, sizeof(GfxFuncs));
    gFuncs->InitGfx = InitGfx;
    gFuncs->DeinitGfx = DeinitGfx;
    gFuncs->Blit = Blit;
    gFuncs->FillRect = FillRect;
    *funcs = gFuncs;
    HDF_LOGI("%s: gfx initialize success", __func__);
    return DISPLAY_SUCCESS;
}

int32_t GfxUninitialize(GfxFuncs *funcs)
{
    CHECK_NULLPOINTER_RETURN_VALUE(funcs, DISPLAY_NULL_PTR);
    free(funcs);
    HDF_LOGI("%s: gfx uninitialize success", __func__);
    return DISPLAY_SUCCESS;
}
