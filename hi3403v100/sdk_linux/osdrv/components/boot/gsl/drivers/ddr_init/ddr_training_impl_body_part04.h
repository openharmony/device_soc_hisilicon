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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART04_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART04_H_

#ifdef DDR_TRAINING_ADJUST_CONFIG
static unsigned int ddr_adjust_get_average(const struct ddr_cfg_st *cfg)
{
    unsigned int dq0_3;
    unsigned int dq4_7;
    unsigned int val;
    unsigned int base_phy = cfg->cur_phy;
    unsigned int byte_index = cfg->cur_byte;
    unsigned int rank = cfg->rank_idx;

    if (cfg->cur_mode == DDR_MODE_WRITE) {
        return (reg_read(base_phy + ddr_phy_dxnwdqnbdl2(rank, byte_index)) >>
            PHY_WDM_BDL_BIT) & PHY_BDL_MASK;
    }

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
        if (dq_bdl > PHY_DQ_BDL_MIDDLE) {
            *accel = dq_bdl - PHY_DQ_BDL_MIDDLE;
        } else if (dq_bdl < PHY_DQ_BDL_MIDDLE) {
            *accel = PHY_DQ_BDL_MIDDLE - dq_bdl;
        }

        ddr_info("byte[%x] bdl[%x] middle[%x] accel[%x] rdqs[%x]",
            cfg->cur_byte, dq_bdl, PHY_DQ_BDL_MIDDLE, *accel,
            (reg_read(cfg->cur_phy + ddr_phy_dxnrdqsdly(cfg->cur_byte)) >>
            PHY_RDQS_BDL_BIT) & PHY_RDQS_BDL_MASK);
    }

    /* window on left */
    if (dq_bdl < (PHY_DQ_BDL_MIDDLE - size)) {
        return DDR_WIN_LEFT;
    } else if (dq_bdl > (PHY_DQ_BDL_MIDDLE + size)) {
        /* window on right */
        return DDR_WIN_RIGHT;
    } else {
        return DDR_WIN_MIDDLE;
    }
}

/* Check adjust value whether valid */
static int ddr_adjust_check_val(int val, unsigned int mode)
{
    if (mode == DDR_MODE_READ) {
        if (val < 0 || val > PHY_RDQS_BDL_MASK) {
            return DDR_FALSE;
        }
    } else {
        if (val < 0 || val > PHY_WDQ_PHASE_MASK) {
            return DDR_FALSE;
        }
    }

    return DDR_TRUE;
}

static void ddr_rdqs_sync(struct ddr_cfg_st *cfg, int val)
{
    unsigned int rdqsdly;
    unsigned int cur_rank = cfg->rank_idx;
    int old;
    int offset;

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
    unsigned int delay = reg_read(cfg->cur_phy + ddr_phy_dxnrdqsdly(cfg->cur_byte));

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
    if (cfg->cur_mode == DDR_MODE_READ) {
        return (reg_read(cfg->cur_phy + ddr_phy_dxnrdqsdly(cfg->cur_byte)) >>
            PHY_RDQS_BDL_BIT) & PHY_RDQS_BDL_MASK;
    } else {
        return (reg_read(cfg->cur_phy + ddr_phy_dxnwdqdly(cfg->rank_idx, cfg->cur_byte)) >>
            PHY_WDQ_PHASE_BIT) & PHY_WDQ_PHASE_MASK;
    }
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
        if (dir == DDR_WIN_RIGHT) {
            (*val) = (*val) + step;
        } else {
            (*val) = (*val) - step;
        }
    } else {
        /* decrease wdq phase, window move to right */
        if (dir == DDR_WIN_RIGHT) {
            (*val) = (*val) - step;
        } else {
            (*val) = (*val) + step;
        }
    }
}

/*
 * @dir : move direction. DDR_TRUE move to right, DDR_FALSE move to left.
 * Move window to specified direction until the best DQ bdl beyond the midline.
 */
static void ddr_adjust_move_win(struct ddr_cfg_st *cfg,
    struct training_data *training, int step, unsigned int dir)
{
    int cur_val;
    int def_val;
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
        if (cfg->cur_mode == DDR_MODE_WRITE) {
            trend = ddr_adjust_trend_check(cfg, 0);
        } else {
            trend = ddr_adjust_trend_check(cfg, &accel);
        }

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
    if (trend == DDR_WIN_LEFT) {
        ddr_adjust_move_win(cfg, training, DDR_DQS_ADJ_STEP, DDR_WIN_RIGHT);
    } else if (trend == DDR_WIN_RIGHT) {
        /* window on right, move to left */
        ddr_adjust_move_win(cfg, training, DDR_DQS_ADJ_STEP, DDR_WIN_LEFT);
    } else {
        /* window on middle, no need to move */
        ddr_debug("Byte[%x] mode[%x] win on middle", cfg->cur_byte, cfg->cur_mode);
    }
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
    if (ddr_training_check_bypass(cfg, DDR_BYPASS_DATAEYE_ADJ_MASK) != DDR_FALSE) {
        return;
    }

    ddr_debug("DDR dataeye adjust PHY[%x][%x] DMC[%x][%x] Rank[%x]",
        cfg->phy_idx, cfg->cur_phy, cfg->dmc_idx, cfg->cur_dmc, cfg->rank_idx);

    if (cfg->adjust == DDR_FALSE) {
        return;
    }

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

#define DDR_DATAEYE_TRAINING


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART04_H_ */
