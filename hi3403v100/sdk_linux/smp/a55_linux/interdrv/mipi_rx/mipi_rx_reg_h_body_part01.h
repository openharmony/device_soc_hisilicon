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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART01_H_


/* Define the union u_phy_mode_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_rg_en_d : 4;                /* [3..0]  */
        unsigned int phy_rg_en_clk : 1;              /* [4]  */
        unsigned int phy_rg_en_clk2 : 1;             /* [5]  */
        unsigned int reserved_0 : 2;                 /* [7..6]  */
        unsigned int phy_rg_en_cmos : 1;             /* [8]  */
        unsigned int phy_rg_en_mipi2lcmos : 1;       /* [9]  */
        unsigned int phy_rg_en_mipi2lcmos1_swap : 1; /* [10]  */
        unsigned int phy_rg_da_en_cmos_tx : 1;       /* [11]  */
        unsigned int phy_rg_int_en : 1;              /* [12]  */
        unsigned int phy_rg_ext2_en : 1;             /* [13]  */
        unsigned int phy_rg_ext_en : 1;              /* [14]  */
        unsigned int phy_rg_extbus_en : 1;           /* [15]  */
        unsigned int phy_rg_drveclk_enz : 1;         /* [16]  */
        unsigned int phy_rg_drveclk2_enz : 1;        /* [17]  */
        unsigned int reserved_1 : 6;                 /* [23..18]  */
        unsigned int phy_rg_mipi_mode : 1;           /* [24]  */
        unsigned int phy_rg_mipi_mode2 : 1;          /* [25]  */
        unsigned int reserved_2 : 6;                 /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_mode_link;

/* Define the union u_phy_skew_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_da_en_deskew : 4; /* [3..0]  */
        unsigned int phy_rg_deskew : 4;    /* [7..4]  */
        unsigned int phy_rg_dirbi : 4;     /* [11..8]  */
        unsigned int phy_rg_aadh_ds : 4;   /* [15..12]  */
        unsigned int phy_rg_adv12 : 4;     /* [19..16]  */
        unsigned int phy_rg_ah_ds : 4;     /* [23..20]  */
        unsigned int reserved_0 : 8;       /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_skew_link;

/* Define the union u_phy_lane_si_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_rg_lane0_si : 8; /* [7..0]  */
        unsigned int phy_rg_lane1_si : 8; /* [15..8]  */
        unsigned int phy_rg_lane2_si : 8; /* [23..16]  */
        unsigned int phy_rg_lane3_si : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_lane_si_link;

/* Define the union u_phy_en_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_da_d0_valid : 1;    /* [0]  */
        unsigned int phy_da_d1_valid : 1;    /* [1]  */
        unsigned int phy_da_d2_valid : 1;    /* [2]  */
        unsigned int phy_da_d3_valid : 1;    /* [3]  */
        unsigned int phy_da_d_valid_inv : 4; /* [7..4]  */
        unsigned int phy_d0_term_en : 1;     /* [8]  */
        unsigned int phy_d1_term_en : 1;     /* [9]  */
        unsigned int phy_d2_term_en : 1;     /* [10]  */
        unsigned int phy_d3_term_en : 1;     /* [11]  */
        unsigned int phy_clk_term_en : 1;    /* [12]  */
        unsigned int phy_clk2_term_en : 1;   /* [13]  */
        unsigned int reserved_0 : 18;        /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_en_link;

/* Define the union u_phy_eq_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_rg_eq_clk : 4;   /* [3..0]  */
        unsigned int phy_rg_eq_clk2 : 4;  /* [7..4]  */
        unsigned int phy_rg_eq_lane0 : 4; /* [11..8]  */
        unsigned int phy_rg_eq_lane1 : 4; /* [15..12]  */
        unsigned int phy_rg_eq_lane2 : 4; /* [19..16]  */
        unsigned int phy_rg_eq_lane3 : 4; /* [23..20]  */
        unsigned int reserved_0 : 8;      /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_eq_link;

/* Define the union u_phy_cfg_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_da_ulpb_ck0 : 1;  /* [0]  */
        unsigned int phy_da_ulpb_ck1 : 1;  /* [1]  */
        unsigned int phy_da_idlb_clk0 : 1; /* [2]  */
        unsigned int phy_da_idlb_clk1 : 1; /* [3]  */
        unsigned int phy_da_ulpb : 4;      /* [7..4]  */
        unsigned int phy_da_idlb : 4;      /* [11..8]  */
        unsigned int phy_rg_test : 4;      /* [15..12]  */
        unsigned int phy_rg_sa_en : 1;     /* [16]  */
        unsigned int reserved_0 : 15;      /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_cfg_link;

