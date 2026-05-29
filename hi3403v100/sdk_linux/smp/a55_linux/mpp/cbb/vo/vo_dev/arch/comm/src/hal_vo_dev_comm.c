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

#include "hal_vo_dev_comm.h"
#include "drv_vo.h"
#include "hal_vo.h"
#include "hal_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */
#if vo_desc("UBOOT_VO")

#if vo_desc("dev&intf")

td_void hal_disp_set_intf_enable(ot_vo_dev dev, td_bool intf)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_ctrl dhd0_ctrl;
    volatile td_ulong addr_reg;

    vo_hal_check_phy_dev_return(dev);

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_ctrl.u32));
    dhd0_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    dhd0_ctrl.bits.intf_en = intf;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, dhd0_ctrl.u32);
}

td_void hal_disp_get_intf_enable(ot_vo_dev dev, td_bool *intf_en)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_ctrl dhd0_ctrl;
    volatile td_ulong addr_reg;

    vo_hal_check_phy_dev_return(dev);

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_ctrl.u32));
    dhd0_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    *intf_en = dhd0_ctrl.bits.intf_en;
}

td_void hal_disp_set_dev_multi_chn_en(ot_vo_dev dev, hal_multichn_en multi_chn_en)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_pwr dhd0_pwr;
    volatile td_ulong addr_reg;

    vo_hal_check_phy_dev_return(dev);

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_pwr.u32));
    dhd0_pwr.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    dhd0_pwr.bits.multichn_en = multi_chn_en;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, dhd0_pwr.u32);
}

td_void hal_disp_clear_int_status(td_u32 int_msk)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();

    /* read interrupt status */
    hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->vomskintsta.u32), int_msk);
}

td_void hal_intf_bt_set_dfir_en(td_u32 dfir_en)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_intf_bt_ctrl intf_bt_ctrl;

    intf_bt_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)&(vo_reg->intf_bt_ctrl.u32));
    intf_bt_ctrl.bits.dfir_en = dfir_en;
    hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->intf_bt_ctrl.u32), intf_bt_ctrl.u32);
}

td_void hal_intf_bt_set_data_width(td_u32 data_width)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_intf_bt_ctrl intf_bt_ctrl;

    intf_bt_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)&(vo_reg->intf_bt_ctrl.u32));
    intf_bt_ctrl.bits.data_width = data_width;
    hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->intf_bt_ctrl.u32), intf_bt_ctrl.u32);
}

td_void hal_disp_set_reg_up(ot_vo_dev dev)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_ctrl dhd0_ctrl;
    volatile td_ulong addr_reg;

    vo_hal_check_phy_dev_return(dev);

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_ctrl.u32));
    dhd0_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    dhd0_ctrl.bits.regup = 0x1;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, dhd0_ctrl.u32);
}

td_u32 hal_disp_get_reg_up(ot_vo_dev dev)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile td_u32 data;
    volatile reg_dhd0_ctrl dhd0_ctrl;
    volatile td_ulong addr_reg;

    if (vo_hal_is_phy_dev(dev) != TD_TRUE) {
        return 0;
    }

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_ctrl.u32));
    dhd0_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    data = dhd0_ctrl.bits.regup;
    return data & 0x1;
}

#endif

#if vo_desc("dev intf")

td_void vo_hal_intf_set_bt_sync_inv(const hal_disp_syncinv *inv)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_intf_bt_sync_inv intf_bt_sync_inv;

    intf_bt_sync_inv.u32 = vo_reg->intf_bt_sync_inv.u32;
    intf_bt_sync_inv.bits.dv_inv = inv->dv_inv;
    intf_bt_sync_inv.bits.hs_inv = inv->hs_inv;
    intf_bt_sync_inv.bits.vs_inv = inv->vs_inv;
    intf_bt_sync_inv.bits.f_inv = inv->f_inv;
    vo_reg->intf_bt_sync_inv.u32 = intf_bt_sync_inv.u32;
}

td_void vo_hal_intf_set_cvbs_sync_inv(const hal_disp_syncinv *inv)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_intf_date_sync_inv intf_date_sync_inv;

    intf_date_sync_inv.u32 = vo_reg->intf_date_sync_inv.u32;
    intf_date_sync_inv.bits.dv_inv = inv->dv_inv;
    intf_date_sync_inv.bits.hs_inv = inv->hs_inv;
    intf_date_sync_inv.bits.vs_inv = inv->vs_inv;
    intf_date_sync_inv.bits.f_inv = inv->f_inv;
    vo_reg->intf_date_sync_inv.u32 = intf_date_sync_inv.u32;
}

td_void vo_hal_intf_set_hdmi_sync_inv(const hal_disp_syncinv *inv)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_intf_hdmi_sync_inv intf_hdmi_sync_inv;

    intf_hdmi_sync_inv.u32 = vo_reg->intf_hdmi_sync_inv.u32;
    intf_hdmi_sync_inv.bits.dv_inv = inv->dv_inv;
    intf_hdmi_sync_inv.bits.hs_inv = inv->hs_inv;
    intf_hdmi_sync_inv.bits.vs_inv = inv->vs_inv;
    intf_hdmi_sync_inv.bits.f_inv = inv->f_inv;
    vo_reg->intf_hdmi_sync_inv.u32 = intf_hdmi_sync_inv.u32;
}

td_void vo_hal_intf_set_hdmi_csc_dc_coef(const vdp_csc_dc_coef *csc_dc_coef)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_hdmi_csc_idc hdmi_csc_idc;
    volatile reg_hdmi_csc_odc hdmi_csc_odc;
    volatile reg_hdmi_csc_iodc hdmi_csc_iodc;

    hdmi_csc_idc.u32 = vo_reg->hdmi_csc_idc.u32;
    hdmi_csc_odc.u32 = vo_reg->hdmi_csc_odc.u32;
    hdmi_csc_iodc.u32 = vo_reg->hdmi_csc_iodc.u32;

	/* the configuration is reversed. */
    hdmi_csc_idc.bits.cscidc0 = csc_dc_coef->csc_in_dc2;
    hdmi_csc_idc.bits.cscidc1 = csc_dc_coef->csc_in_dc1;
    hdmi_csc_iodc.bits.cscidc2 = csc_dc_coef->csc_in_dc0;

    hdmi_csc_odc.bits.cscodc0 = csc_dc_coef->csc_out_dc2;

    hdmi_csc_odc.bits.cscodc1 = csc_dc_coef->csc_out_dc1;
    hdmi_csc_iodc.bits.cscodc2 = csc_dc_coef->csc_out_dc0;

    vo_reg->hdmi_csc_idc.u32 = hdmi_csc_idc.u32;
    vo_reg->hdmi_csc_odc.u32 = hdmi_csc_odc.u32;
    vo_reg->hdmi_csc_iodc.u32 = hdmi_csc_iodc.u32;
}

