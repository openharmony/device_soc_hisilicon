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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART39_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART39_H_

        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp10 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_csc_p1;

/* Define the union u_g3_csc_p2 */
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
} u_g3_csc_p2;

/* Define the union u_g3_csc_p3 */
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
} u_g3_csc_p3;

/* Define the union u_g3_csc_p4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp22 : 15;     /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_csc_p4;

/* Define the union u_g3_csc1_idc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc0 : 11;   /* [10..0]  */
        unsigned int cscidc1 : 11;   /* [21..11]  */
        unsigned int csc_en : 1;     /* [22]  */
        unsigned int csc_mode : 3;   /* [25..23]  */
        unsigned int reserved_0 : 6; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_csc1_idc;

/* Define the union u_g3_csc1_odc */
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
} u_g3_csc1_odc;

/* Define the union u_g3_csc1_iodc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_csc1_iodc;

/* Define the union u_g3_csc1_p0 */
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
} u_g3_csc1_p0;

/* Define the union u_g3_csc1_p1 */
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
} u_g3_csc1_p1;

/* Define the union u_g3_csc1_p2 */
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
} u_g3_csc1_p2;

/* Define the union u_g3_csc1_p3 */
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
} u_g3_csc1_p3;

/* Define the union u_g3_csc1_p4 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp22 : 15;     /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_g3_csc1_p4;

/* Define the union u_v0_cvfir_vinfo */
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
} u_v0_cvfir_vinfo;

/* Define the union u_v0_cvfir_vsp */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vratio : 16;    /* [15..0]  */
        unsigned int reserved_0 : 1; /* [16]  */
        unsigned int reserved_1 : 8; /* [24..17]  */
        unsigned int cvfir_mode : 1; /* [25]  */
        unsigned int reserved_2 : 1; /* [26]  */
        unsigned int reserved_3 : 1; /* [27]  */
        unsigned int cvmid_en : 1;   /* [28]  */
        unsigned int reserved_4 : 1; /* [29]  */
        unsigned int cvfir_en : 1;   /* [30]  */
        unsigned int reserved_5 : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_cvfir_vsp;

/* Define the union u_v0_cvfir_voffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vchroma_offset : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;     /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_cvfir_voffset;

/* Define the union u_v0_cvfir_vboffset */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vbchroma_offset : 16; /* [15..0]  */
        unsigned int reserved_0 : 16;      /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_cvfir_vboffset;

/* Define the union u_v0_cvfir_vcoef0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vccoef02 : 10;  /* [9..0]  */
        unsigned int vccoef01 : 10;  /* [19..10]  */
        unsigned int vccoef00 : 10;  /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_cvfir_vcoef0;

/* Define the union u_v0_cvfir_vcoef1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vccoef11 : 10;  /* [9..0]  */
        unsigned int vccoef10 : 10;  /* [19..10]  */
        unsigned int vccoef03 : 10;  /* [29..20]  */
        unsigned int reserved_0 : 2; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_cvfir_vcoef1;

/* Define the union u_v0_cvfir_vcoef2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vccoef13 : 10;   /* [9..0]  */
        unsigned int vccoef12 : 10;   /* [19..10]  */
        unsigned int reserved_0 : 12; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_v0_cvfir_vcoef2;

/* Define the union u_gfx_osd_glb_info */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int dcmp_en : 1;       /* [0]  */
        unsigned int is_lossless : 1;   /* [1]  */
        unsigned int is_lossless_a : 1; /* [2]  */
        unsigned int cmp_mode : 1;      /* [3]  */
        unsigned int source_mode : 3;   /* [6..4]  */
        unsigned int tpred_en : 1;      /* [7]  */
        unsigned int reserved_0 : 24;   /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_osd_glb_info;

/* Define the union u_gfx_osd_frame_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int frame_width : 13;  /* [12..0]  */
        unsigned int reserved_0 : 3;    /* [15..13]  */
        unsigned int frame_height : 13; /* [28..16]  */
        unsigned int reserved_1 : 3;    /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_osd_frame_size;

/* Define the union u_gfx_osd_dbg_reg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 30; /* [29..0]  */
        unsigned int dcmp_err0 : 1;   /* [30]  */
        unsigned int reserved_1 : 1;  /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_osd_dbg_reg;

/* Define the union u_gfx_osd_dbg_reg1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved_0 : 30; /* [29..0]  */
        unsigned int dcmp_err1 : 1;   /* [30]  */
        unsigned int reserved_1 : 1;  /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gfx_osd_dbg_reg1;

