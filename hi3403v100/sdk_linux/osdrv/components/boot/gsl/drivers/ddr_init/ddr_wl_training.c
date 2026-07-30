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

#define __WRITE_LEVELING__
#ifdef DDR_WL_TRAINING_CONFIG
static void ddr_bdl_add(unsigned int *raw, unsigned int val)
{
    if (((*raw) + val) > PHY_BDL_MASK) {
        *raw = PHY_BDL_MASK;
    } else
        *raw += val;
}

static void ddr_bdl_sub(unsigned int *raw, unsigned int val)
{
    if ((*raw) > val) {
        *raw -= val;
    } else
        *raw = 0;
}

/* DDR PHY DQ phase increase */
static void ddr_phase_inc(unsigned int *raw)
{
#if defined (DDR_PHY_T28_CONFIG) || defined(DDR_PHY_T16_CONFIG) || \
    defined (DDR_PHY_T12_V100_CONFIG) || defined (DDR_PHY_T12_V101_CONFIG)
    if ((*raw) < (PHY_WDQS_PHASE_MASK - 1)) {
        if (((*raw) & 0x3) == 0x2) {
            *raw += 0x2;
        } else
            *raw += 0x1;
    }
#else
    if ((*raw) < PHY_WDQS_PHASE_MASK) {
        *raw += 0x1;
    }
#endif
}

/* DDR PHY DQ phase decrease */
static void ddr_phase_dec(unsigned int *raw)
{
#if defined (DDR_PHY_T28_CONFIG) || defined(DDR_PHY_T16_CONFIG) || \
    defined (DDR_PHY_T12_V100_CONFIG) || defined (DDR_PHY_T12_V101_CONFIG)
    if ((*raw) > 0x1) {
        if (((*raw) & 0x3) == 0x3) {
            *raw -= 0x2;
        } else
            *raw -= 0x1;
    }
#else
    if ((*raw) > 0x0) {
        *raw -= 0x1;
    }
#endif
}

/* DQ bdl add or sub */
static void ddr_dq_bdl_operate(unsigned int base_phy,
    unsigned int addr_offset, unsigned int val, unsigned int is_add)
{
    unsigned int tmp;
    unsigned int dq_bdl[DDR_PHY_REG_DQ_NUM];
    int i;

    tmp = reg_read(base_phy + addr_offset);
    dq_bdl[0] = (tmp >> PHY_BDL_DQ0_BIT) & PHY_BDL_MASK; /* wdq0bdl */
    dq_bdl[1] = (tmp >> PHY_BDL_DQ1_BIT) & PHY_BDL_MASK; /* wdq1bdl */
    dq_bdl[2] = (tmp >> PHY_BDL_DQ2_BIT) & PHY_BDL_MASK; /* wdq2bdl */
    dq_bdl[3] = (tmp >> PHY_BDL_DQ3_BIT) & PHY_BDL_MASK; /* wdq3bdl */

    for (i = 0; i < DDR_PHY_REG_DQ_NUM; i++) {
        if (is_add) {
            ddr_bdl_add(&dq_bdl[i], val);
        } else
            ddr_bdl_sub(&dq_bdl[i], val);
    }

    tmp = (dq_bdl[3] << PHY_BDL_DQ3_BIT) + (dq_bdl[2] << PHY_BDL_DQ2_BIT) + /* wdq3bdl and wdq2bdl */
        (dq_bdl[1] << PHY_BDL_DQ1_BIT)  + (dq_bdl[0] << PHY_BDL_DQ0_BIT); /* wdq1bdl and wdq0bdl */
    reg_write(tmp, base_phy + addr_offset);
}

