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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART38_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART38_H_

    /* define the struct bits */
    struct {
        unsigned int ld_err_clr : 1;  /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_ld_err_sta;

/* define the union reg_vdp_v3r2_line_osd_dcmp_glb_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ice_en : 1;          /* [0]  */
        unsigned int cmp_mode : 1;        /* [1]  */
        unsigned int conv_en : 1;         /* [2]  */
        unsigned int is_lossless : 1;     /* [3]  */
        unsigned int osd_mode : 2;        /* [5..4]  */
        unsigned int max_mb_qp : 3;       /* [8..6]  */
        unsigned int excess_err_mask : 1; /* [9]  */
        unsigned int rw_reg_add : 6;      /* [15..10]  */
        unsigned int reserved_0 : 16;     /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_line_osd_dcmp_glb_info;

/* define the union reg_vdp_v3r2_line_osd_dcmp_frame_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int frame_width : 14;  /* [13..0]  */
        unsigned int reserved_0 : 2;    /* [15..14]  */
        unsigned int frame_height : 14; /* [29..16]  */
        unsigned int reserved_1 : 2;    /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_line_osd_dcmp_frame_size;

/* define the union reg_vdp_v3r2_line_osd_dcmp_error_sta */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dcmp_error : 1;        /* [0]  */
        unsigned int o_pix_forgive : 1;     /* [1]  */
        unsigned int o_pix_consume : 1;     /* [2]  */
        unsigned int o_mb_qp_error : 1;     /* [3]  */
        unsigned int o_dcmp_excess_err : 1; /* [4]  */
        unsigned int o_dcmp_err_add : 5;    /* [9..5]  */
        unsigned int o_dcmp_debug : 22;     /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_line_osd_dcmp_error_sta;

/* define the union reg_wbc_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 4;   /* [3..0]  */
        unsigned int data_width : 1;   /* [4]  */
        unsigned int reserved_1 : 3;   /* [7..5]  */
        unsigned int uv_order : 1;     /* [8]  */
        unsigned int flip_en : 1;      /* [9]  */
        unsigned int align_mode : 1;   /* [10]  */
        unsigned int reserved_2 : 3;   /* [13..11]  */
        unsigned int cap_ck_gt_en : 1; /* [14]  */
        unsigned int reserved_3 : 14;  /* [28..15]  */
        unsigned int wbc_cmp_en : 1;   /* [29]  */
        unsigned int reserved_4 : 2;   /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_ctrl;

/* define the union reg_wbc_mac_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int req_interval : 10; /* [9..0]  */
        unsigned int reserved_0 : 2;    /* [11..10]  */
        unsigned int wbc_len : 2;       /* [13..12]  */
        unsigned int reserved_1 : 18;   /* [31..14]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_mac_ctrl;

/* define the union reg_wbc_smmu_bypass */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int l_bypass : 1;    /* [0]  */
        unsigned int c_bypass : 1;    /* [1]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_smmu_bypass;

/* define the union reg_wbc_lowdlyctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wb_per_line_num : 12;  /* [11..0]  */
        unsigned int partfns_line_num : 12; /* [23..12]  */
        unsigned int reserved_0 : 6;        /* [29..24]  */
        unsigned int lowdly_test : 1;       /* [30]  */
        unsigned int lowdly_en : 1;         /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_lowdlyctrl;

/* define the union reg_wbc_lowdlysta */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 31; /* [30..0]  */
        unsigned int part_finish : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_lowdlysta;

/* define the union reg_wbc_ystride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wbc_ystride : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_ystride;

/* define the union reg_wbc_cstride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wbc_cstride : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cstride;

/* define the union reg_wbc_ynstride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wbc_ynstride : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;   /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_ynstride;

/* define the union reg_wbc_cnstride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wbc_cnstride : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;   /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cnstride;

/* define the union reg_wbc_sta */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wbc_l_busy : 1;  /* [0]  */
        unsigned int wbc_c_busy : 1;  /* [1]  */
        unsigned int wbc_lh_busy : 1; /* [2]  */
        unsigned int wbc_ch_busy : 1; /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_sta;

/* define the union reg_wbc_line_num */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wbc_l_linenum : 16; /* [15..0]  */
        unsigned int wbc_c_linenum : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_line_num;

/* define the union reg_wbc_cap_reso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cap_width : 16;  /* [15..0]  */
        unsigned int cap_height : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_cap_reso;

/* define the union reg_vdp_v3r2_lineseg_cmp_glb_info */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_glb_info;

/* define the union reg_vdp_v3r2_lineseg_cmp_frame_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int frame_width : 14;  /* [13..0]  */
        unsigned int reserved_0 : 2;    /* [15..14]  */
        unsigned int frame_height : 14; /* [29..16]  */
        unsigned int reserved_1 : 2;    /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_frame_size;

/* define the union reg_vdp_v3r2_lineseg_cmp_rc_cfg0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int big_grad_thr : 8;      /* [7..0]  */
        unsigned int diff_thr : 8;          /* [15..8]  */
        unsigned int noise_pix_num_thr : 6; /* [21..16]  */
        unsigned int reserved_0 : 10;       /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_rc_cfg0;

/* define the union reg_vdp_v3r2_lineseg_cmp_rc_cfg1 */
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
} reg_vdp_v3r2_lineseg_cmp_rc_cfg1;

