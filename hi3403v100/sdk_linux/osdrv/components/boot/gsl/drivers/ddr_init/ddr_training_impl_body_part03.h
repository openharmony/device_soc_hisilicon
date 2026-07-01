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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART03_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART03_H_

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
    unsigned int rdm;
    unsigned int rdqs;
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
        if (bdl_dly_s->value[i] < min) {
            min = bdl_dly_s->value[i];
        }
    }

    /* subtract minimum */
    for (i = 0; i < value_num; i++) {
        bdl_dly_s->value[i] = bdl_dly_s->value[i] - min;
    }

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
    int wdqphase_rank0_tmp0;
    int wdqphase_rank1_tmp0;
    int wdqphase_rank0_tmp1;
    int wdqphase_rank1_tmp1;
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
    int wdqsphase_rank0_tmp0;
    int wdqsphase_rank1_tmp0;
    int wdqsphase_rank0_tmp1;
    int wdqsphase_rank1_tmp1;

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
    int wdqsphase_rank0_tmp0;
    int wdqsphase_rank1_tmp0;
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

#define DDR_DATAEYE_ADJUST


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART03_H_ */
