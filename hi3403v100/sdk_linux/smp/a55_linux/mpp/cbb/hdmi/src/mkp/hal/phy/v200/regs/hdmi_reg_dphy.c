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

static volatile hdmitx21_dphy_reg_type *g_hdmitx_dphy_regs[HDMI_ID_MAX] = {TD_NULL};

td_s32 hdmi_reg_tx_phy_init(td_u32 id, td_char *addr)
{
    g_hdmitx_dphy_regs[id] = (hdmitx21_dphy_reg_type *)(addr);
    return TD_SUCCESS;
}

td_u32 *hdmi_reg_tx_get_phy_addr(td_u32 id)
{
    return (td_u32 *)g_hdmitx_dphy_regs[id];
}

td_s32 hdmi_reg_tx_phy_deinit(td_u32 id)
{
    if (g_hdmitx_dphy_regs[id] != TD_NULL) {
        g_hdmitx_dphy_regs[id] = TD_NULL;
    }
    return TD_SUCCESS;
}

static td_void hdmi21_tx_reg_write(td_u32 *reg_addr, td_u32 value)
{
    *(volatile td_u32 *)reg_addr = value;
    return;
}

static td_u32 hdmi21_tx_reg_read(const td_u32 *reg_addr)
{
    return *(volatile td_u32 *)(reg_addr);
}

td_void hdmi_reg_stb_cs_en_set(td_u32 id, td_u16 stb_cs_en)
{
    td_u32 *reg_addr = NULL;
    phy_csen csen;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->stb_cs_en.u32);
    csen.u32 = hdmi21_tx_reg_read(reg_addr);
    csen.bits.stb_cs_en = stb_cs_en;
    hdmi21_tx_reg_write(reg_addr, csen.u32);

    return;
}

td_void hdmi_reg_stb_wen_set(td_u32 id, td_u8 stb_wen)
{
    td_u32 *reg_addr = NULL;
    phy_wr tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->stb_write_en.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.stb_wen = stb_wen;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_resetn_set(td_u32 id, td_u8 reg_resetn)
{
    td_u32 *reg_addr = NULL;
    resetn tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->stb_reset.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.resetn = reg_resetn;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_u8 hdmi_reg_resetn_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    resetn tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->stb_reset.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);

    return tmp.bits.resetn;
}

td_void hdmi_reg_src_enable_set(td_u32 id, td_u8 src_enable)
{
    td_u32 *reg_addr = NULL;
    fdsrcparam tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fd_src_param.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.src_enable = src_enable;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_stb_wdata_set(td_u32 id, td_u8 stb_wdata)
{
    td_u32 *reg_addr = NULL;
    phy_wdata wdata;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->stb_wdata.u32);
    wdata.u32 = hdmi21_tx_reg_read(reg_addr);
    wdata.bits.stb_wdata = stb_wdata;
    hdmi21_tx_reg_write(reg_addr, wdata.u32);

    return;
}

td_void hdmi_reg_fcg_lock_en_set(td_u32 id, td_u8 up_fcg_lock_en)
{
    td_u32 *reg_addr = NULL;
    fcgset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fcg_set.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.fcg_lock_en = up_fcg_lock_en;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_stb_addr_set(td_u32 id, td_u8 stb_addr)
{
    td_u32 *reg_addr = NULL;
    phy_addr tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->stb_addr.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.stb_addr = stb_addr;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_u8 hdmi_reg_stb_rdata_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    phy_rdata rdata;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->stb_rdata.u32);
    rdata.u32 = hdmi21_tx_reg_read(reg_addr);

    return rdata.bits.stb_rdata;
}

