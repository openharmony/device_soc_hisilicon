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

#define __COMMON__

static void ddr_training_adjust_wdq(struct ddr_cfg_st *cfg);
static void ddr_training_adjust_wdqs(struct ddr_cfg_st *cfg);

void *ddrtr_copy_data(void *dst, const void *src, unsigned int len)
{
	const char *s = src;
	char *d = dst;

	while (len--)
		*d++ = *s++;

	return dst;
}

void *ddrtr_set_data(void *b, int c, unsigned int len)
{
	char *bp = b;

	while (len--)
		*bp++ = (unsigned char)c;

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
		if (ddr_training_check_bypass(cfg, phy_mask) != DDR_FALSE)
			continue;
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

	if ((cfg == NULL) || (cfg->phy_num > DDR_PHY_NUM))
		return -1;

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

	if (reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY0_RANK1))
		cfg->phy[0].rank_num = 2; /* rank number equal 2 if SYSCTRL_DDR_HW_PHY0_RANK1 has bean define in boot table */

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

	if (reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_HW_PHY1_RANK1))
		cfg->phy[1].rank_num = 2; /* rank number equal 2 if SYSCTRL_DDR_HW_PHY1_RANK1 has bean define in boot table */

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
	while (cnt--)
		asm("nop");
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
	if (stat)
		return;

	stat = mask;
	if (phy != 0) {
		phy_index = ((phy == DDR_REG_BASE_PHY0) ? DDR_ERR_PHY0 : DDR_ERR_PHY1);
		stat |= phy_index;
	}

	if (byte != -1)
		stat |= ((unsigned int)byte << DDR_ERR_BYTE_BIT);

	if (dq != -1)
		stat |= ((unsigned int)dq << DDR_ERR_DQ_BIT);

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

#if !defined(DDR_TRAINING_CUT_CODE_CONFIG) || defined(DDR_TRAINING_CMD)
/* Save register value before training */
void ddr_training_save_reg(const struct ddr_cfg_st *cfg, struct tr_relate_reg *relate_reg, unsigned int mask)
{
	unsigned int base_dmc;
	unsigned int base_phy;

	if ((cfg == NULL) || (relate_reg == NULL)) {
		ddr_error("Pointer parameter is NULL!");
		return;
	}
	base_dmc = cfg->cur_dmc;
	base_phy = cfg->cur_phy;

	/* save reg value */
	relate_reg->auto_ref_timing = reg_read(base_dmc + DDR_DMC_TIMING2);
	relate_reg->power_down = reg_read(base_dmc + DDR_DMC_CFG_PD);
	relate_reg->misc_scramb = reg_read(base_phy + DDR_PHY_MISC);
	/* Static register have to read two times to get the right value. */
	relate_reg->ac_phy_ctl = reg_read(base_phy + DDR_PHY_ACPHYCTL4);
	relate_reg->ac_phy_ctl = reg_read(base_phy + DDR_PHY_ACPHYCTL4);

	/* set new value */
	switch (mask) {
	case DDR_BYPASS_WL_MASK:
	case DDR_BYPASS_LPCA_MASK:
		/* disable auto refresh */
		ddr_training_set_timing(base_dmc, relate_reg->auto_ref_timing & DMC_AUTO_TIMING_DIS);
		break;
	case DDR_BYPASS_GATE_MASK:
		/* disable auto refresh */
		ddr_training_set_timing(base_dmc, relate_reg->auto_ref_timing & DMC_AUTO_TIMING_DIS);
		if (!(reg_read(base_phy + DDR_PHY_DRAMCFG) & PHY_DRAMCFG_MA2T)) /* set 1T */
			reg_write(0x0, base_phy + DDR_PHY_ACPHYCTL4);
		break;
	case DDR_BYPASS_HW_MASK:
		if (!(reg_read(base_phy + DDR_PHY_DRAMCFG) & PHY_DRAMCFG_MA2T)) /* set 1T */
			reg_write(0x0, base_phy + DDR_PHY_ACPHYCTL4);
		break;
	default:
		break;
	}

	reg_write(relate_reg->power_down & DMC_POWER_DOWN_DIS, base_dmc + DDR_DMC_CFG_PD);
	reg_write(relate_reg->misc_scramb & PHY_MISC_SCRAMB_DIS, base_phy + DDR_PHY_MISC);

	ddr_dqsswap_save_func(relate_reg->swapdfibyte_en, base_phy);

	ddr_axi_save_func(relate_reg);

	ddr_rnkvol_save_func(relate_reg, base_dmc);

	/* save customer reg */
	ddr_training_save_reg_func((void *)relate_reg, mask);

	ddr_phy_cfg_update(base_phy);

	ddr_asm_dsb();
}

/* Restore register value after training */
void ddr_training_restore_reg(const struct ddr_cfg_st *cfg,
	const struct tr_relate_reg *relate_reg)
{
	unsigned int base_dmc;
	unsigned int base_phy;

	if ((cfg == NULL) || (relate_reg == NULL)) {
		ddr_error("Pointer parameter is NULL");
		return;
	}

	base_dmc = cfg->cur_dmc;
	base_phy = cfg->cur_phy;

	/* enable auto refresh */
	ddr_training_set_timing(base_dmc, relate_reg->auto_ref_timing);
	reg_write(relate_reg->power_down, base_dmc + DDR_DMC_CFG_PD);
	reg_write(relate_reg->misc_scramb, base_phy + DDR_PHY_MISC);
	if (!(reg_read(base_phy + DDR_PHY_DRAMCFG) & PHY_DRAMCFG_MA2T))
		reg_write(relate_reg->ac_phy_ctl, base_phy + DDR_PHY_ACPHYCTL4);

	ddr_dqsswap_restore_func(relate_reg->swapdfibyte_en, base_phy);

	ddr_axi_restore_func(relate_reg);

	ddr_rnkvol_restore_func(relate_reg, base_dmc);

	/* restore customer reg */
	ddr_training_restore_reg_func((void *)relate_reg);

	ddr_phy_cfg_update(base_phy);

	ddr_asm_dsb();
}

/* Switch AXI to DMC0/DMC1/DMC2/DMC3 for DDRT test */
void ddr_training_switch_axi(const struct ddr_cfg_st *cfg)
{
	ddr_axi_chsel_remap_func(cfg);
	ddr_axi_switch_func(cfg);
	ddr_debug("AXI region0[%x = %x]",
		(DDR_REG_BASE_AXI + DDR_AXI_REGION_ATTRIB0),
		reg_read(DDR_REG_BASE_AXI + DDR_AXI_REGION_ATTRIB0));
	ddr_debug("AXI region1[%x = %x]",
		(DDR_REG_BASE_AXI + DDR_AXI_REGION_ATTRIB1),
		reg_read(DDR_REG_BASE_AXI + DDR_AXI_REGION_ATTRIB1));

	ddr_rnkvol_set_func(cfg);
}
#endif

#if defined(DDR_WL_TRAINING_CONFIG) || defined(DDR_MPR_TRAINING_CONFIG)
/* Excute DMC sfc command */
static void ddr_dmc_sfc_cmd(unsigned int base_dmc, unsigned int sfc_cmd,
	unsigned int sfc_addr, unsigned int sfc_bank)
{
	unsigned int count = 0;

	/* set sfc cmd */
	dmc_sfc_cmd_write(sfc_cmd, base_dmc + DDR_DMC_SFCCMD);
	/* set col and row */
	reg_write(sfc_addr, base_dmc + DDR_DMC_SFCADDR);
	/* set bank */
	dmc_sfc_bank_write(sfc_bank, base_dmc + DDR_DMC_SFCBANK);
	/* excute cmd */
	reg_write(0x1, base_dmc + DDR_DMC_SFCREQ);

	ddr_asm_dsb();

	while (count < DDR_SFC_WAIT_TIMEOUT) { /* wait command finished */
		if (!(reg_read(base_dmc + DDR_DMC_SFCREQ) & 0x1))
			break;
		count++;
	}

	if (count >= DDR_HWR_WAIT_TIMEOUT)
		ddr_error("SFC cmd wait timeout");
}
#endif

#if defined(DDR_HW_TRAINING_CONFIG) || defined(DDR_DCC_TRAINING_CONFIG)
/* Exit or enter auto self-refresh */
static int ddr_training_easr(unsigned int base_dmc, unsigned int sref_req)
{
	unsigned int count;

	count = DDR_HWR_WAIT_TIMEOUT;
	if (sref_req == DDR_EXIT_SREF) {
		/* Exit Auto-self refresh */
		reg_write(DMC_CTRL_SREF_EXIT, base_dmc + DDR_DMC_CTRL_SREF);
		while (count--) {
			if (!(reg_read(base_dmc + DDR_DMC_CURR_FUNC) & DMC_CURR_FUNC_IN_SREF_MASK))
				break;
		}
	} else if (sref_req == DDR_ENTER_SREF) {
		/* Enter Auto-self refresh */
		reg_write(DMC_CTRL_SREF_ENTER, base_dmc + DDR_DMC_CTRL_SREF);
		while (count--) {
			if (reg_read(base_dmc + DDR_DMC_CURR_FUNC) & DMC_CURR_FUNC_IN_SREF_MASK)
				break;
		}
	}

	if (count == 0xffffffff) {
		ddr_fatal("SREF wait timeout");
		ddr_training_stat(DDR_ERR_HW_RD_DATAEYE, -1, -1, -1);
		return -1;
	}
	return 0;
}

/* DDR hw/dcc training exit or enter auto self-refresh */
int ddr_training_ctrl_easr(const struct ddr_cfg_st *cfg, unsigned int sref_req)
{
	int result = 0;
	unsigned int i;

	const struct ddr_phy_st *phy_st = &cfg->phy[cfg->phy_idx];

	if (phy_st->dmc_num > DDR_DMC_PER_PHY_MAX) {
		ddr_error("loop upper limit cfg->dmc_num out of range");
		return -1;
	}
	for (i = 0; i < phy_st->dmc_num; i++)
		result += ddr_training_easr(phy_st->dmc[i].addr, sref_req);

	return result;
}

void ddr_training_save_timing(const struct ddr_cfg_st *cfg, struct ddr_timing_st *timing_st)
{
	unsigned int i;
	const struct ddr_phy_st *phy_st = &cfg->phy[cfg->phy_idx];

	for (i = 0; i < phy_st->dmc_num; i++) {
		timing_st->val[i] = reg_read(phy_st->dmc[i].addr + DDR_DMC_TIMING2);
		/* disable auto refresh */
		ddr_training_set_timing(phy_st->dmc[i].addr, timing_st->val[i] & DMC_AUTO_TIMING_DIS);
	}
}

void ddr_training_restore_timing(const struct ddr_cfg_st *cfg,
	const struct ddr_timing_st *timing_st)
{
	unsigned int i;
	const struct ddr_phy_st *phy_st = &cfg->phy[cfg->phy_idx];

	for (i = 0; i < phy_st->dmc_num; i++)
		ddr_training_set_timing(phy_st->dmc[i].addr, timing_st->val[i]);
}
#endif /* DDR_HW_TRAINING_CONFIG || DDR_DCC_TRAINING_CONFIG */

/*
 * config ddrc exit self-refresh or exit powerdown
 * bit[3] 0x1:exit self-refresh
 * bit[3] 0x0:exit powerdown
 */
void ddr_sref_cfg(const struct ddr_cfg_st *cfg, struct dmc_cfg_sref_st *cfg_sref,
	unsigned int value)
{
	unsigned int i;
	const struct ddr_phy_st *phy_st = NULL;

	if (cfg == NULL || cfg_sref == NULL) {
		ddr_error("Pointer parameter is NULL!");
		return;
	}

	phy_st = &cfg->phy[cfg->phy_idx];
	for (i = 0; i < phy_st->dmc_num; i++) {
		cfg_sref->val[i] = reg_read(phy_st->dmc[i].addr + DDR_DMC_CFG_SREF);
		reg_write((cfg_sref->val[i] & (~DMC_CFG_INIT_XSREF_PD_MASK)) | value,
			phy_st->dmc[i].addr + DDR_DMC_CFG_SREF);
	}
}

/* Restore DMC_CFG_SREF config */
void ddr_sref_cfg_restore(const struct ddr_cfg_st *cfg, const struct dmc_cfg_sref_st *cfg_sref)
{
	unsigned int i;

	const struct ddr_phy_st *phy_st = &cfg->phy[cfg->phy_idx];
	for (i = 0; i < phy_st->dmc_num; i++)
		reg_write(cfg_sref->val[i], phy_st->dmc[i].addr + DDR_DMC_CFG_SREF);
}

/*
 * Update delay setting in registers to PHY immediately.
 * Make delay setting take effect.
 */
void ddr_phy_cfg_update(unsigned int base_phy)
{
	unsigned int tmp;

	tmp = reg_read(base_phy + DDR_PHY_MISC);
	tmp |= (1 << PHY_MISC_UPDATE_BIT);
	/* update new config to PHY */
	reg_write(tmp, base_phy + DDR_PHY_MISC);
	tmp &= ~(1 << PHY_MISC_UPDATE_BIT);
	reg_write(tmp, base_phy + DDR_PHY_MISC);
	tmp = reg_read(base_phy + DDR_PHY_PHYINITCTRL);
	/* set 1 to issue PHY counter reset signal */
	tmp |= (1 << PHY_PHYCONN_RST_BIT);
	reg_write(tmp, base_phy + DDR_PHY_PHYINITCTRL);
	/* set 0 to end the reset signal */
	tmp &= ~(1 << PHY_PHYCONN_RST_BIT);
	reg_write(tmp, base_phy + DDR_PHY_PHYINITCTRL);

	ddr_asm_dsb();
}

