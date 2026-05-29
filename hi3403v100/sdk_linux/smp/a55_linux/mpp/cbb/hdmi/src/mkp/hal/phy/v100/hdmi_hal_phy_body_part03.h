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

#ifndef CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART03_H_
#define CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART03_H_


/*
 * |-tmdsclk_range---|
 * |   min   max     |
 * |---de_main----||-i_main/0.5m_a--||---de_pre---- ||-i_pre/0.25m_a--|
 * | clk d0 d1 d2 ||  clk d0 d1 d2  || clk d0 d1 d2 ||  clk d0 d1 d2  |
 * |---de_r_pre/ohm---||---r_pre/ohm---|
 * |   clk d0 d1 d2   || clk d0 d1 d2  |
 * |---------------------------------------------r_term-------------------------------------------|
 * | clk_rterm_mode clk_rterm d0_rterm_mode d0_rterm d1_rterm_mode d1_rterm d2_rterm_mode d2_rterm|
 */
#if defined(CHIP_TYPE_ss528v100)
/* otp = 0: default 2.5 inch */
static phy_hwspec_enhance g_phy_hw_spec_enhance[] = {
    {{  25000, 100000 },
     { 0,  0,  0,  0, 24, 28, 28, 28,
       TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE,
       PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
       PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x20, PHY_RTERM_MODE_DOUBLEDIFF, 0x20,
       PHY_RTERM_MODE_DOUBLEDIFF, 0x20, 0x50 }
    },
    {{ 100001, 165000 },
     { 0,  0,  0,  0, 24, 28, 28, 28,
       TD_FALSE,  TD_FALSE,  TD_FALSE,  TD_FALSE,
       PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
       PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x20, PHY_RTERM_MODE_DOUBLEDIFF, 0x20,
       PHY_RTERM_MODE_DOUBLEDIFF, 0x20, 0x50 }
    },
    {{ 165001, 340000 },
     { 0,  0,  0,  0, 24, 30, 30, 30,
       TD_FALSE,  TD_TRUE,  TD_TRUE,  TD_TRUE,
       PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_55, PHY_FEEDTHRU_CAP_DELAY_55, PHY_FEEDTHRU_CAP_DELAY_55,
       PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x30, PHY_RTERM_MODE_DOUBLEDIFF, 0x30,
       PHY_RTERM_MODE_DOUBLEDIFF, 0x30, 0x50 }
    },
    {{ 340001, 600000 },
     { 0,  0,  0,  0, 21, 39, 39, 39,
       TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE,
       PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
       PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_LOAD, 0xA0, PHY_RTERM_MODE_LOAD, 0xA0,
       PHY_RTERM_MODE_LOAD, 0xA0, 0x50 }
    }
};

