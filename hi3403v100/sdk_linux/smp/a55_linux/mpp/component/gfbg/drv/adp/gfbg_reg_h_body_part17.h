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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART17_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART17_H_

    /* Define an unsigned member */
    unsigned int u32;
} u_g2_ot_pp_csc2_max_c;

/* Define the union u_g2_ot_pp_csc_ink_ctrl */
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
} u_g2_ot_pp_csc_ink_ctrl;

/* Define the union u_g2_ot_pp_csc_ink_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int x_pos : 16; /* [15..0]  */
        unsigned int y_pos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g2_ot_pp_csc_ink_pos;

/* Define the union u_g3_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int galpha : 8;       /* [7..0]  */
        unsigned int reserved_0 : 19;  /* [26..8]  */
        unsigned int g1_depremult : 1; /* [27]  */
        unsigned int rupd_field : 1;   /* [28]  */
        unsigned int rgup_mode : 1;    /* [29]  */
        unsigned int nosec_flag : 1;   /* [30]  */
        unsigned int surface_en : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ctrl;

/* Define the union u_g3_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_upd;

/* Define the union u_g3_0reso_read */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0]  */
        unsigned int oh : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_0reso_read;

/* Define the union u_g3_ireso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int iw : 16; /* [15..0]  */
        unsigned int ih : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ireso;

/* Define the union u_g3_dfpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xfpos : 16; /* [15..0]  */
        unsigned int disp_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_dfpos;

/* Define the union u_g3_dlpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xlpos : 16; /* [15..0]  */
        unsigned int disp_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_dlpos;

/* Define the union u_g3_vfpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xfpos : 16; /* [15..0]  */
        unsigned int video_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_vfpos;

/* Define the union u_g3_vlpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xlpos : 16; /* [15..0]  */
        unsigned int video_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_vlpos;

/* Define the union u_g3_bk */
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
} u_g3_bk;

/* Define the union u_g3_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbk_alpha : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_alpha;

/* Define the union u_g3_mute_bk */
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
} u_g3_mute_bk;

/* Define the union u_g3_lbox_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_en : 1;     /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_lbox_ctrl;

/* Define the union u_g3_ot_pp_csc_ctrl */
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
} u_g3_ot_pp_csc_ctrl;

/* Define the union u_g3_ot_pp_csc_coef00 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef00 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_coef00;

/* Define the union u_g3_ot_pp_csc_coef01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef01 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_coef01;

/* Define the union u_g3_ot_pp_csc_coef02 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef02 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_coef02;

/* Define the union u_g3_ot_pp_csc_coef10 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef10 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_coef10;

/* Define the union u_g3_ot_pp_csc_coef11 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef11 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_coef11;

/* Define the union u_g3_ot_pp_csc_coef12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef12 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_coef12;

/* Define the union u_g3_ot_pp_csc_coef20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef20 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_coef20;

/* Define the union u_g3_ot_pp_csc_coef21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef21 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_coef21;

/* Define the union u_g3_ot_pp_csc_coef22 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef22 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_coef22;

/* Define the union u_g3_ot_pp_csc_scale */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_scale : 4; /* [3..0]  */
        unsigned int reserved_0 : 28;    /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_scale;

/* Define the union u_g3_ot_pp_csc_idc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_idc0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;    /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_idc0;

/* Define the union u_g3_ot_pp_csc_idc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_idc1 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;    /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_idc1;

/* Define the union u_g3_ot_pp_csc_idc2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_idc2 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;    /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_idc2;

/* Define the union u_g3_ot_pp_csc_odc0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_odc0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;    /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_odc0;

/* Define the union u_g3_ot_pp_csc_odc1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_odc1 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;    /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_odc1;

/* Define the union u_g3_ot_pp_csc_odc2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_odc2 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;    /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_odc2;

/* Define the union u_g3_ot_pp_csc_min_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_min_y : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;     /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_min_y;

/* Define the union u_g3_ot_pp_csc_min_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_min_c : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;     /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_min_c;

/* Define the union u_g3_ot_pp_csc_max_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_max_y : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;     /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_ot_pp_csc_max_y;

/* Define the union u_g3_ot_pp_csc_max_c */
typedef union {
    /* Define the struct bits */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART17_H_ */
