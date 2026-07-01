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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART05_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART05_H_

#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
td_void fb_hal_v3r2_gfx_set_source_mode(td_u32 layer, td_u32 source_mode)
{
    volatile u_vdp_v3r2_line_osd_dcmp_glb_info gfx_dcmp_ctrl;

    if (layer >= GFX_MAX) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.osd_mode = source_mode;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET), gfx_dcmp_ctrl.u32);

    return;
}
td_void fb_hal_v3r2_gfx_set_is_loss_less(td_u32 layer, td_u32 is_lossless)
{
    volatile u_vdp_v3r2_line_osd_dcmp_glb_info gfx_dcmp_ctrl;

    if (layer >= GFX_MAX) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.is_lossless = is_lossless;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET), gfx_dcmp_ctrl.u32);

    return;
}
td_void fb_hal_v3r2_gfx_set_is_conv_en(td_u32 layer, td_u32 conv_en)
{
    volatile u_vdp_v3r2_line_osd_dcmp_glb_info gfx_dcmp_ctrl;

    if (layer >= GFX_MAX) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.conv_en = conv_en;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET), gfx_dcmp_ctrl.u32);

    return;
}
td_void fb_hal_v3r2_gfx_set_cmp_mode(td_u32 layer, td_u32 cmp_mode)
{
    volatile u_vdp_v3r2_line_osd_dcmp_glb_info gfx_dcmp_ctrl;

    if (layer >= GFX_MAX) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.cmp_mode = cmp_mode;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET), gfx_dcmp_ctrl.u32);

    return;
}
td_void fb_hal_v3r2_gfx_set_is_ice_en(td_u32 layer, td_u32 ice_en)
{
    volatile u_vdp_v3r2_line_osd_dcmp_glb_info gfx_dcmp_ctrl;

    if (layer >= GFX_MAX) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.ice_en = ice_en;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_glb_info.u32)) +
        layer * FDR_GFX_OFFSET), gfx_dcmp_ctrl.u32);

    return;
}
td_void fb_hal_v3r2_gfx_set_frame_size(td_u32 layer, td_u32 width, td_u32 height)
{
    volatile u_vdp_v3r2_line_osd_dcmp_frame_size frame_size;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    frame_size.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_frame_size.u32)) +
        layer * FDR_GFX_OFFSET));
    frame_size.bits.frame_width = width - 1;
    frame_size.bits.frame_height = height - 1;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->vdp_v3r2_line_osd_dcmp_frame_size.u32)) +
        layer * FDR_GFX_OFFSET), frame_size.u32);
}
#endif

/**********************************************************************************
*  Begin   : Graphic layer ZME relative hal functions.
**********************************************************************************/
td_void hal_g0_zme_set_ck_gt_en(td_u32 ck_gt_en)
{
    volatile u_g0_zme_hinfo g0_zme_hinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_hinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hinfo.u32));
    g0_zme_hinfo.bits.ck_gt_en = ck_gt_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hinfo.u32), g0_zme_hinfo.u32);

    return;
}
td_void hal_g0_zme_set_out_width(td_u32 out_width)
{
    volatile u_g0_zme_hinfo g0_zme_hinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_hinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hinfo.u32));
    g0_zme_hinfo.bits.out_width = out_width - 1;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hinfo.u32), g0_zme_hinfo.u32);

    return;
}
td_void hal_g0_zme_set_hfir_en(td_u32 hfir_en)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.hfir_en = hfir_en;
    if (fb_width_and_height_meet_8k_requirement()) {
        g0_zme_hsp.bits.hfir_en = 0;
    }
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}
td_void hal_g0_zme_set_ahfir_mid_en(td_u32 ahfir_mid_en)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.ahfir_mid_en = ahfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}
td_void hal_g0_zme_set_lhfir_mid_en(td_u32 lhfir_mid_en)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.lhfir_mid_en = lhfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}
td_void hal_g0_zme_set_chfir_mid_en(td_u32 chfir_mid_en)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.chfir_mid_en = chfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}
td_void hal_g0_zme_set_lhfir_mode(td_u32 lhfir_mode)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.lhfir_mode = lhfir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}
td_void hal_g0_zme_set_ahfir_mode(td_u32 ahfir_mode)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.ahfir_mode = ahfir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}
td_void hal_g0_zme_set_hfir_order(td_u32 hfir_order)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.hfir_order = hfir_order;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}
td_void hal_g0_zme_set_hratio(td_u32 hratio)
{
    volatile u_g0_zme_hsp g0_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32));
    g0_zme_hsp.bits.hratio = hratio;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hsp.u32), g0_zme_hsp.u32);

    return;
}
td_void hal_g0_zme_set_lhfir_offset(td_u32 lhfir_offset)
{
    volatile u_g0_zme_hloffset g0_zme_hloffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_hloffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hloffset.u32));
    g0_zme_hloffset.bits.lhfir_offset = lhfir_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hloffset.u32), g0_zme_hloffset.u32);

    return;
}
td_void hal_g0_zme_set_chfir_offset(td_u32 chfir_offset)
{
    volatile u_g0_zme_hcoffset g0_zme_hcoffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_hcoffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hcoffset.u32));
    g0_zme_hcoffset.bits.chfir_offset = chfir_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_hcoffset.u32), g0_zme_hcoffset.u32);

    return;
}
td_void hal_g0_zme_set_out_pro(td_u32 out_pro)
{
    volatile u_g0_zme_vinfo g0_zme_vinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_vinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vinfo.u32));
    g0_zme_vinfo.bits.out_pro = out_pro;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vinfo.u32), g0_zme_vinfo.u32);

    return;
}
td_void hal_g0_zme_set_out_height(td_u32 out_height)
{
    volatile u_g0_zme_vinfo g0_zme_vinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_vinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vinfo.u32));
    g0_zme_vinfo.bits.out_height = out_height - 1;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vinfo.u32), g0_zme_vinfo.u32);

    return;
}
td_void hal_g0_zme_set_vfir_en(td_u32 vfir_en)
{
    volatile u_g0_zme_vsp g0_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.vfir_en = vfir_en;
    if (fb_width_and_height_meet_8k_requirement()) {
        g0_zme_vsp.bits.vfir_en = 0;
    }
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}
td_void hal_g0_zme_set_avfir_mid_en(td_u32 avfir_mid_en)
{
    volatile u_g0_zme_vsp g0_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.avfir_mid_en = avfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}
td_void hal_g0_zme_set_lvfir_mid_en(td_u32 lvfir_mid_en)
{
    volatile u_g0_zme_vsp g0_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.lvfir_mid_en = lvfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}
td_void hal_g0_zme_set_cvfir_mid_en(td_u32 cvfir_mid_en)
{
    volatile u_g0_zme_vsp g0_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32));
    g0_zme_vsp.bits.cvfir_mid_en = cvfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g0_zme_vsp.u32), g0_zme_vsp.u32);

    return;
}
td_void hal_g0_zme_set_lvfir_mode(td_u32 lvfir_mode)
{
    volatile u_g0_zme_vsp g0_zme_vsp;
    if (g_gfbg_reg == TD_NULL) {
#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART05_H_ */
