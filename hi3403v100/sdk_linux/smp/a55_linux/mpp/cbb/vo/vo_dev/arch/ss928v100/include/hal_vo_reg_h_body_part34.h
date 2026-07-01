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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART34_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART34_H_

        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_intf2_dither_sed_v2;

/* define the union reg_intf2_dither_sed_w2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_w2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_intf2_dither_sed_w2;

/* define the union reg_intf2_dither_sed_y3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_y3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_intf2_dither_sed_y3;

/* define the union reg_intf2_dither_sed_u3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_u3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_intf2_dither_sed_u3;

/* define the union reg_intf2_dither_sed_v3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_v3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_intf2_dither_sed_v3;

/* define the union reg_intf2_dither_sed_w3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_w3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_intf2_dither_sed_w3;

/* define the union reg_intf2_dither_thr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_thr_min : 16; /* [15..0]  */
        unsigned int dither_thr_max : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_intf2_dither_thr;

/* define the union reg_date_coeff0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tt_seq : 1;          /* [0]  */
        unsigned int chgain_en : 1;       /* [1]  */
        unsigned int sylp_en : 1;         /* [2]  */
        unsigned int chlp_en : 1;         /* [3]  */
        unsigned int oversam2_en : 1;     /* [4]  */
        unsigned int lunt_en : 1;         /* [5]  */
        unsigned int oversam_en : 2;      /* [7..6]  */
        unsigned int reserved_0 : 1;      /* [8]  */
        unsigned int luma_dl : 4;         /* [12..9]  */
        unsigned int agc_amp_sel : 1;     /* [13]  */
        unsigned int length_sel : 1;      /* [14]  */
        unsigned int sync_mode_scart : 1; /* [15]  */
        unsigned int sync_mode_sel : 2;   /* [17..16]  */
        unsigned int style_sel : 4;       /* [21..18]  */
        unsigned int fm_sel : 1;          /* [22]  */
        unsigned int vbi_lpf_en : 1;      /* [23]  */
        unsigned int rgb_en : 1;          /* [24]  */
        unsigned int scanline : 1;        /* [25]  */
        unsigned int pbpr_lpf_en : 1;     /* [26]  */
        unsigned int pal_half_en : 1;     /* [27]  */
        unsigned int reserved_1 : 1;      /* [28]  */
        unsigned int dis_ire : 1;         /* [29]  */
        unsigned int clpf_sel : 2;        /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff0;

/* define the union reg_date_coeff1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dac_test : 10;      /* [9..0]  */
        unsigned int date_test_mode : 2; /* [11..10]  */
        unsigned int date_test_en : 1;   /* [12]  */
        unsigned int amp_outside : 10;   /* [22..13]  */
        unsigned int c_limit_en : 1;     /* [23]  */
        unsigned int cc_seq : 1;         /* [24]  */
        unsigned int cgms_seq : 1;       /* [25]  */
        unsigned int vps_seq : 1;        /* [26]  */
        unsigned int wss_seq : 1;        /* [27]  */
        unsigned int cvbs_limit_en : 1;  /* [28]  */
        unsigned int c_gain : 3;         /* [31..29]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff1;

/* define the union reg_date_coeff3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int coef03 : 26;    /* [25..0]  */
        unsigned int reserved_0 : 6; /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff3;

/* define the union reg_date_coeff4 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int coef04 : 30;    /* [29..0]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff4;

/* define the union reg_date_coeff5 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int coef05 : 29;    /* [28..0]  */
        unsigned int reserved_0 : 3; /* [31..29]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff5;

/* define the union reg_date_coeff6 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int coef06_1 : 23;  /* [22..0]  */
        unsigned int reserved_0 : 8; /* [30..23]  */
        unsigned int coef06_0 : 1;   /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff6;

/* define the union reg_date_coeff7 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tt07_enf2 : 1; /* [0]  */
        unsigned int tt08_enf2 : 1; /* [1]  */
        unsigned int tt09_enf2 : 1; /* [2]  */
        unsigned int tt10_enf2 : 1; /* [3]  */
        unsigned int tt11_enf2 : 1; /* [4]  */
        unsigned int tt12_enf2 : 1; /* [5]  */
        unsigned int tt13_enf2 : 1; /* [6]  */
        unsigned int tt14_enf2 : 1; /* [7]  */
        unsigned int tt15_enf2 : 1; /* [8]  */
        unsigned int tt16_enf2 : 1; /* [9]  */
        unsigned int tt17_enf2 : 1; /* [10]  */
        unsigned int tt18_enf2 : 1; /* [11]  */
        unsigned int tt19_enf2 : 1; /* [12]  */
        unsigned int tt20_enf2 : 1; /* [13]  */
        unsigned int tt21_enf2 : 1; /* [14]  */
        unsigned int tt22_enf2 : 1; /* [15]  */
        unsigned int tt07_enf1 : 1; /* [16]  */
        unsigned int tt08_enf1 : 1; /* [17]  */
        unsigned int tt09_enf1 : 1; /* [18]  */
        unsigned int tt10_enf1 : 1; /* [19]  */
        unsigned int tt11_enf1 : 1; /* [20]  */
        unsigned int tt12_enf1 : 1; /* [21]  */
        unsigned int tt13_enf1 : 1; /* [22]  */
        unsigned int tt14_enf1 : 1; /* [23]  */
        unsigned int tt15_enf1 : 1; /* [24]  */
        unsigned int tt16_enf1 : 1; /* [25]  */
        unsigned int tt17_enf1 : 1; /* [26]  */
        unsigned int tt18_enf1 : 1; /* [27]  */
        unsigned int tt19_enf1 : 1; /* [28]  */
        unsigned int tt20_enf1 : 1; /* [29]  */
        unsigned int tt21_enf1 : 1; /* [30]  */
        unsigned int tt22_enf1 : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff7;

