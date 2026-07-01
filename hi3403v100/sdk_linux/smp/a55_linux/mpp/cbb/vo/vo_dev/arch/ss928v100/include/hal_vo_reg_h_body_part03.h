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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART03_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART03_H_

        unsigned int para_up_vsd_chn07 : 1; /* [7]  */
        unsigned int reserved_0 : 24;       /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_para_up_vsd;

/* define the union reg_para_conflict_clr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int para_conflict_clr_hd : 1; /* [0]  */
        unsigned int para_conflict_clr_sd : 1; /* [1]  */
        unsigned int reserved_0 : 30;          /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_para_conflict_clr;

/* define the union reg_para_conflict_sta */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int para_conflict_hd : 1; /* [0]  */
        unsigned int para_conflict_sd : 1; /* [1]  */
        unsigned int reserved_0 : 30;      /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_para_conflict_sta;

/* define the union reg_v0_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int galpha : 8;      /* [7..0]  */
        unsigned int mir_en : 1;      /* [8]  */
        unsigned int reserved_0 : 19; /* [27..9]  */
        unsigned int rupd_field : 1;  /* [28]  */
        unsigned int rgup_mode : 1;   /* [29]  */
        unsigned int nosec_flag : 1;  /* [30]  */
        unsigned int surface_en : 1;  /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ctrl;

/* define the union reg_v0_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_upd;

/* define the union reg_v0_0reso_read */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0]  */
        unsigned int oh : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_0reso_read;

/* define the union reg_v0_ireso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int iw : 16; /* [15..0]  */
        unsigned int ih : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ireso;

/* define the union reg_v0_dfpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int disp_xfpos : 16; /* [15..0]  */
        unsigned int disp_yfpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_dfpos;

/* define the union reg_v0_dlpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int disp_xlpos : 16; /* [15..0]  */
        unsigned int disp_ylpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_dlpos;

/* define the union reg_v0_vfpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_xfpos : 16; /* [15..0]  */
        unsigned int video_yfpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_vfpos;

/* define the union reg_v0_vlpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_xlpos : 16; /* [15..0]  */
        unsigned int video_ylpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_vlpos;

/* define the union reg_v0_bk */
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
} reg_v0_bk;

/* define the union reg_v0_alpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vbk_alpha : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_alpha;

/* define the union reg_v0_mute_bk */
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
} reg_v0_mute_bk;

/* define the union reg_v0_rimwidth */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int v0_rim_width : 5; /* [4..0]  */
        unsigned int reserved_0 : 27;  /* [31..5]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_rimwidth;

/* define the union reg_v0_rimcol0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int v0_rim_v0 : 10; /* [9..0]  */
        unsigned int v0_rim_u0 : 10; /* [19..10]  */
        unsigned int v0_rim_y0 : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_rimcol0;

/* define the union reg_v0_rimcol1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int v0_rim_v1 : 10; /* [9..0]  */
        unsigned int v0_rim_u1 : 10; /* [19..10]  */
        unsigned int v0_rim_y1 : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_rimcol1;

/* define the union reg_v0_ot_pp_csc_ctrl */
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
} reg_v0_ot_pp_csc_ctrl;

/* define the union reg_v0_ot_pp_csc_coef00 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef00 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_coef00;

/* define the union reg_v0_ot_pp_csc_coef01 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef01 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_coef01;

/* define the union reg_v0_ot_pp_csc_coef02 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef02 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_coef02;

/* define the union reg_v0_ot_pp_csc_coef10 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef10 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_coef10;

/* define the union reg_v0_ot_pp_csc_coef11 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef11 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_coef11;

/* define the union reg_v0_ot_pp_csc_coef12 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef12 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_coef12;

/* define the union reg_v0_ot_pp_csc_coef20 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef20 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_coef20;

/* define the union reg_v0_ot_pp_csc_coef21 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef21 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_coef21;

/* define the union reg_v0_ot_pp_csc_coef22 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef22 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_coef22;

/* define the union reg_v0_ot_pp_csc_scale */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_scale : 4; /* [3..0]  */
        unsigned int reserved_0 : 28;    /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_scale;

/* define the union reg_v0_ot_pp_csc_idc0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_idc0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;    /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_idc0;

/* define the union reg_v0_ot_pp_csc_idc1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_idc1 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;    /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_idc1;

/* define the union reg_v0_ot_pp_csc_idc2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_idc2 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;    /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_idc2;

/* define the union reg_v0_ot_pp_csc_odc0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_odc0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;    /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_odc0;

/* define the union reg_v0_ot_pp_csc_odc1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_odc1 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;    /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_odc1;

/* define the union reg_v0_ot_pp_csc_odc2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_odc2 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;    /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_ot_pp_csc_odc2;

/* define the union reg_v0_ot_pp_csc_min_y */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_min_y : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;     /* [31..10]  */
    } bits;


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART03_H_ */
