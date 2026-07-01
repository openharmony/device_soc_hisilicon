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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_USB3_USB3_HW_H_BODY_PART03_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_USB3_USB3_HW_H_BODY_PART03_H_

	USB3_DEPEVT_ISOC_TIME_PASSED_SHIFT	= 13,

	/** Event Status for Stream Event */
    USB3_DEPEVT_STRM_EVT_BITS		= 0x0000f000,
	USB3_DEPEVT_STRM_EVT_SHIFT		= 12,

	/** Stream Event Status values */
    USB3_DEPEVT_STRM_FOUND		= 1,	/** @< */
	USB3_DEPEVT_STRM_NOT_FOUND	= 2,

	/** Event Status for Xfer Complete or Xfer In Progress Event */
    USB3_DEPEVT_BUS_ERR_BIT			= 0x00001000,
	USB3_DEPEVT_BUS_ERR_SHIFT		= 12,
	USB3_DEPEVT_SHORT_PKT_BIT		= 0x00002000,
	USB3_DEPEVT_SHORT_PKT_SHIFT		= 13,
	USB3_DEPEVT_IOC_BIT			    = 0x00004000,
	USB3_DEPEVT_IOC_SHIFT			= 14,
	USB3_DEPEVT_LST_BIT			    = 0x00008000,
	USB3_DEPEVT_LST_SHIFT			= 15,
#define USB3_DEPEVT_MISSED_ISOC_BIT	    USB3_DEPEVT_LST_BIT
#define USB3_DEPEVT_MISSED_ISOC_SHIFT	USB3_DEPEVT_LST_SHIFT

	/** Event Status for Xfer Not Ready Event */
    USB3_DEPEVT_CTRL_BITS              = 0x00003000,
	USB3_DEPEVT_CTRL_SHIFT			    = 12,
	USB3_DEPEVT_XFER_ACTIVE_BIT		    = 0x00008000,
	USB3_DEPEVT_XFER_ACTIVE_SHIFT		= 15,

	/** Xfer Not Ready Event Status values */
    USB3_DEPEVT_CTRL_SETUP		= 0,	/** @< */
	USB3_DEPEVT_CTRL_DATA		= 1,	/** @< */
	USB3_DEPEVT_CTRL_STATUS		= 2,

	/** Stream ID */
    USB3_DEPEVT_STRM_ID_BITS			= 0xffff0000,
	USB3_DEPEVT_STRM_ID_SHIFT		    = 16,

	/** Isoc uFrame Number (for Xfer Not Ready on Isoc EP) */
    USB3_DEPEVT_ISOC_UFRAME_NUM_BITS    = 0xffff0000,
	USB3_DEPEVT_ISOC_UFRAME_NUM_SHIFT   = 16,

	/** Xfer Resource Index (for Start Xfer Command) */
    USB3_DEPEVT_XFER_RSC_IDX_BITS		= 0x007f0000,
	USB3_DEPEVT_XFER_RSC_IDX_SHIFT		= 16,

	/** Current Data Sequence Number (for Get Endpoint State Command) */
    USB3_DEPEVT_CUR_DAT_SEQ_NUM_BITS    = 0x001f0000,
	USB3_DEPEVT_CUR_DAT_SEQ_NUM_SHIFT	= 16,

	/** Flow Control State (for Get Endpoint State Command) */
    USB3_DEPEVT_FLOW_CTRL_BIT		= 0x00200000,
	USB3_DEPEVT_FLOW_CTRL_SHIFT		= 21,
} depevt_data_t;

/**
 * This enum represents the non-generic bit fields of an Event Buffer entry
 * for other Core events (GEVT).
 */
typedef enum gevt_data {
	/** PHY Port Number */
    USB3_GINT_PHY_PORT_BITS		= 0xf00,
	USB3_GINT_PHY_PORT_SHIFT		= 8,
} gevt_data_t;

/**
 * This struct represents the 32-bit register fields of the Event Buffer
 * Registers (GEVENTBUFn).
 */
typedef struct geventbuf_data {
	/** Event Buffer Address Register Low Word */
    volatile uint32_t geventadr_low;

	/** Event Buffer Address Register High Word */
    volatile uint32_t geventadr_high;

	/** Event Buffer Size Register.
	 * Fields defined in enum @ref geventsiz_data. */
    volatile uint32_t geventsiz;

	/** Event Buffer Count Register.
	 * Fields defined in enum @ref geventcnt_data. */
    volatile uint32_t geventcnt;
} geventbuf_data_t;