/* Set delay value of the bit delay line of the DATA block */
static void ddr_phy_set_dq_bdl(const struct ddr_cfg_st *cfg, unsigned int value)
{
	unsigned int val;
	unsigned int offset;
	unsigned int dq;
	unsigned int base_phy;
	unsigned int byte_index;
	unsigned int rank;

	base_phy = cfg->cur_phy;
	byte_index = cfg->cur_byte;
	rank = cfg->rank_idx;
	dq = cfg->cur_dq & 0x7;
	if (cfg->cur_mode == DDR_MODE_WRITE) {
		if (dq < DDR_DQ_NUM_EACH_REG) /* [DXNWDQNBDL0] 4 bdl: wdq0bdl-wdq3bdl */
			offset = ddr_phy_dxnwdqnbdl0(rank, byte_index);
		else /* [DXNWDQNBDL1] 4 bdl: wdq4bdl-wdq7bdl */
			offset = ddr_phy_dxnwdqnbdl1(rank, byte_index);
	} else {
		if (dq < DDR_DQ_NUM_EACH_REG) /* [DXNRDQNBDL0] 4 bdl: rdq0bdl-rdq3bdl */
			offset = ddr_phy_dxnrdqnbdl0(rank, byte_index);
		else /* [DXNRDQNBDL1] 4 bdl: rdq4bdl-rdq7bdl */
			offset = ddr_phy_dxnrdqnbdl1(rank, byte_index);
	}
	dq &= 0x3; /* one register contains 4 dq */
	val = reg_read(base_phy + offset);
	val &= ~(0xFF << (dq << DDR_DQBDL_SHIFT_BIT));
	val |= ((PHY_BDL_MASK & value) << ((dq << DDR_DQBDL_SHIFT_BIT) + PHY_BDL_DQ_BIT));
	reg_write(val, base_phy + offset);

	ddr_phy_cfg_update(base_phy);
}

/* Get PHY DQ value */
static unsigned int ddr_phy_get_dq_bdl(const struct ddr_cfg_st *cfg)
{
	unsigned int val;
	unsigned int offset;
	unsigned int dq;
	unsigned int byte_index;
	unsigned int rank;

	byte_index = cfg->cur_byte;
	rank = cfg->rank_idx;
	dq = cfg->cur_dq & 0x7;
	if (cfg->cur_mode == DDR_MODE_WRITE) {
		if (dq < DDR_DQ_NUM_EACH_REG) /* [DXNWDQNBDL0] 4 bdl: wdq0bdl-wdq3bdl */
			offset = ddr_phy_dxnwdqnbdl0(rank, byte_index);
		else /* [DXNWDQNBDL1] 4 bdl: wdq4bdl-wdq7bdl */
			offset = ddr_phy_dxnwdqnbdl1(rank, byte_index);
	} else {
		if (dq < DDR_DQ_NUM_EACH_REG) /* [DXNRDQNBDL0] 4 bdl: rdq0bdl-rdq3bdl */
			offset = ddr_phy_dxnrdqnbdl0(rank, byte_index);
		else /* [DXNRDQNBDL1] 4 bdl: rdq4bdl-rdq7bdl */
			offset = ddr_phy_dxnrdqnbdl1(rank, byte_index);
	}

	dq &= 0x3;  /* one register contains 4 dq */
	val = (reg_read(cfg->cur_phy + offset) >>
		((dq << DDR_DQBDL_SHIFT_BIT) + PHY_BDL_DQ_BIT)) & PHY_BDL_MASK;

	return val;
}

static void ddr_rdqs_sync_rdm(const struct ddr_cfg_st *cfg, int offset)
{
	unsigned int rdqnbdl;
	int rdm;

	rdqnbdl = reg_read(cfg->cur_phy + ddr_phy_dxnrdqnbdl2(cfg->rank_idx, cfg->cur_byte));
	rdm = (rdqnbdl >> PHY_RDM_BDL_BIT) & PHY_RDM_BDL_MASK;
	rdm += offset;
	rdm = ((rdm < 0) ? 0 : rdm);
	rdm = ((rdm > PHY_RDM_BDL_MASK) ? PHY_RDM_BDL_MASK : rdm);
	rdqnbdl = rdqnbdl & (~(PHY_RDM_BDL_MASK << PHY_RDM_BDL_BIT));
	reg_write(rdqnbdl | ((unsigned int)rdm << PHY_RDM_BDL_BIT),
		cfg->cur_phy + ddr_phy_dxnrdqnbdl2(cfg->rank_idx, cfg->cur_byte));
}

static void ddr_wdqs_sync_wdm(const struct ddr_cfg_st *cfg, int offset)
{
	unsigned int wdqnbdl;
	int wdm;

	wdqnbdl = reg_read(cfg->cur_phy + ddr_phy_dxnwdqnbdl2(cfg->rank_idx, cfg->cur_byte));
	wdm = (wdqnbdl >> PHY_WDM_BDL_BIT) & PHY_WDM_BDL_MASK;
	wdm += offset;
	wdm = ((wdm < 0) ? 0 : wdm);
	wdm = ((wdm > PHY_WDM_BDL_MASK) ? PHY_WDM_BDL_MASK : wdm);
	wdqnbdl = wdqnbdl & (~(PHY_WDM_BDL_MASK << PHY_WDM_BDL_BIT));
	reg_write(wdqnbdl | ((unsigned int)wdm << PHY_WDM_BDL_BIT),
		cfg->cur_phy + ddr_phy_dxnwdqnbdl2(cfg->rank_idx, cfg->cur_byte));
}

static void ddr_sync_wdqsbdl(const struct ddr_cfg_st *cfg, int offset)
{
	unsigned int wdqsdly;
	int wdqsbdl;

	wdqsdly = reg_read(cfg->cur_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, cfg->cur_byte));
	wdqsbdl = (wdqsdly >> PHY_WDQS_BDL_BIT) & PHY_WDQS_BDL_MASK;
	wdqsbdl += offset;
	wdqsbdl = ((wdqsbdl < 0) ? 0 : wdqsbdl);
	wdqsbdl = ((wdqsbdl > PHY_WDQS_BDL_MASK) ? PHY_WDQS_BDL_MASK : wdqsbdl);
	wdqsdly = wdqsdly & (~(PHY_WDQS_BDL_MASK << PHY_WDQS_BDL_BIT));
	reg_write(wdqsdly | ((unsigned int)wdqsbdl << PHY_WDQS_BDL_BIT),
		cfg->cur_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, cfg->cur_byte));
}

static void ddr_rdqs_sync_rank_rdq(struct ddr_cfg_st *cfg, int offset)
{
	int dq_val;
	int i;
	unsigned int cur_mode = cfg->cur_mode;

	cfg->cur_mode = DDR_MODE_READ;

	/* sync other rank rdm */
	ddr_rdqs_sync_rdm(cfg, offset);

	/* sync other rank rdq */
	ddr_debug("Before sync rank[%x] byte[%x] dq[%x = %x][%x = %x] offset[%x]",
		cfg->rank_idx, cfg->cur_byte,
		cfg->cur_phy + ddr_phy_dxnrdqnbdl0(cfg->rank_idx, cfg->cur_byte),
		reg_read(cfg->cur_phy + ddr_phy_dxnrdqnbdl0(cfg->rank_idx, cfg->cur_byte)),
		cfg->cur_phy + ddr_phy_dxnrdqnbdl1(cfg->rank_idx, cfg->cur_byte),
		reg_read(cfg->cur_phy + ddr_phy_dxnrdqnbdl1(cfg->rank_idx, cfg->cur_byte)), offset);

	for (i = 0; i < DDR_PHY_BIT_NUM; i++) {
		cfg->cur_dq = i;
		dq_val = (int)ddr_phy_get_dq_bdl(cfg);
		dq_val += offset;
		dq_val = ((dq_val < 0) ? 0 : dq_val);
		dq_val = ((dq_val > PHY_BDL_MASK) ? PHY_BDL_MASK : dq_val);
		ddr_phy_set_dq_bdl(cfg, dq_val);
	}

	cfg->cur_mode = cur_mode; /* restore to current mode */

	ddr_debug("After sync rank[%x] byte[%x] dq[%x = %x][%x = %x]",
		cfg->rank_idx, cfg->cur_byte,
		cfg->cur_phy + ddr_phy_dxnrdqnbdl0(cfg->rank_idx, cfg->cur_byte),
		reg_read(cfg->cur_phy + ddr_phy_dxnrdqnbdl0(cfg->rank_idx, cfg->cur_byte)),
		cfg->cur_phy + ddr_phy_dxnrdqnbdl1(cfg->rank_idx, cfg->cur_byte),
		reg_read(cfg->cur_phy + ddr_phy_dxnrdqnbdl1(cfg->rank_idx, cfg->cur_byte)));
}

static void ddr_wdqs_sync_rank_wdq(struct ddr_cfg_st *cfg, int offset)
{
	int dq_val;
	int i;
	unsigned int cur_mode = cfg->cur_mode;

	cfg->cur_mode = DDR_MODE_WRITE;

	/* sync other rank wdm */
	ddr_wdqs_sync_wdm(cfg, offset);

	/* sync other rank wdq */
	ddr_debug("Before sync rank[%x] byte[%x] dq[%x = %x][%x = %x] offset[%x]",
		cfg->rank_idx, cfg->cur_byte,
		cfg->cur_phy + ddr_phy_dxnwdqnbdl0(cfg->rank_idx, cfg->cur_byte),
		reg_read(cfg->cur_phy + ddr_phy_dxnwdqnbdl0(cfg->rank_idx, cfg->cur_byte)),
		cfg->cur_phy + ddr_phy_dxnwdqnbdl1(cfg->rank_idx, cfg->cur_byte),
		reg_read(cfg->cur_phy + ddr_phy_dxnwdqnbdl1(cfg->rank_idx, cfg->cur_byte)), offset);

	for (i = 0; i < DDR_PHY_BIT_NUM; i++) {
		cfg->cur_dq = i;
		dq_val = (int)ddr_phy_get_dq_bdl(cfg);
		dq_val += offset;
		dq_val = ((dq_val < 0) ? 0 : dq_val);
		dq_val = ((dq_val > PHY_BDL_MASK) ? PHY_BDL_MASK : dq_val);
		ddr_phy_set_dq_bdl(cfg, dq_val);
	}

	cfg->cur_mode = cur_mode; /* restore to current mode */

	ddr_debug("After sync rank[%x] byte[%x] dq[%x = %x][%x = %x]",
		cfg->rank_idx, cfg->cur_byte,
		cfg->cur_phy + ddr_phy_dxnwdqnbdl0(cfg->rank_idx, cfg->cur_byte),
		reg_read(cfg->cur_phy + ddr_phy_dxnwdqnbdl0(cfg->rank_idx, cfg->cur_byte)),
		cfg->cur_phy + ddr_phy_dxnwdqnbdl1(cfg->rank_idx, cfg->cur_byte),
		reg_read(cfg->cur_phy + ddr_phy_dxnwdqnbdl1(cfg->rank_idx, cfg->cur_byte)));
}

static void ddr_rdqbdl_adj(struct ddr_cfg_st *cfg, struct ddr_bdl_dly_st *bdl_dly_s)
{
	int i;
	const int value_num = 10;
	unsigned int rank = cfg->rank_idx;
	unsigned int min = 0xffffffff;
	unsigned int rdm, rdqs;
	unsigned int cur_mode = cfg->cur_mode;

	cfg->cur_mode = DDR_MODE_READ;

	rdm = reg_read(cfg->cur_phy + ddr_phy_dxnrdqnbdl2(rank, cfg->cur_byte));
	rdqs = reg_read(cfg->cur_phy + ddr_phy_dxnrdqsdly(cfg->cur_byte));

	/* get dq value */
	for (i = 0; i < DDR_PHY_BIT_NUM; i++) {
		cfg->cur_dq = i;
		bdl_dly_s->value[i] = ddr_phy_get_dq_bdl(cfg);
	}
	bdl_dly_s->value[8] = (rdm >> PHY_RDM_BDL_BIT) & PHY_RDM_BDL_MASK; /* bdl[8]: save rdmbdl */
	bdl_dly_s->value[9] = (rdqs >> PHY_RDQS_BDL_BIT) & PHY_RDQS_BDL_MASK; /* bdl[9]: rdqsbdl */

	for (i = 0; i < value_num; i++) {
		if (bdl_dly_s->value[i] < min)
			min = bdl_dly_s->value[i];
	}

	/* subtract minimum */
	for (i = 0; i < value_num; i++)
		bdl_dly_s->value[i] = bdl_dly_s->value[i] - min;

	/* set dq value */
	for (i = 0; i < DDR_PHY_BIT_NUM; i++) {
		cfg->cur_dq = i;
		ddr_phy_set_dq_bdl(cfg, bdl_dly_s->value[i]);
	}

	rdm = (rdm & (~(PHY_RDM_BDL_MASK << PHY_RDM_BDL_BIT))) |
		(bdl_dly_s->value[8] << PHY_RDM_BDL_BIT); /* bdl[8]: save rdmbdl */
	rdqs = (rdqs & (~(PHY_RDQS_BDL_MASK << PHY_RDQS_BDL_BIT))) |
		(bdl_dly_s->value[9] << PHY_RDQS_BDL_BIT); /* bdl[9]: rdqsbdl */

	reg_write(rdm, cfg->cur_phy + ddr_phy_dxnrdqnbdl2(rank, cfg->cur_byte));
	reg_write(rdqs, cfg->cur_phy + ddr_phy_dxnrdqsdly(cfg->cur_byte));

	cfg->cur_mode = cur_mode; /* restore to current mode */
}

static void ddr_get_dly_value(struct tr_dq_adj_st *wdq_st,
	unsigned int base_phy, unsigned int rank_idx, unsigned int byte_idx)
{
	/* wdqs */
	wdq_st->wdqsdly = reg_read(base_phy + ddr_phy_dxwdqsdly(rank_idx, byte_idx));
	wdq_st->wdqsphase = (wdq_st->wdqsdly >> PHY_WDQS_PHASE_BIT) & PHY_WDQS_PHASE_MASK;
	/* wdq */
	wdq_st->wdqdly = reg_read(base_phy + ddr_phy_dxnwdqdly(rank_idx, byte_idx));
	wdq_st->wdqphase = (wdq_st->wdqdly >> PHY_WDQ_PHASE_BIT) & PHY_WDQ_PHASE_MASK;
	/* wlsl */
	wdq_st->dxnwlsl = reg_read(base_phy + ddr_phy_dxnwlsl(rank_idx, byte_idx));
	wdq_st->wlsl = (wdq_st->dxnwlsl >> PHY_WLSL_BIT) & PHY_WLSL_MASK;
}

