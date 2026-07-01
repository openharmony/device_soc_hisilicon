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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART02_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART02_H_


    /* Define an unsigned member */
    unsigned int u32;
} u_cil_fsm_st1_link;

/* Define the union u_phy_st0_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int physt_rg_en_d : 4;         /* [3..0]  */
        unsigned int physt_rg_en_clk : 1;       /* [4]  */
        unsigned int physt_rg_en_clk2 : 1;      /* [5]  */
        unsigned int reserved_0 : 2;            /* [7..6]  */
        unsigned int physt_rg_en_cmos : 1;      /* [8]  */
        unsigned int reserved_1 : 3;            /* [11..9]  */
        unsigned int physt_rg_int_en : 1;       /* [12]  */
        unsigned int physt_rg_ext2_en : 1;      /* [13]  */
        unsigned int physt_rg_ext_en : 1;       /* [14]  */
        unsigned int physt_rg_extbus_en : 1;    /* [15]  */
        unsigned int physt_rg_drveclk_enz : 1;  /* [16]  */
        unsigned int physt_rg_drveclk2_enz : 1; /* [17]  */
        unsigned int reserved_2 : 6;            /* [23..18]  */
        unsigned int physt_rg_mipi_mode : 1;    /* [24]  */
        unsigned int physt_rg_mipi_mode2 : 1;   /* [25]  */
        unsigned int reserved_3 : 6;            /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st0_link;

/* Define the union u_phy_st1_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int physt_da_en_deskew : 4; /* [3..0]  */
        unsigned int physt_rg_deskew : 4;    /* [7..4]  */
        unsigned int physt_rg_dirbi : 4;     /* [11..8]  */
        unsigned int physt_rg_aadh_ds : 4;   /* [15..12]  */
        unsigned int physt_rg_adv12 : 4;     /* [19..16]  */
        unsigned int physt_rg_ah_ds : 4;     /* [23..20]  */
        unsigned int physt_ad_dirb_d : 4;    /* [27..24]  */
        unsigned int reserved_0 : 4;         /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st1_link;

/* Define the union u_phy_st2_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int physt_rg_lane0_si : 4; /* [3..0]  */
        unsigned int physt_rg_lane1_si : 4; /* [7..4]  */
        unsigned int physt_rg_lane2_si : 4; /* [11..8]  */
        unsigned int physt_rg_lane3_si : 4; /* [15..12]  */
        unsigned int reserved_0 : 16;       /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st2_link;

/* Define the union u_phy_st3_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int physt_da_d0_valid : 1;    /* [0]  */
        unsigned int physt_da_d1_valid : 1;    /* [1]  */
        unsigned int physt_da_d2_valid : 1;    /* [2]  */
        unsigned int physt_da_d3_valid : 1;    /* [3]  */
        unsigned int physt_da_d_valid_inv : 4; /* [7..4]  */
        unsigned int physt_d0_term_en : 1;     /* [8]  */
        unsigned int physt_d1_term_en : 1;     /* [9]  */
        unsigned int physt_d2_term_en : 1;     /* [10]  */
        unsigned int physt_d3_term_en : 1;     /* [11]  */
        unsigned int physt_clk_term_en : 1;    /* [12]  */
        unsigned int physt_clk2_term_en : 1;   /* [13]  */
        unsigned int reserved_0 : 18;          /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st3_link;

/* Define the union u_phy_st4_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int physt_rg_eq_clk : 4;   /* [3..0]  */
        unsigned int physt_rg_eq_clk2 : 4;  /* [7..4]  */
        unsigned int physt_rg_eq_lane0 : 4; /* [11..8]  */
        unsigned int physt_rg_eq_lane1 : 4; /* [15..12]  */
        unsigned int physt_rg_eq_lane2 : 4; /* [19..16]  */
        unsigned int physt_rg_eq_lane3 : 4; /* [23..20]  */
        unsigned int reserved_0 : 8;        /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st4_link;

