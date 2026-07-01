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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART04_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART04_H_

        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_out_ctrl.u32));
        gfx_out_ctrl.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_out_ctrl.bits.premulti_en = enable;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool fb_hal_graphic_get_gfx_premult(hal_disp_layer layer, td_u32 *enable)
{
    volatile u_gfx_out_ctrl gfx_out_ctrl;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (enable == TD_NULL)) {
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
        *enable = gfx_out_ctrl.bits.premulti_en;
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    return TD_TRUE;
}

td_bool fb_hal_graphic_set_gfx_palpha(hal_disp_layer layer, td_u32 alpha_en, td_u32 arange,
                                      td_u8 alpha0, td_u8 alpha1)
{
    volatile u_gfx_out_ctrl gfx_out_ctrl;
    volatile u_gfx_1555_alpha gfx_1555_alpha;
    volatile td_ulong addr_reg;
    ot_unused(arange);

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
        gfx_out_ctrl.bits.palpha_en = alpha_en;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);

        if (alpha_en == TD_TRUE) {
            addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_1555_alpha.u32));
            gfx_1555_alpha.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            gfx_1555_alpha.bits.alpha_1 = alpha1;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_1555_alpha.u32);

            addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_1555_alpha.u32));
            gfx_1555_alpha.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            gfx_1555_alpha.bits.alpha_0 = alpha0;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_1555_alpha.u32);
        } else {
            addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_1555_alpha.u32));
            gfx_1555_alpha.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            gfx_1555_alpha.bits.alpha_1 = 0xff;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_1555_alpha.u32);

            addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_1555_alpha.u32));
            gfx_1555_alpha.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
            gfx_1555_alpha.bits.alpha_0 = 0xff;
            fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_1555_alpha.u32);
        }
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool fb_hal_graphic_set_gfx_key_en(hal_disp_layer layer, td_u32 key_enable)
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
        gfx_out_ctrl.bits.enable = key_enable;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey in %s: L%d\n",
                  (td_s32)layer, __FUNCTION__, __LINE__);
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool fb_hal_graphic_set_gfx_key_mode(hal_disp_layer layer, td_u32 key_out)
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
        gfx_out_ctrl.bits.key_mode = key_out;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_out_ctrl.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey mode in %s: L%d\n",
                  (td_s32)layer, __FUNCTION__, __LINE__);
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool fb_hal_graphic_set_color_key_value(hal_disp_layer layer, hal_gfx_key_max key_max,
                                           hal_gfx_key_min key_min)
{
    volatile u_gfx_ckey_max gfx_ckey_max;
    volatile u_gfx_ckey_min gfx_ckey_min;
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
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_ckey_max.u32));
        gfx_ckey_max.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_ckey_max.bits.key_r_max = key_max.key_max_r;
        gfx_ckey_max.bits.key_g_max = key_max.key_max_g;
        gfx_ckey_max.bits.key_b_max = key_max.key_max_b;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_ckey_max.u32);

        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_ckey_min.u32));
        gfx_ckey_min.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_ckey_min.bits.key_r_min = key_min.key_min_r;
        gfx_ckey_min.bits.key_g_min = key_min.key_min_g;
        gfx_ckey_min.bits.key_b_min = key_min.key_min_b;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_ckey_min.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey in %s: L%d\n",
                  (td_s32)layer, __FUNCTION__, __LINE__);
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool fb_hal_graphic_set_color_key_mask(hal_disp_layer layer, hal_gfx_mask msk)
{
    volatile u_gfx_ckey_mask gfx_ckey_mask;
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
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_ckey_mask.u32));
        gfx_ckey_mask.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_ckey_mask.bits.key_r_msk = msk.mask_r;
        gfx_ckey_mask.bits.key_g_msk = msk.mask_g;
        gfx_ckey_mask.bits.key_b_msk = msk.mask_b;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_ckey_mask.u32);
    } else {
        HAL_PRINT("Error layer id %d not support colorkey mask in %s: L%d\n", (td_s32)layer, __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    return TD_TRUE;
}

/* for gfx decompress */
td_bool fb_hal_graphic_set_gfx_dcmp_enable(hal_disp_layer layer, td_u32 enable)
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
        gfx_src_info.bits.dcmp_en = enable;
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_src_info.u32);
    } else {
        HAL_PRINT("Error layer id %d not support dcmp in %s: L%d\n", (td_s32)layer, __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    return TD_TRUE;
}

td_bool fb_hal_graphic_get_gfx_dcmp_enable_state(hal_disp_layer layer, td_bool *enable)
{
    volatile u_gfx_src_info gfx_src_info;
    volatile td_ulong addr_reg;

    if ((g_gfbg_reg == TD_NULL) || (enable == TD_NULL)) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1)) {
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_src_info.u32));
        gfx_src_info.u32 = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        *enable = gfx_src_info.bits.dcmp_en;
    } else {
        HAL_PRINT("Error layer id %d not support dcmp in %s: L%d\n", (td_s32)layer, __FUNCTION__, __LINE__);
        return TD_FALSE;
    }
    return TD_TRUE;
}

