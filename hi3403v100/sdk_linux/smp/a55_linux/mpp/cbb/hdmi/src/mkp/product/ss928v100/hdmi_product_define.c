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

#include "hdmi_product_define.h"
#include "mod_ext.h"
#include "proc_ext.h"
#include "dev_ext.h"
#include "hdmi_reg_video_path.h"

#define CRG_RESET_DELAY 2
#define HDMI_IO_CFG_HPD_SEL 0x2801
#define HDMI_IO_CFG_DDC_SEL 0x6801

td_void hdmi_tx_reg_write(td_u32 *reg_addr, td_u32 value)
{
    *(volatile td_u32 *)reg_addr = value;
    return;
}

td_u32 hdmi_tx_reg_read(const td_u32 *reg_addr)
{
    return *(volatile td_u32 *)(reg_addr);
}

td_void hdmi_reg_write_u32(td_u32 reg_addr, td_u32 value)
{
    volatile td_u32 *addr = TD_NULL;

    addr = (volatile td_u32 *)osal_ioremap_nocache((td_u64)reg_addr, HDMI_REGISTER_SIZE);
    if (addr != TD_NULL) {
        *addr = value;
        osal_iounmap((void *)addr, HDMI_REGISTER_SIZE);
    } else {
        hdmi_err("osal_ioremap_nocache addr=0x%x err!\n", reg_addr);
    }

    return;
}

td_u32 hdmi_reg_read_u32(td_u32 reg_addr)
{
    volatile td_u32 *addr = TD_NULL;
    td_u32 value = 0;

    addr = (volatile td_u32 *)osal_ioremap_nocache((td_u64)reg_addr, HDMI_REGISTER_SIZE);
    if (addr != TD_NULL) {
        value = *addr;
        osal_iounmap((void *)addr, HDMI_REGISTER_SIZE);
    } else {
        hdmi_err("osal_ioremap_nocache addr=0x%x\n err!\n", reg_addr);
    }

    return value;
}

td_void drv_hdmi_prod_io_cfg_set(td_void)
{
    hdmi_if_fpga_return_void();

    hdmi_reg_write_u32(HDMI_ADDR_IO_CFG_HOTPLUG, HDMI_IO_CFG_HPD_SEL);
    hdmi_reg_write_u32(HDMI_ADDR_IO_CFG_SDA, HDMI_IO_CFG_DDC_SEL);
    hdmi_reg_write_u32(HDMI_ADDR_IO_CFG_SCL, HDMI_IO_CFG_DDC_SEL);

    return;
}

td_void drv_hdmi_prod_crg_gate_set(td_bool enable)
{
    hdmi_if_fpga_return_void();

    hdmi_reg_ctrl_osc_24m_cken_set(enable);
    hdmi_reg_ctrl_cec_cken_set(enable);
    hdmi_reg_ctrl_os_cken_set(enable);
    hdmi_reg_ctrl_as_cken_set(enable);
    hdmi_reg_hdmitx_phy_tmds_cken_set(enable);
    hdmi_reg_hdmitx_phy_modclk_cken_set(enable);
    hdmi_reg_ac_ctrl_modclk_cken_set(enable);
    hdmi_reg_phy_clk_pctrl_set(TD_FALSE);

    return;
}

td_void drv_hdmi_prod_crg_all_reset_set(td_bool enable)
{
    hdmi_if_fpga_return_void();

    hdmi_reg_ctrl_bus_srst_req_set(enable);
    hdmi_reg_ctrl_srst_req_set(enable);
    hdmi_reg_ctrl_cec_srst_req_set(enable);
    hdmi_reg_phy_srst_req_set(enable);
    hdmi_reg_phy_bus_srst_req_set(enable);
    hdmi_reg_ac_ctrl_srst_req_set(enable);
    hdmi_reg_ac_ctrl_bus_srst_req_set(enable);
    enable = !enable;
    /*
     * 2, 2us. to ensure ctrl reset success.
     * because internal clock of HDMI is smaller than APB clock.
     */
    osal_udelay(CRG_RESET_DELAY);
    hdmi_reg_ctrl_bus_srst_req_set(enable);
    hdmi_reg_ctrl_srst_req_set(enable);
    hdmi_reg_ctrl_cec_srst_req_set(enable);
    hdmi_reg_phy_srst_req_set(enable);
    hdmi_reg_phy_bus_srst_req_set(enable);
    hdmi_reg_ac_ctrl_srst_req_set(enable);
    hdmi_reg_ac_ctrl_bus_srst_req_set(enable);

    return;
}

td_void drv_hdmi_low_power_set(td_bool enable)
{
    hdmi_if_fpga_return_void();

    if ((hdmi_reg_crg_init() != TD_SUCCESS) || hdmi_video_path_regs_is_inited() != TD_SUCCESS) {
        return;
    }

    enable = !enable;
    hdmi_reg_ctrl_os_cken_set(enable);
    hdmi_reg_ctrl_as_cken_set(enable);
    /* blank data help for low power. */
    hdmi_reg_video_blank_en_set(enable);

    return;
}

td_void drv_hdmi_prod_crg_init(td_void)
{
    drv_hdmi_prod_io_cfg_set();
    drv_hdmi_prod_crg_gate_set(TD_TRUE);
    drv_hdmi_prod_crg_all_reset_set(TD_TRUE);
    drv_hdmi_low_power_set(TD_TRUE);

    return;
}

td_void drv_hdmi_hardware_reset(td_u32 id)
{
    ot_unused(id);
    hdmi_if_fpga_return_void();

    if (hdmi_reg_crg_init() != TD_SUCCESS) {
        hdmi_err("CRG reg io map error!\n");
        return;
    }
    /* reset all module */
    hdmi_reg_ctrl_bus_srst_req_set(TD_TRUE);
    hdmi_reg_ctrl_srst_req_set(TD_TRUE);
    hdmi_reg_ctrl_cec_srst_req_set(TD_TRUE);
    hdmi_reg_phy_srst_req_set(TD_TRUE);
    hdmi_reg_phy_bus_srst_req_set(TD_TRUE);
    hdmi_reg_ac_ctrl_srst_req_set(TD_TRUE);
    hdmi_reg_ac_ctrl_bus_srst_req_set(TD_TRUE);
    /* close all clk */
    drv_hdmi_prod_crg_gate_set(TD_FALSE);
    hdmi_reg_crg_deinit();

    return;
}
