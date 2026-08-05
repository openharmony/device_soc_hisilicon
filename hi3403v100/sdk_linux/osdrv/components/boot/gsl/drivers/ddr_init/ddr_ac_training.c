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

#include "ddr_training_impl.h"

#define __AC_TRAINING__
#ifdef DDR_AC_TRAINING_CONFIG
/*
 * Get clk value.
 * Assume clk0 and clk1 is the same.
 */
static int ddr_ac_get_clk(unsigned int base_phy)
{
    unsigned int val;
    unsigned int ac_phy_ctl;
    /* Static register have to read two times to get the right value. */
    ac_phy_ctl = reg_read(base_phy + DDR_PHY_ACPHYCTL7);
    ac_phy_ctl = reg_read(base_phy + DDR_PHY_ACPHYCTL7);
    /* halft_dramclk0 */
    val = (ac_phy_ctl >> PHY_ACPHY_DRAMCLK0_BIT) & PHY_ACPHY_DRAMCLK_MASK;
    val = (val << PHY_ACPHY_DRAMCLK_EXT_BIT) |
        ((ac_phy_ctl >> PHY_ACPHY_DCLK0_BIT) & PHY_ACPHY_DCLK_MASK);
    return val;
}

/* Set clk0 and clk1 the same value */
static void ddr_ac_set_clk(unsigned int base_phy, unsigned int val)
{
    unsigned int ac_phy_ctl;
    unsigned int dramclk;
    unsigned int dclk;

    dclk = val & PHY_ACPHY_DCLK_MASK;
    dramclk = (val >> PHY_ACPHY_DRAMCLK_EXT_BIT) & PHY_ACPHY_DRAMCLK_MASK;
    /* Static register have to read two times to get the right value. */
    ac_phy_ctl = reg_read(base_phy + DDR_PHY_ACPHYCTL7);
    ac_phy_ctl = reg_read(base_phy + DDR_PHY_ACPHYCTL7);
    /* clear cp1p_dclk0 */
    ac_phy_ctl &= (~(PHY_ACPHY_DCLK_MASK << PHY_ACPHY_DCLK0_BIT));
    /* clear ck2p_dclk1 */
    ac_phy_ctl &= (~(PHY_ACPHY_DCLK_MASK << PHY_ACPHY_DCLK1_BIT));
    /* clear halft_dramclk0 */
    ac_phy_ctl &= (~(PHY_ACPHY_DRAMCLK_MASK << PHY_ACPHY_DRAMCLK0_BIT));
    /* clear halft_dramclk1 */
    ac_phy_ctl &= (~(PHY_ACPHY_DRAMCLK_MASK << PHY_ACPHY_DRAMCLK1_BIT));

    ac_phy_ctl |= (dclk << PHY_ACPHY_DCLK0_BIT);  /* set cp1p_dclk0 */
    ac_phy_ctl |= (dclk << PHY_ACPHY_DCLK1_BIT);  /* set cp2p_dclk1 */
    /* set halft_dramclk0 */
    ac_phy_ctl |= (dramclk << PHY_ACPHY_DRAMCLK0_BIT);
    /* set halft_dramclk1 */
    ac_phy_ctl |= (dramclk << PHY_ACPHY_DRAMCLK1_BIT);
    reg_write(ac_phy_ctl, base_phy + DDR_PHY_ACPHYCTL7);
}

/*
 * Get cs bdl value.
 * Assume cs0 and cs 1 is the same.
 */
static int ddr_ac_get_cs(unsigned int base_phy)
{
    return (reg_read(base_phy + DDR_PHY_ACCMDBDL2) >> 1) & PHY_BDL_MASK;
}

/* Set CS value */
static void ddr_ac_set_cs(unsigned int base_phy, unsigned int val)
{
    unsigned int ac_cmd_bdl;

    ac_cmd_bdl = reg_read(base_phy + DDR_PHY_ACCMDBDL2);
    ac_cmd_bdl &= (~(PHY_BDL_MASK << PHY_ACCMD_CS0_BIT)); /* clear cs0_bdl */
    ac_cmd_bdl &= (~(PHY_BDL_MASK << PHY_ACCMD_CS1_BIT)); /* clear cs1_bdl */
    ac_cmd_bdl |= (val << PHY_ACCMD_CS0_BIT); /* set cs0_bdl */
    ac_cmd_bdl |= (val << PHY_ACCMD_CS1_BIT); /* set cs1_bdl */
    reg_write(ac_cmd_bdl, base_phy + DDR_PHY_ACCMDBDL2);
}

