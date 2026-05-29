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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART03_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART03_H_


static td_void fb_hal_layer_csc_set_param(hal_disp_layer layer, const csc_coef_param *coef_param)
{
    volatile u_g0_ot_pp_csc_scale g0_ot_pp_csc_scale;
    volatile u_g0_ot_pp_csc_min_y g0_ot_pp_csc_min_y;
    volatile u_g0_ot_pp_csc_min_c g0_ot_pp_csc_min_c;
    volatile u_g0_ot_pp_csc_max_y g0_ot_pp_csc_max_y;
    volatile u_g0_ot_pp_csc_max_c g0_ot_pp_csc_max_c;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_scale.u32));
        g0_ot_pp_csc_scale.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_scale.bits.ot_pp_csc_scale = coef_param->csc_scale2p;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_scale.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_min_y.u32));
        g0_ot_pp_csc_min_y.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_min_y.bits.ot_pp_csc_min_y = coef_param->csc_clip_min;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_min_y.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_min_c.u32));
        g0_ot_pp_csc_min_c.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_min_c.bits.ot_pp_csc_min_c = coef_param->csc_clip_min;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_min_c.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_max_y.u32));
        g0_ot_pp_csc_max_y.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_max_y.bits.ot_pp_csc_max_y = coef_param->csc_clip_max;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_max_y.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_max_c.u32));
        g0_ot_pp_csc_max_c.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_max_c.bits.ot_pp_csc_max_c = coef_param->csc_clip_max;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_max_c.u32);
    }
}

td_bool fb_hal_layer_set_csc_coef(hal_disp_layer layer, const csc_coef *coef)
{
    if ((layer < HAL_DISP_LAYER_VHD0) || (layer > HAL_DISP_LAYER_GFX4)) {
        HAL_PRINT("Error, Wrong layer ID!%d\n", __LINE__);
        return TD_FALSE;
    }

    if (coef == TD_NULL) {
        HAL_PRINT("Error, null pointer\n");
        return TD_FALSE;
    }

    fb_hal_layer_csc_set_dc_coef(layer, (vdp_csc_dc_coef *)(&coef->csc_in_dc0));
    fb_hal_layer_csc_set_coef(layer, (vdp_csc_coef *)(&coef->csc_coef00));
    fb_hal_layer_csc_set_param(layer, (csc_coef_param *)(&coef->new_csc_scale2p));

    return TD_TRUE;
}

td_bool fb_hal_layer_set_csc_en(hal_disp_layer layer, td_bool csc_en)
{
    if ((layer < HAL_DISP_LAYER_VHD0) || (layer > HAL_DISP_LAYER_GFX4)) {
        HAL_PRINT("Error, Wrong layer ID!%d\n", __LINE__);
        return TD_FALSE;
    }
#ifdef CONFIG_GFBG_SET_CSC_CTRL
    fb_hal_layer_csc_set_ck_gt_en(layer, TD_TRUE);
#else
    fb_hal_layer_csc_set_ck_gt_en(layer, TD_FALSE);
#endif
    fb_hal_layer_csc_set_enable(layer, csc_en);

    return TD_TRUE;
}

td_bool fb_hal_layer_set_src_resolution(hal_disp_layer layer, const ot_fb_rect *rect)
{
    volatile u_gfx_src_reso gfx_src_reso;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (rect == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_src_reso.u32));
        gfx_src_reso.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_src_reso.bits.src_w = rect->width - 1;
        gfx_src_reso.bits.src_h = rect->height - 1;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_src_reso.u32);
    } else {
        HAL_PRINT("Error:layer id not found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    return TD_TRUE;
}

td_bool fb_hal_layer_set_layer_in_rect(hal_disp_layer layer, const ot_fb_rect *rect)
{
    volatile u_gfx_ireso gfx_ireso;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (rect == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_ireso.u32));
        gfx_ireso.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_ireso.bits.ireso_w = rect->width - 1;
        gfx_ireso.bits.ireso_h = rect->height - 1;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_ireso.u32);
    } else {
        HAL_PRINT("Error layer id found in %s, %d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    return TD_TRUE;
}

td_bool fb_hal_layer_set_layer_out_rect(hal_disp_layer layer, const ot_fb_rect *rect)
{
    ot_unused(rect);
    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        return TD_TRUE;
    } else {
        HAL_PRINT("Error:layer id not found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }
}

/*
 * Name : hal_layer_set_layer_galpha
 * Desc : Set video/graphic layer's global alpha
 */
td_bool fb_hal_layer_set_layer_galpha(hal_disp_layer layer, td_u8 alpha0)
{
    volatile u_v0_ctrl v0_ctrl;
    volatile u_g0_ctrl g0_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->v0_ctrl.u32));
            v0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            v0_ctrl.bits.galpha = alpha0;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, v0_ctrl.u32);
            break;
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ctrl.u32));
            g0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            g0_ctrl.bits.galpha = alpha0;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ctrl.u32);
            break;
        default:
            HAL_PRINT("Error layer id %d found in %s: L%d\n", layer, __FUNCTION__, __LINE__);
            return TD_FALSE;
    }

    return TD_TRUE;
}