td_void vo_hal_intf_set_hdmi_csc_coef(const vdp_csc_coef *coef)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_hdmi_csc_p0 hdmi_csc_p0;
    volatile reg_hdmi_csc_p1 hdmi_csc_p1;
    volatile reg_hdmi_csc_p2 hdmi_csc_p2;
    volatile reg_hdmi_csc_p3 hdmi_csc_p3;
    volatile reg_hdmi_csc_p4 hdmi_csc_p4;

    hdmi_csc_p0.u32 = vo_reg->hdmi_csc_p0.u32;
    hdmi_csc_p1.u32 = vo_reg->hdmi_csc_p1.u32;
    hdmi_csc_p2.u32 = vo_reg->hdmi_csc_p2.u32;
    hdmi_csc_p3.u32 = vo_reg->hdmi_csc_p3.u32;
    hdmi_csc_p4.u32 = vo_reg->hdmi_csc_p4.u32;

    hdmi_csc_p0.bits.cscp00 = coef->csc_coef00;
    hdmi_csc_p0.bits.cscp01 = coef->csc_coef01;
    hdmi_csc_p1.bits.cscp02 = coef->csc_coef02;
    hdmi_csc_p1.bits.cscp10 = coef->csc_coef10;
    hdmi_csc_p2.bits.cscp11 = coef->csc_coef11;
    hdmi_csc_p2.bits.cscp12 = coef->csc_coef12;
    hdmi_csc_p3.bits.cscp20 = coef->csc_coef20;
    hdmi_csc_p3.bits.cscp21 = coef->csc_coef21;
    hdmi_csc_p4.bits.cscp22 = coef->csc_coef22;

    vo_reg->hdmi_csc_p0.u32 = hdmi_csc_p0.u32;
    vo_reg->hdmi_csc_p1.u32 = hdmi_csc_p1.u32;
    vo_reg->hdmi_csc_p2.u32 = hdmi_csc_p2.u32;
    vo_reg->hdmi_csc_p3.u32 = hdmi_csc_p3.u32;
    vo_reg->hdmi_csc_p4.u32 = hdmi_csc_p4.u32;
}

td_void vo_hal_intf_set_dac_sel(ot_vo_intf_type intf_type)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vomux_dac vomux_dac;

    vomux_dac.u32 = vo_reg->vomux_dac.u32;
    if (intf_type == OT_VO_INTF_VGA) {
        vomux_dac.bits.dac2_sel = 9;   /* 9: DAC2 VGA B */
        vomux_dac.bits.dac1_sel = 8;   /* 8: DAC2 VGA G */
        vomux_dac.bits.dac0_sel = 7;   /* 7: DAC2 VGA R */
    } else {
        vomux_dac.bits.dac3_sel = 0;   /* dac3: CVBS */
    }
    vo_reg->vomux_dac.u32 = vomux_dac.u32;
}

td_void vo_hal_intf_set_vga_and_cvbs_dac_enable(td_bool enable)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vo_dac_ctrl vo_dac_ctrl;

    vo_dac_ctrl.u32 = vo_reg->vo_dac_ctrl.u32;
    vo_dac_ctrl.bits.envbg = enable;
    vo_reg->vo_dac_ctrl.u32 = vo_dac_ctrl.u32;
}

td_void vo_hal_intf_set_dac_chn_enable(ot_vo_intf_type intf_type, td_bool enable)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vo_dac0_ctrl vo_dac0_ctrl;
    volatile reg_vo_dac1_ctrl vo_dac1_ctrl;
    volatile reg_vo_dac2_ctrl vo_dac2_ctrl;
    volatile reg_vo_dac3_ctrl vo_dac3_ctrl;

    if (intf_type == OT_VO_INTF_VGA) {
        vo_dac0_ctrl.u32 = vo_reg->vo_dac0_ctrl.u32;
        vo_dac1_ctrl.u32 = vo_reg->vo_dac1_ctrl.u32;
        vo_dac2_ctrl.u32 = vo_reg->vo_dac2_ctrl.u32;
        vo_dac0_ctrl.bits.dac_en = enable;
        vo_dac1_ctrl.bits.dac_en = enable;
        vo_dac2_ctrl.bits.dac_en = enable;
        vo_reg->vo_dac0_ctrl.u32 = vo_dac0_ctrl.u32;
        vo_reg->vo_dac1_ctrl.u32 = vo_dac1_ctrl.u32;
        vo_reg->vo_dac2_ctrl.u32 = vo_dac2_ctrl.u32;
    } else {    /* dac3: CVBS */
        vo_dac3_ctrl.u32 = vo_reg->vo_dac3_ctrl.u32;
        vo_dac3_ctrl.bits.dac_en = enable;
        vo_reg->vo_dac3_ctrl.u32 = vo_dac3_ctrl.u32;
    }
}

td_void vo_hal_intf_set_dac_gc(ot_vo_intf_type intf_type, td_u32 dac_gc)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vo_dac0_ctrl vo_dac0_ctrl;
    volatile reg_vo_dac1_ctrl vo_dac1_ctrl;
    volatile reg_vo_dac2_ctrl vo_dac2_ctrl;
    volatile reg_vo_dac3_ctrl vo_dac3_ctrl;

    if (intf_type == OT_VO_INTF_VGA) {
        vo_dac0_ctrl.u32 = vo_reg->vo_dac0_ctrl.u32;
        vo_dac1_ctrl.u32 = vo_reg->vo_dac1_ctrl.u32;
        vo_dac2_ctrl.u32 = vo_reg->vo_dac2_ctrl.u32;
        vo_dac0_ctrl.bits.dacgc = dac_gc;
        vo_dac1_ctrl.bits.dacgc = dac_gc;
        vo_dac2_ctrl.bits.dacgc = dac_gc;
        vo_reg->vo_dac0_ctrl.u32 = vo_dac0_ctrl.u32;
        vo_reg->vo_dac1_ctrl.u32 = vo_dac1_ctrl.u32;
        vo_reg->vo_dac2_ctrl.u32 = vo_dac2_ctrl.u32;
    } else {    /* dac3: CVBS */
        vo_dac3_ctrl.u32 = vo_reg->vo_dac3_ctrl.u32;
        vo_dac3_ctrl.bits.dacgc = dac_gc;
        vo_reg->vo_dac3_ctrl.u32 = vo_dac3_ctrl.u32;
    }
}

