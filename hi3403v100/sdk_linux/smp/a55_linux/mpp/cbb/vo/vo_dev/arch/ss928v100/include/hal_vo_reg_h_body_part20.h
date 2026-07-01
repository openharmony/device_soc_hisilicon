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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART20_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART20_H_

    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_ot_pp_csc2_min_y;

/* define the union reg_g4_ot_pp_csc2_min_c */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_min_c : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_ot_pp_csc2_min_c;

/* define the union reg_g4_ot_pp_csc2_max_y */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_max_y : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_ot_pp_csc2_max_y;

/* define the union reg_g4_ot_pp_csc2_max_c */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc2_max_c : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_ot_pp_csc2_max_c;

/* define the union reg_g4_ot_pp_csc_ink_ctrl */
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
} reg_g4_ot_pp_csc_ink_ctrl;

/* define the union reg_g4_ot_pp_csc_ink_pos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int x_pos : 16; /* [15..0]  */
        unsigned int y_pos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_ot_pp_csc_ink_pos;

/* define the union reg_g4_osb_mute_bk */
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
} reg_g4_osb_mute_bk;

/* define the union reg_g4_osb_bk_alpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int osb_bk_alpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;  /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_osb_bk_alpha;

/* define the union reg_g4_osb_coef_rd_en */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int osb_rd_en : 1;   /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_osb_coef_rd_en;

/* define the union reg_gp0_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_upd;

/* define the union reg_gp0_ireso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int iw : 16; /* [15..0]  */
        unsigned int ih : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_ireso;

/* define the union reg_gp0_lbox_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mute_en : 1;     /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_lbox_ctrl;

/* define the union reg_gp0_galpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int galpha : 8;      /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_galpha;

/* define the union reg_gp0_dfpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int disp_xfpos : 12; /* [11..0]  */
        unsigned int disp_yfpos : 12; /* [23..12]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_dfpos;

/* define the union reg_gp0_dlpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int disp_xlpos : 12; /* [11..0]  */
        unsigned int disp_ylpos : 12; /* [23..12]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_dlpos;

/* define the union reg_gp0_vfpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_xfpos : 12; /* [11..0]  */
        unsigned int video_yfpos : 12; /* [23..12]  */
        unsigned int reserved_0 : 8;   /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_vfpos;

/* define the union reg_gp0_vlpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_xlpos : 12; /* [11..0]  */
        unsigned int video_ylpos : 12; /* [23..12]  */
        unsigned int reserved_0 : 8;   /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_vlpos;

/* define the union reg_gp0_bk */
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
} reg_gp0_bk;

/* define the union reg_gp0_alpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vbk_alpha : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_alpha;

/* define the union reg_gp0_mute_bk */
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
} reg_gp0_mute_bk;

/* define the union reg_gp0_csc_idc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc0 : 11;   /* [10..0]  */
        unsigned int cscidc1 : 11;   /* [21..11]  */
        unsigned int csc_en : 1;     /* [22]  */
        unsigned int reserved_0 : 9; /* [31..23]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_csc_idc;

/* define the union reg_gp0_csc_odc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscodc0 : 11;      /* [10..0]  */
        unsigned int cscodc1 : 11;      /* [21..11]  */
        unsigned int csc_sign_mode : 1; /* [22]  */
        unsigned int reserved_0 : 9;    /* [31..23]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_csc_odc;

/* define the union reg_gp0_csc_iodc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_csc_iodc;

/* define the union reg_gp0_csc_p0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp00 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp01 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_csc_p0;

/* define the union reg_gp0_csc_p1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp02 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp10 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_csc_p1;

/* define the union reg_gp0_csc_p2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp11 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp12 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_csc_p2;

/* define the union reg_gp0_csc_p3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp20 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp21 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_csc_p3;

/* define the union reg_gp0_csc_p4 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp22 : 15;     /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gp0_csc_p4;

/* define the union reg_wbc_g0_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int req_interval : 10; /* [9..0]  */
        unsigned int auto_stop_en : 1;  /* [10]  */
        unsigned int reserved_0 : 15;   /* [25..11]  */
        unsigned int format_out : 2;    /* [27..26]  */
        unsigned int reserved_1 : 3;    /* [30..28]  */
        unsigned int wbc_en : 1;        /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_g0_ctrl;

/* define the union reg_wbc_g0_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_g0_upd;

/* define the union reg_wbc_g0_cmp */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cmp_lossy_en : 1; /* [0]  */
        unsigned int reserved_0 : 3;   /* [3..1]  */
        unsigned int cmp_drr : 4;      /* [7..4]  */
        unsigned int reserved_1 : 23;  /* [30..8]  */
        unsigned int cmp_en : 1;       /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_g0_cmp;

/* define the union reg_wbc_g0_stride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wbcstride : 16;  /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_g0_stride;

/* define the union reg_wbc_g0_oreso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ow : 12;        /* [11..0]  */
        unsigned int oh : 12;        /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_g0_oreso;

/* define the union reg_wbc_g0_fcrop */
typedef union {
    /* define the struct bits */
    struct {


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART20_H_ */
