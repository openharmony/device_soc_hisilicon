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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART25_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART25_H_

    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hdmi_csc_p3;

/* Define the union u_hdmi_csc_p4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp22 : 15;     /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hdmi_csc_p4;

/* Define the union u_intf_mipi_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int intf_422_en : 1;   /* [0]  */
        unsigned int intf_420_en : 1;   /* [1]  */
        unsigned int intf_420_mode : 2; /* [3..2]  */
        unsigned int reserved_0 : 28;   /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf_mipi_ctrl;

/* Define the union u_intf_mipi_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf_mipi_upd;

/* Define the union u_intf_mipi_sync_inv */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dv_inv : 1;      /* [0]  */
        unsigned int hs_inv : 1;      /* [1]  */
        unsigned int vs_inv : 1;      /* [2]  */
        unsigned int f_inv : 1;       /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf_mipi_sync_inv;

/* Define the union u_mipi_intf_chksum_high */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int b0_sum_high : 8; /* [7..0]  */
        unsigned int g0_sum_high : 8; /* [15..8]  */
        unsigned int r0_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_intf_chksum_high;

/* Define the union u_mipi_intf1_chksum_high */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int b1_sum_high : 8; /* [7..0]  */
        unsigned int g1_sum_high : 8; /* [15..8]  */
        unsigned int r1_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_intf1_chksum_high;

/* Define the union u_mipi_hfir_coef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hfir_coef0 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;  /* [15..10]  */
        unsigned int hfir_coef1 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;  /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_hfir_coef0;

/* Define the union u_mipi_hfir_coef1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hfir_coef2 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;  /* [15..10]  */
        unsigned int hfir_coef3 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;  /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_hfir_coef1;

/* Define the union u_mipi_hfir_coef2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hfir_coef4 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;  /* [15..10]  */
        unsigned int hfir_coef5 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;  /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_hfir_coef2;

/* Define the union u_mipi_hfir_coef3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hfir_coef6 : 10; /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mipi_hfir_coef3;

/* Define the union u_intf_bt_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 16; /* [15..0]  */
        unsigned int data_width : 1;  /* [16]  */
        unsigned int bit_inv : 1;     /* [17]  */
        unsigned int uv_mode : 1;     /* [18]  */
        unsigned int yc_mode : 1;     /* [19]  */
        unsigned int reserved_1 : 10; /* [29..20]  */
        unsigned int dfir_en : 1;     /* [30]  */
        unsigned int hdmi_mode : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf_bt_ctrl;

/* Define the union u_intf_bt_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf_bt_upd;

/* Define the union u_intf_bt_sync_inv */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dv_inv : 1;      /* [0]  */
        unsigned int hs_inv : 1;      /* [1]  */
        unsigned int vs_inv : 1;      /* [2]  */
        unsigned int f_inv : 1;       /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf_bt_sync_inv;

/* Define the union u_bt_clip0_l */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int clip_cl0 : 10;  /* [9..0]  */
        unsigned int clip_cl1 : 10;  /* [19..10]  */
        unsigned int clip_cl2 : 10;  /* [29..20]  */
        unsigned int reserved_0 : 1; /* [30]  */
        unsigned int clip_en : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_clip0_l;

/* Define the union u_bt_clip0_h */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int clip_ch0 : 10;  /* [9..0]  */
        unsigned int clip_ch1 : 10;  /* [19..10]  */
        unsigned int clip_ch2 : 10;  /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_clip0_h;

/* Define the union u_bt_dither_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_tap_mode : 2;     /* [1..0]  */
        unsigned int dither_domain_mode : 1;  /* [2]  */
        unsigned int dither_round : 1;        /* [3]  */
        unsigned int dither_mode : 1;         /* [4]  */
        unsigned int dither_en : 1;           /* [5]  */
        unsigned int dither_round_unlim : 1;  /* [6]  */
        unsigned int i_data_width_dither : 3; /* [9..7]  */
        unsigned int o_data_width_dither : 3; /* [12..10]  */
        unsigned int reserved_0 : 19;         /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_ctrl;

/* Define the union u_bt_dither_sed_y0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_y0;

/* Define the union u_bt_dither_sed_u0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_u0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_u0;

/* Define the union u_bt_dither_sed_v0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_v0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_v0;

/* Define the union u_bt_dither_sed_w0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_w0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_w0;

/* Define the union u_bt_dither_sed_y1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_y1;

/* Define the union u_bt_dither_sed_u1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_u1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_u1;

/* Define the union u_bt_dither_sed_v1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_v1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_v1;

/* Define the union u_bt_dither_sed_w1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_w1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_w1;

/* Define the union u_bt_dither_sed_y2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_y2;

/* Define the union u_bt_dither_sed_u2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_u2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_u2;

/* Define the union u_bt_dither_sed_v2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_v2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_v2;

/* Define the union u_bt_dither_sed_w2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_w2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_w2;

/* Define the union u_bt_dither_sed_y3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_y3;

/* Define the union u_bt_dither_sed_u3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_u3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_u3;

/* Define the union u_bt_dither_sed_v3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_v3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_v3;

/* Define the union u_bt_dither_sed_w3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_w3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bt_dither_sed_w3;

/* Define the union u_bt_dither_thr */
typedef union {
    /* Define the struct bits */
    struct {


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART25_H_ */
