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

#ifndef HDMI_SRC_MKP_HAL_PHY_V200_REGS_HDMI_REG_DPHY_H_BODY_PART03_H_
#define HDMI_SRC_MKP_HAL_PHY_V200_REGS_HDMI_REG_DPHY_H_BODY_PART03_H_

    } bits;
    unsigned int u32;
} hw_rft_mode;

typedef union {
    struct {
        unsigned int req_length  : 2;  /* [1:0] */
        unsigned int stb_delay2  : 4;  /* [5:2] */
        unsigned int stb_delay1  : 4;  /* [9:6] */
        unsigned int stb_delay0  : 4;  /* [13:10] */
        unsigned int stb_acc_sel : 1;  /* [14] */
        unsigned int stb_cs_sel  : 1;  /* [15] */
        unsigned int rsv_65      : 16; /* [31:16] */
    } bits;
    unsigned int u32;
} stb_opt;

typedef union {
    struct {
        unsigned int req_done : 1;  /* [0] */
        unsigned int rsv_66   : 31; /* [31:1] */
    } bits;
    unsigned int u32;
} stb_req;

typedef union {
    struct {
        unsigned int stb_auto_rdata : 32; /* [31:0] */
    } bits;
    unsigned int u32;
} stb_data;

typedef struct {
    volatile t2gpport0          t2gp_port0;        /* 0 */
    volatile t2gpport1          t2gp_port1;        /* 4 */
    volatile phy_csen           stb_cs_en;         /* 8 */
    volatile phy_wr             stb_write_en;      /* C */
    volatile resetn             stb_reset;         /* 10 */
    volatile phy_addr           stb_addr;          /* 14 */
    volatile phy_wdata          stb_wdata;         /* 18 */
    volatile phy_rdata          stb_rdata;         /* 1C */
    volatile zcalreg            zcal_reg;          /* 20 */
    volatile zcalclk            zcal_clk;          /* 24 */
    volatile shortdet           short_det;         /* 28 */
    volatile det                stb_det;           /* 2C */
    volatile fdsrcparam         fd_src_param;      /* 30 */
    volatile fdsrcfreq          fd_src_freq;       /* 34 */
    volatile fdsrcres           fd_src_res;        /* 38 */
    volatile ctset0             ct_set0;           /* 3C */
    volatile ctset1             ct_set1;           /* 40 */
    unsigned int                reserved_0;        /* 44 */
    volatile fccntr0            fc_cntr0;          /* 48 */
    volatile fcopt              fc_opt;            /* 4C */
    volatile fcstat             fc_stat;           /* 50 */
    volatile fccntval0          fc_cnt_val0;       /* 54 */
    volatile fccntval1          fc_cnt_val1;       /* 58 */
    volatile fcresval           fc_res_sval;       /* 5C */
    volatile fcdstepsetl        fc_dstep_set;      /* 60 */
    volatile fcdstepth          fc_dstep_th;       /* 64 */
    volatile fcdstepth0         fc_dstep_th0;      /* 68 */
    volatile fcdstepth1         fc_dstep_th1;      /* 6C */
    volatile fccntr1            fc_cntr1;          /* 70 */
    volatile fccontinset0       fc_contin_set0;    /* 74 */
    volatile fccontinset1       fc_contin_set1;    /* 78 */
    volatile fccontinset2       fc_contin_set2;    /* 7C */
    unsigned int                reserved_1[4];     /* 80-8C */
    volatile fdivset0           fdiv_set0;         /* 90 */
    volatile fdivset1           fdiv_set1;         /* 94 */
    volatile fdivset2           fdiv_set2;         /* 98 */
    volatile fdivset3           fdiv_set3;         /* 9C */
    volatile fdivset4           fdiv_set4;         /* A0 */
    volatile fdivset5           fdiv_set5;         /* A4 */
    volatile fdivs_tat0         fdiv_stat0;        /* A8 */
    volatile fdivs_tat1         fdiv_stat1;        /* AC */
    volatile fdivs_tat2         fdiv_stat2;        /* B0 */
    volatile fdivs_tat3         fdiv_stat3;        /* B4 */
    volatile fdivs_tat4         fdiv_stat4;        /* B8 */
    volatile fdivmanual         fdiv_manual;       /* BC */
    volatile refclksel          ref_clk_sel;       /* C0 */
    unsigned int                reserved_2[15];    /* C4-FC */
    volatile fdpllparam         fd_pll_param;      /* 100 */
    volatile fdpllfreq          fd_pll_freq;       /* 104 */
    volatile fdpllres           fd_pll_res;        /* 108 */
    unsigned int                reserved_3[5];     /* 10c-11c */
    volatile fcgset             fcg_set;           /* 120 */
    volatile fcgcnt             fcg_cnt;           /* 124 */
    volatile fcgparam           fcg_param;         /* 128 */
    volatile fcgstate           fcg_state;         /* 12C */
    unsigned int                reserved_4[52];    /* 130-1FC */
    volatile txteloset          telo_set;          /* 200 */
    volatile txtelocont0        telo_cnt0;         /* 204 */
    volatile txtelocont1        telo_cnt1;         /* 208 */
    volatile txtelocont2        telo_cnt2;         /* 20C */
    volatile txfifoset0         tx_fifo_set0;      /* 210 */
    volatile txfifoset1         tx_fifo_set1;      /* 214 */
    volatile tx_fifo_stat0      tx_fifo_stat0;     /* 218 */
    volatile tx_fifo_stat1      tx_fifo_stat1;     /* 21C */
    volatile tx_fifo_stat2      tx_fifo_stat2;     /* 220 */
    volatile tx_fifo_stat3      tx_fifo_stat3;     /* 224 */
    volatile data_clk_inv       data_clk_inv;      /* 228 */
    volatile tx_data_out_sel    data_out_sel;      /* 22C */
    volatile hdmi_mode          stb_hdmi_mode;     /* 230 */
    volatile clk_data1          clk_data1;         /* 234 */
    volatile clk_data2          clk_data2;         /* 238 */
    volatile cfg_18_to_20       cfg15_to_20;       /* 23C */
    volatile fifo_stat_18_to_20 fifo_stat18_to_20; /* 240 */
    unsigned int                reserved_5[7];     /* 244-25c */
    volatile hsset              hs_set;            /* 260 */
    volatile hsrxsense          hs_rxsense;        /* 264 */
    volatile hs_fifo_stat       hs_fifo_stat;      /* 268 */
    unsigned int                reserved_6[37];    /* 26C-2FC */
    volatile intr_stat          intr_stat;         /* 300 */
    volatile intr_mask          intr_mask;         /* 304 */
    volatile intr_set           intr_set;          /* 308 */
    volatile clk_set            clk_set;           /* 30C */
    volatile sw_reset           sw_rst;            /* 310 */
    volatile glueset0           glue_set0;         /* 314 */
    volatile glueset1           glue_set1;         /* 318 */
    volatile ct_intr_set        ct_intr_set;       /* 31C */
    unsigned int                reserved_7[56];    /* 320-3FC */
    volatile hw_info            info_hw;           /* 400 */
    volatile hw_vers            vers_hw;           /* 404 */
    unsigned int                reserved_8[6];     /* 408-41C */
    volatile hw_ras_mode        ras_mode;          /* 420 */
    volatile hw_rfs_mode        rfs_mode;          /* 424 */
    volatile hw_rft_mode        rft_mode;          /* 428 */
    unsigned int                reserved_9[181];   /* 42C-6FC */
    volatile stb_opt            opt;               /* 700 */
    volatile stb_req            req;               /* 704 */
    volatile stb_data           rdata;             /* 708 */
} hdmitx21_dphy_reg_type;

