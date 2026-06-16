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

/* s40/t28/t16 not support dcc training */
#define DDR_DCC_TRAINING
#ifdef DDR_DCC_TRAINING_CONFIG
/* Save two rank RDET result */
static void ddr_save_two_rank_bdl(const struct ddr_cfg_st *cfg, struct dcc_data_st *dcc_data)
{
    unsigned int byte_idx;
    unsigned int base_phy = cfg->cur_phy;
    unsigned int rank_idx = cfg->rank_idx;
    unsigned int byte_num = cfg->phy[cfg->phy_idx].total_byte_num;

    for (byte_idx = 0; byte_idx < byte_num; byte_idx++) {
        dcc_data->rank[rank_idx].dq03[byte_idx] = reg_read(base_phy + ddr_phy_dxnrdqnbdl0(rank_idx, byte_idx));
        dcc_data->rank[rank_idx].dq47[byte_idx] = reg_read(base_phy + ddr_phy_dxnrdqnbdl1(rank_idx, byte_idx));
        dcc_data->rank[rank_idx].rdm[byte_idx] = reg_read(base_phy + ddr_phy_dxnrdqnbdl2(rank_idx, byte_idx));
        dcc_data->rank[rank_idx].rdqs[byte_idx] = reg_read(base_phy + ddr_phy_dxnrdqsdly(byte_idx));

        ddr_debug("rank[%x] dq03[%x] dq47[%x] rdm[%x] rdqs[%x]", rank_idx,
      dcc_data->rank[rank_idx].dq03[byte_idx], dcc_data->rank[rank_idx].dq47[byte_idx],
      dcc_data->rank[rank_idx].rdm[byte_idx], dcc_data->rank[rank_idx].rdqs[byte_idx]);
    }
}

/* Restore two rank RDET result */
static void ddr_restore_two_rank_bdl(const struct ddr_cfg_st *cfg, const struct dcc_data_st *dcc_data)
{
    unsigned int byte_idx;
    unsigned int base_phy = cfg->cur_phy;
    unsigned int rank_idx = cfg->rank_idx;
    unsigned int byte_num = cfg->phy[cfg->phy_idx].total_byte_num;

    for (byte_idx = 0; byte_idx < byte_num; byte_idx++) {
        reg_write(dcc_data->rank[rank_idx].dq03[byte_idx], base_phy + ddr_phy_dxnrdqnbdl0(rank_idx, byte_idx));
        reg_write(dcc_data->rank[rank_idx].dq47[byte_idx], base_phy + ddr_phy_dxnrdqnbdl1(rank_idx, byte_idx));
        reg_write(dcc_data->rank[rank_idx].rdm[byte_idx], base_phy + ddr_phy_dxnrdqnbdl2(rank_idx, byte_idx));
        reg_write(dcc_data->rank[rank_idx].rdqs[byte_idx], base_phy + ddr_phy_dxnrdqsdly(byte_idx));
    }
}

/* DCC RDET training */
static int ddr_dcc_dataeye_read(const struct ddr_cfg_st *cfg)
{
    int result;

    /* 0:PHY_TRAINCTRL0_DTR_RANK0, 1:PHY_TRAINCTRL0_DTR_RANK1 */
    ddr_phy_switch_rank(cfg->cur_phy, cfg->rank_idx);
    result = ddr_hw_training_process(cfg, PHY_PHYINITCTRL_RDET_EN);
    reg_write(PHY_PHYINITCTRL_RDET_EN, cfg->cur_phy + DDR_PHY_PHYINITSTATUS);

    return result;
}