/* Define the union u_phy_data_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_data0_mipi : 8; /* [7..0]  */
        unsigned int phy_data1_mipi : 8; /* [15..8]  */
        unsigned int phy_data2_mipi : 8; /* [23..16]  */
        unsigned int phy_data3_mipi : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_data_link;

/* Define the union u_phy_ph_mipi_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_ph0_mipi : 8; /* [7..0]  */
        unsigned int phy_ph1_mipi : 8; /* [15..8]  */
        unsigned int phy_ph2_mipi : 8; /* [23..16]  */
        unsigned int phy_ph3_mipi : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_ph_mipi_link;

/* Define the union u_phy_data_mipi_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_data0_mipi_hs : 8; /* [7..0]  */
        unsigned int phy_data1_mipi_hs : 8; /* [15..8]  */
        unsigned int phy_data2_mipi_hs : 8; /* [23..16]  */
        unsigned int phy_data3_mipi_hs : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_data_mipi_link;

/* Define the union u_phy_sync_dct_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_raw_type0 : 3;        /* [2..0]  */
        unsigned int reserved_0 : 1;           /* [3]  */
        unsigned int cil_split_mode0 : 3;      /* [6..4]  */
        unsigned int reserved_1 : 1;           /* [7]  */
        unsigned int cil_code_big_endian0 : 1; /* [8]  */
        unsigned int reserved_2 : 3;           /* [11..9]  */
        unsigned int cil_raw_type1 : 3;        /* [14..12]  */
        unsigned int reserved_3 : 1;           /* [15]  */
        unsigned int cil_split_mode1 : 3;      /* [18..16]  */
        unsigned int reserved_4 : 1;           /* [19]  */
        unsigned int cil_code_big_endian1 : 1; /* [20]  */
        unsigned int reserved_5 : 3;           /* [23..21]  */
        unsigned int cil_split_sel : 1;        /* [24]  */
        unsigned int cil_sync_det_mode : 1;    /* [25]  */
        unsigned int reserved_6 : 6;           /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_dct_link;

/* Define the union u_phy_cmos_dir */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int pad_datap0_cmos_rx_en : 1; /* [0]  */
        unsigned int pad_datan0_cmos_rx_en : 1; /* [1]  */
        unsigned int pad_datap1_cmos_rx_en : 1; /* [2]  */
        unsigned int pad_datan1_cmos_rx_en : 1; /* [3]  */
        unsigned int pad_datap2_cmos_rx_en : 1; /* [4]  */
        unsigned int pad_datan2_cmos_rx_en : 1; /* [5]  */
        unsigned int pad_datap3_cmos_rx_en : 1; /* [6]  */
        unsigned int pad_datan3_cmos_rx_en : 1; /* [7]  */
        unsigned int pad_clkp_cmos_rx_en : 1;   /* [8]  */
        unsigned int pad_clkn_cmos_rx_en : 1;   /* [9]  */
        unsigned int pad_clkp2_cmos_rx_en : 1;  /* [10]  */
        unsigned int pad_clkn2_cmos_rx_en : 1;  /* [11]  */
        unsigned int pad_datap0_cmos_tx_en : 1; /* [12]  */
        unsigned int pad_datan0_cmos_tx_en : 1; /* [13]  */
        unsigned int pad_datap1_cmos_tx_en : 1; /* [14]  */
        unsigned int pad_datan1_cmos_tx_en : 1; /* [15]  */
        unsigned int pad_datap2_cmos_tx_en : 1; /* [16]  */
        unsigned int pad_datan2_cmos_tx_en : 1; /* [17]  */
        unsigned int pad_datap3_cmos_tx_en : 1; /* [18]  */
        unsigned int pad_datan3_cmos_tx_en : 1; /* [19]  */
        unsigned int pad_clkp_cmos_tx_en : 1;   /* [20]  */
        unsigned int pad_clkn_cmos_tx_en : 1;   /* [21]  */
        unsigned int pad_clkp2_cmos_tx_en : 1;  /* [22]  */
        unsigned int pad_clkn2_cmos_tx_en : 1;  /* [23]  */
        unsigned int reserved_0 : 8;            /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_cmos_dir;

/* Define the union u_phy_sync_sof0_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sof0_word4_0 : 16; /* [15..0]  */
        unsigned int cil_sof1_word4_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sof0_link;

