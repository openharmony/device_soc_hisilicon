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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART30_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART30_H_

    struct {
        unsigned int vtmgthd3 : 13;  /* [12..0]  */
        unsigned int reserved_0 : 2; /* [14..13]  */
        unsigned int thd3_mode : 1;  /* [15]  */
        unsigned int vtmgthd4 : 13;  /* [28..16]  */
        unsigned int reserved_1 : 2; /* [30..29]  */
        unsigned int thd4_mode : 1;  /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_vtthd3;

/* Define the union u_dhd2_vtthd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vtmgthd1 : 13;  /* [12..0]  */
        unsigned int reserved_0 : 2; /* [14..13]  */
        unsigned int thd1_mode : 1;  /* [15]  */
        unsigned int vtmgthd2 : 13;  /* [28..16]  */
        unsigned int reserved_1 : 2; /* [30..29]  */
        unsigned int thd2_mode : 1;  /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_vtthd;

/* Define the union u_dhd2_parathd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int para_thd : 8;    /* [7..0]  */
        unsigned int reserved_0 : 23; /* [30..8]  */
        unsigned int dfs_en : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_parathd;

/* Define the union u_dhd2_precharge_thd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tcon_precharge_thd : 17; /* [16..0]  */
        unsigned int reserved_0 : 3;          /* [19..17]  */
        unsigned int vsync_te_mode : 1;       /* [20]  */
        unsigned int reserved_1 : 11;         /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_precharge_thd;

/* Define the union u_dhd2_start_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int start_pos : 8;        /* [7..0]  */
        unsigned int timing_start_pos : 8; /* [15..8]  */
        unsigned int fi_start_pos : 4;     /* [19..16]  */
        unsigned int req_start_pos : 12;   /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_start_pos;

/* Define the union u_dhd2_start_pos1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int req_start_pos1 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;     /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_start_pos1;

/* Define the union u_dhd2_paraup */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 31; /* [30..0]  */
        unsigned int paraup_mode : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_paraup;

/* Define the union u_dhd2_sync_inv */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lcd_dv_inv : 1;  /* [0]  */
        unsigned int lcd_hs_inv : 1;  /* [1]  */
        unsigned int lcd_vs_inv : 1;  /* [2]  */
        unsigned int reserved_0 : 1;  /* [3]  */
        unsigned int vga_dv_inv : 1;  /* [4]  */
        unsigned int vga_hs_inv : 1;  /* [5]  */
        unsigned int vga_vs_inv : 1;  /* [6]  */
        unsigned int reserved_1 : 1;  /* [7]  */
        unsigned int hdmi_dv_inv : 1; /* [8]  */
        unsigned int hdmi_hs_inv : 1; /* [9]  */
        unsigned int hdmi_vs_inv : 1; /* [10]  */
        unsigned int hdmi_f_inv : 1;  /* [11]  */
        unsigned int date_dv_inv : 1; /* [12]  */
        unsigned int date_hs_inv : 1; /* [13]  */
        unsigned int date_vs_inv : 1; /* [14]  */
        unsigned int date_f_inv : 1;  /* [15]  */
        unsigned int reserved_2 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_sync_inv;

/* Define the union u_dhd2_clk_dv_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int intf_clk_mux : 1;        /* [0]  */
        unsigned int intf_dv_mux : 1;         /* [1]  */
        unsigned int no_active_area_pos : 16; /* [17..2]  */
        unsigned int reserved_0 : 14;         /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_clk_dv_ctrl;

/* Define the union u_dhd2_rgb_fix_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int fix_b : 10;      /* [9..0]  */
        unsigned int fix_g : 10;      /* [19..10]  */
        unsigned int fix_r : 10;      /* [29..20]  */
        unsigned int rgb_fix_mux : 1; /* [30]  */
        unsigned int reserved_0 : 1;  /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_rgb_fix_ctrl;

/* Define the union u_dhd2_lockcfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int measure_en : 1;     /* [0]  */
        unsigned int lock_cnt_en : 1;    /* [1]  */
        unsigned int vdp_measure_en : 1; /* [2]  */
        unsigned int reserved_0 : 29;    /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_lockcfg;

/* Define the union u_dhd2_intf_chksum_high1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int y0_sum_high : 8; /* [7..0]  */
        unsigned int g0_sum_high : 8; /* [15..8]  */
        unsigned int b0_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_intf_chksum_high1;

/* Define the union u_dhd2_intf_chksum_high2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int y1_sum_high : 8; /* [7..0]  */
        unsigned int g1_sum_high : 8; /* [15..8]  */
        unsigned int b1_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_intf_chksum_high2;

/* Define the union u_dhd2_state */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vback_blank : 1;  /* [0]  */
        unsigned int vblank : 1;       /* [1]  */
        unsigned int bottom_field : 1; /* [2]  */
        unsigned int vcnt : 13;        /* [15..3]  */
        unsigned int count_int : 8;    /* [23..16]  */
        unsigned int dhd_even : 1;     /* [24]  */
        unsigned int reserved_0 : 7;   /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_state;

/* Define the union u_dhd2_uf_state */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ud_first_cnt : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;    /* [15..13]  */
        unsigned int start_pos : 8;     /* [23..16]  */
        unsigned int reserved_1 : 8;    /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_uf_state;

/* Define the union u_dhd2_vsync_te_state */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vsync_te_start_sta : 8;  /* [7..0]  */
        unsigned int vsync_te_start_sta1 : 8; /* [15..8]  */
        unsigned int vsync_te_end_sta : 8;    /* [23..16]  */
        unsigned int reserved_0 : 8;          /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_vsync_te_state;

/* Define the union u_dhd2_vsync_te_state1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vsync_te_vfb : 16;   /* [15..0]  */
        unsigned int vsync_te_width : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd2_vsync_te_state1;

/* Define the union u_intf2_dither_ctrl */
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
} u_intf2_dither_ctrl;

/* Define the union u_intf2_dither_sed_y0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf2_dither_sed_y0;

/* Define the union u_intf2_dither_sed_u0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_u0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf2_dither_sed_u0;

/* Define the union u_intf2_dither_sed_v0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_v0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf2_dither_sed_v0;

/* Define the union u_intf2_dither_sed_w0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_w0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf2_dither_sed_w0;

/* Define the union u_intf2_dither_sed_y1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf2_dither_sed_y1;

/* Define the union u_intf2_dither_sed_u1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_u1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf2_dither_sed_u1;

/* Define the union u_intf2_dither_sed_v1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_v1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf2_dither_sed_v1;

/* Define the union u_intf2_dither_sed_w1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_w1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf2_dither_sed_w1;

/* Define the union u_intf2_dither_sed_y2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf2_dither_sed_y2;

/* Define the union u_intf2_dither_sed_u2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_u2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf2_dither_sed_u2;

/* Define the union u_intf2_dither_sed_v2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_v2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf2_dither_sed_v2;

/* Define the union u_intf2_dither_sed_w2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_w2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf2_dither_sed_w2;

/* Define the union u_intf2_dither_sed_y3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf2_dither_sed_y3;

/* Define the union u_intf2_dither_sed_u3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_u3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART30_H_ */
