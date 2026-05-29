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
#include "hdmi_reg_video_path.h"
#include "hdmi_product_define.h"

volatile video_path_reg_regs_type *g_video_path_regs[HDMI_ID_MAX] = {TD_NULL};

int hdmi_reg_video_path_regs_init(td_u32 id, const td_char *addr)
{
    g_video_path_regs[id] = (volatile video_path_reg_regs_type *)(addr + HDMI_TX_BASE_ADDR_VIDEO);
    return TD_SUCCESS;
}

int hdmi_reg_video_path_regs_deinit(td_u32 id)
{
    if (g_video_path_regs[id] != TD_NULL) {
        g_video_path_regs[id] = TD_NULL;
    }
    return TD_SUCCESS;
}

td_void hdmi_reg_video_blank_en_set(td_u32 id, td_u8 reg_video_blank_en)
{
    td_u32 *reg_addr = NULL;
    video_path_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->vid_path_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_video_blank_en = reg_video_blank_en;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_u8 hdmi_reg_video_blank_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    video_path_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->vid_path_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    return ctrl.bits.reg_video_blank_en;
}

td_void hdmi_reg_solid_pattern_en_set(td_u32 id, td_u8 solid_pattern_en)
{
    td_u32 *reg_addr = NULL;
    pattern_gen_ctrll pattern_gen_ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->ptn_gen_ctrl.u32);
    pattern_gen_ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    pattern_gen_ctrl.bits.solid_pattern_en = solid_pattern_en;
    hdmi_tx_reg_write(reg_addr, pattern_gen_ctrl.u32);

    return;
}

td_u8 hdmi_reg_solid_pattern_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    pattern_gen_ctrll pattern_gen_ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->ptn_gen_ctrl.u32);
    pattern_gen_ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    return pattern_gen_ctrl.bits.solid_pattern_en;
}

td_void hdmi_reg_solid_pattern_cr_set(td_u32 id, td_u16 solid_pattern_cr)
{
    td_u32 *reg_addr = NULL;
    solid_pattern_config config;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->solid_ptn_ctrl.u32);
    config.u32 = hdmi_tx_reg_read(reg_addr);
    config.bits.solid_pattern_cr = solid_pattern_cr;
    hdmi_tx_reg_write(reg_addr, config.u32);

    return;
}

td_void hdmi_reg_solid_pattern_y_set(td_u32 id, td_u16 solid_pattern_y)
{
    td_u32 *reg_addr = NULL;
    solid_pattern_config config;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->solid_ptn_ctrl.u32);
    config.u32 = hdmi_tx_reg_read(reg_addr);
    config.bits.solid_pattern_y = solid_pattern_y;
    hdmi_tx_reg_write(reg_addr, config.u32);

    return;
}

td_void hdmi_reg_solid_pattern_cb_set(td_u32 id, td_u16 solid_pattern_cb)
{
    td_u32 *reg_addr = NULL;
    solid_pattern_config config;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->solid_ptn_ctrl.u32);
    config.u32 = hdmi_tx_reg_read(reg_addr);
    config.bits.solid_pattern_cb = solid_pattern_cb;
    hdmi_tx_reg_write(reg_addr, config.u32);

    return;
}

td_void hdmi_reg_fdt_status_clear_set(td_u32 id, td_u8 fdt_status_clear)
{
    td_u32 *reg_addr = NULL;
    format_det_config config;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->fmt_det_cfg.u32);
    config.u32 = hdmi_tx_reg_read(reg_addr);
    config.bits.fdt_status_clear = fdt_status_clear;
    hdmi_tx_reg_write(reg_addr, config.u32);

    return;
}

td_void hdmi_reg_sync_polarity_force_set(td_u32 id, td_u8 sync_polarity_force)
{
    td_u32 *reg_addr = NULL;
    format_det_config config;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->fmt_det_cfg.u32);
    config.u32 = hdmi_tx_reg_read(reg_addr);
    config.bits.sync_polarity_force = sync_polarity_force;
    hdmi_tx_reg_write(reg_addr, config.u32);

    return;
}

td_u8 hdmi_reg_vsync_polarity_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    fdet_status status;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->fmt_dect_status.u32);
    status.u32 = hdmi_tx_reg_read(reg_addr);

    return status.bits.vsync_polarity;
}

