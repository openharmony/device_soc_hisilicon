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

#include "mipi_tx_hi3403.h"
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/delay.h>
#include <asm/barrier.h>
#include "hdf_core_log.h"
#include "securec.h"
#include "osal_time.h"
#include "osal_io.h"
#include "osal_mem.h"
#include "mipi_dsi_define.h"
#include "mipi_dsi_core.h"
#include "mipi_tx_hi3403_reg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HDF_LOG_TAG mipi_tx_hi3403
#define INT_MAX_VALUE 0x7fffffff

/* Hardware-specific constants */
#define MIPI_TX_POWER_BASE         2
#define PLL_SET_DIVISOR            256
#define PHY_TMR_HS_LP_OFFSET       4
#define PHY_TMR_CLK_OFFSET         3
#define PHY_TMR_SCALE_FACTOR       65536
#define MIPI_TX_PHY_RESET_DELAY_US 1000
#define VID_MODE_TYPE_MASK         0x3
#define VID_MODE_TYPE_SHIFT        8
#define VID_MODE_TYPE_SYNC_PULSE   0
#define VID_MODE_TYPE_SYNC_EVENT   1
#define VID_MODE_TYPE_CMD          2
#define TIMING_UNIT_CONV           125
#define CLK_DIV_THRESHOLD          160
#define CLK_DIV_THRESHOLD_MINUS1   159
#define CLKMGR_MIN_DIV             2
#define REG_MAP_FLAG_HW_INIT       2
#define PLL_SET_MODULUS            8
#define CMD_CFG_DELAY_US           10

volatile  MipiTxRegsTypeTag *g_mipiTxRegsVa = NULL;
unsigned int g_mipiTxIrqNum = MIPI_TX_IRQ;
unsigned int g_actualPhyDataRate;
static unsigned int g_regMapFlag;
static bool g_enCfg = false;
static bool g_enHsMode = false;

/* ====== low-level helpers ====== */

static void WriteReg32(unsigned long *addr, unsigned int value, unsigned int mask)
{
    unsigned int t;

    t = (unsigned int)OSAL_READL(addr);
    t &= ~mask;
    t |= value & mask;
    OSAL_WRITEL(t, addr);
}

static void OsalIsb(void)
{
    isb();
}

static void OsalDsb(void)
{
    dsb(sy);
}

static void OsalDmb(void)
{
    dmb(sy);
}

static void HdfIsbDsbDmb(void)
{
    OsalIsb();
    OsalDsb();
    OsalDmb();
}

static void SetPhyReg(unsigned int addr, unsigned char value)
{
    HdfIsbDsbDmb();
    g_mipiTxRegsVa->phyTstCtrL1.u32 = (0x10000 + addr);
    HdfIsbDsbDmb();
    g_mipiTxRegsVa->phyTstCtrL0.u32 = 0x2;
    HdfIsbDsbDmb();
    g_mipiTxRegsVa->phyTstCtrL0.u32 = 0x0;
    HdfIsbDsbDmb();
    g_mipiTxRegsVa->phyTstCtrL1.u32 = value;
    HdfIsbDsbDmb();
    g_mipiTxRegsVa->phyTstCtrL0.u32 = 0x2;
    HdfIsbDsbDmb();
    g_mipiTxRegsVa->phyTstCtrL0.u32 = 0x0;
    HdfIsbDsbDmb();
}

static int MipiTxPower(unsigned int base, unsigned int exp)
{
    int result = 1;
    while (exp) {
        if (exp & 0x01) {
            result = result * (int)base;
        }
        base = base * base;
        exp = exp >> 1;
    }
    return result;
}

static inline unsigned char Hi3403GetPhyTimingByType(unsigned int timingType)
{
    return (unsigned char)((g_actualPhyDataRate * timingType + ROUNDUP_VALUE) / INNER_PEROID);
}

/* ====== hi3403 command mode / clock lane helpers ====== */

static void Hi3403SetCmdLpMode(void)
{
    g_mipiTxRegsVa->cmdModeCfg.u32 = 0xFFF;
}

static void Hi3403SetCmdHsMode(void)
{
    g_mipiTxRegsVa->cmdModeCfg.u32 = 0x0;
}

static void Hi3403SetClkLaneCfg(int hsEnable)
{
    U_LPCLK_CTRL clkLane;
    clkLane.u32 = g_mipiTxRegsVa->lpclkCtrl.u32;
    clkLane.bits.phyTxrequestclkhs = (unsigned int)hsEnable;
    clkLane.bits.autoClklaneCtrl = 0;
    g_mipiTxRegsVa->lpclkCtrl.u32 = clkLane.u32;
}

/* ====== hi3403 phy reset helpers ====== */

static void Hi3403PhyReset(void)
{
    U_PHY_RSTZ phyCtrl;
    phyCtrl.u32 = g_mipiTxRegsVa->phyRstz.u32;
    phyCtrl.bits.phyRstz = 0x0;
    g_mipiTxRegsVa->phyRstz.u32 = phyCtrl.u32;
}

static void Hi3403PhyUnreset(void)
{
    U_PHY_RSTZ phyCtrl;
    phyCtrl.u32 = g_mipiTxRegsVa->phyRstz.u32;
    phyCtrl.bits.phyRstz = 0x1;
    g_mipiTxRegsVa->phyRstz.u32 = phyCtrl.u32;
}

static void Hi3403ControllerPhyReset(void)
{
    g_mipiTxRegsVa->pwrUp.u32 = 0x0;
    Hi3403PhyReset();
    OsalUDelay(1);
    Hi3403PhyUnreset();
    g_mipiTxRegsVa->pwrUp.u32 = 0x1;
}

/* ====== hi3403 PLL config ====== */

static unsigned char Hi3403GetPhyPllSet063(unsigned int phyDataRate, unsigned char pllSet065)
{
    return (unsigned char)(MipiTxCeil(phyDataRate, MIPI_TX_REF_CLK) *
        MipiTxPower(MIPI_TX_POWER_BASE, (pllSet065 % PLL_SET_MODULUS)) % PLL_SET_DIVISOR);
}

static unsigned char Hi3403GetPhyPllSet064(unsigned int phyDataRate)
{
    return (unsigned char)(MipiTxCeil(phyDataRate, MIPI_TX_REF_CLK) / PLL_SET_DIVISOR);
}

static void MipiTxDrvSetPhyPllSetX(unsigned int phyDataRate)
{
    unsigned char pllSet063;
    unsigned char pllSet064;
    const unsigned char pllSet065 = 0x0;

    pllSet063 = Hi3403GetPhyPllSet063(phyDataRate, pllSet065);
    pllSet064 = Hi3403GetPhyPllSet064(phyDataRate);

    SetPhyReg(PLL_SET_0X63, pllSet063);
    SetPhyReg(PLL_SET_0X64, pllSet064);
    SetPhyReg(PLL_SET_0X65, pllSet065);
}

/* ====== hi3403 PHY timing config ====== */