/* Duty direction control */
static unsigned int ddr_dcc_ck_ctl(const struct ddr_cfg_st *cfg, unsigned int ioctl21_def,
    unsigned int ctl_index)
{
    unsigned int ioctl21;

    if (cfg->phy[cfg->phy_idx].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
        ioctl21 = (ioctl21_def & (~(1 << PHY_ACIOCTL21_CTL0_BIT)) & (~(1 << PHY_ACIOCTL21_CTL1_BIT))) |
            (ctl_index << PHY_ACIOCTL21_CTL0_BIT) | (ctl_index << PHY_ACIOCTL21_CTL1_BIT);
        reg_write(ioctl21, cfg->cur_phy + DDR_PHY_ACIOCTL21);
    } else {
        ioctl21 = (ioctl21_def & (~(1 << PHY_ACIOCTL21_CTL0_BIT))) |
            (ctl_index << PHY_ACIOCTL21_CTL0_BIT);
        reg_write(ioctl21, cfg->cur_phy + DDR_PHY_ACIOCTL21);
    }
    return ioctl21;
}

/* Duty Correction. */
static unsigned int ddr_dcc_correct_duty(const struct ddr_cfg_st *cfg, unsigned int cur_duty,
    unsigned int duty_def)
{
    unsigned int ioctl21;

    if (cfg->phy[cfg->phy_idx].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
        /* Correct CK0 & CK1 duty */
        ioctl21 = (duty_def & (~(PHY_ACIOCTL21_MASK << PHY_ACIOCTL21_CK0_BIT)) &
            (~(PHY_ACIOCTL21_MASK << PHY_ACIOCTL21_CK1_BIT))) |
            (cur_duty << PHY_ACIOCTL21_CK0_BIT) |
            (cur_duty << PHY_ACIOCTL21_CK1_BIT);
        reg_write(ioctl21, cfg->cur_phy + DDR_PHY_ACIOCTL21);
    } else {
        /* Correct CK0 duty */
        ioctl21 = (duty_def & (~(PHY_ACIOCTL21_MASK << PHY_ACIOCTL21_CK0_BIT))) |
            (cur_duty << PHY_ACIOCTL21_CK0_BIT);
        reg_write(ioctl21, cfg->cur_phy + DDR_PHY_ACIOCTL21);
    }

    return ioctl21;
}

/* Duty Correction Control get win data */
static unsigned int ddr_dcc_get_win(const struct dcc_data_st *dcc_data, int ck_index, int val_index)
{
    unsigned int win;
    unsigned int rdqsbdl_right;
    unsigned int rdqsbdl_left;

    rdqsbdl_right = (dcc_data->ck[ck_index].val[val_index] >> PHY_DXNRDBOUND_RIGHT_BIT) &
        PHY_DXNRDBOUND_MASK;
    rdqsbdl_left = (dcc_data->ck[ck_index].val[val_index] >> PHY_DXNRDBOUND_LEFT_BIT) &
        PHY_DXNRDBOUND_MASK;
    win = rdqsbdl_right - rdqsbdl_left;

    return win;
}

/* Duty Correction Control get the min win of two byte */
static unsigned int ddr_dcc_get_min_win(const struct dcc_data_st *dcc_data, int ck_index)
{
    int i;
    unsigned int win_min;
    unsigned int cur_win;

    win_min = ddr_dcc_get_win(dcc_data, ck_index, 0);
    for (i = 0; i < DDR_CK_RESULT_MAX; i++) {
        cur_win = ddr_dcc_get_win(dcc_data, ck_index, i);
        ddr_debug("CK win[%x] = [%x]", i, cur_win);
        if (cur_win < win_min) {
            win_min = cur_win;
        }
    }

    return win_min;
}

