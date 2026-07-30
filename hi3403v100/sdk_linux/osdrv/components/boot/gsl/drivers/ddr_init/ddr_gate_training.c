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

#define __GATE_TRAINING__
#ifdef DDR_GATE_TRAINING_CONFIG
/* Find gate phase */
static int ddr_gate_find_phase(const struct ddr_cfg_st *cfg, struct ddr_delay_st *rdqsg)
{
    int i;
    unsigned int base_phy = cfg->cur_phy;

    for (i = 0; i < get_byte_num(cfg); i++) {
        for (rdqsg->phase[i] = PHY_RDQSG_PHASE_MAX;
            rdqsg->phase[i] > PHY_GATE_PHASE_MARGIN;
            rdqsg->phase[i] -= PHY_RDQSG_PHASE_STEP) {
            reg_write(rdqsg->phase[i] << PHY_RDQSG_PHASE_BIT,
                base_phy + ddr_phy_dxnrdqsgdly(cfg->rank_idx, i));
            ddr_phy_cfg_update(base_phy);
            if (ddr_ddrt_test(DDRT_WR_COMPRARE_MODE, i, -1) == 0)
                break;
        }
        if (rdqsg->phase[i] <= PHY_GATE_PHASE_MARGIN) {
            /* find gate phase fail */
            ddr_fatal("find gate phase[%x] fail", rdqsg->phase[i]);
            ddr_training_stat(DDR_ERR_GATING, base_phy, -1, -1);
            return -1;
        } else {
            /* decrease one setp to find bdl */
            rdqsg->phase[i] -= PHY_RDQSG_PHASE_STEP;
            reg_write(rdqsg->phase[i] << PHY_RDQSG_PHASE_BIT,
                base_phy + ddr_phy_dxnrdqsgdly(cfg->rank_idx, i));
        }
    }
    ddr_phy_cfg_update(base_phy);

    return 0;
}

static void ddr_gate_find_bdl_by_byte(const struct ddr_cfg_st *cfg, struct ddr_delay_st *rdqsg,
    unsigned int byte_num, unsigned int gate_result)
{
    int j;
    unsigned int tmp;

    for (j = 0; j < byte_num; j++) {
        if (!(gate_result & (1 << j))) {
            rdqsg->bdl[j] += DDR_GATE_BDL_STEP;
            if (rdqsg->bdl[j] > PHY_BDL_MASK)
                tmp = ((rdqsg->bdl[j] - PHY_BDL_MASK - 1) << PHY_RDQSG_TX_BDL_BIT) +
                    (rdqsg->phase[j] << PHY_RDQSG_PHASE_BIT) + (PHY_BDL_MASK - 1);
            else
                tmp = (rdqsg->phase[j] << PHY_RDQSG_PHASE_BIT) + rdqsg->bdl[j];

            reg_write(tmp, cfg->cur_phy + ddr_phy_dxnrdqsgdly(cfg->rank_idx, j));
        }
    }
}

/* Find gate bdl */
static int ddr_gate_find_bdl(const struct ddr_cfg_st *cfg, struct ddr_delay_st *rdqsg)
{
    int i;
    int j;
	unsigned int gate_result;
	unsigned int base_phy = cfg->cur_phy;
	unsigned int byte_num = get_byte_num(cfg);
	unsigned int swtmode = reg_read(base_phy + DDR_PHY_SWTMODE);

    for (i = 0; i < byte_num; i++)
        rdqsg->bdl[i] = 0;

    /* enable phy sw gate training mode */
    reg_write(swtmode | (1 << PHY_SWTMODE_SW_GTMODE_BIT), base_phy + DDR_PHY_SWTMODE);

    for (i = 0; i < PHY_GATE_BDL_MAX; i++) {
        ddr_phy_cfg_update(base_phy);
        ddr_ddrt_test(DDRT_READ_ONLY_MODE, -1, -1);
        gate_result = (reg_read(base_phy + DDR_PHY_SWTRLT) >> PHY_SWTRLT_GATE_BIT) &
            PHY_SWTRLT_GATE_MASK;
        if (gate_result == ((1 << byte_num) - 1))
            break;

        ddr_gate_find_bdl_by_byte(cfg, rdqsg, byte_num, gate_result);
    }

    /* disable phy sw gate training mode */
    reg_write(swtmode & (~(1 << PHY_SWTMODE_SW_GTMODE_BIT)), base_phy + DDR_PHY_SWTMODE);

    if (i == PHY_GATE_BDL_MAX) {  /* find gate bdl fail */
        ddr_fatal("PHY[%x] find gate bdl fail. result[%x]", base_phy, gate_result);
        for (j = 0; j < byte_num; j++) {
            if (!(gate_result & (1 << j)))
                ddr_training_stat(DDR_ERR_GATING, base_phy, j, -1);
        }
        return -1;
    } else {
        return 0;
    }
}

