/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_PHY_H_BODY_PART01_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_PHY_H_BODY_PART01_H_


#define DDR_PHY_C28_V200_CONFIG
/* register offset address */
/* base address: DDR_REG_BASE_PHY0 DDR_REG_BASE_PHY1 */
/* control the initialization of the PHY */
#define DDR_PHY_PHYINITCTRL    0x4
#define DDR_PHY_PHYINITSTATUS  0x8  /* Read Data Eye Calibration Error */
#define DDR_PHY_CLKGATED       0xc  /* This register control the clock gated of PHY */
#define DDR_PHY_PLLCTRL        0x18 /* specified the timing paramters for PLL in both address/command, and data block */
#define DDR_PHY_IMPSTATUS      0x28 /* This register specify the ZQ calibration result. */
#define DDR_PHY_DRAMCFG        0x2c /* DRAM config register */
typedef union {
    struct {
        unsigned int dram_type : 3;  /* [2:0] */
        unsigned int reserved1 : 2;  /* [4:3] */
        unsigned int ma2t : 1;       /* [5] */
        unsigned int reserved2 : 26; /* [31:6] */
    } bits;
    unsigned int val;
} phy_dramcfg_desc;

#define DDR_PHY_DRAMTIMER1     0x34 /* This register specify the DRAM timing parameters */
#define DDR_PHY_TRAINCTRL0     0x48 /* hw training control */
#define DDR_PHY_RANKEN         0x4c /* This register specify the training rank control */
#define DDR_PHY_TRAINMADDR     0x50 /* This register specify starting address where be used to do data training */
typedef union {
    struct {
        unsigned int dtbank : 3;
        unsigned int reserved1 : 13;
        unsigned int dtrow_low : 12;
        unsigned int dtrow_high : 3;
        unsigned int reserved2 : 1;
    } bits;
    unsigned int val;
} phy_trainmaddr_desc;
#define DDR_PHY_MODEREG01      0x64 /* Extend Mode Register 01 */
#define DDR_PHY_MODEREG23      0x68 /* Extend Mode Register 23 */
/* update delay setting in registers to PHY */
#define DDR_PHY_MISC           0x70
#define DDR_PHY_RNK2RNK        0x74
typedef union {
    struct {
        unsigned int reserved1 : 22;
        unsigned int dfi2iomap_rnksel : 1;
        unsigned int reserved2 : 9;
    } bits;
    unsigned int val;
} phy_rnk2rnk_desc;
#define DDR_PHY_PHYCTRL0       0x78 /* Register in this field are connected to PHY interface directly */
#define DDR_PHY_DMSEL          0x84 /* DM Swap Selection */
#define DDR_PHY_PLLCTRL_AC     0x98 /* PHY PLL control registers */
#define DDR_PHY_SWTMODE        0xa0 /* S/W training mode */
/* issue one DQS pulse from PHY to DRAM */
#define DDR_PHY_SWTWLDQS       0xa4
#define DDR_PHY_SWTRLT         0xa8 /* S/W training result */
/* Host vref. [5:0]range [17:12]refsel */
#define DDR_PHY_PHYRSCTRL      0xB0 /* PHY Register Slice Contrl */
#define DDR_CFG_RX_AGE_COMPST_EN_BIT 31 /* bit[31] cfg_rx_age_compst_en */
#define DDR_PHY_VREFTCTRL      0xc0 /* VREF Training Control Register. */
#define DDR_PHY_HVRFTCTRL      0xc8 /* Host VREF Training Control Regiser. */
typedef union {
    struct {
        unsigned int reserved1 : 15;
        unsigned int cfg_perbit_hvreft_en : 1;
        unsigned int reserved2 : 16;
    } bits;
    unsigned int val;
} phy_hvrftctrl_desc;
#define DDR_PHY_DVRFTCTRL      0xC4 /* DRAM VREF Training */
typedef union {
    struct {
        unsigned int reserved1 : 30;
        unsigned int cfg_perbit_dvreft_en : 1;
        unsigned int reserved2 : 1;
    } bits;
    unsigned int val;
} phy_dvrftctrl_desc;
#define DDR_PHY_TRAINCTRL2     0xd4 /* This register control the data training */
#define DDR_PHY_TRAINCTRL3     0xdc /* This register control the data training */
#define DDR_PHY_MODEREG45      0xe0  /* defines the contents of the Mode Register */
#define DDR_PHY_MODEREG67      0xe4  /* This register defines the contents of the Mode Register */
#define DDR_PHY_TRAINCTRL5     0x118 /* This register control the data training */
#define DDR_PHY_ACCMDBDL2      0x128 /* AC command bit delay line setting */

