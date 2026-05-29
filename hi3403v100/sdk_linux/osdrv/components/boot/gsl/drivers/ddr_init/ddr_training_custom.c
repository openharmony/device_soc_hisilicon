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

#define CRG_REG_BASE_ADDR      0x11010000
#define PERI_CRG_PLL6          0x180  /* DPLL-1 configuration register */
#define PERI_CRG_PLL7          0x184  /* DPLL-2 configuration register */
#define PERI_CRG_DDRT          0x22a0 /* DDRT clock and soft reset control register */
#define CRG_SOC_FREQ_CONFIG    0x2000 /* SOC frequency configuration register */
#define CRG_SOC_FREQ_INDICATE  0x2020 /* SOC frequency indication register */
#define CRG_CLK_RESET          0x2280 /* DDR clock reset configuration register */
#define SYSCTRL_DDRT_CTRL      0x4030 /* DDRT control register */

/* mask */
#define DDR_CKSEL_MASK         0x3 /* [13:12]ddr_cksel */
#define DDR_AXI_CKSEL_MASK     0x3 /* [13:12]ddraxi_cksel */
#define DDR_AXI_SC_SELED_MASK  0x3 /* [13:12]ddraxi_sc_seled */
#define DDR_TRFC_EN_MASK       0x1 /* bit[0] trfc_en */

/* bit */
#define DDR_CKSEL_BIT          12 /* [13:12]ddr_cksel */
#define DDR_AXI_CKSEL_BIT      12 /* [13:12]ddraxi_cksel */
#define DDR_AXI_SC_SELED_BIT   12 /* [13:12]ddraxi_sc_seled */
#define DDR_TRFC_EN_BIT        0  /* bit[0] trfc_en */
#define DDR_CFG_RX_AGE_COMPST_EN_BIT 31 /* bit[31] cfg_rx_age_compst_en */
#define DDR_TEST0_CKEN_BIT     4  /* ddrtest0_cken */
#define DDR_DDRT_CTRL_DDRT0_BIT 0 /* bit[0]ddrt0_mst_sel */
#define DDR_DDRT_CTRL_DDRT1_BIT 1 /* bit[1]ddrt1_mst_sel */

#define CRG_DDR_CKSEL_24M    0x0 /* ddr_cksel 00:24MHz */
#define CRG_DDRAXI_CKSEL_24M 0x0 /* ddraxi_cksel 00:24MHz */
#define CRG_DDR_CKSEL_PLL    0x1 /* ddr_cksel 001:clk_dpll_pst */
/* PLL 400MHz */
#define CRG_PLL7_TMP_1TO4 0x02001032
#define CRG_PLL6_TMP_1TO4 0x23000000
/* PLL 800MHz */
#define CRG_PLL7_TMP_1TO2 0x02001032
#define CRG_PLL6_TMP_1TO2 0x13000000
/* PLL 1600MHz */
#define CRG_PLL7_TMP_1TO1 0x02001064
#define CRG_PLL6_TMP_1TO1 0x13000000

#define DDR_FROM_VALUE0 0x0024140B /* partial boot form value */
#define DDR_FROM_VALUE1 0xFFDBEBF5 /* partial boot form value */

/*
 * Do some prepare before copy code from DDR to SRAM.
 * Keep empty when nothing to do.
 */
void ddr_cmd_prepare_copy(void)
{
    return;
}

/*
 * Save site before DDR training command execute .
 * Keep empty when nothing to do.
 */
void ddr_cmd_site_save(void)
{
    return;
}

/*
 * Restore site after DDR training command execute.
 * Keep empty when nothing to do.
 */
void ddr_cmd_site_restore(void)
{
    return;
}

static void ddr_rdqs_anti_aging(struct tr_custom_reg *custom_reg,
    unsigned int base_phy, unsigned int phy_idx)
{
    /* disable rdqs anti-aging */
    custom_reg->age_compst_en[phy_idx] = reg_read(base_phy + DDR_PHY_PHYRSCTRL);
    reg_write(custom_reg->age_compst_en[phy_idx] & (~(0x1 << DDR_CFG_RX_AGE_COMPST_EN_BIT)),
        base_phy + DDR_PHY_PHYRSCTRL);
}

