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

#ifndef DDR_PHY_S14_H
#define DDR_PHY_S14_H

/* register offset address */
/* base address: DDR_REG_BASE_PHY0 DDR_REG_BASE_PHY1 */
/* control the initialization of the PHY */
#define DDR_PHY_PHYINITCTRL    0x4
#define DDR_PHY_PHYINITSTATUS  0x8  /* Read Data Eye Calibration Error */
#define DDR_PHY_CLKGATED       0xc  /* This register control the clock gated of PHY */
#define DDR_PHY_PLLCTRL        0x18 /* specified the timing paramters for PLL in both address/command, and data block */
#define DDR_PHY_IMPSTATUS      0x28 /* This register specify the ZQ calibration result. */
#define DDR_PHY_DRAMCFG        0x2c /* DRAM config register */
#define DDR_PHY_DRAMTIMER1     0x34 /* This register specify the DRAM timing parameters */
#define DDR_PHY_TRAINCTRL0     0x48 /* hw training control */
#define DDR_PHY_RANKEN         0x4c /* This register specify the training rank control */
#define DDR_PHY_MODEREG01      0x64 /* Extend Mode Register 01 */
#define DDR_PHY_MODEREG23      0x68 /* Extend Mode Register 23 */
/* update delay setting in registers to PHY */
#define DDR_PHY_MISC           0x70
#define DDR_PHY_PHYCTRL0       0x78 /* Register in this field are connected to PHY interface directly */
#define DDR_PHY_DMSEL          0x84 /* DM Swap Selection */
#define DDR_PHY_SWTMODE        0xa0 /* S/W training mode */
/* issue one DQS pulse from PHY to DRAM */
#define DDR_PHY_SWTWLDQS       0xa4
#define DDR_PHY_SWTRLT         0xa8 /* S/W training result */
/* Host vref. [5:0]range [17:12]refsel */
#define DDR_PHY_PHYRSCTRL      0xB0 /* PHY Register Slice Contrl */
#define DDR_PHY_VREFTCTRL      0xc0 /* VREF Training Control Register. */
#define DDR_PHY_HVRFTCTRL      0xc8 /* Host VREF Training Control Regiser. */
#define DDR_PHY_DVRFTCTRL      0xC4 /* DRAM VREF Training */
#define DDR_PHY_TRAINCTRL2     0xd4   /* This register control the data training */
#define DDR_PHY_TRAINCTRL3     0xdc   /* This register control the data training */
#define DDR_PHY_MODEREG45      0xe0   /* This register defines the contents of the Mode Register */
#define DDR_PHY_MODEREG67      0xe4   /* This register defines the contents of the Mode Register */
#define DDR_PHY_TRAINCTRL5     0x118  /* This register control the data training */
#define DDR_PHY_ACCMDBDL2      0x128  /* AC command bit delay line setting */
#define ddr_phy_acaddrbdl(n)   (0x140 + ((n) << 2))
#define DDR_PHY_IMP_CTRL1      0x170  /* AC/DX ZQ selection */
#define DDR_PHY_IMP_STATUS1    0x174  /* AC ZCAL status */
#define DDR_PHY_CATSWAPINDEX   0x01B8 /* CA SWAP index register */
#define DDR_PHY_CATSWAPSEL     0x01BC /* CA SWAP select register */
#define DDR_PHY_CATCONFIG      0x1C8  /* CA Training Configuration */
#define DDR_PHY_PHYDQRESULT    0x1D0  /* SW CA Training DQ result from PHY */
#define DDR_PHY_ADDRPHBOUND    0x1D4  /* CA Training addr phase boundary */
#define DDR_PHY_SWCATPATTERN_P 0x1D8  /* pattern for positive CK edge */
#define DDR_PHY_SWCATPATTERN_N 0x1DC  /* pattern for negative CK edge */
#define DDR_PHY_MRS_SEQ_PROG   0x1e0  /* Programmed MRS sequence in the DRAM initialization */
#define DDR_PHY_TRAINCTRL12    0x1e8  /* TRAINCTRL 12 Register */
#define DDR_PHY_TRFC_THRESHOLD1 0x8d0 /* trfc retrain/tracking threshold setting 1 */
#define DDR_PHY_TRFC_CTRL      0x8e0  /* specify the trfc retrain/tracking control */

/* WR DQ0-DQ3 [6:0] [14:8] [22:16] [30:24] delay value of the bit delay line
on write path */
#define ddr_phy_dxnwdqnbdl0(m, n)   (0x210 + ((m) << 10) + ((n) << 7))
/* WR DQ4-DQ7 [6:0] [14:8] [22:16] [30:24] */
#define ddr_phy_dxnwdqnbdl1(m, n)   (0x214 + ((m) << 10) + ((n) << 7))
/* WR DM [6:0] the delay value of the bit delay line on DQM */
#define ddr_phy_dxnwdqnbdl2(m, n)   (0x218 + ((m) << 10) + ((n) << 7))
/* RD DQ0-DQ3 [6:0] [14:8] [22:16] [30:24] delay value of the bit delay line
 on read path */
