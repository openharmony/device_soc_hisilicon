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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART32_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART32_H_


/* Define the union u_date_coeff30 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int g_secam : 7;     /* [6..0]  */
        unsigned int reserved_0 : 25; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_coeff30;

/* Define the union u_date_isrmask */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tt_mask : 1;     /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_isrmask;

/* Define the union u_date_isrstate */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tt_status : 1;   /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_isrstate;

/* Define the union u_date_isr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tt_int : 1;      /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_isr;

/* Define the union u_date_coeff37 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int fir_y1_coeff0 : 8; /* [7..0]  */
        unsigned int fir_y1_coeff1 : 8; /* [15..8]  */
        unsigned int fir_y1_coeff2 : 8; /* [23..16]  */
        unsigned int fir_y1_coeff3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_coeff37;

/* Define the union u_date_coeff38 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int fir_y2_coeff0 : 16; /* [15..0]  */
        unsigned int fir_y2_coeff1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_coeff38;

/* Define the union u_date_coeff39 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int fir_y2_coeff2 : 16; /* [15..0]  */
        unsigned int fir_y2_coeff3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_coeff39;

/* Define the union u_date_coeff40 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int fir_c1_coeff0 : 8; /* [7..0]  */
        unsigned int fir_c1_coeff1 : 8; /* [15..8]  */
        unsigned int fir_c1_coeff2 : 8; /* [23..16]  */
        unsigned int fir_c1_coeff3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_coeff40;

/* Define the union u_date_coeff41 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int fir_c2_coeff0 : 16; /* [15..0]  */
        unsigned int fir_c2_coeff1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_coeff41;

/* Define the union u_date_coeff42 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int fir_c2_coeff2 : 16; /* [15..0]  */
        unsigned int fir_c2_coeff3 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_coeff42;

/* Define the union u_date_dacdet1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vdac_det_high : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;     /* [15..10]  */
        unsigned int det_line : 10;      /* [25..16]  */
        unsigned int reserved_1 : 6;     /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_dacdet1;

/* Define the union u_date_dacdet2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int det_pixel_sta : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;     /* [15..11]  */
        unsigned int det_pixel_wid : 11; /* [26..16]  */
        unsigned int reserved_1 : 4;     /* [30..27]  */
        unsigned int vdac_det_en : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_dacdet2;

/* Define the union u_date_coeff50 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ovs_coeff0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;  /* [15..11]  */
        unsigned int ovs_coeff1 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;  /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_coeff50;

/* Define the union u_date_coeff51 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ovs_coeff0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;  /* [15..11]  */
        unsigned int ovs_coeff1 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;  /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_coeff51;

/* Define the union u_date_coeff52 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ovs_coeff0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;  /* [15..11]  */
        unsigned int ovs_coeff1 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;  /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_coeff52;

/* Define the union u_date_coeff53 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ovs_coeff0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;  /* [15..11]  */
        unsigned int ovs_coeff1 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;  /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_coeff53;

/* Define the union u_date_coeff54 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ovs_coeff0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;  /* [15..11]  */
        unsigned int ovs_coeff1 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;  /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_coeff54;

/* Define the union u_date_coeff55 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ovs_coeff0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;  /* [15..11]  */
        unsigned int ovs_coeff1 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;  /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_date_coeff55;

/* Define the union u_mac_outstanding */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mstr0_routstanding : 4; /* [3..0]  */
        unsigned int mstr0_woutstanding : 4; /* [7..4]  */
        unsigned int mstr1_routstanding : 4; /* [11..8]  */
        unsigned int mstr1_woutstanding : 4; /* [15..12]  */
        unsigned int mstr2_routstanding : 4; /* [19..16]  */
        unsigned int mstr2_woutstanding : 4; /* [23..20]  */
        unsigned int reserved_0 : 8;         /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mac_outstanding;

/* Define the union u_mac_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int split_mode : 4;  /* [3..0]  */
        unsigned int arb_mode : 4;    /* [7..4]  */
        unsigned int mid_enable : 1;  /* [8]  */
        unsigned int reserved_0 : 3;  /* [11..9]  */
        unsigned int wport_sel : 4;   /* [15..12]  */
        unsigned int reserved_1 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mac_ctrl;

