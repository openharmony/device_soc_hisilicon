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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART21_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART21_H_

        unsigned int wfcrop : 12;    /* [11..0]  */
        unsigned int hfcrop : 12;    /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_g0_fcrop;

/* define the union reg_wbc_g0_lcrop */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wlcrop : 12;    /* [11..0]  */
        unsigned int hlcrop : 12;    /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_g0_lcrop;

/* define the union reg_wbc_gp0_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int req_interval : 10;  /* [9..0]  */
        unsigned int auto_stop_en : 1;   /* [10]  */
        unsigned int reserved_0 : 1;     /* [11]  */
        unsigned int wbc_vtthd_mode : 1; /* [12]  */
        unsigned int reserved_1 : 5;     /* [17..13]  */
        unsigned int three_d_mode : 2;   /* [19..18]  */
        unsigned int reserved_2 : 3;     /* [22..20]  */
        unsigned int flip_en : 1;        /* [23]  */
        unsigned int format_out : 4;     /* [27..24]  */
        unsigned int mode_out : 2;       /* [29..28]  */
        unsigned int reserved_3 : 1;     /* [30]  */
        unsigned int wbc_en : 1;         /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_gp0_ctrl;

/* define the union reg_wbc_gp0_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_gp0_upd;

/* define the union reg_wbc_gp0_stride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wbclstride : 16; /* [15..0]  */
        unsigned int wbccstride : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_gp0_stride;

/* define the union reg_wbc_gp0_oreso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ow : 12;        /* [11..0]  */
        unsigned int oh : 12;        /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_gp0_oreso;

/* define the union reg_wbc_gp0_fcrop */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wfcrop : 12;    /* [11..0]  */
        unsigned int hfcrop : 12;    /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_gp0_fcrop;

/* define the union reg_wbc_gp0_lcrop */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wlcrop : 12;    /* [11..0]  */
        unsigned int hlcrop : 12;    /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_gp0_lcrop;

/* define the union reg_wbc_gp0_dither_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 29;  /* [28..0]  */
        unsigned int dither_round : 1; /* [29]  */
        unsigned int dither_mode : 1;  /* [30]  */
        unsigned int dither_en : 1;    /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_gp0_dither_ctrl;

/* define the union reg_wbc_gp0_dither_coef0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_coef0 : 8; /* [7..0]  */
        unsigned int dither_coef1 : 8; /* [15..8]  */
        unsigned int dither_coef2 : 8; /* [23..16]  */
        unsigned int dither_coef3 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_gp0_dither_coef0;

/* define the union reg_wbc_gp0_dither_coef1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_coef4 : 8; /* [7..0]  */
        unsigned int dither_coef5 : 8; /* [15..8]  */
        unsigned int dither_coef6 : 8; /* [23..16]  */
        unsigned int dither_coef7 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_gp0_dither_coef1;

/* define the union reg_wbc_gp0_hpzme */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 29;  /* [28..0]  */
        unsigned int hpzme_mode : 1;   /* [29]  */
        unsigned int hpzme_mid_en : 1; /* [30]  */
        unsigned int hpzme_en : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_gp0_hpzme;

/* define the union reg_wbc_me_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int req_interval : 10; /* [9..0]  */
        unsigned int reserved_0 : 10;   /* [19..10]  */
        unsigned int ofl_master : 1;    /* [20]  */
        unsigned int reserved_1 : 2;    /* [22..21]  */
        unsigned int mad_data_mode : 1; /* [23]  */
        unsigned int format_out : 4;    /* [27..24]  */
        unsigned int reserved_2 : 1;    /* [28]  */
        unsigned int c_wbc_en : 1;      /* [29]  */
        unsigned int reserved_3 : 1;    /* [30]  */
        unsigned int wbc_en : 1;        /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_ctrl;

/* define the union reg_wbc_me_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_upd;

/* define the union reg_wbc_me_wlen_sel */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wlen_sel : 2;    /* [1..0]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_wlen_sel;

/* define the union reg_wbc_me_stride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wbclstride : 16; /* [15..0]  */
        unsigned int wbccstride : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_stride;

/* define the union reg_wbc_me_oreso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ow : 12;        /* [11..0]  */
        unsigned int oh : 12;        /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_oreso;