td_void vo_hal_intf_set_dac_cablectr(ot_vo_intf_type intf_type, td_u32 cablectr)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vo_dac0_ctrl vo_dac0_ctrl;
    volatile reg_vo_dac1_ctrl vo_dac1_ctrl;
    volatile reg_vo_dac2_ctrl vo_dac2_ctrl;
    volatile reg_vo_dac3_ctrl vo_dac3_ctrl;

    if (intf_type == OT_VO_INTF_VGA) {
        vo_dac0_ctrl.u32 = vo_reg->vo_dac0_ctrl.u32;
        vo_dac1_ctrl.u32 = vo_reg->vo_dac1_ctrl.u32;
        vo_dac2_ctrl.u32 = vo_reg->vo_dac2_ctrl.u32;
        vo_dac0_ctrl.bits.cablectr = cablectr;
        vo_dac1_ctrl.bits.cablectr = cablectr;
        vo_dac2_ctrl.bits.cablectr = cablectr;
        vo_reg->vo_dac0_ctrl.u32 = vo_dac0_ctrl.u32;
        vo_reg->vo_dac1_ctrl.u32 = vo_dac1_ctrl.u32;
        vo_reg->vo_dac2_ctrl.u32 = vo_dac2_ctrl.u32;
    } else {    /* dac3: CVBS */
        vo_dac3_ctrl.u32 = vo_reg->vo_dac3_ctrl.u32;
        vo_dac3_ctrl.bits.cablectr = cablectr;
        vo_reg->vo_dac3_ctrl.u32 = vo_dac3_ctrl.u32;
    }
}

#ifdef CONFIG_OT_VO_VGA

td_void vo_hal_intf_set_vga_sync_inv(const hal_disp_syncinv *inv)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_intf_vga_sync_inv intf_vga_sync_inv;

    intf_vga_sync_inv.u32 = vo_reg->intf_vga_sync_inv.u32;
    intf_vga_sync_inv.bits.dv_inv = inv->dv_inv;
    intf_vga_sync_inv.bits.hs_inv = inv->hs_inv;
    intf_vga_sync_inv.bits.vs_inv = inv->vs_inv;
    intf_vga_sync_inv.bits.f_inv = inv->f_inv;
    vo_reg->intf_vga_sync_inv.u32 = intf_vga_sync_inv.u32;
}

td_void vo_hal_intf_set_vga_csc_dc_coef(const vdp_csc_dc_coef *csc_dc_coef)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vga_csc_idc vga_csc_idc;
    volatile reg_vga_csc_odc vga_csc_odc;
    volatile reg_vga_csc_iodc vga_csc_iodc;

    vga_csc_idc.u32 = vo_reg->vga_csc_idc.u32;
    vga_csc_odc.u32 = vo_reg->vga_csc_odc.u32;
    vga_csc_iodc.u32 = vo_reg->vga_csc_iodc.u32;

    /* the configuration is reversed. */
    vga_csc_idc.bits.cscidc0 = csc_dc_coef->csc_in_dc2;
    vga_csc_idc.bits.cscidc1 = csc_dc_coef->csc_in_dc1;
    vga_csc_iodc.bits.cscidc2 = csc_dc_coef->csc_in_dc0;
    vga_csc_odc.bits.cscodc0 = csc_dc_coef->csc_out_dc2;
    vga_csc_odc.bits.cscodc1 = csc_dc_coef->csc_out_dc1;
    vga_csc_iodc.bits.cscodc2 = csc_dc_coef->csc_out_dc0;

    vo_reg->vga_csc_idc.u32 = vga_csc_idc.u32;
    vo_reg->vga_csc_odc.u32 = vga_csc_odc.u32;
    vo_reg->vga_csc_iodc.u32 = vga_csc_iodc.u32;
}

td_void vo_hal_intf_set_vga_csc_coef(const vdp_csc_coef *coef)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vga_csc_p0 vga_csc_p0;
    volatile reg_vga_csc_p1 vga_csc_p1;
    volatile reg_vga_csc_p2 vga_csc_p2;
    volatile reg_vga_csc_p3 vga_csc_p3;
    volatile reg_vga_csc_p4 vga_csc_p4;

    vga_csc_p0.u32 = vo_reg->vga_csc_p0.u32;
    vga_csc_p1.u32 = vo_reg->vga_csc_p1.u32;
    vga_csc_p2.u32 = vo_reg->vga_csc_p2.u32;
    vga_csc_p3.u32 = vo_reg->vga_csc_p3.u32;
    vga_csc_p4.u32 = vo_reg->vga_csc_p4.u32;

    vga_csc_p0.bits.cscp00 = coef->csc_coef00;
    vga_csc_p0.bits.cscp01 = coef->csc_coef01;
    vga_csc_p1.bits.cscp02 = coef->csc_coef02;
    vga_csc_p1.bits.cscp10 = coef->csc_coef10;
    vga_csc_p2.bits.cscp11 = coef->csc_coef11;
    vga_csc_p2.bits.cscp12 = coef->csc_coef12;
    vga_csc_p3.bits.cscp20 = coef->csc_coef20;
    vga_csc_p3.bits.cscp21 = coef->csc_coef21;
    vga_csc_p4.bits.cscp22 = coef->csc_coef22;

    vo_reg->vga_csc_p0.u32 = vga_csc_p0.u32;
    vo_reg->vga_csc_p1.u32 = vga_csc_p1.u32;
    vo_reg->vga_csc_p2.u32 = vga_csc_p2.u32;
    vo_reg->vga_csc_p3.u32 = vga_csc_p3.u32;
    vo_reg->vga_csc_p4.u32 = vga_csc_p4.u32;
}