/* otp = 1 || otp=2 : default 2.5 inch */
static phy_hwspec_enhance g_phy_hw_spec_enhance_otp[][HDMI_HW_PARAM_NUM] = {
    {{{  25000, 100000 },
      { 0,  0,  0,  0, 24, 26, 26, 26,
        TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE,
        PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
        PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x20, PHY_RTERM_MODE_DOUBLEDIFF, 0x20,
        PHY_RTERM_MODE_DOUBLEDIFF, 0x20, 0x50 }
     },
     {{ 100001, 165000 },
      { 0,  0,  0,  0, 24, 26, 26, 26,
        TD_FALSE,  TD_FALSE,  TD_FALSE,  TD_FALSE,
        PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
        PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x20, PHY_RTERM_MODE_DOUBLEDIFF, 0x20,
        PHY_RTERM_MODE_DOUBLEDIFF, 0x20, 0x50 }
     },
     {{ 165001, 340000 },
      { 0,  0,  0,  0, 24, 28, 28, 28,
        TD_FALSE,  TD_TRUE,  TD_TRUE,  TD_TRUE,
        PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_55, PHY_FEEDTHRU_CAP_DELAY_55, PHY_FEEDTHRU_CAP_DELAY_55,
        PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x30, PHY_RTERM_MODE_DOUBLEDIFF, 0x30,
        PHY_RTERM_MODE_DOUBLEDIFF, 0x30, 0x50 }
     },
     {{ 340001, 600000 },
      { 0,  0,  0,  0, 21, 36, 36, 36,
        TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE,
        PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
        PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_LOAD, 0xA0, PHY_RTERM_MODE_LOAD, 0xA0,
        PHY_RTERM_MODE_LOAD, 0xA0, 0x50 }
     }
    },
    {{{  25000, 100000 },
      { 0,  0,  0,  0, 24, 29, 29, 29,
        TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE,
        PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
        PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x20, PHY_RTERM_MODE_DOUBLEDIFF, 0x20,
        PHY_RTERM_MODE_DOUBLEDIFF, 0x20, 0x50 }
     },
     {{ 100001, 165000 },
      { 0,  0,  0,  0, 24, 29, 29, 29,
        TD_FALSE,  TD_FALSE,  TD_FALSE,  TD_FALSE,
        PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
        PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x20, PHY_RTERM_MODE_DOUBLEDIFF, 0x20,
        PHY_RTERM_MODE_DOUBLEDIFF, 0x20, 0x50 }
     },
     {{ 165001, 340000 },
      { 0,  0,  0,  0, 24, 31, 31, 31,
        TD_FALSE,  TD_TRUE,  TD_TRUE,  TD_TRUE,
        PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_55, PHY_FEEDTHRU_CAP_DELAY_55, PHY_FEEDTHRU_CAP_DELAY_55,
        PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x30, PHY_RTERM_MODE_DOUBLEDIFF, 0x30,
        PHY_RTERM_MODE_DOUBLEDIFF, 0x30, 0x50 }
     },
     {{ 340001, 600000 },
      { 0,  0,  0,  0, 21, 41, 41, 41,
        TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE,
        PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
        PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_LOAD, 0xA0, PHY_RTERM_MODE_LOAD, 0xA0,
        PHY_RTERM_MODE_LOAD, 0xA0, 0x50 }
     }
    }
};
#elif defined(CHIP_TYPE_ss625v100)
static phy_hwspec_enhance g_phy_hw_spec_enhance[] = {
    {{ 25000, 100000 },
     { 0,  0,  0,  0, 20, 26, 26, 26,
       TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE,
       PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
       PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x20, PHY_RTERM_MODE_DOUBLEDIFF, 0x20,
       PHY_RTERM_MODE_DOUBLEDIFF, 0x20, 0x50 }
    },
    {{ 100001, 165000 },
     { 0,  0,  0,  0, 20, 26, 26, 26,
       TD_FALSE,  TD_FALSE,  TD_FALSE,  TD_FALSE,
       PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
       PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x20, PHY_RTERM_MODE_DOUBLEDIFF, 0x20,
       PHY_RTERM_MODE_DOUBLEDIFF, 0x20, 0x50 }
    },
    {{ 165001, 340000 },
     { 0,  0,  0,  0, 20, 27, 27, 27,
       TD_FALSE,  TD_TRUE,  TD_TRUE,  TD_TRUE,
       PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_55, PHY_FEEDTHRU_CAP_DELAY_55, PHY_FEEDTHRU_CAP_DELAY_55,
       PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x30, PHY_RTERM_MODE_DOUBLEDIFF, 0x30,
       PHY_RTERM_MODE_DOUBLEDIFF, 0x30, 0x50 }
    },
    {{ 340001, 600000 },
     { 0,  1,  1,  1, 22, 33, 33, 33,
       TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE,
       PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
       PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_LOAD, 0x70, PHY_RTERM_MODE_LOAD, 0x70,
       PHY_RTERM_MODE_LOAD, 0x70, 0x50 }
    }
};
#elif defined(CHIP_TYPE_ss524v100)
/* default 3.0 inch */
static phy_hwspec_enhance g_phy_hw_spec_enhance[] = {
    {{  25000, 100000 },
     { 0,  4,  4,  4, 24, 29, 29, 29,
       TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE,
       PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
       PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x20, PHY_RTERM_MODE_DOUBLEDIFF, 0x20,
       PHY_RTERM_MODE_DOUBLEDIFF, 0x20, 0x50 }
    },
    {{ 100001, 165000 },
     { 0,  4,  4,  4, 24, 29, 29, 29,
       TD_FALSE,  TD_FALSE,  TD_FALSE,  TD_FALSE,
       PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
       PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x20, PHY_RTERM_MODE_DOUBLEDIFF, 0x20,
       PHY_RTERM_MODE_DOUBLEDIFF, 0x20, 0x50 }
    },
    {{ 165001, 340000 },
     { 0,  4,  4,  4, 24, 31, 31, 31,
       TD_FALSE,  TD_TRUE,  TD_TRUE,  TD_TRUE,
       PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_55, PHY_FEEDTHRU_CAP_DELAY_55, PHY_FEEDTHRU_CAP_DELAY_55,
       PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_DOUBLEDIFF, 0x30, PHY_RTERM_MODE_DOUBLEDIFF, 0x30,
       PHY_RTERM_MODE_DOUBLEDIFF, 0x30, 0x50 }
    },
    {{ 340001, 600000 },
     { 0,  0,  0,  0, 21, 32, 32, 32,
       TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE,
       PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18, PHY_FEEDTHRU_CAP_DELAY_18,
       PHY_RTERM_MODE_LOAD, 0x60, PHY_RTERM_MODE_LOAD, 0x70, PHY_RTERM_MODE_LOAD, 0x70,
       PHY_RTERM_MODE_LOAD, 0x70, 0x50 }
    }
};
#endif

