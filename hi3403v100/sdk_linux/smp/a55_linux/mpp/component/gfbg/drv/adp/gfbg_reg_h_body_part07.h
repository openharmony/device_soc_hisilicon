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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART07_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART07_H_

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_ot_pp_csc2_odc2;

/* Define the union u_v1_ot_pp_csc2_min_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc2_min_y : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_ot_pp_csc2_min_y;

/* Define the union u_v1_ot_pp_csc2_min_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc2_min_c : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_ot_pp_csc2_min_c;

/* Define the union u_v1_ot_pp_csc2_max_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc2_max_y : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_ot_pp_csc2_max_y;

/* Define the union u_v1_ot_pp_csc2_max_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc2_max_c : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_ot_pp_csc2_max_c;

/* Define the union u_v1_ot_pp_csc_ink_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ink_en : 1;       /* [0]  */
        unsigned int ink_sel : 1;      /* [1]  */
        unsigned int data_fmt : 1;     /* [2]  */
        unsigned int cross_enable : 1; /* [3]  */
        unsigned int color_mode : 2;   /* [5..4]  */
        unsigned int reserved_0 : 26;  /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_ot_pp_csc_ink_ctrl;

/* Define the union u_v1_ot_pp_csc_ink_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int x_pos : 16; /* [15..0]  */
        unsigned int y_pos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_ot_pp_csc_ink_pos;

/* Define the union u_v1_cvfir_vinfo */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int out_height : 16;   /* [15..0]  */
        unsigned int out_fmt : 2;       /* [17..16]  */
        unsigned int out_pro : 1;       /* [18]  */
        unsigned int vzme_ck_gt_en : 1; /* [19]  */
        unsigned int reserved_0 : 12;   /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_cvfir_vinfo;

/* Define the union u_v1_cvfir_vsp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vratio : 16;    /* [15..0]  */
        unsigned int reserved_0 : 1; /* [16]  */
        unsigned int reserved_1 : 8; /* [24..17]  */
        unsigned int cvfir_mode : 1; /* [25]  */
        unsigned int reserved_2 : 1; /* [26]  */
        unsigned int reserved_3 : 1; /* [27]  */
        unsigned int cvmid_en : 1;   /* [28]  */
        unsigned int reserved_4 : 1; /* [29]  */
        unsigned int cvfir_en : 1;   /* [30]  */
        unsigned int reserved_5 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_cvfir_vsp;

/* Define the union u_v1_cvfir_voffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vchroma_offset : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;     /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_cvfir_voffset;

/* Define the union u_v1_cvfir_vboffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbchroma_offset : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_cvfir_vboffset;

/* Define the union u_v1_cvfir_vcoef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vccoef02 : 10;  /* [9..0]  */
        unsigned int vccoef01 : 10;  /* [19..10]  */
        unsigned int vccoef00 : 10;  /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_cvfir_vcoef0;

/* Define the union u_v1_cvfir_vcoef1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vccoef11 : 10;  /* [9..0]  */
        unsigned int vccoef10 : 10;  /* [19..10]  */
        unsigned int vccoef03 : 10;  /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_cvfir_vcoef1;

/* Define the union u_v1_cvfir_vcoef2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vccoef13 : 10;   /* [9..0]  */
        unsigned int vccoef12 : 10;   /* [19..10]  */
        unsigned int reserved_0 : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_cvfir_vcoef2;

/* Define the union u_v1_hfir_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;  /* [0]  */
        unsigned int hfir_mode : 2;   /* [2..1]  */
        unsigned int mid_en : 1;      /* [3]  */
        unsigned int ck_gt_en : 1;    /* [4]  */
        unsigned int reserved_1 : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_hfir_ctrl;

/* Define the union u_v1_hfircoef01 */
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
} u_v1_hfircoef01;

/* Define the union u_v1_hfircoef23 */
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
} u_v1_hfircoef23;

/* Define the union u_v1_hfircoef45 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int coef4 : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef5 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_hfircoef45;

/* Define the union u_v1_hfircoef67 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int coef6 : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef7 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_hfircoef67;

/* Define the union u_v2_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int galpha : 8;      /* [7..0]  */
        unsigned int reserved_0 : 20; /* [27..8]  */
        unsigned int rupd_field : 1;  /* [28]  */
        unsigned int rgup_mode : 1;   /* [29]  */
        unsigned int nosec_flag : 1;  /* [30]  */
        unsigned int surface_en : 1;  /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_ctrl;

/* Define the union u_v2_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_upd;

/* Define the union u_v2_0reso_read */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0]  */
        unsigned int oh : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_0reso_read;

/* Define the union u_v2_ireso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int iw : 16; /* [15..0]  */
        unsigned int ih : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_ireso;

/* Define the union u_v2_dfpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xfpos : 16; /* [15..0]  */
        unsigned int disp_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_dfpos;

/* Define the union u_v2_dlpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xlpos : 16; /* [15..0]  */
        unsigned int disp_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_dlpos;

/* Define the union u_v2_vfpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xfpos : 16; /* [15..0]  */
        unsigned int video_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_vfpos;

/* Define the union u_v2_vlpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xlpos : 16; /* [15..0]  */
        unsigned int video_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_vlpos;

/* Define the union u_v2_bk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbk_cr : 10;    /* [9..0]  */
        unsigned int vbk_cb : 10;    /* [19..10]  */
        unsigned int vbk_y : 10;     /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_bk;

/* Define the union u_v2_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbk_alpha : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_alpha;

/* Define the union u_v2_mute_bk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_cr : 10;   /* [9..0]  */
        unsigned int mute_cb : 10;   /* [19..10]  */
        unsigned int mute_y : 10;    /* [29..20]  */
        unsigned int reserved_0 : 1; /* [30]  */
        unsigned int mute_en : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_mute_bk;

/* Define the union u_v2_ot_pp_csc_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_en : 1;       /* [0]  */
        unsigned int ot_pp_csc_demo_en : 1;  /* [1]  */
        unsigned int ot_pp_csc_ck_gt_en : 1; /* [2]  */
        unsigned int reserved_0 : 29;       /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_ot_pp_csc_ctrl;

/* Define the union u_v2_ot_pp_csc_coef00 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef00 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_ot_pp_csc_coef00;

/* Define the union u_v2_ot_pp_csc_coef01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef01 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_ot_pp_csc_coef01;


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART07_H_ */
