/*
 * Copyright (c) 2026 Nanjing Qinuo Information Technology Co., Ltd.
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

#ifndef MIPI_TX_HI3403_REG_H
#define MIPI_TX_HI3403_REG_H

/* ====== Register bitfield types ====== */

typedef union {
    struct {
        unsigned int pwrUp : 1;
        unsigned int reserved0 : 31;
    } bits;
    unsigned int u32;
} U_PWR_UP;

typedef union {
    struct {
        unsigned int txEscClkDivision : 8;
        unsigned int reserved0 : 8;
        unsigned int reserved1 : 16;
    } bits;
    unsigned int u32;
} U_CLKMGR_CFG;

typedef union {
    struct {
        unsigned int vcid : 2;
        unsigned int reserved0 : 30;
    } bits;
    unsigned int u32;
} U_VCID;

typedef union {
    struct {
        unsigned int colorCoding : 4;
        unsigned int reserved0 : 4;
        unsigned int loosely18En : 1;
        unsigned int reserved1 : 23;
    } bits;
    unsigned int u32;
} U_COLOR_CODING;

typedef union {
    struct {
        unsigned int invactLpcmdTime : 8;
        unsigned int reserved0 : 8;
        unsigned int outvactLpcmdTime : 8;
        unsigned int reserved1 : 8;
    } bits;
    unsigned int u32;
} U_LP_CMD_TIM;

typedef union {
    struct {
        unsigned int eotpTxEn : 1;
        unsigned int eotpRxEn : 1;
        unsigned int btaEn : 1;
        unsigned int eccRxEn : 1;
        unsigned int crcRxEn : 1;
        unsigned int reserved0 : 27;
    } bits;
    unsigned int u32;
} U_PCKHDL_CFG;

typedef union {
    struct {
        unsigned int genVcidRx : 2;
        unsigned int reserved0 : 30;
    } bits;
    unsigned int u32;
} U_GEN_VCID;

typedef union {
    struct {
        unsigned int cmdVideoMode : 1;
        unsigned int reserved0 : 31;
    } bits;
    unsigned int u32;
} U_MODE_CFG;

typedef union {
    struct {
        unsigned int vidModeType : 2;
        unsigned int reserved0 : 6;
        unsigned int lpVsaEn : 1;
        unsigned int lpVbpEn : 1;
        unsigned int lpVfpEn : 1;
        unsigned int lpVactEn : 1;
        unsigned int lpHbpEn : 1;
        unsigned int lpHfpEn : 1;
        unsigned int frameBtaAckEn : 1;
        unsigned int lpCmdEn : 1;
        unsigned int vpgEn : 1;
        unsigned int reserved1 : 3;
        unsigned int vpgMode : 1;
        unsigned int reserved2 : 3;
        unsigned int vpgOrientation : 1;
        unsigned int reserved3 : 7;
    } bits;
    unsigned int u32;
} U_VID_MODE_CFG;

typedef union {
    struct {
        unsigned int vidPktSize : 14;
        unsigned int reserved0 : 18;
    } bits;
    unsigned int u32;
} U_VID_PKT_SIZE;

typedef union {
    struct {
        unsigned int vidNumChunks : 13;
        unsigned int reserved0 : 19;
    } bits;
    unsigned int u32;
} U_VID_NUM_CHUNKS;

typedef union {
    struct {
        unsigned int vidNullSize : 13;
        unsigned int reserved0 : 19;
    } bits;
    unsigned int u32;
} U_VID_NULL_SIZE;

typedef union {
    struct {
        unsigned int vidHsaTime : 12;
        unsigned int reserved0 : 20;
    } bits;
    unsigned int u32;
} U_VID_HSA_TIME;

typedef union {
    struct {
        unsigned int vidHbpTime : 12;
        unsigned int reserved0 : 20;
    } bits;
    unsigned int u32;
} U_VID_HBP_TIME;

typedef union {
    struct {
        unsigned int vidHlineTime : 15;
        unsigned int reserved0 : 17;
    } bits;
    unsigned int u32;
} U_VID_HLINE_TIME;