static void ddr_restore_dly_value(const struct tr_dq_adj_st *wdq_st,
	unsigned int base_phy, unsigned int rank_idx, unsigned int byte_idx)
{
	reg_write(wdq_st->wdqsdly, base_phy + ddr_phy_dxwdqsdly(rank_idx, byte_idx));
	reg_write(wdq_st->wdqdly, base_phy + ddr_phy_dxnwdqdly(rank_idx, byte_idx));
	reg_write(wdq_st->dxnwlsl, base_phy + ddr_phy_dxnwlsl(rank_idx, byte_idx));
	ddr_phy_cfg_update(base_phy);
}

static void ddr_judge_wdq_rank(struct ddr_cfg_st *cfg, unsigned int byte_idx,
	struct tr_dq_adj_st *wdq_rank0, struct tr_dq_adj_st *wdq_rank1)
{
	int skew;
	int phase2bdl;
	int wdqphase_rank0_tmp0, wdqphase_rank1_tmp0, wdqphase_rank0_tmp1, wdqphase_rank1_tmp1;
	unsigned int cur_rank;

	cur_rank = cfg->rank_idx;
	phase2bdl = ((reg_read(cfg->cur_phy + ddr_phy_dxnrdqsdly(byte_idx)) >> PHY_RDQS_CYC_BIT) &
		PHY_RDQS_CYC_MASK) / PHY_WDQSPHASE_NUM_T;
	wdqphase_rank0_tmp0 = wdq_rank0->wdqphase & 0xf; /* 0xf:bit[3:0] */
	wdqphase_rank1_tmp0 = wdq_rank1->wdqphase & 0xf; /* 0xf:bit[3:0] */

	/*
	 * Remove phase holes
	 * phase hole in every 4 wdqphase reg value
	 */
	wdqphase_rank0_tmp1 = wdqphase_rank0_tmp0 - (wdqphase_rank0_tmp0 + 1) / 4; /* 4 wdqphase */
	wdqphase_rank1_tmp1 = wdqphase_rank1_tmp0 - (wdqphase_rank1_tmp0 + 1) / 4; /* 4 wdqphase */

	if (wdqphase_rank0_tmp1 >= wdqphase_rank1_tmp1) {
		skew = wdqphase_rank0_tmp1 - wdqphase_rank1_tmp1;
		cfg->rank_idx = 0; /* 0: adjust rank0 */
		if ((skew > (PHY_WDQPHASE_NUM_T >> 1)) && (wdq_rank1->wdqphase > PHY_WDQSPHASE_REG_NUM_T)) {
			skew = PHY_WDQPHASE_NUM_T - skew;
			wdq_rank1->wdqphase = wdq_rank1->wdqphase - PHY_WDQSPHASE_REG_NUM_T;
			cfg->rank_idx = 1; /* 1: adjust rank1 */
		}
	} else {
		skew = wdqphase_rank1_tmp1 - wdqphase_rank0_tmp1;
		cfg->rank_idx = 1; /* 1: adjust rank1 */
		if ((skew > (PHY_WDQPHASE_NUM_T >> 1)) && (wdq_rank0->wdqphase > PHY_WDQSPHASE_REG_NUM_T)) {
			skew = PHY_WDQPHASE_NUM_T - skew;
			wdq_rank0->wdqphase = wdq_rank0->wdqphase - PHY_WDQSPHASE_REG_NUM_T;
			cfg->rank_idx = 0; /* 0: adjust rank0 */
		}
	}
	if (cfg->rank_idx == 0) {
		wdq_rank0->wdqphase = wdq_rank0->wdqphase - wdqphase_rank0_tmp0 + wdqphase_rank1_tmp0;
		reg_write((wdq_rank0->wdqdly & (~(PHY_WDQ_PHASE_MASK << PHY_WDQ_PHASE_BIT))) |
			(wdq_rank0->wdqphase << PHY_WDQ_PHASE_BIT), cfg->cur_phy + ddr_phy_dxnwdqdly(cfg->rank_idx, byte_idx));
	} else if (cfg->rank_idx == 1) {
		wdq_rank1->wdqphase = wdq_rank1->wdqphase - wdqphase_rank1_tmp0 + wdqphase_rank0_tmp0;
		reg_write((wdq_rank1->wdqdly & (~(PHY_WDQ_PHASE_MASK << PHY_WDQ_PHASE_BIT))) |
			(wdq_rank1->wdqphase << PHY_WDQ_PHASE_BIT), cfg->cur_phy + ddr_phy_dxnwdqdly(cfg->rank_idx, byte_idx));
	}
	ddr_wdqs_sync_rank_wdq(cfg, phase2bdl * skew);
	cfg->rank_idx = cur_rank; /* restore to current rank */
}

static void ddr_training_adjust_wdq(struct ddr_cfg_st *cfg)
{
	unsigned int i;
	unsigned int base_phy;

	struct tr_dq_adj_st wdq_rank0;
	struct tr_dq_adj_st wdq_rank1;

	base_phy = cfg->cur_phy;
	if (cfg->phy[cfg->phy_idx].total_byte_num > DDR_PHY_BYTE_MAX) {
		ddr_error("byte num error, byte_num = %x");
		return;
	}
	for (i = 0; i < cfg->phy[cfg->phy_idx].total_byte_num; i++) {
		cfg->cur_byte = i;

		ddr_get_dly_value(&wdq_rank0, base_phy, 0, i);
		ddr_get_dly_value(&wdq_rank1, base_phy, 1, i);

		/* select which rank to adjust */
		ddr_judge_wdq_rank(cfg, i, &wdq_rank0, &wdq_rank1);
	}
	ddr_phy_cfg_update(base_phy);
}

/* select which rank to adjust */
static int ddr_adjust_wdqs_select_rank(unsigned int byte_idx, struct ddr_cfg_st *cfg,
	struct tr_dq_adj_st *wdqs_rank0, struct tr_dq_adj_st *wdqs_rank1)
{
	int skew;
	int wdqsphase_rank0_tmp0, wdqsphase_rank1_tmp0;
	int wdqsphase_rank0_tmp1, wdqsphase_rank1_tmp1;

	wdqsphase_rank0_tmp0 = wdqs_rank0->wdqsphase & 0xf; /* 0xf:bit[4:0] */
	wdqsphase_rank1_tmp0 = wdqs_rank1->wdqsphase & 0xf; /* 0xf:bit[4:0] */
	/*
	 * Remove phase holes
	 * phase hole in every 4 wdqsphase reg value
	 */
	wdqsphase_rank0_tmp1 = wdqsphase_rank0_tmp0 - (wdqsphase_rank0_tmp0 + 1) / 4; /* 4 wdqsphase */
	wdqsphase_rank1_tmp1 = wdqsphase_rank1_tmp0 - (wdqsphase_rank1_tmp0 + 1) / 4; /* 4 wdqsphase */

	if (wdqsphase_rank0_tmp1 >= wdqsphase_rank1_tmp1) {
		skew = wdqsphase_rank0_tmp1 - wdqsphase_rank1_tmp1;
		cfg->rank_idx = 0; /* 0: adjust rank0 */
		if ((skew > (PHY_WDQPHASE_NUM_T >> 1)) && (wdqs_rank1->wlsl >= 1) &&
			(wdqs_rank1->wdqphase < (PHY_WDQ_PHASE_MASK - PHY_WDQPHASE_REG_NUM_T))) {
			skew = PHY_WDQSPHASE_NUM_T - skew;
			wdqs_rank1->wlsl = wdqs_rank1->wlsl - 1;
			wdqs_rank1->wdqphase = wdqs_rank1->wdqphase + PHY_WDQPHASE_REG_NUM_T;
			reg_write((wdqs_rank1->dxnwlsl & (~(PHY_WLSL_MASK << PHY_WLSL_BIT))) |
				(wdqs_rank1->wlsl << PHY_WLSL_BIT), cfg->cur_phy + ddr_phy_dxnwlsl(1, byte_idx));
			reg_write((wdqs_rank1->wdqdly & (~(PHY_WDQ_PHASE_MASK << PHY_WDQ_PHASE_BIT))) |
				(wdqs_rank1->wdqphase << PHY_WDQS_PHASE_BIT), cfg->cur_phy + ddr_phy_dxnwdqdly(1, byte_idx));
			cfg->rank_idx = 1; /* 1: adjust rank1 */
		}
	} else {
		skew = wdqsphase_rank1_tmp1 - wdqsphase_rank0_tmp1;
		cfg->rank_idx = 1; /* 1: adjust rank1 */
		if ((skew > (PHY_WDQPHASE_NUM_T >> 1)) && (wdqs_rank0->wlsl >= 1) &&
			(wdqs_rank0->wdqphase < (PHY_WDQ_PHASE_MASK - PHY_WDQPHASE_REG_NUM_T))) {
			skew = PHY_WDQSPHASE_NUM_T - skew;
			wdqs_rank0->wlsl = wdqs_rank0->wlsl - 1;
			wdqs_rank0->wdqphase = wdqs_rank0->wdqphase + PHY_WDQPHASE_REG_NUM_T;
			reg_write((wdqs_rank0->dxnwlsl & (~(PHY_WLSL_MASK << PHY_WLSL_BIT))) |
				(wdqs_rank0->wlsl << PHY_WLSL_BIT), cfg->cur_phy + ddr_phy_dxnwlsl(0, byte_idx));
			reg_write((wdqs_rank0->wdqdly & (~(PHY_WDQ_PHASE_MASK << PHY_WDQ_PHASE_BIT))) |
				(wdqs_rank0->wdqphase << PHY_WDQS_PHASE_BIT), cfg->cur_phy + ddr_phy_dxnwdqdly(0, byte_idx));
			cfg->rank_idx = 0; /* 0: adjust rank0 */
		}
	}

	return skew;
}

static void ddr_judge_wdqs_rank(struct ddr_cfg_st *cfg, unsigned int byte_idx,
	struct tr_dq_adj_st *wdqs_rank0, struct tr_dq_adj_st *wdqs_rank1)
{
	int skew;
	int phase2bdl;
	int wdqsphase_rank0_tmp0, wdqsphase_rank1_tmp0;
	unsigned int cur_rank;

	cur_rank = cfg->rank_idx;
	phase2bdl = ((reg_read(cfg->cur_phy + ddr_phy_dxnrdqsdly(byte_idx)) >> PHY_RDQS_CYC_BIT) &
		PHY_RDQS_CYC_MASK) / PHY_WDQSPHASE_NUM_T;

	wdqsphase_rank0_tmp0 = wdqs_rank0->wdqsphase & 0xf; /* 0xf:bit[4:0] */
	wdqsphase_rank1_tmp0 = wdqs_rank1->wdqsphase & 0xf; /* 0xf:bit[4:0] */

	skew = ddr_adjust_wdqs_select_rank(byte_idx, cfg, wdqs_rank0, wdqs_rank1);

	if (cfg->rank_idx == 0) {
		wdqs_rank0->wdqsphase = wdqs_rank0->wdqsphase - wdqsphase_rank0_tmp0 + wdqsphase_rank1_tmp0;
		reg_write((wdqs_rank0->wdqsdly & (~(PHY_WDQS_PHASE_MASK << PHY_WDQS_PHASE_BIT))) |
			(wdqs_rank0->wdqsphase << PHY_WDQS_PHASE_BIT), cfg->cur_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, byte_idx));
	} else if (cfg->rank_idx == 1) {
		wdqs_rank1->wdqsphase = wdqs_rank1->wdqsphase - wdqsphase_rank1_tmp0 + wdqsphase_rank0_tmp0;
		reg_write((wdqs_rank1->wdqsdly & (~(PHY_WDQS_PHASE_MASK << PHY_WDQS_PHASE_BIT))) |
			(wdqs_rank1->wdqsphase << PHY_WDQS_PHASE_BIT), cfg->cur_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, byte_idx));
	}

	ddr_sync_wdqsbdl(cfg, phase2bdl * skew);
	cfg->rank_idx = cur_rank; /* restore to current rank */
}

static void ddr_training_adjust_wdqs(struct ddr_cfg_st *cfg)
{
	unsigned int i;
	unsigned int base_phy;
	struct tr_dq_adj_st wdqs_rank0;
	struct tr_dq_adj_st wdqs_rank1;

	base_phy = cfg->cur_phy;
	if (cfg->phy[cfg->phy_idx].total_byte_num > DDR_PHY_BYTE_MAX) {
		ddr_error("byte num error, byte_num = %x", cfg->phy[cfg->phy_idx].total_byte_num);
		return;
	}
	for (i = 0; i < cfg->phy[cfg->phy_idx].total_byte_num; i++) {
		cfg->cur_byte = i;

		ddr_get_dly_value(&wdqs_rank0, base_phy, 0, i);
		ddr_get_dly_value(&wdqs_rank1, base_phy, 1, i);

		/* select which rank to adjust */
		ddr_judge_wdqs_rank(cfg, i, &wdqs_rank0, &wdqs_rank1);
	}
	ddr_phy_cfg_update(base_phy);
}

#ifdef DDR_MPR_TRAINING_CONFIG
/* Check MPR read data */
int ddr_mpr_check(const struct ddr_cfg_st *cfg)
{
	/* read data */
	ddr_dmc_sfc_cmd(cfg->cur_dmc, DMC_CMD_TYPE_READ, 0x0, 0x0);

	return dmc_mpr_check_bit_0_127(cfg);
}
#else
int ddr_mpr_check(const struct ddr_cfg_st *cfg)
{
	return 0;
}
#endif

#ifdef DDR_TRAINING_DEBUG
#define ddr_trining_break_point_func(name) ddr_training_break_point(name)
#else
#define ddr_trining_break_point_func(name)
#endif

void ddr_training_break_point(const char *name)
{
	ddr_info(name);
	ddr_training_console_if();
}

