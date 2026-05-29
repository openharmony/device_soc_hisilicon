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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART06_SUB02_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART06_SUB02_H_

    /* clear ck0 ck1 */
    acphyctl7_tmp = acphyctl7 & (~(PHY_ACPHY_DCLK_MASK << PHY_ACPHY_DCLK0_BIT)) &
        (~(PHY_ACPHY_DCLK_MASK << PHY_ACPHY_DCLK1_BIT));
    /* set the opposite val of ck */
    reg_write(acphyctl7_tmp | ((~ck0) << PHY_ACPHY_DCLK0_BIT) | ((~ck1) << PHY_ACPHY_DCLK1_BIT),
        base_phy + DDR_PHY_ACPHYCTL7);
    /* restore acphyctl7 */
    reg_write(acphyctl7, base_phy + DDR_PHY_ACPHYCTL7);

    ddr_phy_cfg_update(base_phy);
}

/* DDR HW training control */
static int ddr_hw_training_ctl(struct ddr_cfg_st *cfg)
{
    int result = 0;
    unsigned int byte_idx;
    unsigned int temp;
    unsigned int base_phy = cfg->cur_phy;
    struct ddr_tmp_st ddr_temp;
    struct rdqs_data_st *rdqs_st = NULL;
    struct ddr_bdl_dly_st bdl_dly_s;

    rdqs_st = (struct rdqs_data_st *)cfg->res_st;

    if (cfg->cur_item == 0 || rdqs_st == NULL) {
        return 0;
    }

    ddr_phy_cfg_update(base_phy);
    /* NOTE: not support array when boot */
    result += ddr_hw_training_process(cfg, cfg->cur_item & PHY_HW_GP_CNT_RESET_START);
    result += ddr_hw_training_process(cfg, cfg->cur_item & PHY_HW_GP_PLL);

    ddr_ck_cfg(base_phy);

    /* save rdqs bdl after PHY_PHYINITCTRL_DLYMEAS_EN */
    if (cfg->rank_idx == 0) {
        ddr_training_get_rdqs(cfg, &rdqs_st->origin);
        temp = ((reg_read(base_phy + ddr_phy_dxnrdqsdly(0)) >> PHY_RDQS_CYC_BIT) &
            PHY_RDQS_CYC_MASK) >> 2; /* right shift 2: 1/4T */
        temp = (reg_read(base_phy + DDR_PHY_TRAINCTRL12) & (~(PHY_WL_FALLEDGE_BDL_JSTEP_R_MASK <<
            PHY_WL_FALLEDGE_BDL_JSTEP_R_BIT))) | (temp << PHY_WL_FALLEDGE_BDL_JSTEP_R_BIT);
        reg_write(temp, base_phy + DDR_PHY_TRAINCTRL12);
    }
    if ((cfg->phy_idx >= DDR_PHY_NUM) || (cfg->phy[cfg->phy_idx].total_byte_num > DDR_PHY_BYTE_MAX)) {
        ddr_error("phy_idx or byte number error");
        return -1;
    }
    for (byte_idx = 0; byte_idx < (cfg->phy[cfg->phy_idx].total_byte_num); byte_idx++) {
        cfg->cur_byte = byte_idx;
        ddr_rdqbdl_adj(cfg, &bdl_dly_s);
    }
    result += ddr_hw_dataeye_adapt(cfg, &ddr_temp);
    result += ddr_hw_training_process(cfg, cfg->cur_item & PHY_PHYINITCTRL_CAT_EN);
    result += ddr_hw_training_process(cfg, cfg->cur_item & PHY_HW_GP_CS);
    result += ddr_hw_dataeye_vref_set(cfg);
    result += ddr_hw_training_normal_conf(cfg);

#ifdef DDR_WRITE_DM_DISABLE
    result += ddr_hw_write_dm_disable(cfg, &ddr_temp);
#endif
    ddr_phy_cfg_update(base_phy);

    return result;
}

static int ddr_hw_training_by_rank(struct ddr_cfg_st *cfg)
{
    ddr_debug("PHY[%x][%x] Rank[%x] itme[%x]",
        cfg->phy_idx, cfg->cur_phy, cfg->rank_idx, cfg->cur_item);

    /* 0:PHY_TRAINCTRL0_DTR_RANK0, 1:PHY_TRAINCTRL0_DTR_RANK1 */
    ddr_phy_switch_rank(cfg->cur_phy, cfg->rank_idx);

    return ddr_hw_training_ctl(cfg);
}

int ddr_hw_training_by_phy(struct ddr_cfg_st *cfg)
{
    int result = 0;
    unsigned int i;
    struct rdqs_data_st rdqs_data;
    struct rdqs_data_st *rdqs_st = &rdqs_data;
    struct ddr_timing_st timing_st;
    unsigned int rank_num = cfg->phy[cfg->phy_idx].rank_num;

    cfg->res_st = rdqs_st;

    /* disable auto refresh */
    ddr_training_save_timing(cfg, &timing_st);

    if (rank_num > DDR_SUPPORT_RANK_MAX) {
        ddr_error("loop upper limit rank number out of range, rank_num = %x", rank_num);
        return -1;
    }
    for (i = 0; i < rank_num; i++) {
        cfg->rank_idx = i;
        if (cfg->phy_idx >= DDR_PHY_NUM) {
            ddr_error("Array index phy_idx out of range!");
            return -1;
        }
        cfg->cur_item = cfg->phy[cfg->phy_idx].rank[i].item_hw;

        result += ddr_hw_training_by_rank(cfg);

        if (rank_num != DDR_SUPPORT_RANK_MAX) {
            break;
        }

        /* save rank rdqs bdl */
        ddr_training_get_rdqs(cfg, &(rdqs_st->rank[i]));

        /* restore PHY_PHYINITCTRL_DLYMEAS_EN rdqs before training next rank */
        if ((rank_num - 1) != i) {
            ddr_training_set_rdqs(cfg, &(rdqs_st->origin));
        }
    }

    if (rank_num == DDR_SUPPORT_RANK_MAX) {
        ddr_hw_training_adjust_rdqs(cfg, rdqs_st);
        ddr_training_adjust_wdq(cfg);
        ddr_training_adjust_wdqs(cfg);
        ddr_phy_switch_rank(cfg->cur_phy, 0x0); /* switch to rank0 */
    }

    /* restore auto refresh */
    ddr_training_restore_timing(cfg, &timing_st);

    cfg->res_st = 0;

    return result;
}

/* DDR hardware training */
int ddr_hw_training(struct ddr_cfg_st *cfg)
{
    int result = 0;
    unsigned int i;
    struct tr_custom_reg reg;

    if (cfg == NULL) {
        ddr_error("Pointer parameter cfg is NULL!");
        return -1;
    }

    ddrtr_set_data(&reg, 0, sizeof(struct tr_custom_reg));
    /* save customer reg */
    ddr_boot_cmd_save(&reg);
    if (cfg->phy_num > DDR_PHY_NUM) {
        ddr_error("loop upper limit cfg->phy_num out of range!");
        return -1;
    }
    for (i = 0; i < cfg->phy_num; i++) {
        cfg->phy_idx = i;
        cfg->cur_phy = cfg->phy[i].addr;
        result += ddr_hw_training_by_phy(cfg);
    }
    /* restore customer reg */
    ddr_boot_cmd_restore(&reg);

    return result;
}


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_TRAINING_IMPL_BODY_PART06_SUB02_H_ */
