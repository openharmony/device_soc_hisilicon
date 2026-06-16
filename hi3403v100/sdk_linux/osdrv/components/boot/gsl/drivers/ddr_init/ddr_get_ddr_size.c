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

#include "ddr_interface.h"
#include "ddr_osal.h"
#include "ddr_get_ddr_size.h"
#include "ddr_capacity_adapt.h"
#include "ddr_hal_context.h"
#include "securecutil.h"
#include "ddr_hal.h"

/* Identify DDR capacity by winding
 * return: ddr size (KB)
 */
unsigned int ddr_winding_identification(unsigned int step, unsigned int max_capat, unsigned int bit_mask)
{
#define DDR_10_BIT  10 /* Move 10 bits */
    int cnt = 0;
    unsigned int ddr_size;
    unsigned int status;

    do {
        cnt++;
        /* winding */
        ddr_ddrt_process_dword(DDR_WINDING_NUM1, DDRT_CFG_BASE_ADDR, DDRT_WRITE_READ, DDRT_MASK_32BIT, &status);
        if (status != DDRT_NORMAL) {
            return 0;
        }

        while ((step * cnt) < max_capat) {
            ddr_ddrt_process_dword(DDR_WINDING_NUM1, (unsigned long long)DDRT_CFG_BASE_ADDR +
                (((unsigned long long)step * (unsigned long long)cnt) << DDR_10_BIT),
                DDRT_READ_ONLY, bit_mask, &status);
            if (status == DDRT_NORMAL) {
                break;
            } else if (status != DDRT_ERROR) {
                return 0;
            }
            cnt++;
        }

        if ((step * cnt) >= max_capat) {
            return DDR_SIZE_8G; /* The return value is determined based on the project */
        }

        /* check winding */
        ddr_ddrt_process_dword(DDR_WINDING_NUM2, DDRT_CFG_BASE_ADDR, DDRT_WRITE_READ, DDRT_MASK_32BIT, &status);
        if (status != DDRT_NORMAL) {
            return 0;
        }

        ddr_ddrt_process_dword(DDR_WINDING_NUM2, (unsigned long long)DDRT_CFG_BASE_ADDR +
            (((unsigned long long)step * (unsigned long long)cnt) << DDR_10_BIT),
            DDRT_READ_ONLY, bit_mask, &status);
        if (status == DDRT_NORMAL && cnt == 1) {
            return 0; /* widing fail */
        }
    } while (status != DDRT_NORMAL);

    ddr_size = step * cnt;

    return ddr_size;
}

static void ddr_store_ddr(struct ddr_data *ddr_reg_val)
{
    unsigned int i;

    for (i = 0; i < DDR_STORE_NUM; i++) {
        ddr_reg_val->reg_val[i] = ddr_reg_read(DDRT_CFG_BASE_ADDR + DDR_REG_OFFSET * i);
    }
}

static void ddr_restore_ddr(struct ddr_data *ddr_reg_val)
{
    unsigned int i;

    for (i = 0; i < DDR_STORE_NUM; i++) {
        ddr_reg_write(ddr_reg_val->reg_val[i], DDRT_CFG_BASE_ADDR + DDR_REG_OFFSET * i);
    }
}

/* Identifying the DDR Capacity in Different Scenarios
 * DDR4: Distinguish upper and lower 16 bits
 */
void ddr_winding_identy_scence(struct ddr_capat_by_phy *phy_capat)
{
    unsigned int dmc_num = ddr_hal_get_cur_phy_dmc_num();
    if (ddr_hal_get_cur_phy_dram_type() != PHY_DRAMCFG_TYPE_LPDDR4) {
        for (unsigned int dmc_idx = 0; dmc_idx < dmc_num; dmc_idx++) {
            ddr_hal_set_cur_dmc(ddr_hal_get_cur_dmc_addr(dmc_idx));
            unsigned int mem_width = ddr_hal_ddrt_get_mem_width();
            if (mem_width == MEM_WIDTH_32BIT) {
                phy_capat->capat_low16bit = (ddr_winding_identification(WINDING_STEP, DDR_MAX_CAPAT,
                    DDRT_MASK_LOW_16BIT) >> 1) >> DDR_10_BIT;
                phy_capat->capat_high16bit = (ddr_winding_identification(WINDING_STEP, DDR_MAX_CAPAT,
                    DDRT_MASK_HIGH_16BIT) >> 1) >> DDR_10_BIT;
                phy_capat->capacity = phy_capat->capat_low16bit + phy_capat->capat_high16bit;
            } else {
                phy_capat->capat_low16bit =
                    ddr_winding_identification(WINDING_STEP, DDR_MAX_CAPAT, DDRT_MASK_32BIT) >> DDR_10_BIT;
                phy_capat->capat_high16bit = 0;
                phy_capat->capacity = phy_capat->capat_low16bit + phy_capat->capat_high16bit;
            }
        }
    } else {
        phy_capat->capacity = ddr_winding_identification(WINDING_STEP, DDR_MAX_CAPAT, DDRT_MASK_32BIT) >> DDR_10_BIT;
    }
}