#define ddr_phy_dxnrdqnbdl0(m, n)   (0x21C + ((m) << 10) + ((n) << 7))
/* RD DQ4-DQ7 [6:0] [14:8] [22:16] [30:24] delay value of the bit delay line
 on read path */
#define ddr_phy_dxnrdqnbdl1(m, n)   (0x220 + ((m) << 10) + ((n) << 7))
/* [6:0]RD DM */
#define ddr_phy_dxnrdqnbdl2(m, n)   (0x224 + ((m) << 10) + ((n) << 7))

/* [CUSTOM] */
#define ddr_phy_dxnoebdl(m, n)      (0x228 + ((m) << 10) + ((n) << 7))
/* [8:0] rdqs_bdl [24:16]rdqs_cyc.
phase shift of the Read DQS to create 90 degree delays */
#define ddr_phy_dxnrdqsdly(n)       (0x22C + ((n) << 7))
/* [6:0] the delay value of delay applied on WDQS for write leveling */
#define ddr_phy_dxwdqsdly(m, n)     (0x230 + ((m) << 10) + ((n) << 7))
/* WR DQ phase BIT 12:8 */
#define ddr_phy_dxnwdqdly(m, n)     (0x234 + ((m) << 10) + ((n) << 7))
/* DXNWLSL:This reister is used to control if PHY controller add extra system latency */
#define ddr_phy_dxnwlsl(m, n)       (0x238 + ((m) << 10) + ((n) << 7))
/* [CUSTOM] rdqs gating */
#define ddr_phy_dxnrdqsgdly(m, n)   (0x240 + ((m) << 10) + ((n) << 7))
/* read boundary  right 8:0 left 24:16 */
#define ddr_phy_dxnrdbound(n)       (0x250 + ((n) << 7))
/* write boundary  right 4:0 left 20:16 */
#define ddr_phy_dxnwdbound(n)       (0x254 + ((n) << 7))
/* [5:0] DRAM VREF(DQ) training result */
#define ddr_phy_dvreft_status(n)    (0x270 + ((n) << 7))
/* [4:0] Host PHY VREF(DQ) training result */
#define ddr_phy_hvreft_status(m, n) (0x274 + ((m) << 10) + ((n) << 7))

/* DDRPHY AC static register */
/* #define DDR_PHY_CORNER_DETECTOR 0x104C  cfg of corner detector */
#define DDR_PHY_ACIOCTL          0x1018  /* IO control register */
#define DDR_PHY_ACPHYCTL4        0x1064  /* AC block PHY control register */
#define DDR_PHY_ACPHYCTL7        0x1070
#define DDR_PHY_ACIOCTL21        0x10A4
#define DDR_PHY_AC_GATED_BYPASS  0x10A4  /* bypass clock gated function */
#define ddr_phy_dxphyctrl2(q)    (0x1000 + 0x214 + ((q) << 8))
/* Data block PHY debug/miscellaneous control register. dxctl_pre_margin_code [24:22] */
#define dx_dxnmiscctrl3(p)      (0x1254 + ((p) << 7))

#define DDR_VREF_HOST_VAL_MAX    0x3f  /* 78.75%*VDDIO */
#define DDR_VREF_HOST_VAL_MIN    0x0   /* 40.00%*VDDIO */