/* Duty Correction Control get ck0 min win */
static unsigned int ddr_dcc_get_ck0_win(const struct ddr_cfg_st *cfg, struct dcc_data_st *dcc_data,
    unsigned int ck0_win_min)
{
    unsigned int byte_index;
    unsigned int ck0_win;
    unsigned int byte_num = cfg->phy[cfg->phy_idx].total_byte_num;

    if (byte_num > DDR_PHY_BYTE_MAX) {
        ddr_error("byte num error, byte_num = %x", byte_num);
        return 0;
    }
    if ((byte_num / 2) > (DDR_PHY_BYTE_MAX / 2)) { /* byte_num/2:ck0 value include byte0 and byte1 */
        ddr_error("loop upper limit out of range");
        return 0;
    }
    for (byte_index = 0; byte_index < (byte_num / 2); byte_index++) /* byte_num/2:ck0 value include byte0 and byte1 */
        dcc_data->ck[0].val[byte_index] = reg_read(cfg->cur_phy + ddr_phy_dxnrdbound(byte_index));

    ck0_win = ddr_dcc_get_min_win(dcc_data, 0);
    if (ck0_win < ck0_win_min) {
        ck0_win_min = ck0_win;
    }

    return ck0_win_min;
}
/* Duty Correction Control get ck1 min win. */
static unsigned int ddr_dcc_get_ck1_win(const struct ddr_cfg_st *cfg, struct dcc_data_st *dcc_data,
    unsigned int ck1_win_min)
{
    unsigned int byte_index;
    unsigned int ck1_win;
    unsigned int byte_num = cfg->phy[cfg->phy_idx].total_byte_num;

    for (byte_index = 2; byte_index < byte_num; byte_index++) /* ck1 value include byte2 and byte3 */
        dcc_data->ck[1].val[byte_index - 2] = /* store value from byte_idex-2 */
            reg_read(cfg->cur_phy + ddr_phy_dxnrdbound(byte_index)); /* one ck include two value */

    ck1_win = ddr_dcc_get_min_win(dcc_data, 1);
    if (ck1_win < ck1_win_min) {
        ck1_win_min = ck1_win;
    }

    return ck1_win_min;
}

/* dcc training data init */
static void dcc_data_init(struct dcc_data_st *dcc_data)
{
    dcc_data->ck[0].win_min_ctl = 0xffffffff;
    dcc_data->ck[0].win_max_ctl = 0x0;
    dcc_data->ck[1].win_min_ctl = 0xffffffff;
    dcc_data->ck[1].win_max_ctl = 0x0;
    dcc_data->ck[0].idx_duty = 0;
    dcc_data->ck[0].idx_duty_ctl = 0;
    dcc_data->ck[0].idx_ctl = 0;
    dcc_data->ck[1].idx_duty = 0;
    dcc_data->ck[1].idx_duty_ctl = 0;
    dcc_data->ck[1].idx_ctl = 0;
    dcc_data->ck[0].bypass_ck_bit = PHY_BYPASS_CK0_BIT;
    dcc_data->ck[0].acioctl21_ctl_bit = PHY_ACIOCTL21_CTL0_BIT;
    dcc_data->ck[0].acioctl21_ck_bit = PHY_ACIOCTL21_CK0_BIT;
    dcc_data->ck[1].bypass_ck_bit = PHY_BYPASS_CK1_BIT;
    dcc_data->ck[1].acioctl21_ctl_bit = PHY_ACIOCTL21_CTL1_BIT;
    dcc_data->ck[1].acioctl21_ck_bit = PHY_ACIOCTL21_CK1_BIT;
}

/* dcc training get window by rank */
static int ddr_dcc_get_win_by_rank(struct ddr_cfg_st *cfg, struct dcc_data_st *dcc_data)
{
    unsigned int i;
    unsigned int rank_num = cfg->phy[cfg->phy_idx].rank_num;

    for (i = 0; i < rank_num; i++) {
        ddr_debug("cur_rank = [%x]", i);
        cfg->rank_idx = i;

        if (ddr_dcc_dataeye_read(cfg)) {
            dcc_data->ck[0].win = 0x0;
            dcc_data->ck[1].win = 0x0;
        } else {
            /* Get win */
            dcc_data->ck[0].win = ddr_dcc_get_ck0_win(cfg, dcc_data, dcc_data->ck[0].win);
            ddr_debug("ck0 win = [%x]", dcc_data->ck[0].win);

            if (cfg->phy[cfg->phy_idx].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
                dcc_data->ck[1].win = ddr_dcc_get_ck1_win(cfg, dcc_data, dcc_data->ck[1].win);
                ddr_debug("ck1 win = [%x]", dcc_data->ck[1].win);
            }
        }
        /* Restore two rank bdl */
        ddr_restore_two_rank_bdl(cfg, dcc_data);
    }

    return 0;
}