static int ddr_disable_retrain(struct tr_custom_reg *custom_reg,
    unsigned int base_phy, unsigned int phy_idx)
{
    unsigned int cnt = DDR_LOOP_COUNT;

    custom_reg->trfc_en[phy_idx] = reg_read(base_phy + DDR_PHY_TRFC_CTRL);
    reg_write(custom_reg->trfc_en[phy_idx] & (~(DDR_TRFC_EN_MASK << DDR_TRFC_EN_BIT)),
        base_phy + DDR_PHY_TRFC_CTRL);

    while (cnt--) {
        if (!((reg_read(base_phy + DDR_PHY_TRFC_CTRL) >> DDR_TRFC_EN_BIT) & DDR_TRFC_EN_MASK)) {
            break;
        }
    }

    if (cnt == DDR_LOOP_COUNT) {
        return -1;
    }

    return 0;
}

int hal_ddr_boot_cmd_save(struct tr_custom_reg *custom_reg)
{
    if (custom_reg == NULL) {
        return -1;
    }

    /* enable ddrt control */
    custom_reg->ddrt_ctrl = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_CTRL);
    reg_write(custom_reg->ddrt_ctrl | (0x1 << DDR_DDRT_CTRL_DDRT0_BIT) | (0x1 << DDR_DDRT_CTRL_DDRT1_BIT),
        DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_CTRL);
    /* turn on ddrt clock */
    custom_reg->ddrt_clk_reg = reg_read(CRG_REG_BASE_ADDR + PERI_CRG_DDRT);
    /* enable ddrt0 clock */
    reg_write(custom_reg->ddrt_clk_reg | (0x1 << DDR_TEST0_CKEN_BIT),
        CRG_REG_BASE_ADDR + PERI_CRG_DDRT);
    __asm__ __volatile__("nop");
    /* disable ddrt0 soft reset */
    reg_write(reg_read(CRG_REG_BASE_ADDR + PERI_CRG_DDRT) & (~(0x1 << 0)),
        CRG_REG_BASE_ADDR + PERI_CRG_DDRT);

    return 0;
}

void hal_ddr_boot_cmd_restore(const struct tr_custom_reg *custom_reg)
{
    if (custom_reg == NULL) {
        return;
    }

    /* restore ddrt control */
    reg_write(custom_reg->ddrt_ctrl, DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_CTRL);

    /* restore ddrt clock */
    reg_write(custom_reg->ddrt_clk_reg, CRG_REG_BASE_ADDR + PERI_CRG_DDRT);
}

/*
 * Save site before DDR training:include boot and command execute.
 * Keep empty when nothing to do.
 */
int ddr_boot_cmd_save(struct tr_custom_reg *custom_reg)
{
    int result;

    if (custom_reg == NULL) {
        return -1;
    }

    /* enable ddrt control */
    custom_reg->ddrt_ctrl = reg_read(DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_CTRL);
    reg_write(custom_reg->ddrt_ctrl | (0x1 << DDR_DDRT_CTRL_DDRT0_BIT) | (0x1 << DDR_DDRT_CTRL_DDRT1_BIT),
        DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_CTRL);
    /* turn on ddrt clock */
    custom_reg->ddrt_clk_reg = reg_read(CRG_REG_BASE_ADDR + PERI_CRG_DDRT);
    /* enable ddrt0 clock */
    reg_write(custom_reg->ddrt_clk_reg | (0x1 << DDR_TEST0_CKEN_BIT),
        CRG_REG_BASE_ADDR + PERI_CRG_DDRT);
    __asm__ __volatile__("nop");
    /* disable ddrt0 soft reset */
    reg_write(reg_read(CRG_REG_BASE_ADDR + PERI_CRG_DDRT) & (~(0x1 << 0)),
        CRG_REG_BASE_ADDR + PERI_CRG_DDRT);

    /* disable rdqs anti-aging */
    ddr_rdqs_anti_aging(custom_reg, DDR_REG_BASE_PHY0, 0); /* 0: phy0 */
    /* disable retrain */
    result = ddr_disable_retrain(custom_reg, DDR_REG_BASE_PHY0, 0); /* 0: phy0 */

#ifdef DDR_REG_BASE_PHY1
    ddr_rdqs_anti_aging(custom_reg, DDR_REG_BASE_PHY1, 1); /* 1: phy1 */
    result += ddr_disable_retrain(custom_reg, DDR_REG_BASE_PHY1, 1); /* 1: phy1 */
#endif

    return result;
}

