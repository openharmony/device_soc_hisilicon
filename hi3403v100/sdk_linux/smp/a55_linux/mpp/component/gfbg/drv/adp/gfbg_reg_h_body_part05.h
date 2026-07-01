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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART05_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART05_H_

    unsigned int u32;
} u_v0_zme_hl_shootctrl;

/* Define the union u_v0_zme_hc_shootctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hc_coring : 8;         /* [7..0]  */
        unsigned int hc_gain : 6;           /* [13..8]  */
        unsigned int hc_coringadj_en : 1;   /* [14]  */
        unsigned int hc_flatdect_mode : 1;  /* [15]  */
        unsigned int hc_shootctrl_mode : 1; /* [16]  */
        unsigned int reserved_0 : 1;        /* [17]  */
        unsigned int hc_shootctrl_en : 1;   /* [18]  */
        unsigned int reserved_1 : 13;       /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_zme_hc_shootctrl;

/* Define the union u_v0_zme_hcoef_ren */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int apb_vhd_hf_cren : 1; /* [0]  */
        unsigned int apb_vhd_hf_lren : 1; /* [1]  */
        unsigned int reserved_0 : 30;     /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_zme_hcoef_ren;

/* Define the union u_v0_zme_hcoef_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int apb_vhd_hcoef_raddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;         /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_zme_hcoef_rdata;

/* Define the union u_v0_zme_vinfo */
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
} u_v0_zme_vinfo;

/* Define the union u_v0_zme_vsp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vratio : 16;      /* [15..0]  */
        unsigned int graphdet_en : 1;  /* [16]  */
        unsigned int reserved_0 : 8;   /* [24..17]  */
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
} u_v0_zme_vsp;

/* Define the union u_v0_zme_voffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vchroma_offset : 16; /* [15..0]  */
        unsigned int vluma_offset : 16;   /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_zme_voffset;

/* Define the union u_v0_zme_vboffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbchroma_offset : 16; /* [15..0]  */
        unsigned int vbluma_offset : 16;   /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_zme_vboffset;

/* Define the union u_v0_zme_vl_shootctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vl_coring : 8;         /* [7..0]  */
        unsigned int vl_gain : 6;           /* [13..8]  */
        unsigned int vl_coringadj_en : 1;   /* [14]  */
        unsigned int vl_flatdect_mode : 1;  /* [15]  */
        unsigned int vl_shootctrl_mode : 1; /* [16]  */
        unsigned int reserved_0 : 1;        /* [17]  */
        unsigned int vl_shootctrl_en : 1;   /* [18]  */
        unsigned int reserved_1 : 13;       /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_zme_vl_shootctrl;

/* Define the union u_v0_zme_vc_shootctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vc_coring : 8;         /* [7..0]  */
        unsigned int vc_gain : 6;           /* [13..8]  */
        unsigned int vc_coringadj_en : 1;   /* [14]  */
        unsigned int vc_flatdect_mode : 1;  /* [15]  */
        unsigned int vc_shootctrl_mode : 1; /* [16]  */
        unsigned int reserved_0 : 1;        /* [17]  */
        unsigned int vc_shootctrl_en : 1;   /* [18]  */
        unsigned int reserved_1 : 13;       /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_zme_vc_shootctrl;

/* Define the union u_v0_zme_vcoef_ren */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int apb_vhd_vf_cren : 1; /* [0]  */
        unsigned int apb_vhd_vf_lren : 1; /* [1]  */
        unsigned int reserved_0 : 30;     /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_zme_vcoef_ren;

/* Define the union u_v0_zme_vcoef_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int apb_vhd_vcoef_raddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;         /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_zme_vcoef_rdata;

/* Define the union u_v0_hfir_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;  /* [0]  */
        unsigned int hfir_mode : 2;   /* [2..1]  */
        unsigned int mid_en : 1;      /* [3]  */
        unsigned int ck_gt_en : 1;    /* [4]  */
        unsigned int reserved_1 : 27; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_hfir_ctrl;

/* Define the union u_v0_hfircoef01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int coef0 : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef1 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_hfircoef01;

/* Define the union u_v0_hfircoef23 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int coef2 : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef3 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_hfircoef23;

/* Define the union u_v0_hfircoef45 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int coef4 : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef5 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_hfircoef45;

/* Define the union u_v0_hfircoef67 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int coef6 : 10;     /* [9..0]  */
        unsigned int reserved_0 : 6; /* [15..10]  */
        unsigned int coef7 : 10;     /* [25..16]  */
        unsigned int reserved_1 : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_hfircoef67;

/* Define the union u_v1_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int galpha : 8;      /* [7..0]  */
        unsigned int reserved_0 : 20; /* [27..8]  */
        unsigned int rupd_field : 1;  /* [28]  */
        unsigned int rgup_mode : 1;   /* [29]  */
        unsigned int nosec_flag : 1;  /* [30]  */
        unsigned int surface_en : 1;  /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_ctrl;

/* Define the union u_v1_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_upd;

/* Define the union u_v1_0reso_read */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0]  */
        unsigned int oh : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_0reso_read;

/* Define the union u_v1_ireso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int iw : 16; /* [15..0]  */
        unsigned int ih : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_ireso;

/* Define the union u_v1_dfpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xfpos : 16; /* [15..0]  */
        unsigned int disp_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_dfpos;

/* Define the union u_v1_dlpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xlpos : 16; /* [15..0]  */
        unsigned int disp_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_dlpos;

/* Define the union u_v1_vfpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xfpos : 16; /* [15..0]  */
        unsigned int video_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_vfpos;

/* Define the union u_v1_vlpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xlpos : 16; /* [15..0]  */
        unsigned int video_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_vlpos;

/* Define the union u_v1_bk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbk_cr : 10;    /* [9..0]  */
        unsigned int vbk_cb : 10;    /* [19..10]  */
        unsigned int vbk_y : 10;     /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_bk;

/* Define the union u_v1_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbk_alpha : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_alpha;

/* Define the union u_v1_mute_bk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_cr : 10;   /* [9..0]  */
        unsigned int mute_cb : 10;   /* [19..10]  */
        unsigned int mute_y : 10;    /* [29..20]  */
        unsigned int reserved_0 : 1; /* [30]  */
        unsigned int mute_en : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_mute_bk;

/* Define the union u_v1_rimwidth */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int v0_rim_width : 5; /* [4..0]  */
        unsigned int reserved_0 : 27;  /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_rimwidth;

/* Define the union u_v1_rimcol0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int v0_rim_v0 : 10; /* [9..0]  */
        unsigned int v0_rim_u0 : 10; /* [19..10]  */
        unsigned int v0_rim_y0 : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_rimcol0;

/* Define the union u_v1_rimcol1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int v0_rim_v1 : 10; /* [9..0]  */
        unsigned int v0_rim_u1 : 10; /* [19..10]  */
        unsigned int v0_rim_y1 : 10; /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v1_rimcol1;

/* Define the union u_v1_ot_pp_csc_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc_en : 1;       /* [0]  */
        unsigned int ot_pp_csc_demo_en : 1;  /* [1]  */
        unsigned int ot_pp_csc_ck_gt_en : 1; /* [2]  */
        unsigned int reserved_0 : 29;       /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART05_H_ */