/* Define the union u_phy_st5_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int physt_da_ulpb_ck : 1;   /* [0]  */
        unsigned int physt_da_ulpb_ck2 : 1;  /* [1]  */
        unsigned int physt_da_idlb_clk : 1;  /* [2]  */
        unsigned int physt_da_idlb_clk2 : 1; /* [3]  */
        unsigned int physt_da_ulpb : 4;      /* [7..4]  */
        unsigned int physt_da_idlb : 4;      /* [11..8]  */
        unsigned int physt_rg_test : 4;      /* [15..12]  */
        unsigned int physt_rg_sa_en : 1;     /* [16]  */
        unsigned int reserved_0 : 15;        /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st5_link;

/* Define the union u_phy_st_so_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_ad_so_d0 : 8; /* [7..0]  */
        unsigned int phy_ad_so_d1 : 8; /* [15..8]  */
        unsigned int phy_ad_so_d2 : 8; /* [23..16]  */
        unsigned int phy_ad_so_d3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_st_so_link;

/* Define the union u_cil_fsm1_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_cyc_data_lp00 : 6;     /* [5..0]  */
        unsigned int reserved_0 : 2;            /* [7..6]  */
        unsigned int cil_cyc_data_hs0 : 6;      /* [13..8]  */
        unsigned int reserved_1 : 2;            /* [15..14]  */
        unsigned int cil_cyc_data_hs_exit : 6;  /* [21..16]  */
        unsigned int reserved_2 : 2;            /* [23..22]  */
        unsigned int cil_cyc_data_hs_trail : 6; /* [29..24]  */
        unsigned int reserved_3 : 2;            /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cil_fsm1_link;

/* Define the union u_skew_cal_lepgth0_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int skew_cal_length0 : 16; /* [15..0]  */
        unsigned int skew_cal_length1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_skew_cal_lepgth0_link;

/* Define the union u_skew_cal_lepgth1_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int skew_cal_length0 : 16; /* [15..0]  */
        unsigned int skew_cal_length1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_skew_cal_lepgth1_link;

/* Define the union u_freq_measure */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cil_cnt : 16; /* [15..0]  */
        unsigned int hs_cnt : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_freq_measure;

/* Define the union u_mipi_cil_int_raw_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int err_escape_d0_raw : 1;  /* [0]  */
        unsigned int err_escape_d1_raw : 1;  /* [1]  */
        unsigned int err_escape_d2_raw : 1;  /* [2]  */
        unsigned int err_escape_d3_raw : 1;  /* [3]  */
        unsigned int err_escape_ck_raw : 1;  /* [4]  */
        unsigned int reserved_0 : 3;         /* [7..5]  */
        unsigned int err_timeout_d0_raw : 1; /* [8]  */
        unsigned int err_timeout_d1_raw : 1; /* [9]  */
        unsigned int err_timeout_d2_raw : 1; /* [10]  */
        unsigned int err_timeout_d3_raw : 1; /* [11]  */
        unsigned int err_timeout_ck_raw : 1; /* [12]  */
        unsigned int reserved_1 : 3;         /* [15..13]  */
        unsigned int sot_detc_d0_raw : 1;    /* [16]  */
        unsigned int sot_detc_d1_raw : 1;    /* [17]  */
        unsigned int sot_detc_d2_raw : 1;    /* [18]  */
        unsigned int sot_detc_d3_raw : 1;    /* [19]  */
        unsigned int reserved_2 : 12;        /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_cil_int_raw_link;

/* Define the union u_mipi_cil_int_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int err_escape_d0_st : 1;  /* [0]  */
        unsigned int err_escape_d1_st : 1;  /* [1]  */
        unsigned int err_escape_d2_st : 1;  /* [2]  */
        unsigned int err_escape_d3_st : 1;  /* [3]  */
        unsigned int err_escape_ck_st : 1;  /* [4]  */
        unsigned int reserved_0 : 3;        /* [7..5]  */
        unsigned int err_timeout_d0_st : 1; /* [8]  */
        unsigned int err_timeout_d1_st : 1; /* [9]  */
        unsigned int err_timeout_d2_st : 1; /* [10]  */
        unsigned int err_timeout_d3_st : 1; /* [11]  */
        unsigned int err_timeout_ck_st : 1; /* [12]  */
        unsigned int reserved_1 : 3;        /* [15..13]  */
        unsigned int sot_detc_d0_st : 1;    /* [16]  */
        unsigned int sot_detc_d1_st : 1;    /* [17]  */
        unsigned int sot_detc_d2_st : 1;    /* [18]  */
        unsigned int sot_detc_d3_st : 1;    /* [19]  */
        unsigned int reserved_2 : 12;       /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_cil_int_link;