/* register mask */
#define PHY_BDL_MASK            0x7f   /* [6:0] */
#define PHY_WDQ_PHASE_MASK      0x3f   /* [13:8] */
#define PHY_WDQS_PHASE_MASK     0xf    /* [11:8] */
#define PHY_WDQS_BDL_MASK       0x7f   /* [6:0] */
#define PHY_WDM_BDL_MASK        0x7f   /* [6:0] */
#define PHY_RDQS_BDL_MASK       0x1ff  /* [CUSTOM] [8:0] rdqsbdl */
#define PHY_RDQSG_PHASE_MASK    0xff   /* [16:9] rdqsgphase */
#define PHY_RDM_BDL_MASK        0x7f   /* [6:0] */
#define PHY_RDQS_CYC_MASK       0x1ff  /* [24:16]Read DQS Clock Phase Select */
/* hardware gate training result */
#define PHY_INITSTATUS_GT_MASK  0x20
#define PHY_SWTRLT_WL_MASK      0xf
#define PHY_SWTRLT_GATE_MASK    0xf
#define PHY_PHYINITCTRL_MASK    0x1    /* [15:0] all stat */
/* Read Data Eye Calibration Error */
#define PHY_PHYINITSTATUS_RDET_ERR  0x100
#define PHY_ACPHY_DCLK_MASK         0x7   /* cp1p_dclk0 mask */
#define PHY_ACPHY_DRAMCLK_MASK      0x1   /* halft_dramclk0 mask */
#define PHY_VRFTRES_DVREF_MASK      0x3f  /* [5:0] */
#define PHY_VRFTRES_HVREF_MASK      0x3f  /* [5:0] */
#define PHY_VRFTRES_RXDIFFCAL_MASK  0xf   /* [27:24] */
#define PHY_ADDRPH_MASK             0x3f  /* [21:16] */
#define PHY_ACADDR_BDL_MASK         0x7f  /* [6:0] */
#define PHY_CATSWAPSEL_BIT_MASK     0xff
#define PHY_CAT_PATTERN_MASK        0xfff
#define PHY_TRAINCTRL0_MASK         0xf   /* [3:0] */
#define PHY_DRAMCFG_TYPE_MASK       0xf   /* [3:0] */
#define PHY_ACIOCTL21_MASK          0x7   /* [14:12], [10:8] */
#define PHY_VREFS_MRS_ENTER_MASK    0x1 /* [31] */
#define PHY_DXNRDBOUND_MASK         0x3ff /* [25:16], [9:0] */
#define PHY_OSC_START_MASK          0x1   /* [0] */
#define PHY_OSC_RPT_VLD_MASK        0x1   /* [15] */
#define PHY_OSC_CNT_RDATA_MASK      0xffff /* [31:16] */
#define PHY_ZCODE_PDRV_MASK         0x3f   /* [21:16] */
#define PHY_ACCTL_PDRV_LATCH_MASK   0x3f   /* [29:24] */
#define PHY_IMP_STATUS1_USED_MASK   0xffff /* [15:0]/[31:16] */
#define PHY_WLSL_MASK               0x3   /* [17:16] valid value:0/1/2 */
#define PHY_CLKGATED_MASK           0x80007fff /* bit[31], bit[14:0] */
#define PHY_PLL_PWDN_MASK           0x7   /* bit[2:0] pll_pwdn */
#define PHY_DFICLK_RATIO_MASK       0x3   /* [1:0] dficlk_ratio */
#define PHY_WL_FALLEDGE_BDL_JSTEP_R_MASK 0x7f /* [16:10] wl_falledge_bdl_jstep_r */
#define PHY_MISC_ADDR_DELAY_MASK    0x1 /* [18]Add 1-T delay on address/command */
#define PHY_MISC_CFG_WL_MASK        0xff /* [7:0]PHY write latency */
#define PHY_MISC_CFG_RL_MASK        0xff /* [15:8]PHY read latency */
#define PHY_DMSEL_TPHY_WRDATA_MASK  0x1  /* bit[8]tphy_wrdata */
#define PHY_TRFC_MPC_CMD_DLY_MASK   0x3  /* bit[2:1]trfc mpc command delay */
#define PHY_AC_IOCTL_TX_MODE_MASK   0x3  /* bit[13:12]ac_ioctl_tx_mode */
#define DDR_PHY_T_MOD_MASK          0x1f /* bit[8:4]t_mod */
#define DDR_PHY_RANKEN_MASK         0xffff /* rank_en:bit[15:0] */

/* register bit */
#define PHY_MISC_CFG_WL_BIT        0 /* [7:0]PHY write latency */
#define PHY_MISC_CFG_RL_BIT        8 /* [15:8]PHY read latency */
#define PHY_MISC_ADDR_DELAY_BIT    18 /* Add 1-T delay on address/command */
#define PHY_MISC_UPDATE_BIT   19  /* [CUSTOM] delay config update bit */
#define PHY_PHYCONN_RST_BIT   15  /* issue reset signal to PHY counter */
#define PHY_RST_BIT           13  /* bit[13] issue reset signal to PHY */
#define PHY_RDQSG_PHASE_BIT   9   /* [CUSTOM] */
#define PHY_RDQSG_TX_BDL_BIT  24  /* [30:24] rdqsgtxbdl */
#define PHY_WDQS_PHASE_BIT    8
#define PHY_WDQS_BDL_BIT      0
#define PHY_WDQ_PHASE_BIT     8
#define PHY_WDM_BDL_BIT       0
/* [22:16] Write DQS Output Enable Delay Control */
#define PHY_WDQSOE_BDL_BIT    16
#define PHY_OEN_BDL_BIT       0
/* Mode Register 1. Defines the MR3/MR9 of the mode register */
#define PHY_MODEREG01_MR1_BIT 16
/* Bit delay line setting of CS1 */
#define PHY_ACCMD_CS0_BIT          0
#define PHY_ACCMD_CS1_BIT          16
#define PHY_ACPHY_DCLK0_BIT        6  /* [8:6] cp1p_dclk0 */
#define PHY_ACPHY_DCLK1_BIT        9  /* [11:9] ck2p_dclk1 */
#define PHY_ACPHY_DRAMCLK0_BIT     25 /* [25] halft_dramclk0 */
#define PHY_ACPHY_DRAMCLK1_BIT     24 /* [24] halft_dramclk1 */
#define PHY_ACPHY_DRAMCLK_EXT_BIT  3  /* [3] halft_dramclk0 */
#define PHY_SWTMODE_SW_GTMODE_BIT  1  /* [1] SW gate training */
#define PHY_ACADDRBDL_ADDR1_BIT    16 /* [16] ADDR1 delay line */
#define PHY_VRFTRES_RXDIFFCAL_BIT  24 /* [27:24] */
#define PHY_BYPASS_CK0_BIT         16 /* [2]ck_ioctl_DUTY_EN reserver */
#define PHY_BYPASS_CK1_BIT         17 /* [3]ck1_ioctl_DUTY_EN reserver */
#define PHY_ACIOCTL21_CK0_BIT      8  /* [10:8] */
#define PHY_ACIOCTL21_CK1_BIT      12 /* [14:12] */
#define PHY_ACIOCTL21_CTL0_BIT     11 /* [11] */
#define PHY_ACIOCTL21_CTL1_BIT     15 /* [15] */
#define PHY_DXNRDBOUND_RIGHT_BIT   0  /* [9:0] */
#define PHY_DXNRDBOUND_LEFT_BIT    16 /* [25:16] */
#define PHY_VREFS_MRS_ENTER_BIT    31 /* [31] */
#define PHY_HRXDIFFCAL_EN_BIT      31 /* [31] */
#define PHY_OSC_RPT_VLD            15 /* [15] */
#define PHY_OSC_CNT_RDATA_BIT      16 /* [31:16] */
#define PHY_ZCODE_PDRV_BIT         16 /* [21:16] */
#define PHY_ACCTL_PDRV_LATCH_BIT   24 /* [29:24] */
#define PHY_AC_VDDQ_CAL_EN_BIT     8  /* [8] AC ZQ calibration enable */
#define PHY_WLSL_BIT               16 /* [17:16] wlsl */
#define PHY_IMP_STATUS1_USED_BIT   16 /* [15:0]/[31:16] */
#define PHY_RDET_METHOD_SEL_BIT    31 /* [31] rdet_method_sel */
#define PHY_NOPRE4WRDET_BIT        15 /* [15] nopre4wrdet */
#define PHY_WDET_METROD_SEL_BIT    23 /* [23] wdet_method_sel */
#define PHY_TRAINCTL_NOPOSTPRE_BIT 19 /* [19] trainctl_nopostpre */
#define PHY_PLL_PWDN_BIT           0  /* bit[2:0] pll_pwdn */
#define PHY_DXCTL_REG_TX_PHASE_RNK_BIT 14 /* dxctl_reg_tx_phase_rnk */
#define PHY_DFICLK_RATIO_BIT       0  /* [1:0] dficlk_ratio */
#define PHY_SWTRLT_GATE_BIT        8  /* [15:8] gt_result */
#define PHY_WL_FALLEDGE_BDL_JSTEP_R_BIT 10 /* [16:10] wl_falledge_bdl_jstep_r */
#define PHY_DMSEL_TPHY_WRDATA_BIT  8  /* bit[8]tphy_wrdata */
#define PHY_TRFC_MPC_CMD_DLY_BIT   1  /* bit[2:1]trfc mpc command delay */
#define PHY_AC_IOCTL_TX_MODE_BIT   12 /* bit[13:12]ac_ioctl_tx_mode */
#define DDR_PHY_T_MOD_BIT          4  /* bit[8:4]t_mod */
#define PHY_MODEREG67_LP4_FSPWR_BIT   6  /* bit[6] FSPWR */