static void MipiTxDrvGetPhyTimingParam(MipiTxPhyTimingParamTag *tp)
{
    tp->dataTpreDelay = Hi3403GetPhyTimingByType(TPRE_DELAY);
    tp->clkTlpx = Hi3403GetPhyTimingByType(TLPX);
    tp->clkTclkPrepare = Hi3403GetPhyTimingByType(TCLK_PREPARE);
    tp->clkTclkZero = Hi3403GetPhyTimingByType(TCLK_ZERO);
    tp->clkTclkTrail = Hi3403GetPhyTimingByType(TCLK_TRAIL);
    tp->dataTlpx = Hi3403GetPhyTimingByType(TLPX);
    tp->dataThsPrepare = Hi3403GetPhyTimingByType(THS_PREPARE);
    tp->dataThsZero = Hi3403GetPhyTimingByType(THS_ZERO);
    tp->dataThsTrail = Hi3403GetPhyTimingByType(THS_TRAIL);
    tp->clkPostDelay = Hi3403GetPhyTimingByType(TCLK_POST) + tp->dataThsTrail + PHY_TMR_HS_LP_OFFSET;
}

static void MipiTxDrvSetPhyTimingParam(const MipiTxPhyTimingParamTag *tp)
{
    SetPhyReg(HI3403_DATA0_TPRE_DELAY, tp->dataTpreDelay);
    SetPhyReg(HI3403_DATA1_TPRE_DELAY, tp->dataTpreDelay);
    SetPhyReg(HI3403_DATA2_TPRE_DELAY, tp->dataTpreDelay);
    SetPhyReg(HI3403_DATA3_TPRE_DELAY, tp->dataTpreDelay);
    SetPhyReg(HI3403_CLK_POST_DELAY, tp->clkPostDelay);
    SetPhyReg(HI3403_CLK_TLPX, tp->clkTlpx);
    SetPhyReg(HI3403_CLK_TCLK_PREPARE, tp->clkTclkPrepare);
    SetPhyReg(HI3403_CLK_TCLK_ZERO, tp->clkTclkZero);
    SetPhyReg(HI3403_CLK_TCLK_TRAIL, tp->clkTclkTrail);
    SetPhyReg(HI3403_DATA0_TLPX, tp->dataTlpx);
    SetPhyReg(HI3403_DATA0_THS_PREPARE, tp->dataThsPrepare);
    SetPhyReg(HI3403_DATA0_THS_ZERO, tp->dataThsZero);
    SetPhyReg(HI3403_DATA0_THS_TRAIL, tp->dataThsTrail);
    SetPhyReg(HI3403_DATA1_TLPX, tp->dataTlpx);
    SetPhyReg(HI3403_DATA1_THS_PREPARE, tp->dataThsPrepare);
    SetPhyReg(HI3403_DATA1_THS_ZERO, tp->dataThsZero);
    SetPhyReg(HI3403_DATA1_THS_TRAIL, tp->dataThsTrail);
    SetPhyReg(HI3403_DATA2_TLPX, tp->dataTlpx);
    SetPhyReg(HI3403_DATA2_THS_PREPARE, tp->dataThsPrepare);
    SetPhyReg(HI3403_DATA2_THS_ZERO, tp->dataThsZero);
    SetPhyReg(HI3403_DATA2_THS_TRAIL, tp->dataThsTrail);
    SetPhyReg(HI3403_DATA3_TLPX, tp->dataTlpx);
    SetPhyReg(HI3403_DATA3_THS_PREPARE, tp->dataThsPrepare);
    SetPhyReg(HI3403_DATA3_THS_ZERO, tp->dataThsZero);
    SetPhyReg(HI3403_DATA3_THS_TRAIL, tp->dataThsTrail);
}

static void MipiTxDrvSetPhyHsLpSwitchTime(const MipiTxPhyTimingParamTag *tp)
{
    g_mipiTxRegsVa->phyTmrCfg.u32 = tp->dataTpreDelay + tp->dataTlpx +
        tp->dataThsPrepare + tp->dataThsZero + PHY_TMR_HS_LP_OFFSET +
        (tp->dataThsTrail + 1) * PHY_TMR_SCALE_FACTOR;
    g_mipiTxRegsVa->phyTmrLpclkCfg.u32 = tp->clkTlpx + tp->clkTclkPrepare +
        tp->clkTclkZero + PHY_TMR_CLK_OFFSET +
        (tp->clkPostDelay + 1 + tp->dataThsTrail) * PHY_TMR_SCALE_FACTOR;
}

static void MipiTxDrvSetPhyCfg(const ComboDevCfgTag *cfg)
{
    MipiTxPhyTimingParamTag tp;

    (void)cfg;
    (void)memset_s(&tp, sizeof(tp), 0, sizeof(tp));

    /* Reset PHY */
    g_mipiTxRegsVa->phyRstz.u32 = 0x0;
    OsalUDelay(MIPI_TX_PHY_RESET_DELAY_US);

    /* Disable input */
    g_mipiTxRegsVa->operationMode.u32 = 0x0;

    /* ctrl_reset toggle */
    g_mipiTxRegsVa->pwrUp.u32 = 0x0;
    OsalUDelay(1);
    g_mipiTxRegsVa->pwrUp.u32 = 0x1;

    /* PLL + Timing + HS/LP switch */
    MipiTxDrvSetPhyPllSetX(g_actualPhyDataRate);
    MipiTxDrvGetPhyTimingParam(&tp);
    MipiTxDrvSetPhyTimingParam(&tp);
    MipiTxDrvSetPhyHsLpSwitchTime(&tp);

    /* PHY enable */
    g_mipiTxRegsVa->phyRstz.u32 = 0xF;
    OsalMSleep(1);
}

/* ====== output / timing / lane config ====== */

static void SetOutputFormat(const ComboDevCfgTag *cfg)
{
    int colorCoding = 0;

    if (cfg->outputMode == OUTPUT_MODE_CSI) {
        if (cfg->outputFormat == OUT_FORMAT_YUV420_8_BIT_NORMAL) {
            colorCoding = 0xD;
        } else if (cfg->outputFormat == OUT_FORMAT_YUV422_8_BIT) {
            colorCoding = 0x1E;
        }
    } else {
        if (cfg->outputFormat == OUT_FORMAT_RGB_16_BIT) {
            colorCoding = 0x0;
        } else if (cfg->outputFormat == OUT_FORMAT_RGB_18_BIT) {
            colorCoding = 0x3;
        } else if (cfg->outputFormat == OUT_FORMAT_RGB_24_BIT) {
            colorCoding = 0x5;
        } else {
            colorCoding = 0x5;
        }
    }
    g_mipiTxRegsVa->colorCoding.u32 = (unsigned int)colorCoding;
}

