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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART23_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART23_H_


/* define the union reg_wbc_bmp_oreso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ow : 12;        /* [11..0]  */
        unsigned int oh : 12;        /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_bmp_oreso;

/* define the union reg_wbc_bmp_sum */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int bmp_sum : 25;   /* [24..0]  */
        unsigned int reserved_0 : 7; /* [31..25]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_bmp_sum;

/* define the union reg_wbc_dhd0_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int p2i_en : 1;         /* [0]  */
        unsigned int root_path : 2;      /* [2..1]  */
        unsigned int reserved_0 : 19;    /* [21..3]  */
        unsigned int mode_out : 2;       /* [23..22]  */
        unsigned int three_d_mode : 2;   /* [25..24]  */
        unsigned int auto_stop_en : 1;   /* [26]  */
        unsigned int wbc_vtthd_mode : 1; /* [27]  */
        unsigned int rupd_field : 1;     /* [28]  */
        unsigned int rgup_mode : 1;      /* [29]  */
        unsigned int nosec_flag : 1;     /* [30]  */
        unsigned int wbc_en : 1;         /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_dhd0_ctrl;

/* define the union reg_wbc_dhd0_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_dhd0_upd;

/* define the union reg_wbc_dhd0_oreso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0]  */
        unsigned int oh : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_dhd0_oreso;

/* define the union reg_wd_hpzme_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hfir_en : 1;     /* [0]  */
        unsigned int hfir_mode : 2;   /* [2..1]  */
        unsigned int mid_en : 1;      /* [3]  */
        unsigned int ck_gt_en : 1;    /* [4]  */
        unsigned int reserved_0 : 27; /* [31..5]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_hpzme_ctrl;

/* define the union reg_wd_hpzmecoef01 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 10; /* [9..0]  */
        unsigned int reserved_1 : 6;  /* [15..10]  */
        unsigned int reserved_2 : 10; /* [25..16]  */
        unsigned int reserved_3 : 6;  /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_hpzmecoef01;

/* define the union reg_wd_hpzmecoef23 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 10; /* [9..0]  */
        unsigned int reserved_1 : 6;  /* [15..10]  */
        unsigned int reserved_2 : 10; /* [25..16]  */
        unsigned int reserved_3 : 6;  /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_hpzmecoef23;

/* define the union reg_wd_hpzmecoef45 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 10; /* [9..0]  */
        unsigned int reserved_1 : 6;  /* [15..10]  */
        unsigned int reserved_2 : 10; /* [25..16]  */
        unsigned int reserved_3 : 6;  /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_hpzmecoef45;

/* define the union reg_wd_hpzmecoef67 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 10; /* [9..0]  */
        unsigned int reserved_1 : 6;  /* [15..10]  */
        unsigned int reserved_2 : 10; /* [25..16]  */
        unsigned int reserved_3 : 6;  /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_hpzmecoef67;

/* define the union reg_wd_hcds_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hfir_en : 1;     /* [0]  */
        unsigned int hfir_mode : 2;   /* [2..1]  */
        unsigned int mid_en : 1;      /* [3]  */
        unsigned int ck_gt_en : 1;    /* [4]  */
        unsigned int reserved_0 : 27; /* [31..5]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_hcds_ctrl;

/* define the union reg_wd_hcdscoef01 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int coef0 : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef1 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_hcdscoef01;

/* define the union reg_wd_hcdscoef23 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int coef2 : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef3 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_hcdscoef23;

/* define the union reg_wd_hcdscoef45 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 10; /* [9..0]  */
        unsigned int reserved_1 : 6;  /* [15..10]  */
        unsigned int reserved_2 : 10; /* [25..16]  */
        unsigned int reserved_3 : 6;  /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_hcdscoef45;

/* define the union reg_wd_hcdscoef67 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 10; /* [9..0]  */
        unsigned int reserved_1 : 6;  /* [15..10]  */
        unsigned int reserved_2 : 10; /* [25..16]  */
        unsigned int reserved_3 : 6;  /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wd_hcdscoef67;

/* define the union reg_dither_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_tap_mode : 2;     /* [1..0]  */
        unsigned int dither_domain_mode : 1;  /* [2]  */
        unsigned int dither_round : 1;        /* [3]  */
        unsigned int dither_mode : 1;         /* [4]  */
        unsigned int dither_en : 1;           /* [5]  */
        unsigned int dither_round_unlim : 1;  /* [6]  */
        unsigned int i_data_width_dither : 3; /* [9..7]  */
        unsigned int o_data_width_dither : 3; /* [12..10]  */
        unsigned int reserved_0 : 19;         /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_ctrl;

/* define the union reg_dither_sed_y0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_y0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_y0;

/* define the union reg_dither_sed_u0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_u0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_u0;

/* define the union reg_dither_sed_v0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_v0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_v0;

/* define the union reg_dither_sed_w0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_w0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_w0;

/* define the union reg_dither_sed_y1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_y1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_y1;

/* define the union reg_dither_sed_u1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_u1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_u1;

/* define the union reg_dither_sed_v1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_v1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_v1;

/* define the union reg_dither_sed_w1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_w1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_w1;

/* define the union reg_dither_sed_y2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_y2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_y2;

/* define the union reg_dither_sed_u2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_u2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_u2;

/* define the union reg_dither_sed_v2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_v2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_v2;

/* define the union reg_dither_sed_w2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_w2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_w2;

/* define the union reg_dither_sed_y3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_y3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_y3;

/* define the union reg_dither_sed_u3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_u3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_u3;

/* define the union reg_dither_sed_v3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_v3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_v3;

/* define the union reg_dither_sed_w3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_w3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dither_sed_w3;

/* define the union reg_dither_thr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_thr_min : 16; /* [15..0]  */
        unsigned int dither_thr_max : 16; /* [31..16]  */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART23_H_ */