/* BDL register bit */
#define PHY_BDL_DQ_BIT    0
#define PHY_BDL_DQ0_BIT   0
#define PHY_BDL_DQ1_BIT   8
#define PHY_BDL_DQ2_BIT   16
#define PHY_BDL_DQ3_BIT   24
#define PHY_RDM_BDL_BIT   0
#define PHY_RDQS_BDL_BIT  0
#define PHY_RDQS_CYC_BIT  16

/* value */
#define PHY_PHYINITCTRL_DVREFT_SYNC 0x40000 /* DRAM VREF Synchronize */
/* hw training item defined in PHYINITCTRL */
#define PHY_PHYINITCTRL_CTL_CKE_BYPASS      (1 << 31) /* PACK's CKE bypass function enable. */
#define PHY_PHYINITCTRL_PIC_PHYUPD_REQ      (1 << 30) /* PACK's DFI PHY UPDATAE request by SW. */
#define PHY_PHYINITCTRL_PIC_TDQSST          (1 << 28) /* TDQSS training Enable. */
#define PHY_PHYINITCTRL_CFG_LPBK_COMPST_EN  (1 << 27) /* RDQS/CK loopback delay compensate enable. */
#define PHY_PHYINITCTRL_PIC_REFRET_SFT      (1 << 26) /* Update delay line(switch op_sel) during tRFC. */
#define PHY_PHYINITCTRL_PIC_REFRET_WR       (1 << 25) /* Retraining with MPC write during tRFC. */
#define PHY_PHYINITCTRL_PIC_REFRET_RD       (1 << 24) /* Retraining with MPC read during tRFC. */
#define PHY_PHYINITCTRL_JTMT_EN             (1 << 23) /* PLL Jitter Meter Enable. */
#define PHY_PHYINITCTRL_CST_EN              (1 << 22) /* HW CS Traninig Enable. */
#define PHY_PHYINITCTRL_ACDVREFS_EN         (1 << 21) /* DRAM VREF(AC) Synchronize Operations. */
#define PHY_PHYINITCTRL_ACHVREFT_EN         (1 << 20) /* Host VREF(AC) Training Enable. */
#define PHY_PHYINITCTRL_ACDVREFT_EN         (1 << 19) /* DRAM VREF(AC) Training Enable. */
#define PHY_PHYINITCTRL_DXDVREFS_EN         (1 << 18) /* DRAM VREF(DQ) Synchronize Operations. */
#define PHY_PHYINITCTRL_HVREFT_EN           (1 << 17) /* Host VREF(DQ) Training Enable. */
#define PHY_PHYINITCTRL_DVREFT_EN           (1 << 16) /* DRAM VREF(DQ) Training Enable. */
#define PHY_PHYINITCTRL_PHYCONN_RST         (1 << 15) /* PHY Counter Reset. */
#define PHY_PHYINITCTRL_PACK_RST            (1 << 14) /* PACK Reset. */
#define PHY_PHYINITCTRL_PHY_RST             (1 << 13) /* PHY Reset. */
#define PHY_PHYINITCTRL_DRAM_RST            (1 << 12) /* DRAM Reset. */
#define PHY_PHYINITCTRL_CAT_EN              (1 << 11) /* HW CA Traninig Enable. */
#define PHY_PHYINITCTRL_DRAM_INIT_EN        (1 << 10) /* DRAM Initialization Enable. */
#define PHY_PHYINITCTRL_WDET_EN             (1 << 9)  /* Write Data Eye Training Enable. */
#define PHY_PHYINITCTRL_RDET_EN             (1 << 8)  /* Read Data Eye Training Enable. */
#define PHY_PHYINITCTRL_WL2_EN              (1 << 7)  /* Second Write Leveling Enable. */
#define PHY_PHYINITCTRL_GDST_EN             (1 << 6)  /* PHY Read Data Latch Train Enable. */
#define PHY_PHYINITCTRL_GT_EN               (1 << 5)  /* Gate Training Enable. */
#define PHY_PHYINITCTRL_WL_EN               (1 << 4)  /* Write Leveling Enable. */
#define PHY_PHYINITCTRL_ZCAL_EN             (1 << 3)  /* Impedance Calibration Enable. */
#define PHY_PHYINITCTRL_DLYMEAS_EN          (1 << 2)  /* Delay Measurement Enable. */
#define PHY_PHYINITCTRL_PLL_INIT_EN         (1 << 1)  /* PLL Initialization Enable. */
#define PHY_PHYINITCTRL_INIT_EN             (1 << 0)  /* PHY Initialization Enable. */