static void SetVideoModeCfg(const ComboDevCfgTag *cfg)
{
    unsigned int modeCfgVal;
    unsigned int videoModeType;
    U_READ_MEMORY_DELAY_CTRL readMemDelay;

    if (cfg->videoMode == NON_BURST_MODE_SYNC_PULSES) {
        videoModeType = VID_MODE_TYPE_SYNC_PULSE;
    } else if (cfg->videoMode == NON_BURST_MODE_SYNC_EVENTS) {
        videoModeType = VID_MODE_TYPE_SYNC_EVENT;
    } else {
        videoModeType = VID_MODE_TYPE_CMD;
    }

    if ((cfg->outputMode == OUTPUT_MODE_CSI) || (cfg->outputMode == OUTPUT_MODE_DSI_CMD)) {
        videoModeType = VID_MODE_TYPE_CMD;
    }

    /* mode_cfg: cmdVideoMode=1, video_mode_type at bits 9:8 */
    modeCfgVal = g_mipiTxRegsVa->modeCfg.u32;
    modeCfgVal &= ~(VID_MODE_TYPE_MASK << VID_MODE_TYPE_SHIFT);
    modeCfgVal |= (videoModeType << VID_MODE_TYPE_SHIFT);
    modeCfgVal |= 0x1;  /* cmdVideoMode = 1 */
    g_mipiTxRegsVa->modeCfg.u32 = modeCfgVal;

    /* video_lp_en = 0x3f */
    g_mipiTxRegsVa->vidModeCfg.u32 = 0x3f;

    /* read_memory_delay_ctrl */
    readMemDelay.u32 = g_mipiTxRegsVa->readMemoryDelayCtrl.u32;
    if ((cfg->videoMode == NON_BURST_MODE_SYNC_PULSES) ||
        (cfg->videoMode == NON_BURST_MODE_SYNC_EVENTS)) {
        readMemDelay.bits.delayAbnormal = 0x1;
    } else {
        readMemDelay.bits.delayAbnormal = 0x0;
    }
    g_mipiTxRegsVa->readMemoryDelayCtrl.u32 = readMemDelay.u32;
}

static void SetTimingConfig(const ComboDevCfgTag *cfg)
{
    unsigned int hsa;
    unsigned int hbp;
    unsigned int hline;
    unsigned int hact;
    unsigned int hfp;

    if (cfg->pixelClk == 0) {
        HDF_LOGE("SetTimingConfig: pixelClk is 0!");
        return;
    }
    hact = cfg->syncInfo.vidHsaPixels + cfg->syncInfo.vidHbpPixels + cfg->syncInfo.vidPktSize;
    hfp = cfg->syncInfo.vidHlinePixels - hact;
    hsa = (unsigned int)((unsigned long long)g_actualPhyDataRate *
        cfg->syncInfo.vidHsaPixels * TIMING_UNIT_CONV / cfg->pixelClk);
    hbp = (unsigned int)((unsigned long long)g_actualPhyDataRate *
        cfg->syncInfo.vidHbpPixels * TIMING_UNIT_CONV / cfg->pixelClk);
    hline = (unsigned int)((unsigned long long)g_actualPhyDataRate *
        (cfg->syncInfo.vidHsaPixels + cfg->syncInfo.vidHbpPixels +
         cfg->syncInfo.vidPktSize + hfp) * TIMING_UNIT_CONV / cfg->pixelClk);

    g_mipiTxRegsVa->vidHsaTime.u32 = hsa;
    g_mipiTxRegsVa->vidHbpTime.u32 = hbp;
    g_mipiTxRegsVa->vidHlineTime.u32 = hline;
    g_mipiTxRegsVa->vidVsaLines.u32 = cfg->syncInfo.vidVsaLines;
    g_mipiTxRegsVa->vidVbpLines.u32 = cfg->syncInfo.vidVbpLines;
    g_mipiTxRegsVa->vidVfpLines.u32 = cfg->syncInfo.vidVfpLines;
    g_mipiTxRegsVa->vidVactiveLines.u32 = cfg->syncInfo.vidActiveLines;

    HDF_LOGI("%s: hsa=%u hbp=%u hline=%u vsa=%u vbp=%u vfp=%u vact=%u",
        __func__, hsa, hbp, hline,
        cfg->syncInfo.vidVsaLines, cfg->syncInfo.vidVbpLines,
        cfg->syncInfo.vidVfpLines, cfg->syncInfo.vidActiveLines);
}

static void SetLaneConfig(const short laneId[], int len)
{
    int num = 0;
    int i;

    for (i = 0; i < len; i++) {
        if (laneId[i] != MIPI_TX_DISABLE_LANE_ID) {
            num++;
        }
    }
    /* Preserve HW reset lane_id mapping, only update lane count */
    g_mipiTxRegsVa->phyIfCfg.u32 = (unsigned int)(num - 1);
    HDF_LOGI("%s: lane count=%d", __func__, num);
}

static void MipiTxDrvSetClkMgrCfg(void)
{
    if (g_actualPhyDataRate / CLK_DIV_THRESHOLD < CLKMGR_MIN_DIV) {
        g_mipiTxRegsVa->clkmgrCfg.u32 = 0x102;
    } else {
        g_mipiTxRegsVa->clkmgrCfg.u32 = 0x100 + (g_actualPhyDataRate + CLK_DIV_THRESHOLD_MINUS1) / CLK_DIV_THRESHOLD;
    }
}
/* ====== command / read FIFO helpers ====== */

static void MipiTxDrvSetControllerCfg(const ComboDevCfgTag *cfg)
{
    if (cfg == NULL) {
        HDF_LOGE("MipiTxDrvSetControllerCfg: cfg is null!");
        return;
    }

    /* 1. interrupt mask */
    g_mipiTxRegsVa->gintMsk = 0x1ffff;

    /* 2. disable input */
    g_mipiTxRegsVa->operationMode.u32 = 0x0;

    /* 3. vc_id */
    g_mipiTxRegsVa->vcid.u32 = 0x0;

    /* 4. output format, color coding */
    SetOutputFormat(cfg);

    /* 5. clock manager */
    g_actualPhyDataRate = ((cfg->phyDataRate + MIPI_TX_REF_CLK - 1) / MIPI_TX_REF_CLK) * MIPI_TX_REF_CLK;
    MipiTxDrvSetClkMgrCfg();

    /* 6. command transmission mode — LP mode */
    Hi3403SetCmdLpMode();

    /* 7. pck_en */
    if (cfg->outputMode == OUTPUT_MODE_CSI) {
        g_mipiTxRegsVa->pckhdlCfg.u32 = 0x0C;
    } else {
        g_mipiTxRegsVa->pckhdlCfg.u32 = 0x1E;
    }

    /* 8. genVc */
    g_mipiTxRegsVa->genVcid.u32 = 0x0;

    /* 9. video mode cfg */
    SetVideoModeCfg(cfg);

    /* 10. pkt size */
    if ((cfg->outputMode == OUTPUT_MODE_DSI_VIDEO) || (cfg->outputMode == OUTPUT_MODE_CSI)) {
        g_mipiTxRegsVa->vidPktSize.u32 = cfg->syncInfo.vidPktSize;
    } else {
        g_mipiTxRegsVa->edpiCmdSize.u32 = cfg->syncInfo.vidPktSize;
    }

    /* 11. num_chunks / null_size */
    g_mipiTxRegsVa->vidNumChunks.u32 = 0x0;
    g_mipiTxRegsVa->vidNullSize.u32 = 0x0;

    /* 12. timing config */
    SetTimingConfig(cfg);

    /* 13. lp_cmd_tim */
    g_mipiTxRegsVa->lpCmdTim.u32 = 0xff0000;

    /* 14. DSI CMD / CSI extra settings */
    if (cfg->outputMode == OUTPUT_MODE_CSI) {
        g_mipiTxRegsVa->edpiCmdSize.u32 = 0;
        g_mipiTxRegsVa->hsrdToSet.u32 = 0x0;
    }
    g_mipiTxRegsVa->hsLpToSet.u32 = 0x0;
    g_mipiTxRegsVa->lprdToSet.u32 = 0x0;
    g_mipiTxRegsVa->hswrToSet.u32 = 0x0;
    g_mipiTxRegsVa->lpwrToSet.u32 = 0x0;
    g_mipiTxRegsVa->btaToSet.u32 = 0x0;
    g_mipiTxRegsVa->readCmdTime.u32 = 0x7fff;

    /* 15. datatype0/1, csi_ctrl */
    g_mipiTxRegsVa->datatypE0.u32 = 0x111213D;
    g_mipiTxRegsVa->datatypE1.u32 = 0x31081909;
    g_mipiTxRegsVa->csiCtrl.u32 = 0x10100;

    /* 16. lane config */
    SetLaneConfig(cfg->laneId, LANE_MAX_NUM);

    /* 17. ulps_ctrl */
    g_mipiTxRegsVa->phyUlpsCtrl.u32 = 0x0;

    /* 18. txTriggers */
    g_mipiTxRegsVa->txTriggers.u32 = 0x0;

    /* 19. vid_shadow_ctrl */
    g_mipiTxRegsVa->vidShadowCtrl.u32 = 0x0;

    /* 20. int0_mask */
    g_mipiTxRegsVa->intMsK0.u32 = 0x0;

    /* 21. clklane_continue disable */
    Hi3403SetClkLaneCfg(0);
}

