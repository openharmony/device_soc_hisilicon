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

#ifndef SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_USB3_USB3_HW_H_BODY_PART04_H_
#define SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_USB3_USB3_HW_H_BODY_PART04_H_

    /** Restore State Status		<i>Access: RO</i> */
    USB3_DSTS_RSS_BIT		= 0x02000000,
    USB3_DSTS_RSS_SHIFT		= 25,

    /** Save/Restore Error			<i>Access: RO</i> */
    USB3_DSTS_SRE_BIT		= 0x10000000,
    USB3_DSTS_SRE_SHIFT		= 28,

    /** Link-state Not Ready		<i>Access: RO</i> */
    USB3_DSTS_LNR_BIT		= 0x20000000,
    USB3_DSTS_LNR_SHIFT		= 29,
} dsts_data_t;

/**
 * This enum represents the bit fields in the Device Generic Command Parameter
 * Register (DGCMDPARn) for the various commands.
 */
typedef enum dgcmdpar_data {
    /** Force Link PM Accept
     * (for USB3_DGCMD_XMIT_SET_LINK_FUNC_LMP command) */
    USB3_DGCMDPAR_FORCE_LINK_PM_ACCEPT_BIT	= 0x0001,
    USB3_DGCMDPAR_FORCE_LINK_PM_ACCEPT_SHIFT	= 0,

    /** Vendor Specific Test Select
     * (for USB3_DGCMD_XMIT_VEND_DEV_TST_LMP command) */
    USB3_DGCMDPAR_VEND_SPEC_TST_BITS		= 0x00ff,
    USB3_DGCMDPAR_VEND_SPEC_TST_SHIFT	= 0,

    /** Interface Number (for USB3_DGCMD_XMIT_RMT_WKUP_SIG command) */
    USB3_DGCMDPAR_INTF_NUM_BITS		= 0x00ff,
    USB3_DGCMDPAR_INTF_NUM_SHIFT		= 0,

    /** Best Effort Latency Tolerance Value
     * (for USB3_DGCMD_XMIT_LAT_TOL_MSG command) */
    USB3_DGCMDPAR_BELT_VALUE_BITS		= 0x03ff,
    USB3_DGCMDPAR_BELT_VALUE_SHIFT		= 0,

    /** Best Effort Latency Tolerance Scale
     * (for USB3_DGCMD_XMIT_LAT_TOL_MSG command) */
    USB3_DGCMDPAR_BELT_SCALE_BITS		= 0x0c00,
    USB3_DGCMDPAR_BELT_SCALE_SHIFT		= 10,

    /** Latency Scale values (ns) */
    USB3_LATENCY_VALUE_MULT_1024	= 1,	/** @< */
    USB3_LATENCY_VALUE_MULT_32768	= 2,	/** @< */
    USB3_LATENCY_VALUE_MULT_1048576	= 3,

    /** Bus Interval Adjustment
     * (for USB3_DGCMD_XMIT_BUS_INTVL_ADJ_MSG command) */
    USB3_DGCMDPAR_BUS_INTVL_ADJ_BITS		= 0xffff,
    USB3_DGCMDPAR_BUS_INTVL_ADJ_SHIFT	= 0,

    /** Bus Interval Adjustment values (units) */
    USB3_BUS_INTVL_ADJ_DEC_1		= 0xffff,	/** @< */
    USB3_BUS_INTVL_ADJ_DEC_32768	= 0x8000,	/** @< */
    USB3_BUS_INTVL_ADJ_INC_32767	= 0x7fff,	/** @< */
    USB3_BUS_INTVL_ADJ_NO_CHNG	= 0x0000,

    USB3_DGCMDPAR_HOST_ROLE_REQ_INITIATE	= 0x01,
    USB3_DGCMDPAR_HOST_ROLE_REQ_CONFIRM	= 0x02,
} dgcmdpar_data_t;

/**
 * This enum represents the bit fields in the Device Generic Command
 * Register (DGCMDn).
 */
