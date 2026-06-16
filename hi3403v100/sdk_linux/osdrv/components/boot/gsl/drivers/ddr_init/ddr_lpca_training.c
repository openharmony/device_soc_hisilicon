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

#define DDR_LPCA_TRAINING
#ifdef DDR_LPCA_TRAINING_CONFIG
/* Reset address bdl training data */
static void ddr_lpca_reset(struct ca_data_st *data)
{
    unsigned int index;
    for (index = 0; index < DDR_PHY_CA_MAX; index++) {
        data->left[index] = -1;
        data->right[index] = -1;
    }

    data->min = PHY_ACADDR_BDL_MASK;
    data->max = 0;
    data->done = 0;
}

/* Get ca bit relation */
static void ddr_lpca_get_bit(struct ca_data_st *data)
{
    unsigned int index;

    /* get ca bit in four register */
#ifdef DDR_LPCA_GET_BIT
    unsigned int swap_sel;
    for (index = 0; index < (DDR_PHY_CA_REG_MAX - 1); index++) {
        reg_write(index + 1, data->base_phy + DDR_PHY_CATSWAPINDEX);
        swap_sel = reg_read(data->base_phy + DDR_PHY_CATSWAPSEL);

        data->bits[index * 2].bit_p = /* ca 0/2/4/6 Rising edge */
            swap_sel & PHY_CATSWAPSEL_BIT_MASK;
        data->bits[index * 2].bit_n = /* ca 0/2/4/6 Falling edge */
            (swap_sel >> 8) & PHY_CATSWAPSEL_BIT_MASK; /* bit8 */
        data->bits[index * 2 + 1].bit_p = /* ca * 2 + 1: bit 1/3/5/7 Rising edge */
            (swap_sel >> 16) & PHY_CATSWAPSEL_BIT_MASK; /* bit16 */
        data->bits[index * 2 + 1].bit_n = /* ca * 2 + 1: bit 1/3/5/7 Falling edge */
            (swap_sel >> 24) & PHY_CATSWAPSEL_BIT_MASK; /* bit24 */
    }
#else /* for MVPV200 */
    for (index = 0; index < (DDR_PHY_CA_REG_MAX - 1); index++) {
        data->bits[index * 2].bit_p = index * 4 + 0; /* ca 0/2/4/6 Rising edge, dq*4+0:0/4/8/12 */
        data->bits[index * 2].bit_n = index * 4 + 1; /* ca 0/2/4/6 Falling edge, dq*4+1:1/5/9/13 */
        data->bits[index * 2 + 1].bit_p = index * 4 + 2; /* ca*2+1: bit 1/3/5/7 Rising edge, dq*4+2:2/6/10/14 */
        data->bits[index * 2 + 1].bit_n = index * 4 + 3; /* ca*2+1: bit 1/3/5/7 Falling edge, dq*4+3:2/6/10/14 */
    }
#endif

    /*
     * set ca bit for ca4 and ca9
     * ca4 equal ca0 ca9 equal ca5
     */
    for (index = 8; index > 4; index--) { /* set ca bit for ca5 to ca8 equal ca4 to ca7 */
        data->bits[index].bit_p = data->bits[index - 1].bit_p;
        data->bits[index].bit_n = data->bits[index - 1].bit_n;
    }

    data->bits[4].bit_p = data->bits[0].bit_p; /* ca4 equal ca0 */
    data->bits[4].bit_n = data->bits[0].bit_n; /* ca4 equal ca0 */
    data->bits[9].bit_p = data->bits[5].bit_p; /* ca9 equal ca5 */
    data->bits[9].bit_n = data->bits[5].bit_n; /* ca9 equal ca5 */

#if defined(DDR_TRAINING_CMD)
    for (index = 0; index < DDR_PHY_CA_MAX; index++) {
        ddr_info("CA[%x] bit_p[%x]", index, data->bits[index].bit_p);
        ddr_info("CA[%x] bit_n[%x]", index, data->bits[index].bit_n);
    }
#endif
}

/* Get address bdl default value */
static void ddr_lpca_get_def(struct ca_data_st *data)
{
    unsigned int index;

    for (index = 0; index < DDR_PHY_CA_REG_MAX; index++)
        data->def[index] = reg_read(data->base_phy + ddr_phy_acaddrbdl(index));
}