/* ====== clock manager config ====== */

static int MipiTxWaitCmdFifoEmpty(void)
{
    U_CMD_PKT_STATUS cmdPktStatus;
    unsigned int waitCnt;

    waitCnt = 0;
    do {
        cmdPktStatus.u32 = g_mipiTxRegsVa->cmdPktStatus.u32;
        waitCnt++;
        OsalUDelay(1);
        if (waitCnt >  HI3403_MIPI_TX_READ_TIMEOUT_CNT) {
            HDF_LOGW("MipiTxWaitCmdFifoEmpty: timeout when send cmd buffer!");
            return HDF_ERR_TIMEOUT;
        }
    } while (cmdPktStatus.bits.genCmdEmpty == 0);
    return HDF_SUCCESS;
}

static int MipiTxWaitWriteFifoEmpty(void)
{
    U_CMD_PKT_STATUS cmdPktStatus;
    unsigned int waitCnt;

    waitCnt = 0;
    do {
        cmdPktStatus.u32 = g_mipiTxRegsVa->cmdPktStatus.u32;
        waitCnt++;
        OsalUDelay(1);
        if (waitCnt >  HI3403_MIPI_TX_READ_TIMEOUT_CNT) {
            HDF_LOGW("MipiTxWaitWriteFifoEmpty: timeout when send data buffer!");
            return HDF_ERR_TIMEOUT;
        }
    } while (cmdPktStatus.bits.genPldWEmpty == 0);
    return HDF_SUCCESS;
}

static int MipiTxWaitWriteFifoNotFull(void)
{
    U_CMD_PKT_STATUS cmdPktStatus;
    unsigned int waitCnt;

    waitCnt = 0;
    do {
        cmdPktStatus.u32 = g_mipiTxRegsVa->cmdPktStatus.u32;
        if (waitCnt > 0) {
            OsalUDelay(1);
            HDF_LOGW("MipiTxWaitWriteFifoNotFull: write fifo full happened wait count = %u!", waitCnt);
        }
        if (waitCnt >  HI3403_MIPI_TX_READ_TIMEOUT_CNT) {
            HDF_LOGW("MipiTxWaitWriteFifoNotFull: timeout when wait write fifo not full buffer!");
            return HDF_ERR_TIMEOUT;
        }
        waitCnt++;
    } while (cmdPktStatus.bits.genPldWFull == 1);
    return HDF_SUCCESS;
}

/*
 * set payloads data by writing register
 * each 4 bytes in cmd corresponds to one register
 */
static void MipiTxDrvSetPayloadData(const unsigned char *cmd, unsigned short cmdSize)
{
    int32_t ret;
    U_GEN_PLD_DATA genPldData;
    int i;
    int j;

    genPldData.u32 = g_mipiTxRegsVa->genPldData.u32;

    for (i = 0; i < (cmdSize / 4); i++) { /* 4 cmd once */
        genPldData.bits.genPldB1 = cmd[i * 4]; /* 0 in 4 */
        genPldData.bits.genPldB2 = cmd[i * 4 + 1]; /* 1 in 4 */
        genPldData.bits.genPldB3 = cmd[i * 4 + 2]; /* 2 in 4 */
        genPldData.bits.genPldB4 = cmd[i * 4 + 3]; /* 3 in 4 */
        ret = MipiTxWaitWriteFifoNotFull();
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("MipiTxDrvSetPayloadData: [MipiTxWaitWriteFifoNotFull] fail!");
            return;
        }
        g_mipiTxRegsVa->genPldData.u32 = genPldData.u32;
    }
    j = cmdSize % 4; /* remainder of 4 */
    if (j != 0) {
        if (j > 0) {
            genPldData.bits.genPldB1 = cmd[i * 4]; /* 0 in 4 */
        }
        if (j > 1) {
            genPldData.bits.genPldB2 = cmd[i * 4 + 1]; /* 1 in 4 */
        }
        if (j > 2) { /* bigger than 2 */
            genPldData.bits.genPldB3 = cmd[i * 4 + 2]; /* 2 in 4 */
        }
        ret = MipiTxWaitWriteFifoNotFull();
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("MipiTxDrvSetPayloadData: [MipiTxWaitWriteFifoNotFull] fail!");
            return;
        }
        g_mipiTxRegsVa->genPldData.u32 = genPldData.u32;
    }
}

static int32_t LinuxCopyToKernel(void *dest, uint32_t max, const void *src, uint32_t count)
{
    int32_t ret;

    if (access_ok(
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
        VERIFY_READ,
#endif
        src, count)) { /* user space */
        ret = (copy_from_user(dest, src, count) != 0) ? HDF_FAILURE : HDF_SUCCESS;
        if (ret == HDF_FAILURE) {
            HDF_LOGE("LinuxCopyToKernel: [copy_from_user] fail!");
        }
    } else { /* kernel space */
        ret = (memcpy_s(dest, max, src, count) != EOK) ? HDF_FAILURE : HDF_SUCCESS;
        if (ret == HDF_FAILURE) {
            HDF_LOGE("LinuxCopyToKernel: [memcpy_s] fail!");
        }
    }

    return ret;
}