typedef enum dgcmd_data {
    /** Command Type			<i>Access: R_W</i> */
    USB3_DGCMD_TYP_BITS			= 0x0ff,
    USB3_DGCMD_TYP_SHIFT			= 0,

    /** Command Type values */
    USB3_DGCMD_XMIT_SET_LINK_FUNC_LMP		= 1,	/** @< */
    USB3_DGCMD_SET_PERIODIC_PARAMS			= 2,	/** @< */
    USB3_DGCMD_XMIT_FUNC_WAKE_DEV_NOTIF		= 3,	/** @< */
    USB3_DGCMD_SET_SCRATCHPAD_ARRAY_ADR_LO		= 4,	/** @< */
    USB3_DGCMD_SET_SCRATCHPAD_ARRAY_ADR_HI		= 5,	/** @< */
    USB3_DGCMD_XMIT_HOST_ROLE_REQUEST		= 6,	/** @< */
    USB3_DGCMD_SET_EP_NRDY				= 12,	/** @< */
    USB3_DGCMD_RUN_SOC_BUS_LOOPBK_TST		= 16,	/** @< */

    /** Command Interrupt on Complete	<i>Access: R_W</i> */
    USB3_DGCMD_IOC_BIT			= 0x100,
    USB3_DGCMD_IOC_SHIFT			= 8,

    /** Command Active			<i>Access: R_W</i> */
    USB3_DGCMD_ACT_BIT			= 0x400,
    USB3_DGCMD_ACT_SHIFT			= 10,

    /** Command Status			<i>Access: R_W</i> */
    USB3_DGCMD_STS_BITS			= 0xf000,
    USB3_DGCMD_STS_SHIFT			= 12,

    /** Command Status values */
    USB3_DGCMD_STS_ERROR				= 15,
} dgcmd_data_t;

/**
 * This enum represents the bit fields in the Device Endpoint Mapping
 * Registers (DEPMAPn).
 */
typedef enum depmap_data {
    /** Resource Number			<i>Access: R_W / RO</i> */
    USB3_EPMAP_RES_NUM_BITS		= 0x1f,
    USB3_EPMAP_RES_NUM_SHIFT		= 0,
} depmap_data_t;

/**
 * Device Global Registers	<i>Offsets 700h-7FCh</i>.
 *
 * The following structures define the size and relative field offsets
 * for the Device Mode Global Registers.
 */
typedef struct usb3_dev_global_regs {
#define USB3_DEV_GLOBAL_REG_OFFSET	0x700

    /** Device Configuration Register		<i>Offset: 700h</i>.
     * Fields defined in enum @ref dcfg_data. */
    volatile uint32_t dcfg;

    /** Device Control Register			<i>Offset: 704h</i>.
     * Fields defined in enum @ref dctl_data. */
    volatile uint32_t dctl;

    /** Device All Endpoints Interrupt Mask Register <i>Offset: 708h</i>.
     * Fields defined in enum @ref devten_data. */
    volatile uint32_t devten;

    /** Device Status Register			<i>Offset: 70Ch</i>.
     * Fields defined in enum @ref dsts_data. */
    volatile uint32_t dsts;

    /** Device Generic Command Parameter Register	<i>Offset: 710h</i>.
     * Fields defined in enum @ref dgcmdpar_data. */
    volatile uint32_t dgcmdpar;

    /** Device Generic Command Register		<i>Offset: 714h</i>.
     * Fields defined in enum @ref dgcmd_data. */
    volatile uint32_t dgcmd;

    /** reserved				     <i>Offset: 718h-71Ch</i> */
    volatile uint32_t reserved[2];

    /** Device Active Logical Endpoint Enable Register <i>Offset: 720h</i>.
     * One bit per logical endpoint, bit0=EP0 ... bit31=EP31. */
    volatile uint32_t dalepena;
} usb3_dev_global_regs_t;

/****************************************************************************/
/* Device Endpoint Specific Registers */

/**
 * This enum represents the bit fields in the Device Endpoint Command
 * Parameter 1 Register (DEPCMDPAR1n) for the Set Endpoint Configuration
 * (DEPCFG) command.
 */