#define USB3_CORE_REG_BASE	0xC000

/**
 * Core Global Registers	<i>Offsets 100h-5FCh</i>.
 *
 * The dwc_usb3_core_global_regs structure defines the size
 * and relative field offsets for the Core Global Registers.
 */
typedef struct usb3_core_global_regs {
#define USB3_CORE_GLOBAL_REG_OFFSET	0x100

	/** Core BIU Configuration 0 Register	<i>Offset: 100h</i>.
	 * Fields defined in enum @ref gsbuscfg0_data. */
    volatile uint32_t gsbuscfg0;

	/** Core BIU Configuration 1 Register	<i>Offset: 104h</i>.
	 * Fields defined in enum @ref gsbuscfg1_data. */
    volatile uint32_t gsbuscfg1;

	/** Core Tx Threshold Control Register	<i>Offset: 108h</i>.
	 * Fields defined in enum @ref gtxthrcfg_data. */
    volatile uint32_t gtxthrcfg;

	/** Core Threshold Control Register	<i>Offset: 10Ch</i>.
	 * Fields defined in enum @ref grxthrcfg_data. */
    volatile uint32_t grxthrcfg;

	/** Core Control Register		<i>Offset: 110h</i>.
	 * Fields defined in enum @ref gctl_data. */
    volatile uint32_t gctl;

	/** Core Interrupt Mask Register	<i>Offset: 114h</i>.
	 * Fields defined in enum @ref gevten_data. */
    volatile uint32_t gevten;

	/** Core Status Register		<i>Offset: 118h</i>.
	 * Fields defined in enum @ref gsts_data. */
    volatile uint32_t gsts;

	/** reserved				<i>Offset: 11Ch</i> */
    volatile uint32_t reserved0;

#define USB3_CORE_GSNPSID_REG_OFFSET 0x120

	/** Synopsys ID Register		<i>Offset: 120h</i> */
    volatile uint32_t gsnpsid;

	/** General Purpose I/O Register	<i>Offset: 124h</i> */
    volatile uint32_t ggpio;

	/** User ID Register			<i>Offset: 128h</i> */
    volatile uint32_t guid;

	/** reserved				<i>Offset: 12Ch</i> */
    volatile uint32_t reserved1;

	/** Bus Error Address Register		<i>Offset: 130h</i> */
    volatile uint32_t gbuserraddrlo;

	/** Bus Error Address Register		<i>Offset: 134h</i> */
    volatile uint32_t gbuserraddrhi;

	/** reserved				<i>Offset: 138h-13Ch</i> */
    volatile uint32_t reserved2[2];

	/** Hardware Parameter 0 Register	<i>Offset: 140h</i>.
	 * Fields defined in enum @ref ghwparams0_data. */
    volatile uint32_t ghwparams0;

	/** Hardware Parameter 1 Register	<i>Offset: 144h</i>.
	 * Fields defined in enum @ref ghwparams1_data. */
    volatile uint32_t ghwparams1;

	/** Hardware Parameter 2 Register	<i>Offset: 148h</i>.
	 * Fields defined in enum @ref ghwparams2_data. */
    volatile uint32_t ghwparams2;

	/** Hardware Parameter 3 Register	<i>Offset: 14Ch</i>.
	 * Fields defined in enum @ref ghwparams3_data. */
    volatile uint32_t ghwparams3;

	/** Hardware Parameter 4 Register	<i>Offset: 150h</i>.
	 * Fields defined in enum @ref ghwparams4_data. */
    volatile uint32_t ghwparams4;

	/** Hardware Parameter 5 Register	<i>Offset: 154h</i>.
	 * Fields defined in enum @ref ghwparams5_data. */
    volatile uint32_t ghwparams5;

	/** Hardware Parameter 6 Register	<i>Offset: 158h</i>.
	 * Fields defined in enum @ref ghwparams6_data. */
    volatile uint32_t ghwparams6;

	/** Hardware Parameter 7 Register	<i>Offset: 15Ch</i>.
	 * Fields defined in enum @ref ghwparams7_data. */
    volatile uint32_t ghwparams7;

	/** Debug Queue/FIFO Space Register	<i>Offset: 160h</i>.
	 * Fields defined in enum @ref gdbgfifospace_data. */
    volatile uint32_t gdbgfifospace;

	/** Debug LTSSM Register		<i>Offset: 164h</i>.
	 * Fields defined in enum @ref gdbgltssm_data  */
    volatile uint32_t gdbgltssm;

	/** reserved				<i>Offset: 168h-1FCh</i> */
    volatile uint32_t reserved3[38];

	/** USB2 Configuration Registers	<i>Offset: 200h-23Ch</i>.
	 * Fields defined in enum @ref gusb2phycfg_data. */
    volatile uint32_t gusb2phycfg[16];

	/** USB2 I2C Access Registers		<i>Offset: 240h-27Ch</i>.
	 * Fields defined in enum @ref gusb2i2cctl_data. */
    volatile uint32_t gusb2i2cctl[16];

	/** USB2 PHY Vendor Control Registers	<i>Offset: 280h-2BCh</i>.
	 * Fields defined in enum @ref gusb2phyacc_data. */
    volatile uint32_t gusb2phyacc[16];

	/** USB3 Pipe Control Registers		<i>Offset: 2C0h-2FCh</i>.
	 * Fields defined in enum @ref gusb3pipectl_data. */
    volatile uint32_t gusb3pipectl[16];

	/** Transmit FIFO Size Registers	<i>Offset: 300h-37Ch</i>.
	 * Fields defined in enum @ref gfifosize_data. */
    volatile uint32_t gtxfifosiz[32];

	/** Receive FIFO Size Registers		<i>Offset: 380h-3FC0h</i>.
	 * Fields defined in enum @ref gfifosize_data. */
    volatile uint32_t grxfifosiz[32];

	/** Event Buffer Registers		<i>Offset: 400h-5FCh</i>.
	 * Fields defined in struct @ref geventbuf_data. */
    struct geventbuf_data geventbuf[32];

	/** Hardware Parameter 8 Register	<i>Offset: 600h</i>.
	 * Fields defined in enum @ref ghwparams8_data. */
    volatile uint32_t ghwparams8;
} usb3_core_global_regs_t;

