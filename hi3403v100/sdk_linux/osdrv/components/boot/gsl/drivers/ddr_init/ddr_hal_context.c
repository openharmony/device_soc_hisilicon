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

#include "ddr_hal_context.h"
#include "securecutil.h"

DDR_AO_FUNC struct ddr_hal_phy_all *ddr_hal_get_phy(void)
{
    struct ddr_hal_phy_all *phy_position = NULL;

    phy_position = (struct ddr_hal_phy_all *)((uintptr_t)ddr_reg_read(DDR_REG_BASE_SYSCTRL +
        SYSCTRL_DDR_TRAINING_PHY));

    return phy_position;
}

DDR_AO_FUNC struct ddr_hal_context *ddr_hal_get_ctx(void)
{
    struct ddr_hal_context *ctx_position = NULL;

    ctx_position = (struct ddr_hal_context *)((uintptr_t)ddr_reg_read(DDR_REG_BASE_SYSCTRL +
        SYSCTRL_DDR_TRAINING_CTX));

    return ctx_position;
}

DDR_AO_FUNC void ddr_hal_set_cfg_addr(uintptr_t ctx_addr, uintptr_t phy_all_addr)
{
    ddr_reg_write(ctx_addr, DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CTX);
    ddr_reg_write(phy_all_addr, DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_PHY);
}

DDR_AO_FUNC static void ddr_hal_training_cfg_set_rank(struct ddr_hal_phy_all *phy_all)
{
    phy_all->phy[0].rank_num = 1;
    phy_all->phy[0].rank[0].item = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG);
    phy_all->phy[0].rank[0].item_hw = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY0_RANK0);

#ifdef SYSCTRL_DDR_TRAINING_CFG_SEC
    phy_all->phy[0].rank[1].item = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG_SEC);
#endif
    phy_all->phy[0].rank[1].item_hw = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY0_RANK1);

    if (ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY0_RANK1)) {
        /* rank number equal 2 if SYSCTRL_DDR_HW_PHY0_RANK1 has bean define in boot table */
        phy_all->phy[0].rank_num = 2; /* 2 rank */
    }
#ifdef DDR_REG_BASE_PHY1
    phy_all->phy[1].rank_num = 1;
    phy_all->phy[1].rank[0].item = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG);
    phy_all->phy[1].rank[0].item_hw = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY1_RANK0);

    phy_all->phy[1].rank[1].item = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG_SEC);
    phy_all->phy[1].rank[1].item_hw = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY1_RANK1);

    if (ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY1_RANK1)) {
        /* rank number equal 2 if SYSCTRL_DDR_HW_PHY1_RANK1 has bean define in boot table */
        phy_all->phy[1].rank_num = 2; /* 2 rank */
    }
#endif
#ifdef DDR_REG_BASE_PHY2
    phy_all->phy[2].rank_num = 1;
    phy_all->phy[2].rank[0].item = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG); /* phy2 */
    phy_all->phy[2].rank[0].item_hw = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY2_RANK0); /* phy2 */

    phy_all->phy[2].rank[1].item = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG_SEC); /* phy2 */
    phy_all->phy[2].rank[1].item_hw = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY2_RANK1); /* phy2 */

    if (ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY2_RANK1))
        /* rank number equal 2 if SYSCTRL_DDR_HW_PHY1_RANK1 has bean define in boot table */
        phy_all->phy[2].rank_num = 2; /* 2 rank */

#endif
}

#define SYSCTRL_DMC1_DDRT_PATTERN_BIT  16
DDR_AO_FUNC static void ddr_hal_training_cfg_set_phy0_dmc(struct ddr_hal_phy_all *phy_all)
{
    unsigned int ddrt_pattern;

    if (phy_all->phy[0].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
        /* lpddr4: 2 dmc per phy */
        phy_all->phy[0].dmc_num = 0x2;
        ddrt_pattern = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_PATTERN);
        phy_all->phy[0].dmc[0].addr = DDR_REG_BASE_DMC0;
        /* bit[15-0]:dmc0 ddrt pattern */
        phy_all->phy[0].dmc[0].ddrt_pattern = ddrt_pattern & DDR_REG_LOW_16BIT_MASK;
        phy_all->phy[0].dmc[0].byte_num = ddr_hal_phy_get_byte_num(DDR_REG_BASE_DMC0);
        phy_all->phy[0].total_byte_num = phy_all->phy[0].dmc[0].byte_num;
        if (DDR_DMC_PER_PHY_MAX == 0x2) {
            phy_all->phy[0].dmc[1].addr = DDR_REG_BASE_DMC1;
            /* bit[31-16]:dmc1 ddrt pattern */
            phy_all->phy[0].dmc[1].ddrt_pattern = ddrt_pattern >> SYSCTRL_DMC1_DDRT_PATTERN_BIT;
            phy_all->phy[0].dmc[1].byte_num = ddr_hal_phy_get_byte_num(DDR_REG_BASE_DMC1);
            phy_all->phy[0].total_byte_num += phy_all->phy[0].dmc[1].byte_num;
        }
    } else {
        /* other: 1 dmc per phy */
        phy_all->phy[0].dmc_num = 1;
        phy_all->phy[0].dmc[0].addr = DDR_REG_BASE_DMC0;
        phy_all->phy[0].dmc[0].ddrt_pattern = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_PATTERN);
        phy_all->phy[0].dmc[0].byte_num = ddr_hal_phy_get_byte_num(DDR_REG_BASE_DMC0);
        phy_all->phy[0].total_byte_num = phy_all->phy[0].dmc[0].byte_num;
    }
}

