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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART02_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART02_H_

    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (rect == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_dfpos.u32));
            g0_dfpos.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            g0_dfpos.bits.disp_xfpos = rect->x;
            g0_dfpos.bits.disp_yfpos = rect->y;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_dfpos.u32);

            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_dlpos.u32));
            g0_dlpos.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            g0_dlpos.bits.disp_xlpos = rect->x + rect->width - 1;
            g0_dlpos.bits.disp_ylpos = rect->y + rect->height - 1;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_dlpos.u32);
            break;
        default:
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FALSE;
    }

    return TD_TRUE;
}

/* Set the video image display area window */
td_bool fb_hal_video_set_layer_video_rect(hal_disp_layer layer, const ot_fb_rect *rect)
{
    volatile u_g0_vfpos g0_vfpos;
    volatile u_g0_vlpos g0_vlpos;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (rect == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_vfpos.u32));
            g0_vfpos.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            g0_vfpos.bits.video_xfpos = rect->x;
            g0_vfpos.bits.video_yfpos = rect->y;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_vfpos.u32);

            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_vlpos.u32));
            g0_vlpos.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            g0_vlpos.bits.video_xlpos = rect->x + rect->width - 1;
            g0_vlpos.bits.video_ylpos = rect->y + rect->height - 1;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_vlpos.u32);
            break;
        default:
            HAL_PRINT("Error layer id %d# found in %s: L%d\n", layer, __FUNCTION__, __LINE__);
            return TD_FALSE;
    }

    return TD_TRUE;
}

td_void fb_hal_link_get_hc_link(hal_disp_layer layer, td_u32 *data)
{
    volatile u_link_ctrl link_ctrl;

    if ((g_gfbg_reg == TD_NULL) || (data == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    link_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)&(g_gfbg_reg->link_ctrl.u32));
    switch (layer) {
        case HAL_DISP_LAYER_GFX4:
            *data = link_ctrl.bits.g4_link;
            break;
        case HAL_DISP_LAYER_GFX3:
            *data = link_ctrl.bits.g3_link;
            break;
        case HAL_DISP_LAYER_GFX2:
            *data = link_ctrl.bits.g2_link;
            break;
        default:
            break;
    }

    return;
}

/*
 * Name : fb_hal_set_layer_enable
 * Desc : Set layer enable
 */
td_bool fb_hal_set_layer_enable(hal_disp_layer layer, td_u32 enable)
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
        case HAL_DISP_LAYER_VHD2: {
            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->v0_ctrl.u32));
            v0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            v0_ctrl.bits.surface_en = enable;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, v0_ctrl.u32);
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ctrl.u32));
            g0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            g0_ctrl.bits.surface_en = enable;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ctrl.u32);
            break;
        default:
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FALSE;
    }

    return TD_TRUE;
}

td_void fb_hal_set_layer_ck_gt_en(hal_disp_layer layer, td_u32 ck_gt_en)
{
    volatile u_voctrl voctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    addr_reg = (td_ulong)(uintptr_t)&(g_gfbg_reg->voctrl.u32);
    voctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
            voctrl.bits.g0_ck_gt_en = ck_gt_en;
            break;
        case HAL_DISP_LAYER_GFX1:
            voctrl.bits.g1_ck_gt_en = ck_gt_en;
            break;
        case HAL_DISP_LAYER_GFX3:
            voctrl.bits.g3_ck_gt_en = ck_gt_en;
            break;
        default:
            /* Logic aren't configured for G2. Don't write the configuration to avoid affecting other layers. */
            return;
    }
    fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, voctrl.u32);
}

td_bool fb_hal_get_layer_enable(hal_disp_layer layer, td_u32 *enable)
{
    volatile u_v0_ctrl v0_ctrl;
    volatile u_g0_ctrl g0_ctrl;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (enable == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2: {
            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->v0_ctrl.u32));
            v0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *enable = v0_ctrl.bits.surface_en;
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
        case HAL_DISP_LAYER_GFX4:
            addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ctrl.u32));
            g0_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            *enable = g0_ctrl.bits.surface_en;
            break;
        default:
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return TD_FALSE;
    }

    return TD_TRUE;
}