/* ddr dcc training compare result */
static void ddr_dcc_compare_result(struct dcc_data_st *dcc_data, int ck_num,
    unsigned int base_phy, unsigned int gated_bypass_def, unsigned int ioctl21_def)
{
    int ck_idx;

    if (ck_num > DDR_CK_MAX_NUM) {
        ddr_error("ck number out of range");
        return;
    }
    for (ck_idx = 0; ck_idx < ck_num; ck_idx++) {
        /* Config ck0 duty */
        if (dcc_data->ck[ck_idx].win_max_ctl - dcc_data->ck[ck_idx].win_min_ctl <= DDR_DCC_CTL_WIN_DIFF) {
            dcc_data->ck[ck_idx].def_bp = (gated_bypass_def >> dcc_data->ck[ck_idx].bypass_ck_bit) & 0x1;
            dcc_data->ck[ck_idx].def_ctl = (ioctl21_def >> dcc_data->ck[ck_idx].acioctl21_ctl_bit) & 0x1;
            dcc_data->ck[ck_idx].def_duty = (ioctl21_def >> dcc_data->ck[ck_idx].acioctl21_ck_bit) & PHY_ACIOCTL21_MASK;

            gated_bypass_def = (gated_bypass_def & (~(1 << dcc_data->ck[ck_idx].bypass_ck_bit))) |
                (dcc_data->ck[ck_idx].def_bp << dcc_data->ck[ck_idx].bypass_ck_bit);
            reg_write(gated_bypass_def, base_phy + DDR_PHY_AC_GATED_BYPASS);

            ioctl21_def = (ioctl21_def & (~(1 << dcc_data->ck[ck_idx].acioctl21_ctl_bit)) &
                (~(PHY_ACIOCTL21_MASK << dcc_data->ck[ck_idx].acioctl21_ck_bit))) |
                (dcc_data->ck[ck_idx].def_ctl << dcc_data->ck[ck_idx].acioctl21_ctl_bit) |
                (dcc_data->ck[ck_idx].def_duty << dcc_data->ck[ck_idx].acioctl21_ck_bit);
            reg_write(ioctl21_def, base_phy + DDR_PHY_ACIOCTL21);

            ddr_debug("ck[%x] Final AC_GATED_BYPASS[%x]", ck_idx, gated_bypass_def);
            ddr_debug("ck[%x] Final ACIOCTL21[%x]", ck_idx, ioctl21_def);
        } else {
            ioctl21_def = (ioctl21_def & (~(1 << dcc_data->ck[ck_idx].acioctl21_ctl_bit)) &
                (~(PHY_ACIOCTL21_MASK << dcc_data->ck[ck_idx].acioctl21_ck_bit))) |
                (dcc_data->ck[ck_idx].idx_ctl << dcc_data->ck[ck_idx].acioctl21_ctl_bit) |
                (dcc_data->ck[ck_idx].idx_duty_ctl << dcc_data->ck[ck_idx].acioctl21_ck_bit);
            reg_write(ioctl21_def, base_phy + DDR_PHY_ACIOCTL21);

            ddr_debug("ck[%x] Final ACIOCTL21[%x]", ck_idx, ioctl21_def);
        }
    }
}