#define PHY_HW_GP_PHY_RESET        (PHY_PHYINITCTRL_PHY_RST)
#define PHY_HW_GP_CNT_RESET_START  (PHY_PHYINITCTRL_PHYCONN_RST)
#define PHY_HW_GP_PLL              (PHY_PHYINITCTRL_PLL_INIT_EN | PHY_PHYINITCTRL_ZCAL_EN | PHY_PHYINITCTRL_DLYMEAS_EN)
#define PHY_HW_GP_DRAM_RESET       (PHY_PHYINITCTRL_DRAM_RST | PHY_PHYINITCTRL_DRAM_INIT_EN)
#define PHY_HW_GP_VREF_AC          (PHY_PHYINITCTRL_ACDVREFS_EN)
#define PHY_HW_GP_CS               (PHY_PHYINITCTRL_CST_EN)
#define PHY_HW_GP_VREF_DQ          (PHY_PHYINITCTRL_DVREFT_SYNC)
#define PHY_HW_GP_NORMAL           (PHY_PHYINITCTRL_WL_EN | \
                    PHY_PHYINITCTRL_GT_EN | \
                    PHY_PHYINITCTRL_GDST_EN | \
                    PHY_PHYINITCTRL_WL2_EN | \
                    PHY_PHYINITCTRL_RDET_EN | \
                    PHY_PHYINITCTRL_WDET_EN | \
                    PHY_PHYINITCTRL_DVREFT_EN | \
                    PHY_PHYINITCTRL_HVREFT_EN | \
                    PHY_PHYINITCTRL_PIC_TDQSST)
#define PHY_HW_GP_NORMAL_RANK1     (PHY_PHYINITCTRL_GT_EN | \
                    PHY_PHYINITCTRL_GDST_EN | \
                    PHY_PHYINITCTRL_WL2_EN | \
                    PHY_PHYINITCTRL_RDET_EN | \
                    PHY_PHYINITCTRL_WDET_EN)
#define PHY_HW_GP_CNT_RESET_END    (PHY_PHYINITCTRL_PHYCONN_RST)

#define PHY_PHYINITSTATUS_ZCAL_ERROR        (1 << 3)  /* Impedance Calibration Error. */

/* RDQS range[0, 0x7f], middle value is 0x40, but it affected by
   temperature, so middle value change to 0x30 */
#define PHY_RDQS_MIDDLE_VAL     0x40
/* DQ range[0, 0x7f],  middle value is 0x40, but it affected by
   temperature, so middle value change to 0x30 */
#define PHY_DQ_MIDDLE_VAL       0x40404040
#define PHY_MISC_SCRAMB_DIS     0xfffeffff /* scrambler disable */
#define PHY_GATE_BDL_MAX        0xfe /* [6:0]rdqsg_bdl + [22:16]rdqsgtxbdl */
#define PHY_DVRFTCTRL_PDAEN_EN  0x80000000 /* pda enable */
/* [5] two cycle on address or command.(2T timing) */
#define PHY_DRAMCFG_MA2T        0x20

