/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART16_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART16_H_

            tde_error("This operation doesn't support Semi-plannar!\n");
            return DRV_ERR_TDE_INVALID_PARA;
        }

        return tde_osi_2_source_fill(handle, single_src, fill_color, opt);
    }
}

#if (TDE_CAPABILITY & DEFLICKER)
td_s32 tde_osi_set_deflicker_level(drv_tde_deflicker_level deflicker_level)
{
    return tde_hal_set_deflicer_level(deflicker_level);
}

td_s32 tde_osi_get_deflicker_level(drv_tde_deflicker_level *deflicker_level)
{
    return tde_hal_get_deflicer_level(deflicker_level);
}
#endif

td_s32 tde_osi_set_alpha_threshold_value(td_u8 alpha_threshold_value)
{
    return tde_hal_set_alpha_threshold(alpha_threshold_value);
}

td_s32 tde_osi_set_alpha_threshold_state(td_bool alpha_threshold_en)
{
    return tde_hal_set_alpha_threshold_state(alpha_threshold_en);
}

td_s32 tde_osi_get_alpha_threshold_value(td_u8 *alpha_threshold_value)
{
    return tde_hal_get_alpha_threshold(alpha_threshold_value);
}

td_s32 tde_osi_get_alpha_threshold_state(td_bool *alpha_threshold_en)
{
    return tde_hal_get_alpha_threshold_state(alpha_threshold_en);
}