/**
  @brief Save axi register status before ddr capacity adaptation.

  @param[in] reg_bak  DDR axi register.
**/
static void ddr_save_reg_status(unsigned int *reg_bak, unsigned int reg_bak_len)
{
    unsigned int dmc_num;
    unsigned int rank_num;
    unsigned int rank_idx;
    unsigned int dmc_idx;
    unsigned int base_dmc;

    reg_bak[0] = ddr_reg_read(DDR_REG_BASE_AXI + DDR_AXI_REGION0_MAP1); /* register 0 */
    reg_bak[1] = ddr_reg_read(DDR_REG_BASE_AXI + DDR_AXI_REGION0_ATTRIB1); /* register 1 */
    reg_bak[2] = ddr_reg_read(DDR_REG_BASE_AXI + DDR_AXI_REGION0_MAP2); /* register 2 */
    reg_bak[3] = ddr_reg_read(DDR_REG_BASE_AXI + DDR_AXI_REGION0_ATTRIB2); /* register 3 */
    reg_bak[4] = ddr_reg_read(DDR_REG_BASE_AXI + DDR_AXI_REGION1_MAP1); /* register 4 */
    reg_bak[5] = ddr_reg_read(DDR_REG_BASE_AXI + DDR_AXI_REGION0_MAP2_C); /* register 5 */

    dmc_num = ddr_hal_get_cur_phy_dmc_num();
    rank_num = ddr_hal_get_cur_phy_rank_num();
    for (rank_idx = 0; rank_idx < rank_num; rank_idx++) {
        for (dmc_idx = 0; dmc_idx < dmc_num; dmc_idx++) {
            base_dmc = ddr_hal_get_cur_dmc_addr(dmc_idx);
            reg_bak[6 + rank_idx * DDR_DMC_PER_PHY_MAX + dmc_idx] = /* from register 6 */
                ddr_reg_read(base_dmc + ddr_dmc_cfg_rnkvol(rank_idx));
            reg_bak[6 + DDR_DMC_PER_PHY_MAX * DDR_RANK_NUM + /* from register 6 */
                rank_idx * DDR_DMC_PER_PHY_MAX + dmc_idx] =
                ddr_reg_read(DDR_REG_BASE_AXI + qos_ddrc_cfg_rnkvol(dmc_idx, rank_idx));
        }
    }
}