#define __DATAEYE_ADJUST__
#ifdef DDR_TRAINING_ADJUST_CONFIG
static unsigned int ddr_adjust_get_average(const struct ddr_cfg_st *cfg)
{
	unsigned int dq0_3, dq4_7, val;
	unsigned int base_phy = cfg->cur_phy;
	unsigned int byte_index = cfg->cur_byte;
	unsigned int rank = cfg->rank_idx;

	if (cfg->cur_mode == DDR_MODE_WRITE)
		return (reg_read(base_phy + ddr_phy_dxnwdqnbdl2(rank, byte_index)) >>
			PHY_WDM_BDL_BIT) & PHY_BDL_MASK;

	/* read */
	dq0_3 = reg_read(base_phy + ddr_phy_dxnrdqnbdl0(rank, byte_index));
	dq4_7 = reg_read(base_phy + ddr_phy_dxnrdqnbdl1(rank, byte_index));

	val = ((dq0_3 >> PHY_BDL_DQ0_BIT) & PHY_BDL_MASK) +
		((dq0_3 >> PHY_BDL_DQ1_BIT) & PHY_BDL_MASK) +
		((dq0_3 >> PHY_BDL_DQ2_BIT) & PHY_BDL_MASK) +
		((dq0_3 >> PHY_BDL_DQ3_BIT) & PHY_BDL_MASK) +
		((dq4_7 >> PHY_BDL_DQ0_BIT) & PHY_BDL_MASK) +
		((dq4_7 >> PHY_BDL_DQ1_BIT) & PHY_BDL_MASK) +
		((dq4_7 >> PHY_BDL_DQ2_BIT) & PHY_BDL_MASK) +
		((dq4_7 >> PHY_BDL_DQ3_BIT) & PHY_BDL_MASK);

	val = val >> 3; /* shift 3: 8 dq */

	return val;
}

/*
 * @accel : Return a value to adjust quickly.
 * Check dataeye DQ window on left or right or middle.
 */
static unsigned int ddr_adjust_trend_check(const struct ddr_cfg_st *cfg, int *accel)
{
	unsigned int dq_bdl;
	unsigned int size;

	/* 32 BDL middle[13, 17]. 128 BDL middle[40, 56] */
	/* 1 Phase = (DDR_BDL_PHASE_TRANSFORM) BDL */
	size = DDR_BDL_PHASE_TRANSFORM >> 1;
	dq_bdl = ddr_adjust_get_average(cfg);

	/* increase adjust step to accelerate */
	if (accel != NULL) {
		if (dq_bdl > PHY_DQ_BDL_MIDDLE)
			*accel = dq_bdl - PHY_DQ_BDL_MIDDLE;
		else if (dq_bdl < PHY_DQ_BDL_MIDDLE)
			*accel = PHY_DQ_BDL_MIDDLE - dq_bdl;

		ddr_info("byte[%x] bdl[%x] middle[%x] accel[%x] rdqs[%x]",
			cfg->cur_byte, dq_bdl, PHY_DQ_BDL_MIDDLE, *accel,
			(reg_read(cfg->cur_phy + ddr_phy_dxnrdqsdly(cfg->cur_byte)) >>
			PHY_RDQS_BDL_BIT) & PHY_RDQS_BDL_MASK);
	}

	/* window on left */
	if (dq_bdl < (PHY_DQ_BDL_MIDDLE - size))
		return DDR_WIN_LEFT;
	/* on right */
	else if (dq_bdl > (PHY_DQ_BDL_MIDDLE + size))
		return DDR_WIN_RIGHT;
	else
		return DDR_WIN_MIDDLE;
}

/* Check adjust value whether valid */
static int ddr_adjust_check_val(int val, unsigned int mode)
{
	if (mode == DDR_MODE_READ) {
		if (val < 0 || val > PHY_RDQS_BDL_MASK)
			return DDR_FALSE;
	} else {
		if (val < 0 || val > PHY_WDQ_PHASE_MASK)
			return DDR_FALSE;
	}

	return DDR_TRUE;
}

static void ddr_rdqs_sync(struct ddr_cfg_st *cfg, int val)
{
	unsigned int rdqsdly;
	unsigned int cur_rank = cfg->rank_idx;
	int old, offset;

	rdqsdly = reg_read(cfg->cur_phy + ddr_phy_dxnrdqsdly(cfg->cur_byte));
	old = (rdqsdly >> PHY_RDQS_BDL_BIT) & PHY_RDQS_BDL_MASK;
	offset = val - old;

	/* sync rdm */
	ddr_rdqs_sync_rank_rdq(cfg, offset);

	if (cfg->phy[cfg->phy_idx].rank_num == 1) {
		ddr_debug("Rank number[%x] not need sync another rank", cfg->phy[cfg->phy_idx].rank_num);
		return;
	}

	/* sync other rank rdm and rdq */
	cfg->rank_idx = DDR_SUPPORT_RANK_MAX - 1 - cur_rank; /* switch to another rank */
	ddr_rdqs_sync_rank_rdq(cfg, offset);
	cfg->rank_idx = cur_rank; /* resotre to cur rank */
}

static void ddr_set_rdqs(struct ddr_cfg_st *cfg, int val)
{
	unsigned int delay;
	delay = reg_read(cfg->cur_phy + ddr_phy_dxnrdqsdly(cfg->cur_byte));

	ddr_phy_rdqs_sync_rdm(cfg, val);

	/* clear rdqs bdl */
	delay = delay & (~(PHY_RDQS_BDL_MASK << PHY_RDQS_BDL_BIT));

	reg_write(delay | ((unsigned int)val << PHY_RDQS_BDL_BIT),
		cfg->cur_phy + ddr_phy_dxnrdqsdly(cfg->cur_byte));
}

/* Get value which need to adjust */
static int ddr_adjust_get_val(const struct ddr_cfg_st *cfg)
{
	if (cfg == NULL) {
		ddr_error("Pointer parameter cfg is NULL!");
		return 0;
	}
	if (cfg->cur_mode == DDR_MODE_READ)
		return (reg_read(cfg->cur_phy + ddr_phy_dxnrdqsdly(cfg->cur_byte)) >>
			PHY_RDQS_BDL_BIT) & PHY_RDQS_BDL_MASK;
	else
		return (reg_read(cfg->cur_phy + ddr_phy_dxnwdqdly(cfg->rank_idx, cfg->cur_byte)) >>
			PHY_WDQ_PHASE_BIT) & PHY_WDQ_PHASE_MASK;
}

/* Set value which need to adjust */
static void ddr_adjust_set_val(struct ddr_cfg_st *cfg, int val)
{
	unsigned int delay;

	if (cfg->cur_mode == DDR_MODE_READ) {
		ddr_set_rdqs(cfg, val);
	} else {
		delay = reg_read(cfg->cur_phy + ddr_phy_dxnwdqdly(cfg->rank_idx, cfg->cur_byte));
		/* clear wdq phase */
		delay = delay & (~(PHY_WDQ_PHASE_MASK << PHY_WDQ_PHASE_BIT));

		reg_write(delay | ((unsigned int)val << PHY_WDQ_PHASE_BIT),
			cfg->cur_phy + ddr_phy_dxnwdqdly(cfg->rank_idx, cfg->cur_byte));
	}

	ddr_phy_cfg_update(cfg->cur_phy);
}

/* Add or delete value to adjust */
static void ddr_adjust_change_val(unsigned int dir, int *val,
	int step, unsigned int mode)
{
	if (mode == DDR_MODE_READ) {
		if (dir == DDR_WIN_RIGHT)
			(*val) = (*val) + step;
		else
			(*val) = (*val) - step;
	} else {
		/* decrease wdq phase, window move to right */
		if (dir == DDR_WIN_RIGHT)
			(*val) = (*val) - step;
		else
			(*val) = (*val) + step;
	}
}

/*
 * @dir : move direction. DDR_TRUE move to right, DDR_FALSE move to left.
 * Move window to specified direction until the best DQ bdl beyond the midline.
 */
static void ddr_adjust_move_win(struct ddr_cfg_st *cfg,
	struct training_data *training, int step, unsigned int dir)
{
	int cur_val, def_val;
	int accel;
	unsigned int i;
	unsigned int trend;
	unsigned int max_value;

	max_value = ((cfg->cur_mode == DDR_MODE_WRITE) ? PHY_WDQ_PHASE_MASK : PHY_RDQS_BDL_MASK);

	def_val = ddr_adjust_get_val(cfg);
	cur_val = def_val;
	for (i = 0; i <= max_value; i++) {
		accel = step;
		/* write mode no need to accelerate */
		if (cfg->cur_mode == DDR_MODE_WRITE)
			trend = ddr_adjust_trend_check(cfg, 0);
		else
			trend = ddr_adjust_trend_check(cfg, &accel);

		if (trend == DDR_WIN_MIDDLE || trend == dir) {
			ddr_debug("Move byte[%x] window to middle suc", cfg->cur_byte);
			break;
		}

		ddr_adjust_change_val(dir, &cur_val, accel, cfg->cur_mode);
		if (ddr_adjust_check_val(cur_val, cfg->cur_mode) == DDR_FALSE) {
			ddr_warning("Move byte[%x] to middle fail. value[%x]",
				cfg->cur_byte, cur_val);
			break;
		}

		ddr_debug("Byte[%x] mode[%x] set value[%x]",
			cfg->cur_byte, cfg->cur_mode, cur_val);
		ddr_adjust_set_val(cfg, cur_val);
		if (ddr_dataeye_deskew(cfg, training)) {
			ddr_adjust_set_val(cfg, def_val);
			/* MUST deskew dataeye after restore rdqs */
			ddr_dataeye_deskew(cfg, training);
			ddr_error("Byte[%x] deskew fail, restore[%x]", cfg->cur_byte, def_val);
			break;
		}
	}
}

/* Adjust specified byte winodw to middle */
static void ddr_adjust_byte(struct ddr_cfg_st *cfg, struct training_data *training)
{
	unsigned int trend;

	trend = ddr_adjust_trend_check(cfg, 0);
	/* window on left, move to right */
	if (trend == DDR_WIN_LEFT)
		ddr_adjust_move_win(cfg, training, DDR_DQS_ADJ_STEP, DDR_WIN_RIGHT);
	/* window on right, move to left */
	else if (trend == DDR_WIN_RIGHT)
		ddr_adjust_move_win(cfg, training, DDR_DQS_ADJ_STEP, DDR_WIN_LEFT);
	/* window on middle, no need to move */
	else
		ddr_debug("Byte[%x] mode[%x] win on middle", cfg->cur_byte, cfg->cur_mode);
}

/*
 * Adjust PHY dataeye. On normal case,
 * read dateeye window on left after read dataeye hardware training,
 * write dataeye window on left after write leveling training.
 */
static void ddr_adjust_dataeye(struct ddr_cfg_st *cfg, struct training_data *training)
{
	unsigned int i;

	/* dataeye adjust disable */
	if (ddr_training_check_bypass(cfg, DDR_BYPASS_DATAEYE_ADJ_MASK) != DDR_FALSE)
		return;

	ddr_debug("DDR dataeye adjust PHY[%x][%x] DMC[%x][%x] Rank[%x]",
		cfg->phy_idx, cfg->cur_phy, cfg->dmc_idx, cfg->cur_dmc, cfg->rank_idx);

	if (cfg->adjust == DDR_FALSE)
		return;

	if (cfg->phy_idx >= DDR_PHY_NUM || cfg->dmc_idx >= DDR_DMC_PER_PHY_MAX) {
		ddr_error("Array index phy_idx or dmc_idx out of range!");
		return;
	}
	if (get_byte_num(cfg) > DDR_PHY_BYTE_MAX) {
		ddr_error("get byte num fail, byte_num = %x", get_byte_num(cfg));
		return;
	}
	for (i = 0; i < get_byte_num(cfg); i++) {
		cfg->cur_byte = i + (cfg->dmc_idx << 1); /* byte index accord to phy */
		ddr_adjust_byte(cfg, training);
	}
}
#else
#define ddr_adjust_dataeye(cfg, training)
#endif /* DDR_TRAINING_ADJUST_CONFIG */

#define __DATAEYE_TRAINING__
#ifdef DDR_DATAEYE_TRAINING_CONFIG
/* Check dataeye dq */
static int ddr_dataeye_check_dq(const struct ddr_cfg_st *cfg)
{
	if (cfg->dq_check_type == DDR_CHECK_TYPE_DDRT)
		return ddr_ddrt_check(cfg);
	else if (cfg->dq_check_type == DDR_CHECK_TYPE_MPR)
		return ddr_mpr_check(cfg);
	else
		ddr_error("DDR dataeye dq check type not set");

	return 0;
}

/* Check dq whether valid and set mask to reduce search time */
static int ddr_dataeye_check_dir(unsigned int direction, unsigned int left,
	unsigned int right, unsigned int *mask, const struct ddr_cfg_st *cfg)
{
	int result;

	result = ddr_dataeye_check_dq(cfg);
	switch (direction) {
	case DDR_FIND_DQ_BOTH:
		*mask = DDR_FIND_DQ_LEFT | DDR_FIND_DQ_RIGHT;
		break;
	case DDR_FIND_DQ_LEFT:
		if (result) {
			/* ddr test error, search opposite side */
			*mask = DDR_FIND_DQ_RIGHT;
		} else { /* ddr test ok */
			ddr_phy_set_dq_bdl(cfg, left);
			if (!ddr_dataeye_check_dq(cfg))
				/* test ok, go on search this side */
				*mask = DDR_FIND_DQ_LEFT;
		}
		break;
	case DDR_FIND_DQ_RIGHT:
		if (result) {  /* ddr test error, search opposite side */
			*mask = DDR_FIND_DQ_LEFT;
		} else { /* ddr test ok */
			ddr_phy_set_dq_bdl(cfg, right);
			if (!ddr_dataeye_check_dq(cfg))
				/* test OK, go on search this side */
				*mask = DDR_FIND_DQ_RIGHT;
		}
		break;
	default:
		break;
	}

	return result;
}

