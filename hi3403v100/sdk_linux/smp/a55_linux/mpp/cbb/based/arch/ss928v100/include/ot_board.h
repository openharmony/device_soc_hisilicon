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

#ifndef OT_BOARD_H
#define OT_BOARD_H

#include "ot_common.h"

#define DDR_BUS_FR                (310000000) /* ddr bus 频率:310M */

#define INT_TIMERINT0_1           4 /* Timer 0 and 1 */

#define DDRC0_REG_ADDR            0x11140000
#define DDRC_REGS_SIZE            0x20000

#define CRG_REGS_ADDR             0x11010000
#define CRG_REGS_ADDR_OFFSET      0x4500
#define CRG_REGS_SIZE             (0x10000 - CRG_REGS_ADDR_OFFSET)

#define SYS_REGS_ADDR             0x11020000
#define SYS_REGS_SIZE             0x4000

#define MISC_REGS_ADDR            0x11024000
#define MISC_REGS_SIZE            0x5000

#define IOCFG_REGS_ADDR           0x200F0000
#define IOCFG_REGS_SIZE           0x10000

#define VOU_REGS_ADDR             0x17A00000
#define VOU_REGS_SIZE             0x40000

#define VGS0_REGS_ADDR            0x17240000
#define VGS1_REGS_ADDR            0x17250000
#define VGS_REGS_SIZE             0x10000

#define GDC0_BASE_ADDR            0x172c0000
#define GDC1_BASE_ADDR            0x172c0000
#define GDC_REGS_SIZE             0x10000

#define VPSS_REGS_ADDR            0x17900000
#define VPSS_REGS_SIZE            0x10000

#define VI_CAP_REGS_ADDR          0x17400000

#define VDAU_REGS_ADDR            0x170c0000

#define OTP_USER_REGS_ADDR        0x10122000
#define OTP_USER_REGS_SIZE        0x2000

#define VI_ANR_MISC_REGS_ADDR     0x17920000
#define VI_ANR_MISC_REGS_SIZE     0x200

#define GFBG_SOFT_INT_ADDR        0x1202001c

#define MIPI_CTRL_REGS_ADDR       0x11310000

#define VEDU_0_REGS_ADDR          0x17140000
#define VEDU_REGS_SIZE            0x10000

#define DIS_REGS_ADDR             0x11200000
#define DIS_REGS_SIZE             0x10000

#define DIS_SOFTRST_REGS_ADDR     (0x3C + MISC_REGS_ADDR)
#define DIS_SOFTRST_REGS_SIZE     0x100

#define AVS_BASE_ADDR             0x17930000
#define AVS_REG_SIZE              0x10000

#define HEVC0_REGS_ADDR           0x11300000
#define HEVC_REGS_SIZE            0x10000

#define JPEGU_REGS_ADDR           0x171c0000
#define JPEGU_REGS_SIZE           0x10000

#define JPEGD_REGS_ADDR           (0x17180000)
#define JPEGD_REGS_SIZE           0x6BF

#define IVE_REGS_ADDR             0x17000000
#define IVE_REGS_SIZE             0x10000
#define IVE_CRG_RESET_REGS_ADDR   0x110167C0

#define FD_BASE_ADDR              0x11E00000
#define FD_REGS_SIZE              0x10000
#define FD_CRG_RESET_REGS_ADDR    0x12010070

#define DSP0_REGS_ADDR            0x16110000
#define DSP1_REGS_ADDR            0x16310000

#define DPU_REGS_ADDR             0x17030000

#define SVP_MAU_0_REGS_ADDR       0x17030000
#define SVP_MAU_0_REGS_SIZE       0x10000

#define PQP0_REGS_ADDR            0x15000000
#define PQP0_REGS_SIZE            0x10000

#define SVP_NPU0_REGS_ADDR        0x15000000
#define SVP_NPU0_REGS_SIZE        0x10000

#define AIAO_REG_ADDR             0x17c00000
#define AIAO_REG_SIZE             0x10000

#define ACODEC_REG_ADDR           0x17c40000
#define ACODEC_REG_SIZE           0x10000