static int MipiTxDrvSetCmdInfo(const CmdInfoTag *cmdInfo)
{
    int32_t ret;
    U_GEN_HDR genHdr;
    unsigned char *cmd = NULL;

    if (cmdInfo == NULL) {
        HDF_LOGE("MipiTxDrvSetCmdInfo: cmdInfo is null!");
        return HDF_ERR_INVALID_OBJECT;
    }
    Hi3403SetCmdLpMode();
    Hi3403SetClkLaneCfg(0); /* LP: disable HS clock request */
    genHdr.u32 = g_mipiTxRegsVa->genHdr.u32;
    if (cmdInfo->cmd != NULL) {
        if ((cmdInfo->cmdSize > 200) || (cmdInfo->cmdSize == 0)) { /* 200 is max cmd size */
            HDF_LOGE("MipiTxDrvSetCmdInfo: set cmd size illegal, size =%u!", cmdInfo->cmdSize);
            return HDF_ERR_INVALID_PARAM;
        }
        cmd = (unsigned char *)OsalMemCalloc(cmdInfo->cmdSize);
        if (cmd == NULL) {
            HDF_LOGE("MipiTxDrvSetCmdInfo: OsalMemCalloc fail,please check,need %u bytes!", cmdInfo->cmdSize);
            return HDF_ERR_MALLOC_FAIL;
        }
        ret = LinuxCopyToKernel(cmd, cmdInfo->cmdSize, cmdInfo->cmd, cmdInfo->cmdSize);
        if (ret == HDF_SUCCESS) {
            MipiTxDrvSetPayloadData(cmd, cmdInfo->cmdSize);
        }
        OsalMemFree(cmd);
        cmd = NULL;
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("MipiTxDrvSetCmdInfo: [LinuxCopyToKernel] fail!");
            return HDF_ERR_IO;
        }
    }
    genHdr.bits.genDt = cmdInfo->dataType;
    genHdr.bits.genWcLsbyte = cmdInfo->cmdSize & 0xff;
    genHdr.bits.genWcMsbyte = (cmdInfo->cmdSize & 0xff00) >> 8; /* height 8 bits */
    g_mipiTxRegsVa->genHdr.u32 = genHdr.u32;
    OsalUDelay(350);  /* wait 350 us transfer end */
    ret = MipiTxWaitCmdFifoEmpty();
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("MipiTxDrvSetCmdInfo: [MipiTxWaitCmdFifoEmpty] fail!");
        return HDF_FAILURE;
    }
    ret = MipiTxWaitWriteFifoEmpty();
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("MipiTxDrvSetCmdInfo: [MipiTxWaitWriteFifoEmpty] fail!");
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int MipiTxWaitReadFifoNotEmpty(void)
{
    U_INT_ST0 intSt0;
    U_INT_ST1 intSt1;
    unsigned int waitCnt;
    U_CMD_PKT_STATUS cmdPktStatus;

    waitCnt = 0;
    do {
        intSt1.u32 =  g_mipiTxRegsVa->intST1.u32;
        intSt0.u32 =  g_mipiTxRegsVa->intST0.u32;
        if ((intSt1.u32 & 0x3e) != 0) {
            HDF_LOGE("MipiTxWaitReadFifoNotEmpty: err happened when read data, int_st1 = 0x%x,int_st0 = %x!",
                intSt1.u32, intSt0.u32);
            return HDF_FAILURE;
        }
        if (waitCnt >  HI3403_MIPI_TX_READ_TIMEOUT_CNT) {
            HDF_LOGW("MipiTxWaitReadFifoNotEmpty: timeout when read data!");
            return HDF_ERR_TIMEOUT;
        }
        waitCnt++;
        OsalUDelay(1);
        cmdPktStatus.u32 = g_mipiTxRegsVa->cmdPktStatus.u32;
    } while (cmdPktStatus.bits.genPldREmpty == 0x1);
    return HDF_SUCCESS;
}

static int MipiTxWaitReadFifoEmpty(void)
{
    U_GEN_PLD_DATA pldData;
    U_INT_ST1 intSt1;
    unsigned int waitCnt;

    waitCnt = 0;
    do {
        intSt1.u32 = g_mipiTxRegsVa->intST1.u32;
        if ((intSt1.bits.genPldRdErr) == 0x0) {
            pldData.u32 = g_mipiTxRegsVa->genPldData.u32;
        }
        waitCnt++;
        OsalUDelay(1);
        if (waitCnt >  HI3403_MIPI_TX_READ_TIMEOUT_CNT) {
            HDF_LOGW("MipiTxWaitReadFifoEmpty: timeout when clear data buffer, the last read data is 0x%x!",
                pldData.u32);
            return HDF_ERR_TIMEOUT;
        }
    } while ((intSt1.bits.genPldRdErr) == 0x0);
    return HDF_SUCCESS;
}