/* Restore address bdl default value */
static void ddr_lpca_restore_def(struct ca_data_st *data)
{
    unsigned int index;

    for (index = 0; index < DDR_PHY_CA_REG_MAX; index++)
        reg_write(data->def[index], data->base_phy + ddr_phy_acaddrbdl(index));

    ddr_phy_cfg_update(data->base_phy);
}

/* Set address bdl value */
static void ddr_lpca_set_bdl(unsigned int base_phy, unsigned int bdl)
{
    unsigned int index;
    for (index = 0; index < DDR_PHY_CA_REG_MAX; index++)
        reg_write(bdl | (bdl << PHY_ACADDRBDL_ADDR1_BIT),
            base_phy + ddr_phy_acaddrbdl(index));

    ddr_phy_cfg_update(base_phy);
}

/* Update address bdl value with training result */
static void ddr_lpca_update_bdl(struct ca_data_st *data)
{
    unsigned int index;
    unsigned int addr0;
    unsigned int addr1;

    for (index = 0; index < DDR_PHY_CA_REG_MAX; index++) {
        addr0 = (data->left[index * 2] + data->right[index * 2]) >> 1; /* 2:ca middle value */
        addr1 = (data->left[index * 2 + 1] + data->right[index * 2 + 1]) >> 1; /* 2:ca middle value */
        reg_write(addr0 | (addr1 << PHY_ACADDRBDL_ADDR1_BIT),
            data->base_phy + ddr_phy_acaddrbdl(index));
    }

    ddr_phy_cfg_update(data->base_phy);
}

/* Init data before training */
static void ddr_lpca_init(unsigned int base_dmc, unsigned int base_phy, struct ca_data_st *data)
{
    data->base_dmc = base_dmc;
    data->base_phy = base_phy;

    /* gat ca bit relation */
    ddr_lpca_get_bit(data);

    /* get ac addr bdl default value */
    ddr_lpca_get_def(data);

    /* reset training data */
    ddr_lpca_reset(data);
}

/* Display training result */
static void ddr_lpca_display(struct ca_data_st *data)
{
#if defined(DDR_TRAINING_CMD)
    unsigned int index;

    ddr_debug("CA phase[%x = %x]",
        data->base_phy + DDR_PHY_ADDRPHBOUND,
        reg_read(data->base_phy + DDR_PHY_ADDRPHBOUND));

    for (index = 0; index < DDR_PHY_CA_MAX; index++)
        ddr_debug("CA[%x] left[%x] right[%x]",
            index, data->left[index], data->right[index]);

    ddr_debug("min[%x] max[%x] done[%x]", data->min, data->max, data->done);
#endif
}

/* Wait lpca command done */
static void ddr_lpca_wait(volatile union u_phy_catconfig *ca)
{
    unsigned int count = 0;
    while (count < DDR_LPCA_WAIT_TIMEOUT) {
        if (ca->bits.sw_cat_dqvalid == 1) {
            ca->bits.sw_cat_dqvalid = 0; /* clear */
            break;
        }
        count++;
    }

    /* generally, count is 0 */
    if (count >= DDR_LPCA_WAIT_TIMEOUT)
        ddr_error("LPCA wait timeout");
}

/* Compare dq result and pattern */
static int ddr_lpca_compare(struct ca_bit_st *ca_bit,
    unsigned int dq_result, unsigned int pattern_p,
    unsigned int pattern_n, unsigned int index)
{
    if (((dq_result >> ca_bit->bit_p) & 0x1) != ((pattern_p >> index) & 0x1))
        return -1;

    if (((dq_result >> ca_bit->bit_n) & 0x1) != ((pattern_n >> index) & 0x1))
        return -1;

    return 0;
}

static void ddr_lpca_get_data(struct ca_data_st *data, unsigned int index, unsigned int bdl)
{
    /* pass */
    if (data->left[index] == -1) {
        data->left[index] = bdl;
        /* set min left bound */
        if (bdl < data->min)
            data->min = bdl;
    }

    /* unstable border value or abnormal value */
    if ((data->right[index] != -1) && ((bdl - data->right[index]) > 1))
        ddr_warning("CA[%x] bdl[%x] right[%x] ph[%x]",
            index, bdl, data->right[index],
            reg_read(data->base_phy + DDR_PHY_ADDRPHBOUND));

    data->right[index] = bdl;
    data->done |= (0x1 << index);

    /* set max right bound */
    if (data->right[index] > data->max)
        data->max = data->right[index];
}