#define ddr_phy_acaddrbdl(val) (0x140 + ((val) << 2))

#define DDR_PHY_IMP_CTRL1      0x170 /* AC/DX ZQ selection */
#define DDR_PHY_IMP_STATUS1    0x174 /* AC ZCAL status */
#define DDR_PHY_CATSWAPINDEX   0x01B8 /* CA SWAP index register */
#define DDR_PHY_CATSWAPSEL     0x01BC /* CA SWAP select register */
#define DDR_PHY_CATCONFIG      0x1C8  /* CA Training Configuration */
#define DDR_PHY_PHYDQRESULT    0x1D0  /* SW CA Training DQ result from PHY */
#define DDR_PHY_ADDRPHBOUND    0x1D4  /* CA Training addr phase boundary */
#define DDR_PHY_SWCATPATTERN_P (0x1D0 + 0x08)  /* pattern for positive CK edge */
#define DDR_PHY_SWCATPATTERN_N 0x1DC  /* pattern for negative CK edge */
#define DDR_PHY_MRS_SEQ_PROG   0x1e0  /* Programmed MRS sequence in the DRAM initialization */
#define DDR_PHY_TRAINCTRL12    0x1e8  /* TRAINCTRL 12 Register */
#define DDR_PHY_TRFC_THRESHOLD1 0x8d0 /* trfc retrain/tracking threshold setting 1 */
#define DDR_PHY_TRFC_CTRL       0x8e0 /* specify the trfc retrain/tracking control */
#define DDR_TRFC_EN_MASK 0x1 /* bit[0] trfc_en */
#define DDR_TRFC_EN_BIT  0   /* bit[0] trfc_en */

#define DDR_PHY_RANK_SHIFT 10
#define DDR_PHY_BYTE_SHIFT 7
#define DDR_PHY_BLOCK             0x100
#define DDR_PHY_STATIC_REG_OFFSET 0x1000

/* WR DQ0-DQ3 [6:0] [14:8] [22:16] [30:24] delay value of the bit delay line on write path */
#define ddr_hal_phy_dxnwdqnbdl0(rank, byte) \
    (0x210 + ((rank) << DDR_PHY_RANK_SHIFT) + ((byte) << DDR_PHY_BYTE_SHIFT))
/* WR DQ4-DQ7 [6:0] [14:8] [22:16] [30:24] */
#define ddr_hal_phy_dxnwdqnbdl1(rank, byte) \
    (0x214 + ((rank) << DDR_PHY_RANK_SHIFT) + ((byte) << DDR_PHY_BYTE_SHIFT))
/* WR DM [6:0] the delay value of the bit delay line on DQM */
#define ddr_hal_phy_dxnwdqnbdl2(rank, byte) \
    (0x218 + ((rank) << DDR_PHY_RANK_SHIFT) + ((byte) << DDR_PHY_BYTE_SHIFT))
typedef union {
    struct {
        unsigned int wdmbdl : 7;    /* [6:0] */
        unsigned int reserved : 25; /* [31:7] */
    } bits;
    unsigned int val;
} ddr_phy_dxnwdqnbdl2_desc;

/* RD DQ0-DQ3 [6:0] [14:8] [22:16] [30:24] delay value of the bit delay line
 on read path */
#define ddr_hal_phy_dxnrdqnbdl0(rank, byte) \
    (0x21C + ((rank) << DDR_PHY_RANK_SHIFT) + ((byte) << DDR_PHY_BYTE_SHIFT))
/* RD DQ4-DQ7 [6:0] [14:8] [22:16] [30:24] delay value of the bit delay line
 on read path */
#define ddr_hal_phy_dxnrdqnbdl1(rank, byte) \
    (0x220 + ((rank) << DDR_PHY_RANK_SHIFT) + ((byte) << DDR_PHY_BYTE_SHIFT))
/* [6:0]RD DM */
#define ddr_hal_phy_dxnrdqnbdl2(rank, byte) \
    (0x224 + ((rank) << DDR_PHY_RANK_SHIFT) + ((byte) << DDR_PHY_BYTE_SHIFT))