static td_void vo_hal_intf_set_vga_hsp_coef(const hsp_coef *coef)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vga_hspcfg14 vga_hspcfg14;
    volatile reg_vga_hspcfg15 vga_hspcfg15;

    vga_hspcfg14.u32 = vo_reg->vga_hspcfg14.u32;
    vga_hspcfg15.u32 = vo_reg->vga_hspcfg15.u32;

    vga_hspcfg14.bits.hsp_cdti_gain = coef->vga_hsp_cdti_gain;
    vga_hspcfg14.bits.hsp_ldti_gain = coef->vga_hsp_ldti_gain;
    vga_hspcfg14.bits.hsp_lti_ratio = coef->vga_hsp_lti_ratio;
    vga_hspcfg14.bits.hsp_hf_shootdiv = coef->vga_hsp_hf_shootdiv;
    vga_hspcfg15.bits.hsp_glb_underth = coef->vga_hsp_glb_underth;
    vga_hspcfg15.bits.hsp_glb_overth = coef->vga_hsp_glb_overth;
    vga_hspcfg15.bits.hsp_peak_ratio = coef->vga_hsp_peak_ratio;

    vo_reg->vga_hspcfg14.u32 = vga_hspcfg14.u32;
    vo_reg->vga_hspcfg15.u32 = vga_hspcfg15.u32;
}

static td_void vo_hal_intf_set_vga_hsp_h0_coef(const hsp_hf_coef *coef0)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vga_hspcfg0 vga_hspcfg0;
    volatile reg_vga_hspcfg1 vga_hspcfg1;
    volatile reg_vga_hspcfg5 vga_hspcfg5;
    volatile reg_vga_hspcfg6 vga_hspcfg6;

    vga_hspcfg0.u32 = vo_reg->vga_hspcfg0.u32;
    vga_hspcfg1.u32 = vo_reg->vga_hspcfg1.u32;
    vga_hspcfg5.u32 = vo_reg->vga_hspcfg5.u32;
    vga_hspcfg6.u32 = vo_reg->vga_hspcfg6.u32;

    vga_hspcfg0.bits.hsp_hf0_tmp0 = coef0->vga_hsp_tmp0;
    vga_hspcfg0.bits.hsp_hf0_tmp1 = coef0->vga_hsp_tmp1;
    vga_hspcfg0.bits.hsp_hf0_tmp2 = coef0->vga_hsp_tmp2;
    vga_hspcfg0.bits.hsp_hf0_tmp3 = coef0->vga_hsp_tmp3;
    vga_hspcfg1.bits.hsp_hf0_coring = coef0->vga_hsp_coring;
    vga_hspcfg5.bits.hsp_hf0_gainpos = coef0->vga_hsp_gainpos;
    vga_hspcfg5.bits.hsp_hf0_gainneg = coef0->vga_hsp_gainneg;
    vga_hspcfg6.bits.hsp_hf0_overth = coef0->vga_hsp_overth;
    vga_hspcfg6.bits.hsp_hf0_underth = coef0->vga_hsp_underth;
    vga_hspcfg6.bits.hsp_hf0_mixratio = coef0->vga_hsp_mixratio;
    vga_hspcfg6.bits.hsp_hf0_winsize = coef0->vga_hsp_winsize;
    vga_hspcfg6.bits.hsp_hf0_adpshoot_en = coef0->vga_hsp_adpshooten;

    vo_reg->vga_hspcfg0.u32 = vga_hspcfg0.u32;
    vo_reg->vga_hspcfg1.u32 = vga_hspcfg1.u32;
    vo_reg->vga_hspcfg5.u32 = vga_hspcfg5.u32;
    vo_reg->vga_hspcfg6.u32 = vga_hspcfg6.u32;
}

static td_void vo_hal_intf_set_vga_hsp_h1_coef(const hsp_hf_coef *coef1)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vga_hspcfg7 vga_hspcfg7;
    volatile reg_vga_hspcfg8 vga_hspcfg8;
    volatile reg_vga_hspcfg12 vga_hspcfg12;
    volatile reg_vga_hspcfg13 vga_hspcfg13;

    vga_hspcfg7.u32 = vo_reg->vga_hspcfg7.u32;
    vga_hspcfg8.u32 = vo_reg->vga_hspcfg8.u32;
    vga_hspcfg12.u32 = vo_reg->vga_hspcfg12.u32;
    vga_hspcfg13.u32 = vo_reg->vga_hspcfg13.u32;

    vga_hspcfg7.bits.hsp_hf1_tmp0 = coef1->vga_hsp_tmp0;
    vga_hspcfg7.bits.hsp_hf1_tmp1 = coef1->vga_hsp_tmp1;
    vga_hspcfg7.bits.hsp_hf1_tmp2 = coef1->vga_hsp_tmp2;
    vga_hspcfg7.bits.hsp_hf1_tmp3 = coef1->vga_hsp_tmp3;
    vga_hspcfg8.bits.hsp_hf1_coring = coef1->vga_hsp_coring;
    vga_hspcfg12.bits.hsp_hf1_gainpos = coef1->vga_hsp_gainpos;
    vga_hspcfg12.bits.hsp_hf1_gainneg = coef1->vga_hsp_gainneg;
    vga_hspcfg13.bits.hsp_hf1_overth = coef1->vga_hsp_overth;
    vga_hspcfg13.bits.hsp_hf1_underth = coef1->vga_hsp_underth;
    vga_hspcfg13.bits.hsp_hf1_mixratio = coef1->vga_hsp_mixratio;
    vga_hspcfg13.bits.hsp_hf1_winsize = coef1->vga_hsp_winsize;
    vga_hspcfg13.bits.hsp_hf1_adpshoot_en = coef1->vga_hsp_adpshooten;

    vo_reg->vga_hspcfg7.u32 = vga_hspcfg7.u32;
    vo_reg->vga_hspcfg8.u32 = vga_hspcfg8.u32;
    vo_reg->vga_hspcfg12.u32 = vga_hspcfg12.u32;
    vo_reg->vga_hspcfg13.u32 = vga_hspcfg13.u32;
}

static td_void vo_hal_intf_set_vga_hsp_h_enable(td_bool enable)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vga_hspcfg14 vga_hspcfg14;

    vga_hspcfg14.u32 = vo_reg->vga_hspcfg14.u32;
    vga_hspcfg14.bits.hsp_h0_en = enable;
    vga_hspcfg14.bits.hsp_h1_en = enable;
    vga_hspcfg14.bits.hsp_ltih_en = enable;
    vga_hspcfg14.bits.hsp_ctih_en = enable;
    vo_reg->vga_hspcfg14.u32 = vga_hspcfg14.u32;
}

static td_void vo_hal_intf_set_vga_hsp_enable(td_bool enable)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vga_hspcfg1 vga_hspcfg1;

    vga_hspcfg1.u32 = vo_reg->vga_hspcfg1.u32;
    vga_hspcfg1.bits.hsp_en = enable;
    vo_reg->vga_hspcfg1.u32 = vga_hspcfg1.u32;
}

