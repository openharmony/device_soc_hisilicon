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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART21_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART21_H_

        unsigned int dw_mode : 1;     /* [2]  */
        unsigned int sep_cmp_en : 1;  /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_glb_info;

/* Define the union u_wbc_cmp_framesize */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_width : 13;  /* [12..0]  */
        unsigned int reserved_0 : 3;    /* [15..13]  */
        unsigned int frame_height : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;    /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_framesize;

/* Define the union u_wbc_cmp_rc_cfg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mb_bits_y : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6;     /* [15..10]  */
        unsigned int min_mb_bits_y : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;     /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_rc_cfg0;

/* Define the union u_wbc_cmp_rc_cfg2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int max_qp_y : 4;       /* [3..0]  */
        unsigned int reserved_0 : 4;     /* [7..4]  */
        unsigned int sad_bits_ngain : 4; /* [11..8]  */
        unsigned int reserved_1 : 4;     /* [15..12]  */
        unsigned int rc_smth_ngain : 3;  /* [18..16]  */
        unsigned int reserved_2 : 5;     /* [23..19]  */
        unsigned int max_trow_bits : 8;  /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_rc_cfg2;

/* Define the union u_wbc_cmp_rc_cfg3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int max_sad_thr : 7; /* [6..0]  */
        unsigned int reserved_0 : 9;  /* [15..7]  */
        unsigned int min_sad_thr : 7; /* [22..16]  */
        unsigned int reserved_1 : 9;  /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_rc_cfg3;

/* Define the union u_wbc_cmp_rc_cfg4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int smth_thr : 7;      /* [6..0]  */
        unsigned int reserved_0 : 1;    /* [7]  */
        unsigned int still_thr : 7;     /* [14..8]  */
        unsigned int reserved_1 : 1;    /* [15]  */
        unsigned int big_grad_thr : 10; /* [25..16]  */
        unsigned int reserved_2 : 6;    /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_rc_cfg4;

/* Define the union u_wbc_cmp_rc_cfg5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int smth_pix_num_thr : 6;  /* [5..0]  */
        unsigned int reserved_0 : 2;        /* [7..6]  */
        unsigned int still_pix_num_thr : 6; /* [13..8]  */
        unsigned int reserved_1 : 2;        /* [15..14]  */
        unsigned int noise_pix_num_thr : 6; /* [21..16]  */
        unsigned int reserved_2 : 10;       /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_rc_cfg5;

/* Define the union u_wbc_cmp_rc_cfg6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int noise_sad : 7;    /* [6..0]  */
        unsigned int reserved_0 : 9;   /* [15..7]  */
        unsigned int pix_diff_thr : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;   /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_rc_cfg6;

/* Define the union u_wbc_cmp_rc_cfg7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int adj_sad_bits_thr : 7; /* [6..0]  */
        unsigned int reserved_0 : 25;      /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_rc_cfg7;

/* Define the union u_wbc_cmp_rc_cfg8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int qp_inc1_bits_thr_y : 8; /* [7..0]  */
        unsigned int qp_inc2_bits_thr_y : 8; /* [15..8]  */
        unsigned int qp_dec1_bits_thr_y : 8; /* [23..16]  */
        unsigned int qp_dec2_bits_thr_y : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_rc_cfg8;

/* Define the union u_wbc_cmp_rc_cfg10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int est_err_gain : 5;         /* [4..0]  */
        unsigned int reserved_0 : 11;          /* [15..5]  */
        unsigned int max_est_err_level : 9;    /* [24..16]  */
        unsigned int max_vbv_buf_loss_thr : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_rc_cfg10;

/* Define the union u_wbc_cmp_outsize0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_size0_reg : 22; /* [21..0]  */
        unsigned int reserved_0 : 10;      /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_outsize0;

/* Define the union u_wbc_cmp_max_row */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_size1_reg : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_max_row;