static int MipiTxSendShortPacket(unsigned char virtualChannel,
    short unsigned dataType, unsigned short  dataParam)
{
    U_GEN_HDR genHdr;

    genHdr.bits.genVc = virtualChannel;
    genHdr.bits.genDt = dataType;
    genHdr.bits.genWcLsbyte = (dataParam & 0xff);
    genHdr.bits.genWcMsbyte = (dataParam & 0xff00) >> 8; /* height 8 bits */
    g_mipiTxRegsVa->genHdr.u32 = genHdr.u32;
    if (MipiTxWaitCmdFifoEmpty() != HDF_SUCCESS) {
        HDF_LOGE("MipiTxSendShortPacket: [MipiTxWaitCmdFifoEmpty] fail!");
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int MipiTxGetReadFifoData(unsigned int getDataSize, unsigned char *dataBuf)
{
    U_GEN_PLD_DATA pldData;
    unsigned int i;
    unsigned int j;

    for (i = 0; i < getDataSize / 4; i++) {   /* 4byte once */
        if (MipiTxWaitReadFifoNotEmpty() != HDF_SUCCESS) {
            HDF_LOGE("MipiTxGetReadFifoData: [MipiTxWaitReadFifoNotEmpty] fail at first!");
            return HDF_FAILURE;
        }
        pldData.u32 = g_mipiTxRegsVa->genPldData.u32;
        dataBuf[i * 4] = pldData.bits.genPldB1;     /* 0 in 4 */
        dataBuf[i * 4 + 1] = pldData.bits.genPldB2; /* 1 in 4 */
        dataBuf[i * 4 + 2] = pldData.bits.genPldB3; /* 2 in 4 */
        dataBuf[i * 4 + 3] = pldData.bits.genPldB4; /* 3 in 4 */
    }

    j = getDataSize % 4; /* remainder of 4 */

    if (j != 0) {
        if (MipiTxWaitReadFifoNotEmpty() != HDF_SUCCESS) {
            HDF_LOGE("MipiTxGetReadFifoData: [MipiTxWaitReadFifoNotEmpty] fail at second!");
            return HDF_FAILURE;
        }
        pldData.u32 = g_mipiTxRegsVa->genPldData.u32;
        if (j > 0) {
            dataBuf[i * 4] = pldData.bits.genPldB1; /* 0 in 4 */
        }
        if (j > 1) {
            dataBuf[i * 4 + 1] = pldData.bits.genPldB2; /* 1 in 4 */
        }
        if (j > 2) { /* bigger than 2 */
            dataBuf[i * 4 + 2] = pldData.bits.genPldB3; /* 2 in 4 */
        }
    }
    return HDF_SUCCESS;
}

/* ====== MIPI TX reset ====== */

static void MipiTxReset(void)
{
    U_OPERATION_MODE operationMode;
    U_MODE_CFG modeCfg;

    /* disable input */
    operationMode.u32 = g_mipiTxRegsVa->operationMode.u32;
    operationMode.bits.inputEn = 0;
    g_mipiTxRegsVa->operationMode.u32 = operationMode.u32;

    /* set to LP + command mode */
    Hi3403SetCmdLpMode();
    modeCfg.u32 = g_mipiTxRegsVa->modeCfg.u32;
    modeCfg.bits.cmdVideoMode = 1;
    g_mipiTxRegsVa->modeCfg.u32 = modeCfg.u32;
    Hi3403SetClkLaneCfg(0);

    /* phy + controller reset */
    Hi3403ControllerPhyReset();
}
static int MipiTxDrvGetCmdInfo(GetCmdInfoTag *getCmdInfo)
{
    unsigned char *dataBuf = NULL;

    dataBuf = (unsigned char*)OsalMemAlloc(getCmdInfo->getDataSize);
    if (dataBuf == NULL) {
        HDF_LOGE("MipiTxDrvGetCmdInfo: dataBuf is null!");
        return HDF_ERR_MALLOC_FAIL;
    }
    if (MipiTxWaitReadFifoEmpty() != HDF_SUCCESS) {
        HDF_LOGE("MipiTxDrvGetCmdInfo: [MipiTxWaitReadFifoEmpty] fail!");
        goto fail0;
    }
    if (MipiTxSendShortPacket(0, getCmdInfo->dataType, getCmdInfo->dataParam) != HDF_SUCCESS) {
        HDF_LOGE("MipiTxDrvGetCmdInfo: [MipiTxSendShortPacket] fail!");
        goto fail0;
    }
    if (MipiTxGetReadFifoData(getCmdInfo->getDataSize, dataBuf) != HDF_SUCCESS) {
        /* fail will block mipi data lane, so need reset */
        MipiTxReset();
        HDF_LOGE("MipiTxDrvGetCmdInfo: [MipiTxGetReadFifoData] fail!");
        goto fail0;
    }
    if (access_ok(
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
        VERIFY_WRITE,
#endif
        getCmdInfo->getData, getCmdInfo->getDataSize)) { /* user space */
        if (copy_to_user(getCmdInfo->getData, dataBuf, getCmdInfo->getDataSize) != 0) {
            HDF_LOGE("MipiTxDrvGetCmdInfo: copy_to_user fail");
            goto fail0;
        }
    } else { /* kernel space */
        if (memcpy_s(getCmdInfo->getData, getCmdInfo->getDataSize, dataBuf, getCmdInfo->getDataSize) != EOK) {
            HDF_LOGE("MipiTxDrvGetCmdInfo: memcpy_s fail");
            goto fail0;
        }
    }
    OsalMemFree(dataBuf);
    dataBuf = NULL;
    return HDF_SUCCESS;

fail0:
    OsalMemFree(dataBuf);
    dataBuf = NULL;
    return HDF_FAILURE;
}
static void MipiTxDrvEnableInput(const OutPutModeTag outputMode)
{
    U_OPERATION_MODE opMode;

    if ((outputMode == OUTPUT_MODE_DSI_VIDEO) || (outputMode == OUTPUT_MODE_CSI)) {
        U_MODE_CFG modeCfg;
        modeCfg.u32 = g_mipiTxRegsVa->modeCfg.u32;
        modeCfg.bits.cmdVideoMode = 0;
        g_mipiTxRegsVa->modeCfg.u32 = modeCfg.u32;
    }

    if (outputMode == OUTPUT_MODE_DSI_CMD) {
        Hi3403SetCmdHsMode();
    }

    OsalUDelay(1);
    Hi3403SetClkLaneCfg(1); /* HS clock enable */
    OsalUDelay(1);

    /* enable input */
    opMode.u32 = g_mipiTxRegsVa->operationMode.u32;
    opMode.bits.memCkEn = 1;
    opMode.bits.inputEn = 1;
    opMode.bits.hssAbnormalRst = 1;
    opMode.bits.readEmptyVsyncEn = 1;
    g_mipiTxRegsVa->operationMode.u32 = opMode.u32;

    /* reset controller + phy */
    Hi3403ControllerPhyReset();
}

static void MipiTxDrvDisableInput(void)
{
    U_OPERATION_MODE operationMode;
    U_MODE_CFG modeCfg;

    /* disable input */
    operationMode.u32 = g_mipiTxRegsVa->operationMode.u32;
    operationMode.bits.inputEn = 0;
    g_mipiTxRegsVa->operationMode.u32 = operationMode.u32;

    /* set to LP + command mode */
    Hi3403SetCmdLpMode();
    modeCfg.u32 = g_mipiTxRegsVa->modeCfg.u32;
    modeCfg.bits.cmdVideoMode = 1;
    g_mipiTxRegsVa->modeCfg.u32 = modeCfg.u32;
    Hi3403SetClkLaneCfg(0);

    /* phy + controller reset */
    Hi3403ControllerPhyReset();
}
/* ====== register / HW init ====== */

static int MipiTxDrvRegInit(void)
{
    if (!g_mipiTxRegsVa) {
        g_mipiTxRegsVa = (MipiTxRegsTypeTag *)OsalIoRemap(MIPI_TX_REGS_ADDR, (unsigned int)MIPI_TX_REGS_SIZE);
        if (g_mipiTxRegsVa == NULL) {
            HDF_LOGE("MipiTxDrvRegInit: remap mipi_tx reg addr fail!");
            return HDF_FAILURE;
        }
        g_regMapFlag = 1;
    }

    return HDF_SUCCESS;
}

static void MipiTxDrvRegExit(void)
{
    if (g_regMapFlag == 1) {
        if (g_mipiTxRegsVa != NULL) {
            OsalIoUnmap((void *)g_mipiTxRegsVa);
            g_mipiTxRegsVa = NULL;
        }
        g_regMapFlag = 0;
    }
}

static void MipiTxDrvHwInit(int smooth)
{
    unsigned long *mipiTxCrgAddr;

    mipiTxCrgAddr = (unsigned long *)OsalIoRemap(MIPI_TX_CRG, (unsigned long)0x4);
    if (mipiTxCrgAddr == NULL) {
        HDF_LOGE("%s: OsalIoRemap CRG failed", __func__);
        return;
    }
    /* mipi_tx gate clk enable */
    WriteReg32(mipiTxCrgAddr, 1, 0x1);
    /* reset */
    if (smooth == 0) {
        WriteReg32(mipiTxCrgAddr, 1 << 1, 0x1 << 1);
    }
    /* unreset */
    WriteReg32(mipiTxCrgAddr, 0 << 1, 0x1 << 1);
    /* ref clk */
    WriteReg32(mipiTxCrgAddr, 1 << 2, 0x1 << 2); /* 2 clk bit */
    OsalIoUnmap((void *)mipiTxCrgAddr);
}

static int MipiTxDrvInit(int smooth)
{
    int32_t ret;

    ret = MipiTxDrvRegInit();
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("MipiTxDrvInit: MipiTxDrvRegInit fail!");
        return HDF_FAILURE;
    }
    MipiTxDrvHwInit(smooth);
    return HDF_SUCCESS;
}

static void MipiTxDrvExit(void)
{
    MipiTxDrvRegExit();
}

/* ====== combo dev cfg ====== */

static ComboDevCfgTag *GetDevCfg(struct MipiDsiCntlr *cntlr)
{
    static ComboDevCfgTag dev;
    int i;

    if (cntlr == NULL) {
        HDF_LOGE("GetDevCfg: cntlr is null!");
        return NULL;
    }
    dev.devno = cntlr->devNo;
    dev.outputMode = (OutPutModeTag)cntlr->cfg.mode;
    dev.videoMode = (VideoModeTag)cntlr->cfg.burstMode;
    dev.outputFormat = (OutputFormatTag)cntlr->cfg.format;
    dev.syncInfo.vidPktSize = cntlr->cfg.timing.xPixels;
    dev.syncInfo.vidHsaPixels = cntlr->cfg.timing.hsaPixels;
    dev.syncInfo.vidHbpPixels = cntlr->cfg.timing.hbpPixels;
    dev.syncInfo.vidHlinePixels = cntlr->cfg.timing.hlinePixels;
    dev.syncInfo.vidVsaLines = cntlr->cfg.timing.vsaLines;
    dev.syncInfo.vidVbpLines = cntlr->cfg.timing.vbpLines;
    dev.syncInfo.vidVfpLines = cntlr->cfg.timing.vfpLines;
    dev.syncInfo.vidActiveLines = cntlr->cfg.timing.ylines;
    dev.syncInfo.edpiCmdSize = cntlr->cfg.timing.edpiCmdSize;
    dev.phyDataRate = cntlr->cfg.phyDataRate;
    dev.pixelClk = cntlr->cfg.pixelClk;
    for (i = 0; i < LANE_MAX_NUM; i++) {
        dev.laneId[i] = -1;   /* -1 : not use */
    }
    for (i = 0; i < cntlr->cfg.lane; i++) {
        dev.laneId[i] = i;
    }
    return &dev;
}

static int MipiTxCheckCombDevCfg(const ComboDevCfgTag *devCfg)
{
    int i;
    int validLaneId[LANE_MAX_NUM] = {0, 1, 2, 3};

    if (g_enHsMode) {
        HDF_LOGE("MipiTxCheckCombDevCfg: mipi_tx dev has enable!");
        return HDF_FAILURE;
    }
    if (devCfg->devno != 0) {
        HDF_LOGE("MipiTxCheckCombDevCfg: mipi_tx dev devno err!");
        return HDF_ERR_INVALID_PARAM;
    }
    for (i = 0; i < LANE_MAX_NUM; i++) {
        if ((devCfg->laneId[i] != validLaneId[i]) && (devCfg->laneId[i] != MIPI_TX_DISABLE_LANE_ID)) {
            HDF_LOGE("MipiTxCheckCombDevCfg: mipi_tx dev laneId %d err!", devCfg->laneId[i]);
            return HDF_ERR_INVALID_PARAM;
        }
    }
    if ((devCfg->outputMode != OUTPUT_MODE_CSI) && (devCfg->outputMode != OUTPUT_MODE_DSI_VIDEO) &&
        (devCfg->outputMode != OUTPUT_MODE_DSI_CMD)) {
        HDF_LOGE("MipiTxCheckCombDevCfg: mipi_tx dev outputMode %d err!", devCfg->outputMode);
        return HDF_ERR_INVALID_PARAM;
    }
    if ((devCfg->videoMode != BURST_MODE) && (devCfg->videoMode != NON_BURST_MODE_SYNC_PULSES) &&
        (devCfg->videoMode != NON_BURST_MODE_SYNC_EVENTS)) {
        HDF_LOGE("MipiTxCheckCombDevCfg: mipi_tx dev videoMode %d err!", devCfg->videoMode);
        return HDF_ERR_INVALID_PARAM;
    }
    if ((devCfg->outputFormat != OUT_FORMAT_RGB_16_BIT) && (devCfg->outputFormat != OUT_FORMAT_RGB_18_BIT) &&
        (devCfg->outputFormat != OUT_FORMAT_RGB_24_BIT) && (devCfg->outputFormat !=
        OUT_FORMAT_YUV420_8_BIT_NORMAL) && (devCfg->outputFormat != OUT_FORMAT_YUV420_8_BIT_LEGACY) &&
        (devCfg->outputFormat != OUT_FORMAT_YUV422_8_BIT)) {
        HDF_LOGE("MipiTxCheckCombDevCfg: mipi_tx dev outputFormat %d err!", devCfg->outputFormat);
        return HDF_ERR_INVALID_PARAM;
    }

    return HDF_SUCCESS;
}

static int MipiTxSetComboDevCfg(const ComboDevCfgTag *devCfg)
{
    int32_t ret;

    ret = MipiTxCheckCombDevCfg(devCfg);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("MipiTxSetComboDevCfg: check combo_dev config fail!");
        return ret;
    }

    /* Lazy HW init: defer CRG clock+reset to first use */
    if (!g_regMapFlag || g_regMapFlag == 1) {
        MipiTxDrvHwInit(0);
        g_regMapFlag = REG_MAP_FLAG_HW_INIT;
    }

    /* hi3403 HAL flow: disable first, then controller_cfg + phy_cfg */
    MipiTxDrvDisableInput();
    OsalUDelay(CMD_CFG_DELAY_US);

    MipiTxDrvSetControllerCfg(devCfg);
    MipiTxDrvSetPhyCfg(devCfg);
    g_enCfg = true;
    return ret;
}

