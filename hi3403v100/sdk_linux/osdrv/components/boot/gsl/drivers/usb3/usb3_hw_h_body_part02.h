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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_USB3_USB3_HW_H_BODY_PART02_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_USB3_USB3_HW_H_BODY_PART02_H_

/**
 * This enum represents the bit fields of the Hardware Parameters 6
 * Register (GHWPARAMS6).
 */
typedef enum ghwparams6_data {
    USB3_HWP6_PSQ_FIFO_DEPTH_BITS	= 0x0000003f,
	USB3_HWP6_PSQ_FIFO_DEPTH_SHIFT	= 0,

    USB3_HWP6_EN_DBG_PORTS_BIT	= 0x00000040,
	USB3_HWP6_EN_DBG_PORTS_SHIFT	= 6,

    USB3_HWP6_EN_FPGA_BIT		= 0x00000080,
	USB3_HWP6_EN_FPGA_SHIFT		= 7,

    USB3_HWP6_EN_SRP_BIT		= 0x00000400,
	USB3_HWP6_EN_SRP_SHIFT		= 10,

    USB3_HWP6_EN_HNP_BIT		= 0x00000800,
	USB3_HWP6_EN_HNP_SHIFT		= 11,

    USB3_HWP6_EN_ADP_BIT		= 0x00001000,
	USB3_HWP6_EN_ADP_SHIFT		= 12,

    USB3_HWP6_EN_OTG_BIT		= 0x00002000,
	USB3_HWP6_EN_OTG_SHIFT		= 13,

    USB3_HWP6_EN_BC_BIT		= 0x00004000,
	USB3_HWP6_EN_BC_SHIFT		= 14,

    USB3_HWP6_EN_BUS_FILTERS_BIT	= 0x00008000,
	USB3_HWP6_EN_BUS_FILTERS_SHIFT	= 15,

    USB3_HWP6_RAM0_DEPTH_BITS	= 0xffff0000,
	USB3_HWP6_RAM0_DEPTH_SHIFT	= 16,
} ghwparams6_data_t;

/**
 * This enum represents the bit fields of the Hardware Parameters 7
 * Register (GHWPARAMS7).
 */
typedef enum ghwparams7_data {
    USB3_HWP7_RAM1_DEPTH_BITS	= 0x0000ffff,
	USB3_HWP7_RAM1_DEPTH_SHIFT	= 0,

    USB3_HWP7_RAM2_DEPTH_BITS	= 0xffff0000,
	USB3_HWP7_RAM2_DEPTH_SHIFT	= 16,
} ghwparams7_data_t;

/**
 * This enum represents the bit fields of the Hardware Parameters 8
 * Register (GHWPARAMS8).
 */
typedef enum ghwparams8_data {
    USB3_HWP8_DCACHE_DEPTH_BITS	= 0xffffffff,
	USB3_HWP8_DCACHE_DEPTH_SHIFT	= 0,
} ghwparams8_data_t;

/**
 * This enum represents the bit fields of the Debug Queue/FIFO Space
 * Register (GDBGFIFOSPACE).
 */
typedef enum gdbgfifospace_data {
	/** FIFO/Queue Select			<i>Access: R_W</i> */
    USB3_DBGFIFOSPACE_FIFO_QUEUE_SEL_BITS	= 0x000000ff,
	USB3_DBGFIFOSPACE_FIFO_QUEUE_SEL_SHIFT	= 0,

	/*   0 - 31  TxFIFO Number   */
	/*  32 - 63  RxFIFO Number   */
	/*  64 - 95  TxReqQ Number   */
	/*  96 - 127 RxReqQ Number   */
	/* 128 - 159 RxInfoQ Number  */
	/* 160       DescFetchQ      */
	/* 161       EventQ          */
	/* 162       ProtocolStatusQ */

	/** Space Available			<i>Access: R</i> */
    USB3_DBGFIFOSPACE_SPACE_AVAIL_BITS	= 0xffff0000,
	USB3_DBGFIFOSPACE_SPACE_AVAIL_SHIFT	= 16,
} gdbgfifospace_data_t;

