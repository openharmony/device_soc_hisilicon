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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART18_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART18_H_

        unsigned int ot_pp_csc2_coef11 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;       /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_coef11;

/* define the union reg_g3_ot_pp_csc2_coef12 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_coef12 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;       /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_coef12;

/* define the union reg_g3_ot_pp_csc2_coef20 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_coef20 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;       /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_coef20;

/* define the union reg_g3_ot_pp_csc2_coef21 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_coef21 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;       /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_coef21;

/* define the union reg_g3_ot_pp_csc2_coef22 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_coef22 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;       /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_coef22;

/* define the union reg_g3_ot_pp_csc2_scale */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_scale : 4; /* [3..0]  */
        unsigned int reserved_0 : 28;     /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_scale;

/* define the union reg_g3_ot_pp_csc2_idc0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_idc0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;     /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_idc0;

/* define the union reg_g3_ot_pp_csc2_idc1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_idc1 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;     /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_idc1;

/* define the union reg_g3_ot_pp_csc2_idc2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_idc2 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;     /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_idc2;

/* define the union reg_g3_ot_pp_csc2_odc0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_odc0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;     /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_odc0;

/* define the union reg_g3_ot_pp_csc2_odc1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_odc1 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;     /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_odc1;

/* define the union reg_g3_ot_pp_csc2_odc2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_odc2 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;     /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_odc2;

/* define the union reg_g3_ot_pp_csc2_min_y */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_min_y : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_min_y;

/* define the union reg_g3_ot_pp_csc2_min_c */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_min_c : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_min_c;

/* define the union reg_g3_ot_pp_csc2_max_y */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_max_y : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_max_y;

/* define the union reg_g3_ot_pp_csc2_max_c */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_max_c : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc2_max_c;

/* define the union reg_g3_ot_pp_csc_ink_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ink_en : 1;       /* [0]  */
        unsigned int ink_sel : 1;      /* [1]  */
        unsigned int data_fmt : 1;     /* [2]  */
        unsigned int cross_enable : 1; /* [3]  */
        unsigned int color_mode : 2;   /* [5..4]  */
        unsigned int reserved_0 : 26;  /* [31..6]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc_ink_ctrl;

/* define the union reg_g3_ot_pp_csc_ink_pos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int x_pos : 16; /* [15..0]  */
        unsigned int y_pos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_ot_pp_csc_ink_pos;

/* define the union reg_g3_osb_mute_bk */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int osb_bk_v : 10;   /* [9..0]  */
        unsigned int osb_bk_u : 10;   /* [19..10]  */
        unsigned int osb_bk_y : 10;   /* [29..20]  */
        unsigned int reserved_0 : 1;  /* [30]  */
        unsigned int osb_mute_en : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_osb_mute_bk;

/* define the union reg_g3_osb_bk_alpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int osb_bk_alpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;  /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_osb_bk_alpha;

/* define the union reg_g3_osb_coef_rd_en */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int osb_rd_en : 1;   /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_osb_coef_rd_en;

/* define the union reg_g4_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int galpha : 8;       /* [7..0]  */
        unsigned int reserved_0 : 19;  /* [26..8]  */
        unsigned int g1_depremult : 1; /* [27]  */
        unsigned int rupd_field : 1;   /* [28]  */
        unsigned int rgup_mode : 1;    /* [29]  */
        unsigned int nosec_flag : 1;   /* [30]  */
        unsigned int surface_en : 1;   /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_ctrl;

/* define the union reg_g4_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_upd;

/* define the union reg_g4_0reso_read */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0]  */
        unsigned int oh : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_0reso_read;

/* define the union reg_g4_ireso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int iw : 16; /* [15..0]  */
        unsigned int ih : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_ireso;

/* define the union reg_g4_dfpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int disp_xfpos : 16; /* [15..0]  */
        unsigned int disp_yfpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_dfpos;

/* define the union reg_g4_dlpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int disp_xlpos : 16; /* [15..0]  */
        unsigned int disp_ylpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_dlpos;

/* define the union reg_g4_vfpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_xfpos : 16; /* [15..0]  */
        unsigned int video_yfpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_vfpos;

/* define the union reg_g4_vlpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_xlpos : 16; /* [15..0]  */
        unsigned int video_ylpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_vlpos;

/* define the union reg_g4_bk */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vbk_cr : 10;    /* [9..0]  */
        unsigned int vbk_cb : 10;    /* [19..10]  */
        unsigned int vbk_y : 10;     /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_bk;

/* define the union reg_g4_alpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vbk_alpha : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_alpha;

/* define the union reg_g4_mute_bk */
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
} reg_g4_mute_bk;

/* define the union reg_g4_lbox_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mute_en : 1;     /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_lbox_ctrl;

/* define the union reg_g4_ot_pp_csc_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_en : 1;       /* [0]  */
        unsigned int ot_pp_csc_demo_en : 1;  /* [1]  */
        unsigned int ot_pp_csc_ck_gt_en : 1; /* [2]  */
        unsigned int reserved_0 : 29;       /* [31..3]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_ot_pp_csc_ctrl;

/* define the union reg_g4_ot_pp_csc_coef00 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef00 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART18_H_ */