static td_void vo_hal_intf_set_vga_hsp_cfg_coef(td_u32 strength)
{
    const td_u32 pre = 8;  /* 8: offset */
    hsp_coef coef = {0};

    coef.vga_hsp_peak_ratio = 128; /* 128: peak_ratio */
    coef.vga_hsp_glb_overth = 50;  /* 50: glb_overth */
    coef.vga_hsp_hf_shootdiv = 4;  /* 4: hf_shootdiv */
    coef.vga_hsp_lti_ratio = 128;  /* 128: lti_ratio */
    coef.vga_hsp_ldti_gain = (192 * strength) >> pre;  /* 192: hsp_ldti_gain */
    coef.vga_hsp_cdti_gain = (192 * strength) >> pre;  /* 192: hsp_cdti_gain */
    coef.vga_hsp_glb_underth = 255;  /* 255: glb_underth */

    vo_hal_intf_set_vga_hsp_coef(&coef);
}

static td_void vo_hal_intf_set_vga_hsp_cfg_coef0(td_u32 strength)
{
    const td_u32 pre = 8; /* 8: offset */
    hsp_hf_coef coef0 = {0};

    coef0.vga_hsp_tmp0 = 0;
    coef0.vga_hsp_tmp1 = -84; /* -84: coef 1 */
    coef0.vga_hsp_tmp2 = 0;
    coef0.vga_hsp_tmp3 = 20;   /* 20: coef 3 */
    coef0.vga_hsp_coring = 2;  /* 2: coef coring */
    coef0.vga_hsp_gainpos = (strength <= VO_VGA_SHARPEN_STRENGTH_DEF) ? ((154 * strength) >> pre) /* 154:coef */
        : (2 * (strength - VO_VGA_SHARPEN_STRENGTH_DEF) + 77); /* 2:coef 77:coef */
    coef0.vga_hsp_gainneg = (strength <= VO_VGA_SHARPEN_STRENGTH_DEF) ? ((154 * strength) >> pre) /* 154:coef */
        : (2 * (strength - VO_VGA_SHARPEN_STRENGTH_DEF) + 77); /* 2:coef 77:coef */
    coef0.vga_hsp_adpshooten = 1; /* 1: enable */
    coef0.vga_hsp_winsize    = 2; /* 2: window size */
    coef0.vga_hsp_mixratio   = 64; /* 64: 0.5 * 128 */
    coef0.vga_hsp_underth    = (strength <= VO_VGA_SHARPEN_STRENGTH_DEF) ? ((36 * strength) >> pre) /* 36:coef */
        : ((72 * ((strength - VO_VGA_SHARPEN_STRENGTH_DEF) >> pre)) +  18);  /* 72:coef 18:coef */
    coef0.vga_hsp_overth     = (strength <= VO_VGA_SHARPEN_STRENGTH_DEF) ? ((24 * strength) >> pre) /* 24:coef */
        : ((48 * ((strength - VO_VGA_SHARPEN_STRENGTH_DEF) >> pre)) +  12);  /* 48:coef 12:coef */

    vo_hal_intf_set_vga_hsp_h0_coef(&coef0);
}

static td_void vo_hal_intf_set_vga_hsp_cfg_coef1(td_u32 strength)
{
    const td_u32 pre = 8;  /* 8: offset */
    hsp_hf_coef coef1 = {0};

    coef1.vga_hsp_tmp0 = 22;    /* 22: coef 0 */
    coef1.vga_hsp_tmp1 = -62;   /* -62: coef 1 */
    coef1.vga_hsp_tmp2 = -25;   /* -25: coef 2 */
    coef1.vga_hsp_tmp3 = 1;     /* 1: coef 3 */
    coef1.vga_hsp_coring = 2; /* 2: coef coring */
    coef1.vga_hsp_gainpos = (strength <= VO_VGA_SHARPEN_STRENGTH_DEF) ? ((214 * strength) >> pre) /* 214:coef */
        : ((712 * ((strength - VO_VGA_SHARPEN_STRENGTH_DEF) >> pre)) + 107); /* 712:coef 107:coef */
    coef1.vga_hsp_gainneg = (strength <= VO_VGA_SHARPEN_STRENGTH_DEF) ? ((214 * strength) >> pre) /* 214:coef */
        : ((712 * ((strength - VO_VGA_SHARPEN_STRENGTH_DEF) >> pre)) + 107); /* 712:coef 107:coef */
    coef1.vga_hsp_adpshooten = 1;  /* 1: enable */
    coef1.vga_hsp_winsize    = 3;  /* 3: window size */
    coef1.vga_hsp_mixratio   = 102; /* 102:: 0.8 * 128 */
    coef1.vga_hsp_underth    = (strength <= VO_VGA_SHARPEN_STRENGTH_DEF) ? ((36 * strength) >> pre) /* 36:coef */
        : ((72 * ((strength - VO_VGA_SHARPEN_STRENGTH_DEF) >> pre)) +  18);  /* 72:coef 18:coef */
    coef1.vga_hsp_overth     = (strength <= VO_VGA_SHARPEN_STRENGTH_DEF) ? ((30 * strength) >> pre) /* 30:coef */
        : ((60 * ((strength - VO_VGA_SHARPEN_STRENGTH_DEF) >> pre)) +  15);  /* 60:coef 15:coef */

    vo_hal_intf_set_vga_hsp_h1_coef(&coef1);
}

td_void vo_hal_intf_set_vga_hsp_cfg(td_bool enable, td_u32 strength)
{
    vo_hal_intf_set_vga_hsp_cfg_coef(strength);
    vo_hal_intf_set_vga_hsp_cfg_coef0(strength);
    vo_hal_intf_set_vga_hsp_cfg_coef1(strength);
    vo_hal_intf_set_vga_hsp_h_enable(enable);
    vo_hal_intf_set_vga_hsp_enable(enable);
}

#endif

static td_void vo_hal_intf_set_date_coeff(td_u32 data)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    vo_reg->date_coeff0.u32 = data;
}

static td_void vo_hal_intf_set_date_cgain(td_u32 data)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    vo_reg->date_coeff1.u32 = data;
}

