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
#include "hdmi_reg_dphy.h"
#include "hdmi_product_define.h"

static volatile hdmi2tx_dphy_reg_type *g_hdmi2tx_dphy_regs = NULL;

td_s32 hdmi_reg_tx_phy_init(td_char *addr)
{
    g_hdmi2tx_dphy_regs = (volatile hdmi2tx_dphy_reg_type *)(addr);
    return TD_SUCCESS;
}

td_s32 hdmi_reg_tx_phy_deinit(td_void)
{
    if (g_hdmi2tx_dphy_regs != TD_NULL) {
        g_hdmi2tx_dphy_regs = TD_NULL;
    }
    return TD_SUCCESS;
}

td_void hdmi_reg_sscin_bypass_en_set(td_u8 reg_sscin_bypass_en)
{
    td_u32 *reg_addr = NULL;
    hdmitx_inssc_set hdmitx_inssc;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->ssc_in_set.u32);
    hdmitx_inssc.u32 = hdmi_tx_reg_read(reg_addr);
    hdmitx_inssc.bits.reg_sscin_bypass_en = reg_sscin_bypass_en;
    hdmi_tx_reg_write(reg_addr, hdmitx_inssc.u32);

    return;
}

td_void hdmi_reg_pllfbmash111_en_set(td_u8 reg_pllfbmash111_en)
{
    td_u32 *reg_addr = NULL;
    hdmitx_inssc_set hdmitx_inssc;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->ssc_in_set.u32);
    hdmitx_inssc.u32 = hdmi_tx_reg_read(reg_addr);
    hdmitx_inssc.bits.reg_pllfbmash111_en = reg_pllfbmash111_en;
    hdmi_tx_reg_write(reg_addr, hdmitx_inssc.u32);

    return;
}

td_void hdmi_reg_dphy_rst_set(td_u8 reg_rst)
{
    td_u32 *reg_addr = NULL;
    hdmitx_dphy_rst hdmitx_dphy;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->dphy_rst.u32);
    hdmitx_dphy.u32 = hdmi_tx_reg_read(reg_addr);
    hdmitx_dphy.bits.reg_dphy_srst_req = reg_rst;
    hdmi_tx_reg_write(reg_addr, hdmitx_dphy.u32);

    return;
}

td_void hdmi_reg_aphy_data_clk_height_set(td_u16 reg_aphy_data_clk_h)
{
    td_u32 *reg_addr = NULL;
    hdmitx_afifo_data_sel data_sel;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->afifo_data_sel.u32);
    data_sel.u32 = hdmi_tx_reg_read(reg_addr);
    data_sel.bits.reg_aphy_data_clk_h = reg_aphy_data_clk_h;
    hdmi_tx_reg_write(reg_addr, data_sel.u32);

    return;
}

td_void hdmi_reg_aphy_data_clk_low_set(td_u32 reg_aphy_data_clk_l)
{
    td_u32 *reg_addr = NULL;
    hdmitx_afifo_clk afifo_clk;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->afifo_data_clk.u32);
    afifo_clk.u32 = hdmi_tx_reg_read(reg_addr);
    afifo_clk.bits.reg_aphy_data_clk_l = reg_aphy_data_clk_l;
    hdmi_tx_reg_write(reg_addr, afifo_clk.u32);

    return;
}

td_void hdmi_reg_divsel_set(td_u8 reg_divsel)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);
    top_pd.bits.reg_divsel = reg_divsel;
    hdmi_tx_reg_write(reg_addr, top_pd.u32);

    return;
}

td_void hdmi_reg_gc_txpll_pd_set(td_u8 reg_gc_txpll_pd)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);
    top_pd.bits.reg_gc_txpll_pd = reg_gc_txpll_pd;
    hdmi_tx_reg_write(reg_addr, top_pd.u32);

    return;
}

td_u8 hdmi_reg_gc_txpll_pd_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);
    return top_pd.bits.reg_gc_txpll_pd;
}

td_void hdmi_reg_gc_pd_rxsense_set(td_u8 reg_gc_pd_rxsense)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);
    top_pd.bits.reg_gc_pd_rxsense = reg_gc_pd_rxsense;
    hdmi_tx_reg_write(reg_addr, top_pd.u32);

    return;
}