/* Check each CA whether pass */
static void ddr_lpca_check(struct ca_data_st *data, unsigned int bdl, unsigned int is_ca49)
{
    unsigned int dq_result = reg_read(data->base_phy + DDR_PHY_PHYDQRESULT);
    unsigned int pattern_p = reg_read(data->base_phy +
        DDR_PHY_SWCATPATTERN_P) & PHY_CAT_PATTERN_MASK;
    unsigned int pattern_n = reg_read(data->base_phy +
        DDR_PHY_SWCATPATTERN_N) & PHY_CAT_PATTERN_MASK;
    unsigned int index;

    for (index = 0; index < DDR_PHY_CA_MAX; index++) {
        if (is_ca49) {
            if (index != 4 && index != 9) /* ca4 ca9 */
                continue;
        } else {
            if (index == 4 || index == 9) /* ca4 ca9 */
                continue;
        }

        /* compare result and pattern */
        if (!ddr_lpca_compare(&data->bits[index], dq_result, pattern_p, pattern_n, index))
            ddr_lpca_get_data(data, index, bdl); /* pass */
    }
}

/* Excute lpca command and check result */
static void ddr_lpca_excute(struct ca_data_st *data, unsigned int bdl, unsigned int is_ca49)
{
    volatile union u_phy_catconfig *ca = (union u_phy_catconfig *)
        (data->base_phy + DDR_PHY_CATCONFIG);

    if (is_ca49)
        ca->bits.sw_cat_mrw48 = 1;
    else
        ca->bits.sw_cat_mrw41 = 1;

    ddr_lpca_wait(ca);
    ca->bits.sw_cat_cke_low = 1;
    ddr_lpca_wait(ca);
    ca->bits.sw_cat_strobe = 1;
    ddr_lpca_wait(ca);

    /* check PHYDQRESULT */
    ddr_lpca_check(data, bdl, is_ca49);

    ca->bits.sw_cat_cke_high = 1;
    ddr_lpca_wait(ca);
    ca->bits.sw_cat_mrw42 = 1;
    ddr_lpca_wait(ca);
}

/* Find address bdl */
static int ddr_lpca_find_bdl(struct ca_data_st *data)
{
    unsigned int bdl;

    for (bdl = 0; bdl <= PHY_ACADDR_BDL_MASK; bdl++) {
        /* update bdl */
        ddr_lpca_set_bdl(data->base_phy, bdl);

        /* ca0~ca3, ca5~ca8 */
        ddr_lpca_excute(data, bdl, DDR_FALSE);

        /* ca4, ca9 */
        ddr_lpca_excute(data, bdl, DDR_TRUE);
    }

    if (data->done == PHY_CAT_PATTERN_MASK)
        return 0;

    return -1;
}

/* Loop phase to find valid bdl and phase */
static int ddr_lpca_loop_phase(struct ca_data_st *data, int step)
{
    volatile union u_phy_addrphbound *ph = (union u_phy_addrphbound *)
        (data->base_phy + DDR_PHY_ADDRPHBOUND);
    unsigned int phase;
    unsigned int addrph_def = ph->bits.addrph_a;
    int addrph = addrph_def;

    for (phase = 0; phase <= PHY_ADDRPH_MASK; phase++) {
        /* reset ca training data */
        ddr_lpca_reset(data);

        /* find bdl */
        if (!ddr_lpca_find_bdl(data))
            return 0;

        addrph += step;
        if (addrph < 0 || addrph > PHY_ADDRPH_MASK)
            break;

        ph->bits.addrph_a = addrph;
        ddr_phy_cfg_update(data->base_phy);
    }

    /* restore default value */
    ddr_debug("current phase[%x = %x], restore default[%x]", ph, *ph, addrph_def);
    ph->bits.addrph_a = addrph_def;

    return -1;
}

/* Find a valid phase */
static int ddr_lpca_find_phase(struct ca_data_st *data)
{
    /* increase default value to find */
    if (!ddr_lpca_loop_phase(data, 1))
        return 0;

    /* decrease default value to find */
    if (!ddr_lpca_loop_phase(data, -1))
        return 0;

    return -1;
}

