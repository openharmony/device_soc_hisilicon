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

#ifndef DDR_CAPACITY_ADAPT_H
#define DDR_CAPACITY_ADAPT_H
#include <types.h>
#include "ddr_training_custom.h"
#include "ddr_osal.h"

#define DDR_SIZE_8G     0x2000 /* unit:MB, equal to 8GB */

struct ddr_reg_val_conf {
    unsigned int offset;
    unsigned int val;
};

struct ddr_capat_by_phy {
    unsigned int capat_low16bit;
    unsigned int capat_high16bit;
    unsigned int capacity;
};

struct ddr_capat_phy_all {
    struct ddr_capat_by_phy phy_capat[DDR_PHY_NUM];
    unsigned int ddr_capat_sum;
    unsigned int cur_phy_rank_num[DDR_PHY_NUM];
};

static inline void ddr_reg_config(const struct ddr_reg_val_conf *reg_val, unsigned int array_size)
{
    unsigned int i;
    const struct ddr_reg_val_conf *pt = reg_val;

    if (pt == NULL) {
        return;
    }

    for (i = 0; i < array_size; i++) {
        ddr_reg_write(pt->val, pt->offset);
        pt++;
    }
}

void ddr_chsel_remap_func(void);
#endif /* DDR_CAPACITY_ADAPT_H */
