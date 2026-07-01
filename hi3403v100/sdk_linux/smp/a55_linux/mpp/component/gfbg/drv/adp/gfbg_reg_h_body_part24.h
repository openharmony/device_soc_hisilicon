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

#ifndef SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART24_H_
#define SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART24_H_

} u_dhd0_ccdoimgmod;

/* Define the union u_dhd0_ccdoposmskh */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int p32_en : 1; /* [0]  */
        unsigned int p33_en : 1; /* [1]  */
        unsigned int p34_en : 1; /* [2]  */
        unsigned int p35_en : 1; /* [3]  */
        unsigned int p36_en : 1; /* [4]  */
        unsigned int p37_en : 1; /* [5]  */
        unsigned int p38_en : 1; /* [6]  */
        unsigned int p39_en : 1; /* [7]  */
        unsigned int p40_en : 1; /* [8]  */
        unsigned int p41_en : 1; /* [9]  */
        unsigned int p42_en : 1; /* [10]  */
        unsigned int p43_en : 1; /* [11]  */
        unsigned int p44_en : 1; /* [12]  */
        unsigned int p45_en : 1; /* [13]  */
        unsigned int p46_en : 1; /* [14]  */
        unsigned int p47_en : 1; /* [15]  */
        unsigned int p48_en : 1; /* [16]  */
        unsigned int p49_en : 1; /* [17]  */
        unsigned int p50_en : 1; /* [18]  */
        unsigned int p51_en : 1; /* [19]  */
        unsigned int p52_en : 1; /* [20]  */
        unsigned int p53_en : 1; /* [21]  */
        unsigned int p54_en : 1; /* [22]  */
        unsigned int p55_en : 1; /* [23]  */
        unsigned int p56_en : 1; /* [24]  */
        unsigned int p57_en : 1; /* [25]  */
        unsigned int p58_en : 1; /* [26]  */
        unsigned int p59_en : 1; /* [27]  */
        unsigned int p60_en : 1; /* [28]  */
        unsigned int p61_en : 1; /* [29]  */
        unsigned int p62_en : 1; /* [30]  */
        unsigned int p63_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_ccdoposmskh;

/* Define the union u_dhd0_ccdoposmskl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int p0_en : 1;  /* [0]  */
        unsigned int p1_en : 1;  /* [1]  */
        unsigned int p2_en : 1;  /* [2]  */
        unsigned int p3_en : 1;  /* [3]  */
        unsigned int p4_en : 1;  /* [4]  */
        unsigned int p5_en : 1;  /* [5]  */
        unsigned int p6_en : 1;  /* [6]  */
        unsigned int p7_en : 1;  /* [7]  */
        unsigned int p8_en : 1;  /* [8]  */
        unsigned int p9_en : 1;  /* [9]  */
        unsigned int p10_en : 1; /* [10]  */
        unsigned int p11_en : 1; /* [11]  */
        unsigned int p12_en : 1; /* [12]  */
        unsigned int p13_en : 1; /* [13]  */
        unsigned int p14_en : 1; /* [14]  */
        unsigned int p15_en : 1; /* [15]  */
        unsigned int p16_en : 1; /* [16]  */
        unsigned int p17_en : 1; /* [17]  */
        unsigned int p18_en : 1; /* [18]  */
        unsigned int p19_en : 1; /* [19]  */
        unsigned int p20_en : 1; /* [20]  */
        unsigned int p21_en : 1; /* [21]  */
        unsigned int p22_en : 1; /* [22]  */
        unsigned int p23_en : 1; /* [23]  */
        unsigned int p24_en : 1; /* [24]  */
        unsigned int p25_en : 1; /* [25]  */
        unsigned int p26_en : 1; /* [26]  */
        unsigned int p27_en : 1; /* [27]  */
        unsigned int p28_en : 1; /* [28]  */
        unsigned int p29_en : 1; /* [29]  */
        unsigned int p30_en : 1; /* [30]  */
        unsigned int p31_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_ccdoposmskl;

/* Define the union u_dhd0_dacdet1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int vdac_det_high : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;     /* [15..10]  */
        unsigned int det_line : 10;      /* [25..16]  */
        unsigned int reserved_1 : 6;     /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_dacdet1;

/* Define the union u_dhd0_dacdet2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int det_pixel_sta : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;     /* [15..11]  */
        unsigned int det_pixel_wid : 11; /* [26..16]  */
        unsigned int reserved_1 : 4;     /* [30..27]  */
        unsigned int vdac_det_en : 1;    /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_dacdet2;

/* Define the union u_dhd0_ccd_info1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int img_mode : 7;    /* [6..0]  */
        unsigned int img_right : 1;   /* [7]  */
        unsigned int img_id : 2;      /* [9..8]  */
        unsigned int reserved_0 : 1;  /* [10]  */
        unsigned int ccd_en : 1;      /* [11]  */
        unsigned int reserved_1 : 20; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_ccd_info1;