#define PHY_DRAMCFG_TYPE_DDR1   0x0  /* [2:0] 000 DDR1  */
#define PHY_DRAMCFG_TYPE_DDR2   0x1  /* [2:0] 001 DDR2 */
#define PHY_DRAMCFG_TYPE_DDR3   0x2  /* [2:0] 010 DDR3 */
#define PHY_DRAMCFG_TYPE_DDR3L  0x3  /* [2:0] 011 DDR3L */
#define PHY_DRAMCFG_TYPE_LPDDR1 0x4  /* [2:0] 100 LPDDR1 */
#define PHY_DRAMCFG_TYPE_LPDDR2 0x5  /* [2:0] 101 LPDDR2 */
#define PHY_DRAMCFG_TYPE_LPDDR3 0x5  /* [2:0] 101 LPDDR3 */
#define PHY_DRAMCFG_TYPE_LPDDR4 0x6  /* [2:0] 110 LPDDR4 */
#define PHY_DRAMCFG_TYPE_DDR4   0xa  /* [3] 1010 DDR4 */

#define DDR_PHY_LPDDR4X_MODE    0x0  /* LPDDR4X */

#define PHY_DMSEL_SWAPDFIBYTE   0xf8ffffff /* [24:26] No Swap */

/* AC_DDRPHY_GATED_BYPASS */
#define PHY_CK_IOCTL_DUTY_EN    0x0  /* enable ck_ioctl_DUTY_EN_v */
#define PHY_CK1_IOCTL_DUTY_EN   0x0  /* enable ck1_ioctl_DUTY_EN_v */
/* CK AC_IOCTL22 */
#define DDR_DUTY_NUM            4 /* CK duty number */
#define DDR_CK_MAX_NUM          2 /* DDR CK max number */
#define DDR_CK_NUM_LPDDR4       2 /* LPDDR4 CK number */
#define DDR_CK_NUM_NONLPDDR4    1 /* NONLPDDR4 CK number */
#define DDR_DUTY_CTL_NUM        2 /* CK duty has two control direction */
/* CK duty step. */
#define PHY_AC_IOCTL21_STEP     1
#define DDR_DCC_CTL_WIN_DIFF    2

#define PHY_CLK_GATED_CLOSE   0x0
#define PHY_CLK_GATED_OPEN    0x80007fff
#define PHY_PLL_POWER_DOWN    0x7
#define PHY_PLL_POWER_UP      0x0

/* other */
#define PHY_RDQSG_PHASE_STEP       2     /* gate training phase step. */
#define PHY_GATE_PHASE_MARGIN      8     /* gate phase margin */
#define PHY_DQ_BDL_LEVEL           128   /* [CUSTOM] DQ BDL range */
#define PHY_DQ_BDL_MIDDLE          64    /* special middle DQ BDL value */
#define PHY_RDQSG_PHASE_MAX        0xff  /* RDQSG phase max value */
#define PHY_ACPHY_CLK_MAX          0xf   /* halft_dramclk0 + cp1p_dclk0 */
#define PHY_PCODE_MIN              0x14
#define PHY_PCODE_MAX              0x24
#define PHY_WLSL_MAX               0x2   /* valid wlsl value:0/1/2 */
#define PHY_WDQPHASE_REG_MAX       0x3e  /* 0x3f - 1 */
#define PHY_WDQPHASE_NUM_T         12    /* wdqphase num of 1T */
#define PHY_WDQPHASE_REG_NUM_T     16    /* wdqphase reg num of 1T */
#define PHY_WDQSPHASE_REG_MIN      0x0
#define PHY_WDQSPHASE_REG_MAX      0xe   /* 0xf - 1 */
#define PHY_WDQSPHASE_NUM_T        12    /* wdqsphase num of 1T */
#define PHY_WDQSPHASE_REG_NUM_T    16    /* wdqsphase reg num of 1T */

/*
 * DDR_BDL_PHASE_REL Calculation Method:
 * 1. Calculation How many picosecond to one phase.
 *    PICOSECOND : 1 second is (1000 * 1000 * 1000) picosecond
 *    WAVE       : 1 cycle is 2
 *    RATE       : DDR rate is 1600 Mbps, is (1600 * 1000) bps
 *    PHASE      : 1 wave is 12 phase
 *    phase equal (((PICOSECOND * WAVE) / RATE) / PHASE)
 *          = (((1000 * 1000 * 1000 * 2) / (1600 * 1000)) / 12)
 *          = 104.17 ps.
 * 2. Calculation How many bdl to one phase.
 *    one BDL is 6 ps.
 *    result = phase/bdl = 104.17 / 6 = 17.36 approximately equal to 17 ~= 16
 * 3. 16 = 1 << 4, so the relation is 4.
 */
#ifndef DDR_BDL_PHASE_TRANSFORM
/* [CUSTOM] one Phase equal how much BDL. 1 phase = 16 bdl */
#define DDR_BDL_PHASE_TRANSFORM  16
#endif
#ifndef DDR_BDL_PHASE_REL
/* [CUSTOM] relation between BDL and Phase. 1 phase = 16 bdl, 16 = 1 << 4 */
#define DDR_BDL_PHASE_REL  4
#endif

#define ddr_variable_declare(var) \
    unsigned int var

