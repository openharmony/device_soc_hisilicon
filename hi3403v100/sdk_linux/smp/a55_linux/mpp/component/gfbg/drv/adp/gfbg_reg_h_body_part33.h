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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART33_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART33_H_

        unsigned int fdr_ck_gt_en : 1;  /* [21]  */
        unsigned int reserved_3 : 10;   /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_read_ctrl;

/* Define the union u_vid_mac_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int req_ctrl : 2;    /* [1..0]  */
        unsigned int req_len : 2;     /* [3..2]  */
        unsigned int reserved_0 : 4;  /* [7..4]  */
        unsigned int ofl_master : 1;  /* [8]  */
        unsigned int reserved_1 : 22; /* [30..9]  */
        unsigned int pre_rd_en : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_mac_ctrl;

/* Define the union u_vid_out_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int draw_pixel_mode : 3;  /* [2..0]  */
        unsigned int draw_pixel_en : 1;    /* [3]  */
        unsigned int uv_order_en : 1;      /* [4]  */
        unsigned int single_port_mode : 1; /* [5]  */
        unsigned int testpattern_en : 1;   /* [6]  */
        unsigned int reserved_0 : 25;      /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_out_ctrl;

/* Define the union u_vid_mute_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_alpha : 8;  /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_mute_alpha;

/* Define the union u_vid_mute_bk */
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
} u_vid_mute_bk;

/* Define the union u_vid_src_info */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int data_type : 3;   /* [2..0]  */
        unsigned int data_fmt : 2;    /* [4..3]  */
        unsigned int reserved_0 : 3;  /* [7..5]  */
        unsigned int data_width : 2;  /* [9..8]  */
        unsigned int reserved_1 : 2;  /* [11..10]  */
        unsigned int field_type : 1;  /* [12]  */
        unsigned int reserved_2 : 3;  /* [15..13]  */
        unsigned int disp_mode : 4;   /* [19..16]  */
        unsigned int dcmp_en : 2;     /* [21..20]  */
        unsigned int reserved_3 : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_src_info;

/* Define the union u_vid_src_reso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int src_w : 16; /* [15..0]  */
        unsigned int src_h : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_src_reso;

/* Define the union u_vid_src_crop */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int src_crop_x : 16; /* [15..0]  */
        unsigned int src_crop_y : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_src_crop;

/* Define the union u_vid_in_reso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ireso_w : 16; /* [15..0]  */
        unsigned int ireso_h : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_in_reso;

/* Define the union u_vid_stride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lm_stride : 16;  /* [15..0]  */
        unsigned int chm_stride : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_stride;

/* Define the union u_vid_2bit_stride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lm_tile_stride : 16;  /* [15..0]  */
        unsigned int chm_tile_stride : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_2bit_stride;

/* Define the union u_vid_head_stride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lm_head_stride : 16;  /* [15..0]  */
        unsigned int chm_head_stride : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_head_stride;

/* Define the union u_vid_smmu_bypass */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lm_bypass_2d : 1;  /* [0]  */
        unsigned int chm_bypass_2d : 1; /* [1]  */
        unsigned int lm_bypass_3d : 1;  /* [2]  */
        unsigned int chm_bypass_3d : 1; /* [3]  */
        unsigned int reserved_0 : 28;   /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_smmu_bypass;

/* Define the union u_vid_testpat_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tp_speed : 10;     /* [9..0]  */
        unsigned int reserved_0 : 2;    /* [11..10]  */
        unsigned int tp_line_w : 1;     /* [12]  */
        unsigned int tp_color_mode : 1; /* [13]  */
        unsigned int reserved_1 : 2;    /* [15..14]  */
        unsigned int tp_mode : 2;       /* [17..16]  */
        unsigned int reserved_2 : 14;   /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_testpat_cfg;

/* Define the union u_vid_testpat_seed */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tp_seed : 30;   /* [29..0]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_testpat_seed;

/* Define the union u_vid_dcmp_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int c_is_lossless : 1; /* [0]  */
        unsigned int l_is_lossless : 1; /* [1]  */
        unsigned int c_cmp_mode : 1;    /* [2]  */
        unsigned int l_cmp_mode : 1;    /* [3]  */
        unsigned int c_cmp_rate : 2;    /* [5..4]  */
        unsigned int l_cmp_rate : 2;    /* [7..6]  */
        unsigned int mem_mode : 1;      /* [8]  */
        unsigned int reserved_0 : 23;   /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vid_dcmp_ctrl;

/* Define the union u_vdp_v3r2_lineseg_dcmp_glb_info */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ice_en : 1;      /* [0]  */
        unsigned int is_lossless : 1; /* [1]  */
        unsigned int cmp_mode : 1;    /* [2]  */
        unsigned int max_mb_qp_y : 3; /* [5..3]  */
        unsigned int reserved_0 : 10; /* [15..6]  */
        unsigned int max_mb_qp_c : 3; /* [18..16]  */
        unsigned int seg_en : 1;      /* [19]  */
        unsigned int bit_depth : 1;   /* [20]  */
        unsigned int reserved_1 : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_dcmp_glb_info;

