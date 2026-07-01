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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART43_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART43_H_

        unsigned int cscp11 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp12 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_csc1_p2;

/* define the union reg_g3_csc1_p3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp20 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp21 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_csc1_p3;

/* define the union reg_g3_csc1_p4 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp22 : 15;     /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g3_csc1_p4;

/* define the union reg_v0_zme_hinfo */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int out_width : 16;    /* [15..0]  */
        unsigned int hzme_ck_gt_en : 1; /* [16]  */
        unsigned int reserved_0 : 15;   /* [31..17]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_hinfo;

/* define the union reg_v0_zme_hsp */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hratio : 24;    /* [23..0]  */
        unsigned int hfir_order : 1; /* [24]  */
        unsigned int chfir_mode : 1; /* [25]  */
        unsigned int lhfir_mode : 1; /* [26]  */
        unsigned int non_lnr_en : 1; /* [27]  */
        unsigned int chmid_en : 1;   /* [28]  */
        unsigned int lhmid_en : 1;   /* [29]  */
        unsigned int chfir_en : 1;   /* [30]  */
        unsigned int lhfir_en : 1;   /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_hsp;

/* define the union reg_v0_zme_hloffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int lhfir_offset : 28; /* [27..0]  */
        unsigned int reserved_0 : 4;    /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_hloffset;

/* define the union reg_v0_zme_hcoffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int chfir_offset : 28; /* [27..0]  */
        unsigned int reserved_0 : 4;    /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_hcoffset;

/* define the union reg_v0_zme_hzone0delta */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int zone0_delta : 22; /* [21..0]  */
        unsigned int reserved_0 : 10;  /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_hzone0delta;

/* define the union reg_v0_zme_hzone2delta */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int zone2_delta : 22; /* [21..0]  */
        unsigned int reserved_0 : 10;  /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_hzone2delta;

/* define the union reg_v0_zme_hzoneend */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int zone0_end : 12; /* [11..0]  */
        unsigned int zone1_end : 12; /* [23..12]  */
        unsigned int reserved_0 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_hzoneend;

/* define the union reg_v0_zme_hl_shootctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hl_coring : 8;         /* [7..0]  */
        unsigned int hl_gain : 6;           /* [13..8]  */
        unsigned int hl_coringadj_en : 1;   /* [14]  */
        unsigned int hl_flatdect_mode : 1;  /* [15]  */
        unsigned int hl_shootctrl_mode : 1; /* [16]  */
        unsigned int reserved_0 : 1;        /* [17]  */
        unsigned int hl_shootctrl_en : 1;   /* [18]  */
        unsigned int reserved_1 : 13;       /* [31..19]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_hl_shootctrl;

/* define the union reg_v0_zme_hc_shootctrl */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_hc_shootctrl;

/* define the union reg_v0_zme_hcoef_ren */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int apb_vhd_hf_cren : 1; /* [0]  */
        unsigned int apb_vhd_hf_lren : 1; /* [1]  */
        unsigned int reserved_0 : 30;     /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_hcoef_ren;

/* define the union reg_v0_zme_hcoef_rdata */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int apb_vhd_hcoef_raddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;         /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_hcoef_rdata;

/* define the union reg_v0_zme_vinfo */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int out_height : 16;   /* [15..0]  */
        unsigned int out_fmt : 2;       /* [17..16]  */
        unsigned int out_pro : 1;       /* [18]  */
        unsigned int vzme_ck_gt_en : 1; /* [19]  */
        unsigned int reserved_0 : 12;   /* [31..20]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_vinfo;

/* define the union reg_v0_zme_vsp */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_vsp;

/* define the union reg_v0_zme_voffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vchroma_offset : 16; /* [15..0]  */
        unsigned int vluma_offset : 16;   /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_voffset;

/* define the union reg_v0_zme_vboffset */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vbchroma_offset : 16; /* [15..0]  */
        unsigned int vbluma_offset : 16;   /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_vboffset;

/* define the union reg_v0_zme_vl_shootctrl */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_vl_shootctrl;

/* define the union reg_v0_zme_vc_shootctrl */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_vc_shootctrl;

/* define the union reg_v0_zme_vcoef_ren */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int apb_vhd_vf_cren : 1; /* [0]  */
        unsigned int apb_vhd_vf_lren : 1; /* [1]  */
        unsigned int reserved_0 : 30;     /* [31..2]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_vcoef_ren;

/* define the union reg_v0_zme_vcoef_rdata */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int apb_vhd_vcoef_raddr : 8; /* [7..0]  */
        unsigned int reserved_0 : 24;         /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v0_zme_vcoef_rdata;

/* define the union reg_gfx_osd_glb_info */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dcmp_en : 1;       /* [0]  */
        unsigned int is_lossless : 1;   /* [1]  */
        unsigned int is_lossless_a : 1; /* [2]  */
        unsigned int cmp_mode : 1;      /* [3]  */
        unsigned int source_mode : 3;   /* [6..4]  */
        unsigned int tpred_en : 1;      /* [7]  */
        unsigned int reserved_0 : 24;   /* [31..8]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_osd_glb_info;

/* define the union reg_gfx_osd_frame_size */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int frame_width : 13;  /* [12..0]  */
        unsigned int reserved_0 : 3;    /* [15..13]  */
        unsigned int frame_height : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;    /* [31..29]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_osd_frame_size;

/* define the union reg_gfx_osd_dbg_reg */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 30; /* [29..0]  */
        unsigned int dcmp_err0 : 1;   /* [30]  */
        unsigned int reserved_1 : 1;  /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_osd_dbg_reg;

