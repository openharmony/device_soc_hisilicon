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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART01_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART01_H_


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

/* define the union reg_voctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 23;      /* [22..0]  */
        unsigned int g3_ck_gt_en : 1;      /* [23]  */
        unsigned int v2_ck_gt_en : 1;      /* [24]  */
        unsigned int wbc_dhd_ck_gt_en : 1; /* [25]  */
        unsigned int g1_ck_gt_en : 1;      /* [26]  */
        unsigned int g0_ck_gt_en : 1;      /* [27]  */
        unsigned int v1_ck_gt_en : 1;      /* [28]  */
        unsigned int v0_ck_gt_en : 1;      /* [29]  */
        unsigned int chk_sum_en : 1;       /* [30]  */
        unsigned int vo_ck_gt_en : 1;      /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_voctrl;

/* define the union reg_vointsta */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dhd0vtthd1_int : 1;   /* [0]  */
        unsigned int dhd0vtthd2_int : 1;   /* [1]  */
        unsigned int dhd0vtthd3_int : 1;   /* [2]  */
        unsigned int dhd0uf_int : 1;       /* [3]  */
        unsigned int dhd1vtthd1_int : 1;   /* [4]  */
        unsigned int dhd1vtthd2_int : 1;   /* [5]  */
        unsigned int dhd1vtthd3_int : 1;   /* [6]  */
        unsigned int dhd1uf_int : 1;       /* [7]  */
        unsigned int dsdvtthd1_int : 1;    /* [8]  */
        unsigned int dsdvtthd2_int : 1;    /* [9]  */
        unsigned int dsdvtthd3_int : 1;    /* [10]  */
        unsigned int dsduf_int : 1;        /* [11]  */
        unsigned int b0_err_int : 1;       /* [12]  */
        unsigned int b1_err_int : 1;       /* [13]  */
        unsigned int b2_err_int : 1;       /* [14]  */
        unsigned int wbc_dhd_over_int : 1; /* [15]  */
        unsigned int vdac0_int : 1;        /* [16]  */
        unsigned int vdac1_int : 1;        /* [17]  */
        unsigned int vdac2_int : 1;        /* [18]  */
        unsigned int vdac3_int : 1;        /* [19]  */
        unsigned int v0_tunl_int : 1;      /* [20]  */
        unsigned int v1_tunl_int : 1;      /* [21]  */
        unsigned int reserved_0 : 10;      /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vointsta;

/* define the union reg_vomskintsta */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dhd0vtthd1_clr : 1;   /* [0]  */
        unsigned int dhd0vtthd2_clr : 1;   /* [1]  */
        unsigned int dhd0vtthd3_clr : 1;   /* [2]  */
        unsigned int dhd0uf_clr : 1;       /* [3]  */
        unsigned int dhd1vtthd1_clr : 1;   /* [4]  */
        unsigned int dhd1vtthd2_clr : 1;   /* [5]  */
        unsigned int dhd1vtthd3_clr : 1;   /* [6]  */
        unsigned int dhd1uf_clr : 1;       /* [7]  */
        unsigned int dsdvtthd1_clr : 1;    /* [8]  */
        unsigned int dsdvtthd2_clr : 1;    /* [9]  */
        unsigned int dsdvtthd3_clr : 1;    /* [10]  */
        unsigned int dsduf_clr : 1;        /* [11]  */
        unsigned int b0_err_clr : 1;       /* [12]  */
        unsigned int b1_err_clr : 1;       /* [13]  */
        unsigned int b2_err_clr : 1;       /* [14]  */
        unsigned int wbc_dhd_over_clr : 1; /* [15]  */
        unsigned int vdac0_clr : 1;        /* [16]  */
        unsigned int vdac1_clr : 1;        /* [17]  */
        unsigned int vdac2_clr : 1;        /* [18]  */
        unsigned int vdac3_clr : 1;        /* [19]  */
        unsigned int v0_tunl_clr : 1;      /* [20]  */
        unsigned int v1_tunl_clr : 1;      /* [21]  */
        unsigned int reserved_0 : 10;      /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vomskintsta;