/****************************************************************************/
/* Device Global Registers */

/**
 * This enum represents the bit fields in the Device Configuration
 * Register (DCFG).
 */
typedef enum dcfg_data {
	/** Device Speed			<i>Access: R_W</i> */
    USB3_DCFG_DEVSPD_BITS		= 0x000007,
	USB3_DCFG_DEVSPD_SHIFT		= 0,

	/** Device Speed values */
    USB3_SPEED_HS_PHY_30MHZ_OR_60MHZ		= 0,	/** @< */
	USB3_SPEED_FS_PHY_30MHZ_OR_60MHZ		= 1,	/** @< */
	USB3_SPEED_LS_PHY_6MHZ			= 2,	/** @< */
	USB3_SPEED_FS_PHY_48MHZ			= 3,	/** @< */
	USB3_SPEED_SS_PHY_125MHZ_OR_250MHZ	= 4,

	/** Device Address			<i>Access: R_W</i> */
    USB3_DCFG_DEVADDR_BITS		= 0x0003f8,
	USB3_DCFG_DEVADDR_SHIFT		= 3,

	/** Periodic Frame Interval		<i>Access: R_W</i> */
    USB3_DCFG_PER_FR_INTVL_BITS	= 0x000c00,
	USB3_DCFG_PER_FR_INTVL_SHIFT	= 10,

	/** Periodic Frame Interval values */
    USB3_DCFG_PER_FR_INTVL_80		= 0,	/** @< */
	USB3_DCFG_PER_FR_INTVL_85		= 1,	/** @< */
	USB3_DCFG_PER_FR_INTVL_90		= 2,	/** @< */
	USB3_DCFG_PER_FR_INTVL_95		= 3,

	/** Device Interrupt Number		<i>Access: R_W</i> */
    USB3_DCFG_DEV_INTR_NUM_BITS	= 0x01f000,
	USB3_DCFG_DEV_INTR_NUM_SHIFT	= 12,

	/** Number of Receive Buffers		<i>Access: R_W</i> */
    USB3_DCFG_NUM_RCV_BUF_BITS	= 0x3e0000,
	USB3_DCFG_NUM_RCV_BUF_SHIFT	= 17,

	/** LPM Capable				<i>Access: R_W</i> */
    USB3_DCFG_LPM_CAP_BIT		= 0x400000,
	USB3_DCFG_LPM_CAP_SHIFT		= 22,
} dcfg_data_t;