/* define the union reg_date_coeff10 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tt_pktoff : 8;    /* [7..0]  */
        unsigned int tt_mode : 2;      /* [9..8]  */
        unsigned int tt_highest : 1;   /* [10]  */
        unsigned int full_page : 1;    /* [11]  */
        unsigned int nabts_100ire : 1; /* [12]  */
        unsigned int reserved_0 : 18;  /* [30..13]  */
        unsigned int tt_ready : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff10;

/* define the union reg_date_coeff11 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int date_clf2 : 10;  /* [9..0]  */
        unsigned int date_clf1 : 10;  /* [19..10]  */
        unsigned int cc_enf2 : 1;     /* [20]  */
        unsigned int cc_enf1 : 1;     /* [21]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff11;

/* define the union reg_date_coeff12 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cc_f2data : 16; /* [15..0]  */
        unsigned int cc_f1data : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff12;

/* define the union reg_date_coeff13 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cg_f1data : 20;  /* [19..0]  */
        unsigned int cg_enf2 : 1;     /* [20]  */
        unsigned int cg_enf1 : 1;     /* [21]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff13;

/* define the union reg_date_coeff14 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cg_f2data : 20;  /* [19..0]  */
        unsigned int reserved_0 : 12; /* [31..20]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff14;

/* define the union reg_date_coeff15 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wss_data : 14;   /* [13..0]  */
        unsigned int wss_en : 1;      /* [14]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff15;

/* define the union reg_date_coeff16 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vps_data : 24;  /* [23..0]  */
        unsigned int vps_en : 1;     /* [24]  */
        unsigned int reserved_0 : 7; /* [31..25]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff16;

/* define the union reg_date_coeff19 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vps_data : 16;   /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff19;

/* define the union reg_date_coeff20 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tt05_enf2 : 1;   /* [0]  */
        unsigned int tt06_enf2 : 1;   /* [1]  */
        unsigned int tt06_enf1 : 1;   /* [2]  */
        unsigned int reserved_0 : 29; /* [31..3]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff20;

/* define the union reg_date_coeff21 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dac0_in_sel : 3; /* [2..0]  */
        unsigned int reserved_0 : 1;  /* [3]  */
        unsigned int dac1_in_sel : 3; /* [6..4]  */
        unsigned int reserved_1 : 1;  /* [7]  */
        unsigned int dac2_in_sel : 3; /* [10..8]  */
        unsigned int reserved_2 : 1;  /* [11]  */
        unsigned int dac3_in_sel : 3; /* [14..12]  */
        unsigned int reserved_3 : 1;  /* [15]  */
        unsigned int dac4_in_sel : 3; /* [18..16]  */
        unsigned int reserved_4 : 1;  /* [19]  */
        unsigned int dac5_in_sel : 3; /* [22..20]  */
        unsigned int reserved_5 : 9;  /* [31..23]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff21;

/* define the union reg_date_coeff22 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_phase_delta : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;        /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff22;

/* define the union reg_date_coeff23 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dac0_out_dly : 3; /* [2..0]  */
        unsigned int reserved_0 : 1;   /* [3]  */
        unsigned int dac1_out_dly : 3; /* [6..4]  */
        unsigned int reserved_1 : 1;   /* [7]  */
        unsigned int dac2_out_dly : 3; /* [10..8]  */
        unsigned int reserved_2 : 1;   /* [11]  */
        unsigned int dac3_out_dly : 3; /* [14..12]  */
        unsigned int reserved_3 : 1;   /* [15]  */
        unsigned int dac4_out_dly : 3; /* [18..16]  */
        unsigned int reserved_4 : 1;   /* [19]  */
        unsigned int dac5_out_dly : 3; /* [22..20]  */
        unsigned int reserved_5 : 9;   /* [31..23]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff23;

/* define the union reg_date_coeff25 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int x_n_coef : 13;   /* [12..0]  */
        unsigned int reserved_0 : 3;  /* [15..13]  */
        unsigned int x_n_1_coef : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;  /* [31..29]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff25;

/* define the union reg_date_coeff26 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int x_n_1_coef : 13; /* [12..0]  */
        unsigned int reserved_0 : 19; /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff26;

/* define the union reg_date_coeff27 */
typedef union {


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART34_H_ */