typedef union {
    struct {
        unsigned int vsaLines : 10;
        unsigned int reserved0 : 22;
    } bits;
    unsigned int u32;
} U_VID_VSA_LINES;

typedef union {
    struct {
        unsigned int vbpLines : 10;
        unsigned int reserved0 : 22;
    } bits;
    unsigned int u32;
} U_VID_VBP_LINES;

typedef union {
    struct {
        unsigned int vfpLines : 10;
        unsigned int reserved0 : 22;
    } bits;
    unsigned int u32;
} U_VID_VFP_LINES;

typedef union {
    struct {
        unsigned int vActiveLines : 14;
        unsigned int reserved0 : 18;
    } bits;
    unsigned int u32;
} U_VID_VACTIVE_LINES;

typedef union {
    struct {
        unsigned int edpiAllowedCmdSize : 16;
        unsigned int reserved0 : 16;
    } bits;
    unsigned int u32;
} U_EDPI_CMD_SIZE;

typedef union {
    struct {
        unsigned int reserved0 : 1;
        unsigned int reserved1 : 1;
        unsigned int reserved2 : 6;
        unsigned int genSw0pTx : 1;
        unsigned int genSw1pTx : 1;
        unsigned int genSw2pTx : 1;
        unsigned int genSr0pTx : 1;
        unsigned int genSr1pTx : 1;
        unsigned int genSr2pTx : 1;
        unsigned int genLwTx : 1;
        unsigned int reserved3 : 1;
        unsigned int dcsSw0pTx : 1;
        unsigned int dcsSw1pTx : 1;
        unsigned int dcsSr0pTx : 1;
        unsigned int dcsLwTx : 1;
        unsigned int reserved4 : 4;
        unsigned int maxRdPktSize : 1;
        unsigned int reserved5 : 7;
    } bits;
    unsigned int u32;
} U_CMD_MODE_CFG;

typedef union {
    struct {
        unsigned int genDt : 6;
        unsigned int genVc : 2;
        unsigned int genWcLsbyte : 8;
        unsigned int genWcMsbyte : 8;
        unsigned int reserved0 : 8;
    } bits;
    unsigned int u32;
} U_GEN_HDR;

typedef union {
    struct {
        unsigned int genPldB1 : 8;
        unsigned int genPldB2 : 8;
        unsigned int genPldB3 : 8;
        unsigned int genPldB4 : 8;
    } bits;
    unsigned int u32;
} U_GEN_PLD_DATA;

typedef union {
    struct {
        unsigned int genCmdEmpty : 1;
        unsigned int genCmdFull : 1;
        unsigned int genPldWEmpty : 1;
        unsigned int genPldWFull : 1;
        unsigned int genPldREmpty : 1;
        unsigned int genPldRFull : 1;
        unsigned int genRdCmdBusy : 1;
        unsigned int reserved0 : 25;
    } bits;
    unsigned int u32;
} U_CMD_PKT_STATUS;

typedef union {
    struct {
        unsigned int lpWrToCnt : 16;
        unsigned int reserved0 : 16;
    } bits;
    unsigned int u32;
} U_LP_WR_TO_CNT;

typedef union {
    struct {
        unsigned int btaToCnt : 16;
        unsigned int reserved0 : 16;
    } bits;
    unsigned int u32;
} U_BTA_TO_CNT;

typedef union {
    struct {
        unsigned int phyTxrequestclkhs : 1;
        unsigned int autoClklaneCtrl : 1;
        unsigned int reserved0 : 30;
    } bits;
    unsigned int u32;
} U_LPCLK_CTRL;

typedef union {
    struct {
        unsigned int phyClklp2hsTime : 10;
        unsigned int reserved0 : 6;
        unsigned int phyClkhs2lpTime : 10;
        unsigned int reserved1 : 6;
    } bits;
    unsigned int u32;
} U_PHY_TMR_LPCLK_CFG;

typedef union {
    struct {
        unsigned int phyLp2hsTime : 10;
        unsigned int reserved0 : 6;
        unsigned int phyHs2lpTime : 10;
        unsigned int reserved1 : 6;
    } bits;
    unsigned int u32;
} U_PHY_TMR_CFG;

