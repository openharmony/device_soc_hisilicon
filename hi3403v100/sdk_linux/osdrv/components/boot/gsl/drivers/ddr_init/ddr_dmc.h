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

#ifndef DDR_DMC_H
#define DDR_DMC_H

#include "ddr_training_custom.h"

#define DDR_DDRC_S14_CONFIG
/* ******* DMC ************************* */
/* base address: DDR_REG_BASE_DMC0 DDR_REG_BASE_DMC1 */
/* register offset address */

#define DDR_DMC_CFG_DDRMODE   0x50

#define ddr_dmc_cfg_rnkvol(dmc_rnks) (0x60 + ((dmc_rnks) << 2)) /* bit operation , 2 : dmc_rnks */

/* storing data bus width. [00]8bit, [01]16bit, [10]32bit, [11]64bit */
#define DMC_MEM_WIDTH_MASK          0x3
#define DMC_DDRMODE_RANK_MASK       0x3 /* [21:20] rank */

/* register bit */
#define DMC_MEM_WIDTH_BIT       4  /* storing data bus width */
#define DMC_DDRMODE_RANK_BIT    20 /* [21:20] rank */

/* memory width */
#define MEM_WIDTH_16BIT  0x1 /* 16bit */
#define MEM_WIDTH_32BIT  0x2 /* 32bit */

#ifndef DDR_RANK_NUM
#define DDR_RANK_NUM 2 /* rank number */
#endif

/* ******* AXI ************************* */
/* *
 * DMC -- PHY
 * /
 * DDRT -- AXI
 * \
 * DMC -- PHY
 */
/* base address: DDR_REG_BASE_AXI */
/* register offset address */
#define DDR_AXI_ACTION         0x20  /* module action */
#define SPECIAL_INTLV_EN_BIT   3 /* special_intlv_en */
#define DDR_AXI_CHSEL_REMAP    0x40  /* Channel selection remapping */
#define DDR_AXI_REGION0_MAP1    0x100 /* Address area mapping 1 register 0 */
#define DDR_AXI_REGION1_MAP1    0x110 /* Address area mapping 1 register 1 */
#define DDR_AXI_REGION0_ATTRIB1 0x104 /* Address Area Attributes 1 region 0 */
#define DDR_AXI_REGION1_ATTRIB1 0x114 /* Address Area Attributes 1 region 1 */
#define DDR_AXI_REGION0_MAP2_C    0x120 /* Address area mapping 2 register 0 */
#define DDR_AXI_REGION0_MAP2    0x804 /* Address area mapping 2 register 0 */
#define DDR_AXI_REGION1_MAP2    0x814 /* Address area mapping 2 register 1 */
#define DDR_AXI_REGION0_ATTRIB2 0x808 /* Address Area Attributes 2 rigion 0 */
#define DDR_AXI_REGION1_ATTRIB2 0x818 /* Address Area Attributes 2 rigion 1 */

/* QOS */
#define qos_ddrc_cfg_rnkvol(chs, rnks)  (0x4634 + ((chs) << 4) + ((rnks) << 2)) /* 4:channels 2:ranks */

#endif /* DDR_DMC_H */
