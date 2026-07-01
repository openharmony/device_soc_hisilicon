/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART01_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART01_H_


#line 1 "ddr_training_impl.c"
/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <types.h>
#include "ddr_training_impl.h"
#include "ddr_interface.h"

#define DDR_COMMON

#define DDR_DATAEYE_DQ_STEP 2

static void ddr_training_adjust_wdq(struct ddr_cfg_st *cfg);
static void ddr_training_adjust_wdqs(struct ddr_cfg_st *cfg);

void *ddrtr_copy_data(void *dst, const void *src, unsigned int len)
{
    const char *s = src;
    char *d = dst;

    while (len--) {
        *d++ = *s++;
    }

    return dst;
}

void *ddrtr_set_data(void *b, int c, unsigned int len)
{
    char *bp = b;

    while (len--) {
        *bp++ = (unsigned char)c;
    }

    return b;
}

/* Get byte number */
static unsigned int ddr_phy_get_byte_num(unsigned int base_dmc)
{
    unsigned int byte_num;

    /* memery width -> byte number */
    byte_num = ((reg_read(base_dmc + DDR_DMC_CFG_DDRMODE) >>
        DMC_MEM_WIDTH_BIT) & DMC_MEM_WIDTH_MASK) << 1;
    /* for codedex */
    if (byte_num > DDR_PHY_BYTE_MAX) {
        byte_num = DDR_PHY_BYTE_MAX;
        ddr_error("get byte num fail");
    }

    return byte_num;
}

static int ddr_training_by_dmc(struct ddr_cfg_st *cfg)
{
    if (cfg->cmd_st != NULL) {
#ifdef DDR_TRAINING_CMD
        return ddr_training_cmd_func(cfg);
#endif
    } else {
        return ddr_training_boot_func(cfg);
    }

    return 0;
}

static int ddr_training_by_rank(struct ddr_cfg_st *cfg)
{
    int result = 0;
    unsigned int i;

    ddr_phy_switch_rank(cfg->cur_phy, cfg->rank_idx);

    for (i = 0; i < cfg->phy[cfg->phy_idx].dmc_num; i++) {
        cfg->dmc_idx = i;
        cfg->cur_dmc = cfg->phy[cfg->phy_idx].dmc[i].addr;
        cfg->cur_pattern = cfg->phy[cfg->phy_idx].dmc[i].ddrt_pattern;
        result += ddr_training_by_dmc(cfg);
    }

    return result;
}

static int ddr_training_by_phy(struct ddr_cfg_st *cfg)
{
    int result = 0;
    unsigned int i;
    unsigned int phy_mask;
    unsigned int rank_num;

    phy_mask = 1 << (cfg->phy_idx);
    rank_num = cfg->phy[cfg->phy_idx].rank_num;

    for (i = 0; i < rank_num; i++) {
        cfg->rank_idx = i;
        cfg->cur_item = cfg->phy[cfg->phy_idx].rank[i].item;
        if (ddr_training_check_bypass(cfg, phy_mask) != DDR_FALSE) {
            continue;
        }
        result += ddr_training_by_rank(cfg);
    }

    if (rank_num == DDR_SUPPORT_RANK_MAX) {
        ddr_training_adjust_wdq(cfg);
        ddr_training_adjust_wdqs(cfg);
        ddr_phy_switch_rank(cfg->cur_phy, 0x0); /* switch to rank0 */
    }
    return result;
}

int ddr_training_all(struct ddr_cfg_st *cfg)
{
    int result = 0;
    unsigned int i;

    if ((cfg == NULL) || (cfg->phy_num > DDR_PHY_NUM)) {
        return -1;
    }

    for (i = 0; i < cfg->phy_num; i++) {
        cfg->phy_idx = i;
        cfg->cur_phy = cfg->phy[i].addr;
        result += ddr_training_by_phy(cfg);
    }

    return result;
}