typedef union {
    struct {
        unsigned int rdmbdl : 7; /* [6:0] */
        unsigned int reserved : 25; /* [31:7] */
    } bits;
    unsigned int val;
} ddr_phy_dxnrdqnbdl2_desc;

/* [CUSTOM] */
#define ddr_hal_phy_dxnoebdl(rank, byte) \
    (0x228 + ((rank) << DDR_PHY_RANK_SHIFT) + ((byte) << DDR_PHY_BYTE_SHIFT))
/* [8:0] rdqs_bdl [24:16]rdqs_cyc.
phase shift of the Read DQS to create 90 degree delays */
#define ddr_hal_phy_dxnrdqsdly(byte) (0x22C + ((byte) << DDR_PHY_BYTE_SHIFT))
typedef union {
    struct {
        unsigned int rdqsbdl : 9;          /* [8:0] */
        unsigned int tst_wdqsbdl_left : 7; /* [15:9] */
        unsigned int rdqscyc : 9;          /* [24:16] */
        unsigned int reserved : 7;         /* [31:25] */
    } bits;
    unsigned int val;
} ddr_phy_dxnrdqsdly_desc;

/* [6:0] the delay value of delay applied on WDQS for write leveling */
#define ddr_hal_phy_dxwdqsdly(rank, byte) \
    (0x230 + ((rank) << DDR_PHY_RANK_SHIFT) + ((byte) << DDR_PHY_BYTE_SHIFT))
/* WR DQ phase BIT 12:8 */
#define ddr_hal_phy_dxnwdqdly(rank, byte) \
    (0x234 + ((rank) << DDR_PHY_RANK_SHIFT) + ((byte) << DDR_PHY_BYTE_SHIFT))
typedef union {
    struct {
        unsigned int reserved1 : 8;  /* [7:0] */
        unsigned int wdqphase : 6;   /* [13:8] */
        unsigned int reserved2 : 18; /* [31:14] */
    } bits;
    unsigned int val;
} ddr_phy_dxnwdqdly_desc;

/* DXNWLSL:This reister is used to control if PHY controller add extra system
 * latency */
#define ddr_hal_phy_dxnwlsl(rank, byte) \
    (0x238 + ((rank) << DDR_PHY_RANK_SHIFT) + ((byte) << DDR_PHY_BYTE_SHIFT))
/* [CUSTOM] rdqs gating */
#define ddr_hal_phy_dxnrdqsgdly(rank, byte) \
    (0x240 + ((rank) << DDR_PHY_RANK_SHIFT) + ((byte) << DDR_PHY_BYTE_SHIFT))
/* read boundary  right 8:0 left 24:16 */
#define ddr_hal_phy_dxnrdbound(byte) (0x250 + ((byte) << DDR_PHY_BYTE_SHIFT))
typedef union {
    struct {
        unsigned int rdqsbdl_right : 10; /* [9:0] */
        unsigned int reserved1 : 6; /* [15:10] */
        unsigned int rdqsbdl_left : 10; /* [25:16] */
        unsigned int reserved2 : 6; /* [31:26] */
    } bits;
    unsigned int val;
} phy_dxnrdbound_desc;

/* write boundary  right 4:0 left 20:16 */
#define ddr_hal_phy_dxnwdbound(byte) (0x254 + ((byte) << DDR_PHY_BYTE_SHIFT))
/* [5:0] DRAM VREF(DQ) training result */
#define ddr_hal_phy_dvreft_status(byte) (0x270 + ((byte) << DDR_PHY_BYTE_SHIFT))
/* [4:0] Host PHY VREF(DQ) training result */
#define ddr_hal_phy_hvreft_status(rank, byte) \
    (0x274 + ((rank) << DDR_PHY_RANK_SHIFT) + ((byte) << DDR_PHY_BYTE_SHIFT))
/* This register specify the PHY dynamic tracking configure for DM/DQS */
#define ddr_hal_phy_dyn_dm_s(rank, byte)  (0x81c + ((rank) * 0x50) + ((byte) * 0x14))
typedef union {
    struct {
        unsigned int reserved1 : 20; /* [19:0] */
        unsigned int dqs_rdcc : 4; /* [23:20] */
        unsigned int reserved2 : 8; /* [31:24] */
    } bits;
    unsigned int val;
} phy_dyn_dm_s_desc;

