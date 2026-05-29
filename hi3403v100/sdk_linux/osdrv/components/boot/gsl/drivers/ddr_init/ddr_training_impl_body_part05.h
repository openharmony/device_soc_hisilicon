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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART05_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART05_H_

#ifdef DDR_DATAEYE_TRAINING_CONFIG
/* Check dataeye dq */
static int ddr_dataeye_check_dq(const struct ddr_cfg_st *cfg)
{
    if (cfg->dq_check_type == DDR_CHECK_TYPE_DDRT) {
        return ddr_ddrt_check(cfg);
    } else if (cfg->dq_check_type == DDR_CHECK_TYPE_MPR) {
        return ddr_mpr_check(cfg);
    } else {
        ddr_error("DDR dataeye dq check type not set");
    }

    return 0;
}

/* Check dq whether valid and set mask to reduce search time */
static int ddr_dataeye_check_dir(unsigned int direction, unsigned int left,
    unsigned int right, unsigned int *mask, const struct ddr_cfg_st *cfg)
{
    int result;

    result = ddr_dataeye_check_dq(cfg);
    switch (direction) {
        case DDR_FIND_DQ_BOTH:
            *mask = DDR_FIND_DQ_LEFT | DDR_FIND_DQ_RIGHT;
            break;
        case DDR_FIND_DQ_LEFT:
            if (result) {
                /* ddr test error, search opposite side */
                *mask = DDR_FIND_DQ_RIGHT;
            } else { /* ddr test ok */
                ddr_phy_set_dq_bdl(cfg, left);
                if (!ddr_dataeye_check_dq(cfg)) {
                    /* test ok, go on search this side */
                    *mask = DDR_FIND_DQ_LEFT;
                }
            }
            break;
        case DDR_FIND_DQ_RIGHT:
            if (result) {  /* ddr test error, search opposite side */
                *mask = DDR_FIND_DQ_LEFT;
            } else { /* ddr test ok */
                ddr_phy_set_dq_bdl(cfg, right);
                if (!ddr_dataeye_check_dq(cfg)) {
                    /* test OK, go on search this side */
                    *mask = DDR_FIND_DQ_RIGHT;
                }
            }
            break;
        default:
            break;
    }

    return result;
}

/* Binary search the valid dq bdl */
static void ddr_dataeye_search_dq(unsigned int left, unsigned int right,
    int *target, unsigned int direction, const struct ddr_cfg_st *cfg)
{
    unsigned int middle;
    unsigned int mask = 0;

    middle = left + ((right - left) >> 1);

    ddr_phy_set_dq_bdl(cfg, middle);
    if (!ddr_dataeye_check_dir(direction, left, right, &mask, cfg)) { /* test ok */
        *target = (int)middle;
        return;
    }

    /* not found */
    if (left == middle || middle == right) {
        return;
    }

    /* find left side */
    if (DDR_FIND_DQ_LEFT & mask) {
        ddr_dataeye_search_dq(left, middle, target, direction, cfg);
    }

    /* find right side */
    if (DDR_FIND_DQ_RIGHT & mask) {
        ddr_dataeye_search_dq(middle, right, target, direction, cfg);
    }

    return;
}

static int ddr_dataeye_find_left_dq(const struct ddr_cfg_st *cfg, int cur_dq)
{
    int left_dq = cur_dq;

    ddr_dataeye_search_dq(0, cur_dq, &left_dq, DDR_FIND_DQ_LEFT, cfg);
    while (left_dq > 0) {
        left_dq--;
        ddr_phy_set_dq_bdl(cfg, left_dq);
        if (ddr_dataeye_check_dq(cfg)) {
            return left_dq + 1;
        }
    }
    return left_dq;
}

static int ddr_dataeye_find_right_dq(const struct ddr_cfg_st *cfg, int cur_dq)
{
    int right_dq = cur_dq;

    ddr_dataeye_search_dq(cur_dq, PHY_BDL_MASK, &right_dq, DDR_FIND_DQ_RIGHT, cfg);
    while (right_dq < PHY_BDL_MASK) {
        right_dq++;
        ddr_phy_set_dq_bdl(cfg, right_dq);
        if (ddr_dataeye_check_dq(cfg)) {
            return right_dq - 1;
        }
    }
    return right_dq;
}