/* DDR training phy/dmc/dram_type config init */
static void ddr_training_cfg_set_phy0_dmc(struct ddr_cfg_st *cfg)
{
    unsigned int ddrt_pattern;

    if (cfg->phy[0].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
        cfg->phy[0].dmc_num = 2; /* lpddr4: 2 dmc per phy */
        ddrt_pattern = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_PATTERN);
        cfg->phy[0].dmc[0].addr = DDR_REG_BASE_DMC0;
        cfg->phy[0].dmc[0].ddrt_pattern = ddrt_pattern & 0xffff; /* bit[15-0]:dmc0 ddrt pattern */
        cfg->phy[0].dmc[0].byte_num = ddr_phy_get_byte_num(DDR_REG_BASE_DMC0);
        cfg->phy[0].dmc[1].addr = DDR_REG_BASE_DMC1;
        cfg->phy[0].dmc[1].ddrt_pattern = ddrt_pattern >> 16; /* bit[31-16]:dmc1 ddrt pattern */
        cfg->phy[0].dmc[1].byte_num = ddr_phy_get_byte_num(DDR_REG_BASE_DMC1);
        cfg->phy[0].total_byte_num = cfg->phy[0].dmc[0].byte_num + cfg->phy[0].dmc[1].byte_num;
    } else {
        cfg->phy[0].dmc_num = 1; /* other: 1 dmc per phy */
        cfg->phy[0].dmc[0].addr = DDR_REG_BASE_DMC0;
        cfg->phy[0].dmc[0].ddrt_pattern = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_PATTERN);
        cfg->phy[0].dmc[0].byte_num = ddr_phy_get_byte_num(DDR_REG_BASE_DMC0);
        cfg->phy[0].total_byte_num = cfg->phy[0].dmc[0].byte_num;
    }
    ddr_info("phy[0] total_byte_num[%x] dram_type[%x]", cfg->phy[0].total_byte_num, cfg->phy[0].dram_type);
}

#ifdef DDR_REG_BASE_PHY1
static void ddr_training_cfg_set_phy1_dmc(struct ddr_cfg_st *cfg)
{
    unsigned int ddrt_pattern;

    if (cfg->phy[1].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
        cfg->phy[1].dmc_num = 2; /* lpddr4: 2 dmc per phy */
        ddrt_pattern = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_PATTERN_SEC);
        cfg->phy[1].dmc[0].addr = DDR_REG_BASE_DMC2;
        cfg->phy[1].dmc[0].ddrt_pattern = ddrt_pattern & 0xffff; /* bit[15-0]:dmc0 ddrt pattern */
        cfg->phy[1].dmc[0].byte_num = ddr_phy_get_byte_num(DDR_REG_BASE_DMC2);
        cfg->phy[1].dmc[1].addr = DDR_REG_BASE_DMC3;
        cfg->phy[1].dmc[1].ddrt_pattern = ddrt_pattern >> 16; /* bit[31-16]:dmc1 ddrt pattern */
        cfg->phy[1].dmc[1].byte_num = ddr_phy_get_byte_num(DDR_REG_BASE_DMC3);
        cfg->phy[1].total_byte_num = cfg->phy[1].dmc[0].byte_num + cfg->phy[1].dmc[1].byte_num;
    } else {
        cfg->phy[1].dmc_num = 1; /* other: 1 dmc per phy */
#ifdef DDR_CHANNEL_MAP_PHY0_DMC0_PHY1_DMC2
        cfg->phy[1].dmc[0].addr = DDR_REG_BASE_DMC2;
        cfg->phy[1].dmc[0].byte_num = ddr_phy_get_byte_num(DDR_REG_BASE_DMC2);
#else
        cfg->phy[1].dmc[0].addr = DDR_REG_BASE_DMC1;
        cfg->phy[1].dmc[0].byte_num = ddr_phy_get_byte_num(DDR_REG_BASE_DMC1);
#endif
        cfg->phy[1].dmc[0].ddrt_pattern = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_PATTERN_SEC);
        cfg->phy[1].total_byte_num = cfg->phy[1].dmc[0].byte_num;
    }
    ddr_info("phy[1] total_byte_num[%x] dram_type[%x]", cfg->phy[1].total_byte_num, cfg->phy[1].dram_type);
}
#endif

#ifdef DDR_REG_BASE_PHY2
static void ddr_training_cfg_set_phy2_dmc(struct ddr_cfg_st *cfg)
{
    unsigned int ddrt_pattern;

    if (cfg->phy[2].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) { /* phy2 */
        cfg->phy[2].dmc_num = 2; /* lpddr4: 2 dmc per phy */
        ddrt_pattern = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_PATTERN_THIRD);
        cfg->phy[2].dmc[0].addr = DDR_REG_BASE_DMC4; /* phy2 */
        cfg->phy[2].dmc[0].ddrt_pattern = ddrt_pattern & 0xffff; /* phy2, bit[15-0]:dmc0 ddrt pattern */
        cfg->phy[2].dmc[0].byte_num = ddr_phy_get_byte_num(DDR_REG_BASE_DMC4);
        cfg->phy[2].dmc[1].addr = DDR_REG_BASE_DMC5; /* phy2 */
        cfg->phy[2].dmc[1].ddrt_pattern = ddrt_pattern >> 16; /* phy2, bit[31-16]:dmc1 ddrt pattern */
        cfg->phy[2].dmc[1].byte_num = ddr_phy_get_byte_num(DDR_REG_BASE_DMC5);
        cfg->phy[2].total_byte_num = cfg->phy[2].dmc[0].byte_num + cfg->phy[2].dmc[1].byte_num; /* phy2 */
    } else {
        cfg->phy[2].dmc_num = 1; /* phy2, other: 1 dmc per phy */
#ifdef DDR_CHANNEL_MAP_PHY0_DMC0_PHY1_DMC2
        cfg->phy[2].dmc[0].addr = DDR_REG_BASE_DMC4;
        cfg->phy[2].dmc[0].byte_num = ddr_phy_get_byte_num(DDR_REG_BASE_DMC4); /* phy2 */
#else
        cfg->phy[2].dmc[0].addr = DDR_REG_BASE_DMC2; /* phy2 */
        cfg->phy[2].dmc[0].byte_num = ddr_phy_get_byte_num(DDR_REG_BASE_DMC2); /* phy2 */
#endif
        cfg->phy[2].dmc[0].ddrt_pattern = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_PATTERN_THIRD); /* phy2 */
        cfg->phy[2].total_byte_num = cfg->phy[2].dmc[0].byte_num; /* phy2 */
    }
    ddr_info("phy[2] total_byte_num[%x] dram_type[%x]", cfg->phy[2].total_byte_num, cfg->phy[2].dram_type); /* phy2 */
}
#endif

