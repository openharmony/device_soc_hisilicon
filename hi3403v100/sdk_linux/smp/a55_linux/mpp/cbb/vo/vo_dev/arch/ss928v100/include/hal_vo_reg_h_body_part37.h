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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART37_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART37_H_

        unsigned int c_tunl_err : 1;  /* [1]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_tunl_errsta;

/* define the union reg_vid_tunl_debug */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int l_tunl_err_num : 16; /* [15..0]  */
        unsigned int c_tunl_err_num : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_tunl_debug;

/* define the union reg_vid_dcmp_ctrl */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_vid_dcmp_ctrl;

/* define the union reg_vdp_v3r2_lineseg_dcmp_glb_info */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_dcmp_glb_info;

/* define the union reg_vdp_v3r2_lineseg_dcmp_frame_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int frame_height : 14; /* [13..0]  */
        unsigned int reserved_0 : 2;    /* [15..14]  */
        unsigned int frame_width : 14;  /* [29..16]  */
        unsigned int reserved_1 : 2;    /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_dcmp_frame_size;

/* define the union reg_vdp_v3r2_lineseg_dcmp_smth_deltabits_thr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int smooth_deltabits_thr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;          /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_dcmp_smth_deltabits_thr;

/* define the union reg_vdp_v3r2_lineseg_dcmp_error_sta */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dcmp_error : 1;  /* [0]  */
        unsigned int forgive : 1;     /* [1]  */
        unsigned int consume : 1;     /* [2]  */
        unsigned int reserved_0 : 29; /* [31..3]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_dcmp_error_sta;

/* define the union reg_vdp_v3r2_lineseg_dcmp_glb_info_c */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_dcmp_glb_info_c;

/* define the union reg_vdp_v3r2_lineseg_dcmp_frame_size_c */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int frame_height : 14; /* [13..0]  */
        unsigned int reserved_0 : 2;    /* [15..14]  */
        unsigned int frame_width : 14;  /* [29..16]  */
        unsigned int reserved_1 : 2;    /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_dcmp_frame_size_c;

/* define the union reg_vdp_v3r2_lineseg_dcmp_smth_deltabits_thr_c */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int smooth_deltabits_thr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;          /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_dcmp_smth_deltabits_thr_c;

/* define the union reg_vdp_v3r2_lineseg_dcmp_error_sta_c */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dcmp_error : 1;  /* [0]  */
        unsigned int forgive : 1;     /* [1]  */
        unsigned int consume : 1;     /* [2]  */
        unsigned int reserved_0 : 29; /* [31..3]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_dcmp_error_sta_c;

/* define the union reg_gfx_read_ctrl */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_read_ctrl;

/* define the union reg_gfx_mac_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int req_ctrl : 2;       /* [1..0]  */
        unsigned int req_len : 2;        /* [3..2]  */
        unsigned int reserved_0 : 4;     /* [7..4]  */
        unsigned int ofl_master : 1;     /* [8]  */
        unsigned int dcmp_thd_close : 1; /* [9]  */
        unsigned int dcmp_mute_ctrl : 1; /* [10]  */
        unsigned int reserved_1 : 13;    /* [23..11]  */
        unsigned int req_ld_mode : 2;    /* [25..24]  */
        unsigned int reserved_2 : 6;     /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_mac_ctrl;

/* define the union reg_gfx_out_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int palpha_range : 1;   /* [0]  */
        unsigned int palpha_en : 1;      /* [1]  */
        unsigned int reserved_0 : 2;     /* [3..2]  */
        unsigned int key_mode : 1;       /* [4]  */
        unsigned int key_en : 1;         /* [5]  */
        unsigned int reserved_1 : 2;     /* [7..6]  */
        unsigned int bitext : 2;         /* [9..8]  */
        unsigned int premulti_en : 1;    /* [10]  */
        unsigned int testpattern_en : 1; /* [11]  */
        unsigned int reserved_2 : 20;    /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_out_ctrl;

/* define the union reg_gfx_mute_alpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mute_alpha : 8;  /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_mute_alpha;

/* define the union reg_gfx_mute_bk */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mute_cr : 10;   /* [9..0]  */
        unsigned int mute_cb : 10;   /* [19..10]  */
        unsigned int mute_y : 10;    /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_mute_bk;

/* define the union reg_gfx_smmu_bypass */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int smmu_bypass_2d : 1; /* [0]  */
        unsigned int smmu_bypass_3d : 1; /* [1]  */
        unsigned int reserved_0 : 30;    /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_smmu_bypass;

/* define the union reg_gfx_1555_alpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int alpha_0 : 8;     /* [7..0]  */
        unsigned int alpha_1 : 8;     /* [15..8]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_1555_alpha;

/* define the union reg_gfx_src_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ifmt : 8;        /* [7..0]  */
        unsigned int reserved_0 : 8;  /* [15..8]  */
        unsigned int disp_mode : 4;   /* [19..16]  */
        unsigned int dcmp_en : 1;     /* [20]  */
        unsigned int reserved_1 : 11; /* [31..21]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_src_info;

/* define the union reg_gfx_src_reso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int src_w : 16; /* [15..0]  */
        unsigned int src_h : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_src_reso;

/* define the union reg_gfx_src_crop */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int src_crop_x : 16; /* [15..0]  */
        unsigned int src_crop_y : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_src_crop;

/* define the union reg_gfx_ireso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ireso_w : 16; /* [15..0]  */
        unsigned int ireso_h : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_ireso;

/* define the union reg_gfx_stride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int surface_stride : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;     /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_stride;

/* define the union reg_gfx_ckey_max */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int key_b_max : 8;  /* [7..0]  */
        unsigned int key_g_max : 8;  /* [15..8]  */
        unsigned int key_r_max : 8;  /* [23..16]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_ckey_max;

/* define the union reg_gfx_ckey_min */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int key_b_min : 8;  /* [7..0]  */
        unsigned int key_g_min : 8;  /* [15..8]  */
        unsigned int key_r_min : 8;  /* [23..16]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_ckey_min;

/* define the union reg_gfx_ckey_mask */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int key_b_msk : 8;  /* [7..0]  */
        unsigned int key_g_msk : 8;  /* [15..8]  */
        unsigned int key_r_msk : 8;  /* [23..16]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_ckey_mask;

/* define the union reg_gfx_testpat_cfg */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tp_speed : 10;     /* [9..0]  */
        unsigned int reserved_0 : 2;    /* [11..10]  */
        unsigned int tp_line_w : 1;     /* [12]  */
        unsigned int tp_color_mode : 1; /* [13]  */
        unsigned int reserved_1 : 2;    /* [15..14]  */
        unsigned int tp_mode : 2;       /* [17..16]  */
        unsigned int reserved_2 : 14;   /* [31..18]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_testpat_cfg;

/* define the union reg_gfx_testpat_seed */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tp_seed : 30;   /* [29..0]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_testpat_seed;

/* define the union reg_gfx_ld_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 1;     /* [0]  */
        unsigned int hw_mute_clr : 1;    /* [1]  */
        unsigned int ld_mute_en : 1;     /* [2]  */
        unsigned int ld_err_mute_en : 1; /* [3]  */
        unsigned int reserved_1 : 28;    /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_ld_ctrl;

/* define the union reg_gfx_ld_smute_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 31; /* [30..0]  */
        unsigned int sw_mute_clr : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_ld_smute_ctrl;

/* define the union reg_gfx_ld_err_sta */
typedef union {


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART37_H_ */
