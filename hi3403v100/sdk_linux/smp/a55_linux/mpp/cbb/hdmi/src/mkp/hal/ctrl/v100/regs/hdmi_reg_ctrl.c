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
#include "hdmi_reg_ctrl.h"
#include "hdmi_product_define.h"

volatile hdmi_reg_tx_ctrl *g_tx_ctrl_regs = NULL;

int hdmi_reg_tx_ctrl_regs_init(const td_char *addr)
{
    g_tx_ctrl_regs = (volatile hdmi_reg_tx_ctrl *)(addr + HDMI_TX_BASE_ADDR_CTRL);
    return TD_SUCCESS;
}

int hdmi_reg_tx_ctrl_regs_deinit(void)
{
    if (g_tx_ctrl_regs != TD_NULL) {
        g_tx_ctrl_regs = TD_NULL;
    }
    return TD_SUCCESS;
}

td_void hdmi_reg_tx_afifo_srst_req_set(td_u8 tx_afifo_srst_req)
{
    td_u32 *reg_addr = NULL;
    tx_pwd_rst_ctrl tx_pwd_rst;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->pwd_rst_ctrl.u32);
    tx_pwd_rst.u32 = hdmi_tx_reg_read(reg_addr);
    tx_pwd_rst.bits.tx_afifo_srst_req = tx_afifo_srst_req;
    hdmi_tx_reg_write(reg_addr, tx_pwd_rst.u32);

    return;
}

td_void hdmi_reg_tx_acr_srst_req_set(td_u8 tx_acr_srst_req)
{
    td_u32 *reg_addr = NULL;
    tx_pwd_rst_ctrl tx_pwd_rst;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->pwd_rst_ctrl.u32);
    tx_pwd_rst.u32 = hdmi_tx_reg_read(reg_addr);
    tx_pwd_rst.bits.tx_acr_srst_req = tx_acr_srst_req;
    hdmi_tx_reg_write(reg_addr, tx_pwd_rst.u32);

    return;
}

td_void hdmi_reg_tx_aud_srst_req_set(td_u8 tx_aud_srst_req)
{
    td_u32 *reg_addr = NULL;
    tx_pwd_rst_ctrl tx_pwd_rst;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->pwd_rst_ctrl.u32);
    tx_pwd_rst.u32 = hdmi_tx_reg_read(reg_addr);
    tx_pwd_rst.bits.tx_aud_srst_req = tx_aud_srst_req;
    hdmi_tx_reg_write(reg_addr, tx_pwd_rst.u32);

    return;
}

td_void hdmi_reg_tx_hdmi_srst_req_set(td_u8 tx_hdmi_srst_req)
{
    td_u32 *reg_addr = NULL;
    tx_pwd_rst_ctrl tx_pwd_rst;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->pwd_rst_ctrl.u32);
    tx_pwd_rst.u32 = hdmi_tx_reg_read(reg_addr);
    tx_pwd_rst.bits.tx_hdmi_srst_req = tx_hdmi_srst_req;
    hdmi_tx_reg_write(reg_addr, tx_pwd_rst.u32);

    return;
}

td_void hdmi_reg_pwd_fifo_data_in_set(td_u8 pwd_fifo_data_in)
{
    td_u32 *reg_addr = NULL;
    pwd_fifo_wdata fifo_wdata;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->wdata.u32);
    fifo_wdata.u32 = hdmi_tx_reg_read(reg_addr);
    fifo_wdata.bits.pwd_fifo_data_in = pwd_fifo_data_in;
    hdmi_tx_reg_write(reg_addr, fifo_wdata.u32);

    return;
}

td_void hdmi_reg_pwd_data_out_cnt_set(td_u16 pwd_data_out_cnt)
{
    td_u32 *reg_addr = NULL;
    pwd_data_cnt data_cnt;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->data_cnt.u32);
    data_cnt.u32 = hdmi_tx_reg_read(reg_addr);
    data_cnt.bits.pwd_data_out_cnt = pwd_data_out_cnt;
    hdmi_tx_reg_write(reg_addr, data_cnt.u32);

    return;
}