td_u8 hdmi_reg_gc_pd_rxsense_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);
    return top_pd.bits.reg_gc_pd_rxsense;
}

td_u8 hdmi_reg_gc_pd_rterm_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_reg_read(reg_addr);
    return top_pd.bits.reg_gc_pd_rterm;
}

td_void hdmi_reg_gc_pd_rterm_set(td_u8 reg_gc_pd_rterm)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);
    top_pd.bits.reg_gc_pd_rterm = reg_gc_pd_rterm;
    hdmi_tx_reg_write(reg_addr, top_pd.u32);

    return;
}

td_void hdmi_reg_gc_pd_ldo_set(td_u8 reg_gc_pd_ldo)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);
    top_pd.bits.reg_gc_pd_ldo = reg_gc_pd_ldo;
    hdmi_tx_reg_write(reg_addr, top_pd.u32);

    return;
}

td_u8 hdmi_reg_gc_pd_ldo_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);
    return top_pd.bits.reg_gc_pd_ldo;
}

td_void hdmi_reg_gc_pd_de_set(td_u8 reg_gc_pd_de)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);
    top_pd.bits.reg_gc_pd_de = reg_gc_pd_de;
    hdmi_tx_reg_write(reg_addr, top_pd.u32);

    return;
}

td_u8 hdmi_reg_gc_pd_de_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);
    return top_pd.bits.reg_gc_pd_de;
}

td_void hdmi_reg_gc_pd_bist_set(td_u8 reg_gc_pd_bist)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);
    top_pd.bits.reg_gc_pd_bist = reg_gc_pd_bist;
    hdmi_tx_reg_write(reg_addr, top_pd.u32);

    return;
}

td_u8 hdmi_reg_gc_pd_bist_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);

    return top_pd.bits.reg_gc_pd_bist;
}

td_void hdmi_reg_gc_pd_set(td_u8 reg_gc_pd)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);
    top_pd.bits.reg_gc_pd = reg_gc_pd;
    hdmi_tx_reg_write(reg_addr, top_pd.u32);

    return;
}

td_u8 hdmi_reg_gc_pd_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_top_pd top_pd;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_top_misc_ctrl.u32);
    top_pd.u32 = hdmi_tx_reg_read(reg_addr);
    return top_pd.bits.reg_gc_pd;
}

td_void hdmi_reg_isel_main_de_clk_set(td_u8 reg_isel_main_de_clk)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_imain driver_imain;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_imain.u32);
    driver_imain.u32 = hdmi_tx_reg_read(reg_addr);
    driver_imain.bits.reg_isel_main_de_clk = reg_isel_main_de_clk;
    hdmi_tx_reg_write(reg_addr, driver_imain.u32);

    return;
}

td_u8 hdmi_reg_isel_main_de_clk_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_imain driver_imain;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_imain.u32);
    driver_imain.u32 = hdmi_tx_reg_read(reg_addr);
    return driver_imain.bits.reg_isel_main_de_clk;
}

td_void hdmi_reg_isel_main_d2_set(td_u8 reg_isel_main_d2)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_imain driver_imain;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_imain.u32);
    driver_imain.u32 = hdmi_tx_reg_read(reg_addr);
    driver_imain.bits.reg_isel_main_d2 = reg_isel_main_d2;
    hdmi_tx_reg_write(reg_addr, driver_imain.u32);

    return;
}

td_void hdmi_reg_isel_main_d1_set(td_u8 reg_isel_main_d1)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_imain driver_imain;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_imain.u32);
    driver_imain.u32 = hdmi_tx_reg_read(reg_addr);
    driver_imain.bits.reg_isel_main_d1 = reg_isel_main_d1;
    hdmi_tx_reg_write(reg_addr, driver_imain.u32);

    return;
}

td_void hdmi_reg_isel_main_d0_set(td_u8 reg_isel_main_d0)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_imain driver_imain;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_imain.u32);
    driver_imain.u32 = hdmi_tx_reg_read(reg_addr);
    driver_imain.bits.reg_isel_main_d0 = reg_isel_main_d0;
    hdmi_tx_reg_write(reg_addr, driver_imain.u32);

    return;
}