static int ddr_gate_training(const struct ddr_cfg_st *cfg)
{
    unsigned int i;
    unsigned int tmp;
	unsigned int byte_num;
	struct ddr_delay_st rdqsg;
	unsigned int def_delay[DDR_PHY_BYTE_MAX];
	int result;
	unsigned int base_phy = cfg->cur_phy;

    ddr_debug("DDR Gate training");

    byte_num = get_byte_num(cfg);

    for (i = 0; i < byte_num; i++)
        def_delay[i] = reg_read(base_phy + ddr_phy_dxnrdqsgdly(cfg->rank_idx, i));

    /* find phase first */
    result = ddr_gate_find_phase(cfg, &rdqsg);
    /* find bdl */
    if (!result)
        result = ddr_gate_find_bdl(cfg, &rdqsg);

    /* set new phase */
    if (!result) {
        for (i = 0; i < byte_num; i++) {
            rdqsg.phase[i] -= PHY_GATE_PHASE_MARGIN;
            tmp = reg_read(base_phy + ddr_phy_dxnrdqsgdly(cfg->rank_idx, i));
            tmp &= ~(PHY_RDQSG_PHASE_MASK << PHY_RDQSG_PHASE_BIT);
            tmp |= rdqsg.phase[i] << PHY_RDQSG_PHASE_BIT;
            reg_write(tmp, base_phy + ddr_phy_dxnrdqsgdly(cfg->rank_idx, i));
        }
    } else {
        /* restore default value */
        for (i = 0; i < byte_num; i++)
            reg_write(def_delay[i], base_phy + ddr_phy_dxnrdqsgdly(cfg->rank_idx, i));
    }
    ddr_phy_cfg_update(base_phy);

    return 0; /* use default value and not reset */
}

int ddr_gating_func(const struct ddr_cfg_st *cfg)
{
    struct tr_relate_reg relate_reg;
    int result = 0;

    /* gate training disable */
    if (ddr_training_check_bypass(cfg, DDR_BYPASS_GATE_MASK) != DDR_FALSE) {
        /* check hardware gating */
        if (reg_read(cfg->cur_phy + DDR_PHY_PHYINITSTATUS) & PHY_INITSTATUS_GT_MASK) {
            ddr_fatal("PHY[%x] hw gating fail", cfg->cur_phy);
            ddr_training_stat(DDR_ERR_HW_GATING, cfg->cur_phy, -1, -1);
            return -1;
        }
        return 0;
    }

    ddr_training_save_reg(cfg, &relate_reg, DDR_BYPASS_GATE_MASK);

    ddr_training_switch_axi(cfg);
    ddr_ddrt_init(cfg, DDR_DDRT_MODE_GATE);
    result += ddr_gate_training(cfg);

    ddr_training_restore_reg(cfg, &relate_reg);

    return result;
}
#else
int ddr_gating_func(const struct ddr_cfg_st *cfg)
{
    ddr_warning("Not support DDR gate training");

    return 0;
}
#endif /* DDR_GATE_TRAINING_CONFIG */
