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
#ifndef HDMI_HAL_PHY_H
#define HDMI_HAL_PHY_H

#include "hdmi_hal_intf.h"
#include "drv_hdmi_common.h"

#define HDMI_TRACE_COUNT 10

typedef enum {
    PHY_RPRE_50,
    PHY_RPRE_56,
    PHY_RPRE_71,
    PHY_RPRE_83,
    PHY_RPRE_100,
    PHY_RPRE_125,
    PHY_RPRE_250,
    PHY_RPRE_500,
    PHY_RPRE_BUTT
} phy_rpre;

typedef enum {
    PHY_RTERM_MODE_SINGLE,
    PHY_RTERM_MODE_SOURCE,
    PHY_RTERM_MODE_LOAD,
    PHY_RTERM_MODE_BUTT
} phy_rterm_mode;

typedef struct {
    td_u32 ssc_amptd; /* 1/1M ppm(spread rate range) */
    td_u32 ssc_freq;  /* 1_hz(spread the frequency) */
} phy_ssc_cfg;

typedef struct {
    td_bool     ssc_enable;
    td_bool     ssc_debug_en;
    phy_ssc_cfg ssc_cfg;
} hdmi_phy_ssc;

typedef struct {
    td_u32          pix_clk;    /* pixel colck, in k_hz */
    td_u32          tmds_clk;   /* TMDS colck, in k_hz */
    hdmi_deep_color deep_color; /* deep color(color depth) */
    hdmi_phy_ssc    phy_ssc;    /* spread spectrum ctrl(ssc) para */
} hdmi_phy_ssc_cfg;

typedef struct {
    td_u32         i_main_clk;
    td_u32         i_main_d0;
    td_u32         i_main_d1;
    td_u32         i_main_d2;
    td_u32         i_pre_clk;
    td_u32         i_pre_d0;
    td_u32         i_pre_d1;
    td_u32         i_pre_d2;
    phy_rpre       r_pre_clk;
    phy_rpre       r_pre_d0;
    phy_rpre       r_pre_d1;
    phy_rpre       r_pre_d2;
    phy_rterm_mode r_term_mode_clk;
    td_u32         r_term_clk;
    phy_rterm_mode r_term_mode_d0;
    td_u32         r_term_d0;
    phy_rterm_mode r_term_mode_d1;
    td_u32         r_term_d1;
    phy_rterm_mode r_term_mode_d2;
    td_u32         r_term_d2;
} phy_hw_spec_cfg;

typedef struct {
    td_bool         hw_spec_debug_en;
    phy_hw_spec_cfg spec_cfg;
} hdmi_phy_hw_spec;

typedef struct {
    td_u32           tmds_clk;      /* TMDS colck, in k_hz */
    td_u8            frl_dat_rat;   /* FRL_DATA_RATA */
    hdmi_trace_len   trace_len;
    hdmi_phy_hw_spec hdmi_phy_spec; /* phy specification para */
} hdmi_phy_hw_spec_cfg;

typedef struct {
    td_u32            pixel_clk;
    td_u32            tmds_clk;   /* TMDS colck, in k_hz */
    td_bool           emi_enable;
    hdmi_deep_color   deep_color; /* deep color(color depth) */
    hdmi_phy_mode_cfg mode_cfg;   /* TMDS/FRL/tx_ffe */
    hdmi_trace_len    trace_len;
} hdmi_phy_tmds_cfg;

typedef struct {
    td_bool           init;
    td_bool           power_enable;
    td_bool           oe_enable;
    hdmi_phy_tmds_cfg tmds_cfg;
    hdmi_phy_ssc      ssc_cfg;
    phy_hw_spec_cfg   hw_spec_cfg;
    hdmi_hw_spec      spec_user[HDMI_TRACE_COUNT]; /* user set crg */
    hdmi_hw_spec      hw_spec[HDMI_TRACE_COUNT];   /* drv use now = chip def + use set */
} hdmi_phy_info;

