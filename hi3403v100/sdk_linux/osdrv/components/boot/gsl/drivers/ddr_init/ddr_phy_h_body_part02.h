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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_PHY_H_BODY_PART02_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_PHY_H_BODY_PART02_H_

#define PHY_PHYINITCTRL_RDET_EN             (1 << 8)  /* Read Data Eye Training Enable. */
#define PHY_PHYINITCTRL_WL2_EN              (1 << 7)  /* Second Write Leveling Enable. */
#define PHY_PHYINITCTRL_GDST_EN             (1 << 6)  /* PHY Read Data Latch Train Enable. */
#define PHY_PHYINITCTRL_GT_EN               (1 << 5)  /* Gate Training Enable. */
#define PHY_PHYINITCTRL_WL_EN               (1 << 4)  /* Write Leveling Enable. */
#define PHY_PHYINITCTRL_ZCAL_EN             (1 << 3)  /* Impedance Calibration Enable. */
#define PHY_PHYINITCTRL_DLYMEAS_EN          (1 << 2)  /* Delay Measurement Enable. */
#define PHY_PHYINITCTRL_PLL_INIT_EN         (1 << 1)  /* PLL Initialization Enable. */
#define PHY_PHYINITCTRL_INIT_EN             (1 << 0)  /* PHY Initialization Enable. */

#define PHY_HW_GP_PHY_RESET (PHY_PHYINITCTRL_PHY_RST)
#define PHY_HW_GP_CNT_RESET_START (PHY_PHYINITCTRL_PHYCONN_RST)
#define PHY_HW_GP_PLL (PHY_PHYINITCTRL_PLL_INIT_EN | PHY_PHYINITCTRL_ZCAL_EN | \
    PHY_PHYINITCTRL_DLYMEAS_EN)
#define PHY_HW_GP_DRAM_RESET (PHY_PHYINITCTRL_DRAM_RST | PHY_PHYINITCTRL_DRAM_INIT_EN)
#define PHY_HW_GP_VREF_AC (PHY_PHYINITCTRL_ACDVREFS_EN)
#define PHY_HW_GP_CS (PHY_PHYINITCTRL_CST_EN)
#define PHY_HW_GP_VREF_DQ (PHY_PHYINITCTRL_DVREFT_SYNC)
#define PHY_HW_GP_NORMAL (PHY_PHYINITCTRL_GT_EN | PHY_PHYINITCTRL_GDST_EN | \
    PHY_PHYINITCTRL_WL2_EN | PHY_PHYINITCTRL_RDET_EN |                     \
    PHY_PHYINITCTRL_WDET_EN | PHY_PHYINITCTRL_HVREFT_EN | PHY_PHYINITCTRL_PIC_TDQSST)
#define PHY_HW_GP_NORMAL_RANK1 (PHY_PHYINITCTRL_GT_EN | PHY_PHYINITCTRL_GDST_EN | PHY_PHYINITCTRL_WL2_EN | \
    PHY_PHYINITCTRL_RDET_EN | PHY_PHYINITCTRL_WDET_EN)
#define PHY_HW_GP_CNT_RESET_END (PHY_PHYINITCTRL_PHYCONN_RST)

#define PHY_PHYINITSTATUS_ZCAL_ERROR (1 << 3) /* Impedance Calibration Error. */

/* RDQS range[0, 0x7f], middle value is 0x40, but it affected by
   temperature, so middle value change to 0x30 */
#define PHY_RDQS_MIDDLE_VAL 0x40
/* DQ range[0, 0x7f],  middle value is 0x40, but it affected by
   temperature, so middle value change to 0x30 */
#define PHY_DQ_MIDDLE_VAL      0x40404040
#define PHY_MISC_SCRAMB_DIS    0xfffeffff /* scrambler disable */
#define PHY_DVRFTCTRL_PDAEN_EN 0x80000000 /* pda enable */
/* [5] two cycle on address or command.(2T timing) */
#define PHY_DRAMCFG_MA2T       0x20

#define PHY_DRAMCFG_TYPE_DDR1   0x0 /* [2:0] 000 DDR1  */
#define PHY_DRAMCFG_TYPE_DDR2   0x1 /* [2:0] 001 DDR2 */
#define PHY_DRAMCFG_TYPE_DDR3   0x2 /* [2:0] 010 DDR3 */
#define PHY_DRAMCFG_TYPE_DDR3L  0x3 /* [2:0] 011 DDR3L */
#define PHY_DRAMCFG_TYPE_LPDDR1 0x4 /* [2:0] 100 LPDDR1 */
#define PHY_DRAMCFG_TYPE_LPDDR2 0x5 /* [2:0] 101 LPDDR2 */
#define PHY_DRAMCFG_TYPE_LPDDR3 0x5 /* [2:0] 101 LPDDR3 */
#define PHY_DRAMCFG_TYPE_LPDDR4 0x6 /* [2:0] 110 LPDDR4 */
#define PHY_DRAMCFG_TYPE_DDR4   0xa /* [3] 1010 DDR4 */