/* define the union reg_gfx_osd_dbg_reg1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int reserved_0 : 30; /* [29..0]  */
        unsigned int dcmp_err1 : 1;   /* [30]  */
        unsigned int reserved_1 : 1;  /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_gfx_osd_dbg_reg1;

// ==============================================================================
/* define the global struct */
typedef struct {
    volatile reg_voctrl voctrl;                                                     /* 0x0 */
    volatile reg_vointsta vointsta;                                                 /* 0x4 */
    volatile reg_vomskintsta vomskintsta;                                           /* 0x8 */
    volatile reg_vointmsk vointmsk;                                                 /* 0xc */
    volatile reg_vodebug vodebug;                                                   /* 0x10 */
    volatile reg_vointsta1 vointsta1;                                               /* 0x14 */
    volatile reg_vomskintsta1 vomskintsta1;                                         /* 0x18 */
    volatile reg_vointmsk1 vointmsk1;                                               /* 0x1c */
    volatile unsigned int vdpversion1;                                              /* 0x20 */
    volatile unsigned int vdpversion2;                                              /* 0x24 */
    volatile reg_volowpower_ctrl volowpower_ctrl;                                   /* 0x28 */
    volatile reg_voufsta voufsta;                                                   /* 0x2c */
    volatile reg_voufclr voufclr;                                                   /* 0x30 */
    volatile reg_vointproc_tim vointproc_tim;                                       /* 0x34 */
    volatile unsigned int vofpgatest;                                               /* 0x38 */
    volatile unsigned int reserved_0[3];                                            /* 0x3c~0x44  3 regs */
    volatile reg_volowpower_ctrl1 volowpower_ctrl1;                                 /* 0x48 */
    volatile reg_vofpgadef vofpgadef;                                               /* 0x4c */
    volatile reg_volowpower_ctrl2 volowpower_ctrl2;                                 /* 0x50 */
    volatile reg_volowpower_ctrl3 volowpower_ctrl3;                                 /* 0x54 */
    volatile unsigned int reserved_1[43];                                           /* 0x58~0x100 43 regs */
    volatile reg_vomux_dac vomux_dac;                                               /* 0x104 */
    volatile reg_vomux_testsync vomux_testsync;                                     /* 0x108 */
    volatile reg_vomux_testdata vomux_testdata;                                     /* 0x10c */
    volatile unsigned int reserved_2[4];                                            /* 0x110~0x11c 4 regs  */
    volatile reg_vo_dac_ctrl vo_dac_ctrl;                                           /* 0x120 */
    volatile reg_vo_dac_otp vo_dac_otp;                                             /* 0x124 */
    volatile unsigned int reserved_3[2];                                            /* 0x128~0x12c 2 regs  */
    volatile reg_vo_dac0_ctrl vo_dac0_ctrl;                                         /* 0x130 */
    volatile reg_vo_dac1_ctrl vo_dac1_ctrl;                                         /* 0x134 */
    volatile reg_vo_dac2_ctrl vo_dac2_ctrl;                                         /* 0x138 */
    volatile reg_vo_dac3_ctrl vo_dac3_ctrl;                                         /* 0x13c */
    volatile reg_vo_dac_stat0 vo_dac_stat0;                                         /* 0x140 */
    volatile unsigned int reserved_4[111];                                          /* 0x144~0x2fc 111 regs */
    volatile reg_cbm_bkg1 cbm_bkg1;                                                 /* 0x300 */
    volatile unsigned int reserved_5;                                               /* 0x304 */
    volatile reg_cbm_mix1 cbm_mix1;                                                 /* 0x308 */
    volatile unsigned int reserved_6[14];                                           /* 0x30c~0x340 14 regs */
    volatile reg_wbc_bmp_thd wbc_bmp_thd;                                           /* 0x344 */
    volatile unsigned int reserved_7[2];                                            /* 0x348~0x34c 2 regs */
    volatile unsigned int cbm1_lay0_debug;                                          /* 0x350 */
    volatile unsigned int cbm1_lay1_debug;                                          /* 0x354 */
    volatile unsigned int cbm1_lay2_debug;                                          /* 0x358 */
    volatile unsigned int cbm1_lay3_debug;                                          /* 0x35c */
    volatile unsigned int cbm1_lay4_debug;                                          /* 0x360 */
    volatile unsigned int cbm1_lay0_last_debug;                                     /* 0x364 */
    volatile unsigned int cbm1_lay1_last_debug;                                     /* 0x368 */
    volatile unsigned int cbm1_lay2_last_debug;                                     /* 0x36c */
    volatile unsigned int cbm1_lay3_last_debug;                                     /* 0x370 */
    volatile unsigned int cbm1_lay4_last_debug;                                     /* 0x374 */
    volatile unsigned int reserved_8[2];                                            /* 0x378~0x37c 2 regs */
    volatile reg_cbm_bkg2 cbm_bkg2;                                                 /* 0x380 */
    volatile unsigned int reserved_9;                                               /* 0x384 */
    volatile reg_cbm_mix2 cbm_mix2;                                                 /* 0x388 */
    volatile unsigned int reserved_10[14];                                          /* 0x38c~0x3c0 14 regs */
    volatile reg_hc_bmp_thd hc_bmp_thd;                                             /* 0x3c4 */
    volatile unsigned int reserved_11[2];                                           /* 0x3c8~0x3cc 2 regs */
    volatile unsigned int cbm2_lay0_debug;                                          /* 0x3d0 */
    volatile unsigned int cbm2_lay1_debug;                                          /* 0x3d4 */
    volatile unsigned int cbm2_lay2_debug;                                          /* offset 0x3d0 + 0x08 */


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART43_H_ */
