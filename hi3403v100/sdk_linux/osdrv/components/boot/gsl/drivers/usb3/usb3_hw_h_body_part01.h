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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_USB3_USB3_HW_H_BODY_PART01_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_USB3_USB3_HW_H_BODY_PART01_H_


#include "usb3.h"

/****************************************************************************/
/* Core Global Registers */

/**
 * This enum represents the bit fields of the Core SoC Bus Configuration 0
 * Register (GSBUSCFG0).
 */
typedef enum gsbuscfg0_data {
    /** Bus Burst Len			<i>Access: R_W</i>.
     * - 0:   single
     * - 1:   incr
     * - 3:   incr4
     * - 7:   incr8
     * - 15:  incr16
     * - 31:  incr32  (non-AHB mode only)
     * - 63:  incr64  (non-AHB mode only)
     * - 127: incr128 (non-AHB mode only)
     * - 255: incr256 (non-AHB mode only)
     */
    USB3_SBUSCFG0_HBURSTLEN_BITS		= 0x000000ff,
    USB3_SBUSCFG0_HBURSTLEN_SHIFT		= 0,

    USB3_SBUSCFG0_INT_DMA_BURST_SINGLE	= 0,
    USB3_SBUSCFG0_INT_DMA_BURST_INCR    = 1,
    USB3_SBUSCFG0_INT_DMA_BURST_INCR4	= 3,
    USB3_SBUSCFG0_INT_DMA_BURST_INCR8	= 7,
    USB3_SBUSCFG0_INT_DMA_BURST_INCR16	= 15,
    USB3_SBUSCFG0_INT_DMA_BURST_INCR32	= 31,
    USB3_SBUSCFG0_INT_DMA_BURST_INCR64	= 63,
    USB3_SBUSCFG0_INT_DMA_BURST_INCR128	= 127,
    USB3_SBUSCFG0_INT_DMA_BURST_INCR256	= 255,

    /** Descriptor Write is Posted		<i>Access: R_W</i> */
    USB3_SBUSCFG0_DES_WR_POST_BIT		= 0x00000100,
    USB3_SBUSCFG0_DES_WR_POST_SHIFT		= 8,

    /** Data Write is Posted		<i>Access: R_W</i> */
    USB3_SBUSCFG0_DAT_WR_POST_BIT		= 0x00000200,
    USB3_SBUSCFG0_DAT_WR_POST_SHIFT		= 9,

    /** Descriptor Access is Big-Endian	<i>Access: R_W</i> */
    USB3_SBUSCFG0_DES_BIG_END_BIT		= 0x00000400,
    USB3_SBUSCFG0_DES_BIG_END_SHIFT		= 10,

    /** Data Access is Big-Endian		<i>Access: R_W</i> */
    USB3_SBUSCFG0_DAT_BIG_END_BIT		= 0x00000800,
    USB3_SBUSCFG0_DAT_BIG_END_SHIFT		= 11,

    /** Store and Forward Mode		<i>Access: R_W</i> */
    USB3_SBUSCFG0_STORE_AND_FORWARD_BIT	    = 0x00001000,
    USB3_SBUSCFG0_STORE_AND_FORWARD_SHIFT	= 12,

    /** Force Single Request		<i>Access: R_W</i> */
    USB3_SBUSCFG0_SING_REQ_BIT		    = 0x00004000,
    USB3_SBUSCFG0_SING_REQ_SHIFT		= 14,

    /** Descriptor Readback Enable		<i>Access: R_W</i> */
    USB3_SBUSCFG0_READ_AFTER_WRITE_BIT	    = 0x00008000,
    USB3_SBUSCFG0_READ_AFTER_WRITE_SHIFT	= 15,

    /** Descriptor Write Request Info	<i>Access: R_W</i> */
    USB3_SBUSCFG0_DES_WR_REQ_INFO_BITS	= 0x000f0000,
    USB3_SBUSCFG0_DES_WR_REQ_INFO_SHIFT	= 16,

    /** Data Write Request Info		<i>Access: R_W</i> */
    USB3_SBUSCFG0_DAT_WR_REQ_INFO_BITS	= 0x00f00000,
    USB3_SBUSCFG0_DAT_WR_REQ_INFO_SHIFT	= 20,

    /** Descriptor Read Request Info	<i>Access: R_W</i> */
    USB3_SBUSCFG0_DES_RD_REQ_INFO_BITS	= 0x0f000000,
    USB3_SBUSCFG0_DES_RD_REQ_INFO_SHIFT	= 24,

    /** Data Read Request Info		<i>Access: R_W</i> */
    USB3_SBUSCFG0_DAT_RD_REQ_INFO_BITS	= 0xf0000000,
    USB3_SBUSCFG0_DAT_RD_REQ_INFO_SHIFT	= 28,
} gsbuscfg0_data_t;