td_void hdmi_reg_isel_main_clk_set(td_u8 reg_isel_main_clk)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_imain driver_imain;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_imain.u32);
    driver_imain.u32 = hdmi_tx_reg_read(reg_addr);
    driver_imain.bits.reg_isel_main_clk = reg_isel_main_clk;
    hdmi_tx_reg_write(reg_addr, driver_imain.u32);

    return;
}

td_void hdmi_reg_isel_main_de_d2_set(td_u8 reg_isel_main_de_d2)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_ipre driver_ipre;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_ipre.u32);
    driver_ipre.u32 = hdmi_tx_reg_read(reg_addr);
    driver_ipre.bits.reg_isel_main_de_d2 = reg_isel_main_de_d2;
    hdmi_tx_reg_write(reg_addr, driver_ipre.u32);

    return;
}

td_u8 hdmi_reg_isel_main_de_d2_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_ipre driver_ipre;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_ipre.u32);
    driver_ipre.u32 = hdmi_tx_reg_read(reg_addr);

    return driver_ipre.bits.reg_isel_main_de_d2;
}

td_void hdmi_reg_isel_main_de_d1_set(td_u8 reg_isel_main_de_d1)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_ipre driver_ipre;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_ipre.u32);
    driver_ipre.u32 = hdmi_tx_reg_read(reg_addr);
    driver_ipre.bits.reg_isel_main_de_d1 = reg_isel_main_de_d1;
    hdmi_tx_reg_write(reg_addr, driver_ipre.u32);

    return;
}

td_u8 hdmi_reg_isel_main_de_d1_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_ipre driver_ipre;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_ipre.u32);
    driver_ipre.u32 = hdmi_tx_reg_read(reg_addr);

    return driver_ipre.bits.reg_isel_main_de_d1;
}

td_void hdmi_reg_isel_main_de_d0_set(td_u8 reg_isel_main_de_d0)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_ipre driver_ipre;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_ipre.u32);
    driver_ipre.u32 = hdmi_tx_reg_read(reg_addr);
    driver_ipre.bits.reg_isel_main_de_d0 = reg_isel_main_de_d0;
    hdmi_tx_reg_write(reg_addr, driver_ipre.u32);

    return;
}

td_u8 hdmi_reg_isel_main_de_d0_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_ipre driver_ipre;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_ipre.u32);
    driver_ipre.u32 = hdmi_tx_reg_read(reg_addr);

    return driver_ipre.bits.reg_isel_main_de_d0;
}

td_u8 hdmi_reg_isel_main_clk_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_imain driver_imain;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_imain.u32);
    driver_imain.u32 = hdmi_tx_reg_read(reg_addr);

    return driver_imain.bits.reg_isel_main_clk;
}

td_u8 hdmi_reg_isel_main_d0_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_imain driver_imain;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_imain.u32);
    driver_imain.u32 = hdmi_tx_reg_read(reg_addr);

    return driver_imain.bits.reg_isel_main_d0;
}

td_void hdmi_reg_rsel_pre_d2_set(td_u8 reg_rsel_pre_d2)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_rpre driver_rpre;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_rpre.u32);
    driver_rpre.u32 = hdmi_tx_reg_read(reg_addr);
    driver_rpre.bits.reg_rsel_pre_d2 = reg_rsel_pre_d2;
    hdmi_tx_reg_write(reg_addr, driver_rpre.u32);

    return;
}

td_void hdmi_reg_rsel_pre_d1_set(td_u8 reg_rsel_pre_d1)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_rpre driver_rpre;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_rpre.u32);
    driver_rpre.u32 = hdmi_tx_reg_read(reg_addr);
    driver_rpre.bits.reg_rsel_pre_d1 = reg_rsel_pre_d1;
    hdmi_tx_reg_write(reg_addr, driver_rpre.u32);

    return;
}

td_void hdmi_reg_rsel_pre_d0_set(td_u8 reg_rsel_pre_d0)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_rpre driver_rpre;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_rpre.u32);
    driver_rpre.u32 = hdmi_tx_reg_read(reg_addr);
    driver_rpre.bits.reg_rsel_pre_d0 = reg_rsel_pre_d0;
    hdmi_tx_reg_write(reg_addr, driver_rpre.u32);

    return;
}

