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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART22_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART22_H_

    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_upd;

/* define the union reg_wbc_fi_wlen_sel */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wlen_sel : 2;    /* [1..0]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_wlen_sel;

/* define the union reg_wbc_fi_stride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wbclstride : 16; /* [15..0]  */
        unsigned int wbccstride : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_stride;

/* define the union reg_wbc_fi_oreso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ow : 12;        /* [11..0]  */
        unsigned int oh : 12;        /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_oreso;

/* define the union reg_wbc_fi_smmu_bypass */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int l_bypass : 1;    /* [0]  */
        unsigned int c_bypass : 1;    /* [1]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_smmu_bypass;

/* define the union reg_wbc_fi_frame_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int frame_size : 23; /* [22..0]  */
        unsigned int reserved_0 : 9;  /* [31..23]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_frame_size;

/* define the union reg_wbc_fi_hcds */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 29; /* [28..0]  */
        unsigned int hchfir_en : 1;   /* [29]  */
        unsigned int hchmid_en : 1;   /* [30]  */
        unsigned int hcds_en : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_hcds;

/* define the union reg_wbc_fi_hcds_coef0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int coef0 : 10;     /* [9..0]  */
        unsigned int coef1 : 10;     /* [19..10]  */
        unsigned int coef2 : 10;     /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_hcds_coef0;

/* define the union reg_wbc_fi_hcds_coef1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int coef3 : 10;      /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_hcds_coef1;

/* define the union reg_wbc_fi_cmp_mb */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mb_bits : 10;    /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_cmp_mb;

/* define the union reg_wbc_fi_cmp_max_min */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int min_bits_cnt : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;    /* [15..10]  */
        unsigned int max_bits_cnt : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;    /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_cmp_max_min;

/* define the union reg_wbc_fi_cmp_adj_thr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int adj_sad_thr : 12;     /* [11..0]  */
        unsigned int reserved_0 : 4;       /* [15..12]  */
        unsigned int adj_sad_bit_thr : 8;  /* [23..16]  */
        unsigned int adj_spec_bit_thr : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_cmp_adj_thr;

/* define the union reg_wbc_fi_cmp_big_grad */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int big_grad_thr : 7;     /* [6..0]  */
        unsigned int reserved_0 : 1;       /* [7]  */
        unsigned int big_grad_num_thr : 5; /* [12..8]  */
        unsigned int reserved_1 : 19;      /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_cmp_big_grad;

/* define the union reg_wbc_fi_cmp_blk */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int smth_thr : 6;     /* [5..0]  */
        unsigned int reserved_0 : 2;   /* [7..6]  */
        unsigned int blk_comp_thr : 3; /* [10..8]  */
        unsigned int reserved_1 : 21;  /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_cmp_blk;

/* define the union reg_wbc_fi_cmp_graphic_judge */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int graphic_en : 1;     /* [0]  */
        unsigned int reserved_0 : 15;    /* [15..1]  */
        unsigned int video_sad_thr : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_cmp_graphic_judge;

/* define the union reg_wbc_fi_cmp_rc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int sadbits_ngain : 3; /* [2..0]  */
        unsigned int reserved_0 : 5;    /* [7..3]  */
        unsigned int rc_smth_gain : 3;  /* [10..8]  */
        unsigned int reserved_1 : 5;    /* [15..11]  */
        unsigned int max_trow_bits : 6; /* [21..16]  */
        unsigned int reserved_2 : 10;   /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_cmp_rc;

/* define the union reg_wbc_fi_cmp_frame_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int frame_size : 21; /* [20..0]  */
        unsigned int reserved_0 : 11; /* [31..21]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_cmp_frame_size;

/* define the union reg_wbc_cmp_glb_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int is_lossless : 1; /* [0]  */
        unsigned int cmp_mode : 1;    /* [1]  */
        unsigned int dw_mode : 1;     /* [2]  */
        unsigned int sep_cmp_en : 1;  /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_glb_info;