/* Find DQ valid range */
static void ddr_dataeye_find_dq(const struct ddr_cfg_st *cfg, struct training_data *training)
{
    int cur_dq;
    int left_dq;
    int right_dq;
    int def_dq;
    unsigned int dq_num;
    unsigned int win_num;

    dq_num = (cfg->cur_byte << DDR_BYTE_DQ) + cfg->cur_dq;
    def_dq = (int)ddr_phy_get_dq_bdl(cfg);
    cur_dq = def_dq;

    /* check default dq */
    if (ddr_dataeye_check_dq(cfg)) {
        /* test error */
        cur_dq = -1;
        ddr_dataeye_search_dq(0, PHY_BDL_MASK, &cur_dq, DDR_FIND_DQ_BOTH, cfg);
        ddr_debug("DQ[%x] def[%x] not ok, find new value[%x]", dq_num, def_dq, cur_dq);
        if (cur_dq == -1) {  /* no valid dq */
            training->ddr_bit_result[dq_num] = 0;
            training->ddr_bit_best[dq_num] = 0;
            /* restore default value */
            ddr_phy_set_dq_bdl(cfg, def_dq);
            ddr_warning("DQ[%x] not found dq. restore[%x]", dq_num, def_dq);
            return;
        }
    }
    /* find the left boundary */
    left_dq = ddr_dataeye_find_left_dq(cfg, cur_dq);
    /* find the right boundary */
    right_dq = ddr_dataeye_find_right_dq(cfg, cur_dq);
    /* reset dq */
    ddr_phy_set_dq_bdl(cfg, def_dq);

    /*
        * 0 1 2 3 4 5 6 7 8 9
        * x x - - - - - x x x
        *     |       |
        * left_dq   right_dq
        *
        * so left_dq = 2, right_dq = 6
        */
    win_num = right_dq - left_dq + 1;
    training->ddr_bit_result[dq_num] = ((unsigned int)left_dq << DDR_DATAEYE_RESULT_BIT) |
        (unsigned int)right_dq;
    training->ddr_bit_best[dq_num] = (win_num << DDR_DATAEYE_RESULT_BIT) |
        ((win_num >> 1) + (unsigned int)left_dq);

    ddr_info("DQ[%x] range: left[%x] right[%x] best[%x] mode[%x] rank[%x]", dq_num,
        left_dq, right_dq, training->ddr_bit_best[dq_num], cfg->cur_mode, cfg->rank_idx);
}

static int ddr_dataeye_deskew_one(struct ddr_cfg_st *cfg, struct training_data *training, int *dq_idx,
    unsigned int *loop_times, unsigned int *dq_sum)
{
    unsigned int dq_num = (cfg->cur_byte << DDR_BYTE_DQ) + (unsigned int)(*dq_idx);
    unsigned int def_dq;
    unsigned int win_num;
    unsigned int best_dq;

    if (dq_num >= DDR_PHY_BIT_MAX) {
        ddr_error("Array index dq_num out of range");
        return -1;
    }
    def_dq = ddr_phy_get_dq_bdl(cfg);
    ddr_dataeye_find_dq(cfg, training);
    win_num = training->ddr_bit_best[dq_num] >> DDR_DATAEYE_RESULT_BIT;
    best_dq = training->ddr_bit_best[dq_num] & DDR_DATAEYE_RESULT_MASK;
    if (win_num < DDR_DATAEYE_WIN_NUM) {
        if (*loop_times < DDR_LOOP_TIMES_LMT) {
            (*loop_times)++;
            (*dq_idx)++;
            return 0;
        } else if (win_num == 0) {
            ddr_warning("Byte[%x] DQ[%x] no win", cfg->cur_byte, dq_num);
            ddr_phy_set_dq_bdl(cfg, def_dq);
            ddr_training_stat(DDR_ERR_DATAEYE, cfg->cur_phy, cfg->cur_byte, *dq_idx);
            *dq_idx += DDR_DATAEYE_DQ_STEP;
            return 0;
        }
    }
    *loop_times = 0;
    ddr_phy_set_dq_bdl(cfg, best_dq);
    *dq_sum += best_dq;
    training->ddr_win_sum += win_num;
    *dq_idx += DDR_DATAEYE_DQ_STEP;
    return 0;
}