/* Disable or enable DDR write leveling mode */
static void ddr_wl_switch(unsigned int base_dmc, unsigned int base_phy, int val)
{
    unsigned int mr1_raw;
    unsigned int sfc_cmd;
    unsigned int sfc_bank;

    /* Set Rank = 0, Cmd = MRS, No Precharch CMD */
    mr1_raw = reg_read(base_phy + DDR_PHY_MODEREG01) >> PHY_MODEREG01_MR1_BIT;
    sfc_cmd  = DMC_CMD_TYPE_LMR;
    sfc_bank = DMC_BANK_MR1;

    if (val == DDR_TRUE) /* enable DDR wl */
        /* Set A7 as 1 */
        sfc_cmd  += (mr1_raw | DMC_CMD_MRS_A7) << DMC_SFC_CMD_MRS_BIT;
    else
        /* Set A7 as 0 */
        sfc_cmd  += (mr1_raw & ((~DMC_CMD_MRS_A7) & DMC_CMD_MRS_MASK)) << DMC_SFC_CMD_MRS_BIT;

    ddr_dmc_sfc_cmd(base_dmc, sfc_cmd, 0x0, sfc_bank);

    /* clear */
    if (val == DDR_FALSE) {
        reg_write(0x0, base_dmc + DDR_DMC_SFCBANK);
        reg_write(0x0, base_dmc + DDR_DMC_SFCREQ);
    }

    /* phy sw write leveling mode */
    reg_write(val, base_phy + DDR_PHY_SWTMODE);
}

#ifdef DDR_WL_DATAEYE_ADJUST_CONFIG
static void ddr_wl_wdq_cmp(const struct ddr_cfg_st *cfg, struct ddr_delay_st *wdqs_new,
    struct ddr_delay_st *wdqs_old, unsigned int byte_idx)
{
    unsigned int val;
    unsigned int phase_adj;
    unsigned int wdq_phase;
    unsigned int wdm_bdl;
    unsigned int bdl_adj = 0;  /* for write dataeye */
    unsigned int base_phy = cfg->cur_phy;
    unsigned int rank_idx = cfg->rank_idx;

    phase_adj = 0;
    wdq_phase = (reg_read(base_phy + ddr_phy_dxnwdqdly(rank_idx, byte_idx)) >>
        PHY_WDQ_PHASE_BIT) & PHY_WDQ_PHASE_MASK;
    wdm_bdl = (reg_read(base_phy + ddr_phy_dxnwdqnbdl2(rank_idx, byte_idx)) >>
        PHY_WDM_BDL_BIT) & PHY_BDL_MASK;

    if (wdqs_new->bdl[byte_idx] > wdqs_old->bdl[byte_idx]) {
        val = wdqs_new->bdl[byte_idx] - wdqs_old->bdl[byte_idx];
        phase_adj = val >> DDR_BDL_PHASE_REL;
        wdq_phase = wdq_phase + phase_adj;

        if (wdq_phase > PHY_WDQ_PHASE_MASK) {
            wdq_phase = PHY_WDQ_PHASE_MASK;
        }
        /* adjust wdq bdl and dm bdl in opposite direction */
        bdl_adj = phase_adj << DDR_BDL_PHASE_REL;
        ddr_dq_bdl_operate(base_phy, ddr_phy_dxnwdqnbdl0(rank_idx, byte_idx),
            bdl_adj, DDR_FALSE);
        ddr_dq_bdl_operate(base_phy, ddr_phy_dxnwdqnbdl1(rank_idx, byte_idx),
            bdl_adj, DDR_FALSE);
        ddr_bdl_sub(&wdm_bdl, bdl_adj);
    } else if (wdqs_new->bdl[byte_idx] < wdqs_old->bdl[byte_idx]) {
        val = wdqs_old->bdl[byte_idx] - wdqs_new->bdl[byte_idx];
        phase_adj = val >> DDR_BDL_PHASE_REL;
        wdq_phase = (wdq_phase > phase_adj) ? (wdq_phase - phase_adj) : 0;

        /* adjust wdq bdl and dm bdl in opposite direction */
        bdl_adj  = phase_adj << DDR_BDL_PHASE_REL;
        ddr_dq_bdl_operate(base_phy, ddr_phy_dxnwdqnbdl0(rank_idx, byte_idx),
            bdl_adj, DDR_TRUE);
        ddr_dq_bdl_operate(base_phy, ddr_phy_dxnwdqnbdl1(rank_idx, byte_idx),
            bdl_adj, DDR_TRUE);
        ddr_bdl_add(&wdm_bdl, bdl_adj);
    }

