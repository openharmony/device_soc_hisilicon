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

#include "hdmi_reg_crg.h"
#include "hdmi_product_define.h"

volatile hdmi_reg_crg *g_crg_regs = TD_NULL;

int hdmi_reg_crg_init(td_void)
{
    if (g_crg_regs != NULL) {
        return TD_SUCCESS;
    }
    g_crg_regs = (volatile hdmi_reg_crg *)osal_ioremap_nocache(HDMI_CRG_ADDR, sizeof(hdmi_reg_crg));
    if (g_crg_regs == NULL) {
        hdmi_err("crg addr ioremap failed!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

int hdmi_reg_crg_deinit(td_void)
{
    if (g_crg_regs != TD_NULL) {
        osal_iounmap((void *)g_crg_regs, sizeof(hdmi_reg_crg));
        g_crg_regs = TD_NULL;
    }
    return TD_SUCCESS;
}

td_void hdmi_reg_ctrl_osc_24m_cken_set(td_u8 hdmitx_ctrl_osc_24m_cken)
{
    td_u32 *reg_addr = NULL;
    peri_crg8144 crg8144;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8144.u32);
    crg8144.u32 = hdmi_tx_reg_read(reg_addr);
    crg8144.bits.hdmitx_ctrl_osc_24m_cken = hdmitx_ctrl_osc_24m_cken;
    hdmi_tx_reg_write(reg_addr, crg8144.u32);

    return;
}

td_void hdmi_reg_ctrl_cec_cken_set(td_u8 hdmitx_ctrl_cec_cken)
{
    td_u32 *reg_addr = NULL;
    peri_crg8144 crg8144;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8144.u32);
    crg8144.u32 = hdmi_tx_reg_read(reg_addr);
    crg8144.bits.hdmitx_ctrl_cec_cken = hdmitx_ctrl_cec_cken;
    hdmi_tx_reg_write(reg_addr, crg8144.u32);

    return;
}

td_void hdmi_reg_ctrl_os_cken_set(td_u8 hdmitx_ctrl_os_cken)
{
    td_u32 *reg_addr = NULL;
    peri_crg8144 crg8144;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8144.u32);
    crg8144.u32 = hdmi_tx_reg_read(reg_addr);
    crg8144.bits.hdmitx_ctrl_os_cken = hdmitx_ctrl_os_cken;
    hdmi_tx_reg_write(reg_addr, crg8144.u32);

    return;
}

td_void hdmi_reg_ctrl_as_cken_set(td_u8 as_cken)
{
    td_u32 *reg_addr = NULL;
    peri_crg8144 crg8144;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8144.u32);
    crg8144.u32 = hdmi_tx_reg_read(reg_addr);
    crg8144.bits.hdmitx_ctrl_as_cken = as_cken;
    hdmi_tx_reg_write(reg_addr, crg8144.u32);

    return;
}

td_void hdmi_reg_ctrl_bus_srst_req_set(td_u8 bus_srst_req)
{
    td_u32 *reg_addr = NULL;
    peri_crg8144 crg8144;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8144.u32);
    crg8144.u32 = hdmi_tx_reg_read(reg_addr);
    crg8144.bits.hdmitx_ctrl_bus_srst_req = bus_srst_req;
    hdmi_tx_reg_write(reg_addr, crg8144.u32);

    return;
}

td_void hdmi_reg_ctrl_srst_req_set(td_u8 srst_req)
{
    td_u32 *reg_addr = NULL;
    peri_crg8144 crg8144;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8144.u32);
    crg8144.u32 = hdmi_tx_reg_read(reg_addr);
    crg8144.bits.hdmitx_ctrl_srst_req = srst_req;
    hdmi_tx_reg_write(reg_addr, crg8144.u32);

    return;
}

td_void hdmi_reg_ctrl_cec_srst_req_set(td_u8 cec_srst_req)
{
    td_u32 *reg_addr = NULL;
    peri_crg8144 crg8144;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8144.u32);
    crg8144.u32 = hdmi_tx_reg_read(reg_addr);
    crg8144.bits.hdmitx_ctrl_cec_srst_req = cec_srst_req;
    hdmi_tx_reg_write(reg_addr, crg8144.u32);

    return;
}