td_void hdmi_reg_pwd_slave_seg_set(td_u8 pwd_slave_seg)
{
    td_u32 *reg_addr = NULL;
    pwd_slave_cfg cfg;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->slave_cfg.u32);
    cfg.u32 = hdmi_tx_reg_read(reg_addr);
    cfg.bits.pwd_slave_seg = pwd_slave_seg;
    hdmi_tx_reg_write(reg_addr, cfg.u32);

    return;
}

td_void hdmi_reg_pwd_slave_offset_set(td_u8 pwd_slave_offset)
{
    td_u32 *reg_addr = NULL;
    pwd_slave_cfg cfg;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->slave_cfg.u32);
    cfg.u32 = hdmi_tx_reg_read(reg_addr);
    cfg.bits.pwd_slave_offset = pwd_slave_offset;
    hdmi_tx_reg_write(reg_addr, cfg.u32);

    return;
}

td_void hdmi_reg_pwd_slave_addr_set(td_u8 pwd_slave_addr)
{
    td_u32 *reg_addr = NULL;
    pwd_slave_cfg cfg;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->slave_cfg.u32);
    cfg.u32 = hdmi_tx_reg_read(reg_addr);
    cfg.bits.pwd_slave_addr = pwd_slave_addr;
    hdmi_tx_reg_write(reg_addr, cfg.u32);

    return;
}

td_void hdmi_reg_pwd_mst_cmd_set(td_u8 mst_cmd)
{
    td_u32 *reg_addr = NULL;
    pwd_mst_cmd cmd;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->mst_cmd.u32);
    cmd.u32 = hdmi_tx_reg_read(reg_addr);
    cmd.bits.pwd_mst_cmd = mst_cmd;
    hdmi_tx_reg_write(reg_addr, cmd.u32);

    return;
}

td_void hdmi_reg_cpu_ddc_req_set(td_u8 cpu_ddc_req)
{
    td_u32 *reg_addr = NULL;
    ddc_mst_arb_reql arb_req;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->ddc_arb_req.u32);
    arb_req.u32 = hdmi_tx_reg_read(reg_addr);
    arb_req.bits.cpu_ddc_req = cpu_ddc_req;
    hdmi_tx_reg_write(reg_addr, arb_req.u32);

    return;
}

td_u8 hdmi_reg_rdata_pwd_fifo_data_out_get(void)
{
    td_u32 *reg_addr = NULL;
    pwd_fifo_rdata fifo_rdata;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->rdata.u32);
    fifo_rdata.u32 = hdmi_tx_reg_read(reg_addr);
    return fifo_rdata.bits.pwd_fifo_data_out;
}

td_u8 hdmi_reg_pwd_fifo_data_out_get(void)
{
    td_u32 *reg_addr = NULL;
    pwd_data_cnt data_cnt;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->data_cnt.u32);
    data_cnt.u32 = hdmi_tx_reg_read(reg_addr);
    return data_cnt.bits.pwd_fifo_data_cnt;
}

td_u8 hdmi_reg_pwd_fifo_empty_get(void)
{
    td_u32 *reg_addr = NULL;
    pwd_mst_state mst_state;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->mst_state.u32);
    mst_state.u32 = hdmi_tx_reg_read(reg_addr);
    return mst_state.bits.pwd_fifo_empty;
}

td_u8 hdmi_reg_pwd_i2c_in_prog_get(void)
{
    td_u32 *reg_addr = NULL;
    pwd_mst_state mst_state;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->mst_state.u32);
    mst_state.u32 = hdmi_tx_reg_read(reg_addr);
    return mst_state.bits.pwd_i2c_in_prog;
}

td_u8 hdmi_reg_cpu_ddc_req_ack_get(void)
{
    td_u32 *reg_addr = NULL;
    ddc_mst_arb_ack arb_ack;

    reg_addr = (td_u32 *)&(g_tx_ctrl_regs->ddc_arb_ack.u32);
    arb_ack.u32 = hdmi_tx_reg_read(reg_addr);
    return arb_ack.bits.cpu_ddc_req_ack;
}