/* Set step to adjust address window */
static int ddr_lpca_set_step(struct ca_data_st *data)
{
    /* max window, no need to found */
    if (data->min == 0 && data->max == PHY_ACADDR_BDL_MASK)
        return 0;

    if (data->min == 0)
        return -1; /* window on left, move to right */
    else
        return 1; /* window on right, move to left */
}

/*
 * Adjust address window via change phase.
 * Increase phase, window will move to left.
 */
static void ddr_lpca_adjust(struct ca_data_st *data)
{
    int step;
    volatile union u_phy_addrphbound *ph = (union u_phy_addrphbound *)
        (data->base_phy + DDR_PHY_ADDRPHBOUND);
    unsigned int phase;
    unsigned int addrph_last = ph->bits.addrph_a;
    int addrph_cur = addrph_last;

    /* set step to increase or decrease phase */
    step = ddr_lpca_set_step(data);
    if (!step)
        return;

    for (phase = 0; phase <= PHY_ADDRPH_MASK; phase++) {
        addrph_cur += step;
        if (addrph_cur < 0 || addrph_cur > PHY_ADDRPH_MASK)
            return;

        ph->bits.addrph_a = addrph_cur;
        ddr_phy_cfg_update(data->base_phy);

        /* reset ca training data */
        ddr_lpca_reset(data);

        if (ddr_lpca_find_bdl(data)) {
            /* not find bdl, restore last value */
            addrph_cur -= step;
            ddr_lpca_find_bdl(data);
            return;
        }

        /* max window: ------- */
        if (data->min == 0 && data->max == PHY_ACADDR_BDL_MASK)
            return;

        /* last window: -----xx */
        if (data->min == 0 && step == 1) {
            /* last value is best */
            addrph_cur -= step;
            ph->bits.addrph_a = addrph_cur;
            ddr_phy_cfg_update(data->base_phy);
            ddr_lpca_reset(data);
            ddr_lpca_find_bdl(data);
            return;
        }

        /* best window: x-----x */
        if (data->min > 0 && step == -1)
            return;
    }
}

/* Low power DDR CA training */
int ddr_lpca_training(const struct ddr_cfg_st *cfg)
{
    volatile union u_phy_catconfig *ca = (union u_phy_catconfig *)
        (cfg->cur_phy + DDR_PHY_CATCONFIG);

    struct ca_data_st data;
    int ret;

    ddr_debug("DDR LPCA training");

    /* init data */
    ddr_lpca_init(cfg->cur_dmc, cfg->cur_phy, &data);

    /* enable sw ca training, wait 62.5ns */
    ca->bits.sw_cat_en = 1;

    /* find a valid phase first */
    ret = ddr_lpca_find_phase(&data);

    /* display training result */
    ddr_lpca_display(&data);

    if (ret) {
        /* restore default value when fail */
        ddr_lpca_restore_def(&data);
        ddr_error("PHY[%x] found phase fail, result[%x]", cfg->cur_phy, data.done);
        ddr_training_stat(DDR_ERR_LPCA, cfg->cur_phy, -1, -1);
    } else {
        /* adjust window via phase */
        ddr_lpca_adjust(&data);
        ddr_lpca_display(&data);
        /* set training result */
        ddr_lpca_update_bdl(&data);
    }

    /* disable sw ca training */
    ca->bits.sw_cat_en = 0;

    /* save lpca result data to printf */
    ddr_lpca_data_save(cfg, &data);

    return ret;
}

int ddr_lpca_training_func(const struct ddr_cfg_st *cfg)
{
    int result = 0;
    struct tr_relate_reg relate_reg;

    /* LPCA training disable */
    if (ddr_training_check_bypass(cfg, DDR_BYPASS_LPCA_MASK) != DDR_FALSE)
        return 0;

    ddr_training_save_reg(cfg, &relate_reg, DDR_BYPASS_LPCA_MASK);

    /* only lowpower ddr3 support */
    if ((reg_read(cfg->cur_phy + DDR_PHY_DRAMCFG) & PHY_DRAMCFG_TYPE_LPDDR3) ==
        PHY_DRAMCFG_TYPE_LPDDR3)
        result += ddr_lpca_training(cfg);

    ddr_training_restore_reg(cfg, &relate_reg);

    return result;
}
#else
int ddr_lpca_training_func(const struct ddr_cfg_st *cfg)
{
    ddr_warning("Not support LPDDR CA training");
    return 0;
}
#endif /* DDR_LPCA_TRAINING_CONFIG */