/* Desc : Set layer data type */
td_bool fb_hal_layer_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt)
{
    volatile u_gfx_src_info gfx_src_info;
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
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_src_info.u32));
        gfx_src_info.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_src_info.bits.ifmt = data_fmt;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_src_info.u32);
    } else {
        HAL_PRINT("Error layer id%d found in %s: L%d\n", layer, __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    return TD_TRUE;
}

td_bool fb_hal_layer_get_layer_data_fmt(hal_disp_layer layer, td_u32 *fmt)
{
    volatile u_gfx_src_info gfx_src_info;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (fmt == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    if (layer == HAL_DISP_LAYER_GFX0 ||
        layer == HAL_DISP_LAYER_GFX1 ||
        layer == HAL_DISP_LAYER_GFX2 ||
        layer == HAL_DISP_LAYER_GFX3 ||
        layer == HAL_DISP_LAYER_GFX4) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_src_info.u32));
        gfx_src_info.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        *fmt = gfx_src_info.bits.ifmt;
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }
    return TD_TRUE;
}

static td_bool fb_width_and_height_meet_8k_requirement(td_void)
{
    ot_vo_dev vo_dev = 0;
    hal_disp_syncinfo sync_info = {0};
    td_u32 height;
    td_u32 width;

    fb_hal_disp_get_intf_sync(vo_dev, &sync_info);
    height = (sync_info.iop) ? sync_info.vact : (sync_info.vact * 2); /* 2 alg data */
    width = sync_info.hact;

    if (height == FB_8K_SCREEN_HEIGHT_G0 && width == FB_8K_SCREEN_WIDTH_G0) {
        return TD_TRUE;
    }
    return TD_FALSE;
}

static td_void fb_hal_layer_csc_set_enable(hal_disp_layer layer, td_bool csc_en)
{
    volatile u_g0_ot_pp_csc_ctrl g0_ot_pp_csc_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_ctrl.u32));
        g0_ot_pp_csc_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_ctrl.bits.ot_pp_csc_en = csc_en;
        if (layer == HAL_DISP_LAYER_GFX0 && fb_width_and_height_meet_8k_requirement()) {
            g0_ot_pp_csc_ctrl.bits.ot_pp_csc_en = 0;
        }
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_ctrl.u32);
    }
}

static td_void fb_hal_layer_csc_set_ck_gt_en(hal_disp_layer layer, td_bool ck_gt_en)
{
    volatile u_g0_ot_pp_csc_ctrl g0_ot_pp_csc_ctrl;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_ctrl.u32));
        g0_ot_pp_csc_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_ctrl.bits.ot_pp_csc_ck_gt_en = ck_gt_en;
        if (layer == HAL_DISP_LAYER_GFX0 && fb_width_and_height_meet_8k_requirement()) {
            g0_ot_pp_csc_ctrl.bits.ot_pp_csc_ck_gt_en = 0;
        }
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_ctrl.u32);
    }
}

