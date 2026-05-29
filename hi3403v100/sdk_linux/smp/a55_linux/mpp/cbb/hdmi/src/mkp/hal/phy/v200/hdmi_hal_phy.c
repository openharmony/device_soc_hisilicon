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
#include "hdmi_reg_tx.h"
#include "hdmi_reg_dphy.h"
#include "hdmi_reg_crg.h"
#include "hdmi_reg_ctrl.h"
#include "hdmi_osal.h"
#include "hdmi_product_define.h"
#include "drv_hdmi_common.h"

/* Constants for reported magic-number values. */
#define HDMI_PHY_PHY_CLK_DIV_INDEX_2           2
#define HDMI_PHY_PHY_CLK_DIV_INDEX_3           3
#define HDMI_PHY_PHY_CLK_DIV_INDEX_4           4

#define APHY_CS_0               0x1
#define APHY_CS_1               0x2
#define APHY_CS_2               0x4
#define APHY_CS_3               0x8
#define APHY_CS_4               0x10
#define APHY_CS_012             0x7
#define APHY_CS_4567            0xf0
#define APHY_CS_8               0x100
#define APHY_CS_9               0x200
#define TMDS_CLOCK_340M         340000
#define PHY_POW_BASE_NUM        2
#define PHY_POW_INDEX_NUM       24
#define TMDS_CLK_FREQ_MUITIPLE  5
#define MOD_N_MULTI_COEFFICIENT 10000
#define MOD_D_MULTI_COEFFICIENT 100000
#define PHY_HWSPEC_I_16         16
#define PHY_HWSPEC_I_8          8
#define HDMI_HW_PARAM_NUM       4

#define hdmi_id_check_invalid_return(id)               \
    do {                                               \
        if ((id) >= HDMI_ID_MAX) {                     \
            hdmi_err("invalid hdmi_dev_id:%u!\n", id); \
            return OT_ERR_HDMI_INVALID_PARA;           \
        }                                              \
    } while (0)

#define hdmi_trace_len_check_invalid_return(trace_len)               \
    do {                                               \
        if ((trace_len) >= HDMI_TRACE_COUNT) {                     \
            hdmi_err("invalid hdmi trace len:%d!\n", trace_len); \
            return OT_ERR_HDMI_INVALID_PARA;           \
        }                                              \
    } while (0)

#define TMDS_DRV_CFG_CH0     0x510
#define cfg_hdmi_ffe_sel(x)  (((x) & 0x1) << 30)
#define CFG_HDMI_FFE_SEL_M   (0x1 << 30)
#define cfg_drv_post2_ch0(x) (((x) & 0x3f) << 24)
#define CFG_DRV_POST2_CH0_M  (0x3f << 24)
#define cfg_drv_post1_ch0(x) (((x) & 0x3f) << 16)
#define CFG_DRV_POST1_CH0_M  (0x3f << 16)
#define cfg_drv_m_ch0(x)     (((x) & 0x3f) << 8)
#define CFG_DRV_M_CH0_M      (0x3f << 8)
#define cfg_drv_pre_ch0(x)   (((x) & 0x3f) << 0)
#define CFG_DRV_PRE_CH0_M    (0x3f << 0)

#define TMDS_DRV_CFG_CH1     0x514
#define cfg_drv_post2_ch1(x) (((x) & 0x3f) << 24)
#define CFG_DRV_POST2_CH1_M  (0x3f << 24)
#define cfg_drv_post1_ch1(x) (((x) & 0x3f) << 16)
#define CFG_DRV_POST1_CH1_M  (0x3f << 16)
#define cfg_drv_m_ch1(x)     (((x) & 0x3f) << 8)
#define CFG_DRV_M_CH1_M      (0x3f << 8)
#define cfg_drv_pre_ch1(x)   (((x) & 0x3f) << 0)
#define CFG_DRV_PRE_CH1_M    (0x3f << 0)

#define TMDS_DRV_CFG_CH2     0x518
#define cfg_drv_post2_ch2(x) (((x) & 0x3f) << 24)
#define CFG_DRV_POST2_CH2_M  (0x3f << 24)
#define cfg_drv_post1_ch2(x) (((x) & 0x3f) << 16)
#define CFG_DRV_POST1_CH2_M  (0x3f << 16)
#define cfg_drv_m_ch2(x)     (((x) & 0x3f) << 8)
#define CFG_DRV_M_CH2_M      (0x3f << 8)
#define cfg_drv_pre_ch2(x)   (((x) & 0x3f) << 0)
#define CFG_DRV_PRE_CH2_M    (0x3f << 0)

#define TMDS_DRV_CFG_CH3     0x51C
#define cfg_drv_post2_ch3(x) (((x) & 0x3f) << 24)
#define CFG_DRV_POST2_CH3_M  (0x3f << 24)
#define cfg_drv_post1_ch3(x) (((x) & 0x3f) << 16)
#define CFG_DRV_POST1_CH3_M  (0x3f << 16)
#define cfg_drv_m_ch3(x)     (((x) & 0x3f) << 8)
#define CFG_DRV_M_CH3_M      (0x3f << 8)
#define cfg_drv_pre_ch3(x)   (((x) & 0x3f) << 0)
#define CFG_DRV_PRE_CH3_M    (0x3f << 0)

#define FFE_EN_CFG         0x67C
#define cfg_c3_pre_en(x)   (((x) & 0x1) << 11)
#define CFG_C3_PRE_EN_M    (1 << 11)
#define cfg_c3_post1_en(x) (((x) & 0x1) << 10)
#define CFG_C3_POST1_EN_M  (1 << 10)
#define cfg_c3_post2_en(x) (((x) & 0x1) << 9)
#define CFG_C3_POST2_EN_M  (1 << 9)
#define cfg_c2_pre_en(x)   (((x) & 0x1) << 8)
#define CFG_C2_PRE_EN_M    (1 << 8)
#define cfg_c2_post1_en(x) (((x) & 0x1) << 7)
#define CFG_C2_POST1_EN_M  (1 << 7)
#define cfg_c2_post2_en(x) (((x) & 0x1) << 6)
#define CFG_C2_POST2_EN_M  (1 << 6)
#define cfg_c1_pre_en(x)   (((x) & 0x1) << 5)
#define CFG_C1_PRE_EN_M    (1 << 5)
#define cfg_c1_post1_en(x) (((x) & 0x1) << 4)
#define CFG_C1_POST1_EN_M  (1 << 4)
#define cfg_c1_post2_en(x) (((x) & 0x1) << 3)
#define CFG_C1_POST2_EN_M  (1 << 3)
#define cfg_c0_pre_en(x)   (((x) & 0x1) << 2)
#define CFG_C0_PRE_EN_M    (1 << 2)
#define cfg_c0_post1_en(x) (((x) & 0x1) << 1)
#define CFG_C0_POST1_EN_M  (1 << 1)
#define cfg_c0_post2_en(x) (((x) & 0x1) << 0)
#define CFG_C0_POST2_EN_M  (1 << 0)

#define HDMI_OE_CFG        0x520
#define cfg_oe_sync_en(x)  (((x) & 0x1) << 4)
#define CFG_OE_SYNC_EN_M   (1 << 4)
#define cfg_hdmi_oe_ch3(x) (((x) & 0x1) << 3)
#define CFG_HDMI_OE_CH3_M  (1 << 3)
#define cfg_hdmi_oe_ch2(x) (((x) & 0x1) << 2)
#define CFG_HDMI_OE_CH2_M  (1 << 2)
#define cfg_hdmi_oe_ch1(x) (((x) & 0x1) << 1)
#define CFG_HDMI_OE_CH1_M  (1 << 1)
#define cfg_hdmi_oe_ch0(x) (((x) & 0x1) << 0)
#define CFG_HDMI_OE_CH0_M  (1 << 0)

typedef struct {
    phy_clk_range phy_tmds_clk_range;
    phy_ssc_cfg   ssc_cfg;
} phy_ssc;

static phy_mnx g_mnx_get;
static hdmi_phy_info g_hdmi_phy_info[HDMI_ID_MAX];

static const phy_clk_range_value g_phy_hw_fcd_step_set[] = {
    {{      0,  37500 }, 4},
    {{  37500,  75000 }, 3},
    {{  75000, 150000 }, 2},
    {{ 150000, 300000 }, 1},
    {{ 300000, 600000 }, 0}
};

static const phy_clk_range_value g_phy_hw_def_clk_div[] = {
    {{      0,  37500 }, 0},
    {{  37500,  75000 }, 1},
    {{  75000, 150000 }, 2},
    {{ 150000, 300000 }, 3},
    {{ 300000, 600000 }, 4}
};

static const phy_clk_range_value g_phy_hw_tmds_divn_sel[] = {
    {{ 300000, 600000 }, 0},
    {{ 150000, 300000 }, 1},
    {{  75000, 150000 }, 2},
    {{  37500,  75000 }, 3},
    {{  25000,  37500 }, 4}
};

/*
 * tmds clk range
 * data
 * post2_en  post1_en pre_en
 * post2_cfg post1_cfg main_cfg pre_cfg
 * offset_0 offset_1 offset_2 offset_3 offset_4 offset_8 offset_9 offset_a offset_b
 * clock
 * post2_en  post1_en pre_en
 * post2_cfg post1_cfg main_cfg pre_cfg
 * offset_0 offset_1 offset_2 offset_3 offset_4 offset_8 offset_9 offset_a offset_b
 */
