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

#ifndef __MIPI_TX_HI3403_REG_H_BODY_PART01_H__
#define __MIPI_TX_HI3403_REG_H_BODY_PART01_H__

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

#endif /* __MIPI_TX_HI3403_REG_H_BODY_PART01_H__ */
