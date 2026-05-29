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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART07_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART07_H_

#if (TDE_CAPABILITY & RESIZE)
#define TDE_OSI_ZME_SHIFT TDE_FLOAT_BITLEN

typedef struct {
    td_s32 in_start;
    td_s32 in_stop;
    td_s32 out_start;
    td_s32 out_stop;
    td_s32 phase;
} tde_osi_zme_range;

static td_void tde_osi_set_value_to_clear(td_s32 *zme_vin, td_s32 *zme_vout, td_s32 *zme_vphase)
{
    *zme_vin = 0;
    *zme_vout = 0;
    *zme_vphase = 0;
}
static td_void tde_osi_get_info_zme(const tde_update_config *reg, tde_update_info *info)
{
    info->zme_instart_w = reg->update_instart_w;
    info->zme_outstart_w = reg->update_instart_w;
    info->zme_in_width = reg->update_in_width;
    info->zme_out_width = reg->update_in_width;
    info->zme_hphase = 0;
}
static td_void tde_osi_set_hupdate_info(tde_update_info *info, const tde_osi_zme_range *zme)
{
    info->zme_instart_w = zme->in_start;
    info->zme_outstart_w = zme->out_start;
    info->zme_in_width = zme->in_stop - zme->in_start + 1;
    info->zme_out_width = zme->out_stop - zme->out_start + 1;
    info->zme_hphase = zme->phase;
}
static td_void tde_osi_get_hupdate_info(const tde_update_config *reg, tde_update_info *info, td_bool scaler)
{
    tde_osi_zme_range zme;
    td_s32 ratio;
    td_s32 dratio;
    td_s32 update_hstart = reg->update_instart_w;
    td_s32 update_hstop = update_hstart + reg->update_in_width - 1;

    if (((reg->zme_out_width - 1) == 0) || ((reg->ori_in_width - 1) == 0)) {
        return;
    }
    /* 4096 2 alg data */
    ratio = (td_s32)(4096 * (reg->ori_in_width - 1) / (reg->zme_out_width - 1) + 1 / 2);
    /* 4096 alg data */
    dratio = 4096 * (reg->zme_out_width - 1) / (reg->ori_in_width - 1);
    /* hor_scaler not enable */
    if (scaler != TD_TRUE) {
        tde_osi_get_info_zme(reg, info);
        return;
    }
    if ((update_hstart >= 0) && (update_hstart < 3)) { /* 3 alg data */
        /* update outstretched area exceed left limit bordline */
        tde_osi_set_value_to_clear(&zme.in_start, &zme.out_start, &zme.phase);
    } else {
        /* update outstretched area didn't exceed the left limit bordline */
        zme.in_start = (update_hstart - 3) * dratio; /* 3 alg data */
        /* 12 4096 alg data */
        zme.out_start = (zme.in_start % 4096) == 0 ? ((td_u32)zme.in_start >> TDE_OSI_ZME_SHIFT) :
            (((td_u32)zme.in_start >> TDE_OSI_ZME_SHIFT) + 1);
        zme.in_start = (td_u32)(zme.out_start * ratio) >> TDE_OSI_ZME_SHIFT; /* 12 alg data */
        if (zme.in_start - 2 < 0) { /* 2 alg data */
            /* 4096 the left few point need mirror pixels when scale */
            zme.phase = (zme.out_start * ratio) % 4096 + zme.in_start * 4096;
            zme.in_start = 0;
        } else {
            /* 4096 3 the left few point not need mirror pixels when scale */
            zme.phase = (zme.out_start * ratio) % 4096 + 3 * 4096;
            zme.in_start = zme.in_start - 2; /* 2 alg data */
        }
    }
    if ((update_hstop > (reg->ori_in_width - 3)) && (update_hstop < reg->ori_in_width)) { /* 3 alg data */
        /* update outstretched area exceed the right limit bordline */
        zme.in_stop = reg->ori_in_width - 1;
        zme.out_stop = reg->zme_out_width - 1;
    } else {
        /* 2 1 alg data, update outstretched area didn't exceed the right limit bordline */
        zme.in_stop = (update_hstop + 2 + 1) * dratio;
        /* 12 4096 alg data */
        zme.out_stop = (zme.in_stop % 4096) == 0 ? (((td_u32)zme.in_stop >> TDE_OSI_ZME_SHIFT) - 1) :
            ((td_u32)zme.in_stop >> TDE_OSI_ZME_SHIFT);
        zme.in_stop = (td_u32)(zme.out_stop * ratio) >> TDE_OSI_ZME_SHIFT;  /* 12 alg data */
        if (zme.in_stop + 3 > (reg->ori_in_width - 1)) { /* 3 alg data */
            /* the right few point need mirror pixels when scale */
            zme.in_stop = reg->ori_in_width - 1;
        } else {
            /* the right few point need mirror pixels when scale */
            zme.in_stop = zme.in_stop + 3; /* 3 alg data */
        }
    }
    tde_osi_set_hupdate_info(info, &zme);
}
static td_void tde_osi_get_info_to_reg(const tde_update_config *reg, tde_update_info *info)
{
    info->zme_instart_h = reg->update_instart_h;
    info->zme_in_height = reg->update_in_height;
    info->zme_outstart_h = reg->update_instart_h;
    info->zme_out_height = reg->update_in_height;
    info->zme_vphase = 0;
    info->def_offsetup = 0;
    info->def_offsetdown = 0;
}
static td_void tde_osi_set_vupdate_info(tde_update_info *info, const tde_osi_zme_range *zme)
{
    info->zme_in_height = zme->in_stop - zme->in_start + 1;
    info->zme_instart_h = zme->in_start;
    info->zme_outstart_h = zme->out_start;
    info->zme_out_height = zme->out_stop - zme->out_start + 1;
    info->zme_vphase = zme->phase;
    info->def_offsetup = 0;
    info->def_offsetdown = 0;
}
static td_void tde_osi_get_vupdate_info(const tde_update_config *reg, tde_update_info *info, int scaler)
{
    tde_osi_zme_range zme;
    td_s32 ratio;
    td_s32 dratio;
    td_s32 update_vstart = reg->update_instart_h;
    td_s32 update_vstop = update_vstart + reg->update_in_height - 1;

    if ((reg->zme_out_height == 0) || (reg->ori_in_height == 0)) {
        return;
    }
    ratio = (td_s32)(4096 * (reg->ori_in_height) / (reg->zme_out_height)); /* 4096 alg data */
    dratio = 4096 * (reg->zme_out_height) / (reg->ori_in_height); /* 4096 alg data */

    if (scaler != TD_TRUE) {
        tde_osi_get_info_to_reg(reg, info);
        return;
    }
    /* ver_scale enable & deflicker not enable */
    if ((update_vstart >= 0) && (update_vstart < 2)) { /* 2 Vstart */
        /* the update outstreatched area exceed the up limit bordline */
        tde_osi_set_value_to_clear(&zme.in_start, &zme.out_start, &zme.phase);
    } else {
        /* the update outstreatched area didn't exceed the up limit bordline */
        zme.in_start = (update_vstart - 2) * dratio; /* 2 alg data */
        /* 4096 12 alg data */
        zme.out_start = (zme.in_start % 4096) == 0 ? ((td_u32)zme.in_start >> TDE_OSI_ZME_SHIFT) :
            (((td_u32)zme.in_start >> TDE_OSI_ZME_SHIFT) + 1);
        zme.in_start = (td_u32)(zme.out_start * ratio) >> TDE_OSI_ZME_SHIFT; /* 12 alg data */
        if (zme.in_start - 1 < 0) {
            /* the up few point need mirror pixels when scale */
            zme.phase = (zme.out_start * ratio) % 4096 + zme.in_start * 4096; /* 4096 alg data */
            zme.in_start = 0;
        } else {
            /* the up few point not need mirror pixels when scale */
            zme.phase = (zme.out_start * ratio) % 4096 + 1 * 4096; /* 4096 alg data */
            zme.in_start = zme.in_start - 1;
        }
    }
    /* 2 alg data */
    if ((update_vstop > (reg->ori_in_height - 2)) && (update_vstop < reg->ori_in_height)) {
        /* the update outstreatched area exceed the down limit bordline */
        zme.in_stop = reg->ori_in_height - 1;
        zme.out_stop = reg->zme_out_height - 1;
    } else {
        /* the update outstreatched area didn't exceed the down limit bordline */
        zme.in_stop = (update_vstop + 1 + 1) * dratio;
        /* 4096 12 alg data */
        zme.out_stop = (zme.in_stop % 4096) == 0 ? (((td_u32)zme.in_stop >> TDE_OSI_ZME_SHIFT) - 1) :
            ((td_u32)zme.in_stop >> TDE_OSI_ZME_SHIFT);
        zme.in_stop = (td_u32)(zme.out_stop * ratio) >> TDE_OSI_ZME_SHIFT; /* 12 alg data */
        if (zme.in_stop + 2 > (reg->ori_in_height - 1)) { /* 2 alg data */
            /* the down few point need mirror pixels when scale */
            zme.in_stop = reg->ori_in_height - 1;
        } else {
            /* the down few point not need mirror pixels when scale */
            zme.in_stop = zme.in_stop + 2; /* 2 alg data */
        }
    }
    tde_osi_set_vupdate_info(info, &zme);
}