/* define the union reg_wbc_me_smmu_bypass */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int l_bypass : 1;    /* [0]  */
        unsigned int c_bypass : 1;    /* [1]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_smmu_bypass;

/* define the union reg_wbc_me_paraup */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wbc_hlcoef_upd : 1; /* [0]  */
        unsigned int wbc_hccoef_upd : 1; /* [1]  */
        unsigned int wbc_vlcoef_upd : 1; /* [2]  */
        unsigned int wbc_vccoef_upd : 1; /* [3]  */
        unsigned int reserved_0 : 28;    /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_paraup;

/* define the union reg_wbc_me_dither_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 29;  /* [28..0]  */
        unsigned int dither_round : 1; /* [29]  */
        unsigned int dither_mode : 1;  /* [30]  */
        unsigned int dither_en : 1;    /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_dither_ctrl;

/* define the union reg_wbc_me_dither_coef0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_coef0 : 8; /* [7..0]  */
        unsigned int dither_coef1 : 8; /* [15..8]  */
        unsigned int dither_coef2 : 8; /* [23..16]  */
        unsigned int dither_coef3 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_dither_coef0;

/* define the union reg_wbc_me_dither_coef1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_coef4 : 8; /* [7..0]  */
        unsigned int dither_coef5 : 8; /* [15..8]  */
        unsigned int dither_coef6 : 8; /* [23..16]  */
        unsigned int dither_coef7 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_dither_coef1;

/* define the union reg_wbc_me_zme_hsp */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hratio : 24;    /* [23..0]  */
        unsigned int hfir_order : 1; /* [24]  */
        unsigned int hchfir_en : 1;  /* [25]  */
        unsigned int hlfir_en : 1;   /* [26]  */
        unsigned int reserved_0 : 1; /* [27]  */
        unsigned int hchmid_en : 1;  /* [28]  */
        unsigned int hlmid_en : 1;   /* [29]  */
        unsigned int hchmsc_en : 1;  /* [30]  */
        unsigned int hlmsc_en : 1;   /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_zme_hsp;

/* define the union reg_wbc_me_zme_hloffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hor_loffset : 28; /* [27..0]  */
        unsigned int reserved_0 : 4;   /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_zme_hloffset;

/* define the union reg_wbc_me_zme_hcoffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hor_coffset : 28; /* [27..0]  */
        unsigned int reserved_0 : 4;   /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_zme_hcoffset;

/* define the union reg_wbc_me_zme_vsp */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 19; /* [18..0]  */
        unsigned int zme_in_fmt : 2;  /* [20..19]  */
        unsigned int zme_out_fmt : 2; /* [22..21]  */
        unsigned int vchfir_en : 1;   /* [23]  */
        unsigned int vlfir_en : 1;    /* [24]  */
        unsigned int reserved_1 : 3;  /* [27..25]  */
        unsigned int vchmid_en : 1;   /* [28]  */
        unsigned int vlmid_en : 1;    /* [29]  */
        unsigned int vchmsc_en : 1;   /* [30]  */
        unsigned int vlmsc_en : 1;    /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_zme_vsp;

/* define the union reg_wbc_me_zme_vsr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vratio : 16;     /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_zme_vsr;

/* define the union reg_wbc_me_zme_voffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vchroma_offset : 16; /* [15..0]  */
        unsigned int vluma_offset : 16;   /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_zme_voffset;

/* define the union reg_wbc_me_zme_vboffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vbchroma_offset : 16; /* [15..0]  */
        unsigned int vbluma_offset : 16;   /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_me_zme_vboffset;

/* define the union reg_wbc_fi_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int req_interval : 10; /* [9..0]  */
        unsigned int reserved_0 : 3;    /* [12..10]  */
        unsigned int addr_mode : 1;     /* [13]  */
        unsigned int fsize_mode : 1;    /* [14]  */
        unsigned int tnr_nrds_en : 1;   /* [15]  */
        unsigned int reserved_1 : 4;    /* [19..16]  */
        unsigned int ofl_master : 1;    /* [20]  */
        unsigned int data_width : 1;    /* [21]  */
        unsigned int reserved_2 : 2;    /* [23..22]  */
        unsigned int format_out : 4;    /* [27..24]  */
        unsigned int reserved_3 : 2;    /* [29..28]  */
        unsigned int cmp_en : 1;        /* [30]  */
        unsigned int wbc_en : 1;        /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_wbc_fi_ctrl;

/* define the union reg_wbc_fi_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART21_H_ */
