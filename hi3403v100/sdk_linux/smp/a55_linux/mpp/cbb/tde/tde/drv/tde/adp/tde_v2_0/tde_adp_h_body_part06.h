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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART06_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART06_H_

    /* Define the struct bits  */
    struct {
        td_u32 debug3 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_debug3;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 debug4 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_debug4;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 debug5 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_debug5;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 debug6 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_debug6;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 debug7 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_debug7;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 debug8 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_debug8;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 debug9 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_debug9;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 debug10 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_debug10;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 debug11 : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 u32;
} u_tde_debug11;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cmp1_frame_size_reg : 22; /* [21..0]  */
        unsigned int reserved_0 : 10;          /* [31..22]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_bs_size_cmp1;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cmp1_max_frm_row_len : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;           /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_worst_row_cmp1;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cmp1_min_frm_row_len : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;           /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_best_row_cmp1;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cmp1_max_gap_bw_row_len_cnt : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;                  /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_stat_info_cmp1;

typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int cmp1_glb_st : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_debug0_cmp1;

typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int cmp1_bitsmux_st : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_debug1_cmp1;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int special_max_row_len : 16;      /* [15..0]  */
        unsigned int cmp1_special_max_row_len : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_od_pic_osd_special_worst_row_cmp1;

#ifdef CONFIG_TDE_DRD_LINE_SUPPORT
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line0_enable : 1;      /* [0]  */
        unsigned int line1_enable : 1;      /* [1]  */
        unsigned int line2_enable : 1;      /* [2]  */
        unsigned int line3_enable : 1;      /* [3]  */
        unsigned int align_mode : 2;      /* [5..4]  */
        unsigned int drd_req_merge_en : 1;      /* [6]  */
        unsigned int reserved : 24; /* [30..7]  */
        unsigned int drd_enable : 1; /* [31]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_mask;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line0_width : 8;      /* [7..0]  */
        unsigned int line1_width : 8;      /* [15..8]  */
        unsigned int line2_width : 8;      /* [23..16]  */
        unsigned int line3_width : 8;      /* [31..24]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_line_width;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line0_color_fill : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_line0_color_cfg;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line0_st_x : 16; /* [15..0]  */
        unsigned int line0_st_y : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_line0_st;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line0_ed_x : 16; /* [15..0]  */
        unsigned int line0_ed_y : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_line0_ed;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line1_color_fill : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_line1_color_cfg;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line1_st_x : 16; /* [15..0]  */
        unsigned int line1_st_y : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_line1_st;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line1_ed_x : 16; /* [15..0]  */
        unsigned int line1_ed_y : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_line1_ed;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line2_color_fill : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_line2_color_cfg;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line2_st_x : 16; /* [15..0]  */
        unsigned int line2_st_y : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_line2_st;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line2_ed_x : 16; /* [15..0]  */
        unsigned int line2_ed_y : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_line2_ed;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line3_color_fill : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_line3_color_cfg;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line3_st_x : 16; /* [15..0]  */
        unsigned int line3_st_y : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_line3_st;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int line3_ed_x : 16; /* [15..0]  */
        unsigned int line3_ed_y : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_tde_drd_line3_ed;
#endif