/* Binary search the valid dq bdl */
static void ddr_dataeye_search_dq(unsigned int left, unsigned int right,
	int *target, unsigned int direction, const struct ddr_cfg_st *cfg)
{
	unsigned int middle;
	unsigned int mask = 0;

	middle = left + ((right - left) >> 1);

	ddr_phy_set_dq_bdl(cfg, middle);
	if (!ddr_dataeye_check_dir(direction, left, right, &mask, cfg)) { /* test ok */
		*target = (int)middle;
		return;
	}

	if (left == middle || middle == right) /* not found */
		return;

	/* find left side */
	if (DDR_FIND_DQ_LEFT & mask)
		ddr_dataeye_search_dq(left, middle, target, direction, cfg);

	/* find right side */
	if (DDR_FIND_DQ_RIGHT & mask)
		ddr_dataeye_search_dq(middle, right, target, direction, cfg);

	return;
}

/* Find DQ valid range */
static void ddr_dataeye_find_dq(const struct ddr_cfg_st *cfg, struct training_data *training)
{
	int cur_dq, left_dq, right_dq, def_dq;
	unsigned int dq_num;
	unsigned int win_num;

	dq_num = (cfg->cur_byte << DDR_BYTE_DQ) + cfg->cur_dq;
	def_dq = (int)ddr_phy_get_dq_bdl(cfg);
	cur_dq = def_dq;

	/* check default dq */
	if (ddr_dataeye_check_dq(cfg)) {
		/* test error */
		cur_dq = -1;
		ddr_dataeye_search_dq(0, PHY_BDL_MASK, &cur_dq, DDR_FIND_DQ_BOTH, cfg);
		ddr_debug("DQ[%x] def[%x] not ok, find new value[%x]", dq_num, def_dq, cur_dq);
		if (cur_dq == -1) {  /* no valid dq */
			training->ddr_bit_result[dq_num] = 0;
			training->ddr_bit_best[dq_num] = 0;
			/* restore default value */
			ddr_phy_set_dq_bdl(cfg, def_dq);
			ddr_warning("DQ[%x] not found dq. restore[%x]", dq_num, def_dq);
			return;
		}
	}
	/* find the left boundary */
	left_dq = cur_dq;
	ddr_dataeye_search_dq(0, cur_dq, &left_dq, DDR_FIND_DQ_LEFT, cfg);
	while (left_dq > 0) {
		left_dq--;
		ddr_phy_set_dq_bdl(cfg, left_dq);
		if (ddr_dataeye_check_dq(cfg)) {
			/* test error */
			left_dq++;
			break;
		}
	}
	/* find the right boundary */
	right_dq = cur_dq;
	ddr_dataeye_search_dq(cur_dq, PHY_BDL_MASK, &right_dq, DDR_FIND_DQ_RIGHT, cfg);
	while (right_dq < PHY_BDL_MASK) {
		right_dq++;
		ddr_phy_set_dq_bdl(cfg, right_dq);
		if (ddr_dataeye_check_dq(cfg)) {
			/* test error */
			right_dq--;
			break;
		}
	}
	/* reset dq */
	ddr_phy_set_dq_bdl(cfg, def_dq);

	/*
	 * 0 1 2 3 4 5 6 7 8 9
	 * x x - - - - - x x x
	 *     |       |
	 * left_dq   right_dq
	 *
	 * so left_dq = 2, right_dq = 6
	 */
	win_num = right_dq - left_dq + 1;
	training->ddr_bit_result[dq_num] = ((unsigned int)left_dq << DDR_DATAEYE_RESULT_BIT) |
		(unsigned int)right_dq;
	training->ddr_bit_best[dq_num] = (win_num << DDR_DATAEYE_RESULT_BIT) |
		((win_num >> 1) + (unsigned int)left_dq);

	ddr_info("DQ[%x] range: left[%x] right[%x] best[%x] mode[%x] rank[%x]", dq_num,
		left_dq, right_dq, training->ddr_bit_best[dq_num], cfg->cur_mode, cfg->rank_idx);
}

/* DDR dataeye training one byte. */
int ddr_dataeye_deskew(struct ddr_cfg_st *cfg, struct training_data *training)
{
	unsigned int dq_num;
	unsigned int loop_times = 0;
	unsigned int win_num, dq_sum;
	unsigned int def_dq, best_dq;
	int i;

	if (cfg == NULL || training == NULL) {
		ddr_error("Pointer parameter is NULL");
		return -1;
	}
	dq_sum = 0;
	training->ddr_win_sum = 0;
	for (i = 0; i < DDR_PHY_BIT_NUM; i++) {
		cfg->cur_dq = i;
		dq_num = (cfg->cur_byte << DDR_BYTE_DQ) + i;
		if (dq_num >= DDR_PHY_BIT_MAX) {
			ddr_error("Array index dq_num out of range");
			return -1;
		}
		def_dq = ddr_phy_get_dq_bdl(cfg);
		ddr_dataeye_find_dq(cfg, training);
		win_num = training->ddr_bit_best[dq_num] >> DDR_DATAEYE_RESULT_BIT;
		best_dq = training->ddr_bit_best[dq_num] & DDR_DATAEYE_RESULT_MASK;
		/* check window number */
		if (win_num < DDR_DATAEYE_WIN_NUM) {
			if (loop_times < DDR_LOOP_TIMES_LMT) {
				loop_times++;
				i--;
				continue;
			} else if (win_num == 0) {
				ddr_warning("Byte[%x] DQ[%x] no win", cfg->cur_byte, dq_num);
				/* restore default value */
				ddr_phy_set_dq_bdl(cfg, def_dq);
				ddr_training_stat(DDR_ERR_DATAEYE, cfg->cur_phy, cfg->cur_byte, i);
				continue;
			}
		}
		loop_times = 0;
		ddr_phy_set_dq_bdl(cfg, best_dq);
		dq_sum = dq_sum + best_dq;
		training->ddr_win_sum = training->ddr_win_sum + win_num;
	}
	dq_sum = dq_sum >> DDR_BYTE_DQ;

	/* only DDR_MODE_WRITE need to set */
	if (cfg->cur_mode == DDR_MODE_WRITE)
		reg_write((dq_sum & PHY_BDL_MASK) << PHY_WDM_BDL_BIT, cfg->cur_phy +
			ddr_phy_dxnwdqnbdl2(cfg->rank_idx, cfg->cur_byte));

	ddr_phy_cfg_update(cfg->cur_phy);

	return 0;
}

/* DDR write or read dataeye training */
static int ddr_dataeye_process(struct ddr_cfg_st *cfg, struct training_data *training)
{
	int result = 0;
	unsigned int i;

	if (cfg->phy_idx >= DDR_PHY_NUM || cfg->dmc_idx >= DDR_DMC_PER_PHY_MAX) {
		ddr_error("Array index phy_idx or dmc_idx out of range!");
		return -1;
	}
	if (get_byte_num(cfg) > DDR_PHY_BYTE_MAX) {
		ddr_error("get byte num fail, byte_num = %x", get_byte_num(cfg));
		return -1;
	}
	/* dataeye training */
	for (i = 0; i < get_byte_num(cfg); i++) {
		cfg->cur_byte = i + (cfg->dmc_idx << 1); /* byte index accord to phy */
		result += ddr_dataeye_deskew(cfg, training);
	}

	if (result) {
		result = -1;
		ddr_error("PHY[%x] mode[%x] dataeye training fail", cfg->cur_phy, cfg->cur_mode);
	} else {
		/* dataeye training result adjust */
		ddr_adjust_dataeye(cfg, training);
	}
	/* save training result to printf */
	ddr_result_data_save(cfg, training);

	return result;
}

int ddr_dataeye_training(struct ddr_cfg_st *cfg)
{
	struct training_data tmp_result;
	struct training_data *training = &tmp_result;
	int result_read, result_write;

	ddr_debug("DDR dataeye training PHY[%x][%x] DMC[%x][%x] Rank[%x]",
		cfg->phy_idx, cfg->cur_phy, cfg->dmc_idx, cfg->cur_dmc, cfg->rank_idx);

	/* write dataeye training */
	cfg->cur_mode = DDR_MODE_WRITE;
	ddrtr_set_data(training, 0, sizeof(struct training_data));
	result_write = ddr_dataeye_process(cfg, training);

	/* read dataeye training */
	cfg->cur_mode = DDR_MODE_READ;
	ddrtr_set_data(training, 0, sizeof(struct training_data));
	result_read = ddr_dataeye_process(cfg, training);
	if (result_read || result_write)
		return -1;
	else
		return 0;
}

int ddr_dataeye_training_func(struct ddr_cfg_st *cfg)
{
	struct tr_relate_reg relate_reg;
	int result;

	if (cfg == NULL) {
		ddr_error("Pointer parameter cfg is NULL!");
		return -1;
	}
	/* dataeye training disable */
	if (ddr_training_check_bypass(cfg, DDR_BYPASS_DATAEYE_MASK) != DDR_FALSE)
		return 0;

	ddr_training_save_reg(cfg, &relate_reg, DDR_BYPASS_DATAEYE_MASK);
	ddr_training_switch_axi(cfg);
	ddr_ddrt_init(cfg, DDR_DDRT_MODE_DATAEYE);
	cfg->adjust = DDR_DATAEYE_NORMAL_ADJUST;
	cfg->dq_check_type = DDR_CHECK_TYPE_DDRT;
	result = ddr_dataeye_training(cfg);
	ddr_training_restore_reg(cfg, &relate_reg);

	return result;
}
#else
int ddr_dataeye_training_func(struct ddr_cfg_st *cfg)
{
	ddr_warning("Not support DDR dataeye training");

	return 0;
}
#endif  /* DDR_DATAEYE_TRAINING_CONFIG */

#define __HARDWARE_TRAINING__
#ifdef DDR_HW_TRAINING_CONFIG
#ifdef DDR_HW_READ_ADJ_CONFIG
/*
 * Adjust rdqs and dq after hw read training.
 * When define DDR_TRAINING_ADJUST_DISABLE, MUST define DDR_HW_READ_ADJ_CONFIG.
 */
static void ddr_hw_read_adj(const struct ddr_cfg_st *cfg)
{
	int i;
	unsigned int base_phy = cfg->cur_phy;
	unsigned int byte_num = cfg->phy[cfg->phy_idx].total_byte_num;

	ddr_debug("DDR hw read adjust");
	/* check hw read adjust bypass bit */
	if (ddr_training_check_bypass(cfg, DDR_BYPASS_HW_ADJ_MASK) != DDR_FALSE)
		return;

	/* assume read dataeye window on left */
	for (i = 0; i < byte_num; i++) {
		reg_write(reg_read(base_phy + ddr_phy_dxnrdqnbdl0(cfg->rank_idx, i)) +
			(PHY_DQ_MIDDLE_VAL << PHY_BDL_DQ_BIT),
			base_phy + ddr_phy_dxnrdqnbdl0(cfg->rank_idx, i));
		reg_write(reg_read(base_phy + ddr_phy_dxnrdqnbdl1(cfg->rank_idx, i)) +
			(PHY_DQ_MIDDLE_VAL << PHY_BDL_DQ_BIT),
			base_phy + ddr_phy_dxnrdqnbdl1(cfg->rank_idx, i));
		reg_write(reg_read(base_phy + ddr_phy_dxnrdqsdly(i)) +
			(PHY_RDQS_MIDDLE_VAL << PHY_RDQS_BDL_BIT),
			base_phy + ddr_phy_dxnrdqsdly(i));
	}
}
#else
static void ddr_hw_read_adj(const struct ddr_cfg_st *cfg)
{
}
#endif /* DDR_HW_READ_ADJ_CONFIG */

static void ddr_training_get_rdqs(const struct ddr_cfg_st *cfg, struct ddr_bdl_st *rdqs)
{
	unsigned int i;
	unsigned int byte_num = cfg->phy[cfg->phy_idx].total_byte_num;
	unsigned int base_phy = cfg->cur_phy;

	if (byte_num > DDR_PHY_BYTE_MAX) {
		ddr_error("byte num error, byte_num = %x", byte_num);
		return;
	}
	for (i = 0; i < byte_num; i++)
		rdqs->bdl[i] = reg_read(base_phy + ddr_phy_dxnrdqsdly(i));
}

static void ddr_training_set_rdqs(const struct ddr_cfg_st *cfg, const struct ddr_bdl_st *rdqs)
{
	unsigned int i;
	unsigned int byte_num = cfg->phy[cfg->phy_idx].total_byte_num;
	unsigned int base_phy = cfg->cur_phy;

	if (byte_num > DDR_PHY_BYTE_MAX) {
		ddr_error("byte num error, byte_num = %x", byte_num);
		return;
	}
	for (i = 0; i < byte_num; i++)
		reg_write(rdqs->bdl[i], base_phy + ddr_phy_dxnrdqsdly(i));
}

static void ddr_hw_training_adjust_rdqs(struct ddr_cfg_st *cfg, const struct rdqs_data_st *rdqs_st)
{
	unsigned int i;
	unsigned int byte_num = cfg->phy[cfg->phy_idx].total_byte_num;
	unsigned int rdqs_rank0, rdqs_rank1;
	unsigned int cur_rank = cfg->rank_idx;
	int offset;

	if (byte_num > DDR_PHY_BYTE_MAX) {
		ddr_error("byte num error, byte_num = %x", byte_num);
		return;
	}
	for (i = 0; i < byte_num; i++) {
		/* struct rdqs_data_st store the whole register value */
		rdqs_rank0 = (rdqs_st->rank[0].bdl[i] >> PHY_RDQS_BDL_BIT) & PHY_RDQS_BDL_MASK;
		rdqs_rank1 = (rdqs_st->rank[1].bdl[i] >> PHY_RDQS_BDL_BIT) & PHY_RDQS_BDL_MASK;

		cfg->cur_byte = i;
		if (rdqs_rank0 > rdqs_rank1) {
			offset = rdqs_rank0 - rdqs_rank1;
			reg_write(rdqs_st->rank[0].bdl[i], cfg->cur_phy + ddr_phy_dxnrdqsdly(i));
			cfg->rank_idx = 1; /* switch to rank1 for sync rank1 rdq */
		} else {
			offset = rdqs_rank1 - rdqs_rank0;
			reg_write(rdqs_st->rank[1].bdl[i], cfg->cur_phy + ddr_phy_dxnrdqsdly(i));
			cfg->rank_idx = 0; /* switch to rank0 for sync rank0 rdq */
		}
		ddr_rdqs_sync_rank_rdq(cfg, offset);
	}
	cfg->rank_idx = cur_rank; /* restore to current rank */

	ddr_phy_cfg_update(cfg->cur_phy);
}