/* DDRPHY AC static register */
/* #define DDR_PHY_CORNER_DETECTOR 0x104C  cfg of corner detector */
#define DDR_PHY_ACIOCTL         0x10c8 /* IO control register */
#define DDR_PHY_ACPHYCTRL0      0x1014 /* AC PHYCTRL0 */
#define DDR_PHY_ACPHYRSVDC      0x1078 /* AC PHYRSVDC(ck ldo) */
typedef union {
    struct {
        unsigned int pd : 1;
        unsigned int sel_reg0 : 1;
        unsigned int sel_reg1 : 1;
        unsigned int sel_reg2 : 1;
        unsigned int psw_en : 1;
        unsigned int reserved : 27;
    } bits;
    unsigned int val;
} phy_acphyrsvdc_desc;
#define ACPHYCTRL0_AC_OE_MASK   0xfff  /* [11:0] */
#define AC_OE_CK1OEN_DISABLE    0xff7  /* [11:0] disable acctl_ck1oen */
#define DDR_PHY_ACPHYCTL1       0x101c /* AC PHY control register */
#define DDR_PHY_ACPHYCTL4       0x1064 /* AC block PHY control register */
#define DDR_PHY_ACPHYCTL7       0x1070
#define DDR_PHY_ACIOCTL21       0x10A4
#define DDR_PHY_AC_GATED_BYPASS 0x10A4 /* bypass clock gated function */

/* DDRPHY DX static register */
static inline unsigned int ddr_phy_dxphyctrl2(unsigned int byte)
{
    return (0x1000 + 0x214 + (byte << 8)); /* dxphyctrl2 addr, 8 : byte shift */
}
static inline unsigned int ddr_phy_dxioctl2(unsigned int phy_block)
{
    return (0x121c + phy_block * 0x100);
}
typedef union {
    struct {
        unsigned int reserved1 : 17;
        unsigned int dxctl_rxdiffcal_sel : 1;
        unsigned int reserved2 : 14;
    } bits;
    unsigned int val;
} phy_ioctl2_desc;

static inline unsigned int ddr_phy_dxphyctrl(unsigned int byte)
{
    return (0x1000 + 0x234 + (byte << 8)); /* DX PHY control registers, 8 : byte shift */
}

static inline unsigned int ddr_phy_ioctl_rx_vcom_ctl0(unsigned int byte)
{
    return (0x1000 + 0x29c + (byte << 8)); /* DX PHY control registers, 8 : byte shift */
}
#define DUMMY0_IOCTL_OE_BIT  31 /* [31] IOCTL_RX_VCOM_CTL0 dummy0_ioctl_oe */

static inline unsigned int ddr_phy_ioctl_rx_vcom_ctl1(unsigned int byte)
{
    return (0x1000 + 0x2a0 + (byte << 8)); /* DX PHY control registers, 8 : byte shift */
}
#define DUMMY1_IOCTL_OE_BIT  31 /* [31] IOCTL_RX_VCOM_CTL1 dummy1_ioctl_oe */

/* Data block PHY debug/miscellaneous control register. dxctl_pre_margin_code [24:22] */
#define dx_dxnmiscctrl3(byte) (0x1254 + ((byte) << DDR_PHY_BYTE_SHIFT))
#define PHY_DXNMISCCTRL4  0x1258
static inline unsigned int ddr_phy_dxnmiscctrl4(unsigned int phy_block)
{
    return (PHY_DXNMISCCTRL4 + phy_block * DDR_PHY_BLOCK);
}
typedef union {
    struct {
        unsigned int reserved1 : 10;          /* [9:0] */
        unsigned int dxctl_clkgated_mclk : 1; /* [10] */
        unsigned int reserved2 : 21;          /* [31:11] */
    } bits;
    unsigned int val;
} ddr_phy_dxnmiscctrl4_desc;