td_bool fb_hal_layer_get_layer_galpha(hal_disp_layer layer, td_u8 *alpha0)
{
    volatile u_v0_ctrl v0_ctrl;
    volatile u_g0_ctrl g0_ctrl;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (alpha0 == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->v0_ctrl.u32));
            v0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *alpha0 = v0_ctrl.bits.galpha;
            break;
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ctrl.u32));
            g0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *alpha0 = g0_ctrl.bits.galpha;
            break;
        default:
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FALSE;
    }

    return TD_TRUE;
}

/*
 * Name : fb_hal_layer_set_reg_up
 * Desc : Set layer(video or graphic) register update.
 */
td_bool fb_hal_layer_set_reg_up(hal_disp_layer layer)
{
    volatile u_v0_upd v0_upd;
    volatile u_g0_upd g0_upd;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2: {
            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->v0_upd.u32));
            v0_upd.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            /* video layer register update */
            v0_upd.bits.regup = 0x1;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, v0_upd.u32);
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4: {
            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_upd.u32));
            g0_upd.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            /* graphic layer register update */
            g0_upd.bits.regup = 0x1;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_upd.u32);
            break;
        }
        default: {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FALSE;
        }
    }

    return TD_TRUE;
}

/* set layer addr */
td_bool fb_hal_graphic_set_gfx_addr(hal_disp_layer layer, td_phys_addr_t laddr)
{
    volatile td_ulong gfx_addr_h;
    volatile td_ulong gfx_addr_l;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        /* Write low address to register. */
        gfx_addr_l = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_addr_l));
        fb_hal_write_reg((td_u32 *)(uintptr_t)gfx_addr_l, get_low_addr(laddr));

        /* Write high address to register. */
        gfx_addr_h = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_addr_h));
        fb_hal_write_reg((td_u32 *)(uintptr_t)gfx_addr_h, get_high_addr(laddr));
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }
    return TD_TRUE;
}

/*
 * Name : hal_graphic_get_gfx_addr
 * Desc : get layer addr.
 */
td_bool fb_hal_graphic_get_gfx_addr(hal_disp_layer layer, td_phys_addr_t *gfx_addr)
{
    volatile td_ulong addr_reg;
    volatile td_phys_addr_t addr_h = 0x0;
    volatile td_phys_addr_t addr_l = 0x0;

    if ((g_gfbg_reg == TD_NULL) || (gfx_addr == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_addr_l));
        addr_l = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_addr_h));
        addr_h = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    *gfx_addr = addr_l + ((td_u64)addr_h << 32); /* 32 max address */
    return TD_TRUE;
}

/* layer stride */
td_bool fb_hal_graphic_set_gfx_stride(hal_disp_layer layer, td_u16 pitch)
{
    volatile u_gfx_stride gfx_stride;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_stride.u32));
        gfx_stride.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_stride.bits.surface_stride = pitch;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_stride.u32);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    return TD_TRUE;
}

/* get layer stride */
td_bool fb_hal_graphic_get_gfx_stride(hal_disp_layer layer, td_u32 *gfx_stride)
{
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (gfx_stride == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_stride.u32));
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    *gfx_stride = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);

    return TD_TRUE;
}

/* layer bit ext. */
td_bool fb_hal_graphic_set_gfx_ext(hal_disp_layer layer, hal_gfx_bitextend mode)
{
    volatile u_gfx_out_ctrl gfx_out_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_out_ctrl.bits.bitext = mode;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool fb_hal_graphic_set_gfx_pre_mult(hal_disp_layer layer, td_u32 enable)
{
    volatile u_gfx_out_ctrl gfx_out_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART03_H_ */