static const struct tmds_spec_params g_tmds_spec[] = {
    { 25000, 100000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 100000, 165000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x15, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xe6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x14, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 165000, 340000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x1e, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb7, 0x03, 0x2d, 0x6c, 0x80, 0x00, 0x60 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x17, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x80, 0x00, 0x60 }}
    },
    { 340000, 600000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x25, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x05, 0x17, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }}
    }
};

#if defined(HDMI_PRODUCT_SS928V100) || defined(HDMI_PRODUCT_SS626V100)
/* 1.0 inch */
static const struct tmds_spec_params g_tmds_spec_1inch[] = {
    { 25000, 100000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 100000, 165000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xE6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x14, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 165000, 340000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x1c, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb7, 0x03, 0x2d, 0x6c, 0x80, 0x00, 0x60 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x17, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x80, 0x00, 0x60 }}
    },
    { 340000, 600000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x24, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x05, 0x16, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }}
    }
};

/* 1.5 inch */
static const struct tmds_spec_params g_tmds_spec_1p5inch[] = {
    { 25000, 100000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 100000, 165000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x14, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xe6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x14, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 165000, 340000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x1d, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb7, 0x03, 0x2d, 0x6c, 0x80, 0x00, 0x60 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x17, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x80, 0x00, 0x60 }}
    },
    { 340000, 600000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x24, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x05, 0x16, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }}
    }
};

/* 2.0 inch */
static const struct tmds_spec_params g_tmds_spec_2inch[] = {
    { 25000, 100000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 100000, 165000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x15, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xe6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x14, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 165000, 340000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x1e, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb7, 0x03, 0x2d, 0x6c, 0x80, 0x00, 0x60 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x17, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x80, 0x00, 0x60 }}
    },
    { 340000, 600000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x25, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x05, 0x17, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }}
    }
};

/* 2.5 inch */
static const struct tmds_spec_params g_tmds_spec_2p5inch[] = {
    { 25000, 100000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 100000, 165000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x15, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xe6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x14, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 165000, 340000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x01, 0x1e, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb7, 0x03, 0x2d, 0x6c, 0x80, 0x00, 0x60 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x17, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x80, 0x00, 0x60 }}
    },
    { 340000, 600000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x25, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x05, 0x17, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }}
    }
};

/* 3.0 inch */
static const struct tmds_spec_params g_tmds_spec_3inch[] = {
    { 25000, 100000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 100000, 165000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x15, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xe6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x14, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 165000, 340000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x1e, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xc7, 0x05, 0x2d, 0x6c, 0x80, 0x00, 0x60 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x17, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x80, 0x00, 0x60 }}
    },
    { 340000, 600000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x25, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x05, 0x17, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }}
    }
};

/* 3.5 inch */
static const struct tmds_spec_params g_tmds_spec_3p5inch[] = {
    { 25000, 100000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 100000, 165000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x15, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xe6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x14, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 165000, 340000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x01, 0x1e, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xc7, 0x05, 0x2d, 0x6c, 0x80, 0x00, 0x60 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x17, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x80, 0x00, 0x60 }}
    },
    { 340000, 600000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x01, 0x26, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x05, 0x17, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }}
    }
};

/* 4 inch */
static const struct tmds_spec_params g_tmds_spec_4inch[] = {
    { 25000, 100000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 100000, 165000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x15, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xe6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x14, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 165000, 340000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x02, 0x1e, 0x02 },
       { 0x7f, 0x7e, 0x3e, 0xc7, 0x05, 0x2d, 0x6c, 0x80, 0x00, 0x60 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x17, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x80, 0x00, 0x60 }}
    },
    { 340000, 600000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x02, 0x27, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x05, 0x17, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }}
    }
};

/* 4.5 inch */
static const struct tmds_spec_params g_tmds_spec_4p5inch[] = {
    { 25000, 100000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 100000, 165000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x15, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xe6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x14, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 165000, 340000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x03, 0x1e, 0x03 },
       { 0x7f, 0x7e, 0x3e, 0xf7, 0x06, 0x2d, 0x6c, 0x80, 0x00, 0x60 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x17, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x80, 0x00, 0x60 }}
    },
    { 340000, 600000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x03, 0x28, 0x02 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x05, 0x17, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }}
    }
};

/* 5 inch */
static const struct tmds_spec_params g_tmds_spec_5inch[] = {
    { 25000, 100000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xb6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x13, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 100000, 165000,
      {{ TD_FALSE, TD_TRUE, TD_FALSE },
       { 0x00, 0x02, 0x15, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0xe6, 0x07, 0x2d, 0x48, 0x80, 0x00, 0x70 }},
      {{ TD_FALSE, TD_FALSE, TD_FALSE },
       { 0x00, 0x00, 0x14, 0x00 },
       { 0x7f, 0x3e, 0x3e, 0x06, 0x00, 0x2d, 0x48, 0x80, 0x00, 0x70 }}
    },
    { 165000, 340000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x04, 0x1e, 0x05 },
       { 0x7f, 0x7e, 0x3e, 0xf7, 0x06, 0x2d, 0x6c, 0x80, 0x00, 0x60 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x00, 0x17, 0x00 },
       { 0x7f, 0x7e, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x80, 0x00, 0x60 }}
    },
    { 340000, 600000,
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x04, 0x29, 0x05 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }},
      {{ TD_FALSE, TD_TRUE, TD_TRUE },
       { 0x00, 0x05, 0x17, 0x00 },
       { 0x7f, 0x7f, 0x3e, 0x06, 0x00, 0x2d, 0x6c, 0x81, 0x00, 0x50 }}
    }
};
#endif