static td_void vo_hal_intf_set_date_out_ctrl(td_u32 data)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_date_coeff21 date_coeff21;
    volatile reg_date_coeff21 input_data;

    date_coeff21.u32 = vo_reg->date_coeff21.u32;
    input_data.u32 = data;

    date_coeff21.bits.dac0_in_sel = input_data.bits.dac0_in_sel;
    date_coeff21.bits.dac1_in_sel = input_data.bits.dac1_in_sel;
    date_coeff21.bits.dac2_in_sel = input_data.bits.dac2_in_sel;
    date_coeff21.bits.dac3_in_sel = input_data.bits.dac3_in_sel;
    date_coeff21.bits.dac4_in_sel = input_data.bits.dac4_in_sel;
    date_coeff21.bits.dac5_in_sel = input_data.bits.dac5_in_sel;

    vo_reg->date_coeff21.u32 = date_coeff21.u32;
}

static td_void vo_hal_intf_set_date_delta(td_u32 data)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    vo_reg->date_coeff22.u32 = data;
}

td_void vo_hal_intf_set_date_cvbs_color_burst(td_void)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    vo_reg->date_coeff24 = 0x12099; /* 0x12099: color burst start position. */
}

static td_u32 vo_hal_intf_get_date_user(td_u32 vact)
{
    if (vact == 288) { /* 288 = 576 / 2 */
        return VO_CVBS_DATE_PAL;
    } else if (vact == 240) { /* 240 = 480 / 2 */
        return VO_CVBS_DATE_NTSC;
    } else {
        vo_err_trace("user sync wrong vact: %u\n", vact);
        return VO_CVBS_DATE_NTSC;
    }
}

td_u32 vo_hal_intf_get_date(ot_vo_intf_sync intf_sync, td_u32 vact)
{
    if (intf_sync == OT_VO_OUT_PAL) {
        return VO_CVBS_DATE_PAL;
    } else if (intf_sync == OT_VO_OUT_NTSC) {
        return VO_CVBS_DATE_NTSC;
    }
    return vo_hal_intf_get_date_user(vact);
}

td_void vo_hal_intf_set_date_cfg(ot_vo_intf_sync intf_sync, td_u32 date, td_u32 pal_cvbs_gain, td_u32 ntsc_cvbs_gain)
{
    vo_hal_intf_set_date_coeff(date);
    vo_hal_intf_set_date_cgain(0x20000000);   /* 0x20000000: data cgain */
    vo_hal_intf_set_date_out_ctrl(0x111111);  /* 0x111111: data out ctrl */
    vo_hal_intf_set_date_delta(0xa4);         /* 0xa4: data delta */

    if (intf_sync == OT_VO_OUT_NTSC) {
        vo_hal_intf_set_date_cvbs_gain(ntsc_cvbs_gain);
        vo_hal_intf_set_date_cvbs_color_burst();
        vo_hal_intf_set_date_cvbs_burst_start();
    } else {
        vo_hal_intf_set_date_cvbs_gain(pal_cvbs_gain);
    }
}

td_void vo_hal_intf_set_clip_cfg(ot_vo_intf_type intf_type, td_bool clip, const hal_disp_clip *clip_data)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_bt_clip0_l bt_clip0_l;
    volatile reg_bt_clip0_h bt_clip0_h;
    volatile reg_date_clip0_l date_clip0_l;
    volatile reg_date_clip0_h date_clip0_h;

    if ((intf_type == OT_VO_INTF_BT1120) || (intf_type == OT_VO_INTF_BT656)) {
        bt_clip0_l.u32 = vo_reg->bt_clip0_l.u32;
        bt_clip0_h.u32 = vo_reg->bt_clip0_h.u32;
        bt_clip0_l.bits.clip_en = clip;
        bt_clip0_l.bits.clip_cl2 = clip_data->clip_low_y;
        bt_clip0_l.bits.clip_cl1 = clip_data->clip_low_cb;
        bt_clip0_l.bits.clip_cl0 = clip_data->clip_low_cr;
        bt_clip0_h.bits.clip_ch2 = clip_data->clip_high_y;
        bt_clip0_h.bits.clip_ch1 = clip_data->clip_high_cb;
        bt_clip0_h.bits.clip_ch0 = clip_data->clip_high_cr;
        vo_reg->bt_clip0_l.u32 = bt_clip0_l.u32;
        vo_reg->bt_clip0_h.u32 = bt_clip0_h.u32;
    } else {    /* date_clip0: CVBS */
        date_clip0_l.u32 = vo_reg->date_clip0_l.u32;
        date_clip0_h.u32 = vo_reg->date_clip0_h.u32;
        date_clip0_l.bits.clip_en = clip;
        date_clip0_l.bits.clip_cl2 = clip_data->clip_low_y;
        date_clip0_l.bits.clip_cl1 = clip_data->clip_low_cb;
        date_clip0_l.bits.clip_cl0 = clip_data->clip_low_cr;
        date_clip0_h.bits.clip_ch2 = clip_data->clip_high_y;
        date_clip0_h.bits.clip_ch1 = clip_data->clip_high_cb;
        date_clip0_h.bits.clip_ch0 = clip_data->clip_high_cr;
        vo_reg->date_clip0_l.u32 = date_clip0_l.u32;
        vo_reg->date_clip0_h.u32 = date_clip0_h.u32;
    }
}

static td_void vo_hal_intf_set_sync_info_hvsync(ot_vo_dev dev,
    const hal_disp_syncinfo *sync_info)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_hsync1 dhd0_hsync1;
    volatile reg_dhd0_hsync2 dhd0_hsync2;
    volatile reg_dhd0_vsync1 dhd0_vsync1;
    volatile reg_dhd0_vsync2 dhd0_vsync2;
    volatile td_ulong addr_reg;

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_hsync1.u32));
    dhd0_hsync1.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    dhd0_hsync1.bits.hact = sync_info->hact - 1;
    dhd0_hsync1.bits.hbb = (sync_info->hbb) - 1;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, dhd0_hsync1.u32);

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_hsync2.u32));
    dhd0_hsync2.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    dhd0_hsync2.bits.hmid = (sync_info->hmid == 0) ? 0 : (sync_info->hmid - 1);
    dhd0_hsync2.bits.hfb = (sync_info->hfb) - 1;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, dhd0_hsync2.u32);

    /* config VHD interface vertical timing */
    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_vsync1.u32));
    dhd0_vsync1.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    dhd0_vsync1.bits.vact = sync_info->vact - 1;
    dhd0_vsync1.bits.vbb = sync_info->vbb - 1;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, dhd0_vsync1.u32);

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_vsync2.u32));
    dhd0_vsync2.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    dhd0_vsync2.bits.vfb = sync_info->vfb - 1;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, dhd0_vsync2.u32);
}