#undef TDE_OSI_ZME_SHIFT
#endif

static td_void tde_osi_src2_filter_reg(const drv_tde_double_src *double_src, tde_update_config *reg)
{
    reg->ori_in_height = double_src->fg_surface->height;
    reg->ori_in_width = double_src->fg_surface->width;
    reg->zme_out_height = double_src->dst_surface->height;
    reg->zme_out_width = double_src->dst_surface->width;
    reg->update_instart_w = double_src->fg_rect->pos_x;
    reg->update_instart_h = double_src->fg_rect->pos_y;
    reg->update_in_width = double_src->fg_rect->width;
    reg->update_in_height = double_src->fg_rect->height;
}
static td_void tde_osi_src2_filter_rect_opt(tde_rect_opt *rect_opt, const tde_update_info *info)
{
    rect_opt->in_rect->pos_x = info->zme_instart_w;
    rect_opt->in_rect->pos_y = info->zme_instart_h;
    rect_opt->in_rect->width = info->zme_in_width;
    rect_opt->in_rect->height = info->zme_in_height;

    rect_opt->out_rect->pos_x = info->zme_outstart_w;
    rect_opt->out_rect->pos_y = info->zme_outstart_h;
    rect_opt->out_rect->width = info->zme_out_width;
    rect_opt->out_rect->height = info->zme_out_height;
}
static td_s32 tde_osi_src2_filter_opt(tde_hw_node *node, drv_tde_double_src *double_src, tde_rect_opt *rect_opt,
    drv_tde_deflicker_mode deflicker_mode, drv_tde_deflicker_mode filter_mode)
{
    td_s32 ret;
    tde_update_config reg = { 0 };
    tde_update_info info = { 0 };
    td_bool is_deflicker;
    td_bool scale = TD_FALSE;

#if (TDE_CAPABILITY & DEFLICKER)
    is_deflicker = (deflicker_mode == DRV_TDE_DEFLICKER_LEVEL_MODE_NONE) ? TD_FALSE : TD_TRUE;
#endif
    tde_osi_src2_filter_reg(double_src, &reg);
    if (node->src2_hsp.bits.hratio != TDE_NO_SCALE_HSTEP) {
        scale = TD_TRUE;
    }
    tde_info("is_h_scale:%x\n", scale);
    tde_osi_get_hupdate_info(&reg, &info, scale);

    scale = TD_FALSE;
    if (node->src2_vsr.bits.vratio != TDE_NO_SCALE_VSTEP) {
        scale = TD_TRUE;
    }
    tde_info("is_v_scale:%x\n", scale);
    tde_osi_get_vupdate_info(&reg, &info, scale);

    node->src2_imgsize.bits.src2_width = info.zme_in_width - 1;
    node->src2_zmeireso.bits.iw = info.zme_in_width - 1;

    node->src2_imgsize.bits.src2_height = info.zme_in_height - 1;
    node->src2_zmeireso.bits.ih = info.zme_in_height - 1;

    double_src->dst_rect->pos_x = info.zme_outstart_w;
    double_src->dst_rect->pos_y = info.zme_outstart_h;
    double_src->dst_rect->width = info.zme_out_width;
    double_src->dst_rect->height = info.zme_out_height;

    node->src2_hcoffset = info.zme_hphase;
    node->src2_voffset.bits.vluma_offset = info.zme_vphase;

    tde_osi_src2_filter_rect_opt(rect_opt, &info);
    if ((rect_opt->in_rect->width > MAX_LINE_BUFFER) && (rect_opt->out_rect->width > MAX_LINE_BUFFER)) {
    tde_error("it does not support local resize in width %u && out width %u over max line buffer %d!! \n",
              rect_opt->in_rect->width, rect_opt->out_rect->width, MAX_LINE_BUFFER);
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    ret = tde_hal_calc_src2_filter_opt(node, rect_opt, is_deflicker, filter_mode);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return TD_SUCCESS;
}
static td_s32 tde_osi_filter_reg(tde_hw_node *node, drv_tde_double_src *double_src,
    drv_tde_deflicker_mode deflicker_mode, drv_tde_deflicker_mode filter_mode)
{
    td_s32 ret;
    tde_rect_opt rect_opt;
    td_phys_addr_t src2_addr;
    td_phys_addr_t dst_addr;
    drv_tde_rect in_rect;
    drv_tde_rect out_rect;
    drv_tde_color_fmt out_fmt = double_src->dst_surface->color_format;
    td_bool back_ground_operation = ((double_src->bg_surface != TD_NULL) && (double_src->bg_rect != TD_NULL));

    rect_opt.in_fmt = double_src->fg_surface->color_format;
    rect_opt.in_rect = &in_rect;
    rect_opt.out_rect = &out_rect;
    if (((double_src->fg_surface->width != double_src->fg_rect->width) ||
        (double_src->fg_surface->height != double_src->fg_rect->height)) && g_region_deflicker) {
        ret = tde_osi_src2_filter_opt(node, double_src, &rect_opt, deflicker_mode, filter_mode);
        if (ret != TD_SUCCESS) {
            return ret;
        }
        if (double_src->dst_surface->width <= 1) {
            node->src2_hsp.bits.hratio = 0;
        } else {
            node->src2_hsp.bits.hratio = osal_div_u64(((td_u64)(double_src->fg_surface->width) <<
                TDE_HAL_HSTEP_FLOATLEN), (double_src->dst_surface->width));
        }
        if (double_src->dst_surface->height <= 1) {
            node->src2_vsr.bits.vratio = 0;
        } else {
            node->src2_vsr.bits.vratio = osal_div_u64(((td_u64)(double_src->fg_surface->height) << TDE_FLOAT_BITLEN),
                (double_src->dst_surface->height));
        }
        src2_addr = double_src->fg_surface->phys_addr;
        dst_addr = double_src->dst_surface->phys_addr;
        src2_addr += rect_opt.in_rect->pos_y * node->src2_ch0_stride.bits.src2_ch0_stride  +
            ((rect_opt.in_rect->pos_x * tde_osi_get_bpp_by_fmt(rect_opt.in_fmt)) / 8); /* 8 bits */
        dst_addr += rect_opt.out_rect->pos_y * node->des_ch0_stride.bits.des_ch0_stride +
            ((rect_opt.out_rect->pos_x * tde_osi_get_bpp_by_fmt(out_fmt)) / 8); /* 8 bits */

        node->src2_ch0_addr_low.bits.src2_ch0_addr_low = get_low_addr(src2_addr);
        node->src2_ch0_addr_high.bits.src2_ch0_addr_high = get_high_addr(src2_addr);

        node->des_ch0_addr_low.bits.des_ch0_addr_low = get_low_addr(dst_addr);
        node->des_ch0_addr_high.bits.des_ch0_addr_high = get_high_addr(dst_addr);
        if (back_ground_operation) {
            node->src1_imgsize.bits.src1_width = node->des_imgsize.bits.des_width;
            node->src1_imgsize.bits.src1_height = node->des_imgsize.bits.des_height;
        }
    }
    return TD_SUCCESS;
}
static td_s32 tde_osi_filter_opera(tde_hw_node *node, drv_tde_double_src *double_src,
    drv_tde_deflicker_mode deflicker_mode, drv_tde_deflicker_mode filter_mode)
{
    td_s32 ret;
    td_bool is_deflicker;
    tde_rect_opt rect_opt;
    td_bool fore_ground_operation = ((double_src->fg_surface != TD_NULL) && (double_src->fg_rect != TD_NULL));

#if (TDE_CAPABILITY & DEFLICKER)
    is_deflicker = (deflicker_mode == DRV_TDE_DEFLICKER_LEVEL_MODE_NONE) ? TD_FALSE : TD_TRUE;
#endif
    if (fore_ground_operation) {
        if ((double_src->fg_rect->width > MAX_LINE_BUFFER) && (double_src->dst_rect->width > MAX_LINE_BUFFER)) {
            tde_error("it does not support resize in width %u && out width %u over max line buffer %d!! \n",
                      double_src->fg_rect->width, double_src->dst_rect->width, MAX_LINE_BUFFER);
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }
        rect_opt.in_fmt = double_src->fg_surface->color_format;
        rect_opt.in_rect = double_src->fg_rect;
        rect_opt.out_rect = double_src->dst_rect;
        ret = tde_hal_calc_src2_filter_opt(node, &rect_opt, is_deflicker, filter_mode);
        if (ret != TD_SUCCESS) {
            return ret;
        }
        ret = tde_osi_filter_reg(node, double_src, deflicker_mode, filter_mode);
        if (ret != TD_SUCCESS) {
            return ret;
        }
    }
    return TD_SUCCESS;
}
static td_s32 tde_osi_set_filter_node(td_s32 handle, tde_hw_node *node, drv_tde_double_src *double_src,
    drv_tde_deflicker_mode deflicker_mode, drv_tde_deflicker_mode filter_mode)
{
    td_s32 ret;
    td_bool back_ground_operation = ((double_src->bg_surface != TD_NULL) && (double_src->bg_rect != TD_NULL));

    if (back_ground_operation && (g_region_deflicker == TD_FALSE)) {
        if ((double_src->bg_rect->width != double_src->dst_rect->width) ||
            (double_src->bg_rect->height != double_src->dst_rect->height)) {
            tde_error("it does not support  src1 resize!! \n");
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }
    }
    ret = tde_osi_filter_opera(node, double_src, deflicker_mode, filter_mode);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#if (TDE_CAPABILITY & SLICE)
    return tde_osi_calc_slice(handle, node);
#else
    ret = tde_osi_set_node_finish(handle, node, 0, TDE_NODE_SUBM_ALONE);
    if (ret < 0) {
        return ret;
    }
    return TD_SUCCESS;
#endif
}
/*
 * Function:      tde_osi_adj_para4_ycb_cr422_r
 * Description:   when fill color is YCbCr422R, fill by word
 * Input:         dst_surface: target bitmap info struct
 *                dst_rect: target operate zone
 *                fill_color: fill color
 * Others:        add software fill, when YCbCr422, fill function by word
 */
static td_void tde_osi_adj_para4_ycb_cr422_r(drv_tde_surface *dst_surface, drv_tde_rect *dst_rect,
                                             drv_tde_fill_color *fill_color)
{
    if ((dst_surface->color_format != DRV_TDE_COLOR_FMT_YCBCR422) &&
        (dst_surface->color_format != DRV_TDE_COLOR_FMT_PKGVYUY)) {
        return;
    }
    dst_surface->color_format = DRV_TDE_COLOR_FMT_AYCBCR8888;
    dst_surface->alpha_max_is_255 = TD_TRUE;
    fill_color->color_format = DRV_TDE_COLOR_FMT_AYCBCR8888;

    dst_surface->width /= 2; /* 2 width */

    dst_rect->width /= 2; /* 2 width */
    dst_rect->pos_x /= 2; /* 2 pos_x */

    fill_color->color_value = tde_get_yc422r_fillvalue(fill_color->color_value);
}
static td_s32 tde_osi_check_fill_opt(const drv_tde_opt *opt)
{
    td_bool real = (((opt->blend_opt.global_alpha_en != TD_TRUE) && (opt->blend_opt.global_alpha_en != TD_FALSE)) ||
        ((opt->blend_opt.pixel_alpha_en != TD_TRUE) && (opt->blend_opt.pixel_alpha_en != TD_FALSE)) ||
        ((opt->blend_opt.src1_alpha_premulti != TD_TRUE) && (opt->blend_opt.src1_alpha_premulti != TD_FALSE)) ||
        ((opt->blend_opt.src2_alpha_premulti != TD_TRUE) && (opt->blend_opt.src2_alpha_premulti != TD_FALSE)) ||
        ((opt->clut_reload != TD_TRUE) && (opt->clut_reload != TD_FALSE)) ||
        ((opt->is_compress != TD_TRUE) && (opt->is_compress != TD_FALSE)) ||
        ((opt->is_decompress != TD_TRUE) && (opt->is_decompress != TD_FALSE)));

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
#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART07_H_ */
