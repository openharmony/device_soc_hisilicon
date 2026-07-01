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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART37_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART37_H_

    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_mrg_disp_pos;

/* Define the union u_v1_mrg_disp_reso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_width : 16;  /* [15..0]  */
        unsigned int mrg_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_mrg_disp_reso;

/* Define the union u_v1_mrg_src_reso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_src_width : 16;  /* [15..0]  */
        unsigned int mrg_src_height : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_mrg_src_reso;

/* Define the union u_v1_mrg_src_offset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_src_hoffset : 16; /* [15..0]  */
        unsigned int mrg_src_voffset : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_mrg_src_offset;

/* Define the union u_v1_mrg_stride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_c_stride : 16; /* [15..0]  */
        unsigned int mrg_y_stride : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_mrg_stride;

/* Define the union u_v1_mrg_hstride */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mrg_ch_stride : 16; /* [15..0]  */
        unsigned int mrg_yh_stride : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_mrg_hstride;

/* Define the union u_v1_mrg_read_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rd_region : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_mrg_read_ctrl;

/* Define the union u_v1_mrg_read_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rd_en : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_mrg_read_en;

/* define the union reg_osb_ctrl1_box_0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mode_0 : 2;       /* [1..0]  */
        unsigned int thick_w_0 : 6;    /* [7..2]  */
        unsigned int arm_w_0 : 8;      /* [15..8]  */
        unsigned int edge_v_0 : 4;     /* [19..16]  */
        unsigned int edge_reg_0 : 4;   /* [23..20]  */
        unsigned int edge_y_0 : 4;     /* [27..24]  */
        unsigned int edge_alpha_0 : 4; /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_osb_ctrl1_box_0;

/* define the union reg_osb_ctrl2_box_0 */
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
} u_osb_ctrl2_box_0;

/* define the union reg_osb_ctrl3_box_0 */
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
} u_osb_ctrl3_box_0;

/* Define the union u_v1_csc_idc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc0 : 11;     /* [10..0]  */
        unsigned int cscidc1 : 11;     /* [21..11]  */
        unsigned int csc_en : 1;       /* [22]  */
        unsigned int csc_mode : 3;     /* [25..23]  */
        unsigned int csc_ck_gt_en : 1; /* [26]  */
        unsigned int reserved_0 : 5;   /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc_idc;

/* Define the union u_v1_csc_odc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscodc0 : 11;      /* [10..0]  */
        unsigned int cscodc1 : 11;      /* [21..11]  */
        unsigned int csc_sign_mode : 1; /* [22]  */
        unsigned int reserved_0 : 9;    /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc_odc;

/* Define the union u_v1_csc_iodc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc_iodc;

/* Define the union u_v1_csc_p0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp00 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp01 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc_p0;

/* Define the union u_v1_csc_p1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp02 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp10 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc_p1;

/* Define the union u_v1_csc_p2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp11 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp12 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc_p2;

/* Define the union u_v1_csc_p3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp20 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp21 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc_p3;

/* Define the union u_v1_csc_p4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp22 : 15;     /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc_p4;

/* Define the union u_v1_csc1_idc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc0 : 11;   /* [10..0]  */
        unsigned int cscidc1 : 11;   /* [21..11]  */
        unsigned int csc_en : 1;     /* [22]  */
        unsigned int csc_mode : 3;   /* [25..23]  */
        unsigned int reserved_0 : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc1_idc;

/* Define the union u_v1_csc1_odc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscodc0 : 11;      /* [10..0]  */
        unsigned int cscodc1 : 11;      /* [21..11]  */
        unsigned int csc_sign_mode : 1; /* [22]  */
        unsigned int reserved_0 : 9;    /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc1_odc;

/* Define the union u_v1_csc1_iodc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc1_iodc;

/* Define the union u_v1_csc1_p0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp00 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp01 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc1_p0;

/* Define the union u_v1_csc1_p1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp02 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp10 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc1_p1;

/* Define the union u_v1_csc1_p2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp11 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp12 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc1_p2;

/* Define the union u_v1_csc1_p3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp20 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp21 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc1_p3;

/* Define the union u_v1_csc1_p4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp22 : 15;     /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_csc1_p4;

/* Define the union u_v2_csc_idc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc0 : 11;     /* [10..0]  */
        unsigned int cscidc1 : 11;     /* [21..11]  */
        unsigned int csc_en : 1;       /* [22]  */
        unsigned int csc_mode : 3;     /* [25..23]  */
        unsigned int csc_ck_gt_en : 1; /* [26]  */
        unsigned int reserved_0 : 5;   /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_csc_idc;

/* Define the union u_v2_csc_odc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscodc0 : 11;      /* [10..0]  */
        unsigned int cscodc1 : 11;      /* [21..11]  */
        unsigned int csc_sign_mode : 1; /* [22]  */
        unsigned int reserved_0 : 9;    /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_csc_odc;

/* Define the union u_v2_csc_iodc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_csc_iodc;

/* Define the union u_v2_csc_p0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp00 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp01 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_csc_p0;

/* Define the union u_v2_csc_p1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp02 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp10 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v2_csc_p1;

/* Define the union u_v2_csc_p2 */
typedef union {
    /* Define the struct bits */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART37_H_ */