/* DDR HW training process */
int ddr_hw_training_process(const struct ddr_cfg_st *cfg, unsigned int item)
{
	unsigned int count;
	unsigned int base_phy = cfg->cur_phy;
	unsigned int init_ctrl = reg_read(base_phy + DDR_PHY_PHYINITCTRL);

	if (!item)
		return 0;

	ddr_debug("base_phy[%x] itme[%x]", base_phy, item);
	/* hardware training enable */
	reg_write(item | PHY_PHYINITCTRL_INIT_EN | init_ctrl, base_phy + DDR_PHY_PHYINITCTRL);

	if ((item & PHY_PHYINITCTRL_DRAM_RST) && (item & PHY_PHYINITCTRL_DRAM_INIT_EN)) {
		if (ddr_training_ctrl_easr(cfg, DDR_EXIT_SREF))
			return -1;
	}

	count = DDR_HWR_WAIT_TIMEOUT;
	/* auto cleared to 0 after training finished */
	while (count--) {
		if (!(reg_read(base_phy + DDR_PHY_PHYINITCTRL) & PHY_PHYINITCTRL_MASK))
			break;
	}

	if (count == 0xffffffff) {
		ddr_fatal("HWR wait timeout");
		ddr_training_stat(DDR_ERR_HW_RD_DATAEYE, base_phy, item, reg_read(base_phy + DDR_PHY_PHYINITSTATUS));
		return -1;
	}

	if (reg_read(base_phy + DDR_PHY_PHYINITSTATUS)) {
		ddr_fatal("Phy[%x] hw[%x] failed[%x]", base_phy, item, reg_read(base_phy + DDR_PHY_PHYINITSTATUS));
		ddr_training_stat(DDR_ERR_HW_RD_DATAEYE, base_phy, item, reg_read(base_phy + DDR_PHY_PHYINITSTATUS));
		return -1;
	}

	return 0;
}

/* Dataeye hardware training */
int ddr_hw_dataeye_read(struct ddr_cfg_st *cfg)
{
	unsigned int base_phy;
	unsigned int byte_num;

	unsigned int i;
	int result;

	if (cfg == NULL) {
		ddr_error("Pointer parameter cfg is NULL!");
		return -1;
	}
	base_phy = cfg->cur_phy;
	byte_num = cfg->phy[cfg->phy_idx].total_byte_num;

	ddr_training_cfg_init(cfg);

	if (byte_num > DDR_PHY_BYTE_MAX) {
		ddr_error("byte num error, byte_num = %x", byte_num);
		return -1;
	}
	/* clear */
	for (i = 0; i < byte_num; i++) {
		reg_write(0, base_phy + ddr_phy_dxnrdqnbdl0(cfg->rank_idx, i));
		reg_write(0, base_phy + ddr_phy_dxnrdqnbdl1(cfg->rank_idx, i));
		reg_write(0, base_phy + ddr_phy_dxnrdqsdly(i));
	}
	ddr_phy_cfg_update(base_phy);

	result = ddr_hw_training_process(cfg, PHY_PHYINITCTRL_RDET_EN);

	ddr_hw_read_adj(cfg);

	return result;
}

/* ca odt disable, DRAM_RST and DRAM_INIT are required to take effect
 * The DRAM_RST cannot be performed more than once
 */
static int ddr_hw_ca_odt_disable(const struct ddr_cfg_st *cfg)
{
	int result;
	unsigned int temp;
	unsigned int base_phy = cfg->cur_phy;

	temp = reg_read(base_phy + DDR_PHY_MODEREG01);
	reg_write(temp & 0x8fffffff, base_phy + DDR_PHY_MODEREG01); /* ca odt disable:bit[30:28] set 0 */
	result = ddr_hw_training_process(cfg, cfg->cur_item & PHY_HW_GP_DRAM_RESET);

	reg_write(temp, base_phy + DDR_PHY_MODEREG01); /* restore */

	return result;
}

/* CA Vref Sync, rank0 and rank1 */
static int ddr_hw_ca_vref_sync(const struct ddr_cfg_st *cfg)
{
	int result;
	unsigned int temp;
	unsigned int base_phy = cfg->cur_phy;
	unsigned int item = cfg->cur_item;

	temp = reg_read(base_phy + DDR_PHY_TRAINCTRL0);
	reg_write(temp & (~PHY_TRAINCTRL0_MASK), base_phy + DDR_PHY_TRAINCTRL0); /* select rank0 */
	result = ddr_hw_training_process(cfg, item & PHY_HW_GP_VREF_AC);

	reg_write((temp & (~PHY_TRAINCTRL0_MASK)) | 0x1,
		base_phy + DDR_PHY_TRAINCTRL0); /* select rank1 */
	result += ddr_hw_training_process(cfg, item & PHY_HW_GP_VREF_AC);

	reg_write(temp, base_phy + DDR_PHY_TRAINCTRL0); /* restore */

	return result;
}

static int ddr_hw_dram_mr_init(const struct ddr_cfg_st *cfg)
{
	int result;
	unsigned int tx_odt_mode;
	unsigned int base_phy = cfg->cur_phy;
	unsigned int item = cfg->cur_item;

	tx_odt_mode = (reg_read(base_phy + DDR_PHY_ACIOCTL) >> PHY_AC_IOCTL_TX_MODE_BIT) &
		PHY_AC_IOCTL_TX_MODE_MASK;
	if (tx_odt_mode == DDR_PHY_LPDDR4X_MODE) {
		unsigned int temp;
		unsigned int temp1;

		/* rank0 */
		temp = reg_read(base_phy + DDR_PHY_RANKEN);
		reg_write((temp & (~DDR_PHY_RANKEN_MASK)) | 0x1,
			base_phy + DDR_PHY_RANKEN); /* select rank0 */

		temp1 = reg_read(base_phy + DDR_PHY_MODEREG23); /* store the contents of the Mode Register */
		reg_write(temp1 & 0xffffffc7, base_phy + DDR_PHY_MODEREG23); /* rank0 ck/cs/ca odt enable */
		result = ddr_hw_training_process(cfg, item & PHY_PHYINITCTRL_DRAM_INIT_EN); /* rank0 draminit */
		/* restore */
		reg_write(temp, base_phy + DDR_PHY_RANKEN);
		reg_write(temp1, base_phy + DDR_PHY_MODEREG23);

		/* rank1 */
		temp = reg_read(base_phy + DDR_PHY_RANKEN);
		reg_write((temp & (~DDR_PHY_RANKEN_MASK)) | 0x2, /* 0x2:bit1 set 1 */
			base_phy + DDR_PHY_RANKEN); /* select rank1 */

		temp1 = reg_read(base_phy + DDR_PHY_MODEREG23);
		reg_write((temp1 & 0xffffffc7) | 0x28, base_phy + DDR_PHY_MODEREG23); /* rank1 ck/caodt diable, rank1 cs odt enable */
		result += ddr_hw_training_process(cfg, item & PHY_PHYINITCTRL_DRAM_INIT_EN);
		/* restore */
		reg_write(temp, base_phy + DDR_PHY_RANKEN);
		reg_write(temp1, base_phy + DDR_PHY_MODEREG23);
	} else {
		result = ddr_hw_training_process(cfg, item & PHY_PHYINITCTRL_DRAM_INIT_EN);
	}

	return result;
}

/* DDR HW training adapt dram type */
static int ddr_hw_dataeye_adapt(const struct ddr_cfg_st *cfg, struct ddr_tmp_st *ddr_temp)
{
	int result;
	unsigned int dramtimer1 = 0;
	unsigned int modereg67;
	unsigned int base_phy = cfg->cur_phy;

	if (cfg->phy[cfg->phy_idx].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
		dramtimer1 = reg_read(base_phy + DDR_PHY_DRAMTIMER1);
		reg_write(dramtimer1 & (~(DDR_PHY_T_MOD_MASK << DDR_PHY_T_MOD_BIT)),
			base_phy + DDR_PHY_DRAMTIMER1); /* TMOD:0 */

		result = ddr_hw_ca_odt_disable(cfg); /* CA odt disable */
		result += ddr_hw_ca_vref_sync(cfg); /* CA vref sync */
		result += ddr_hw_dram_mr_init(cfg); /* in WR0 */

		modereg67 = reg_read(base_phy + DDR_PHY_MODEREG67);
		/* turn to WR1 */
		reg_write(modereg67 | (0x1 << PHY_MODEREG67_LP4_FSPWR_BIT),
			base_phy + DDR_PHY_MODEREG67); /* bit6 set 1 */
		result += ddr_hw_dram_mr_init(cfg);
		result += ddr_hw_ca_vref_sync(cfg); /* CA vref sync */

		/* turn to WR0 */
		reg_write(modereg67 & (~(0x1 << PHY_MODEREG67_LP4_FSPWR_BIT)),
			base_phy + DDR_PHY_MODEREG67); /* bit6 set 0 */
		result += ddr_hw_dram_mr_init(cfg);

		/* restore DRAMTIMER1 */
		reg_write(dramtimer1, base_phy + DDR_PHY_DRAMTIMER1);
	} else {
#ifdef DDR_WRITE_DM_DISABLE
		unsigned int modereg45 = 0;
		if (cfg->phy[cfg->phy_idx].dram_type == PHY_DRAMCFG_TYPE_DDR4) {
			modereg45 = reg_read(base_phy + DDR_PHY_MODEREG45);
			reg_write((modereg45 & 0xFBFFFFFF) | 0x8000000, base_phy + DDR_PHY_MODEREG45); /* write dm disable */
		}
		ddr_temp->temp = modereg45; /* for restore 0xe0 in ddr_hw_training_ctl */
#else
		ddr_temp->temp = 0;
#endif
		result = ddr_hw_training_process(cfg, cfg->cur_item & PHY_HW_GP_DRAM_RESET);
	}

	return result;
}

static int ddr_hw_dataeye_vref_set(const struct ddr_cfg_st *cfg)
{
	int result;
	unsigned int base_phy = cfg->cur_phy;
	unsigned int item = cfg->cur_item;
	unsigned int dvrft_ctrl;

	dvrft_ctrl = reg_read(base_phy + DDR_PHY_DVRFTCTRL);
	reg_write(dvrft_ctrl & (~PHY_DVRFTCTRL_PDAEN_EN), base_phy + DDR_PHY_DVRFTCTRL);
	/* DDR_PHY_VREFTCTRL 31bit:1 do vref dram set twice */
	reg_write((reg_read(base_phy + DDR_PHY_VREFTCTRL) &
		(~(PHY_VREFS_MRS_ENTER_MASK << PHY_VREFS_MRS_ENTER_BIT))) |
		(PHY_VREFS_MRS_ENTER_MASK << PHY_VREFS_MRS_ENTER_BIT),
		base_phy + DDR_PHY_VREFTCTRL);
	result = ddr_hw_training_process(cfg, item & PHY_HW_GP_VREF_DQ);
	result += ddr_hw_training_process(cfg, item & PHY_HW_GP_VREF_DQ);
	/* DDR_PHY_VREFTCTRL 31bit:0 do vref dram set once */
	reg_write(reg_read(base_phy + DDR_PHY_VREFTCTRL) &
		(~(PHY_VREFS_MRS_ENTER_MASK << PHY_VREFS_MRS_ENTER_BIT)),
		base_phy + DDR_PHY_VREFTCTRL);
	result += ddr_hw_training_process(cfg, item & PHY_HW_GP_VREF_DQ);
	reg_write(dvrft_ctrl, base_phy + DDR_PHY_DVRFTCTRL);

	return result;
}

#ifdef DDR_WRITE_DM_DISABLE
static int ddr_hw_write_dm_disable(const struct ddr_cfg_st *cfg, const struct ddr_tmp_st *ddr_temp)
{
	int result = 0;
	unsigned int temp;
	unsigned int temp1;

	if (cfg->phy[cfg->phy_idx].dram_type == PHY_DRAMCFG_TYPE_DDR4) {
		reg_write(ddr_temp->temp, cfg->cur_phy + DDR_PHY_MODEREG45); /* restore */
		temp = reg_read(cfg->cur_phy + DDR_PHY_MRS_SEQ_PROG);
		temp1 = reg_read(cfg->cur_phy + DDR_PHY_DRAMCFG);
		reg_write(PHY_MRS_SEQ_PROG_VAL, cfg->cur_phy + DDR_PHY_MRS_SEQ_PROG); /* inti MR5 */
		reg_write(temp1 | PHY_WDM_DISABLE_VAL, cfg->cur_phy + DDR_PHY_DRAMCFG); /* write dm disable */
		result += ddr_hw_training_process(cfg, cfg->cur_item & PHY_PHYINITCTRL_DRAM_INIT_EN);
		reg_write(temp, cfg->cur_phy + DDR_PHY_MRS_SEQ_PROG); /* restore */
		reg_write(temp1, cfg->cur_phy + DDR_PHY_DRAMCFG); /* restore */
	}

	return result;
}
#endif

/* sync rank1 WDQSPH/WDQPH to rank0 */
static void ddr_set_rank1_wdq_to_rank0(unsigned int base_phy, unsigned int byte_num)
{
	unsigned int byte_idx;

	for (byte_idx = 0; byte_idx < byte_num; byte_idx++) {
		reg_write(reg_read(base_phy + ddr_phy_dxwdqsdly(1, byte_idx)),
			base_phy + ddr_phy_dxwdqsdly(0, byte_idx));
		reg_write(reg_read(base_phy + ddr_phy_dxnwdqdly(1, byte_idx)),
			base_phy + ddr_phy_dxnwdqdly(0, byte_idx));
	}
	ddr_phy_cfg_update(base_phy);
}

