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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART40_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART40_H_

typedef union {
    /* define the struct bits */
    struct {
        unsigned int max_gap_bw_row_len_thr : 7; /* [6..0]  */
        unsigned int reserved_0 : 25;            /* [31..7]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_stat_thr;

/* define the union reg_od_pic_osd_pcmp */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int pcmp_start_hpos : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;       /* [15..13]  */
        unsigned int pcmp_end_hpos : 13;   /* [28..16]  */
        unsigned int reserved_1 : 3;       /* [31..29]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_pcmp;

/* define the union reg_od_pic_osd_bs_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int frame_size_reg : 22; /* [21..0]  */
        unsigned int reserved_0 : 10;     /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_bs_size;

/* define the union reg_od_pic_osd_worst_row */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int max_frm_row_len : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_worst_row;

/* define the union reg_od_pic_osd_best_row */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int min_frm_row_len : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_best_row;

/* define the union reg_od_pic_osd_stat_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int max_gap_bw_row_len_cnt : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;             /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_od_pic_osd_stat_info;

/* define the union reg_v0_mrg_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_y_l4_addr : 4; /* [3..0]  */
        unsigned int mrg_c_l4_addr : 4; /* [7..4]  */
        unsigned int reserved_0 : 12;   /* [19..8]  */
        unsigned int mrg_edge_en : 1;   /* [20]  */
        unsigned int reserved_1 : 4;    /* [24..21]  */
        unsigned int mrg_edge_typ : 1;  /* [25]  */
        unsigned int reserved_2 : 2;    /* [27..26]  */
        unsigned int mrg_crop_en : 1;   /* [28]  */
        unsigned int mrg_dcmp_en : 1;   /* [29]  */
        unsigned int mrg_mute_en : 1;   /* [30]  */
        unsigned int mrg_en : 1;        /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_mrg_ctrl;

/* define the union reg_v0_mrg_disp_pos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_xpos : 16; /* [15..0]  */
        unsigned int mrg_ypos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_mrg_disp_pos;

/* define the union reg_v0_mrg_disp_reso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_width : 16;  /* [15..0]  */
        unsigned int mrg_height : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_mrg_disp_reso;

/* define the union reg_v0_mrg_src_reso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_src_width : 16;  /* [15..0]  */
        unsigned int mrg_src_height : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_mrg_src_reso;

/* define the union reg_v0_mrg_src_offset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_src_hoffset : 16; /* [15..0]  */
        unsigned int mrg_src_voffset : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_mrg_src_offset;

/* define the union reg_v0_mrg_stride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_c_stride : 16; /* [15..0]  */
        unsigned int mrg_y_stride : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_mrg_stride;

/* define the union reg_v0_mrg_hstride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_ch_stride : 16; /* [15..0]  */
        unsigned int mrg_yh_stride : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_mrg_hstride;

/* define the union reg_v0_mrg_read_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int rd_region : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_mrg_read_ctrl;

/* define the union reg_v0_mrg_read_en */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int rd_en : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_mrg_read_en;

/* define the union reg_v1_mrg_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_y_l4_addr : 4; /* [3..0]  */
        unsigned int mrg_c_l4_addr : 4; /* [7..4]  */
        unsigned int reserved_0 : 12;   /* [19..8]  */
        unsigned int mrg_edge_en : 1;   /* [20]  */
        unsigned int reserved_1 : 4;    /* [24..21]  */
        unsigned int mrg_edge_typ : 1;  /* [25]  */
        unsigned int reserved_2 : 2;    /* [27..26]  */
        unsigned int mrg_crop_en : 1;   /* [28]  */
        unsigned int mrg_dcmp_en : 1;   /* [29]  */
        unsigned int mrg_mute_en : 1;   /* [30]  */
        unsigned int mrg_en : 1;        /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_mrg_ctrl;

/* define the union reg_v1_mrg_disp_pos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_xpos : 16; /* [15..0]  */
        unsigned int mrg_ypos : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_mrg_disp_pos;

/* define the union reg_v1_mrg_disp_reso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_width : 16;  /* [15..0]  */
        unsigned int mrg_height : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_mrg_disp_reso;

/* define the union reg_v1_mrg_src_reso */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_src_width : 16;  /* [15..0]  */
        unsigned int mrg_src_height : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_mrg_src_reso;

/* define the union reg_v1_mrg_src_offset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_src_hoffset : 16; /* [15..0]  */
        unsigned int mrg_src_voffset : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_mrg_src_offset;

/* define the union reg_v1_mrg_stride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_c_stride : 16; /* [15..0]  */
        unsigned int mrg_y_stride : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_mrg_stride;

/* define the union reg_v1_mrg_hstride */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mrg_ch_stride : 16; /* [15..0]  */
        unsigned int mrg_yh_stride : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_mrg_hstride;

/* define the union reg_v1_mrg_read_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int rd_region : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_mrg_read_ctrl;

/* define the union reg_v1_mrg_read_en */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int rd_en : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_mrg_read_en;

/* define the union reg_g1_osb_ctrl1_box_0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mode_0 : 2;       /* [1..0]  */
        unsigned int thick_w_0 : 6;    /* [7..2]  */
        unsigned int arm_w_0 : 8;      /* [15..8]  */
        unsigned int edge_v_0 : 4;     /* [19..16]  */
        unsigned int edge_u_0 : 4;     /* [23..20]  */
        unsigned int edge_y_0 : 4;     /* [27..24]  */
        unsigned int edge_alpha_0 : 4; /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_osb_ctrl1_box_0;

/* define the union reg_g1_osb_ctrl2_box_0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hstr_pos_0 : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;  /* [15..12]  */
        unsigned int hend_pos_0 : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;  /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_osb_ctrl2_box_0;

/* define the union reg_g1_osb_ctrl3_box_0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vstr_pos_0 : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;  /* [15..12]  */
        unsigned int vend_pos_0 : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;  /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g1_osb_ctrl3_box_0;

/* define the union reg_g3_osb_ctrl1_box_0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mode_0 : 2;       /* [1..0]  */
        unsigned int thick_w_0 : 6;    /* [7..2]  */
        unsigned int arm_w_0 : 8;      /* [15..8]  */
        unsigned int edge_v_0 : 4;     /* [19..16]  */
        unsigned int edge_u_0 : 4;     /* [23..20]  */
        unsigned int edge_y_0 : 4;     /* [27..24]  */
        unsigned int edge_alpha_0 : 4; /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_osb_ctrl1_box_0;

/* define the union reg_g3_osb_ctrl2_box_0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hstr_pos_0 : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;  /* [15..12]  */
        unsigned int hend_pos_0 : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;  /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_osb_ctrl2_box_0;

/* define the union reg_g3_osb_ctrl3_box_0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vstr_pos_0 : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;  /* [15..12]  */
        unsigned int vend_pos_0 : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;  /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_osb_ctrl3_box_0;

/* define the union reg_g4_osb_ctrl1_box_0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mode_0 : 2;       /* [1..0]  */
        unsigned int thick_w_0 : 6;    /* [7..2]  */
        unsigned int arm_w_0 : 8;      /* [15..8]  */
        unsigned int edge_v_0 : 4;     /* [19..16]  */
        unsigned int edge_u_0 : 4;     /* [23..20]  */
        unsigned int edge_y_0 : 4;     /* [27..24]  */
        unsigned int edge_alpha_0 : 4; /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_osb_ctrl1_box_0;

/* define the union reg_g4_osb_ctrl2_box_0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hstr_pos_0 : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;  /* [15..12]  */
        unsigned int hend_pos_0 : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;  /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_osb_ctrl2_box_0;

/* define the union reg_g4_osb_ctrl3_box_0 */
typedef union {


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART40_H_ */
