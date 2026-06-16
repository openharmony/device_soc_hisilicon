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

#include "ddr_interface.h"
#include "ddr_training_impl.h"

#ifdef DDR_SW_TRAINING_FUNC_PUBLIC
int ddr_training_boot_func(struct ddr_cfg_st *cfg)
{
    int result;

    /* check hardware gating */
    if (reg_read(cfg->cur_phy + DDR_PHY_PHYINITSTATUS) & PHY_INITSTATUS_GT_MASK) {
        ddr_fatal("PHY[%x] hw gating fail", cfg->cur_phy);
        ddr_training_stat(DDR_ERR_HW_GATING, cfg->cur_phy, -1, -1);
    }

    /* lpca */
    result = ddr_lpca_training_func(cfg);
    /* write leveling */
    result += ddr_wl_func(cfg);
    /* dataeye/gate/vref need switch axi */
    /* dataeye */
    result += ddr_dataeye_training_func(cfg);
#ifdef DDR_HW_TRAINING_CONFIG
    /* hardware read */
    if (result && (ddr_training_check_bypass(cfg, DDR_BYPASS_HW_MASK) == DDR_FALSE)) {
        struct tr_relate_reg relate_reg_ac;
        ddr_training_save_reg(cfg, &relate_reg_ac, DDR_BYPASS_HW_MASK);
        result = ddr_hw_dataeye_read(cfg);
        ddr_training_restore_reg(cfg, &relate_reg_ac);
        cfg->adjust = DDR_DATAEYE_ABNORMAL_ADJUST;
        result += ddr_dataeye_training(cfg);
    }
#endif
    /* mpr */
    result += ddr_mpr_training_func(cfg);
    /* gate */
    result += ddr_gating_func(cfg);
    /* vref */
    result += ddr_vref_training_func(cfg);

    return result;
}

/* Support DDRC510 with two PHY */
int ddr_sw_training_func(void)
{
    struct ddr_cfg_st ddr_cfg;
    struct ddr_cfg_st *cfg = &ddr_cfg;
    struct tr_custom_reg reg;
    int result;

#ifdef SYSCTRL_DDR_TRAINING_VERSION_FLAG
    /* DDR training version flag */
    unsigned int tmp_reg = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_VERSION_FLAG);
    tmp_reg = (tmp_reg & 0xffff0000) | DDR_VERSION;
    reg_write(tmp_reg, DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_VERSION_FLAG);
#endif

    /* check sw ddr training enable */
    if (DDR_BYPASS_ALL_MASK == reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG)
#ifdef SYSCTRL_DDR_TRAINING_CFG_SEC
        && DDR_BYPASS_ALL_MASK == reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_CFG_SEC)
#endif
    )
        return 0;

    ddr_training_start();

    ddrtr_set_data(&reg, 0, sizeof(struct tr_custom_reg));
    /* save customer reg */
    if (ddr_boot_cmd_save(&reg) != 0) {
        return -1;
    }

#ifdef DDR_TRAINING_STAT_CONFIG
    /* clear stat register */
    reg_write(0x0, DDR_REG_BASE_SYSCTRL + SYSCTRL_DDR_TRAINING_STAT);
#endif

    ddr_training_cfg_init(cfg);
    cfg->cmd_st = 0;

    result = ddr_training_all(cfg);
    result += ddr_dcc_training_func(cfg);
    if (!result) {
        ddr_training_suc();
    } else {
        ddr_training_console_if();
    }

    /* restore customer reg */
    ddr_boot_cmd_restore(&reg);

    return result;
}
#endif /* DDR_SW_TRAINING_FUNC_PUBLIC */

#ifdef DDR_PCODE_TRAINING_CONFIG
int ddr_pcode_training_func(void)
{
    struct ddr_cfg_st ddr_cfg;
    struct ddr_cfg_st *cfg = &ddr_cfg;

    ddr_training_cfg_init(cfg);
    return ddr_pcode_training(cfg);
}
#else
int ddr_pcode_training_func(void)
{
    ddr_warning("Not support DDR pcode training");
    return 0;
}
#endif

#ifdef DDR_HW_TRAINING_CONFIG
int ddr_hw_training_func(void)
{
    struct ddr_cfg_st ddr_cfg;
    struct ddr_cfg_st *cfg = &ddr_cfg;

    ddr_training_cfg_init(cfg);

    return ddr_hw_training(cfg);
}
#else
int ddr_hw_training_func(void)
{
    ddr_warning("Not support DDR HW training");
    return 0;
}
#endif /* DDR_HW_TRAINING_CONFIG */

int ddr_sw_training_if(void)
{
    return DDR_SW_TRAINING_FUNC();
}

int ddr_hw_training_if(void)
{
    return DDR_HW_TRAINING_FUNC();
}

int ddr_pcode_training_if(void)
{
    return DDR_PCODE_TRAINING_FUNC();
}