    ddr_info("Byte[%x] WDQ adjust phase[%x] bdl[%x]",
        byte_idx, phase_adj, bdl_adj);

    reg_write(wdq_phase << PHY_WDQ_PHASE_BIT,
        base_phy + ddr_phy_dxnwdqdly(rank_idx, byte_idx));
    reg_write(wdm_bdl << PHY_WDM_BDL_BIT, base_phy + ddr_phy_dxnwdqnbdl2(rank_idx, byte_idx));
}

/* Adjust dataeye WDQ after Write leveling */
static void ddr_wl_wdq_adjust(const struct ddr_cfg_st *cfg,
    struct ddr_delay_st *wdqs_new, struct ddr_delay_st *wdqs_old)
{
    unsigned int i;
    unsigned int byte_num = get_byte_num(cfg);

    ddr_debug("DDR WL write adjust");

    /* check wl write adjust bypass bit */
    if (ddr_training_check_bypass(cfg, DDR_BYPASS_WL_ADJ_MASK) != DDR_FALSE) {
        return;
    }
    /* adjust wdq phase, wdq bdl, wdm bdl */
    for (i = 0; i < byte_num; i++) {
        if (wdqs_new->phase[i] == wdqs_old->phase[i] &&
            wdqs_new->bdl[i]   == wdqs_old->bdl[i]) {
            continue;
        }
        ddr_wl_wdq_cmp(cfg, wdqs_new, wdqs_old, i);
    }

    ddr_phy_cfg_update(cfg->cur_phy);
}
#endif /* DDR_WL_DATAEYE_ADJUST_CONFIG */

/* Sync WDQ phase, WDQ bdl, WDM bdl, OEN bdl, WDQ SOE bdl by WDQS value */
static void ddr_wl_bdl_sync(const struct ddr_cfg_st *cfg,
    const struct ddr_delay_st *wdqs_new, const struct ddr_delay_st *wdqs_old)
{
    int i;
    unsigned int val;
    unsigned int oen_bdl;
    unsigned int wdqsoe_bdl;
    unsigned int wdm_bdl;
    unsigned int wdq_phase;
    unsigned int base_phy = cfg->cur_phy;

    /* sync wdq phase, wdq bdl, wdm bdl, oen bdl, wdq soe bdl */
    for (i = 0; i < get_byte_num(cfg); i++) {
        if (wdqs_new->phase[i] == wdqs_old->phase[i] && wdqs_new->bdl[i] == wdqs_old->bdl[i]) {
            continue;
        }
        ddr_debug("Byte[%x] new[%x][%x] old[%x][%x]", i,
            wdqs_new->phase[i], wdqs_new->bdl[i], wdqs_old->phase[i], wdqs_old->bdl[i]);

        /* wdq phase */
        wdq_phase = (reg_read(base_phy + ddr_phy_dxnwdqdly(cfg->rank_idx, i)) >>
            PHY_WDQ_PHASE_BIT) & PHY_WDQ_PHASE_MASK;
        /* always new_phase >= old_phase */
        wdq_phase = wdq_phase + (wdqs_new->phase[i] - wdqs_old->phase[i]);

        /* bdl */
        oen_bdl = (reg_read(base_phy + ddr_phy_dxnoebdl(cfg->rank_idx, i)) >>
            PHY_OEN_BDL_BIT) & PHY_BDL_MASK;
        wdqsoe_bdl = (reg_read(base_phy + ddr_phy_dxnoebdl(cfg->rank_idx, i)) >>
            PHY_WDQSOE_BDL_BIT) & PHY_BDL_MASK;
        wdm_bdl = (reg_read(base_phy + ddr_phy_dxnwdqnbdl2(cfg->rank_idx, i)) >>
            PHY_WDM_BDL_BIT) & PHY_BDL_MASK;

        if (wdqs_new->bdl[i] > wdqs_old->bdl[i]) {
            val = wdqs_new->bdl[i] - wdqs_old->bdl[i];
            ddr_dq_bdl_operate(base_phy, ddr_phy_dxnwdqnbdl0(cfg->rank_idx, i), val, DDR_TRUE);
            ddr_dq_bdl_operate(base_phy, ddr_phy_dxnwdqnbdl1(cfg->rank_idx, i), val, DDR_TRUE);
            ddr_bdl_add(&oen_bdl, val);
            ddr_bdl_add(&wdqsoe_bdl, val);
            ddr_bdl_add(&wdm_bdl, val);
        } else if (wdqs_new->bdl[i] < wdqs_old->bdl[i]) {
            val = wdqs_old->bdl[i] - wdqs_new->bdl[i];
            ddr_dq_bdl_operate(base_phy, ddr_phy_dxnwdqnbdl0(cfg->rank_idx, i), val, DDR_FALSE);
            ddr_dq_bdl_operate(base_phy, ddr_phy_dxnwdqnbdl1(cfg->rank_idx, i), val, DDR_FALSE);
            ddr_bdl_sub(&oen_bdl, val);
            ddr_bdl_sub(&wdqsoe_bdl, val);
            ddr_bdl_sub(&wdm_bdl, val);
        }

        if (wdq_phase > PHY_WDQ_PHASE_MASK) {
            wdq_phase = PHY_WDQ_PHASE_MASK;
        }
        reg_write(wdq_phase << PHY_WDQ_PHASE_BIT, base_phy + ddr_phy_dxnwdqdly(cfg->rank_idx, i));
        reg_write((wdqsoe_bdl << PHY_WDQSOE_BDL_BIT) + (oen_bdl << PHY_OEN_BDL_BIT),
            base_phy + ddr_phy_dxnoebdl(cfg->rank_idx, i));
        reg_write((wdm_bdl << PHY_WDM_BDL_BIT), base_phy + ddr_phy_dxnwdqnbdl2(cfg->rank_idx, i));
    }

    ddr_phy_cfg_update(base_phy);
}

