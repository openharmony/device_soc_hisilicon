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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART35_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART35_H_

    /* define the struct bits */
    struct {
        unsigned int y_n_coef : 11;   /* [10..0]  */
        unsigned int reserved_0 : 5;  /* [15..11]  */
        unsigned int y_n_1_coef : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;  /* [31..27]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff27;

/* define the union reg_date_coeff28 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int pixel_begin1 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;    /* [15..11]  */
        unsigned int pixel_begin2 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;    /* [31..27]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff28;

/* define the union reg_date_coeff29 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int pixel_end : 11;  /* [10..0]  */
        unsigned int reserved_0 : 21; /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff29;

/* define the union reg_date_coeff30 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int g_secam : 7;     /* [6..0]  */
        unsigned int reserved_0 : 25; /* [31..7]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff30;

/* define the union reg_date_isrmask */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tt_mask : 1;     /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_isrmask;

/* define the union reg_date_isrstate */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tt_status : 1;   /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_isrstate;

/* define the union reg_date_isr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tt_int : 1;      /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_isr;

/* define the union reg_date_coeff37 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int fir_y1_coeff0 : 8; /* [7..0]  */
        unsigned int fir_y1_coeff1 : 8; /* [15..8]  */
        unsigned int fir_y1_coeff2 : 8; /* [23..16]  */
        unsigned int fir_y1_coeff3 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff37;

/* define the union reg_date_coeff38 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int fir_y2_coeff0 : 16; /* [15..0]  */
        unsigned int fir_y2_coeff1 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff38;

/* define the union reg_date_coeff39 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int fir_y2_coeff2 : 16; /* [15..0]  */
        unsigned int fir_y2_coeff3 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff39;

/* define the union reg_date_coeff40 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int fir_c1_coeff0 : 8; /* [7..0]  */
        unsigned int fir_c1_coeff1 : 8; /* [15..8]  */
        unsigned int fir_c1_coeff2 : 8; /* [23..16]  */
        unsigned int fir_c1_coeff3 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff40;

/* define the union reg_date_coeff41 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int fir_c2_coeff0 : 16; /* [15..0]  */
        unsigned int fir_c2_coeff1 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff41;

/* define the union reg_date_coeff42 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int fir_c2_coeff2 : 16; /* [15..0]  */
        unsigned int fir_c2_coeff3 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff42;

/* define the union reg_date_dacdet1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vdac_det_high : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;     /* [15..10]  */
        unsigned int det_line : 10;      /* [25..16]  */
        unsigned int reserved_1 : 6;     /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_dacdet1;

/* define the union reg_date_dacdet2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int det_pixel_sta : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;     /* [15..11]  */
        unsigned int det_pixel_wid : 11; /* [26..16]  */
        unsigned int reserved_1 : 4;     /* [30..27]  */
        unsigned int vdac_det_en : 1;    /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_dacdet2;

/* define the union reg_date_coeff50 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ovs_coeff0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;  /* [15..11]  */
        unsigned int ovs_coeff1 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;  /* [31..27]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff50;

/* define the union reg_date_coeff51 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ovs_coeff0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;  /* [15..11]  */
        unsigned int ovs_coeff1 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;  /* [31..27]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff51;

/* define the union reg_date_coeff52 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ovs_coeff0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;  /* [15..11]  */
        unsigned int ovs_coeff1 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;  /* [31..27]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff52;

/* define the union reg_date_coeff53 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ovs_coeff0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;  /* [15..11]  */
        unsigned int ovs_coeff1 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;  /* [31..27]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff53;

/* define the union reg_date_coeff54 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ovs_coeff0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;  /* [15..11]  */
        unsigned int ovs_coeff1 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;  /* [31..27]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff54;

/* define the union reg_date_coeff55 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ovs_coeff0 : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;  /* [15..11]  */
        unsigned int ovs_coeff1 : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;  /* [31..27]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff55;

/* define the union reg_date_coeff57 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int v_gain : 8;       /* [7..0]  */
        unsigned int reg_gain : 8;     /* [15..8]  */
        unsigned int ycvbs_gain : 8;   /* [23..16]  */
        unsigned int reserved_0 : 7;   /* [30..24]  */
        unsigned int cvbs_gain_en : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_date_coeff57;

/* define the union reg_mac_outstanding */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mstr0_routstanding : 4; /* [3..0]  */
        unsigned int mstr0_woutstanding : 4; /* [7..4]  */
        unsigned int mstr1_routstanding : 4; /* [11..8]  */
        unsigned int mstr1_woutstanding : 4; /* [15..12]  */
        unsigned int mstr2_routstanding : 4; /* [19..16]  */
        unsigned int mstr2_woutstanding : 4; /* [23..20]  */
        unsigned int reserved_0 : 8;         /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_outstanding;

/* define the union reg_mac_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int split_mode : 4;  /* [3..0]  */
        unsigned int arb_mode : 4;    /* [7..4]  */
        unsigned int mid_enable : 1;  /* [8]  */
        unsigned int reserved_0 : 3;  /* [11..9]  */
        unsigned int wport_sel : 4;   /* [15..12]  */
        unsigned int reserved_1 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_ctrl;

/* define the union reg_mac_rchn_prio */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int para_prio : 1;    /* [0]  */
        unsigned int v0_prio : 1;      /* [1]  */
        unsigned int v0h_prio : 1;     /* [2]  */
        unsigned int v0t_prio : 1;     /* [3]  */
        unsigned int v1_prio : 1;      /* [4]  */
        unsigned int v1t_prio : 1;     /* [5]  */
        unsigned int v2_prio : 1;      /* [6]  */
        unsigned int g0_prio : 1;      /* [7]  */
        unsigned int g1_prio : 1;      /* [8]  */
        unsigned int g3_prio : 1;      /* [9]  */
        unsigned int rchn31_prio : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_rchn_prio;

/* define the union reg_mac_wchn_prio */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wchn_prio : 1;    /* [0]  */
        unsigned int wchnh_prio : 1;   /* [1]  */
        unsigned int wchn31_prio : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_wchn_prio;

/* define the union reg_mac_rchn_sel0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int para_sel : 2;   /* [1..0]  */
        unsigned int v0_sel : 2;     /* [3..2]  */
        unsigned int v1_sel : 2;     /* [5..4]  */
        unsigned int v2_sel : 2;     /* [7..6]  */
        unsigned int v3_sel : 2;     /* [9..8]  */
        unsigned int g0_sel : 2;     /* [11..10]  */
        unsigned int g1_sel : 2;     /* [13..12]  */
        unsigned int g2_sel : 2;     /* [15..14]  */
        unsigned int g3_sel : 2;     /* [17..16]  */
        unsigned int g4_sel : 2;     /* [19..18]  */
        unsigned int rchn10_sel : 2; /* [21..20]  */
        unsigned int rchn11_sel : 2; /* [23..22]  */
        unsigned int rchn12_sel : 2; /* [25..24]  */
        unsigned int rchn13_sel : 2; /* [27..26]  */
        unsigned int rchn14_sel : 2; /* [29..28]  */
        unsigned int rchn15_sel : 2; /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_rchn_sel0;

/* define the union reg_mac_wchn_sel0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int wchn0_sel : 2;   /* [1..0]  */
        unsigned int wchn15_sel : 30; /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_wchn_sel0;

/* define the union reg_mac_bus_err_clr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int bus_error_clr : 1; /* [0]  */
        unsigned int reserved_0 : 31;   /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_bus_err_clr;

/* define the union reg_mac_bus_err */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mst0_r_error : 1; /* [0]  */
        unsigned int mst0_w_error : 1; /* [1]  */
        unsigned int mst1_r_error : 1; /* [2]  */
        unsigned int mst1_w_error : 1; /* [3]  */
        unsigned int mst2_r_error : 1; /* [4]  */
        unsigned int mst2_w_error : 1; /* [5]  */
        unsigned int reserved_0 : 26;  /* [31..6]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mac_bus_err;

/* define the union reg_mac_debug_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int axi_det_enable : 1; /* [0]  */
        unsigned int reserved_0 : 3;     /* [3..1]  */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART35_H_ */