/* define the union reg_vointmsk */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dhd0vtthd1_intmask : 1;   /* [0]  */
        unsigned int dhd0vtthd2_intmask : 1;   /* [1]  */
        unsigned int dhd0vtthd3_intmask : 1;   /* [2]  */
        unsigned int dhd0uf_intmask : 1;       /* [3]  */
        unsigned int dhd1vtthd1_intmask : 1;   /* [4]  */
        unsigned int dhd1vtthd2_intmask : 1;   /* [5]  */
        unsigned int dhd1vtthd3_intmask : 1;   /* [6]  */
        unsigned int dhd1uf_intmask : 1;       /* [7]  */
        unsigned int dsdvtthd1_intmask : 1;    /* [8]  */
        unsigned int dsdvtthd2_intmask : 1;    /* [9]  */
        unsigned int dsdvtthd3_intmask : 1;    /* [10]  */
        unsigned int dsduf_intmask : 1;        /* [11]  */
        unsigned int b0_err_intmask : 1;       /* [12]  */
        unsigned int b1_err_intmask : 1;       /* [13]  */
        unsigned int b2_err_intmask : 1;       /* [14]  */
        unsigned int wbc_dhd_over_intmask : 1; /* [15]  */
        unsigned int vdac0_intmask : 1;        /* [16]  */
        unsigned int vdac1_intmask : 1;        /* [17]  */
        unsigned int vdac2_intmask : 1;        /* [18]  */
        unsigned int vdac3_intmask : 1;        /* [19]  */
        unsigned int v0_tunl_intmask : 1;      /* [20]  */
        unsigned int v1_tunl_intmask : 1;      /* [21]  */
        unsigned int reserved_0 : 10;          /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vointmsk;

/* define the union reg_vodebug */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int rm_en_chn : 4;     /* [3..0]  */
        unsigned int dhd0_ff_info : 2;  /* [5..4]  */
        unsigned int dhd1_ff_info : 2;  /* [7..6]  */
        unsigned int dsd0_ff_info : 2;  /* [9..8]  */
        unsigned int bfm_vga_en : 1;    /* [10]  */
        unsigned int bfm_cvbs_en : 1;   /* [11]  */
        unsigned int bfm_lcd_en : 1;    /* [12]  */
        unsigned int bfm_bt1120_en : 1; /* [13]  */
        unsigned int wbc2_ff_info : 2;  /* [15..14]  */
        unsigned int wbc_mode : 4;      /* [19..16]  */
        unsigned int node_num : 4;      /* [23..20]  */
        unsigned int wbc_cmp_mode : 2;  /* [25..24]  */
        unsigned int bfm_mode : 3;      /* [28..26]  */
        unsigned int bfm_clk_sel : 3;   /* [31..29]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vodebug;

/* define the union reg_vointsta1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dhd0vtthd1_int : 1;   /* [0]  */
        unsigned int dhd0vtthd2_int : 1;   /* [1]  */
        unsigned int dhd0vtthd3_int : 1;   /* [2]  */
        unsigned int dhd0uf_int : 1;       /* [3]  */
        unsigned int dhd1vtthd1_int : 1;   /* [4]  */
        unsigned int dhd1vtthd2_int : 1;   /* [5]  */
        unsigned int dhd1vtthd3_int : 1;   /* [6]  */
        unsigned int dhd1uf_int : 1;       /* [7]  */
        unsigned int dsdvtthd1_int : 1;    /* [8]  */
        unsigned int dsdvtthd2_int : 1;    /* [9]  */
        unsigned int dsdvtthd3_int : 1;    /* [10]  */
        unsigned int dsduf_int : 1;        /* [11]  */
        unsigned int b0_err_int : 1;       /* [12]  */
        unsigned int b1_err_int : 1;       /* [13]  */
        unsigned int b2_err_int : 1;       /* [14]  */
        unsigned int wbc_dhd_over_int : 1; /* [15]  */
        unsigned int vdac0_int : 1;        /* [16]  */
        unsigned int vdac1_int : 1;        /* [17]  */
        unsigned int vdac2_int : 1;        /* [18]  */
        unsigned int vdac3_int : 1;        /* [19]  */
        unsigned int v0_tunl_int : 1;      /* [20]  */
        unsigned int v1_tunl_int : 1;      /* [21]  */
        unsigned int reserved_0 : 10;      /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vointsta1;