/* Define the union u_phy_sync_sof1_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sof0_word4_1 : 16; /* [15..0]  */
        unsigned int cil_sof1_word4_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sof1_link;

/* Define the union u_phy_sync_sof2_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sof0_word4_2 : 16; /* [15..0]  */
        unsigned int cil_sof1_word4_2 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sof2_link;

/* Define the union u_phy_sync_sof3_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sof0_word4_3 : 16; /* [15..0]  */
        unsigned int cil_sof1_word4_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sof3_link;

/* Define the union u_phy_out_valid_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_lvds_out_valid : 4; /* [3..0]  */
        unsigned int cil_mipi_out_valid : 4; /* [7..4]  */
        unsigned int reserved_0 : 24;        /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_out_valid_link;

/* Define the union u_phy_data_lvds_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_data0_lvds_hs : 8; /* [7..0]  */
        unsigned int phy_data1_lvds_hs : 8; /* [15..8]  */
        unsigned int phy_data2_lvds_hs : 8; /* [23..16]  */
        unsigned int phy_data3_lvds_hs : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_data_lvds_link;

/* Define the union u_phy_deskew_cal_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int deskew_cal_cycle : 16;     /* [15..0]  */
        unsigned int deskew_cal_en : 4;         /* [19..16]  */
        unsigned int deskew_cal_rstn_en : 4;    /* [23..20]  */
        unsigned int deskew_cal_sot_bypass : 4; /* [27..24]  */
        unsigned int deskew_cal_8b1 : 4;        /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_deskew_cal_link;

/* Define the union u_phy_pix_pum_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_lane02_pix_num : 16; /* [15..0]  */
        unsigned int phy_lane13_pix_num : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_pix_pum_link;

/* Define the union u_phy_sync_sol0_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sol0_word4_0 : 16; /* [15..0]  */
        unsigned int cil_sol1_word4_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sol0_link;

/* Define the union u_phy_sync_sol1_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sol0_word4_1 : 16; /* [15..0]  */
        unsigned int cil_sol1_word4_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sol1_link;

/* Define the union u_phy_sync_sol2_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sol0_word4_2 : 16; /* [15..0]  */
        unsigned int cil_sol1_word4_2 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sol2_link;

/* Define the union u_phy_sync_sol3_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_sol0_word4_3 : 16; /* [15..0]  */
        unsigned int cil_sol1_word4_3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_sync_sol3_link;

/* Define the union u_cil_timeout_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_timeout_cyc : 24;  /* [23..0]  */
        unsigned int reserved_0 : 6;        /* [29..24]  */
        unsigned int cil_timeout_en_ck : 1; /* [30]  */
        unsigned int cil_timeout_en_d : 1;  /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cil_timeout_link;

/* Define the union u_cil_fsm0_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_cyc_clk_lp00 : 6;     /* [5..0]  */
        unsigned int reserved_0 : 2;           /* [7..6]  */
        unsigned int cil_cyc_clk_hs0 : 6;      /* [13..8]  */
        unsigned int reserved_1 : 2;           /* [15..14]  */
        unsigned int cil_cyc_clk_hs_exit : 6;  /* [21..16]  */
        unsigned int reserved_2 : 2;           /* [23..22]  */
        unsigned int cil_cyc_clk_hs_trail : 6; /* [29..24]  */
        unsigned int reserved_3 : 2;           /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cil_fsm0_link;

/* Define the union u_cil_fsm_st0_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_cur_st_d0 : 4; /* [3..0]  */
        unsigned int cil_nxt_st_d0 : 4; /* [7..4]  */
        unsigned int cil_cur_st_d1 : 4; /* [11..8]  */
        unsigned int cil_nxt_st_d1 : 4; /* [15..12]  */
        unsigned int cil_cur_st_d2 : 4; /* [19..16]  */
        unsigned int cil_nxt_st_d2 : 4; /* [23..20]  */
        unsigned int cil_cur_st_d3 : 4; /* [27..24]  */
        unsigned int cil_nxt_st_d3 : 4; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cil_fsm_st0_link;

/* Define the union u_cil_fsm_st1_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_cur_st_ck : 4;  /* [3..0]  */
        unsigned int cil_nxt_st_ck : 4;  /* [7..4]  */
        unsigned int cil_cur_st_ck2 : 4; /* [11..8]  */
        unsigned int cil_nxt_st_ck2 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;    /* [31..16]  */
    } bits;


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART01_H_ */
