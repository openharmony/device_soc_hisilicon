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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART20_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART20_H_

        unsigned int ow : 12;        /* [11..0]  */
        unsigned int oh : 12;        /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_oreso;

/* Define the union u_wbc_me_smmu_bypass */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int l_bypass : 1;    /* [0]  */
        unsigned int c_bypass : 1;    /* [1]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_smmu_bypass;

/* Define the union u_wbc_me_paraup */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbc_hlcoef_upd : 1; /* [0]  */
        unsigned int wbc_hccoef_upd : 1; /* [1]  */
        unsigned int wbc_vlcoef_upd : 1; /* [2]  */
        unsigned int wbc_vccoef_upd : 1; /* [3]  */
        unsigned int reserved_0 : 28;    /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_paraup;

/* Define the union u_wbc_me_dither_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 29;  /* [28..0]  */
        unsigned int dither_round : 1; /* [29]  */
        unsigned int dither_mode : 1;  /* [30]  */
        unsigned int dither_en : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_dither_ctrl;

/* Define the union u_wbc_me_dither_coef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_coef0 : 8; /* [7..0]  */
        unsigned int dither_coef1 : 8; /* [15..8]  */
        unsigned int dither_coef2 : 8; /* [23..16]  */
        unsigned int dither_coef3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_dither_coef0;

/* Define the union u_wbc_me_dither_coef1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_coef4 : 8; /* [7..0]  */
        unsigned int dither_coef5 : 8; /* [15..8]  */
        unsigned int dither_coef6 : 8; /* [23..16]  */
        unsigned int dither_coef7 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_dither_coef1;

/* Define the union u_wbc_me_zme_hsp */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_zme_hsp;

/* Define the union u_wbc_me_zme_hloffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hor_loffset : 28; /* [27..0]  */
        unsigned int reserved_0 : 4;   /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_zme_hloffset;

/* Define the union u_wbc_me_zme_hcoffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hor_coffset : 28; /* [27..0]  */
        unsigned int reserved_0 : 4;   /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_zme_hcoffset;

/* Define the union u_wbc_me_zme_vsp */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_zme_vsp;

/* Define the union u_wbc_me_zme_vsr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vratio : 16;     /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_zme_vsr;

/* Define the union u_wbc_me_zme_voffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vchroma_offset : 16; /* [15..0]  */
        unsigned int vluma_offset : 16;   /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_zme_voffset;

/* Define the union u_wbc_me_zme_vboffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbchroma_offset : 16; /* [15..0]  */
        unsigned int vbluma_offset : 16;   /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_me_zme_vboffset;

/* Define the union u_wbc_fi_ctrl */
typedef union {
    /* Define the struct bits */
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

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_ctrl;

/* Define the union u_wbc_fi_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_upd;

/* Define the union u_wbc_fi_wlen_sel */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wlen_sel : 2;    /* [1..0]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_wlen_sel;

/* Define the union u_wbc_fi_stride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbclstride : 16; /* [15..0]  */
        unsigned int wbccstride : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_stride;

/* Define the union u_wbc_fi_oreso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 12;        /* [11..0]  */
        unsigned int oh : 12;        /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_oreso;

/* Define the union u_wbc_fi_smmu_bypass */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int l_bypass : 1;    /* [0]  */
        unsigned int c_bypass : 1;    /* [1]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_smmu_bypass;

/* Define the union u_wbc_fi_frame_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_size : 23; /* [22..0]  */
        unsigned int reserved_0 : 9;  /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_frame_size;

/* Define the union u_wbc_fi_hcds */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 29; /* [28..0]  */
        unsigned int hchfir_en : 1;   /* [29]  */
        unsigned int hchmid_en : 1;   /* [30]  */
        unsigned int hcds_en : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_hcds;

/* Define the union u_wbc_fi_hcds_coef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int coef0 : 10;     /* [9..0]  */
        unsigned int coef1 : 10;     /* [19..10]  */
        unsigned int coef2 : 10;     /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_hcds_coef0;

/* Define the union u_wbc_fi_hcds_coef1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int coef3 : 10;      /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_hcds_coef1;

/* Define the union u_wbc_fi_cmp_mb */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mb_bits : 10;    /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_cmp_mb;

/* Define the union u_wbc_fi_cmp_max_min */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int min_bits_cnt : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;    /* [15..10]  */
        unsigned int max_bits_cnt : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;    /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_cmp_max_min;

/* Define the union u_wbc_fi_cmp_adj_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int adj_sad_thr : 12;     /* [11..0]  */
        unsigned int reserved_0 : 4;       /* [15..12]  */
        unsigned int adj_sad_bit_thr : 8;  /* [23..16]  */
        unsigned int adj_spec_bit_thr : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_cmp_adj_thr;

/* Define the union u_wbc_fi_cmp_big_grad */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int big_grad_thr : 7;     /* [6..0]  */
        unsigned int reserved_0 : 1;       /* [7]  */
        unsigned int big_grad_num_thr : 5; /* [12..8]  */
        unsigned int reserved_1 : 19;      /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_cmp_big_grad;

/* Define the union u_wbc_fi_cmp_blk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int smth_thr : 6;     /* [5..0]  */
        unsigned int reserved_0 : 2;   /* [7..6]  */
        unsigned int blk_comp_thr : 3; /* [10..8]  */
        unsigned int reserved_1 : 21;  /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_cmp_blk;

/* Define the union u_wbc_fi_cmp_graphic_judge */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int graphic_en : 1;     /* [0]  */
        unsigned int reserved_0 : 15;    /* [15..1]  */
        unsigned int video_sad_thr : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_cmp_graphic_judge;

/* Define the union u_wbc_fi_cmp_rc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int sadbits_ngain : 3; /* [2..0]  */
        unsigned int reserved_0 : 5;    /* [7..3]  */
        unsigned int rc_smth_gain : 3;  /* [10..8]  */
        unsigned int reserved_1 : 5;    /* [15..11]  */
        unsigned int max_trow_bits : 6; /* [21..16]  */
        unsigned int reserved_2 : 10;   /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_cmp_rc;

/* Define the union u_wbc_fi_cmp_frame_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_size : 21; /* [20..0]  */
        unsigned int reserved_0 : 11; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wbc_fi_cmp_frame_size;

/* Define the union u_wbc_cmp_glb_info */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int is_lossless : 1; /* [0]  */
        unsigned int cmp_mode : 1;    /* [1]  */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART20_H_ */