/* define the union reg_vomskintsta1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dhd0vtthd1_clr : 1;   /* [0]  */
        unsigned int dhd0vtthd2_clr : 1;   /* [1]  */
        unsigned int dhd0vtthd3_clr : 1;   /* [2]  */
        unsigned int dhd0uf_clr : 1;       /* [3]  */
        unsigned int dhd1vtthd1_clr : 1;   /* [4]  */
        unsigned int dhd1vtthd2_clr : 1;   /* [5]  */
        unsigned int dhd1vtthd3_clr : 1;   /* [6]  */
        unsigned int dhd1uf_clr : 1;       /* [7]  */
        unsigned int dsdvtthd1_clr : 1;    /* [8]  */
        unsigned int dsdvtthd2_clr : 1;    /* [9]  */
        unsigned int dsdvtthd3_clr : 1;    /* [10]  */
        unsigned int dsduf_clr : 1;        /* [11]  */
        unsigned int b0_err_clr : 1;       /* [12]  */
        unsigned int b1_err_clr : 1;       /* [13]  */
        unsigned int b2_err_clr : 1;       /* [14]  */
        unsigned int wbc_dhd_over_clr : 1; /* [15]  */
        unsigned int vdac0_clr : 1;        /* [16]  */
        unsigned int vdac1_clr : 1;        /* [17]  */
        unsigned int vdac2_clr : 1;        /* [18]  */
        unsigned int vdac3_clr : 1;        /* [19]  */
        unsigned int v0_tunl_clr : 1;      /* [20]  */
        unsigned int v1_tunl_clr : 1;      /* [21]  */
        unsigned int reserved_0 : 10;      /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vomskintsta1;

/* define the union reg_vointmsk1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dhd0vtthd1_intmask : 1;   /* [0]  */
        unsigned int dhd0vtthd2_intmask : 1;   /* [1]  */
        unsigned int dhd0vtthd3_intmask : 1;   /* [2]  */
        unsigned int dhd0uf_intmask : 1;       /* [3]  */
        unsigned int dhd1vtthd1_intmask : 1;   /* [4]  */
        unsigned int dhd1vtthd2_intmask : 1;   /* [5]  */
        unsigned int dhd1vtthd3_intmask : 1;   /* [6]  */
        unsigned int dhd1uf_intmask : 1;       /* [7]  */
        unsigned int dsdvtthd1_intmask : 1;    /* [8]  */
        unsigned int dsdvtthd2_intmask : 1;    /* [9]  */
        unsigned int dsdvtthd3_intmask : 1;    /* [10]  */
        unsigned int dsduf_intmask : 1;        /* [11]  */
        unsigned int b0_err_intmask : 1;       /* [12]  */
        unsigned int b1_err_intmask : 1;       /* [13]  */
        unsigned int b2_err_intmask : 1;       /* [14]  */
        unsigned int wbc_dhd_over_intmask : 1; /* [15]  */
        unsigned int vdac0_intmask : 1;        /* [16]  */
        unsigned int vdac1_intmask : 1;        /* [17]  */
        unsigned int vdac2_intmask : 1;        /* [18]  */
        unsigned int vdac3_intmask : 1;        /* [19]  */
        unsigned int v0_tunl_intmask : 1;      /* [20]  */
        unsigned int v1_tunl_intmask : 1;      /* [21]  */
        unsigned int reserved_0 : 10;          /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vointmsk1;

/* define the union reg_volowpower_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int rasehd_twac : 2;  /* [1..0]  */
        unsigned int rasehd_twa : 2;   /* [3..2]  */
        unsigned int rashd_tselw : 2;  /* [5..4]  */
        unsigned int rashd_tselr : 3;  /* [8..6]  */
        unsigned int rfshd_tselw : 2;  /* [10..9]  */
        unsigned int rfshd_tselr : 3;  /* [13..11]  */
        unsigned int rfsehd_tselw : 2; /* [15..14]  */
        unsigned int rfsehd_tselr : 3; /* [18..16]  */
        unsigned int rasehd_tselw : 2; /* [20..19]  */
        unsigned int rasehd_tselr : 3; /* [23..21]  */
        unsigned int rfthd_tselw : 2;  /* [25..24]  */
        unsigned int rfthd_tselr : 2;  /* [27..26]  */
        unsigned int rftehd_tselw : 2; /* [29..28]  */
        unsigned int reserved_0 : 2;   /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_volowpower_ctrl;

