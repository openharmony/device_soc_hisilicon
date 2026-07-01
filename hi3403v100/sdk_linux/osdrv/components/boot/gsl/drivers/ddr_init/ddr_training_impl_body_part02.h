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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART02_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART02_H_

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
            if (!(reg_read(base_phy + DDR_PHY_DRAMCFG) & PHY_DRAMCFG_MA2T)) { /* set 1T */
                reg_write(0x0, base_phy + DDR_PHY_ACPHYCTL4);
            }
            break;
        case DDR_BYPASS_HW_MASK:
            if (!(reg_read(base_phy + DDR_PHY_DRAMCFG) & PHY_DRAMCFG_MA2T)) { /* set 1T */
                reg_write(0x0, base_phy + DDR_PHY_ACPHYCTL4);
            }
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
    if (!(reg_read(base_phy + DDR_PHY_DRAMCFG) & PHY_DRAMCFG_MA2T)) {
        reg_write(relate_reg->ac_phy_ctl, base_phy + DDR_PHY_ACPHYCTL4);
    }
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
            if (!(reg_read(base_dmc + DDR_DMC_CURR_FUNC) & DMC_CURR_FUNC_IN_SREF_MASK)) {
                break;
            }
        }
    } else if (sref_req == DDR_ENTER_SREF) {
        /* Enter Auto-self refresh */
        reg_write(DMC_CTRL_SREF_ENTER, base_dmc + DDR_DMC_CTRL_SREF);
        while (count--) {
            if (reg_read(base_dmc + DDR_DMC_CURR_FUNC) & DMC_CURR_FUNC_IN_SREF_MASK) {
                break;
            }
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
    for (i = 0; i < phy_st->dmc_num; i++) {
        result += ddr_training_easr(phy_st->dmc[i].addr, sref_req);
    }
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

    for (i = 0; i < phy_st->dmc_num; i++) {
        ddr_training_set_timing(phy_st->dmc[i].addr, timing_st->val[i]);
    }
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
    for (i = 0; i < phy_st->dmc_num; i++) {
        reg_write(cfg_sref->val[i], phy_st->dmc[i].addr + DDR_DMC_CFG_SREF);
    }
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
        /* [DXNWDQNBDL0] 4 bdl: wdq0bdl-wdq3bdl */
        if (dq < DDR_DQ_NUM_EACH_REG) {
            offset = ddr_phy_dxnwdqnbdl0(rank, byte_index);
        } else {
            /* [DXNWDQNBDL1] 4 bdl: wdq4bdl-wdq7bdl */
            offset = ddr_phy_dxnwdqnbdl1(rank, byte_index);
        }
    } else {
        /* [DXNRDQNBDL0] 4 bdl: rdq0bdl-rdq3bdl */
        if (dq < DDR_DQ_NUM_EACH_REG) {
            offset = ddr_phy_dxnrdqnbdl0(rank, byte_index);
        } else {
            /* [DXNRDQNBDL1] 4 bdl: rdq4bdl-rdq7bdl */
            offset = ddr_phy_dxnrdqnbdl1(rank, byte_index);
        }
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
        /* [DXNWDQNBDL0] 4 bdl: wdq0bdl-wdq3bdl */
        if (dq < DDR_DQ_NUM_EACH_REG) {
            offset = ddr_phy_dxnwdqnbdl0(rank, byte_index);
        } else {
            /* [DXNWDQNBDL1] 4 bdl: wdq4bdl-wdq7bdl */
            offset = ddr_phy_dxnwdqnbdl1(rank, byte_index);
        }
    } else {
        /* [DXNRDQNBDL0] 4 bdl: rdq0bdl-rdq3bdl */
        if (dq < DDR_DQ_NUM_EACH_REG) {
            offset = ddr_phy_dxnrdqnbdl0(rank, byte_index);
        } else {
            /* [DXNRDQNBDL1] 4 bdl: rdq4bdl-rdq7bdl */
            offset = ddr_phy_dxnrdqnbdl1(rank, byte_index);
        }
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

    i = 0;
    while (i < DDR_PHY_BIT_NUM) {
        cfg->cur_dq = i;
        dq_val = (int)ddr_phy_get_dq_bdl(cfg);
        dq_val += offset;
        dq_val = ((dq_val < 0) ? 0 : dq_val);
        dq_val = ((dq_val > PHY_BDL_MASK) ? PHY_BDL_MASK : dq_val);
        ddr_phy_set_dq_bdl(cfg, dq_val);
    }
    cfg->cur_mode = cur_mode; /* restore to current mode */
#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART02_H_ */