/* Interrupt Request Number  */
#define VOU_IRQ_NR                191
#define VOU1_IRQ_NR               192
#define MIPI_IRQ_NR               60
#define VI_CAP0_IRQ_NR            92
#define VI_PROC0_IRQ_NR           93
#define VI_PROC1_IRQ_NR           94
#define VPSS_IRQ_NR               158
#define TDE_IRQ_NR                66
#define VGS0_IRQ_NR               170
#define VGS1_IRQ_NR               171
#define AIO_IRQ_NR                194
#define VEDU_0_IRQ_NR             183
#define JPEGU_IRQ_NR              181
#define JPEGD_IRQ_NR              179
#define HEVCU_IRQ_NR              69
#define IVE_IRQ_NR                181
#define FD_IRQ_NR                 89
#define GDC0_IRQ_NR               179
#define GDC1_IRQ_NR               179
#define DIS_IRQ_NR                83
#define AVS_IRQ_NR                184
#define SVP_MAU_IRQ_NR0           196
#define DPU_RECT_IRQ_NR           225
#define DPU_MATCH_IRQ_NR          226
#define DPU_POSTPROC_IRQ_NR       227
#define VDAU_IRQ_NR               183
#define PQP0_NS_IRQ_NR            190
#define PQP0_S_IRQ_NR             191
#define SVP_NPU0_NS_IRQ_NR        190
#define SVP_NPU0_S_IRQ_NR         191