/**
 * This enum represents the bit fields of the Debug LTSSM
 * Register (GDBGLTSSM).
 */
typedef enum gdbgltssm_data {
	/** Pipe Status				<i>Access: R</i> */
    USB3_DBGLTSSM_PIPE_STATUS_BITS		= 0x0003ffff,
	USB3_DBGLTSSM_PIPE_STATUS_SHIFT		= 0,

	/** LTDB SubState			<i>Access: R</i> */
    USB3_DBGLTSSM_LTDB_SUB_STATE_BITS	= 0x003c0000,
	USB3_DBGLTSSM_LTDB_SUB_STATE_SHIFT	= 18,

	/** LTDB State				<i>Access: R</i> */
    USB3_DBGLTSSM_LTDB_STATE_BITS		= 0x03c00000,
	USB3_DBGLTSSM_LTDB_STATE_SHIFT		= 22,

	/** LTDB Timeout			<i>Access: R</i> */
    USB3_DBGLTSSM_LTDB_TIMEOUT_BIT		= 0x04000000,
	USB3_DBGLTSSM_LTDB_TIMEOUT_SHIFT		= 26,
} gdbgltssm_data_t;

/**
 * This enum represents the bit fields of the Core USB2 PHY Configuration
 * Registers (GUSB2PHYCFGn).
 */
typedef enum gusb2phycfg_data {
	/** HS/FS Timeout Calibration			<i>Access: R_W</i> */
    USB3_USB2PHYCFG_TOUT_CAL_BITS		= 0x00000007,
	USB3_USB2PHYCFG_TOUT_CAL_SHIFT		= 0,

	/** UTMI+ PHY Intf Width (8-bit/16-bit) SelecT	<i>Access: R_W</i> */
    USB3_USB2PHYCFG_PHY_IF_BIT		= 0x00000008,
	USB3_USB2PHYCFG_PHY_IF_SHIFT		= 3,
	/*--------*/
	/** ULPI DDR Select				<i>Access: R_W</i> */
    USB3_USB2PHYCFG_DDR_SEL_BIT		= 0x00000008,
	USB3_USB2PHYCFG_DDR_SEL_SHIFT		= 3,

	/** UTMI+ / ULPI Select				<i>Access: R_W</i> */
    USB3_USB2PHYCFG_UTMI_ULPI_BIT		= 0x00000010,
	USB3_USB2PHYCFG_UTMI_ULPI_SHIFT		= 4,

	/** Full-speed Serial Interface Select		<i>Access: R_W</i> */
    USB3_USB2PHYCFG_FSINTF_BIT		= 0x00000020,
	USB3_USB2PHYCFG_FSINTF_SHIFT		= 5,

	/** Suspend USB2 Phy				<i>Access: R_W</i> */
    USB3_USB2PHYCFG_SUS_PHY_BIT		= 0x00000040,
	USB3_USB2PHYCFG_SUS_PHY_SHIFT		= 6,

	/** USB2.0 HS PHY/USB1.1 FS Serial Xcvr Select	<i>Access: R_W</i> */
    USB3_USB2PHYCFG_PHY_SEL_BIT		= 0x00000080,
	USB3_USB2PHYCFG_PHY_SEL_SHIFT		= 7,

	/** Enable UTMI Sleep				<i>Access: R_W</i> */
    USB3_USB2PHYCFG_ENBL_SLP_M_BIT		= 0x00000100,
	USB3_USB2PHYCFG_ENBL_SLP_M_SHIFT		= 8,

	/** USB2.0 Turnaround Time			<i>Access: R_W</i> */
    USB3_USB2PHYCFG_USB_TRD_TIM_BITS		= 0x00003c00,
	USB3_USB2PHYCFG_USB_TRD_TIM_SHIFT	= 10,

	/** PHY Low-power Clock Select			<i>Access: R_W</i> */
    USB3_USB2PHYCFG_PHY_LPWR_CLK_SEL_BIT	= 0x00004000,
	USB3_USB2PHYCFG_PHY_LPWR_CLK_SEL_SHIFT	= 14,

	/** ULPI Auto Resume				<i>Access: R_W</i> */
    USB3_USB2PHYCFG_ULPI_AUTO_RES_BIT	= 0x00008000,
	USB3_USB2PHYCFG_ULPI_AUTO_RES_SHIFT	= 15,

	/** ULPI Clock SuspendM				<i>Access: R_W</i> */
    USB3_USB2PHYCFG_ULPI_CLK_SUS_M_BIT	= 0x00010000,
	USB3_USB2PHYCFG_ULPI_CLK_SUS_M_SHIFT	= 16,

	/** ULPI External Vbus Drive			<i>Access: R_W</i> */
    USB3_USB2PHYCFG_ULPI_EXT_VBUS_DRV_BIT	= 0x00020000,
	USB3_USB2PHYCFG_ULPI_EXT_VBUS_DRV_SHIFT	= 17,

	/** ULPI External Vbus Indicator		<i>Access: R_W</i> */
    USB3_USB2PHYCFG_ULPI_EXT_VBUS_IND_BIT	= 0x00040000,
	USB3_USB2PHYCFG_ULPI_EXT_VBUS_IND_SHIFT	= 18,

	/** PHY Interrupt Number			<i>Access: R_W</i> */
    USB3_USB2PHYCFG_PHY_INTR_NUM_BITS	= 0x01f80000,
	USB3_USB2PHYCFG_PHY_INTR_NUM_SHIFT	= 19,

	/** OTG Interrupt Number			<i>Access: R_W</i> */
    USB3_USB2PHYCFG_OTG_INTR_NUM_BITS	= 0x7e000000,
	USB3_USB2PHYCFG_OTG_INTR_NUM_SHIFT	= 25,

	/** PHY Soft Reset				<i>Access: R_W</i> */
    USB3_USB2PHYCFG_PHY_SOFT_RST_BIT		= 0x80000000,
	USB3_USB2PHYCFG_PHY_SOFT_RST_SHIFT	= 31,
} gusb2phycfg_data_t;