#define DDR_PHY_LPDDR4X_MODE    0x0 /* LPDDR4X */

#define PHY_DMSEL_SWAPDFIBYTE   0xf8ffffff /* [24:26] No Swap */

/* AC_DDRPHY_GATED_BYPASS */
#define PHY_CK_IOCTL_DUTY_EN  0x0  /* enable ck_ioctl_DUTY_EN_v */
#define PHY_CK1_IOCTL_DUTY_EN 0x0  /* enable ck1_ioctl_DUTY_EN_v */
/* CK AC_IOCTL22 */
#ifndef DDR_DUTY_NUM
#define DDR_DUTY_NUM 4 /* CK duty number */
#endif
#ifndef DDR_CK_NUM
#define DDR_CK_NUM   2 /* DDR CK number */
#define DDR_CK_NUM_LPDDR4       2 /* LPDDR4 CK number */
#define DDR_CK_NUM_NONLPDDR4    1 /* NONLPDDR4 CK number */
#endif
#ifndef DDR_DUTY_CTL_NUM
#define DDR_DUTY_CTL_NUM 2 /* CK duty has two control direction */
#endif
/* CK duty step. */
#ifndef PHY_AC_IOCTL21_STEP
#define PHY_AC_IOCTL21_STEP 1
#endif
#ifndef DDR_DCC_CTL_WIN_DIFF
#define DDR_DCC_CTL_WIN_DIFF 2
#endif

#define PHY_CLK_GATED_CLOSE 0x0
#define PHY_CLK_GATED_OPEN  0x80007fff
#define PHY_PLL_POWER_DOWN  0x7
#define PHY_PLL_POWER_UP    0x0
#define PHY_MRS_SEQ_PROG_VAL  0x05555555
#define PHY_WDM_DISABLE_VAL   0x00004000
#define PHY_ACCTL_PLL_SP_VAL  0x3 /* bit[24:23]acctl_pll_sp */
#define PHY_DXCTL_PLL_SP_VAL  0x3 /* bit[8:7]dxctl_pll_sp */
#define PHY_ACCTL_DCLK_NUM    2 /* acctl_ck1p_dclk0 acctl_ck2p_dclk1 */

/* other */
#define PHY_DQ_BDL_LEVEL        128  /* [CUSTOM] DQ BDL range */
#define PHY_DQ_BDL_MIDDLE       64   /* special middle DQ BDL value */
#define PHY_ACPHY_CLK_MAX       0xf  /* halft_dramclk0 + cp1p_dclk0 */
#define PHY_WLSL_MAX            0x2  /* valid wlsl value:0/1/2 */
#define PHY_WDQPHASE_REG_MAX    0x3e /* 0x3f - 1 */
#define CK_PHASE_MAX            0xf
#define PHY_WDQPHASE_NUM_T      12   /* wdqphase num of 1T */
#define PHY_WDQPHASE_REG_NUM_T  16   /* wdqphase reg num of 1T */
#define PHY_WDQSPHASE_REG_MIN   0x0
#define PHY_WDQSPHASE_REG_MAX   0xe  /* 0xf - 1 */
#define PHY_WDQSPHASE_NUM_T     12   /* wdqsphase num of 1T */
#define PHY_WDQSPHASE_REG_NUM_T 16   /* wdqsphase reg num of 1T */
#define PHY_WDQ_PHASE_MAX       PHY_WDQ_PHASE_MASK
#define PHY_RDM_BDL_MAX         PHY_RDM_BDL_MASK
#define PHY_WDM_BDL_MAX         PHY_WDM_BDL_MASK
#define PHY_WDQS_BDL_MAX        PHY_WDQS_BDL_MASK
#define PHY_BDL_MAX             PHY_BDL_MASK
#define PHY_RDQS_BDL_MAX        PHY_RDQS_BDL_MASK
#define NUM_FOR_REMAINING       2

