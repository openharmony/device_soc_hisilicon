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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART39_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART39_H_

    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_rc_cfg16_c;

/* define the union reg_vdp_v3r2_lineseg_cmp_glb_st_c */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int max_left_bits_buffer : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;           /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_glb_st_c;

/* define the union reg_wbc_cmp_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int req_interval : 10; /* [9..0]  */
        unsigned int reserved_0 : 17;   /* [26..10]  */
        unsigned int mem_mode : 1;      /* [27]  */
        unsigned int data_width : 1;    /* [28]  */
        unsigned int reserved_1 : 1;    /* [29]  */
        unsigned int l_cmp_en : 1;      /* [30]  */
        unsigned int wbc_en : 1;        /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_ctrl;

/* define the union reg_wbc_cmp_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_upd;

/* define the union reg_wbc_cmp_height */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int c_max_height : 13; /* [12..0]  */
        unsigned int l_max_height : 13; /* [25..13]  */
        unsigned int addr_mode : 1;     /* [26]  */
        unsigned int fsize_mode : 1;    /* [27]  */
        unsigned int rgb_cmp_mode : 2;  /* [29..28]  */
        unsigned int pause_mode : 1;    /* [30]  */
        unsigned int buffer_mode : 1;   /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_height;

/* define the union reg_wbc_cmp_oreso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ow : 12;        /* [11..0]  */
        unsigned int oh : 12;        /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cmp_oreso;

/* define the union reg_wbc_od_state */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int addr_err : 1;      /* [0]  */
        unsigned int he_addr_err0 : 1;  /* [1]  */
        unsigned int he_addr_err1 : 1;  /* [2]  */
        unsigned int he_addr_err2 : 1;  /* [3]  */
        unsigned int w_addr_err : 1;    /* [4]  */
        unsigned int he_fsize_err0 : 1; /* [5]  */
        unsigned int he_fsize_err1 : 1; /* [6]  */
        unsigned int he_fsize_err2 : 1; /* [7]  */
        unsigned int w_fsize_err : 1;   /* [8]  */
        unsigned int he_fsize_war0 : 1; /* [9]  */
        unsigned int he_fsize_war1 : 1; /* [10]  */
        unsigned int he_fsize_war2 : 1; /* [11]  */
        unsigned int w_fsize_war : 1;   /* [12]  */
        unsigned int reserved_0 : 19;   /* [31..13]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_od_state;

/* define the union reg_od_pic_osd_glb_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int is_lossless : 1;   /* [0]  */
        unsigned int is_lossless_a : 1; /* [1]  */
        unsigned int cmp_mode : 1;      /* [2]  */
        unsigned int source_mode : 3;   /* [5..3]  */
        unsigned int part_cmp_en : 1;   /* [6]  */
        unsigned int top_pred_en : 1;   /* [7]  */
        unsigned int graphic_en : 1;    /* [8]  */
        unsigned int reserved_0 : 23;   /* [31..9]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_glb_info;

/* define the union reg_od_pic_osd_frame_size */
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
} reg_od_pic_osd_frame_size;

/* define the union reg_od_pic_osd_rc_cfg0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mb_bits : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6;   /* [15..10]  */
        unsigned int min_mb_bits : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;   /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg0;

/* define the union reg_od_pic_osd_rc_cfg1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int max_qp : 4;        /* [3..0]  */
        unsigned int reserved_0 : 4;    /* [7..4]  */
        unsigned int sad_bits_gain : 4; /* [11..8]  */
        unsigned int reserved_1 : 4;    /* [15..12]  */
        unsigned int rc_smth_ngain : 3; /* [18..16]  */
        unsigned int reserved_2 : 5;    /* [23..19]  */
        unsigned int max_trow_bits : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg1;

/* define the union reg_od_pic_osd_rc_cfg2 */
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
} reg_od_pic_osd_rc_cfg2;

/* define the union reg_od_pic_osd_rc_cfg3 */
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
} reg_od_pic_osd_rc_cfg3;

/* define the union reg_od_pic_osd_rc_cfg4 */
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
} reg_od_pic_osd_rc_cfg4;

/* define the union reg_od_pic_osd_rc_cfg5 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int noise_sad : 7;     /* [6..0]  */
        unsigned int reserved_0 : 9;    /* [15..7]  */
        unsigned int pix_diff_thr : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;    /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg5;

/* define the union reg_od_pic_osd_rc_cfg6 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int adj_sad_bits_thr : 7; /* [6..0]  */
        unsigned int reserved_0 : 25;      /* [31..7]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg6;

/* define the union reg_od_pic_osd_rc_cfg7 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int qp_inc1_bits_thr : 8; /* [7..0]  */
        unsigned int qp_inc2_bits_thr : 8; /* [15..8]  */
        unsigned int qp_dec1_bits_thr : 8; /* [23..16]  */
        unsigned int qp_dec2_bits_thr : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg7;