/**
 * This enum represents the bit fields of the Core SoC Bus Configuration 1
 * Register (GSBUSCFG1).
 */
typedef enum gsbuscfg1_data {
    /** OCP Address Space For Descriptor	<i>Access: R_W</i> */
    USB3_SBUSCFG1_DES_ADDR_SPC_BITS		= 0x0000000f,
    USB3_SBUSCFG1_DES_ADDR_SPC_SHIFT		= 0,

    /** OCP Address Space For Data		<i>Access: R_W</i> */
    USB3_SBUSCFG1_DAT_ADDR_SPC_BITS		= 0x000000f0,
    USB3_SBUSCFG1_DAT_ADDR_SPC_SHIFT		= 4,
} gsbuscfg1_data_t;

/**
 * This enum represents the bit fields of the Core Tx Threshold Control
 * Register (GTXTHRCFG).
 */
typedef enum gtxthrcfg_data {
    /** SoC Bus Transmit Threshold Length		<i>Access: R_W</i> */
    USB3_TXTHRCFG_SBUS_THR_LEN_BITS		= 0x000007ff,
    USB3_TXTHRCFG_SBUS_THR_LEN_SHIFT		= 0,

    /** SoC Bus Non-ISO Transmit Threshold Enable	<i>Access: R_W</i> */
    USB3_TXTHRCFG_SBUS_NON_ISO_THR_EN_BIT	= 0x00004000,
    USB3_TXTHRCFG_SBUS_NON_ISO_THR_EN_SHIFT	= 14,

    /** SoC Bus ISO Transmit Threshold Enable	<i>Access: R_W</i> */
    USB3_TXTHRCFG_SBUS_ISO_THR_EN_BIT	= 0x00008000,
    USB3_TXTHRCFG_SBUS_ISO_THR_EN_SHIFT	= 15,

    /** USB Transmit Threshold Length		<i>Access: R_W</i> */
    USB3_TXTHRCFG_USB_THR_LEN_BITS		= 0x1fff0000,
    USB3_TXTHRCFG_USB_THR_LEN_SHIFT		= 16,

    /** USB Non-ISO Transmit Threshold Enable	<i>Access: R_W</i> */
    USB3_TXTHRCFG_USB_NON_ISO_THR_EN_BIT	= 0x40000000,
    USB3_TXTHRCFG_USB_NON_ISO_THR_EN_SHIFT	= 30,

    /** USB ISO Transmit Threshold Enable		<i>Access: R_W</i> */
    USB3_TXTHRCFG_USB_ISO_THR_EN_BIT		= 0x80000000,
    USB3_TXTHRCFG_USB_ISO_THR_EN_SHIFT	= 31,
} gtxthrcfg_data_t;

/**
 * This enum represents the bit fields of the Core Rx Threshold Control
 * Register (GRXTHRCFG).
 */
typedef enum grxthrcfg_data {
    /** Receive Threshold Length		<i>Access: R_W</i> */
    USB3_RXTHRCTL_THR_LEN_BITS	= 0x07ff,
    USB3_RXTHRCTL_THR_LEN_SHIFT	= 0,

    /** Receive Threshold Enable		<i>Access: R_W</i> */
    USB3_RXTHRCTL_THR_EN_BIT		= 0x8000,
    USB3_RXTHRCTL_THR_EN_SHIFT	= 15,
} grxthrcfg_data_t;

/**
 * This enum represents the bit fields of the Core Control
 * Register (GCTL).
 */
