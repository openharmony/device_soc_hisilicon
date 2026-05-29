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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART09_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART09_H_

#if (TDE_CAPABILITY & ROP)
    tde_rop_opt rop_opt = {0};
    rop_opt.alpha_blending_cmd = opt->alpha_blending_cmd;
    rop_opt.rop_code_color = opt->rop_color;
    rop_opt.rop_code_alpha = opt->rop_alpha;
    rop_opt.single_sr2_rop = TD_FALSE;
    ret = tde_osi_set_rop(hw_node, &rop_opt, &alu_mode);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#endif

    ret = tde_osi_set_blend(hw_node, opt->alpha_blending_cmd, opt->blend_opt, &alu_mode, TD_TRUE);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#if (TDE_CAPABILITY & COLORIZE)
    ret = tde_osi_set_colorize(hw_node, opt->alpha_blending_cmd, opt->color_resize);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#endif
    tde_hal_node_set_global_alpha(hw_node, opt->global_alpha, opt->blend_opt.global_alpha_en);

    double_src.bg_surface = TD_NULL;
    double_src.bg_rect = TD_NULL;
    double_src.fg_surface = single_src->src_surface;
    double_src.fg_rect = single_surface_dou->src_rect;
    double_src.dst_surface = single_src->dst_surface;
    double_src.dst_rect = single_surface_dou->dst_rect;
    if (tde_osi_set_clip_para(&double_src, opt, hw_node) != TD_SUCCESS) {
        return DRV_ERR_TDE_CLIP_AREA;
    }

    if (tde_osi_color_convert(fill_color, single_src->src_surface, &drv_color_fill.fill_data) != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (tde_hal_node_set_base_operate(hw_node, TDE_NORM_FILL_2OPT, alu_mode, &drv_color_fill) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_2_source_node(td_s32 handle, const drv_tde_single_src *single_src, const drv_tde_opt *opt,
                                    tde_hw_node *hw_node, const drv_tde_single_src *single_surface_dou)
{
    drv_tde_double_src double_src;
    tde_clut_usage clut_usage = TDE_CLUT_USAGE_BUTT;
    td_bool set_file_node;
    td_s32 ret = tde_osi_set_clut_opt(single_src->src_surface, single_src->dst_surface, &clut_usage,
        opt->clut_reload, hw_node);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = tde_osi_set_foreground_color_key(hw_node, single_src->src_surface, opt, clut_usage);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    set_file_node = ((opt->resize) || (opt->deflicker_mode != DRV_TDE_DEFLICKER_LEVEL_MODE_NONE) ||
                     (single_src->src_surface->color_format >= DRV_TDE_COLOR_FMT_YCBCR422));
    if (set_file_node) {
        double_src.bg_surface = single_src->src_surface;
        double_src.bg_rect = single_surface_dou->src_rect;
        double_src.fg_surface = TD_NULL;
        double_src.fg_rect = TD_NULL;
        double_src.dst_surface = single_src->dst_surface;
        double_src.dst_rect = single_surface_dou->dst_rect;
        ret = tde_osi_set_filter_node(handle, hw_node, &double_src,
                                      opt->deflicker_mode, (drv_tde_deflicker_mode)opt->filter_mode);
        if (ret != TD_SUCCESS) {
            return ret;
        }
#if (TDE_CAPABILITY & SLICE)
        tde_hal_free_node_buf(hw_node);
#endif
        return TD_SUCCESS;
    }
    ret = tde_osi_set_node_finish(handle, hw_node, 0, TDE_NODE_SUBM_ALONE);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_2_source_fill(td_s32 handle, const drv_tde_single_src *single_src,
    const drv_tde_fill_color *fill_color, const drv_tde_opt *opt)
{
    tde_hw_node *hw_node = TD_NULL;
    tde_surface_msg drv_surface = { 0 };
    tde_scandirection_mode src_scan_info = { 0 };
    tde_scandirection_mode dst_scan_info = { 0 };
    drv_tde_rect src_rect = { 0 };
    drv_tde_rect dst_rect = { 0 };
    td_s32 ret;
    drv_tde_single_src single_surface_dou;

    single_surface_dou.dst_rect = &dst_rect;
    single_surface_dou.dst_surface = TD_NULL;
    single_surface_dou.src_rect = &src_rect;
    single_surface_dou.src_surface = TD_NULL;
    ret = tde_osi_2_source_check(single_src, fill_color, opt, &single_surface_dou);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if (tde_hal_node_init_nd(&hw_node) != TD_SUCCESS) {
        return DRV_ERR_TDE_NO_MEM;
    }

    ret = tde_osi_2_source_para(single_src, fill_color, opt, hw_node, &single_surface_dou);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }
    tde_osi_set_ext_alpha(TD_NULL, single_src->src_surface, hw_node);
    single_surface_dou.src_surface = single_src->src_surface;
    single_surface_dou.dst_surface = single_src->dst_surface;
    if (tde_osi_get_scan_info_ex(&single_surface_dou, opt, &src_scan_info, &dst_scan_info) != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return DRV_ERR_TDE_INVALID_PARA;
    }

    tde_osi_convert_surface(single_src->src_surface, single_surface_dou.src_rect, &src_scan_info, &drv_surface);

    tde_hal_node_set_src1(hw_node, &drv_surface);

    tde_osi_convert_surface(single_src->dst_surface, single_surface_dou.dst_rect, &dst_scan_info, &drv_surface);

    tde_hal_node_set_tqt(hw_node, &drv_surface, opt->out_alpha_from);
    ret = tde_osi_2_source_node(handle, single_src, opt, hw_node, &single_surface_dou);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 tde_osi_set_foreground_color_key(tde_hw_node *hw_node, const drv_tde_surface *src_surface,
                                               const drv_tde_opt *opt, tde_clut_usage clut_usage)
{
    tde_color_key_cmd color_key;
    tde_colorfmt_category fmt_category;
    td_bool foreground_colorkey_after_clut;
    color_key.colorkey_value = opt->colorkey_value;
    foreground_colorkey_after_clut = ((clut_usage != TDE_CLUT_COLOREXPENDING) && (clut_usage != TDE_CLUT_CLUT_BYPASS));

    if (opt->colorkey_mode == DRV_TDE_COLOR_KEY_MODE_BACKGROUND) {
        tde_error("Unsupported solidraw colorkey in background mode!\n");

        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((opt->colorkey_mode != DRV_TDE_COLOR_KEY_MODE_FOREGROUND) &&
        (opt->colorkey_mode != DRV_TDE_COLOR_KEY_MODE_BACKGROUND)) {
        return TD_SUCCESS;
    }

    color_key.colorkey_mode = (foreground_colorkey_after_clut) ? TDE_DRV_COLORKEY_FOREGROUND_AFTER_CLUT :
                                TDE_DRV_COLORKEY_FOREGROUND_BEFORE_CLUT;

    fmt_category = tde_osi_get_fmt_category(src_surface->color_format);
    if (fmt_category >= TDE_COLORFMT_CATEGORY_BUTT) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    if (tde_hal_node_set_colorkey(hw_node, fmt_category, &color_key) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_check_surface
 * Description:   adjust right operate zone, according by the size of bitmap and operate zone from user upload
 * Input:         surface: bitmap info
 *                rect: bitmap operate zone
 * Return:        success/fail
 */
static td_s32 tde_osi_check_surface(const drv_tde_surface *surface, drv_tde_rect *rect)
{
    td_bool invalid_operation_area;
    td_s32 ret = tde_osi_pre_check_surface_ex(surface, rect);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    invalid_operation_area = ((TDE_MAX_RECT_WIDTH < rect->width) || (TDE_MAX_RECT_HEIGHT < rect->height));

    if (invalid_operation_area) {
        tde_error("operation area is over maximum!Width:%d,Height:%d\n", rect->width, rect->height);
        return DRV_ERR_TDE_INVALID_PARA;
    }

    return TD_SUCCESS;
}

static td_s32 tde_osi_pre_check_surface(const drv_tde_surface *surface, const drv_tde_rect *rect)
{
    td_bool real = ((surface->stride > TDE_MAX_SURFACE_PITCH) || (surface->stride == 0) || (rect->height == 0) ||
        (rect->width == 0) || (rect->pos_x < 0) || ((td_u32)rect->pos_x >= surface->width) ||
        (rect->pos_y < 0) || ((td_u32)rect->pos_y >= surface->height) || (surface->phys_addr == 0));

    if (surface->color_format >= DRV_TDE_COLOR_FMT_MAX) {
        tde_error("color format error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (real) {
        tde_error("surface_w:%d, surface_h:%d, stride:%d, sphy:%lx, x:%d, y:%d, w:%d, h:%d\n", surface->width,
            surface->height, surface->stride, (td_ulong)surface->phys_addr, rect->pos_x,
            rect->pos_y, rect->width, rect->height);
        tde_error("invalid surface phyaddr or invalid surface size or operation area!\n");
        return -1;
    }
    real = (((surface->support_alpha_ex_1555 != TD_TRUE) && (surface->support_alpha_ex_1555 != TD_FALSE)) ||
        ((surface->alpha_max_is_255 != TD_TRUE) && (surface->alpha_max_is_255 != TD_FALSE)) ||
        ((surface->is_ycbcr_clut != TD_TRUE) && (surface->is_ycbcr_clut != TD_FALSE)));
    if (real) {
        tde_error("bool should be TRUE or FALSE!!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    return TD_SUCCESS;
}

static td_void tde_osi_get_rect_wh(const drv_tde_surface *surface, drv_tde_rect *rect)
{
    if (rect->pos_x + rect->width > surface->width) {
        rect->width = surface->width - rect->pos_x;
    }

    if (rect->pos_y + rect->height > surface->height) {
        rect->height = surface->height - rect->pos_y;
    }
}

static td_s32 tde_osi_pre_check_surface_ex(const drv_tde_surface *surface, drv_tde_rect *rect)
{
    td_bool unknown_color_fmt;
    td_bool raster_fmt;

    td_s32 ret = tde_osi_pre_check_surface(surface, rect);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    unknown_color_fmt = (surface->color_format >= DRV_TDE_COLOR_FMT_MAX);
    if (unknown_color_fmt) {
        tde_error("Unknown color format!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    unknown_color_fmt = ((((td_u32)(rect->pos_x) & 0x1) || (rect->width & 0x1)) &&
        ((surface->color_format == DRV_TDE_COLOR_FMT_YCBCR422) ||
        (surface->color_format == DRV_TDE_COLOR_FMT_PKGVYUY)));
    if (unknown_color_fmt) {
        tde_error("x, width of YCbCr422R couldn't be odd!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    raster_fmt = (surface->color_format <= DRV_TDE_COLOR_FMT_HALFWORD);
    if (raster_fmt) {
        ret = tde_osi_raster_fmt_check_align(surface);
        if (ret != TD_SUCCESS) {
            return ret;
        }
    } else {
        if (((surface->cbcr_stride > TDE_MAX_SURFACE_PITCH) || (surface->cbcr_stride == 0)) &&
            (surface->color_format != DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP)) {
            tde_error("Invalid CbCr stride!\n");
            return DRV_ERR_TDE_INVALID_PARA;
        }
    }
    tde_osi_get_rect_wh(surface, rect);
    return TD_SUCCESS;
}

static td_s32 tde_osi_raster_fmt_check_align(const drv_tde_surface *surface)
{
    td_u32 byte_per_pixel;
    td_s32 bpp = tde_osi_get_bpp_by_fmt(surface->color_format);
    if ((bpp >= 8) && (bpp != 24)) { /* 8 24 Bpp */
        byte_per_pixel = (bpp / 8); /* 8 bits */

        if (osal_div_u64_rem(surface->phys_addr, byte_per_pixel)) {
            tde_error("Bitmap address is not aligned!\n");
            return DRV_ERR_TDE_NOT_ALIGNED;
        }

        if (surface->stride % byte_per_pixel) {
            tde_error("stride is not aligned!\n");
            return DRV_ERR_TDE_NOT_ALIGNED;
        }
    } else if (bpp == 24) { /* 24 Bpp */
        if (osal_div_u64_rem(surface->phys_addr, 4)) { /* 4 align_num */
            tde_error("Bitmap address is not aligned!\n");
            return DRV_ERR_TDE_NOT_ALIGNED;
        }
        if (surface->stride % 4) { /* 4 align_num */
            tde_error("stride is not aligned!\n");
            return DRV_ERR_TDE_NOT_ALIGNED;
        }
    }

    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_check_mb_surface_ex
 * Description:   adjust right operate zone,
                  according by the size of bitmap is more than 4095*4095 and less than 8190*8190
 * Input:         mb_surface: bitmap info
 *                rect: bitmap operate zone
 * Return:        success/fail
 */
static td_s32 tde_osi_check_mb_surface_ex(const drv_tde_mb_surface *mb_surface, drv_tde_rect *rect)
{
    td_bool real = ((rect->pos_x < 0) || (rect->width > TDE_MAX_RECT_WIDTH_EX) ||
        (rect->height > TDE_MAX_RECT_HEIGHT_EX) || (rect->height == 0) || (rect->width == 0) ||
        (mb_surface->y_stride > TDE_MAX_SURFACE_PITCH) || (mb_surface->cbcr_stride > TDE_MAX_SURFACE_PITCH) ||
        ((td_u32)rect->pos_x >= mb_surface->y_width) || (rect->pos_y < 0) ||
        ((td_u32)rect->pos_y >= mb_surface->y_height) || (mb_surface->y_addr == 0));

    if (mb_surface->mb_color_format >= DRV_TDE_MB_COLOR_FMT_MAX) {
        tde_error("mb color format error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((mb_surface->mb_color_format > DRV_TDE_MB_COLOR_FMT_JPG_YCBCR400MBP) &&
        (mb_surface->mb_color_format < DRV_TDE_MB_COLOR_FMT_MAX)) {
        if (mb_surface->cbcr_stride == 0) {
            tde_error(" mb cbcr_stride is null !\n");
            return DRV_ERR_TDE_INVALID_PARA;
        }
        if (mb_surface->cbcr_phys_addr == 0) {
            tde_error("mb cbcr_phy_addr is 0!\n");
            return DRV_ERR_TDE_INVALID_PARA;
        }
    }
    if (real) {
        tde_error("syw:%u, syh:%u, systride:%u, syphy:%lx, scbcrstride:%u, scbcrphy:%lx, x:%d, y:%d, w:%u, h:%u\n",
            mb_surface->y_width, mb_surface->y_height, mb_surface->y_stride, (td_ulong)mb_surface->y_addr,
            mb_surface->cbcr_stride, (td_ulong)mb_surface->cbcr_phys_addr, rect->pos_x, rect->pos_y, rect->width,
            rect->height);
        tde_error("invalid mbsurface phyaddr or invalid surface size or operation area!\n");
        return -1;
    }
    if ((mb_surface->y_stride % 4) || (mb_surface->cbcr_stride % 4)) { /* 4 align_num */
        tde_error("stride is not aligned!\n");
        return DRV_ERR_TDE_NOT_ALIGNED;
    }

    if ((((td_u32)(rect->pos_x) & 0x1) || (rect->width & 0x1)) &&
        ((mb_surface->mb_color_format > DRV_TDE_MB_COLOR_FMT_JPG_YCBCR400MBP) &&
        (mb_surface->mb_color_format < DRV_TDE_MB_COLOR_FMT_JPG_YCBCR444MBP))) {
        tde_error("x, width of YCbCr422 420 couldn't be odd!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (rect->pos_x + rect->width > mb_surface->y_width) {
        rect->width = mb_surface->y_width - rect->pos_x;
    }

    if (rect->pos_y + rect->height > mb_surface->y_height) {
        rect->height = mb_surface->y_height - rect->pos_y;
    }

    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_get_opt_category
 * Description:   analyze TDE operate type
 * Return:        TDE operate type
 */
static tde_operation_category tde_osi_get_opt_category(drv_tde_double_src *double_src, const drv_tde_opt *opt)
{
    if ((double_src->dst_surface == TD_NULL) || (double_src->dst_rect == TD_NULL)) {
        tde_error("dst is NULL!\n");
        return TDE_OPERATION_BUTT;
    }

    if (double_src->dst_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) {
        tde_error("This operation doesn't support Semi-plannar!\n");
        return TDE_OPERATION_BUTT;
    }

    if (tde_osi_check_surface(double_src->dst_surface, double_src->dst_rect) != TD_SUCCESS) {
        return TDE_OPERATION_BUTT;
    }
    if ((double_src->bg_surface == TD_NULL) && (double_src->fg_surface == TD_NULL)) {
        tde_error("null pointer in single src or double src operation!");
        return TDE_OPERATION_BUTT;
    } else if ((double_src->bg_surface != TD_NULL) && (double_src->fg_surface != TD_NULL)) {
        return tde_osi_double_src_operation(double_src, opt);
    } else {
        return tde_osi_single_src_operation(double_src, opt);
    }
}

static tde_operation_category tde_osi_single_src_operation(const drv_tde_double_src *double_src, const drv_tde_opt *opt)
{
    drv_tde_surface *tmp_src2 = TD_NULL;
    drv_tde_rect *tmp_src2_rect = TD_NULL;

    if ((double_src->bg_surface != TD_NULL) && (double_src->fg_surface == TD_NULL)) {
        tmp_src2 = double_src->bg_surface;
        tmp_src2_rect = double_src->bg_rect;
    } else {
        tmp_src2 = double_src->fg_surface;
        tmp_src2_rect = double_src->fg_rect;
    }

    if (tmp_src2_rect == TD_NULL) {
        return TDE_OPERATION_BUTT;
    }

    if (tde_osi_check_surface(tmp_src2, tmp_src2_rect) != TD_SUCCESS) {
        return TDE_OPERATION_BUTT;
    }

    if ((opt == TD_NULL) || (!opt->resize)) {
        tde_unify_rect(tmp_src2_rect, double_src->dst_rect);
    }

    if ((opt == TD_NULL) && (tmp_src2->color_format == double_src->dst_surface->color_format)) {
        return TDE_OPERATION_SINGLE_SRC1;


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART09_H_ */
