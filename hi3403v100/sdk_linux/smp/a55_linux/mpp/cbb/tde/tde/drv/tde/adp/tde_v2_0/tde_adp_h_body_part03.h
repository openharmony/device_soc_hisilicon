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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART03_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART03_H_

    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_dither_ctrl;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 hratio : 24;    /* [23..0]  */
        td_u32 hfir_order : 1; /* [24]  */
        td_u32 hchfir_en : 1;  /* [25]  */
        td_u32 hlfir_en : 1;   /* [26]  */
        td_u32 hafir_en : 1;   /* [27]  */
        td_u32 hchmid_en : 1;  /* [28]  */
        td_u32 hlmid_en : 1;   /* [29]  */
        td_u32 hchmsc_en : 1;  /* [30]  */
        td_u32 hlmsc_en : 1;   /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_hsp;

typedef union {
    /* Define the struct bits */
    struct {
        td_s32 hor_loffset : 28; /* [27..0]  */
        td_u32 reserved_0 : 4;   /* [31..28]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_hloffset;

typedef union {
    /* Define the struct bits */
    struct {
        td_s32 hor_coffset : 28; /* [27..0]  */
        td_u32 reserved_0 : 4;   /* [31..28]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_hcoffset;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 reserved_0 : 19;    /* [18..0]  */
        td_u32 zme_in_fmt : 2;     /* [20..19]  */
        td_u32 zme_out_fmt : 2;    /* [22..21]  */
        td_u32 vchfir_en : 1;      /* [23]  */
        td_u32 vlfir_en : 1;       /* [24]  */
        td_u32 vafir_en : 1;       /* [25]  */
        td_u32 vsc_chroma_tap : 1; /* [26]  */
        td_u32 reserved_1 : 1;     /* [27]  */
        td_u32 vchmid_en : 1;      /* [28]  */
        td_u32 vlmid_en : 1;       /* [29]  */
        td_u32 vchmsc_en : 1;      /* [30]  */
        td_u32 vlmsc_en : 1;       /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_vsp;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 vratio : 16;     /* [15..0]  */
        td_u32 reserved_0 : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_vsr;

typedef union {
    /* Define the struct bits */
    struct {
        td_s32 vchroma_offset : 16; /* [15..0]  */
        td_s32 vluma_offset : 16;   /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_voffset;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 ow : 16; /* [15..0]  */
        td_u32 oh : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_zmeoreso;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 iw : 16; /* [15..0]  */
        td_u32 ih : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_zmeireso;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src2_crop_x : 13; /* [12..0]  */
        td_u32 reserved_0 : 3;   /* [15..13]  */
        td_u32 src2_crop_y : 13; /* [28..16]  */
        td_u32 reserved_1 : 2;   /* [30..29]  */
        td_u32 src2_crop_en : 1; /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_crop_pos;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src2_crop_width : 13;  /* [12..0]  */
        td_u32 reserved_0 : 3;        /* [15..13]  */
        td_u32 src2_crop_height : 13; /* [28..16]  */
        td_u32 reserved_1 : 3;        /* [31..29]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_crop_size;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src2_hpzme_en : 1;   /* [0]  */
        td_u32 reserved_0 : 3;      /* [3..1]  */
        td_u32 src2_hpzme_mode : 4; /* [7..4]  */
        td_u32 reserved_1 : 24;     /* [31..8]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_hpzme;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src2_hpzme_width : 16; /* [15..0]  */
        td_u32 reserved_0 : 16;       /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_hpzme_size;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src2_csc_mode : 1; /* [0]  */
        td_u32 des_premulten : 1; /* [1]  */
        td_u32 src_premulten : 1; /* [2]  */
        td_u32 reserved_0 : 29;   /* [31..3]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_csc_mux;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 cscidc0 : 11;   /* [10..0]  */
        td_u32 reserved_0 : 5; /* [15..11]  */
        td_u32 cscidc1 : 11;   /* [26..16]  */
        td_u32 reserved_1 : 4; /* [30..27]  */
        td_u32 csc_en : 1;     /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_csc_idc0;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 cscidc2 : 11;    /* [10..0]  */
        td_u32 reserved_0 : 20; /* [31..11]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_csc_idc1;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 cscodc0 : 11;   /* [10..0]  */
        td_u32 reserved_0 : 5; /* [15..11]  */
        td_u32 cscodc1 : 11;   /* [26..16]  */
        td_u32 reserved_1 : 5; /* [31..27]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_csc_odc0;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 cscodc2 : 11;    /* [10..0]  */
        td_u32 reserved_0 : 20; /* [31..11]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_csc_odc1;

/* Define the union U_DES_CSC_P0 */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 cscp00 : 15;    /* [14..0]  */
        td_u32 reserved_0 : 1; /* [15]  */
        td_u32 cscp01 : 15;    /* [30..16]  */
        td_u32 reserved_1 : 1; /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_csc_p0;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 cscp02 : 15;    /* [14..0]  */
        td_u32 reserved_0 : 1; /* [15]  */
        td_u32 cscp10 : 15;    /* [30..16]  */
        td_u32 reserved_1 : 1; /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_csc_p1;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 cscp11 : 15;    /* [14..0]  */
        td_u32 reserved_0 : 1; /* [15]  */
        td_u32 cscp12 : 15;    /* [30..16]  */
        td_u32 reserved_1 : 1; /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_csc_p2;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 cscp20 : 15;    /* [14..0]  */
        td_u32 reserved_0 : 1; /* [15]  */
        td_u32 cscp21 : 15;    /* [30..16]  */
        td_u32 reserved_1 : 1; /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_csc_p3;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 cscp22 : 15;     /* [14..0]  */
        td_u32 reserved_0 : 17; /* [31..15]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_csc_p4;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 reserved_0 : 29;  /* [28..0]  */
        td_u32 dither_round : 1; /* [29]  */
        td_u32 reserved_1 : 1;   /* [30]  */
        td_u32 dither_en : 1;    /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_dither_ctrl;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 des_h_dswm_mode : 2;        /* [1..0]  */
        td_u32 reserved_0 : 2;             /* [3..2]  */
        td_u32 des_v_dswm_mode : 1;        /* [4]  */
        td_u32 des_alpha_detect_clear : 1; /* [5]  */
        td_u32 reserved_1 : 26;            /* [31..6]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_dswm;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 reserved_0 : 8;           /* [7..0]  */
        td_u32 src1_rcopy_pixel_num : 8; /* [15..8]  */
        td_u32 reserved_1 : 14;          /* [29..16]  */
        td_u32 src1_rcopy_en : 1;        /* [30]  */
        td_u32 src1_copy_cfg_from : 1;   /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_befor_zme_copy;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 reserved_0 : 8;           /* [7..0]  */
        td_u32 src1_rcrop_pixel_num : 8; /* [15..8]  */
        td_u32 reserved_1 : 14;          /* [29..16]  */
        td_u32 src1_rcrop_en : 1;        /* [30]  */
        td_u32 src1_crop_cfg_from : 1;   /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_after_zme_crop;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 reserved_0 : 8;           /* [7..0]  */
        td_u32 src2_rcopy_pixel_num : 8; /* [15..8]  */
        td_u32 reserved_1 : 14;          /* [29..16]  */
        td_u32 src2_rcopy_en : 1;        /* [30]  */
        td_u32 src2_copy_cfg_from : 1;   /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_befor_zme_copy;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 reserved_0 : 8;           /* [7..0]  */
        td_u32 src2_rcrop_pixel_num : 8; /* [15..8]  */
        td_u32 reserved_1 : 14;          /* [29..16]  */
        td_u32 src2_rcrop_en : 1;        /* [30]  */
        td_u32 src2_crop_cfg_from : 1;   /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_after_zme_crop;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 mix_prio0 : 3;  /* [2..0]  */
        td_u32 mix_prio1 : 3;  /* [5..3]  */
        td_u32 mix_prio2 : 3;  /* [8..6]  */
        td_u32 mix_prio3 : 3;  /* [11..9]  */
        td_u32 mix_prio4 : 3;  /* [14..12]  */
        td_u32 mix_prio5 : 3;  /* [17..15]  */
        td_u32 mix_prio6 : 3;  /* [20..18]  */
        td_u32 alu_mode : 4;   /* [24..21]  */
        td_u32 cbm_mode : 1;   /* [25]  */
        td_u32 reserved_0 : 5; /* [30..26]  */
        td_u32 cbm_en : 1;     /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_cbmctrl;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 bkgb : 8; /* [7..0]  */
        td_u32 bkgg : 8; /* [15..8]  */
        td_u32 bkgr : 8; /* [23..16]  */
        td_u32 bkga : 8; /* [31..24]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_cbmbkg;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 colorizeb : 8;  /* [7..0]  */
        td_u32 colorizeg : 8;  /* [15..8]  */
        td_u32 colorizer : 8;  /* [23..16]  */
        td_u32 reserved_0 : 7; /* [30..24]  */
        td_u32 colorizeen : 1; /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_cbmcolorize;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 rgb_rop : 4;         /* [3..0]  */
        td_u32 a_rop : 4;           /* [7..4]  */
        td_u32 reserved_0 : 8;      /* [15..8]  */
        td_u32 alpha_from : 2;      /* [17..16]  */
        td_u32 alpha_border_en : 2; /* [19..18]  */
        td_u32 reserved_1 : 11;     /* [30..20]  */
        td_u32 blendropen : 1;      /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_cbmalupara;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 keybmode : 2;    /* [1..0]  */
        td_u32 keygmode : 2;    /* [3..2]  */
        td_u32 keyrmode : 2;    /* [5..4]  */
        td_u32 keyamode : 2;    /* [7..6]  */
        td_u32 keysel : 2;      /* [9..8]  */
        td_u32 reserved_0 : 21; /* [30..10]  */
        td_u32 keyen : 1;       /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_cbmkeypara;

typedef union {
    /* Define the struct bits */
    struct {
        td_u8 keybmin : 8; /* [7..0]  */
        td_u8 keygmin : 8; /* [15..8]  */
        td_u8 keyrmin : 8; /* [23..16]  */
        td_u8 keyamin : 8; /* [31..24]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_cbmkeymin;

typedef union {
    /* Define the struct bits */


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART03_H_ */