td_void hdmi_reg_rsel_pre_clk_set(td_u8 reg_rsel_pre_clk)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_rpre driver_rpre;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_rpre.u32);
    driver_rpre.u32 = hdmi_tx_reg_read(reg_addr);
    driver_rpre.bits.reg_rsel_pre_clk = reg_rsel_pre_clk;
    hdmi_tx_reg_write(reg_addr, driver_rpre.u32);

    return;
}

td_u8 hdmi_reg_rsel_pre_clk_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_rpre driver_rpre;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_rpre.u32);
    driver_rpre.u32 = hdmi_tx_reg_read(reg_addr);

    return driver_rpre.bits.reg_rsel_pre_clk;
}

td_u8 hdmi_reg_rsel_pre_d0_get(td_void)
{
    td_u32 *reg_addr = NULL;
    aphy_driver_rpre driver_rpre;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->aphy_rpre.u32);
    driver_rpre.u32 = hdmi_tx_reg_read(reg_addr);

    return driver_rpre.bits.reg_rsel_pre_d0;
}

td_void hdmi_reg_rt_d2_set(td_u8 reg_rt_d2)
{
    td_u32 *reg_addr = NULL;
    aphy_rterm_ctrl rterm_ctrl;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->rterm_ctrl.u32);
    rterm_ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    rterm_ctrl.bits.reg_rt_d2 = reg_rt_d2;
    hdmi_tx_reg_write(reg_addr, rterm_ctrl.u32);

    return;
}

td_void hdmi_reg_test_set(td_u16 reg_test)
{
    td_u32 *reg_addr = NULL;
    aphy_test_ctrl test_ctrl;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->test_ctrl_aphy.u32);
    test_ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    test_ctrl.bits.reg_test = reg_test;
    hdmi_tx_reg_write(reg_addr, test_ctrl.u32);

    return;
}

td_void hdmi_reg_rt_d1_set(td_u8 reg_rt_d1)
{
    td_u32 *reg_addr = NULL;
    aphy_rterm_ctrl rterm_ctrl;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->rterm_ctrl.u32);
    rterm_ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    rterm_ctrl.bits.reg_rt_d1 = reg_rt_d1;
    hdmi_tx_reg_write(reg_addr, rterm_ctrl.u32);

    return;
}

td_void hdmi_reg_rt_d0_set(td_u8 reg_rt_d0)
{
    td_u32 *reg_addr = NULL;
    aphy_rterm_ctrl rterm_ctrl;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->rterm_ctrl.u32);
    rterm_ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    rterm_ctrl.bits.reg_rt_d0 = reg_rt_d0;
    hdmi_tx_reg_write(reg_addr, rterm_ctrl.u32);

    return;
}

td_void hdmi_reg_rt_clk_set(td_u8 reg_rt_clk)
{
    td_u32 *reg_addr = NULL;
    aphy_rterm_ctrl rterm_ctrl;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->rterm_ctrl.u32);
    rterm_ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    rterm_ctrl.bits.reg_rt_clk = reg_rt_clk;
    hdmi_tx_reg_write(reg_addr, rterm_ctrl.u32);

    return;
}

td_void hdmi_reg_gc_txpll_en_sscdiv_set(td_u8 reg_gc_txpll_en_sscdiv)
{
    td_u32 *reg_addr = NULL;
    aphy_txpll_ctrl txpll_ctrl;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->txpll_ctrl.u32);
    txpll_ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    txpll_ctrl.bits.reg_gc_txpll_en_sscdiv = reg_gc_txpll_en_sscdiv;
    hdmi_tx_reg_write(reg_addr, txpll_ctrl.u32);

    return;
}

td_void hdmi_reg_txpll_icp_ictrl_set(td_u8 reg_txpll_icp_ictrl)
{
    td_u32 *reg_addr = NULL;
    aphy_txpll_ctrl txpll_ctrl;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->txpll_ctrl.u32);
    txpll_ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    txpll_ctrl.bits.reg_txpll_icp_ictrl = reg_txpll_icp_ictrl;
    hdmi_tx_reg_write(reg_addr, txpll_ctrl.u32);

    return;
}

