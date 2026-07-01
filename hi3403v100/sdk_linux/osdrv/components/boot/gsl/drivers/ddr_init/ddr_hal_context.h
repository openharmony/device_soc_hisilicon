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

#ifndef DDRTN_HAL_CONTEXT_H
#define DDRTN_HAL_CONTEXT_H
#include "ddr_osal.h"
#include "ddr_training_custom.h"
#include "ddr_dmc.h"
#include "ddr_hal.h"

#define unused(x) (void)(x)

/* asm function */
/* DSB to make sure the operation is complete */
#ifdef __riscv
#define ddr_asm_dsb()                { __asm__ __volatile__("fence"); }
#elif (__LINUX_ARM_ARCH__ >= 8)
#define ddr_asm_dsb()                { __asm__ __volatile__("dsb sy"); }
#else
#define ddr_asm_dsb()                { __asm__ __volatile__("dsb"); }
#endif

#define ddr_asm_isb()                { __asm__ __volatile__("isb"); }

#define ddr_asm_nop()                { __asm__ __volatile__("nop"); }

#define DDR_BOOT_TYPE_UBOOT 0
#define DDR_BOOT_TYPE_CMD_BIN 1
#define DDR_BOOT_TYPE_HSL 2
#define DDR_BOOT_TYPE_AUX_CODE 3

#define DDR_INT_MAX    0x7FFFFFFF
#define DDR_INT_MIN    (-DDR_INT_MAX - 1)
#define DDR_UINT_MAX   0xFFFFFFFFU

/* check config */
#if defined(DDR_TRAINING_ADJUST_DISABLE) && defined(DDR_HW_TRAINING_CONFIG) && !defined(DDR_HW_READ_ADJ_CONFIG)
#error when defined DDR_TRAINING_ADJUST_DISABLE, MUST define DDR_HW_READ_ADJ_CONFIG.
#endif

#if (defined(DDR_HW_TRAINING_CONFIG) || defined(DDR_MPR_TRAINING_CONFIG) || \
    defined(DDR_VREF_TRAINING_CONFIG) ||                                    \
    defined(DDR_TRAINING_ADJUST_CONFIG)) &&                                 \
    !defined(DDR_DATAEYE_TRAINING_CONFIG)
#error when enable HW/GATE/VREF training or dataeye adjust, MUST define DDR_DATAEYE_TRAINING_CONFIG.
#endif

struct ddr_hal_dmc {
    unsigned int addr;
    unsigned int byte_num;
    unsigned int ddrt_pattern; /* ddrt reversed data */
};

struct ddr_hal_rank {
    unsigned int item;      /* software training item */
    unsigned int item_hw; /* hardware training item */
};

struct ddr_hal_phy {
    unsigned int addr;
    unsigned int dram_type;
    unsigned int dmc_num;
    unsigned int rank_num;
    unsigned int total_byte_num;
    struct ddr_hal_dmc dmc[DDR_DMC_PER_PHY_MAX];
    struct ddr_hal_rank rank[DDR_RANK_NUM];
};

struct ddr_hal_phy_all {
    struct ddr_hal_phy phy[DDR_PHY_NUM];
};

struct ddr_hal_context {
    unsigned int phy_num;
    unsigned int phy0_status;
    unsigned int phy1_status;
    unsigned int cur_phy;        /* current training phy addr */
    unsigned int cur_dmc;        /* current training dmc addr */
    unsigned int cur_item;        /* current SW or HW training item */
    unsigned int cur_pattern;    /* current ddrt pattern */
    unsigned int cur_mode;        /* read or write */
    unsigned int cur_byte;        /* current training byte index */
    unsigned int cur_dq;        /* current training dq index */
    unsigned int phy_idx;        /* current training phy index */
    unsigned int rank_idx;        /* current training rank index */
    unsigned int dmc_idx;        /* current training dmc index */
    unsigned int cmd_flag;       /* Flag indicating whether the command is executed */
    unsigned int res_flag;
};

DDR_AO_FUNC struct ddr_hal_phy_all *ddr_hal_get_phy(void);
DDR_AO_FUNC struct ddr_hal_context *ddr_hal_get_ctx(void);

