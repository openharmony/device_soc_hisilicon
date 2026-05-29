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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART02_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART02_H_

        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vomux_testdata;

/* Define the union u_vo_dac_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dac_reg_rev : 16; /* [15..0]  */
        unsigned int enctr : 4;        /* [19..16]  */
        unsigned int enextref : 1;     /* [20]  */
        unsigned int pdchopper : 1;    /* [21]  */
        unsigned int envbg : 1;        /* [22]  */
        unsigned int reserved_0 : 9;   /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vo_dac_ctrl;

/* Define the union u_vo_dac_otp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dac_otp_reg : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vo_dac_otp;

/* Define the union u_vo_dac0_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cablectr : 2;    /* [1..0]  */
        unsigned int reserved_0 : 2;  /* [3..2]  */
        unsigned int dacgc : 6;       /* [9..4]  */
        unsigned int reserved_1 : 21; /* [30..10]  */
        unsigned int dac_en : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vo_dac0_ctrl;

/* Define the union u_vo_dac1_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cablectr : 2;    /* [1..0]  */
        unsigned int reserved_0 : 2;  /* [3..2]  */
        unsigned int dacgc : 6;       /* [9..4]  */
        unsigned int reserved_1 : 21; /* [30..10]  */
        unsigned int dac_en : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vo_dac1_ctrl;

/* Define the union u_vo_dac2_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cablectr : 2;    /* [1..0]  */
        unsigned int reserved_0 : 2;  /* [3..2]  */
        unsigned int dacgc : 6;       /* [9..4]  */
        unsigned int reserved_1 : 21; /* [30..10]  */
        unsigned int dac_en : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vo_dac2_ctrl;

/* Define the union u_vo_dac3_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cablectr : 2;    /* [1..0]  */
        unsigned int reserved_0 : 2;  /* [3..2]  */
        unsigned int dacgc : 6;       /* [9..4]  */
        unsigned int reserved_1 : 21; /* [30..10]  */
        unsigned int dac_en : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vo_dac3_ctrl;

/* Define the union u_vo_dac_stat0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cableout0 : 1;   /* [0]  */
        unsigned int cableout1 : 1;   /* [1]  */
        unsigned int cableout2 : 1;   /* [2]  */
        unsigned int cableout3 : 1;   /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vo_dac_stat0;

/* Define the union u_cbm_bkg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cbm_bkgcr1 : 10; /* [9..0]  */
        unsigned int cbm_bkgcb1 : 10; /* [19..10]  */
        unsigned int cbm_bkgy1 : 10;  /* [29..20]  */
        unsigned int reserved_0 : 2;  /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cbm_bkg1;

/* Define the union u_cbm_mix1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_prio0 : 4; /* [3..0]  */
        unsigned int mixer_prio1 : 4; /* [7..4]  */
        unsigned int mixer_prio2 : 4; /* [11..8]  */
        unsigned int mixer_prio3 : 4; /* [15..12]  */
        unsigned int mixer_prio4 : 4; /* [19..16]  */
        unsigned int reserved_0 : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cbm_mix1;

/* Define the union u_wbc_bmp_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbc_bmp_thd : 8; /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_bmp_thd;

/* Define the union u_cbm_bkg2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cbm_bkgcr2 : 10; /* [9..0]  */
        unsigned int cbm_bkgcb2 : 10; /* [19..10]  */
        unsigned int cbm_bkgy2 : 10;  /* [29..20]  */
        unsigned int reserved_0 : 2;  /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cbm_bkg2;

/* Define the union u_cbm_mix2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_prio0 : 4; /* [3..0]  */
        unsigned int mixer_prio1 : 4; /* [7..4]  */
        unsigned int mixer_prio2 : 4; /* [11..8]  */
        unsigned int mixer_prio3 : 4; /* [15..12]  */
        unsigned int mixer_prio4 : 4; /* [19..16]  */
        unsigned int reserved_0 : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cbm_mix2;