#define SYS_PERCTL0_ADDR          (0x0 + SYS_REGS_ADDR)
#define SYS_PERCTL1_ADDR          (0x4 + SYS_REGS_ADDR)
#define SYS_PERCTL2_ADDR          (0x8 + SYS_REGS_ADDR)
#define SYS_PERCTL3_ADDR          (0xc + SYS_REGS_ADDR)
#define SYS_PERCTL4_ADDR          (0x10 + SYS_REGS_ADDR)
#define SYS_PERCTL5_ADDR          (0x14 + SYS_REGS_ADDR)
#define SYS_PERCTL6_ADDR          (0x18 + SYS_REGS_ADDR)
#define SYS_PERCTL7_ADDR          (0x1c + SYS_REGS_ADDR)
#define SYS_PERCTL8_ADDR          (0x20 + SYS_REGS_ADDR)
#define SYS_PERCTL9_ADDR          (0x24 + SYS_REGS_ADDR)
#define SYS_PERCTL10_ADDR         (0x28 + SYS_REGS_ADDR)
#define SYS_PERCTL11_ADDR         (0x2C + SYS_REGS_ADDR)
#define SYS_PERCTL12_ADDR         (0x30 + SYS_REGS_ADDR)
#define SYS_PERCTL13_ADDR         (0x34 + SYS_REGS_ADDR)
#define SYS_PERCTL14_ADDR         (0x38 + SYS_REGS_ADDR)
#define SYS_PERCTL15_ADDR         (0x3C + SYS_REGS_ADDR)
#define SYS_PERCTL16_ADDR         (0x40 + SYS_REGS_ADDR)
#define SYS_PERCTL17_ADDR         (0x44 + SYS_REGS_ADDR)
#define SYS_PERCTL18_ADDR         (0x48 + SYS_REGS_ADDR)
#define SYS_PERCTL19_ADDR         (0x4c + SYS_REGS_ADDR)
#define SYS_PERCTL20_ADDR         (0x50 + SYS_REGS_ADDR)
#define SYS_PERCTL21_ADDR         (0x54 + SYS_REGS_ADDR)
#define SYS_PERCTL22_ADDR         (0x58 + SYS_REGS_ADDR)
#define SYS_PERCTL23_ADDR         (0x5c + SYS_REGS_ADDR)
#define SYS_PERCTL24_ADDR         (0x60 + SYS_REGS_ADDR)
#define SYS_PERCTL25_ADDR         (0x64 + SYS_REGS_ADDR)
#define SYS_PERCTL26_ADDR         (0x68 + SYS_REGS_ADDR)
#define SYS_PERCTL27_ADDR         (0x6C + SYS_REGS_ADDR)
#define SYS_PERCTL28_ADDR         (0x70 + SYS_REGS_ADDR)
#define SYS_PERCTL29_ADDR         (0x74 + SYS_REGS_ADDR)
#define SYS_PERCTL30_ADDR         (0x78 + SYS_REGS_ADDR)
#define SYS_PERCTL31_ADDR         (0x7C + SYS_REGS_ADDR)
#define SYS_PERCTL32_ADDR         (0x80 + SYS_REGS_ADDR)
#define SYS_PERCTL33_ADDR         (0x84 + SYS_REGS_ADDR)
#define SYS_PERCTL34_ADDR         (0x88 + SYS_REGS_ADDR)
#define SYS_PERCTL35_ADDR         (0x8C + SYS_REGS_ADDR)
#define SYS_PERCTL36_ADDR         (0x90 + SYS_REGS_ADDR)
#define SYS_PERCTL37_ADDR         (0x94 + SYS_REGS_ADDR)
#define SYS_PERCTL38_ADDR         (0x98 + SYS_REGS_ADDR)
#define SYS_PERCTL39_ADDR         (0x9C + SYS_REGS_ADDR)
#define SYS_PERCTL40_ADDR         (0xa0 + SYS_REGS_ADDR)
#define SYS_PERCTL41_ADDR         (0xa4 + SYS_REGS_ADDR)
#define SYS_PERCTL42_ADDR         (0xa8 + SYS_REGS_ADDR)
#define SYS_PERCTL43_ADDR         (0xaC + SYS_REGS_ADDR)
#define SYS_PERCTL44_ADDR         (0xb0 + SYS_REGS_ADDR)
#define SYS_PERCTL45_ADDR         (0xb4 + SYS_REGS_ADDR)
#define SYS_PERCTL46_ADDR         (0xb8 + SYS_REGS_ADDR)
#define SYS_PERCTL47_ADDR         (0xbC + SYS_REGS_ADDR)
#define SYS_PERCTL48_ADDR         (0xc0 + SYS_REGS_ADDR)
#define SYS_PERCTL49_ADDR         (0xc4 + SYS_REGS_ADDR)
#define SYS_PERCTL50_ADDR         (0xc8 + SYS_REGS_ADDR)
#define SYS_PERCTL51_ADDR         (0xcC + SYS_REGS_ADDR)
#define SYS_PERCTL52_ADDR         (0xd0 + SYS_REGS_ADDR)
#define SYS_PERCTL53_ADDR         (0xd4 + SYS_REGS_ADDR)
#define SYS_PERCTL54_ADDR         (0xd8 + SYS_REGS_ADDR)
#define SYS_PERCTL55_ADDR         (0xdC + SYS_REGS_ADDR)
#define SYS_PERCTL56_ADDR         (0xe0 + SYS_REGS_ADDR)
#define SYS_PERCTL57_ADDR         (0xe4 + SYS_REGS_ADDR)
#define SYS_PERCTL58_ADDR         (0xe8 + SYS_REGS_ADDR)
#define SYS_PERCTL59_ADDR         (0xeC + SYS_REGS_ADDR)
#define SYS_PERCTL60_ADDR         (0xf0 + SYS_REGS_ADDR)
#define SYS_PERCTL61_ADDR         (0xf4 + SYS_REGS_ADDR)
#define SYS_PERCTL62_ADDR         (0xf8 + SYS_REGS_ADDR)
#define SYS_PERCTL63_ADDR         (0xfC + SYS_REGS_ADDR)
#define SYS_PERCTL64_ADDR         (0x100 + SYS_REGS_ADDR)
#define SYS_PERCTL65_ADDR         (0x104 + SYS_REGS_ADDR)
#define SYS_PERCTL66_ADDR         (0x108 + SYS_REGS_ADDR)
#define SYS_PERCTL67_ADDR         (0x10c + SYS_REGS_ADDR)
#define SYS_PERCTL68_ADDR         (0x110 + SYS_REGS_ADDR)
#define SYS_PERCTL69_ADDR         (0x114 + SYS_REGS_ADDR)
#define SYS_PERCTL70_ADDR         (0x118 + SYS_REGS_ADDR)
#define SYS_PERCTL71_ADDR         (0x11c + SYS_REGS_ADDR)
#define SYS_PERCTL72_ADDR         (0x120 + SYS_REGS_ADDR)
#define SYS_PERCTL73_ADDR         (0x124 + SYS_REGS_ADDR)
#define SYS_PERCTL74_ADDR         (0x128 + SYS_REGS_ADDR)
#define SYS_PERCTL75_ADDR         (0x12C + SYS_REGS_ADDR)
#define SYS_PERCTL76_ADDR         (0x130 + SYS_REGS_ADDR)
#define SYS_PERCTL77_ADDR         (0x134 + SYS_REGS_ADDR)
#define SYS_PERCTL78_ADDR         (0x138 + SYS_REGS_ADDR)
#define SYS_PERCTL79_ADDR         (0x13C + SYS_REGS_ADDR)
#define SYS_PERCTL80_ADDR         (0x140 + SYS_REGS_ADDR)
#define SYS_PERCTL81_ADDR         (0x144 + SYS_REGS_ADDR)
#define SYS_PERCTL82_ADDR         (0x148 + SYS_REGS_ADDR)
#define SYS_PERCTL83_ADDR         (0x14c + SYS_REGS_ADDR)
#define SYS_PERCTL84_ADDR         (0x150 + SYS_REGS_ADDR)
#define SYS_PERCTL85_ADDR         (0x154 + SYS_REGS_ADDR)
#define SYS_PERCTL86_ADDR         (0x158 + SYS_REGS_ADDR)
#define SYS_PERCTL87_ADDR         (0x15c + SYS_REGS_ADDR)
#define SYS_PERCTL88_ADDR         (0x160 + SYS_REGS_ADDR)
#define SYS_PERCTL89_ADDR         (0x164 + SYS_REGS_ADDR)
#define SYS_PERCTL90_ADDR         (0x168 + SYS_REGS_ADDR)
#define SYS_PERCTL91_ADDR         (0x16C + SYS_REGS_ADDR)
#define SYS_PERCTL92_ADDR         (0x170 + SYS_REGS_ADDR)
#define SYS_PERCTL93_ADDR         (0x174 + SYS_REGS_ADDR)
#define SYS_PERCTL94_ADDR         (0x178 + SYS_REGS_ADDR)
#define SYS_PERCTL95_ADDR         (0x17C + SYS_REGS_ADDR)
#define SYS_PERCTL96_ADDR         (0x180 + SYS_REGS_ADDR)
#define SYS_PERCTL97_ADDR         (0x184 + SYS_REGS_ADDR)
#define SYS_PERCTL98_ADDR         (0x188 + SYS_REGS_ADDR)
#define SYS_PERCTL99_ADDR         (0x18C + SYS_REGS_ADDR)
#define SYS_PERCTL100_ADDR        (0x190 + SYS_REGS_ADDR)
#define SYS_PERCTL101_ADDR        (0x194 + SYS_REGS_ADDR)
#define SYS_PERCTL102_ADDR        (0x198 + SYS_REGS_ADDR)
#define SYS_PERCTL103_ADDR        (0x19C + SYS_REGS_ADDR)
#define SYS_PERCTL104_ADDR        (0x1a0 + SYS_REGS_ADDR)
#define SYS_PERCTL105_ADDR        (0x1a4 + SYS_REGS_ADDR)
#define SYS_PERCTL106_ADDR        (0x1a8 + SYS_REGS_ADDR)
#define SYS_PERCTL107_ADDR        (0x1aC + SYS_REGS_ADDR)
#define SYS_PERCTL108_ADDR        (0x1b0 + SYS_REGS_ADDR)
#define SYS_PERCTL109_ADDR        (0x1b4 + SYS_REGS_ADDR)
#define SYS_PERCTL110_ADDR        (0x1b8 + SYS_REGS_ADDR)
#define SYS_PERCTL111_ADDR        (0x1bC + SYS_REGS_ADDR)
#define SYS_PERCTL112_ADDR        (0x1c0 + SYS_REGS_ADDR)
#define SYS_PERCTL113_ADDR        (0x1c4 + SYS_REGS_ADDR)
#define SYS_PERCTL114_ADDR        (0x1c8 + SYS_REGS_ADDR)
#define SYS_PERCTL115_ADDR        (0x1cC + SYS_REGS_ADDR)
#define SYS_PERCTL116_ADDR        (0x1d0 + SYS_REGS_ADDR)
#define SYS_PERCTL117_ADDR        (0x1d4 + SYS_REGS_ADDR)
#define SYS_PERCTL118_ADDR        ((0x1d0 + 0x08) + SYS_REGS_ADDR)
#define SYS_PERCTL119_ADDR        (0x1dC + SYS_REGS_ADDR)
#define SYS_PERCTL120_ADDR        (0x1e0 + SYS_REGS_ADDR)
#define SYS_PERCTL121_ADDR        (0x1e4 + SYS_REGS_ADDR)
#define SYS_PERCTL122_ADDR        (0x1e8 + SYS_REGS_ADDR)
#define SYS_PERCTL123_ADDR        (0x1eC + SYS_REGS_ADDR)
#define SYS_PERCTL124_ADDR        (0x1f0 + SYS_REGS_ADDR)
#define SYS_PERCTL125_ADDR        (0x1f4 + SYS_REGS_ADDR)
#define SYS_PERCTL126_ADDR        (0x1f8 + SYS_REGS_ADDR)
#define SYS_PERCTL127_ADDR        (0x1fC + SYS_REGS_ADDR)