typedef union {
    struct {
        unsigned int phyShutdownz : 1;
        unsigned int phyRstz : 1;
        unsigned int phyEnableclk : 1;
        unsigned int phyForcepll : 1;
        unsigned int reserved0 : 28;
    } bits;
    unsigned int u32;
} U_PHY_RSTZ;

typedef union {
    struct {
        unsigned int nLanes : 2;
        unsigned int reserved0 : 6;
        unsigned int phyStopWaitTime : 8;
        unsigned int reserved1 : 16;
    } bits;
    unsigned int u32;
} U_PHY_IF_CFG;

typedef union {
    struct {
        unsigned int phyTxrequlpsclk : 1;
        unsigned int phyTxexitulpsclk : 1;
        unsigned int phyTxrequlpslan : 1;
        unsigned int phyTxexitulpslan : 1;
        unsigned int reserved0 : 28;
    } bits;
    unsigned int u32;
} U_PHY_ULPS_CTRL;

typedef union {
    struct {
        unsigned int phyTestclr : 1;
        unsigned int phyTestclk : 1;
        unsigned int reserved0 : 30;
    } bits;
    unsigned int u32;
} U_PHY_TST_CTRL0;

typedef union {
    struct {
        unsigned int phyTestdin : 8;
        unsigned int phyTestdout : 8;
        unsigned int phyTesten : 1;
        unsigned int reserved0 : 15;
    } bits;
    unsigned int u32;
} U_PHY_TST_CTRL1;

typedef union {
    struct {
        unsigned int ackWithErr0 : 1;
        unsigned int ackWithErr1 : 1;
        unsigned int ackWithErr2 : 1;
        unsigned int ackWithErr3 : 1;
        unsigned int ackWithErr4 : 1;
        unsigned int ackWithErr5 : 1;
        unsigned int ackWithErr6 : 1;
        unsigned int ackWithErr7 : 1;
        unsigned int ackWithErr8 : 1;
        unsigned int ackWithErr9 : 1;
        unsigned int ackWithErr10 : 1;
        unsigned int ackWithErr11 : 1;
        unsigned int ackWithErr12 : 1;
        unsigned int ackWithErr13 : 1;
        unsigned int ackWithErr14 : 1;
        unsigned int ackWithErr15 : 1;
        unsigned int dphyErrors0 : 1;
        unsigned int dphyErrors1 : 1;
        unsigned int dphyErrors2 : 1;
        unsigned int dphyErrors3 : 1;
        unsigned int dphyErrors4 : 1;
        unsigned int reserved0 : 11;
    } bits;
    unsigned int u32;
} U_INT_ST0;

typedef union {
    struct {
        unsigned int toHsTx : 1;
        unsigned int toLpRx : 1;
        unsigned int eccSingleErr : 1;
        unsigned int eccMultiErr : 1;
        unsigned int crcErr : 1;
        unsigned int pktSizeErr : 1;
        unsigned int eoptErr : 1;
        unsigned int dpiPldWrErr : 1;
        unsigned int genCmdWrErr : 1;
        unsigned int genPldWrErr : 1;
        unsigned int genPldSendErr : 1;
        unsigned int genPldRdErr : 1;
        unsigned int genPldRecevErr : 1;
        unsigned int reserved0 : 3;
        unsigned int vsstart : 1;
        unsigned int reserved1 : 3;
        unsigned int rxtrigger0 : 1;
        unsigned int rxtrigger1 : 1;
        unsigned int rxtrigger2 : 1;
        unsigned int rxtrigger3 : 1;
        unsigned int hssAbnormal : 1;
        unsigned int reserved2 : 7;
    } bits;
    unsigned int u32;
} U_INT_ST1;

