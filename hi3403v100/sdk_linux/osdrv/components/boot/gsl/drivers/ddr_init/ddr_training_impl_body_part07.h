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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART07_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART07_H_


#define DDR_VREF_TRAINING
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

    if ((cfg->phy_idx >= DDR_PHY_NUM) || (cfg->dmc_idx >= DDR_DMC_PER_PHY_MAX)) {
        return;
    }
    if (get_byte_num(cfg) > DDR_PHY_BYTE_MAX) {
        return;
    }
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
        if (!(reg_read(base_phy + DDR_PHY_PHYINITCTRL) & PHY_PHYINITCTRL_INIT_EN)) {
            break;
        }
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

    if (cfg->cur_mode == DDR_MODE_READ) {
        ddr_vref_get_host_max(cfg->rank_idx, vref_max);
    }
    if (vref < vref_min) {
        vref_set = vref_min;
    } else if (vref > vref_max)
        vref_set = vref_max;
    else {
        vref_set = vref;
    }

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

    if (cfg->cur_mode == DDR_MODE_READ) {
        ddr_vref_get_host_max(cfg->rank_idx, vref_max);
    }
    max_win = 0;
    cur_vref = vref + step;

    if (vref < vref_min) {
        best_vref = vref_min;
    } else if (vref > vref_max)
        best_vref = vref_max;
    else {
        best_vref = vref;
    }

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
        if (cfg->cur_mode == DDR_MODE_READ) {
            ddr_vref_get_host_max(cfg->rank_idx, vref_max);
        }
        if (def_vref < (vref_max >> 1)) {
            best_vref = ddr_vref_find_best(cfg, training, def_vref, 1);
        } else {
            best_vref = ddr_vref_find_best(cfg, training, def_vref, -1);
        }
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

    if (dram_type != PHY_DRAMCFG_TYPE_LPDDR4 && dram_type != PHY_DRAMCFG_TYPE_DDR4) {
        return -1;
    }
    if (dram_type == PHY_DRAMCFG_TYPE_LPDDR4)
        bank_group = DMC_CFG_MEM_2BG; /* lpddr4 not training byte1 byte3 */

    if (cfg->dmc_idx >= DDR_DMC_PER_PHY_MAX) {
        return -1;
    }
    if (get_byte_num(cfg) > DDR_PHY_BYTE_MAX) {
        return -1;
    }
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

    if (get_byte_num(cfg) > DDR_PHY_BYTE_MAX) {
        return -1;
    }
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


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART07_H_ */
