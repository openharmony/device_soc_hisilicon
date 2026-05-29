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

#ifndef HDMI_SRC_MKP_HAL_PHY_V200_REGS_HDMI_REG_DPHY_H_BODY_PART01_H_
#define HDMI_SRC_MKP_HAL_PHY_V200_REGS_HDMI_REG_DPHY_H_BODY_PART01_H_

#include "ot_type.h"

typedef union {
    struct {
        unsigned int gpport0 : 16; /* [15:0] */
        unsigned int rsv_0   : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} t2gpport0;

typedef union {
    struct {
        unsigned int gpport1 : 16; /* [15:0] */
        unsigned int rsv_1   : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} t2gpport1;

typedef union {
    struct {
        unsigned int stb_cs_en : 16; /* [15:0] */
        unsigned int rsv_2     : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} phy_csen;

typedef union {
    struct {
        unsigned int stb_wen : 1; /* [0] */
        unsigned int rsv_3 : 31;  /* [31:1] */
    } bits;
    unsigned int u32;
} phy_wr;

typedef union {
    struct {
        unsigned int resetn : 1;  /* [0] */
        unsigned int rsv_4  : 31; /* [31:1] */
    } bits;
    unsigned int u32;
} resetn;

typedef union {
    struct {
        unsigned int stb_addr : 4;  /* [3:0] */
        unsigned int rsv_5    : 28; /* [31:4] */
    } bits;
    unsigned int u32;
} phy_addr;

typedef union {
    struct {
        unsigned int stb_wdata : 8;  /* [7:0] */
        unsigned int rsv_6     : 24; /* [31:8] */
    } bits;
    unsigned int u32;
} phy_wdata;

typedef union {
    struct {
        unsigned int stb_rdata : 8;  /* [7:0] */
        unsigned int rsv_7     : 24; /* [31:8] */
    } bits;
    unsigned int u32;
} phy_rdata;

typedef union {
    struct {
        unsigned int zcal     : 5;  /* [4:0] */
        unsigned int zcaldone : 1;  /* [5] */
        unsigned int zcalsub  : 2;  /* [7:6] */
        unsigned int rxsense  : 4;  /* [11:8] */
        unsigned int rsv_8    : 20; /* [31:12] */
    } bits;
    unsigned int u32;
} zcalreg;

typedef union {
    struct {
        unsigned int zcalclk : 1;  /* [0] */
        unsigned int rsv_9   : 31; /* [31:1] */
    } bits;
    unsigned int u32;
} zcalclk;

typedef union {
    struct {
        unsigned int c0shortdet  : 1;  /* [0] */
        unsigned int c1shortdet  : 1;  /* [1] */
        unsigned int c2shortdet  : 1;  /* [2] */
        unsigned int clkshortdet : 1;  /* [3] */
        unsigned int rsv_10      : 28; /* [31:4] */
    } bits;
    unsigned int u32;
} shortdet;

typedef union {
    struct {
        unsigned int rsv_11        : 12; /* [11:0] */
        unsigned int t2_plllkvdetl : 1;  /* [12] */
        unsigned int t2_plllkcdet  : 1;  /* [13] */
        unsigned int t2_plllkvdet2 : 1;  /* [14] */
        unsigned int t2_lkvdetlow  : 1;  /* [15] */
        unsigned int t2_lkvdethigh : 1;  /* [16] */
        unsigned int rsv_12        : 15; /* [31:17] */
    } bits;
    unsigned int u32;
} det;

typedef union {
    struct {
        unsigned int src_lock_val      : 8;  /* [7:0] */
        unsigned int src_lock_cnt      : 8;  /* [15:8] */
        unsigned int src_enable        : 1;  /* [16] */
        unsigned int fdsrcparam_unused : 3;  /* [19:17] */
        unsigned int rsv_13            : 12; /* [31:20] */
    } bits;
    unsigned int u32;
} fdsrcparam;

typedef union {
    struct {
        unsigned int src_cnt_opt        : 3;  /* [2:0] */
        unsigned int fdsrcfreq_unused_1 : 1;  /* [3] */
        unsigned int src_freq_opt       : 2;  /* [5:4] */
        unsigned int fdsrcfreq_unused_2 : 2;  /* [7:6] */
        unsigned int src_freq_ext       : 16; /* [23:8] */
        unsigned int rsv_14             : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} fdsrcfreq;

typedef union {
    struct {
        unsigned int src_det_stat : 4;  /* [3:0] */
        unsigned int src_cnt_out  : 20; /* [23:4] */
        unsigned int rsv_15       : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} fdsrcres;

typedef union {
    struct {
        unsigned int i_enable      : 1;  /* [0] */
        unsigned int i_run         : 1;  /* [1] */
        unsigned int ctset0_unused : 2;  /* [3:2] */
        unsigned int rsv_16        : 28; /* [31:4] */
    } bits;
    unsigned int u32;
} ctset0;

typedef union {
    struct {
        unsigned int i_mpll_fcon   : 10; /* [9:0] */
        unsigned int i_mpll_divn   : 3;  /* [12:10] */
        unsigned int i_mpll_ctlck  : 1;  /* [13] */
        unsigned int ctset1_unused : 18; /* [31:14] */
    } bits;
    unsigned int u32;
} ctset1;

typedef union {
    struct {
        unsigned int i_deci_cnt_len     : 8; /* [7:0] */
        unsigned int i_vco_st_wait_len  : 8; /* [15:8] */
        unsigned int i_vco_end_wait_len : 8; /* [23:16] */
        unsigned int i_ref_cnt_len      : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} fccntr0;

typedef union {
    struct {
        unsigned int i_ct_sel       : 1;  /* [0] */
        unsigned int i_clkdet_sel   : 1;  /* [1] */
        unsigned int i_ct_mode      : 2;  /* [3:2] */
        unsigned int fcopt_unused_1 : 4;  /* [7:4] */
        unsigned int i_ct_en        : 1;  /* [8] */
        unsigned int fcopt_unused_2 : 3;  /* [11:9] */
        unsigned int i_ct_idx_sel   : 1;  /* [12] */
        unsigned int i_deci_try_sel : 1;  /* [13] */
        unsigned int fcopt_unused   : 2;  /* [15:14] */
        unsigned int rsv_17         : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} fcopt;

typedef union {
    struct {
        unsigned int clk_ok          : 1;  /* [0] */
        unsigned int busy            : 1;  /* [1] */
        unsigned int done            : 1;  /* [2] */
        unsigned int error           : 1;  /* [3] */
        unsigned int divn            : 3;  /* [6:4] */
        unsigned int fcstat_unused_1 : 1;  /* [7] */
        unsigned int ref_clk_stat    : 1;  /* [8] */
        unsigned int pllvco_clk_stat : 1;  /* [9] */
        unsigned int fcstat_unused_2 : 2;  /* [11:10] */
        unsigned int confin_stat     : 6;  /* [17:12] */
        unsigned int fcstat_unused_3 : 2;  /* [19:18] */
        unsigned int fcon_init       : 10; /* [29:20] */
        unsigned int rsv_18          : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} fcstat;

typedef union {
    struct {
        unsigned int cnt_ref : 16; /* [15:0] */
        unsigned int rsv_19  : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} fccntval0;

typedef union {
    struct {
        unsigned int cnt_mpll : 16; /* [15:0] */
        unsigned int rsv_20   : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} fccntval1;

typedef union {
    struct {
        unsigned int divn_mpll       : 3;  /* [2:0] */
        unsigned int fcresval_unused : 1;  /* [3] */
        unsigned int fcon_mpll       : 10; /* [13:4] */
        unsigned int rsv_21          : 18; /* [31:14] */
    } bits;
    unsigned int u32;
} fcresval;

typedef union {
    struct {
        unsigned int divn_h20             : 3;  /* [2:0] */
        unsigned int fcdstepset_unused    : 1;  /* [3] */
        unsigned int up_sampler_ratio_sel : 1;  /* [4] */
        unsigned int rsv_22               : 27; /* [31:5] */
    } bits;
    unsigned int u32;
} fcdstepsetl;

typedef union {
    struct {
        unsigned int i_h2_sel   : 1;  /* [0] */
        unsigned int i_deci_sel : 1;  /* [1] */
        unsigned int rsv_23     : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} fcdstepth;

typedef union {
    struct {
        unsigned int i_deci2x_th : 16; /* [15:0] */
        unsigned int i_deci4x_th : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} fcdstepth0;

typedef union {
    struct {
        unsigned int i_deci8x_th  : 16; /* [15:0] */
        unsigned int i_deci16x_th : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} fcdstepth1;

typedef union {
    struct {
        unsigned int i_ref_cnt : 16; /* [15:0] */
        unsigned int rsv_24    : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} fccntr1;

typedef union {
    struct {
        unsigned int contin_upd_en       : 1;  /* [0] */
        unsigned int contin_upd_opt      : 1;  /* [1] */
        unsigned int contin_upd_pol      : 1;  /* [2] */
        unsigned int fccontinset0_unused : 1;  /* [3] */
        unsigned int contin_upd_step     : 4;  /* [7:4] */
        unsigned int rsv_25              : 24; /* [31:8] */
    } bits;
    unsigned int u32;
} fccontinset0;

typedef union {
    struct {
        unsigned int contin_upd_rate : 28; /* [27:0] */
        unsigned int contin_upd_time : 4;  /* [31:28] */
    } bits;
    unsigned int u32;
} fccontinset1;

typedef union {
    struct {
        unsigned int contin_upd_th_dn    : 10; /* [9:0] */
        unsigned int fccontinset2_unused : 2;  /* [11:10] */
        unsigned int contin_upd_th_up    : 10; /* [21:12] */
        unsigned int rsv_26              : 10; /* [31:22] */
    } bits;
    unsigned int u32;
} fccontinset2;

typedef union {
    struct {
        unsigned int init   : 1;  /* [0] */
        unsigned int ctrl   : 1;  /* [1] */
        unsigned int mod    : 1;  /* [2] */
        unsigned int sdm    : 1;  /* [3] */
        unsigned int rsv_27 : 28; /* [31:4] */
    } bits;
    unsigned int u32;
} fdivset0;

typedef union {
    struct {
        unsigned int step_d : 8;  /* [7:0] */
        unsigned int step_t : 8;  /* [15:8] */
        unsigned int step_n : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} fdivset1;

typedef union {
    struct {
        unsigned int up     : 1;  /* [0] */
        unsigned int dn     : 1;  /* [1] */
        unsigned int rsv_28 : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} fdivset2;

typedef union {
    struct {
        unsigned int mod_len : 8;  /* [7:0] */
        unsigned int mod_t   : 8;  /* [15:8] */
        unsigned int mod_n   : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} fdivset3;

typedef union {
    struct {
        unsigned int mod_d  : 16;  /* [15:0] */
        unsigned int rsv_29 : 16;  /* [31:16] */
    } bits;
    unsigned int u32;
} fdivset4;

typedef union {
    struct {
        unsigned int mod_up          : 1;  /* [0] */
        unsigned int mod_dn          : 1;  /* [1] */
        unsigned int fdivset5_unused : 2;  /* [3:2] */
        unsigned int rsv_30          : 28; /* [31:4] */
    } bits;
    unsigned int u32;
} fdivset5;

typedef union {
    struct {
        unsigned int stc_run : 1;  /* [0] */
        unsigned int stc_dir : 1;  /* [1] */
        unsigned int stc_ov  : 1;  /* [2] */
        unsigned int stc_un  : 1;  /* [3] */
        unsigned int stc_cnt : 16; /* [19:4] */
        unsigned int rsv_31  : 12; /* [31:20] */
    } bits;
    unsigned int u32;
} fdivs_tat0;

typedef union {
    struct {
        unsigned int i_fdiv_in : 32; /* [31:0] */
    } bits;
    unsigned int u32;
} fdivs_tat1;

typedef union {
    struct {
        unsigned int div_out : 32; /* [31:0] */
    } bits;
    unsigned int u32;
} fdivs_tat2;

typedef union {
    struct {
        unsigned int div_sdm : 16; /* [15:0] */
        unsigned int rsv_32  : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} fdivs_tat3;

typedef union {
    struct {
        unsigned int state_run        : 1;  /* [0] */
        unsigned int state_phase      : 2;  /* [2:1] */
        unsigned int state_overflow   : 1;  /* [3] */
        unsigned int state_underflow  : 1;  /* [4] */
        unsigned int fdivstat4_unused : 3;  /* [7:5] */
        unsigned int state_count      : 16; /* [23:8] */
        unsigned int rsv_33           : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} fdivs_tat4;

typedef union {
    struct {
        unsigned int i_manual_en       : 4; /* [3:0] */
        unsigned int i_divn            : 3; /* [6:4] */
        unsigned int fdivmanual_unused : 1; /* [7] */
        unsigned int i_mdiv            : 4; /* [11:8] */
        unsigned int i_ref_cnt_div     : 2; /* [13:12] */
        unsigned int i_dc_sel          : 2; /* [15:14] */
        unsigned int i_vic             : 8; /* [23:16] */
        unsigned int rsv_34            : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} fdivmanual;

typedef union {
    struct {
        unsigned int t2_refclksel       : 1;  /* [0] */
        unsigned int t2_refclksel2      : 1;  /* [1] */
        unsigned int i_ref_clk_sel      : 1;  /* [2] */
        unsigned int refclksel_unused_1 : 1;  /* [3] */
        unsigned int t2_pixelclksel     : 1;  /* [4] */
        unsigned int refclksel_unused_2 : 1;  /* [5] */


#endif /* HDMI_SRC_MKP_HAL_PHY_V200_REGS_HDMI_REG_DPHY_H_BODY_PART01_H_ */
