/*
 * Copyright (c) 2021 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
#include "i2s_hi35xx.h"
#include "i2s_aiao_hi35xx.h"
#include "i2s_codec_hi35xx.h"
#include "i2s_core.h"
#include "device_resource_if.h"
#include "hdf_base.h"
#include "hdf_log.h"
#include "los_vm_phys.h"
#include "los_vm_iomap.h"
#include "osal_io.h"
#include "osal_irq.h"
#include "osal_mem.h"
#include "osal_sem.h"
#include "osal_time.h"
#include "platform_dumper.h"

#define HDF_LOG_TAG i2s_hi35xx

static void I2sDumperDump(struct I2sConfigInfo *configInfo)
{
    int32_t ret;
    struct PlatformDumperData datas[] = {
        {"I2S_AIAO_INT_ENA", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + I2S_AIAO_INT_ENA)},
        {"RX_IF_ATTR1", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + RX_IF_ATTR1)},
        {"RX_DSP_CTRL", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + RX_DSP_CTRL)},
        {"RX_BUFF_SIZE", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + RX_BUFF_SIZE)},
        {"RX_BUFF_WPTR", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + RX_BUFF_WPTR)},
        {"RX_BUFF_RPTR", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + RX_BUFF_RPTR)},
        {"RX_TRANS_SIZE", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + RX_TRANS_SIZE)},
        {"RX_INT_ENA", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + RX_INT_ENA)},
        {"RX_INT_STATUS", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + RX_INT_STATUS)},
        {"RX_INT_CLR", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + RX_INT_CLR)},
        {"TX_IF_ATTR1", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + TX_IF_ATTR1)},
        {"TX_DSP_CTRL", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + TX_DSP_CTRL)},
        {"TX_BUFF_SADDR", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + TX_BUFF_SADDR)},
        {"TX_BUFF_SIZE", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + TX_BUFF_SIZE)},
        {"TX_BUFF_WPTR", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + TX_BUFF_WPTR)},
        {"TX_BUFF_RPTR", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + TX_BUFF_RPTR)},
        {"TX_TRANS_SIZE", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + TX_TRANS_SIZE)},
        {"AUDIO_CTRL_REG_1", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + AUDIO_CTRL_REG_1)},
        {"AUDIO_DAC_REG_0", PLATFORM_DUMPER_REGISTERL, (void *)(configInfo->regBase + AUDIO_DAC_REG_0)},
    };

    if (configInfo->dumper == NULL) {
        HDF_LOGE("%s: configInfo dumper is NULL!", __func__);
        return;
    }
    ret = PlatformDumperAddDatas(configInfo->dumper, datas, sizeof(datas) / sizeof(struct PlatformDumperData));
    if (ret != HDF_SUCCESS) {
        return;
    }
    (void)PlatformDumperDump(configInfo->dumper);
    (void)PlatformDumperClearDatas(configInfo->dumper);
}

static int32_t I2sDumperCreate(struct I2sConfigInfo *configInfo)
{
    struct PlatformDumper *dumper = NULL;
    char *name = NULL;

    name = (char *)OsalMemCalloc(I2S_DUMPER_NAME_LEN);
    if (name == NULL) {
        HDF_LOGE("%s: alloc name fail", __func__);
        return HDF_ERR_MALLOC_FAIL;
    }

    if (snprintf_s(name, I2S_DUMPER_NAME_LEN, I2S_DUMPER_NAME_LEN - 1, "%s%u",
        I2S_DUMPER_NAME_PREFIX, configInfo->busNum) < 0) {
        HDF_LOGE("%s: snprintf_s name fail!", __func__);
        OsalMemFree(name);
        return HDF_ERR_IO;
    }
    dumper = PlatformDumperCreate(name);
    if (dumper == NULL) {
        HDF_LOGE("%s: get dumper for %s fail!", __func__, name);
        OsalMemFree(name);
        return HDF_ERR_IO;
    }
    configInfo->dumper = dumper;
    configInfo->dumperName = name;

    return HDF_SUCCESS;
}

static inline void I2sDumperDestroy(struct I2sConfigInfo *configInfo)
{
    PlatformDumperDestroy(configInfo->dumper);
    OsalMemFree(configInfo->dumperName);
}

void GetI2sRxRegInfo(struct I2sConfigInfo *i2sCfg)
{
    I2S_PRINT_LOG_DBG("%s: PERI_CRG103[%px][%px][%px]", __func__,
        i2sCfg->crg103Addr, i2sCfg->regBase, i2sCfg->codecAddr);
    uint32_t value = OSAL_READL(i2sCfg->crg103Addr);
    I2S_PRINT_LOG_DBG("%s: PERI_CRG103[0x%x]", __func__, value);
    GetI2sAiaoRxInfo(i2sCfg);
    GetI2sAiaoTxInfo(i2sCfg);
    GetI2sCodecInfo(i2sCfg);
}

void GetI2sTxRegInfo(struct I2sConfigInfo *i2sCfg)
{
    GetI2sAiaoTxInfo(i2sCfg);
    GetI2sCodecInfo(i2sCfg);
}

int32_t Hi35xxI2sRegWrite(uint32_t value, volatile unsigned char *addr)
{
    if (addr == NULL) {
        I2S_PRINT_LOG_ERR("%s: addr is NULL", __func__);
        return HDF_ERR_INVALID_PARAM;
    }
    I2S_PRINT_LOG_DBG("%s: I2S set REG: addr[%px] value[0x%x]", __func__, addr, value);
    OSAL_WRITEL(value, addr);
    return HDF_SUCCESS;
}

uint32_t Hi35xxI2sRegRead(volatile unsigned char *addr)
{
    if (addr == NULL) {
        I2S_PRINT_LOG_ERR("%s: addr is NULL", __func__);
        return 0;
    }

    uint32_t val = OSAL_READL(addr);
    I2S_PRINT_LOG_DBG("%s: I2S read REG: addr[%px] value[0x%x]", __func__, addr, val);
    return val;
}

static int32_t Hi35xxI2sEnable(struct I2sCntlr *cntlr)
{
    if (cntlr == NULL || cntlr->priv == NULL) {
        I2S_PRINT_LOG_ERR("%s: cntlr priv or cfg is NULL", __func__);
        return HDF_ERR_INVALID_PARAM;
    }
    return HDF_SUCCESS;
}

static int32_t Hi35xxI2sDisable(struct I2sCntlr *cntlr)
{
    if (cntlr == NULL || cntlr->priv == NULL) {
        I2S_PRINT_LOG_ERR("%s: cntlr priv or cfg is NULL", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct I2sConfigInfo *i2sCfg = (struct I2sConfigInfo *)cntlr->priv;
    if (Hi35xxI2sRegWrite(0x0, i2sCfg->crg103Addr) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: Hi35xxI2sRegWrite i2sCfg->crg103Addr failed", __func__);
        I2sDumperDump(i2sCfg);
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

static int32_t Hi35xxI2sOpen(struct I2sCntlr *cntlr)
{
    (void)cntlr;
    return HDF_SUCCESS;
}

static int32_t Hi35xxI2sClose(struct I2sCntlr *cntlr)
{
    (void)cntlr;
    return HDF_SUCCESS;
}

static int32_t Hi35xxI2sGetCfg(struct I2sCntlr *cntlr, struct I2sCfg *cfg)
{
    if (cntlr == NULL || cntlr->priv == NULL || cfg == NULL) {
        I2S_PRINT_LOG_ERR("%s: cntlr priv or cfg is NULL", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct I2sConfigInfo *i2sCfg = (struct I2sConfigInfo *)cntlr->priv;

    cfg->sampleRate = i2sCfg->sampleRate;
    cfg->width = i2sCfg->width;
    cfg->writeChannel = i2sCfg->writeChannel;
    cfg->channelIfMode = i2sCfg->channelIfMode;
    cfg->channelMode = i2sCfg->channelMode;
    AudioCodecGetCfgI2slFsSel(i2sCfg->i2slFsSel, &cfg->i2slFsSel);

    cfg->bclk = i2sCfg->bclk;
    cfg->mclk = i2sCfg->mclk;
    cfg->type = i2sCfg->type;
    cfg->samplePrecision = i2sCfg->samplePrecision;

    I2S_PRINT_LOG_DBG("%s: writeChannel[%u], i2slFsSel[%u], mclk[%d], bclk[%d], sampleRate[%u], \
        type[%u], channelMode[%u], channelIfMode[%u], samplePrecision[%u]", __func__,
        cfg->writeChannel, cfg->i2slFsSel, cfg->mclk, cfg->bclk, cfg->sampleRate,
        cfg->type, cfg->channelMode, cfg->channelIfMode, cfg->samplePrecision);
    return HDF_SUCCESS;
}

static int32_t Hi35xxI2sSetCfg(struct I2sCntlr *cntlr, struct I2sCfg *cfg)
{
    struct I2sConfigInfo *i2sCfg = NULL;

    if (cntlr == NULL || cntlr->priv == NULL || cfg == NULL) {
        I2S_PRINT_LOG_ERR("%s: cntlr priv or cfg is NULL", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    i2sCfg = (struct I2sConfigInfo *)cntlr->priv;

    I2S_PRINT_LOG_DBG("%s: writeChannel[%u], i2slFsSel[%u], mclk[%d], bclk[%d], sampleRate[%u], \
        type[%u], channelMode[%u], channelIfMode[%u], samplePrecision[%u]", __func__,
        cfg->writeChannel, cfg->i2slFsSel, cfg->mclk, cfg->bclk, cfg->sampleRate,
        cfg->type, cfg->channelMode, cfg->channelIfMode, cfg->samplePrecision);

    // audio or output
    i2sCfg->writeChannel = cfg->writeChannel;
    if (AudioCodecSetCfgI2slFsSel(&i2sCfg->i2slFsSel, cfg->i2slFsSel) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: AudioCodecSetCfgI2slFsSel error", __func__);
        return HDF_FAILURE;
    }
    // Audio Codec info set
    Hi35xxSetAudioCodec(i2sCfg, cfg->sampleRate, cfg->width);
    i2sCfg->mclk = cfg->mclk;
    i2sCfg->bclk = cfg->bclk;
    int32_t rate = Hi35xxSampleRateShift(cfg->sampleRate);
    if (rate == HDF_ERR_INVALID_PARAM) {
        I2S_PRINT_LOG_ERR("%s: Hi35xxSampleRateShift error", __func__);
        return HDF_FAILURE;
    }
    if (Hi35xxSetCfgAiaoFsclkDiv(&i2sCfg->regCfg100.aiaoFsclkDiv, (cfg->bclk / rate)) != 0) {
        I2S_PRINT_LOG_ERR("%s: Hi35xxSetCfgAiaoFsclkDiv set error", __func__);
        return HDF_FAILURE;
    }
    if (Hi35xxSetCfgAiaoBclkDiv(&i2sCfg->regCfg100.aiaoBclkDiv, (cfg->mclk / cfg->bclk)) != 0) {
        I2S_PRINT_LOG_ERR("%s: Hi35xxSetCfgAiaoFsclkDiv set error", __func__);
        return HDF_FAILURE;
    }

    i2sCfg->regCfg100.aiaoSrstReq = I2S_AIAO_SRST_REQ_NO_RESET;
    i2sCfg->regCfg100.aiaoCken = I2S_AIAO_CKEN_OPEN;
    i2sCfg->regRxIfAttr1.rxSdSourceSel = RX_SD_SOURCE_SEL_NORMAL;
    i2sCfg->type = cfg->type;
    if (AiaoGetRxIfAttri(i2sCfg, cfg->type, cfg->channelMode, cfg->channelIfMode, cfg->samplePrecision)
        != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: AiaoGetRxIfAttri failed", __func__);
        return HDF_FAILURE;
    }

    // Rx
    CfgSetI2sCrgCfg000(i2sCfg, cfg->i2slFsSel, cfg->sampleRate);
    CfgSetI2sCrgCfg100(i2sCfg);
    CfgSetRxIfSAttr1(i2sCfg);

    // Tx
    CfgSetI2sCrgCfg008(i2sCfg, cfg->i2slFsSel, cfg->sampleRate);
    CfgSetI2sCrgCfg108(i2sCfg);
    CfgSetTxIfSAttr1(i2sCfg);
    I2sDumperDump(i2sCfg);
    return HDF_SUCCESS;
}

static int32_t Hi35xxI2sStartWrite(struct I2sCntlr *cntlr)
{
    if (cntlr == NULL || cntlr->priv == NULL) {
        I2S_PRINT_LOG_ERR("%s: cntlr priv or cfg is NULL", __func__);
        return HDF_ERR_INVALID_PARAM;
    }
    struct I2sConfigInfo *i2sCfg = (struct I2sConfigInfo *)cntlr->priv;

    // prepare tx buff
    if (Hi35xxI2sWriteGetBuff(i2sCfg) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: Hi35xxI2sWrite error", __func__);
        I2sDumperDump(i2sCfg);
        return HDF_FAILURE;
    }
    CfgSetTxBuffInfo(i2sCfg);
    i2sCfg->isplay = true;
    i2sCfg->txEn = false;

    I2S_PRINT_LOG_DBG("%s: after set TX CRG--------->\r\n\r\n", __func__);
    GetI2sTxRegInfo(i2sCfg);
    I2S_PRINT_LOG_DBG("%s: after set TX CRG--------->\r\n\r\n", __func__);

    return HDF_SUCCESS;
}

static int32_t Hi35xxI2sStopWrite(struct I2sCntlr *cntlr)
{
    struct I2sConfigInfo *i2sCfg = NULL;

    if (cntlr == NULL || cntlr->priv == NULL) {
        I2S_PRINT_LOG_ERR("%s: cntlr priv or cfg is NULL", __func__);
        return HDF_ERR_INVALID_PARAM;
    }
    i2sCfg = (struct I2sConfigInfo *)cntlr->priv;

    uint32_t value = Hi35xxI2sRegRead(i2sCfg->regBase + TX_DSP_CTRL);
    value &= ~TX_DISABLE;
    value |= (0x0 << TX_DISABLE_SHIFT);
    if (Hi35xxI2sRegWrite(value, i2sCfg->regBase + TX_DSP_CTRL) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: Hi35xxI2sRegWrite i2sCfg->regBase + TX_DSP_CTRL failed", __func__);
        I2sDumperDump(i2sCfg);
        return HDF_FAILURE;
    }

    while (1) {
        value = Hi35xxI2sRegRead(i2sCfg->regBase + TX_DSP_CTRL);
        I2S_PRINT_LOG_DBG("%s: TX_DSP_CTRL[0x%x]", __func__, value);
        if ((value & TX_DISABLE_DONE) == TX_DISABLE_DONE) {
            I2S_PRINT_LOG_ERR("%s: TX_DSP_CTRL stop success val 0x%x", __func__, value);
            break;
        } else {
            OsalMSleep(AIAO_STOP_RX_TX_MSLEEP);
        }
    }

    GetI2sTxRegInfo(i2sCfg);
    i2sCfg->isplay = false;
    i2sCfg->txEn = false;
    if (i2sCfg->txVirData != NULL) {
        LOS_DmaMemFree(i2sCfg->txVirData);
        i2sCfg->txVirData = NULL;
        i2sCfg->txWptr = 0;
        i2sCfg->txRptr = 0;
        i2sCfg->txSize = 0;
        CfgSetTxBuffInfo(i2sCfg);
    }
    return HDF_SUCCESS;
}

static int32_t Hi35xxI2sStartRead(struct I2sCntlr *cntlr)
{
    struct I2sConfigInfo *i2sCfg = NULL;
    if (cntlr == NULL || cntlr->priv == NULL) {
        I2S_PRINT_LOG_ERR("%s: cntlr priv or cfg is NULL", __func__);
        return HDF_ERR_INVALID_PARAM;
    }
    i2sCfg = (struct I2sConfigInfo *)cntlr->priv;

    // prepare rx buff
    if (Hi35xxI2sReadGetBuff(i2sCfg) != 0) {
        I2S_PRINT_LOG_ERR("%s: Hi35xxI2sRead error", __func__);
        I2sDumperDump(i2sCfg);
        return HDF_FAILURE;
    }

    CfgSetRxBuffInfo(i2sCfg);
    CfgStartRecord(i2sCfg);

    I2S_PRINT_LOG_DBG("%s: after set RX CRG--------->\r\n\r\n", __func__);
    GetI2sRxRegInfo(i2sCfg);
    I2S_PRINT_LOG_DBG("%s: after set RX CRG--------->\r\n\r\n", __func__);

    return HDF_SUCCESS;
}

static int32_t Hi35xxI2sStopRead(struct I2sCntlr *cntlr)
{
    struct I2sConfigInfo *i2sCfg = NULL;

    if (cntlr == NULL || cntlr->priv == NULL) {
        I2S_PRINT_LOG_ERR("%s: cntlr priv or cfg is NULL", __func__);
        return HDF_ERR_INVALID_PARAM;
    }
    i2sCfg = (struct I2sConfigInfo *)cntlr->priv;

    I2S_PRINT_LOG_DBG("%s: before stop", __func__);
    GetI2sRxRegInfo(i2sCfg);
    I2S_PRINT_LOG_DBG("%s: before stop", __func__);

    // stop read
    uint32_t value = Hi35xxI2sRegRead(i2sCfg->regBase + RX_DSP_CTRL);
    value &= ~RX_DISABLE;
    value |= (0x0 << RX_DISABLE_SHIFT);
    if (Hi35xxI2sRegWrite(value, i2sCfg->regBase + RX_DSP_CTRL) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: Hi35xxI2sRegWrite i2sCfg->regBase + RX_DSP_CTRL failed", __func__);
        I2sDumperDump(i2sCfg);
        return HDF_FAILURE;
    }

    while (1) {
        value = Hi35xxI2sRegRead(i2sCfg->regBase + RX_DSP_CTRL);
        I2S_PRINT_LOG_DBG("%s: RX_DSP_CTRL val 0x%x", __func__, value);
        if ((value & RX_DISABLE_DONE) == RX_DISABLE_DONE) {
            I2S_PRINT_LOG_ERR("%s: RX_DSP_CTRL stop success val 0x%x", __func__, value);
            break;
        } else {
            OsalMSleep(AIAO_STOP_RX_TX_MSLEEP);
        }
    }

    I2S_PRINT_LOG_DBG("%s: after stop", __func__);
    GetI2sRxRegInfo(i2sCfg);
    I2S_PRINT_LOG_DBG("%s: after  stop", __func__);

    if (i2sCfg->rxVirData != NULL) {
        LOS_DmaMemFree(i2sCfg->rxVirData);
        i2sCfg->rxVirData = NULL;
        i2sCfg->rxWptr = 0;
        i2sCfg->rxRptr = 0;
        i2sCfg->rxSize = 0;
        CfgSetRxBuffInfo(i2sCfg);
    }
    return HDF_SUCCESS;
}

static int32_t Hi35xxI2sRead(struct I2sCntlr *cntlr, struct I2sMsg *msgs)
{
    if (cntlr == NULL || cntlr->priv == NULL || msgs == NULL || msgs->rbuf == NULL || msgs->pRlen == NULL) {
        I2S_PRINT_LOG_ERR("%s: input param is invalid", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct I2sConfigInfo *i2sCfg = NULL;
    i2sCfg = (struct I2sConfigInfo *)cntlr->priv;

    uint32_t rptrOffset = 0;
    if (GetRxBuffData(i2sCfg, msgs, &rptrOffset) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: GetRxBuffData failed", __func__);
        I2sDumperDump(i2sCfg);
        return HDF_FAILURE;
    }

    if (*msgs->pRlen == 0) {
        I2S_PRINT_LOG_DBG("%s: not available data.", __func__);
        return HDF_SUCCESS;
    }

    if (Hi35xxI2sRegWrite(rptrOffset, i2sCfg->regBase + RX_BUFF_RPTR) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: Hi35xxI2sRegWrite i2sCfg->regBase + RX_BUFF_RPTR failed", __func__);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int32_t Hi35xxI2sWrite(struct I2sCntlr *cntlr, struct I2sMsg *msgs)
{
    if (cntlr == NULL || cntlr->priv == NULL || msgs == NULL) {
        I2S_PRINT_LOG_ERR("%s: input param is invalid", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct I2sConfigInfo *i2sCfg = NULL;
    i2sCfg = (struct I2sConfigInfo *)cntlr->priv;

    I2S_PRINT_LOG_DBG("%s:len[0x%x], isplay[%d], txSize[0x%x]", __func__,
        msgs->len, i2sCfg->isplay, i2sCfg->txSize);

    if (i2sCfg->txVirData == NULL) {
        I2S_PRINT_LOG_ERR("%s: txVirData NULL", __func__);
        return HDF_FAILURE;
    }

    if (!i2sCfg->isplay) {
        I2S_PRINT_LOG_ERR("%s: play is not set", __func__);
        return HDF_FAILURE;
    }

    if (msgs->len >= i2sCfg->txSize) {
        I2S_PRINT_LOG_ERR("%s: play data too big", __func__);
        return HDF_FAILURE;
    }
    // play data set
    CfgStartPlay(i2sCfg);

    uint32_t offset = 0;
    if (UpdateTxBuffData(i2sCfg, msgs, &offset) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s:UpdateTxBuffData failed", __func__);
        return HDF_FAILURE;
    }

    I2S_PRINT_LOG_DBG("%s: offset[0x%x]", __func__, offset);
    if (*msgs->pRlen == 0) {
        I2S_PRINT_LOG_DBG("%s: tx buff full, wait to write.", __func__);
        return HDF_SUCCESS;
    }

    if (Hi35xxI2sRegWrite(offset, i2sCfg->regBase + TX_BUFF_WPTR) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: Hi35xxI2sRegWrite i2sCfg->regBase + TX_BUFF_WPTR failed", __func__);
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

static int32_t Hi35xxI2sTransfer(struct I2sCntlr *cntlr, struct I2sMsg *msgs)
{
    if (cntlr == NULL || cntlr->priv == NULL || msgs == NULL) {
        I2S_PRINT_LOG_ERR("%s: input param is invalid", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct I2sConfigInfo *i2sCfg = NULL;
    i2sCfg = (struct I2sConfigInfo *)cntlr->priv;

    if (msgs->rbuf != NULL) {
        Hi35xxI2sRead(cntlr, msgs);
    } else if (msgs->wbuf != NULL) {
        Hi35xxI2sWrite(cntlr, msgs);
    } else {
        I2S_PRINT_LOG_ERR("%s: buf null", __func__);
        I2sDumperDump(i2sCfg);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

struct I2sCntlrMethod g_i2sCntlrMethod = {
    .GetCfg = Hi35xxI2sGetCfg,
    .SetCfg = Hi35xxI2sSetCfg,
    .Transfer = Hi35xxI2sTransfer,
    .Open = Hi35xxI2sOpen,
    .Close = Hi35xxI2sClose,
    .Enable = Hi35xxI2sEnable,
    .Disable = Hi35xxI2sDisable,
    .StartWrite = Hi35xxI2sStartWrite,
    .StopWrite = Hi35xxI2sStopWrite,
    .StartRead = Hi35xxI2sStartRead,
    .StopRead = Hi35xxI2sStopRead,
};

static int32_t I2sGetConfigInfoFromHcs(struct I2sCntlr *cntlr, const struct DeviceResourceNode *node)
{
    struct DeviceResourceIface *iface = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);

    if (cntlr == NULL || node == NULL || iface == NULL || iface->GetUint32 == NULL) {
        I2S_PRINT_LOG_ERR("%s: iface is invalid", __func__);
        return HDF_FAILURE;
    }
    if (iface->GetUint32(node, "busNum", &cntlr->busNum, 0) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: read busNum fail", __func__);
        return HDF_FAILURE;
    }
    if (iface->GetUint32(node, "irqNum", &cntlr->irqNum, 0) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: read irqNum fail", __func__);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int32_t I2sGetRegCfgFromHcs(struct I2sConfigInfo *configInfo, const struct DeviceResourceNode *node)
{
    uint32_t tmp;
    struct DeviceResourceIface *iface = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);

    if (configInfo == NULL || node == NULL || iface == NULL || iface->GetUint32 == NULL) {
        I2S_PRINT_LOG_ERR("%s: face is invalid", __func__);
        return HDF_FAILURE;
    }
    if (iface->GetUint32(node, "i2s_pad_enable", &tmp, 0) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: read i2s_pad_enable fail", __func__);
        return HDF_FAILURE;
    }
    configInfo->i2sPadEnable = tmp;
    if (iface->GetUint32(node, "audio_enable", &tmp, 0) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: read audio_enable fail", __func__);
        return HDF_FAILURE;
    }
    configInfo->audioEnable = tmp;
    if (iface->GetUint32(node, "PERI_CRG103", &configInfo->PERICRG103, 0) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: read PERI_CRG103 fail", __func__);
        return HDF_FAILURE;
    }
    if (iface->GetUint32(node, "regBase", &tmp, 0) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: read regBase fail", __func__);
        return HDF_FAILURE;
    }
    I2S_PRINT_LOG_DBG("%s: regBase[0x%x]", __func__, tmp);

    configInfo->regBase = OsalIoRemap(tmp, I2S_AIAO_MAX_REG_SIZE);
    I2S_PRINT_LOG_DBG("%s:regBase[0x%x][%px]", __func__, tmp, configInfo->regBase);
    configInfo->codecAddr = OsalIoRemap(AUDIO_CODEC_BASE_ADDR, ACODEC_MAX_REG_SIZE);
    I2S_PRINT_LOG_DBG("%s:codecAddr[0x%x][%px]", __func__, AUDIO_CODEC_BASE_ADDR, configInfo->codecAddr);
    configInfo->crg103Addr = OsalIoRemap(PERI_CRG103, sizeof(unsigned int));
    I2S_PRINT_LOG_DBG("%s:crg103Addr[0x%x][%px]", __func__, PERI_CRG103, configInfo->crg103Addr);

    return HDF_SUCCESS;
}

static int32_t PeriCrg103Set(struct I2sConfigInfo *configInfo)
{
    if (configInfo == NULL) {
        I2S_PRINT_LOG_ERR("%s: configInfo is null", __func__);
        return HDF_FAILURE;
    }

    if (Hi35xxI2sRegWrite(configInfo->PERICRG103, configInfo->crg103Addr) != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: Hi35xxI2sRegWrite configInfo->crg103Addr failed", __func__);
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

static int32_t I2sInit(struct I2sCntlr *cntlr, const struct HdfDeviceObject *device)
{
    int32_t ret;
    struct I2sConfigInfo *configInfo = NULL;

    if (device->property == NULL || cntlr == NULL) {
        I2S_PRINT_LOG_ERR("%s: property is null", __func__);
        return HDF_FAILURE;
    }

    configInfo = (struct I2sConfigInfo *)OsalMemCalloc(sizeof(*configInfo));
    if (configInfo == NULL) {
        I2S_PRINT_LOG_ERR("%s: OsalMemCalloc configInfo error", __func__);
        return HDF_ERR_MALLOC_FAIL;
    }

    ret = I2sGetConfigInfoFromHcs(cntlr, device->property);
    if (ret != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: I2sGetConfigInfoFromHcs error", __func__);
        OsalMemFree(configInfo);
        return HDF_FAILURE;
    }

    ret = I2sGetRegCfgFromHcs(configInfo, device->property);
    if (ret != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: I2sGetRegCfgFromHcs error", __func__);
        OsalMemFree(configInfo);
        return HDF_FAILURE;
    }
    configInfo->busNum = cntlr->busNum;
    configInfo->isplay = false;
    configInfo->rxVirData = NULL;
    configInfo->txVirData = NULL;

    // CLK RESET
    (void)PeriCrg103Set(configInfo);
    (void)AiaoInit(configInfo);
    (void)CodecInit(configInfo);

    cntlr->priv = configInfo;
    cntlr->method = &g_i2sCntlrMethod;

    ret = I2sDumperCreate(configInfo);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: create dumper failed:%d", __func__, ret);
        OsalMemFree(configInfo);
        return ret;
    }
    I2sDumperDump(configInfo);
    return HDF_SUCCESS;
}

static int32_t HdfI2sDeviceBind(struct HdfDeviceObject *device)
{
    if (device == NULL) {
        I2S_PRINT_LOG_ERR("%s: device NULL", __func__);
        return HDF_ERR_INVALID_OBJECT;
    }
    return (I2sCntlrCreate(device) == NULL) ? HDF_FAILURE : HDF_SUCCESS;
}

static int32_t HdfI2sDeviceInit(struct HdfDeviceObject *device)
{
    struct I2sCntlr *cntlr = NULL;

    if (device == NULL) {
        I2S_PRINT_LOG_ERR("%s: device NULL", __func__);
        return HDF_ERR_INVALID_OBJECT;
    }

    cntlr = I2sCntlrFromDevice(device);
    if (cntlr == NULL) {
        I2S_PRINT_LOG_ERR("%s: cntlr is null", __func__);
        return HDF_FAILURE;
    }

    int32_t ret = I2sInit(cntlr, device);
    if (ret != HDF_SUCCESS) {
        I2S_PRINT_LOG_ERR("%s: I2sInit error", __func__);
        return HDF_FAILURE;
    }
    return ret;
}

static void HdfI2sDeviceRelease(struct HdfDeviceObject *device)
{
    struct I2sCntlr *cntlr = NULL;
    if (device == NULL) {
        I2S_PRINT_LOG_ERR("%s: device is NULL", __func__);
        return;
    }

    cntlr = I2sCntlrFromDevice(device);
    if (cntlr == NULL) {
        I2S_PRINT_LOG_ERR("%s: cntlr is NULL", __func__);
        return;
    }

    if (cntlr->priv != NULL) {
        struct I2sConfigInfo *configInfo = cntlr->priv;
        if (configInfo->regBase != 0) {
            OsalIoUnmap((void *)configInfo->regBase);
        }
        if (configInfo->codecAddr != 0) {
            OsalIoUnmap((void *)configInfo->codecAddr);
        }
        if (configInfo->crg103Addr != 0) {
            OsalIoUnmap((void *)configInfo->crg103Addr);
        }
        I2sDumperDestroy(configInfo);
        OsalMemFree(cntlr->priv);
    }

    I2sCntlrDestroy(cntlr);
}

struct HdfDriverEntry g_hdfI2sDevice = {
    .moduleVersion = 1,
    .moduleName = "HDF_PLATFORM_I2S",
    .Bind = HdfI2sDeviceBind,
    .Init = HdfI2sDeviceInit,
    .Release = HdfI2sDeviceRelease,
};

HDF_INIT(g_hdfI2sDevice);