/*
 * Restore site after DDR training:include boot and command execute.
 * Keep empty when nothing to do.
 */
void ddr_boot_cmd_restore(const struct tr_custom_reg *custom_reg)
{
    if (custom_reg == NULL) {
        return;
    }

    /* restore ddrt control */
    reg_write(custom_reg->ddrt_ctrl, DDR_REG_BASE_SYSCTRL + SYSCTRL_DDRT_CTRL);

    /* restore ddrt clock */
    reg_write(custom_reg->ddrt_clk_reg, CRG_REG_BASE_ADDR + PERI_CRG_DDRT);

    /* restore ddrt clock */
    reg_write(custom_reg->ddrt_clk_reg, CRG_REG_BASE_ADDR + PERI_CRG_DDRT);

    /* restore rdqs anti-aging */
    reg_write(custom_reg->age_compst_en[0], DDR_REG_BASE_PHY0 + DDR_PHY_PHYRSCTRL);
    /* restore retrain */
    reg_write(custom_reg->trfc_en[0], DDR_REG_BASE_PHY0 + DDR_PHY_TRFC_CTRL);

#ifdef DDR_REG_BASE_PHY1
    reg_write(custom_reg->age_compst_en[1], DDR_REG_BASE_PHY1 + DDR_PHY_PHYRSCTRL);
    reg_write(custom_reg->trfc_en[1], DDR_REG_BASE_PHY1 + DDR_PHY_TRFC_CTRL);
#endif
}

/*
 * DDR clock select.
 * For ddr osc training.
 */
#ifdef DDR_PCODE_TRAINING_CONFIG
int ddr_get_cksel(void)
{
    int freq;
    unsigned int ddr_cksel;

    ddr_cksel = (reg_read(CRG_REG_BASE_ADDR + PERI_CRG_DDRCKSEL) >> 0x3) & 0x7;
    switch (ddr_cksel) {
        case 0x0:
            freq = 24;  /* 24 MHz */
            break;
        case 0x1:
            freq = 450; /* 450 MHz */
            break;
        case 0x3:
            freq = 300; /* 300 MHz */
            break;
        case 0x4:
            freq = 297; /* 297 MHz */
            break;
        default:
            freq = 300; /* 300 MHz */
            break;
    }
    return freq;
}
#endif