static td_void vo_hal_intf_set_sync_info_other(ot_vo_dev dev,
    const hal_disp_syncinfo *sync_info)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_ctrl dhd0_ctrl;
    volatile reg_dhd0_vplus1 dhd0_vplus1;
    volatile reg_dhd0_vplus2 dhd0_vplus2;
    volatile reg_dhd0_pwr dhd0_pwr;
    volatile td_ulong addr_reg;

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_ctrl.u32));
    dhd0_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    dhd0_ctrl.bits.iop = sync_info->iop;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, dhd0_ctrl.u32);

    /* config VHD interface vertical bottom timing, no use in progressive mode */
    addr_reg = vou_get_chn_abs_addr(dev, (td_ulong)(td_uintptr_t)&(vo_reg->dhd0_vplus1.u32));
    dhd0_vplus1.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    dhd0_vplus1.bits.bvact = sync_info->bvact - 1;
    dhd0_vplus1.bits.bvbb = sync_info->bvbb - 1;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, dhd0_vplus1.u32);

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_vplus2.u32));
    dhd0_vplus2.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    dhd0_vplus2.bits.bvfb = sync_info->bvfb - 1;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, dhd0_vplus2.u32);

    /* config VHD interface vertical bottom timing, */
    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_pwr.u32));
    dhd0_pwr.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    dhd0_pwr.bits.hpw = sync_info->hpw - 1;
    dhd0_pwr.bits.vpw = sync_info->vpw - 1;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, dhd0_pwr.u32);
}

td_void vo_hal_intf_set_sync_info(ot_vo_dev dev, const hal_disp_syncinfo *sync_info)
{
    vo_hal_check_phy_dev_return(dev);

    vo_hal_intf_set_sync_info_hvsync(dev, sync_info);
    vo_hal_intf_set_sync_info_other(dev, sync_info);
}

td_void vo_hal_set_intf_bt_component_order(td_bool component_inverse_en)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_intf_bt_ctrl intf_bt_ctrl;
    volatile td_ulong addr_reg;

    addr_reg = (td_ulong)(td_uintptr_t)&(vo_reg->intf_bt_ctrl.u32);
    intf_bt_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    intf_bt_ctrl.bits.yc_mode = component_inverse_en;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, intf_bt_ctrl.u32);
}

td_void vo_hal_set_intf_bt_bit_inverse(td_bool bit_inverse_en)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_intf_bt_ctrl intf_bt_ctrl;
    volatile td_ulong addr_reg;

    addr_reg = (td_ulong)(td_uintptr_t)&(vo_reg->intf_bt_ctrl.u32);
    intf_bt_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    intf_bt_ctrl.bits.bit_inv = bit_inverse_en;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, intf_bt_ctrl.u32);
}

#endif

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

#if vo_desc("dev&intf")

/* 设置显示通道中断产生模式 */
td_void hal_disp_set_vtthd_mode(ot_vo_dev dev, td_u32 field_mode)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_vtthd dhd0_vtthd;
    volatile td_ulong addr_reg;

    vo_hal_check_phy_dev_return(dev);

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_vtthd.u32));
    dhd0_vtthd.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    dhd0_vtthd.bits.thd1_mode = field_mode;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, dhd0_vtthd.u32);
}

/* 设置显示通道中断产生门限 */
td_void hal_disp_set_vtthd(ot_vo_dev dev, td_u32 vtthd, td_u32 vtthd2)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_vtthd dhd0_vtthd;
    volatile td_ulong addr_reg;

    vo_hal_check_phy_dev_return(dev);

    /* for vtmgthd1 and vtmgthd2 */
    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_vtthd.u32));
    dhd0_vtthd.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    dhd0_vtthd.bits.vtmgthd1 = vtthd;
    dhd0_vtthd.bits.vtmgthd2 = vtthd2;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, dhd0_vtthd.u32);
}

/* 获取接口显示帧状态: 顶底场 */
td_void hal_disp_get_int_state(ot_vo_dev dev, td_bool *bottom)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_state dhd0_state;
    volatile td_ulong addr_reg;

    vo_hal_check_phy_dev_return(dev);

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_state.u32));
    dhd0_state.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    *bottom = dhd0_state.bits.bottom_field;
}

#ifdef CONFIG_OT_VO_CASCADE
td_void hal_disp_get_int_state_vblank(ot_vo_dev dev, td_bool *vblank)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_state dhd0_state;
    volatile td_ulong addr_reg;

    vo_hal_check_phy_dev_return(dev);

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_state.u32));
    dhd0_state.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    *vblank = dhd0_state.bits.vblank;
}
#endif

td_void hal_disp_get_int_state_vcnt(ot_vo_dev dev, td_u32 *vcnt)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_state dhd0_state;
    volatile td_ulong addr_reg;

    vo_hal_check_phy_dev_return(dev);

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_state.u32));
    dhd0_state.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    *vcnt = dhd0_state.bits.vcnt;
}

#if (defined(CONFIG_OT_VO_WBC) || !defined(CONFIG_OT_VO_FB_SEPARATE))
/* 获取接口时序的逐隔行信息 interlaced or progressive */
td_void hal_disp_get_disp_iop(ot_vo_dev dev, td_bool *iop)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_ctrl dhd0_ctrl;
    volatile td_ulong addr_reg;

    vo_hal_check_phy_dev_return(dev);

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_ctrl.u32));
    dhd0_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    *iop = dhd0_ctrl.bits.iop;
}
#endif

/* 获取显示通道中断产生模式 */
td_void hal_disp_get_vtthd_mode(ot_vo_dev dev, td_bool *field_mode)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_vtthd dhd0_vtthd;
    volatile td_ulong addr_reg;

    vo_hal_check_phy_dev_return(dev);

    addr_reg = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_vtthd.u32));
    dhd0_vtthd.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    *field_mode = dhd0_vtthd.bits.thd1_mode;
}

/* 打开、关闭中断 */
td_void hal_disp_set_int_mask(td_u32 mask_en)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vointmsk vo_intmsk;

    /* display interrupt mask enable */
    vo_intmsk.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)&(vo_reg->vointmsk.u32));
    vo_intmsk.u32 = vo_intmsk.u32 | mask_en;
    hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->vointmsk.u32), vo_intmsk.u32);
}