td_bool fb_hal_graphic_set_gfx_dcmp_addr(hal_disp_layer layer, td_phys_addr_t addr_ar)
{
    volatile td_u32 gfx_addr_h;
    volatile td_u32 gfx_addr_l;
    volatile td_ulong addr_reg;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    if ((layer == HAL_DISP_LAYER_GFX0) || (layer == HAL_DISP_LAYER_GFX1)) {
        /* DCMP low addr for AR */
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_addr_l));
        gfx_addr_l = fb_hal_read_reg((td_u32 *)(uintptr_t)addr_reg);
        gfx_addr_l = get_low_addr(addr_ar);
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_addr_l);

        /* DCMP high addr for AR */
        addr_reg = fb_vou_get_gfx_abs_addr(layer, (td_ulong)(uintptr_t)&(g_gfbg_reg->gfx_addr_h));
        gfx_addr_h = fb_hal_read_reg((td_u32 *)(uintptr_t)(uintptr_t)addr_reg);
        gfx_addr_h = get_high_addr(addr_ar);
        fb_hal_write_reg((td_u32 *)(uintptr_t)addr_reg, gfx_addr_h);
    } else {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return TD_FALSE;
    }

    return TD_TRUE;
}

td_void fb_hal_para_set_para_addr_vhd_chn(td_phys_addr_t addr)
{
    volatile td_phys_addr_t para_addr_vhd_chn;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    para_addr_vhd_chn = addr;

#ifdef CONFIG_GFBG_G1_SUPPORT_ZME
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn01), get_high_addr(para_addr_vhd_chn));
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn01), get_low_addr(para_addr_vhd_chn));
#endif

#ifdef CONFIG_GFBG_G0_SUPPORT_ZME_CHN0
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn00), get_high_addr(para_addr_vhd_chn));
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn00), get_low_addr(para_addr_vhd_chn));
#else
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn06), get_high_addr(para_addr_vhd_chn));
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn06), get_low_addr(para_addr_vhd_chn));
#endif

    return;
}

td_void fb_hal_para_set_para_up_vhd_chn(td_u32 chn_num)
{
    volatile u_para_up_vhd para_up_vhd;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    para_up_vhd.u32 = (1 << chn_num);
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);

    return;
}

#ifdef CONFIG_TDE_GFBG_COMPRESS_V1
td_void fb_hal_fdr_gfx_set_source_mode(td_u32 layer, td_u32 source_mode)
{
    volatile u_gfx_dcmp_ctrl gfx_dcmp_ctrl;

    if (layer >= GFX_MAX) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer *
        FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.osd_mode = source_mode;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer * FDR_GFX_OFFSET),
        gfx_dcmp_ctrl.u32);
    return;
}

td_void fb_hal_fdr_gfx_set_cmp_mode(td_u32 layer, td_u32 cmp_mode)
{
    volatile u_gfx_dcmp_ctrl gfx_dcmp_ctrl;

    if (layer >= GFX_MAX) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer *
        FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.cmp_mode = cmp_mode;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer * FDR_GFX_OFFSET),
        gfx_dcmp_ctrl.u32);

    return;
}

td_void fb_hal_fdr_gfx_set_is_loss_lessa(td_u32 layer, td_u32 is_lossless_a)
{
    volatile u_gfx_dcmp_ctrl gfx_dcmp_ctrl;

    if (layer >= GFX_MAX) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer *
        FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.is_lossless_a = is_lossless_a;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer * FDR_GFX_OFFSET),
        gfx_dcmp_ctrl.u32);

    return;
}

td_void fb_hal_fdr_gfx_set_is_loss_less(td_u32 layer, td_u32 is_lossless)
{
    volatile u_gfx_dcmp_ctrl gfx_dcmp_ctrl;

    if (layer >= GFX_MAX) {
        OT_TRACE(OT_DBG_ERR, OT_ID_FB, "Error, %s(),%d Select Wrong Layer ID\n", __FUNCTION__, __LINE__);
    }

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_dcmp_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer *
        FDR_GFX_OFFSET));
    gfx_dcmp_ctrl.bits.is_lossless = is_lossless;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->gfx_dcmp_ctrl.u32)) + layer * FDR_GFX_OFFSET),
        gfx_dcmp_ctrl.u32);

    return;
}
#endif


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART04_H_ */