/**
 * This enum represents the bit fields in the Device Control
 * Register (DCTL).
 */
typedef enum dctl_data {
	/** Soft Disconnect			<i>Access: R_W</i> */
    USB3_DCTL_SFT_DISCONN_BIT		= 0x00000001,
	USB3_DCTL_SFT_DISCONN_SHIFT		= 0,

	/** Test Control			<i>Access: R_W</i> */
    USB3_DCTL_TSTCTL_BITS			= 0x0000001e,
	USB3_DCTL_TSTCTL_SHIFT			= 1,

	/** USB/Link State Change Request	<i>Access: R_W</i> */
    USB3_DCTL_ULST_CHNG_REQ_BITS		= 0x000001e0,
	USB3_DCTL_ULST_CHNG_REQ_SHIFT		= 5,

	/** Requested Link State Transition/Action In SS Mode */
    USB3_LINK_STATE_REQ_NO_ACTION		= 0,
	USB3_LINK_STATE_REQ_SS_DISABLED		= 4,
	USB3_LINK_STATE_REQ_RX_DETECT		= 5,
	USB3_LINK_STATE_REQ_INACTIVE		= 6,
	USB3_LINK_STATE_REQ_RECOVERY		= 8,
	USB3_LINK_STATE_REQ_COMPLIANCE		= 10,
	USB3_LINK_STATE_REQ_LOOPBACK		= 11,
	USB3_LINK_STATE_REQ_HOST_MODE_ONLY	= 15,

	/** Requested Link State Transition/Action In HS/FS/LS Mode */
    USB3_LINK_STATE_REQ_REMOTE_WAKEUP	= 8,

	/** U1/U2 control			<i>Access: R_W</i> */
    USB3_DCTL_ACCEPT_U1_EN_BIT		= 0x00000200,
	USB3_DCTL_ACCEPT_U1_EN_SHIFT		= 9,
	USB3_DCTL_INIT_U1_EN_BIT			= 0x00000400,
	USB3_DCTL_INIT_U1_EN_SHIFT		= 10,
	USB3_DCTL_ACCEPT_U2_EN_BIT		= 0x00000800,
	USB3_DCTL_ACCEPT_U2_EN_SHIFT		= 11,
	USB3_DCTL_INIT_U2_EN_BIT			= 0x00001000,
	USB3_DCTL_INIT_U2_EN_SHIFT		= 12,

	/** Controller Save State		<i>Access: R_W</i> */
    USB3_DCTL_CSS_BIT			= 0x00010000,
	USB3_DCTL_CSS_SHIFT			= 16,

	/** Controller Restore State		<i>Access: R_W</i> */
    USB3_DCTL_CRS_BIT			= 0x00020000,
	USB3_DCTL_CRS_SHIFT			= 17,

	/** L1 Bernation Enable		<i>Access: R_W</i> */
    USB3_DCTL_L1_BER_EN_BIT		= 0x00040000,
	USB3_DCTL_L1_BER_EN_RES_SHIFT		= 18,

	/** Keep Connect (for bernation)	<i>Access: R_W</i> */
    USB3_DCTL_KEEP_CONNECT_BIT		= 0x00080000,
	USB3_DCTL_KEEP_CONNECT_SHIFT		= 19,

	/** LPM Response			<i>Access: R_W</i> */
    USB3_DCTL_APP_L1_RES_BIT			= 0x00800000,
	USB3_DCTL_APP_L1_RES_SHIFT		= 23,

	/* RD Threshold			<i>Access: R_W</i> */
    USB3_DCTL_RD_THR_BITS			= 0x1f000000,
	USB3_DCTL_RD_THR_SHIFT			= 24,

	/** Light Soft Reset			<i>Access: R_W</i> */
    USB3_DCTL_LSFT_RST_BIT			= 0x20000000,
	USB3_DCTL_LSFT_RST_SHIFT			= 29,

	/** Core Soft Reset			<i>Access: R_W</i> */
    USB3_DCTL_CSFT_RST_BIT			= 0x40000000,
	USB3_DCTL_CSFT_RST_SHIFT			= 30,

	/** Run/Stop				<i>Access: R_W</i> */
    USB3_DCTL_RUN_STOP_BIT			= 0x80000000,
	USB3_DCTL_RUN_STOP_SHIFT			= 31,
} dctl_data_t;