/* This function is used to prevent logic bugs */
static int ddr_hw_training_normal_conf(const struct ddr_cfg_st *cfg)
{
	int result;
	unsigned int byte_idx;
	unsigned int byte_num;
	unsigned int base_phy;
	struct tr_dq_byte_st wdq_rank0_byte;

	ddrtr_set_data(&wdq_rank0_byte, 0, sizeof(struct tr_dq_byte_st));
	base_phy = cfg->cur_phy;
	byte_num = cfg->phy[cfg->phy_idx].total_byte_num;

	if (cfg->rank_idx == 0) {
		result = ddr_hw_training_process(cfg, cfg->cur_item & PHY_HW_GP_NORMAL);
	} else { /* rank1 */
		/* save rank0 WDQSPH/WDQPH of all byte */
		for (byte_idx = 0; byte_idx < byte_num; byte_idx++)
			ddr_get_dly_value(&wdq_rank0_byte.dq_val[byte_idx], base_phy, 0, byte_idx);
		/* WL */
		result = ddr_hw_training_process(cfg, cfg->cur_item & PHY_PHYINITCTRL_WL_EN);
		/* sync rank1 WDQSPH/WDQPH to rank0 */
		ddr_set_rank1_wdq_to_rank0(base_phy, byte_num);

		/* GATE/GDS/WL2/RDET/WDET */
		result += ddr_hw_training_process(cfg, cfg->cur_item & PHY_HW_GP_NORMAL_RANK1);
		/* sync rank1 WDQSPH/WDQPH to rank0 */
		ddr_set_rank1_wdq_to_rank0(base_phy, byte_num);

		/* HVREFT/DVREFT */
		result += ddr_hw_training_process(cfg, cfg->cur_item & PHY_PHYINITCTRL_HVREFT_EN);
		result += ddr_hw_training_process(cfg, cfg->cur_item & PHY_PHYINITCTRL_DVREFT_EN);
		/* sync rank1 WDQSPH/WDQPH to rank0 */
		ddr_set_rank1_wdq_to_rank0(base_phy, byte_num);

		/* TDQSST */
		result += ddr_hw_training_process(cfg, cfg->cur_item & PHY_PHYINITCTRL_PIC_TDQSST);

		/* restore rank0 WDQSPH/WDQPH of all byte */
		for (byte_idx = 0; byte_idx < byte_num; byte_idx++)
			ddr_restore_dly_value(&wdq_rank0_byte.dq_val[byte_idx], base_phy, 0, byte_idx);
	}

	return result;
}

void ddr_ck_cfg(unsigned int base_phy)
{
	unsigned int acphyctl7, acphyctl7_tmp;
	unsigned int ck0, ck1;

	acphyctl7 = reg_read(base_phy + DDR_PHY_ACPHYCTL7);
	ck0 = (acphyctl7 >> PHY_ACPHY_DCLK0_BIT) & PHY_ACPHY_DCLK_MASK;
	ck1 = (acphyctl7 >> PHY_ACPHY_DCLK1_BIT) & PHY_ACPHY_DCLK_MASK;
	/* clear ck0 ck1 */
	acphyctl7_tmp = acphyctl7 & (~(PHY_ACPHY_DCLK_MASK << PHY_ACPHY_DCLK0_BIT)) &
		(~(PHY_ACPHY_DCLK_MASK << PHY_ACPHY_DCLK1_BIT));
	/* set the opposite val of ck */
	reg_write(acphyctl7_tmp | ((~ck0) << PHY_ACPHY_DCLK0_BIT) | ((~ck1) << PHY_ACPHY_DCLK1_BIT),
		base_phy + DDR_PHY_ACPHYCTL7);
	/* restore acphyctl7 */
	reg_write(acphyctl7, base_phy + DDR_PHY_ACPHYCTL7);

	ddr_phy_cfg_update(base_phy);
}

/* DDR HW training control */
static int ddr_hw_training_ctl(struct ddr_cfg_st *cfg)
{
	int result = 0;
	unsigned int byte_idx;
	unsigned int temp;
	unsigned int base_phy = cfg->cur_phy;
	struct ddr_tmp_st ddr_temp;
	struct rdqs_data_st *rdqs_st = NULL;
	struct ddr_bdl_dly_st bdl_dly_s;

	rdqs_st = (struct rdqs_data_st *)cfg->res_st;

	if (cfg->cur_item == 0 || rdqs_st == NULL)
		return 0;

	ddr_phy_cfg_update(base_phy);
	/* NOTE: not support array when boot */
	result += ddr_hw_training_process(cfg, cfg->cur_item & PHY_HW_GP_CNT_RESET_START);
	result += ddr_hw_training_process(cfg, cfg->cur_item & PHY_HW_GP_PLL);

	ddr_ck_cfg(base_phy);

	/* save rdqs bdl after PHY_PHYINITCTRL_DLYMEAS_EN */
	if (cfg->rank_idx == 0) {
		ddr_training_get_rdqs(cfg, &rdqs_st->origin);
		temp = ((reg_read(base_phy + ddr_phy_dxnrdqsdly(0)) >> PHY_RDQS_CYC_BIT) &
			PHY_RDQS_CYC_MASK) >> 2; /* right shift 2: 1/4T */
		temp = (reg_read(base_phy + DDR_PHY_TRAINCTRL12) & (~(PHY_WL_FALLEDGE_BDL_JSTEP_R_MASK <<
			PHY_WL_FALLEDGE_BDL_JSTEP_R_BIT))) | (temp << PHY_WL_FALLEDGE_BDL_JSTEP_R_BIT);
		reg_write(temp, base_phy + DDR_PHY_TRAINCTRL12);
	}
	if ((cfg->phy_idx >= DDR_PHY_NUM) || (cfg->phy[cfg->phy_idx].total_byte_num > DDR_PHY_BYTE_MAX)) {
		ddr_error("phy_idx or byte number error");
		return -1;
	}
	for (byte_idx = 0; byte_idx < (cfg->phy[cfg->phy_idx].total_byte_num); byte_idx++) {
		cfg->cur_byte = byte_idx;
		ddr_rdqbdl_adj(cfg, &bdl_dly_s);
	}
	result += ddr_hw_dataeye_adapt(cfg, &ddr_temp);
	result += ddr_hw_training_process(cfg, cfg->cur_item & PHY_PHYINITCTRL_CAT_EN);
	result += ddr_hw_training_process(cfg, cfg->cur_item & PHY_HW_GP_CS);
	result += ddr_hw_dataeye_vref_set(cfg);
	result += ddr_hw_training_normal_conf(cfg);

#ifdef DDR_WRITE_DM_DISABLE
	result += ddr_hw_write_dm_disable(cfg, &ddr_temp);
#endif
	ddr_phy_cfg_update(base_phy);

	return result;
}

static int ddr_hw_training_by_rank(struct ddr_cfg_st *cfg)
{
	ddr_debug("PHY[%x][%x] Rank[%x] itme[%x]",
		cfg->phy_idx, cfg->cur_phy, cfg->rank_idx, cfg->cur_item);

	/* 0:PHY_TRAINCTRL0_DTR_RANK0, 1:PHY_TRAINCTRL0_DTR_RANK1 */
	ddr_phy_switch_rank(cfg->cur_phy, cfg->rank_idx);

	return ddr_hw_training_ctl(cfg);
}

int ddr_hw_training_by_phy(struct ddr_cfg_st *cfg)
{
	int result = 0;
	unsigned int i;
	struct rdqs_data_st rdqs_data;
	struct rdqs_data_st *rdqs_st = &rdqs_data;
	struct ddr_timing_st timing_st;
	unsigned int rank_num = cfg->phy[cfg->phy_idx].rank_num;

	cfg->res_st = rdqs_st;

	/* disable auto refresh */
	ddr_training_save_timing(cfg, &timing_st);

	if (rank_num > DDR_SUPPORT_RANK_MAX) {
		ddr_error("loop upper limit rank number out of range, rank_num = %x", rank_num);
		return -1;
	}
	for (i = 0; i < rank_num; i++) {
		cfg->rank_idx = i;
		if (cfg->phy_idx >= DDR_PHY_NUM) {
			ddr_error("Array index phy_idx out of range!");
			return -1;
		}
		cfg->cur_item = cfg->phy[cfg->phy_idx].rank[i].item_hw;

		result += ddr_hw_training_by_rank(cfg);

		if (rank_num != DDR_SUPPORT_RANK_MAX)
			break;

		/* save rank rdqs bdl */
		ddr_training_get_rdqs(cfg, &(rdqs_st->rank[i]));

		/* restore PHY_PHYINITCTRL_DLYMEAS_EN rdqs before training next rank */
		if ((rank_num - 1) != i)
			ddr_training_set_rdqs(cfg, &(rdqs_st->origin));
	}

	if (rank_num == DDR_SUPPORT_RANK_MAX) {
		ddr_hw_training_adjust_rdqs(cfg, rdqs_st);
		ddr_training_adjust_wdq(cfg);
		ddr_training_adjust_wdqs(cfg);
		ddr_phy_switch_rank(cfg->cur_phy, 0x0); /* switch to rank0 */
	}

	/* restore auto refresh */
	ddr_training_restore_timing(cfg, &timing_st);

	cfg->res_st = 0;

	return result;
}

/* DDR hardware training */
int ddr_hw_training(struct ddr_cfg_st *cfg)
{
	int result = 0;
	unsigned int i;
	struct tr_custom_reg reg;

	if (cfg == NULL) {
		ddr_error("Pointer parameter cfg is NULL!");
		return -1;
	}

	ddrtr_set_data(&reg, 0, sizeof(struct tr_custom_reg));
	/* save customer reg */
	ddr_boot_cmd_save(&reg);
	if (cfg->phy_num > DDR_PHY_NUM) {
		ddr_error("loop upper limit cfg->phy_num out of range!");
		return -1;
	}
	for (i = 0; i < cfg->phy_num; i++) {
		cfg->phy_idx = i;
		cfg->cur_phy = cfg->phy[i].addr;
		result += ddr_hw_training_by_phy(cfg);
	}
	/* restore customer reg */
	ddr_boot_cmd_restore(&reg);

	return result;
}
#endif /* DDR_HW_TRAINING_CONFIG */

#define __VREF_TRAINING__
#ifdef DDR_VREF_TRAINING_CONFIG
#ifdef DDR_VREF_WITHOUT_BDL_CONFIG
/* Save dataeye dq bdl before vref training */
static void ddr_vref_save_bdl(const struct ddr_cfg_st *cfg, struct tr_dq_data *dq_data)
{
	int i;
	unsigned int base_phy = cfg->cur_phy;
	unsigned int rank = cfg->rank_idx;
	unsigned int byte_index;

	for (i = 0; i < get_byte_num(cfg); i++) {
		byte_index = i + (cfg->dmc_idx << 1); /* byte index accord to phy */
		if (cfg->cur_mode == DDR_MODE_WRITE) {
			dq_data->dq03[i] = reg_read(base_phy + ddr_phy_dxnwdqnbdl0(rank, byte_index));
			dq_data->dq47[i] = reg_read(base_phy + ddr_phy_dxnwdqnbdl1(rank, byte_index));
			dq_data->wdm[i] = reg_read(base_phy + ddr_phy_dxnwdqnbdl2(rank, byte_index));
		} else {
			dq_data->dq03[i] = reg_read(base_phy + ddr_phy_dxnrdqnbdl0(rank, byte_index));
			dq_data->dq47[i] = reg_read(base_phy + ddr_phy_dxnrdqnbdl1(rank, byte_index));
		}
	}
}

/* Restore dataeye dq bdl after vref training */
static void ddr_vref_restore_bdl(const struct ddr_cfg_st *cfg, const struct tr_dq_data *dq_data)
{
	int i;
	unsigned int base_phy = cfg->cur_phy;
	unsigned int rank = cfg->rank_idx;
	unsigned int byte_index;

	if ((cfg->phy_idx >= DDR_PHY_NUM) || (cfg->dmc_idx >= DDR_DMC_PER_PHY_MAX))
		return;

	if (get_byte_num(cfg) > DDR_PHY_BYTE_MAX)
		return;

	for (i = 0; i < get_byte_num(cfg); i++) {
		byte_index = i + (cfg->dmc_idx << 1); /* byte index accord to phy */
		if (cfg->cur_mode == DDR_MODE_WRITE) {
			reg_write(dq_data->dq03[i], base_phy + ddr_phy_dxnwdqnbdl0(rank, byte_index));
			reg_write(dq_data->dq47[i], base_phy + ddr_phy_dxnwdqnbdl1(rank, byte_index));
			reg_write(dq_data->wdm[i], base_phy + ddr_phy_dxnwdqnbdl2(rank, byte_index));
		} else {
			reg_write(dq_data->dq03[i], base_phy + ddr_phy_dxnrdqnbdl0(rank, byte_index));
			reg_write(dq_data->dq47[i], base_phy + ddr_phy_dxnrdqnbdl1(rank, byte_index));
		}
	}
}
#else
static void ddr_vref_save_bdl(const struct ddr_cfg_st *cfg, struct tr_dq_data *dq_data)
{
}
static void ddr_vref_restore_bdl(const struct ddr_cfg_st *cfg, const struct tr_dq_data *dq_data)
{
}
#endif /* DDR_VREF_WITHOUT_BDL_CONFIG */