td_u8 hdmi_reg_hsync_polarity_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    fdet_status status;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->fmt_dect_status.u32);
    status.u32 = hdmi_tx_reg_read(reg_addr);

    return status.bits.hsync_polarity;
}

td_u8 hdmi_reg_interlaced_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    fdet_status status;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->fmt_dect_status.u32);
    status.u32 = hdmi_tx_reg_read(reg_addr);

    return status.bits.interlaced;
}

td_u16 hdmi_reg_hsync_total_cnt_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    fdet_hori_res tmp;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->fmt_dect_hori_res.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);

    return tmp.bits.hsync_total_cnt;
}

td_u16 hdmi_reg_hsync_active_cnt_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    fdet_hori_res tmp;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->fmt_dect_hori_res.u32);
    tmp.u32 = hdmi_tx_reg_read(reg_addr);
    return tmp.bits.hsync_active_cnt;
}

td_u16 hdmi_reg_vsync_total_cnt_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    fdet_hori_vert_res fdet_vert_res;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->fmt_dect_hori_vert_res.u32);
    fdet_vert_res.u32 = hdmi_tx_reg_read(reg_addr);
    return fdet_vert_res.bits.vsync_total_cnt;
}

td_u16 hdmi_reg_vsync_active_cnt_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    fdet_hori_vert_res fdet_vert_res;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->fmt_dect_hori_vert_res.u32);
    fdet_vert_res.u32 = hdmi_tx_reg_read(reg_addr);

    return fdet_vert_res.bits.vsync_active_cnt;
}

td_u8 hdmi_reg_dwsm_vert_bypass_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    video_dwsm_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dwsm_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);

    return ctrl.bits.reg_dwsm_vert_byp;
}

td_u8 hdmi_reg_dwsm_vert_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    video_dwsm_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dwsm_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);

    return ctrl.bits.reg_dwsm_vert_en;
}

td_u8 hdmi_reg_hori_filter_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;

    video_dwsm_ctrl ctrl;
    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dwsm_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    return ctrl.bits.reg_hori_filter_en;
}

td_u8 hdmi_reg_dwsm_hori_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    video_dwsm_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dwsm_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);

    return ctrl.bits.reg_dwsm_hori_en;
}

td_u8 hdmi_reg_pxl_div_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    data_align_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->align_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);

    return ctrl.bits.reg_pxl_div_en;
}

td_u8 hdmi_reg_demux_420_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    data_align_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->align_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);

    return ctrl.bits.reg_demux_420_en;
}

td_u8 hdmi_reg_inver_sync_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    video_dmux_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dmux_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);

    return ctrl.bits.reg_inver_sync;
}

td_u8 hdmi_reg_vmux_cr_sel_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    video_dmux_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dmux_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);

    return ctrl.bits.reg_vmux_cr_sel;
}

td_u8 hdmi_reg_vmux_cb_sel_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    video_dmux_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dmux_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);

    return ctrl.bits.reg_vmux_cb_sel;
}

td_u8 hdmi_reg_vmux_y_sel_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    video_dmux_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dmux_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);

    return ctrl.bits.reg_vmux_y_sel;
}

td_u8 hdmi_reg_dither_mode_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    dither_config config;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dither_cfg.u32);
    config.u32 = hdmi_tx_reg_read(reg_addr);

    return config.bits.dither_mode;
}

td_u8 hdmi_reg_dither_rnd_bypass_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    dither_config config;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dither_cfg.u32);
    config.u32 = hdmi_tx_reg_read(reg_addr);

    return config.bits.dither_rnd_byp;
}

td_u8 hdmi_reg_csc_en_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    multi_csc_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->csc_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);

    return ctrl.bits.reg_csc_en;
}

td_u8 hdmi_reg_csc_mode_get(td_u32 id)
{
    td_u32 *reg_addr = NULL;
    multi_csc_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->csc_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    return ctrl.bits.reg_csc_mode;
}

td_void hdmi_reg_dither_mode_set(td_u32 id, td_u8 dither_mode)
{
    td_u32 *reg_addr = NULL;
    dither_config config;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dither_cfg.u32);
    config.u32 = hdmi_tx_reg_read(reg_addr);
    config.bits.dither_mode = dither_mode;
    hdmi_tx_reg_write(reg_addr, config.u32);

    return;
}