static void ddr_dcc_get_duty(struct dcc_data_st *dcc_data, int ck_num, unsigned int cur_duty)
{
    int ck_idx;

    if (ck_num > DDR_CK_MAX_NUM) {
        ddr_error("ck number out of range");
        return;
    }
    for (ck_idx = 0; ck_idx < ck_num; ck_idx++) {
        if (dcc_data->ck[ck_idx].win < dcc_data->ck[ck_idx].win_min_duty) {
            dcc_data->ck[ck_idx].win_min_duty = dcc_data->ck[ck_idx].win;
        }

        if (dcc_data->ck[ck_idx].win > dcc_data->ck[ck_idx].win_max_duty) {
            dcc_data->ck[ck_idx].win_max_duty = dcc_data->ck[ck_idx].win;
            dcc_data->ck[ck_idx].idx_duty = cur_duty;
        }
        ddr_debug("ck[%x] duty_win_min[%x] duty_win_max[%x] duty_index[%x]", ck_idx,
      dcc_data->ck[ck_idx].win_min_duty,
      dcc_data->ck[ck_idx].win_max_duty,
      dcc_data->ck[ck_idx].idx_duty);
    }
}

/* Get ck0/ck1 duty_win_min/duty_win_max/duty_index */
static void ddr_dcc_get_ctrl(struct dcc_data_st *dcc_data, int ck_num, unsigned int cur_ctl)
{
    int ck_idx;

    if (ck_num > DDR_CK_MAX_NUM) {
        ddr_error("ck number out of range");
        return;
    }
    for (ck_idx = 0; ck_idx < ck_num; ck_idx++) {
        if (dcc_data->ck[ck_idx].win_min_duty < dcc_data->ck[ck_idx].win_min_ctl) {
            dcc_data->ck[ck_idx].win_min_ctl = dcc_data->ck[ck_idx].win_min_duty;
        }

        if (dcc_data->ck[ck_idx].win_max_duty > dcc_data->ck[ck_idx].win_max_ctl) {
            dcc_data->ck[ck_idx].win_max_ctl = dcc_data->ck[ck_idx].win_max_duty;
            dcc_data->ck[ck_idx].idx_duty_ctl = dcc_data->ck[ck_idx].idx_duty;
            dcc_data->ck[ck_idx].idx_ctl = cur_ctl;
        }
        ddr_debug("ck[%x] win_min_ctl[%x] win_max_ctl[%x] ctl_index0[%x] duty_ctl_idx0[%x]", ck_idx,
      dcc_data->ck[ck_idx].win_min_ctl,
      dcc_data->ck[ck_idx].win_max_ctl,
      dcc_data->ck[ck_idx].idx_ctl,
      dcc_data->ck[ck_idx].idx_duty_ctl);
    }
}

static int ddr_dcc_process(struct ddr_cfg_st *cfg, struct dcc_data_st *dcc_data,
    int ck_num, unsigned int ioctl21_def)
{
    unsigned int cur_ctl;
    unsigned int cur_duty;
    int result;

    for (cur_ctl = 0; cur_ctl < DDR_DUTY_CTL_NUM; cur_ctl++) {
        dcc_data->ck[0].win_min_duty = 0xffffffff;
        dcc_data->ck[0].win_max_duty = 0x0;
        if (ck_num > 1) {
            dcc_data->ck[1].win_min_duty = 0xffffffff;
            dcc_data->ck[1].win_max_duty = 0x0;
        }
        ddr_debug("cur_ctl = [%x]", cur_ctl);

        if (ddr_training_ctrl_easr(cfg, DDR_ENTER_SREF)) {
            return -1;
        }

        /* Correct CK duty dirrection control */
        dcc_data->ioctl21_tmp = ddr_dcc_ck_ctl(cfg, ioctl21_def, cur_ctl);

        if (ddr_training_ctrl_easr(cfg, DDR_EXIT_SREF)) {
            return -1;
        }

        for (cur_duty = 0; cur_duty < DDR_DUTY_NUM; cur_duty += PHY_AC_IOCTL21_STEP) {
            dcc_data->ck[0].win = 0xffffffff;
            if (ck_num > 1) {
                dcc_data->ck[1].win = 0xffffffff;
            }

            ddr_debug("cur_duty = [%x]", cur_duty);
            /* Correct ck0 and ck1 duty */
            if (ddr_training_ctrl_easr(cfg, DDR_ENTER_SREF)) {
                return -1;
            }

            dcc_data->ioctl21_tmp = ddr_dcc_correct_duty(cfg, cur_duty, dcc_data->ioctl21_tmp);
            if (ddr_training_ctrl_easr(cfg, DDR_EXIT_SREF)) {
                return -1;
            }

            ddr_debug("Cur ACIOCTL21[%x]", dcc_data->ioctl21_tmp);
            result = ddr_dcc_get_win_by_rank(cfg, dcc_data);
            ddr_dcc_get_duty(dcc_data, ck_num, cur_duty);
        }

        ddr_dcc_get_ctrl(dcc_data, ck_num, cur_ctl);
    }

    return result;
}

