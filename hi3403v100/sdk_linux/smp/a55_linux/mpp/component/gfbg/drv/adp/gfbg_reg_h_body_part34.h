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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART34_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART34_H_

    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_smmu_bypass;

/* Define the union u_gfx_1555_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int alpha_0 : 8;     /* [7..0]  */
        unsigned int alpha_1 : 8;     /* [15..8]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_1555_alpha;

/* Define the union u_gfx_src_info */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ifmt : 8;        /* [7..0]  */
        unsigned int reserved_0 : 8;  /* [15..8]  */
        unsigned int disp_mode : 4;   /* [19..16]  */
        unsigned int dcmp_en : 1;     /* [20]  */
        unsigned int reserved_1 : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_src_info;

/* Define the union u_gfx_src_reso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int src_w : 16; /* [15..0]  */
        unsigned int src_h : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_src_reso;

/* Define the union u_gfx_src_crop */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int src_crop_x : 16; /* [15..0]  */
        unsigned int src_crop_y : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_src_crop;

/* Define the union u_gfx_ireso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ireso_w : 16; /* [15..0]  */
        unsigned int ireso_h : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_ireso;

/* Define the union u_gfx_stride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int surface_stride : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;     /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_stride;

/* Define the union u_gfx_ckey_max */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int key_b_max : 8;  /* [7..0]  */
        unsigned int key_g_max : 8;  /* [15..8]  */
        unsigned int key_r_max : 8;  /* [23..16]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_ckey_max;

/* Define the union u_gfx_ckey_min */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int key_b_min : 8;  /* [7..0]  */
        unsigned int key_g_min : 8;  /* [15..8]  */
        unsigned int key_r_min : 8;  /* [23..16]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_ckey_min;

/* Define the union u_gfx_ckey_mask */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int key_b_msk : 8;  /* [7..0]  */
        unsigned int key_g_msk : 8;  /* [15..8]  */
        unsigned int key_r_msk : 8;  /* [23..16]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_ckey_mask;

/* Define the union u_gfx_testpat_cfg */
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
} u_gfx_testpat_cfg;

/* Define the union u_gfx_testpat_seed */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tp_seed : 30;   /* [29..0]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_testpat_seed;

/* Define the union u_gfx_ld_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;     /* [0] */
        unsigned int hw_mute_clr : 1;    /* [1] */
        unsigned int ld_mute_en : 1;     /* [2] */
        unsigned int ld_err_mute_en : 1; /* [3] */
        unsigned int reserved_1 : 28;    /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_ld_ctrl;

/* Define the union u_gfx_ld_smute_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved : 31;   /* [30..0] */
        unsigned int sw_mute_clr : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_ld_smute_ctrl;

/* Define the union u_gfx_ld_err_sta */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ld_err_clr : 1; /* [0] */
        unsigned int reserved : 31;  /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_ld_err_sta;

#ifdef CONFIG_TDE_GFBG_COMPRESS_V1
/* Define the union u_gfx_dcmp_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int is_lossless : 1;   /* [0]  */
        unsigned int is_lossless_a : 1; /* [1]  */
        unsigned int cmp_mode : 1;      /* [2]  */
        unsigned int osd_mode : 2;      /* [4..3]  */
        unsigned int reserved_0 : 27;   /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_dcmp_ctrl;

/* Define the union u_gfx_dcmp_wrong_sta */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int bs_err : 1;      /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_dcmp_wrong_sta;
#endif /* CONFIG_TDE_GFBG_COMPRESS_V1 */

#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
/* define the union reg_vdp_v3r2_line_osd_dcmp_glb_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ice_en : 1; /* [0]  */
        unsigned int cmp_mode : 1; /* [1]  */
        unsigned int conv_en : 1; /* [2]  */
        unsigned int is_lossless : 1; /* [3]  */
        unsigned int osd_mode : 2; /* [5..4]  */
        unsigned int max_mb_qp : 3; /* [8..6]  */
        unsigned int excess_err_mask : 1; /* [9]  */
        unsigned int rw_reg_add : 6; /* [15..10]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_line_osd_dcmp_glb_info;

/* define the union reg_vdp_v3r2_line_osd_dcmp_frame_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int frame_width : 14; /* [13..0]  */
        unsigned int reserved_0 : 2; /* [15..14]  */
        unsigned int frame_height : 14; /* [29..16]  */
        unsigned int reserved_1 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_line_osd_dcmp_frame_size;

/* define the union reg_vdp_v3r2_line_osd_dcmp_error_sta */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dcmp_error : 1; /* [0]  */
        unsigned int o_pix_forgive : 1; /* [1]  */
        unsigned int o_pix_consume : 1; /* [2]  */
        unsigned int o_mb_qp_error : 1; /* [3]  */
        unsigned int o_dcmp_excess_err : 1; /* [4]  */
        unsigned int o_dcmp_err_add : 5; /* [9..5]  */
        unsigned int o_dcmp_debug : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_vdp_v3r2_line_osd_dcmp_error_sta;
#endif /* CONFIG_TDE_GFBG_COMPRESS_V2 */

/* Define the union u_wbc_ctrl */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_ctrl;

/* Define the union u_wbc_mac_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int req_interval : 10; /* [9..0]  */
        unsigned int reserved_0 : 2;    /* [11..10]  */
        unsigned int wbc_len : 2;       /* [13..12]  */
        unsigned int reserved_1 : 18;   /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_mac_ctrl;

/* Define the union u_wbc_smmu_bypass */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int l_bypass : 1;    /* [0]  */
        unsigned int c_bypass : 1;    /* [1]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_smmu_bypass;

/* Define the union u_wbc_lowdlyctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wb_per_line_num : 12;  /* [11..0]  */
        unsigned int partfns_line_num : 12; /* [23..12]  */
        unsigned int reserved_0 : 6;        /* [29..24]  */
        unsigned int lowdly_test : 1;       /* [30]  */
        unsigned int lowdly_en : 1;         /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_lowdlyctrl;

/* Define the union u_wbc_lowdlysta */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 31; /* [30..0]  */
        unsigned int part_finish : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_lowdlysta;

/* Define the union u_wbc_ystride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbc_ystride : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_ystride;

/* Define the union u_wbc_cstride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbc_cstride : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cstride;

/* Define the union u_wbc_ynstride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbc_ynstride : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;   /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_ynstride;

/* Define the union u_wbc_cnstride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbc_cnstride : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;   /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_cnstride;

/* Define the union u_wbc_sta */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbc_l_busy : 1;  /* [0]  */
        unsigned int wbc_c_busy : 1;  /* [1]  */
        unsigned int wbc_lh_busy : 1; /* [2]  */
        unsigned int wbc_ch_busy : 1; /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_sta;

/* Define the union u_wbc_line_num */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbc_l_linenum : 16; /* [15..0]  */
        unsigned int wbc_c_linenum : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_line_num;

/* Define the union u_wbc_cap_reso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cap_width : 16;  /* [15..0]  */
        unsigned int cap_height : 16; /* [31..16]  */
    } bits;


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART34_H_ */