td_void hdmi_reg_dither_rnd_bypass_set(td_u32 id, td_u8 dither_rnd_byp)
{
    td_u32 *reg_addr = NULL;
    dither_config config;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dither_cfg.u32);
    config.u32 = hdmi_tx_reg_read(reg_addr);
    config.bits.dither_rnd_byp = dither_rnd_byp;
    hdmi_tx_reg_write(reg_addr, config.u32);

    return;
}

td_void hdmi_reg_csc_mode_set(td_u32 id, td_u8 reg_csc_mode)
{
    td_u32 *reg_addr = NULL;
    multi_csc_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->csc_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_csc_mode = reg_csc_mode;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_void hdmi_reg_csc_saturate_en_set(td_u32 id, td_u8 reg_csc_saturate_en)
{
    td_u32 *reg_addr = NULL;
    multi_csc_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->csc_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_csc_saturate_en = reg_csc_saturate_en;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_void hdmi_reg_csc_en_set(td_u32 id, td_u8 reg_csc_en)
{
    td_u32 *reg_addr = NULL;
    multi_csc_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->csc_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_csc_en = reg_csc_en;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_void hdmi_reg_dwsm_vert_bypass_set(td_u32 id, td_u8 reg_dwsm_vert_byp)
{
    td_u32 *reg_addr = NULL;
    video_dwsm_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dwsm_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_dwsm_vert_byp = reg_dwsm_vert_byp;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_void hdmi_reg_dwsm_vert_en_set(td_u32 id, td_u8 reg_dwsm_vert_en)
{
    td_u32 *reg_addr = NULL;
    video_dwsm_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dwsm_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_dwsm_vert_en = reg_dwsm_vert_en;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_void hdmi_reg_hori_filter_en_set(td_u32 id, td_u8 reg_hori_filter_en)
{
    td_u32 *reg_addr = NULL;
    video_dwsm_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dwsm_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_hori_filter_en = reg_hori_filter_en;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_void hdmi_reg_dwsm_hori_en_set(td_u32 id, td_u8 reg_dwsm_hori_en)
{
    td_u32 *reg_addr = NULL;
    video_dwsm_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dwsm_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_dwsm_hori_en = reg_dwsm_hori_en;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_void hdmi_reg_pxl_div_en_set(td_u32 id, td_u8 reg_pxl_div_en)
{
    td_u32 *reg_addr = NULL;
    data_align_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->align_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_pxl_div_en = reg_pxl_div_en;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_void hdmi_reg_demux_420_en_set(td_u32 id, td_u8 reg_demux_420_en)
{
    td_u32 *reg_addr = NULL;
    data_align_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->align_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_demux_420_en = reg_demux_420_en;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_void hdmi_reg_inver_sync_set(td_u32 id, td_u8 reg_inver_sync)
{
    td_u32 *reg_addr = NULL;
    video_dmux_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dmux_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_inver_sync = reg_inver_sync;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_void hdmi_reg_syncmask_en_set(td_u32 id, td_u8 reg_syncmask_en)
{
    td_u32 *reg_addr = NULL;
    video_dmux_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dmux_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_syncmask_en = reg_syncmask_en;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_void hdmi_reg_vmux_cr_sel_set(td_u32 id, td_u8 reg_vmux_cr_sel)
{
    td_u32 *reg_addr = NULL;
    video_dmux_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dmux_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_vmux_cr_sel = reg_vmux_cr_sel;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_void hdmi_reg_vmux_cb_sel_set(td_u32 id, td_u8 reg_vmux_cb_sel)
{
    td_u32 *reg_addr = NULL;
    video_dmux_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dmux_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_vmux_cb_sel = reg_vmux_cb_sel;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

td_void hdmi_reg_vmux_y_sel_set(td_u32 id, td_u8 reg_vmux_y_sel)
{
    td_u32 *reg_addr = NULL;
    video_dmux_ctrl ctrl;

    reg_addr = (td_u32 *)&(g_video_path_regs[id]->dmux_ctrl.u32);
    ctrl.u32 = hdmi_tx_reg_read(reg_addr);
    ctrl.bits.reg_vmux_y_sel = reg_vmux_y_sel;
    hdmi_tx_reg_write(reg_addr, ctrl.u32);

    return;
}