typedef union {
    struct {
        unsigned int maskAckWithErr0 : 1;
        unsigned int maskAckWithErr1 : 1;
        unsigned int maskAckWithErr2 : 1;
        unsigned int maskAckWithErr3 : 1;
        unsigned int maskAckWithErr4 : 1;
        unsigned int maskAckWithErr5 : 1;
        unsigned int maskAckWithErr6 : 1;
        unsigned int maskAckWithErr7 : 1;
        unsigned int maskAckWithErr8 : 1;
        unsigned int maskAckWithErr9 : 1;
        unsigned int maskAckWithErr10 : 1;
        unsigned int maskAckWithErr11 : 1;
        unsigned int maskAckWithErr12 : 1;
        unsigned int maskAckWithErr13 : 1;
        unsigned int maskAckWithErr14 : 1;
        unsigned int maskAckWithErr15 : 1;
        unsigned int maskDphyErrors0 : 1;
        unsigned int maskDphyErrors1 : 1;
        unsigned int maskDphyErrors2 : 1;
        unsigned int maskDphyErrors3 : 1;
        unsigned int maskDphyErrors4 : 1;
        unsigned int reserved0 : 11;
    } bits;
    unsigned int u32;
} U_INT_MSK0;

typedef union {
    struct {
        unsigned int maskToHsTx : 1;
        unsigned int maskToLpRx : 1;
        unsigned int maskEccSingleErr : 1;
        unsigned int maskEccMultiErr : 1;
        unsigned int maskCrcErr : 1;
        unsigned int maskPktSizeErr : 1;
        unsigned int maskEoptErr : 1;
        unsigned int maskDpiPldWrErr : 1;
        unsigned int maskGenCmdWrErr : 1;
        unsigned int maskGenPldWrErr : 1;
        unsigned int maskGenPldSendErr : 1;
        unsigned int maskGenPldRdErr : 1;
        unsigned int maskGenPldRecevErr : 1;
        unsigned int reserved0 : 3;
        unsigned int maskVsstart : 1;
        unsigned int reserved1 : 3;
        unsigned int maskTrigger0 : 1;
        unsigned int maskTrigger1 : 1;
        unsigned int maskTrigger2 : 1;
        unsigned int maskTrigger3 : 1;
        unsigned int maskHssAbnormal : 1;
        unsigned int reserved2 : 7;
    } bits;
    unsigned int u32;
} U_INT_MSK1;

typedef union {
    struct {
        unsigned int reserved0 : 1;
        unsigned int memCkEn : 1;
        unsigned int reserved1 : 2;
        unsigned int reserved2 : 1;
        unsigned int initSkewEn : 1;
        unsigned int periodSkewEn : 1;
        unsigned int reserved3 : 1;
        unsigned int reserved4 : 1;
        unsigned int reserved5 : 1;
        unsigned int reserved6 : 1;
        unsigned int reserved7 : 1;
        unsigned int reserved8 : 1;
        unsigned int reserved9 : 1;
        unsigned int reserved10 : 1;
        unsigned int reserved11 : 1;
        unsigned int readEmptyVsyncEn : 1;
        unsigned int reserved12 : 1;
        unsigned int bufClrEn : 1;
        unsigned int reserved13 : 1;
        unsigned int hssAbnormalRst : 1;
        unsigned int reserved14 : 3;
        unsigned int reserved15 : 1;
        unsigned int reserved16 : 1;
        unsigned int reserved17 : 1;
        unsigned int reserved18 : 1;
        unsigned int reserved19 : 3;
        unsigned int inputEn : 1;
    } bits;
    unsigned int u32;
} U_OPERATION_MODE;

typedef union {
    struct {
        unsigned int reserved0 : 6;
        unsigned int reserved1 : 2;
        unsigned int dtHss : 6;
        unsigned int reserved2 : 2;
        unsigned int dtVse : 6;
        unsigned int reserved3 : 2;
        unsigned int dtVss : 6;
        unsigned int reserved4 : 2;
    } bits;
    unsigned int u32;
} U_DATATYPE0;

typedef union {
    struct {
        unsigned int csiEn : 1;
        unsigned int reserved0 : 3;
        unsigned int reserved1 : 1;
        unsigned int reserved2 : 3;
        unsigned int reserved3 : 1;
        unsigned int reserved4 : 3;
        unsigned int reserved5 : 1;
        unsigned int reserved6 : 3;
        unsigned int reserved7 : 1;
        unsigned int reserved8 : 15;
    } bits;
    unsigned int u32;
} U_CSI_CTRL;

