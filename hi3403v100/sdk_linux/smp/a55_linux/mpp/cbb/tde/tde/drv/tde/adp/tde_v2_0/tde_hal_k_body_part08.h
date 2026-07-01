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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART08_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART08_H_

    hw_node->src2_cbmpara.bits.s2_multiglobalen = TD_FALSE;

    hw_node->src1_cbmpara.bits.s1_premulten = blend_opt->src1_alpha_premulti;
    hw_node->src2_cbmpara.bits.s2_premulten = blend_opt->src2_alpha_premulti;
    hw_node->src2_cbmpara.bits.s2_palphaen = blend_opt->pixel_alpha_en;
    hw_node->src2_cbmpara.bits.s2_galphaen = blend_opt->global_alpha_en;

    /* set mode for src1 and src2 */ /* CNcomment:  配置Src1、Src2模式 */
    tde_hal_node_blend_mode(hw_node, blend_opt);
    hw_node->cbmctrl.bits.cbm_en = 1;
    hw_node->cbmctrl.bits.cbm_mode = 1;
    return TD_SUCCESS;
}

#if (TDE_CAPABILITY & COLORIZE)
td_s32 tde_hal_node_set_colorize(tde_hw_node *hw_node, td_u32 colorize)
{
    td_u32 capability;
    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    tde_hal_get_capability(&capability);
    if (!(capability & COLORIZE)) {
        tde_error("It deos not support Colorize\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    hw_node->cbmcolorize.bits.colorizeen = 1;
    hw_node->cbmcolorize.bits.colorizeb = colorize & 0xff;
    hw_node->cbmcolorize.bits.colorizeg = (colorize >> 8) & 0xff; /* 8 move to a minimum */
    hw_node->cbmcolorize.bits.colorizer = (colorize >> 16) & 0xff; /* 16 move to a minimum */
    return TD_SUCCESS;
}
#endif

td_void tde_hal_node_enable_alpha_rop(tde_hw_node *hw_node)
{
    if (hw_node == TD_NULL) {
        tde_error("null pointer\n");
        return;
    }
    hw_node->cbmalupara.bits.blendropen = 1;
    return;
}

/*
 * Function:      tde_hal_node_set_clut_opt
 * Description:   set color extend or color revise parameter
 * Input:         hw_node: pointer of node
 *                clut_cmd: Clut operation parameter
 */
td_s32 tde_hal_node_set_clut_opt(tde_hw_node *hw_node, const tde_clut_cmd *clut_cmd, td_bool reload)
{
    td_u32 capability = 0;

    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (clut_cmd == TD_NULL) {
        tde_error("ClutCmd is null !");
        return DRV_ERR_TDE_NULL_PTR;
    }
    tde_hal_get_capability(&capability);
    if (!(capability & CLUT)) {
        tde_error("It deos not support Clut\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    hw_node->src2_ctrl.bits.src2_clut_mode = (td_u32)clut_cmd->clut_mode;
    /* set node */ /* CNcomment:配置缓存节点 */
    if (clut_cmd->phy_clut_addr) {
        hw_node->tde_clut_addr_low = get_low_addr(clut_cmd->phy_clut_addr);
        hw_node->tde_clut_addr_high = get_high_addr(clut_cmd->phy_clut_addr);
    }

    ot_unused(reload);
    return TD_SUCCESS;
}

/*
 * Function:      tde_hal_node_set_colorkey
 * Description:   set parameter for color key operation  according color format
 * Input:         hw_node:pointer of node
 *                fmt_cat: color format
 *                color_key: pointer of color key value
 */
td_s32 tde_hal_node_set_colorkey(tde_hw_node *hw_node, tde_colorfmt_category fmt_cat,
                                 const tde_color_key_cmd *color_key)
{
    td_u32 capability = 0;

    if (hw_node == TD_NULL) {
        tde_error("hw_node is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (color_key == TD_NULL) {
        tde_error("color_key is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    tde_hal_get_capability(&capability);
    if (!(capability & COLORKEY)) {
        tde_error("It deos not support ColorKey\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    hw_node->cbmkeypara.bits.keysel = (td_u32)color_key->colorkey_mode;
    hw_node->cbmkeypara.bits.keyen = 1;
    if (fmt_cat == TDE_COLORFMT_CATEGORY_ARGB) {
        tde_hal_node_set_argbcolor_key_para(hw_node, color_key);
    } else if (fmt_cat == TDE_COLORFMT_CATEGORY_CLUT) {
        tde_hal_node_set_clutcolor_key_para(hw_node, color_key);
    } else if (fmt_cat == TDE_COLORFMT_CATEGORY_YCBCR) {
        tde_hal_node_set_ycbcr_color_key_para(hw_node, color_key);
    } else {
        tde_error("The clorfmt deos not support ColorKey\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    hw_node->cbmctrl.bits.cbm_en = 1;
    hw_node->cbmctrl.bits.cbm_mode = 1;

    return TD_SUCCESS;
}

static td_void tde_hal_node_set_ycbcr_color_key_para(tde_hw_node *hw_node, const tde_color_key_cmd *color_key)
{
    drv_tde_color_key_comp cr = color_key->colorkey_value.ycbcr_colorkey.cr;
    drv_tde_color_key_comp cb = color_key->colorkey_value.ycbcr_colorkey.cb;
    drv_tde_color_key_comp y = color_key->colorkey_value.ycbcr_colorkey.y;
    drv_tde_color_key_comp alpha = color_key->colorkey_value.ycbcr_colorkey.alpha;

    hw_node->cbmkeymin.all =
        tde_hal_get_ycb_cr_key_mask(cr.component_min, cb.component_min, y.component_min, alpha.component_min);
    hw_node->cbmkeymax.all =
        tde_hal_get_ycb_cr_key_mask(cr.component_max, cb.component_max, y.component_max, alpha.component_max);
    hw_node->cbmkeymask.all =
        tde_hal_get_ycb_cr_key_mask(cr.component_mask, cb.component_mask, y.component_mask, alpha.component_mask);

    hw_node->cbmkeypara.bits.keybmode = tde_hal_get_color_key_mode(&cr);
    hw_node->cbmkeypara.bits.keygmode = tde_hal_get_color_key_mode(&cb);
    hw_node->cbmkeypara.bits.keyrmode = tde_hal_get_color_key_mode(&y);
    hw_node->cbmkeypara.bits.keyamode = tde_hal_get_color_key_mode(&alpha);

    return;
}

static td_void tde_hal_node_set_clutcolor_key_para(tde_hw_node *hw_node, const tde_color_key_cmd *color_key)
{
    drv_tde_color_key_comp clut = color_key->colorkey_value.clut_colorkey.clut;
    drv_tde_color_key_comp alpha = color_key->colorkey_value.clut_colorkey.alpha;

    hw_node->cbmkeymin.all = tde_hal_get_clut_key_mask(clut.component_min, alpha.component_min);
    hw_node->cbmkeymax.all = tde_hal_get_clut_key_mask(clut.component_max, alpha.component_max);
    hw_node->cbmkeymask.all = tde_hal_get_clut_key_mask(clut.component_mask, alpha.component_mask);

    hw_node->cbmkeypara.bits.keybmode = tde_hal_get_color_key_mode(&clut);
    hw_node->cbmkeypara.bits.keyamode = tde_hal_get_color_key_mode(&alpha);

    return;
}

static td_void tde_hal_node_set_argbcolor_key_para(tde_hw_node *hw_node, const tde_color_key_cmd *color_key)
{
    drv_tde_color_key_comp blue = color_key->colorkey_value.argb_colorkey.blue;
    drv_tde_color_key_comp green = color_key->colorkey_value.argb_colorkey.green;
    drv_tde_color_key_comp red = color_key->colorkey_value.argb_colorkey.red;
    drv_tde_color_key_comp alpha = color_key->colorkey_value.argb_colorkey.alpha;

    hw_node->cbmkeymin.all =
        tde_hal_get_argb_key_mask(blue.component_min, green.component_min, red.component_min, alpha.component_min);
    hw_node->cbmkeymax.all =
        tde_hal_get_argb_key_mask(blue.component_max, green.component_max, red.component_max, alpha.component_max);
    hw_node->cbmkeymask.all =
        tde_hal_get_argb_key_mask(blue.component_mask, green.component_mask, red.component_mask, alpha.component_mask);

    hw_node->cbmkeypara.bits.keybmode = tde_hal_get_color_key_mode(&blue);
    hw_node->cbmkeypara.bits.keygmode = tde_hal_get_color_key_mode(&green);
    hw_node->cbmkeypara.bits.keyrmode = tde_hal_get_color_key_mode(&red);
    hw_node->cbmkeypara.bits.keyamode = tde_hal_get_color_key_mode(&alpha);
    return;
}

static td_u32 tde_hal_get_color_key_mode(const drv_tde_color_key_comp *color_key)
{
    return (td_u32)((color_key->is_component_ignore) ? TDE_COLORKEY_IGNORE :
                    (color_key->is_component_out) ? TDE_COLORKEY_AREA_OUT : TDE_COLORKEY_AREA_IN);
}

static td_u32 tde_hal_get_ycb_cr_key_mask(td_u8 cr, td_u8 cb, td_u8 cy, td_u8 alpha)
{
    return (td_u32)(cr | (cb << TDE_EIGHT_BITS_SHIFT) | (cy << TDE_SIXTEEN_BITS_SHIFT) |
            (alpha << TDE_TWENTYFOUR_BITS_SHIFT));
}

static td_u32 tde_hal_get_clut_key_mask(td_u8 clut, td_u8 alpha)
{
    return (td_u32)(clut | (alpha << TDE_TWENTYFOUR_BITS_SHIFT));
}

static td_u32 tde_hal_get_argb_key_mask(td_u8 blue, td_u8 green, td_u8 red, td_u8 alpha)
{
    return (td_u32)(blue | (green << TDE_EIGHT_BITS_SHIFT) | (red << TDE_SIXTEEN_BITS_SHIFT) |
            (alpha << TDE_TWENTYFOUR_BITS_SHIFT));
}

td_s32 tde_hal_node_set_clipping(tde_hw_node *hw_node, const tde_clip_cmd *clip)
{
    td_u32 capability = 0;

    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (clip == TD_NULL) {
        tde_error("Clip is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    tde_hal_get_capability(&capability);
    if (!(capability & CLIP)) {
        tde_error("It deos not support Clip\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    hw_node->des_alpha.bits.des_crop_mode = 0;
    if (!clip->inside_clip) {
        /* over clip */ /* CNcomment: 区域外clip指示 */
        hw_node->des_alpha.bits.des_crop_mode = 1;
    }
    hw_node->des_alpha.bits.des_crop_en = 1;
    hw_node->des_crop_pos_st.bits.des_crop_start_x = clip->clip_start_x;
    hw_node->des_crop_pos_st.bits.des_crop_start_y = clip->clip_start_y;
    hw_node->des_crop_pos_ed.bits.des_crop_end_x = clip->clip_end_x;
    hw_node->des_crop_pos_ed.bits.des_crop_end_y = clip->clip_end_y;
    return TD_SUCCESS;
}

static td_s32 tde_hal_calc_src2_filter_opt_check(const tde_hw_node *node, const tde_rect_opt *rect_opt)
{
    if (node == TD_NULL || rect_opt == TD_NULL) {
        tde_error("null pointer !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (rect_opt->in_rect == TD_NULL) {
        tde_error("rect_opt->pInRect is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (rect_opt->out_rect == TD_NULL) {
        tde_error("rect_opt->pOutRect is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (rect_opt->in_fmt >= (DRV_TDE_COLOR_FMT_MAX + 1)) {
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}

static td_s32 tde_hal_set_zme_in_limit(tde_hw_node *node, const tde_rect_opt *rect_opt)
{
    td_s32 ver_scale_coeff = 1;
    td_s32 ret;
    if ((rect_opt->out_rect->width * 16) <= rect_opt->in_rect->width) { /* 16 2^4 alg data */
        node->src2_hpzme.bits.src2_hpzme_en = 1;
        node->src2_hpzme.bits.src2_hpzme_mode = tde_hal_get_hpzme_mode(rect_opt->out_rect->width,
            rect_opt->in_rect->width);
        if ((node->src2_hpzme.bits.src2_hpzme_mode + 1) == 0) {
            return DRV_ERR_TDE_INVALID_PARA;
        }
        node->src2_hpzme_size.bits.src2_hpzme_width =
            rect_opt->in_rect->width / (node->src2_hpzme.bits.src2_hpzme_mode + 1) +
            ((rect_opt->in_rect->width % (node->src2_hpzme.bits.src2_hpzme_mode + 1)) ? 1 : 0);
        node->src2_hsp.bits.hratio = (rect_opt->out_rect->width <= 1) ? 0 :
            (osal_div_u64(((td_u64)(node->src2_hpzme_size.bits.src2_hpzme_width) << TDE_HAL_HSTEP_FLOATLEN),
                (rect_opt->out_rect->width)));
    }

    if ((rect_opt->out_rect->height * 16) <= rect_opt->in_rect->height) { /* 16 2^4 alg data */
        ret = tde_hal_get_ver_scale_coeff(rect_opt->out_rect->height, rect_opt->in_rect->height, &ver_scale_coeff);
        if (ret != TD_SUCCESS) {
            return ret;
        }
        if (ver_scale_coeff == 0) {
            return DRV_ERR_TDE_INVALID_PARA;
        }
        node->src2_zmeireso.bits.ih = rect_opt->in_rect->height / ver_scale_coeff - 1;
        node->src2_imgsize.bits.src2_height = rect_opt->in_rect->height / ver_scale_coeff - 1;
        node->src2_ch0_stride.bits.src2_ch0_stride = node->src2_ch0_stride.bits.src2_ch0_stride * ver_scale_coeff;
        if (rect_opt->out_rect->height == 0) {
            return DRV_ERR_TDE_INVALID_PARA;
        }
        node->src2_vsr.bits.vratio = osal_div_u64(((td_u64)(node->src2_imgsize.bits.src2_height) <<
            TDE_FLOAT_BITLEN), (rect_opt->out_rect->height));
    } else {
        if (rect_opt->out_rect->height == 0) {
            return DRV_ERR_TDE_INVALID_PARA;
        }
        node->src2_vsr.bits.vratio = (rect_opt->out_rect->height <= 1) ? 0 :
            (osal_div_u64(((td_u64)(rect_opt->in_rect->height) << TDE_FLOAT_BITLEN), (rect_opt->out_rect->height)));
    }
    return TD_SUCCESS;
}

static td_void tde_hal_set_zme(tde_hw_node *node, const tde_rect_opt *rect_opt, drv_tde_deflicker_mode filter_mode,
    td_bool defilicker)
{
    tde_filtermode flt_mode = {0};
    td_phys_addr_t ch_coef_addr;
    td_phys_addr_t cv_coef_addr;

    node->src2_hsp.bits.hchmsc_en = 1;
    node->src2_hsp.bits.hlmsc_en = 1;

    tde_hal_get_filter_mode((drv_tde_filter_mode)filter_mode, &flt_mode);

    if (node->src2_hsp.bits.hratio != NO_HSCALE_STEP) {
        if (rect_opt->out_rect->width > MAX_LINE_BUFFER) {
            node->src2_hsp.bits.hfir_order = 1;
        }
        if (node->src2_vsp.bits.zme_in_fmt == TDE_ZME_FMT_ARGB) {
            node->src2_hsp.bits.hlfir_en = flt_mode.alpha_en;
        } else {
            node->src2_hsp.bits.hlfir_en = flt_mode.luma_en;
        }
        node->src2_hsp.bits.hafir_en = flt_mode.alpha_en;
        node->src2_hsp.bits.hchfir_en = flt_mode.chrome_en;
        ch_coef_addr = g_para_table.hf_coef_addr + tde_hal_get_resize_para_htable(node->src2_hsp.bits.hratio) *
                       TDE_PARA_HTABLE_SIZE;

        node->tde_src2_zme_chaddr_low = get_low_addr(ch_coef_addr);
        node->tde_src2_zme_chaddr_high = get_high_addr(ch_coef_addr);

        node->tde_src2_zme_lhaddr_low = node->tde_src2_zme_chaddr_low;
        node->tde_src2_zme_lhaddr_high = node->tde_src2_zme_chaddr_high;
    }

    node->src2_vsp.bits.vchmsc_en = 1;
    node->src2_vsp.bits.vlmsc_en = 1;

    if ((node->src2_vsr.bits.vratio != NO_VSCALE_STEP) && (!defilicker)) {
        node->src2_vsp.bits.vafir_en = flt_mode.alpha_en;
        if (node->src2_vsp.bits.zme_in_fmt == TDE_ZME_FMT_ARGB) {
            node->src2_vsp.bits.vlfir_en = flt_mode.alpha_en;
        } else {
            node->src2_vsp.bits.vlfir_en = flt_mode.luma_en;
        }
        node->src2_vsp.bits.vchfir_en = flt_mode.chrome_en;

        cv_coef_addr = g_para_table.vf_coef_addr + tde_hal_get_resize_para_vtable(node->src2_vsr.bits.vratio) *
                       TDE_PARA_VTABLE_SIZE;
        node->tde_src2_zme_cvaddr_low = get_low_addr(cv_coef_addr);
        node->tde_src2_zme_cvaddr_high = get_high_addr(cv_coef_addr);

        node->tde_src2_zme_lvaddr_low = node->tde_src2_zme_cvaddr_low;
        node->tde_src2_zme_lvaddr_high = node->tde_src2_zme_cvaddr_high;
    }
}

#if (TDE_CAPABILITY & DEFLICKER)
static td_void tde_hal_set_defilicker(tde_hw_node *node)
{
    td_phys_addr_t cv_deflicker_coef_addr;

    node->src2_vsp.bits.vchmsc_en = 1;
    node->src2_vsp.bits.vlmsc_en = 1;
    node->src2_vsp.bits.vafir_en = 1;
    node->src2_vsp.bits.vlfir_en = 1;
    node->src2_vsp.bits.vchfir_en = 1;

    cv_deflicker_coef_addr = g_para_table.deflicker_vf_coef_addr +
                             tde_hal_get_resize_para_vtable(node->src2_vsr.bits.vratio) * TDE_PARA_VTABLE_SIZE;

    node->tde_src2_zme_cvaddr_low = get_low_addr(cv_deflicker_coef_addr);
    node->tde_src2_zme_cvaddr_high = get_high_addr(cv_deflicker_coef_addr);

    node->tde_src2_zme_lvaddr_low = node->tde_src2_zme_cvaddr_low;
    node->tde_src2_zme_lvaddr_high = node->tde_src2_zme_cvaddr_high;
}
#endif

td_s32 tde_hal_calc_src2_filter_opt(tde_hw_node *node, const tde_rect_opt *rect_opt, td_bool defilicker,
                                    drv_tde_deflicker_mode filter_mode)
{
    tde_color_fmt in_drv_fmt;
    td_s32 ret;

    ret = tde_hal_calc_src2_filter_opt_check(node, rect_opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    in_drv_fmt = tde_get_common_drv_color_fmt((td_u32)rect_opt->in_fmt);

    node->src2_vsp.bits.zme_in_fmt = tde_hal_get_zme_in_fmt(in_drv_fmt);

    node->src2_vsp.bits.zme_out_fmt = (node->src2_vsp.bits.zme_in_fmt == TDE_ZME_FMT_ARGB) ? TDE_ZME_FMT_ARGB :
                                       TDE_ZME_FMT_YUV444;

    node->src2_imgsize.bits.src2_width = rect_opt->in_rect->width - 1;
    node->src2_zmeireso.bits.iw = rect_opt->in_rect->width - 1;
    node->src2_zmeoreso.bits.ow = rect_opt->out_rect->width - 1;
    node->src2_zmeoreso.bits.oh = rect_opt->out_rect->height - 1;
    node->src2_imgsize.bits.src2_height = rect_opt->in_rect->height - 1;
    node->src2_zmeireso.bits.ih = rect_opt->in_rect->height - 1;

    node->des_imgsize.bits.des_width = rect_opt->out_rect->width - 1;
    node->des_imgsize.bits.des_height = rect_opt->out_rect->height - 1;

    node->src2_hsp.bits.hratio = (rect_opt->out_rect->width <= 1) ? 0 :
        (osal_div_u64(((td_u64)(rect_opt->in_rect->width) << TDE_HAL_HSTEP_FLOATLEN), (rect_opt->out_rect->width)));

    node->src2_hpzme_size.bits.src2_hpzme_width = rect_opt->in_rect->width;

    ret = tde_hal_set_zme_in_limit(node, rect_opt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    tde_hal_set_zme(node, rect_opt, filter_mode, defilicker);

#if (TDE_CAPABILITY & DEFLICKER)
    if (defilicker) {
        tde_hal_set_defilicker(node);
    }
#endif

    node->src2_zmeireso.bits.iw = node->src2_hpzme_size.bits.src2_hpzme_width - 1;

    node->src2_hpzme_size.bits.src2_hpzme_width = node->src2_hpzme_size.bits.src2_hpzme_width - 1;

    return TD_SUCCESS;
}

static td_u32 tde_hal_get_zme_in_fmt(tde_color_fmt in_drv_fmt)


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART08_H_ */