/* define the union reg_od_pic_osd_rc_cfg8 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int est_err_gain : 5;      /* [4..0]  */
        unsigned int reserved_0 : 11;       /* [15..5]  */
        unsigned int max_est_err_level : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;        /* [31..25]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg8;

/* define the union reg_od_pic_osd_rc_cfg9 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 16;       /* [15..0]  */
        unsigned int vbv_buf_loss1_thr : 7; /* [22..16]  */
        unsigned int reserved_1 : 1;        /* [23]  */
        unsigned int vbv_buf_loss2_thr : 7; /* [30..24]  */
        unsigned int reserved_2 : 1;        /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg9;

/* define the union reg_od_pic_osd_rc_cfg10 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int qp_thr0 : 3;     /* [2..0]  */
        unsigned int reserved_0 : 5;  /* [7..3]  */
        unsigned int qp_thr1 : 3;     /* [10..8]  */
        unsigned int reserved_1 : 5;  /* [15..11]  */
        unsigned int qp_thr2 : 3;     /* [18..16]  */
        unsigned int reserved_2 : 13; /* [31..19]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg10;

/* define the union reg_od_pic_osd_rc_cfg11 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int grph_bias_bit_thr0 : 8;  /* [7..0]  */
        unsigned int grph_bias_bit_thr1 : 8;  /* [15..8]  */
        unsigned int grph_ideal_bit_thr : 10; /* [25..16]  */
        unsigned int reserved_0 : 6;          /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg11;

/* define the union reg_od_pic_osd_rc_cfg12 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int force_rc_en : 1;           /* [0]  */
        unsigned int reserved_0 : 7;            /* [7..1]  */
        unsigned int forcerc_bits_diff_thr : 8; /* [15..8]  */
        unsigned int reserved_1 : 16;           /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg12;

/* define the union reg_od_pic_osd_rc_cfg13 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int maxdiff_ctrl_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;     /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg13;

/* define the union reg_od_pic_osd_rc_cfg14 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mb_bits_cap : 10;       /* [9..0]  */
        unsigned int reserved_0 : 6;         /* [15..10]  */
        unsigned int init_buf_bits_cap : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg14;

/* define the union reg_od_pic_osd_rc_cfg15 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lfw_mb_len : 7;    /* [6..0]  */
        unsigned int reserved_0 : 1;    /* [7]  */
        unsigned int cmplx_sad_thr : 4; /* [11..8]  */
        unsigned int reserved_1 : 4;    /* [15..12]  */
        unsigned int err_thr0 : 4;      /* [19..16]  */
        unsigned int reserved_2 : 4;    /* [23..20]  */
        unsigned int err_thr1 : 4;      /* [27..24]  */
        unsigned int reserved_3 : 4;    /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg15;

/* define the union reg_od_pic_osd_rc_cfg16 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int sim_num_thr : 3;   /* [2..0]  */
        unsigned int reserved_0 : 5;    /* [7..3]  */
        unsigned int sum_y_err_thr : 7; /* [14..8]  */
        unsigned int reserved_1 : 1;    /* [15]  */
        unsigned int sum_c_err_thr : 7; /* [22..16]  */
        unsigned int reserved_2 : 9;    /* [31..23]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg16;

/* define the union reg_od_pic_osd_rc_cfg17 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cpmlx_sad_thr_y : 4;   /* [3..0]  */
        unsigned int reserved_0 : 4;        /* [7..4]  */
        unsigned int smpl_sad_thr_c : 4;    /* [11..8]  */
        unsigned int reserved_1 : 4;        /* [15..12]  */
        unsigned int smpl_sumsad_thr_y : 8; /* [23..16]  */
        unsigned int smpl_sumsad_thr_c : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg17;

/* define the union reg_od_pic_osd_rc_cfg18 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int future_sad_y_thr0 : 4; /* [3..0]  */
        unsigned int reserved_0 : 4;        /* [7..4]  */
        unsigned int future_sad_c_thr0 : 4; /* [11..8]  */
        unsigned int reserved_1 : 4;        /* [15..12]  */
        unsigned int future_sad_y_thr1 : 4; /* [19..16]  */
        unsigned int reserved_2 : 4;        /* [23..20]  */
        unsigned int future_sad_c_thr1 : 4; /* [27..24]  */
        unsigned int reserved_3 : 4;        /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg18;

/* define the union reg_od_pic_osd_rc_cfg19 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cmplx_sumsad_thr_y : 8; /* [7..0]  */
        unsigned int cmplx_sumsad_thr_c : 8; /* [15..8]  */
        unsigned int reserved_0 : 16;        /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_rc_cfg19;

/* define the union reg_od_pic_osd_stat_thr */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART39_H_ */