/**
 * This enum represents the bit fields of the Device Event Enable
 * Register (DEVTEN).
 */
typedef enum devten_data {
	/** Disconnect Detected Event Enable	<i>Access: R_W</i> */
    USB3_DEVTEN_DISCONN_BIT		= 0x0001,
	USB3_DEVTEN_DISCONN_SHIFT	= 0,

	/** USB Reset Enable			<i>Access: R_W</i> */
    USB3_DEVTEN_USBRESET_BIT		= 0x0002,
	USB3_DEVTEN_USBRESET_SHIFT	= 1,

	/** Connect Done Enable			<i>Access: R_W</i> */
    USB3_DEVTEN_CONNDONE_BIT		= 0x0004,
	USB3_DEVTEN_CONNDONE_SHIFT	= 2,

	/** USB/Link State Change Event Enable	<i>Access: R_W</i> */
    USB3_DEVTEN_ULST_CHNG_BIT	= 0x0008,
	USB3_DEVTEN_ULST_CHNG_SHIFT	= 3,

	/** Resume/Remote-Wakeup Event Enable	<i>Access: R_W</i> */
    USB3_DEVTEN_WKUP_BIT		= 0x0010,
	USB3_DEVTEN_WKUP_SHIFT		= 4,

	/** Bernation Request Event Enable	<i>Access: R_W</i> */
    USB3_DEVTEN_BER_REQ_EVT_BIT	= 0x0020,
	USB3_DEVTEN_BER_REQ_EVT_SHIFT	= 5,

	/** End of Periodic Frame Event Enable	<i>Access: R_W</i> */
    USB3_DEVTEN_EOPF_BIT		= 0x0040,
	USB3_DEVTEN_EOPF_SHIFT		= 6,

	/** Start of (Micro)Frame Enable	<i>Access: R_W</i> */
    USB3_DEVTEN_SOF_BIT		= 0x0080,
	USB3_DEVTEN_SOF_SHIFT		= 7,

	/** Erratic Error Event Enable		<i>Access: R_W</i> */
    USB3_DEVTEN_ERRATICERR_BIT	= 0x0200,
	USB3_DEVTEN_ERRATICERR_SHIFT	= 9,

	/** U2 Inactivity Timeout Enable	<i>Access: R_W</i> */
    USB3_DEVTEN_INACT_TIMEOUT_BIT	= 0x2000,
	USB3_DEVTEN_INACT_TIMEOUT_SHIFT	= 13,
} devten_data_t;

/**
 * This enum represents the bit fields in the Device Status
 * Register (DSTS).
 */
typedef enum dsts_data {
	/** Connected Speed			<i>Access: RO</i>.
	 * (see enum @ref dcfg_data for values) */
    USB3_DSTS_CONNSPD_BITS		= 0x00000007,
	USB3_DSTS_CONNSPD_SHIFT		= 0,

	/** (Micro)Frame Number of Received SOF	<i>Access: RO</i> */
    USB3_DSTS_SOF_FN_BITS		= 0x0001fff8,
	USB3_DSTS_SOF_FN_SHIFT		= 3,

	/** RX Fifo Empty			<i>Access: RO</i> */
    USB3_DSTS_RXFIFO_EMPTY_BIT	= 0x00020000,
	USB3_DSTS_RXFIFO_EMPTY_SHIFT	= 17,

	/** USB/Link State			<i>Access: RO</i> */
    USB3_DSTS_USBLNK_STATE_BITS	= 0x003c0000,
	USB3_DSTS_USBLNK_STATE_SHIFT	= 18,

	/** USB/Link State values same as for devt_data_t */

	/** Device Controller Halted		<i>Access: RO</i> */
    USB3_DSTS_DEV_CTRL_HLT_BIT	= 0x00400000,
	USB3_DSTS_DEV_CTRL_HLT_SHIFT	= 22,

	/** Core Idle				<i>Access: RO</i> */
    USB3_DSTS_CORE_IDLE_BIT		= 0x00800000,
	USB3_DSTS_CORE_IDLE_SHIFT	= 23,

	/** Save State Status			<i>Access: RO</i> */
    USB3_DSTS_SSS_BIT		= 0x01000000,
	USB3_DSTS_SSS_SHIFT		= 24,


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_USB3_USB3_HW_H_BODY_PART03_H_ */
