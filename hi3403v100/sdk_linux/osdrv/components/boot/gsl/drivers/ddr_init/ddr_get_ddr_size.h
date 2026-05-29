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

#ifndef DDR_GET_DDR_SIZE_H
#define DDR_GET_DDR_SIZE_H

#include "ddr_dmc.h"

#define WINDING_STEP                 0x4000 /* unit:KB, equal to 16MB */
#define WINDING_STEP_FOR_DDR4_2DIE   0x2 /* unit:KB, equal to 2KB */
#define DDR_MAX_CAPAT                0x800000 /* unit: KB, equal to 8GB */
#define DDR_MAX_CAPAT_FOR_DDR4_2DIE  0x10 /* unit:KB, equal to 16KB */
#define DDR4_WINDING_POSITION1       0x4 /* unit:KB, equal to 4KB */
#define DDR4_WINDING_POSITION2       0x8 /* unit:KB, equal to 8KB */

#define DDR_STORE_NUM  4 /* 1:4Byte */

#define DDR_WINDING_NUM1  0x12345678
#define DDR_WINDING_NUM2  0xedcba987

struct ddr_data {
    unsigned int reg_val[DDR_STORE_NUM];
};

#endif /* DDR_GET_DDR_SIZE_H */