#define DDR_VREF_HOST_VAL_MAX   0x4f /* 90%*VDDIO */
#define DDR_VREF_HOST_VAL_MIN   0x0  /* 40.00%*VDDIO */

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
#define PHY_INITSTATUS_GT_MASK      0x20
#define PHY_SWTRLT_WL_MASK          0xf
#define PHY_SWTRLT_GATE_MASK        0xf
#define PHY_PHYINITCTRL_MASK        0x1 /* [0] init_en */
/* Read Data Eye Calibration Error */
#define PHY_PHYINITSTATUS_RDET_ERR  0x100
#define PHY_ACPHY_DCLK_MASK         0x7     /* cp1p_dclk0 mask */
#define PHY_ACPHY_DRAMCLK_MASK      0x1     /* halft_dramclk0 mask */
#define PHY_VRFTRES_DVREF_MASK      0x3f    /* [5:0] */
#define PHY_VRFTRES_HVREF_MASK      0x7f    /* [6:0] */
#define PHY_VRFTRES_RXDIFFCAL_MASK  0xf     /* [27:24] */
#define PHY_ADDRPH_MASK             0x3f    /* [21:16] */
#define PHY_ACADDR_BDL_MASK         0x7f    /* [6:0] */
#define PHY_TRAINCTRL0_MASK         0xf     /* [3:0] */
#define PHY_DRAMCFG_TYPE_MASK       0xf     /* [3:0] */
#define PHY_ACIOCTL21_MASK          0x7     /* [14:12], [10:8] */
#define PHY_DXNRDBOUND_MASK         0x3ff   /* [25:16], [9:0] */
#define PHY_IMP_STATUS1_USED_MASK   0xffff  /* [15:0]/[31:16] */
#define PHY_WLSL_MASK               0x3     /* [17:16] valid value:0/1/2 */
#define PHY_CLKGATED_MASK           0x80007fff /* bit[31], bit[14:0] */
#define PHY_PLL_PWDN_MASK           0x7     /* bit[2:0] pll_pwdn */
#define PHY_DFICLK_RATIO_MASK       0x3     /* [1:0] dficlk_ratio */
#define PHY_WL_FALLEDGE_BDL_JSTEP_R_MASK 0x7f /* [16:10] wl_falledge_bdl_jstep_r */
#define PHY_MISC_ADDR_DELAY_MASK    0x1     /* [18]Add 1-T delay on address/command */
#define PHY_MISC_CFG_WL_MASK        0xff    /* [7:0]PHY write latency */
#define PHY_MISC_CFG_RL_MASK        0xff    /* [15:8]PHY read latency */
#define PHY_DMSEL_TPHY_WRDATA_MASK  0x1     /* bit[8]tphy_wrdata */
#define PHY_TRFC_MPC_CMD_DLY_MASK   0x3     /* bit[2:1]trfc mpc command delay */
#define PHY_AC_IOCTL_TX_MODE_MASK   0x3     /* bit[1:0]ac_ioctl_tx_mode */
#define PHY_ACCTL_PLL_SP_MASK       0x3 /* bit[24:23]acctl_pll_sp */
#define PHY_DXCTL_PLL_SP_MASK       0x3 /* bit[8:7]dxctl_pll_sp */
#define DDR_PHY_T_MOD_MASK          0x1f /* bit[8:4]t_mod */
#define DDR_PHY_RANKEN_MASK         0xffff /* rank_en:bit[15:0] */