static int ddr_ac_ddrt_test(unsigned int mask, unsigned int base_phy)
{
    unsigned int regval;
    unsigned int times = 0;

    ddrt_reg_write(mask | DDRT_CFG_START, DDR_REG_BASE_DDRT + DDRT_OP);
    ddrt_reg_write(0, DDR_REG_BASE_DDRT + DDRT_STATUS);

    do {
        regval = ddrt_reg_read(DDR_REG_BASE_DDRT + DDRT_STATUS);
        times++;
    } while ((!(regval & DDRT_TEST_DONE_MASK)) && (times < DDRT_WAIT_TIMEOUT));

    if (times >= DDRT_WAIT_TIMEOUT) {
        ddr_fatal("DDRT wait timeout");
        ddr_training_stat(DDR_ERR_DDRT_TIME_OUT, base_phy, -1, -1);
        return -1;
    }

    /* DDRT_WRITE_ONLY_MODE */
    if ((mask & DDRT_TEST_MODE_MASK) == DDRT_WRITE_ONLY_MODE) {
        return 0;
    }
    /* DDRT_READ_ONLY_MODE */
    if (regval & DDRT_TEST_PASS_MASK) /* No error occurred, test pass. */
        return 0;
    else
        return -1;
}

/* Check CS value */
static int ddr_ac_check_cs(unsigned int base_phy, unsigned int def_cs, unsigned int step)
{
    ddr_ac_set_cs(base_phy, def_cs + step);
    ddr_phy_cfg_update(base_phy);

    ddr_ac_ddrt_test(DDRT_WRITE_ONLY_MODE, base_phy);

    ddr_ac_set_cs(base_phy, def_cs); /* restore default to check */
    ddr_phy_cfg_update(base_phy);

    return ddr_ac_ddrt_test(DDRT_READ_ONLY_MODE, base_phy);
}

/* Check CLK value */
static int ddr_ac_check_clk(const struct ddr_cfg_st *cfg, unsigned int def_clk,
    struct ddr_delay_st *def_phase, unsigned int step)
{
    int i;
    unsigned int wdqs_phase_range;
    unsigned int wdq_phase_range;
    unsigned int phase_range;
    unsigned int base_phy = cfg->cur_phy;
    unsigned int byte_num = get_byte_num(cfg);

    /* set new value */
    ddr_ac_set_clk(base_phy, def_clk + step);
    for (i = 0; i < byte_num; i++) {
        wdqs_phase_range = PHY_WDQS_PHASE_MASK -
            ((def_phase->phase[i] >> PHY_WDQS_PHASE_BIT) & PHY_WDQS_PHASE_MASK);
        wdq_phase_range  = PHY_WDQ_PHASE_MASK -
            ((def_phase->bdl[i] >> PHY_WDQ_PHASE_BIT) & PHY_WDQ_PHASE_MASK);
        phase_range = (wdqs_phase_range < wdq_phase_range) ?
            wdqs_phase_range : wdq_phase_range;
        phase_range = (phase_range < step) ? phase_range : step;

        reg_write(def_phase->phase[i] + (phase_range << PHY_WDQS_PHASE_BIT),
            base_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, i));
        reg_write(def_phase->bdl[i] + (phase_range << PHY_WDQ_PHASE_BIT),
            base_phy + ddr_phy_dxnwdqdly(cfg->rank_idx, i));
    }
    ddr_phy_cfg_update(base_phy);

    ddr_ac_ddrt_test(DDRT_WRITE_ONLY_MODE, base_phy);

    /* restore default to check */
    ddr_ac_set_clk(base_phy, def_clk);
    for (i = 0; i < byte_num; i++) {
        reg_write(def_phase->phase[i],
            base_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, i));
        reg_write(def_phase->bdl[i],
            base_phy + ddr_phy_dxnwdqdly(cfg->rank_idx, i));
    }
    ddr_phy_cfg_update(base_phy);

    return ddr_ac_ddrt_test(DDRT_READ_ONLY_MODE, base_phy);
}

/* Find CS difference */
static int ddr_ac_find_cs(unsigned int base_phy)
{
    unsigned int def_cs;
    unsigned int step;

    def_cs = ddr_ac_get_cs(base_phy);
    for (step = 1; step <= (PHY_BDL_MASK - def_cs); step++) {
        if (ddr_ac_check_cs(base_phy, def_cs, step)) {
            ddr_debug("PHY[%x] default cs[%x], find diff_cs[%x]", base_phy, def_cs, step);
            break;
        }
    }

    return step;
}