/**
  @brief Restore axi register status after ddr capacity adaptation.

  @param[in] reg_bak  DDR axi register.
**/
static void ddr_restore_reg_status(unsigned int *reg_bak)
{
    unsigned int dmc_num;
    unsigned int rank_num;
    unsigned int rank_idx;
    unsigned int dmc_idx;
    unsigned int base_dmc;

    ddr_reg_write(reg_bak[0], DDR_REG_BASE_AXI + DDR_AXI_REGION0_MAP1); /* register 0 */
    ddr_reg_write(reg_bak[1], DDR_REG_BASE_AXI + DDR_AXI_REGION0_ATTRIB1); /* register 1 */
    ddr_reg_write(reg_bak[2], DDR_REG_BASE_AXI + DDR_AXI_REGION0_MAP2); /* register 2 */
    ddr_reg_write(reg_bak[3], DDR_REG_BASE_AXI + DDR_AXI_REGION0_ATTRIB2); /* register 3 */
    ddr_reg_write(reg_bak[4], DDR_REG_BASE_AXI + DDR_AXI_REGION1_MAP1); /* register 4 */
    ddr_reg_write(reg_bak[5], DDR_REG_BASE_AXI + DDR_AXI_REGION0_MAP2_C); /* register 5 */

    dmc_num = ddr_hal_get_cur_phy_dmc_num();
    rank_num = ddr_hal_get_cur_phy_rank_num();
    for (rank_idx = 0; rank_idx < rank_num; rank_idx++) {
        for (dmc_idx = 0; dmc_idx < dmc_num; dmc_idx++) {
            base_dmc = ddr_hal_get_cur_dmc_addr(dmc_idx);
            ddr_reg_write(reg_bak[6 + rank_idx * DDR_DMC_PER_PHY_MAX + dmc_idx], /* from register 6 */
                base_dmc + ddr_dmc_cfg_rnkvol(rank_idx));
            ddr_reg_write(reg_bak[6 + DDR_DMC_PER_PHY_MAX * DDR_RANK_NUM + /* from register 6 */
                rank_idx * DDR_DMC_PER_PHY_MAX + dmc_idx],
                DDR_REG_BASE_AXI + qos_ddrc_cfg_rnkvol(dmc_idx, rank_idx));
        }
    }
}

/* return DDR size, unit:MB */
void ddr_get_ddr_size(struct ddr_capat_phy_all *capat_phy_all)
{
    unsigned int phy_idx = ddr_hal_get_phy_id();
    unsigned int reg_bak_len = 6 + DDR_DMC_PER_PHY_MAX * DDR_RANK_NUM * 2; /* 6 axi registers and 2 special registers */
    unsigned int reg_bak[reg_bak_len];
    struct ddr_data ddr_reg_val;
    unsigned int axi_action_val;

    ddr_save_reg_status(reg_bak, reg_bak_len);
    axi_action_val = ddr_reg_read(DDR_REG_BASE_AXI + DDR_AXI_ACTION);

    /* Address mapping to PHY */
    ddr_chsel_remap_func();

    /* store ddr_base value */
    ddr_store_ddr(&ddr_reg_val);

    ddr_ddrt_init_for_capacity();

    ddr_winding_identy_scence(&capat_phy_all->phy_capat[phy_idx]);

    /* restore ddr base value */
    ddr_restore_ddr(&ddr_reg_val);

    ddr_reg_write(axi_action_val, DDR_REG_BASE_AXI + DDR_AXI_ACTION);
    ddr_restore_reg_status(reg_bak);
}

/* return DDR capacity (MB) */
unsigned int ddr_capat_adapt(void)
{
    int i;
    struct ddr_hal_context *ctx;
    ctx = ddr_hal_get_ctx();
    unsigned int phy_num = ddr_hal_get_phy_num();
    struct ddr_capat_phy_all capat_phy_all;
    struct tr_custom_reg reg;
    (void)memset_s(&reg, sizeof(struct tr_custom_reg), 0, sizeof(struct tr_custom_reg));
    (void)memset_s((char *)&capat_phy_all, sizeof(struct ddr_capat_phy_all), 0, sizeof(struct ddr_capat_phy_all));

    if (ddr_boot_cmd_save(&reg) != 0) {
        return -1;
    }

    for (i = 0; i < phy_num; i++) {
        uint8_t status = (i == 0) ? ctx->phy0_status : ctx->phy1_status;
        if (status == 1) {
            ddr_hal_set_phy_id(i);
            ddr_hal_set_cur_phy(ddr_hal_get_phy_addr(i));
            capat_phy_all.cur_phy_rank_num[i] = ddr_hal_get_cur_phy_rank_num();
            ddr_get_ddr_size(&capat_phy_all);
        }
    }
    capat_phy_all.ddr_capat_sum = capat_phy_all.phy_capat[0].capacity + capat_phy_all.phy_capat[1].capacity;

    ddr_boot_cmd_restore(&reg);

    return capat_phy_all.ddr_capat_sum;
}

void ddr_capat_adapt_start(void)
{
    unsigned int ddr_size = 0;
    ddr_size = ddr_capat_adapt();
    ddr_serial_puts("\r\nDDR size: 0x");
    ddr_serial_put_hex(ddr_size);
    ddr_serial_puts("MB\n");
    ddr_reg_write(ddr_size, DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_SIZE);
    return;
}