typedef union {
    struct {
        unsigned int lane0Id : 2;
        unsigned int reserved0 : 2;
        unsigned int lane1Id : 2;
        unsigned int reserved1 : 2;
        unsigned int lane2Id : 2;
        unsigned int reserved2 : 2;
        unsigned int lane3Id : 2;
        unsigned int reserved3 : 18;
    } bits;
    unsigned int u32;
} U_LANE_ID;

/* ====== hi3403v100 additional register union types ====== */

typedef union {
    struct {
        unsigned int lprxToCnt : 16;
        unsigned int hstxToCnt : 16;
    } bits;
    unsigned int u32;
} U_HS_LP_TO_SET;

typedef union {
    struct {
        unsigned int hsrdToSet : 16;
        unsigned int reserved0 : 16;
    } bits;
    unsigned int u32;
} U_HSRD_TO_SET;

typedef union {
    struct {
        unsigned int lprdToSet : 16;
        unsigned int reserved0 : 16;
    } bits;
    unsigned int u32;
} U_LPRD_TO_SET;

typedef union {
    struct {
        unsigned int hswrToSet : 16;
        unsigned int reserved0 : 8;
        unsigned int preToMode : 1;
        unsigned int reserved1 : 7;
    } bits;
    unsigned int u32;
} U_HSWR_TO_SET;

typedef union {
    struct {
        unsigned int lpwrToSet : 16;
        unsigned int reserved0 : 16;
    } bits;
    unsigned int u32;
} U_LPWR_TO_SET;

typedef union {
    struct {
        unsigned int btaToSet : 16;
        unsigned int reserved0 : 16;
    } bits;
    unsigned int u32;
} U_BTA_TO_SET;

typedef union {
    struct {
        unsigned int txTriggers : 4;
        unsigned int reserved0 : 28;
    } bits;
    unsigned int u32;
} U_TX_TRIGGERS;

typedef union {
    struct {
        unsigned int readCmdTime : 15;
        unsigned int reserved0 : 17;
    } bits;
    unsigned int u32;
} U_READ_CMD_TIME;

typedef union {
    struct {
        unsigned int vidShadowEn : 1;
        unsigned int reserved0 : 7;
        unsigned int vidShadowReq : 1;
        unsigned int reserved1 : 23;
    } bits;
    unsigned int u32;
} U_VID_SHADOW_CTRL;

typedef union {
    struct {
        unsigned int dtNull : 6;
        unsigned int reserved0 : 2;
        unsigned int dtBlank : 6;
        unsigned int reserved1 : 2;
        unsigned int dtEtop : 6;
        unsigned int reserved2 : 2;
        unsigned int dtHse : 6;
        unsigned int reserved3 : 2;
    } bits;
    unsigned int u32;
} U_DATATYPE1;

typedef union {
    struct {
        unsigned int delayRegin : 17;
        unsigned int reserved0 : 3;
        unsigned int delayAbnormal : 1;
        unsigned int delayFromReg : 1;
        unsigned int reserved1 : 10;
    } bits;
    unsigned int u32;
} U_READ_MEMORY_DELAY_CTRL;

