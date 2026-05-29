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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART35_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART35_H_


    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cap_reso;

/* Define the union u_vdp_v3r2_lineseg_cmp_glb_info */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ice_en : 1;      /* [0]  */
        unsigned int cmp_mode : 1;    /* [1]  */
        unsigned int is_lossless : 1; /* [2]  */
        unsigned int chroma_en : 1;   /* [3]  */
        unsigned int esl_qp : 3;      /* [6..4]  */
        unsigned int bit_depth : 1;   /* [7]  */
        unsigned int mirror_en : 1;   /* [8]  */
        unsigned int seg_en : 1;      /* [9]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_glb_info;

/* Define the union u_vdp_v3r2_lineseg_cmp_frame_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_width : 14;  /* [13..0]  */
        unsigned int reserved_0 : 2;    /* [15..14]  */
        unsigned int frame_height : 14; /* [29..16]  */
        unsigned int reserved_1 : 2;    /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_frame_size;

/* Define the union u_vdp_v3r2_lineseg_cmp_rc_cfg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int big_grad_thr : 8;      /* [7..0]  */
        unsigned int diff_thr : 8;          /* [15..8]  */
        unsigned int noise_pix_num_thr : 6; /* [21..16]  */
        unsigned int reserved_0 : 10;       /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_rc_cfg0;

/* Define the union u_vdp_v3r2_lineseg_cmp_rc_cfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int qp_inc1_bits_thr : 8; /* [7..0]  */
        unsigned int qp_inc2_bits_thr : 8; /* [15..8]  */
        unsigned int qp_dec1_bits_thr : 8; /* [23..16]  */
        unsigned int qp_dec2_bits_thr : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_rc_cfg1;

/* Define the union u_vdp_v3r2_lineseg_cmp_rc_cfg12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int buffer_init_bits : 16; /* [15..0]  */
        unsigned int buffer_size : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_rc_cfg12;

/* Define the union u_vdp_v3r2_lineseg_cmp_rc_cfg13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int budget_mb_bits : 10;      /* [9..0]  */
        unsigned int budget_mb_bits_last : 10; /* [19..10]  */
        unsigned int min_mb_bits : 10;         /* [29..20]  */
        unsigned int reserved_0 : 2;           /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_rc_cfg13;

/* Define the union u_vdp_v3r2_lineseg_cmp_rc_cfg16 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int smooth_status_thr : 4;    /* [3..0]  */
        unsigned int smooth_deltabits_thr : 8; /* [11..4]  */
        unsigned int max_mb_qp : 3;            /* [14..12]  */
        unsigned int reserved_0 : 17;          /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_rc_cfg16;

/* Define the union u_vdp_v3r2_lineseg_cmp_glb_st */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int max_left_bits_buffer : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;           /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_glb_st;

/* Define the union u_vdp_v3r2_lineseg_cmp_glb_info_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ice_en : 1;      /* [0]  */
        unsigned int cmp_mode : 1;    /* [1]  */
        unsigned int is_lossless : 1; /* [2]  */
        unsigned int chroma_en : 1;   /* [3]  */
        unsigned int esl_qp : 3;      /* [6..4]  */
        unsigned int bit_depth : 1;   /* [7]  */
        unsigned int mirror_en : 1;   /* [8]  */
        unsigned int seg_en : 1;      /* [9]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_glb_info_c;

/* Define the union u_vdp_v3r2_lineseg_cmp_frame_size_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_width : 14;  /* [13..0]  */
        unsigned int reserved_0 : 2;    /* [15..14]  */
        unsigned int frame_height : 14; /* [29..16]  */
        unsigned int reserved_1 : 2;    /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_frame_size_c;

/* Define the union u_vdp_v3r2_lineseg_cmp_rc_cfg0_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int big_grad_thr : 8;      /* [7..0]  */
        unsigned int diff_thr : 8;          /* [15..8]  */
        unsigned int noise_pix_num_thr : 6; /* [21..16]  */
        unsigned int reserved_0 : 10;       /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_rc_cfg0_c;

