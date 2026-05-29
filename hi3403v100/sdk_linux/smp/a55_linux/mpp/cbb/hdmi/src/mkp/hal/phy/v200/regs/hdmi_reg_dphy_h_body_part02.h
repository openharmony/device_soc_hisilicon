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

#ifndef HDMI_SRC_MKP_HAL_PHY_V200_REGS_HDMI_REG_DPHY_H_BODY_PART02_H_
#define HDMI_SRC_MKP_HAL_PHY_V200_REGS_HDMI_REG_DPHY_H_BODY_PART02_H_

        unsigned int pr_enc_val         : 2;  /* [7:6] */
        unsigned int rsv_35             : 24; /* [31:8] */
    } bits;
    unsigned int u32;
} refclksel;

typedef union {
    struct {
        unsigned int pll_lock_val      : 8;  /* [7:0] */
        unsigned int pll_lock_cnt      : 8;  /* [15:8] */
        unsigned int pll_enable        : 1;  /* [16] */
        unsigned int fdpllparam_unused : 3;  /* [19:17] */
        unsigned int rsv_36            : 12; /* [31:20] */
    } bits;
    unsigned int u32;
} fdpllparam;

typedef union {
    struct {
        unsigned int pll_cnt_opt        : 3;  /* [2:0] */
        unsigned int fdpllfreq_unused_1 : 1;  /* [3] */
        unsigned int pll_freq_opt       : 2;  /* [5:4] */
        unsigned int fdpllfreq_unused_2 : 2;  /* [7:6] */
        unsigned int pll_freq_ext       : 16; /* [23:8] */
        unsigned int rsv_37             : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} fdpllfreq;

typedef union {
    struct {
        unsigned int pll_det_stat : 4;  /* [3:0] */
        unsigned int pll_cnt_out  : 20; /* [23:4] */
        unsigned int rsv_38       : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} fdpllres;

typedef union {
    struct {
        unsigned int fcg_en        : 1;  /* [0] */
        unsigned int fcg_dlf_en    : 1;  /* [1] */
        unsigned int fcg_dither_en : 1;  /* [2] */
        unsigned int fcg_lock_en   : 1;  /* [3] */
        unsigned int rsv_39        : 28; /* [31:4] */
    } bits;
    unsigned int u32;
} fcgset;

typedef union {
    struct {
        unsigned int tmds_cnt_val : 16; /* [15:0] */
        unsigned int cnt1_target  : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} fcgcnt;

typedef union {
    struct {
        unsigned int lock_cnt  : 8; /* [7:0] */
        unsigned int lock_th   : 8; /* [15:8] */
        unsigned int ki        : 6; /* [21:16] */
        unsigned int lock_mode : 1; /* [22] */
        unsigned int rsv_40    : 9; /* [31:23] */
    } bits;
    unsigned int u32;
} fcgparam;

typedef union {
    struct {
        unsigned int dlf_lock : 1;  /* [0] */
        unsigned int dlf_ov   : 1;  /* [1] */
        unsigned int dlf_un   : 1;  /* [2] */
        unsigned int rsv_41   : 29; /* [31:3] */
    } bits;
    unsigned int u32;
} fcgstate;

typedef union {
    struct {
        unsigned int ch_en_h20          : 4; /* [3:0] */
        unsigned int prbs_clr_h20       : 4; /* [7:4] */
        unsigned int ch_en_h21          : 4; /* [11:8] */
        unsigned int prbs_clr_h21       : 4; /* [15:12] */
        unsigned int test_pat_type      : 3; /* [18:16] */
        unsigned int ch_test_en         : 1; /* [19] */
        unsigned int test_4to1_mux_sel0 : 2; /* [21:20] */
        unsigned int test_4to1_mux_sel1 : 2; /* [23:22] */
        unsigned int test_4to1_mux_sel2 : 2; /* [25:24] */
        unsigned int test_4to1_mux_sel3 : 2; /* [27:26] */
        unsigned int rsv_42             : 4; /* [31:28] */
    } bits;
    unsigned int u32;
} txteloset;

typedef union {
    struct {
        unsigned int test_pat_ch0   : 20; /* [19:0] */
        unsigned int test_pat_ch1_l : 10; /* [29:20] */
        unsigned int rsv_43         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} txtelocont0;

typedef union {
    struct {
        unsigned int test_pat_ch1_h : 10; /* [9:0] */
        unsigned int test_pat_ch2   : 20; /* [29:10] */
        unsigned int rsv_44         : 2;  /* [31:30] */
    } bits;
    unsigned int u32;
} txtelocont1;

typedef union {
    struct {
        unsigned int test_pat_ch3 : 20; /* [19:0] */
        unsigned int rsv_45       : 12; /* [31:20] */
    } bits;
    unsigned int u32;
} txtelocont2;

typedef union {
    struct {
        unsigned int pr_en_h20         : 1;  /* [0] */
        unsigned int enable_h20        : 1;  /* [1] */
        unsigned int txfifoset0_unused : 6;  /* [7:2] */
        unsigned int rsv_46            : 24; /* [31:8] */
    } bits;
    unsigned int u32;
} txfifoset0;

typedef union {
    struct {
        unsigned int pol_inv0_h20   : 4; /* [3:0] */
        unsigned int data_swap0_h20 : 4; /* [7:4] */
        unsigned int ch_sel0_h20    : 8; /* [15:8] */
        unsigned int pol_inv1_h20   : 4; /* [19:16] */
        unsigned int data_swap1_h20 : 4; /* [23:20] */
        unsigned int ch_sel1_h20    : 8; /* [31:24] */
    } bits;
    unsigned int u32;
} txfifoset1;

typedef union {
    struct {
        unsigned int pr_fifo_state_h20  : 12; /* [11:0] */
        unsigned int txfifostat0_unused : 12; /* [23:12] */
        unsigned int rsv_47             : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} tx_fifo_stat0;

typedef union {
    struct {
        unsigned int txfifostat1_unused_0 : 12; /* [11:0] */
        unsigned int txfifostat1_unused_1 : 12; /* [23:12] */
        unsigned int rsv_48               : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} tx_fifo_stat1;

typedef union {
    struct {
        unsigned int tx_fifo_state_h20  : 12; /* [11:0] */
        unsigned int txfifostat2_unused : 12; /* [23:12] */
        unsigned int rsv_49             : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} tx_fifo_stat2;

typedef union {
    struct {
        unsigned int txfifostat3_unused_0 : 12; /* [11:0] */
        unsigned int txfifostat3_unused_1 : 12; /* [23:12] */
        unsigned int rsv_50               : 8;  /* [31:24] */
    } bits;
    unsigned int u32;
} tx_fifo_stat3;

typedef union {
    struct {
        unsigned int dataclkinv : 1;  /* [0] */
        unsigned int rsv_51     : 31; /* [31:1] */
    } bits;
    unsigned int u32;
} data_clk_inv;

typedef union {
    struct {
        unsigned int ch_out_sel : 2;  /* [1:0] */
        unsigned int rsv_52     : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} tx_data_out_sel;

typedef union {
    struct {
        unsigned int reg_hdmi_mode_en : 1;  /* [0] */
        unsigned int rsv_53           : 31; /* [31:1] */
    } bits;
    unsigned int u32;
} hdmi_mode;

typedef union {
    struct {
        unsigned int reg_clk_data_phase0 : 10; /* [9:0] */
        unsigned int reg_clk_data_phase1 : 10; /* [19:10] */
        unsigned int reg_sw_clk_en       : 1;  /* [20] */
        unsigned int rsv_54              : 11; /* [31:21] */
    } bits;
    unsigned int u32;
} clk_data1;

typedef union {
    struct {
        unsigned int reg_clk_data_phase2 : 10; /* [9:0] */
        unsigned int reg_clk_data_phase3 : 10; /* [19:10] */
        unsigned int rsv_55              : 12; /* [31:20] */
    } bits;
    unsigned int u32;
} clk_data2;

typedef union {
    struct {
        unsigned int reg_18to20_fifo_rd_rst : 1;  /* [0] */
        unsigned int reg_18to20_fifo_wr_rst : 1;  /* [1] */
        unsigned int reg_rd_bypass          : 1;  /* [2] */
        unsigned int reg_status_rrst        : 1;  /* [3] */
        unsigned int reg_status_wrst        : 1;  /* [4] */
        unsigned int rsv_56                 : 27; /* [31:5] */
    } bits;
    unsigned int u32;
} cfg_18_to_20;

typedef union {
    struct {
        unsigned int empty_status  : 1;  /* [0] */
        unsigned int aempty_status : 1;  /* [1] */
        unsigned int full_status   : 1;  /* [2] */
        unsigned int afull_status  : 1;  /* [3] */
        unsigned int rsv_57        : 28; /* [31:4] */
    } bits;
    unsigned int u32;
} fifo_stat_18_to_20;

typedef union {
    struct {
        unsigned int hsset  : 2;   /* [1:0] */
        unsigned int rsv_58 : 30;  /* [31:2] */
    } bits;
    unsigned int u32;
} hsset;

typedef union {
    struct {
        unsigned int w_hsrxsense : 2;  /* [1:0] */
        unsigned int rsv_59      : 30; /* [31:2] */
    } bits;
    unsigned int u32;
} hsrxsense;

typedef union {
    struct {
        unsigned int fifo_state_hs : 12; /* [11:0] */
        unsigned int rsv_60        : 20; /* [31:12] */
    } bits;
    unsigned int u32;
} hs_fifo_stat;

typedef union {
    struct {
        unsigned int hs_fifo_empty_intr          : 1;  /* [0] */
        unsigned int hs_fifo_full_intr           : 1;  /* [1] */
        unsigned int up_sample_fifo_empty_intr   : 1;  /* [2] */
        unsigned int up_sample_fifo_full_intr    : 1;  /* [3] */
        unsigned int trinsmitter_fifo_empty_intr : 1;  /* [4] */
        unsigned int trinsmitter_fifo_full_intr  : 1;  /* [5] */
        unsigned int lkvdethigh_intr             : 1;  /* [6] */
        unsigned int lkvdetlow_intr              : 1;  /* [7] */
        unsigned int ct_fcon_intr                : 1;  /* [8] */
        unsigned int rsv_61                      : 23; /* [31:9] */
    } bits;
    unsigned int u32;
} intr_stat;

typedef union {
    struct {
        unsigned int hs_fifo_empty_intr_mask          : 1;  /* [0] */
        unsigned int hs_fifo_full_intr_mask           : 1;  /* [1] */
        unsigned int up_sample_fifo_empty_intr_mask   : 1;  /* [2] */
        unsigned int up_sample_fifo_full_intr_mask    : 1;  /* [3] */
        unsigned int trinsmitter_fifo_empty_intr_mask : 1;  /* [4] */
        unsigned int trinsmitter_fifo_full_intr_mask  : 1;  /* [5] */
        unsigned int lkvdethigh_intr_mask             : 1;  /* [6] */
        unsigned int lkvdetlow_intr_mask              : 1;  /* [7] */
        unsigned int ct_fcon_intr_mask                : 1;  /* [8] */
        unsigned int rsv_62                           : 23; /* [31:9] */
    } bits;
    unsigned int u32;
} intr_mask;

typedef union {
    struct {
        unsigned int hs_fifo_empty_triger_type          : 3; /* [2:0] */
        unsigned int hs_fifo_empty_intr_en              : 1; /* [3] */
        unsigned int hs_fifo_full_triger_type           : 3; /* [6:4] */
        unsigned int hs_fifo_full_intr_en               : 1; /* [7] */
        unsigned int up_sample_fifo_empty_triger_type   : 3; /* [10:8] */
        unsigned int up_sample_fifo_empty_intr_en       : 1; /* [11] */
        unsigned int up_sample_fifo_full_triger_type    : 3; /* [14:12] */
        unsigned int up_sample_fifo_full_intr_en        : 1; /* [15] */
        unsigned int transmitter_fifo_empty_triger_type : 3; /* [18:16] */
        unsigned int transmitter_fifo_empty_intr_en     : 1; /* [19] */
        unsigned int transmitter_fifo_full_triger_type  : 3; /* [22:20] */
        unsigned int transmitter_fifo_full_intr_en      : 1; /* [23] */
        unsigned int lkvdethigh_triger_type             : 3; /* [26:24] */
        unsigned int lkvdethigh_intr_en                 : 1; /* [27] */
        unsigned int lkvdetlow_triger_type              : 3; /* [30:28] */
        unsigned int lkvdetlow_intr_en                  : 1; /* [31] */
    } bits;
    unsigned int u32;
} intr_set;

typedef union {
    struct {
        unsigned int fd_clk_sel  : 2;  /* [1:0] */
        unsigned int refclk_sel  : 2;  /* [3:2] */
        unsigned int ctman       : 2;  /* [5:4] */
        unsigned int modclk_sel  : 1;  /* [6] */
        unsigned int fdivclk_sel : 1;  /* [7] */
        unsigned int mod_div_val : 4;  /* [11:8] */
        unsigned int rsv_63      : 20; /* [31:12] */
    } bits;
    unsigned int u32;
} clk_set;

typedef union {
    struct {
        unsigned int sw_reset_mod_clock          : 1;  /* [0] */
        unsigned int sw_reset_tmds_clock         : 1;  /* [1] */
        unsigned int sw_reset_mpll_clock         : 1;  /* [2] */
        unsigned int sw_reset_nco_clock          : 1;  /* [3] */
        unsigned int sw_reset_fd_clock           : 1;  /* [4] */
        unsigned int sw_reset_mod_and_mpll_clock : 1;  /* [5] */
        unsigned int sw_reset_mod_and_nco_clock  : 1;  /* [6] */
        unsigned int sw_reset_mod_and_fd_clock   : 1;  /* [7] */
        unsigned int sw_reset_hsfifo_clock       : 1;  /* [8] */
        unsigned int sw_reset_txfifo_clock       : 1;  /* [9] */
        unsigned int sw_reset_data_clock         : 1;  /* [10] */
        unsigned int sw_reset_hs_clock           : 1;  /* [11] */
        unsigned int sw_reset_pllref_clock       : 1;  /* [12] */
        unsigned int sw_reset_dac_clock          : 1;  /* [13] */
        unsigned int dac_clock_gat               : 1;  /* [14] */
        unsigned int up_sample_fifo_clock_swrst  : 1;  /* [15] */
        unsigned int sw_reset_frl_clock          : 1;  /* [16] */
        unsigned int swreset_unused              : 14; /* [30:17] */
        unsigned int global_reset                : 1;  /* [31] */
    } bits;
    unsigned int u32;
} sw_reset;

typedef union {
    struct {
        unsigned int clk0_div : 4; /* [3:0] */
        unsigned int clk1_div : 4; /* [7:4] */
        unsigned int clk2_div : 4; /* [11:8] */
        unsigned int clk3_div : 4; /* [15:12] */
        unsigned int clk4_div : 4; /* [19:16] */
        unsigned int clk5_div : 4; /* [23:20] */
        unsigned int clk6_div : 4; /* [27:24] */
        unsigned int clk7_div : 4; /* [31:28] */
    } bits;
    unsigned int u32;
} glueset0;

typedef union {
    struct {
        unsigned int clk8_div          : 4;  /* [3:0] */
        unsigned int glueset1_unused_1 : 4;  /* [7:4] */
        unsigned int clk10_div         : 4;  /* [11:8] */
        unsigned int clk11_div         : 4;  /* [15:12] */
        unsigned int clk_sel           : 4;  /* [19:16] */
        unsigned int glueset1_unused_2 : 12; /* [31:20] */
    } bits;
    unsigned int u32;
} glueset1;

typedef union {
    struct {
        unsigned int ct_fcon_triger_type : 3;  /* [2:0] */
        unsigned int ct_fcon_intr_en     : 1;  /* [3] */
        unsigned int rsv_64              : 28; /* [31:4] */
    } bits;
    unsigned int u32;
} ct_intr_set;

typedef union {
    struct {
        unsigned int hw_info : 32; /* [31:0] */
    } bits;
    unsigned int u32;
} hw_info;

typedef union {
    struct {
        unsigned int hw_vers_unused_1  : 4;  /* [3:0] */
        unsigned int hdmi20_compliance : 1;  /* [4] */
        unsigned int hdmi21_compliance : 1;  /* [5] */
        unsigned int hw_vers_unused_2  : 26; /* [31:6] */
    } bits;
    unsigned int u32;
} hw_vers;

typedef union {
    struct {
        unsigned int ras_mode : 32; /* [31:0] */
    } bits;
    unsigned int u32;
} hw_ras_mode;

typedef union {
    struct {
        unsigned int rfs_mode : 32; /* [31:0] */
    } bits;
    unsigned int u32;
} hw_rfs_mode;

typedef union {
    struct {
        unsigned int rft_mode : 32; /* [31:0] */


#endif /* HDMI_SRC_MKP_HAL_PHY_V200_REGS_HDMI_REG_DPHY_H_BODY_PART02_H_ */
