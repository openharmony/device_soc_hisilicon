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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART22_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART22_H_

        unsigned int i_data_width_dither : 3; /* [9..7]  */
        unsigned int o_data_width_dither : 3; /* [12..10]  */
        unsigned int reserved_0 : 19;         /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_ctrl;

/* Define the union u_dither_sed_y0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_y0;

/* Define the union u_dither_sed_u0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_u0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_u0;

/* Define the union u_dither_sed_v0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_v0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_v0;

/* Define the union u_dither_sed_w0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_w0 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_w0;

/* Define the union u_dither_sed_y1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_y1;

/* Define the union u_dither_sed_u1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_u1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_u1;

/* Define the union u_dither_sed_v1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_v1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_v1;

/* Define the union u_dither_sed_w1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_w1 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_w1;

/* Define the union u_dither_sed_y2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_y2;

/* Define the union u_dither_sed_u2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_u2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_u2;

/* Define the union u_dither_sed_v2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_v2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_v2;

/* Define the union u_dither_sed_w2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_w2 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_w2;

/* Define the union u_dither_sed_y3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_y3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_y3;

/* Define the union u_dither_sed_u3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_u3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_u3;

/* Define the union u_dither_sed_v3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_v3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_v3;

/* Define the union u_dither_sed_w3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_sed_w3 : 31; /* [30..0]  */
        unsigned int reserved_0 : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_sed_w3;

/* Define the union u_dither_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dither_thr_min : 16; /* [15..0]  */
        unsigned int dither_thr_max : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dither_thr;

/* Define the union u_wd_zme_hinfo */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int out_width : 16;    /* [15..0]  */
        unsigned int hzme_ck_gt_en : 1; /* [16]  */
        unsigned int reserved_0 : 15;   /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_hinfo;

/* Define the union u_wd_zme_hsp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 24; /* [23..0]  */
        unsigned int hfir_order : 1;  /* [24]  */
        unsigned int chfir_mode : 1;  /* [25]  */
        unsigned int lhfir_mode : 1;  /* [26]  */
        unsigned int non_lnr_en : 1;  /* [27]  */
        unsigned int chmid_en : 1;    /* [28]  */
        unsigned int lhmid_en : 1;    /* [29]  */
        unsigned int chfir_en : 1;    /* [30]  */
        unsigned int lhfir_en : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_hsp;

/* Define the union u_wd_zme_hloffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lhfir_offset : 28; /* [27..0]  */
        unsigned int reserved_0 : 4;    /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_hloffset;

/* Define the union u_wd_zme_hcoffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chfir_offset : 28; /* [27..0]  */
        unsigned int reserved_0 : 4;    /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_hcoffset;

/* Define the union u_wd_zme_hcoef_ren */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int apb_vhd_hf_cren : 1; /* [0]  */
        unsigned int apb_vhd_hf_lren : 1; /* [1]  */
        unsigned int reserved_0 : 30;     /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_hcoef_ren;

/* Define the union u_wd_zme_hcoef_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int apb_vhd_hcoef_raddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;         /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_hcoef_rdata;

/* Define the union u_wd_zme_hdraw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hdraw_mode : 2;  /* [1..0]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_hdraw;

/* Define the union u_wd_zme_hratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hratio : 27;    /* [26..0]  */
        unsigned int reserved_0 : 5; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_hratio;

/* Define the union u_wd_zme_vinfo */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int out_height : 16;   /* [15..0]  */
        unsigned int out_fmt : 2;       /* [17..16]  */
        unsigned int out_pro : 1;       /* [18]  */
        unsigned int vzme_ck_gt_en : 1; /* [19]  */
        unsigned int reserved_0 : 12;   /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_vinfo;

/* Define the union u_wd_zme_vsp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 16;  /* [15..0]  */
        unsigned int graphdet_en : 1;  /* [16]  */
        unsigned int reserved_1 : 8;   /* [24..17]  */
        unsigned int cvfir_mode : 1;   /* [25]  */
        unsigned int lvfir_mode : 1;   /* [26]  */
        unsigned int vfir_1tap_en : 1; /* [27]  */
        unsigned int cvmid_en : 1;     /* [28]  */
        unsigned int lvmid_en : 1;     /* [29]  */
        unsigned int cvfir_en : 1;     /* [30]  */
        unsigned int lvfir_en : 1;     /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_vsp;

/* Define the union u_wd_zme_voffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vchroma_offset : 16; /* [15..0]  */
        unsigned int vluma_offset : 16;   /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_voffset;

/* Define the union u_wd_zme_vboffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbchroma_offset : 16; /* [15..0]  */
        unsigned int vbluma_offset : 16;   /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_vboffset;

/* Define the union u_wd_zme_vcoef_ren */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int apb_vhd_vf_cren : 1; /* [0]  */
        unsigned int apb_vhd_vf_lren : 1; /* [1]  */
        unsigned int reserved_0 : 30;     /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_vcoef_ren;

/* Define the union u_wd_zme_vcoef_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int apb_vhd_vcoef_raddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;         /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_vcoef_rdata;

/* Define the union u_wd_zme_vdraw */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vdraw_mode : 2;  /* [1..0]  */
        unsigned int reserved_0 : 30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_vdraw;

/* Define the union u_wd_zme_vratio */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vratio : 19;     /* [18..0]  */
        unsigned int reserved_0 : 13; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_wd_zme_vratio;

/* Define the union u_dhd0_ctrl */
typedef union {
    /* Define the struct bits */
    struct {


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART22_H_ */