/*
 * DDR_BDL_PHASE_REL Calculation Method:
 * 1. Calculation How many picosecond to one phase.
 * PICOSECOND : 1 second is (1000 * 1000 * 1000) picosecond
 * WAVE       : 1 cycle is 2
 * RATE       : DDR rate is 1600 Mbps, is (1600 * 1000) bps
 * PHASE      : 1 wave is 12 phase
 * phase equal (((PICOSECOND * WAVE) / RATE) / PHASE)
 * = (((1000 * 1000 * 1000 * 2) / (1600 * 1000)) / 12)
 * = 104.17 ps.
 * 2. Calculation How many bdl to one phase.
 * one BDL is 6 ps.
 * result = phase/bdl = 104.17 / 6 = 17.36 approximately equal to 17 ~= 16
 * 3. 16 = 1 << 4, so the relation is 4.
 */
#ifndef DDR_BDL_PHASE_TRANSFORM
/* [CUSTOM] one Phase equal how much BDL. 1 phase = 16 bdl */
#define DDR_BDL_PHASE_TRANSFORM 16
#endif
#ifndef DDR_BDL_PHASE_REL
/* [CUSTOM] relation between BDL and Phase. 1 phase = 16 bdl, 16 = 1 << 4 */
#define DDR_BDL_PHASE_REL 4
#endif

/* Define the union ddr_phy_cat_config */
union ddr_phy_cat_config {
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

/* Define the union ddr_phy_addr_phbound */
union ddr_phy_addr_phbound {
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

#define DDR_PHY_VREF_HOST_DISPLAY { 0, 0, ddr_hal_phy_hvreft_status(0, 0), 0, "Host Vref Byte0" },     \
        { 0, 1, ddr_hal_phy_hvreft_status(0, 1), 0, "Host Vref Byte1" }, \
        { 0, 2, ddr_hal_phy_hvreft_status(0, 2), 0, "Host Vref Byte2" }, \
        { 0, 3, ddr_hal_phy_hvreft_status(0, 3), 0, "Host Vref Byte3" },

#define DDR_PHY_VREF_HOST_DISPLAY_RANK1 { 1, 0, ddr_hal_phy_hvreft_status(1, 0), 0, "Host Vref Byte0" },     \
        { 1, 1, ddr_hal_phy_hvreft_status(1, 1), 0, "Host Vref Byte1" }, \
        { 1, 2, ddr_hal_phy_hvreft_status(1, 2), 0, "Host Vref Byte2" }, \
        { 1, 3, ddr_hal_phy_hvreft_status(1, 3), 0, "Host Vref Byte3" },

#define DDR_PHY_VREF_DRAM_DISPLAY { 0, 0, ddr_hal_phy_dvreft_status(0), 0, "DRAM Vref Byte0" },     \
        { 0, 1, ddr_hal_phy_dvreft_status(1), 0, "DRAM Vref Byte1" }, \
        { 0, 2, ddr_hal_phy_dvreft_status(2), 0, "DRAM Vref Byte2" }, \
        { 0, 3, ddr_hal_phy_dvreft_status(3), 0, "DRAM Vref Byte3" },

/* Dx dpmc operations */
#define DDR_DX_DPMC_DISPLAY \
    {0, 0, dx_dxnmiscctrl3(0), 0, "Dpmc Byte0"}, \
    {0, 1, dx_dxnmiscctrl3(1), 0, "Dpmc Byte1"}, \
    {0, 2, dx_dxnmiscctrl3(2), 0, "Dpmc Byte2"}, \
    {0, 3, dx_dxnmiscctrl3(3), 0, "Dpmc Byte3"},

/* phy t28 not support DCC training */
#define DDR_PHY_DCC_DISPLAY { 0, 0, DDR_PHY_ACIOCTL21, 0, "CK DUTY" },

/* lowpower ddr ca operations */
#define DDR_PHY_ADDRPH_DISPLAY { 0, 0, DDR_PHY_ADDRPHBOUND, 0, "CA Phase" },

#define DDR_PHY_ADDRBDL_DISPLAY { 0, 0, ddr_phy_acaddrbdl(0), 0, "CA BDL(0)" },     \
        { 0, 0, ddr_phy_acaddrbdl(1), 0, "CA BDL(1)" }, \
        { 0, 0, ddr_phy_acaddrbdl(2), 0, "CA BDL(2)" }, \
        { 0, 0, ddr_phy_acaddrbdl(3), 0, "CA BDL(3)" }, \
        { 0, 0, ddr_phy_acaddrbdl(4), 0, "CA BDL(4)" }, \
        { 0, 0, ddr_phy_acaddrbdl(6), 0, "CA BDL(5)" },


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_DDR_INIT_DDR_PHY_H_BODY_PART02_H_ */