typedef enum gctl_data {
    /** Disable Clock Gating		<i>Access: R_W</i> */
    USB3_GCTL_DSBL_CLCK_GTNG_BIT	= 0x00000001,
    USB3_GCTL_DSBL_CLCK_GTNG_SHIFT	= 0,

    /** Global Bernation Enable		<i>Access: R_W</i> */
    USB3_GCTL_GBL_BER_EN_BIT	= 0x00000002,
    USB3_GCTL_GBL_BER_EN_SHIFT	= 1,

    /** U2Exit LFPS		                <i>Access: R_W</i> */
    USB3_GCTL_GBL_U2EXIT_LFPS	= 0x00000004,
    USB3_GCTL_GBL_U2EXIT_LFPS_SHIFT	= 2,

    /** Disable Scrambling			<i>Access: R_W</i> */
    USB3_GCTL_DIS_SCRAMBLE_BIT	= 0x00000008,
    USB3_GCTL_DIS_SCRAMBLE_SHIFT	= 3,

    /** Scale-down Mode			<i>Access: R_W</i> */
    USB3_GCTL_SCALE_DOWN_BITS	= 0x00000030,
    USB3_GCTL_SCALE_DOWN_SHIFT	= 4,

    /** RAM Clock Select			<i>Access: R_W</i> */
    USB3_GCTL_RAM_CLK_SEL_BITS	= 0x000000c0,
    USB3_GCTL_RAM_CLK_SEL_SHIFT	= 6,

    /** Debug Attach			<i>Access: R_W</i> */
    USB3_GCTL_DEBUG_ATTACH_BIT	= 0x00000100,
    USB3_GCTL_DEBUG_ATTACH_SHIFT	= 8,

    /** Loopback Enable			<i>Access: R_W</i> */
    USB3_GCTL_LPBK_EN_BIT		= 0x00000200,
    USB3_GCTL_LPBK_EN_SHIFT		= 9,

    /** Local Loopback Enable		<i>Access: R_W</i> */
    USB3_GCTL_LOCAL_LPBK_EN_BIT	= 0x00000400,
    USB3_GCTL_LOCAL_LPBK_EN_SHIFT	= 10,

    /** Core Soft Reset			<i>Access: R_W</i> */
    USB3_GCTL_CORE_SOFT_RST_BIT	= 0x00000800,
    USB3_GCTL_CORE_SOFT_RST_SHIFT	= 11,

    /** Port Capability Direction		<i>Access: R_W</i> */
    USB3_GCTL_PRT_CAP_DIR_BITS	= 0x00003000,
    USB3_GCTL_PRT_CAP_DIR_SHIFT	= 12,

    /** Port Capability Values */
    USB3_GCTL_PRT_CAP_HOST		= 1,
    USB3_GCTL_PRT_CAP_DEVICE		= 2,
    USB3_GCTL_PRT_CAP_OTG		= 3,

    /** Frame Scale Down			<i>Access: R_W</i> */
    USB3_GCTL_FRMSCLDWN_BITS		= 0x0000c000,
    USB3_GCTL_FRMSCLDWN_SHIFT	= 14,

    /** U2 Reset ECN			<i>Access: R_W</i> */
    USB3_GCTL_U2RSTECN_BIT		= 0x00010000,
    USB3_GCTL_U2RSTECN_SHIFT		= 16,

    /** Power Down Scale			<i>Access: R_W</i> */
    USB3_GCTL_PWR_DN_SCALE_BITS	= 0xfff80000,
    USB3_GCTL_PWR_DN_SCALE_SHIFT	= 19,
} gctl_data_t;

/**
 * This enum represents the bit fields of the Core Interrupt Mask
 * Register (GEVTEN).
 */
typedef enum gevten_data {
    /** ULPI Carkit Event Enable		<i>Access: R_W</i> */
    USB3_GEVTEN_ULPI_CK_EVT_EN_BIT	= 0x00000001,
    USB3_GEVTEN_ULPI_CK_EVT_SHIFT	= 0,

    /** I2C Event Enable			<i>Access: R_W</i> */
    USB3_GEVTEN_I2C_EVT_EN_BIT	= 0x00000002,
    USB3_GEVTEN_I2C_EVT_EN_SHIFT	= 1,
} gevten_data_t;

/**
 * This enum represents the bit fields of the Core Status
 * Register (GSTS).
 */
typedef enum gsts_data {
    /** Current Mode			<i>Access: RO</i>.
     * - 0: Device Mode
     * - 1: Host Mode
     * - 2: DRD Mode
     */
    USB3_GSTS_CURMODE_BITS		= 0x00000003,
    USB3_GSTS_CURMODE_SHIFT		= 0,

    USB3_GSTS_DEVICE_MODE		= 0,
    USB3_GSTS_HOST_MODE		= 1,
    USB3_GSTS_DRD_MODE		= 2,

    /** Bus Error Address Valid		<i>Access: RO</i> */
    USB3_GSTS_BUS_ERR_ADDR_VLD_BIT	= 0x00000010,
    USB3_GSTS_BUS_ERR_ADDR_VLD_SHIFT	= 4,

    /** CSR Timeout */
    USB3_GSTS_CSR_TIMEOUT_BIT	= 0x00000020,
    USB3_GSTS_CSR_TIMEOUT_SHIFT	= 5,

    /** Device Interrupt Pending */
    USB3_GSTS_DEV_EVT_PENDING_BIT	= 0x00000040,
    USB3_GSTS_DEV_EVT_PENDING_SHIFT	= 6,

    /** Host Interrupt Pending */
    USB3_GSTS_HOST_EVT_PENDING_BIT	= 0x00000080,
    USB3_GSTS_HOST_EVT_PENDING_SHIFT	= 7,

    /** ADP Interrupt Pending */
    USB3_GSTS_ADP_EVT_PENDING_BIT	= 0x00000100,
    USB3_GSTS_ADP_EVT_PENDING_SHIFT	= 8,

    /** BC Interrupt Pending */
    USB3_GSTS_BC_EVT_PENDING_BIT	= 0x00000200,
    USB3_GSTS_BC_EVT_PENDING_SHIFT	= 9,

    /** OTG Interrupt Pending */
    USB3_GSTS_OTG_EVT_PENDING_BIT	= 0x00000400,
    USB3_GSTS_OTG_EVT_PENDING_SHIFT	= 10,

    /** Current BELT Value			<i>Access: RO</i> */
    USB3_GSTS_CBELT_BITS		= 0xfff00000,
    USB3_GSTS_CBELT_SHIFT		= 20,
} gsts_data_t;