static td_s32 tde_osi_check_src_pattern_fill_opt(const drv_tde_pattern_fill_opt *opt)
{
    td_bool is_invalid;
    if (opt == TD_NULL) {
        return TD_FAILURE;
    }

    is_invalid = (opt->colorkey_mode >= DRV_TDE_COLOR_KEY_MODE_MAX) ||
        (opt->colorkey_mode < DRV_TDE_COLOR_KEY_MODE_NONE);
    if (is_invalid == TD_TRUE) {
        tde_error("color_key_mode error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    is_invalid = ((opt->clut_reload != TD_TRUE) && (opt->clut_reload != TD_FALSE)) ||
        ((opt->blend_opt.global_alpha_en != TD_TRUE) && (opt->blend_opt.global_alpha_en != TD_FALSE)) ||
        ((opt->blend_opt.pixel_alpha_en != TD_TRUE) && (opt->blend_opt.pixel_alpha_en != TD_FALSE)) ||
        ((opt->blend_opt.src1_alpha_premulti != TD_TRUE) && (opt->blend_opt.src1_alpha_premulti != TD_FALSE)) ||
        ((opt->blend_opt.src2_alpha_premulti != TD_TRUE) && (opt->blend_opt.src2_alpha_premulti != TD_FALSE));
    if (is_invalid == TD_TRUE) {
        tde_error("bool should be TRUE or FALSE!!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (opt->colorkey_mode != DRV_TDE_COLOR_KEY_MODE_NONE) {
        tde_error("It doesn't support colorkey in single source pattern mode!\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    is_invalid = (opt->alpha_blending_cmd != DRV_TDE_ALPHA_BLENDING_NONE) &&
        (opt->alpha_blending_cmd != DRV_TDE_ALPHA_BLENDING_BLEND) &&
        (opt->alpha_blending_cmd != DRV_TDE_ALPHA_BLENDING_ROP) &&
        (opt->alpha_blending_cmd != DRV_TDE_ALPHA_BLENDING_COLORIZE);
    if (is_invalid == TD_TRUE) {
        tde_error("alpha_blending_cmd error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_check_single_src_pattern_opt
 * Description:   check if single source mode fill operate is valid
 * Input:         src_fmt foreground pixel format
                  dst_fmt  target pixel format
                  opt    operate attribute pointer
 * Return:        0  valid parameter
                  -1 invalid parameter
 */
static td_s32 tde_osi_check_single_src_pattern_opt(drv_tde_color_fmt src_fmt, drv_tde_color_fmt dst_fmt,
                                                   const drv_tde_pattern_fill_opt *opt)
{
    td_s32 ret;
    tde_colorfmt_transform color_trans_type = tde_osi_get_fmt_trans_type(src_fmt, dst_fmt);
    if (color_trans_type == TDE_COLORFMT_TRANSFORM_BUTT) {
        tde_error("Unknown color transport type!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    ret = tde_osi_check_src_pattern_fill_opt(opt);
    if (ret != TD_SUCCESS) {
        if (ret == TD_FAILURE) {
            return TD_SUCCESS;
        }
        return ret;
    }
#if (TDE_CAPABILITY & ROP)
    if ((td_u32)opt->alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_ROP) {
        if ((!tde_osi_is_single_src_to_rop(opt->rop_alpha)) ||
            (!tde_osi_is_single_src_to_rop(opt->rop_color))) {
            tde_error("Only support single s2 rop!\n");
            return DRV_ERR_TDE_INVALID_PARA;
        }
    } else {
        if (opt->alpha_blending_cmd != DRV_TDE_ALPHA_BLENDING_NONE) {
            tde_error("single src not suppot alpha blending!\n");
            return DRV_ERR_TDE_INVALID_PARA;
        }
    }
#else
    if (opt->alpha_blending_cmd != DRV_TDE_ALPHA_BLENDING_NONE) {
        tde_error("It doesn't ROP/Blend/Colorize!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
#endif
    if (color_trans_type == TDE_COLORFMT_TRANSFORM_CLUT_CLUT) {
        if ((opt->alpha_blending_cmd != DRV_TDE_ALPHA_BLENDING_NONE)) {
            tde_error("It doesn't ROP/Blend/Colorize!\n");
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }
        if (src_fmt != dst_fmt) {
            tde_error("If src fmt and dst fmt are clut, they shoulod be the same fmt!\n");
            return DRV_ERR_TDE_INVALID_PARA;
        }
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_check_bool_opt(drv_tde_color_fmt back_ground_fmt, const drv_tde_pattern_fill_opt *opt)
{
    td_bool real = (((opt->clut_reload != TD_TRUE) && (opt->clut_reload != TD_FALSE)) ||
        ((opt->blend_opt.global_alpha_en != TD_TRUE) && (opt->blend_opt.global_alpha_en != TD_FALSE)) ||
        ((opt->blend_opt.pixel_alpha_en != TD_TRUE) && (opt->blend_opt.pixel_alpha_en != TD_FALSE)) ||
        ((opt->blend_opt.src1_alpha_premulti != TD_TRUE) && (opt->blend_opt.src1_alpha_premulti != TD_FALSE)) ||
        ((opt->blend_opt.src2_alpha_premulti != TD_TRUE) && (opt->blend_opt.src2_alpha_premulti != TD_FALSE)));

    if (opt->alpha_blending_cmd >= DRV_TDE_ALPHA_BLENDING_MAX) {
        tde_error("alpha_blending_cmd error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((back_ground_fmt == DRV_TDE_COLOR_FMT_YCBCR422) || (back_ground_fmt == DRV_TDE_COLOR_FMT_PKGVYUY)) {
        tde_error("This operation doesn't support PKG!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (real) {
        tde_error("bool should be TRUE or FALSE!!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((opt->colorkey_mode >= DRV_TDE_COLOR_KEY_MODE_MAX) ||
        (opt->colorkey_mode < DRV_TDE_COLOR_KEY_MODE_NONE)) {
        tde_error("color_key_mode error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (opt->out_alpha_from >= DRV_TDE_OUT_ALPHA_FROM_MAX) {
        tde_error("out_alpha_from error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_check_double_src_alpha_category(tde_colorfmt_category bg_category,
                                                      tde_colorfmt_category fg_category,
                                                      drv_tde_color_fmt back_ground_fmt,
                                                      drv_tde_color_fmt fore_ground_fmt,
                                                      drv_tde_color_fmt dst_fmt)
{
    td_bool dst_have_alpha = tde_osi_whether_contain_alpha(dst_fmt);
    if ((bg_category == TDE_COLORFMT_CATEGORY_ARGB) &&
        (fg_category == TDE_COLORFMT_CATEGORY_AN) && (!dst_have_alpha)) {
        tde_error("Target must have alpha component!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if ((fg_category == TDE_COLORFMT_CATEGORY_AN) && ((back_ground_fmt == DRV_TDE_COLOR_FMT_YCBCR888) ||
        (fore_ground_fmt == DRV_TDE_COLOR_FMT_AYCBCR8888))) {
        tde_error("Target must have alpha component!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}

typedef struct {
    tde_colorfmt_category bg_category;
    tde_colorfmt_category fg_category;
    tde_colorfmt_category dst_category;
    drv_tde_color_fmt back_ground_fmt;
    drv_tde_color_fmt fore_ground_fmt;
    drv_tde_color_fmt dst_fmt;
    const drv_tde_pattern_fill_opt *opt;
} tde_osi_double_src_fmt_ctx;

static td_s32 tde_osi_check_double_src_clut_category(const tde_osi_double_src_fmt_ctx *fmt_ctx)
{
    if ((fmt_ctx->bg_category == TDE_COLORFMT_CATEGORY_CLUT) &&
        (fmt_ctx->opt->alpha_blending_cmd != DRV_TDE_ALPHA_BLENDING_NONE)) {
        tde_error("It doesn't support alpha blending!\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    if ((fmt_ctx->bg_category == TDE_COLORFMT_CATEGORY_CLUT) &&
        ((fmt_ctx->back_ground_fmt != fmt_ctx->fore_ground_fmt) ||
        (fmt_ctx->back_ground_fmt != fmt_ctx->dst_fmt))) {
        tde_error("If background, foreground , dst are clut, they should be the same fmt!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if ((fmt_ctx->bg_category == TDE_COLORFMT_CATEGORY_CLUT) &&
        ((fmt_ctx->fg_category != TDE_COLORFMT_CATEGORY_CLUT) ||
        (fmt_ctx->dst_category != TDE_COLORFMT_CATEGORY_CLUT))) {
        tde_error("Unsupported operation!\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_check_double_src_dst_category(tde_colorfmt_category bg_category,
                                                    tde_colorfmt_category fg_category,
                                                    tde_colorfmt_category dst_category,
                                                    const drv_tde_pattern_fill_opt *opt)
{
    if ((bg_category == TDE_COLORFMT_CATEGORY_AN) && (fg_category == TDE_COLORFMT_CATEGORY_AN) &&
        (dst_category == TDE_COLORFMT_CATEGORY_AN) && (opt->alpha_blending_cmd != DRV_TDE_ALPHA_BLENDING_NONE)) {
        tde_error("It doesn't support ROP or mirror!\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    if ((dst_category == TDE_COLORFMT_CATEGORY_CLUT) && ((fg_category != TDE_COLORFMT_CATEGORY_CLUT) ||
        (bg_category != TDE_COLORFMT_CATEGORY_CLUT))) {
        tde_error("Unsupported operation!\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_check_double_src_pattern_opt
 * Description:   check if doul source mode fill operate parameter is valid
 * Input:         back_ground_fmt background pixel format
                  fore_ground_fmt foreground pixel format
                  dst_fmt  target pixel format
                  opt     operate attribute pointer
 * Return:        0  valid parameter
                  -1 invalid parameter
 */
static td_s32 tde_osi_check_double_src_pattern_opt(drv_tde_color_fmt back_ground_fmt, drv_tde_color_fmt fore_ground_fmt,
                                                   drv_tde_color_fmt dst_fmt, const drv_tde_pattern_fill_opt *opt)
{
    tde_colorfmt_category bg_category;
    tde_colorfmt_category fg_category;
    tde_colorfmt_category dst_category;
    td_bool is_invalid;
    tde_osi_double_src_fmt_ctx fmt_ctx;

    td_s32 ret = tde_osi_check_bool_opt(back_ground_fmt, opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    bg_category = tde_osi_get_fmt_category(back_ground_fmt);
    fg_category = tde_osi_get_fmt_category(fore_ground_fmt);
    dst_category = tde_osi_get_fmt_category(dst_fmt);
    is_invalid = (bg_category >= TDE_COLORFMT_CATEGORY_BYTE) || (fg_category >= TDE_COLORFMT_CATEGORY_BYTE) ||
        (dst_category >= TDE_COLORFMT_CATEGORY_BYTE);
    if (is_invalid == TD_TRUE) {
        tde_error("unknown format!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    ret = tde_osi_check_double_src_alpha_category(bg_category, fg_category, back_ground_fmt, fore_ground_fmt, dst_fmt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    fmt_ctx.bg_category = bg_category;
    fmt_ctx.fg_category = fg_category;
    fmt_ctx.dst_category = dst_category;
    fmt_ctx.back_ground_fmt = back_ground_fmt;
    fmt_ctx.fore_ground_fmt = fore_ground_fmt;
    fmt_ctx.dst_fmt = dst_fmt;
    fmt_ctx.opt = opt;
    ret = tde_osi_check_double_src_clut_category(&fmt_ctx);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return tde_osi_check_double_src_dst_category(bg_category, fg_category, dst_category, opt);
}

static td_s32 tde_osi_check_pattern_dst_color_format(drv_tde_color_fmt color_format)
{
    if ((color_format == DRV_TDE_COLOR_FMT_YCBCR422) || (color_format == DRV_TDE_COLOR_FMT_PKGVYUY)) {
        tde_error("It doesn't support YCbCr422 or PKGVYUY in pattern fill!\n");
        return TD_FAILURE;
    }

    if (color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) {
        tde_error("This operation doesn't support Semi-plannar!\n");
        return TD_FAILURE;
    }

    if ((color_format >= DRV_TDE_COLOR_FMT_A1) && (color_format != DRV_TDE_COLOR_FMT_YCBCR888) &&
        (color_format != DRV_TDE_COLOR_FMT_AYCBCR8888)) {
        tde_error("pattern fill:dst color format doesn't support format:%d!\n", color_format);
        return TD_FAILURE;
    }

    if (color_format >= DRV_TDE_COLOR_FMT_CLUT1 && color_format <= DRV_TDE_COLOR_FMT_ACLUT88 &&
        color_format != DRV_TDE_COLOR_FMT_CLUT8) {
        tde_error("This operation doesn't support dst color format clut,except clut8!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_check_pattern_src_color_format(drv_tde_color_fmt color_format)
{
    if (color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) {
        tde_error("pattern fill:bg or fg color format doesn't support Semi-plannar!\n");
        return TD_FAILURE;
    }

    if ((color_format >= DRV_TDE_COLOR_FMT_A1) && (color_format != DRV_TDE_COLOR_FMT_YCBCR888) &&
        (color_format != DRV_TDE_COLOR_FMT_AYCBCR8888)) {
        tde_error("pattern fill:bg or fg color format doesn't support format:%d!\n", color_format);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_get_pattern_opt_category
 * Description:   analyze pattern fill operate type
 * Input:         dst_surface: target bitmap info
 *                dst_rect: target bitmap operate zone
 *                opt: operate option
 * Return:        TDE operate type
 */
static tde_pattern_operation_category tde_osi_get_pattern_opt_category(const drv_tde_double_src *double_src,
                                                                       const drv_tde_pattern_fill_opt *opt)
{
    if ((double_src == TD_NULL) || (double_src->dst_surface == TD_NULL) || (double_src->dst_rect == TD_NULL)) {
        tde_error("dst_surface/dst_rect should not be null!\n");
        return TDE_PATTERN_OPERATION_BUTT;
    }

    if (tde_osi_check_pattern_dst_color_format(double_src->dst_surface->color_format) != TD_SUCCESS) {
        return TDE_PATTERN_OPERATION_BUTT;
    }

    if (tde_osi_check_surface(double_src->dst_surface, double_src->dst_rect) != TD_SUCCESS) {
        return TDE_PATTERN_OPERATION_BUTT;
    }

    if ((double_src->bg_surface == TD_NULL) && (double_src->fg_surface == TD_NULL)) {
        tde_error("No src:both bg_surface and fg_surface are NULL!\n");
        return TDE_PATTERN_OPERATION_BUTT;
    } else if ((double_src->bg_surface != TD_NULL) && (double_src->fg_surface != TD_NULL)) {
        return tde_osi_double_src_pattern_operation(double_src, opt);
    } else {
        return tde_osi_check_single_src_pattern_operation(double_src, opt);
    }
}

static tde_pattern_operation_category tde_osi_check_single_src_pattern_operation(const drv_tde_double_src *double_src,
                                                                                 const drv_tde_pattern_fill_opt *opt)
{
    drv_tde_surface *tmp_src2 = TD_NULL;
    drv_tde_rect *tmp_src2_rect = TD_NULL;
    ot_unused(opt);

    if (double_src->bg_surface != TD_NULL) {
        if (double_src->bg_rect == TD_NULL) {
            tde_error("Background rect shouldn't be NULL!\n");
            return TDE_PATTERN_OPERATION_BUTT;
        }
        if (tde_osi_check_pattern_src_color_format(double_src->bg_surface->color_format) != TD_SUCCESS) {
            return TDE_PATTERN_OPERATION_BUTT;
        }
        tmp_src2 = double_src->bg_surface;
        tmp_src2_rect = double_src->bg_rect;
    } else if (double_src->fg_surface != TD_NULL) {
        if (double_src->fg_rect == TD_NULL) {
            tde_error("Foreground rect shouldn't be NULL!\n");
            return TDE_PATTERN_OPERATION_BUTT;
        }
        if (tde_osi_check_pattern_src_color_format(double_src->fg_surface->color_format) != TD_SUCCESS) {
            return TDE_PATTERN_OPERATION_BUTT;
        }
        tmp_src2 = double_src->fg_surface;
        tmp_src2_rect = double_src->fg_rect;
    }

    if ((tmp_src2 != TD_NULL) && (tmp_src2_rect != TD_NULL)) {
        if (tmp_src2_rect->width > TDE_MAX_PATTERNWIDTH) {
            tde_error("Max pattern width is 256!\n");
            return TDE_PATTERN_OPERATION_BUTT;
        }
        if (tde_osi_check_surface(tmp_src2, tmp_src2_rect) != TD_SUCCESS) {
            return TDE_PATTERN_OPERATION_BUTT;
        }
    }

    return TDE_PATTERN_OPERATION_SINGLE_SRC;
}

static tde_pattern_operation_category tde_osi_double_src_pattern_operation(const drv_tde_double_src *double_src,
                                                                           const drv_tde_pattern_fill_opt *opt)
{
    if ((double_src->bg_rect == TD_NULL) || (double_src->fg_rect == TD_NULL) || (opt == TD_NULL)) {
        tde_error("bg_rect/fg_rect/opt should not be null in two src pattern fill!\n");
        return TDE_PATTERN_OPERATION_BUTT;
    }

    if (tde_osi_check_pattern_src_color_format(double_src->bg_surface->color_format) != TD_SUCCESS) {
        return TDE_PATTERN_OPERATION_BUTT;
    }

    if (tde_osi_check_pattern_src_color_format(double_src->fg_surface->color_format) != TD_SUCCESS) {
        return TDE_PATTERN_OPERATION_BUTT;
    }

    if ((tde_osi_check_surface(double_src->bg_surface, double_src->bg_rect) != TD_SUCCESS) ||
        (tde_osi_check_surface(double_src->fg_surface, double_src->fg_rect) != TD_SUCCESS)) {


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART16_H_ */