typedef struct {
    u_src1_ctrl src1_ctrl;                   /* 0x0 */
    u_src1_ch0_addr_high src1_ch0_addr_high; /* 0x4 */
    u_src1_ch0_addr_low src1_ch0_addr_low;   /* 0x8 */
    u_src1_ch1_addr_high src1_ch1_addr_high; /* 0xc */
    u_src1_ch1_addr_low src1_ch1_addr_low;   /* 0x10 */
    u_src1_ch0_stride src1_ch0_stride;       /* 0x14 */
    u_src1_ch1_stride src1_ch1_stride;       /* 0x18 */
    u_src1_imgsize src1_imgsize;             /* 0x1c */
    u_src1_fill src1_fill;                   /* 0x20 */
    u_src1_alpha src1_alpha;                 /* 0x24 */
    u_src1_pix_offset src1_pix_offset;       /* 0x28 */
    u_src2_ctrl src2_ctrl;                   /* 0x2c */
    u_src2_ch0_addr_high src2_ch0_addr_high; /* 0x30 */
    u_src2_ch0_addr_low src2_ch0_addr_low;   /* 0x34 */
    u_src2_ch1_addr_high src2_ch1_addr_high; /* 0x38 */
    u_src2_ch1_addr_low src2_ch1_addr_low;   /* 0x3c */
    u_src2_ch0_stride src2_ch0_stride;       /* 0x40 */
    u_src2_ch1_stride src2_ch1_stride;       /* 0x44 */
    u_src2_imgsize src2_imgsize;             /* 0x48 */
    u_src2_fill src2_fill;                   /* 0x4c */
    u_src2_alpha src2_alpha;                 /* 0x50 */
    u_src2_pix_offset src2_pix_offset;       /* 0x54 */
    td_u32 gdc3_ctrl;                        /* 0x58 */
    td_u32 gdc3_addr_high;                   /* 0x5c */
    td_u32 gdc3_addr_low;                    /* 0x60 */
    td_u32 gdc3_stride;                      /* 0x64 */
    td_u32 gdc3_imgsize;                     /* 0x68 */
    td_u32 gdc3_fill;                        /* 0x6c */
    td_u32 gdc3_alpha;                       /* 0x70 */
    td_u32 gdc3_pix_offset;                  /* 0x74 */
    td_u32 gdc4_ctrl;                        /* 0x78 */
    td_u32 gdc4_addr_high;                   /* 0x7c */
    td_u32 gdc4_addr_low;                    /* 0x80 */
    td_u32 gdc4_stride;                      /* 0x84 */
    td_u32 gdc4_imgsize;                     /* 0x88 */
    td_u32 gdc4_fill;                        /* 0x8c */
    td_u32 gdc4_alpha;                       /* 0x90 */
    td_u32 gdc4_pix_offset;                  /* 0x94 */
    td_u32 gdc5_ctrl;                        /* 0x98 */
    td_u32 gdc5_addr_high;                   /* 0x9C */
    td_u32 gdc5_addr_low;                    /* 0xA0 */
    td_u32 gdc5_stride;                      /* 0xA4 */
    td_u32 gdc5_imgsize;                     /* 0xA8 */
    td_u32 gdc5_fill;                        /* 0xAC */
    td_u32 gdc5_alpha;                       /* 0xB0 */
    td_u32 gdc5_pix_offset;                  /* 0xB4 */
    td_u32 gdc6_ctrl;                        /* 0xB8 */
    td_u32 gdc6_addr_high;                   /* 0xBC */
    td_u32 gdc6_addr_low;                    /* 0xC0 */
    td_u32 gdc6_stride;                      /* 0xC4 */
    td_u32 gdc6_imgsize;                     /* 0xC8 */
    td_u32 gdc6_fill;                        /* 0xCc */
    td_u32 gdc6_alpha;                       /* 0xD0 */
    td_u32 gdc6_pix_offset;                  /* 0xD4 */
    td_u32 gdc7_ctrl;                        /* 0xD8 */
    td_u32 gdc7_addr_high;                   /* 0xDC */
    td_u32 gdc7_addr_low;                    /* 0xE0 */
    td_u32 gdc7_stride;                      /* 0xE4 */
    td_u32 gdc7_imgsize;                     /* 0xE8 */
    td_u32 gdc7_fill;                        /* 0xEC */
    td_u32 gdc7_alpha;                       /* 0xF0 */
    td_u32 gdc7_pix_offset;                  /* 0xF4 */
    u_des_ctrl des_ctrl;                     /* 0xF8 */
    u_des_ch0_addr_high des_ch0_addr_high;   /* 0xFC */
    u_des_ch0_addr_low des_ch0_addr_low;     /* 0x100 */
    u_des_ch1_addr_high des_ch1_addr_high;   /* 0x104 */
    u_des_ch1_addr_low des_ch1_addr_low;     /* 0x108 */
    u_des_ch0_stride des_ch0_stride;         /* 0x10C */
    u_des_ch1_stride des_ch1_stride;         /* 0x110 */
    u_des_imgsize des_imgsize;               /* 0x114 */
    u_des_alpha des_alpha;                   /* 0x118 */
    u_des_crop_pos_st des_crop_pos_st;       /* 0x11c */
    u_des_crop_pos_ed des_crop_pos_ed;       /* 0x120 */
    u_des_pix_offset des_pix_offset;         /* 0x124 */
    td_u32 reserved_1_01[4];                 /* 4:0x128~0x134 */
#if defined CONFIG_TDE_DMA_CORNER_V2
    u_tde_dma_corner_reso tde_dma_corner_reso;  /* 0x138 */
    u_tde_dma_corner_ctrl tde_dma_corner_ctrl;  /* 0x13c */
    u_tde_dma_corner_value tde_dma_corner_value; /* 0x140 */
    u_tde_dma_other_value tde_dma_other_value; /* 0x144 */
    u_des_safe_dist des_safe_dist;              /* 0x148 */
    u_des_safe_dist_inverse des_safe_dist_inverse;  /* 0x14c */
    u_des_bind_buffer_size des_bind_buffer_size; /* 0x150 */
    td_u32 reserved_1_02[43];                   /* 43:0x154~0x1fc */
#elif defined CONFIG_TDE_DMA_CORNER_V1
    u_tde_dma_corner_reso tde_dma_corner_reso;  /* 0x138 */
    u_tde_dma_corner_ctrl tde_dma_corner_ctrl;  /* 0x13c */
    u_des_safe_dist des_safe_dist;              /* 0x140 */
    u_des_safe_dist_inverse des_safe_dist_inverse;  /* 0x144 */
    u_des_bind_buffer_size des_bind_buffer_size; /* 0x148 */
    td_u32 reserved_1_02[45];                   /* 45:0x14c~0x1fc */
#endif
    u_src1_hsp src1_hsp;                     /* 0x200 */
    td_s32 src1_hloffset;                    /* 0x204 */
    td_s32 src1_hcoffset;                    /* 0x208 */
    u_src1_vsp src1_vsp;                     /* 0x20c */
    u_src1_vsr src1_vsr;                     /* 0x210 */
    u_src1_voffset src1_voffset;             /* 0x214 */
    u_src1_zmeoreso src1_zmeoreso;           /* 0x218 */
    u_src1_zmeireso src1_zmeireso;           /* 0x21c */
    td_u32 reserved_2[2];                    /* 2:0x220~0x224 */
    u_src1_hpzme src1_hpzme;                 /* 0x228 */
    u_src1_hpzme_size src1_hpzme_size;       /* 0x22c */
    u_src1_csc_idc0 src1_csc_idc0;           /* 0x230 */
    u_src1_csc_idc1 src1_csc_idc1;           /* 0x234 */
    u_src1_csc_odc0 src1_csc_odc0;           /* 0x238 */
    u_src1_csc_odc1 src1_csc_odc1;           /* 0x23c */
    u_src1_csc_p0 src1_csc_p0;               /* 0x240 */
    u_src1_csc_p1 src1_csc_p1;               /* 0x244 */
    u_src1_csc_p2 src1_csc_p2;               /* 0x248 */
    u_src1_csc_p3 src1_csc_p3;               /* 0x24c */
    u_src1_csc_p4 src1_csc_p4;               /* 0x250 */
    u_src1_dither_ctrl src1_dither_ctrl;     /* 0x254 */
    td_u32 reserved_3[10];                   /* 10:0x258~0x27c */
    u_src2_hsp src2_hsp;                     /* 0x280 */
    td_s32 src2_hloffset;                    /* 0x284 */
    td_s32 src2_hcoffset;                    /* 0x288 */
    u_src2_vsp src2_vsp;                     /* 0x28c */
    u_src2_vsr src2_vsr;                     /* 0x290 */
    u_src2_voffset src2_voffset;             /* 0x294 */
    u_src2_zmeoreso src2_zmeoreso;           /* 0x298 */
    u_src2_zmeireso src2_zmeireso;           /* 0x29c */
    td_u32 reserved_4[2];                    /* 2:0x2a0~0x2a4 */
    u_src2_hpzme src2_hpzme;                 /* 0x2a8 */
    u_src2_hpzme_size src2_hpzme_size;       /* 0x2ac */
    u_src2_csc_mux src2_csc_mux;             /* 0x2b0 */
    u_des_csc_idc0 des_csc_idc0;             /* 0x2b4 */
    u_des_csc_idc1 des_csc_idc1;             /* 0x2b8 */
    u_des_csc_odc0 des_csc_odc0;             /* 0x2bc */
    u_des_csc_odc1 des_csc_odc1;             /* 0x2c0 */
    u_des_csc_p0 des_csc_p0;                 /* 0x2c4 */
    u_des_csc_p1 des_csc_p1;                 /* 0x2c8 */
    u_des_csc_p2 des_csc_p2;                 /* 0x2cc */
    u_des_csc_p3 des_csc_p3;                 /* 0x2d0 */
    u_des_csc_p4 des_csc_p4;                 /* 0x2d4 */


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART06_H_ */