#ifdef DDR_REG_BASE_PHY1
DDR_AO_FUNC static void ddr_hal_training_cfg_set_phy1_dmc(struct ddr_hal_phy_all *phy_all)
{
    unsigned int ddrt_pattern;

    if (phy_all->phy[1].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
        /* lpddr4: 2 dmc per phy */
        phy_all->phy[1].dmc_num = 0x2;
        ddrt_pattern = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_PATTERN_SEC);
        phy_all->phy[1].dmc[0].addr = DDR_REG_BASE_DMC2;
        /* bit[15-0]:dmc0 ddrt pattern */
        phy_all->phy[1].dmc[0].ddrt_pattern = ddrt_pattern & DDR_REG_LOW_16BIT_MASK;
        phy_all->phy[1].dmc[0].byte_num = ddr_hal_phy_get_byte_num(DDR_REG_BASE_DMC2);
        phy_all->phy[1].total_byte_num = phy_all->phy[1].dmc[0].byte_num;
        if (DDR_DMC_PER_PHY_MAX == 0x2) {
            phy_all->phy[1].dmc[1].addr = DDR_REG_BASE_DMC3;
            /* bit[31-16]:dmc1 ddrt pattern */
            phy_all->phy[1].dmc[1].ddrt_pattern = ddrt_pattern >> SYSCTRL_DMC1_DDRT_PATTERN_BIT;
            phy_all->phy[1].dmc[1].byte_num = ddr_hal_phy_get_byte_num(DDR_REG_BASE_DMC3);
            phy_all->phy[1].total_byte_num += phy_all->phy[1].dmc[1].byte_num;
        }
    } else {
        /* other: 1 dmc per phy */
        phy_all->phy[1].dmc_num = 1;
#ifdef DDR_CHANNEL_MAP_PHY0_DMC0_PHY1_DMC2
        phy_all->phy[1].dmc[0].addr = DDR_REG_BASE_DMC2;
        phy_all->phy[1].dmc[0].byte_num = ddr_hal_phy_get_byte_num(DDR_REG_BASE_DMC2);
#else
        phy_all->phy[1].dmc[0].addr = DDR_REG_BASE_DMC1;
        phy_all->phy[1].dmc[0].byte_num = ddr_hal_phy_get_byte_num(DDR_REG_BASE_DMC1);
#endif
        phy_all->phy[1].dmc[0].ddrt_pattern = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_PATTERN_SEC);
        phy_all->phy[1].total_byte_num = phy_all->phy[1].dmc[0].byte_num;
    }
}
#endif

