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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART08_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART08_H_

#ifndef CONFIG_TDE_CLUT_RECT_V2
td_void hal_clut_set_up_param(td_u32 layer_id, td_phys_addr_t clut_phy_addr)
{
    volatile unsigned int para_haddr_vhd_chn11;
    volatile unsigned int para_addr_vhd_chn11;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if (layer_id != SMART_RECT_LAYER_G3) {
        HAL_PRINT("layer%u doesn't support clut\n", layer_id);
        return;
    }
    para_haddr_vhd_chn11 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn11));
    para_haddr_vhd_chn11 = ((td_u64)clut_phy_addr >> 32) & 0xffffffff; /* 32 high bits */
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn11), para_haddr_vhd_chn11);
    para_addr_vhd_chn11 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn11));
    para_addr_vhd_chn11 = clut_phy_addr & 0xffffffff;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn11), para_addr_vhd_chn11);
    /* up */
}
td_void hal_clut_set_up(td_u32 layer_id)
{
    volatile u_para_up_vhd  para_up_vhd;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if (layer_id != SMART_RECT_LAYER_G3) {
        HAL_PRINT("layer%u doesn't support clut\n", layer_id);
        return;
    }
    para_up_vhd.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32));
    /* up */
    para_up_vhd.bits.para_up_vhd_chn11 = 0x1;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);
}

#else
td_void hal_clut_set_up_param(td_u32 layer_id, td_phys_addr_t clut_phy_addr)
{
    volatile unsigned int para_haddr_vhd_chn06;
    volatile unsigned int para_haddr_vhd_chn07;
    volatile unsigned int para_addr_vhd_chn06;
    volatile unsigned int para_addr_vhd_chn07;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    if (layer_id == SMART_RECT_LAYER_G3) {
        para_haddr_vhd_chn07 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn07));
        para_haddr_vhd_chn07 = ((td_u64)clut_phy_addr >> 32) & 0xffffffff; /* 32 high bits */
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn07), para_haddr_vhd_chn07);
        para_addr_vhd_chn07 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn07));
        para_addr_vhd_chn07 = clut_phy_addr & 0xffffffff;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn07), para_addr_vhd_chn07);
    } else if (layer_id == SMART_RECT_LAYER_G4) {
        para_haddr_vhd_chn06 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn06));
        para_haddr_vhd_chn06 = ((td_u64)clut_phy_addr >> 32) & 0xffffffff; /* 32 high bits */
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn06), para_haddr_vhd_chn06);
        para_addr_vhd_chn06 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn06));
        para_addr_vhd_chn06 = clut_phy_addr & 0xffffffff;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn06), para_addr_vhd_chn06);
    } else {
        HAL_PRINT("layer%u doesn't support clut\n", layer_id);
    }
}
td_void hal_clut_set_up(td_u32 layer_id)
{
    volatile u_para_up_vhd  para_up_vhd;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    para_up_vhd.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32));
    if (layer_id == SMART_RECT_LAYER_G3) {
        /* up */
        para_up_vhd.bits.para_up_vhd_chn07 = 0x1;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);
    } else if (layer_id == SMART_RECT_LAYER_G4) {
        /* up */
        para_up_vhd.bits.para_up_vhd_chn06 = 0x1;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);
    } else {
        HAL_PRINT("layer%u doesn't support clut\n", layer_id);
    }
}

#endif

#ifndef CONFIG_GFBG_SMART_RECT_V2
td_void hal_smart_rect_set_up(td_u32 layer_id, td_phys_addr_t phys_addr)
{
    volatile unsigned int para_haddr_vhd_chn05;
    volatile unsigned int para_addr_vhd_chn05;
    volatile u_osb_mute_bk osb_mute_bk;
    if (layer_id != SMART_RECT_LAYER_G3) {
        return;
    }
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    para_haddr_vhd_chn05 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn05));
    para_haddr_vhd_chn05 = ((td_u64)phys_addr >> 32) & 0xffffffff; /* 32 high bits */
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn05), para_haddr_vhd_chn05);
    para_addr_vhd_chn05 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn05));
    para_addr_vhd_chn05 = phys_addr & 0xffffffff;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn05), para_addr_vhd_chn05);

    /* enable OSB_MUTE_BK */
    osb_mute_bk.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->osb_mute_bk.u32));
    osb_mute_bk.bits.osb_mute_en = 0x1;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->osb_mute_bk.u32), osb_mute_bk.u32);
}
#else
td_void hal_smart_rect_set_up(td_u32 layer_id, td_phys_addr_t phys_addr)
{
    volatile unsigned int para_haddr_vhd_chn02;
    volatile unsigned int para_addr_vhd_chn02;
    volatile unsigned int para_haddr_vhd_chn03;
    volatile unsigned int para_addr_vhd_chn03;
    volatile u_g0_mute_bk g0_mute_bk;
    volatile u_g0_lbox_ctrl g0_lbox_ctrl;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    if (layer_id == SMART_RECT_LAYER_G3) {
        para_haddr_vhd_chn03 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn03));
        para_haddr_vhd_chn03 = ((td_u64)phys_addr >> 32) & 0xffffffff; /* 32 high bits */
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn03), para_haddr_vhd_chn03);
        para_addr_vhd_chn03 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn03));
        para_addr_vhd_chn03 = phys_addr & 0xffffffff;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn03), para_addr_vhd_chn03);
    } else if (layer_id == SMART_RECT_LAYER_G4) {
        para_haddr_vhd_chn02 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn02));
        para_haddr_vhd_chn02 = ((td_u64)phys_addr >> 32) & 0xffffffff; /* 32 high bits */
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_haddr_vhd_chn02), para_haddr_vhd_chn02);
        para_addr_vhd_chn02 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn02));
        para_addr_vhd_chn02 = phys_addr & 0xffffffff;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_addr_vhd_chn02), para_addr_vhd_chn02);
    }

    g0_mute_bk.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->g0_mute_bk.u32)) +
        layer_id * FDR_GFX_OFFSET));
    g0_mute_bk.u32 = 0x0;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->g0_mute_bk.u32)) + layer_id * FDR_GFX_OFFSET),
        g0_mute_bk.u32);

    g0_lbox_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->g0_lbox_ctrl.u32)) +
        layer_id * FDR_GFX_OFFSET));
    g0_lbox_ctrl.bits.mute_en = 0x1;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->g0_lbox_ctrl.u32)) + layer_id * FDR_GFX_OFFSET),
        g0_lbox_ctrl.u32);
    return;
}
#endif

