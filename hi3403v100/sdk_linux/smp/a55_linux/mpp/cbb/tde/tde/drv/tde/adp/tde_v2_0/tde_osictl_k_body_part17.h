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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART17_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART17_H_

        return TDE_PATTERN_OPERATION_BUTT;
    }

    if ((double_src->bg_rect->height != double_src->dst_rect->height) ||
        (double_src->bg_rect->width != double_src->dst_rect->width)) {
        tde_error("Size of background rect and dst rect should be the same in two src pattern fill.\n");
        tde_error("background x:%d, y:%d, w:%d, h:%d; dst x:%d, y:%d, w:%d, h:%d\n", double_src->bg_rect->pos_x,
            double_src->bg_rect->pos_y, double_src->bg_rect->width, double_src->bg_rect->height,
            double_src->dst_rect->pos_x, double_src->dst_rect->pos_y, double_src->dst_rect->width,
            double_src->dst_rect->height);
        return TDE_PATTERN_OPERATION_BUTT;
    }

    if (double_src->fg_rect->width > TDE_MAX_PATTERNWIDTH) {
        tde_error("Max pattern width is %d!\n", TDE_MAX_PATTERNWIDTH);
        return TDE_PATTERN_OPERATION_BUTT;
    }

    return TDE_PATTERN_OPERATION_DOUBLE_SRC;
}

static td_bool tde_osi_check_overlap(const drv_tde_surface *sur1, drv_tde_rect *rect1, const drv_tde_surface *sur2,
                                     const drv_tde_rect *rect2)
{
    td_phys_addr_t rect1_start_phy;
    td_phys_addr_t rect2_start_phy;
    td_phys_addr_t rect1_end_phy;
    td_phys_addr_t rect2_end_phy;
    td_u32 bpp1;
    td_u32 bpp2;

    if (rect2->height < rect1->height) {
        rect1->height = rect2->height;
    }

    if (rect2->width < rect1->width) {
        rect1->width = rect2->width;
    }

    bpp1 = tde_osi_get_bpp_by_fmt(sur1->color_format) / 8; /* 8 alg data */
    bpp2 = tde_osi_get_bpp_by_fmt(sur2->color_format) / 8; /* 8 alg data */

    rect1_start_phy = sur1->phys_addr + (rect1->pos_y * sur1->stride)  +
        rect1->pos_x * bpp1;
    rect1_end_phy = rect1_start_phy + (rect1->height - 1) * sur1->stride  +
        (rect1->width - 1) * bpp1;

    rect2_start_phy = sur2->phys_addr + (rect2->pos_y * sur2->stride)  +
        rect2->pos_x * bpp2;
    rect2_end_phy = rect2_start_phy + (rect2->height - 1) * sur2->stride  +
        (rect2->width - 1) * bpp2;
    tde_info("rect1_start_phy:%lx, rect1_end_phy:%lx, rect2_start_phy:%lx, rect2_end_phy:%lx\n", (td_ulong)
        rect1_start_phy, (td_ulong)rect1_end_phy, (td_ulong)rect2_start_phy, (td_ulong)rect2_end_phy);

    return TD_FALSE;
}

static td_s32 tde_osi_pattern_fill_opt(tde_hw_node *hw_node, const drv_tde_pattern_fill_opt *opt,
                                       tde_alu_mode *alu_mode, td_bool is_check_single_src2rop,
                                       td_bool is_check_blend)
{
    td_s32 ret;

#if (TDE_CAPABILITY & ROP)
    tde_rop_opt rop_opt = {0};
    rop_opt.alpha_blending_cmd = opt->alpha_blending_cmd;
    rop_opt.rop_code_color = opt->rop_color;
    rop_opt.rop_code_alpha = opt->rop_alpha;
    rop_opt.single_sr2_rop = is_check_single_src2rop;
    ret = tde_osi_set_rop(hw_node, &rop_opt, alu_mode);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#endif
    ret = tde_osi_set_blend(hw_node, opt->alpha_blending_cmd, opt->blend_opt, alu_mode,
                            is_check_blend);
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
    return TD_SUCCESS;
}