/* Configure ddr frequency */
static void ddr_cfg_freq_process(unsigned int crg_pll7, unsigned int crg_pll6)
{
    unsigned int clk_reset;
    unsigned int soc_freq_conf;
    unsigned int ddraxi_sc_seled;

    clk_reset = reg_read(CRG_REG_BASE_ADDR + CRG_CLK_RESET);
    soc_freq_conf = reg_read(CRG_REG_BASE_ADDR + CRG_SOC_FREQ_CONFIG);

    /* DDR cksel switch to 24MHZ */
    reg_write((clk_reset & (~(DDR_CKSEL_MASK << DDR_CKSEL_BIT))) | (CRG_DDR_CKSEL_24M << DDR_CKSEL_BIT),
        CRG_REG_BASE_ADDR + CRG_CLK_RESET);
    ddr_training_delay(DDR_SET_FRE_DELAY_10US); /* wait 10us */
    /* ddraxi cksel switch to 24MHZ */
    reg_write((soc_freq_conf & (~(DDR_AXI_CKSEL_MASK << DDR_AXI_CKSEL_BIT))) |
        (CRG_DDRAXI_CKSEL_24M << DDR_AXI_CKSEL_BIT),
        CRG_REG_BASE_ADDR + CRG_SOC_FREQ_CONFIG);
    ddr_training_delay(DDR_SET_FRE_DELAY_10US); /* wait 10us */
    /* check ddr axi clock stat */
    ddraxi_sc_seled = (reg_read(CRG_REG_BASE_ADDR + CRG_SOC_FREQ_INDICATE) >>
        DDR_AXI_SC_SELED_BIT) & DDR_AXI_SC_SELED_MASK;
    if (ddraxi_sc_seled != CRG_DDRAXI_CKSEL_24M) {
        return;
    }

    /* Configure DPLL */
    reg_write(crg_pll7 | (0x1 << 20), CRG_REG_BASE_ADDR + PERI_CRG_PLL7); /* bit[20] */
    reg_write(crg_pll6, CRG_REG_BASE_ADDR + PERI_CRG_PLL6);
    ddr_training_delay(DDR_SET_FRE_DELAY_1US); /* wait 1us */
    reg_write(crg_pll7 & (~(0x1 << 20)), CRG_REG_BASE_ADDR + PERI_CRG_PLL7); /* bit[20] */
    ddr_training_delay(DDR_SET_FRE_DELAY_100US); /* wait 100us */

    /* restore  CRG_CLK_RESET and CRG_SOC_FREQ_CONFIG */
    reg_write(clk_reset, CRG_REG_BASE_ADDR + CRG_CLK_RESET);
    reg_write(soc_freq_conf, CRG_REG_BASE_ADDR + CRG_SOC_FREQ_CONFIG);
    ddr_training_delay(DDR_SET_FRE_DELAY_10US);
}

static void ddr_cfg_freq(unsigned int base_phy)
{
    unsigned int dficlk_ratio;
    unsigned int crg_pll7_tmp;
    unsigned int crg_pll6_tmp;

    /* dramCK Clock Ratio */
    dficlk_ratio = (reg_read(base_phy + DDR_PHY_PHYCTRL0) >> PHY_DFICLK_RATIO_BIT) &
        PHY_DFICLK_RATIO_MASK;
    switch (dficlk_ratio) {
        case 0x0: /* 1:1 */
            crg_pll7_tmp = CRG_PLL7_TMP_1TO1; /* PLL 1600MHz */
            crg_pll6_tmp = CRG_PLL6_TMP_1TO1;
            break;
        case 0x1: /* 1:2 */
            crg_pll7_tmp = CRG_PLL7_TMP_1TO2; /* PLL 800MHz */
            crg_pll6_tmp = CRG_PLL6_TMP_1TO2;
            break;
        case 0x2: /* 1:4 */
    /* Fall through to PLL 400MHz */
        default:
            crg_pll7_tmp = CRG_PLL7_TMP_1TO4; /* PLL 400MHz */
            crg_pll6_tmp = CRG_PLL6_TMP_1TO4;
            break;
    }

    /* Configure DDR low frequency */
    ddr_cfg_freq_process(crg_pll7_tmp, crg_pll6_tmp);
}

static void ddr_cfg_phy_clk(unsigned int clk_status)
{
    unsigned int gated0;
    unsigned int gated1;
    /* bit[31], bit[14:0] */
     gated0 = reg_read(DDR_REG_BASE_PHY0 + DDR_PHY_CLKGATED);
    reg_write((gated0 & (~PHY_CLKGATED_MASK)) | clk_status,
        DDR_REG_BASE_PHY0 + DDR_PHY_CLKGATED);

#ifdef DDR_REG_BASE_PHY1
    gated1 = reg_read(DDR_REG_BASE_PHY1 + DDR_PHY_CLKGATED);
    reg_write((gated1 & (~PHY_CLKGATED_MASK)) | clk_status,
        DDR_REG_BASE_PHY1 + DDR_PHY_CLKGATED);
#endif
}

