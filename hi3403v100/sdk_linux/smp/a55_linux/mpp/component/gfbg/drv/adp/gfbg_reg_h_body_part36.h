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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART36_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART36_H_

        unsigned int reserved_1 : 6;    /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg5;

/* Define the union u_od_pic_osd_rc_cfg6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int adj_sad_bits_thr : 7; /* [6..0]  */
        unsigned int reserved_0 : 25;      /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg6;

/* Define the union u_od_pic_osd_rc_cfg7 */
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
} u_od_pic_osd_rc_cfg7;

/* Define the union u_od_pic_osd_rc_cfg8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int est_err_gain : 5;      /* [4..0]  */
        unsigned int reserved_0 : 11;       /* [15..5]  */
        unsigned int max_est_err_level : 9; /* [24..16]  */
        unsigned int reserved_1 : 7;        /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg8;

/* Define the union u_od_pic_osd_rc_cfg9 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 16;       /* [15..0]  */
        unsigned int vbv_buf_loss1_thr : 7; /* [22..16]  */
        unsigned int reserved_1 : 1;        /* [23]  */
        unsigned int vbv_buf_loss2_thr : 7; /* [30..24]  */
        unsigned int reserved_2 : 1;        /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg9;

/* Define the union u_od_pic_osd_rc_cfg10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int qp_thr0 : 3;     /* [2..0]  */
        unsigned int reserved_0 : 5;  /* [7..3]  */
        unsigned int qp_thr1 : 3;     /* [10..8]  */
        unsigned int reserved_1 : 5;  /* [15..11]  */
        unsigned int qp_thr2 : 3;     /* [18..16]  */
        unsigned int reserved_2 : 13; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg10;

/* Define the union u_od_pic_osd_rc_cfg11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int grph_bias_bit_thr0 : 8;  /* [7..0]  */
        unsigned int grph_bias_bit_thr1 : 8;  /* [15..8]  */
        unsigned int grph_ideal_bit_thr : 10; /* [25..16]  */
        unsigned int reserved_0 : 6;          /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg11;

/* Define the union u_od_pic_osd_rc_cfg12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int force_rc_en : 1;           /* [0]  */
        unsigned int reserved_0 : 7;            /* [7..1]  */
        unsigned int forcerc_bits_diff_thr : 8; /* [15..8]  */
        unsigned int reserved_1 : 16;           /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg12;

/* Define the union u_od_pic_osd_rc_cfg13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int maxdiff_ctrl_en : 1; /* [0]  */
        unsigned int reserved_0 : 31;     /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg13;

/* Define the union u_od_pic_osd_rc_cfg14 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mb_bits_cap : 10;       /* [9..0]  */
        unsigned int reserved_0 : 6;         /* [15..10]  */
        unsigned int init_buf_bits_cap : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg14;

/* Define the union u_od_pic_osd_rc_cfg15 */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg15;

/* Define the union u_od_pic_osd_rc_cfg16 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sim_num_thr : 3;   /* [2..0]  */
        unsigned int reserved_0 : 5;    /* [7..3]  */
        unsigned int sum_y_err_thr : 7; /* [14..8]  */
        unsigned int reserved_1 : 1;    /* [15]  */
        unsigned int sum_c_err_thr : 7; /* [22..16]  */
        unsigned int reserved_2 : 9;    /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg16;

/* Define the union u_od_pic_osd_rc_cfg17 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cpmlx_sad_thr_y : 4;   /* [3..0]  */
        unsigned int reserved_0 : 4;        /* [7..4]  */
        unsigned int smpl_sad_thr_c : 4;    /* [11..8]  */
        unsigned int reserved_1 : 4;        /* [15..12]  */
        unsigned int smpl_sumsad_thr_y : 8; /* [23..16]  */
        unsigned int smpl_sumsad_thr_c : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg17;

/* Define the union u_od_pic_osd_rc_cfg18 */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg18;