#define ddr_vref_get_host_max(rank, val) do { \
    if ((rank) == 0) \
        (val) = PHY_VRFTRES_HVREF_MASK; \
    else \
        (val) = PHY_VRFTRES_RXDIFFCAL_MASK; \
} while (0)

#define ddr_phy_vref_host_set(base_phy, rank, bytenum, byte_index, val) \
    ddr_phy_vref_host_set_process(base_phy, rank, bytenum, byte_index, val)

#define ddr_phy_vref_host_get(base_phy, rank, byte_index, val) do { \
    if ((rank) == 0) { \
        (val) = reg_read((base_phy) + ddr_phy_hvreft_status(rank, byte_index)) & \
            PHY_VRFTRES_HVREF_MASK; \
    } else { \
        (val) = (reg_read((base_phy) + ddr_phy_hvreft_status(rank, byte_index)) >> PHY_VRFTRES_RXDIFFCAL_BIT) & \
            PHY_VRFTRES_RXDIFFCAL_MASK; \
    } \
} while (0)

#define DDR_PHY_VREF_HOST_DISPLAY \
    {0, 0, ddr_phy_hvreft_status(0, 0), 0, "Host Vref Byte0"}, \
    {0, 1, ddr_phy_hvreft_status(0, 1), 0, "Host Vref Byte1"}, \
    {0, 2, ddr_phy_hvreft_status(0, 2), 0, "Host Vref Byte2"}, \
    {0, 3, ddr_phy_hvreft_status(0, 3), 0, "Host Vref Byte3"},

#define DDR_PHY_VREF_HOST_DISPLAY_RANK1 \
    {1, 0, ddr_phy_hvreft_status(1, 0), 0, "Host Vref Byte0"}, \
    {1, 1, ddr_phy_hvreft_status(1, 1), 0, "Host Vref Byte1"}, \
    {1, 2, ddr_phy_hvreft_status(1, 2), 0, "Host Vref Byte2"}, \
    {1, 3, ddr_phy_hvreft_status(1, 3), 0, "Host Vref Byte3"},

#define ddr_phy_vref_host_display_cmd(base_phy, rank, byte_num) do { \
    unsigned int _i; \
    for (_i = 0; _i < (byte_num); _i++) { \
        ddr_info("[%x = %x] Host Vref Byte(%x)", \
            (base_phy) + ddr_phy_hvreft_status(rank, _i), \
            reg_read((base_phy) + ddr_phy_hvreft_status(rank, _i)), _i); \
    } \
} while (0)

/* DRAM vref operations */
#define ddr_phy_vref_dram_set(base_phy, val, byte_index) \
    ddr_vref_dram_set_process(base_phy, val, byte_index)

#define ddr_phy_vref_dram_get(base_phy, val, byte_index) do { \
    (val) = reg_read((base_phy) + ddr_phy_dvreft_status(byte_index)) & \
        PHY_VRFTRES_DVREF_MASK; \
} while (0)

#define DDR_PHY_VREF_DRAM_DISPLAY \
    {0, 0, ddr_phy_dvreft_status(0), 0, "DRAM Vref Byte0"}, \
    {0, 1, ddr_phy_dvreft_status(1), 0, "DRAM Vref Byte1"}, \
    {0, 2, ddr_phy_dvreft_status(2), 0, "DRAM Vref Byte2"}, \
    {0, 3, ddr_phy_dvreft_status(3), 0, "DRAM Vref Byte3"},

#define ddr_phy_vref_dram_display_cmd(base_phy, byte_num) do { \
    unsigned int _i; \
    for (_i = 0; _i < (byte_num); _i++) { \
        ddr_info("[%x = %x] DRAM Vref Byte(%x)", \
            (base_phy) + ddr_phy_dvreft_status(_i), \
            reg_read((base_phy) + ddr_phy_dvreft_status(_i)), _i); \
    } \
} while (0)

/* Dx dpmc operations */
#define DDR_DX_DPMC_DISPLAY \
    {0, 0, dx_dxnmiscctrl3(0), 0, "Dpmc Byte0"}, \
    {0, 1, dx_dxnmiscctrl3(1), 0, "Dpmc Byte1"}, \
    {0, 2, dx_dxnmiscctrl3(2), 0, "Dpmc Byte2"}, \
    {0, 3, dx_dxnmiscctrl3(3), 0, "Dpmc Byte3"},

#define ddr_dx_dpmc_display_cmd(base_phy, byte_num) do { \
    unsigned int _i; \
    for (_i = 0; _i < (byte_num); _i++) { \
        ddr_info("[%x = %x] Dpmc Byte(%x)", \
            (base_phy) + dx_dxnmiscctrl3(_i), \
            reg_read((base_phy) + dx_dxnmiscctrl3(_i)), _i); \
    } \
} while (0)

/* phy t28 not support DCC training */
#define DDR_PHY_DCC_DISPLAY \
    {0, 0, DDR_PHY_ACIOCTL21, 0, "CK DUTY"},

#define ddr_phy_dcc_display_cmd(base_phy) do { \
    unsigned int val; \
    val = reg_read((base_phy) + DDR_PHY_ACIOCTL21); \
    val = reg_read((base_phy) + DDR_PHY_ACIOCTL21); \
    ddr_info("[%x = %x] DCC duty", \
        (base_phy) + DDR_PHY_ACIOCTL21, val); \
} while (0)