/* DDR training phy/dmc/dram_type config init */
static void ddr_training_cfg_set_dmc(struct ddr_cfg_st *cfg)
{
    ddr_training_cfg_set_phy0_dmc(cfg);

#ifdef DDR_REG_BASE_PHY1
    ddr_training_cfg_set_phy1_dmc(cfg);
#endif

#ifdef DDR_REG_BASE_PHY2
    ddr_training_cfg_set_phy2_dmc(cfg);
#endif
}

static void ddr_training_cfg_set_rank(struct ddr_cfg_st *cfg)
{
    cfg->phy[0].rank_num = 1;
    cfg->phy[0].rank[0].item = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG);
    cfg->phy[0].rank[0].item_hw = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY0_RANK0);
#ifdef SYSCTRL_DDR_TRAINING_CFG_SEC
    cfg->phy[0].rank[1].item = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG_SEC);
#endif
    cfg->phy[0].rank[1].item_hw = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY0_RANK1);

    if (reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY0_RANK1)) {
        cfg->phy[0].rank_num = 2; /* rank number equal 2 if SYSCTRL_DDR_HW_PHY0_RANK1 has bean define in boot table */
    }

    ddr_info("Rank number PHY0 [%x]", cfg->phy[0].rank_num);
    ddr_info("HW training item PHY0[%x = %x][%x = %x]",
        (DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY0_RANK0), cfg->phy[0].rank[0].item_hw,
        (DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY0_RANK1), cfg->phy[0].rank[1].item_hw);

#ifdef DDR_REG_BASE_PHY1
    cfg->phy[1].rank_num = 1;
    cfg->phy[1].rank[0].item = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG);
    cfg->phy[1].rank[0].item_hw = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY1_RANK0);

    cfg->phy[1].rank[1].item = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG_SEC);
    cfg->phy[1].rank[1].item_hw = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY1_RANK1);

    if (reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY1_RANK1)) {
        cfg->phy[1].rank_num = 2; /* rank number equal 2 if SYSCTRL_DDR_HW_PHY1_RANK1 has bean define in boot table */
    }

    ddr_info("Rank number PHY1[%x]", cfg->phy[1].rank_num);
    ddr_info("HW training item PHY1[%x = %x][%x = %x]",
        (DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY1_RANK0), cfg->phy[1].rank[0].item_hw,
        (DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY1_RANK1), cfg->phy[1].rank[1].item_hw);
#endif

#ifdef DDR_REG_BASE_PHY2
    cfg->phy[2].rank_num = 1;
    cfg->phy[2].rank[0].item = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG); /* phy2 */
    cfg->phy[2].rank[0].item_hw = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY2_RANK0); /* phy2 */

    cfg->phy[2].rank[1].item = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG_SEC); /* phy2 */
    cfg->phy[2].rank[1].item_hw = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY2_RANK1); /* phy2 */

    if (reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY2_RANK1))
        cfg->phy[2].rank_num = 2; /* rank number equal 2 if SYSCTRL_DDR_HW_PHY1_RANK1 has bean define in boot table */

    ddr_info("Rank number PHY2[%x]", cfg->phy[2].rank_num); /* phy2 */
    ddr_info("HW training item PHY2[%x = %x][%x = %x]",
        (DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY2_RANK0), cfg->phy[2].rank[0].item_hw, /* phy2 */
        (DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY2_RANK1), cfg->phy[2].rank[1].item_hw); /* phy2 */
#endif

    ddr_info("SW training item Rank0[%x = %x]",
        (DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG), cfg->phy[0].rank[0].item);
