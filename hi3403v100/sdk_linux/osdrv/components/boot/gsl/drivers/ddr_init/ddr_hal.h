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

#ifndef DDR_HAL_INTERFACE_H
#define DDR_HAL_INTERFACE_H

#include "ddr_training_custom.h"
#include "ddr_phy.h"
#include "ddr_dmc.h"
#include "ddr_osal.h"

#define DDR_PHY_BYTE_MAX 4

#define DDR_TRUE  1
#define DDR_FALSE 0
#define CMD_ENABLE    1
#define CMD_DISABLE   0
#define RES_ENABLE    1
#define RES_DISABLE   0

#define DDR_PHY_CA_MAX 10
#define DDR_PHY_CA_REG_MAX (DDR_PHY_CA_MAX >> 1)

#define DDR_REG_OFFSET      4 /* DDR register offset */

#define DDR_REG_LOW_16BIT_MASK    0x0000ffff /* Lower 16-bit mask of the register */
#define DDR_REG_HIGH_16BIT_MASK   0xffff0000 /* Upper 16-bit mask of the register */

#define DDR_PHASE_HOLE_MASK  0x3
typedef union {
    struct {
        unsigned long long low_32_bit : 32;
        unsigned long long high_32_bit : 32;
    } dword;
    unsigned long long addr;
} addr_in_64bit;

typedef enum {
    DDRT_READ_ONLY = 0x201,
    DDRT_WRITE_ONLY = 0x101,
    DDRT_WRITE_READ = 0x001,
} ddrt_mode;

typedef enum {
    DDRT_RUNNING = 0x0,
    DDRT_ERROR = 0x1,
    DDRT_NORMAL = 0x3,
    DDRT_TIMEOUT = 0x11,
} ddrt_status;

typedef enum {
    DDRT_MASK_LOW_16BIT = 0x0,
    DDRT_MASK_HIGH_16BIT = 0x1,
    DDRT_MASK_32BIT = 0x2,
} ddrt_byte_mask_tag;

/************************ data define ****************************/

struct ddr_dmc_cfg_sref {
    unsigned int val[DDR_DMC_PER_PHY_MAX];
};

struct ddr_hal_bdl {
    unsigned int bdl[DDR_PHY_BYTE_MAX];
};

struct ddr_ca_bit {
    unsigned int bit_p;
    unsigned int bit_n;
};

struct ddr_ca_data {
    unsigned int base_dmc;
    unsigned int base_phy;
    unsigned int done; /* whether all ca found bdl range */
    unsigned int min;  /* min left bound */
    unsigned int max;  /* max right bound */
    unsigned def[DDR_PHY_CA_REG_MAX];
    int left[DDR_PHY_CA_MAX];
    int right[DDR_PHY_CA_MAX];
    struct ddr_ca_bit bits[DDR_PHY_CA_MAX];
};

/* function dmc */
DDR_AO_FUNC unsigned int ddr_hal_phy_get_byte_num(unsigned int base_dmc);
DDR_AO_FUNC void ddr_hal_axi_special_intlv_en(void);
/* set mem_row to 0 */
void ddr_sref_cfg_restore(const struct ddr_dmc_cfg_sref *cfg_sref);

/* function ddrt */
DDR_AO_FUNC unsigned int ddr_hal_ddrt_get_mem_width(void);

DDR_AO_FUNC void ddr_ddrt_init_for_capacity(void);
DDR_AO_FUNC void ddr_ddrt_process_dword(
    unsigned int val, unsigned long long addr, unsigned int mode, unsigned int bit_mask, unsigned int *status);

DDR_AO_FUNC int ddr_hal_hw_pre_dq_perbit_verf(
    phy_hvrftctrl_desc *phy_hvrftctrl_reg, phy_dvrftctrl_desc *phy_dvrftctrl_reg, \
    phy_ioctl2_desc *phy_ioctl2_reg, unsigned int phy_ioctl2_reg_len);
DDR_AO_FUNC int ddr_hal_hw_dq_perbit_verf(unsigned int item, phy_hvrftctrl_desc *phy_hvrftctrl_reg,
    phy_dvrftctrl_desc *phy_dvrftctrl_reg, phy_ioctl2_desc *phy_ioctl2_reg);

/* other */
#ifdef DDR_TRAINING_EXEC_TIME
void ddr_hal_chip_timer_start(void);
void ddr_hal_chip_timer_stop(void);
#endif
#endif /* DDR_HAL_INTERFACE_H */
