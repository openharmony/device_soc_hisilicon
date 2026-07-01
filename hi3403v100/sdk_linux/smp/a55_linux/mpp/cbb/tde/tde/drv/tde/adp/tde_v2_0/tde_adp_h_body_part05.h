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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART05_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART05_H_

#ifdef CONFIG_TDE_GFBG_COMPRESS_V1
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int is_lossless : 1;       /* [0]  */
        unsigned int is_lossless_alpha : 1; /* [1]  */
        unsigned int cmp_mode : 1;          /* [2]  */
        unsigned int osd_mode : 2;          /* [4..3]  */
        unsigned int partition_en : 1;      /* [5]  */
        unsigned int part_num : 3;          /* [8..6]  */
        unsigned int reserved_0 : 23;       /* [31..9]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_glb_info;

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
} u_tde_od_pic_osd_frame_size;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int budget_bits_mb : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;      /* [15..10]  */
        unsigned int min_mb_bits : 10;    /* [25..16]  */
        unsigned int reserved_1 : 6;      /* [31..26]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_rc_cfg0;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int budget_bits_mb_cap : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;         /* [31..10]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_rc_cfg1;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int max_qp : 4;            /* [3..0]  */
        unsigned int smth_qp : 4;           /* [7..4]  */
        unsigned int sad_bits_ngain : 4;    /* [11..8]  */
        unsigned int reserved_0 : 4;        /* [15..12]  */
        unsigned int rc_smth_ngain : 3;     /* [18..16]  */
        unsigned int reserved_1 : 5;        /* [23..19]  */
        unsigned int special_bits_gain : 4; /* [27..24]  */
        unsigned int reserved_2 : 4;        /* [31..28]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_rc_cfg2;

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
} u_tde_od_pic_osd_rc_cfg3;

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
} u_tde_od_pic_osd_rc_cfg4;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int smth_pix_num_thr : 6;       /* [5..0]  */
        unsigned int reserved_0 : 2;             /* [7..6]  */
        unsigned int still_pix_num_thr : 6;      /* [13..8]  */
        unsigned int reserved_1 : 2;             /* [15..14]  */
        unsigned int noise_pix_num_thr : 6;      /* [21..16]  */
        unsigned int reserved_2 : 2;             /* [23..22]  */
        unsigned int large_smth_pix_num_thr : 6; /* [29..24]  */
        unsigned int reserved_3 : 2;             /* [31..30]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_rc_cfg5;

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
} u_tde_od_pic_osd_rc_cfg6;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int adj_sad_bits_thr : 7; /* [6..0]  */
        unsigned int reserved_0 : 1;       /* [7]  */
        unsigned int max_trow_bits : 8;    /* [15..8]  */
        unsigned int reserved_1 : 16;      /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_rc_cfg7;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int qp_inc1_bits_thr : 8; /* [7..0]  */
        unsigned int qp_dec1_bits_thr : 8; /* [15..8]  */
        unsigned int qp_dec2_bits_thr : 8; /* [23..16]  */
        unsigned int qp_dec3_bits_thr : 8; /* [31..24]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_rc_cfg8;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int force_qp_thr : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6;        /* [15..10]  */
        unsigned int force_qp_thr_cap : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;        /* [31..26]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_rc_cfg9;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int max_row_len : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;  /* [31..10]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_rc_cfg10;
#endif /* CONFIG_TDE_GFBG_COMPRESS_V1 */

#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ice_en : 1; /* [0]  */
        unsigned int cmp_mode : 1; /* [1]  */
        unsigned int is_lossless : 1; /* [2]  */
        unsigned int conv_en : 1; /* [3]  */
        unsigned int osd_mode : 2; /* [5..4]  */
        unsigned int reserved : 26;       /* [31..6]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_glb_info;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_width : 14;  /* [13..0]  */
        unsigned int reserved_0 : 2;    /* [15..14]  */
        unsigned int frame_height : 14; /* [29..16]  */
        unsigned int reserved_1 : 2;    /* [31..30]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_frame_size;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int budget_mb_bits : 12; /* [11..0]  */
        unsigned int budget_mb_bits_last : 12; /* [23..12]  */
        unsigned int max_mb_qp : 3; /* [26..24]  */
        unsigned int reserved : 5; /* [31..27]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg0;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int smth_thr : 8; /* [7..0]  */
        unsigned int still_thr : 8; /* [15..8]  */
        unsigned int big_grad_thr : 8; /* [23..16]  */
        unsigned int diff_thr : 8; /* [31..24]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg1;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int smth_pix_num_thr : 6; /* [5..0]  */
        unsigned int reserved_0 : 2; /* [7..6]  */
        unsigned int still_pix_num_thr : 6; /* [13..8]  */
        unsigned int reserved_1 : 2; /* [15..14]  */
        unsigned int noise_pix_num_thr : 6; /* [21..16]  */
        unsigned int reserved_2 : 2; /* [23..22]  */
        unsigned int raw_bits_penalty : 8; /* [31..24]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg2;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int qp_inc1_bits_thr : 10; /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int qp_inc2_bits_thr : 10; /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg3;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int qp_dec1_bits_thr : 10; /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int qp_dec2_bits_thr : 10; /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg4;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int buf_fullness_thr_reg0 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg5;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int buf_fullness_thr_reg1 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg6;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int buf_fullness_thr_reg2 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg7;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int qp_rge_reg0 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg8;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int qp_rge_reg1 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg9;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int qp_rge_reg2 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg10;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int bits_offset_reg0 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg11;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int bits_offset_reg1 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg12;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int bits_offset_reg2 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg13;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int est_err_gain_map : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg14;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int buffer_init_bits : 16; /* [15..0]  */
        unsigned int smooth_status_thr : 4; /* [19..16]  */
        unsigned int min_mb_bits : 12; /* [31..20]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg15;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int first_col_adj_bits : 8; /* [7..0]  */
        unsigned int first_row_adj_bits : 8; /* [15..8]  */
        unsigned int first_mb_adj_bits : 8; /* [23..16]  */
        unsigned int reserved : 8; /* [31..24]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_v4r2_line_osd_cmp_rc_cfg16;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int still_diff_thr : 8; /* [7..0]  */
        unsigned int still_status_thr : 4; /* [11..8]  */
        unsigned int reserved : 20; /* [31..12]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_rc_cfg17;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int max_left_bits_buffer : 16; /* [15..0]  */
        unsigned int reserved : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_glb_st;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int debug_info : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int all;
} u_tde_line_osd_cmp_dbg_reg;
#endif /* CONFIG_TDE_GFBG_COMPRESS_V2 */

/* Define the union U_TDE_DEBUG0 */
typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 debug0 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_debug0;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 debug1 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_debug1;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 debug2 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_debug2;

typedef union {


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART05_H_ */