typedef enum depcfgpar1_data {
    /** Interrupt number */
    USB3_EPCFG1_INTRNUM_BITS		= 0x0000003f,
    USB3_EPCFG1_INTRNUM_SHIFT	= 0,

    /** Stream Completed */
    USB3_EPCFG1_XFER_CMPL_BIT	= 0x00000100,
    USB3_EPCFG1_XFER_CMPL_SHIFT	= 8,

    /** Stream In Progress */
    USB3_EPCFG1_XFER_IN_PROG_BIT	= 0x00000200,
    USB3_EPCFG1_XFER_IN_PROG_SHIFT	= 9,

    /** Stream Not Ready */
    USB3_EPCFG1_XFER_NRDY_BIT	= 0x00000400,
    USB3_EPCFG1_XFER_NRDY_SHIFT	= 10,

    /** Rx FIFO Underrun / Tx FIFO Overrun */
    USB3_EPCFG1_FIFOXRUN_BIT		= 0x00000800,
    USB3_EPCFG1_FIFOXRUN_SHIFT	= 11,

    /** Back-to-Back Setup Packets Received	 */
    USB3_EPCFG1_SETUP_PNDG_BIT	= 0x00001000,
    USB3_EPCFG1_SETUP_PNDG_SHIFT	= 12,

    /** Endpoint Command Complete */
    USB3_EPCFG1_EPCMD_CMPL_BIT	= 0x00002000,
    USB3_EPCFG1_EPCMD_CMPL_SHIFT	= 13,

    /** Endpoint bInterval */
    USB3_EPCFG1_BINTERVAL_BITS	= 0x00ff0000,
    USB3_EPCFG1_BINTERVAL_SHIFT	= 16,

    /** Endpoint Stream Capability */
    USB3_EPCFG1_STRM_CAP_BIT		= 0x01000000,
    USB3_EPCFG1_STRM_CAP_SHIFT	= 24,

    /** Endpoint Direction */
    USB3_EPCFG1_EP_DIR_BIT		= 0x02000000,
    USB3_EPCFG1_EP_DIR_SHIFT		= 25,

    /** Endpoint Number */
    USB3_EPCFG1_EP_NUM_BITS		= 0x3c000000,
    USB3_EPCFG1_EP_NUM_SHIFT		= 26,
} depcfgpar1_data_t;

/**
 * This enum represents the bit fields in the Device Endpoint Command
 * Parameter 0 Register (DEPCMDPAR0n) for the Set Endpoint Configuration
 * (DWC_EPCMD_SET_EP_CFG) command.
 */
typedef enum depcfgpar0_data {
    /** Endpoint Type			<i>Access: R_W</i> */
    USB3_EPCFG0_EPTYPE_BITS			= 0x00000006,
    USB3_EPCFG0_EPTYPE_SHIFT        = 1,

    /** Endpoint Type values */
    USB3_EP_TYPE_CONTROL	        = 0,	/** @< */
    USB3_EP_TYPE_ISOC		        = 1,	/** @< */
    USB3_EP_TYPE_BULK		        = 2,	/** @< */
    USB3_EP_TYPE_INTR		        = 3,

    /** Maximum Packet Size			<i>Access: R_W</i> */
    USB3_EPCFG0_MPS_BITS			= 0x00003ff8,
    USB3_EPCFG0_MPS_SHIFT			= 3,

    /** Flow Control State			<i>Access: R_W</i> */
    USB3_EPCFG0_FLOW_CTRL_STATE_BIT		= 0x00010000,
    USB3_EPCFG0_FLOW_CTRL_STATE_SHIFT	= 16,

    /** Tx Fifo Number (IN endpoints only)	<i>Access: R_W</i> */
    USB3_EPCFG0_TXFNUM_BITS			    = 0x003e0000,
    USB3_EPCFG0_TXFNUM_SHIFT			= 17,

    /** Burst Size				<i>Access: R_W</i> */
    USB3_EPCFG0_BRSTSIZ_BITS			= 0x03c00000,
    USB3_EPCFG0_BRSTSIZ_SHIFT		    = 22,

    /** Data Sequence Num (old)		<i>Access: R_W</i> */
    USB3_EPCFG0_DSNUM_BITS			= 0x7c000000,
    USB3_EPCFG0_DSNUM_SHIFT			= 26,

    /** Ignore Data Sequence Num (old)	<i>Access: R_W</i> */
    USB3_EPCFG0_IGN_DSNUM_BIT		= 0x80000000,
    USB3_EPCFG0_IGN_DSNUM_SHIFT		= 31,

    /** Config Action (new)			<i>Access: R_W</i> */
    USB3_EPCFG0_CFG_ACTION_BITS		= 0xc0000000,
    USB3_EPCFG0_CFG_ACTION_SHIFT		= 30,

    /** Config Action values (new) */
    USB3_CFG_ACTION_INIT		= 0,	/** @< */
    USB3_CFG_ACTION_RESTORE		= 1,	/** @< */
    USB3_CFG_ACTION_MODIFY		= 2,
} depcfgpar0_data_t;