/* define the union reg_voufsta */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int v0_uf_sta : 1;   /* [0]  */
        unsigned int v1_uf_sta : 1;   /* [1]  */
        unsigned int reserved_0 : 1;  /* [2]  */
        unsigned int v3_uf_sta : 1;   /* [3]  */
        unsigned int reserved_1 : 4;  /* [7..4]  */
        unsigned int g0_uf_sta : 1;   /* [8]  */
        unsigned int g1_uf_sta : 1;   /* [9]  */
        unsigned int g2_uf_sta : 1;   /* [10]  */
        unsigned int g3_uf_sta : 1;   /* [11]  */
        unsigned int g4_uf_sta : 1;   /* [12]  */
        unsigned int reserved_2 : 19; /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_voufsta;

/* define the union reg_voufclr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int v0_uf_clr : 1;   /* [0]  */
        unsigned int v1_uf_clr : 1;   /* [1]  */
        unsigned int reserved_0 : 1;  /* [2]  */
        unsigned int v3_uf_clr : 1;   /* [3]  */
        unsigned int reserved_1 : 4;  /* [7..4]  */
        unsigned int g0_uf_clr : 1;   /* [8]  */
        unsigned int g1_uf_clr : 1;   /* [9]  */
        unsigned int g2_uf_clr : 1;   /* [10]  */
        unsigned int g3_uf_clr : 1;   /* [11]  */
        unsigned int g4_uf_clr : 1;   /* [12]  */
        unsigned int reserved_2 : 19; /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_voufclr;

/* define the union reg_vointproc_tim */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vointproc_time : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;      /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vointproc_tim;

/* define the union reg_volowpower_ctrl1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int rftehd_tselr : 3; /* [2..0]  */
        unsigned int rftehd_tselm : 2; /* [4..3]  */
        unsigned int rasehd_test : 3;  /* [7..5]  */
        unsigned int rashd_test : 3;   /* [10..8]  */
        unsigned int rfsehd_test : 3;  /* [13..11]  */
        unsigned int rfshd_test : 3;   /* [16..14]  */
        unsigned int rftehd_test : 3;  /* [19..17]  */
        unsigned int rfthd_test : 3;   /* [22..20]  */
        unsigned int reserved_0 : 9;   /* [31..23]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_volowpower_ctrl1;

/* define the union reg_vofpgadef */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_hdr_v_def : 1;  /* [0]  */
        unsigned int ot_hdr_g_def : 1;  /* [1]  */
        unsigned int ot_hdr_wd_def : 1; /* [2]  */
        unsigned int reserved_0 : 29;  /* [31..3]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vofpgadef;

/* define the union reg_volowpower_ctrl2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int s14_rfshd_rm : 4;   /* [3..0]  */
        unsigned int s14_rfshs_rm : 4;   /* [7..4]  */
        unsigned int s14_rasehd_rm : 4;  /* [11..8]  */
        unsigned int s14_rashd_rm : 4;   /* [15..12]  */
        unsigned int s14_rfshd_rme : 1;  /* [16]  */
        unsigned int s14_rfshs_rme : 1;  /* [17]  */
        unsigned int s14_rasehd_rme : 1; /* [18]  */
        unsigned int s14_rashd_rme : 1;  /* [19]  */
        unsigned int s14_rfthd_rma : 4;  /* [23..20]  */
        unsigned int s14_rfthd_rmb : 4;  /* [27..24]  */
        unsigned int s14_rfthd_rmea : 1; /* [28]  */
        unsigned int s14_rfthd_rmeb : 1; /* [29]  */
        unsigned int reserved_0 : 2;     /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_volowpower_ctrl2;

/* define the union reg_volowpower_ctrl3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int s14_rom_rm : 4;  /* [3..0]  */
        unsigned int s14_rom_rme : 1; /* [4]  */
        unsigned int reserved_0 : 27; /* [31..5]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_volowpower_ctrl3;

/* define the union reg_vomux_dac */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dac0_sel : 4;    /* [3..0]  */
        unsigned int dac1_sel : 4;    /* [7..4]  */
        unsigned int dac2_sel : 4;    /* [11..8]  */
        unsigned int dac3_sel : 4;    /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vomux_dac;

/* define the union reg_vomux_testsync */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int test_dv : 1;     /* [0]  */
        unsigned int test_hsync : 1;  /* [1]  */
        unsigned int test_vsync : 1;  /* [2]  */
        unsigned int test_field : 1;  /* [3]  */
        unsigned int reserved_0 : 27; /* [30..4]  */
        unsigned int vo_test_en : 1;  /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vomux_testsync;

/* define the union reg_vomux_testdata */
typedef union {
    /* define the struct bits */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART01_H_ */
