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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART01_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART01_H_


#include "tde_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SIZE_256BYTE_ALIGN

#define TDE_NODE_HEAD_BYTE 16

#define TDE_NODE_TAIL_BYTE 12

#ifndef SIZE_256BYTE_ALIGN
#define CMD_SIZE    64
#define JOB_SIZE    96
#define NODE_SIZE   208
#define FILTER_SIZE 960
#else
#define CMD_SIZE    64
#define JOB_SIZE    96
#define NODE_SIZE   ((((sizeof(tde_hw_node)) + (TDE_NODE_HEAD_BYTE) + (TDE_NODE_TAIL_BYTE)) + (0x0F)) & (~0x0F))
#define FILTER_SIZE 1792
#endif

#define OT_TDE_FILTER_NUM 3

#define TDE_INTNUM       207
#define TDE_REG_BASEADDR 0x17280000
#define TDE_REG_CLOCK    0x11019d40

#define TDE_CTRL         0x0500
#define TDE_INT          0x0504
#define TDE_INTCLR       0x0508
#define TDE_AQ_NADDR_LOW 0x04fc
#define TDE_AQ_NADDR_HI  0x04f8

#define TDE_STA 0x4000

#define TDE_AQ_ADDR_LOW  0x4098
#define TDE_AQ_ADDR_HI  0x4094

#ifdef CONFIG_TDE_ZME_LINE_BUFFER2048
#define MAX_LINE_BUFFER  2048
#else
#define MAX_LINE_BUFFER  1920
#endif

#define TDE_MISCELLANEOUS 0x0514

#ifndef __LITEOS__
#define TDE_IRQ_NAME "tde_osr_isr"
#else
#define TDE_IRQ_NAME "tde"
#endif

#ifndef __RTOS__
#define DESCRIPTION "TDE Device driver"
#define AUTHOR      "Digital Media Team."
#define TDE_VERSION "V1.0.0.0"
#endif

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src1_fmt : 6;         /* [5..0]  */
        td_u32 src1_argb_order : 5;  /* [10..6]  */
        td_u32 src1_cbcr_order : 1;  /* [11]  */
        td_u32 src1_rgb_exp : 2;     /* [13..12]  */
        td_u32 reserved_0 : 1;       /* [14]  */
        td_u32 src1_rgb_mode : 1;    /* [15]  */
        td_u32 reserved_1 : 2;       /* [17..16]  */
        td_u32 src1_alpha_range : 1; /* [18]  */
        td_u32 src1_v_scan_ord : 1;  /* [19]  */
        td_u32 src1_h_scan_ord : 1;  /* [20]  */
        td_u32 src1_422v_pro : 1;    /* [21]  */
        td_u32 reserved_2 : 7;       /* [28..22]  */
        td_u32 src1_dma : 1;         /* [29]  */
        td_u32 src1_mode : 1;        /* [30]  */
        td_u32 src1_en : 1;          /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_ctrl;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src1_ch0_addr_high : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_ch0_addr_high;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src1_ch0_addr_low : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_ch0_addr_low;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src1_ch1_addr_high : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_ch1_addr_high;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src1_ch1_addr_low : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_ch1_addr_low;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src1_ch0_stride : 20; /* [19..0]  */
        td_u32 reserved : 12;        /* [31..20]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_ch0_stride;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src1_ch1_stride : 20; /* [19..0]  */
        td_u32 reserved : 12;        /* [31..20]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_ch1_stride;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src1_width : 16;  /* [15..0]  */
        td_u32 src1_height : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_imgsize;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src1_color_fill : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_fill;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src1_alpha0 : 8; /* [7..0]  */
        td_u32 src1_alpha1 : 8; /* [15..8]  */
        td_u32 reserved_0 : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_alpha;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src1_hoffset_pix : 16; /* [15..0]  */
        td_u32 reserved_0 : 16;       /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src1_pix_offset;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src2_fmt : 6;           /* [5..0]  */
        td_u32 src2_argb_order : 5;    /* [10..6]  */
        td_u32 src2_cbcr_order : 1;    /* [11]  */
        td_u32 src2_rgb_exp : 2;       /* [13..12]  */
        td_u32 src2_clut_mode : 1;     /* [14]  */
        td_u32 src2_rgb_mode : 1;      /* [15]  */
        td_u32 reserved_0 : 2;         /* [17..16]  */
        td_u32 src2_alpha_range : 1;   /* [18]  */
        td_u32 src2_v_scan_ord : 1;    /* [19]  */
        td_u32 src2_h_scan_ord : 1;    /* [20]  */
        td_u32 src2_422v_pro : 1;      /* [21]  */
        td_u32 src2_dcmp_en : 1;       /* [22]  */
        td_u32 src2_is_lossless : 1;   /* [23]  */
        td_u32 src2_is_lossless_a : 1; /* [24]  */
        td_u32 src2_cmp_mode : 1;      /* [25]  */
        td_u32 src2_top_pred_en : 1;   /* [26]  */
        td_u32 reserved_1 : 2;         /* [28..27]  */
        td_u32 src2_mode : 2;          /* [30..29]  */
        td_u32 src2_en : 1;            /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_ctrl;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 des_start_x : 16; /* [15..0]  */
        td_u32 des_start_y : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_sur_xy;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src2_ch0_addr_high : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_ch0_addr_high;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src2_ch0_addr_low : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_ch0_addr_low;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src2_ch1_addr_high : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_ch1_addr_high;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src2_ch1_addr_low : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_ch1_addr_low;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src2_ch0_stride : 20; /* [19..0]  */
        td_u32 reserved : 12;        /* [31..20]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_ch0_stride;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src2_ch1_stride : 20; /* [19..0]  */
        td_u32 reserved : 12;        /* [31..20]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_ch1_stride;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src2_width : 16;  /* [15..0]  */
        td_u32 src2_height : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_imgsize;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 src2_color_fill : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_fill;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src2_alpha0 : 8; /* [7..0]  */
        td_u32 src2_alpha1 : 8; /* [15..8]  */
        td_u32 reserved_0 : 16; /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_alpha;