static td_void fb_hal_layer_csc_set_coef(hal_disp_layer layer, const vdp_csc_coef *coef)
{
    volatile u_g0_ot_pp_csc_coef00 g0_ot_pp_csc_coef00;
    volatile u_g0_ot_pp_csc_coef01 g0_ot_pp_csc_coef01;
    volatile u_g0_ot_pp_csc_coef02 g0_ot_pp_csc_coef02;
    volatile u_g0_ot_pp_csc_coef10 g0_ot_pp_csc_coef10;
    volatile u_g0_ot_pp_csc_coef11 g0_ot_pp_csc_coef11;
    volatile u_g0_ot_pp_csc_coef12 g0_ot_pp_csc_coef12;
    volatile u_g0_ot_pp_csc_coef20 g0_ot_pp_csc_coef20;
    volatile u_g0_ot_pp_csc_coef21 g0_ot_pp_csc_coef21;
    volatile u_g0_ot_pp_csc_coef22 g0_ot_pp_csc_coef22;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if ((layer >= HAL_DISP_LAYER_GFX0) && (layer <= HAL_DISP_LAYER_GFX4)) {
        g0_ot_pp_csc_coef00.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef00.u32));
        g0_ot_pp_csc_coef00.bits.ot_pp_csc_coef00 = coef->csc_coef00;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef00.u32);

        g0_ot_pp_csc_coef01.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef01.u32));
        g0_ot_pp_csc_coef01.bits.ot_pp_csc_coef01 = coef->csc_coef01;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef01.u32);

        g0_ot_pp_csc_coef02.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef02.u32));
        g0_ot_pp_csc_coef02.bits.ot_pp_csc_coef02 = coef->csc_coef02;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef02.u32);

        g0_ot_pp_csc_coef10.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef10.u32));
        g0_ot_pp_csc_coef10.bits.ot_pp_csc_coef10 = coef->csc_coef10;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef10.u32);

        g0_ot_pp_csc_coef11.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef11.u32));
        g0_ot_pp_csc_coef11.bits.ot_pp_csc_coef11 = coef->csc_coef11;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef11.u32);

        g0_ot_pp_csc_coef12.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef12.u32));
        g0_ot_pp_csc_coef12.bits.ot_pp_csc_coef12 = coef->csc_coef12;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef12.u32);

        g0_ot_pp_csc_coef20.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef20.u32));
        g0_ot_pp_csc_coef20.bits.ot_pp_csc_coef20 = coef->csc_coef20;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef20.u32);

        g0_ot_pp_csc_coef21.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef21.u32));
        g0_ot_pp_csc_coef21.bits.ot_pp_csc_coef21 = coef->csc_coef21;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef21.u32);

        g0_ot_pp_csc_coef22.u32 = hal_get_addr_abs(&addr_reg, layer, &(g_gfbg_reg->g0_ot_pp_csc_coef22.u32));
        g0_ot_pp_csc_coef22.bits.ot_pp_csc_coef22 = coef->csc_coef22;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_coef22.u32);
    } else {
        HAL_PRINT("Error layer id found in %s, %d\n", __FUNCTION__, __LINE__);
    }
    return;
}

static td_void fb_hal_layer_csc_set_dc_coef(hal_disp_layer layer, const vdp_csc_dc_coef *csc_dc_coef)
{
    volatile u_g0_ot_pp_csc_idc0 g0_ot_pp_csc_idc0;
    volatile u_g0_ot_pp_csc_idc1 g0_ot_pp_csc_idc1;
    volatile u_g0_ot_pp_csc_idc2 g0_ot_pp_csc_idc2;
    volatile u_g0_ot_pp_csc_odc0 g0_ot_pp_csc_odc0;
    volatile u_g0_ot_pp_csc_odc1 g0_ot_pp_csc_odc1;
    volatile u_g0_ot_pp_csc_odc2 g0_ot_pp_csc_odc2;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    if ((layer >= HAL_DISP_LAYER_GFX0) && (layer <= HAL_DISP_LAYER_GFX4)) {
        addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_idc0.u32));
        g0_ot_pp_csc_idc0.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_idc0.bits.ot_pp_csc_idc0 = csc_dc_coef->csc_in_dc0;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_idc0.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_idc1.u32));
        g0_ot_pp_csc_idc1.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_idc1.bits.ot_pp_csc_idc1 = csc_dc_coef->csc_in_dc1;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_idc1.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_idc2.u32));
        g0_ot_pp_csc_idc2.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_idc2.bits.ot_pp_csc_idc2 = csc_dc_coef->csc_in_dc2;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_idc2.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_odc0.u32));
        g0_ot_pp_csc_odc0.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_odc0.bits.ot_pp_csc_odc0 = csc_dc_coef->csc_out_dc0;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_odc0.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_odc1.u32));
        g0_ot_pp_csc_odc1.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_odc1.bits.ot_pp_csc_odc1 = csc_dc_coef->csc_out_dc1;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_odc1.u32);

        addr_reg = fb_vou_get_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->g0_ot_pp_csc_odc2.u32));
        g0_ot_pp_csc_odc2.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        g0_ot_pp_csc_odc2.bits.ot_pp_csc_odc2 = csc_dc_coef->csc_out_dc2;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, g0_ot_pp_csc_odc2.u32);
    } else {
        HAL_PRINT("Error layer id found in %s, %d\n", __FUNCTION__, __LINE__);
    }
}


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART02_H_ */