#define CRG_PERCTL_PLL96_ADDR       (0x0180 + CRG_REGS_ADDR)
#define CRG_PERCTL_PLL97_ADDR       (0x0184 + CRG_REGS_ADDR)
#define CRG_PERCTL_PLL224_ADDR      (0x0380 + CRG_REGS_ADDR)
#define CRG_PERCTL_PLL225_ADDR      (0x0384 + CRG_REGS_ADDR)

#define CRG_PERCTL_PLL0_ADDR        (0x0000 + CRG_REGS_ADDR)
#define CRG_PERCTL_PLL1_ADDR        (0x0004 + CRG_REGS_ADDR)

#define CRG_PERCTL4448_ADDR         (0x4580 + CRG_REGS_ADDR)
#define CRG_PERCTL4496_ADDR         (0x4640 + CRG_REGS_ADDR)
#define CRG_PERCTL4498_ADDR         (0x4648 + CRG_REGS_ADDR)
#define CRG_PERCTL4688_ADDR         (0x4940 + CRG_REGS_ADDR)
#define CRG_PERCTL4768_ADDR         (0x4A80 + CRG_REGS_ADDR)
#define CRG_PERCTL4896_ADDR         (0x4C80 + CRG_REGS_ADDR)
#define CRG_PERCTL4897_ADDR         (0x4C84 + CRG_REGS_ADDR)
#define CRG_PERCTL4898_ADDR         (0x4C88 + CRG_REGS_ADDR)
#define CRG_PERCTL5029_ADDR         (0x4E94 + CRG_REGS_ADDR)
#define CRG_PERCTL6464_ADDR         (0x6500 + CRG_REGS_ADDR)
#define CRG_PERCTL6470_ADDR         (0x6518 + CRG_REGS_ADDR)
#define CRG_PERCTL6560_ADDR         (0x6680 + CRG_REGS_ADDR)
#define CRG_PERCTL6561_ADDR         (0x6684 + CRG_REGS_ADDR)
#define CRG_PERCTL6592_ADDR         (0x6700 + CRG_REGS_ADDR)
#define CRG_PERCTL6624_ADDR         (0x6780 + CRG_REGS_ADDR)
#define CRG_PERCTL6625_ADDR         (0x6784 + CRG_REGS_ADDR)
#define CRG_PERCTL6640_ADDR         (0x67C0 + CRG_REGS_ADDR)
#define CRG_PERCTL6704_ADDR         (0x68C0 + CRG_REGS_ADDR)
#define CRG_PERCTL7248_ADDR         (0x7140 + CRG_REGS_ADDR)
#define CRG_PERCTL7256_ADDR         (0x7160 + CRG_REGS_ADDR)
#define CRG_PERCTL7264_ADDR         (0x7180 + CRG_REGS_ADDR)
#define CRG_PERCTL7376_ADDR         (0x7340 + CRG_REGS_ADDR)
#define CRG_PERCTL7408_ADDR         (0x73C0 + CRG_REGS_ADDR)
#define CRG_PERCTL7568_ADDR         (0x7640 + CRG_REGS_ADDR)
#define CRG_PERCTL8144_ADDR         (0x7F40 + CRG_REGS_ADDR)
#define CRG_PERCTL8152_ADDR         (0x7F60 + CRG_REGS_ADDR)
#define CRG_PERCTL8336_ADDR         (0x8240 + CRG_REGS_ADDR)
#define CRG_PERCTL8338_ADDR         (0x8248 + CRG_REGS_ADDR)
#define CRG_PERCTL8340_ADDR         (0x8250 + CRG_REGS_ADDR)
#define CRG_PERCTL8341_ADDR         (0x8254 + CRG_REGS_ADDR)
#define CRG_PERCTL8342_ADDR         (0x8258 + CRG_REGS_ADDR)
#define CRG_PERCTL8346_ADDR         (0x8268 + CRG_REGS_ADDR)
#define CRG_PERCTL8348_ADDR         (0x8270 + CRG_REGS_ADDR)
#define CRG_PERCTL8349_ADDR         (0x8274 + CRG_REGS_ADDR)
#define CRG_PERCTL8350_ADDR         (0x8278 + CRG_REGS_ADDR)
#define CRG_PERCTL8351_ADDR         (0x827C + CRG_REGS_ADDR)
#define CRG_PERCTL8352_ADDR         (0x8280 + CRG_REGS_ADDR)
#define CRG_PERCTL8528_ADDR         (0x8540 + CRG_REGS_ADDR)
#define CRG_PERCTL8536_ADDR         (0x8560 + CRG_REGS_ADDR)
#define CRG_PERCTL8544_ADDR         (0x8580 + CRG_REGS_ADDR)
#define CRG_PERCTL8552_ADDR         (0x85A0 + CRG_REGS_ADDR)
#define CRG_PERCTL8560_ADDR         (0x85C0 + CRG_REGS_ADDR)
#define CRG_PERCTL8568_ADDR         (0x85E0 + CRG_REGS_ADDR)
#define CRG_PERCTL8576_ADDR         (0x8600 + CRG_REGS_ADDR)
#define CRG_PERCTL8584_ADDR         (0x8620 + CRG_REGS_ADDR)
#define CRG_PERCTL8592_ADDR         (0x8640 + CRG_REGS_ADDR)
#define CRG_PERCTL8784_ADDR         (0x8940 + CRG_REGS_ADDR)
#define CRG_PERCTL8792_ADDR         (0x8960 + CRG_REGS_ADDR)
#define CRG_PERCTL8800_ADDR         (0x8980 + CRG_REGS_ADDR)
#define CRG_PERCTL8808_ADDR         (0x89A0 + CRG_REGS_ADDR)
#define CRG_PERCTL9296_ADDR         (0x9140 + CRG_REGS_ADDR)
#define CRG_PERCTL9297_ADDR         (0x9144 + CRG_REGS_ADDR)
#define CRG_PERCTL9298_ADDR         (0x9148 + CRG_REGS_ADDR)
#define CRG_PERCTL9300_ADDR         (0x9150 + CRG_REGS_ADDR)
#define CRG_PERCTL9301_ADDR         (0x9154 + CRG_REGS_ADDR)
#define CRG_PERCTL9302_ADDR         (0x9158 + CRG_REGS_ADDR)
#define CRG_PERCTL9303_ADDR         (0x915C + CRG_REGS_ADDR)
#define CRG_PERCTL9304_ADDR         (0x9160 + CRG_REGS_ADDR)
#define CRG_PERCTL9305_ADDR         (0x9164 + CRG_REGS_ADDR)
#define CRG_PERCTL9313_ADDR         (0x9184 + CRG_REGS_ADDR)
#define CRG_PERCTL9321_ADDR         (0x91A4 + CRG_REGS_ADDR)
#define CRG_PERCTL9329_ADDR         (0x91C4 + CRG_REGS_ADDR)
#define CRG_PERCTL9680_ADDR         (0x9740 + CRG_REGS_ADDR)
#define CRG_PERCTL9681_ADDR         (0x9744 + CRG_REGS_ADDR)
#define CRG_PERCTL9688_ADDR         (0x9760 + CRG_REGS_ADDR)
#define CRG_PERCTL9689_ADDR         (0x9764 + CRG_REGS_ADDR)
#define CRG_PERCTL9936_ADDR         (0x9B40 + CRG_REGS_ADDR)
#define CRG_PERCTL9937_ADDR         (0x9B44 + CRG_REGS_ADDR)
#define CRG_PERCTL9944_ADDR         (0x9B60 + CRG_REGS_ADDR)
#define CRG_PERCTL9945_ADDR         (0x9B64 + CRG_REGS_ADDR)
#define CRG_PERCTL9952_ADDR         (0x9B80 + CRG_REGS_ADDR)
#define CRG_PERCTL9953_ADDR         (0x9B84 + CRG_REGS_ADDR)
#define CRG_PERCTL10064_ADDR        (0x9D40 + CRG_REGS_ADDR)
#define CRG_PERCTL10065_ADDR        (0x9D44 + CRG_REGS_ADDR)
#define CRG_PERCTL10128_ADDR        (0x9E40 + CRG_REGS_ADDR)
#define CRG_PERCTL10129_ADDR        (0x9E44 + CRG_REGS_ADDR)
#define CRG_PERCTL10160_ADDR        (0x9EC0 + CRG_REGS_ADDR)
#define CRG_PERCTL10161_ADDR        (0x9EC4 + CRG_REGS_ADDR)
#define CRG_PERCTL10168_ADDR        (0x9EE0 + CRG_REGS_ADDR)
#define CRG_PERCTL10169_ADDR        (0x9EE4 + CRG_REGS_ADDR)
#define CRG_PERCTL10256_ADDR        (0xA040 + CRG_REGS_ADDR)
#define CRG_PERCTL10784_ADDR        (0xA880 + CRG_REGS_ADDR)
#define CRG_PERCTL10912_ADDR        (0xAA80 + CRG_REGS_ADDR)
#define CRG_PERCTL12288_ADDR        (0xC000 + CRG_REGS_ADDR)
#define CRG_PERCTL12289_ADDR        (0xC004 + CRG_REGS_ADDR)
#define CRG_PERCTL12290_ADDR        (0xC008 + CRG_REGS_ADDR)