/**
 * This enum represents the bit fields in the USB2 I2C Control
 * Registers (GUSB2I2CCTLn).
 */
typedef enum gusb2i2cctl_data {
	/** All bits are reserved */
    USB3_USB2I2C_RSVD_BITS		= 0xffffffff,
	USB3_USB2I2C_RSVD_SHIFT		= 0,
} gusb2i2cctl_data_t;

/**
 * This enum represents the bit fields in the USB2 Phy Vendor Control
 * Registers (GUSB2PHYACCn).
 */
typedef enum gusb2phyacc_data {
	/** Register Data			<i>Access: R_W</i> */
    USB3_USB2PHY_REGDATA_BITS	= 0x000000ff,
	USB3_USB2PHY_REGDATA_SHIFT	= 0,

	/** UTMI+ Vendor Ctrl Register Address	<i>Access: R_W</i> */
    USB3_USB2PHY_VCTRL_BITS		= 0x0000ff00,
	USB3_USB2PHY_VCTRL_SHIFT		= 8,
	/*--------*/
	/** ULPI Extended Register Address	<i>Access: R_W</i> */
    USB3_USB2PHY_EXTREGADDR_BITS	= 0x00003f00,
	USB3_USB2PHY_EXTREGADDR_SHIFT	= 8,

	/** Register Address			<i>Access: R_W</i> */
    USB3_USB2PHY_REGADDR_BITS	= 0x003f0000,
	USB3_USB2PHY_REGADDR_SHIFT	= 16,

	/** Register Write			<i>Access: R_W</i> */
    USB3_USB2PHY_REGWR_BIT		= 0x00400000,
	USB3_USB2PHY_REGWR_SHIFT		= 22,

	/** VStatus Busy			<i>Access: RO</i> */
    USB3_USB2PHY_VSTSBSY_BIT		= 0x00800000,
	USB3_USB2PHY_VSTSBSY_SHIFT	= 23,

	/** VStatus Done			<i>Access: R_SS_SC</i> */
    USB3_USB2PHY_VSTSDONE_BIT	= 0x01000000,
	USB3_USB2PHY_VSTSDONE_SHIFT	= 24,

	/** New Register Request		<i>Access: R_WS_SC</i> */
    USB3_USB2PHY_NEWREGREQ_BIT	= 0x02000000,
	USB3_USB2PHY_NEWREGREQ_SHIFT	= 25,

	/** Disable ULPI Drivers		<i>Access: R_WS_SC</i> */
    USB3_USB2PHY_DIS_ULPI_DRVR_BIT	= 0x04000000,
	USB3_USB2PHY_DIS_ULPI_DRVR_SHIFT	= 26,
} gusb2phyacc_data_t;