/* Define the union u_vdp_v3r2_lineseg_cmp_rc_cfg1_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int qp_inc1_bits_thr : 8; /* [7..0]  */
        unsigned int qp_inc2_bits_thr : 8; /* [15..8]  */
        unsigned int qp_dec1_bits_thr : 8; /* [23..16]  */
        unsigned int qp_dec2_bits_thr : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_rc_cfg1_c;

/* Define the union u_vdp_v3r2_lineseg_cmp_rc_cfg12_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int buffer_init_bits : 16; /* [15..0]  */
        unsigned int buffer_size : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_rc_cfg12_c;

/* Define the union u_vdp_v3r2_lineseg_cmp_rc_cfg13_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int budget_mb_bits : 10;      /* [9..0]  */
        unsigned int budget_mb_bits_last : 10; /* [19..10]  */
        unsigned int min_mb_bits : 10;         /* [29..20]  */
        unsigned int reserved_0 : 2;           /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_rc_cfg13_c;

/* Define the union u_vdp_v3r2_lineseg_cmp_rc_cfg16_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int smooth_status_thr : 4;    /* [3..0]  */
        unsigned int smooth_deltabits_thr : 8; /* [11..4]  */
        unsigned int max_mb_qp : 3;            /* [14..12]  */
        unsigned int reserved_0 : 17;          /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_rc_cfg16_c;

/* Define the union u_vdp_v3r2_lineseg_cmp_glb_st_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int max_left_bits_buffer : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;           /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_cmp_glb_st_c;

/* Define the union u_wbc_cmp_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int req_interval : 10; /* [9..0]  */
        unsigned int reserved_0 : 17;   /* [26..10]  */
        unsigned int mem_mode : 1;      /* [27]  */
        unsigned int data_width : 1;    /* [28]  */
        unsigned int reserved_1 : 1;    /* [29]  */
        unsigned int l_cmp_en : 1;      /* [30]  */
        unsigned int wbc_en : 1;        /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_ctrl;

/* Define the union u_wbc_cmp_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_upd;

/* Define the union u_wbc_cmp_height */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int c_max_height : 13; /* [12..0]  */
        unsigned int l_max_height : 13; /* [25..13]  */
        unsigned int addr_mode : 1;     /* [26]  */
        unsigned int fsize_mode : 1;    /* [27]  */
        unsigned int rgb_cmp_mode : 2;  /* [29..28]  */
        unsigned int pause_mode : 1;    /* [30]  */
        unsigned int buffer_mode : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_height;

/* Define the union u_wbc_cmp_oreso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 12;        /* [11..0]  */
        unsigned int oh : 12;        /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cmp_oreso;

/* Define the union u_wbc_od_state */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_od_state;

/* Define the union u_od_pic_osd_glb_info */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_glb_info;

/* Define the union u_od_pic_osd_frame_size */
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
} u_od_pic_osd_frame_size;

/* Define the union u_od_pic_osd_rc_cfg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mb_bits : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6;   /* [15..10]  */
        unsigned int min_mb_bits : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;   /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg0;

/* Define the union u_od_pic_osd_rc_cfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int max_qp : 4;        /* [3..0]  */
        unsigned int reserved_0 : 4;    /* [7..4]  */
        unsigned int sad_bits_gain : 4; /* [11..8]  */
        unsigned int reserved_1 : 4;    /* [15..12]  */
        unsigned int rc_smth_ngain : 3; /* [18..16]  */
        unsigned int reserved_2 : 5;    /* [23..19]  */
        unsigned int max_trow_bits : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg1;

/* Define the union u_od_pic_osd_rc_cfg2 */
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
} u_od_pic_osd_rc_cfg2;

/* Define the union u_od_pic_osd_rc_cfg3 */
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
} u_od_pic_osd_rc_cfg3;

/* Define the union u_od_pic_osd_rc_cfg4 */
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
} u_od_pic_osd_rc_cfg4;

/* Define the union u_od_pic_osd_rc_cfg5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int noise_sad : 7;     /* [6..0]  */
        unsigned int reserved_0 : 9;    /* [15..7]  */
        unsigned int pix_diff_thr : 10; /* [25..16]  */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART35_H_ */