#define VI_PORT_CLK_OFFSET        12
#define VI_PORT_CLK_MASK          0x7

#define MISC_CTL12C_ADDR          (0x12C + MISC_REGS_ADDR)
#define MISC_CTL98_ADDR           (0x98 + MISC_REGS_ADDR)
#define MISC_CTL9C_ADDR           (0x9C + MISC_REGS_ADDR)
#define MISC_CTLAC_ADDR           (0xAC + MISC_REGS_ADDR)
#define MISC_CTLB0_ADDR           (0xB0 + MISC_REGS_ADDR)
#define MISC_CTL2014_ADDR         (0x2014 + MISC_REGS_ADDR)

#define MISC_VICTRL_ADDR          (0x009c + MISC_REGS_ADDR)
#define MISC_VICTRL1_ADDR         (0x1000 + MISC_REGS_ADDR)

#define SYS_VI_DIV_SEL2           0x00 /* 2 division  */
#define SYS_VI_DIV_SEL4           0x01 /* 4 division  */
#define SYS_VI_DIV_SEL1           0x02 /* no division */

#define SYS_VO_DIV_SEL1           0x00 /* 1 division  */
#define SYS_VO_DIV_SEL2           0x01 /* 2 division  */
#define SYS_VO_DIV_SEL4           0x02 /* 4 division  */

