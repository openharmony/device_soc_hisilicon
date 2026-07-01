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

#ifndef MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART41_H_
#define MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART41_H_

    /* define the struct bits */
    struct {
        unsigned int vstr_pos_0 : 12; /* [11..0]  */
        unsigned int reserved_0 : 4;  /* [15..12]  */
        unsigned int vend_pos_0 : 12; /* [27..16]  */
        unsigned int reserved_1 : 4;  /* [31..28]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_g4_osb_ctrl3_box_0;

/* define the union reg_v1_csc_idc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc0 : 11;     /* [10..0]  */
        unsigned int cscidc1 : 11;     /* [21..11]  */
        unsigned int csc_en : 1;       /* [22]  */
        unsigned int csc_mode : 3;     /* [25..23]  */
        unsigned int csc_ck_gt_en : 1; /* [26]  */
        unsigned int reserved_0 : 5;   /* [31..27]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_csc_idc;

/* define the union reg_v1_csc_odc */
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
} reg_v1_csc_odc;

/* define the union reg_v1_csc_iodc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_csc_iodc;

/* define the union reg_v1_csc_p0 */
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
} reg_v1_csc_p0;

/* define the union reg_v1_csc_p1 */
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
} reg_v1_csc_p1;

/* define the union reg_v1_csc_p2 */
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
} reg_v1_csc_p2;

/* define the union reg_v1_csc_p3 */
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
} reg_v1_csc_p3;

/* define the union reg_v1_csc_p4 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp22 : 15;     /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_csc_p4;

/* define the union reg_v1_csc1_idc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc0 : 11;   /* [10..0]  */
        unsigned int cscidc1 : 11;   /* [21..11]  */
        unsigned int csc_en : 1;     /* [22]  */
        unsigned int csc_mode : 3;   /* [25..23]  */
        unsigned int reserved_0 : 6; /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_csc1_idc;

/* define the union reg_v1_csc1_odc */
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
} reg_v1_csc1_odc;

/* define the union reg_v1_csc1_iodc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_csc1_iodc;

/* define the union reg_v1_csc1_p0 */
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
} reg_v1_csc1_p0;

/* define the union reg_v1_csc1_p1 */
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
} reg_v1_csc1_p1;

/* define the union reg_v1_csc1_p2 */
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
} reg_v1_csc1_p2;

/* define the union reg_v1_csc1_p3 */
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
} reg_v1_csc1_p3;

/* define the union reg_v1_csc1_p4 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp22 : 15;     /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v1_csc1_p4;

/* define the union reg_v2_csc_idc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc0 : 11;     /* [10..0]  */
        unsigned int cscidc1 : 11;     /* [21..11]  */
        unsigned int csc_en : 1;       /* [22]  */
        unsigned int csc_mode : 3;     /* [25..23]  */
        unsigned int csc_ck_gt_en : 1; /* [26]  */
        unsigned int reserved_0 : 5;   /* [31..27]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v2_csc_idc;

/* define the union reg_v2_csc_odc */
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
} reg_v2_csc_odc;

/* define the union reg_v2_csc_iodc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v2_csc_iodc;

/* define the union reg_v2_csc_p0 */
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
} reg_v2_csc_p0;

/* define the union reg_v2_csc_p1 */
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
} reg_v2_csc_p1;

/* define the union reg_v2_csc_p2 */
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
} reg_v2_csc_p2;

/* define the union reg_v2_csc_p3 */
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
} reg_v2_csc_p3;

/* define the union reg_v2_csc_p4 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscp22 : 15;     /* [14..0]  */
        unsigned int reserved_0 : 17; /* [31..15]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v2_csc_p4;

/* define the union reg_v2_csc1_idc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc0 : 11;   /* [10..0]  */
        unsigned int cscidc1 : 11;   /* [21..11]  */
        unsigned int csc_en : 1;     /* [22]  */
        unsigned int csc_mode : 3;   /* [25..23]  */
        unsigned int reserved_0 : 6; /* [31..26]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v2_csc1_idc;

/* define the union reg_v2_csc1_odc */
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
} reg_v2_csc1_odc;

/* define the union reg_v2_csc1_iodc */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int cscidc2 : 11;    /* [10..0]  */
        unsigned int cscodc2 : 11;    /* [21..11]  */
        unsigned int reserved_0 : 10; /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} reg_v2_csc1_iodc;

/* define the union reg_v2_csc1_p0 */
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
} reg_v2_csc1_p0;

/* define the union reg_v2_csc1_p1 */
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
} reg_v2_csc1_p1;

/* define the union reg_v2_csc1_p2 */
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
} reg_v2_csc1_p2;

/* define the union reg_v2_csc1_p3 */
typedef union {


#endif /* MPP_CBB_VO_VO_DEV_ARCH_SS928V100_INCLUDE_HAL_VO_REG_H_BODY_PART41_H_ */