/* Define the union u_vdp_v3r2_lineseg_dcmp_frame_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_height : 14; /* [13..0]  */
        unsigned int reserved_0 : 2;    /* [15..14]  */
        unsigned int frame_width : 14;  /* [29..16]  */
        unsigned int reserved_1 : 2;    /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_dcmp_frame_size;

/* Define the union u_vdp_v3r2_lineseg_dcmp_smth_deltabits_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int smooth_deltabits_thr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;          /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_dcmp_smth_deltabits_thr;

/* Define the union u_vdp_v3r2_lineseg_dcmp_error_sta */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dcmp_error : 1;  /* [0]  */
        unsigned int forgive : 1;     /* [1]  */
        unsigned int consume : 1;     /* [2]  */
        unsigned int reserved_0 : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_dcmp_error_sta;

/* Define the union u_vdp_v3r2_lineseg_dcmp_glb_info_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ice_en : 1;      /* [0]  */
        unsigned int is_lossless : 1; /* [1]  */
        unsigned int cmp_mode : 1;    /* [2]  */
        unsigned int max_mb_qp_y : 3; /* [5..3]  */
        unsigned int reserved_0 : 10; /* [15..6]  */
        unsigned int max_mb_qp_c : 3; /* [18..16]  */
        unsigned int seg_en : 1;      /* [19]  */
        unsigned int bit_depth : 1;   /* [20]  */
        unsigned int reserved_1 : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_dcmp_glb_info_c;

/* Define the union u_vdp_v3r2_lineseg_dcmp_frame_size_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_height : 14; /* [13..0]  */
        unsigned int reserved_0 : 2;    /* [15..14]  */
        unsigned int frame_width : 14;  /* [29..16]  */
        unsigned int reserved_1 : 2;    /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_dcmp_frame_size_c;

/* Define the union u_vdp_v3r2_lineseg_dcmp_smth_deltabits_thr_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int smooth_deltabits_thr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;          /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_dcmp_smth_deltabits_thr_c;

/* Define the union u_vdp_v3r2_lineseg_dcmp_error_sta_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dcmp_error : 1;  /* [0]  */
        unsigned int forgive : 1;     /* [1]  */
        unsigned int consume : 1;     /* [2]  */
        unsigned int reserved_0 : 29; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_lineseg_dcmp_error_sta_c;

/* Define the union u_gfx_read_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int read_mode : 2;    /* [1..0]  */
        unsigned int reserved_0 : 2;   /* [3..2]  */
        unsigned int draw_mode : 2;    /* [5..4]  */
        unsigned int reserved_1 : 2;   /* [7..6]  */
        unsigned int flip_en : 1;      /* [8]  */
        unsigned int reserved_2 : 1;   /* [9]  */
        unsigned int mute_en : 1;      /* [10]  */
        unsigned int mute_req_en : 1;  /* [11]  */
        unsigned int fdr_ck_gt_en : 1; /* [12]  */
        unsigned int reserved_3 : 19;  /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_read_ctrl;

/* Define the union u_gfx_mac_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int req_ctrl : 2;       /* [1..0]  */
        unsigned int req_len : 2;        /* [3..2]  */
        unsigned int reserved_0 : 4;     /* [7..4]  */
        unsigned int ofl_master : 1;     /* [8]  */
        unsigned int dcmp_thd_close : 1; /* [9] */
        unsigned int dcmp_mute_ctrl : 1; /* [10] */
        unsigned int reserved_1 : 13;    /* [23..11]  */
        unsigned int req_ld_mode : 2;    /* [25..24] */
        unsigned int reserved_2 : 6;     /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_mac_ctrl;

/* Define the union u_gfx_out_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int palpha_range : 1;   /* [0]  */
        unsigned int palpha_en : 1;      /* [1]  */
        unsigned int reserved_0 : 2;     /* [3..2]  */
        unsigned int key_mode : 1;       /* [4]  */
        unsigned int enable : 1;         /* [5]  */
        unsigned int reserved_1 : 2;     /* [7..6]  */
        unsigned int bitext : 2;         /* [9..8]  */
        unsigned int premulti_en : 1;    /* [10]  */
        unsigned int testpattern_en : 1; /* [11]  */
        unsigned int reserved_2 : 20;    /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_out_ctrl;

/* Define the union u_gfx_mute_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_alpha : 8;  /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_mute_alpha;

/* Define the union u_gfx_mute_bk */
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
} u_gfx_mute_bk;

/* Define the union u_gfx_smmu_bypass */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int smmu_bypass_2d : 1; /* [0]  */
        unsigned int smmu_bypass_3d : 1; /* [1]  */
        unsigned int reserved_0 : 30;    /* [31..2]  */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART33_H_ */