/**
 * This enum represents the bit fields of the USB3 Pipe Control
 * Registers (GUSB3PIPECTLn).
 */
typedef enum gusb3pipectl_data {
	/** Elastic Buffer Mode			<i>Access: R_W</i> */
    USB3_PIPECTL_ELAS_BUF_MODE_BIT		= 0x00000001,
	USB3_PIPECTL_ELAS_BUF_MODE_SHIFT		= 0,

	/** Tx De-Emphasis			<i>Access: R_W</i> */
    USB3_PIPECTL_TX_DEMPH_BITS		= 0x00000006,
	USB3_PIPECTL_TX_DEMPH_SHIFT		= 1,

	/** Tx Margin				<i>Access: R_W</i> */
    USB3_PIPECTL_TX_MARGIN_BITS		= 0x00000038,
	USB3_PIPECTL_TX_MARGIN_SHIFT		= 3,

	/** Tx Swing				<i>Access: R_W</i> */
    USB3_PIPECTL_TX_SWING_BIT		= 0x00000040,
	USB3_PIPECTL_TX_SWING_SHIFT		= 6,

	/** Port Operation Direction		<i>Access: R_W</i> */
    USB3_PIPECTL_PRT_OP_DIR_BITS		= 0x00000180,
	USB3_PIPECTL_PRT_OP_DIR_SHIFT		= 7,

	/** LFPS Filter				<i>Access: R_W</i> */
    USB3_PIPECTL_LFPS_FILTER_BIT		= 0x00000200,
	USB3_PIPECTL_LFPS_FILTER_SHIFT		= 9,

	/** P3 Exit Signal In P2		<i>Access: R_W</i> */
    USB3_PIPECTL_P3_EX_SIG_P2_BIT		= 0x00000400,
	USB3_PIPECTL_P3_EX_SIG_P2_SHIFT		= 10,

	/** P3-P2 Transitions OK		<i>Access: R_W</i> */
    USB3_PIPECTL_P3_P2_TRAN_OK_BIT		= 0x00000800,
	USB3_PIPECTL_P3_P2_TRAN_OK_SHIFT		= 11,

	/** LFPS P0 Align			<i>Access: R_W</i> */
    USB3_PIPECTL_LFPS_P0_ALGN_BIT		= 0x00001000,
	USB3_PIPECTL_LFPS_P0_ALGN_SHIFT		= 12,

	/** Pipe Data Width			<i>Access: R_W</i> */
    USB3_PIPECTL_DATA_WIDTH_BITS		= 0x00018000,
	USB3_PIPECTL_DATA_WIDTH_SHIFT		= 15,

	/** Suspend USB3 Phy			<i>Access: R_W</i> */
    USB3_PIPECTL_SUS_PHY_BIT			= 0x00020000,
	USB3_PIPECTL_SUS_PHY_SHIFT		= 17,

	/** PHY Soft Reset			<i>Access: R_W</i> */
    USB3_PIPECTL_PHY_SOFT_RST_BIT		= 0x80000000,
	USB3_PIPECTL_PHY_SOFT_RST_SHIFT		= 31,
} gusb3pipectl_data_t;

