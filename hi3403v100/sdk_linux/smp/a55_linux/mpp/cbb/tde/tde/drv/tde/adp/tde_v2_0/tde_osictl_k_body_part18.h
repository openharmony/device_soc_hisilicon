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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART18_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART18_H_

    td_bool b_unknown_blend_cmd = (blend_opt.blend_cmd >= DRV_TDE_BLEND_CMD_MAX);

    if (!set_blend) {
        return TD_SUCCESS;
    }

    *alu_mode = TDE_ALU_BLEND;

    if (check_blend) {
        if (b_unknown_blend_cmd) {
            tde_error("Unknown blend cmd!\n");

            return DRV_ERR_TDE_INVALID_PARA;
        }

        if (unknown_blend_mode) {
            tde_error("Unknown blend mode!\n");

            return DRV_ERR_TDE_INVALID_PARA;
        }
    }

    if (tde_hal_node_set_blend(hw_node, &blend_opt) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
#if (TDE_CAPABILITY & ROP)

    if (enable_alpha_rop) {
        tde_hal_node_enable_alpha_rop(hw_node);
    }
#endif
    return TD_SUCCESS;
}

#if (TDE_CAPABILITY & COLORIZE)
static td_s32 tde_osi_set_colorize(tde_hw_node *hw_node, drv_tde_alpha_blending alpha_blending_cmd,
    td_s32 color_resize)
{
    td_bool set_colorize = ((td_u32)alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_COLORIZE) ? TD_TRUE : TD_FALSE;

    if (!set_colorize) {
        return TD_SUCCESS;
    }

    if (tde_hal_node_set_colorize(hw_node, color_resize) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    return TD_SUCCESS;
}
#endif

#if (TDE_CAPABILITY & ROP)
static td_s32 tde_osi_set_rop(tde_hw_node *hw_node, const tde_rop_opt *rop_opt, tde_alu_mode *alu_mode)
{
    td_bool set_rop = ((td_u32)rop_opt->alpha_blending_cmd & DRV_TDE_ALPHA_BLENDING_ROP) ? TD_TRUE : TD_FALSE;
    td_bool error_rop_code = (rop_opt->rop_code_color >= DRV_TDE_ROP_MAX) ||
                              (rop_opt->rop_code_alpha >= DRV_TDE_ROP_MAX);
    td_bool only_support_single_sr2_rop = ((!tde_osi_is_single_src_to_rop(rop_opt->rop_code_alpha)) ||
                                           (!tde_osi_is_single_src_to_rop(rop_opt->rop_code_color)));

    if (!set_rop) {
        return TD_SUCCESS;
    }

    *alu_mode = TDE_ALU_ROP;

    if (error_rop_code) {
        tde_error("enRopCode error!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (rop_opt->single_sr2_rop) {
        if (only_support_single_sr2_rop) {
            tde_error("Only support single s2 rop!\n");
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    }

    if (tde_hal_node_set_rop(hw_node, rop_opt->rop_code_color, rop_opt->rop_code_alpha) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    return TD_SUCCESS;
}
#endif

/*
 * Function:      tde_osi_pattern_fill
 * Description:   pattern fill
 * Input:         handle:task handle
                  opt: operate option
 * Return:        success/fail
 */
td_s32 tde_osi_pattern_fill(td_s32 handle, const drv_tde_double_src *double_src, const drv_tde_pattern_fill_opt *opt)
{
    tde_pattern_operation_category opt_category_en;
    drv_tde_single_src single_src = {0};

    opt_category_en = tde_osi_get_pattern_opt_category(double_src, opt);
    switch (opt_category_en) {
        case TDE_PATTERN_OPERATION_SINGLE_SRC:
            if (double_src->bg_surface != TD_NULL) {
                single_src.src_surface = double_src->bg_surface;
                single_src.src_rect = double_src->bg_rect;
                single_src.dst_surface = double_src->dst_surface;
                single_src.dst_rect = double_src->dst_rect;
            } else {
                single_src.src_surface = double_src->fg_surface;
                single_src.src_rect = double_src->fg_rect;
                single_src.dst_surface = double_src->dst_surface;
                single_src.dst_rect = double_src->dst_rect;
            }
            return tde_osi_single_src_pattern_fill(handle, &single_src, opt);
        case TDE_PATTERN_OPERATION_DOUBLE_SRC:
            return tde_osi_double_src_pattern_fill(handle, double_src, opt);
        default:
            return DRV_ERR_TDE_INVALID_PARA;
    }
}

/*
 * Function:      tde_cal_scale_rect
 * Description:   update zoom rect information
 * Input:         src_rect:source bitmap operate zone
                  dst_rect: target bitmap operate zone
 *                rect_in_src: source bitmap scale zone
 *                rect_in_dst: target bitmap info atfer calculating
 * Return:        success/fail
 */
td_s32 tde_cal_scale_rect(const drv_tde_rect *src_rect, const drv_tde_rect *dst_rect,
                          drv_tde_rect *rect_in_src, drv_tde_rect *rect_in_dst)
{
#if (TDE_CAPABILITY & RESIZE)
    tde_update_config reg = { 0 };
    tde_update_info info = { 0 };

    if (src_rect == TD_NULL || dst_rect == TD_NULL || rect_in_src == TD_NULL || rect_in_dst == TD_NULL) {
        return DRV_ERR_TDE_NULL_PTR;
    }
    reg.ori_in_height = src_rect->height;
    reg.ori_in_width = src_rect->width;
    reg.zme_out_height = dst_rect->height;
    reg.zme_out_width = dst_rect->width;

    reg.update_instart_w = rect_in_src->pos_x;
    reg.update_instart_h = rect_in_src->pos_y;
    reg.update_in_width = rect_in_src->width;
    reg.update_in_height = rect_in_src->height;

    tde_osi_get_hupdate_info(&reg, &info, TD_TRUE);
    tde_osi_get_vupdate_info(&reg, &info, TD_TRUE);

    rect_in_src->pos_x = info.zme_instart_w;
    rect_in_src->pos_y = info.zme_instart_h;
    rect_in_src->width = info.zme_in_width;
    rect_in_src->height = info.zme_in_height;

    rect_in_dst->pos_x = info.zme_outstart_w;
    rect_in_dst->pos_y = info.zme_outstart_h;
    rect_in_dst->width = info.zme_out_width;
    rect_in_dst->height = info.zme_out_height;
#endif
    return TD_SUCCESS;
}

td_s32 tde_osi_enable_region_deflicker(td_bool is_region_deflicker)
{
#if (TDE_CAPABILITY & DEFLICKER)
    if ((is_region_deflicker != TD_TRUE) && (is_region_deflicker != TD_FALSE)) {
        tde_error("bool should be TRUE or FALSE!!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    g_region_deflicker = is_region_deflicker;
#endif
    return TD_SUCCESS;
}

#ifdef CONFIG_TDE_BLIT_EX
td_s32 tde_osi_single_blit_ex(td_s32 handle, drv_tde_surface *src_surface, drv_tde_rect *src_rect,
                              drv_tde_surface *dst_surface, drv_tde_rect *dst_rect,
                              drv_tde_opt *opt, td_bool mmz_for_src, td_bool mmz_for_dst)
{
    td_s32 ret;
    drv_tde_single_src single_src;

    ret = tde_osi_check_surface(src_surface, src_rect);
    if (ret < 0) {
        return ret;
    }
    ret = tde_osi_check_surface(dst_surface, dst_rect);
    if (ret < 0) {
        return ret;
    }

    single_src.src_surface = src_surface;
    single_src.src_rect    = src_rect;
    single_src.dst_surface = dst_surface;
    single_src.dst_rect    = dst_rect;
    return tde_osi_single_src_2_blit(handle, &single_src, opt, mmz_for_src, mmz_for_dst);
}
#endif


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART18_H_ */