static void ddr_wl_error(unsigned int type, unsigned int byte_num,
    unsigned int wl_result, unsigned int base_phy)
{
    int j;

    if (type == DDR_DELAY_BDL) {
        ddr_fatal("PHY[%x] WL fail, result[%x]", base_phy, wl_result);
        for (j = 0; j < byte_num; j++) {
            if (!(wl_result & (1 << j))) {
                ddr_training_stat(DDR_ERR_WL, base_phy, j, -1);
            }
        }
    } else {
        ddr_debug("PHY[%x] WL not found phase, result[%x]", base_phy, wl_result);
    }
}

/*
 * Write leveling process.
 * WL depend default WDQS phase value in register init table.
 */
static int ddr_wl_process(const struct ddr_cfg_st *cfg, unsigned int type, struct ddr_delay_st *wdqs)
{
    int i;
    int j;
    unsigned int wl_result;
    unsigned int length;
    unsigned int base_phy = cfg->cur_phy;
    unsigned int byte_num = get_byte_num(cfg);

    if (type == DDR_DELAY_PHASE)
    {
        length = PHY_WDQS_PHASE_MASK;
    } else
        length = PHY_BDL_MASK;

    /* find WDQS phase or bdl, assume CLK Delay > DQS Delay */
    for (i = 0; i <= length; i++) {
        ddr_phy_cfg_update(base_phy);
        reg_write(0x1, base_phy + DDR_PHY_SWTWLDQS);
        ddr_asm_dsb();
        wl_result = reg_read(base_phy + DDR_PHY_SWTRLT) & PHY_SWTRLT_WL_MASK;
        reg_write(0x0, base_phy + DDR_PHY_SWTWLDQS);

        if ((wl_result & ((1 << byte_num) - 1)) == ((1 << byte_num) - 1)) {
            break;
        }
        for (j = 0; j < byte_num; j++) {
            ddr_info("type[0x%x] byte[0x%x] phase[0x%x] bdl[0x%x] wl_result[0x%x]",
                type, j, wdqs->phase[j], wdqs->bdl[j], wl_result);
            if (wl_result & (1 << j)) {
                continue;
            }
            if (type == DDR_DELAY_PHASE) {
                ddr_phase_inc(&wdqs->phase[j]);
            } else
                wdqs->bdl[j] += DDR_WL_BDL_STEP;

            reg_write((wdqs->phase[j] << PHY_WDQS_PHASE_BIT) +
                (wdqs->bdl[j] << PHY_WDQS_BDL_BIT),
                base_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, j));
        }
    }

    if (i > length) {   /* wl error, not find wdqs delay */
        ddr_wl_error(type, byte_num, wl_result, base_phy);
        return -1;
    } else {
        return 0;
    }
}