/* ====== HDF driver ops ====== */

static int32_t Hi3403SetCntlrCfg(struct MipiDsiCntlr *cntlr)
{
    ComboDevCfgTag *dev = GetDevCfg(cntlr);

    if (dev == NULL) {
        HDF_LOGE("Hi3403SetCntlrCfg: dev is null!");
        return HDF_ERR_INVALID_OBJECT;
    }
    return MipiTxSetComboDevCfg(dev);
}

static int MipiTxCheckSetCmdInfo(const CmdInfoTag *cmdInfo)
{
    if (g_enHsMode) {
        HDF_LOGE("MipiTxCheckSetCmdInfo: mipi_tx dev has enable!");
        return HDF_FAILURE;
    }

    if (!g_enCfg) {
        HDF_LOGE("MipiTxCheckSetCmdInfo: mipi_tx dev has not config!");
        return HDF_FAILURE;
    }
    if (cmdInfo->devno != 0) {
        HDF_LOGE("MipiTxCheckSetCmdInfo: mipi_tx devno %d err!", cmdInfo->devno);
        return HDF_ERR_INVALID_PARAM;
    }
    if (cmdInfo->cmd != NULL) {
        if (cmdInfo->cmdSize > MIPI_TX_SET_DATA_SIZE) {
            HDF_LOGE("MipiTxCheckSetCmdInfo: mipi_tx dev cmd_size %d err!", cmdInfo->cmdSize);
            return HDF_ERR_INVALID_PARAM;
        }
    }
    return HDF_SUCCESS;
}