#if defined(HDMI_PRODUCT_SS928V100) || defined(HDMI_PRODUCT_SS626V100)
static const phy_ssc g_phy_ssc_cfg[] = {
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

static const phy_ssc_cfg *phy_ssc_data_get(td_u32 tmds_clk)
{
#if defined(HDMI_PRODUCT_SS928V100) || defined(HDMI_PRODUCT_SS626V100)
    td_u32 i;
    const phy_ssc *ssc_cfg = TD_NULL;

    for (i = 0, ssc_cfg = &g_phy_ssc_cfg[0]; (ssc_cfg && (i < hdmi_array_size(g_phy_ssc_cfg))); ssc_cfg++, i++) {
        if ((tmds_clk >= ssc_cfg->phy_tmds_clk_range.clk_min) && (tmds_clk <= ssc_cfg->phy_tmds_clk_range.clk_max)) {
            return (&ssc_cfg->ssc_cfg);
        }
    }
#else
    ot_unused(tmds_clk);
#endif

    return TD_NULL;
}

static td_void phy_hw_write_stb1_byte(td_u32 id, td_u32 cs, aphy_offset_addr aphy_offset, td_u32 wdata)
{
    /* CS reset */
    hdmi_reg_stb_cs_en_set(id, 0x00);
    /* WR reset */
    hdmi_reg_stb_wen_set(id, TD_FALSE);
    /* WDATA set */
    hdmi_reg_stb_wdata_set(id, wdata);
    /* ADDR set */
    hdmi_reg_stb_addr_set(id, aphy_offset);
    osal_udelay(1);
    /* WR set */
    hdmi_reg_stb_wen_set(id, TD_TRUE);
    /* CS set */
    hdmi_reg_stb_cs_en_set(id, cs);
    osal_udelay(1);
    /* CS reset */
    hdmi_reg_stb_cs_en_set(id, 0x00);
    /* WR reset */
    hdmi_reg_stb_wen_set(id, TD_FALSE);

    return;
}

static td_void phy_hw_read_stb1_byte(td_u32 id, td_u32 cs, aphy_offset_addr aphy_offset, td_u8 *rdata)
{
    /* CS reset */
    hdmi_reg_stb_cs_en_set(id, 0x00);
    /* WR reset */
    hdmi_reg_stb_wen_set(id, TD_FALSE);
    /* ADDR set */
    hdmi_reg_stb_addr_set(id, aphy_offset);
    osal_udelay(1);
    /* CS set */
    hdmi_reg_stb_cs_en_set(id, cs);
    osal_udelay(1);
    /* RDATA read */
    *rdata = (td_u8)hdmi_reg_stb_rdata_get(id);
    /* CS reset */
    hdmi_reg_stb_cs_en_set(id, 0x00);

    return;
}

static td_void phy_hw_write_stb(td_u32 id, write_param param)
{
    td_u8 rdata = 0;
    td_u32 mask_value;
    td_u32 write_value;

    if ((param.msb == 0x7) && (param.lsb == 0x0)) {
        phy_hw_write_stb1_byte(id, param.cs, param.aphy_offset, param.wdata);
    } else {
        if (param.cs == (APHY_CS_012 | APHY_CS_3)) {
            phy_hw_read_stb1_byte(id, APHY_CS_0, param.aphy_offset, &rdata);
        } else if (param.cs == APHY_CS_4567) {
            phy_hw_read_stb1_byte(id, APHY_CS_4, param.aphy_offset, &rdata);
        } else {
            phy_hw_read_stb1_byte(id, param.cs, param.aphy_offset, &rdata);
        }

        mask_value  = (0xff >> (0x7 - param.msb)) & (0xff << param.lsb);
        write_value = (~mask_value & rdata) + (mask_value & (param.wdata << param.lsb));
        phy_hw_write_stb1_byte(id, param.cs, param.aphy_offset, write_value);
    }

    return;
}

#if defined(HDMI_PRODUCT_SS928V100) || defined(HDMI_PRODUCT_SS626V100)
static td_void trace_printf(hdmi_trace_len trace_len)
{
    switch (trace_len) {
        case HDMI_TRACE_LEN_0:
            hdmi_info("1.0 inch\n");
            break;
        case HDMI_TRACE_LEN_1:
            hdmi_info("1.5 inch\n");
            break;
        case HDMI_TRACE_LEN_2:
            hdmi_info("2.0 inch\n");
            break;
        case HDMI_TRACE_LEN_3:
            hdmi_info("2.5 inch\n");
            break;
        case HDMI_TRACE_LEN_4:
            hdmi_info("3.0 inch\n");
            break;
        case HDMI_TRACE_LEN_5:
            hdmi_info("3.5 inch\n");
            break;
        case HDMI_TRACE_LEN_6:
            hdmi_info("4.0 inch\n");
            break;
        case HDMI_TRACE_LEN_7:
            hdmi_info("4.5 inch\n");
            break;
        case HDMI_TRACE_LEN_8:
            hdmi_info("5.0 inch\n");
            break;
        default:
            hdmi_info("default inch\n");
            break;
    }

    return;
}

static td_void phy_hw_tmds_spec_trace_len_get(const struct tmds_spec_params **hwspec_enhance, hdmi_trace_len trace_len)
{
    const struct tmds_spec_params *hwspec = TD_NULL;

    trace_printf(trace_len);

    switch (trace_len) {
        case HDMI_TRACE_LEN_0:
            hwspec = &g_tmds_spec_1inch[0];
            break;
        case HDMI_TRACE_LEN_1:
            hwspec = &g_tmds_spec_1p5inch[0];
            break;
        case HDMI_TRACE_LEN_2:
            hwspec = &g_tmds_spec_2inch[0];
            break;
        case HDMI_TRACE_LEN_3:
            hwspec = &g_tmds_spec_2p5inch[0];
            break;
        case HDMI_TRACE_LEN_4:
            hwspec = &g_tmds_spec_3inch[0];
            break;
        case HDMI_TRACE_LEN_5:
            hwspec = &g_tmds_spec_3p5inch[0];
            break;
        case HDMI_TRACE_LEN_6:
            hwspec = &g_tmds_spec_4inch[0];
            break;
        case HDMI_TRACE_LEN_7:
            hwspec = &g_tmds_spec_4p5inch[0];
            break;
        case HDMI_TRACE_LEN_8:
            hwspec = &g_tmds_spec_5inch[0];
            break;
        default:
            hwspec = &g_tmds_spec[0];
            break;
    }

    *hwspec_enhance = hwspec;

    return;
}
#endif

static const struct tmds_spec_params *get_tmds_spec_params(const hdmi_phy_tmds_cfg *tmds_cfg)
{
    td_u32 i;
    td_u32 len;
    const struct tmds_spec_params *tmp = TD_NULL;

#if defined(HDMI_PRODUCT_SS928V100) || defined(HDMI_PRODUCT_SS626V100)
    if (tmds_cfg->trace_len == HDMI_TRACE_DEFAULT) {
        tmp = &g_tmds_spec[0];
    } else {
        phy_hw_tmds_spec_trace_len_get(&tmp, tmds_cfg->trace_len);
    }
#else
    tmp = &g_tmds_spec[0];
#endif
    len = (td_u32)hdmi_array_size(g_tmds_spec);

    for (i = 0; i < len; i++) {
        if (tmds_cfg->tmds_clk >= tmp[i].min_tmds_clk && tmds_cfg->tmds_clk < tmp[i].max_tmds_clk) {
            return &tmp[i];
        }
    }

    return TD_NULL;
}

static void phy_hw_tmds_aphy_spec_set(td_u32 id, const struct tmds_spec_params *tmds)
{
    const struct aphy_spec_params *data = &tmds->data.aphy;
    const struct aphy_spec_params *clk = &tmds->clock.aphy;

    /* data drv set  */
    phy_hw_write_stb1_byte(id, APHY_CS_012, APHY_OFFSET_0, data->offset_0);
    phy_hw_write_stb1_byte(id, APHY_CS_012, APHY_OFFSET_1, data->offset_1);
    phy_hw_write_stb1_byte(id, APHY_CS_012, APHY_OFFSET_2, data->offset_2);
    phy_hw_write_stb1_byte(id, APHY_CS_012, APHY_OFFSET_3, data->offset_3);
    phy_hw_write_stb1_byte(id, APHY_CS_012, APHY_OFFSET_4, data->offset_4);
    phy_hw_write_stb1_byte(id, APHY_CS_012, APHY_OFFSET_5, data->offset_5);
    phy_hw_write_stb1_byte(id, APHY_CS_012, APHY_OFFSET_8, data->offset_8);
    phy_hw_write_stb1_byte(id, APHY_CS_012, APHY_OFFSET_9, data->offset_9);
    phy_hw_write_stb1_byte(id, APHY_CS_012, APHY_OFFSET_A, data->offset_a);
    phy_hw_write_stb1_byte(id, APHY_CS_012, APHY_OFFSET_B, data->offset_b);
    /* clk drv set */
    phy_hw_write_stb1_byte(id, APHY_CS_3, APHY_OFFSET_0, clk->offset_0);
    phy_hw_write_stb1_byte(id, APHY_CS_3, APHY_OFFSET_1, clk->offset_1);
    phy_hw_write_stb1_byte(id, APHY_CS_3, APHY_OFFSET_2, clk->offset_2);
    phy_hw_write_stb1_byte(id, APHY_CS_3, APHY_OFFSET_3, clk->offset_3);
    phy_hw_write_stb1_byte(id, APHY_CS_3, APHY_OFFSET_4, clk->offset_4);
    phy_hw_write_stb1_byte(id, APHY_CS_3, APHY_OFFSET_5, clk->offset_5);
    phy_hw_write_stb1_byte(id, APHY_CS_3, APHY_OFFSET_8, clk->offset_8);
    phy_hw_write_stb1_byte(id, APHY_CS_3, APHY_OFFSET_9, clk->offset_9);
    phy_hw_write_stb1_byte(id, APHY_CS_3, APHY_OFFSET_A, clk->offset_a);
    phy_hw_write_stb1_byte(id, APHY_CS_3, APHY_OFFSET_B, clk->offset_b);
}

static void hdmi_write_mask(td_u32 id, td_u32 offset, td_u32 val, td_u32 mask)
{
    td_u32 tmp;
    td_char *reg = TD_NULL;

    reg = (td_char *)hdmi_reg_tx_get_phy_addr(id);
    if (reg == TD_NULL) {
        hdmi_warn("phy addr is null!\n");
        return;
    }
    reg += offset;
    tmp = *(volatile td_u32 *)(reg);
    tmp = (tmp & ~mask) | (val & mask);
    *(volatile td_u32 *)reg = tmp;
}

static td_u32 hdmi_read_mask(td_u32 id, td_u32 offset, td_u32 mask)
{
    td_u32 tmp;
    td_char *reg = TD_NULL;

    reg = (td_char *)hdmi_reg_tx_get_phy_addr(id);
    if (reg == TD_NULL) {
        hdmi_warn("phy addr is null!\n");
        return TD_FAILURE;
    }
    reg += offset;
    tmp = *(volatile td_u32 *)(reg);
    tmp = (tmp & mask);

    return tmp;
}

static void phy_hw_tmds_dphy_spec_set(td_u32 id, const struct tmds_spec_params *tmds)
{
    const struct dphy_spec_params *data = &tmds->data.dphy;
    const struct dphy_spec_params *clk = &tmds->clock.dphy;
    const struct dphy_spec_en *data_en = &tmds->data.en;
    const struct dphy_spec_en *clk_en = &tmds->clock.en;

    /* select dphy drv set mode */
    hdmi_write_mask(id, TMDS_DRV_CFG_CH0, cfg_hdmi_ffe_sel(0x1), CFG_HDMI_FFE_SEL_M);
    /* dphy data drv set */
    hdmi_write_mask(id, TMDS_DRV_CFG_CH0, cfg_drv_post2_ch0(data->drv_post2), CFG_DRV_POST2_CH0_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH0, cfg_drv_post1_ch0(data->drv_post1), CFG_DRV_POST1_CH0_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH0, cfg_drv_m_ch0(data->drv_main), CFG_DRV_M_CH0_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH0, cfg_drv_pre_ch0(data->drv_pre), CFG_DRV_PRE_CH0_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH1, cfg_drv_post2_ch1(data->drv_post2), CFG_DRV_POST2_CH1_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH1, cfg_drv_post1_ch1(data->drv_post1), CFG_DRV_POST1_CH1_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH1, cfg_drv_m_ch1(data->drv_main), CFG_DRV_M_CH1_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH1, cfg_drv_pre_ch1(data->drv_pre), CFG_DRV_PRE_CH1_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH2, cfg_drv_post2_ch2(data->drv_post2), CFG_DRV_POST2_CH2_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH2, cfg_drv_post1_ch2(data->drv_post1), CFG_DRV_POST1_CH2_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH2, cfg_drv_m_ch2(data->drv_main), CFG_DRV_M_CH2_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH2, cfg_drv_pre_ch2(data->drv_pre), CFG_DRV_PRE_CH2_M);
    /* dphy clk drv set */
    hdmi_write_mask(id, TMDS_DRV_CFG_CH3, cfg_drv_post2_ch3(clk->drv_post2), CFG_DRV_POST2_CH3_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH3, cfg_drv_post1_ch3(clk->drv_post1), CFG_DRV_POST1_CH3_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH3, cfg_drv_m_ch3(clk->drv_main), CFG_DRV_M_CH3_M);
    hdmi_write_mask(id, TMDS_DRV_CFG_CH3, cfg_drv_pre_ch3(clk->drv_pre), CFG_DRV_PRE_CH3_M);
    /* dphy data drv enable */
    hdmi_write_mask(id, FFE_EN_CFG, cfg_c2_pre_en((td_u32)data_en->drv_pre_en) |
        cfg_c2_post1_en((td_u32)data_en->drv_post1_en) | cfg_c2_post2_en((td_u32)data_en->drv_post2_en) |
        cfg_c1_pre_en((td_u32)data_en->drv_pre_en) | cfg_c1_post1_en((td_u32)data_en->drv_post1_en) |
        cfg_c1_post2_en((td_u32)data_en->drv_post2_en) | cfg_c0_pre_en((td_u32)data_en->drv_pre_en) |
        cfg_c0_post1_en((td_u32)data_en->drv_post1_en) | cfg_c0_post2_en((td_u32)data_en->drv_post2_en),
        CFG_C2_PRE_EN_M | CFG_C2_POST1_EN_M | CFG_C2_POST2_EN_M | CFG_C1_PRE_EN_M |
        CFG_C1_POST1_EN_M | CFG_C1_POST2_EN_M | CFG_C0_PRE_EN_M | CFG_C0_POST1_EN_M | CFG_C0_POST2_EN_M);
    /* dphy clock drv enable */
    hdmi_write_mask(id, FFE_EN_CFG, cfg_c3_pre_en((td_u32)clk_en->drv_pre_en) |
        cfg_c3_post1_en((td_u32)clk_en->drv_post1_en) | cfg_c3_post2_en((td_u32)clk_en->drv_post2_en),
        CFG_C3_PRE_EN_M | CFG_C3_POST1_EN_M | CFG_C3_POST2_EN_M);
}

static td_s32 phy_user_spec_param_set(td_u32 id, td_s32 tmds_clk, hdmi_trace_len len, struct tmds_spec_params *spec)
{
    td_u32 i;
    const struct tmds_spec_params *tmp = TD_NULL;

    hdmi_id_check_invalid_return(id);
    hdmi_trace_len_check_invalid_return(len);

    tmp = &g_tmds_spec[0];

    /* User's four frequency hw spec settings corresponding to four frequency are configured by default(g_tmds_spec). */
    for (i = 0; tmp != TD_NULL && (i < hdmi_array_size(g_tmds_spec)); i++, tmp++) {
        /* Confirm the current frequency band. */
        if (tmds_clk >= tmp->min_tmds_clk && tmds_clk <= tmp->max_tmds_clk) {
            if (spec->data.dphy.drv_post1 != g_hdmi_phy_info[id].hw_spec[len].hw_param[i].i_de_main_data) {
                spec->data.dphy.drv_post1 = g_hdmi_phy_info[id].hw_spec[len].hw_param[i].i_de_main_data;
            }
            if (spec->clock.dphy.drv_post1 != g_hdmi_phy_info[id].hw_spec[len].hw_param[i].i_de_main_clk) {
                spec->clock.dphy.drv_post1 = g_hdmi_phy_info[id].hw_spec[len].hw_param[i].i_de_main_clk;
            }
            if (spec->data.dphy.drv_main != g_hdmi_phy_info[id].hw_spec[len].hw_param[i].i_main_data) {
                spec->data.dphy.drv_main = g_hdmi_phy_info[id].hw_spec[len].hw_param[i].i_main_data;
            }
            if (spec->clock.dphy.drv_main != g_hdmi_phy_info[id].hw_spec[len].hw_param[i].i_main_clk) {
                spec->clock.dphy.drv_main = g_hdmi_phy_info[id].hw_spec[len].hw_param[i].i_main_clk;
            }
            break;
        }
    }
    return TD_SUCCESS;
}

static td_s32 phy_tmds_spec_set(td_u32 id, const hdmi_phy_tmds_cfg *tmds_cfg)
{
    td_s32 ret;
    const struct tmds_spec_params *tmds = NULL;
    struct tmds_spec_params tmds_spec = {0};

    tmds = get_tmds_spec_params(tmds_cfg);
    hdmi_if_null_return(tmds, OT_ERR_HDMI_NULL_PTR);
    ret = memcpy_s(&tmds_spec, sizeof(tmds_spec), tmds, sizeof(*tmds));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    phy_user_spec_param_set(id, tmds_cfg->tmds_clk, tmds_cfg->trace_len, &tmds_spec);

    phy_hw_tmds_aphy_spec_set(id, &tmds_spec);
    phy_hw_tmds_dphy_spec_set(id, &tmds_spec);

    return TD_SUCCESS;
}

static td_void phy_hw_ref_clk_div_set(td_u32 id, td_u32 pixel_clk)
{
    td_u32 ref_clk_div = 0;

    if (pixel_clk < g_phy_hw_def_clk_div[0].clk_range.clk_max) {
        ref_clk_div = g_phy_hw_def_clk_div[0].seek_value;
    } else if (pixel_clk >= g_phy_hw_def_clk_div[1].clk_range.clk_min &&
               pixel_clk < g_phy_hw_def_clk_div[1].clk_range.clk_max) {
        ref_clk_div = g_phy_hw_def_clk_div[1].seek_value;
    } else if (pixel_clk >= g_phy_hw_def_clk_div[HDMI_PHY_PHY_CLK_DIV_INDEX_2].clk_range.clk_min &&
               pixel_clk < g_phy_hw_def_clk_div[HDMI_PHY_PHY_CLK_DIV_INDEX_2].clk_range.clk_max) {
        ref_clk_div = g_phy_hw_def_clk_div[HDMI_PHY_PHY_CLK_DIV_INDEX_2].seek_value;
    } else if (pixel_clk >= g_phy_hw_def_clk_div[HDMI_PHY_PHY_CLK_DIV_INDEX_3].clk_range.clk_min &&
               pixel_clk < g_phy_hw_def_clk_div[HDMI_PHY_PHY_CLK_DIV_INDEX_3].clk_range.clk_max) {
        ref_clk_div = g_phy_hw_def_clk_div[HDMI_PHY_PHY_CLK_DIV_INDEX_3].seek_value;
    } else if (pixel_clk >= g_phy_hw_def_clk_div[HDMI_PHY_PHY_CLK_DIV_INDEX_4].clk_range.clk_min &&
               pixel_clk < g_phy_hw_def_clk_div[HDMI_PHY_PHY_CLK_DIV_INDEX_4].clk_range.clk_max) {
        ref_clk_div = g_phy_hw_def_clk_div[HDMI_PHY_PHY_CLK_DIV_INDEX_4].seek_value;
    }
    phy_hw_write_stb1_byte(id, APHY_CS_8, APHY_OFFSET_2, ref_clk_div);

    return;
}

static td_s32 phy_hw_divn_sel_get(td_u32 tmds_clk, td_u8 *tmds_divn)
{
    td_u32 i;
    const phy_clk_range_value *phy_tmds_divnsel = TD_NULL;

    for (i = 0, phy_tmds_divnsel = &g_phy_hw_tmds_divn_sel[0];
        (i < hdmi_array_size(g_phy_hw_tmds_divn_sel)); phy_tmds_divnsel++, i++) {
        if ((tmds_clk >= phy_tmds_divnsel->clk_range.clk_min) && (tmds_clk < phy_tmds_divnsel->clk_range.clk_max)) {
            *tmds_divn = phy_tmds_divnsel->seek_value;
            return TD_SUCCESS;
        }
    }
    hdmi_err("can't find param, tmds_clk:%u\n", tmds_clk);

    return TD_FAILURE;
}

static td_void phy_hw_tmds_clk_div_set(td_u32 id, td_u32 tmds_clk)
{
    td_s32 ret;
    td_u8 tmds_clk_div = 0;

    ret = phy_hw_divn_sel_get(tmds_clk, &tmds_clk_div);
    if (ret != TD_SUCCESS) {
        return;
    }
    phy_hw_write_stb1_byte(id, APHY_CS_8, APHY_OFFSET_3, ((0x14 << 0x3) | (tmds_clk_div & 0x7)));
}

static td_void phy_hw_init_tmds(td_u32 id, const hdmi_phy_tmds_cfg *hdmi_spec_cfg)
{
    /* PLL settings */
    phy_hw_write_stb1_byte(id, APHY_CS_8, APHY_OFFSET_E, 0x11);
    phy_hw_write_stb1_byte(id, APHY_CS_8, APHY_OFFSET_0, 0xFF);
    phy_hw_write_stb1_byte(id, APHY_CS_8, APHY_OFFSET_1, 0x11);

    /* aphy refclk select */
    phy_hw_ref_clk_div_set(id, hdmi_spec_cfg->pixel_clk);
    phy_hw_tmds_clk_div_set(id, hdmi_spec_cfg->tmds_clk);

    phy_hw_write_stb1_byte(id, APHY_CS_8, APHY_OFFSET_4, 0x00);
    phy_hw_write_stb1_byte(id, APHY_CS_8, APHY_OFFSET_9, 0x00);
    phy_hw_write_stb1_byte(id, APHY_CS_8, APHY_OFFSET_A, 0x03);
    phy_hw_write_stb1_byte(id, APHY_CS_8, APHY_OFFSET_B, 0xE0);
    phy_hw_write_stb1_byte(id, APHY_CS_8, APHY_OFFSET_C, 0x14);
    phy_hw_write_stb1_byte(id, APHY_CS_8, APHY_OFFSET_D, 0xF0);

    return;
}

static td_s32 phy_hw_init_set(td_u32 id, const hdmi_phy_tmds_cfg *hdmi_spec_cfg)
{
    hdmi_info("mode_cfg = %u\n", hdmi_spec_cfg->mode_cfg);
    if (hdmi_spec_cfg->mode_cfg == HDMI_PHY_MODE_CFG_TMDS) {
        phy_hw_init_tmds(id, hdmi_spec_cfg);
    }

    return TD_SUCCESS;
}

static td_s32 phy_hw_input_clock_check(td_u32 id, td_u32 *pixel_clk)
{
    td_u32 reg_ret_value;
    td_u32 input_clk;

    /* select pixel clock */
    hdmi_reg_fd_clk_sel_set(id, 0x0);
    hdmi_reg_refclk_sel_set(id, 0x0);
    hdmi_reg_ctman_set(id, 0x0);
    hdmi_reg_modclk_sel_set(id, 0x1);
    hdmi_reg_fdivclk_sel_set(id, 0x0);
    hdmi_reg_mod_div_val_set(id, 0x0);
    /* set lock_val and lock_cnt */
    hdmi_reg_src_lock_val_set(id, 0x04);
    hdmi_reg_src_lock_cnt_set(id, 0x02);
    hdmi_reg_src_enable_set(id, 0x0);
    hdmi_reg_fdsrcparam_unused_set(id, 0x0);
    /* set frequency options */
    hdmi_reg_src_cnt_opt_set(id, 0x1);
    hdmi_reg_fdsrcfreq_unused1_set(id, 0);
    hdmi_reg_src_freq_opt_set(id, 0);
    hdmi_reg_fdsrcfreq_unused2_set(id, 0);
    hdmi_reg_src_freq_ext_set(id, 0);
    /* FD enable */
    hdmi_reg_src_lock_val_set(id, 0x4);
    hdmi_reg_src_lock_cnt_set(id, 0x2);
    hdmi_reg_src_enable_set(id, 0x1);
    hdmi_reg_fdsrcparam_unused_set(id, 0x0);
    /* delay time */
    osal_msleep(1);
    /* read status and result */
    reg_ret_value = hdmi_reg_src_det_stat_get(id);
    input_clk = hdmi_reg_src_cnt_out_get(id);
    hdmi_info("input clock = : %u  khz\n", input_clk);
    if ((reg_ret_value & 0xF) == 0xF) {
        hdmi_info("input clock quality : stable\n");
        *pixel_clk = input_clk;
    } else {
        hdmi_info("warning! input clock is unstable!\n");
        *pixel_clk = input_clk;
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void phy_hw_reset_release(td_u32 id)
{
#ifdef HDMI_PRODUCT_SS928V100
    hdmi_reg_phy_bus_srst_req_set(TD_TRUE);
    osal_msleep(1);
    hdmi_reg_phy_bus_srst_req_set(TD_FALSE);
#else
    hdmi_reg_phy_bus_srst_req_set(id, TD_TRUE);
    osal_msleep(1);
    hdmi_reg_phy_bus_srst_req_set(id, TD_FALSE);
#endif
    osal_msleep(1);
    hdmi_reg_resetn_set(id, TD_FALSE);
    hdmi_reg_sw_reset_mod_clock_set(id, TD_FALSE);
    hdmi_reg_sw_reset_tmds_clock_set(id, TD_FALSE);
    hdmi_reg_sw_reset_mpll_clock_set(id, TD_FALSE);
    hdmi_reg_sw_reset_nco_clock_set(id, TD_FALSE);
    hdmi_reg_sw_reset_fd_clock_set(id, TD_FALSE);
    hdmi_reg_sw_reset_mod_and_mpll_clock_set(id, TD_FALSE);
    hdmi_reg_sw_reset_mod_and_nco_clock_set(id, TD_FALSE);
    hdmi_reg_sw_reset_mod_and_fd_clock_set(id, TD_FALSE);
    hdmi_reg_sw_reset_hsfifo_clock_set(id, TD_FALSE);
    hdmi_reg_sw_reset_txfifo_clock_set(id, TD_FALSE);
    hdmi_reg_sw_reset_data_clock_set(id, TD_FALSE);
    hdmi_reg_sw_reset_hs_clock_set(id, TD_FALSE);
    hdmi_reg_sw_reset_pllref_clock_set(id, TD_FALSE);
    hdmi_reg_sw_reset_dac_clock_set(id, TD_FALSE);
    hdmi_reg_dac_clock_gat_set(id, TD_FALSE);
    hdmi_reg_sw_reset_frl_clock_set(id, TD_FALSE);
    hdmi_reg_up_sample_fifo_clock_swrst_set(id, TD_FALSE);
    hdmi_reg_swreset_unused_set(id, TD_FALSE);
    hdmi_reg_global_reset_set(id, TD_TRUE);
    hdmi_reg_resetn_set(id, TD_TRUE);
    hdmi_reg_global_reset_set(id, TD_FALSE);
    hdmi_reg_dac_clock_gat_set(id, TD_TRUE);
    hdmi_reg_fd_clk_sel_set(id, TD_FALSE);
    hdmi_reg_refclk_sel_set(id, TD_FALSE);
    hdmi_reg_ctman_set(id, TD_FALSE);
    hdmi_reg_modclk_sel_set(id, TD_TRUE);
    hdmi_reg_fdivclk_sel_set(id, TD_FALSE);
    hdmi_reg_mod_div_val_set(id, TD_FALSE);
    hdmi_reg_ref_clk_sel_set(id, TD_TRUE);
    hdmi_reg_req_length_set(id, 0x0);
    hdmi_reg_stb_delay2_set(id, 0x2);
    hdmi_reg_stb_delay1_set(id, 0x2);
    hdmi_reg_stb_delay0_set(id, 0x2);
    hdmi_reg_stb_acc_sel_set(id, TD_FALSE);
    hdmi_reg_stb_cs_sel_set(id, TD_FALSE);

    return;
}

static td_u32 phy_hw_pow(td_u32 base_num, td_u32 index_num)
{
    td_u32 i;
    td_u32 ret_val = 1;

    for (i = 0; i < index_num; i++) {
        ret_val = ret_val * base_num;
    }

    return ret_val;
}

static td_void phy_hw_clock_tmds_set(td_u32 id, const phy_clk_set *phy_clk)
{
    hdmi_reg_divn_h20_set(id, (phy_clk->fcd_step & 0x07));
    hdmi_reg_fcdstepset_unused_set(id, TD_FALSE);
    hdmi_reg_up_sampler_ratio_sel_set(id, TD_FALSE);
    hdmi_reg_manual_en_set(id, 0xe);
    hdmi_info("g_mnx_get: M:%u, N:%u\n", g_mnx_get.m_val, g_mnx_get.n_val);
    hdmi_reg_mdiv_set(id, g_mnx_get.m_val);
    hdmi_reg_fdiv_in_set(id, ((td_u32)g_mnx_get.n_val) << 24); /* 24'b, BIT[32:25] */
    hdmi_reg_mode_en_set(id, (phy_clk->tmds_cfg.tmds_clk > TMDS_CLOCK_340M) ? 0x1 : 0x0);
    hdmi_info("tmds_clk: %u\n", phy_clk->tmds_cfg.tmds_clk);
    /* FDIV init */
    hdmi_reg_init_set(id, TD_FALSE);
    hdmi_reg_en_ctrl_set(id, TD_FALSE);
    hdmi_reg_en_mod_set(id, TD_FALSE);
    hdmi_reg_en_sdm_set(id, TD_FALSE);
    hdmi_reg_init_set(id, TD_TRUE);
    /* delay 1us */
    osal_udelay(1);
    hdmi_reg_init_set(id, TD_FALSE);
    hdmi_reg_en_ctrl_set(id, TD_FALSE);
    hdmi_reg_en_mod_set(id, TD_FALSE);
    hdmi_reg_en_sdm_set(id, TD_FALSE);

    return;
}

static td_s32 phy_hw_post_set_up(td_u32 id, const hdmi_phy_tmds_cfg *tmds_cfg)
{
    write_param param = {0};

    ot_unused(tmds_cfg);
    hdmi_reg_hsset_set(id, 3); /* 3, HS FIFO enable & HS data selection: External(HS Link) */
    hdmi_reg_pr_en_h20_set(id, TD_TRUE);
    hdmi_reg_enable_h20_set(id, TD_TRUE);
    hdmi_reg_txfifoset0_unused_set(id, 0);

    hdmi_reg_ch_out_sel_set(id, 0x0);
    param.cs = APHY_CS_0;
    param.aphy_offset = APHY_OFFSET_0;
    param.msb = 1;
    param.lsb = 1;
    param.wdata = 0x0;
    phy_hw_write_stb(id, param);
    param.cs = APHY_CS_1;
    phy_hw_write_stb(id, param);
    param.cs = APHY_CS_2;
    phy_hw_write_stb(id, param);
    param.cs = APHY_CS_3;
    phy_hw_write_stb(id, param);
    osal_msleep(1);
    param.cs = APHY_CS_0;
    param.wdata = 0x1;
    phy_hw_write_stb(id, param);
    param.cs = APHY_CS_1;
    phy_hw_write_stb(id, param);
    param.cs = APHY_CS_2;
    phy_hw_write_stb(id, param);
    param.cs = APHY_CS_3;
    phy_hw_write_stb(id, param);

    return TD_SUCCESS;
}

static td_s32 phy_hw_clk_rang_value_get(td_u32 tmds_clk, td_u8 size,
    const phy_clk_range_value *phy_rang_sel, td_u8 *seek_value)
{
    td_u32 i;
    const phy_clk_range_value *phy_rang = TD_NULL;

    for (i = 0, phy_rang = &phy_rang_sel[0]; (i < size); phy_rang++, i++) {
        if ((tmds_clk >= phy_rang->clk_range.clk_min) && (tmds_clk < phy_rang->clk_range.clk_max)) {
            *seek_value = phy_rang->seek_value;
            return TD_SUCCESS;
        }
    }
    hdmi_err("can't find param,tmds_clk:%u,i=%u\n", tmds_clk, i);

    return TD_FAILURE;
}

static td_s32 phy_hw_fractional_mnx_get(td_u32 tmds_clk, td_u32 pixel_clk, hdmi_deep_color deep_color)
{
    td_u8 k;
    td_u8 m_value;
    td_u8 size;
    td_u32 mn_value;
    td_u32 pll_ref_clk;
    td_u8 seek_value = 0;
    td_u8 tmds_divnsel = 0;

    /* HDMI 2.0 configure pll feedback coefficient M, N, X */
    size = (td_u8)hdmi_array_size(g_phy_hw_def_clk_div);
    /* determine the reference clock division factor */
    if (phy_hw_clk_rang_value_get(pixel_clk, size, &g_phy_hw_def_clk_div[0], &seek_value) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* obtain  TMDS_DIVNSEL */
    if (phy_hw_divn_sel_get(tmds_clk, &tmds_divnsel) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    hdmi_info("pixel_clk: %u, seek_value: %u-%u\n", pixel_clk, seek_value, phy_hw_pow(PHY_POW_BASE_NUM, seek_value));
    pll_ref_clk = pixel_clk / phy_hw_pow(PHY_POW_BASE_NUM, seek_value);
    g_mnx_get.pll_ref_clk = pll_ref_clk;
    hdmi_info("pll_ref_clk(%u), tmds_divnsel(%u), deep_color(%u) \n", g_mnx_get.pll_ref_clk, tmds_divnsel, deep_color);
    switch (deep_color) {
        case HDMI_DEEP_COLOR_30BIT:
            mn_value = TMDS_CLK_FREQ_MUITIPLE * phy_hw_pow(PHY_POW_BASE_NUM, tmds_divnsel) *
                phy_hw_pow(PHY_POW_BASE_NUM, seek_value) * 5 / 4; /* 5 and 4 means 10bit is std 5/4 multiple */
            break;
        case HDMI_DEEP_COLOR_36BIT:
            mn_value = TMDS_CLK_FREQ_MUITIPLE * phy_hw_pow(PHY_POW_BASE_NUM, tmds_divnsel) *
                phy_hw_pow(PHY_POW_BASE_NUM, seek_value) * 3 / 2; /* 3 and 2 means 12bit is std 3/2 multiple */
            break;
        default:
            mn_value = TMDS_CLK_FREQ_MUITIPLE * phy_hw_pow(PHY_POW_BASE_NUM, tmds_divnsel) *
                phy_hw_pow(PHY_POW_BASE_NUM, seek_value);
            break;
    }
    hdmi_info("mn_value = %u \n", mn_value);
    g_mnx_get.mn_value = mn_value;

    /* calculate N value */
    g_mnx_get.n_val = (td_u8)(mn_value % 10); /* 10, about pll coefficient N calculate protocol */
    if (g_mnx_get.n_val == 0) {
        k = 2;                /* 2, means 1 + 1, about pll coefficient N real value calculate */
        g_mnx_get.n_val = 10; /* calculate result n_val is 0, get real n_val 10 */
    } else {
        k = 1;
    }

    /* calculate M value */
    m_value = (td_u8)(mn_value / 10); /* 10, about pll coefficient M calculate protocol */
    g_mnx_get.m_val = m_value - k;
    hdmi_info("get mnx M:%x, N:%u\n", g_mnx_get.m_val, g_mnx_get.n_val);

    return TD_SUCCESS;
}

static td_void phy_hw_clock_set(td_u32 id, const phy_clk_set *phy_clk)
{
    if (phy_clk->tmds_cfg.mode_cfg == HDMI_PHY_MODE_CFG_TMDS) {
        phy_hw_clock_tmds_set(id, phy_clk);
    } else {
        hdmi_err("mode err.\n");
    }

    return;
}

static td_s32 hal_hdmi_clk_set_para_get(td_u32 id, phy_clk_set *phy_clk)
{
    td_u8 size;

    size = (td_u8)hdmi_array_size(g_phy_hw_fcd_step_set);
    if (phy_hw_clk_rang_value_get(phy_clk->tmds_cfg.tmds_clk, size,
        &g_phy_hw_fcd_step_set[0], &phy_clk->fcd_step) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    hdmi_info("get fcd_step = %x\n", phy_clk->fcd_step);

    size = (td_u32)hdmi_array_size(g_phy_hw_def_clk_div);
    if (phy_hw_clk_rang_value_get(phy_clk->tmds_cfg.pixel_clk, size,
        &g_phy_hw_def_clk_div[0], &phy_clk->ref_clk_div) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    hdmi_info("get ref_clk_div = %x\n", phy_clk->ref_clk_div);

    size = (td_u8)hdmi_array_size(g_phy_hw_tmds_divn_sel);
    if (phy_hw_clk_rang_value_get(phy_clk->tmds_cfg.tmds_clk, size,
        &g_phy_hw_tmds_divn_sel[0], &phy_clk->tmds_divnsel) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    /* shut down FCG */
    phy_hw_write_stb1_byte(id, 0x100, 0xB, 0x00);
    hdmi_reg_fcg_en_set(id, TD_FALSE);
    hdmi_reg_fcg_dlf_en_set(id, TD_FALSE);
    hdmi_reg_fcg_dither_en_set(id, TD_FALSE);
    hdmi_reg_fcg_lock_en_set(id, TD_FALSE);
    hdmi_info("get tmds_divnsel = %x\n", phy_clk->tmds_divnsel);

    return TD_SUCCESS;
}

static td_void phy_default_spec_set(const hdmi_hal_init *hal_init)
{
    td_u8 i;
    hdmi_hw_spec *spec = TD_NULL;
    const struct tmds_spec_params *tmp = TD_NULL;

#if defined(HDMI_PRODUCT_SS928V100) || defined(HDMI_PRODUCT_SS626V100)
    hdmi_trace_len tmp_trace;

    if (hal_init->hdmi_dev_id >= HDMI_ID_MAX) {
        hdmi_err("invalid hdmi_dev_id:%u!\n", hal_init->hdmi_dev_id);
        return;
    }

    for (tmp_trace = HDMI_TRACE_LEN_0; tmp_trace <= HDMI_TRACE_DEFAULT; tmp_trace++) {
        spec = &g_hdmi_phy_info[hal_init->hdmi_dev_id].hw_spec[tmp_trace];
        phy_hw_tmds_spec_trace_len_get(&tmp, tmp_trace);
        for (i = 0; i < HDMI_HW_PARAM_NUM; i++) {
            spec->hw_param[i].i_de_main_clk  = tmp[i].clock.dphy.drv_post1;
            spec->hw_param[i].i_de_main_data = tmp[i].data.dphy.drv_post1;
            spec->hw_param[i].i_main_clk     = tmp[i].clock.dphy.drv_main;
            spec->hw_param[i].i_main_data    = tmp[i].data.dphy.drv_main;
        }
    }
#else
    spec = &g_hdmi_phy_info[hal_init->hdmi_dev_id].hw_spec[HDMI_TRACE_DEFAULT];
    tmp = &g_tmds_spec[0];
    for (i = 0; i < HDMI_HW_PARAM_NUM; i++) {
        spec->hw_param[i].i_de_main_clk  = tmp[i].clock.dphy.drv_post1;
        spec->hw_param[i].i_de_main_data = tmp[i].data.dphy.drv_post1;
        spec->hw_param[i].i_main_clk     = tmp[i].clock.dphy.drv_main;
        spec->hw_param[i].i_main_data    = tmp[i].data.dphy.drv_main;
    }
#endif
    return;
}

td_s32 hal_hdmi_phy_init(const hdmi_hal_init *hal_init)
{
    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_null_return(hal_init, OT_ERR_HDMI_NULL_PTR);

    if (g_hdmi_phy_info[hal_init->hdmi_dev_id].init == TD_FALSE) {
        if (hal_init->phy_addr != TD_NULL) {
            hdmi_reg_tx_phy_init(hal_init->hdmi_dev_id, hal_init->phy_addr);
            g_hdmi_phy_info[hal_init->hdmi_dev_id].init = TD_TRUE;
        } else {
            hdmi_err("null pointer!\n");
            return TD_FAILURE;
        }
    }
    phy_default_spec_set(hal_init);
    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_deinit(td_u32 id)
{
    hdmi_if_fpga_return(TD_SUCCESS);
    g_hdmi_phy_info[id].init = TD_FALSE;
    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_reg_tx_phy_deinit(id);

    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_power_get(td_u32 id, td_bool *enable)
{
    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_null_return(enable, OT_ERR_HDMI_NULL_PTR);

    *enable = TD_TRUE;
    /* obtain power's status:0 reset; 1:set */
    if (hdmi_reg_resetn_get(id) == TD_FALSE) {
        *enable = TD_FALSE;
    }

    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_power_set(td_u32 id, td_bool enable)
{
    g_hdmi_phy_info[id].power_enable = enable;
    hdmi_if_fpga_return(TD_SUCCESS);
    /* PHY MODE power on or off */
    hdmi_reg_resetn_set(id, enable);

    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_oe_set(td_u32 id, td_bool enable)
{
    td_bool oe_en;
    td_u32 oe_cfg;
    td_u32 mask;

    oe_en = enable;
    g_hdmi_phy_info[id].oe_enable = oe_en;
    hdmi_if_fpga_return(TD_SUCCESS);

    mask = CFG_HDMI_OE_CH3_M | CFG_HDMI_OE_CH2_M | CFG_HDMI_OE_CH1_M | CFG_HDMI_OE_CH0_M;
    oe_cfg = cfg_hdmi_oe_ch3((td_u32)oe_en) | cfg_hdmi_oe_ch2((td_u32)oe_en) |
        cfg_hdmi_oe_ch1((td_u32)oe_en) | cfg_hdmi_oe_ch0((td_u32)oe_en);
    hdmi_write_mask(id, HDMI_OE_CFG, oe_cfg, mask);

    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_oe_get(td_u32 id, td_bool *enable)
{
    td_u32 oe_status;

    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_null_return(enable, OT_ERR_HDMI_NULL_PTR);

    if (hdmi_reg_resetn_get(id) == TD_FALSE) {
        hdmi_info("phy is reset now, OE disable.\n");
        *enable = TD_FALSE;
        return TD_SUCCESS;
    }

    oe_status = hdmi_read_mask(id, HDMI_OE_CFG,
        CFG_HDMI_OE_CH0_M | CFG_HDMI_OE_CH1_M | CFG_HDMI_OE_CH2_M | CFG_HDMI_OE_CH3_M);
    if (oe_status == 0xF) {
        *enable = TD_TRUE;
    } else {
        *enable = TD_FALSE;
    }

    return TD_SUCCESS;
}

static td_s32 phy_tmds_clk_set(td_u32 id, const hdmi_phy_tmds_cfg *tmds_cfg, phy_clk_set *phy_clk)
{
    phy_clk->tmds_cfg.mode_cfg   = tmds_cfg->mode_cfg;
    phy_clk->tmds_cfg.deep_color = tmds_cfg->deep_color;
    phy_clk->tmds_cfg.pixel_clk  = tmds_cfg->pixel_clk;
    phy_clk->tmds_cfg.tmds_clk   = tmds_cfg->tmds_clk;

    if (tmds_cfg->mode_cfg == HDMI_PHY_MODE_CFG_TMDS) {
        /* calculate ll parameter */
        hdmi_if_failure_return(hal_hdmi_clk_set_para_get(id, phy_clk), TD_FAILURE);
        /* pll feedback clock divider */
        hdmi_if_failure_return(phy_hw_fractional_mnx_get(phy_clk->tmds_cfg.tmds_clk,
            phy_clk->tmds_cfg.pixel_clk, phy_clk->tmds_cfg.deep_color), TD_FAILURE);
    }
    phy_hw_clock_set(id, phy_clk);

    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_tmds_set(td_u32 id, const hdmi_phy_tmds_cfg *tmds_cfg)
{
    td_s32           ret;
    td_u32           pixel_clk    = 0;
    hdmi_phy_ssc_cfg hdmi_ssc_cfg = {0};
    phy_clk_set      phy_clk      = {0};

    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_null_return(tmds_cfg, OT_ERR_HDMI_NULL_PTR);

    if ((tmds_cfg->mode_cfg == HDMI_PHY_MODE_CFG_TMDS) || (tmds_cfg->mode_cfg == HDMI_PHY_MODE_CFG_FRL)) {
        hdmi_info("TMDS mode: %u, pixel_clk: %u, tmds_clk: %u, deep_color: %u, trace_len: %u\n", tmds_cfg->mode_cfg,
            tmds_cfg->pixel_clk, tmds_cfg->tmds_clk, tmds_cfg->deep_color, tmds_cfg->trace_len);
        /* reset: clear all the aphy register */
        phy_hw_reset_release(id);
        /* input clock check: to configurate dphy source clock detection module */
        if (phy_hw_input_clock_check(id, &pixel_clk) != TD_SUCCESS) {
            hdmi_info("input clock unstable\n");
        }
        /* initial: finish aphy, dphy configuration, configurate the register no change with standard */
        phy_hw_init_set(id, tmds_cfg);
        hdmi_if_failure_return(phy_tmds_clk_set(id, tmds_cfg, &phy_clk), TD_FAILURE);

#if defined(HDMI_PRODUCT_SS928V100) || defined(HDMI_PRODUCT_SS626V100)
        hdmi_ssc_cfg.phy_ssc.ssc_enable = tmds_cfg->emi_enable;
#else
        hdmi_ssc_cfg.phy_ssc.ssc_enable = TD_FALSE;
#endif
        hdmi_ssc_cfg.pix_clk    = tmds_cfg->pixel_clk;
        hdmi_ssc_cfg.tmds_clk   = tmds_cfg->tmds_clk;
        hdmi_ssc_cfg.deep_color = tmds_cfg->deep_color;
        /* calculate spread spectrum */
        if (tmds_cfg->mode_cfg == HDMI_PHY_MODE_CFG_TMDS) {
            hal_hdmi_phy_ssc_set(id, &hdmi_ssc_cfg);
        }
        /* initial: index */
        ret = phy_tmds_spec_set(id, tmds_cfg);
        if (ret != TD_SUCCESS) {
            hdmi_err("phy tmds spec set fail.\n");
            return ret;
        }
        /* data path enable */
        phy_hw_post_set_up(id, tmds_cfg);
        /* then disable output and enable by user. */
        hal_hdmi_phy_oe_set(id, TD_FALSE);
        /* status info set */
        ret = memcpy_s(&g_hdmi_phy_info[id].tmds_cfg, sizeof(hdmi_phy_tmds_cfg), tmds_cfg, sizeof(*tmds_cfg));
        hdmi_unequal_eok_return(ret, OT_ERR_HDMI_INVALID_PARA);
    }

    return TD_SUCCESS;
}

static td_void hal_hdmi_phy_ssc_init(td_u32 id, td_u16 mod_d, td_u16 mod_n)
{
    /* MOD_N MOD_T */
    hdmi_reg_mod_len_set(id, TD_FALSE);
    hdmi_reg_mod_t_set(id, TD_TRUE);
    hdmi_reg_mod_n_set(id, mod_n);
    /* MOD_D */
    hdmi_reg_mod_d_set(id, mod_d);
    /* FDIV init */
    hdmi_reg_init_set(id, TD_FALSE);
    hdmi_reg_en_ctrl_set(id, TD_FALSE);
    hdmi_reg_en_mod_set(id, TD_FALSE);
    hdmi_reg_en_sdm_set(id, TD_FALSE);
    /* FDIV init */
    hdmi_reg_init_set(id, TD_TRUE);
    osal_udelay(1); /* 150 nsec */
    /* FDIV control */
    hdmi_reg_init_set(id, TD_FALSE);
    hdmi_reg_en_ctrl_set(id, TD_FALSE);
    hdmi_reg_en_mod_set(id, TD_FALSE);
    hdmi_reg_en_sdm_set(id, TD_FALSE);
    osal_udelay(1); /* 150 nsec */
    /* FDIV control */
    hdmi_reg_init_set(id, TD_FALSE);
    hdmi_reg_en_ctrl_set(id, TD_TRUE);
    hdmi_reg_en_mod_set(id, TD_FALSE);
    hdmi_reg_en_sdm_set(id, TD_FALSE);
    /* FDIV control */
    hdmi_reg_init_set(id, TD_FALSE);
    hdmi_reg_en_ctrl_set(id, TD_TRUE);
    hdmi_reg_en_mod_set(id, TD_TRUE);
    hdmi_reg_en_sdm_set(id, TD_FALSE);
    /* FDIV control */
    hdmi_reg_init_set(id, TD_FALSE);
    hdmi_reg_en_ctrl_set(id, TD_TRUE);
    hdmi_reg_en_mod_set(id, TD_TRUE);
    hdmi_reg_en_sdm_set(id, TD_TRUE);

    return;
}

static td_s32 hal_hdmi_phy_ssc_prepare(hdmi_phy_ssc_cfg *hdmi_ssc_cfg, td_u32 *ssc_freq, td_u32 *ssc_amptd)
{
    const phy_ssc_cfg *phy_ssc_cfg_tmp = phy_ssc_data_get(hdmi_ssc_cfg->tmds_clk);

    if (hdmi_ssc_cfg->phy_ssc.ssc_enable != TD_TRUE || phy_ssc_cfg_tmp == TD_NULL) {
        return TD_FAILURE;
    }
    hdmi_ssc_cfg->phy_ssc.ssc_cfg = *phy_ssc_cfg_tmp;
    *ssc_amptd = hdmi_ssc_cfg->phy_ssc.ssc_cfg.ssc_amptd;
    *ssc_freq = hdmi_ssc_cfg->phy_ssc.ssc_cfg.ssc_freq / HDMI_THOUSAND; /* k_hz/1000, unit is hz */
    if (*ssc_freq == 0) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_u16 hal_hdmi_phy_ssc_calc_mod_n(td_u32 ssc_freq)
{
    td_u32 mod_dn;

    /* g_mnx_get.pll_ref_clk * 10000 / (ssc_freq * 4) */
    mod_dn = (td_u32)osal_div64_u64(((td_u64)g_mnx_get.pll_ref_clk * 2500), ssc_freq); /* 2500, means 10000/4 */
    /* 10000 eq 1000*10. 1000 get to unit k_hz; 10 is get 1 significant digits. 5000 means rounding off mod_n */
    return (mod_dn % MOD_N_MULTI_COEFFICIENT) >= 5000 ? (td_u16)(mod_dn / MOD_N_MULTI_COEFFICIENT + 1) :
        (td_u16)(mod_dn / MOD_N_MULTI_COEFFICIENT);
}

static td_u16 hal_hdmi_phy_ssc_calc_mod_d(td_u32 ssc_amptd, td_u16 mod_n)
{
    td_u32 rem;
    td_u64 mod_dl;

    mod_dl = (td_u64)osal_div64_u64(((td_u64)phy_hw_pow(PHY_POW_BASE_NUM, PHY_POW_INDEX_NUM) *
        g_mnx_get.mn_value * ssc_amptd), mod_n);
    /* 100000 eq 1000*100. 1000 get to unit k_hz; 100 is get 2 significant digits. */
    rem = (td_u32)osal_div_u64_rem(mod_dl, MOD_D_MULTI_COEFFICIENT);
    if (rem >= 50000) { /* 50000 means rounding off mod_n */
        return (td_u16)(osal_div64_u64(mod_dl, MOD_D_MULTI_COEFFICIENT) + 1);
    }

    return (td_u16)osal_div64_u64(mod_dl, MOD_D_MULTI_COEFFICIENT);
}

td_s32 hal_hdmi_phy_ssc_set(td_u32 id, hdmi_phy_ssc_cfg *hdmi_ssc_cfg)
{
    td_u16 mod_d;
    td_u16 mod_n;
    td_u32 ssc_freq;
    td_u32 ssc_amptd;

    hdmi_if_null_return(hdmi_ssc_cfg, OT_ERR_HDMI_NULL_PTR);

    if (hal_hdmi_phy_ssc_prepare(hdmi_ssc_cfg, &ssc_freq, &ssc_amptd) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (phy_hw_fractional_mnx_get(hdmi_ssc_cfg->tmds_clk,
        hdmi_ssc_cfg->pix_clk, hdmi_ssc_cfg->deep_color) != TD_SUCCESS) {
        hdmi_info("MNX get fail\n");
    }
    mod_n = hal_hdmi_phy_ssc_calc_mod_n(ssc_freq);
    /* relative deviation = (MOD_D(0x80A0[15:0])*MOD_N(0x809C[31:16])) / (65536*(((M+1)*10)+(N+X))) */
    if (mod_n == 0) {
        return TD_FAILURE;
    }
    mod_d = hal_hdmi_phy_ssc_calc_mod_d(ssc_amptd, mod_n);
    hdmi_info("mod_n = %x, mod_d = %x \n", mod_n, mod_d);

    if (hdmi_ssc_cfg->phy_ssc.ssc_enable) {
        phy_hw_write_stb1_byte(id, APHY_CS_8, APHY_OFFSET_D, 0xFD);
        hal_hdmi_phy_ssc_init(id, mod_d, mod_n);
    } else {
        /* FDIV init */
        hdmi_reg_init_set(id, TD_TRUE);
        osal_udelay(1);
        hdmi_reg_init_set(id, TD_FALSE);
        hdmi_reg_en_ctrl_set(id, TD_FALSE);
        hdmi_reg_en_mod_set(id, TD_FALSE);
    }

    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_ssc_get(td_u32 id, td_bool *enable)
{
    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_null_return(enable, OT_ERR_HDMI_NULL_PTR);

    *enable = TD_FALSE;
    if (hdmi_reg_en_mod_get(id) && hdmi_reg_en_ctrl_get(id) && hdmi_reg_en_sdm_get(id)) {
        *enable = TD_TRUE;
    }

    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_info_get(td_u32 id, hdmi_phy_info *phy_status)
{
    td_s32 ret;

    hdmi_if_null_return(phy_status, OT_ERR_HDMI_NULL_PTR);
    /* status information obtain */
    ret = memcpy_s(phy_status, sizeof(*phy_status), &g_hdmi_phy_info[id], sizeof(hdmi_phy_info));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    return TD_SUCCESS;
}

static td_s32 hdmi_phy_spec_set(td_u32 id, const hdmi_phy_hw_spec_cfg *cfg)
{
    td_s32 ret;
    const struct tmds_spec_params *tmds = NULL;
    struct tmds_spec_params tmds_spec = {0};
    hdmi_phy_tmds_cfg tmds_cfg = {0};

    tmds_cfg.tmds_clk = cfg->tmds_clk;
    tmds_cfg.trace_len = cfg->trace_len;
    tmds = get_tmds_spec_params(&tmds_cfg);
    hdmi_if_null_return(tmds, OT_ERR_HDMI_NULL_PTR);

    ret = memcpy_s(&tmds_spec, sizeof(tmds_spec), tmds, sizeof(*tmds));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    ret = phy_user_spec_param_set(id, tmds_cfg.tmds_clk, tmds_cfg.trace_len, &tmds_spec);
    hdmi_if_failure_return(ret, TD_FAILURE);

    phy_hw_tmds_aphy_spec_set(id, &tmds_spec);
    phy_hw_tmds_dphy_spec_set(id, &tmds_spec);

    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_params_set(td_u32 id, td_u32 tmds_clk, const hdmi_hw_spec *hw_spec)
{
    errno_t ret;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_phy_hw_spec_cfg hw_spec_cfg = {0};

    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_id_check_invalid_return(id);
    hdmi_if_false_return(g_hdmi_phy_info[id].init, OT_ERR_HDMI_INVALID_PARA);
    hdmi_if_null_return(hw_spec, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device((hdmi_device_id)id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_trace_len_check_invalid_return(hdmi_dev->mode_param.trace_len);

    ret = memcpy_s(&g_hdmi_phy_info[id].spec_user[hdmi_dev->mode_param.trace_len].hw_param[0], sizeof(hdmi_hw_spec),
        &hw_spec->hw_param[0], sizeof(hw_spec->hw_param));
    hdmi_unequal_eok_return(ret, OT_ERR_HDMI_INVALID_PARA);
    ret = memcpy_s(&g_hdmi_phy_info[id].hw_spec[hdmi_dev->mode_param.trace_len].hw_param[0], sizeof(hdmi_hw_spec),
        &hw_spec->hw_param[0], sizeof(hw_spec->hw_param));
    hdmi_unequal_eok_return(ret, OT_ERR_HDMI_INVALID_PARA);

    hw_spec_cfg.tmds_clk = tmds_clk;
    hw_spec_cfg.trace_len = hdmi_dev->mode_param.trace_len;
    ret = hdmi_phy_spec_set(id, &hw_spec_cfg);

    return ret;
}

static td_void phy_def_hw_param_get(hdmi_hw_spec *hw_spec, hdmi_trace_len trace_len)
{
    td_s32 i;
    const struct tmds_spec_params *tmds_spec = TD_NULL;
#if defined(HDMI_PRODUCT_SS928V100) || defined(HDMI_PRODUCT_SS626V100)
    phy_hw_tmds_spec_trace_len_get(&tmds_spec, trace_len);
#else
    ot_unused(trace_len);
    tmds_spec = &g_tmds_spec[0];
#endif
    for (i = 0; i < HDMI_HW_PARAM_LEN; i++) {
        hw_spec->hw_param[i].i_de_main_clk = tmds_spec[i].clock.dphy.drv_post1;
        hw_spec->hw_param[i].i_de_main_data = tmds_spec[i].data.dphy.drv_post1;
        hw_spec->hw_param[i].i_main_clk = tmds_spec[i].clock.dphy.drv_main;
        hw_spec->hw_param[i].i_main_data = tmds_spec[i].data.dphy.drv_main;
    }
    return;
}

static td_void phy_cur_hw_param_get(td_u32 id, hdmi_hw_param *hw_param)
{
    td_u32 tmp;

    /* dphy data drv get */
    tmp = hdmi_read_mask(id, TMDS_DRV_CFG_CH0, CFG_DRV_POST1_CH0_M);
    hw_param->i_de_main_data = (td_u8)(tmp >> PHY_HWSPEC_I_16);
    tmp = hdmi_read_mask(id, TMDS_DRV_CFG_CH0, CFG_DRV_M_CH0_M);
    hw_param->i_main_data = (td_u8)(tmp >> PHY_HWSPEC_I_8);

    /* dphy clk drv get */
    tmp = hdmi_read_mask(id, TMDS_DRV_CFG_CH3, CFG_DRV_POST1_CH3_M);
    hw_param->i_de_main_clk = (td_u8)(tmp >> PHY_HWSPEC_I_16);
    tmp = hdmi_read_mask(id, TMDS_DRV_CFG_CH3, CFG_DRV_M_CH3_M);
    hw_param->i_main_clk = (td_u8)(tmp >> PHY_HWSPEC_I_8);

    return;
}

td_s32 hal_hdmi_phy_params_get(td_u32 id, hdmi_phy_hw_param *hw_param)
{
    errno_t ret;
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_id_check_invalid_return(id);
    hdmi_if_false_return(g_hdmi_phy_info[id].init, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(hw_param, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device((hdmi_device_id)id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);

    phy_def_hw_param_get(&hw_param->hw_spec_def, hdmi_dev->mode_param.trace_len);
    hdmi_trace_len_check_invalid_return(hdmi_dev->mode_param.trace_len);

    ret = memcpy_s(&hw_param->spec_drv_use, sizeof(hw_param->spec_drv_use),
        &g_hdmi_phy_info[id].hw_spec[hdmi_dev->mode_param.trace_len], sizeof(hdmi_hw_spec));
    hdmi_unequal_eok_return(ret, OT_ERR_HDMI_INVALID_PARA);
    ret = memcpy_s(&hw_param->spec_user_set, sizeof(hw_param->spec_user_set),
        &g_hdmi_phy_info[id].spec_user[hdmi_dev->mode_param.trace_len], sizeof(hdmi_hw_spec));
    hdmi_unequal_eok_return(ret, OT_ERR_HDMI_INVALID_PARA);

    phy_cur_hw_param_get(id, &hw_param->hw_param_cur);

    return TD_SUCCESS;
}