/*
 * Find WDQS delay, sync to WDQ delay and OE delay.
 * WL depend default WDQS phase value in register init table.
 */
static int ddr_write_leveling(const struct ddr_cfg_st *cfg)
{
    int result;
    unsigned int i;
    unsigned int tmp;
    unsigned int base_phy = cfg->cur_phy;
    unsigned int base_dmc = cfg->cur_dmc;
    struct ddr_delay_st wdqs_old;
    struct ddr_delay_st wdqs_new;

    ddr_debug("DDR Write Leveling training");

    /* init wdqs */
    for (i = 0; i < get_byte_num(cfg); i++) {
        tmp = reg_read(base_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, i));

        wdqs_old.phase[i] = (tmp >> PHY_WDQS_PHASE_BIT) & PHY_WDQS_PHASE_MASK;
        wdqs_old.bdl[i] = (tmp >> PHY_WDQS_BDL_BIT) & PHY_BDL_MASK;

        wdqs_new.phase[i] = wdqs_old.phase[i];
        wdqs_new.bdl[i] = 0;

        /* clear wdqs bdl */
        reg_write(wdqs_new.phase[i] << PHY_WDQS_PHASE_BIT,
            base_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, i));
    }

    /* enable sw write leveling mode */
    ddr_wl_switch(base_dmc, base_phy, DDR_TRUE);

    /* find first WDQS phase, assume CLK delay > DQS delay. */
    result = ddr_wl_process(cfg, DDR_DELAY_PHASE, &wdqs_new);

    /* check phase result */
    for (i = 0; i < get_byte_num(cfg); i++) {
        /* find phase error, keep max value to find bdl. */
        /* find phase ok, decrease to find bdl. */
        if (!result) {
            ddr_phase_dec(&wdqs_new.phase[i]);
        }
        reg_write(wdqs_new.phase[i] << PHY_WDQS_PHASE_BIT,
            base_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, i));
    }

    /* find WDQS bdl */
    result = ddr_wl_process(cfg, DDR_DELAY_BDL, &wdqs_new);

    /* disable sw write leveling mode */
    ddr_wl_switch(base_dmc, base_phy, DDR_FALSE);

    if (result) {
        /* restore default value when find WDQS fail */
        for (i = 0; i < get_byte_num(cfg); i++) {
            tmp = (wdqs_old.phase[i] << PHY_WDQS_PHASE_BIT) +
                (wdqs_old.bdl[i] << PHY_WDQS_BDL_BIT);
            reg_write(tmp, base_phy + ddr_phy_dxwdqsdly(cfg->rank_idx, i));
        }
        ddr_phy_cfg_update(base_phy);
        return -1;
    }

    /* sync delay */
    ddr_wl_bdl_sync(cfg, &wdqs_new, &wdqs_old);

#ifdef DDR_WL_DATAEYE_ADJUST_CONFIG
    /* adjust WDQ for dataeye */
    ddr_wl_wdq_adjust(cfg, &wdqs_new, &wdqs_old);
#endif

    return 0;
}

int ddr_wl_func(const struct ddr_cfg_st *cfg)
{
    struct tr_relate_reg relate_reg;
    int result = 0;

    /* write leveling disable */
    if (ddr_training_check_bypass(cfg, DDR_BYPASS_WL_MASK) != DDR_FALSE) {
        return 0;
    }
    ddr_training_save_reg(cfg, &relate_reg, DDR_BYPASS_WL_MASK);

    result += ddr_write_leveling(cfg);

    ddr_training_restore_reg(cfg, &relate_reg);

    return result;
}
#else
int ddr_wl_func(const struct ddr_cfg_st *cfg)
{
    ddr_warning("Not support DDR WL training");
    return 0;
}
#endif /* DDR_WL_TRAINING_CONFIG */