td_void hal_smart_rect_up_param(td_u32 layer_id)
{
    volatile u_para_up_vhd  para_up_vhd;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
#ifndef CONFIG_GFBG_SMART_RECT_V2
    /* up */
    if (layer_id == SMART_RECT_LAYER_G3) {
        para_up_vhd.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32));
        para_up_vhd.bits.para_up_vhd_chn05 = 0x1;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);
    }
#else
    if (layer_id == SMART_RECT_LAYER_G3) {
        para_up_vhd.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32));
        para_up_vhd.bits.para_up_vhd_chn03 = 0x1;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);
    } else if (layer_id == SMART_RECT_LAYER_G4) {
        para_up_vhd.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32));
        para_up_vhd.bits.para_up_vhd_chn02 = 0x1;
        fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->para_up_vhd.u32), para_up_vhd.u32);
    }
#endif
    return;
}

#ifndef CONFIG_GFBG_SMART_RECT_V2
td_void hal_smart_rect_disable(td_u32 layer_id)
{
    volatile u_osb_mute_bk osb_mute_bk;
    ot_unused(layer_id);
    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    /* disable OSB_MUTE_BK */
    osb_mute_bk.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->osb_mute_bk.u32));
    osb_mute_bk.bits.osb_mute_en = 0x0;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->osb_mute_bk.u32), osb_mute_bk.u32);
}
#else
td_void hal_smart_rect_disable(td_u32 layer_id)
{
    volatile u_g0_lbox_ctrl g0_lbox_ctrl;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }
    g0_lbox_ctrl.u32 = fb_hal_read_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->g0_lbox_ctrl.u32)) +
        layer_id * FDR_GFX_OFFSET));
    g0_lbox_ctrl.bits.mute_en = 0x0;
    fb_hal_write_reg((td_u32 *)((uintptr_t)(&(g_gfbg_reg->g0_lbox_ctrl.u32)) + layer_id * FDR_GFX_OFFSET),
        g0_lbox_ctrl.u32);
    return;
}
#endif

td_void hal_gfx_set_bind_mode(td_bool is_sync)
{
    volatile u_gfx_mac_ctrl gfx_mac_ctrl;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_mac_ctrl.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->gfx_mac_ctrl));
    if (is_sync == TD_TRUE) {
        gfx_mac_ctrl.bits.req_ld_mode = 1; /* bind G0 */
    } else {
        gfx_mac_ctrl.bits.req_ld_mode = 0; /* not bind */
    }
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->gfx_mac_ctrl.u32), gfx_mac_ctrl.u32);
    return;
}

td_void hal_gfx_set_hardware_mute_clr(td_u32 hw_mute_clr)
{
    volatile u_gfx_ld_ctrl gfx_ld_ctrl;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_ld_ctrl.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->gfx_ld_ctrl));
    gfx_ld_ctrl.bits.hw_mute_clr = hw_mute_clr;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->gfx_ld_ctrl.u32), gfx_ld_ctrl.u32);
    return;
}

td_void hal_gfx_set_tde_safe_dis(td_u32 start_safe_dis)
{
    volatile unsigned int gfx_tde_safe_dis;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_tde_safe_dis = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->gfx_tde_safe_dis));
    gfx_tde_safe_dis = start_safe_dis;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->gfx_tde_safe_dis), gfx_tde_safe_dis);
    return;
}

td_void hal_gfx_set_ld_mute_en(td_u32 ld_mute_en)
{
    volatile u_gfx_ld_ctrl gfx_ld_ctrl;

    if (g_gfbg_reg == TD_NULL) {
        HAL_PRINT("NULL pointer %s: L%d\n", __FUNCTION__, __LINE__);
        return;
    }

    gfx_ld_ctrl.u32 = fb_hal_read_reg((td_u32 *)&(g_gfbg_reg->gfx_ld_ctrl));
    gfx_ld_ctrl.bits.ld_mute_en = ld_mute_en;
    fb_hal_write_reg((td_u32 *)&(g_gfbg_reg->gfx_ld_ctrl.u32), gfx_ld_ctrl.u32);
    return;
}


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_GRAPHIC_HAL_BODY_PART08_H_ */
