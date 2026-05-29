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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART27_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART27_H_


/* Define the union u_intf_vga_sync_inv */
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
} u_intf_vga_sync_inv;

/* Define the union u_vga_csc_idc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc0 : 11;   /* [10..0]  */
        unsigned int cscidc1 : 11;   /* [21..11]  */
        unsigned int csc_en : 1;     /* [22]  */
        unsigned int reserved_0 : 9; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vga_csc_idc;

/* Define the union u_vga_csc_odc */
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
} u_vga_csc_odc;

/* Define the union u_vga_csc_iodc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vga_csc_iodc;

/* Define the union u_vga_csc_p0 */
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
} u_vga_csc_p0;

/* Define the union u_vga_csc_p1 */
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
} u_vga_csc_p1;

/* Define the union u_vga_csc_p2 */
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
} u_vga_csc_p2;

/* Define the union u_vga_csc_p3 */
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
} u_vga_csc_p3;

/* Define the union u_vga_csc_p4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp22 : 15;     /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vga_csc_p4;

/* Define the union u_vga_hspcfg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hsp_hf0_tmp0 : 8; /* [7..0]  */
        unsigned int hsp_hf0_tmp1 : 8; /* [15..8]  */
        unsigned int hsp_hf0_tmp2 : 8; /* [23..16]  */
        unsigned int hsp_hf0_tmp3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vga_hspcfg0;

/* Define the union u_vga_hspcfg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hsp_hf0_coring : 8; /* [7..0]  */
        unsigned int reserved_0 : 23;    /* [30..8]  */
        unsigned int hsp_en : 1;         /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vga_hspcfg1;

/* Define the union u_vga_hspcfg5 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hsp_hf0_gainpos : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;       /* [15..11]  */
        unsigned int hsp_hf0_gainneg : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vga_hspcfg5;

/* Define the union u_vga_hspcfg6 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hsp_hf0_overth : 8;      /* [7..0]  */
        unsigned int hsp_hf0_underth : 8;     /* [15..8]  */
        unsigned int hsp_hf0_mixratio : 8;    /* [23..16]  */
        unsigned int reserved_0 : 4;          /* [27..24]  */
        unsigned int hsp_hf0_winsize : 3;     /* [30..28]  */
        unsigned int hsp_hf0_adpshoot_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vga_hspcfg6;

/* Define the union u_vga_hspcfg7 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hsp_hf1_tmp0 : 8; /* [7..0]  */
        unsigned int hsp_hf1_tmp1 : 8; /* [15..8]  */
        unsigned int hsp_hf1_tmp2 : 8; /* [23..16]  */
        unsigned int hsp_hf1_tmp3 : 8; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vga_hspcfg7;

/* Define the union u_vga_hspcfg8 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hsp_hf1_coring : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;    /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vga_hspcfg8;

/* Define the union u_vga_hspcfg12 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hsp_hf1_gainpos : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;       /* [15..11]  */
        unsigned int hsp_hf1_gainneg : 11; /* [26..16]  */
        unsigned int reserved_1 : 5;       /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vga_hspcfg12;

/* Define the union u_vga_hspcfg13 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hsp_hf1_overth : 8;      /* [7..0]  */
        unsigned int hsp_hf1_underth : 8;     /* [15..8]  */
        unsigned int hsp_hf1_mixratio : 8;    /* [23..16]  */
        unsigned int reserved_0 : 4;          /* [27..24]  */
        unsigned int hsp_hf1_winsize : 3;     /* [30..28]  */
        unsigned int hsp_hf1_adpshoot_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vga_hspcfg13;

/* Define the union u_vga_hspcfg14 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hsp_cdti_gain : 8;   /* [7..0]  */
        unsigned int hsp_ldti_gain : 8;   /* [15..8]  */
        unsigned int hsp_lti_ratio : 8;   /* [23..16]  */
        unsigned int hsp_hf_shootdiv : 3; /* [26..24]  */
        unsigned int reserved_0 : 1;      /* [27]  */
        unsigned int hsp_ctih_en : 1;     /* [28]  */
        unsigned int hsp_ltih_en : 1;     /* [29]  */
        unsigned int hsp_h1_en : 1;       /* [30]  */
        unsigned int hsp_h0_en : 1;       /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vga_hspcfg14;

/* Define the union u_vga_hspcfg15 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hsp_glb_underth : 9; /* [8..0]  */
        unsigned int reserved_0 : 1;      /* [9]  */
        unsigned int hsp_glb_overth : 9;  /* [18..10]  */
        unsigned int reserved_1 : 1;      /* [19]  */
        unsigned int hsp_peak_ratio : 8;  /* [27..20]  */
        unsigned int reserved_2 : 4;      /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_vga_hspcfg15;

/* Define the union u_intf_date_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 23;        /* [22..0]  */
        unsigned int uv_mode : 1;            /* [23]  */
        unsigned int yc_mode : 1;            /* [24]  */
        unsigned int lcd_parallel_mode : 1;  /* [25]  */
        unsigned int lcd_data_inv : 1;       /* [26]  */
        unsigned int lcd_parallel_order : 1; /* [27]  */
        unsigned int lcd_serial_perd : 1;    /* [28]  */
        unsigned int lcd_serial_mode : 1;    /* [29]  */
        unsigned int dfir_en : 1;            /* [30]  */
        unsigned int hdmi_mode : 1;          /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf_date_ctrl;

/* Define the union u_intf_date_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf_date_upd;

/* Define the union u_intf_date_sync_inv */
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
} u_intf_date_sync_inv;

/* Define the union u_date_clip0_l */
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
} u_date_clip0_l;

/* Define the union u_date_clip0_h */
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
} u_date_clip0_h;

/* Define the union u_intf0_dither_ctrl */
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
} u_intf0_dither_ctrl;

/* Define the union u_intf0_dither_sed_y0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf0_dither_sed_y0;

/* Define the union u_intf0_dither_sed_u0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_u0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf0_dither_sed_u0;

/* Define the union u_intf0_dither_sed_v0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_v0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf0_dither_sed_v0;

/* Define the union u_intf0_dither_sed_w0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_w0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf0_dither_sed_w0;

/* Define the union u_intf0_dither_sed_y1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf0_dither_sed_y1;

/* Define the union u_intf0_dither_sed_u1 */
typedef union {


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART27_H_ */
