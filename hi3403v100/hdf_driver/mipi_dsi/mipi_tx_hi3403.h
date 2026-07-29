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

#ifndef MIPI_TX_HI3403_H
#define MIPI_TX_HI3403_H

/****************************************************************************
 * hi3403v100 specific definitions
 ****************************************************************************/
#define MIPI_TX_REGS_ADDR   0x17A80000
#define MIPI_TX_REGS_SIZE   0x10000

#define MIPI_TX_IRQ         121

#define MIPI_TX_CRG         0x11018140

#define MIPI_TX_REF_CLK     27

#define TLPX                60
#define TCLK_PREPARE        60
#define TCLK_ZERO           260
#define TCLK_TRAIL          70
#define TCLK_POST           95
#define TPRE_DELAY          100
#define THS_PREPARE         80
#define THS_ZERO            180
#define THS_TRAIL           80

/* phy pll addr */
#define PLL_SET_0X63        0x63
#define PLL_SET_0X64        0x64
#define PLL_SET_0X65        0x65

/* phy timing reg addr */
#define HI3403_CLK_POST_DELAY      0x68
#define HI3403_DATA3_TPRE_DELAY    0x58
#define HI3403_DATA2_TPRE_DELAY    0x48
#define HI3403_DATA1_TPRE_DELAY    0x38
#define HI3403_DATA0_TPRE_DELAY    0x28

#define HI3403_DATA3_TLPX          0x50
#define HI3403_DATA3_THS_PREPARE   0x51
#define HI3403_DATA3_THS_ZERO      0x52
#define HI3403_DATA3_THS_TRAIL     0x53
#define HI3403_DATA2_TLPX          0x40
#define HI3403_DATA2_THS_PREPARE   0x41
#define HI3403_DATA2_THS_ZERO      0x42
#define HI3403_DATA2_THS_TRAIL     0x43
#define HI3403_DATA1_TLPX          0x30
#define HI3403_DATA1_THS_PREPARE   0x31
#define HI3403_DATA1_THS_ZERO      0x32
#define HI3403_DATA1_THS_TRAIL     0x33
#define HI3403_DATA0_TLPX          0x20
#define HI3403_DATA0_THS_PREPARE   0x21
#define HI3403_DATA0_THS_ZERO      0x22
#define HI3403_DATA0_THS_TRAIL     0x23

#define HI3403_CLK_TLPX            0x10
#define HI3403_CLK_TCLK_PREPARE    0x11
#define HI3403_CLK_TCLK_ZERO       0x12
#define HI3403_CLK_TCLK_TRAIL      0x13

#define HI3403_MIPI_TX_READ_TIMEOUT_CNT 1000

#define PREPARE_COMPENSATE    10
#define ROUNDUP_VALUE     7999
#define INNER_PEROID      8000

#define MIPI_TX_INTERRUPTS_ENABLE 0

/* PLL helper */
static inline unsigned int MipiTxCeil(unsigned int a, unsigned int b)
{
    return ((a) + (b) - 1) / (b);
}

typedef struct {
    unsigned char dataTpreDelay;
    unsigned char clkTlpx;
    unsigned char clkTclkPrepare;
    unsigned char clkTclkZero;
    unsigned char clkTclkTrail;
    unsigned char dataTlpx;
    unsigned char dataThsPrepare;
    unsigned char dataThsZero;
    unsigned char dataThsTrail;
    unsigned char clkPostDelay;
} MipiTxPhyTimingParamTag;

typedef struct {
    unsigned int vallDet;
    unsigned int vactDet;
    unsigned int hallDet;
    unsigned int hactDet;
    unsigned int hbpDet;
    unsigned int hsaDet;
    unsigned int vsaDet;
} MipiTxDevPhyTag;


#endif /* MIPI_TX_HI3403_H */