/* config DDR PLL power down or power up */
static void ddr_cfg_pll_power(unsigned int pll_status)
{
    unsigned int pll_ctrl_0;
    unsigned int pll_ctrl_1;

    pll_ctrl_0 = reg_read(DDR_REG_BASE_PHY0 + DDR_PHY_PLLCTRL);
    reg_write((pll_ctrl_0 & (~(PHY_PLL_PWDN_MASK << PHY_PLL_PWDN_BIT))) |
        (pll_status << PHY_PLL_PWDN_BIT), DDR_REG_BASE_PHY0 + DDR_PHY_PLLCTRL);

#ifdef DDR_REG_BASE_PHY1
    pll_ctrl_1 = reg_read(DDR_REG_BASE_PHY1 + DDR_PHY_PLLCTRL);
    reg_write((pll_ctrl_1 & (~(PHY_PLL_PWDN_MASK << PHY_PLL_PWDN_BIT))) |
        (pll_status << PHY_PLL_PWDN_BIT), DDR_REG_BASE_PHY1 + DDR_PHY_PLLCTRL);
#endif
    ddr_training_delay(DDR_SET_FRE_DELAY_100US); /* wait 100us */
}

static int ddr_low_fre_start(struct tr_custom_reg *reg)
{
    int result;
    unsigned int i;
    unsigned int crg_pll7_init_val;
    unsigned int crg_pll6_init_val;

    struct dmc_cfg_sref_st cfg_sref;
    struct ddr_cfg_st ddr_cfg;
    struct ddr_cfg_st *cfg = &ddr_cfg;

    crg_pll7_init_val = reg_read(CRG_REG_BASE_ADDR + PERI_CRG_PLL7);
    crg_pll6_init_val = reg_read(CRG_REG_BASE_ADDR + PERI_CRG_PLL6);

    ddr_training_cfg_init(cfg);

    if (ddr_boot_cmd_save(reg) != 0) {
        return -1;
    }

    /* Configure DDR low frequency, does not distinguish PHY */
    ddr_cfg_freq(cfg->phy[0].addr);

    /* Perform partial initialization of DDR */
    ddr_training_hw_item_cfg(cfg, DDR_FROM_VALUE0);
    result = ddr_hw_training(cfg);
    /* enter auto self-refresh */
    for (i = 0; i < cfg->phy_num; i++) {
        cfg->phy_idx = i;
        if (cfg->phy[cfg->phy_idx].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
            ddr_sref_cfg(cfg, &cfg_sref, DMC_CFG_INIT_XSREF | DMC_CFG_SREF_PD); /* bit[3:2] 0x3 */
        }

        if (ddr_training_ctrl_easr(cfg, DDR_ENTER_SREF)) {
            return -1;
        }
    }

    /* close phy clock gated */
    ddr_cfg_phy_clk(PHY_CLK_GATED_CLOSE);

    /* DDR PLL power down */
    ddr_cfg_pll_power(PHY_PLL_POWER_DOWN);

    /* Configure DDR high frequency */
    ddr_cfg_freq_process(crg_pll7_init_val, crg_pll6_init_val);

    /* DDR PLL power up */
    ddr_cfg_pll_power(PHY_PLL_POWER_UP);

    /* open phy clock gated */
    ddr_cfg_phy_clk(PHY_CLK_GATED_OPEN);

    for (i = 0; i < cfg->phy_num; i++) {
        ddr_ck_cfg(cfg->phy[i].addr);
    }

    /* exit auto self-refresh */
    for (i = 0; i < cfg->phy_num; i++) {
        cfg->phy_idx = i;
        if (cfg->phy[cfg->phy_idx].dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
            ddr_sref_cfg(cfg, &cfg_sref, DMC_CFG_INIT_XSREF | DMC_CFG_SREF_PD); /* bit[3:2] 0x3 */
        }

        if (ddr_training_ctrl_easr(cfg, DDR_EXIT_SREF)) {
            return -1;
        }
    }

    /* Perform partial initialization of DDR */
    ddr_training_hw_item_cfg(cfg, DDR_FROM_VALUE1);
    result += ddr_hw_training(cfg);
    /* open rdqs anti-aging */
    ddr_boot_cmd_restore(reg);

    return result;
}

