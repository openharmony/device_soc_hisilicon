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

#ifndef CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART01_H_
#define CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART01_H_


#line 1 "hdmi_hal_phy.c"
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
#include "hdmi_hal_phy.h"
#include "hdmi_reg_dphy.h"
#include "hdmi_product_define.h"
#include "hdmi_reg_tx.h"
#include "hdmi_hal_intf.h"
#include "drv_hdmi_common.h"

#define PHY_LOOP_DIV_CFG_25    0x640000  /* 25 x 4 = 100 = 0x64 */
#define PHY_LOOP_DIV_CFG_22    0x5a0000  /* 22 x 4 = 90  = 0x5a */
#define PHY_FEEDTHRU_EN_MASK   (0x1 << 2)
#define PHY_FEEDTHRU_DATA_MASK 0x3
#define PHY_HWSPEC_I_16        16
#define PHY_HWSPEC_I_28        28
#define PHY_HWSPEC_I_32        32
#define PHY_HWSPEC_I_MAX       43
#define PHY_RATIO_1_10_MASK    0x1f      /* 10bit/2 eq 1 */
#define PHY_RATIO_1_40_MASK    0xfffff   /* 40bit/2 eq 1 */
#define PHY_TXPLL_TEST_OFF     0
#define PHY_TXPLL_TEST_ON      0x30000
#define PHY_POWER_ON_MASK_CLK  0xe
#define PHY_POWER_ON_MASK_D0   0xd
#define PHY_POWER_ON_MASK_D1   0xb
#define PHY_POWER_ON_MASK_D2   0x7
#define PHY_POWER_OFF          0xf
#define PHY_POWER_ON           0
#define PHY_POWER_LDO_OFF      0x3
#define PHY_OE_OFF             0xf
#define PHY_OE_ON              0
#define PHY_I_CHARGE_PUMP      0x3
#define PHY_PLL_LOOP_DEVIDER   0x20
#define TMDS_CLOCK_25M         25000
#define TMDS_CLOCK_100M        100000
#define TMDS_CLOCK_165M        165000
#define TMDS_CLOCK_340M        340000
#define HDMI_HW_PARAM_NUM      4

typedef enum {
    PHY_CLK_RATIO_1_10,
    PHY_CLK_RATIO_1_40
} phy_clk_ratio;

typedef enum {
    INTERPOLATION_MODE_1X,
    INTERPOLATION_MODE_2X,
    INTERPOLATION_MODE_4X,
    INTERPOLATION_MODE_8X,
    INTERPOLATION_MODE_BUTT
} phy_div_mode;

typedef struct {
    phy_rterm_mode r_term_mode;
    td_u32 mode_data_clk;
    td_u32 mode_data_d0;
    td_u32 mode_data_d1;
    td_u32 mode_data_d2;
} phy_rterm_mode_data;

typedef struct {
    td_u32  phy_gc_ldo_pd;
    td_u32  phy_gc_de_pd;
    td_bool phy_gc_bist_pd;
    td_bool phy_gc_txpll_pd;
    td_bool phy_gc_rxsense_pd;
} phy_power_cfg;

typedef struct {
    td_u32 clk_min;
    td_u32 clk_max;
} phy_clk_range;

typedef struct {
    phy_clk_range   phy_tmds_clk_range;
    phy_hw_spec_cfg phy_spec_cfg;
} phy_hw_spec;

typedef struct {
    phy_clk_range range;
    phy_hwspec_enhance_cfg cfg;
} phy_hwspec_enhance;

typedef struct {
    phy_clk_range phy_tmds_clk_range;
    phy_ssc_cfg   ssc_cfg;
} phy_ssc;

typedef struct {
    phy_clk_range   pix_clk_range;
    hdmi_deep_color deep_color;
    td_u32          pixel_clk_div;
    td_u32          ssc_byp_div;
    td_u32          tmds_nxdiv;
    td_u32          loop_div;
    phy_clk_ratio   clk_ratio;
    phy_div_mode    div_mode;
} phy_div_cfg;

static hdmi_phy_info g_hdmi_phy_info;

static phy_rterm_mode_data g_phy_rterm_mode_deta[] = {
    /* R term mode                clk   d0   d1     d2 */
    { PHY_RTERM_MODE_SINGLE,     0x03, 0x0c, 0x30, 0xc0 },
    { PHY_RTERM_MODE_SOURCE,     0x02, 0x08, 0x20, 0x80 },
    { PHY_RTERM_MODE_LOAD,       0x01, 0x04, 0x10, 0x40 },
    { PHY_RTERM_MODE_DOUBLEDIFF, 0x00, 0x00, 0x00, 0x00 }
};


#endif /* CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART01_H_ */