/**
 * This enum represents the bit fields in the FIFO Size Registers.
 */
typedef enum gfifosize_data {
	/** Depth				<i>Access: R_W</i> */
    USB3_FIFOSZ_DEPTH_BITS		= 0x0000ffff,
	USB3_FIFOSZ_DEPTH_SHIFT		= 0,

	/** Starting Address			<i>Access: RO or R_W</i> */
    USB3_FIFOSZ_STARTADDR_BITS	= 0xffff0000,
	USB3_FIFOSZ_STARTADDR_SHIFT	= 16,
} gfifosize_data_t;

/**
 * This enum represents the bit fields of the Event Buffer Size
 * Registers (GEVENTSIZn).
 */
typedef enum geventsiz_data {
	/** Event Buffer Size			<i>Access: R_W</i> */
    USB3_EVENTSIZ_SIZ_BITS		= 0x0000ffff,
	USB3_EVENTSIZ_SIZ_SHIFT		= 0,

	/** Event Interrupt Mask (1 == disable)	<i>Access: R_W</i> */
    USB3_EVENTSIZ_INT_MSK_BIT	= 0x80000000,
	USB3_EVENTSIZ_INT_MSK_SHIFT	= 31,
} geventsiz_data_t;

/**
 * This enum represents the bit fields of the Event Buffer Count
 * Registers (GEVENTCNTn).
 */
typedef enum geventcnt_data {
	/** Event Count				<i>Access: R_W</i> */
    USB3_EVENTCNT_CNT_BITS		= 0x0000ffff,
	USB3_EVENTCNT_CNT_SHIFT		= 0,
} geventcnt_data_t;

/**
 * This enum represents the bit fields of a generic Event Buffer entry.
 */
typedef enum gevent_data {
	/** Non-Endpoint Specific Event flag */
    USB3_EVENT_NON_EP_BIT			= 0x01,
	USB3_EVENT_NON_EP_SHIFT			= 0,

	/** Non-Endpoint Specific Event Type */
    USB3_EVENT_INTTYPE_BITS			= 0xfe,
	USB3_EVENT_INTTYPE_SHIFT			= 1,

	/** Non-Endpoint Specific Event Type values */
    USB3_EVENT_DEV_INT		= 0,	/** @< */
	USB3_EVENT_OTG_INT		= 1,	/** @< */
	USB3_EVENT_CARKIT_INT		= 3,	/** @< */
	USB3_EVENT_I2C_INT		= 4,
} gevent_data_t;

/**
 * This enum represents the non-generic bit fields of an Event Buffer entry
 * for Device Specific events (DEVT).
 */