typedef union {
    /* Define the struct bits */
    struct {
        td_u32 src2_hoffset_pix : 16; /* [15..0]  */
        td_u32 reserved_0 : 16;       /* [31..16]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_src2_pix_offset;

typedef union {
    struct {
        td_u32 x : 12;       /* First X coordinate */
        td_u32 reserve1 : 4; /* Reserve */
        td_u32 y : 12;       /* First Y coordinate */
        td_u32 reserve2 : 4; /* Reserve */
    } bits;
    td_u32 all;
} u_sur_xy;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int des_fmt : 6;         /* [5..0]  */
        unsigned int des_argb_order : 5;  /* [10..6]  */
        unsigned int des_cbcr_order : 1;  /* [11]  */
        unsigned int des_bind_en : 1;     /* [12]  */
        unsigned int des_bind_mode : 1;   /* [13]  */
        unsigned int reserved_0 : 3;      /* [16..14]  */
        unsigned int des_rgb_round : 1;   /* [17]  */
        unsigned int des_alpha_range : 1; /* [18]  */
        unsigned int des_v_scan_ord : 1;  /* [19]  */
        unsigned int des_h_scan_ord : 1;  /* [20]  */
        unsigned int reserved_1 : 2;      /* [22..21]  */
        unsigned int cmp_en : 1;          /* [23]  */
        unsigned int cmp_addr_chg : 1;    /* [24]  */
        unsigned int reserved_2 : 6;      /* [30..25]  */
        unsigned int des_en : 1;          /* [31]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_ctrl;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 head_ar_addr_hi : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_head_ar_addr_hi;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 head_ar_addr_low : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_head_ar_addr_low;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 head_gb_addr_hi : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_head_gb_addr_hi;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 head_gb_addr_low : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_head_gb_addr_low;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 des_ch0_addr_high : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_ch0_addr_high;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 des_ch0_addr_low : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_ch0_addr_low;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 des_ch1_addr_hi : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_ch1_addr_high;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 des_ch1_addr_low : 32; /* [31..0]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_ch1_addr_low;

typedef union {
    /* Define the struct bits  */
    struct {
        td_u32 des_ch0_stride : 20; /* [19..0]  */
        td_u32 reserved : 12;       /* [31..20]  */
    } bits;
    /* Define an unsigned member */
    td_u32 all;
} u_des_ch0_stride;


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART01_H_ */