static int ddr_dcc_get_best_duty(struct ddr_cfg_st *cfg,
    struct dmc_cfg_sref_st *cfg_sref, struct dcc_data_st *dcc_data)
{
    int ck_num;
    unsigned int base_phy = cfg->cur_phy;
    unsigned int ioctl21_def;
    unsigned int gated_bypass_def;
    unsigned int gated_bypass_temp;

    if (cfg->phy[cfg->phy_idx].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
        ck_num = DDR_CK_NUM_LPDDR4; /* lpddr4: 2 ck */
    } else {
        ck_num = DDR_CK_NUM_NONLPDDR4; /* other: 1 ck */
    }

    dcc_data_init(dcc_data);

    /* Save ck duty default config. Read two times to get the right static register value. */
    gated_bypass_def = reg_read(base_phy + DDR_PHY_AC_GATED_BYPASS);
    gated_bypass_def = reg_read(base_phy + DDR_PHY_AC_GATED_BYPASS);
    ioctl21_def = reg_read(base_phy + DDR_PHY_ACIOCTL21);
    ioctl21_def = reg_read(base_phy + DDR_PHY_ACIOCTL21);

    ddr_debug("gated_bypass_def[%x] ioctl21_def[%x]", gated_bypass_def, ioctl21_def);

    /* DCC training exit self-refresa enter powerdown. */
    if (cfg->phy[cfg->phy_idx].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
        ddr_sref_cfg(cfg, cfg_sref, DMC_CFG_INIT_XSREF | DMC_CFG_SREF_PD);
    }

    /* DDR dcc training enter auto self-refresh. */
    if (ddr_training_ctrl_easr(cfg, DDR_ENTER_SREF)) {
        return -1;
    }

    /* Enable ck0 & ck1 duty. */
    if (cfg->phy[cfg->phy_idx].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
        gated_bypass_temp = gated_bypass_def | PHY_CK1_IOCTL_DUTY_EN | PHY_CK_IOCTL_DUTY_EN;
        reg_write(gated_bypass_temp, base_phy + DDR_PHY_AC_GATED_BYPASS);
    } else {
        gated_bypass_temp = gated_bypass_def | PHY_CK_IOCTL_DUTY_EN;
        reg_write(gated_bypass_temp, base_phy + DDR_PHY_AC_GATED_BYPASS);
    }
    ddr_debug("Cur GATED_BYPASS[%x]", gated_bypass_temp);

    if (ddr_training_ctrl_easr(cfg, DDR_EXIT_SREF)) {
        return -1;
    }

    if (ddr_dcc_process(cfg, dcc_data, ck_num, ioctl21_def)) {
        return -1;
    }

    /* Config ck duty */
    /* DCC training exit self-refresa enter powerdown. */
    if (cfg->phy[cfg->phy_idx].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
        ddr_sref_cfg(cfg, cfg_sref, DMC_CFG_INIT_XSREF | DMC_CFG_SREF_PD);
    }

