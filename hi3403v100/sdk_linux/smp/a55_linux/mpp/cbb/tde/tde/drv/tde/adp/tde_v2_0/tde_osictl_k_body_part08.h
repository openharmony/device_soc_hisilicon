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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART08_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART08_H_

        tde_error("mirror error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (opt->alpha_blending_cmd >= DRV_TDE_ALPHA_BLENDING_MAX ||
        opt->alpha_blending_cmd < DRV_TDE_ALPHA_BLENDING_NONE) {
        tde_error("alpha_blending_cmd error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (opt->alpha_blending_cmd != DRV_TDE_ALPHA_BLENDING_NONE) {
        tde_error("invalid alu command!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (opt->resize) {
        tde_error("Not support resize in single source fill operation!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_1_source_src1(drv_tde_surface *dst_surface, const drv_tde_opt *opt, tde_hw_node *hw_node,
                                    drv_tde_rect *dst_rect)
{
    td_s32 ret;
    drv_tde_double_src double_src;
    ret = tde_osi_check_fill_opt(opt);
    if (ret != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if ((opt->out_alpha_from >= DRV_TDE_OUT_ALPHA_FROM_MAX) ||
        (opt->out_alpha_from == DRV_TDE_OUT_ALPHA_FROM_FOREGROUND)) {
        tde_error("out_alpha_from error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    tde_hal_node_set_global_alpha(hw_node, opt->global_alpha, opt->blend_opt.global_alpha_en);

    double_src.bg_surface = TD_NULL;
    double_src.bg_rect = TD_NULL;
    double_src.fg_surface = dst_surface;
    double_src.fg_rect = dst_rect;
    double_src.dst_surface = dst_surface;
    double_src.dst_rect = dst_rect;
    if (tde_osi_set_clip_para(&double_src, opt, hw_node) != TD_SUCCESS) {
        return DRV_ERR_TDE_CLIP_AREA;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_1_source_check(drv_tde_surface *dst_surface, const drv_tde_rect *dst_rect,
                                     drv_tde_fill_color *fill_color, drv_tde_rect *rect)
{
    td_bool null_ptr = ((dst_surface == TD_NULL) || (dst_rect == TD_NULL) || (fill_color == TD_NULL));
    td_s32 bpp;

    if (null_ptr) {
        tde_error("NULL pointer!\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (memcpy_s(rect, sizeof(drv_tde_rect), dst_rect, sizeof(drv_tde_rect)) != EOK) {
        tde_error("secure function failure\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((dst_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) ||
        (fill_color->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP)) {
        tde_error("This operation doesn't support Semi-plannar!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

#ifdef TDE_BACKGROUND_COLORFOMATSUPPORT_YCBCR422
    if (dst_surface->color_format >= DRV_TDE_COLOR_FMT_YCBCR422) {
        tde_error("Background  doesn't support the colorfmt!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
#endif

    if (tde_osi_check_surface(dst_surface, rect) != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    bpp = tde_osi_get_bpp_by_fmt(dst_surface->color_format);
    if (bpp < 0) {
        tde_error("Unknown color format!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (bpp < 8) { /* 8 is bpp */
        /* when writing, 8 subbyte format align ask start point byte align */
        if ((rect->pos_x * bpp % 8) || (rect->width * bpp % 8)) {
            tde_error("The input start or write width does not meet the single byte alignment!\n");
            return DRV_ERR_TDE_INVALID_PARA;
        }
    }
    tde_osi_adj_para4_ycb_cr422_r(dst_surface, rect, fill_color);
    return TD_SUCCESS;
}

static td_void tde_osi_1_source_drv(const drv_tde_surface *dst_surface, tde_base_opt_mode base_mode,
                                    const drv_tde_rect *dst_rect, tde_hw_node *hw_node,
                                    tde_drv_outalpha_from out_alpha_from)
{
    tde_scandirection_mode scan_info = { 0 };
    tde_surface_msg drv_surface = { 0 };
    scan_info.hor_scan = TDE_SCAN_LEFT_RIGHT;
    scan_info.ver_scan = TDE_SCAN_UP_DOWN;
    tde_osi_convert_surface(dst_surface, dst_rect, &scan_info, &drv_surface);

    if (base_mode == TDE_NORM_FILL_1OPT) {
        tde_hal_node_set_src1(hw_node, &drv_surface);
    }

    tde_hal_node_set_tqt(hw_node, &drv_surface, out_alpha_from);

    tde_osi_set_ext_alpha(dst_surface, TD_NULL, hw_node);
    return;
}

/*
 * Function:      tde_osi_1_source_fill
 * Description:   single source fill operate,source1 is fill color,target bitmap is dst_surface,
                  support source1 and fill color do ROP or alpha blending to target bitmap,
                  unsupported mirror,colorkey
 *                src bitmap is not support MB color format
 * Input:         handle: task handle
 *                dst_surface: foreground bitmap info struct
 *                fill_color:  target bitmap info struct
 *                opt: operate parameter setting struct
 * Return:        TD_SUCCESS/TD_FAILURE
 */
static td_s32 tde_osi_1_source_fill(td_s32 handle, drv_tde_surface *dst_surface, drv_tde_rect *dst_rect,
                                    drv_tde_fill_color *fill_color, const drv_tde_opt *opt)
{
    tde_base_opt_mode base_mode = { 0 };
    tde_hw_node *hw_node = TD_NULL;
    tde_drv_outalpha_from out_alpha_from = DRV_TDE_OUT_ALPHA_FROM_NORM;
    drv_tde_rect rect = { 0 };
    tde_color_fill drv_color_fill = { 0 };
    td_s32 ret = tde_osi_1_source_check(dst_surface, dst_rect, fill_color, &rect);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if (tde_hal_node_init_nd(&hw_node) != TD_SUCCESS) {
        return DRV_ERR_TDE_NO_MEM;
    }

    if (opt == TD_NULL) {
        base_mode = (fill_color->color_format == dst_surface->color_format) ? TDE_QUIKE_FILL : TDE_NORM_FILL_1OPT;
        out_alpha_from = DRV_TDE_OUT_ALPHA_FROM_NORM;
    } else {
        ret = tde_osi_1_source_src1(dst_surface, opt, hw_node, &rect);
        if (ret != TD_SUCCESS) {
            tde_hal_free_node_buf(hw_node);
            return ret;
        }
        out_alpha_from = opt->out_alpha_from;
        base_mode = (fill_color->color_format == dst_surface->color_format) ? TDE_QUIKE_FILL : TDE_NORM_FILL_1OPT;
    }
    if (base_mode == TDE_NORM_FILL_1OPT) {
        if (tde_osi_color_convert(fill_color, dst_surface, &drv_color_fill.fill_data) != TD_SUCCESS) {
            tde_hal_free_node_buf(hw_node);
            return DRV_ERR_TDE_INVALID_PARA;
        }
        drv_color_fill.drv_color_fmt = TDE_DRV_COLOR_FMT_ARGB8888;
    } else {
        drv_color_fill.fill_data = fill_color->color_value;
        drv_color_fill.drv_color_fmt = g_tde_common_drv_color_fmt[fill_color->color_format];
    }
    if (tde_hal_node_set_base_operate(hw_node, base_mode, TDE_ALU_NONE, &drv_color_fill) != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    tde_osi_1_source_drv(dst_surface, base_mode, &rect, hw_node, out_alpha_from);
    ret = tde_osi_set_node_finish(handle, hw_node, 0, TDE_NODE_SUBM_ALONE);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }
    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_des_dma_draw_rect
 * Description:   single source fill operate,source1 is fill color,target bitmap is dst_surface,support
 *                source1 and fill color do ROP or alpha blending to target bitmap, unsupported mirror,colorkey
 *                 src bitmap is not support MB color format
 * Input:         handle: task handle
 *                pSrc: background bitmap info struct
 *                dst_surface: foreground bitmap info struct
 *                pstFillColor:  target bitmap info struct
 *                pstOpt: operate parameter setting struct
 * Output:        none
 * Return:        TD_SUCCESS/TD_FAILURE
 * Others:        none
 */
static td_s32 tde_osi_des_dma_draw_rect(td_s32 handle, const drv_tde_surface *dst_surface, const drv_tde_rect *dst_rect,
                                        const drv_tde_corner_rect_info *corner_info, const drv_tde_opt *opt)
{
    tde_hw_node *hw_node = TD_NULL;
    tde_surface_msg surface = { 0 };
    tde_corner_rect_info corner_hal_info = { 0 };
    tde_scandirection_mode st_scan_info = { 0 };
    tde_drv_outalpha_from out_alpha_from = DRV_TDE_OUT_ALPHA_FROM_NORM;
    drv_tde_rect tmp_dst_rect = { 0 };
    td_s32 ret = TD_FAILURE;
    ot_unused(opt);
    if ((dst_surface == TD_NULL) || (dst_rect == TD_NULL) || (corner_info == TD_NULL)) {
        tde_error("NULL pointer!\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (memcpy_s(&tmp_dst_rect, sizeof(tmp_dst_rect), dst_rect, sizeof(drv_tde_rect)) != EOK) {
        tde_error("secure function failure\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (tde_osi_check_format_support_draw(dst_surface->color_format) != TD_TRUE) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (tde_osi_check_surface(dst_surface, &tmp_dst_rect) != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (tde_check_subbyte_startx(tmp_dst_rect.pos_x, tmp_dst_rect.width, dst_surface->color_format) != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (tde_hal_node_init_nd(&hw_node) != TD_SUCCESS) {
        return DRV_ERR_TDE_NO_MEM;
    }
    corner_hal_info.drv_color_fmt = g_tde_common_drv_color_fmt[dst_surface->color_format];
    corner_hal_info.width = corner_info->width;
    corner_hal_info.height = corner_info->height;
    corner_hal_info.inner_color = corner_info->inner_color;
    corner_hal_info.outer_color = corner_info->outer_color;

    st_scan_info.hor_scan = TDE_SCAN_LEFT_RIGHT;
    st_scan_info.ver_scan = TDE_SCAN_UP_DOWN;

    tde_osi_convert_surface(dst_surface, &tmp_dst_rect, &st_scan_info, &surface);
    tde_hal_node_set_tqt(hw_node, &surface, out_alpha_from);
    tde_hal_node_set_corner_rect(hw_node, &corner_hal_info);

    if ((ret = tde_osi_set_node_finish(handle, hw_node, 0, TDE_NODE_SUBM_ALONE)) != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }
    return TD_SUCCESS;
}

#ifdef CONFIG_TDE_DRD_LINE_SUPPORT
static td_s32 tde_osi_des_dma_draw_line(td_s32 handle, const drv_tde_surface *dst_surface, const drv_tde_line *line,
                                        td_u32 num)
{
    tde_hw_node *hw_node = TD_NULL;
    tde_surface_msg surface = { 0 };
    drv_tde_rect dst_rect;
    tde_line_info line_hal_info[TDE_MAX_LINE_NUM] = { 0 };
    tde_scandirection_mode st_scan_info = { 0 };
    td_s32 ret;
    td_u32 i;
    if ((dst_surface == TD_NULL) || (line == TD_NULL)) {
        tde_error("NULL pointer!\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    if (tde_hal_node_init_nd(&hw_node) != TD_SUCCESS) {
        return DRV_ERR_TDE_NO_MEM;
    }
    for (i = 0; i < num; i++) {
        line_hal_info[i].start_x = line[i].start_x;
        line_hal_info[i].start_y = line[i].start_y;
        line_hal_info[i].end_x = line[i].end_x;
        line_hal_info[i].end_y = line[i].end_y;
        line_hal_info[i].thick = line[i].thick;
        line_hal_info[i].color = line[i].color;
        line_hal_info[i].enable = 0x1;
    }

    st_scan_info.hor_scan = TDE_SCAN_LEFT_RIGHT;
    st_scan_info.ver_scan = TDE_SCAN_UP_DOWN;

    /* draw line rect default full surface */
    dst_rect.pos_x = 0;
    dst_rect.pos_y = 0;
    dst_rect.width = dst_surface->width;
    dst_rect.height = dst_surface->height;

    tde_osi_convert_surface(dst_surface, &dst_rect, &st_scan_info, &surface);
    tde_hal_node_set_tqt(hw_node, &surface, DRV_TDE_OUT_ALPHA_FROM_NORM);
    tde_hal_node_draw_line(hw_node, line_hal_info, TDE_MAX_LINE_NUM);

    if ((ret = tde_osi_set_node_finish(handle, hw_node, 0, TDE_NODE_SUBM_ALONE)) != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }
    return TD_SUCCESS;
}
#else
static td_s32 tde_osi_des_dma_draw_line(td_s32 handle, const drv_tde_surface *dst_surface, const drv_tde_line *line,
                                        td_u32 num)
{
    ot_unused(handle);
    ot_unused(dst_surface);
    ot_unused(line);
    ot_unused(num);
    tde_error("DRD draw line unsupported!\n");
    return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
}
#endif

static td_s32 tde_osi_2_check_opt(const drv_tde_opt *opt)
{
    td_bool real = (((opt->blend_opt.global_alpha_en != TD_TRUE) && (opt->blend_opt.global_alpha_en != TD_FALSE)) ||
        ((opt->blend_opt.pixel_alpha_en != TD_TRUE) && (opt->blend_opt.pixel_alpha_en != TD_FALSE)) ||
        ((opt->blend_opt.src1_alpha_premulti != TD_TRUE) && (opt->blend_opt.src1_alpha_premulti != TD_FALSE)) ||
        ((opt->blend_opt.src2_alpha_premulti != TD_TRUE) && (opt->blend_opt.src2_alpha_premulti != TD_FALSE)) ||
        ((opt->clut_reload != TD_TRUE) && (opt->clut_reload != TD_FALSE)) ||
        ((opt->is_compress != TD_TRUE) && (opt->is_compress != TD_FALSE)) ||
        ((opt->is_decompress != TD_TRUE) && (opt->is_decompress != TD_FALSE)));

    if (opt->out_alpha_from >= DRV_TDE_OUT_ALPHA_FROM_MAX) {
        tde_error("out_alpha_from error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (opt->alpha_blending_cmd >= DRV_TDE_ALPHA_BLENDING_MAX) {
        tde_error("alpha_blending_cmd error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((opt->colorkey_mode >= DRV_TDE_COLOR_KEY_MODE_MAX) ||
        (opt->colorkey_mode < DRV_TDE_COLOR_KEY_MODE_NONE)) {
        tde_error("color_key_mode error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (real) {
        tde_error("bool should be TRUE or FALSE!!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((opt->deflicker_mode >= DRV_TDE_DEFLICKER_LEVEL_MODE_MAX) ||
        (opt->deflicker_mode < DRV_TDE_DEFLICKER_LEVEL_MODE_NONE)) {
        tde_error("deflicker_mode error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((opt->filter_mode >= DRV_TDE_FILTER_MODE_MAX) || (opt->filter_mode < DRV_TDE_FILTER_MODE_COLOR)) {
        tde_error("filter_mode error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if ((opt->mirror >= DRV_TDE_MIRROR_MAX) || (opt->mirror < DRV_TDE_MIRROR_NONE)) {
        tde_error("mirror error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_2_source_check(const drv_tde_single_src *single_src, const drv_tde_fill_color *fill_color,
                                     const drv_tde_opt *opt, const drv_tde_single_src *single_surface_dou)
{
    td_s32 ret;
    td_bool null_ptr = ((single_src->dst_surface == TD_NULL) || (single_src->dst_rect == TD_NULL) ||
                        (fill_color == TD_NULL) || ((opt == TD_NULL)) ||
                        (single_src->src_surface == TD_NULL) || (single_src->src_rect == TD_NULL));

    if (null_ptr) {
        return DRV_ERR_TDE_NULL_PTR;
    }

    ret = memcpy_s(single_surface_dou->src_rect, sizeof(drv_tde_rect), single_src->src_rect, sizeof(drv_tde_rect));
    tde_unequal_eok_return(ret);
    ret = memcpy_s(single_surface_dou->dst_rect, sizeof(drv_tde_rect), single_src->dst_rect, sizeof(drv_tde_rect));
    tde_unequal_eok_return(ret);
    if (fill_color->color_format >= DRV_TDE_COLOR_FMT_MAX) {
        tde_error("color format error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    ret = tde_osi_check_dst_fmt(single_src->dst_surface->color_format);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if ((single_src->src_surface->color_format == DRV_TDE_COLOR_FMT_YCBCR422) ||
        (single_src->src_surface->color_format == DRV_TDE_COLOR_FMT_PKGVYUY)) {
        tde_error("This operation doesn't support PKG!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    ret = tde_osi_2_check_opt(opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if (tde_osi_check_surface(single_src->dst_surface, single_surface_dou->dst_rect) != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (tde_osi_check_surface(single_src->src_surface, single_surface_dou->src_rect) != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (opt->resize) {
        if (tde_osi_check_resize_para(single_src->src_rect->width, single_src->src_rect->height,
            single_src->dst_rect->width, single_src->dst_rect->height) != TD_SUCCESS) {
            return DRV_ERR_TDE_MINIFICATION;
        }
    } else {
        tde_unify_rect(single_surface_dou->src_rect, single_surface_dou->dst_rect);
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_2_source_para(const drv_tde_single_src *single_src, const drv_tde_fill_color *fill_color,
    const drv_tde_opt *opt, tde_hw_node *hw_node, const drv_tde_single_src *single_surface_dou)
{
    td_s32 ret;
    drv_tde_double_src double_src;
    tde_alu_mode alu_mode = TDE_ALU_NONE;
    tde_color_fill drv_color_fill = {
        0
    };

#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART08_H_ */