/**
 * This enum represents the bit fields of the Hardware Parameters 0
 * Register (GHWPARAMS0).
 */
typedef enum ghwparams0_data {
    USB3_HWP0_MODE_BITS		= 0x00000007,
    USB3_HWP0_MODE_SHIFT		= 0,

    USB3_HWP0_MBUS_TYPE_BITS		= 0x00000038,
    USB3_HWP0_MBUS_TYPE_SHIFT	= 3,

    USB3_HWP0_SBUS_TYPE_BITS		= 0x000000c0,
    USB3_HWP0_SBUS_TYPE_SHIFT	= 6,

    USB3_HWP0_MDWIDTH_BITS		= 0x0000ff00,
    USB3_HWP0_MDWIDTH_SHIFT		= 8,

    USB3_HWP0_SDWIDTH_BITS		= 0x00ff0000,
    USB3_HWP0_SDWIDTH_SHIFT		= 16,

    USB3_HWP0_AWIDTH_BITS		= 0x3f000000,
    USB3_HWP0_AWIDTH_SHIFT		= 24,
} ghwparams0_data_t;

/**
 * This enum represents the bit fields of the Hardware Parameters 1
 * Register (GHWPARAMS1).
 */
typedef enum ghwparams1_data {
    USB3_HWP1_IDWIDTH_M1_BITS		= 0x00000007,
    USB3_HWP1_IDWIDTH_M1_SHIFT		= 0,

    USB3_HWP1_BURSTWIDTH_M1_BITS		= 0x00000038,
    USB3_HWP1_BURSTWIDTH_M1_SHIFT		= 3,

    USB3_HWP1_DATAINFOWIDTH_BITS		= 0x000001c0,
    USB3_HWP1_DATAINFOWIDTH_SHIFT		= 6,

    USB3_HWP1_REQINFOWIDTH_BITS		= 0x00000e00,
    USB3_HWP1_REQINFOWIDTH_SHIFT		= 9,

    USB3_HWP1_ASPACEWIDTH_BITS		= 0x00007000,
    USB3_HWP1_ASPACEWIDTH_SHIFT		= 12,

    USB3_HWP1_DEV_NUM_INT_BITS		= 0x001f8000,
    USB3_HWP1_DEV_NUM_INT_SHIFT		= 15,

    USB3_HWP1_NUM_RAMS_BITS			= 0x00600000,
    USB3_HWP1_NUM_RAMS_SHIFT			= 21,

    USB3_HWP1_SPRAM_TYP_BIT			= 0x00800000,
    USB3_HWP1_SPRAM_TYP_SHIFT		= 23,

    USB3_HWP1_EN_PWROPT_BITS			= 0x03000000,
    USB3_HWP1_EN_PWROPT_SHIFT		= 24,

    USB3_EN_PWROPT_NONE		= 0,
    USB3_EN_PWROPT_CLK_GATING_ONLY	= 1,
    USB3_EN_PWROPT_BERNATION	= 2,

    USB3_HWP1_MAC_PHY_CLKS_SYNC_BIT		= 0x04000000,
    USB3_HWP1_MAC_PHY_CLKS_SYNC_SHIFT	= 26,

    USB3_HWP1_MAC_RAM_CLKS_SYNC_BIT		= 0x08000000,
    USB3_HWP1_MAC_RAM_CLKS_SYNC_SHIFT	= 27,

    USB3_HWP1_RAM_BUS_CLKS_SYNC_BIT		= 0x10000000,
    USB3_HWP1_RAM_BUS_CLKS_SYNC_SHIFT	= 28,

    USB3_HWP1_RM_OPT_FEATURES_BIT		= 0x40000000,
    USB3_HWP1_RM_OPT_FEATURES_SHIFT		= 30,
} ghwparams1_data_t;

