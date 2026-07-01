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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART15_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART15_H_

#if (TDE_CAPABILITY & MASKROP)
static td_s32 tde_osi_bitmap_mask_rop_set_hw_node(tde_hw_node *hw_node, const drv_tde_triple_src *triple_src,
                                                  drv_tde_surface *mid_surface, drv_tde_rect *mid_rect)
{
    tde_scandirection_mode scan_info = {0};
    tde_surface_msg src_drv_surface = {0};
    tde_surface_msg dst_drv_surface = {0};

    scan_info.hor_scan = TDE_SCAN_LEFT_RIGHT;
    scan_info.ver_scan = TDE_SCAN_UP_DOWN;

    tde_osi_convert_surface(triple_src->fg_surface, triple_src->fg_rect, &scan_info, &src_drv_surface);

    tde_hal_node_set_src1(hw_node, &src_drv_surface);

    tde_osi_convert_surface(triple_src->mask_surface, triple_src->mask_rect, &scan_info, &src_drv_surface);

    tde_hal_node_set_src2(hw_node, &src_drv_surface);

    if (memcpy_s(mid_surface, sizeof(drv_tde_surface), triple_src->fg_surface, sizeof(drv_tde_surface)) != EOK) {
        tde_error("secure function failure\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    mid_surface->phys_addr = tde_osi_list_get_phy_buff(0);
    if (mid_surface->phys_addr == 0) {
        tde_error("There is no temp buffer in tde_osi_bitmap_mask_rop!\n");
        return DRV_ERR_TDE_NO_MEM;
    }
    mid_surface->clut_phys_addr = 0;
    mid_rect->pos_x = 0;
    mid_rect->pos_y = 0;
    mid_rect->height = triple_src->fg_rect->height;
    mid_rect->width = triple_src->fg_rect->width;

    tde_osi_convert_surface(mid_surface, mid_rect, &scan_info, &dst_drv_surface);

    tde_hal_node_set_tqt(hw_node, &dst_drv_surface, DRV_TDE_OUT_ALPHA_FROM_NORM);

    tde_osi_set_ext_alpha(triple_src->fg_surface, mid_surface, hw_node);

    if (tde_hal_node_set_base_operate(hw_node, TDE_NORM_BLIT_2OPT, TDE_ALU_MASK_ROP1, 0) != TD_SUCCESS) {
        tde_osi_list_put_phy_buff(1);
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_bitmap_mask_rop_set_hw_node_pass2(tde_hw_node *hw_node_pass2,
                                                        const drv_tde_triple_src *triple_src,
                                                        const drv_tde_none_src *none_src,
                                                        drv_tde_rop_mode rop_color, drv_tde_rop_mode rop_alpha)
{
    tde_scandirection_mode scan_info = {0};
    tde_surface_msg src_drv_surface = {0};
    tde_surface_msg dst_drv_surface = {0};
    td_u16 code;
    tde_conv_mode_cmd conv = {0};

    scan_info.hor_scan = TDE_SCAN_LEFT_RIGHT;
    scan_info.ver_scan = TDE_SCAN_UP_DOWN;

    tde_osi_convert_surface(triple_src->bg_surface, triple_src->bg_rect, &scan_info, &src_drv_surface);

    tde_hal_node_set_src1(hw_node_pass2, &src_drv_surface);

    tde_osi_convert_surface(none_src->dst_surface, none_src->dst_rect, &scan_info, &dst_drv_surface);

    tde_hal_node_set_src2(hw_node_pass2, &dst_drv_surface);

    tde_osi_convert_surface(triple_src->dst_surface, triple_src->dst_rect, &scan_info, &dst_drv_surface);

    tde_hal_node_set_tqt(hw_node_pass2, &dst_drv_surface, DRV_TDE_OUT_ALPHA_FROM_NORM);

    code = tde_osi_single_src2_get_opt_code(triple_src->fg_surface->color_format,
                                            triple_src->dst_surface->color_format);

    tde_osi_get_conv_by_code(code, &conv);

    if (tde_hal_node_set_color_convert(hw_node_pass2, &conv) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    if (tde_hal_node_set_rop(hw_node_pass2, rop_color, rop_alpha) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    tde_osi_set_ext_alpha(triple_src->bg_surface, none_src->dst_surface, hw_node_pass2);

    /* logical operation second passs */
    if (tde_hal_node_set_base_operate(hw_node_pass2, TDE_NORM_BLIT_2OPT, TDE_ALU_MASK_ROP2, 0) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_bitmap_mask_rop_check_color(const drv_tde_triple_src *triple_src)
{
    if (!tde_osi_whether_contain_alpha(triple_src->fg_surface->color_format)) {
        tde_error("ForeGround bitmap must contains alpha component!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (triple_src->mask_surface->color_format != DRV_TDE_COLOR_FMT_A1) {
        tde_error("Maskbitmap's colorformat can only be A1 in tde_osi_bitmap_mask_rop!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (triple_src->dst_surface->color_format >= DRV_TDE_COLOR_FMT_CLUT1 &&
        triple_src->dst_surface->color_format <= DRV_TDE_COLOR_FMT_A8) {
        tde_error("dst color format %d not support\n", triple_src->dst_surface->color_format);
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}

td_s32 tde_osi_bitmap_mask_rop(td_s32 handle, const drv_tde_triple_src *triple_src, drv_tde_rop_mode rop_color,
                               drv_tde_rop_mode rop_alpha)
{
    tde_hw_node *hw_node = TD_NULL;
    tde_hw_node *hw_node_pass2 = TD_NULL;
    drv_tde_surface mid_surface = {0};
    drv_tde_rect mid_rect = {0};
    drv_tde_none_src none_src = {&mid_surface, &mid_rect};
    td_s32 ret;

    ret = tde_osi_bitmap_mask_check_para(triple_src);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (tde_osi_bitmap_mask_rop_check_color(triple_src) != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    /* init hw_node */
    if (tde_hal_node_init_nd(&hw_node) != TD_SUCCESS) {
        return DRV_ERR_TDE_NO_MEM;
    }
    /*
     * set hw_node:
     * set src1
     * set src2
     * set tqt
     * set ext alpha
     * set opt
     * set global alpha
     */
    ret = tde_osi_bitmap_mask_rop_set_hw_node(hw_node, triple_src, &mid_surface, &mid_rect);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }

    ret = tde_osi_set_node_finish(handle, hw_node, 0, TDE_NODE_SUBM_ALONE);
    if (ret != TD_SUCCESS) {
        goto err1;
    }
    /* init hw_node_pass2 */
    if (tde_hal_node_init_nd(&hw_node_pass2) != TD_SUCCESS) {
        ret =  DRV_ERR_TDE_NO_MEM;
        goto err1;
    }

    /*
     * set hw_node:
     * set src1
     * set src2
     * set tqt
     * set color convert
     * set rop
     * set ext alpha
     * set opt
     */
    ret = tde_osi_bitmap_mask_rop_set_hw_node_pass2(hw_node_pass2, triple_src, &none_src,
                                                    rop_color, rop_alpha);
    if (ret != TD_SUCCESS) {
        goto err2;
    }

    ret = tde_osi_set_node_finish(handle, hw_node_pass2, 1, TDE_NODE_SUBM_ALONE);
    if (ret != TD_SUCCESS) {
        goto err2;
    }
    return TD_SUCCESS;
err2:
    tde_hal_free_node_buf(hw_node_pass2);
err1:
    tde_hal_free_node_buf(hw_node);
    tde_osi_list_put_phy_buff(1);
    return ret;
}
#endif

#if (TDE_CAPABILITY & MASKBLEND)
static td_s32 tde_osi_bitmap_mask_blend_set_hw_node(tde_hw_node *hw_node, const drv_tde_triple_src *triple_src,
                                                    drv_tde_surface *mid_surface, drv_tde_rect *mid_rect)
{
    tde_scandirection_mode scan_info = {0};
    tde_surface_msg src_drv_surface = {0};
    tde_surface_msg dst_drv_surface = {0};

    scan_info.hor_scan = TDE_SCAN_LEFT_RIGHT;
    scan_info.ver_scan = TDE_SCAN_UP_DOWN;

    tde_osi_convert_surface(triple_src->fg_surface, triple_src->fg_rect, &scan_info, &src_drv_surface);

    tde_hal_node_set_src1(hw_node, &src_drv_surface);

    tde_osi_convert_surface(triple_src->mask_surface, triple_src->mask_rect, &scan_info, &src_drv_surface);

    tde_hal_node_set_src2(hw_node, &src_drv_surface);

    if (memcpy_s(mid_surface, sizeof(drv_tde_surface), triple_src->fg_surface, sizeof(drv_tde_surface)) != EOK) {
        tde_error("secure function failure\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    mid_surface->phys_addr = tde_osi_list_get_phy_buff(0);
    if (mid_surface->phys_addr == 0) {
        tde_error("There is no temp buffer in tde_osi_bitmap_mask_blend!\n");
        return DRV_ERR_TDE_NO_MEM;
    }
    mid_rect->pos_x = 0;
    mid_rect->pos_y = 0;
    mid_rect->height = triple_src->fg_rect->height;
    mid_rect->width = triple_src->fg_rect->width;

    tde_osi_convert_surface(mid_surface, mid_rect, &scan_info, &dst_drv_surface);

    tde_hal_node_set_tqt(hw_node, &dst_drv_surface, DRV_TDE_OUT_ALPHA_FROM_NORM);

    tde_osi_set_ext_alpha(triple_src->fg_surface, mid_surface, hw_node);

    if (tde_hal_node_set_base_operate(hw_node, TDE_NORM_BLIT_2OPT, TDE_ALU_MASK_BLEND, 0) != TD_SUCCESS) {
        tde_osi_list_put_phy_buff(1);
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    tde_hal_node_set_global_alpha(hw_node, 0xff, TD_TRUE);
    return TD_SUCCESS;
}

static td_s32 tde_osi_bitmap_mask_blend_set_hw_node_pass2(tde_hw_node *hw_node_pass2,
                                                          const drv_tde_triple_src *triple_src,
                                                          const drv_tde_surface *mid_surface,
                                                          const drv_tde_rect *mid_rect,
                                                          td_u8 alpha)
{
    tde_scandirection_mode scan_info = {0};
    tde_surface_msg src_drv_surface = {0};
    tde_surface_msg dst_drv_surface = {0};
    td_u16 code;
    tde_conv_mode_cmd conv = {0};
    tde_alu_mode drv_alu_mode = TDE_ALU_BLEND;
    drv_tde_blend_opt blend_opt = {0};

    scan_info.hor_scan = TDE_SCAN_LEFT_RIGHT;
    scan_info.ver_scan = TDE_SCAN_UP_DOWN;
    tde_osi_convert_surface(triple_src->bg_surface, triple_src->bg_rect, &scan_info, &src_drv_surface);

    tde_hal_node_set_src1(hw_node_pass2, &src_drv_surface);

    tde_osi_convert_surface(mid_surface, mid_rect, &scan_info, &dst_drv_surface);

    tde_hal_node_set_src2(hw_node_pass2, &dst_drv_surface);

    tde_osi_convert_surface(triple_src->dst_surface, triple_src->dst_rect, &scan_info, &dst_drv_surface);

    tde_hal_node_set_tqt(hw_node_pass2, &dst_drv_surface, DRV_TDE_OUT_ALPHA_FROM_NORM);

    code = tde_osi_single_src2_get_opt_code(triple_src->fg_surface->color_format,
        triple_src->dst_surface->color_format);

    tde_osi_get_conv_by_code(code, &conv);

    if (tde_hal_node_set_color_convert(hw_node_pass2, &conv) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    tde_hal_node_set_global_alpha(hw_node_pass2, alpha, TD_TRUE);

    if (tde_hal_node_set_base_operate(hw_node_pass2, TDE_NORM_BLIT_2OPT, drv_alu_mode, 0) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    blend_opt.global_alpha_en = TD_TRUE;
    blend_opt.pixel_alpha_en = TD_TRUE;
    blend_opt.blend_cmd = DRV_TDE_BLEND_CMD_NONE;
    if (tde_hal_node_set_blend(hw_node_pass2, &blend_opt) != TD_SUCCESS) {
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    tde_osi_set_ext_alpha(triple_src->bg_surface, mid_surface, hw_node_pass2);
    return TD_SUCCESS;
}

static td_s32 tde_osi_bitmap_mask_blend_check_color(const drv_tde_triple_src *triple_src,
                                                    drv_tde_alpha_blending blend_mode)
{
    if ((triple_src->mask_surface->color_format != DRV_TDE_COLOR_FMT_A1) &&
        (triple_src->mask_surface->color_format != DRV_TDE_COLOR_FMT_A8)) {
        tde_error("Maskbitmap's colorformat can only be An in tde_osi_bitmap_mask_blend!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (blend_mode < 0 || blend_mode >= DRV_TDE_ALPHA_BLENDING_MAX) {
        tde_error("Alum mode invalid!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (blend_mode != DRV_TDE_ALPHA_BLENDING_BLEND) {
        tde_error("Alum mode can only be blending in tde_osi_bitmap_mask_blend!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (triple_src->dst_surface->color_format >= DRV_TDE_COLOR_FMT_CLUT1 &&
        triple_src->dst_surface->color_format <= DRV_TDE_COLOR_FMT_A8) {
        tde_error("dst color format %d not support\n", triple_src->dst_surface->color_format);
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}

td_s32 tde_osi_bitmap_mask_blend(td_s32 handle, const drv_tde_triple_src *triple_src, td_u8 alpha,
                                 drv_tde_alpha_blending blend_mode)
{
    tde_hw_node *hw_node = TD_NULL;
    tde_hw_node *hw_node_pass2 = TD_NULL;
    drv_tde_surface mid_surface = {0};
    drv_tde_rect mid_rect = {0};
    td_s32 ret;

    ret = tde_osi_bitmap_mask_check_para(triple_src);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    /* check color format and blend mode */
    if (tde_osi_bitmap_mask_blend_check_color(triple_src, blend_mode) != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }

    /* init hw_node */
    if (tde_hal_node_init_nd(&hw_node) != TD_SUCCESS) {
        return DRV_ERR_TDE_NO_MEM;
    }

    /*
     * set hw_node:
     * set src1
     * set src2
     * set tqt
     * set ext alpha
     * set opt
     * set global alpha
     */
    ret = tde_osi_bitmap_mask_blend_set_hw_node(hw_node, triple_src, &mid_surface, &mid_rect);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        return ret;
    }

    ret = tde_osi_set_node_finish(handle, hw_node, 0, TDE_NODE_SUBM_ALONE);
    if (ret != TD_SUCCESS) {
        tde_osi_list_put_phy_buff(1);
        tde_hal_free_node_buf(hw_node);
        return ret;
    }

    /* init hw_node_pass2 */
    if (tde_hal_node_init_nd(&hw_node_pass2) != TD_SUCCESS) {
        tde_osi_list_put_phy_buff(1);
        tde_hal_free_node_buf(hw_node);
        return DRV_ERR_TDE_NO_MEM;
    }

    /*
     * set hw_node_pass2:
     * set src1
     * set src2
     * set tqt
     * set color convert
     * set global alpha
     * set opt
     * set blend
     * set ext alpha
     */
    ret = tde_osi_bitmap_mask_blend_set_hw_node_pass2(hw_node_pass2, triple_src, &mid_surface, &mid_rect,
                                                      alpha);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        tde_hal_free_node_buf(hw_node_pass2);
        return ret;
    }

    ret = tde_osi_set_node_finish(handle, hw_node_pass2, 1, TDE_NODE_SUBM_ALONE);
    if (ret != TD_SUCCESS) {
        tde_hal_free_node_buf(hw_node);
        tde_hal_free_node_buf(hw_node_pass2);
        tde_osi_list_put_phy_buff(1);
        return ret;
    }

    return TD_SUCCESS;
}
#endif

/*
 * Function:      tde_osi_solid_draw
 * Description:   operate src1 with src2, which result to dst_surface,operate setting is in pstOpt
 *                if src is MB, only support single source operate,
                  just to say to only support pstBackGround or pstForeGround
 * Input:         handle: task handle
 *                fill_color:  fill  color
 *                opt: operate parameter setting struct
 * Return:        TD_SUCCESS/TD_FAILURE
 */
td_s32 tde_osi_solid_draw(td_s32 handle, const drv_tde_single_src *single_src, drv_tde_fill_color *fill_color,
                          const drv_tde_opt *opt)
{
    if (single_src == TD_NULL) {
        tde_error("null pointer!\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (single_src->src_surface == TD_NULL) {
        return tde_osi_1_source_fill(handle, single_src->dst_surface, single_src->dst_rect, fill_color, opt);
    } else {
        if ((single_src->dst_surface == TD_NULL) || (fill_color == TD_NULL)) {
            tde_error("when src_surface is not NULL, dst_surface and fill_color should not NULL!\n");
            return DRV_ERR_TDE_NULL_PTR;
        }

        if ((single_src->src_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) ||
            (single_src->dst_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) ||
            (fill_color->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP)) {


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART15_H_ */