/* Define the union u_hc_bmp_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hc_bmp_thd : 8;  /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hc_bmp_thd;

/* Define the union u_cbm_bkg3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cbm_bkgcr3 : 10; /* [9..0]  */
        unsigned int cbm_bkgcb3 : 10; /* [19..10]  */
        unsigned int cbm_bkgy3 : 10;  /* [29..20]  */
        unsigned int reserved_0 : 2;  /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cbm_bkg3;

/* Define the union u_cbm_mix3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_prio0 : 4; /* [3..0]  */
        unsigned int mixer_prio1 : 4; /* [7..4]  */
        unsigned int mixer_prio2 : 4; /* [11..8]  */
        unsigned int reserved_0 : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cbm_mix3;

/* Define the union u_mixv0_bkg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_bkgcr : 10; /* [9..0]  */
        unsigned int mixer_bkgcb : 10; /* [19..10]  */
        unsigned int mixer_bkgy : 10;  /* [29..20]  */
        unsigned int reserved_0 : 2;   /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mixv0_bkg;

/* Define the union u_mixv0_mix */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_prio0 : 4; /* [3..0]  */
        unsigned int mixer_prio1 : 4; /* [7..4]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mixv0_mix;

/* Define the union u_mixg0_bkg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_bkgcr : 10; /* [9..0]  */
        unsigned int mixer_bkgcb : 10; /* [19..10]  */
        unsigned int mixer_bkgy : 10;  /* [29..20]  */
        unsigned int reserved_0 : 2;   /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mixg0_bkg;

/* Define the union u_mixg0_bkalpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_alpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mixg0_bkalpha;

/* Define the union u_mixg0_mix */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mixer_prio0 : 4; /* [3..0]  */
        unsigned int mixer_prio1 : 4; /* [7..4]  */
        unsigned int mixer_prio2 : 4; /* [11..8]  */
        unsigned int mixer_prio3 : 4; /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mixg0_mix;

#if (defined CONFIG_GFBG_DPU_V1) || (defined CONFIG_GFBG_DPU_V2)
typedef union {
    struct {
        unsigned int g2_link : 2;      /* [1..0]  */
        unsigned int wbc_dhd_link : 2; /* [3..2]  */
        unsigned int v2_link : 1;      /* [4]  */
        unsigned int v1_link : 1;      /* [5]  */
        unsigned int reserved_0 : 1;   /* [6]  */
        unsigned int g3_link : 2;      /* [8..7]  */
        unsigned int g4_link : 2;      /* borrow 2 bits from reserved for complie */
        unsigned int reserved_1 : 21;  /* [31..11]  */
    } bits;
    unsigned int u32;
} u_link_ctrl;
#elif (defined CONFIG_GFBG_DPU_V3)
typedef union {
    struct {
        unsigned int v2_link : 2;      /* [1..0]  */
        unsigned int g3_link : 2;      /* [3..2]  */
        unsigned int g2_link : 2;      /* borrow 2 bits from reserved for complie */
        unsigned int g4_link : 2;      /* borrow 2 bits from reserved for complie */
        unsigned int reserved : 24;  /* [31..8]  */
    } bits;
    unsigned int u32;
} u_link_ctrl;
#else
typedef union {
    struct {
        unsigned int g2_link : 2;      /* [1..0]  */
        unsigned int wbc_dhd_link : 2; /* [3..2]  */
        unsigned int v3_link : 2;      /* [5..4]  */
        unsigned int v2_link : 2;      /* [7..6]  */
        unsigned int v2v3_link : 2;   /* [9..8]  */
        unsigned int g4_link : 2;      /* [11..10]  */
        unsigned int g3_link : 2;      /* [13..12]  */
        unsigned int g3g4_link : 2;      /* [15..14]  */
        unsigned int reserved : 16;  /* [31..16]  */
    } bits;
    unsigned int u32;
} u_link_ctrl;
#endif
/* Define the union u_vpss_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vpss_en : 1;         /* [0]  */
        unsigned int chk_sum_en : 1;      /* [1]  */
        unsigned int dei_en : 1;          /* [2]  */
        unsigned int mcdi_en : 1;         /* [3]  */
        unsigned int nx2_vc1_en : 1;      /* [4]  */
        unsigned int rgme_en : 1;         /* [5]  */
        unsigned int meds_en : 1;         /* [6]  */
        unsigned int hsp_en : 1;          /* [7]  */
        unsigned int snr_en : 1;          /* [8]  */
        unsigned int tnr_en : 1;          /* [9]  */
        unsigned int rfr_en : 1;          /* [10]  */
        unsigned int ifmd_en : 1;         /* [11]  */
        unsigned int igbm_en : 1;         /* [12]  */
        unsigned int cue_en : 1;          /* [13]  */
        unsigned int scd_en : 1;          /* [14]  */
        unsigned int blk_det_en : 1;      /* [15]  */
        unsigned int reserved_0 : 7;      /* [22..16]  */
        unsigned int vpss_node_init : 1;  /* [23]  */
        unsigned int ram_bank : 4;        /* [27..24]  */
        unsigned int dei_debug_en : 1;    /* [28]  */
        unsigned int dei_repeat_mode : 1; /* [29]  */
        unsigned int reserved_1 : 2;      /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vpss_ctrl;

