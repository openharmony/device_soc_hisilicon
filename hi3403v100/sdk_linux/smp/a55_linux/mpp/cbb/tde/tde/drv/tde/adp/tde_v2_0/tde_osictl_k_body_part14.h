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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART14_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART14_H_

    tde_osi_convert_surface(double_src->bg_surface, double_src->bg_rect, &src_scan_info, &drv_surface);

    tde_hal_node_set_src1(hw_node, &drv_surface);

    tde_osi_convert_surface(double_src->fg_surface, double_src->fg_rect, &src_scan_info, &drv_surface);

    tde_hal_node_set_src2(hw_node, &drv_surface);

    tde_osi_convert_surface(double_src->dst_surface, double_src->dst_rect, &dst_scan_info, &drv_surface);
    ret = tde_osi_double_opt(double_src, opt, hw_node, &drv_surface);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }

    ret = tde_osi_double_node(handle, double_src, opt, hw_node);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_set_color_key(const drv_tde_double_src *double_src,
                                    tde_hw_node *hw_node, drv_tde_color_key color_key_value,
                                    drv_tde_color_key_mode color_key_mode,
                                    tde_clut_usage clut_usage)
{
    tde_color_key_cmd color_key;
    tde_colorfmt_category fmt_category;
    td_bool unknown_fmt_category;
    td_bool color_key_foreground_before_clut_mode = (clut_usage != TDE_CLUT_COLOREXPENDING) &&
                                                     (clut_usage != TDE_CLUT_CLUT_BYPASS);
    td_bool set_color_key = (color_key_mode != DRV_TDE_COLOR_KEY_MODE_NONE);

    color_key.colorkey_value = color_key_value;

    if (!set_color_key) {
        return TD_SUCCESS;
    }

    switch (color_key_mode) {
        case DRV_TDE_COLOR_KEY_MODE_BACKGROUND:
            color_key.colorkey_mode = TDE_DRV_COLORKEY_BACKGROUND;

            fmt_category = tde_osi_get_fmt_category(double_src->bg_surface->color_format);

            break;

        case DRV_TDE_COLOR_KEY_MODE_FOREGROUND:

            color_key.colorkey_mode = (color_key_foreground_before_clut_mode) ?
                TDE_DRV_COLORKEY_FOREGROUND_AFTER_CLUT : TDE_DRV_COLORKEY_FOREGROUND_BEFORE_CLUT;

            fmt_category = tde_osi_get_fmt_category(double_src->fg_surface->color_format);

            break;

        default:
            tde_error("invalid ColorKeyMode!\n");

            return DRV_ERR_TDE_INVALID_PARA;
    }

    unknown_fmt_category = fmt_category >= TDE_COLORFMT_CATEGORY_BUTT;

    if (unknown_fmt_category) {
        tde_error("Unknown fmt category!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (tde_hal_node_set_colorkey(hw_node, fmt_category, &color_key) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    return TD_SUCCESS;
}

static td_s32 tde_osi_mb_val(const drv_tde_mb_src *mb_src, drv_tde_mb_opt *mb_opt, td_u32 *cb_cr_height,
    td_u32 *byte_per_pixel, td_u32 *cb_cr_byte_per_pixel)
{
    td_s32 ret;
    td_s32 bpp;

    ret = tde_osi_check_mb_blit_para(mb_src, mb_opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if ((mb_src->dst_rect->height == mb_src->src_rect->height) &&
        (mb_src->dst_rect->width == mb_src->src_rect->width)) {
        mb_opt->resize_en = DRV_TDE_MB_RESIZE_NONE;
    }

    if (mb_opt->resize_en == DRV_TDE_MB_RESIZE_NONE) {
        tde_unify_rect(mb_src->src_rect, mb_src->dst_rect);
    }

    if (tde_osi_check_resize_para(mb_src->src_rect->width, mb_src->src_rect->height,
        mb_src->dst_rect->width, mb_src->dst_rect->height) != TD_SUCCESS) {
        tde_error("The Scale is too large!\n");
        return DRV_ERR_TDE_MINIFICATION;
    }

    *cb_cr_height = TDE_MAX_SLICE_RECT_HEIGHT;
    if ((mb_src->mb_surface->mb_color_format == DRV_TDE_MB_COLOR_FMT_MP1_YCBCR420MBP) ||
        (mb_src->mb_surface->mb_color_format == DRV_TDE_MB_COLOR_FMT_MP2_YCBCR420MBP) ||
        (mb_src->mb_surface->mb_color_format == DRV_TDE_MB_COLOR_FMT_MP2_YCBCR420MBI) ||
        (mb_src->mb_surface->mb_color_format == DRV_TDE_MB_COLOR_FMT_JPG_YCBCR420MBP) ||
        (mb_src->mb_surface->mb_color_format == DRV_TDE_MB_COLOR_FMT_JPG_YCBCR422MBVP)) {
        *cb_cr_height = TDE_MAX_SLICE_RECT_HEIGHT / 2; /* 2 alg data */
    }

    *cb_cr_byte_per_pixel = ((mb_src->mb_surface->mb_color_format == DRV_TDE_MB_COLOR_FMT_JPG_YCBCR444MBP) ||
        (mb_src->mb_surface->mb_color_format == DRV_TDE_MB_COLOR_FMT_JPG_YCBCR422MBVP)) ? 2 : 1; /* 2 1 alg data */

    bpp = tde_osi_get_bpp_by_fmt(mb_src->dst_surface->color_format);
    *byte_per_pixel = ((td_u32)bpp >> 3); /* 3 alg data */
    return TD_SUCCESS;
}

typedef struct {
    td_u32 byte_per_pixel;
    td_u32 cbcr_pixel;
    td_u32 cb_cr_height;
    td_phys_addr_t yphy_addr;
    td_phys_addr_t u64phy;
    td_phys_addr_t cb_crphy_addr;
} tde_mb_blit_addr_info;

static td_void tde_osi_set_mb_addr(drv_tde_mb_src *mb_src, const tde_mb_blit_addr_info *addr_info,
    td_u32 m, td_u32 n)
{
    mb_src->mb_surface->y_addr = addr_info->yphy_addr + (td_u64)m * TDE_MAX_SLICE_RECT_WIDTH +
        (td_u64)n * (td_u64)(mb_src->mb_surface->y_stride) * TDE_MAX_SLICE_RECT_HEIGHT;
    mb_src->dst_surface->phys_addr = addr_info->u64phy + (td_u64)m * (td_u64)addr_info->byte_per_pixel *
        TDE_MAX_SLICE_RECT_WIDTH + (td_u64)n * (td_u64)(mb_src->dst_surface->stride) * TDE_MAX_SLICE_RECT_HEIGHT;
    mb_src->mb_surface->cbcr_phys_addr = addr_info->cb_crphy_addr + (td_u64)m *
        (td_u64)addr_info->cbcr_pixel * TDE_MAX_SLICE_RECT_WIDTH +
        (td_u64)n * (td_u64)(mb_src->mb_surface->cbcr_stride) * (td_u64)(addr_info->cb_cr_height);
}

/*
 * Function:      tde_osi_mb_blit
 * Description:   Mb blit
 * Return:        >0: return task id of current operate; <0: fail
 */
td_s32 tde_osi_mb_blit(td_s32 handle, drv_tde_mb_src *mb_src, drv_tde_mb_opt *mb_opt)
{
    td_u32 height;
    td_u32 width;
    td_u32 i;
    td_u32 j;
    td_u32 m;
    td_u32 n;
    tde_mb_blit_addr_info addr_info = {0};
    td_s32 ret = tde_osi_mb_val(mb_src, mb_opt, &addr_info.cb_cr_height, &addr_info.byte_per_pixel,
        &addr_info.cbcr_pixel);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    drv_tde_mb_src_val(mb_src, height, width);
    drv_tde_mb_src_addr(mb_src, addr_info.yphy_addr, addr_info.cb_crphy_addr, addr_info.u64phy);
    ret = tde_osi_get_double_cycle_data(mb_src->src_rect, &i, &j);
    if (ret != TD_SUCCESS) {
        tde_error("Tde osi Get double cycle data failed, ret = 0x%x\n", ret);
    }
    for (n = 0; n < j; n++) {
        for (m = 0; m < i; m++) {
            if ((i - 1) == m) {
                mb_src->src_rect->width = width - m * TDE_MAX_SLICE_RECT_WIDTH;
            } else {
                mb_src->src_rect->width = TDE_MAX_SLICE_RECT_WIDTH;
            }

            if ((j - 1) == n) {
                mb_src->src_rect->height = height - n * TDE_MAX_SLICE_RECT_HEIGHT;
            } else {
                mb_src->src_rect->height = TDE_MAX_SLICE_RECT_HEIGHT;
            }
            if (!((i == 1) && (j == 1))) {
                mb_src->dst_rect->width = mb_src->src_rect->width;
                mb_src->dst_rect->height = mb_src->src_rect->height;
            }
            tde_osi_set_mb_addr(mb_src, &addr_info, m, n);
            ret = tde_osi_set_mb_para(handle, mb_src, mb_opt);
            if (ret != TD_SUCCESS) {
                return ret;
            }
        }
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_check_surface_ex(const drv_tde_surface *surface, drv_tde_rect *rect)
{
    td_bool invalid_operation_area;
    td_s32 ret = tde_osi_pre_check_surface_ex(surface, rect);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    invalid_operation_area = ((rect->width > TDE_MAX_RECT_WIDTH_EX) ||
                              (rect->height > TDE_MAX_RECT_HEIGHT_EX));

    if (invalid_operation_area) {
        tde_error("invalid operation SurfaceEX area!width=%d,height=%d\n", rect->width, rect->height);
        return DRV_ERR_TDE_INVALID_PARA;
    }

    return TD_SUCCESS;
}

static td_s32 tde_osi_check_mb_src_opt(const drv_tde_mb_src *mb_src, const drv_tde_mb_opt *mb_opt)
{
    if (mb_src->dst_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) {
        tde_error("This operation doesn't support Semi-plannar!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (mb_src->mb_surface->mb_color_format >= DRV_TDE_MB_COLOR_FMT_MAX) {
        tde_error("mb color format error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (((mb_opt->is_deflicker != TD_TRUE) && (mb_opt->is_deflicker != TD_FALSE)) ||
        ((mb_opt->is_set_out_alpha != TD_TRUE) && (mb_opt->is_set_out_alpha != TD_FALSE))) {
        tde_error("bool should be TRUE or FALSE!!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (mb_opt->resize_en >= DRV_TDE_MB_RESIZE_MAX) {
        tde_error("enMBResize error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_check_mb_blit_para(const drv_tde_mb_src *mb_src, const drv_tde_mb_opt *mb_opt)
{
    td_s32 ret;
    td_bool null_ptr = ((mb_src->mb_surface == TD_NULL) || ((mb_src->src_rect == TD_NULL)) ||
                        (mb_src->dst_surface == TD_NULL) || (mb_src->dst_rect == TD_NULL) || (mb_opt == TD_NULL));

    if (null_ptr) {
        tde_error("Contains NULL ptr!\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    ret = tde_osi_check_mb_src_opt(mb_src, mb_opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if ((tde_osi_check_surface_ex(mb_src->dst_surface, mb_src->dst_rect) != TD_SUCCESS) ||
        (tde_osi_check_mb_surface_ex(mb_src->mb_surface, mb_src->src_rect) != TD_SUCCESS)) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    return TD_SUCCESS;
}

static td_s32 tde_osi_get_double_cycle_data(const drv_tde_rect *mb_rect, td_u32 *i, td_u32 *j)
{
    if ((mb_rect->height > TDE_MAX_RECT_HEIGHT) && (mb_rect->width > TDE_MAX_RECT_WIDTH)) {
        *i = 2; /* 2 alg data */
        *j = 2; /* 2 alg data */
    } else if ((mb_rect->height > TDE_MAX_RECT_HEIGHT) && (mb_rect->width <= TDE_MAX_RECT_WIDTH)) {
        *i = 1;
        *j = 2; /* 2 alg data */
    } else if ((mb_rect->height <= TDE_MAX_RECT_HEIGHT) && (mb_rect->width > TDE_MAX_RECT_WIDTH)) {
        *i = 2; /* 2 alg data */
        *j = 1;
    } else {
        *i = 1;
        *j = 1;
    }
    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_bitmap_mask_check_para
 * Description:   check for trinal source operate parameter
 * Return:        =0: success; <0: fail
 */
#if (TDE_CAPABILITY & MASKROP || TDE_CAPABILITY & MASKBLEND)
static td_s32 tde_osi_bitmap_mask_check_triple_src(const drv_tde_triple_src *triple_src)
{
    if (tde_osi_check_surface(triple_src->bg_surface, triple_src->bg_rect) != TD_SUCCESS) {
        tde_error("pstBackGroundRect does not correct!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (tde_osi_check_surface(triple_src->fg_surface, triple_src->fg_rect) != TD_SUCCESS) {
        tde_error("pstForeGroundRect does not correct!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (tde_osi_check_surface(triple_src->mask_surface, triple_src->mask_rect) != TD_SUCCESS) {
        tde_error("pstMaskRect does not correct!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (tde_osi_check_surface(triple_src->dst_surface, triple_src->dst_rect) != TD_SUCCESS) {
        tde_error("dst_rect does not correct!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_bitmap_mask_check_para(const drv_tde_triple_src *triple_src)
{
    td_bool is_unsupported_scale;
    td_s32 ret;
    is_unsupported_scale = ((triple_src == TD_NULL) || (triple_src->bg_surface == TD_NULL) ||
        (triple_src->bg_rect == TD_NULL) || (triple_src->fg_surface == TD_NULL) ||
        (triple_src->fg_rect == TD_NULL) || (triple_src->mask_surface == TD_NULL) ||
        (triple_src->mask_rect == TD_NULL) || (triple_src->dst_surface == TD_NULL) ||
        (triple_src->dst_rect == TD_NULL));

    if (is_unsupported_scale) {
        tde_error("Contains NULL ptr!\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    if ((triple_src->fg_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) ||
        (triple_src->bg_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) ||
        (triple_src->mask_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) ||
        (triple_src->dst_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP)) {
        tde_error("This operation doesn't support Semi-plannar!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((triple_src->bg_surface->color_format == DRV_TDE_COLOR_FMT_YCBCR422) ||
        (triple_src->bg_surface->color_format == DRV_TDE_COLOR_FMT_PKGVYUY)) {
        tde_error("This operation doesn't support PKG!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    ret = tde_osi_bitmap_mask_check_triple_src(triple_src);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    is_unsupported_scale = ((triple_src->bg_rect->width != triple_src->fg_rect->width) ||
                          (triple_src->fg_rect->width != triple_src->mask_rect->width) ||
                          (triple_src->mask_rect->width != triple_src->dst_rect->width) ||
                          (triple_src->bg_rect->height != triple_src->fg_rect->height) ||
                          (triple_src->fg_rect->height != triple_src->mask_rect->height) ||
                          (triple_src->mask_rect->height != triple_src->dst_rect->height));

    if (is_unsupported_scale) {
        tde_error("Don't support scale!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    return TD_SUCCESS;
}
#endif

/*
 * Function:      tde_osi_bitmap_mask_rop
 * Description:   Ropmask foreground and mask at firs, and then ropmask background and middle bitmap
 *                output result to target bitmap
 */


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART14_H_ */
