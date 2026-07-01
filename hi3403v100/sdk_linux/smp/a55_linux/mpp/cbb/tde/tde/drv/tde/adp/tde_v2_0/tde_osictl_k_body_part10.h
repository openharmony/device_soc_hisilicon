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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART10_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART10_H_
    }
    if (opt == TD_NULL) {
        tde_error("source format must be the same with dst format!\n");
        return TDE_OPERATION_BUTT;
    }
    return TDE_OPERATION_SINGLE_SRC2;
}
static tde_operation_category tde_osi_double_src_operation(drv_tde_double_src *double_src, const drv_tde_opt *opt)
{
    if ((double_src->bg_rect == TD_NULL) || (double_src->fg_rect == TD_NULL) || (opt == TD_NULL)) {
        tde_error("Null pointer!\n");
        return TDE_OPERATION_BUTT;
    }
    if (double_src->bg_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) {
        tde_error("This operation doesn't support Semi-plannar!\n");
        return TDE_OPERATION_BUTT;
    }
#ifdef TDE_BACKGROUND_COLORFOMATSUPPORT_YCBCR422
    if (double_src->bg_surface->color_format >= DRV_TDE_COLOR_FMT_YCBCR422) {
        tde_error("Background  doesn't support the colorfmt!\n");
        return TDE_OPERATION_BUTT;
    }
#endif

    if (tde_osi_check_surface(double_src->bg_surface, double_src->bg_rect) != TD_SUCCESS) {
        return TDE_OPERATION_BUTT;
    }
    if (tde_osi_check_surface(double_src->fg_surface, double_src->fg_rect) != TD_SUCCESS) {
        return TDE_OPERATION_BUTT;
    }
    if ((double_src->bg_rect->height != double_src->dst_rect->height) ||
        (double_src->bg_rect->width != double_src->dst_rect->width)) {
        tde_error("BackGroundRect is not the same with DstRect!\n");
        return TDE_OPERATION_BUTT;
    }
    if (!opt->resize) {
        if (double_src->bg_rect->height != double_src->fg_rect->height) {
            double_src->bg_rect->height = tde_min(double_src->bg_rect->height, double_src->fg_rect->height);
            double_src->dst_rect->height = double_src->bg_rect->height;
            double_src->fg_rect->height = double_src->bg_rect->height;
        }
        if (double_src->bg_rect->width != double_src->fg_rect->width) {
            double_src->bg_rect->width = tde_min(double_src->bg_rect->width, double_src->fg_rect->width);
            double_src->dst_rect->width = double_src->bg_rect->width;
            double_src->fg_rect->width = double_src->bg_rect->width;
        }
    }
    return TDE_OPERATION_DOUBLE_SRC;
}
/*
 * Function:      tde_osi_convert_surface
 * Description:   raster bitmap info by user upload translate to bitmap info which driver and hardware need
 * Input:         sur: raster bitmap info by user upload
 *                rect: raster bitmap operate zone by user upload
 *                scan_info: scanning direction info
 *                drv_sur: bitmap info which driver and hardware need
 * Output:        pstOperationArea: new operate zone fixed by scanning direction
 */