int ddr_hw_training_init(void)
{
    int result;
    unsigned int dram_type;
    struct tr_custom_reg reg;

    ddrtr_set_data(&reg, 0, sizeof(struct tr_custom_reg));
    dram_type = reg_read(DDR_REG_BASE_PHY0 + DDR_PHY_DRAMCFG) & PHY_DRAMCFG_TYPE_MASK;
    if (dram_type == PHY_DRAMCFG_TYPE_LPDDR4) {
        result = ddr_low_fre_start(&reg);
    } else {
        if (ddr_boot_cmd_save(&reg) != 0) {
            return -1;
        }

        result = ddr_hw_training_if();
        ddr_boot_cmd_restore(&reg);
    }
    return result;
}

static void ddr_retrain_anti_aging_enable_process(unsigned int base_phy)
{
    unsigned int age_compst_en;
    unsigned int trfc_ctrl;
    unsigned int addr_delay;
    unsigned int trfc_mpc_cmd_dly;
    unsigned int tphy_wrdata;
    unsigned int dficlk_ratio_sel;
    unsigned int cfg_rl;
    unsigned int cfg_wl;
    unsigned int ddr_phy_misc;

    /* enable rdqs anti-aging */
    age_compst_en = reg_read(base_phy + DDR_PHY_PHYRSCTRL);
    reg_write((age_compst_en & (~(0x1 << DDR_CFG_RX_AGE_COMPST_EN_BIT))) |
        (0x1 << DDR_CFG_RX_AGE_COMPST_EN_BIT), base_phy + DDR_PHY_PHYRSCTRL);

    /* control by reg */
     trfc_ctrl = reg_read(base_phy + DDR_PHY_TRFC_CTRL);
    if (trfc_ctrl == 0) {
        return;
    }

    /* update cfg_wl & cfg_rl */
    ddr_phy_misc = reg_read(base_phy + DDR_PHY_MISC);
    addr_delay = (ddr_phy_misc >> PHY_MISC_ADDR_DELAY_BIT) & PHY_MISC_ADDR_DELAY_MASK;
    if (addr_delay > PHY_MISC_ADDR_DELAY_MASK) {
        return;
    }

    trfc_mpc_cmd_dly = (reg_read(base_phy + DDR_PHY_TRFC_THRESHOLD1) >>
        PHY_TRFC_MPC_CMD_DLY_BIT) & PHY_TRFC_MPC_CMD_DLY_MASK;
    if (trfc_mpc_cmd_dly > PHY_TRFC_MPC_CMD_DLY_MASK) {
        return;
    }

    tphy_wrdata = (reg_read(base_phy + DDR_PHY_DMSEL) >> PHY_DMSEL_TPHY_WRDATA_BIT) &
        PHY_DMSEL_TPHY_WRDATA_MASK;
    if (tphy_wrdata > PHY_DMSEL_TPHY_WRDATA_MASK) {
        return;
    }

    dficlk_ratio_sel = (reg_read(base_phy + DDR_PHY_PHYCTRL0) >>
        PHY_DFICLK_RATIO_BIT) & PHY_DFICLK_RATIO_MASK;
    if (dficlk_ratio_sel > PHY_DFICLK_RATIO_MASK) {
        return;
    }

    cfg_rl = (ddr_phy_misc >> PHY_MISC_CFG_RL_BIT) & PHY_MISC_CFG_RL_MASK;
    cfg_wl = (ddr_phy_misc >> PHY_MISC_CFG_WL_BIT) & PHY_MISC_CFG_WL_MASK;
    cfg_rl = cfg_rl -
        (4 + addr_delay - trfc_mpc_cmd_dly * tphy_wrdata) * dficlk_ratio_sel * 2; /* 4,2: Logical design requirements */
    cfg_wl = cfg_wl -
        (4 + addr_delay - trfc_mpc_cmd_dly * tphy_wrdata) * dficlk_ratio_sel * 2; /* 4,2: Logical design requirements */
    ddr_phy_misc &= ~((PHY_MISC_CFG_RL_MASK << PHY_MISC_CFG_RL_BIT) | (PHY_MISC_CFG_WL_MASK << PHY_MISC_CFG_WL_BIT));
    ddr_phy_misc |= (cfg_rl << PHY_MISC_CFG_RL_BIT) | (cfg_wl << PHY_MISC_CFG_WL_BIT);
    reg_write(ddr_phy_misc, base_phy + DDR_PHY_MISC);

    /* enable retrain */
    reg_write((trfc_ctrl & (~(DDR_TRFC_EN_MASK << DDR_TRFC_EN_BIT))) | (0x1 << DDR_TRFC_EN_BIT),
        base_phy + DDR_PHY_TRFC_CTRL);
}