DDR_AO_FUNC static inline unsigned int ddr_hal_get_phy_num(void)
{
    struct ddr_hal_context *ctx;

    ctx = ddr_hal_get_ctx();

    return ctx->phy_num;
}

DDR_AO_FUNC static inline unsigned int ddr_hal_get_cur_phy(void)
{
    struct ddr_hal_context *ctx;

    ctx = ddr_hal_get_ctx();

    return ctx->cur_phy;
}

DDR_AO_FUNC static inline unsigned int ddr_hal_get_cur_dmc(void)
{
    struct ddr_hal_context *ctx;

    ctx = ddr_hal_get_ctx();

    return ctx->cur_dmc;
}

DDR_AO_FUNC static inline unsigned int ddr_hal_get_phy_id(void)
{
    struct ddr_hal_context *ctx;

    ctx = ddr_hal_get_ctx();

    return ctx->phy_idx;
}

DDR_AO_FUNC static inline unsigned int ddr_hal_get_phy_addr(unsigned int idx)
{
    struct ddr_hal_phy_all *phy_all;

    phy_all = ddr_hal_get_phy();

    return phy_all->phy[idx].addr;
}

DDR_AO_FUNC static inline unsigned int ddr_hal_get_phy_dram_type(unsigned int idx)
{
    struct ddr_hal_phy_all *phy_all;

    phy_all = ddr_hal_get_phy();

    return phy_all->phy[idx].dram_type;
}

DDR_AO_FUNC static inline void ddr_hal_set_cur_dmc(unsigned int value)
{
    struct ddr_hal_context *ctx;

    ctx = ddr_hal_get_ctx();

    ctx->cur_dmc = value;
}

DDR_AO_FUNC static inline void ddr_hal_set_cur_phy(unsigned int value)
{
    struct ddr_hal_context *ctx;

    ctx = ddr_hal_get_ctx();

    ctx->cur_phy = value;
}

DDR_AO_FUNC static inline void ddr_hal_set_phy_id(unsigned int value)
{
    struct ddr_hal_context *ctx;

    ctx = ddr_hal_get_ctx();

    ctx->phy_idx = value;
}

DDR_AO_FUNC static inline void ddr_hal_set_dmc_id(unsigned int value)
{
    struct ddr_hal_context *ctx;

    ctx = ddr_hal_get_ctx();

    ctx->dmc_idx = value;
}

DDR_AO_FUNC static inline unsigned int ddr_hal_get_cur_phy_rank_num(void)
{
    struct ddr_hal_context *ctx;
    struct ddr_hal_phy_all *phy_all;

    ctx = ddr_hal_get_ctx();
    phy_all = ddr_hal_get_phy();

    return phy_all->phy[ctx->phy_idx].rank_num;
}
DDR_AO_FUNC static inline unsigned int ddr_hal_get_cur_phy_dram_type(void)
{
    struct ddr_hal_context *ctx;
    struct ddr_hal_phy_all *phy_all;

    ctx = ddr_hal_get_ctx();
    phy_all = ddr_hal_get_phy();

    return phy_all->phy[ctx->phy_idx].dram_type;
}

DDR_AO_FUNC static inline unsigned int ddr_hal_get_cur_phy_dmc_num(void)
{
    struct ddr_hal_context *ctx;
    struct ddr_hal_phy_all *phy_all;

    ctx = ddr_hal_get_ctx();
    phy_all = ddr_hal_get_phy();

    return phy_all->phy[ctx->phy_idx].dmc_num;
}

DDR_AO_FUNC static inline unsigned int ddr_hal_get_cur_dmc_addr(unsigned int dmc_id)
{
    struct ddr_hal_context *ctx;
    struct ddr_hal_phy_all *phy_all;

    ctx = ddr_hal_get_ctx();
    phy_all = ddr_hal_get_phy();

    return phy_all->phy[ctx->phy_idx].dmc[dmc_id].addr;
}

void ddr_capat_adapt_start(void);
DDR_AO_FUNC void ddr_hal_set_cfg_addr(uintptr_t ctx_addr, uintptr_t phy_all_addr);
DDR_AO_FUNC void ddr_hal_cfg_init(void);
#endif /* DDRTN_HAL_CONTEXT_H */