/* lowpower ddr ca operations */
#define DDR_PHY_ADDRPH_DISPLAY \
    {0, 0, DDR_PHY_ADDRPHBOUND, 0, "CA Phase"},

#define DDR_PHY_ADDRBDL_DISPLAY \
    {0, 0, ddr_phy_acaddrbdl(0), 0, "CA BDL(0)"}, \
    {0, 0, ddr_phy_acaddrbdl(1), 0, "CA BDL(1)"}, \
    {0, 0, ddr_phy_acaddrbdl(2), 0, "CA BDL(2)"}, \
    {0, 0, ddr_phy_acaddrbdl(3), 0, "CA BDL(3)"}, \
    {0, 0, ddr_phy_acaddrbdl(4), 0, "CA BDL(4)"}, \
    {0, 0, ddr_phy_acaddrbdl(6), 0, "CA BDL(5)"},

#define ddr_phy_addrph_display_cmd(base_phy) do { \
    ddr_info("[%x = %x] CA Phase", (base_phy) + DDR_PHY_ADDRPHBOUND, \
        reg_read((base_phy) + DDR_PHY_ADDRPHBOUND)); \
} while (0)

#define ddr_phy_addrbdl_display_cmd(base_phy) do { \
    unsigned int _i; \
    for (_i = 0; _i < DDR_PHY_CA_REG_MAX; _i++) { \
        ddr_info("[%x = %x] ACADDRBDL(%x)", \
            (base_phy) + ddr_phy_acaddrbdl(_i), \
            reg_read((base_phy) + ddr_phy_acaddrbdl(_i)), _i); \
    } \
} while (0)

/* PHY t28 DDR4 RDQS synchronize to RDM */
#define ddr_phy_rdqs_sync_rdm(cfg, val) \
    ddr_rdqs_sync(cfg, val)

/* dqs swap */
#define ddr_dqsswap_save_func(swapdfibyte_en, base_phy) do { \
    (swapdfibyte_en) = reg_read((base_phy) + DDR_PHY_DMSEL); \
    reg_write((swapdfibyte_en) & PHY_DMSEL_SWAPDFIBYTE, \
        (base_phy) + DDR_PHY_DMSEL); \
} while (0)

#define ddr_dqsswap_restore_func(swapdfibyte_en, base_phy) \
    do { \
        reg_write(swapdfibyte_en, (base_phy) + DDR_PHY_DMSEL); \
    } while (0)

#define ddr_phy_switch_rank(base_phy, val) do { \
    reg_write((reg_read((base_phy) + DDR_PHY_TRAINCTRL0) & (~PHY_TRAINCTRL0_MASK)) | (val), \
        (base_phy) + DDR_PHY_TRAINCTRL0); \
    reg_write((reg_read((base_phy) + DDR_PHY_HVRFTCTRL) & (~(0x1 << PHY_HRXDIFFCAL_EN_BIT))) | \
        ((val) << PHY_HRXDIFFCAL_EN_BIT), (base_phy) + DDR_PHY_HVRFTCTRL); \
} while (0)

/* Define the union u_phy_catconfig */
union u_phy_catconfig {
    /* Define the struct bits */
    struct {
        unsigned int ca_samp_num_bdl : 4;   /* [3:0] */
        unsigned int ca_samp_num_ph : 4;    /* [7:4] */
        unsigned int ca_trysamp_num : 4;    /* [11:8] */
        unsigned int cat_rb_backtap : 4;    /* [15:12] */
        unsigned int reserved : 1;          /* [16] */
        unsigned int cat_openeye_en : 1;    /* [17] */
        unsigned int cat_cat_phydq_sel : 1; /* [18] */
        unsigned int cat_restore_en : 1;    /* [19] */
        unsigned int cat_lb_backtap : 4;    /* [23:20] */
        unsigned int sw_cat_mrw42 : 1;      /* [24] */
        unsigned int sw_cat_mrw48 : 1;      /* [25] */
        unsigned int sw_cat_mrw41 : 1;      /* [26] */
        unsigned int sw_cat_strobe : 1;     /* [27] */
        unsigned int sw_cat_cke_high : 1;   /* [28] */
        unsigned int sw_cat_cke_low : 1;    /* [29] */
        unsigned int sw_cat_dqvalid : 1;    /* [30] */
        unsigned int sw_cat_en : 1;         /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
};

/* Define the union u_phy_addrphbound */
union u_phy_addrphbound {
    /* Define the struct bits */
    struct {
        unsigned int addrph_a_right : 5; /* [4:0] */
        unsigned int reserved0 : 3;      /* [7:5] */
        unsigned int addrph_a_left : 5;  /* [12:8] */
        unsigned int reserved1 : 3;      /* [15:13] */
        unsigned int addrph_a : 5;       /* [20:16] */
        unsigned int reserved2 : 3;      /* [23:21] */
        unsigned int addrph_a_ori : 5;   /* [28:24] */
        unsigned int reserved3 : 3;      /* [31:29] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
};
#endif /* DDR_PHY_S14_H */