/**
 * This enum represents the bit fields in the Device Endpoint Command
 * Register (DEPCMDn).
 */
typedef enum depcmd_data {
    /** Command Type			<i>Access: R_W</i> */
    USB3_EPCMD_TYP_BITS		= 0x0ff,
    USB3_EPCMD_TYP_SHIFT		= 0,

    /** Command Type values */
    USB3_EPCMD_SET_EP_CFG	= 1,	/** @< */
    USB3_EPCMD_SET_XFER_CFG	= 2,	/** @< */
    USB3_EPCMD_GET_EP_STATE	= 3,	/** @< */
    USB3_EPCMD_SET_STALL	= 4,	/** @< */
    USB3_EPCMD_CLR_STALL	= 5,	/** @< */
    USB3_EPCMD_START_XFER	= 6,	/** @< */
    USB3_EPCMD_UPDATE_XFER	= 7,	/** @< */
    USB3_EPCMD_END_XFER	= 8,	/** @< */
    USB3_EPCMD_START_NEW_CFG	= 9,

    /** Command Interrupt on Complete	<i>Access: R_W</i> */
    USB3_EPCMD_IOC_BIT		= 0x100,
    USB3_EPCMD_IOC_SHIFT		= 8,

    /** Command Active			<i>Access: R_W</i> */
    USB3_EPCMD_ACT_BIT		= 0x400,
    USB3_EPCMD_ACT_SHIFT		= 10,

    /** High Priority / Force RM Bit	<i>Access: R_W</i> */
    USB3_EPCMD_HP_FRM_BIT		= 0x800,
    USB3_EPCMD_HP_FRM_SHIFT		= 11,

    /** Command Completion Status		<i>Access: R_W</i> */
    USB3_EPCMD_CMPL_STS_BITS		= 0xf000,
    USB3_EPCMD_CMPL_STS_SHIFT	= 12,

    /** Stream Number or uFrame (input)	<i>Access: R_W</i> */
    USB3_EPCMD_STR_NUM_OR_UF_BITS	= 0xffff0000,
    USB3_EPCMD_STR_NUM_OR_UF_SHIFT	= 16,

    /** Transfer Resource Index (output)	<i>Access: R_W</i> */
    USB3_EPCMD_XFER_RSRC_IDX_BITS	= 0x007f0000,
    USB3_EPCMD_XFER_RSRC_IDX_SHIFT	= 16,
} depcmd_data_t;

/**
 * Device Endpoint Specific Registers <i>Offsets 800h-9ECh for OUT,
 *						 810h-9FCh for IN</i>.
 * There will be one set of endpoint registers per logical endpoint
 * implemented.
 */
typedef struct usb3_dev_ep_regs {
#define USB3_DEV_OUT_EP_REG_OFFSET	0x800
#define USB3_DEV_IN_EP_REG_OFFSET	0x810
#define USB3_EP_REG_OFFSET		    0x20

    /** Device Endpoint Command Parameter 2 Register <i>Offset: 800h/810h +
     *					(ep_num * 20h) + 00h</i> */
    volatile uint32_t depcmdpar2;

    /** Device Endpoint Command Parameter 1 Register <i>Offset: 800h/810h +
     *					(ep_num * 20h) + 04h</i> */
    volatile uint32_t depcmdpar1;

    /** Device Endpoint Command Parameter 0 Register <i>Offset: 800h/810h +
     *					(ep_num * 20h) + 08h</i> */
    volatile uint32_t depcmdpar0;

    /** Device Endpoint Command Register	<i>Offset: 800h/810h +
     *					(ep_num * 20h) + 0Ch</i>.
     * Fields defined in enum @ref depcmd_data. */
    volatile uint32_t depcmd;

    /** reserved				<i>Offset: 800h/810h +
     *					(ep_num * 20h) + 10h-1Ch</i> */
    volatile uint32_t reserved[4];
} usb3_dev_ep_regs_t;

/****************************************************************************/
/* DMA Descriptor Specific Structures */

/**
 * This enum represents the bit fields in the DMA Descriptor
 * Status quadlet.
 */