/* phy s40 not support DRAM vref */
static int ddr_vref_dram_set_process(unsigned int base_phy, unsigned int val, unsigned int byte_index)
{
	unsigned int count;
	unsigned int dvrftctrl = reg_read(base_phy + DDR_PHY_DVRFTCTRL);
	unsigned int dvreft = reg_read(base_phy + ddr_phy_dvreft_status(byte_index)) &
		(~PHY_VRFTRES_DVREF_MASK);

	reg_write(dvrftctrl | PHY_DVRFTCTRL_PDAEN_EN, base_phy + DDR_PHY_DVRFTCTRL);
	reg_write(dvreft | val, base_phy + ddr_phy_dvreft_status(byte_index));
	reg_write(PHY_PHYINITCTRL_DVREFT_SYNC | PHY_PHYINITCTRL_INIT_EN,
		base_phy + DDR_PHY_PHYINITCTRL);

	count = DDR_HWR_WAIT_TIMEOUT;
	/* auto cleared to 0 after training finished */
	while (count--) {
		if (!(reg_read(base_phy + DDR_PHY_PHYINITCTRL) & PHY_PHYINITCTRL_INIT_EN))
			break;
	}

	if (count == 0xffffffff) {
		ddr_fatal("vref dram set wait timeout");
		ddr_training_stat(DDR_ERR_HW_RD_DATAEYE, base_phy, byte_index,
			reg_read(base_phy + DDR_PHY_PHYINITSTATUS));
		return -1;
	}

	reg_write(dvrftctrl & (~PHY_DVRFTCTRL_PDAEN_EN), base_phy + DDR_PHY_DVRFTCTRL);

	return 0;
}

#if defined(DDR_PHY_T12_V100_CONFIG) || defined(DDR_PHY_T12_V101_CONFIG) || defined(DDR_PHY_S14_CONFIG)
static void ddr_phy_vref_host_set_process(unsigned int base_phy, unsigned int rank,
	unsigned int bytenum, unsigned int byte_index, unsigned int val)
{
	unsigned int hvreft;

	if (rank == 0) {
		hvreft = reg_read(base_phy + ddr_phy_hvreft_status(rank, byte_index)) &
			(~PHY_VRFTRES_HVREF_MASK);
		reg_write(hvreft | val, base_phy + ddr_phy_hvreft_status(rank, byte_index));
		reg_write(hvreft | val, base_phy + ddr_phy_hvreft_status(rank, byte_index + 1));
	} else {
		hvreft = reg_read(base_phy + ddr_phy_hvreft_status(rank, byte_index)) &
			(~(PHY_VRFTRES_RXDIFFCAL_MASK << PHY_VRFTRES_RXDIFFCAL_BIT));
		reg_write(hvreft | (val << PHY_VRFTRES_RXDIFFCAL_BIT),
			base_phy + ddr_phy_hvreft_status(rank, byte_index));
		reg_write(hvreft | (val << PHY_VRFTRES_RXDIFFCAL_BIT),
			base_phy + ddr_phy_hvreft_status(rank, byte_index + 1));
	}
}
#endif

/* Set DDR Vref value */
static void ddr_vref_set(const struct ddr_cfg_st *cfg, unsigned int val)
{
	if (cfg->cur_mode == DDR_MODE_READ) { /* HOST vref */
		ddr_phy_vref_host_set(cfg->cur_phy, cfg->rank_idx, get_byte_num(cfg), cfg->cur_byte, val);
	} else { /* DRAM vref */
		unsigned int auto_ref_timing = reg_read(cfg->cur_dmc + DDR_DMC_TIMING2);
		/* disable auto refresh */
		ddr_training_set_timing(cfg->cur_dmc, auto_ref_timing & DMC_AUTO_TIMING_DIS);

		/* DDR_PHY_VREFTCTRL 31bit:1 do vref dram set twice */
		reg_write((reg_read(cfg->cur_phy + DDR_PHY_VREFTCTRL) &
			(~(PHY_VREFS_MRS_ENTER_MASK << PHY_VREFS_MRS_ENTER_BIT))) |
			(PHY_VREFS_MRS_ENTER_MASK << PHY_VREFS_MRS_ENTER_BIT),
			cfg->cur_phy + DDR_PHY_VREFTCTRL);
		/* DRAM vref operations */
		ddr_phy_vref_dram_set(cfg->cur_phy, val, cfg->cur_byte);
		ddr_phy_vref_dram_set(cfg->cur_phy, val, cfg->cur_byte);
		/* DDR_PHY_VREFTCTRL 31bit:0 do vref dram set once */
		reg_write(reg_read(cfg->cur_phy + DDR_PHY_VREFTCTRL) &
			(~(PHY_VREFS_MRS_ENTER_MASK << PHY_VREFS_MRS_ENTER_BIT)),
			cfg->cur_phy + DDR_PHY_VREFTCTRL);
		/* DRAM vref operations */
		ddr_phy_vref_dram_set(cfg->cur_phy, val, cfg->cur_byte);
		/* enable auto refresh */
		ddr_training_set_timing(cfg->cur_dmc, auto_ref_timing);
	}
	ddr_info("byte[%x] mode[%x] set vref [%x]", cfg->cur_byte, cfg->cur_mode, val);
}

/* Get DDR Vref value */
static unsigned int ddr_vref_get(const struct ddr_cfg_st *cfg)
{
	unsigned int val = 0;

	if (cfg->cur_mode == DDR_MODE_READ) /* HOST vref */
		ddr_phy_vref_host_get(cfg->cur_phy, cfg->rank_idx, cfg->cur_byte, val);
	else /* DRAM vref */
		ddr_phy_vref_dram_get(cfg->cur_phy, val, cfg->cur_byte);

	ddr_info("byte[%x] mode[%x] get vref [%x]", cfg->cur_byte, cfg->cur_mode, val);

	return val;
}

/* Get totol win number of training result */
static unsigned int ddr_vref_get_win(struct ddr_cfg_st *cfg,
	struct training_data *training, int vref)
{
	unsigned int vref_min = 0;
	unsigned int vref_max = DDR_VREF_DRAM_VAL_MAX;
	int vref_set;

	training->ddr_win_sum = 0;

	if (cfg->cur_mode == DDR_MODE_READ)
		ddr_vref_get_host_max(cfg->rank_idx, vref_max);

	if (vref < vref_min)
		vref_set = vref_min;
	else if (vref > vref_max)
		vref_set = vref_max;
	else
		vref_set = vref;

	ddr_vref_set(cfg, vref_set);
	ddr_dataeye_deskew(cfg, training);

	return training->ddr_win_sum;
}

/* Find the best vref which win number is max */
static unsigned int ddr_vref_find_best(struct ddr_cfg_st *cfg,
	struct training_data *training, unsigned int vref, int step)
{
	int cur_vref;
	unsigned int best_vref;
	unsigned int cur_win;
	unsigned int max_win;
	unsigned int lower_times = 0;
	unsigned int vref_min = 0;
	unsigned int vref_max = DDR_VREF_DRAM_VAL_MAX;

	if (cfg->cur_mode == DDR_MODE_READ)
		ddr_vref_get_host_max(cfg->rank_idx, vref_max);

	max_win = 0;
	cur_vref = vref + step;

	if (vref < vref_min)
		best_vref = vref_min;
	else if (vref > vref_max)
		best_vref = vref_max;
	else
		best_vref = vref;

	/* find parabola vertex */
	while (cur_vref >= vref_min && cur_vref <= vref_max) {
		cur_win = ddr_vref_get_win(cfg, training, cur_vref);
		ddr_debug("byte[%x] vref[%x] win[%x] mode[%x]",
			cfg->cur_byte, cur_vref, cur_win, cfg->cur_mode);
		if (cur_win < max_win) {
			lower_times++;
			if (lower_times == DDR_VREF_COMPARE_TIMES)
				/* Continuous decline, mean found vertex */
				break;
		} else {
			lower_times = 0;
			max_win = cur_win;
			best_vref = cur_vref;
		}
		cur_vref = cur_vref + step;
	}

	return best_vref;
}

/* DDR Vref calibrate and set the best value */
static void ddr_vref_cal(struct ddr_cfg_st *cfg, struct training_data *training)
{
	unsigned int def_vref;
	unsigned int best_vref;
	unsigned int left_win;
	unsigned int right_win;

	def_vref = ddr_vref_get(cfg);
	left_win = ddr_vref_get_win(cfg, training, def_vref - DDR_VREF_COMPARE_STEP);
	right_win = ddr_vref_get_win(cfg, training, def_vref + DDR_VREF_COMPARE_STEP);

	ddr_debug("byte[%x] default vref[%x] win[%x][%x] mode[%x]",
		cfg->cur_byte, def_vref, left_win, right_win, cfg->cur_mode);

	/* With vref increments, WIN number is a parabola.
	   So firstly determine the result on left or right. */
	/* parabola vertex */
	if (left_win < right_win) { /* the result on right */
		best_vref = ddr_vref_find_best(cfg, training, def_vref, 1);
	} else if (left_win > right_win) { /* the result on left */
		best_vref = ddr_vref_find_best(cfg, training, def_vref, -1);
	} else {
		/* when (left_win == right_win), check def_vref */
		unsigned int vref_max = DDR_VREF_DRAM_VAL_MAX;
		if (cfg->cur_mode == DDR_MODE_READ)
			ddr_vref_get_host_max(cfg->rank_idx, vref_max);

		if (def_vref < (vref_max >> 1))
			best_vref = ddr_vref_find_best(cfg, training, def_vref, 1);
		else
			best_vref = ddr_vref_find_best(cfg, training, def_vref, -1);
	}

	ddr_debug("byte[%x] best vref[%x] mode[%x]", cfg->cur_byte, best_vref, cfg->cur_mode);
	ddr_vref_set(cfg, best_vref);
}

/* vref write calibrate: support DDR4 and LPDDR4
 * if the dram type is not ddr4 or lpddr4, do nothing
 */
static int ddr_vref_write(struct ddr_cfg_st *cfg, struct training_data *training)
{
	unsigned int i;
	unsigned int dram_type = cfg->phy[cfg->phy_idx].dram_type;
	unsigned int bank_group = (reg_read(cfg->cur_dmc +
		ddr_dmc_cfg_rnkvol(cfg->rank_idx)) >> DMC_CFG_MEM_BG_BIT) & DMC_CFG_MEM_BG_MASK;

	if (dram_type != PHY_DRAMCFG_TYPE_LPDDR4 && dram_type != PHY_DRAMCFG_TYPE_DDR4)
		return -1;

	if (dram_type == PHY_DRAMCFG_TYPE_LPDDR4)
		bank_group = DMC_CFG_MEM_2BG; /* lpddr4 not training byte1 byte3 */

	if (cfg->dmc_idx >= DDR_DMC_PER_PHY_MAX)
		return -1;

	if (get_byte_num(cfg) > DDR_PHY_BYTE_MAX)
		return -1;

	for (i = 0; i < get_byte_num(cfg); i++) {
		cfg->cur_byte = i + (cfg->dmc_idx << 1); /* byte index accord to phy */
		/* byte1 and byte3 bypass when 2 Bank Group */
		if ((bank_group == DMC_CFG_MEM_2BG) && ((i == 1) || (i == 3))) /* bypass byte1 and byte3 */
			continue;

		ddr_vref_cal(cfg, training);
	}

	return 0;
}

static int ddr_vref_training(struct ddr_cfg_st *cfg)
{
	struct training_data tmp_result;
	struct training_data *training = &tmp_result;
	struct tr_dq_data dq_data;
	int result = 0;
	unsigned int i;

	ddr_debug("DDR Vref[%x] training PHY[%x][%x] DMC[%x][%x] Rank[%x]",
		cfg->cur_mode, cfg->phy_idx, cfg->cur_phy, cfg->dmc_idx, cfg->cur_dmc, cfg->rank_idx);

	ddr_vref_save_bdl(cfg, &dq_data);
	ddrtr_set_data(training, 0, sizeof(struct training_data));

	if (get_byte_num(cfg) > DDR_PHY_BYTE_MAX)
		return -1;

	/* vref calibrate */
	if (cfg->cur_mode == DDR_MODE_READ) {
		for (i = 0; i < get_byte_num(cfg); i++) {
			cfg->cur_byte = i + (cfg->dmc_idx << 1); /* byte index accord to phy */
			if (cfg->cur_byte == 1 || cfg->cur_byte == 3) /* not training byte 1 and byte 3 */
				continue;

			ddr_vref_cal(cfg, training);
		}
	} else {
		if (ddr_vref_write(cfg, training))
			return 0; /* do nothing */
	}

#if !defined(DDR_VREF_WITHOUT_BDL_CONFIG) || defined(DDR_TRAINING_CMD)
	/* dataeye deskew again on best vref. */
	for (i = 0; i < get_byte_num(cfg); i++) {
		cfg->cur_byte = i + (cfg->dmc_idx << 1); /* byte index accord to phy */
		result += ddr_dataeye_deskew(cfg, training);
	}
#endif

	ddr_vref_restore_bdl(cfg, &dq_data);
	ddr_result_data_save(cfg, training);

	return result;
}

int ddr_vref_training_func(struct ddr_cfg_st *cfg)
{
	struct tr_relate_reg relate_reg;
	int result = 0;

	if (cfg == NULL) {
		ddr_error("Pointer parameter cfg is NULL!");
		return -1;
	}
	ddr_training_save_reg(cfg, &relate_reg, DDR_BYPASS_VREF_HOST_MASK);
	ddr_training_switch_axi(cfg);
	ddr_ddrt_init(cfg, DDR_DDRT_MODE_DATAEYE);
	cfg->dq_check_type = DDR_CHECK_TYPE_DDRT;

	/* host vref training disable */
	if (ddr_training_check_bypass(cfg, DDR_BYPASS_VREF_HOST_MASK) == DDR_FALSE) {
		cfg->cur_mode = DDR_MODE_READ;
		result += ddr_vref_training(cfg);
	}

	/* dram vref training enable && DDR4 */
	if (ddr_training_check_bypass(cfg, DDR_BYPASS_VREF_DRAM_MASK) == DDR_FALSE) {
		cfg->cur_mode = DDR_MODE_WRITE;
		result += ddr_vref_training(cfg);
	}
	ddr_training_restore_reg(cfg, &relate_reg);

	return result;
}
#else
int ddr_vref_training_func(struct ddr_cfg_st *cfg)
{
	ddr_warning("Not support DDR vref training");

	return 0;
}
#endif /* DDR_VREF_TRAINING_CONFIG */
