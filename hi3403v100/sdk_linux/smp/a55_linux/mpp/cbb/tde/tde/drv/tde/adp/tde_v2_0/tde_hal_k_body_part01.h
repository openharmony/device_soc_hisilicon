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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART01_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART01_H_


#line 1 "tde_hal_k.c"
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

#include "tde_hal_k.h"

#ifdef CONFIG_USE_SYS_CONFIG
#include "sys_ext.h"
#endif
#include "securec.h"
#include "ot_common.h"
#include "ot_math.h"

#include "tde_hal.h"
#include "wmalloc.h"
#include "tde_adp.h"
#include "tde_osictl_k.h"

#define UPDATE_SIZE 64

#define NO_HSCALE_STEP 0x100000
#define NO_VSCALE_STEP 0x1000

#define drv_tde_node_blend_mode(node, a, b)                 \
        do {                                                \
            (node)->src1_cbmpara.bits.s1_blendmode = (a);   \
            (node)->src2_cbmpara.bits.s2_blendmode = (b);   \
        } while (0)

/* R/W register's encapsulation */
#define tde_read_reg(base, offset) (*((volatile td_u32 *)(((td_void *)(base)) + (offset))))
#define tde_write_reg(base, offset, val) (*((volatile td_u32 *)(((td_void *)(base)) + (offset))) = (val))
/* TDE register's Address range */
#define TDE_REG_SIZE 0x4100

/* Step range's type by algorithm team */
#define TDE_RESIZE_PARA_AREA_0 4096  /* 1.0 */
#define TDE_RESIZE_PARA_AREA_1 5461  /* 0.75 */
#define TDE_RESIZE_PARA_AREA_2 8192  /* 0.5 */
#define TDE_RESIZE_PARA_AREA_3 12412 /* 0.33 */
#define TDE_RESIZE_PARA_AREA_4 16384 /* 0.25 */

/* coefficient table rangle of 8*32 area */
#define TDE_RESIZE_8X32_AREA_0 1048576 /* 1.0 */
#define TDE_RESIZE_8X32_AREA_1 1398101 /* 0.75 */
#define TDE_RESIZE_8X32_AREA_2 2097152 /* 0.5 */
#define TDE_RESIZE_8X32_AREA_3 3177503 /* 0.33 */
#define TDE_RESIZE_8X32_AREA_4 4194304 /* 0.25 */

/* Colorkey mode of CMOS chip */
#define TDE_COLORKEY_IGNORE   2
#define TDE_COLORKEY_AREA_OUT 1
#define TDE_COLORKEY_AREA_IN  0

/* Aq control mode */
#define TDE_AQ_CTRL_COMP_LIST 0x0 /* start next AQ list, after complete current list's operations */
#define TDE_AQ_CTRL_COMP_LINE 0x4 /* start next AQ list, after complete current node and line */

#define TDE_MAX_READ_STATUS_TIME 10

#define TDE_TWENTYFOUR_BITS_SHIFT 24
#define TDE_EIGHT_BITS_SHIFT      8
#define TDE_SIXTEEN_BITS_SHIFT    16

#define TDE_PARA_TABLE_SIZE 32
#define TDE_PARA_TABLE_ORG_SIZE 40
#define TDE_PARA_TABLE_NUM 12

#define TDE_PARA_VTABLE_ORG_SIZE (17 * 2)

#define TDE_PARA_HTABLE_ORG_SIZE (17 * 3)
#define TDE_PARA_HTABLE_SIZE     (64 * 4)
#define TDE_PARA_HTABLE_NUM      7

#define TDE_PARA_VTABLE_SIZE (64 * 4)
#define TDE_PARA_VTABLE_NUM 7