/* define the union reg_vdp_v3r2_lineseg_cmp_rc_cfg12 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int buffer_init_bits : 16; /* [15..0]  */
        unsigned int buffer_size : 16;      /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_rc_cfg12;

/* define the union reg_vdp_v3r2_lineseg_cmp_rc_cfg13 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int budget_mb_bits : 10;      /* [9..0]  */
        unsigned int budget_mb_bits_last : 10; /* [19..10]  */
        unsigned int min_mb_bits : 10;         /* [29..20]  */
        unsigned int reserved_0 : 2;           /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_rc_cfg13;

/* define the union reg_vdp_v3r2_lineseg_cmp_rc_cfg16 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int smooth_status_thr : 4;    /* [3..0]  */
        unsigned int smooth_deltabits_thr : 8; /* [11..4]  */
        unsigned int max_mb_qp : 3;            /* [14..12]  */
        unsigned int reserved_0 : 17;          /* [31..15]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_rc_cfg16;

/* define the union reg_vdp_v3r2_lineseg_cmp_glb_st */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int max_left_bits_buffer : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;           /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_glb_st;

/* define the union reg_vdp_v3r2_lineseg_cmp_glb_info_c */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_glb_info_c;

/* define the union reg_vdp_v3r2_lineseg_cmp_frame_size_c */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int frame_width : 14;  /* [13..0]  */
        unsigned int reserved_0 : 2;    /* [15..14]  */
        unsigned int frame_height : 14; /* [29..16]  */
        unsigned int reserved_1 : 2;    /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_frame_size_c;

/* define the union reg_vdp_v3r2_lineseg_cmp_rc_cfg0_c */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int big_grad_thr : 8;      /* [7..0]  */
        unsigned int diff_thr : 8;          /* [15..8]  */
        unsigned int noise_pix_num_thr : 6; /* [21..16]  */
        unsigned int reserved_0 : 10;       /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_rc_cfg0_c;

/* define the union reg_vdp_v3r2_lineseg_cmp_rc_cfg1_c */
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
} reg_vdp_v3r2_lineseg_cmp_rc_cfg1_c;

/* define the union reg_vdp_v3r2_lineseg_cmp_rc_cfg12_c */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int buffer_init_bits : 16; /* [15..0]  */
        unsigned int buffer_size : 16;      /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_rc_cfg12_c;

/* define the union reg_vdp_v3r2_lineseg_cmp_rc_cfg13_c */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int budget_mb_bits : 10;      /* [9..0]  */
        unsigned int budget_mb_bits_last : 10; /* [19..10]  */
        unsigned int min_mb_bits : 10;         /* [29..20]  */
        unsigned int reserved_0 : 2;           /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vdp_v3r2_lineseg_cmp_rc_cfg13_c;

/* define the union reg_vdp_v3r2_lineseg_cmp_rc_cfg16_c */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int smooth_status_thr : 4;    /* [3..0]  */
        unsigned int smooth_deltabits_thr : 8; /* [11..4]  */
        unsigned int max_mb_qp : 3;            /* [14..12]  */
        unsigned int reserved_0 : 17;          /* [31..15]  */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART38_H_ */