td_s32 hdmi_reg_tx_phy_init(td_u32 id, td_char *addr);
td_u32 *hdmi_reg_tx_get_phy_addr(td_u32 id);
td_s32 hdmi_reg_tx_phy_deinit(td_u32 id);
td_void hdmi_reg_stb_cs_en_set(td_u32 id, td_u16 stb_cs_en);
td_void hdmi_reg_stb_wen_set(td_u32 id, td_u8 stb_wen);
td_void hdmi_reg_resetn_set(td_u32 id, td_u8 reg_resetn);
td_u8 hdmi_reg_resetn_get(td_u32 id);
td_void hdmi_reg_stb_addr_set(td_u32 id, td_u8 stb_addr);
td_void hdmi_reg_stb_wdata_set(td_u32 id, td_u8 stb_wdata);
td_u8 hdmi_reg_stb_rdata_get(td_u32 id);
td_void hdmi_reg_src_lock_val_set(td_u32 id, td_u8 src_lock_val);
td_void hdmi_reg_src_lock_cnt_set(td_u32 id, td_u8 src_lock_cnt);
td_void hdmi_reg_src_enable_set(td_u32 id, td_u8 src_enable);
td_void hdmi_reg_src_cnt_opt_set(td_u32 id, td_u8 src_cnt_opt);
td_void hdmi_reg_fdsrcfreq_unused1_set(td_u32 id, td_u8 fdsrcfreq_unused_1);
td_void hdmi_reg_src_freq_opt_set(td_u32 id, td_u8 src_freq_opt);
td_void hdmi_reg_fdsrcfreq_unused2_set(td_u32 id, td_u8 fdsrcfreq_unused_2);
td_void hdmi_reg_src_freq_ext_set(td_u32 id, td_u16 src_freq_ext);
td_u8 hdmi_reg_src_det_stat_get(td_u32 id);
td_u32 hdmi_reg_src_cnt_out_get(td_u32 id);
td_void hdmi_reg_clkdet_sel_set(td_u32 id, td_u8 i_clkdet_sel);
td_void hdmi_reg_divn_h20_set(td_u32 id, td_u8 up_divn_h20);
td_void hdmi_reg_up_sampler_ratio_sel_set(td_u32 id, td_u8 up_sampler_ratio_sel);
td_void hdmi_reg_init_set(td_u32 id, td_u8 init);
td_void hdmi_reg_en_ctrl_set(td_u32 id, td_u8 en_ctrl);
td_u8 hdmi_reg_en_ctrl_get(td_u32 id);
td_void hdmi_reg_en_mod_set(td_u32 id, td_u8 en_mod);
td_u8 hdmi_reg_en_mod_get(td_u32 id);
td_void hdmi_reg_en_sdm_set(td_u32 id, td_u8 en_sdm);
td_u8 hdmi_reg_en_sdm_get(td_u32 id);
td_void hdmi_reg_mod_len_set(td_u32 id, td_u8 mod_len);
td_void hdmi_reg_mod_t_set(td_u32 id, td_u8 mod_t);
td_void hdmi_reg_mod_n_set(td_u32 id, td_u16 mod_n);
td_void hdmi_reg_mod_d_set(td_u32 id, td_u16 mod_d);
td_void hdmi_reg_fdiv_in_set(td_u32 id, td_u32 i_fdiv_in);
td_void hdmi_reg_manual_en_set(td_u32 id, td_u8 i_manual_en);
td_void hdmi_reg_mdiv_set(td_u32 id, td_u8 i_mdiv);
td_void hdmi_reg_ref_clk_sel_set(td_u32 id, td_u8 i_ref_clk_sel);
td_void hdmi_reg_pr_en_h20_set(td_u32 id, td_u8 up_pr_en_h20);
td_void hdmi_reg_enable_h20_set(td_u32 id, td_u8 up_enable_h20);
td_void hdmi_reg_ch_out_sel_set(td_u32 id, td_u8 up_ch_out_sel);
td_void hdmi_reg_hsset_set(td_u32 id, td_u8 up_hsset);
td_void hdmi_reg_fd_clk_sel_set(td_u32 id, td_u8 up_fd_clk_sel);
td_void hdmi_reg_refclk_sel_set(td_u32 id, td_u8 up_refclk_sel);
td_void hdmi_reg_ctman_set(td_u32 id, td_u8 up_ctman);
td_void hdmi_reg_modclk_sel_set(td_u32 id, td_u8 up_modclk_sel);
td_void hdmi_reg_fdivclk_sel_set(td_u32 id, td_u8 up_fdivclk_sel);
td_void hdmi_reg_mod_div_val_set(td_u32 id, td_u8 mod_div_val);
td_void hdmi_reg_dac_clock_gat_set(td_u32 id, td_u8 dac_clock_gat);
td_void hdmi_reg_swreset_unused_set(td_u32 id, td_u16 swreset_unused);
td_void hdmi_reg_global_reset_set(td_u32 id, td_u8 global_reset);
td_void hdmi_reg_sw_reset_mod_clock_set(td_u32 id, td_u8 sw_reset_mod_clock);
td_void hdmi_reg_sw_reset_tmds_clock_set(td_u32 id, td_u8 sw_reset_tmds_clock);
td_void hdmi_reg_sw_reset_mpll_clock_set(td_u32 id, td_u8 sw_reset_mpll_clock);
td_void hdmi_reg_sw_reset_nco_clock_set(td_u32 id, td_u8 sw_reset_nco_clock);
td_void hdmi_reg_sw_reset_fd_clock_set(td_u32 id, td_u8 sw_reset_fd_clock);
td_void hdmi_reg_sw_reset_mod_and_mpll_clock_set(td_u32 id, td_u8 sw_reset_mod_and_mpll_clock);
td_void hdmi_reg_sw_reset_mod_and_nco_clock_set(td_u32 id, td_u8 sw_reset_mod_and_nco_clock);
td_void hdmi_reg_sw_reset_mod_and_fd_clock_set(td_u32 id, td_u8 sw_reset_mod_and_fd_clock);
td_void hdmi_reg_sw_reset_hsfifo_clock_set(td_u32 id, td_u8 sw_reset_hsfifo_clock);
td_void hdmi_reg_sw_reset_txfifo_clock_set(td_u32 id, td_u8 sw_reset_txfifo_clock);
td_void hdmi_reg_sw_reset_data_clock_set(td_u32 id, td_u8 sw_reset_data_clock);
td_void hdmi_reg_sw_reset_hs_clock_set(td_u32 id, td_u8 sw_reset_hs_clock);
td_void hdmi_reg_sw_reset_pllref_clock_set(td_u32 id, td_u8 sw_reset_pllref_clock);
td_void hdmi_reg_sw_reset_dac_clock_set(td_u32 id, td_u8 sw_reset_dac_clock);
td_void hdmi_reg_up_sample_fifo_clock_swrst_set(td_u32 id, td_u8 up_sample_fifo_clock_swrst);
td_void hdmi_reg_fcdstepset_unused_set(td_u32 id, td_u8 fcdstepset_unused);
td_void hdmi_reg_req_length_set(td_u32 id, td_u8 req_length);
td_void hdmi_reg_stb_cs_sel_set(td_u32 id, td_u8 stb_cs_sel);
td_void hdmi_reg_fdsrcparam_unused_set(td_u32 id, td_u8 fdsrcparam_unused);
td_void hdmi_reg_fcg_en_set(td_u32 id, td_u8 up_fcg_en);
td_void hdmi_reg_fcg_dlf_en_set(td_u32 id, td_u8 up_fcg_dlf_en);
td_void hdmi_reg_fcg_dither_en_set(td_u32 id, td_u8 up_fcg_dither_en);
td_void hdmi_reg_fcg_lock_en_set(td_u32 id, td_u8 up_fcg_lock_en);
td_void hdmi_reg_lock_th_set(td_u32 id, td_u8 up_lock_th);
td_void hdmi_reg_txfifoset0_unused_set(td_u32 id, td_u8 txfifoset0_unused);
td_void hdmi_reg_mode_en_set(td_u32 id, td_u8 reg_hdmi_mode_en);
td_void hdmi_reg_sw_reset_frl_clock_set(td_u32 id, td_u8 sw_reset_frl_clock);
td_void hdmi_reg_stb_delay2_set(td_u32 id, td_u8 stb_delay2);
td_void hdmi_reg_stb_delay1_set(td_u32 id, td_u8 stb_delay1);
td_void hdmi_reg_stb_delay0_set(td_u32 id, td_u8 stb_delay0);
td_void hdmi_reg_stb_acc_sel_set(td_u32 id, td_u8 stb_acc_sel);


#endif /* HDMI_SRC_MKP_HAL_PHY_V200_REGS_HDMI_REG_DPHY_H_BODY_PART03_H_ */