td_void hdmi_reg_src_lock_cnt_set(td_u32 id, td_u8 src_lock_cnt)
{
    td_u32 *reg_addr = NULL;
    fdsrcparam tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fd_src_param.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.src_lock_cnt = src_lock_cnt;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_src_lock_val_set(td_u32 id, td_u8 src_lock_val)
{
    td_u32 *reg_addr = NULL;
    fdsrcparam tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fd_src_param.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.src_lock_val = src_lock_val;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_src_freq_ext_set(td_u32 id, td_u16 src_freq_ext)
{
    td_u32 *reg_addr = NULL;
    fdsrcfreq tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fd_src_freq.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.src_freq_ext = src_freq_ext;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_fdsrcfreq_unused2_set(td_u32 id, td_u8 fdsrcfreq_unused_2)
{
    td_u32 *reg_addr = NULL;
    fdsrcfreq tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fd_src_freq.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.fdsrcfreq_unused_2 = fdsrcfreq_unused_2;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_txfifoset0_unused_set(td_u32 id, td_u8 txfifoset0_unused)
{
    td_u32 *reg_addr = NULL;
    txfifoset0 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->tx_fifo_set0.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.txfifoset0_unused = txfifoset0_unused;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_src_freq_opt_set(td_u32 id, td_u8 src_freq_opt)
{
    td_u32 *reg_addr = NULL;
    fdsrcfreq tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fd_src_freq.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.src_freq_opt = src_freq_opt;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_fdsrcfreq_unused1_set(td_u32 id, td_u8 fdsrcfreq_unused_1)
{
    td_u32 *reg_addr = NULL;
    fdsrcfreq tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fd_src_freq.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.fdsrcfreq_unused_1 = fdsrcfreq_unused_1;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_src_cnt_opt_set(td_u32 id, td_u8 src_cnt_opt)
{
    td_u32 *reg_addr = NULL;
    fdsrcfreq tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fd_src_freq.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.src_cnt_opt = src_cnt_opt;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_u32 hdmi_reg_src_cnt_out_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    fdsrcres tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fd_src_res.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);

    return tmp.bits.src_cnt_out;
}

td_u8 hdmi_reg_src_det_stat_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    fdsrcres tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fd_src_res.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);

    return tmp.bits.src_det_stat;
}

td_void hdmi_reg_clkdet_sel_set(td_u32 id, td_u8 i_clkdet_sel)
{
    td_u32 *reg_addr = NULL;
    fcopt tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fc_opt.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.i_clkdet_sel = i_clkdet_sel;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_up_sampler_ratio_sel_set(td_u32 id, td_u8 up_sampler_ratio_sel)
{
    td_u32 *reg_addr = NULL;
    fcdstepsetl tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fc_dstep_set.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.up_sampler_ratio_sel = up_sampler_ratio_sel;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_fcdstepset_unused_set(td_u32 id, td_u8 fcdstepset_unused)
{
    td_u32 *reg_addr = NULL;
    fcdstepsetl tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fc_dstep_set.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.fcdstepset_unused = fcdstepset_unused;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_divn_h20_set(td_u32 id, td_u8 up_divn_h20)
{
    td_u32 *reg_addr = NULL;
    fcdstepsetl tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fc_dstep_set.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.divn_h20 = up_divn_h20;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_en_sdm_set(td_u32 id, td_u8 en_sdm)
{
    td_u32 *reg_addr = NULL;
    fdivset0 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_set0.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sdm = en_sdm;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_mode_en_set(td_u32 id, td_u8 reg_hdmi_mode_en)
{
    td_u32 *reg_addr = NULL;
    hdmi_mode mode;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->stb_hdmi_mode.u32);
    mode.u32 = hdmi21_tx_reg_read(reg_addr);
    mode.bits.reg_hdmi_mode_en = reg_hdmi_mode_en;
    hdmi21_tx_reg_write(reg_addr, mode.u32);

    return;
}

td_u8 hdmi_reg_en_sdm_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    fdivset0 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_set0.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);

    return tmp.bits.sdm;
}

td_void hdmi_reg_en_mod_set(td_u32 id, td_u8 en_mod)
{
    td_u32 *reg_addr = NULL;
    fdivset0 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_set0.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.mod = en_mod;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_u8 hdmi_reg_en_mod_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    fdivset0 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_set0.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    return tmp.bits.mod;
}

td_void hdmi_reg_en_ctrl_set(td_u32 id, td_u8 en_ctrl)
{
    td_u32 *reg_addr = NULL;
    fdivset0 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_set0.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.ctrl = en_ctrl;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_u8 hdmi_reg_en_ctrl_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    fdivset0 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_set0.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);

    return tmp.bits.ctrl;
}

td_void hdmi_reg_init_set(td_u32 id, td_u8 init)
{
    td_u32 *reg_addr = NULL;
    fdivset0 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_set0.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.init = init;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_mod_n_set(td_u32 id, td_u16 mod_n)
{
    td_u32 *reg_addr = NULL;
    fdivset3 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_set3.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.mod_n = mod_n;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_mod_t_set(td_u32 id, td_u8 mod_t)
{
    td_u32 *reg_addr = NULL;
    fdivset3 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_set3.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.mod_t = mod_t;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_mod_len_set(td_u32 id, td_u8 mod_len)
{
    td_u32 *reg_addr = NULL;
    fdivset3 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_set3.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.mod_len = mod_len;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_mod_d_set(td_u32 id, td_u16 mod_d)
{
    td_u32 *reg_addr = NULL;
    fdivset4 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_set4.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.mod_d = mod_d;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_fdsrcparam_unused_set(td_u32 id, td_u8 fdsrcparam_unused)
{
    td_u32 *reg_addr = NULL;
    fdsrcparam tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fd_src_param.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.fdsrcparam_unused = fdsrcparam_unused;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_fdiv_in_set(td_u32 id, td_u32 i_fdiv_in)
{
    td_u32 *reg_addr = NULL;
    fdivs_tat1 fdivstat1;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_stat1.u32);
    fdivstat1.u32 = hdmi21_tx_reg_read(reg_addr);
    fdivstat1.bits.i_fdiv_in = i_fdiv_in;
    hdmi21_tx_reg_write(reg_addr, fdivstat1.u32);

    return;
}

td_void hdmi_reg_mdiv_set(td_u32 id, td_u8 i_mdiv)
{
    td_u32 *reg_addr = NULL;
    fdivmanual tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_manual.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.i_mdiv = i_mdiv;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_frl_clock_set(td_u32 id, td_u8 sw_reset_frl_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_frl_clock = sw_reset_frl_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_manual_en_set(td_u32 id, td_u8 i_manual_en)
{
    td_u32 *reg_addr = NULL;
    fdivmanual tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fdiv_manual.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.i_manual_en = i_manual_en;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_stb_delay1_set(td_u32 id, td_u8 stb_delay1)
{
    td_u32 *reg_addr = NULL;
    stb_opt stbopt;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->opt.u32);
    stbopt.u32 = hdmi21_tx_reg_read(reg_addr);
    stbopt.bits.stb_delay1 = stb_delay1;
    hdmi21_tx_reg_write(reg_addr, stbopt.u32);

    return;
}

td_void hdmi_reg_ref_clk_sel_set(td_u32 id, td_u8 i_ref_clk_sel)
{
    td_u32 *reg_addr = NULL;
    refclksel tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->ref_clk_sel.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.i_ref_clk_sel = i_ref_clk_sel;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_fcg_dither_en_set(td_u32 id, td_u8 up_fcg_dither_en)
{
    td_u32 *reg_addr = NULL;
    fcgset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fcg_set.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.fcg_dither_en = up_fcg_dither_en;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_fcg_dlf_en_set(td_u32 id, td_u8 up_fcg_dlf_en)
{
    td_u32 *reg_addr = NULL;
    fcgset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fcg_set.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.fcg_dlf_en = up_fcg_dlf_en;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_fcg_en_set(td_u32 id, td_u8 up_fcg_en)
{
    td_u32 *reg_addr = NULL;
    fcgset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->fcg_set.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.fcg_en = up_fcg_en;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_enable_h20_set(td_u32 id, td_u8 up_enable_h20)
{
    td_u32 *reg_addr = NULL;
    txfifoset0 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->tx_fifo_set0.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.enable_h20 = up_enable_h20;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_pr_en_h20_set(td_u32 id, td_u8 up_pr_en_h20)
{
    td_u32 *reg_addr = NULL;
    txfifoset0 tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->tx_fifo_set0.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.pr_en_h20 = up_pr_en_h20;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_ch_out_sel_set(td_u32 id, td_u8 up_ch_out_sel)
{
    td_u32 *reg_addr = NULL;
    tx_data_out_sel txdataoutsel;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->data_out_sel.u32);
    txdataoutsel.u32 = hdmi21_tx_reg_read(reg_addr);
    txdataoutsel.bits.ch_out_sel = up_ch_out_sel;
    hdmi21_tx_reg_write(reg_addr, txdataoutsel.u32);

    return;
}

td_void hdmi_reg_hsset_set(td_u32 id, td_u8 up_hsset)
{
    td_u32 *reg_addr = NULL;
    hsset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->hs_set.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.hsset = up_hsset;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_global_reset_set(td_u32 id, td_u8 global_reset)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.global_reset = global_reset;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_swreset_unused_set(td_u32 id, td_u16 swreset_unused)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.swreset_unused = swreset_unused;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_dac_clock_gat_set(td_u32 id, td_u8 dac_clock_gat)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.dac_clock_gat = dac_clock_gat;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_stb_delay2_set(td_u32 id, td_u8 stb_delay2)
{
    td_u32 *reg_addr = NULL;
    stb_opt stbopt;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->opt.u32);
    stbopt.u32 = hdmi21_tx_reg_read(reg_addr);
    stbopt.bits.stb_delay2 = stb_delay2;
    hdmi21_tx_reg_write(reg_addr, stbopt.u32);

    return;
}

td_void hdmi_reg_stb_cs_sel_set(td_u32 id, td_u8 stb_cs_sel)
{
    td_u32 *reg_addr = NULL;
    stb_opt stbopt;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->opt.u32);
    stbopt.u32 = hdmi21_tx_reg_read(reg_addr);
    stbopt.bits.stb_cs_sel = stb_cs_sel;
    hdmi21_tx_reg_write(reg_addr, stbopt.u32);

    return;
}

td_void hdmi_reg_stb_acc_sel_set(td_u32 id, td_u8 stb_acc_sel)
{
    td_u32 *reg_addr = NULL;
    stb_opt stbopt;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->opt.u32);
    stbopt.u32 = hdmi21_tx_reg_read(reg_addr);
    stbopt.bits.stb_acc_sel = stb_acc_sel;
    hdmi21_tx_reg_write(reg_addr, stbopt.u32);

    return;
}

td_void hdmi_reg_stb_delay0_set(td_u32 id, td_u8 stb_delay0)
{
    td_u32 *reg_addr = NULL;
    stb_opt stbopt;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->opt.u32);
    stbopt.u32 = hdmi21_tx_reg_read(reg_addr);
    stbopt.bits.stb_delay0 = stb_delay0;
    hdmi21_tx_reg_write(reg_addr, stbopt.u32);

    return;
}

td_void hdmi_reg_fd_clk_sel_set(td_u32 id, td_u8 up_fd_clk_sel)
{
    td_u32 *reg_addr = NULL;
    clk_set clkset;
    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->clk_set.u32);
    clkset.u32 = hdmi21_tx_reg_read(reg_addr);
    clkset.bits.fd_clk_sel = up_fd_clk_sel;
    hdmi21_tx_reg_write(reg_addr, clkset.u32);

    return;
}

td_void hdmi_reg_refclk_sel_set(td_u32 id, td_u8 up_refclk_sel)
{
    td_u32 *reg_addr = NULL;
    clk_set clkset;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->clk_set.u32);
    clkset.u32 = hdmi21_tx_reg_read(reg_addr);
    clkset.bits.refclk_sel = up_refclk_sel;
    hdmi21_tx_reg_write(reg_addr, clkset.u32);

    return;
}

td_void hdmi_reg_ctman_set(td_u32 id, td_u8 up_ctman)
{
    td_u32 *reg_addr = NULL;
    clk_set clkset;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->clk_set.u32);
    clkset.u32 = hdmi21_tx_reg_read(reg_addr);
    clkset.bits.ctman = up_ctman;
    hdmi21_tx_reg_write(reg_addr, clkset.u32);

    return;
}

td_void hdmi_reg_req_length_set(td_u32 id, td_u8 req_length)
{
    td_u32 *reg_addr = NULL;
    stb_opt stbopt;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->opt.u32);
    stbopt.u32 = hdmi21_tx_reg_read(reg_addr);
    stbopt.bits.req_length = req_length;
    hdmi21_tx_reg_write(reg_addr, stbopt.u32);

    return;
}

td_void hdmi_reg_fdivclk_sel_set(td_u32 id, td_u8 up_fdivclk_sel)
{
    td_u32 *reg_addr = NULL;
    clk_set clkset;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->clk_set.u32);
    clkset.u32 = hdmi21_tx_reg_read(reg_addr);
    clkset.bits.fdivclk_sel = up_fdivclk_sel;
    hdmi21_tx_reg_write(reg_addr, clkset.u32);

    return;
}

td_void hdmi_reg_mod_div_val_set(td_u32 id, td_u8 mod_div_val)
{
    td_u32 *reg_addr = NULL;
    clk_set clkset;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->clk_set.u32);
    clkset.u32 = hdmi21_tx_reg_read(reg_addr);
    clkset.bits.mod_div_val = mod_div_val;
    hdmi21_tx_reg_write(reg_addr, clkset.u32);

    return;
}

td_void hdmi_reg_modclk_sel_set(td_u32 id, td_u8 up_modclk_sel)
{
    td_u32 *reg_addr = NULL;
    clk_set clkset;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->clk_set.u32);
    clkset.u32 = hdmi21_tx_reg_read(reg_addr);
    clkset.bits.modclk_sel = up_modclk_sel;
    hdmi21_tx_reg_write(reg_addr, clkset.u32);

    return;
}

td_void hdmi_reg_sw_reset_mod_clock_set(td_u32 id, td_u8 sw_reset_mod_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_mod_clock = sw_reset_mod_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_tmds_clock_set(td_u32 id, td_u8 sw_reset_tmds_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_tmds_clock = sw_reset_tmds_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_mpll_clock_set(td_u32 id, td_u8 sw_reset_mpll_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_mpll_clock = sw_reset_mpll_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_nco_clock_set(td_u32 id, td_u8 sw_reset_nco_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_nco_clock = sw_reset_nco_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_fd_clock_set(td_u32 id, td_u8 sw_reset_fd_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_fd_clock = sw_reset_fd_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_mod_and_mpll_clock_set(td_u32 id, td_u8 sw_reset_mod_and_mpll_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_mod_and_mpll_clock = sw_reset_mod_and_mpll_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_mod_and_nco_clock_set(td_u32 id, td_u8 sw_reset_mod_and_nco_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_mod_and_nco_clock = sw_reset_mod_and_nco_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_mod_and_fd_clock_set(td_u32 id, td_u8 sw_reset_mod_and_fd_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_mod_and_fd_clock = sw_reset_mod_and_fd_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_hsfifo_clock_set(td_u32 id, td_u8 sw_reset_hsfifo_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_hsfifo_clock = sw_reset_hsfifo_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_txfifo_clock_set(td_u32 id, td_u8 sw_reset_txfifo_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_txfifo_clock = sw_reset_txfifo_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_data_clock_set(td_u32 id, td_u8 sw_reset_data_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_data_clock = sw_reset_data_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_hs_clock_set(td_u32 id, td_u8 sw_reset_hs_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_hs_clock = sw_reset_hs_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_pllref_clock_set(td_u32 id, td_u8 sw_reset_pllref_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_pllref_clock = sw_reset_pllref_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_sw_reset_dac_clock_set(td_u32 id, td_u8 sw_reset_dac_clock)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.sw_reset_dac_clock = sw_reset_dac_clock;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

td_void hdmi_reg_up_sample_fifo_clock_swrst_set(td_u32 id, td_u8 up_sample_fifo_clock_swrst)
{
    td_u32 *reg_addr = NULL;
    sw_reset tmp;

    reg_addr = (td_u32 *)&(g_hdmitx_dphy_regs[id]->sw_rst.u32);
    tmp.u32 = hdmi21_tx_reg_read(reg_addr);
    tmp.bits.up_sample_fifo_clock_swrst = up_sample_fifo_clock_swrst;
    hdmi21_tx_reg_write(reg_addr, tmp.u32);

    return;
}