td_void hdmi_reg_txpll_divsel_loop_set(td_u8 reg_txpll_divsel_loop)
{
    td_u32 *reg_addr = NULL;
    aphy_txpll_ctrl txpll_ctrl;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->txpll_ctrl.u32);
    txpll_ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    txpll_ctrl.bits.reg_txpll_divsel_loop = reg_txpll_divsel_loop;
    hdmi_tx_reg_write(reg_addr, txpll_ctrl.u32);

    return;
}

td_void hdmi_reg_gc_txpll_test_set(td_u32 reg_txpll_test)
{
    td_u32 *reg_addr = NULL;
    aphy_txpll_test txpll_test;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->txpll_test.u32);
    txpll_test.u32 = hdmi_tx_reg_read(reg_addr);
    txpll_test.bits.reg_txpll_test = reg_txpll_test;
    hdmi_tx_reg_write(reg_addr, txpll_test.u32);

    return;
}

td_void hdmi_reg_ssc_mode_fb_set(td_u8 reg_ssc_mode_fb)
{
    td_u32 *reg_addr = NULL;
    hdmitx_fbssc_set hdmitx_fbssc;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->fb_ssc_set.u32);
    hdmitx_fbssc.u32 = hdmi_tx_reg_read(reg_addr);
    hdmitx_fbssc.bits.reg_ssc_mode_fb = reg_ssc_mode_fb;
    hdmi_tx_reg_write(reg_addr, hdmitx_fbssc.u32);

    return;
}

td_void hdmi_reg_load_fb_set(td_u8 reg_set_load_fb)
{
    td_u32 *reg_addr = NULL;
    hdmitx_fbssc_set hdmitx_fbssc;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->fb_ssc_set.u32);
    hdmitx_fbssc.u32 = hdmi_tx_reg_read(reg_addr);
    hdmitx_fbssc.bits.reg_set_load_fb = reg_set_load_fb;
    hdmi_tx_reg_write(reg_addr, hdmitx_fbssc.u32);

    return;
}

td_void hdmi_reg_fb_set(td_u32 reg_set_fb)
{
    td_u32 *reg_addr = NULL;
    hdmitx_fbssc_set hdmitx_fbssc;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->fb_ssc_set.u32);
    hdmitx_fbssc.u32 = hdmi_tx_reg_read(reg_addr);
    hdmitx_fbssc.bits.reg_set_fb = reg_set_fb;
    hdmi_tx_reg_write(reg_addr, hdmitx_fbssc.u32);

    return;
}

td_void hdmi_reg_span_fb_set(td_u16 reg_span_fb)
{
    td_u32 *reg_addr = NULL;
    hdmitx_fbssc_step fbssc_step;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->fb_ssc_step.u32);
    fbssc_step.u32 = hdmi_tx_reg_read(reg_addr);
    fbssc_step.bits.reg_span_fb = reg_span_fb;
    hdmi_tx_reg_write(reg_addr, fbssc_step.u32);

    return;
}

td_u16 hdmi_reg_span_fb_get(td_void)
{
    td_u32 *reg_addr = NULL;
    hdmitx_fbssc_step fbssc_step;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->fb_ssc_step.u32);
    fbssc_step.u32 = hdmi_tx_reg_read(reg_addr);
    return fbssc_step.bits.reg_span_fb;
}

td_void hdmi_reg_step_fb_set(td_u16 reg_step_fb)
{
    td_u32 *reg_addr = NULL;
    hdmitx_fbssc_step fbssc_step;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->fb_ssc_step.u32);
    fbssc_step.u32 = hdmi_tx_reg_read(reg_addr);
    fbssc_step.bits.reg_step_fb = reg_step_fb;
    hdmi_tx_reg_write(reg_addr, fbssc_step.u32);

    return;
}

td_u16 hdmi_reg_step_fb_get(td_void)
{
    td_u32 *reg_addr = NULL;
    hdmitx_fbssc_step fbssc_step;

    reg_addr = (td_u32 *)&(g_hdmi2tx_dphy_regs->fb_ssc_step.u32);
    fbssc_step.u32 = hdmi_tx_reg_read(reg_addr);
    return fbssc_step.bits.reg_step_fb;
}