/* Define the union u_mac_rchn_prio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int para_prio : 1;   /* [0]  */
        unsigned int v0l_prio : 1;    /* [1]  */
        unsigned int v0c_prio : 1;    /* [2]  */
        unsigned int v0lh_prio : 1;   /* [3]  */
        unsigned int v0ch_prio : 1;   /* [4]  */
        unsigned int v1l_prio : 1;    /* [5]  */
        unsigned int v1c_prio : 1;    /* [6]  */
        unsigned int v1lh_prio : 1;   /* [7]  */
        unsigned int v1ch_prio : 1;   /* [8]  */
        unsigned int g0ar_prio : 1;   /* [9]  */
        unsigned int g0gb_prio : 1;   /* [10]  */
        unsigned int g1ar_prio : 1;   /* [11]  */
        unsigned int g1gb_prio : 1;   /* [12]  */
        unsigned int v2l_prio : 1;    /* [13]  */
        unsigned int v2c_prio : 1;    /* [14]  */
        unsigned int v2lh_prio : 1;   /* [15]  */
        unsigned int v2ch_prio : 1;   /* [16]  */
        unsigned int g3ar_prio : 1;   /* [17]  */
        unsigned int g3gb_prio : 1;   /* [18]  */
        unsigned int reserved_0 : 13; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mac_rchn_prio;

/* Define the union u_mac_wchn_prio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbcl_prio : 1;   /* [0]  */
        unsigned int wbcc_prio : 1;   /* [1]  */
        unsigned int wbclh_prio : 1;  /* [2]  */
        unsigned int wbcch_prio : 1;  /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mac_wchn_prio;

/* Define the union u_mac_rchn_sel0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int para_sel : 2;   /* [1..0]  */
        unsigned int v0l_sel : 2;    /* [3..2]  */
        unsigned int v0c_sel : 2;    /* [5..4]  */
        unsigned int v0lh_sel : 2;   /* [7..6]  */
        unsigned int v0ch_sel : 2;   /* [9..8]  */
        unsigned int v1l_sel : 2;    /* [11..10]  */
        unsigned int v1c_sel : 2;    /* [13..12]  */
        unsigned int v1lh_sel : 2;   /* [15..14]  */
        unsigned int v1ch_sel : 2;   /* [17..16]  */
        unsigned int g0ar_sel : 2;   /* [19..18]  */
        unsigned int g0gb_sel : 2;   /* [21..20]  */
        unsigned int g1ar_sel : 2;   /* [23..22]  */
        unsigned int g1gb_sel : 2;   /* [25..24]  */
        unsigned int v2_sel : 2;     /* [27..26]  */
        unsigned int g3_sel : 2;     /* [29..28]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mac_rchn_sel0;

/* Define the union u_mac_wchn_sel0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int wbcl_sel : 2;    /* [1..0]  */
        unsigned int wbcc_sel : 2;    /* [3..2]  */
        unsigned int wbclh_sel : 2;   /* [5..4]  */
        unsigned int wbcch_sel : 2;   /* [7..6]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mac_wchn_sel0;

/* Define the union u_mac_bus_err_clr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int bus_error_clr : 1; /* [0]  */
        unsigned int reserved_0 : 31;   /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mac_bus_err_clr;

/* Define the union u_mac_bus_err */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mst0_r_error : 1; /* [0]  */
        unsigned int mst0_w_error : 1; /* [1]  */
        unsigned int mst1_r_error : 1; /* [2]  */
        unsigned int mst1_w_error : 1; /* [3]  */
        unsigned int mst2_r_error : 1; /* [4]  */
        unsigned int mst2_w_error : 1; /* [5]  */
        unsigned int reserved_0 : 26;  /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mac_bus_err;

/* Define the union u_mac_debug_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int axi_det_enable : 1; /* [0]  */
        unsigned int reserved_0 : 3;     /* [3..1]  */
        unsigned int fifo_det_mode : 4;  /* [7..4]  */
        unsigned int reserved_1 : 24;    /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mac_debug_ctrl;

/* Define the union u_mac_debug_clr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int axi_det_clr : 1; /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mac_debug_clr;

/* Define the union u_vid_read_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chm_rmode : 3;     /* [2..0]  */
        unsigned int reserved_0 : 1;    /* [3]  */
        unsigned int lm_rmode : 3;      /* [6..4]  */
        unsigned int reserved_1 : 1;    /* [7]  */
        unsigned int chm_draw_mode : 2; /* [9..8]  */
        unsigned int lm_draw_mode : 2;  /* [11..10]  */
        unsigned int flip_en : 1;       /* [12]  */
        unsigned int chm_copy_en : 1;   /* [13]  */
        unsigned int reserved_2 : 2;    /* [15..14]  */
        unsigned int mute_en : 1;       /* [16]  */
        unsigned int mute_req_en : 1;   /* [17]  */
        unsigned int vicap_mute_en : 1; /* [18]  */
        unsigned int mrg_enable : 1;    /* [19]  */
        unsigned int mrg_mute_mode : 1; /* [20]  */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART32_H_ */
