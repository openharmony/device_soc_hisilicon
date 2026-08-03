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

#include <securec.h>
#include "hdf_base.h"
#include "hdf_device_desc.h"
#include "hdf_disp.h"
#include "hdf_core_log.h"
#include "mipi_dsi_if.h"
#include "osal_io.h"

#define HDF_LOG_TAG HI3403_DISP

#define IOCFG2_BASE     0x0102F0000
#define IOCFG_SIZE      0x10000

#define TRANSFORM_KILO 1000
#define TRANSFORM_MEGA 1000000

static uint32_t CalcPixelClk(struct PanelInfo *info)
{
    uint16_t hpixel;
    uint16_t vline;

    hpixel = info->width + info->hbp + info->hfp + info->hsw;
    vline = info->height + info->vbp + info->vfp + info->vsw;
    uint32_t pixNum = hpixel * vline * info->frameRate;
    return (pixNum + (TRANSFORM_KILO - 1)) / TRANSFORM_KILO;
}

static uint32_t CalcDataRate(struct PanelInfo *info)
{
    uint16_t hpixel;
    uint16_t vline;

    hpixel = info->width + info->hbp + info->hfp + info->hsw;
    vline = info->height + info->vbp + info->vfp + info->vsw;
    uint32_t pixNum = hpixel * vline * info->frameRate;
    /* 24 bits per pixel */
    uint32_t bitClk = (pixNum * 24 + ((uint32_t)info->mipi.lane * TRANSFORM_MEGA - 1)) /
                      ((uint32_t)info->mipi.lane * TRANSFORM_MEGA);
    return bitClk;
}

static int32_t MipiDsiInit(struct PanelInfo *info)
{
    int32_t ret;
    DevHandle mipiHandle = NULL;
    struct MipiCfg cfg;

    mipiHandle = MipiDsiOpen(0);
    if (mipiHandle == NULL) {
        HDF_LOGE("%s: MipiDsiOpen failed", __func__);
        return HDF_FAILURE;
    }
    (void)memset_s(&cfg, sizeof(cfg), 0, sizeof(cfg));
    cfg.lane = info->mipi.lane;
    cfg.mode = info->mipi.mode;
    cfg.burstMode = info->mipi.burstMode;
    cfg.format = info->mipi.format;
    cfg.timing.xPixels = info->width;
    cfg.timing.hsaPixels = info->hsw;
    cfg.timing.hbpPixels = info->hbp;
    cfg.timing.hlinePixels = info->width + info->hbp + info->hfp + info->hsw;
    cfg.timing.vsaLines = info->vsw;
    cfg.timing.vbpLines = info->vbp;
    cfg.timing.vfpLines = info->vfp;
    cfg.timing.ylines = info->height;
    cfg.timing.edpiCmdSize = info->width;
    cfg.pixelClk = CalcPixelClk(info);
    cfg.phyDataRate = CalcDataRate(info);

    HDF_LOGI("%s: lane=%u mode=%u %ux%u pixelClk=%u dataRate=%u",
        __func__, info->mipi.lane, info->mipi.mode,
        info->width, info->height, cfg.pixelClk, cfg.phyDataRate);

    ret = MipiDsiSetCfg(mipiHandle, &cfg);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: MipiDsiSetCfg failed, ret=%d", __func__, ret);
    }
    MipiDsiClose(mipiHandle);
    return ret;
}

static int32_t Hi3403HardWareInit(void)
{
    int32_t i;
    int32_t ret;
    struct PanelManager *panelManager = NULL;
    struct PanelData *panel = NULL;
    struct PanelInfo *info = NULL;

    /* LCD pin mux config */
    unsigned long ioCfgBase = (unsigned long)OsalIoRemap(IOCFG2_BASE, IOCFG_SIZE);
    if (ioCfgBase == 0) {
        HDF_LOGE("%s: OsalIoRemap failed", __func__);
        return HDF_FAILURE;
    }
    OSAL_WRITEL(0x1100, ioCfgBase + 0x01EC); /* LCD_BL */
    OSAL_WRITEL(0x1100, ioCfgBase + 0x007C); /* LCD_VDD */
    OSAL_WRITEL(0x1101, ioCfgBase + 0x010C); /* LCD_RESET */

    panelManager = GetPanelManager();
    if (panelManager == NULL) {
        HDF_LOGE("%s: panelManager is null", __func__);
        return HDF_FAILURE;
    }
    for (i = 0; i < panelManager->panelNum; i++) {
        panel = panelManager->panel[i];
        info = panel->info;
        if (info == NULL) {
            HDF_LOGE("%s: GetPanelInfo failed", __func__);
            return HDF_FAILURE;
        }
        /* configure MIPI DSI controller timing */
        if (info->intfType == MIPI_DSI) {
            ret = MipiDsiInit(info);
            if (ret != HDF_SUCCESS) {
                HDF_LOGE("%s: MipiDsiInit failed", __func__);
                return HDF_FAILURE;
            }
        }
        /* call panel init callback */
        if (panel->init != NULL) {
            ret = panel->init(panel);
            if (ret != HDF_SUCCESS) {
                HDF_LOGE("%s: panel[%d] init failed, ret=%d", __func__, i, ret);
                return HDF_FAILURE;
            }
        }
    }
    HDF_LOGI("%s: success, panelNum=%u", __func__, panelManager->panelNum);
    return HDF_SUCCESS;
}

static int32_t Hi3403EntryInit(struct HdfDeviceObject *object)
{
    HDF_LOGI("%s: HI3403_DISP init start", __func__);
    if (object == NULL) {
        HDF_LOGE("%s: object is null", __func__);
        return HDF_FAILURE;
    }
    return Hi3403HardWareInit();
}

struct HdfDriverEntry g_hi3403DevEntry = {
    .moduleVersion = 1,
    .moduleName = "HI3403_DISP",
    .Init = Hi3403EntryInit,
};

HDF_INIT(g_hi3403DevEntry);