/**
 * This enum represents the bit fields of the Hardware Parameters 2
 * Register (GHWPARAMS2).
 */
typedef enum ghwparams2_data {
    USB3_HWP2_USERID_BITS	= 0xffffffff,
    USB3_HWP2_USERID_SHIFT	= 0,
} ghwparams2_data_t;

/**
 * This enum represents the bit fields of the Hardware Parameters 3
 * Register (GHWPARAMS3).
 */
typedef enum ghwparams3_data {
    USB3_HWP3_SSPHY_IFC_BITS		= 0x00000003,
    USB3_HWP3_SSPHY_IFC_SHIFT	= 0,

    USB3_HWP3_HSPHY_IFC_BITS		= 0x0000000c,
    USB3_HWP3_HSPHY_IFC_SHIFT	= 2,

    USB3_HWP3_FSPHY_IFC_BITS		= 0x00000030,
    USB3_HWP3_FSPHY_IFC_SHIFT	= 4,

    USB3_HWP3_HSPHY_DWIDTH_BITS	= 0x000000c0,
    USB3_HWP3_HSPHY_DWIDTH_SHIFT	= 6,

    USB3_HWP3_VEND_CTL_IFC_BIT	= 0x00000400,
    USB3_HWP3_VEND_CTL_IFC_SHIFT	= 10,

    USB3_HWP3_ULPI_CARKIT_BIT	= 0x00000800,
    USB3_HWP3_ULPI_CARKIT_SHIFT	= 11,

    USB3_HWP3_NUM_EPS_BITS		= 0x0003f000,
    USB3_HWP3_NUM_EPS_SHIFT		= 12,

    USB3_HWP3_NUM_IN_EPS_BITS	= 0x007c0000,
    USB3_HWP3_NUM_IN_EPS_SHIFT	= 18,

    USB3_HWP3_TOT_XFR_RSRC_BITS	= 0x7f800000,
    USB3_HWP3_TOT_XFR_RSRC_SHIFT	= 23,
} ghwparams3_data_t;

/**
 * This enum represents the bit fields of the Hardware Parameters 4
 * Register (GHWPARAMS4).
 */
typedef enum ghwparams4_data {
    USB3_HWP4_TRBS_PER_XFER_BITS	= 0x0000003f,
    USB3_HWP4_TRBS_PER_XFER_SHIFT	= 0,

    USB3_HWP4_BER_SPAD_BITS	= 0x0001e000,
    USB3_HWP4_BER_SPAD_SHIFT	= 13,

    USB3_HWP4_NUM_SS_USB_INST_BITS	= 0x001e0000,
    USB3_HWP4_NUM_SS_USB_INST_SHIFT	= 17,

    USB3_HWP4_EN_ISOC_SUPT_BIT	= 0x00800000,
    USB3_HWP4_EN_ISOC_SUPT_SHIFT	= 23,

    USB3_HWP4_BMU_PTL_DEPTH_BITS	= 0x0f000000,
    USB3_HWP4_BMU_PTL_DEPTH_SHIFT	= 24,

    USB3_HWP4_BMU_LSP_DEPTH_BITS	= 0xf0000000,
    USB3_HWP4_BMU_LSP_DEPTH_SHIFT	= 28,
} ghwparams4_data_t;

/**
 * This enum represents the bit fields of the Hardware Parameters 5
 * Register (GHWPARAMS5).
 */
typedef enum ghwparams5_data {
    USB3_HWP5_BMU_BUSGM_DEPTH_BITS	= 0x0000000f,
    USB3_HWP5_BMU_BUSGM_DEPTH_SHIFT	= 0,

    USB3_HWP5_RXQ_FIFO_DEPTH_BITS	= 0x000003f0,
    USB3_HWP5_RXQ_FIFO_DEPTH_SHIFT	= 4,

    USB3_HWP5_TXQ_FIFO_DEPTH_BITS	= 0x0000fc00,
    USB3_HWP5_TXQ_FIFO_DEPTH_SHIFT	= 10,

    USB3_HWP5_DWQ_FIFO_DEPTH_BITS	= 0x003f0000,
    USB3_HWP5_DWQ_FIFO_DEPTH_SHIFT	= 16,

    USB3_HWP5_DFQ_FIFO_DEPTH_BITS	= 0x0fc00000,
    USB3_HWP5_DFQ_FIFO_DEPTH_SHIFT	= 22,
} ghwparams5_data_t;


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_USB3_USB3_HW_H_BODY_PART01_H_ */