typedef struct {
    hdmi_hw_spec  hw_spec_cfg;
    hdmi_hw_spec  hw_spec_def;   /* chip default cfg */
    hdmi_hw_param hw_param_cur;  /* reg cfg now */
    hdmi_hw_spec  spec_drv_use;  /* drv use now */
    hdmi_hw_spec  spec_user_set; /* user set cfg */
} hdmi_phy_hw_param;

typedef enum {
    APHY_OFFSET_0,
    APHY_OFFSET_1,
    APHY_OFFSET_2,
    APHY_OFFSET_3,
    APHY_OFFSET_4,
    APHY_OFFSET_5,
    APHY_OFFSET_6,
    APHY_OFFSET_7,
    APHY_OFFSET_8,
    APHY_OFFSET_9,
    APHY_OFFSET_A,
    APHY_OFFSET_B,
    APHY_OFFSET_C,
    APHY_OFFSET_D,
    APHY_OFFSET_E,
    APHY_OFFSET_F,
    APHY_OFFSET_BUTT
} aphy_offset_addr;

typedef struct {
    td_u32 cs;
    aphy_offset_addr aphy_offset;
    td_u8 msb;
    td_u8 lsb;
    td_u32 wdata;
} write_param;

struct dphy_spec_en {
    td_bool drv_post2_en;
    td_bool drv_post1_en;
    td_bool drv_pre_en;
};

struct dphy_spec_params {
    td_u8 drv_post2;
    td_u8 drv_post1;
    td_u8 drv_main;
    td_u8 drv_pre;
};

struct aphy_spec_params {
    td_u8 offset_0;
    td_u8 offset_1;
    td_u8 offset_2;
    td_u8 offset_3;
    td_u8 offset_4;
    td_u8 offset_5;
    td_u8 offset_8;
    td_u8 offset_9;
    td_u8 offset_a;
    td_u8 offset_b;
};

struct spec_params {
    struct dphy_spec_en en;
    struct dphy_spec_params dphy;
    struct aphy_spec_params aphy;
};

struct tmds_spec_params {
    u32 min_tmds_clk;
    u32 max_tmds_clk;
    struct spec_params data;
    struct spec_params clock;
};

typedef struct {
    td_u32 clk_min;
    td_u32 clk_max;
} phy_clk_range;

typedef struct {
    phy_clk_range clk_range;
    td_u8 seek_value;
} phy_clk_range_value;

typedef struct {
    td_u8 ref_clk_div;
    td_u8 vp_divnsel;
    td_u8 cpzs;
    td_u8 tmds_divnsel;
    td_u8 vp_mode;
    td_u8 fcd_step;
    hdmi_phy_tmds_cfg tmds_cfg;
} phy_clk_set;

typedef struct {
    td_u8  m_val;
    td_u8  n_val;
    td_u32 pll_ref_clk;
    td_u32 mn_value;
} phy_mnx;

td_s32 hal_hdmi_phy_init(const hdmi_hal_init *hal_init);

td_s32 hal_hdmi_phy_deinit(td_u32 id);

td_s32 hal_hdmi_phy_power_get(td_u32 id, td_bool *enable);

td_s32 hal_hdmi_phy_power_set(td_u32 id, td_bool enable);

td_s32 hal_hdmi_phy_oe_set(td_u32 id, td_bool enable);

td_s32 hal_hdmi_phy_oe_get(td_u32 id, td_bool *enable);

td_s32 hal_hdmi_phy_tmds_set(td_u32 id, const hdmi_phy_tmds_cfg *tmds_cfg);

td_s32 hal_hdmi_phy_info_get(td_u32 id, hdmi_phy_info *phy_status);

td_s32 hal_hdmi_phy_ssc_set(td_u32 id, hdmi_phy_ssc_cfg *hdmi_ssc_cfg);

td_s32 hal_hdmi_phy_ssc_get(td_u32 id, td_bool *enable);

td_s32 hal_hdmi_phy_params_set(td_u32 id, td_u32 tmds_clk, const hdmi_hw_spec *hw_spec);

td_s32 hal_hdmi_phy_params_get(td_u32 id, hdmi_phy_hw_param *hw_param);

#endif /* HDMI_HAL_PHY_H */