/* Define the union u_dhd0_ccd_info2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int p32_en : 1; /* [0]  */
        unsigned int p33_en : 1; /* [1]  */
        unsigned int p34_en : 1; /* [2]  */
        unsigned int p35_en : 1; /* [3]  */
        unsigned int p36_en : 1; /* [4]  */
        unsigned int p37_en : 1; /* [5]  */
        unsigned int p38_en : 1; /* [6]  */
        unsigned int p39_en : 1; /* [7]  */
        unsigned int p40_en : 1; /* [8]  */
        unsigned int p41_en : 1; /* [9]  */
        unsigned int p42_en : 1; /* [10]  */
        unsigned int p43_en : 1; /* [11]  */
        unsigned int p44_en : 1; /* [12]  */
        unsigned int p45_en : 1; /* [13]  */
        unsigned int p46_en : 1; /* [14]  */
        unsigned int p47_en : 1; /* [15]  */
        unsigned int p48_en : 1; /* [16]  */
        unsigned int p49_en : 1; /* [17]  */
        unsigned int p50_en : 1; /* [18]  */
        unsigned int p51_en : 1; /* [19]  */
        unsigned int p52_en : 1; /* [20]  */
        unsigned int p53_en : 1; /* [21]  */
        unsigned int p54_en : 1; /* [22]  */
        unsigned int p55_en : 1; /* [23]  */
        unsigned int p56_en : 1; /* [24]  */
        unsigned int p57_en : 1; /* [25]  */
        unsigned int p58_en : 1; /* [26]  */
        unsigned int p59_en : 1; /* [27]  */
        unsigned int p60_en : 1; /* [28]  */
        unsigned int p61_en : 1; /* [29]  */
        unsigned int p62_en : 1; /* [30]  */
        unsigned int p63_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_ccd_info2;

/* Define the union u_dhd0_ccd_info3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int p0_en : 1;  /* [0]  */
        unsigned int p1_en : 1;  /* [1]  */
        unsigned int p2_en : 1;  /* [2]  */
        unsigned int p3_en : 1;  /* [3]  */
        unsigned int p4_en : 1;  /* [4]  */
        unsigned int p5_en : 1;  /* [5]  */
        unsigned int p6_en : 1;  /* [6]  */
        unsigned int p7_en : 1;  /* [7]  */
        unsigned int p8_en : 1;  /* [8]  */
        unsigned int p9_en : 1;  /* [9]  */
        unsigned int p10_en : 1; /* [10]  */
        unsigned int p11_en : 1; /* [11]  */
        unsigned int p12_en : 1; /* [12]  */
        unsigned int p13_en : 1; /* [13]  */
        unsigned int p14_en : 1; /* [14]  */
        unsigned int p15_en : 1; /* [15]  */
        unsigned int p16_en : 1; /* [16]  */
        unsigned int p17_en : 1; /* [17]  */
        unsigned int p18_en : 1; /* [18]  */
        unsigned int p19_en : 1; /* [19]  */
        unsigned int p20_en : 1; /* [20]  */
        unsigned int p21_en : 1; /* [21]  */
        unsigned int p22_en : 1; /* [22]  */
        unsigned int p23_en : 1; /* [23]  */
        unsigned int p24_en : 1; /* [24]  */
        unsigned int p25_en : 1; /* [25]  */
        unsigned int p26_en : 1; /* [26]  */
        unsigned int p27_en : 1; /* [27]  */
        unsigned int p28_en : 1; /* [28]  */
        unsigned int p29_en : 1; /* [29]  */
        unsigned int p30_en : 1; /* [30]  */
        unsigned int p31_en : 1; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_dhd0_ccd_info3;

/* Define the union u_intf_hdmi_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int intf_422_en : 1;   /* [0]  */
        unsigned int intf_420_en : 1;   /* [1]  */
        unsigned int intf_420_mode : 2; /* [3..2]  */
        unsigned int hdmi_mode : 2;     /* [5..4]  */
        unsigned int reserved_0 : 26;   /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf_hdmi_ctrl;

/* Define the union u_intf_hdmi_upd */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_intf_hdmi_upd;

/* Define the union u_intf_hdmi_sync_inv */
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
} u_intf_hdmi_sync_inv;

/* Define the union u_hdmi_intf_chksum_high */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int r0_sum_high : 8; /* [7..0]  */
        unsigned int g0_sum_high : 8; /* [15..8]  */
        unsigned int b0_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hdmi_intf_chksum_high;

/* Define the union u_hdmi_intf1_chksum_high */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int r1_sum_high : 8; /* [7..0]  */
        unsigned int g1_sum_high : 8; /* [15..8]  */
        unsigned int b1_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hdmi_intf1_chksum_high;

/* Define the union u_hdmi_hfir_coef0 */
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
} u_hdmi_hfir_coef0;

/* Define the union u_hdmi_hfir_coef1 */
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
} u_hdmi_hfir_coef1;

/* Define the union u_hdmi_hfir_coef2 */
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
} u_hdmi_hfir_coef2;

/* Define the union u_hdmi_hfir_coef3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int hfir_coef6 : 10; /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hdmi_hfir_coef3;

/* Define the union u_hdmi_csc_idc */
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
} u_hdmi_csc_idc;

/* Define the union u_hdmi_csc_odc */
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
} u_hdmi_csc_odc;

/* Define the union u_hdmi_csc_iodc */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hdmi_csc_iodc;

/* Define the union u_hdmi_csc_p0 */
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
} u_hdmi_csc_p0;

/* Define the union u_hdmi_csc_p1 */
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
} u_hdmi_csc_p1;

/* Define the union u_hdmi_csc_p2 */
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
} u_hdmi_csc_p2;

/* Define the union u_hdmi_csc_p3 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cscp20 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp21 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */


#endif /* SMP_A55_LINUX_MPP_COMPONENT_GFBG_DRV_ADP_GFBG_REG_H_BODY_PART24_H_ */