/* Define the union u_mipi_cil_int_msk_link */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int err_escape_d0_msk : 1;  /* [0]  */
        unsigned int err_escape_d1_msk : 1;  /* [1]  */
        unsigned int err_escape_d2_msk : 1;  /* [2]  */
        unsigned int err_escape_d3_msk : 1;  /* [3]  */
        unsigned int err_escape_ck_msk : 1;  /* [4]  */
        unsigned int reserved_0 : 3;         /* [7..5]  */
        unsigned int err_timeout_d0_msk : 1; /* [8]  */
        unsigned int err_timeout_d1_msk : 1; /* [9]  */
        unsigned int err_timeout_d2_msk : 1; /* [10]  */
        unsigned int err_timeout_d3_msk : 1; /* [11]  */
        unsigned int err_timeout_ck_msk : 1; /* [12]  */
        unsigned int reserved_1 : 3;         /* [15..13]  */
        unsigned int sot_detc_d0_msk : 1;    /* [16]  */
        unsigned int sot_detc_d1_msk : 1;    /* [17]  */
        unsigned int sot_detc_d2_msk : 1;    /* [18]  */
        unsigned int sot_detc_d3_msk : 1;    /* [19]  */
        unsigned int reserved_2 : 12;        /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_cil_int_msk_link;

/* Define the union u_hs_mode_select */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hs_mode : 4;     /* [3..0]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hs_mode_select;

/* Define the union u_d_p_swap */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy0_d0_swap : 1;  /* [0]  */
        unsigned int phy0_d1_swap : 1;  /* [1]  */
        unsigned int phy0_d2_swap : 1;  /* [2]  */
        unsigned int phy0_d3_swap : 1;  /* [3]  */
        unsigned int phy0_ck_swap : 1;  /* [4]  */
        unsigned int phy0_ck2_swap : 1; /* [5]  */
        unsigned int reserved_0 : 2;    /* [7..6]  */
        unsigned int phy1_d0_swap : 1;  /* [8]  */
        unsigned int phy1_d1_swap : 1;  /* [9]  */
        unsigned int phy1_d2_swap : 1;  /* [10]  */
        unsigned int phy1_d3_swap : 1;  /* [11]  */
        unsigned int phy1_ck_swap : 1;  /* [12]  */
        unsigned int phy1_ck2_swap : 1; /* [13]  */
        unsigned int reserved_1 : 2;    /* [15..14]  */
        unsigned int phy2_d0_swap : 1;  /* [16]  */
        unsigned int phy2_d1_swap : 1;  /* [17]  */
        unsigned int phy2_d2_swap : 1;  /* [18]  */
        unsigned int phy2_d3_swap : 1;  /* [19]  */
        unsigned int phy2_ck_swap : 1;  /* [20]  */
        unsigned int phy2_ck2_swap : 1; /* [21]  */
        unsigned int reserved_2 : 2;    /* [23..22]  */
        unsigned int phy3_d0_swap : 1;  /* [24]  */
        unsigned int phy3_d1_swap : 1;  /* [25]  */
        unsigned int phy3_d2_swap : 1;  /* [26]  */
        unsigned int phy3_d3_swap : 1;  /* [27]  */
        unsigned int phy3_ck_swap : 1;  /* [28]  */
        unsigned int phy3_ck2_swap : 1; /* [29]  */
        unsigned int reserved_3 : 2;    /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_d_p_swap;