static int MipiTxSetCmd(const CmdInfoTag *cmdInfo)
{
    int32_t ret;
    if (cmdInfo == NULL) {
        HDF_LOGE("MipiTxSetCmd: cmdInfo is null!");
        return HDF_ERR_INVALID_OBJECT;
    }
    ret = MipiTxCheckSetCmdInfo(cmdInfo);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("MipiTxSetCmd: mipi_tx check combo_dev config fail!");
        return ret;
    }
    return MipiTxDrvSetCmdInfo(cmdInfo);
}

static int32_t Hi3403SetCmd(struct MipiDsiCntlr *cntlr, struct DsiCmdDesc *cmd)
{
    CmdInfoTag cmdInfo;

    (void)cntlr;
    if (cmd == NULL) {
        HDF_LOGE("Hi3403SetCmd: cmd is null!");
        return HDF_ERR_INVALID_OBJECT;
    }
    cmdInfo.devno = 0;
    if (cmd->dataLen > 2) {                     /* 2: use long data type */
        cmdInfo.cmdSize = cmd->dataLen;
        cmdInfo.dataType = cmd->dataType;       /* 0x29: long data type */
        cmdInfo.cmd = cmd->payload;
    } else if (cmd->dataLen == 2) {             /* 2: use short data type */
        uint16_t tmp = cmd->payload[1];         /* 3: payload */
        tmp = (tmp & 0x00ff) << 8;              /* 0x00ff , 8: payload to high */
        tmp = 0xff00 & tmp;
        tmp = tmp | cmd->payload[0];            /* 2: reg addr */
        cmdInfo.cmdSize = tmp;
        cmdInfo.dataType = cmd->dataType;       /* 0x23: short data type */
        cmdInfo.cmd = NULL;
    } else if (cmd->dataLen == 1) {
        cmdInfo.cmdSize = cmd->payload[0];      /* 2: reg addr */
        cmdInfo.dataType = cmd->dataType;       /* 0x05: short data type */
        cmdInfo.cmd = NULL;
    } else {
        HDF_LOGE("Hi3403SetCmd: dataLen error!");
        return HDF_ERR_INVALID_PARAM;
    }
    return MipiTxSetCmd(&cmdInfo);
}

static int MipiTxCheckGetCmdInfo(const GetCmdInfoTag *getCmdInfo)
{
    if (g_enHsMode) {
        HDF_LOGE("MipiTxCheckGetCmdInfo: mipi_tx dev has enable!");
        return HDF_FAILURE;
    }

    if (!g_enCfg) {
        HDF_LOGE("MipiTxCheckGetCmdInfo: mipi_tx dev has not config!");
        return HDF_FAILURE;
    }
    if (getCmdInfo->devno != 0) {
        HDF_LOGE("MipiTxCheckGetCmdInfo: mipi_tx dev devno %u err!", getCmdInfo->devno);
        return HDF_ERR_INVALID_PARAM;
    }
    if ((getCmdInfo->getDataSize == 0) || (getCmdInfo->getDataSize > MIPI_TX_GET_DATA_SIZE)) {
        HDF_LOGE("MipiTxCheckGetCmdInfo: mipi_tx dev getDataSize %hu err!", getCmdInfo->getDataSize);
        return HDF_ERR_INVALID_PARAM;
    }
    if (getCmdInfo->getData == NULL) {
        HDF_LOGE("MipiTxCheckGetCmdInfo: mipi_tx dev getData is null!");
        return HDF_ERR_INVALID_OBJECT;
    }
    return HDF_SUCCESS;
}

static int MipiTxGetCmd(GetCmdInfoTag *getCmdInfo)
{
    int32_t ret;

    ret = MipiTxCheckGetCmdInfo(getCmdInfo);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("MipiTxGetCmd: [MipiTxCheckGetCmdInfo] fail!");
        return ret;
    }
    return MipiTxDrvGetCmdInfo(getCmdInfo);
}

static int32_t Hi3403GetCmd(struct MipiDsiCntlr *cntlr, struct DsiCmdDesc *cmd, uint32_t readLen, uint8_t *out)
{
    GetCmdInfoTag cmdInfo;

    (void)cntlr;
    if (cmd == NULL || out == NULL) {
        HDF_LOGE("Hi3403GetCmd: cmd or out is null!");
        return HDF_ERR_INVALID_OBJECT;
    }
    cmdInfo.devno = 0;
    cmdInfo.dataType = cmd->dataType;
    cmdInfo.dataParam = cmd->payload[0];
    cmdInfo.getDataSize = readLen;
    cmdInfo.getData = out;
    return MipiTxGetCmd(&cmdInfo);
}

static void Hi3403ToLp(struct MipiDsiCntlr *cntlr)
{
    (void)cntlr;
    MipiTxDrvDisableInput();
}

static void Hi3403ToHs(struct MipiDsiCntlr *cntlr)
{
    ComboDevCfgTag *dev = GetDevCfg(cntlr);

    if (dev == NULL) {
        HDF_LOGE("Hi3403ToHs: dev is null!");
        return;
    }
    MipiTxDrvEnableInput(dev->outputMode);
}

/* ====== HDF driver entry ====== */

static struct MipiDsiCntlr g_mipiTx = {
    .devNo = 0
};

static struct MipiDsiCntlrMethod g_method = {
    .setCntlrCfg = Hi3403SetCntlrCfg,
    .setCmd = Hi3403SetCmd,
    .getCmd = Hi3403GetCmd,
    .toHs = Hi3403ToHs,
    .toLp = Hi3403ToLp
};

static int32_t Hi3403MipiTxInit(struct HdfDeviceObject *device)
{
    int32_t ret;

    g_mipiTx.priv = NULL;
    g_mipiTx.ops = &g_method;
    ret = MipiDsiRegisterCntlr(&g_mipiTx, device);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("Hi3403MipiTxInit: [MipiDsiRegisterCntlr] fail!");
        return ret;
    }

    /* hi3403: defer HwInit (CRG clock+reset) to first SetComboDevCfg.
       Early reset may conflict with GPU/VO during kernel boot. */
    ret = MipiTxDrvRegInit();
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("Hi3403MipiTxInit: [MipiTxDrvRegInit] fail.");
        return ret;
    }
    HDF_LOGI("Hi3403MipiTxInit: load mipi tx driver successfully!");

    return ret;
}

static void Hi3403MipiTxRelease(struct HdfDeviceObject *device)
{
    struct MipiDsiCntlr *cntlr = NULL;

    if (device == NULL) {
        HDF_LOGE("Hi3403MipiTxRelease: device is null!");
        return;
    }
    cntlr = MipiDsiCntlrFromDevice(device);
    if (cntlr == NULL) {
        HDF_LOGE("Hi3403MipiTxRelease: cntlr is null!");
        return;
    }

    MipiTxDrvExit();
    MipiDsiUnregisterCntlr(&g_mipiTx);
    g_mipiTx.priv = NULL;
    HDF_LOGI("Hi3403MipiTxRelease: unload mipi tx driver successfully!");
}

struct HdfDriverEntry g_mipiTxDriverEntry = {
    .moduleVersion = 1,
    .Init = Hi3403MipiTxInit,
    .Release = Hi3403MipiTxRelease,
    .moduleName = "HDF_MIPI_TX",
};
HDF_INIT(g_mipiTxDriverEntry);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