/* spread spectrum ctrl(ssc) */
#if defined(CHIP_TYPE_ss528v100) || defined(CHIP_TYPE_ss625v100)
static phy_ssc g_phy_ssc_cfg[] = {
    {{  25000,  70000 }, {   0,     0 }},  // 0ppm
    {{  70001,  90000 }, { 150, 45000 }},  // 0.15
    {{  90001, 110000 }, {  70, 45000 }},  // 0.07%
    {{ 110001, 145000 }, {  50, 45000 }},  // 0.05%
    {{ 145001, 180000 }, { 100, 45000 }},  // 0.10%
    {{ 180001, 220000 }, {  40, 45000 }},  // 0.04%
    {{ 220001, 250000 }, {  40, 45000 }},  // 0.04%
    {{ 250001, 300000 }, {  40, 45000 }},  // 0.04%
    {{ 300001, 600000 }, {   0,     0 }},  // 0
};
#elif defined(CHIP_TYPE_ss524v100)
static phy_ssc g_phy_ssc_cfg[] = {
    {{  25000,  70000 }, {   0,     0 }},  // 0ppm
    {{  70001,  90000 }, { 150, 45000 }},  // 0.15
    {{  90001, 110000 }, {  70, 45000 }},  // 0.07%
    {{ 110001, 145000 }, {  50, 45000 }},  // 0.05%
    {{ 145001, 180000 }, { 100, 45000 }},  // 0.10%
    {{ 180001, 220000 }, {  40, 45000 }},  // 0.04%
    {{ 220001, 250000 }, {  40, 45000 }},  // 0.04%
    {{ 250001, 300000 }, {  40, 45000 }},  // 0.04%
    {{ 300001, 600000 }, {   0,     0 }},  // 0
};
#endif

/*
 * |-tmdsclk_range---|
 * |   min   max     |
 * |   deepcolor --  pix_div -- byp_div -- tmds_nx_div -- loopdiv -- clk_ratio -- phy_div_mode
 */
