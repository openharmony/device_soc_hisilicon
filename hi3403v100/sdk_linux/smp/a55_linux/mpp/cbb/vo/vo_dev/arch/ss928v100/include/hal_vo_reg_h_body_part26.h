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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART26_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART26_H_

        unsigned int p29_en : 1; /* [29]  */
        unsigned int p30_en : 1; /* [30]  */
        unsigned int p31_en : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_dhd0_ccd_info3;

/* define the union reg_intf_hdmi_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int intf_422_en : 1;   /* [0]  */
        unsigned int intf_420_en : 1;   /* [1]  */
        unsigned int intf_420_mode : 2; /* [3..2]  */
        unsigned int hdmi_mode : 2;     /* [5..4]  */
        unsigned int reserved_0 : 26;   /* [31..6]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_intf_hdmi_ctrl;

/* define the union reg_intf_hdmi_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_intf_hdmi_upd;

/* define the union reg_intf_hdmi_sync_inv */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dv_inv : 1;      /* [0]  */
        unsigned int hs_inv : 1;      /* [1]  */
        unsigned int vs_inv : 1;      /* [2]  */
        unsigned int f_inv : 1;       /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_intf_hdmi_sync_inv;

/* define the union reg_hdmi_intf_chksum_high */
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
} reg_hdmi_intf_chksum_high;

/* define the union reg_hdmi_intf1_chksum_high */
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
} reg_hdmi_intf1_chksum_high;

/* define the union reg_hdmi_hfir_coef0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hfir_coef0 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;  /* [15..10]  */
        unsigned int hfir_coef1 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;  /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hdmi_hfir_coef0;

/* define the union reg_hdmi_hfir_coef1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hfir_coef2 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;  /* [15..10]  */
        unsigned int hfir_coef3 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;  /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hdmi_hfir_coef1;

/* define the union reg_hdmi_hfir_coef2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hfir_coef4 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;  /* [15..10]  */
        unsigned int hfir_coef5 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;  /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hdmi_hfir_coef2;

/* define the union reg_hdmi_hfir_coef3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hfir_coef6 : 10; /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hdmi_hfir_coef3;

/* define the union reg_hdmi_csc_idc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc0 : 11;   /* [10..0]  */
        unsigned int cscidc1 : 11;   /* [21..11]  */
        unsigned int csc_en : 1;     /* [22]  */
        unsigned int reserved_0 : 9; /* [31..23]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hdmi_csc_idc;

/* define the union reg_hdmi_csc_odc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscodc0 : 11;      /* [10..0]  */
        unsigned int cscodc1 : 11;      /* [21..11]  */
        unsigned int csc_sign_mode : 1; /* [22]  */
        unsigned int reserved_0 : 9;    /* [31..23]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hdmi_csc_odc;

/* define the union reg_hdmi_csc_iodc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hdmi_csc_iodc;

/* define the union reg_hdmi_csc_p0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp00 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp01 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hdmi_csc_p0;

/* define the union reg_hdmi_csc_p1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp02 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp10 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hdmi_csc_p1;

/* define the union reg_hdmi_csc_p2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp11 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp12 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hdmi_csc_p2;

/* define the union reg_hdmi_csc_p3 */
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
} reg_hdmi_csc_p3;

/* define the union reg_hdmi_csc_p4 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp22 : 15;     /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_hdmi_csc_p4;

/* define the union reg_intf_mipi_del_ctrl */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int intf_422_en : 1;   /* [0]  */
        unsigned int intf_420_en : 1;   /* [1]  */
        unsigned int intf_420_mode : 2; /* [3..2]  */
        unsigned int reserved_0 : 28;   /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_intf_mipi_del_ctrl;

/* define the union reg_intf_mipi_del_upd */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int regup : 1;       /* [0]  */
        unsigned int reserved_0 : 31; /* [31..1]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_intf_mipi_del_upd;

/* define the union reg_intf_mipi_del_sync_inv */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int dv_inv : 1;      /* [0]  */
        unsigned int hs_inv : 1;      /* [1]  */
        unsigned int vs_inv : 1;      /* [2]  */
        unsigned int f_inv : 1;       /* [3]  */
        unsigned int reserved_0 : 28; /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_intf_mipi_del_sync_inv;

/* define the union reg_mipi_del_intf_chksum_high */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int b0_sum_high : 8; /* [7..0]  */
        unsigned int g0_sum_high : 8; /* [15..8]  */
        unsigned int r0_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_del_intf_chksum_high;

/* define the union reg_mipi_del_intf1_chksum_high */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int b1_sum_high : 8; /* [7..0]  */
        unsigned int g1_sum_high : 8; /* [15..8]  */
        unsigned int r1_sum_high : 8; /* [23..16]  */
        unsigned int reserved_0 : 8;  /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_del_intf1_chksum_high;

/* define the union reg_mipi_del_hfir_coef0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hfir_coef0 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;  /* [15..10]  */
        unsigned int hfir_coef1 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;  /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_del_hfir_coef0;

/* define the union reg_mipi_del_hfir_coef1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hfir_coef2 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;  /* [15..10]  */
        unsigned int hfir_coef3 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;  /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_del_hfir_coef1;

/* define the union reg_mipi_del_hfir_coef2 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hfir_coef4 : 10; /* [9..0]  */
        unsigned int reserved_0 : 6;  /* [15..10]  */
        unsigned int hfir_coef5 : 10; /* [25..16]  */
        unsigned int reserved_1 : 6;  /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_del_hfir_coef2;

/* define the union reg_mipi_del_hfir_coef3 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int hfir_coef6 : 10; /* [9..0]  */
        unsigned int reserved_0 : 22; /* [31..10]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_del_hfir_coef3;

/* define the union reg_mipi_del_csc_idc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc0 : 11;   /* [10..0]  */
        unsigned int cscidc1 : 11;   /* [21..11]  */
        unsigned int csc_en : 1;     /* [22]  */
        unsigned int reserved_0 : 9; /* [31..23]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_del_csc_idc;

/* define the union reg_mipi_del_csc_odc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscodc0 : 11;      /* [10..0]  */
        unsigned int cscodc1 : 11;      /* [21..11]  */
        unsigned int csc_sign_mode : 1; /* [22]  */
        unsigned int reserved_0 : 9;    /* [31..23]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_del_csc_odc;

/* define the union reg_mipi_del_csc_iodc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_del_csc_iodc;

/* define the union reg_mipi_del_csc_p0 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp00 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp01 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_mipi_del_csc_p0;

/* define the union reg_mipi_del_csc_p1 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp02 : 15;    /* [14..0]  */
        unsigned int reserved_0 : 1; /* [15]  */
        unsigned int cscp10 : 15;    /* [30..16]  */
        unsigned int reserved_1 : 1; /* [31]  */
    } bits;


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART26_H_ */