/* Find CLK difference */
static int ddr_ac_find_clk(const struct ddr_cfg_st *cfg)
{
    int i;
    unsigned int def_clk;
    unsigned int step;
    struct ddr_delay_st def_phase;
    unsigned int base_phy = cfg->cur_phy;
    unsigned int byte_num = get_byte_num(cfg);

    def_clk = ddr_ac_get_clk(base_phy);
    for (i = 0; i < byte_num; i++) {
        /* WDQS phase */
        def_phase.phase[i] = reg_read(base_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, i));
        /* WDQ phase */
        def_phase.bdl[i] = reg_read(base_phy + ddr_phy_dxnwdqdly(cfg->rank_idx, i));
    }

    for (step = 1; step <= (PHY_ACPHY_CLK_MAX - def_clk); step++) {
        if (ddr_ac_check_clk(cfg, def_clk, &def_phase, step)) {
            ddr_debug("PHY[%x] default clk[%x], find diff_clk[%x]", base_phy, def_clk, step);
            break;
        }
    }

    return step;
}

static void ddr_ac_set_phase_range(unsigned int base_phy, unsigned int def_clk,
    unsigned int diff_clk, unsigned int phase_tmp, const struct ddr_cfg_st *cfg)
{
    unsigned int i;
    unsigned int clk_phase;
    unsigned int wdqs_phase;
    unsigned int wdq_phase;
    unsigned int wdqs_phase_range;
    unsigned int wdq_phase_range;
    unsigned int phase_range;

    clk_phase = (diff_clk - phase_tmp) >> 1;

    /* set new value */
    ddr_ac_set_clk(base_phy, def_clk + clk_phase);
    for (i = 0; i < get_byte_num(cfg); i++) {
        wdqs_phase = reg_read(base_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, i));
        wdq_phase  = reg_read(base_phy + ddr_phy_dxnwdqdly(cfg->rank_idx, i));

        wdqs_phase_range = PHY_WDQS_PHASE_MASK -
            ((wdqs_phase >> PHY_WDQS_PHASE_BIT) & PHY_WDQS_PHASE_MASK);
        wdq_phase_range  = PHY_WDQ_PHASE_MASK -
            ((wdq_phase >> PHY_WDQ_PHASE_BIT) & PHY_WDQ_PHASE_MASK);
        phase_range = (wdqs_phase_range < wdq_phase_range) ? wdqs_phase_range : wdq_phase_range;
        phase_range = (phase_range < clk_phase) ? phase_range : clk_phase;
        reg_write(wdqs_phase + (phase_range << PHY_WDQS_PHASE_BIT),
            base_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, i));
        reg_write(wdq_phase + (phase_range << PHY_WDQ_PHASE_BIT),
            base_phy + ddr_phy_dxnwdqdly(cfg->rank_idx, i));
    }
    ddr_debug("PHY[%x] def clk[%x] add phase[%x]", base_phy, def_clk, clk_phase);
}

/* DDR AC training */
static int ddr_ac_training(const struct ddr_cfg_st *cfg)
{
    unsigned int diff_cs;
    unsigned int diff_clk;
    unsigned int cs_bdl;
    unsigned int phase_tmp;
    unsigned int def_clk;
    unsigned int def_cs;
    unsigned int base_phy = cfg->cur_phy;

    ddr_debug("DDR AC training");
    def_clk = ddr_ac_get_clk(base_phy);
    diff_cs = ddr_ac_find_cs(base_phy); /* setup time(bdl) */
    diff_clk = ddr_ac_find_clk(cfg); /* hold time(phase) */
    /* cs bdl transform to clk phase */
    phase_tmp = diff_cs >> DDR_BDL_PHASE_REL;

    if (diff_clk > phase_tmp) {
        ddr_ac_set_phase_range(base_phy, def_clk, diff_clk, phase_tmp, cfg);
    } else {
        def_cs = ddr_ac_get_cs(base_phy);
        cs_bdl = 0;
        if (diff_cs > (diff_clk << DDR_BDL_PHASE_REL)) {
            cs_bdl = diff_cs - (diff_clk << DDR_BDL_PHASE_REL);
        }
        ddr_ac_set_cs(base_phy, def_cs + cs_bdl);
        ddr_debug("PHY[%x] def cs[%x] add bdl[%x]", base_phy, def_cs, cs_bdl);
    }
    ddr_phy_cfg_update(base_phy);

    return 0;
}

int ddr_ac_training_func(const struct ddr_cfg_st *cfg)
{
    int result = 0;
    struct tr_relate_reg relate_reg;

    /* AC training disable */
    if (ddr_training_check_bypass(cfg, DDR_BYPASS_AC_MASK) != DDR_FALSE) {
        return 0;
    }
    ddr_training_save_reg(cfg, &relate_reg, DDR_BYPASS_AC_MASK);

    ddr_training_switch_axi(cfg);
    ddr_ddrt_init(cfg, DDR_DDRT_MODE_DATAEYE);
    result += ddr_ac_training(cfg);

    ddr_training_restore_reg(cfg, &relate_reg);

    return result;
}
#else
int ddr_ac_training_func(const struct ddr_cfg_st *cfg)
{
    ddr_warning("Not support DDR AC training");

    return 0;
}
#endif /* DDR_AC_TRAINING_CONFIG */