static td_void tde_osi_convert_surface(const drv_tde_surface *sur, const drv_tde_rect *rect,
                                       const tde_scandirection_mode *scan_info,
                                       tde_surface_msg *drv_sur)
{
    if (sur->color_format > DRV_TDE_COLOR_FMT_MAX) {
        tde_error("sur->color_format is invalid !\n");
        return;
    }
    drv_sur->color_format = g_tde_common_drv_color_fmt[sur->color_format];
    drv_sur->width = rect->width;
    drv_sur->height = rect->height;
    drv_sur->pitch = sur->stride;
    drv_sur->alpha_max_is_255 = sur->alpha_max_is_255;
    drv_sur->hor_scan = scan_info->hor_scan;
    drv_sur->ver_scan = scan_info->ver_scan;
    drv_sur->phys_addr = sur->phys_addr;
    drv_sur->cbcr_phys_addr = sur->cbcr_phys_addr;
    drv_sur->cb_cr_pitch = sur->cbcr_stride;
    drv_sur->rgb_order = g_tde_argb_order[sur->color_format];

    drv_sur->xpos = (td_u32)rect->pos_x;

    drv_sur->ypos = (td_u32)rect->pos_y;

    return;
}
static td_s32 tde_osi_check_clip_para(const drv_tde_double_src *double_src, const drv_tde_opt *opt,
                                      const tde_hw_node *hw_node)
{
    if (double_src->fg_surface == TD_NULL) {
        tde_error("double_src->fg_surface is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (double_src->dst_surface == TD_NULL) {
        tde_error("double_src->dst_surface is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (opt == TD_NULL) {
        tde_error("pstOpt is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (hw_node == TD_NULL) {
        tde_error("pstHwNode is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (opt->clip_mode >= DRV_TDE_CLIP_MODE_MAX) {
        tde_error("clip_mode error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (opt->clip_mode != DRV_TDE_CLIP_MODE_NONE) {
        if ((opt->clip_rect.height == 0) || (opt->clip_rect.width == 0) || (opt->clip_rect.pos_x < 0) ||
            ((td_u32)opt->clip_rect.pos_x > TDE_MAX_RECT_WIDTH) ||
            (opt->clip_rect.pos_y < 0) || ((td_u32)opt->clip_rect.pos_y > TDE_MAX_RECT_HEIGHT) ||
            ((td_u32)opt->clip_rect.width > TDE_MAX_RECT_WIDTH) ||
            ((td_u32)opt->clip_rect.height > TDE_MAX_RECT_HEIGHT)) {
            tde_error(" Clip rect x:%d, y:%d, w:%u, h:%u error!\n", opt->clip_rect.pos_x,
                opt->clip_rect.pos_y, opt->clip_rect.width, opt->clip_rect.height);
            return -1;
        }
    }
    return TD_SUCCESS;
}
static td_void tde_osi_double_src(drv_tde_double_src *double_src, const drv_tde_rect *inter_rect)
{
    if (double_src->bg_surface != TD_NULL) {
        double_src->bg_rect->pos_x += inter_rect->pos_x - double_src->dst_rect->pos_x;
        double_src->bg_rect->pos_y += inter_rect->pos_y - double_src->dst_rect->pos_y;
        double_src->bg_rect->height = inter_rect->height;
        double_src->bg_rect->width = inter_rect->width;
    }
    double_src->fg_rect->pos_x += inter_rect->pos_x - double_src->dst_rect->pos_x;
    double_src->fg_rect->pos_y += inter_rect->pos_y - double_src->dst_rect->pos_y;
    double_src->fg_rect->height = inter_rect->height;
    double_src->fg_rect->width = inter_rect->width;

    *(double_src->dst_rect) = *inter_rect;
}
static td_void tde_osi_get_clip_pos(tde_clip_cmd *clip, const drv_tde_opt *opt, const drv_tde_double_src *double_src,
    const drv_tde_rect *inter_rect)
{
    clip->clip_start_x = (opt->clip_rect.pos_x > double_src->dst_rect->pos_x) ?
        (opt->clip_rect.pos_x - double_src->dst_rect->pos_x) : 0;
    clip->clip_start_y = (opt->clip_rect.pos_y > double_src->dst_rect->pos_y) ?
        (opt->clip_rect.pos_y - double_src->dst_rect->pos_y) : 0;
    clip->clip_end_x = clip->clip_start_x + inter_rect->width - 1;
    clip->clip_end_y = clip->clip_start_y + inter_rect->height - 1;
    return;
}
/*
 * Function:      tde_osi_set_clip_para
 * Description:   set clip zone parameter
 * Input:         double_src:  bitmap info
 *                opt: operate option
 *                hw_node: hardware operate node
 */
static td_s32 tde_osi_set_clip_para(drv_tde_double_src *double_src, const drv_tde_opt *opt, tde_hw_node *hw_node)
{
    tde_clip_cmd clip = { 0 };
    drv_tde_rect inter_rect = { 0 };
    td_s32 ret;
    ret = tde_osi_check_clip_para(double_src, opt, hw_node);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if ((opt->clip_mode == DRV_TDE_CLIP_MODE_INSIDE) && (!opt->resize)) {
        if (tde_osi_get_inter_rect(double_src->dst_rect, &opt->clip_rect, &inter_rect) != TD_SUCCESS) {
            tde_error("clip and operation area have no inrerrect!\n");
            return DRV_ERR_TDE_CLIP_AREA;
        }
        tde_osi_double_src(double_src, &inter_rect);
    } else if (opt->clip_mode == DRV_TDE_CLIP_MODE_INSIDE) {
        if (tde_osi_get_inter_rect(double_src->dst_rect, &opt->clip_rect, &inter_rect) != TD_SUCCESS) {
            tde_error("clip and operation area have no inter-rect!\n");
            return DRV_ERR_TDE_CLIP_AREA;
        }
        clip.inside_clip = TD_TRUE;
        tde_osi_get_clip_pos(&clip, opt, double_src, &inter_rect);
        if (tde_hal_node_set_clipping(hw_node, &clip) < 0) {
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    } else if (opt->clip_mode == DRV_TDE_CLIP_MODE_OUTSIDE) {
        if (tde_osi_is_rect1_in_rect2(double_src->dst_rect, &opt->clip_rect)) {
            tde_error("clip and operation area have no inter-rect!\n");
            return DRV_ERR_TDE_CLIP_AREA;
        }
        if (tde_osi_get_inter_rect(double_src->dst_rect, &opt->clip_rect, &inter_rect) != TD_SUCCESS) {
            return TD_SUCCESS;
        }
        clip.inside_clip = TD_FALSE;
        tde_osi_get_clip_pos(&clip, opt, double_src, &inter_rect);
        if (tde_hal_node_set_clipping(hw_node, &clip) != TD_SUCCESS) {
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    } else if (opt->clip_mode >= DRV_TDE_CLIP_MODE_MAX) {
        tde_error("error clip mode!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}
static td_s32 tde_osi_check_sre_para(const drv_tde_double_src *double_src, const drv_tde_pattern_fill_opt *opt,
                                     const tde_hw_node *hw_node)
{
    if (double_src->fg_surface == TD_NULL) {
        tde_error("double_src->fg_surface is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (double_src->dst_surface == TD_NULL) {
        tde_error("double_src->dst_surface is null !");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (opt == TD_NULL) {
        tde_error("opt is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (hw_node == TD_NULL) {
        tde_error("hw_node is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (opt->clip_mode >= DRV_TDE_CLIP_MODE_MAX) {
        tde_error("clip_mode error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (opt->clip_mode != DRV_TDE_CLIP_MODE_NONE) {
        if ((opt->clip_rect.height == 0) || (opt->clip_rect.width == 0) || (opt->clip_rect.pos_x < 0) ||
            ((td_u32)opt->clip_rect.pos_x >= TDE_MAX_RECT_WIDTH) ||
            (opt->clip_rect.pos_y < 0) || ((td_u32)opt->clip_rect.pos_y >= TDE_MAX_RECT_HEIGHT) ||
            ((td_u32)opt->clip_rect.width >= TDE_MAX_RECT_WIDTH) ||
            ((td_u32)opt->clip_rect.height >= TDE_MAX_RECT_HEIGHT)) {
            tde_error(" Clip rect x:%d, y:%d, w:%u, h:%u error!\n", opt->clip_rect.pos_x,
                opt->clip_rect.pos_y, opt->clip_rect.width, opt->clip_rect.height);
            return -1;
        }
    }
    return TD_SUCCESS;
}
static td_s32 tde_osi_set_pattern_clip_para(const drv_tde_double_src *double_src, const drv_tde_pattern_fill_opt *opt,
                                            tde_hw_node *hw_node)
{
    tde_clip_cmd clip = {0};
    drv_tde_rect inter_rect = {0};
    td_s32 ret;

    ret = tde_osi_check_sre_para(double_src, opt, hw_node);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if (opt->clip_mode == DRV_TDE_CLIP_MODE_INSIDE) {
        if (tde_osi_get_inter_rect(double_src->dst_rect, &opt->clip_rect, &inter_rect) != TD_SUCCESS) {
            tde_error("clip and operation area have no inrerrect!\n");
            return DRV_ERR_TDE_CLIP_AREA;
        }
        clip.inside_clip = TD_TRUE;
        clip.clip_start_x = (opt->clip_rect.pos_x > double_src->dst_rect->pos_x) ?
            (opt->clip_rect.pos_x - double_src->dst_rect->pos_x) : 0;
        clip.clip_start_y = (opt->clip_rect.pos_y > double_src->dst_rect->pos_y) ?
            (opt->clip_rect.pos_y - double_src->dst_rect->pos_y) : 0;
        clip.clip_end_x = clip.clip_start_x + inter_rect.width - 1;
        clip.clip_end_y = clip.clip_start_y + inter_rect.height - 1;
        if (tde_hal_node_set_clipping(hw_node, &clip) != TD_SUCCESS) {
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    } else if (opt->clip_mode == DRV_TDE_CLIP_MODE_OUTSIDE) {
        if (tde_osi_is_rect1_in_rect2(double_src->dst_rect, &opt->clip_rect)) {
            tde_error("clip and operation area have no inter-rect!\n");
            return DRV_ERR_TDE_CLIP_AREA;
        }
        if (tde_osi_get_inter_rect(double_src->dst_rect, &opt->clip_rect, &inter_rect) != TD_SUCCESS) {
            return TD_SUCCESS;
        }
        clip.inside_clip = TD_FALSE;
        clip.clip_start_x = (opt->clip_rect.pos_x > double_src->dst_rect->pos_x) ?
            (opt->clip_rect.pos_x - double_src->dst_rect->pos_x) : 0;
        clip.clip_start_y = (opt->clip_rect.pos_y > double_src->dst_rect->pos_y) ?
            (opt->clip_rect.pos_y - double_src->dst_rect->pos_y) : 0;
        clip.clip_end_x = clip.clip_start_x + inter_rect.width - 1;
        clip.clip_end_y = clip.clip_start_y + inter_rect.height - 1;

        if (tde_hal_node_set_clipping(hw_node, &clip) != TD_SUCCESS) {
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    } else if (opt->clip_mode >= DRV_TDE_CLIP_MODE_MAX) {
        tde_error("error clip mode!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}
static td_void tde_osi_set_ext_alpha(const drv_tde_surface *back_ground, const drv_tde_surface *fore_ground,
                                     tde_hw_node *hw_node)
{
    tde_src_mode src = TDE_DRV_SRC_NONE;
    td_bool real;

    real = ((fore_ground != TD_NULL) && (fore_ground->color_format >= DRV_TDE_COLOR_FMT_ARGB1555) &&
        (fore_ground->color_format <= DRV_TDE_COLOR_FMT_BGRA1555));
    if (real) {
        tde_hal_node_set_src2_alpha(hw_node);
    }
    real = ((back_ground != TD_NULL) &&
        (back_ground->color_format >= DRV_TDE_COLOR_FMT_ARGB1555) &&
        (back_ground->color_format <= DRV_TDE_COLOR_FMT_BGRA1555));
    if (real) {
        tde_hal_node_set_src1_alpha(hw_node);
    }
    real = ((fore_ground != TD_NULL) && (fore_ground->support_alpha_ex_1555) &&
        (fore_ground->color_format >= DRV_TDE_COLOR_FMT_ARGB1555) &&
        (fore_ground->color_format <= DRV_TDE_COLOR_FMT_BGRA1555));
    if (real) {
        src = (td_u32)src | TDE_DRV_SRC_S2;
    }
    real = ((back_ground != TD_NULL) && (back_ground->support_alpha_ex_1555) &&
        (back_ground->color_format >= DRV_TDE_COLOR_FMT_ARGB1555) &&
        (back_ground->color_format <= DRV_TDE_COLOR_FMT_BGRA1555));
    if (real) {
        src = (td_u32)src | TDE_DRV_SRC_S1;
    }
    if ((td_u32)src & TDE_DRV_SRC_S1) {
        tde_hal_node_set_exp_alpha(hw_node, src, back_ground->alpha0, back_ground->alpha1);
    } else if ((td_u32)src & TDE_DRV_SRC_S2) {
        tde_hal_node_set_exp_alpha(hw_node, src, fore_ground->alpha0, fore_ground->alpha1);
    }
}
static td_s32 tde_osi_set_blend_opt_rop(const drv_tde_opt *opt, tde_hw_node *hw_node, tde_alu_mode *alu)
{
    if ((td_u32)opt->alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_BLEND) {
        *alu = TDE_ALU_BLEND;
        if (tde_hal_node_set_blend(hw_node, &opt->blend_opt) != TD_SUCCESS) {
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }
#if (TDE_CAPABILITY & ROP)
        if ((td_u32)opt->alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_ROP) {
            tde_hal_node_enable_alpha_rop(hw_node);
        }
#endif
    }
#if (TDE_CAPABILITY & COLORIZE)
    if ((td_u32)opt->alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_COLORIZE) {
        if (tde_hal_node_set_colorize(hw_node, opt->color_resize) != TD_SUCCESS) {
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    }
#endif
    return TD_SUCCESS;
}
/*
 * Function:      tde_osi_set_base_opt_para_for_blit
 * Description:   encapsulation function used to set operate type
 * Input:         opt: operate option
 *                opt_category: operate category
 *                hw_node: nareware operate node
 */
static td_s32 tde_osi_set_base_opt_para_for_blit(const drv_tde_opt *opt, const drv_tde_surface *src1,
                                                 const drv_tde_surface *src2, tde_operation_category opt_category,
                                                 tde_hw_node *hw_node)
{
    tde_base_opt_mode base_opt = { 0 };
    tde_alu_mode alu = TDE_ALU_NONE;
    td_s32 ret;

    if ((hw_node == TD_NULL) || (opt == TD_NULL)) {
        return TD_SUCCESS;
    }
    if ((src1 != TD_NULL) && (src2 != TD_NULL)) {
        if ((tde_osi_get_fmt_category(src2->color_format) == TDE_COLORFMT_CATEGORY_AN) &&
            ((tde_osi_get_fmt_category(src1->color_format) == TDE_COLORFMT_CATEGORY_ARGB) ||
            (tde_osi_get_fmt_category(src1->color_format) == TDE_COLORFMT_CATEGORY_YCBCR))) {
            alu = TDE_SRC1_BYPASS;
        }
    }
#if (TDE_CAPABILITY & ROP)
    if ((td_u32)opt->alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_ROP) {
        alu = TDE_ALU_ROP;

        if (tde_hal_node_set_rop(hw_node, opt->rop_color, opt->rop_alpha) != TD_SUCCESS) {
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    }
#endif
    ret = tde_osi_set_blend_opt_rop(opt, hw_node, &alu);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    base_opt = (opt_category == TDE_OPERATION_DOUBLE_SRC) ? TDE_NORM_BLIT_2OPT : TDE_NORM_BLIT_1OPT;

    tde_hal_node_set_global_alpha(hw_node, opt->global_alpha, opt->blend_opt.global_alpha_en);

    if (tde_hal_node_set_base_operate(hw_node, base_opt, alu, 0) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    ret = tde_set_node_csc(hw_node, opt->csc_opt);

    return ret;
}
#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART10_H_ */