/* Define the union u_od_pic_osd_rc_cfg19 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cmplx_sumsad_thr_y : 8; /* [7..0]  */
        unsigned int cmplx_sumsad_thr_c : 8; /* [15..8]  */
        unsigned int reserved_0 : 16;        /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_rc_cfg19;

/* Define the union u_od_pic_osd_stat_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int max_gap_bw_row_len_thr : 7; /* [6..0]  */
        unsigned int reserved_0 : 25;            /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_stat_thr;

/* Define the union u_od_pic_osd_pcmp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int pcmp_start_hpos : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;       /* [15..13]  */
        unsigned int pcmp_end_hpos : 13;   /* [28..16]  */
        unsigned int reserved_1 : 3;       /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_pcmp;

/* Define the union u_od_pic_osd_bs_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_size_reg : 22; /* [21..0]  */
        unsigned int reserved_0 : 10;     /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_bs_size;

/* Define the union u_od_pic_osd_worst_row */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int max_frm_row_len : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_worst_row;

/* Define the union u_od_pic_osd_best_row */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int min_frm_row_len : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_best_row;

/* Define the union u_od_pic_osd_stat_info */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int max_gap_bw_row_len_cnt : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;             /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_od_pic_osd_stat_info;

/* Define the union u_v0_mrg_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_y_l4_addr : 4; /* [3..0]  */
        unsigned int mrg_c_l4_addr : 4; /* [7..4]  */
        unsigned int reserved_0 : 12;   /* [19..8]  */
        unsigned int mrg_edge_en : 1;   /* [20]  */
        unsigned int reserved_1 : 4;    /* [24..21]  */
        unsigned int mrg_edge_typ : 1;  /* [25]  */
        unsigned int reserved_2 : 2;    /* [27..26]  */
        unsigned int mrg_crop_en : 1;   /* [28]  */
        unsigned int mrg_dcmp_en : 1;   /* [29]  */
        unsigned int mrg_mute_en : 1;   /* [30]  */
        unsigned int mrg_en : 1;        /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_mrg_ctrl;

/* Define the union u_v0_mrg_disp_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_xpos : 16; /* [15..0]  */
        unsigned int mrg_ypos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_mrg_disp_pos;

/* Define the union u_v0_mrg_disp_reso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_width : 16;  /* [15..0]  */
        unsigned int mrg_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_mrg_disp_reso;

/* Define the union u_v0_mrg_src_reso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_src_width : 16;  /* [15..0]  */
        unsigned int mrg_src_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_mrg_src_reso;

/* Define the union u_v0_mrg_src_offset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_src_hoffset : 16; /* [15..0]  */
        unsigned int mrg_src_voffset : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_mrg_src_offset;

/* Define the union u_v0_mrg_stride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_c_stride : 16; /* [15..0]  */
        unsigned int mrg_y_stride : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_mrg_stride;

/* Define the union u_v0_mrg_hstride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_ch_stride : 16; /* [15..0]  */
        unsigned int mrg_yh_stride : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_mrg_hstride;

/* Define the union u_v0_mrg_read_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rd_region : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_mrg_read_ctrl;

/* Define the union u_v0_mrg_read_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rd_en : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_mrg_read_en;

/* Define the union u_v1_mrg_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_y_l4_addr : 4; /* [3..0]  */
        unsigned int mrg_c_l4_addr : 4; /* [7..4]  */
        unsigned int reserved_0 : 12;   /* [19..8]  */
        unsigned int mrg_edge_en : 1;   /* [20]  */
        unsigned int reserved_1 : 4;    /* [24..21]  */
        unsigned int mrg_edge_typ : 1;  /* [25]  */
        unsigned int reserved_2 : 2;    /* [27..26]  */
        unsigned int mrg_crop_en : 1;   /* [28]  */
        unsigned int mrg_dcmp_en : 1;   /* [29]  */
        unsigned int mrg_mute_en : 1;   /* [30]  */
        unsigned int mrg_en : 1;        /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_mrg_ctrl;

/* Define the union u_v1_mrg_disp_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_xpos : 16; /* [15..0]  */
        unsigned int mrg_ypos : 16; /* [31..16]  */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART36_H_ */
