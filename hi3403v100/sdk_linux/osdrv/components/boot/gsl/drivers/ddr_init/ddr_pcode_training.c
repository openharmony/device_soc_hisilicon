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

#define __PCODE_TRAINING__
#ifdef DDR_PCODE_TRAINING_CONFIG
/* Set pcode value to register IMPSTATUS and DDR_PHY_IMP_STATUS1 */
static void ddr_pcode_set_value(unsigned int base_phy, unsigned int pcode_value)
{
    unsigned int imp_ctrl1;

    reg_write((reg_read(base_phy + DDR_PHY_IMPSTATUS) &
        (~(PHY_ZCODE_PDRV_MASK << PHY_ZCODE_PDRV_BIT))) |
        (pcode_value << PHY_ZCODE_PDRV_BIT), base_phy + DDR_PHY_IMPSTATUS);
    ddr_debug("cur IMPSTATUS [%x] = [%x]",
        base_phy + DDR_PHY_IMPSTATUS, reg_read(base_phy + DDR_PHY_IMPSTATUS));

    imp_ctrl1 = reg_read(base_phy + DDR_PHY_IMP_CTRL1);
    /* ac_vddq_cal_en set 0 */
    reg_write(imp_ctrl1 & (~(0x1 << PHY_AC_VDDQ_CAL_EN_BIT)), base_phy + DDR_PHY_IMP_CTRL1);

    reg_write((reg_read(base_phy + DDR_PHY_IMP_STATUS1) &
        (~(PHY_ACCTL_PDRV_LATCH_MASK << PHY_ACCTL_PDRV_LATCH_BIT))) |
        (pcode_value << PHY_ACCTL_PDRV_LATCH_BIT), base_phy + DDR_PHY_IMP_STATUS1);
    ddr_debug("cur IMP_STATUS1 [%x] = [%x]",
        base_phy + DDR_PHY_IMP_STATUS1, reg_read(base_phy + DDR_PHY_IMP_STATUS1));

    /* restore ac_vddq_cal_en */
    reg_write(imp_ctrl1, base_phy + DDR_PHY_IMP_CTRL1);
}

static int ddr_pcode_trainig_by_phy(const struct ddr_cfg_st *cfg)
{
    unsigned int times = 0;
    unsigned int base_phy = cfg->cur_phy;
    unsigned int pcode_value;
    unsigned int osc_rpt_vld;
    unsigned int osc_cnt_rdata;
    int ddr_freq;

    /* test start */
    reg_write(reg_read(base_phy + DDR_PHY_CORNER_DETECTOR) | PHY_OSC_START_MASK,
        base_phy + DDR_PHY_CORNER_DETECTOR);

    do {
        osc_rpt_vld = (reg_read(base_phy + DDR_PHY_CORNER_DETECTOR) >>
            PHY_OSC_RPT_VLD) & PHY_OSC_RPT_VLD_MASK;
        times++;
    } while ((!osc_rpt_vld) && (times < DDRT_PCODE_WAIT_TIMEOUT));

    if (times >= DDRT_PCODE_WAIT_TIMEOUT) {
        ddr_fatal("IO pcode training wait timeout");
        return -1;
    }

    osc_cnt_rdata = (reg_read(base_phy + DDR_PHY_CORNER_DETECTOR) >>
        PHY_OSC_CNT_RDATA_BIT) & PHY_OSC_CNT_RDATA_MASK;

    /* test stop */
    reg_write(reg_read(base_phy + DDR_PHY_CORNER_DETECTOR) &
        (~PHY_OSC_START_MASK), base_phy + DDR_PHY_CORNER_DETECTOR);

    ddr_freq = ddr_get_cksel();

    /* get pcode_value: a formula based on simulation */
    pcode_value =
        (490960 - (89 * osc_cnt_rdata * ddr_freq) / 300) / 10000; /* y equal (490960 - (89*x*fre)/300)/10000 */

    ddr_debug("pcode value[%x]", pcode_value);
    if (pcode_value < PHY_PCODE_MIN) {
        pcode_value = PHY_PCODE_MIN;
    } else if (pcode_value > PHY_PCODE_MAX) {
        pcode_value = PHY_PCODE_MAX;
    }

    /* set pcode value */
    ddr_pcode_set_value(base_phy, pcode_value);
    return 0;
}

int ddr_pcode_training(struct ddr_cfg_st *cfg)
{
    struct tr_relate_reg relate_reg;
    int result = 0;
    int i;

    if (cfg == NULL) {
        return -1;
    }
    for (i = 0; i < cfg->phy_num; i++) {
        cfg->phy_idx = i;
        cfg->cur_phy = cfg->phy[i].addr;
        cfg->cur_item = cfg->phy[i].rank[0].item;

        if (ddr_training_check_bypass(cfg, 1 << (cfg->phy_idx)) != DDR_FALSE) {
            continue;
        }
        /* pcode training disable */
        if (ddr_training_check_bypass(cfg, DDR_BYPASS_PCODE_MASK) != DDR_FALSE) {
            continue;
        }
        ddr_training_save_reg(cfg, &relate_reg, DDR_BYPASS_PCODE_MASK);
        result += ddr_pcode_trainig_by_phy(cfg);
        ddr_training_restore_reg(cfg, &relate_reg);
    }

    return result;
}
#else
int ddr_pcode_training(struct ddr_cfg_st *cfg)
{
    ddr_warning("Not support DDR pcode training");
    return 0;
}
#endif