static td_s32 tde_osi_single_pattern_fill_opt(tde_hw_node *hw_node, const drv_tde_single_src *single_src,
                                              const drv_tde_pattern_fill_opt *opt)
{
    td_bool is_check_single_src2rop = TD_TRUE;
    td_bool is_check_blend = TD_FALSE;
    td_s32 ret;
    tde_alu_mode alu_mode = TDE_ALU_NONE;
    tde_base_opt_mode base_mode = TDE_SINGLE_SRC_PATTERN_FILL_OPT;
    drv_tde_double_src double_src = {0};

    if (opt != TD_NULL) {
        if (opt->out_alpha_from >= DRV_TDE_OUT_ALPHA_FROM_MAX) {
            tde_error("out_alpha_from error!\n");
            return DRV_ERR_TDE_INVALID_PARA;
        }

        if (opt->out_alpha_from == DRV_TDE_OUT_ALPHA_FROM_BACKGROUND) {
            tde_error("Single src pattern fill doesn't support out alpha form background!\n");
            return DRV_ERR_TDE_INVALID_PARA;
        }

        ret = tde_osi_pattern_fill_opt(hw_node, opt, &alu_mode, is_check_single_src2rop, is_check_blend);
        if (ret != TD_SUCCESS) {
            return ret;
        }

        double_src.bg_surface = TD_NULL;
        double_src.bg_rect = TD_NULL;
        double_src.fg_surface = single_src->src_surface;
        double_src.fg_rect = single_src->src_rect;
        double_src.dst_surface = single_src->dst_surface;
        double_src.dst_rect = single_src->dst_rect;
        if (tde_osi_set_pattern_clip_para(&double_src, opt, hw_node) != TD_SUCCESS) {
            return DRV_ERR_TDE_CLIP_AREA;
        }
    }

    if (tde_hal_node_set_base_operate(hw_node, base_mode, alu_mode, TD_NULL) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_single_pattern_fill_set_up(tde_hw_node *hw_node, const drv_tde_single_src *single_src,
                                                 const drv_tde_pattern_fill_opt *opt)
{
    td_s32 ret;
    tde_scandirection_mode src_scan_info = {0};
    tde_scandirection_mode dst_scan_info = {0};
    tde_surface_msg src_drv_surface = {0};
    tde_surface_msg dst_drv_surface = {0};
    td_u16 code;
    tde_conv_mode_cmd conv = { 0 };
    tde_clut_usage clut_usage = TDE_CLUT_USAGE_BUTT;

    /* set ext alpha */
    tde_osi_set_ext_alpha(TD_NULL, single_src->src_surface, hw_node);
    /* set src2 and tqt */
    src_scan_info.hor_scan = TDE_SCAN_LEFT_RIGHT;
    src_scan_info.ver_scan = TDE_SCAN_UP_DOWN;
    dst_scan_info.hor_scan = TDE_SCAN_LEFT_RIGHT;
    dst_scan_info.ver_scan = TDE_SCAN_UP_DOWN;

    tde_osi_convert_surface(single_src->src_surface, single_src->src_rect, &src_scan_info, &src_drv_surface);

    tde_hal_node_set_src2(hw_node, &src_drv_surface);

    tde_osi_convert_surface(single_src->dst_surface, single_src->dst_rect, &dst_scan_info, &dst_drv_surface);

    (opt != TD_NULL) ? tde_hal_node_set_tqt(hw_node, &dst_drv_surface, opt->out_alpha_from) :
        tde_hal_node_set_tqt(hw_node, &dst_drv_surface, DRV_TDE_OUT_ALPHA_FROM_FOREGROUND);
    /* set color */
    code = tde_osi_single_src2_get_opt_code(single_src->src_surface->color_format,
        single_src->dst_surface->color_format);

    tde_osi_get_conv_by_code(code, &conv);

    if (tde_hal_node_set_color_convert(hw_node, &conv) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    /* set clut opt */
    ret = (opt != TD_NULL) ?
           tde_osi_set_clut_opt(single_src->src_surface, single_src->dst_surface, &clut_usage, opt->clut_reload,
                                hw_node) :
           tde_osi_set_clut_opt(single_src->src_surface, single_src->dst_surface, &clut_usage, TD_TRUE, hw_node);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /* set csc */
    if (opt != TD_NULL) {
        ret = tde_set_node_csc(hw_node, opt->csc_opt);
        if (ret != TD_SUCCESS) {
            return ret;
        }
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_single_src_pattern_fill(td_s32 handle, const drv_tde_single_src *single_src,
                                              const drv_tde_pattern_fill_opt *opt)
{
    tde_hw_node *hw_node = TD_NULL;
    td_s32 ret;

    ret = tde_osi_check_single_src_pattern_fill_para(single_src, opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (tde_hal_node_init_nd(&hw_node) != TD_SUCCESS) {
        return DRV_ERR_TDE_NO_MEM;
    }

    /* deal with opt */
    ret = tde_osi_single_pattern_fill_opt(hw_node, single_src, opt);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }

    /* set:
     * set ext alpha
     * set src2
     * set tqt
     * set color format
     * set clut opt
     * set csc
     */
    ret = tde_osi_single_pattern_fill_set_up(hw_node, single_src, opt);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }

    /* set node finish */
    ret = tde_osi_set_node_finish(handle, hw_node, 0, TDE_NODE_SUBM_ALONE);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_check_single_src_pattern_fill_para(const drv_tde_single_src *single_src,
                                                         const drv_tde_pattern_fill_opt *opt)
{
    td_bool is_null_ptr = ((single_src == TD_NULL) || (single_src->src_surface == TD_NULL) ||
                           ((single_src->src_rect == TD_NULL)) || (single_src->dst_surface == TD_NULL) ||
                           (single_src->dst_rect == TD_NULL));

    if (is_null_ptr) {
        return DRV_ERR_TDE_NULL_PTR;
    }

    if (tde_osi_check_single_src_pattern_opt(single_src->src_surface->color_format,
        single_src->dst_surface->color_format, opt) != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (tde_osi_check_overlap(single_src->src_surface, single_src->src_rect,
        single_src->dst_surface, single_src->dst_rect)) {
        tde_error("Surface overlap!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_double_src_pattern_fill
 * Description:   dual resource pattern fill
 * Input:         opt: operate option
 * Return:        success/fail
 */
static td_s32 tde_osi_double_pattern_fill_opt(tde_hw_node *hw_node, const drv_tde_double_src *double_src,
                                              const drv_tde_pattern_fill_opt *opt)
{
    td_bool is_check_single_src2rop = TD_FALSE;
    td_bool is_check_blend = TD_TRUE;
    tde_alu_mode alu_mode = TDE_ALU_NONE;
    td_s32 ret;
    tde_base_opt_mode base_mode = TDE_DOUBLE_SRC_PATTERN_FILL_OPT;

    ret = tde_osi_pattern_fill_opt(hw_node, opt, &alu_mode, is_check_single_src2rop, is_check_blend);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (tde_osi_set_pattern_clip_para(double_src, opt, hw_node) != TD_SUCCESS) {
        return DRV_ERR_TDE_CLIP_AREA;
    }

    if (tde_hal_node_set_base_operate(hw_node, base_mode, alu_mode, TD_NULL) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    return TD_SUCCESS;
}

static td_void tde_osi_get_scan_info(tde_scandirection_mode *src_scan_info, tde_scandirection_mode *dst_scan_info)
{
    src_scan_info->hor_scan = TDE_SCAN_LEFT_RIGHT;
    src_scan_info->ver_scan = TDE_SCAN_UP_DOWN;
    dst_scan_info->hor_scan = TDE_SCAN_LEFT_RIGHT;
    dst_scan_info->ver_scan = TDE_SCAN_UP_DOWN;
}

static td_s32 tde_osi_double_pattern_fill_set_up(tde_hw_node *hw_node, const drv_tde_double_src *double_src,
                                                 const drv_tde_pattern_fill_opt *opt)
{
    tde_scandirection_mode src_scan_info = {0};
    tde_scandirection_mode dst_scan_info = {0};
    tde_surface_msg src_drv_surface = {0};
    tde_surface_msg dst_drv_surface = {0};
    td_u16 code;
    tde_conv_mode_cmd conv = { 0 };
    tde_clut_usage clut_usage = TDE_CLUT_USAGE_BUTT;
    td_s32 ret;

    tde_osi_set_ext_alpha(double_src->bg_surface, double_src->fg_surface, hw_node);

    tde_osi_get_scan_info(&src_scan_info, &dst_scan_info);

    tde_osi_convert_surface(double_src->bg_surface, double_src->bg_rect, &src_scan_info, &src_drv_surface);

    tde_hal_node_set_src1(hw_node, &src_drv_surface);

    tde_osi_convert_surface(double_src->fg_surface, double_src->fg_rect, &src_scan_info, &src_drv_surface);

    tde_hal_node_set_src2(hw_node, &src_drv_surface);

    tde_osi_convert_surface(double_src->dst_surface, double_src->dst_rect, &dst_scan_info, &dst_drv_surface);

    tde_hal_node_set_tqt(hw_node, &dst_drv_surface, opt->out_alpha_from);

    code = tde_osi_double_src_get_opt_code(double_src->bg_surface->color_format,
        double_src->fg_surface->color_format, double_src->dst_surface->color_format);

    tde_osi_get_conv_by_code(code, &conv);

    if (tde_hal_node_set_color_convert(hw_node, &conv) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    ret = tde_osi_set_clut_opt(double_src->fg_surface, double_src->bg_surface, &clut_usage, opt->clut_reload, hw_node);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if ((opt->colorkey_mode >= DRV_TDE_COLOR_KEY_MODE_MAX) || (opt->colorkey_mode < DRV_TDE_COLOR_KEY_MODE_NONE)) {
        tde_error("color_key_mode error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    ret = tde_osi_set_color_key(double_src, hw_node, opt->colorkey_value, opt->colorkey_mode, clut_usage);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = tde_set_node_csc(hw_node, opt->csc_opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_double_src_pattern_fill(td_s32 handle, const drv_tde_double_src *double_src,
                                              const drv_tde_pattern_fill_opt *opt)
{
    tde_hw_node *hw_node = TD_NULL;
    td_s32 ret;

    ret = tde_osi_check_double_src_pattern_fill_para(double_src, opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (tde_hal_node_init_nd(&hw_node) != TD_SUCCESS) {
        return DRV_ERR_TDE_NO_MEM;
    }

    /* deal with opt */
    ret = tde_osi_double_pattern_fill_opt(hw_node, double_src, opt);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }

    ret = tde_osi_double_pattern_fill_set_up(hw_node, double_src, opt);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }

    ret = tde_osi_set_node_finish(handle, hw_node, 0, TDE_NODE_SUBM_ALONE);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 tde_osi_check_double_src_pattern_fill_para(const drv_tde_double_src *double_src,
                                                         const drv_tde_pattern_fill_opt *opt)
{
    td_bool is_contain_null_ptr = ((double_src == TD_NULL) || (opt == TD_NULL) ||
                                   (double_src->bg_surface == TD_NULL) || (double_src->bg_rect == TD_NULL) ||
                                   (double_src->fg_surface == TD_NULL) || (double_src->fg_rect == TD_NULL) ||
                                   (double_src->dst_surface == TD_NULL) || (double_src->dst_rect == TD_NULL));

    if (is_contain_null_ptr) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (tde_osi_check_double_src_pattern_opt(double_src->bg_surface->color_format,
        double_src->fg_surface->color_format, double_src->dst_surface->color_format, opt) != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (tde_osi_check_overlap(double_src->fg_surface, double_src->fg_rect, double_src->bg_surface,
        double_src->bg_rect) ||
        tde_osi_check_overlap(double_src->fg_surface, double_src->fg_rect, double_src->dst_surface,
        double_src->dst_rect)) {
        tde_error("Surface overlap!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    return TD_SUCCESS;
}

static td_s32 tde_osi_set_blend(tde_hw_node *hw_node, drv_tde_alpha_blending alpha_blending_cmd,
                                drv_tde_blend_opt blend_opt,
                                tde_alu_mode *alu_mode, td_bool check_blend)
{
    td_bool set_blend = ((td_u32)alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_BLEND) ? TD_TRUE : TD_FALSE;
    td_bool unknown_blend_mode = (blend_opt.blend_cmd == DRV_TDE_BLEND_CMD_CONFIG) &&
                                 ((blend_opt.src1_blend_mode >= DRV_TDE_BLEND_MAX) ||
                                 (blend_opt.src2_blend_mode >= DRV_TDE_BLEND_MAX));
#if (TDE_CAPABILITY & ROP)
    td_bool enable_alpha_rop = ((td_u32)alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_ROP) ? TD_TRUE : TD_FALSE;
#endif


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART17_H_ */
