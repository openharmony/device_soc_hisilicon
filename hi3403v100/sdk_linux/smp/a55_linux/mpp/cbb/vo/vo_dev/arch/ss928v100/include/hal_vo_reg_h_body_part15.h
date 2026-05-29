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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART15_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART15_H_

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_osb_mute_bk;

/* define the union reg_g1_osb_bk_alpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int osb_bk_alpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;  /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_osb_bk_alpha;

/* define the union reg_g1_osb_coef_rd_en */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int osb_rd_en : 1;   /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_osb_coef_rd_en;

/* define the union reg_g1_zme_hinfo */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int out_width : 16;  /* [15..0]  */
        unsigned int ck_gt_en : 1;    /* [16]  */
        unsigned int reserved_0 : 15; /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_zme_hinfo;

/* define the union reg_g1_zme_hsp */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hratio : 24;      /* [23..0]  */
        unsigned int hfir_order : 1;   /* [24]  */
        unsigned int ahfir_mode : 1;   /* [25]  */
        unsigned int lhfir_mode : 1;   /* [26]  */
        unsigned int reserved_0 : 1;   /* [27]  */
        unsigned int chfir_mid_en : 1; /* [28]  */
        unsigned int lhfir_mid_en : 1; /* [29]  */
        unsigned int ahfir_mid_en : 1; /* [30]  */
        unsigned int hfir_en : 1;      /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_zme_hsp;

/* define the union reg_g1_zme_hloffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lhfir_offset : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;    /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_zme_hloffset;

/* define the union reg_g1_zme_hcoffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int chfir_offset : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;    /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_zme_hcoffset;

/* define the union reg_g1_zme_coef_ren */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 1;     /* [0]  */
        unsigned int apb_g1_vf_lren : 1; /* [1]  */
        unsigned int reserved_1 : 1;     /* [2]  */
        unsigned int apb_g1_hf_lren : 1; /* [3]  */
        unsigned int reserved_2 : 28;    /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_zme_coef_ren;

/* define the union reg_g1_zme_coef_rdata */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int apb_vhd_coef_raddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;        /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_zme_coef_rdata;

/* define the union reg_g1_zme_vinfo */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int out_height : 16; /* [15..0]  */
        unsigned int reserved_0 : 2;  /* [17..16]  */
        unsigned int out_pro : 1;     /* [18]  */
        unsigned int reserved_1 : 13; /* [31..19]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_zme_vinfo;

/* define the union reg_g1_zme_vsp */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vratio : 16;      /* [15..0]  */
        unsigned int reserved_0 : 9;   /* [24..16]  */
        unsigned int vafir_mode : 1;   /* [25]  */
        unsigned int lvfir_mode : 1;   /* [26]  */
        unsigned int reserved_1 : 1;   /* [27]  */
        unsigned int cvfir_mid_en : 1; /* [28]  */
        unsigned int lvfir_mid_en : 1; /* [29]  */
        unsigned int avfir_mid_en : 1; /* [30]  */
        unsigned int vfir_en : 1;      /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_zme_vsp;

/* define the union reg_g1_zme_voffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vbtm_offset : 16; /* [15..0]  */
        unsigned int vtp_offset : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_zme_voffset;

/* define the union reg_g2_ctrl */
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
} reg_g2_ctrl;

/* define the union reg_g2_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_upd;

/* define the union reg_g2_0reso_read */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0]  */
        unsigned int oh : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_0reso_read;

/* define the union reg_g2_ireso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int iw : 16; /* [15..0]  */
        unsigned int ih : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_ireso;

/* define the union reg_g2_dfpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int disp_xfpos : 16; /* [15..0]  */
        unsigned int disp_yfpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_dfpos;

/* define the union reg_g2_dlpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int disp_xlpos : 16; /* [15..0]  */
        unsigned int disp_ylpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_dlpos;

/* define the union reg_g2_vfpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_xfpos : 16; /* [15..0]  */
        unsigned int video_yfpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_vfpos;

/* define the union reg_g2_vlpos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int video_xlpos : 16; /* [15..0]  */
        unsigned int video_ylpos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_vlpos;

/* define the union reg_g2_bk */
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
} reg_g2_bk;

/* define the union reg_g2_alpha */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vbk_alpha : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_alpha;

/* define the union reg_g2_mute_bk */
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
} reg_g2_mute_bk;

/* define the union reg_g2_lbox_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mute_en : 1;     /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_lbox_ctrl;

/* define the union reg_g2_ot_pp_csc_ctrl */
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
} reg_g2_ot_pp_csc_ctrl;

/* define the union reg_g2_ot_pp_csc_coef00 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef00 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_ot_pp_csc_coef00;

/* define the union reg_g2_ot_pp_csc_coef01 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef01 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_ot_pp_csc_coef01;

/* define the union reg_g2_ot_pp_csc_coef02 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef02 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_ot_pp_csc_coef02;

/* define the union reg_g2_ot_pp_csc_coef10 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef10 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_ot_pp_csc_coef10;

/* define the union reg_g2_ot_pp_csc_coef11 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef11 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_ot_pp_csc_coef11;

/* define the union reg_g2_ot_pp_csc_coef12 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef12 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_ot_pp_csc_coef12;

/* define the union reg_g2_ot_pp_csc_coef20 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef20 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_ot_pp_csc_coef20;

/* define the union reg_g2_ot_pp_csc_coef21 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef21 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_ot_pp_csc_coef21;

/* define the union reg_g2_ot_pp_csc_coef22 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ot_pp_csc_coef22 : 12; /* [11..0]  */
        unsigned int reserved_0 : 20;      /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g2_ot_pp_csc_coef22;


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART15_H_ */
