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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART02_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART02_H_


typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 des_ch1_stride : 20; /* [19..0]  */
        td_u32 reserved : 12;       /* [31..20]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_ch1_stride;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 des_width : 16;  /* [15..0]  */
        td_u32 des_height : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_imgsize;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 des_alpha_thd : 8; /* [7..0]  */
        td_u32 des_crop_mode : 1; /* [8]  */
        td_u32 des_crop_en : 1;   /* [9]  */
        td_u32 reserved_0 : 22;   /* [31..10]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_alpha;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 des_crop_start_x : 16; /* [15..0]  */
        td_u32 des_crop_start_y : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_crop_pos_st;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 des_crop_end_x : 16; /* [15..0]  */
        td_u32 des_crop_end_y : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_crop_pos_ed;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src1_ch0_mmu_bypass : 1; /* [0]  */
        td_u32 src1_ch1_mmu_bypass : 1; /* [1]  */
        td_u32 src2_ch0_mmu_bypass : 1; /* [2]  */
        td_u32 src2_ch1_mmu_bypass : 1; /* [3]  */
        td_u32 gdc3_mmu_bypass : 1;     /* [4]  */
        td_u32 gdc4_mmu_bypass : 1;     /* [5]  */
        td_u32 gdc5_mmu_bypass : 1;     /* [6]  */
        td_u32 gdc6_mmu_bypass : 1;     /* [7]  */
        td_u32 gdc7_mmu_bypass : 1;     /* [8]  */
        td_u32 clut_mmu_bypass : 1;     /* [9]  */
        td_u32 reserved_0 : 6;          /* [15..10]  */
        td_u32 src1_ch0_prot : 1;       /* [16]  */
        td_u32 src1_ch1_prot : 1;       /* [17]  */
        td_u32 src2_ch0_prot : 1;       /* [18]  */
        td_u32 src2_ch1_prot : 1;       /* [19]  */
        td_u32 gdc3_prot : 1;           /* [20]  */
        td_u32 gdc4_prot : 1;           /* [21]  */
        td_u32 gdc5_prot : 1;           /* [22]  */
        td_u32 gdc6_prot : 1;           /* [23]  */
        td_u32 gdc7_prot : 1;           /* [24]  */
        td_u32 reserved_1 : 7;          /* [31..25]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_mmu_prot_ctrl;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 des_hoffset_pix : 16; /* [15..0]  */
        td_u32 reserved_0 : 16;      /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_pix_offset;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 dma_corner_width : 16; /* [15..0]  */
        td_u32 dma_corner_height : 16;      /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_dma_corner_reso;

#ifdef CONFIG_TDE_DMA_CORNER_V1
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 dma_corner_value : 4; /* [3..0]  */
        td_u32 dma_other_value : 4;      /* [7..4]  */
        td_u32 reserved_0 : 22;      /* [29..8]  */
        td_u32 dma_des_en : 1;      /* [30]  */
        td_u32 dma_corner_en : 1;      /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_dma_corner_ctrl;
#endif

#ifdef CONFIG_TDE_DMA_CORNER_V2
typedef union {
    /* Define the struct bits */
    struct {
        td_u32 reserved : 30;      /* [29..0]  */
        td_u32 dma_des_en : 1;      /* [30]  */
        td_u32 dma_corner_en : 1;      /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_dma_corner_ctrl;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 dma_corner_value : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_dma_corner_value;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 dma_other_value : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_tde_dma_other_value;
#endif

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 des_safe_dist : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_safe_dist;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 des_safe_dist_inverse : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_safe_dist_inverse;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 des_bind_buffer_size : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_bind_buffer_size;

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
} u_src1_hsp;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 hor_loffset : 28; /* [27..0]  */
        td_u32 reserved_0 : 4;   /* [31..28]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_hloffset;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 hor_coffset : 28; /* [27..0]  */
        td_u32 reserved_0 : 4;   /* [31..28]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_hcoffset;

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
} u_src1_vsp;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 vratio : 16;     /* [15..0]  */
        td_u32 reserved_0 : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_vsr;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 vchroma_offset : 16; /* [15..0]  */
        td_u32 vluma_offset : 16;   /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_voffset;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 ow : 16; /* [15..0]  */
        td_u32 oh : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_zmeoreso;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 iw : 16; /* [15..0]  */
        td_u32 ih : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_zmeireso;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src1_crop_x : 13; /* [12..0]  */
        td_u32 reserved_0 : 3;   /* [15..13]  */
        td_u32 src1_crop_y : 13; /* [28..16]  */
        td_u32 reserved_1 : 2;   /* [30..29]  */
        td_u32 src1_crop_en : 1; /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_crop_pos;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src1_crop_width : 13;  /* [12..0]  */
        td_u32 reserved_0 : 3;        /* [15..13]  */
        td_u32 src1_crop_height : 13; /* [28..16]  */
        td_u32 reserved_1 : 3;        /* [31..29]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_crop_size;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src1_hpzme_en : 1;   /* [0]  */
        td_u32 reserved_0 : 3;      /* [3..1]  */
        td_u32 src1_hpzme_mode : 4; /* [7..4]  */
        td_u32 reserved_1 : 24;     /* [31..8]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_hpzme;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src1_hpzme_width : 16; /* [15..0]  */
        td_u32 reserved_0 : 16;       /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_hpzme_size;

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
} u_src1_csc_idc0;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 cscidc2 : 11;    /* [10..0]  */
        td_u32 reserved_0 : 20; /* [31..11]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_csc_idc1;

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
} u_src1_csc_odc0;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 cscodc2 : 11;    /* [10..0]  */
        td_u32 reserved_0 : 20; /* [31..11]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_csc_odc1;

/* Define the union U_SRC1_CSC_P0 */
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
} u_src1_csc_p0;

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
} u_src1_csc_p1;

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
} u_src1_csc_p2;

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
} u_src1_csc_p3;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 cscp22 : 15;     /* [14..0]  */
        td_u32 reserved_0 : 17; /* [31..15]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_csc_p4;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 reserved_0 : 29;  /* [28..0]  */
        td_u32 dither_round : 1; /* [29]  */
        td_u32 reserved_1 : 1;   /* [30]  */
        td_u32 dither_en : 1;    /* [31]  */


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART02_H_ */