#ifdef SYSCTRL_DDR_TRAINING_CFG_SEC
    ddr_info("SW training item Rank1[%x = %x]",
        (DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG_SEC), cfg->phy[0].rank[1].item);
#endif
}

static void ddr_training_cfg_set_phy(struct ddr_cfg_st *cfg)
{
    cfg->phy_num = DDR_PHY_NUM;
    cfg->phy[0].addr = DDR_REG_BASE_PHY0;
    cfg->phy[0].dram_type = reg_read(DDR_REG_BASE_PHY0 + DDR_PHY_DRAMCFG) & PHY_DRAMCFG_TYPE_MASK;
#ifdef DDR_REG_BASE_PHY1
    cfg->phy[1].addr = DDR_REG_BASE_PHY1;
    cfg->phy[1].dram_type = reg_read(DDR_REG_BASE_PHY1 + DDR_PHY_DRAMCFG) & PHY_DRAMCFG_TYPE_MASK;
#endif
#ifdef DDR_REG_BASE_PHY2
    cfg->phy[2].addr = DDR_REG_BASE_PHY2; /* phy2 */
    cfg->phy[2].dram_type = reg_read(DDR_REG_BASE_PHY2 + DDR_PHY_DRAMCFG) & PHY_DRAMCFG_TYPE_MASK; /* phy2 */
#endif
}

void ddr_training_cfg_init(struct ddr_cfg_st *cfg)
{
    ddrtr_set_data(cfg, 0, sizeof(struct ddr_cfg_st));
    ddr_training_cfg_set_phy(cfg);
    ddr_training_cfg_set_dmc(cfg);
    ddr_training_cfg_set_rank(cfg);
}

/* config DDR hw item */
void ddr_training_hw_item_cfg(struct ddr_cfg_st *cfg, unsigned int form_value)
{
    cfg->phy[0].rank[0].item_hw =
        reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY0_RANK0) & form_value;
    cfg->phy[0].rank[1].item_hw =
        reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY0_RANK1) & form_value;

#ifdef DDR_REG_BASE_PHY1
    cfg->phy[1].rank[0].item_hw =
        reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY1_RANK0) & form_value;
    cfg->phy[1].rank[1].item_hw =
        reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY1_RANK1) & form_value;
#endif

#ifdef DDR_REG_BASE_PHY2
    cfg->phy[2].rank[0].item_hw = /* phy2 */
        reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY2_RANK0) & form_value;
    cfg->phy[2].rank[1].item_hw = /* phy2 */
        reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY2_RANK1) & form_value;
#endif
}

/* 2GHz CPU run 2000 "nop" in 1 ns */
void ddr_training_delay(unsigned int cnt)
{
    while (cnt--) {
        asm("nop");
    }
}

/* set auto refresh */
static void ddr_training_set_timing(unsigned int base_dmc, unsigned int timing)
{
    ddr_training_delay(DDR_AUTO_TIMING_DELAY);
    reg_write(timing, base_dmc + DDR_DMC_TIMING2);
    /* need to delay 1 ns */
    ddr_training_delay(DDR_AUTO_TIMING_DELAY);
}

#ifdef DDR_TRAINING_STAT_CONFIG
/* Save training result in stat register */
static void ddr_training_save(unsigned int mask, unsigned int phy, int byte, int dq)
{
    unsigned int stat;
    unsigned int phy_index;

    stat = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_STAT);
    /* only record the first error */
    if (stat) {
        return;
    }

    stat = mask;
    if (phy != 0) {
        phy_index = ((phy == DDR_REG_BASE_PHY0) ? DDR_ERR_PHY0 : DDR_ERR_PHY1);
        stat |= phy_index;
    }

    if (byte != -1) {
        stat |= ((unsigned int)byte << DDR_ERR_BYTE_BIT);
    }

    if (dq != -1) {
        stat |= ((unsigned int)dq << DDR_ERR_DQ_BIT);
    }

    reg_write(stat, DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_STAT);
}
#endif

/* Record error code in register */
void ddr_training_stat(unsigned int mask, unsigned int phy, int byte, int dq)
{
    ddr_training_error(mask, phy, byte, dq);
#ifdef DDR_TRAINING_STAT_CONFIG
    ddr_training_save(mask, phy, byte, dq);
#endif
}

/* Check DDR training item whether by pass */
int ddr_training_check_bypass(const struct ddr_cfg_st *cfg, unsigned int mask)
{
    /* training item disable */
    if ((cfg->cur_item) & mask) {
        ddr_debug("DDR training [%x] is disable, rank[%x] cfg[%x]", mask, cfg->rank_idx, cfg->cur_item);
        return DDR_TRUE;
    } else {
        return DDR_FALSE;
    }
}


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART01_H_ */
