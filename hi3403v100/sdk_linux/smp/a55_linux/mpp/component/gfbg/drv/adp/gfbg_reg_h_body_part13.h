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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART13_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART13_H_

        unsigned int reserved_0 : 21;     /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_ot_pp_csc2_odc1;

/* Define the union u_g0_ot_pp_csc2_odc2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc2_odc2 : 11; /* [10..0]  */
        unsigned int reserved_0 : 21;     /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_ot_pp_csc2_odc2;

/* Define the union u_g0_ot_pp_csc2_min_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc2_min_y : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_ot_pp_csc2_min_y;

/* Define the union u_g0_ot_pp_csc2_min_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc2_min_c : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_ot_pp_csc2_min_c;

/* Define the union u_g0_ot_pp_csc2_max_y */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc2_max_y : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_ot_pp_csc2_max_y;

/* Define the union u_g0_ot_pp_csc2_max_c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ot_pp_csc2_max_c : 10; /* [9..0]  */
        unsigned int reserved_0 : 22;      /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_ot_pp_csc2_max_c;

/* Define the union u_g0_ot_pp_csc_ink_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ink_en : 1;       /* [0]  */
        unsigned int ink_sel : 1;      /* [1]  */
        unsigned int data_fmt : 1;     /* [2]  */
        unsigned int cross_enable : 1; /* [3]  */
        unsigned int color_mode : 2;   /* [5..4]  */
        unsigned int reserved_0 : 26;  /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_ot_pp_csc_ink_ctrl;

/* Define the union u_g0_ot_pp_csc_ink_pos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int x_pos : 16; /* [15..0]  */
        unsigned int y_pos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_ot_pp_csc_ink_pos;

/* Define the union u_g0_dof_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 31; /* [30..0]  */
        unsigned int dof_en : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_dof_ctrl;

/* Define the union u_g0_dof_step */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int left_step : 8;   /* [7..0]  */
        unsigned int right_step : 8;  /* [15..8]  */
        unsigned int reserved_0 : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_dof_step;

/* Define the union u_g0_dof_bkg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dof_bk_cr : 10; /* [9..0]  */
        unsigned int dof_bk_cb : 10; /* [19..10]  */
        unsigned int dof_bk_y : 10;  /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_dof_bkg;

/* Define the union u_g0_dof_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dof_bk_alpha : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;  /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_dof_alpha;

/* Define the union u_g0_zme_hinfo */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int out_width : 16;  /* [15..0]  */
        unsigned int ck_gt_en : 1;    /* [16]  */
        unsigned int reserved_0 : 15; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_hinfo;

/* Define the union u_g0_zme_hsp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hratio : 24;      /* [23..0]  */
        unsigned int hfir_order : 1;   /* [24]  */
        unsigned int ahfir_mode : 1;   /* [25]  */
        unsigned int lhfir_mode : 1;   /* [26]  */
        unsigned int reserved_0 : 1;   /* [27]  */
        unsigned int chfir_mid_en : 1; /* [28]  */
        unsigned int lhfir_mid_en : 1; /* [29]  */
        unsigned int ahfir_mid_en : 1; /* [30]  */
        unsigned int hfir_en : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_hsp;

/* Define the union u_g0_zme_hloffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lhfir_offset : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;    /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_hloffset;

/* Define the union u_g0_zme_hcoffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int chfir_offset : 24; /* [23..0]  */
        unsigned int reserved_0 : 8;    /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_hcoffset;

/* Define the union u_g0_zme_coef_ren */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 1;     /* [0]  */
        unsigned int apb_g0_vf_lren : 1; /* [1]  */
        unsigned int reserved_1 : 1;     /* [2]  */
        unsigned int apb_g0_hf_lren : 1; /* [3]  */
        unsigned int reserved_2 : 28;    /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_coef_ren;

/* Define the union u_g0_zme_coef_rdata */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int apb_vhd_coef_raddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;        /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_coef_rdata;

/* Define the union u_g0_zme_vinfo */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int out_height : 16; /* [15..0]  */
        unsigned int reserved_0 : 2;  /* [17..16]  */
        unsigned int out_pro : 1;     /* [18]  */
        unsigned int reserved_1 : 13; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_vinfo;

/* Define the union u_g0_zme_vsp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vratio : 16;      /* [15..0]  */
        unsigned int reserved_0 : 9;   /* [24..16]  */
        unsigned int vafir_mode : 1;   /* [25]  */
        unsigned int lvfir_mode : 1;   /* [26]  */
        unsigned int reserved_1 : 1;   /* [27]  */
        unsigned int cvfir_mid_en : 1; /* [28]  */
        unsigned int lvfir_mid_en : 1; /* [29]  */
        unsigned int avfir_mid_en : 1; /* [30]  */
        unsigned int vfir_en : 1;      /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_vsp;

/* Define the union u_g0_zme_voffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbtm_offset : 16; /* [15..0]  */
        unsigned int vtp_offset : 16;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g0_zme_voffset;

/* Define the union u_g1_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int galpha : 8;       /* [7..0]  */
        unsigned int reserved_0 : 19;  /* [26..8]  */
        unsigned int g1_depremult : 1; /* [27]  */
        unsigned int rupd_field : 1;   /* [28]  */
        unsigned int rgup_mode : 1;    /* [29]  */
        unsigned int nosec_flag : 1;   /* [30]  */
        unsigned int surface_en : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g1_ctrl;

/* Define the union u_g1_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g1_upd;

/* Define the union u_g1_0reso_read */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ow : 16; /* [15..0]  */
        unsigned int oh : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g1_0reso_read;

/* Define the union u_g1_ireso */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int iw : 16; /* [15..0]  */
        unsigned int ih : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g1_ireso;

/* Define the union u_g1_dfpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xfpos : 16; /* [15..0]  */
        unsigned int disp_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g1_dfpos;

/* Define the union u_g1_dlpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int disp_xlpos : 16; /* [15..0]  */
        unsigned int disp_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g1_dlpos;

/* Define the union u_g1_vfpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xfpos : 16; /* [15..0]  */
        unsigned int video_yfpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g1_vfpos;

/* Define the union u_g1_vlpos */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int video_xlpos : 16; /* [15..0]  */
        unsigned int video_ylpos : 16; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g1_vlpos;

/* Define the union u_g1_bk */
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
} u_g1_bk;

/* Define the union u_g1_alpha */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbk_alpha : 8;   /* [7..0]  */
        unsigned int reserved_0 : 24; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g1_alpha;

/* Define the union u_g1_mute_bk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_cr : 10;   /* [9..0]  */
        unsigned int mute_cb : 10;   /* [19..10]  */
        unsigned int mute_y : 10;    /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g1_mute_bk;

/* Define the union u_g1_lbox_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mute_en : 1;     /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g1_lbox_ctrl;

/* Define the union u_g1_ot_pp_csc_ctrl */
typedef union {
    /* Define the struct bits */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART13_H_ */