    /* DDR dcc training enter auto self-refresh. */
    if (ddr_training_ctrl_easr(cfg, DDR_ENTER_SREF)) {
        return -1;
    }

    /* DDR dcc training compare result. */
    ddr_dcc_compare_result(dcc_data, ck_num, base_phy, gated_bypass_def, ioctl21_def);

    /* DDR dcc training exit auto self-refresh. */
    if (ddr_training_ctrl_easr(cfg, DDR_EXIT_SREF)) {
        return -1;
    }

    return 0;
}

static int ddr_dcc_training(struct ddr_cfg_st *cfg)
{
    unsigned int i;
    int result = 0;
    unsigned int rank_num = cfg->phy[cfg->phy_idx].rank_num;

    struct dmc_cfg_sref_st cfg_sref;
    struct ddr_timing_st timing_st;
    struct dcc_data_st dcc_st;
    struct dcc_data_st *dcc_data = &dcc_st;

    ddrtr_set_data(&cfg_sref, 0, sizeof(struct ddr_cfg_st));
    ddrtr_set_data(&timing_st, 0, sizeof(struct ddr_cfg_st));
    ddrtr_set_data(dcc_data, 0, sizeof(struct ddr_cfg_st));

    ddr_debug("dram_type[%x]", cfg->phy[cfg->phy_idx].dram_type);
    ddr_debug("rank num[%x]", rank_num);

    /* Save two rank DERT default result: rdq/rdqs/rdm/ bdl */
    for (i = 0; i < rank_num; i++) {
        cfg->rank_idx = i;
        ddr_save_two_rank_bdl(cfg, dcc_data);
    }

    /* Disable auto refresh */
    ddr_training_save_timing(cfg, &timing_st);

    /* Duty Correction Control training. */
    result += ddr_dcc_get_best_duty(cfg, &cfg_sref, dcc_data);

    /* Do DERT training again */
    for (i = 0; i < rank_num; i++) {
        cfg->rank_idx = i;
        dcc_data->item[i] = cfg->phy[cfg->phy_idx].rank[i].item_hw;
        cfg->phy[cfg->phy_idx].rank[i].item_hw = PHY_PHYINITCTRL_HVREFT_EN;
        ddr_debug("item_hw[%x]=[%x]", i, cfg->phy[cfg->phy_idx].rank[i].item_hw);
    }
    result += ddr_hw_training_by_phy(cfg);
    for (i = 0; i < rank_num; i++) {
        cfg->rank_idx = i;
        cfg->phy[cfg->phy_idx].rank[i].item_hw = dcc_data->item[i];
    }

    /* Enable auto refresh */
    ddr_training_restore_timing(cfg, &timing_st);

    if (cfg->phy[cfg->phy_idx].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
        /* DCC restore DMC_CFG_SREF config. */
        ddr_sref_cfg_restore(cfg, &cfg_sref);
    }

    return result;
}

int ddr_dcc_training_func(struct ddr_cfg_st *cfg)
{
    unsigned int i;
    int result = 0;

    if (cfg == NULL) {
        ddr_error("Pointer parameter cfg is NULL");
        return -1;
    }

    for (i = 0; i < cfg->phy_num; i++) {
        cfg->phy_idx = i;
        cfg->cur_phy = cfg->phy[i].addr;
        cfg->cur_item = cfg->phy[i].rank[0].item;

        if (ddr_training_check_bypass(cfg, 1 << (cfg->phy_idx)) != DDR_FALSE) {
            continue;
        }
        /* dpmc training disable */
        if (ddr_training_check_bypass(cfg, DDR_BYPASS_DCC_MASK) == DDR_FALSE) {
            result += ddr_dcc_training(cfg);
        }
    }
    return result;
}

#else
int ddr_dcc_training_func(struct ddr_cfg_st *cfg)
{
    ddr_warning("Not support DCC training");
    return 0;
}
#endif /* DDR_DCC_TRAINING_CONFIG */