/* define the union reg_wbc_cmp_framesize */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int frame_width : 13;  /* [12..0]  */
        unsigned int reserved_0 : 3;    /* [15..13]  */
        unsigned int frame_height : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;    /* [31..29]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_framesize;

/* define the union reg_wbc_cmp_rc_cfg0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mb_bits_y : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6;     /* [15..10]  */
        unsigned int min_mb_bits_y : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;     /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_rc_cfg0;

/* define the union reg_wbc_cmp_rc_cfg2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int max_qp_y : 4;       /* [3..0]  */
        unsigned int reserved_0 : 4;     /* [7..4]  */
        unsigned int sad_bits_ngain : 4; /* [11..8]  */
        unsigned int reserved_1 : 4;     /* [15..12]  */
        unsigned int rc_smth_ngain : 3;  /* [18..16]  */
        unsigned int reserved_2 : 5;     /* [23..19]  */
        unsigned int max_trow_bits : 8;  /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_rc_cfg2;

/* define the union reg_wbc_cmp_rc_cfg3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int max_sad_thr : 7; /* [6..0]  */
        unsigned int reserved_0 : 9;  /* [15..7]  */
        unsigned int min_sad_thr : 7; /* [22..16]  */
        unsigned int reserved_1 : 9;  /* [31..23]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_rc_cfg3;

/* define the union reg_wbc_cmp_rc_cfg4 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int smth_thr : 7;      /* [6..0]  */
        unsigned int reserved_0 : 1;    /* [7]  */
        unsigned int still_thr : 7;     /* [14..8]  */
        unsigned int reserved_1 : 1;    /* [15]  */
        unsigned int big_grad_thr : 10; /* [25..16]  */
        unsigned int reserved_2 : 6;    /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_rc_cfg4;

/* define the union reg_wbc_cmp_rc_cfg5 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int smth_pix_num_thr : 6;  /* [5..0]  */
        unsigned int reserved_0 : 2;        /* [7..6]  */
        unsigned int still_pix_num_thr : 6; /* [13..8]  */
        unsigned int reserved_1 : 2;        /* [15..14]  */
        unsigned int noise_pix_num_thr : 6; /* [21..16]  */
        unsigned int reserved_2 : 10;       /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_rc_cfg5;

/* define the union reg_wbc_cmp_rc_cfg6 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int noise_sad : 7;    /* [6..0]  */
        unsigned int reserved_0 : 9;   /* [15..7]  */
        unsigned int pix_diff_thr : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;   /* [31..25]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_rc_cfg6;

/* define the union reg_wbc_cmp_rc_cfg7 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int adj_sad_bits_thr : 7; /* [6..0]  */
        unsigned int reserved_0 : 25;      /* [31..7]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_rc_cfg7;

/* define the union reg_wbc_cmp_rc_cfg8 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int qp_inc1_bits_thr_y : 8; /* [7..0]  */
        unsigned int qp_inc2_bits_thr_y : 8; /* [15..8]  */
        unsigned int qp_dec1_bits_thr_y : 8; /* [23..16]  */
        unsigned int qp_dec2_bits_thr_y : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_rc_cfg8;

/* define the union reg_wbc_cmp_rc_cfg10 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int est_err_gain : 5;         /* [4..0]  */
        unsigned int reserved_0 : 11;          /* [15..5]  */
        unsigned int max_est_err_level : 9;    /* [24..16]  */
        unsigned int max_vbv_buf_loss_thr : 7; /* [31..25]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_rc_cfg10;

/* define the union reg_wbc_cmp_outsize0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int frame_size0_reg : 22; /* [21..0]  */
        unsigned int reserved_0 : 10;      /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_outsize0;

/* define the union reg_wbc_cmp_max_row */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int frame_size1_reg : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_max_row;

/* define the union reg_wbc_bmp_ctrl */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_bmp_ctrl;

/* define the union reg_wbc_bmp_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_bmp_upd;


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART22_H_ */
