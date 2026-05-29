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

#ifndef KLAD_REG_BASE_H
#define KLAD_REG_BASE_H

/* rkp register base addr and offset */
#define RKP_REG_BASE_ADDR_PHY       0x10111000
#define RKP_REG_BASE_ADDR_SIZE      0x1000
#define RKP_DBG_STAT                0x4
#define RKP_INT_EN                  0x50
#define RKP_INT_RAW                 0x54
#define RKP_INT                     0x58
#define RKP_SEC_CFG                 0x74
#define RKP_SW_REG                  0x78
#define RKP_CALC_CTRL               0x7C
#define RKP_ERROR                   0x80

/* klad register base addr and offset */
#define HKL_REG_BASE_ADDR_PHY       0x10110000
#define HKL_REG_BASE_ADDR_SIZE      0x1000
#define KL_DATA_IN_0                0x00
#define KL_DATA_IN_1                0x04
#define KL_DATA_IN_2                0x08
#define KL_DATA_IN_3                0x0C
#define KL_KEY_ADDR                 0x10
#define KL_KEY_CFG                  0x14
#define KL_KEY_SEC_CFG              0x18
#define KL_STATE                    0x30
#define KL_ERROR                    0x38
#define KC_ERROR                    0x3C
#define KL_INT_EN                   0x40
#define KL_INT_RAW                  0x44
#define KL_INT                      0x48
#define KL_LOCK_CTRL                0x74
#define KL_UNLOCK_CTRL              0x78
#define KL_COM_LOCK_INFO            0x7C
#define KL_COM_LOCK_STATUS          0x80
#define KL_COM_CTRL                 0x84
#define KL_CLR_CTRL                 0x438

#endif /* KLAD_REG_BASE_H */