/* DDR dataeye training one byte. */
int ddr_dataeye_deskew(struct ddr_cfg_st *cfg, struct training_data *training)
{
    unsigned int loop_times = 0;
    unsigned int dq_sum;
    int i;

    if (cfg == NULL || training == NULL) {
        ddr_error("Pointer parameter is NULL");
        return -1;
    }
    dq_sum = 0;
    training->ddr_win_sum = 0;
    i = 0;
    while (i < DDR_PHY_BIT_NUM) {
        cfg->cur_dq = i;
        if (ddr_dataeye_deskew_one(cfg, training, &i, &loop_times, &dq_sum) != 0) {
            return -1;
        }
    }
    dq_sum = dq_sum >> DDR_BYTE_DQ;

    /* only DDR_MODE_WRITE need to set */
    if (cfg->cur_mode == DDR_MODE_WRITE) {
        reg_write((dq_sum & PHY_BDL_MASK) << PHY_WDM_BDL_BIT, cfg->cur_phy +
            ddr_phy_dxnwdqnbdl2(cfg->rank_idx, cfg->cur_byte));
    }

    ddr_phy_cfg_update(cfg->cur_phy);

    return 0;
}

/* DDR write or read dataeye training */
static int ddr_dataeye_process(struct ddr_cfg_st *cfg, struct training_data *training)
{
    int result = 0;
    unsigned int i;

    if (cfg->phy_idx >= DDR_PHY_NUM || cfg->dmc_idx >= DDR_DMC_PER_PHY_MAX) {
        ddr_error("Array index phy_idx or dmc_idx out of range!");
        return -1;
    }
    if (get_byte_num(cfg) > DDR_PHY_BYTE_MAX) {
        ddr_error("get byte num fail, byte_num = %x", get_byte_num(cfg));
        return -1;
    }
    /* dataeye training */
    for (i = 0; i < get_byte_num(cfg); i++) {
        cfg->cur_byte = i + (cfg->dmc_idx << 1); /* byte index accord to phy */
        result += ddr_dataeye_deskew(cfg, training);
    }

    if (result) {
        result = -1;
        ddr_error("PHY[%x] mode[%x] dataeye training fail", cfg->cur_phy, cfg->cur_mode);
    } else {
        /* dataeye training result adjust */
        ddr_adjust_dataeye(cfg, training);
    }
    /* save training result to printf */
    ddr_result_data_save(cfg, training);

    return result;
}

int ddr_dataeye_training(struct ddr_cfg_st *cfg)
{
    struct training_data tmp_result;
    struct training_data *training = &tmp_result;
    int result_read;
    int result_write;

    ddr_debug("DDR dataeye training PHY[%x][%x] DMC[%x][%x] Rank[%x]",
        cfg->phy_idx, cfg->cur_phy, cfg->dmc_idx, cfg->cur_dmc, cfg->rank_idx);

    /* write dataeye training */
    cfg->cur_mode = DDR_MODE_WRITE;
    ddrtr_set_data(training, 0, sizeof(struct training_data));
    result_write = ddr_dataeye_process(cfg, training);

    /* read dataeye training */
    cfg->cur_mode = DDR_MODE_READ;
    ddrtr_set_data(training, 0, sizeof(struct training_data));
    result_read = ddr_dataeye_process(cfg, training);
    if (result_read || result_write) {
        return -1;
    } else {
        return 0;
    }
}

int ddr_dataeye_training_func(struct ddr_cfg_st *cfg)
{
    struct tr_relate_reg relate_reg;
    int result;

    if (cfg == NULL) {
        ddr_error("Pointer parameter cfg is NULL!");
        return -1;
    }
    /* dataeye training disable */
    if (ddr_training_check_bypass(cfg, DDR_BYPASS_DATAEYE_MASK) != DDR_FALSE) {
        return 0;
    }

    ddr_training_save_reg(cfg, &relate_reg, DDR_BYPASS_DATAEYE_MASK);
    ddr_training_switch_axi(cfg);
    ddr_ddrt_init(cfg, DDR_DDRT_MODE_DATAEYE);
    cfg->adjust = DDR_DATAEYE_NORMAL_ADJUST;
    cfg->dq_check_type = DDR_CHECK_TYPE_DDRT;
    result = ddr_dataeye_training(cfg);
    ddr_training_restore_reg(cfg, &relate_reg);

    return result;
}
#else
int ddr_dataeye_training_func(struct ddr_cfg_st *cfg)
{
    ddr_warning("Not support DDR dataeye training");

    return 0;
}
#endif  /* DDR_DATAEYE_TRAINING_CONFIG */

#define DDR_HARDWARE_TRAINING


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART05_H_ */