static phy_div_cfg g_phy_div_cfg[] = {
    {{ 25000,  50000  },
        HDMI_DEEP_COLOR_24BIT, 16, 10, 8, PHY_LOOP_DIV_CFG_25, PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_8X
    },
    {{ 50001,  100000 },
        HDMI_DEEP_COLOR_24BIT, 8,  10, 4, PHY_LOOP_DIV_CFG_25, PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_4X
    },
    {{ 100001, 200000 },
        HDMI_DEEP_COLOR_24BIT, 4,  10, 2, PHY_LOOP_DIV_CFG_25, PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_2X
    },
    {{ 200001, 340000 },
        HDMI_DEEP_COLOR_24BIT, 2,  10, 1, PHY_LOOP_DIV_CFG_25, PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_1X
    },
    {{ 340001, 600000 },
        HDMI_DEEP_COLOR_24BIT, 1,  5,  1, PHY_LOOP_DIV_CFG_25, PHY_CLK_RATIO_1_40, INTERPOLATION_MODE_1X
    },
    {{ 25001,  50000  },
        HDMI_DEEP_COLOR_30BIT, 16, 8, 8, PHY_LOOP_DIV_CFG_25, PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_8X
    },
    {{ 50001,  100000 },
        HDMI_DEEP_COLOR_30BIT, 8,  8, 4, PHY_LOOP_DIV_CFG_25, PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_4X
    },
    {{ 100001, 200000 },
        HDMI_DEEP_COLOR_30BIT, 4,  8, 2, PHY_LOOP_DIV_CFG_25, PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_2X
    },
    {{ 200001, 272000 },
        HDMI_DEEP_COLOR_30BIT, 2,  8, 1, PHY_LOOP_DIV_CFG_25, PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_1X
    },
    {{ 272001, 340000 },
        HDMI_DEEP_COLOR_30BIT, 2,  8, 1, PHY_LOOP_DIV_CFG_25, PHY_CLK_RATIO_1_40, INTERPOLATION_MODE_1X
    },
    {{ 340001, 600000 },
        HDMI_DEEP_COLOR_30BIT, 1,  8, 1, PHY_LOOP_DIV_CFG_25, PHY_CLK_RATIO_1_40, INTERPOLATION_MODE_1X
    },
    {{ 25001,  50000  },
        HDMI_DEEP_COLOR_36BIT, 16, 6, 8, PHY_LOOP_DIV_CFG_22, PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_8X
    },
    {{ 50001,  100000 },
        HDMI_DEEP_COLOR_36BIT, 8,  6, 4, PHY_LOOP_DIV_CFG_22, PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_4X
    },
    {{ 100001, 200000 },
        HDMI_DEEP_COLOR_36BIT, 4,  6, 2, PHY_LOOP_DIV_CFG_22, PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_2X
    },
    {{ 200001, 227000 },
        HDMI_DEEP_COLOR_36BIT, 2,  6, 1, PHY_LOOP_DIV_CFG_22, PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_1X
    },
    {{ 227001, 340000 },
        HDMI_DEEP_COLOR_36BIT, 2,  6, 1, PHY_LOOP_DIV_CFG_22, PHY_CLK_RATIO_1_40, INTERPOLATION_MODE_1X
    },
    {{ 340001, 600000 },
        HDMI_DEEP_COLOR_36BIT, 1,  3, 1, PHY_LOOP_DIV_CFG_22, PHY_CLK_RATIO_1_40, INTERPOLATION_MODE_1X
    }
};

static phy_rterm_mode_data *phy_rterm_mode_data_get(phy_rterm_mode r_term_mode)
{
    td_u32 i;
    phy_rterm_mode_data *phy_r_term_mode_cfg = TD_NULL;

    for (i = 0, phy_r_term_mode_cfg = &g_phy_rterm_mode_deta[0];
         (phy_r_term_mode_cfg && (i < hdmi_array_size(g_phy_rterm_mode_deta)));
         phy_r_term_mode_cfg++, i++) {
        if (phy_r_term_mode_cfg->r_term_mode == r_term_mode) {
            return phy_r_term_mode_cfg;
        }
    }
    hdmi_err("can't find param, r_term_mode:%u.\n", r_term_mode);

    return &g_phy_rterm_mode_deta[0];
}


#endif /* CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART03_H_ */