/* ====== hi3403v100 register struct (hi3403 hardware offsets) ====== */
typedef volatile struct {
    U_PWR_UP pwrUp;                           /* 0x0 */
    unsigned int reserved0;                   /* 0x4 */
    U_CLKMGR_CFG clkmgrCfg;                   /* 0x8 */
    U_VCID vcid;                               /* 0xc */
    U_COLOR_CODING colorCoding;               /* 0x10 */
    unsigned int reserved1;                   /* 0x14 */
    U_LP_CMD_TIM lpCmdTim;                   /* 0x18 */
    unsigned int reserved2[4];                /* 0x1c~0x28 */
    U_PCKHDL_CFG pckhdlCfg;                   /* 0x2c */
    U_GEN_VCID genVcid;                       /* 0x30 */
    U_MODE_CFG modeCfg;                       /* 0x34 */
    U_VID_MODE_CFG vidModeCfg;               /* 0x38 */
    U_VID_PKT_SIZE vidPktSize;               /* 0x3c */
    U_VID_NUM_CHUNKS vidNumChunks;           /* 0x40 */
    U_VID_NULL_SIZE vidNullSize;             /* 0x44 */
    U_VID_HSA_TIME vidHsaTime;               /* 0x48 */
    U_VID_HBP_TIME vidHbpTime;               /* 0x4c */
    U_VID_HLINE_TIME vidHlineTime;           /* 0x50 */
    U_VID_VSA_LINES vidVsaLines;             /* 0x54 */
    U_VID_VBP_LINES vidVbpLines;             /* 0x58 */
    U_VID_VFP_LINES vidVfpLines;             /* 0x5c */
    U_VID_VACTIVE_LINES vidVactiveLines;     /* 0x60 */
    U_EDPI_CMD_SIZE edpiCmdSize;             /* 0x64 */
    U_CMD_MODE_CFG cmdModeCfg;               /* 0x68 */
    U_GEN_HDR genHdr;                         /* 0x6c */
    U_GEN_PLD_DATA genPldData;               /* 0x70 */
    U_CMD_PKT_STATUS cmdPktStatus;           /* 0x74 */
    U_HS_LP_TO_SET hsLpToSet;               /* 0x78 */
    U_HSRD_TO_SET hsrdToSet;                 /* 0x7c */
    U_LPRD_TO_SET lprdToSet;                 /* 0x80 */
    U_HSWR_TO_SET hswrToSet;                 /* 0x84 */
    U_LPWR_TO_SET lpwrToSet;                 /* 0x88 */
    U_BTA_TO_SET btaToSet;                   /* 0x8c */
    unsigned int reserved3;                   /* 0x90 */
    U_LPCLK_CTRL lpclkCtrl;                   /* 0x94 */
    U_PHY_TMR_LPCLK_CFG phyTmrLpclkCfg;     /* 0x98 */
    U_PHY_TMR_CFG phyTmrCfg;                 /* 0x9c */
    U_PHY_RSTZ phyRstz;                       /* 0xa0 */
    U_PHY_IF_CFG phyIfCfg;                   /* 0xa4 */
    U_PHY_ULPS_CTRL phyUlpsCtrl;             /* 0xa8 */
    U_TX_TRIGGERS txTriggers;                 /* 0xac */
    unsigned int reserved4;                   /* 0xb0 */
    U_PHY_TST_CTRL0 phyTstCtrL0;             /* 0xb4 */
    U_PHY_TST_CTRL1 phyTstCtrL1;             /* 0xb8 */
    U_INT_ST0 intST0;                         /* 0xbc */
    U_INT_ST1 intST1;                         /* 0xc0 */
    U_INT_MSK0 intMsK0;                       /* 0xc4 */
    U_INT_MSK1 intMsK1;                       /* 0xc8 */
    unsigned int reserved6[10];               /* 0xcc~0xf0 */
    U_READ_CMD_TIME readCmdTime;             /* 0xf4 */
    unsigned int reserved7[2];                /* 0xf8~0xfc */
    U_VID_SHADOW_CTRL vidShadowCtrl;         /* 0x100 */
    unsigned int reserved8[82];               /* 0x104~0x248 */
    unsigned int gintMsk;                     /* 0x24c */
    unsigned int reserved9[46];               /* 0x250~0x304 */
    U_OPERATION_MODE operationMode;           /* 0x308 */
    unsigned int reserved10[9];               /* 0x30c~0x32c */
    U_DATATYPE0 datatypE0;                     /* 0x330 */
    U_DATATYPE1 datatypE1;                     /* 0x334 */
    U_CSI_CTRL csiCtrl;                       /* 0x338 */
    unsigned int reserved11[5];               /* 0x33c~0x34c */
    U_LANE_ID laneId;                         /* 0x350 */
    U_READ_MEMORY_DELAY_CTRL readMemoryDelayCtrl; /* 0x354 */
} MipiTxRegsTypeTag;

#endif /* __MIPI_TX_HI3403_REG_H__ */
