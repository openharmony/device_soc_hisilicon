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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART01_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART01_H_


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/* Define the union u_voctrl */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_voctrl;

/* Define the union u_vointsta */
typedef union {
    /* Define the struct bits */
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
        unsigned int reserved_0 : 12;      /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vointsta;

/* Define the union u_vomskintsta */
typedef union {
    /* Define the struct bits */
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
        unsigned int reserved_0 : 12;      /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vomskintsta;

/* Define the union u_vointmsk */
typedef union {
    /* Define the struct bits */
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
        unsigned int reserved_0 : 12;          /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vointmsk;

/* Define the union u_vodebug */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_vodebug;

/* Define the union u_vointsta1 */
typedef union {
    /* Define the struct bits */
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
        unsigned int reserved_0 : 12;      /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vointsta1;

/* Define the union u_vomskintsta1 */
typedef union {
    /* Define the struct bits */
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
        unsigned int reserved_0 : 12;      /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vomskintsta1;

/* Define the union u_vointmsk1 */
typedef union {
    /* Define the struct bits */
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
        unsigned int reserved_0 : 12;          /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vointmsk1;

/* Define the union u_volowpower_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rfs_ema : 3;       /* [2..0]  */
        unsigned int rfs_emaw : 2;      /* [4..3]  */
        unsigned int ret1n : 1;         /* [5]  */
        unsigned int rft_emaa : 3;      /* [8..6]  */
        unsigned int rft_emab : 3;      /* [11..9]  */
        unsigned int rfs_colldisn : 1;  /* [12]  */
        unsigned int rft_emasa : 1;     /* [13]  */
        unsigned int rftuhd_ema : 3;    /* [16..14]  */
        unsigned int rftuhd_emaw : 2;   /* [18..17]  */
        unsigned int rftuhd_emas : 1;   /* [19]  */
        unsigned int rftuhd_emap : 1;   /* [20]  */
        unsigned int rftuhd_stov : 1;   /* [21]  */
        unsigned int rftuhd_stovab : 1; /* [22]  */
        unsigned int rfs_wabl : 1;      /* [23]  */
        unsigned int rfs_wablm : 2;     /* [25..24]  */
        unsigned int ras_ema : 3;       /* [28..26]  */
        unsigned int ras_emaw : 2;      /* [30..29]  */
        unsigned int ras_stov : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_volowpower_ctrl;

/* Define the union u_voufsta */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_voufsta;

/* Define the union u_voufclr */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_voufclr;

/* Define the union u_vointproc_tim */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vointproc_time : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;      /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vointproc_tim;

/* Define the union u_volowpower_ctrl1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rftf_rct : 2;      /* [1..0]  */
        unsigned int rftf_kp : 3;       /* [4..2]  */
        unsigned int rft_wtsel : 2;     /* [6..5]  */
        unsigned int rft_rtsel : 2;     /* [8..7]  */
        unsigned int rft_mtsel : 2;     /* [10..9]  */
        unsigned int rasshds_wtsel : 2; /* [12..11]  */
        unsigned int rasshds_rtsel : 2; /* [14..13]  */
        unsigned int rasshdm_wtsel : 2; /* [16..15]  */
        unsigned int rasshdm_rtsel : 2; /* [18..17]  */
        unsigned int rashds_wtsel : 2;  /* [20..19]  */
        unsigned int rashds_rtsel : 2;  /* [22..21]  */
        unsigned int rashdm_wtsel : 2;  /* [24..23]  */
        unsigned int rashdm_rtsel : 2;  /* [26..25]  */
        unsigned int ras_wtsel : 2;     /* [28..27]  */
        unsigned int ras_rtsel : 2;     /* [30..29]  */
        unsigned int ras_emas : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_volowpower_ctrl1;

/* Define the union u_vofpgadef */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_hdr_v_def : 1;  /* [0]  */
        unsigned int ot_hdr_g_def : 1;  /* [1]  */
        unsigned int ot_hdr_wd_def : 1; /* [2]  */
        unsigned int reserved_0 : 29;  /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vofpgadef;

/* Define the union u_volowpower_ctrl2 */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_volowpower_ctrl2;

/* Define the union u_volowpower_ctrl3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int s14_rom_rm : 4;  /* [3..0]  */
        unsigned int s14_rom_rme : 1; /* [4]  */
        unsigned int reserved_0 : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_volowpower_ctrl3;

/* Define the union u_vomux_dac */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dac0_sel : 4;    /* [3..0]  */
        unsigned int dac1_sel : 4;    /* [7..4]  */
        unsigned int dac2_sel : 4;    /* [11..8]  */
        unsigned int dac3_sel : 4;    /* [15..12]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vomux_dac;

/* Define the union u_vomux_testsync */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int test_dv : 1;     /* [0]  */
        unsigned int test_hsync : 1;  /* [1]  */
        unsigned int test_vsync : 1;  /* [2]  */
        unsigned int test_field : 1;  /* [3]  */
        unsigned int reserved_0 : 27; /* [30..4]  */
        unsigned int vo_test_en : 1;  /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vomux_testsync;

/* Define the union u_vomux_testdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int test_data : 30; /* [29..0]  */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART01_H_ */