#define SYS_AIO_SAMPLE_CLK16      0x0 /* 16 division */
#define SYS_AIO_SAMPLE_CLK32      0x01 /* 32 division */
#define SYS_AIO_SAMPLE_CLK48      0x02 /* 48 division */
#define SYS_AIO_SAMPLE_CLK64      0x03 /* 64 division */
#define SYS_AIO_SAMPLE_CLK128     0x04 /* 128 division */
#define SYS_AIO_SAMPLE_CLK256     0x05 /* 256 division */
#define SYS_AIO_SAMPLE_CLK320     0x06 /* 320 division */
#define SYS_AIO_SAMPLE_CLK384     0x07 /* 384 division */

#define SYS_AIO_BS_CLK1           0x00 /* 1 division */
#define SYS_AIO_BS_CLK2           0x02 /* 2 division */
#define SYS_AIO_BS_CLK3           0x01 /* 3 division */
#define SYS_AIO_BS_CLK4           0x03 /* 4 division */
#define SYS_AIO_BS_CLK6           0x04 /* 6 division */
#define SYS_AIO_BS_CLK8           0x05 /* 8 division */
#define SYS_AIO_BS_CLK12          0x06 /* 12 division */
#define SYS_AIO_BS_CLK16          0x07 /* 16 division */
#define SYS_AIO_BS_CLK24          0x08 /* 24 division */
#define SYS_AIO_BS_CLK32          0x09 /* 32 division */
#define SYS_AIO_BS_CLK48          0x0a /* 48 division */
#define SYS_AIO_BS_CLK64          0x0b /* 64 division */

#define MIPI_RX_CFG_ADDR          (0x0 + VI_ANR_MISC_REGS_ADDR)

#define OTP_USER_LOCKABLE0        (0x58 + OTP_USER_REGS_ADDR)

#endif /* OT_BOARD_H */