/* vertical filter coefficient */
td_u32 g_org_vf_coef[TDE_PARA_VTABLE_ORG_SIZE * TDE_PARA_HTABLE_NUM] = {
    /* ratio is greater than 1.0 */
    0x3f0000,
    0x0,
    0x3f0000,
    0x1,
    0x3f03fe,
    0x3,
    0x3f03fd,
    0x3ff0005,
    0x3f03fc,
    0x3ff0006,
    0x3d03fc,
    0x3ff0008,
    0x3c03fb,
    0x3ff000a,
    0x3b03fb,
    0x3fe000c,
    0x3803fb,
    0x3fe000f,
    0x3603fb,
    0x3fe0011,
    0x3403fa,
    0x3fe0014,
    0x3203fb,
    0x3fd0016,
    0x2f03fb,
    0x3fd0019,
    0x2c03fb,
    0x3fd001c,
    0x2a03fb,
    0x3fc001f,
    0x2703fb,
    0x3fc0022,
    0x2403fc,
    0x3fc0024,

    /* ratio is equal to 1.0 */
    0x3f0000,
    0x0,
    0x3f0000,
    0x1,
    0x3f03fe,
    0x3,
    0x3f03fd,
    0x3ff0005,
    0x3f03fc,
    0x3ff0006,
    0x3d03fc,
    0x3ff0008,
    0x3c03fb,
    0x3ff000a,
    0x3b03fb,
    0x3fe000c,
    0x3803fb,
    0x3fe000f,
    0x3603fb,
    0x3fe0011,
    0x3403fa,
    0x3fe0014,
    0x3203fb,
    0x3fd0016,
    0x2f03fb,
    0x3fd0019,
    0x2c03fb,
    0x3fd001c,
    0x2a03fb,
    0x3fc001f,
    0x2703fb,
    0x3fc0022,
    0x2403fc,
    0x3fc0024,

    /* ratio is greater than 0.75 */
    0x3f0000,
    0x0,
    0x3f0000,
    0x1,
    0x3f03fe,
    0x3,
    0x3f03fd,
    0x3ff0005,
    0x3f03fc,
    0x3ff0006,
    0x3d03fc,
    0x3ff0008,
    0x3c03fb,
    0x3ff000a,
    0x3b03fb,
    0x3fe000c,
    0x3803fb,
    0x3fe000f,
    0x3603fb,
    0x3fe0011,
    0x3403fa,
    0x3fe0014,
    0x3203fb,
    0x3fd0016,
    0x2f03fb,
    0x3fd0019,
    0x2c03fb,
    0x3fd001c,
    0x2a03fb,
    0x3fc001f,
    0x2703fb,
    0x3fc0022,
    0x2403fc,
    0x3fc0024,

    /* ratio is not less than 0.5 */
    0x31000a,
    0x3fb000a,
    0x330008,
    0x3fa000b,
    0x320007,
    0x3fa000d,
    0x330005,
    0x3f9000f,
    0x330004,
    0x3f90010,
    0x320003,
    0x3f90012,
    0x320001,
    0x3f90014,
    0x320000,
    0x3f80016,
    0x3103ff,
    0x3f80018,
    0x3003fe,
    0x3f8001a,
    0x2f03fd,
    0x3f8001c,
    0x2e03fc,
    0x3f8001e,
    0x2c03fc,
    0x3f80020,
    0x2c03fb,
    0x3f80021,
    0x2a03fa,
    0x3f90023,
    0x2803fa,
    0x3f90025,
    0x2703f9,
    0x3f90027,

    /* ratio is greater than 0.33 */
    0x170011,
    0x70011,
    0x170011,
    0x70011,
    0x160010,
    0x80012,
    0x160010,
    0x80012,
    0x160010,
    0x80012,
    0x17000f,
    0x80012,
    0x16000f,
    0x90012,
    0x15000f,
    0x90013,
    0x16000e,
    0x90013,
    0x16000e,
    0x90013,
    0x15000e,
    0xa0013,
    0x16000d,
    0xa0013,
    0x15000d,
    0xa0014,
    0x14000d,
    0xb0014,
    0x15000c,
    0xb0014,
    0x15000c,
    0xb0014,
    0x15000c,
    0xb0014,

    /* ratio is greater than 0.25 */
    0x130011,
    0xb0011,
    0x130011,
    0xb0011,
    0x130010,
    0xc0011,
    0x130010,
    0xc0011,
    0x130010,
    0xc0011,
    0x130010,
    0xc0011,
    0x130010,
    0xc0011,
    0x14000f,
    0xc0011,
    0x13000f,
    0xd0011,
    0x12000f,
    0xd0012,
    0x12000f,
    0xd0012,
    0x12000f,
    0xd0012,
    0x12000f,
    0xd0012,
    0x13000e,
    0xd0012,
    0x12000e,
    0xe0012,
    0x12000e,
    0xe0012,
    0x12000e,
    0xe0012,

    /* ratio is less than 0.25 */
    0x110011,
    0xd0011,
    0x120010,
    0xd0011,
    0x120010,
    0xd0011,
    0x120010,
    0xd0011,
    0x120010,
    0xd0011,
    0x120010,
    0xd0011,
    0x120010,
    0xd0011,
    0x110010,
    0xe0011,
    0x110010,
    0xe0011,
    0x12000f,
    0xe0011,
    0x12000f,
    0xe0011,
    0x12000f,
    0xe0011,
    0x12000f,
    0xe0011,
    0x12000f,
    0xe0011,
    0x12000f,
    0xe0011,
    0x12000f,
    0xe0011,
    0x11000f,
    0xf0011,
};

/* anti-flash filter coefficient */
td_u32 g_deflicker_vf_coef[TDE_PARA_VTABLE_ORG_SIZE * TDE_PARA_HTABLE_NUM] = {
    /* ratio is greater than 1 */
    0x1f0011,
    0x3ff0011,
    0x1f0011,
    0x3ff0011,
    0x1e0010,
    0x12,
    0x1e0010,
    0x12,
    0x1e000f,
    0x13,
    0x1e000f,
    0x13,
    0x1e000d,
    0x10014,
    0x1e000d,
    0x10014,
    0x1d000b,
    0x20016,
    0x1d000b,
    0x20016,
    0x1d000a,
    0x20017,
    0x1d000a,
    0x20017,
    0x1d0008,
    0x30018,
    0x1d0008,
    0x30018,
    0x1c0007,
    0x40019,
    0x1c0007,
    0x40019,
    0x1a0006,
    0x6001a,
    /* ratio is equal to  1 */
    0x200010,
    0x10,
    0x1f000f,
    0x10011,
    0x1f000e,
    0x10012,
    0x1f000c,
    0x20013,
    0x1f000b,
    0x20014,
    0x1f000a,
    0x20015,
    0x1e0009,
    0x30016,
    0x1e0009,
    0x30016,
    0x1e0008,
    0x30017,
    0x1d0008,
    0x40017,
    0x1d0007,
    0x40018,
    0x1c0007,
    0x40019,
    0x1c0007,
    0x40019,
    0x1c0006,
    0x4001a,
    0x1b0006,
    0x5001a,
    0x1b0005,
    0x5001b,
    0x1b0005,
    0x5001b,
    /* ratio is less than 1, and not less than 0.75 */
    0x1f0011,
    0x3ff0011,
    0x1f0011,
    0x3ff0011,
    0x1e0010,
    0x12,
    0x1e0010,
    0x12,
    0x1e000e,


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART01_H_ */
