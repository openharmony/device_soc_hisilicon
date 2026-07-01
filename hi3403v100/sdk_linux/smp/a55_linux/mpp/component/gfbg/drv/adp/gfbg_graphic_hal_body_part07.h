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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART07_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART07_H_

#ifdef CONFIG_GFBG_G1_SUPPORT_ZME
td_void hal_g1_zme_set_ck_gt_en(td_u32 ck_gt_en)
{
    volatile u_g1_zme_hinfo g1_zme_hinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hinfo.u32));
    g1_zme_hinfo.bits.ck_gt_en = ck_gt_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hinfo.u32), g1_zme_hinfo.u32);

    return;
}

td_void hal_g1_zme_set_out_width(td_u32 out_width)
{
    volatile u_g1_zme_hinfo g1_zme_hinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hinfo.u32));
    g1_zme_hinfo.bits.out_width = out_width - 1;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hinfo.u32), g1_zme_hinfo.u32);

    return;
}

td_void hal_g1_zme_set_hfir_en(td_u32 hfir_en)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.hfir_en = hfir_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_ahfir_mid_en(td_u32 ahfir_mid_en)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.ahfir_mid_en = ahfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_lhfir_mid_en(td_u32 lhfir_mid_en)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.lhfir_mid_en = lhfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_chfir_mid_en(td_u32 chfir_mid_en)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.chfir_mid_en = chfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_lhfir_mode(td_u32 lhfir_mode)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.lhfir_mode = lhfir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_ahfir_mode(td_u32 ahfir_mode)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.ahfir_mode = ahfir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_hfir_order(td_u32 hfir_order)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.hfir_order = hfir_order;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_hratio(td_u32 hratio)
{
    volatile u_g1_zme_hsp g1_zme_hsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32));
    g1_zme_hsp.bits.hratio = hratio;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hsp.u32), g1_zme_hsp.u32);

    return;
}

td_void hal_g1_zme_set_lhfir_offset(td_u32 lhfir_offset)
{
    volatile u_g1_zme_hloffset g1_zme_hloffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hloffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hloffset.u32));
    g1_zme_hloffset.bits.lhfir_offset = lhfir_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hloffset.u32), g1_zme_hloffset.u32);

    return;
}

td_void hal_g1_zme_set_chfir_offset(td_u32 chfir_offset)
{
    volatile u_g1_zme_hcoffset g1_zme_hcoffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_hcoffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hcoffset.u32));
    g1_zme_hcoffset.bits.chfir_offset = chfir_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_hcoffset.u32), g1_zme_hcoffset.u32);

    return;
}

td_void hal_g1_zme_set_out_pro(td_u32 out_pro)
{
    volatile u_g1_zme_vinfo g1_zme_vinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vinfo.u32));
    g1_zme_vinfo.bits.out_pro = out_pro;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vinfo.u32), g1_zme_vinfo.u32);

    return;
}

td_void hal_g1_zme_set_out_height(td_u32 out_height)
{
    volatile u_g1_zme_vinfo g1_zme_vinfo;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vinfo.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vinfo.u32));
    g1_zme_vinfo.bits.out_height = out_height - 1;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vinfo.u32), g1_zme_vinfo.u32);

    return;
}

td_void hal_g1_zme_set_vfir_en(td_u32 vfir_en)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.vfir_en = vfir_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_avfir_mid_en(td_u32 avfir_mid_en)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.avfir_mid_en = avfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_lvfir_mid_en(td_u32 lvfir_mid_en)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.lvfir_mid_en = lvfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_cvfir_mid_en(td_u32 cvfir_mid_en)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.cvfir_mid_en = cvfir_mid_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_lvfir_mode(td_u32 lvfir_mode)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.lvfir_mode = lvfir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_vafir_mode(td_u32 vafir_mode)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.vafir_mode = vafir_mode;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_vratio(td_u32 vratio)
{
    volatile u_g1_zme_vsp g1_zme_vsp;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_vsp.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32));
    g1_zme_vsp.bits.vratio = vratio;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_vsp.u32), g1_zme_vsp.u32);

    return;
}

td_void hal_g1_zme_set_vtp_offset(td_u32 vtp_offset)
{
    volatile u_g1_zme_voffset g1_zme_voffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_voffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_voffset.u32));
    g1_zme_voffset.bits.vtp_offset = vtp_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_voffset.u32), g1_zme_voffset.u32);

    return;
}

td_void hal_g1_zme_set_vbtm_offset(td_u32 vbtm_offset)
{
    volatile u_g1_zme_voffset g1_zme_voffset;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    g1_zme_voffset.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->g1_zme_voffset.u32));
    g1_zme_voffset.bits.vbtm_offset = vbtm_offset;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->g1_zme_voffset.u32), g1_zme_voffset.u32);

    return;
}
#endif


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART07_H_ */