/* Define the union u_phy_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy0_en : 1;     /* [0]  */
        unsigned int phy1_en : 1;     /* [1]  */
        unsigned int phy2_en : 1;     /* [2]  */
        unsigned int phy3_en : 1;     /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_en;

/* Define the union u_lane_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lane0_en : 1;    /* [0]  */
        unsigned int lane1_en : 1;    /* [1]  */
        unsigned int lane2_en : 1;    /* [2]  */
        unsigned int lane3_en : 1;    /* [3]  */
        unsigned int lane4_en : 1;    /* [4]  */
        unsigned int lane5_en : 1;    /* [5]  */
        unsigned int lane6_en : 1;    /* [6]  */
        unsigned int lane7_en : 1;    /* [7]  */
        unsigned int lane8_en : 1;    /* [8]  */
        unsigned int lane9_en : 1;    /* [9]  */
        unsigned int lane10_en : 1;   /* [10]  */
        unsigned int lane11_en : 1;   /* [11]  */
        unsigned int lane12_en : 1;   /* [12]  */
        unsigned int lane13_en : 1;   /* [13]  */
        unsigned int lane14_en : 1;   /* [14]  */
        unsigned int lane15_en : 1;   /* [15]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_lane_en;

/* Define the union u_phy_cil_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phycil0_cken : 1; /* [0]  */
        unsigned int phycil1_cken : 1; /* [1]  */
        unsigned int phycil2_cken : 1; /* [2]  */
        unsigned int phycil3_cken : 1; /* [3]  */
        unsigned int reserved_0 : 4;   /* [7..4]  */
        unsigned int cil0_rst_req : 1; /* [8]  */
        unsigned int cil1_rst_req : 1; /* [9]  */
        unsigned int cil2_rst_req : 1; /* [10]  */
        unsigned int cil3_rst_req : 1; /* [11]  */
        unsigned int cil4_rst_req : 1; /* [12]  */
        unsigned int cil5_rst_req : 1; /* [13]  */
        unsigned int cil6_rst_req : 1; /* [14]  */
        unsigned int cil7_rst_req : 1; /* [15]  */
        unsigned int reserved_1 : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_cil_ctrl;

/* Define the union u_phy_lp_select */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy1_lp_ck_sel : 1; /* [0]  */
        unsigned int phy3_lp_ck_sel : 1; /* [1]  */
        unsigned int reserved_0 : 2;     /* [3..2]  */
        unsigned int pix2_ck_sel : 1;    /* [4]  */
        unsigned int pix6_ck_sel : 1;    /* [5]  */
        unsigned int reserved_1 : 26;    /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phy_lp_select;

/* Define the union u_phycfg_mode */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phycil0_0_cfg_mode : 3;   /* [2..0]  */
        unsigned int phycil0_cfg_mode_sel : 1; /* [3]  */
        unsigned int phycil0_1_cfg_mode : 3;   /* [6..4]  */
        unsigned int reserved_0 : 1;           /* [7]  */
        unsigned int phycil1_0_cfg_mode : 3;   /* [10..8]  */
        unsigned int phycil1_cfg_mode_sel : 1; /* [11]  */
        unsigned int phycil1_1_cfg_mode : 3;   /* [14..12]  */
        unsigned int reserved_1 : 1;           /* [15]  */
        unsigned int phycil2_0_cfg_mode : 3;   /* [18..16]  */
        unsigned int phycil2_cfg_mode_sel : 1; /* [19]  */
        unsigned int phycil2_1_cfg_mode : 3;   /* [22..20]  */
        unsigned int reserved_2 : 1;           /* [23]  */
        unsigned int phycil3_0_cfg_mode : 3;   /* [26..24]  */
        unsigned int phycil3_cfg_mode_sel : 1; /* [27]  */
        unsigned int phycil3_1_cfg_mode : 3;   /* [30..28]  */
        unsigned int reserved_3 : 1;           /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_phycfg_mode;

/* Define the union u_phycfg_en */


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_INTERDRV_MIPI_RX_MIPI_RX_REG_H_BODY_PART02_H_ */
