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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART25_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART25_H_

        unsigned int paraup_mode : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_paraup;

/* define the union reg_dhd0_sync_inv */
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
} reg_dhd0_sync_inv;

/* define the union reg_dhd0_clk_dv_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int intf_clk_mux : 1;        /* [0]  */
        unsigned int intf_dv_mux : 1;         /* [1]  */
        unsigned int no_active_area_pos : 16; /* [17..2]  */
        unsigned int reserved_0 : 14;         /* [31..18]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_clk_dv_ctrl;

/* define the union reg_dhd0_rgb_fix_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int fix_b : 10;      /* [9..0]  */
        unsigned int fix_g : 10;      /* [19..10]  */
        unsigned int fix_r : 10;      /* [29..20]  */
        unsigned int rgb_fix_mux : 1; /* [30]  */
        unsigned int reserved_0 : 1;  /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_rgb_fix_ctrl;

/* define the union reg_dhd0_lockcfg */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int measure_en : 1;     /* [0]  */
        unsigned int lock_cnt_en : 1;    /* [1]  */
        unsigned int vdp_measure_en : 1; /* [2]  */
        unsigned int reserved_0 : 29;    /* [31..3]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_lockcfg;

/* define the union reg_dhd0_intf_chksum_high1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int r0_sum_high : 8; /* [7..0]  */
        unsigned int g0_sum_high : 8; /* [15..8]  */
        unsigned int b0_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_intf_chksum_high1;

/* define the union reg_dhd0_intf_chksum_high2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int r1_sum_high : 8; /* [7..0]  */
        unsigned int g1_sum_high : 8; /* [15..8]  */
        unsigned int b1_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_intf_chksum_high2;

/* define the union reg_dhd0_state */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vback_blank : 1;  /* [0]  */
        unsigned int vblank : 1;       /* [1]  */
        unsigned int bottom_field : 1; /* [2]  */
        unsigned int vcnt : 13;        /* [15..3]  */
        unsigned int count_int : 8;    /* [23..16]  */
        unsigned int dhd_even : 1;     /* [24]  */
        unsigned int reserved_0 : 7;   /* [31..25]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_state;

/* define the union reg_dhd0_uf_state */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ud_first_cnt : 13; /* [12..0]  */
        unsigned int reserved_0 : 3;    /* [15..13]  */
        unsigned int start_pos : 8;     /* [23..16]  */
        unsigned int reserved_1 : 8;    /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_uf_state;

/* define the union reg_vo_mux */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mipi_sel : 4;    /* [3..0]  */
        unsigned int lcd_sel : 4;     /* [7..4]  */
        unsigned int bt_sel : 4;      /* [11..8]  */
        unsigned int sddate_sel : 4;  /* [15..12]  */
        unsigned int hdmi_sel : 4;    /* [19..16]  */
        unsigned int hdmi1_sel : 4;   /* [23..20]  */
        unsigned int vga_sel : 4;     /* [27..24]  */
        unsigned int digital_sel : 4; /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vo_mux;

/* define the union reg_vo_mux_sync */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int sync_dv : 1;        /* [0]  */
        unsigned int sync_hsync : 1;     /* [1]  */
        unsigned int sync_vsync : 1;     /* [2]  */
        unsigned int sync_field : 1;     /* [3]  */
        unsigned int reserved_0 : 27;    /* [30..4]  */
        unsigned int sync_test_mode : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vo_mux_sync;

/* define the union reg_vo_mux_data */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vomux_data : 30; /* [29..0]  */
        unsigned int reserved_0 : 2;  /* [31..30]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_vo_mux_data;

/* define the union reg_dhd0_vsync_te_state */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vsync_te_start_sta : 8;  /* [7..0]  */
        unsigned int vsync_te_start_sta1 : 8; /* [15..8]  */
        unsigned int vsync_te_end_sta : 8;    /* [23..16]  */
        unsigned int reserved_0 : 8;          /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_vsync_te_state;

/* define the union reg_dhd0_vsync_te_state1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vsync_te_vfb : 16;   /* [15..0]  */
        unsigned int vsync_te_width : 16; /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_vsync_te_state1;

/* define the union reg_dhd0_ccdoimgmod */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int img_mode : 7;   /* [6..0]  */
        unsigned int img_right : 1;  /* [7]  */
        unsigned int img_id : 2;     /* [9..8]  */
        unsigned int slave_mode : 1; /* [10]  */
        unsigned int ccd_en : 1;     /* [11]  */
        unsigned int reserved_0 : 4; /* [15..12]  */
        unsigned int vbi_pos : 8;    /* [23..16]  */
        unsigned int reserved_1 : 8; /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_ccdoimgmod;

/* define the union reg_dhd0_ccdoposmskh */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_ccdoposmskh;

/* define the union reg_dhd0_ccdoposmskl */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_ccdoposmskl;

/* define the union reg_dhd0_dacdet1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int vdac_det_high : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;     /* [15..10]  */
        unsigned int det_line : 10;      /* [25..16]  */
        unsigned int reserved_1 : 6;     /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_dacdet1;

/* define the union reg_dhd0_dacdet2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int det_pixel_sta : 11; /* [10..0]  */
        unsigned int reserved_0 : 5;     /* [15..11]  */
        unsigned int det_pixel_wid : 11; /* [26..16]  */
        unsigned int reserved_1 : 4;     /* [30..27]  */
        unsigned int vdac_det_en : 1;    /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_dacdet2;

/* define the union reg_dhd0_ccd_info1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int img_mode : 7;    /* [6..0]  */
        unsigned int img_right : 1;   /* [7]  */
        unsigned int img_id : 2;      /* [9..8]  */
        unsigned int reserved_0 : 1;  /* [10]  */
        unsigned int ccd_en : 1;      /* [11]  */
        unsigned int reserved_1 : 20; /* [31..12]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_ccd_info1;

/* define the union reg_dhd0_ccd_info2 */
typedef union {
    /* define the struct bits */
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

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_ccd_info2;

/* define the union reg_dhd0_ccd_info3 */
typedef union {
    /* define the struct bits */
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


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART25_H_ */
