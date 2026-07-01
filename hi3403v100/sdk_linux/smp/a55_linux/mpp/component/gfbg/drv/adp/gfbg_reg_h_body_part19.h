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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART19_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART19_H_

/* Define the union u_gp0_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbk_alpha : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gp0_alpha;

/* Define the union u_gp0_mute_bk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_cr : 10;   /* [9..0]  */
        unsigned int mute_cb : 10;   /* [19..10]  */
        unsigned int mute_y : 10;    /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gp0_mute_bk;

/* Define the union u_gp0_csc_idc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc0 : 11;   /* [10..0]  */
        unsigned int cscidc1 : 11;   /* [21..11]  */
        unsigned int csc_en : 1;     /* [22]  */
        unsigned int reserved_0 : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gp0_csc_idc;

/* Define the union u_gp0_csc_odc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscodc0 : 11;      /* [10..0]  */
        unsigned int cscodc1 : 11;      /* [21..11]  */
        unsigned int csc_sign_mode : 1; /* [22]  */
        unsigned int reserved_0 : 9;    /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gp0_csc_odc;

/* Define the union u_gp0_csc_iodc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gp0_csc_iodc;

/* Define the union u_gp0_csc_p0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp00 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp01 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gp0_csc_p0;

/* Define the union u_gp0_csc_p1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp02 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp10 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gp0_csc_p1;

/* Define the union u_gp0_csc_p2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp11 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp12 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gp0_csc_p2;

/* Define the union u_gp0_csc_p3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp20 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp21 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gp0_csc_p3;

/* Define the union u_gp0_csc_p4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp22 : 15;     /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gp0_csc_p4;

/* Define the union u_wbc_g0_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int req_interval : 10; /* [9..0]  */
        unsigned int auto_stop_en : 1;  /* [10]  */
        unsigned int reserved_0 : 15;   /* [25..11]  */
        unsigned int format_out : 2;    /* [27..26]  */
        unsigned int reserved_1 : 3;    /* [30..28]  */
        unsigned int wbc_en : 1;        /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_g0_ctrl;

/* Define the union u_wbc_g0_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_g0_upd;

/* Define the union u_wbc_g0_cmp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cmp_lossy_en : 1; /* [0]  */
        unsigned int reserved_0 : 3;   /* [3..1]  */
        unsigned int cmp_drr : 4;      /* [7..4]  */
        unsigned int reserved_1 : 23;  /* [30..8]  */
        unsigned int cmp_en : 1;       /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_g0_cmp;

/* Define the union u_wbc_g0_stride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbcstride : 16;  /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_g0_stride;

/* Define the union u_wbc_g0_oreso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 12;        /* [11..0]  */
        unsigned int oh : 12;        /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_g0_oreso;

/* Define the union u_wbc_g0_fcrop */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wfcrop : 12;    /* [11..0]  */
        unsigned int hfcrop : 12;    /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_g0_fcrop;

/* Define the union u_wbc_g0_lcrop */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wlcrop : 12;    /* [11..0]  */
        unsigned int hlcrop : 12;    /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_g0_lcrop;

/* Define the union u_wbc_gp0_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int req_interval : 10;  /* [9..0]  */
        unsigned int auto_stop_en : 1;   /* [10]  */
        unsigned int reserved_0 : 1;     /* [11]  */
        unsigned int wbc_vtthd_mode : 1; /* [12]  */
        unsigned int reserved_1 : 5;     /* [17..13]  */
        unsigned int three_d_mode : 2;   /* [19..18]  */
        unsigned int reserved_2 : 3;     /* [22..20]  */
        unsigned int flip_en : 1;        /* [23]  */
        unsigned int format_out : 4;     /* [27..24]  */
        unsigned int mode_out : 2;       /* [29..28]  */
        unsigned int reserved_3 : 1;     /* [30]  */
        unsigned int wbc_en : 1;         /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_gp0_ctrl;

/* Define the union u_wbc_gp0_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_gp0_upd;

/* Define the union u_wbc_gp0_stride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbclstride : 16; /* [15..0]  */
        unsigned int wbccstride : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_gp0_stride;

/* Define the union u_wbc_gp0_oreso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 12;        /* [11..0]  */
        unsigned int oh : 12;        /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_gp0_oreso;

/* Define the union u_wbc_gp0_fcrop */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wfcrop : 12;    /* [11..0]  */
        unsigned int hfcrop : 12;    /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_gp0_fcrop;

/* Define the union u_wbc_gp0_lcrop */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wlcrop : 12;    /* [11..0]  */
        unsigned int hlcrop : 12;    /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_gp0_lcrop;

/* Define the union u_wbc_gp0_dither_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 29;  /* [28..0]  */
        unsigned int dither_round : 1; /* [29]  */
        unsigned int dither_mode : 1;  /* [30]  */
        unsigned int dither_en : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_gp0_dither_ctrl;

/* Define the union u_wbc_gp0_dither_coef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_coef0 : 8; /* [7..0]  */
        unsigned int dither_coef1 : 8; /* [15..8]  */
        unsigned int dither_coef2 : 8; /* [23..16]  */
        unsigned int dither_coef3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_gp0_dither_coef0;

/* Define the union u_wbc_gp0_dither_coef1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_coef4 : 8; /* [7..0]  */
        unsigned int dither_coef5 : 8; /* [15..8]  */
        unsigned int dither_coef6 : 8; /* [23..16]  */
        unsigned int dither_coef7 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_gp0_dither_coef1;

/* Define the union u_wbc_gp0_hpzme */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 29;  /* [28..0]  */
        unsigned int hpzme_mode : 1;   /* [29]  */
        unsigned int hpzme_mid_en : 1; /* [30]  */
        unsigned int hpzme_en : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_gp0_hpzme;

/* Define the union u_wbc_me_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int req_interval : 10; /* [9..0]  */
        unsigned int reserved_0 : 10;   /* [19..10]  */
        unsigned int ofl_master : 1;    /* [20]  */
        unsigned int reserved_1 : 2;    /* [22..21]  */
        unsigned int mad_data_mode : 1; /* [23]  */
        unsigned int format_out : 4;    /* [27..24]  */
        unsigned int reserved_2 : 1;    /* [28]  */
        unsigned int c_wbc_en : 1;      /* [29]  */
        unsigned int reserved_3 : 1;    /* [30]  */
        unsigned int wbc_en : 1;        /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_ctrl;

/* Define the union u_wbc_me_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_upd;

/* Define the union u_wbc_me_wlen_sel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wlen_sel : 2;    /* [1..0]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_wlen_sel;

/* Define the union u_wbc_me_stride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbclstride : 16; /* [15..0]  */
        unsigned int wbccstride : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_stride;

/* Define the union u_wbc_me_oreso */
typedef union {
    /* Define the struct bits */
    struct {


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART19_H_ */