typedef enum devt_data {
	/** Device Specific Event Type */
    USB3_DEVT_BITS				= 0x00000f00,
	USB3_DEVT_SHIFT				= 8,

	/** Device Specific Event Type values */
    USB3_DEVT_DISCONN		= 0,	/** @< */
	USB3_DEVT_USBRESET		= 1,	/** @< */
	USB3_DEVT_CONNDONE		= 2,	/** @< */
	USB3_DEVT_ULST_CHNG		= 3,	/** @< */
	USB3_DEVT_WKUP			= 4,	/** @< */
	USB3_DEVT_BER_REQ		= 5,	/** @< */
	USB3_DEVT_EOPF			= 6,	/** @< */
	USB3_DEVT_SOF			= 7,	/** @< */
	USB3_DEVT_ERRATICERR		= 9,	/** @< */
	USB3_DEVT_CMD_CMPL		= 10,	/** @< */
	USB3_DEVT_OVERFLOW		= 11,	/** @< */
	USB3_DEVT_VNDR_DEV_TST_RCVD	= 12,	/** @< */
	USB3_DEVT_INACT_TIMEOUT_RCVD	= 13,

	/** Event Information */
    USB3_DEVT_EVT_INFO_BITS			= 0xffff0000,
	USB3_DEVT_EVT_INFO_SHIFT			= 16,

	/** USB/Link State */
    USB3_DEVT_ULST_STATE_BITS		= 0x000f0000,
	USB3_DEVT_ULST_STATE_SHIFT		= 16,

	/** USB/Link State values in SS */
    USB3_LINK_STATE_U0		= 0,	/** @< */
	USB3_LINK_STATE_U1		= 1,	/** @< */
	USB3_LINK_STATE_U2		= 2,	/** @< */
	USB3_LINK_STATE_U3		= 3,	/** @< */
	USB3_LINK_STATE_SS_DIS		= 4,	/** @< */
	USB3_LINK_STATE_RX_DET		= 5,	/** @< */
	USB3_LINK_STATE_SS_INACT		= 6,	/** @< */
	USB3_LINK_STATE_POLL		= 7,	/** @< */
	USB3_LINK_STATE_RECOV		= 8,	/** @< */
	USB3_LINK_STATE_HRESET		= 9,	/** @< */
	USB3_LINK_STATE_CMPLY		= 10,	/** @< */
	USB3_LINK_STATE_LPBK		= 11,	/** @< */
	USB3_LINK_STATE_RESET		= 14,	/** @< */
	USB3_LINK_STATE_RESUME		= 15,

	/** USB/Link State values in HS/FS/LS */
    USB3_LINK_STATE_ON		    = 0,	/** @< */
	USB3_LINK_STATE_SLEEP		= 2,	/** @< */
	USB3_LINK_STATE_SUSPEND		= 3,	/** @< */
	USB3_LINK_STATE_EARLY_SUSPEND	= 5,

    USB3_DEVT_ULST_SS_BIT			= 0x00100000,
	USB3_DEVT_ULST_SS_SHIFT			= 20,

#define USB3_DEVT_BER_STATE_BITS	USB3_DEVT_ULST_STATE_BITS
#define USB3_DEVT_BER_STATE_SHIFT	USB3_DEVT_ULST_STATE_SHIFT

#define USB3_DEVT_BER_SS_BIT		USB3_DEVT_ULST_SS_BIT
#define USB3_DEVT_BER_SS_SHIFT    USB3_DEVT_ULST_SS_SHIFT

	USB3_DEVT_BER_HIRD_BITS		= 0x0f000000,
	USB3_DEVT_BER_HIRD_SHIFT		= 24,
} devt_data_t;

/**
 * This enum represents the bit fields of an Event Buffer entry for
 * Endpoint Specific events (DEPEVT).
 */
typedef enum depevt_data {
	/** Endpoint Number */
    USB3_DEPEVT_EPNUM_BITS			= 0x0000003e,
	USB3_DEPEVT_EPNUM_SHIFT			= 1,

	/** Endpoint Event Type */
    USB3_DEPEVT_INTTYPE_BITS			= 0x000003c0,
	USB3_DEPEVT_INTTYPE_SHIFT		= 6,

	/** Endpoint Event Type values */
    USB3_DEPEVT_XFER_CMPL		    = 1,	/** @< */
	USB3_DEPEVT_XFER_IN_PROG		= 2,	/** @< */
	USB3_DEPEVT_XFER_NRDY		    = 3,	/** @< */
	USB3_DEPEVT_FIFOXRUN		    = 4,	/** @< */
	USB3_DEPEVT_STRM_EVT		    = 6,	/** @< */
	USB3_DEPEVT_EPCMD_CMPL		    = 7,

	/** Event Status for Start Xfer Command */
    USB3_DEPEVT_NO_MORE_RSCS_BIT		= 0x00001000,
	USB3_DEPEVT_NO_MORE_RSCS_SHIFT		= 12,
	USB3_DEPEVT_ISOC_TIME_PASSED_BIT    = 0x00002000,


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_USB3_USB3_HW_H_BODY_PART02_H_ */