/* Define the union u_wbc_bmp_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int req_interval : 10; /* [9..0]  */
        unsigned int reserved_0 : 10;   /* [19..10]  */
        unsigned int ofl_master : 1;    /* [20]  */
        unsigned int data_width : 1;    /* [21]  */
        unsigned int reserved_1 : 2;    /* [23..22]  */
        unsigned int format_out : 4;    /* [27..24]  */
        unsigned int reserved_2 : 3;    /* [30..28]  */
        unsigned int wbc_en : 1;        /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_bmp_ctrl;

/* Define the union u_wbc_bmp_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_bmp_upd;

/* Define the union u_wbc_bmp_oreso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 12;        /* [11..0]  */
        unsigned int oh : 12;        /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_bmp_oreso;

/* Define the union u_wbc_bmp_sum */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int bmp_sum : 25;   /* [24..0]  */
        unsigned int reserved_0 : 7; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_bmp_sum;

/* Define the union u_wbc_dhd0_ctrl */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_dhd0_ctrl;

/* Define the union u_wbc_dhd0_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_dhd0_upd;

/* Define the union u_wbc_dhd0_oreso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0]  */
        unsigned int oh : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_dhd0_oreso;

/* Define the union u_wd_hpzme_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hfir_en : 1;     /* [0]  */
        unsigned int hfir_mode : 2;   /* [2..1]  */
        unsigned int mid_en : 1;      /* [3]  */
        unsigned int ck_gt_en : 1;    /* [4]  */
        unsigned int reserved_0 : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_hpzme_ctrl;

/* Define the union u_wd_hpzmecoef01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 10; /* [9..0]  */
        unsigned int reserved_1 : 6;  /* [15..10]  */
        unsigned int reserved_2 : 10; /* [25..16]  */
        unsigned int reserved_3 : 6;  /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_hpzmecoef01;

/* Define the union u_wd_hpzmecoef23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 10; /* [9..0]  */
        unsigned int reserved_1 : 6;  /* [15..10]  */
        unsigned int reserved_2 : 10; /* [25..16]  */
        unsigned int reserved_3 : 6;  /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_hpzmecoef23;

/* Define the union u_wd_hpzmecoef45 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 10; /* [9..0]  */
        unsigned int reserved_1 : 6;  /* [15..10]  */
        unsigned int reserved_2 : 10; /* [25..16]  */
        unsigned int reserved_3 : 6;  /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_hpzmecoef45;

/* Define the union u_wd_hpzmecoef67 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 10; /* [9..0]  */
        unsigned int reserved_1 : 6;  /* [15..10]  */
        unsigned int reserved_2 : 10; /* [25..16]  */
        unsigned int reserved_3 : 6;  /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_hpzmecoef67;

/* Define the union u_wd_hcds_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hfir_en : 1;     /* [0]  */
        unsigned int hfir_mode : 2;   /* [2..1]  */
        unsigned int mid_en : 1;      /* [3]  */
        unsigned int ck_gt_en : 1;    /* [4]  */
        unsigned int reserved_0 : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_hcds_ctrl;

/* Define the union u_wd_hcdscoef01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int coef0 : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef1 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_hcdscoef01;

/* Define the union u_wd_hcdscoef23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int coef2 : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef3 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_hcdscoef23;

/* Define the union u_wd_hcdscoef45 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 10; /* [9..0]  */
        unsigned int reserved_1 : 6;  /* [15..10]  */
        unsigned int reserved_2 : 10; /* [25..16]  */
        unsigned int reserved_3 : 6;  /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_hcdscoef45;

/* Define the union u_wd_hcdscoef67 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 10; /* [9..0]  */
        unsigned int reserved_1 : 6;  /* [15..10]  */
        unsigned int reserved_2 : 10; /* [25..16]  */
        unsigned int reserved_3 : 6;  /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_hcdscoef67;

/* Define the union u_dither_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_tap_mode : 2;     /* [1..0]  */
        unsigned int dither_domain_mode : 1;  /* [2]  */
        unsigned int dither_round : 1;        /* [3]  */
        unsigned int dither_mode : 1;         /* [4]  */
        unsigned int dither_en : 1;           /* [5]  */
        unsigned int dither_round_unlim : 1;  /* [6]  */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART21_H_ */