td_void hdmi_reg_hdmitx_phy_tmds_cken_set(td_u8 phy_tmds_cken)
{
    td_u32 *reg_addr = NULL;
    peri_crg8152 crg_8152;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8152.u32);
    crg_8152.u32 = hdmi_tx_reg_read(reg_addr);
    crg_8152.bits.hdmitx_phy_tmds_cken = phy_tmds_cken;
    hdmi_tx_reg_write(reg_addr, crg_8152.u32);

    return;
}

td_void hdmi_reg_hdmitx_phy_modclk_cken_set(td_u8 phy_modclk_cken)
{
    td_u32 *reg_addr = NULL;
    peri_crg8152 crg_8152;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8152.u32);
    crg_8152.u32 = hdmi_tx_reg_read(reg_addr);
    crg_8152.bits.hdmitx_phy_modclk_cken = phy_modclk_cken;
    hdmi_tx_reg_write(reg_addr, crg_8152.u32);

    return;
}

td_void hdmi_reg_ac_ctrl_modclk_cken_set(td_u8 ac_ctrl_modclk_cken)
{
    td_u32 *reg_addr = NULL;
    peri_crg8152 crg_8152;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8152.u32);
    crg_8152.u32 = hdmi_tx_reg_read(reg_addr);
    crg_8152.bits.ac_ctrl_modclk_cken = ac_ctrl_modclk_cken;
    hdmi_tx_reg_write(reg_addr, crg_8152.u32);

    return;
}

td_void hdmi_reg_phy_srst_req_set(td_u8 phy_srst_req)
{
    td_u32 *reg_addr = NULL;
    peri_crg8152 crg_8152;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8152.u32);
    crg_8152.u32 = hdmi_tx_reg_read(reg_addr);
    crg_8152.bits.hdmitx_phy_srst_req = phy_srst_req;
    hdmi_tx_reg_write(reg_addr, crg_8152.u32);

    return;
}

td_u8 hdmi_reg_phy_srst_req_get(td_void)
{
    td_u32 *reg_addr = NULL;
    peri_crg8152 crg_8152;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8152.u32);
    crg_8152.u32 = hdmi_tx_reg_read(reg_addr);
    return crg_8152.bits.hdmitx_phy_srst_req;
}

td_void hdmi_reg_phy_bus_srst_req_set(td_u8 bus_srst_req)
{
    td_u32 *reg_addr = NULL;
    peri_crg8152 crg_8152;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8152.u32);
    crg_8152.u32 = hdmi_tx_reg_read(reg_addr);
    crg_8152.bits.hdmitx_phy_bus_srst_req = bus_srst_req;
    hdmi_tx_reg_write(reg_addr, crg_8152.u32);

    return;
}

td_void hdmi_reg_ac_ctrl_srst_req_set(td_u8 ac_ctrl_srst_req)
{
    td_u32 *reg_addr = NULL;
    peri_crg8152 crg_8152;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8152.u32);
    crg_8152.u32 = hdmi_tx_reg_read(reg_addr);
    crg_8152.bits.ac_ctrl_srst_req = ac_ctrl_srst_req;
    hdmi_tx_reg_write(reg_addr, crg_8152.u32);

    return;
}

td_void hdmi_reg_ac_ctrl_bus_srst_req_set(td_u8 ac_ctrl_bus_srst_req)
{
    td_u32 *reg_addr = NULL;
    peri_crg8152 crg_8152;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8152.u32);
    crg_8152.u32 = hdmi_tx_reg_read(reg_addr);
    crg_8152.bits.ac_ctrl_bus_srst_req = ac_ctrl_bus_srst_req;
    hdmi_tx_reg_write(reg_addr, crg_8152.u32);

    return;
}

td_void hdmi_reg_phy_clk_pctrl_set(td_u8 clk_pctrl)
{
    td_u32 *reg_addr = NULL;
    peri_crg8152 crg_8152;

    reg_addr = (td_u32 *)&(g_crg_regs->crg8152.u32);
    crg_8152.u32 = hdmi_tx_reg_read(reg_addr);
    crg_8152.bits.hdmitx_phy_clk_pctrl = clk_pctrl;
    hdmi_tx_reg_write(reg_addr, crg_8152.u32);

    return;
}