/* Define the union u_vpss_miscellaneous */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 4;    /* [3..0]  */
        unsigned int reserved_1 : 4;    /* [7..4]  */
        unsigned int reserved_2 : 16;   /* [23..8]  */
        unsigned int ck_gt_en : 1;      /* [24]  */
        unsigned int ck_gt_en_calc : 1; /* [25]  */
        unsigned int reserved_3 : 2;    /* [27..26]  */
        unsigned int reserved_4 : 4;    /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vpss_miscellaneous;

/* Define the union u_vpss_ftconfig */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int node_rst_en : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vpss_ftconfig;

/* Define the union u_para_up_vhd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int para_up_vhd_chn00 : 1; /* [0]  */
        unsigned int para_up_vhd_chn01 : 1; /* [1]  */
        unsigned int para_up_vhd_chn02 : 1; /* [2]  */
        unsigned int para_up_vhd_chn03 : 1; /* [3]  */
        unsigned int para_up_vhd_chn04 : 1; /* [4]  */
        unsigned int para_up_vhd_chn05 : 1; /* [5]  */
        unsigned int para_up_vhd_chn06 : 1; /* [6]  */
        unsigned int para_up_vhd_chn07 : 1; /* [7]  */
        unsigned int para_up_vhd_chn08 : 1; /* [8]  */
        unsigned int para_up_vhd_chn09 : 1; /* [9]  */
        unsigned int para_up_vhd_chn10 : 1; /* [10]  */
        unsigned int para_up_vhd_chn11 : 1; /* [11]  */
        unsigned int para_up_vhd_chn12 : 1; /* [12]  */
        unsigned int para_up_vhd_chn13 : 1; /* [13]  */
        unsigned int para_up_vhd_chn14 : 1; /* [14]  */
        unsigned int para_up_vhd_chn15 : 1; /* [15]  */
        unsigned int para_up_vhd_chn16 : 1; /* [16]  */
        unsigned int para_up_vhd_chn17 : 1; /* [17]  */
        unsigned int para_up_vhd_chn18 : 1; /* [18]  */
        unsigned int para_up_vhd_chn19 : 1; /* [19]  */
        unsigned int para_up_vhd_chn20 : 1; /* [20]  */
        unsigned int para_up_vhd_chn21 : 1; /* [21]  */
        unsigned int para_up_vhd_chn22 : 1; /* [22]  */
        unsigned int para_up_vhd_chn23 : 1; /* [23]  */
        unsigned int para_up_vhd_chn24 : 1; /* [24]  */
        unsigned int para_up_vhd_chn25 : 1; /* [25]  */
        unsigned int para_up_vhd_chn26 : 1; /* [26]  */
        unsigned int para_up_vhd_chn27 : 1; /* [27]  */
        unsigned int para_up_vhd_chn28 : 1; /* [28]  */
        unsigned int para_up_vhd_chn29 : 1; /* [29]  */
        unsigned int para_up_vhd_chn30 : 1; /* [30]  */
        unsigned int para_up_vhd_chn31 : 1; /* [31]  */
    } bits;


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART02_H_ */