/* register bit */
#define PHY_MISC_CFG_WL_BIT     0   /* [7:0]PHY write latency */
#define PHY_MISC_CFG_RL_BIT     8   /* [15:8]PHY read latency */
#define PHY_MISC_ADDR_DELAY_BIT 18  /* Add 1-T delay on address/command */
#define PHY_MISC_UPDATE_BIT     19  /* [CUSTOM] delay config update bit */
#define PHY_PHYCONN_RST_BIT     15  /* issue reset signal to PHY counter */
#define PHY_RST_BIT             13  /* bit[13] issue reset signal to PHY */
#define PHY_RDQSG_PHASE_BIT     9   /* [CUSTOM] */
#define PHY_RDQSG_TX_BDL_BIT    24  /* [30:24] rdqsgtxbdl */
#define PHY_WDQS_PHASE_BIT      8
#define PHY_WDQS_BDL_BIT        0
#define PHY_WDQ_PHASE_BIT       8
#define PHY_WDM_BDL_BIT         0
/* [22:16] Write DQS Output Enable Delay Control */
#define PHY_WDQSOE_BDL_BIT      16
#define PHY_OEN_BDL_BIT         0
/* Mode Register 1. Defines the MR3/MR9 of the mode register */
#define PHY_MODEREG01_MR1_BIT   16
/* Bit delay line setting of CS1 */
#define PHY_ACCMD_CS0_BIT         0
#define PHY_ACCMD_CS1_BIT         16
#define PHY_ACPHY_DCLK0_BIT       6   /* [8:6] cp1p_dclk0 */
#define PHY_ACPHY_DCLK1_BIT       9   /* [11:9] ck2p_dclk1 */
#define PHY_ACPHY_DRAMCLK0_BIT    25  /* [25] halft_dramclk0 */
#define PHY_ACPHY_DRAMCLK1_BIT    24  /* [24] halft_dramclk1 */
#define PHY_ACPHY_DRAMCLK_EXT_BIT 3   /* [3] move left 3 bit */
#define PHY_SWTMODE_SW_GTMODE_BIT 1   /* [1] SW gate training */
#define PHY_ACADDRBDL_ADDR1_BIT   16  /* [16] ADDR1 delay line */
#define PHY_VRFTRES_RXDIFFCAL_BIT 24  /* [27:24] */
#define PHY_BYPASS_CK0_BIT        24  /* [24]ck_ioctl_DUTY_EN reserver */
#define PHY_BYPASS_CK1_BIT        25  /* [25]ck1_ioctl_DUTY_EN reserver */
#define PHY_ACIOCTL21_CK0_BIT     8   /* [10:8] */
#define PHY_ACIOCTL21_CK1_BIT     12  /* [14:12] */
#define PHY_ACIOCTL21_CTL0_BIT    11  /* [11] */
#define PHY_ACIOCTL21_CTL1_BIT    15  /* [15] */
#define PHY_DXNRDBOUND_RIGHT_BIT  0   /* [9:0] */
#define PHY_DXNRDBOUND_LEFT_BIT   16  /* [25:16] */
#define PHY_VREFS_MRS_ENTER_BIT   31  /* [31] */
#define PHY_HRXDIFFCAL_EN_BIT     31  /* [31] */
#define PHY_AC_VDDQ_CAL_EN_BIT    8   /* [8] AC ZQ calibration enable */
#define PHY_WLSL_BIT              16  /* [17:16] wlsl */
#define PHY_IMP_STATUS1_USED_BIT  16  /* [15:0]/[31:16] */
#define PHY_RDET_METHOD_SEL_BIT   31  /* [31] rdet_method_sel */
#define PHY_NOPRE4WRDET_BIT       15  /* [15] nopre4wrdet */
#define PHY_WDET_METROD_SEL_BIT   23  /* [23] wdet_method_sel */
#define PHY_ACCTL_PLL_SP_BIT      23  /* bit[24:23]acctl_pll_sp */
#define PHY_DXCTL_PLL_SP_BIT      7   /* bit[8:7]dxctl_pll_sp */
#define PHY_TRAINCTL_NOPOSTPRE_BIT 19  /* [19] trainctl_nopostpre */
#define PHY_PLL_PWDN_BIT           0   /* bit[2:0] pll_pwdn */
#define PHY_DXCTL_REG_TX_PHASE_RNK_BIT  14  /* dxctl_reg_tx_phase_rnk */
#define PHY_DFICLK_RATIO_BIT            0   /* [1:0] dficlk_ratio */
#define PHY_PHYCTRL0_CMDOEN_BIT         7   /* [7] cmdoen */
#define PHY_SWTRLT_GATE_BIT             8   /* [15:8] gt_result */
#define PHY_WL_FALLEDGE_BDL_JSTEP_R_BIT 10  /* [16:10] wl_falledge_bdl_jstep_r */
#define PHY_DMSEL_TPHY_WRDATA_BIT       8   /* bit[8]tphy_wrdata */
#define PHY_TRFC_MPC_CMD_DLY_BIT        1   /* bit[2:1]trfc mpc command delay */
#define DDR_PHY_T_MOD_BIT               4   /* bit[8:4]t_mod */
#define PHY_MODEREG67_LP4_FSPWR_BIT     6   /* bit[6] FSPWR */
#define PHY_AC_IOCTL_TX_MODE_BIT        0   /* bit[1:0]ac_ioctl_tx_mode */
#define PHY_AC_PLL_LOCK_BIT             31  /* bit[31] ac_pll_lock */

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
#define PHY_PHYINITCTRL_PERBIT_HVREFT_EN    (1 << 31) /* Host VREF(DQ) perbit training enable. */
#define PHY_PHYINITCTRL_PERBIT_DVREFT_EN    (1 << 30) /* DRAM VREF(DQ) perbit training enable. */
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


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_PHY_H_BODY_PART01_H_ */