/* enable retrain and rdqs anti-aging after ddr training */
void ddr_retrain_anti_aging_enable(void)
{
    ddr_retrain_anti_aging_enable_process(DDR_REG_BASE_PHY0);

#ifdef DDR_REG_BASE_PHY1
    ddr_retrain_anti_aging_enable_process(DDR_REG_BASE_PHY1);
#endif
}

static void ddr_dmc_auto_pd_by_phy(unsigned int base_phy, unsigned int dmc0, unsigned int dmc1)
{
    unsigned int dmc_pd0;
    unsigned int pd_prd;

    dmc_pd0 = reg_read(dmc0 + DDR_DMC_CFG_PD);
    pd_prd = (dmc_pd0 >> DMC_CFG_PD_PRD_BIT) & DMC_CFG_PD_PRD_MASK;
    if (pd_prd == 0) {
        return;
    }

    if ((reg_read(base_phy + DDR_PHY_DRAMCFG) &
        PHY_DRAMCFG_TYPE_MASK) == PHY_DRAMCFG_TYPE_LPDDR4) {
        reg_write(dmc_pd0 | (0x1 << DMC_CFG_PD_EN_BIT), dmc0 + DDR_DMC_CFG_PD);
        reg_write(reg_read(dmc1 + DDR_DMC_CFG_PD) | (0x1 << DMC_CFG_PD_EN_BIT), dmc1 + DDR_DMC_CFG_PD);
    } else {
        reg_write(dmc_pd0 | (0x1 << DMC_CFG_PD_EN_BIT), dmc0 + DDR_DMC_CFG_PD);
    }
}

/* ddr DMC auto power down config
 * the value should config after trainning, or it will cause chip compatibility problems
 */
void ddr_dmc_auto_power_down_cfg(void)
{
    ddr_dmc_auto_pd_by_phy(DDR_REG_BASE_PHY0, DDR_REG_BASE_DMC0, DDR_REG_BASE_DMC1);

#ifdef DDR_REG_BASE_PHY1
    ddr_dmc_auto_pd_by_phy(DDR_REG_BASE_PHY1, DDR_REG_BASE_DMC2, DDR_REG_BASE_DMC3);
#endif

#ifdef DDR_REG_BASE_PHY2
    ddr_dmc_auto_pd_by_phy(DDR_REG_BASE_PHY2, DDR_REG_BASE_DMC4, DDR_REG_BASE_DMC5);
#endif
}

static void ddr_set_rdqbdl_def_val_by_phy(unsigned int base_phy)
{
    unsigned int rank_idx;
    unsigned int byte_idx;

    for (rank_idx = 0; rank_idx < 2; rank_idx++) { /* 2:rank number */
        for (byte_idx = 0; byte_idx < 4; byte_idx++) { /* 4:byte number */
            reg_write(0x20202020, base_phy + ddr_phy_dxnrdqnbdl0(rank_idx, byte_idx));
            reg_write(0x20202020, base_phy + ddr_phy_dxnrdqnbdl1(rank_idx, byte_idx));
            reg_write(0x20, base_phy + ddr_phy_dxnrdqnbdl2(rank_idx, byte_idx));
        }
    }
}

void ddr_set_rdqbdl_def_val(void)
{
    ddr_set_rdqbdl_def_val_by_phy(DDR_REG_BASE_PHY0);

#ifdef DDR_REG_BASE_PHY1
    ddr_set_rdqbdl_def_val_by_phy(DDR_REG_BASE_PHY1);
#endif

#ifdef DDR_REG_BASE_PHY2
    ddr_set_rdqbdl_def_val_by_phy(DDR_REG_BASE_PHY2);
#endif
}