#ifdef DDR_REG_BASE_PHY2
DDR_AO_FUNC static void ddr_hal_training_cfg_set_phy2_dmc(struct ddr_hal_phy_all *phy_all)
{
    unsigned int ddrt_pattern;

    if (phy_all->phy[2].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) { /* phy2 */
        phy_all->phy[2].dmc_num = 2; /* lpddr4: 2 dmc per phy */
        ddrt_pattern = ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_PATTERN_THIRD);
        phy_all->phy[2].dmc[0].addr = DDR_REG_BASE_DMC4; /* phy2 */
        phy_all->phy[2].dmc[0].ddrt_pattern = ddrt_pattern & DDR_REG_LOW_16BIT_MASK; /* phy2 */
        phy_all->phy[2].dmc[0].byte_num = ddr_hal_phy_get_byte_num(DDR_REG_BASE_DMC4);
        phy_all->phy[2].total_byte_num = phy_all->phy[2].dmc[0].byte_num; /* phy2 */
        if (DDR_DMC_PER_PHY_MAX == 0x2) {
            phy_all->phy[2].dmc[1].addr = DDR_REG_BASE_DMC5; /* phy2 */
            /* phy2, bit[31-16]:dmc1 ddrt pattern */
            phy_all->phy[2].dmc[1].ddrt_pattern = ddrt_pattern >> SYSCTRL_DMC1_DDRT_PATTERN_BIT;
            phy_all->phy[2].dmc[1].byte_num = ddr_hal_phy_get_byte_num(DDR_REG_BASE_DMC5); /* phy2 */
            phy_all->phy[2].total_byte_num += phy_all->phy[2].dmc[1].byte_num; /* phy2 */
        }
    } else {
        phy_all->phy[2].dmc_num = 1; /* phy2, other: 1 dmc per phy */
#ifdef DDR_CHANNEL_MAP_PHY0_DMC0_PHY1_DMC2
        phy_all->phy[2].dmc[0].addr = DDR_REG_BASE_DMC4;
        phy_all->phy[2].dmc[0].byte_num = ddr_hal_phy_get_byte_num(DDR_REG_BASE_DMC4); /* phy2 */
#else
        phy_all->phy[2].dmc[0].addr = DDR_REG_BASE_DMC2; /* phy2 */
        phy_all->phy[2].dmc[0].byte_num = ddr_hal_phy_get_byte_num(DDR_REG_BASE_DMC2); /* phy2 */
#endif
        /* phy2 */
        phy_all->phy[2].dmc[0].ddrt_pattern =
            ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_PATTERN_THIRD); /* phy2 */
        phy_all->phy[2].total_byte_num = phy_all->phy[2].dmc[0].byte_num; /* phy2 */
    }
}
#endif

/* DDR training phy/dmc/dram_type config init */
DDR_AO_FUNC static void ddr_hal_training_cfg_set_dmc(struct ddr_hal_phy_all *phy_all)
{
    ddr_hal_training_cfg_set_phy0_dmc(phy_all);

#ifdef DDR_REG_BASE_PHY1
    ddr_hal_training_cfg_set_phy1_dmc(phy_all);
#endif

#ifdef DDR_REG_BASE_PHY2
    ddr_hal_training_cfg_set_phy2_dmc(phy_all);
#endif
}

DDR_AO_FUNC static void ddr_hal_training_cfg_set_phy(struct ddr_hal_context *ctx, struct ddr_hal_phy_all *phy_all)
{
    ctx->phy_num = DDR_PHY_NUM;
    ctx->phy0_status = 1;
    ctx->phy1_status = 1;
    if ((ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY0_RANK0) == 0) && \
        (ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY0_RANK1) == 0)) {
        ctx->phy0_status = 0;
    }

    if ((ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY1_RANK0) == 0) && \
        (ddr_reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY1_RANK1) == 0)) {
        ctx->phy1_status = 0;
    }
    phy_all->phy[0].addr = DDR_REG_BASE_PHY0;
    phy_all->phy[0].dram_type = ddr_reg_read(DDR_REG_BASE_PHY0 + DDR_PHY_DRAMCFG) &
        PHY_DRAMCFG_TYPE_MASK;

#ifdef DDR_REG_BASE_PHY1
    phy_all->phy[1].addr = DDR_REG_BASE_PHY1;
    phy_all->phy[1].dram_type = ddr_reg_read(DDR_REG_BASE_PHY1 + DDR_PHY_DRAMCFG) &
        PHY_DRAMCFG_TYPE_MASK;
#endif

#ifdef DDR_REG_BASE_PHY2
    phy_all->phy[2].addr = DDR_REG_BASE_PHY2;
    phy_all->phy[2].dram_type = /* phy2 */
        ddr_reg_read(DDR_REG_BASE_PHY2 + DDR_PHY_DRAMCFG) & PHY_DRAMCFG_TYPE_MASK;
#endif
}

DDR_AO_FUNC void ddr_hal_cfg_init(void)
{
    struct ddr_hal_context *ctx = NULL;
    struct ddr_hal_phy_all *phy_all = NULL;
    ctx = ddr_hal_get_ctx();
    phy_all = ddr_hal_get_phy();

    (void)memset_s(ctx, sizeof(struct ddr_hal_context), 0, sizeof(struct ddr_hal_context));
    (void)memset_s(phy_all, sizeof(struct ddr_hal_phy_all), 0, sizeof(struct ddr_hal_phy_all));
    ddr_hal_training_cfg_set_phy(ctx, phy_all);
    ddr_hal_training_cfg_set_dmc(phy_all);
    ddr_hal_training_cfg_set_rank(phy_all);
}

