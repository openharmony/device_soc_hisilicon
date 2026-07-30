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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART06_SUB01_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART06_SUB01_H_

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
    if (ddr_training_check_bypass(cfg, DDR_BYPASS_HW_ADJ_MASK) != DDR_FALSE) {
        return;
    }
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
    for (i = 0; i < byte_num; i++) {
        rdqs->bdl[i] = reg_read(base_phy + ddr_phy_dxnrdqsdly(i));
    }
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
    for (i = 0; i < byte_num; i++) {
        reg_write(rdqs->bdl[i], base_phy + ddr_phy_dxnrdqsdly(i));
    }
}

static void ddr_hw_training_adjust_rdqs(struct ddr_cfg_st *cfg, const struct rdqs_data_st *rdqs_st)
{
    unsigned int i;
    unsigned int byte_num = cfg->phy[cfg->phy_idx].total_byte_num;
    unsigned int rdqs_rank0;
    unsigned int rdqs_rank1;
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

    if (!item) {
        return 0;
    }

    ddr_debug("base_phy[%x] itme[%x]", base_phy, item);
    /* hardware training enable */
    reg_write(item | PHY_PHYINITCTRL_INIT_EN | init_ctrl, base_phy + DDR_PHY_PHYINITCTRL);

    if ((item & PHY_PHYINITCTRL_DRAM_RST) && (item & PHY_PHYINITCTRL_DRAM_INIT_EN)) {
        if (ddr_training_ctrl_easr(cfg, DDR_EXIT_SREF)) {
            return -1;
        }
    }

    count = DDR_HWR_WAIT_TIMEOUT;
    /* auto cleared to 0 after training finished */
    while (count--) {
        if (!(reg_read(base_phy + DDR_PHY_PHYINITCTRL) & PHY_PHYINITCTRL_MASK)) {
            break;
        }
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
        /* rank1 ck/caodt diable, rank1 cs odt enable */
        reg_write((temp1 & 0xffffffc7) | 0x28, base_phy + DDR_PHY_MODEREG23);
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
        for (byte_idx = 0; byte_idx < byte_num; byte_idx++) {
            ddr_get_dly_value(&wdq_rank0_byte.dq_val[byte_idx], base_phy, 0, byte_idx);
        }
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
        for (byte_idx = 0; byte_idx < byte_num; byte_idx++) {
            ddr_restore_dly_value(&wdq_rank0_byte.dq_val[byte_idx], base_phy, 0, byte_idx);
        }
    }

    return result;
}

void ddr_ck_cfg(unsigned int base_phy)
{
    unsigned int acphyctl7;
    unsigned int acphyctl7_tmp;
    unsigned int ck0;
    unsigned int ck1;

        acphyctl7 = reg_read(base_phy + DDR_PHY_ACPHYCTL7);
    ck0 = (acphyctl7 >> PHY_ACPHY_DCLK0_BIT) & PHY_ACPHY_DCLK_MASK;
    ck1 = (acphyctl7 >> PHY_ACPHY_DCLK1_BIT) & PHY_ACPHY_DCLK_MASK;


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART06_SUB01_H_ */