typedef enum desc_sts_data {
    /** Transfer Count */
    USB3_DSCSTS_XFRCNT_BITS	= 0x00ffffff,
    USB3_DSCSTS_XFRCNT_SHIFT	= 0,

    /** Packet Count Minus 1 (for HS IN transfers) */
    USB3_DSCSTS_PCM1_BITS	= 0x03000000,
    USB3_DSCSTS_PCM1_SHIFT	= 24,

    /** Transfer Request Block Response */
    USB3_DSCSTS_TRBRSP_BITS	= 0xf0000000,
    USB3_DSCSTS_TRBRSP_SHIFT	= 28,

    USB3_TRBRSP_MISSED_ISOC_IN	= 1,
    USB3_TRBRSP_SETUP_PEND		= 2,
    USB3_TRBRSP_XFER_IN_PROG		= 4,
} desc_sts_data_t;

/**
 * This enum represents the bit fields in the DMA Descriptor
 * Control quadlet.
 */
typedef enum desc_ctl_data {
    /** Hardware-Owned bit */
    USB3_DSCCTL_HWO_BIT		= 0x00000001,
    USB3_DSCCTL_HWO_SHIFT		= 0,

    /** Last Descriptor bit */
    USB3_DSCCTL_LST_BIT		= 0x00000002,
    USB3_DSCCTL_LST_SHIFT		= 1,

    /** Chain Buffer bit */
    USB3_DSCCTL_CHN_BIT		= 0x00000004,
    USB3_DSCCTL_CHN_SHIFT		= 2,

    /** Continue on Short Packet bit */
    USB3_DSCCTL_CSP_BIT		= 0x00000008,
    USB3_DSCCTL_CSP_SHIFT		= 3,

    /** Transfer Request Block Control field */
    USB3_DSCCTL_TRBCTL_BITS		= 0x000003f0,
    USB3_DSCCTL_TRBCTL_SHIFT		= 4,

    /** Transfer Request Block Control types */
    USB3_DSCCTL_TRBCTL_NORMAL		= 1,	/** @< */
    USB3_DSCCTL_TRBCTL_SETUP			= 2,	/** @< */
    USB3_DSCCTL_TRBCTL_STATUS_2		= 3,	/** @< */
    USB3_DSCCTL_TRBCTL_STATUS_3		= 4,	/** @< */
    USB3_DSCCTL_TRBCTL_CTLDATA_1ST		= 5,	/** @< */
    USB3_DSCCTL_TRBCTL_ISOC_1ST		= 6,	/** @< */
    USB3_DSCCTL_TRBCTL_ISOC			= 7,	/** @< */
    USB3_DSCCTL_TRBCTL_LINK			= 8,

    /** Interrupt on Short Packet bit */
    USB3_DSCCTL_ISP_BIT		        = 0x00000400,
    USB3_DSCCTL_ISP_SHIFT		    = 10,
#define USB3_DSCCTL_IMI_BIT	    USB3_DSCCTL_ISP_BIT
#define USB3_DSCCTL_IMI_SHIFT	USB3_DSCCTL_ISP_SHIFT

    /** Interrupt on Completion bit */
    USB3_DSCCTL_IOC_BIT		        = 0x00000800,
    USB3_DSCCTL_IOC_SHIFT		    = 11,

    /** Stream ID / SOF Number */
    USB3_DSCCTL_STRMID_SOFN_BITS	= 0x3fffc000,
    USB3_DSCCTL_STRMID_SOFN_SHIFT	= 14,
} desc_ctl_data_t;

/**
 * DMA Descriptor structure
 *
 * DMA Descriptor structure contains 4 quadlets:
 * Buffer Pointer Low address, Buffer Pointer High address, Status, and Control.
 */
typedef struct usb3_dma_desc {
    /** Buffer Pointer - Low address quadlet */
    uint32_t	bptl;

    /** Buffer Pointer - High address quadlet */
    uint32_t	bpth;

    /** Status quadlet. Fields defined in enum @ref desc_sts_data. */
    uint32_t	status;

    /** Control quadlet. Fields defined in enum @ref desc_ctl_data. */
    uint32_t	control;
} usb3_dma_desc_t;

#ifdef __cplusplus
}
#endif


#endif /* SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_DRIVERS_USB3_USB3_HW_H_BODY_PART04_H_ */
