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

#ifndef DDR_DDRT_H
#define DDR_DDRT_H

#define DDR_DDRT_V2_0_SHF0_CONFIG

/* register offset address */
/* base address: DDR_REG_BASE_DDRT */
#define DDRT_OP            0x0  /* DDRT operation config */
#define DDRT_STATUS        0x4  /* DDRT status indicating */
#define DDRT_BURST_CONFIG  0x8  /* DDRT burst transfer config */
typedef union {
    struct {
        unsigned int ddrt_burst_len : 4;  /* [0-3] */
        unsigned int ddrt_burst_size : 3; /* [4-6] */
        unsigned int reserved : 25;       /* [7-31] */
    } bits;
    unsigned int val;
} ddrt_burst_config_desc;
typedef enum {
    DDRT_DATA_8BYTE = 0x3,
    DDRT_DATA_16BYTE = 0x4,
    DDRT_DATA_32BYTE = 0x5,
} ddrt_data_width;

#define DDRT_MEM_CONFIG    0xc  /* DDRT SDRAM config */
#define DDRT_LP4_MEM_CFG   0x1152
#define DDRT_NOLP4_MEM_CFG 0x1052
#define DDRT_BURST_NUM     0x10 /* DDRT burst number config */
/* DDRT burst number config register while testing address */
#define DDRT_ADDR_NUM      0x14
#define DDRT_LOOP_NUM      0x18 /* DDRT loop number config */
/* This register specified the system DDR starting address */
#define DDRT_DDR_BASE_ADDR 0x1c
#define DDRT_ADDR          0x20  /* DDRT test start address config  */
#define DDRT_REVERSED_DQ   0x30  /* DDRT reversed DQ indicating */
#define DDRT_SEED          0x38  /* DDRT starting random seed */
#define DDRT_KDATA         0x3c  /* DDRT kdata config */
#define DDRT_PRBS_SEED0    0x40  /* DDRT PRBS seed config register0 */
#define DDRT_PRBS_SEED1    0x44  /* DDRT PRBS seed config register1 */
#define DDRT_PRBS_SEED2    0x48  /* DDRT PRBS seed config register2 */
#define DDRT_PRBS_SEED3    0x4c  /* DDRT PRBS seed config register3 */
#define DDRT_PRBS_SEED4    0x50  /* DDRT PRBS seed config register4 */
#define DDRT_PRBS_SEED5    0x54  /* DDRT PRBS seed config register5 */
#define DDRT_PRBS_SEED6    0x58  /* DDRT PRBS seed config register6 */
#define DDRT_PRBS_SEED7    0x5c  /* DDRT PRBS seed config register7 */
#define DDRT_DQ_ERR_CNT0   0x60  /* DDRT error number indicator register of DQ3~DQ0 */
#define DDRT_DQ_ERR_CNT1   0x64  /* DDRT error number indicator register of DQ7~DQ4 */
#define DDRT_DQ_ERR_CNT2   0x68  /* DDRT error number indicator register of DQ11~DQ8 */
#define DDRT_DQ_ERR_CNT3   0x6c  /* DDRT error number indicator register of DQ15~DQ12 */
#define DDRT_DQ_ERR_CNT4   0x70  /* DDRT error number indicator register of DQ19~DQ16 */
#define DDRT_DQ_ERR_CNT5   0x74  /* DDRT error number indicator register of DQ23~DQ20 */
#define DDRT_DQ_ERR_CNT6   0x78  /* DDRT error number indicator register of DQ27~DQ24 */
#define DDRT_DQ_ERR_CNT7   0x7c  /* DDRT error number indicator register of DQ31~DQ28 */
#define DDRT_DQ_ERR_OFFSET 0x10

#endif /* DDR_DDRT_H */