td_void hal_disp_clr_int_mask(td_u32 mask_en)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_vointmsk vo_intmsk;

    /* display interrupt mask enable */
    vo_intmsk.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)&(vo_reg->vointmsk.u32));
    vo_intmsk.u32 = vo_intmsk.u32 & (~mask_en);
    hal_write_reg((td_u32 *)(td_uintptr_t)&(vo_reg->vointmsk.u32), vo_intmsk.u32);
}

#ifdef CONFIG_OT_VO_VGA
static td_void vo_hal_intf_get_vga_dec_det(ot_vo_dev dev, td_u32 *det_line, td_u32 *det_pixel)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_dhd0_vsync1 dhd0_vsync1;
    volatile reg_dhd0_vsync2 dhd0_vsync2;
    volatile reg_dhd0_hsync1 dhd0_hsync1;
    volatile reg_dhd0_hsync2 dhd0_hsync2;
    volatile td_ulong addr_reg_vsync1;
    volatile td_ulong addr_reg_vsync2;
    volatile td_ulong addr_reg_hsync1;
    volatile td_ulong addr_reg_hsync2;
    td_u32 vbb;
    td_u32 vfb;
    td_u32 hact;
    td_u32 hbb;
    td_u32 hfb;

    vo_hal_check_phy_dev_return(dev);

    addr_reg_vsync1 = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_vsync1.u32));
    addr_reg_vsync2 = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_vsync2.u32));
    addr_reg_hsync1 = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_hsync1.u32));
    addr_reg_hsync2 = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_hsync2.u32));

    dhd0_vsync1.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg_vsync1);
    dhd0_vsync2.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg_vsync2);
    dhd0_hsync1.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg_hsync1);
    dhd0_hsync2.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg_hsync2);

    vbb = dhd0_vsync1.bits.vbb;
    vfb = dhd0_vsync2.bits.vfb;
    hact = dhd0_hsync1.bits.hact;
    hbb = dhd0_hsync1.bits.hbb;
    hfb = dhd0_hsync2.bits.hfb;

    *det_line = vfb + (vbb >> 1);
    *det_pixel = (hact + hbb + hfb) >> 1;
}
#endif

static td_void vo_hal_intf_get_dec_det(ot_vo_dev dev, ot_vo_intf_type intf_type, td_bool enable,
    td_u32 *det_line, td_u32 *det_pixel)
{
#ifndef CONFIG_OT_VO_VGA
    ot_unused(dev);
#endif

    if (enable == TD_FALSE) {
        *det_line = 0;
        *det_pixel = 0;
        return;
    }

#ifdef CONFIG_OT_VO_CVBS
    if (intf_type == OT_VO_INTF_CVBS) {
        *det_line = 0xd;
        *det_pixel = 0x118;
        return;
    }
#endif

#ifdef CONFIG_OT_VO_VGA
    if (intf_type == OT_VO_INTF_VGA) {
        vo_hal_intf_get_vga_dec_det(dev, det_line, det_pixel);
    }
#endif
}

#ifdef CONFIG_OT_VO_CVBS
static td_void vo_hal_intf_set_cvbs_dac_det(ot_vo_dev dev, ot_vo_intf_type intf_type, td_bool enable)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();

    volatile reg_date_dacdet1 date_dacdet1;
    volatile reg_date_dacdet2 date_dacdet2;

    td_u32 det_line = 0;
    td_u32 det_pixel = 0;
    td_u32 det_high = 0;

    date_dacdet1.u32 = vo_reg->date_dacdet1.u32;
    date_dacdet2.u32 = vo_reg->date_dacdet2.u32;

    vo_hal_intf_get_dec_det(dev, intf_type, enable, &det_line, &det_pixel);
    vo_hal_intf_get_dec_det_high(intf_type, enable, &det_high);

    date_dacdet1.bits.vdac_det_high = det_high;   /* 这里后端默认值是，芯片默认值是0x303  */
    date_dacdet1.bits.det_line = det_line;
    date_dacdet2.bits.det_pixel_sta = det_pixel;
    date_dacdet2.bits.det_pixel_wid = 0x30;
    date_dacdet2.bits.vdac_det_en = (enable == TD_TRUE) ? 1 : 0;

    vo_reg->date_dacdet1.u32 = date_dacdet1.u32;
    vo_reg->date_dacdet2.u32 = date_dacdet2.u32;
}
#endif

#ifdef CONFIG_OT_VO_VGA
static td_void vo_hal_intf_set_vga_dac_det(ot_vo_dev dev, ot_vo_intf_type intf_type, td_bool enable)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();

    volatile reg_dhd0_dacdet1 dhd0_dacdet1;
    volatile reg_dhd0_dacdet2 dhd0_dacdet2;
    volatile td_ulong addr_reg_dacdet1;
    volatile td_ulong addr_reg_dacdet2;

    td_u32 det_line = 0;
    td_u32 det_pixel = 0;
    td_u32 det_high = 0;

    addr_reg_dacdet1 = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_dacdet1.u32));
    addr_reg_dacdet2 = vou_get_chn_abs_addr(dev, (td_uintptr_t)&(vo_reg->dhd0_dacdet2.u32));
    dhd0_dacdet1.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg_dacdet1);
    dhd0_dacdet2.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg_dacdet2);

    vo_hal_intf_get_dec_det(dev, intf_type, enable, &det_line, &det_pixel);
    vo_hal_intf_get_dec_det_high(intf_type, enable, &det_high);

    dhd0_dacdet1.bits.vdac_det_high = det_high;
    dhd0_dacdet1.bits.det_line = det_line;
    dhd0_dacdet2.bits.det_pixel_sta = det_pixel;
    dhd0_dacdet2.bits.det_pixel_wid = 0x30;
    dhd0_dacdet2.bits.vdac_det_en = (enable == TD_TRUE) ? 1 : 0;

    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg_dacdet1, dhd0_dacdet1.u32);
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg_dacdet2, dhd0_dacdet2.u32);
}
#endif

td_void vo_hal_intf_set_dac_det(ot_vo_dev dev, ot_vo_intf_type intf_type, td_bool enable)
{
#ifdef CONFIG_OT_VO_CVBS
    if (intf_type == OT_VO_INTF_CVBS) {
        vo_hal_intf_set_cvbs_dac_det(dev, intf_type, enable);
        return;
    }
#endif

#ifdef CONFIG_OT_VO_VGA
    if (intf_type == OT_VO_INTF_VGA) {
        vo_hal_intf_set_vga_dac_det(dev, intf_type, enable);
    }
#endif
}

#endif

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
