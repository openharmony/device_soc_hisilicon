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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART31_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART31_H_

typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_v0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_v0;

/* define the union reg_mipi_dither_sed_w0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_w0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_w0;

/* define the union reg_mipi_dither_sed_y1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_y1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_y1;

/* define the union reg_mipi_dither_sed_u1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_u1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_u1;

/* define the union reg_mipi_dither_sed_v1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_v1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_v1;

/* define the union reg_mipi_dither_sed_w1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_w1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_w1;

/* define the union reg_mipi_dither_sed_y2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_y2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_y2;

/* define the union reg_mipi_dither_sed_u2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_u2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_u2;

/* define the union reg_mipi_dither_sed_v2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_v2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_v2;

/* define the union reg_mipi_dither_sed_w2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_w2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_w2;

/* define the union reg_mipi_dither_sed_y3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_y3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_y3;

/* define the union reg_mipi_dither_sed_u3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_u3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_u3;

/* define the union reg_mipi_dither_sed_v3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_v3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_v3;

/* define the union reg_mipi_dither_sed_w3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_sed_w3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_sed_w3;

/* define the union reg_mipi_dither_thr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dither_thr_min : 16; /* [15..0]  */
        unsigned int dither_thr_max : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_dither_thr;

/* define the union reg_dhd1_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;          /* [0]  */
        unsigned int disp_mode : 3;      /* [3..1]  */
        unsigned int iop : 1;            /* [4]  */
        unsigned int intf_ivs : 1;       /* [5]  */
        unsigned int intf_ihs : 1;       /* [6]  */
        unsigned int intf_idv : 1;       /* [7]  */
        unsigned int reserved_0 : 1;     /* [8]  */
        unsigned int hdmi420c_sel : 1;   /* [9]  */
        unsigned int hdmi420_en : 1;     /* [10]  */
        unsigned int uf_offline_en : 1;  /* [11]  */
        unsigned int reserved_1 : 2;     /* [13..12]  */
        unsigned int hdmi_mode : 1;      /* [14]  */
        unsigned int twochn_debug : 1;   /* [15]  */
        unsigned int twochn_en : 1;      /* [16]  */
        unsigned int reserved_2 : 1;     /* [17]  */
        unsigned int cbar_mode : 1;      /* [18]  */
        unsigned int sin_en : 1;         /* [19]  */
        unsigned int fpga_lmt_width : 7; /* [26..20]  */
        unsigned int fpga_lmt_en : 1;    /* [27]  */
        unsigned int p2i_en : 1;         /* [28]  */
        unsigned int cbar_sel : 1;       /* [29]  */
        unsigned int cbar_en : 1;        /* [30]  */
        unsigned int intf_en : 1;        /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_ctrl;

/* define the union reg_dhd1_vsync1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vact : 16; /* [15..0]  */
        unsigned int vbb : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_vsync1;

/* define the union reg_dhd1_vsync2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vfb : 16;        /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_vsync2;

/* define the union reg_dhd1_hsync1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hact : 16; /* [15..0]  */
        unsigned int hbb : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_hsync1;

/* define the union reg_dhd1_hsync2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hfb : 16;  /* [15..0]  */
        unsigned int hmid : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_hsync2;

/* define the union reg_dhd1_vplus1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int bvact : 16; /* [15..0]  */
        unsigned int bvbb : 16;  /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_vplus1;

/* define the union reg_dhd1_vplus2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int bvfb : 16;       /* [15..0]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_vplus2;

/* define the union reg_dhd1_pwr */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hpw : 16;        /* [15..0]  */
        unsigned int vpw : 8;         /* [23..16]  */
        unsigned int reserved_0 : 3;  /* [26..24]  */
        unsigned int multichn_en : 2; /* [28..27]  */
        unsigned int reserved_1 : 3;  /* [31..29]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_pwr;

/* define the union reg_dhd1_vtthd3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vtmgthd3 : 13;  /* [12..0]  */
        unsigned int reserved_0 : 2; /* [14..13]  */
        unsigned int thd3_mode : 1;  /* [15]  */
        unsigned int vtmgthd4 : 13;  /* [28..16]  */
        unsigned int reserved_1 : 2; /* [30..29]  */
        unsigned int thd4_mode : 1;  /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_vtthd3;

/* define the union reg_dhd1_vtthd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vtmgthd1 : 13;  /* [12..0]  */
        unsigned int reserved_0 : 2; /* [14..13]  */
        unsigned int thd1_mode : 1;  /* [15]  */
        unsigned int vtmgthd2 : 13;  /* [28..16]  */
        unsigned int reserved_1 : 2; /* [30..29]  */
        unsigned int thd2_mode : 1;  /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_vtthd;

/* define the union reg_dhd1_parathd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int para_thd : 8;    /* [7..0]  */
        unsigned int reserved_0 : 23; /* [30..8]  */
        unsigned int dfs_en : 1;      /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_parathd;

/* define the union reg_dhd1_precharge_thd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int tcon_precharge_thd : 17; /* [16..0]  */
        unsigned int reserved_0 : 3;          /* [19..17]  */
        unsigned int vsync_te_mode : 1;       /* [20]  */
        unsigned int reserved_1 : 11;         /* [31..21]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_precharge_thd;

/* define the union reg_dhd1_start_pos */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int start_pos : 8;        /* [7..0]  */
        unsigned int timing_start_pos : 8; /* [15..8]  */
        unsigned int fi_start_pos : 4;     /* [19..16]  */
        unsigned int req_start_pos : 12;   /* [31..20]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_start_pos;

/* define the union reg_dhd1_start_pos1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int req_start_pos1 : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;     /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_start_pos1;

/* define the union reg_dhd1_paraup */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 31; /* [30..0]  */
        unsigned int paraup_mode : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_paraup;

/* define the union reg_dhd1_sync_inv */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd1_sync_inv;

/* define the union reg_dhd1_clk_dv_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int intf_clk_mux : 1;        /* [0]  */
        unsigned int intf_dv_mux : 1;         /* [1]  */
        unsigned int no_active_area_pos : 16; /* [17..2]  */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART31_H_ */