/* Define the global struct */
typedef struct {
    volatile u_voctrl voctrl;                                                     /* 0x0 */
    volatile u_vointsta vointsta;                                                 /* 0x4 */
    volatile u_vomskintsta vomskintsta;                                           /* 0x8 */
    volatile u_vointmsk vointmsk;                                                 /* 0xc */
    volatile u_vodebug vodebug;                                                   /* 0x10 */
    volatile u_vointsta1 vointsta1;                                               /* 0x14 */
    volatile u_vomskintsta1 vomskintsta1;                                         /* 0x18 */
    volatile u_vointmsk1 vointmsk1;                                               /* 0x1c */
    volatile unsigned int vdpversion1;                                            /* 0x20 */
    volatile unsigned int vdpversion2;                                            /* 0x24 */
    volatile u_volowpower_ctrl volowpower_ctrl;                                   /* 0x28 */
    volatile u_voufsta voufsta;                                                   /* 0x2c */
    volatile u_voufclr voufclr;                                                   /* 0x30 */
    volatile u_vointproc_tim vointproc_tim;                                       /* 0x34 */
    volatile unsigned int vofpgatest;                                             /* 0x38 */
    volatile unsigned int reserved_0[3];                                          /* 0x3c~0x44 */
    volatile u_volowpower_ctrl1 volowpower_ctrl1;                                 /* 0x48 */
    volatile u_vofpgadef vofpgadef;                                               /* 0x4c */
    volatile u_volowpower_ctrl2 volowpower_ctrl2;                                 /* 0x50 */
    volatile u_volowpower_ctrl3 volowpower_ctrl3;                                 /* 0x54 */
    volatile unsigned int reserved_1[43];                                         /* 43:0x58~0x100 */
    volatile u_vomux_dac vomux_dac;                                               /* 0x104 */
    volatile u_vomux_testsync vomux_testsync;                                     /* 0x108 */
    volatile u_vomux_testdata vomux_testdata;                                     /* 0x10c */
    volatile unsigned int reserved_2[4];                                          /* 4:0x110~0x11c */
    volatile u_vo_dac_ctrl vo_dac_ctrl;                                           /* 0x120 */
    volatile u_vo_dac_otp vo_dac_otp;                                             /* 0x124 */
    volatile unsigned int reserved_3[2];                                          /* 2:0x128~0x12c */
    volatile u_vo_dac0_ctrl vo_dac0_ctrl;                                         /* 0x130 */
    volatile u_vo_dac1_ctrl vo_dac1_ctrl;                                         /* 0x134 */
    volatile u_vo_dac2_ctrl vo_dac2_ctrl;                                         /* 0x138 */
    volatile u_vo_dac3_ctrl vo_dac3_ctrl;                                         /* 0x13c */
    volatile u_vo_dac_stat0 vo_dac_stat0;                                         /* 0x140 */
    volatile unsigned int reserved_4[111];                                        /* 111:0x144~0x2fc */
    volatile u_cbm_bkg1 cbm_bkg1;                                                 /* 0x300 */
    volatile unsigned int reserved_5;                                             /* 0x304 */
    volatile u_cbm_mix1 cbm_mix1;                                                 /* 0x308 */
    volatile unsigned int reserved_6[14];                                         /* 14:0x30c~0x340 */
    volatile u_wbc_bmp_thd wbc_bmp_thd;                                           /* 0x344 */
    volatile unsigned int reserved_7[2];                                          /* 2:0x348~0x34c */
    volatile unsigned int cbm1_lay0_debug;                                        /* 0x350 */
    volatile unsigned int cbm1_lay1_debug;                                        /* 0x354 */
    volatile unsigned int cbm1_lay2_debug;                                        /* 0x358 */
    volatile unsigned int cbm1_lay3_debug;                                        /* 0x35c */
    volatile unsigned int cbm1_lay4_debug;                                        /* 0x360 */
    volatile unsigned int cbm1_lay0_last_debug;                                   /* 0x364 */
    volatile unsigned int cbm1_lay1_last_debug;                                   /* 0x368 */
    volatile unsigned int cbm1_lay2_last_debug;                                   /* 0x36c */
    volatile unsigned int cbm1_lay3_last_debug;                                   /* 0x370 */
    volatile unsigned int cbm1_lay4_last_debug;                                   /* 0x374 */
    volatile unsigned int reserved_8[2];                                          /* 2:0x378~0x37c */
    volatile u_cbm_bkg2 cbm_bkg2;                                                 /* 0x380 */
    volatile unsigned int reserved_9;                                             /* 0x384 */
    volatile u_cbm_mix2 cbm_mix2;                                                 /* 0x388 */
    volatile unsigned int reserved_10[14];                                        /* 14:0x38c~0x3c0 */
    volatile u_hc_bmp_thd hc_bmp_thd;                                             /* 0x3c4 */
    volatile unsigned int reserved_11[2];                                         /* 2:0x3c8~0x3cc */
    volatile unsigned int cbm2_lay0_debug;                                        /* 0x3d0 */
    volatile unsigned int cbm2_lay1_debug;                                        /* 0x3d4 */
    volatile unsigned int cbm2_lay2_debug;                                        /* offset 0x3d0 + 0x08 */
    volatile unsigned int cbm2_lay3_debug;                                        /* 0x3dc */
    volatile unsigned int cbm2_lay4_debug;                                        /* 0x3e0 */
    volatile unsigned int cbm2_lay0_last_debug;                                   /* 0x3e4 */
    volatile unsigned int cbm2_lay1_last_debug;                                   /* 0x3e8 */
    volatile unsigned int cbm2_lay2_last_debug;                                   /* 0x3ec */
    volatile unsigned int cbm2_lay3_last_debug;                                   /* 0x3f0 */
    volatile unsigned int cbm2_lay4_last_debug;                                   /* 0x3f4 */
    volatile unsigned int reserved_12[2];                                         /* 2:0x3f8~0x3fc */
    volatile u_cbm_bkg3 cbm_bkg3;                                                 /* 0x400 */
    volatile unsigned int reserved_13;                                            /* 0x404 */
    volatile u_cbm_mix3 cbm_mix3;                                                 /* 0x408 */
    volatile unsigned int reserved_14[17];                                        /* 17:0x40c~0x44c */
    volatile unsigned int cbm3_lay0_debug;                                        /* 0x450 */
    volatile unsigned int cbm3_lay1_debug;                                        /* 0x454 */
    volatile unsigned int cbm3_lay2_debug;                                        /* 0x458 */
    volatile unsigned int cbm3_lay3_debug;                                        /* 0x45c */
    volatile unsigned int cbm3_lay4_debug;                                        /* 0x460 */
    volatile unsigned int cbm3_lay0_last_debug;                                   /* 0x464 */
    volatile unsigned int cbm3_lay1_last_debug;                                   /* 0x468 */
    volatile unsigned int cbm3_lay2_last_debug;                                   /* 0x46c */
    volatile unsigned int cbm3_lay3_last_debug;                                   /* 0x470 */
    volatile unsigned int cbm3_lay4_last_debug;                                   /* 0x474 */
    volatile unsigned int reserved_15[98];                                        /* 98:0x478~0x5fc */
    volatile u_mixv0_bkg mixv0_bkg;                                               /* 0x600 */
    volatile unsigned int reserved_16;                                            /* 0x604 */
    volatile u_mixv0_mix mixv0_mix;                                               /* 0x608 */
    volatile unsigned int reserved_17[189];                                       /* 189:0x60c~0x8fc */
    volatile u_mixg0_bkg mixg0_bkg;                                               /* 0x900 */
    volatile u_mixg0_bkalpha mixg0_bkalpha;                                       /* 0x904 */
    volatile u_mixg0_mix mixg0_mix;                                               /* 0x908 */
    volatile unsigned int reserved_18[189];                                       /* 189:0x90c~0xbfc */
    volatile u_link_ctrl link_ctrl;                                               /* 0xc00 */
    volatile unsigned int reserved_19[63];                                        /* 63:0xc04~0xcfc */
    volatile u_vpss_ctrl vpss_ctrl;                                               /* 0xd00 */
    volatile u_vpss_miscellaneous vpss_miscellaneous;                             /* 0xd04 */
    volatile u_vpss_ftconfig vpss_ftconfig;                                       /* 0xd08 */
    volatile unsigned int reserved_20[5];                                         /* 5:0xd0c~0xd1c */
    volatile unsigned int vpss_version;                                           /* 0xd20 */
    volatile unsigned int vpss_debug0;                                            /* 0xd24 */
    volatile unsigned int vpss_debug1;                                            /* 0xd28 */
    volatile unsigned int vpss_debug2;                                            /* 0xd2c */
    volatile unsigned int vpss_debug3;                                            /* 0xd30 */
    volatile unsigned int vpss_debug4;                                            /* 0xd34 */
    volatile unsigned int vpss_debug5;                                            /* 0xd38 */
    volatile unsigned int vpss_debug6;                                            /* 0xd3c */
    volatile unsigned int reserved_21[48];                                        /* 48:0xd40~0xdfc */
    volatile unsigned int para_haddr_vhd_chn00;                                   /* 0xe00 */
    volatile unsigned int para_addr_vhd_chn00;                                    /* 0xe04 */
    volatile unsigned int para_haddr_vhd_chn01;                                   /* 0xe08 */
    volatile unsigned int para_addr_vhd_chn01;                                    /* 0xe0c */
    volatile unsigned int para_haddr_vhd_chn02;                                   /* 0xe10 */
    volatile unsigned int para_addr_vhd_chn02;                                    /* 0xe14 */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART39_H_ */
