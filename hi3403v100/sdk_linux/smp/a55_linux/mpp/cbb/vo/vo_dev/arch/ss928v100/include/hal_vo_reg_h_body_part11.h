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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART11_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART11_H_

        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef1 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v3_hfircoef01;

/* define the union reg_v3_hfircoef23 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int coef2 : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef3 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v3_hfircoef23;

/* define the union reg_v3_hfircoef45 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int coef4 : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef5 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v3_hfircoef45;

/* define the union reg_v3_hfircoef67 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int coef6 : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef7 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v3_hfircoef67;

/* define the union reg_vp0_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vp0_upd;

/* define the union reg_vp0_ireso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int iw : 16; /* [15..0]  */
        unsigned int ih : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vp0_ireso;

/* define the union reg_vp0_lbox_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mute_en : 1;     /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vp0_lbox_ctrl;

/* define the union reg_vp0_galpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int galpha : 8;      /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vp0_galpha;

/* define the union reg_vp0_dfpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int disp_xfpos : 12; /* [11..0]  */
        unsigned int disp_yfpos : 12; /* [23..12]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vp0_dfpos;

/* define the union reg_vp0_dlpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int disp_xlpos : 12; /* [11..0]  */
        unsigned int disp_ylpos : 12; /* [23..12]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vp0_dlpos;

/* define the union reg_vp0_vfpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_xfpos : 12; /* [11..0]  */
        unsigned int video_yfpos : 12; /* [23..12]  */
        unsigned int reserved_0 : 8;   /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vp0_vfpos;

/* define the union reg_vp0_vlpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_xlpos : 12; /* [11..0]  */
        unsigned int video_ylpos : 12; /* [23..12]  */
        unsigned int reserved_0 : 8;   /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vp0_vlpos;

/* define the union reg_vp0_bk */
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
} reg_vp0_bk;

/* define the union reg_vp0_alpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vbk_alpha : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vp0_alpha;

/* define the union reg_vp0_mute_bk */
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
} reg_vp0_mute_bk;

/* define the union reg_g0_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int galpha : 8;       /* [7..0]  */
        unsigned int reserved_0 : 19;  /* [26..8]  */
        unsigned int g0_depremult : 1; /* [27]  */
        unsigned int rupd_field : 1;   /* [28]  */
        unsigned int rgup_mode : 1;    /* [29]  */
        unsigned int nosec_flag : 1;   /* [30]  */
        unsigned int surface_en : 1;   /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_ctrl;

/* define the union reg_g0_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_upd;

/* define the union reg_g0_0reso_read */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0]  */
        unsigned int oh : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_0reso_read;

/* define the union reg_g0_ireso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int iw : 16; /* [15..0]  */
        unsigned int ih : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_ireso;

/* define the union reg_g0_dfpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int disp_xfpos : 16; /* [15..0]  */
        unsigned int disp_yfpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_dfpos;

/* define the union reg_g0_dlpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int disp_xlpos : 16; /* [15..0]  */
        unsigned int disp_ylpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_dlpos;

/* define the union reg_g0_vfpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_xfpos : 16; /* [15..0]  */
        unsigned int video_yfpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_vfpos;

/* define the union reg_g0_vlpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_xlpos : 16; /* [15..0]  */
        unsigned int video_ylpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_vlpos;

/* define the union reg_g0_bk */
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
} reg_g0_bk;

/* define the union reg_g0_alpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vbk_alpha : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_alpha;

/* define the union reg_g0_mute_bk */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mute_cr : 10;   /* [9..0]  */
        unsigned int mute_cb : 10;   /* [19..10]  */
        unsigned int mute_y : 10;    /* [29..20]  */
        unsigned int reserved_0 : 1; /* [30]  */
        unsigned int mute_en : 1;    /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_mute_bk;

/* define the union reg_g0_lbox_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mute_en : 1;     /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_lbox_ctrl;

/* define the union reg_g0_ot_pp_csc_ctrl */
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
} reg_g0_ot_pp_csc_ctrl;

/* define the union reg_g0_ot_pp_csc_coef00 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef00 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_ot_pp_csc_coef00;

/* define the union reg_g0_ot_pp_csc_coef01 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef01 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_ot_pp_csc_coef01;

/* define the union reg_g0_ot_pp_csc_coef02 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef02 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_ot_pp_csc_coef02;

/* define the union reg_g0_ot_pp_csc_coef10 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef10 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_ot_pp_csc_coef10;

/* define the union reg_g0_ot_pp_csc_coef11 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef11 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_ot_pp_csc_coef11;

/* define the union reg_g0_ot_pp_csc_coef12 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef12 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g0_ot_pp_csc_coef12;


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART11_H_ */
