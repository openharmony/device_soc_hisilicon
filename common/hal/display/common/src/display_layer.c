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

#include "display_layer.h"
#include <math.h>
#include <securec.h>
#include <sys/ioctl.h>
#include "display_layer_internal.h"
#include "display_overlay_layer.h"
#ifndef DISENABLE_DISP
#include "disp_common.h"
#include "disp_hal.h"
#include "hdf_io_service_if.h"
#include "hdf_sbuf.h"
#endif
static uint8_t g_vedioLayerIsInit = 0;
static const int MAX_RETRY_TIMES = 10;
static const int RETRY_SLEEP_IN_US = 1000000;
#ifndef __LITEOS__
static const Bitfield BIT_FIELD[] = {
    {
        .red = { 8, 4, 0 }, /* RGBA4444 */
        .green = { 4, 4, 0 },
        .blue = { 0, 4, 0 },
        .transp = { 12, 4, 0 },
    },
    {
        .red = { 8, 4, 0 }, /* RGBA4440 */
        .green = { 4, 4, 0 },
        .blue = { 0, 4, 0 },
        .transp = { 0, 0, 0 },
    },
    {
        .red = { 10, 5, 0 }, /* RGBA5551 */
        .green = { 5, 5, 0 },
        .blue = { 0, 5, 0 },
        .transp = { 15, 1, 0 },
    },
    {
        .red = { 10, 5, 0 }, /* RGBA5550 */
        .green = { 5, 5, 0 },
        .blue = { 0, 5, 0 },
        .transp = { 0, 0, 0 },
    },
    {
        .red = { 11, 5, 0 }, /* RGB565 */
        .green = { 5, 6, 0 },
        .blue = { 0, 5, 0 },
        .transp = { 0, 0, 0 },
    },
    {
        .red = { 16, 8, 0 }, /* RGBA8888 */
        .green = { 8, 8, 0 },
        .blue = { 0, 8, 0 },
        .transp = { 24, 8, 0 },
    },
    {
        .red = { 16, 8, 0 }, /* RGBA8880 */
        .green = { 8, 8, 0 },
        .blue = { 0, 8, 0 },
        .transp = { 0, 0, 0 },
    },
    {
        .red = { 0, 8, 0 }, /* CLUT8 */
        .green = { 0, 8, 0 },
        .blue = { 0, 8, 0 },
        .transp = { 0, 0, 0 },
    },
    {
        .red = { 11, 5, 0 }, /* RGBA5658 */
        .green = { 5, 6, 0 },
        .blue = { 0, 5, 0 },
        .transp = { 16, 8, 0 },
    }
};
#endif

#ifndef DISENABLE_DISP
static struct IntfSyncConvert g_intfMapTable[] = {
    { OUTPUT_USER, VO_OUTPUT_USER },
    { OUTPUT_PAL, VO_OUTPUT_PAL },
    { OUTPUT_NTSC, VO_OUTPUT_NTSC },
    { OUTPUT_1080P24, VO_OUTPUT_1080P24 },
    { OUTPUT_1080P25, VO_OUTPUT_1080P25 },
    { OUTPUT_1080P30, VO_OUTPUT_1080P30 },
    { OUTPUT_720P50, VO_OUTPUT_720P50 },
    { OUTPUT_720P60, VO_OUTPUT_720P60 },
    { OUTPUT_1080I50, VO_OUTPUT_1080I50 },
    { OUTPUT_1080I60, VO_OUTPUT_1080I60 },
    { OUTPUT_1080P50, VO_OUTPUT_1080P50 },
    { OUTPUT_1080P60, VO_OUTPUT_1080P60 },
    { OUTPUT_576P50, VO_OUTPUT_576P50 },
    { OUTPUT_480P60, VO_OUTPUT_480P60 },
    { OUTPUT_800X600_60, VO_OUTPUT_800X600_60 },
    { OUTPUT_1024X768_60, VO_OUTPUT_1024X768_60 },
    { OUTPUT_1280X1024_60, VO_OUTPUT_1280X1024_60 },
    { OUTPUT_1366X768_60, VO_OUTPUT_1366X768_60 },
    { OUTPUT_1440X900_60, VO_OUTPUT_1440X900_60 },
    { OUTPUT_1280X800_60, VO_OUTPUT_1280X800_60 },
    { OUTPUT_1600X1200_60, VO_OUTPUT_1600X1200_60 },
    { OUTPUT_1680X1050_60, VO_OUTPUT_1680X1050_60 },
    { OUTPUT_1920X1200_60, VO_OUTPUT_1920X1200_60 },
    { OUTPUT_640X480_60, VO_OUTPUT_640X480_60 },
    { OUTPUT_960H_PAL, VO_OUTPUT_960H_PAL },
    { OUTPUT_960H_NTSC, VO_OUTPUT_960H_NTSC },
    { OUTPUT_1920X2160_30, VO_OUTPUT_1920X2160_30 },
    { OUTPUT_2560X1440_30, VO_OUTPUT_2560X1440_30 },
    { OUTPUT_2560X1440_60, VO_OUTPUT_2560X1440_60 },
    { OUTPUT_2560X1600_60, VO_OUTPUT_2560X1600_60 },
    { OUTPUT_3840X2160_24, VO_OUTPUT_3840X2160_24 },
    { OUTPUT_3840X2160_25, VO_OUTPUT_3840X2160_25 },
    { OUTPUT_3840X2160_30, VO_OUTPUT_3840X2160_30 },
    { OUTPUT_3840X2160_50, VO_OUTPUT_3840X2160_50 },
    { OUTPUT_3840X2160_60, VO_OUTPUT_3840X2160_60 },
    { OUTPUT_4096X2160_24, VO_OUTPUT_4096X2160_24 },
    { OUTPUT_4096X2160_25, VO_OUTPUT_4096X2160_25 },
    { OUTPUT_4096X2160_30, VO_OUTPUT_4096X2160_30 },
    { OUTPUT_4096X2160_50, VO_OUTPUT_4096X2160_50 },
    { OUTPUT_4096X2160_60, VO_OUTPUT_4096X2160_60 },
    { OUTPUT_320X240_60, VO_OUTPUT_320X240_60 },
    { OUTPUT_320X240_50, VO_OUTPUT_320X240_50 },
    { OUTPUT_240X320_50, VO_OUTPUT_240X320_50 },
    { OUTPUT_240X320_60, VO_OUTPUT_240X320_60 },
    { OUTPUT_800X600_50, VO_OUTPUT_800X600_50 },
    { OUTPUT_720X1280_60, VO_OUTPUT_720X1280_60 },
    { OUTPUT_1080X1920_60, VO_OUTPUT_1080X1920_60 },
    { OUTPUT_7680X4320_30, VO_OUTPUT_7680X4320_30 },
};

static struct DispInfo *GetDispInfo(uint32_t devId)
{
    static struct DispInfo dispInfo[DEV_ID_NUM];
    return &dispInfo[devId];
}
#endif // DISENABLE_DISP
static LayerPrivate *GetLayerInstance(uint32_t devId, uint32_t layerId)
{
    static LayerPrivate layerPriv[DEV_ID_NUM][GRA_LAYER_MAX] = {
        {
            {
                .fd = -1,
                .type = LAYER_TYPE_GRAPHIC,
                .layerName = "/dev/fb0",
                .pixFmt = PIXEL_FMT_RGBA_5551,
                .flushRect = {0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT},
                .width = DEFAULT_WIDTH,
                .height = DEFAULT_HEIGHT,
                .cap = {TD_TRUE, DEFAULT_WIDTH, DEFAULT_HEIGHT, TD_TRUE},
            },
            {
                .fd = -1,
                .type = LAYER_TYPE_GRAPHIC,
                .layerName = "/dev/fb1",
                .pixFmt = PIXEL_FMT_RGBA_5551,
                .flushRect = {0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT},
                .width = DEFAULT_WIDTH,
                .height = DEFAULT_HEIGHT,
                .cap = {TD_TRUE, DEFAULT_WIDTH, DEFAULT_HEIGHT, TD_TRUE},
            },
            {
                .fd = -1,
                .type = LAYER_TYPE_GRAPHIC,
                .layerName = "/dev/fb2",
                .pixFmt = PIXEL_FMT_RGBA_5551,
                .flushRect = {0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT},
                .width = DEFAULT_WIDTH,
                .height = DEFAULT_HEIGHT,
                .cap = {TD_TRUE, DEFAULT_WIDTH, DEFAULT_HEIGHT, TD_TRUE},
            }
        }
    };
    return &layerPriv[devId][layerId];
}

static void PrintLayerInfo(const LayerInfo *layerInfo)
{
    HDF_LOGI("%s: [Print Layer Info] type = %d", __func__, layerInfo->type);
    HDF_LOGI("%s: width = %d, height = %d", __func__, layerInfo->width, layerInfo->height);
    HDF_LOGI("%s: bpp = %d, pixFormat = %d", __func__, layerInfo->bpp, layerInfo->pixFormat);
}

static HIFB_COLOR_FMT_E Convert2FbPixFmt(PixelFormat fmt)
{
    HIFB_COLOR_FMT_E hiFmt = HIFB_FMT_RGB565;
    switch (fmt) {
        case PIXEL_FMT_RGBA_5551:
            hiFmt = HIFB_FMT_ARGB1555;
            break;
        case PIXEL_FMT_RGB_565:
            hiFmt = HIFB_FMT_RGB565;
            break;
        case PIXEL_FMT_RGB_888:
            hiFmt = HIFB_FMT_RGB888;
            break;
        case PIXEL_FMT_RGBA_8888:
            hiFmt = HIFB_FMT_ARGB8888;
            break;
        default:
            HDF_LOGE("%s: not support pixel format: %d", __func__, fmt);
            break;
    }
    return hiFmt;
}

static PixelFormat Convert2PixFmt(HIFB_COLOR_FMT_E hiFmt)
{
    PixelFormat fmt = PIXEL_FMT_RGBA_8888;
    switch (hiFmt) {
        case HIFB_FMT_ARGB4444:
            fmt = PIXEL_FMT_RGBA_4444;
            break;
        case HIFB_FMT_ARGB1555:
            fmt = PIXEL_FMT_RGBA_5551;
            break;
        case HIFB_FMT_RGB565:
            fmt = PIXEL_FMT_RGB_565;
            break;
        case HIFB_FMT_ARGB8888:
            fmt = PIXEL_FMT_RGBA_8888;
            break;
        case HIFB_FMT_RGB888:
            fmt = PIXEL_FMT_RGB_888;
            break;
        case HIFB_FMT_8BPP:
            fmt = PIXEL_FMT_CLUT8;
            break;
        case HIFB_FMT_ARGB8565:
            fmt = PIXEL_FMT_RGBA_5658;
            break;
        default:
            fmt = PIXEL_FMT_RGBA_8888;
            break;
    }
    return fmt;
}

static void ConvertFieldInfo(PixelFormat type, Bitfield *fieldInfo)
{
    uint32_t index = 0;

    switch (type) {
        case PIXEL_FMT_RGBA_4444:
            index = 0; /* 0: RGBA4444 format bit field index */
            break;
        case PIXEL_FMT_RGB_444:
            index = 1; /* 1: RGB444 format bit field index */
            break;
        case PIXEL_FMT_RGBA_5551:
            index = 2; /* 2: RGBA5551 format bit field index */
            break;
        case PIXEL_FMT_RGB_555:
            index = 3; /* 3: RGBA555 format bit field index */
            break;
        case PIXEL_FMT_RGB_565:
            index = 4; /* 4: RGB565 format bit field index */
            break;
        case PIXEL_FMT_RGBA_8888:
            index = 5; /* 5: RGBA8888 format bit field index */
            break;
        case PIXEL_FMT_RGB_888:
            index = 6; /* 6: RGB888 format bit field index */
            break;
        case PIXEL_FMT_CLUT8:
            index = 7; /* 7: CLUT8 format bit field index */
            break;
        case PIXEL_FMT_RGBA_5658:
            index = 8; /* 8: RGBA5658 format bit field index */
            break;
        default:
            HDF_LOGE("%s: not support pixel type", __func__);
            return;
    }
    if (memcpy_s(fieldInfo, sizeof(Bitfield), &BIT_FIELD[index], sizeof(BIT_FIELD[index])) != EOK) {
        HDF_LOGE("%s: memcpy_s fail", __func__);
    }
    return;
}

static int32_t SetVarScreenInfo(int32_t fd, const LayerInfo *info)
{
    int32_t ret;
    struct fb_var_screeninfo vInfo;
    Bitfield bitField = {{0}, {0}, {0}, {0}};

    ret = ioctl(fd, FBIOGET_VSCREENINFO, &vInfo);
    if (ret < 0) {
        HDF_LOGE("%s: FBIOGET_VSCREENINFO fail, err:%d", __func__, errno);
        return DISPLAY_FAILURE;
    }

    vInfo.yoffset = 0;
    ConvertFieldInfo(info->pixFormat, &bitField);
    vInfo.xres = vInfo.xres_virtual = DEFAULT_WIDTH;
    vInfo.yres = vInfo.yres_virtual = DEFAULT_HEIGHT;
    vInfo.bits_per_pixel = info->bpp < 0 ? 0 : info->bpp;
    vInfo.red = bitField.red;
    vInfo.blue = bitField.blue;
    vInfo.green = bitField.green;
    vInfo.transp = bitField.transp;
    ret = ioctl(fd, FBIOPUT_VSCREENINFO, &vInfo);
    if (ret < 0) {
        HDF_LOGE("%s: FBIOPUT_VSCREENINFO fail, err:%d", __func__, errno);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

static int32_t SetLayerInfo(int32_t fd, const LayerInfo *info)
{
    int32_t ret;
    HIFB_LAYER_INFO_S fbLayerInfo;

    ret = ioctl(fd, FBIOGET_LAYER_INFO, &fbLayerInfo);
    if (ret < 0) {
        HDF_LOGE("%s: FBIOGET_LAYER_INFO fail", __func__);
        return DISPLAY_FAILURE;
    }
    fbLayerInfo.buf_mode = HIFB_LAYER_BUF_DOUBLE;
    fbLayerInfo.antiflicker_level = HIFB_LAYER_ANTIFLICKER_NONE;
    if (fbLayerInfo.buf_mode == HIFB_LAYER_BUF_NONE) {
        fbLayerInfo.mask = (uint32_t)HIFB_LAYERMASK_BUFMODE |
                            (uint32_t)HIFB_LAYERMASK_ANTIFLICKER_MODE |
                            (uint32_t)HIFB_LAYERMASK_POS;
    } else {
        fbLayerInfo.mask = (uint32_t)HIFB_LAYERMASK_BUFMODE |
                            (uint32_t)HIFB_LAYERMASK_ANTIFLICKER_MODE |
                            (uint32_t)HIFB_LAYERMASK_POS |
                            (uint32_t)HIFB_LAYERMASK_DISPSIZE;
    }
    fbLayerInfo.canvas_width = info->width < 0 ? 0 : info->width;
    fbLayerInfo.canvas_height = info->height < 0 ? 0 : info->height;
    fbLayerInfo.mask |= (uint32_t)HIFB_LAYERMASK_CANVASSIZE;
    ret = ioctl(fd, FBIOPUT_LAYER_INFO, &fbLayerInfo);
    if (ret < 0) {
        HDF_LOGE("%s: FBIOPUT_LAYER_INFO fail", __func__);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

static int32_t EnableAlpha(int32_t fd)
{
    HIFB_ALPHA_S alphaInfo;

    int32_t ret = ioctl(fd, FBIOGET_ALPHA_HIFB, &alphaInfo);
    if (ret < 0) {
        HDF_LOGE("%s: FBIOGET_ALPHA_HIFB fail", __func__);
        return DISPLAY_FAILURE;
    }
    alphaInfo.alpha_en = TD_TRUE;
    ret = ioctl(fd, FBIOPUT_ALPHA_HIFB, &alphaInfo);
    if (ret < 0) {
        HDF_LOGE("%s: FBIOPUT_ALPHA_HIFB fail", __func__);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

static void SavePrivLayerInfo(uint32_t devId, uint32_t layerId, int32_t fd, const LayerInfo *info)
{
    LayerPrivate *priv = GetLayerInstance(devId, layerId);
    priv->pixFmt = info->pixFormat;
    priv->type = info->type;
    priv->fd = fd;
    priv->flushRect.w = info->width;
    priv->flushRect.h = info->height;
    priv->width = info->width;
    priv->height = info->height;
}

static int32_t SetLayerOrigin(int32_t fd)
{
    HIFB_POINT_S stPoint = {0};
    if (ioctl(fd, FBIOPUT_SCREEN_ORIGIN_HIFB, &stPoint) < 0) {
        HDF_LOGE("%s: set screen original show position failed", __func__);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

static int32_t LayerConfig(int32_t fd, const LayerInfo *info)
{
    int32_t ret;
    ret = SetVarScreenInfo(fd, info);
    if (ret < 0) {
        HDF_LOGE("%s: set var screen info failed", __func__);
        return DISPLAY_FAILURE;
    }

    ret = SetLayerInfo(fd, info);
    if (ret < 0) {
        HDF_LOGE("%s: set layer info failed", __func__);
        return DISPLAY_FAILURE;
    }
    ret = SetLayerOrigin(fd);
    if (ret < 0) {
        HDF_LOGE("%s: set layer origin failed", __func__);
        return DISPLAY_FAILURE;
    }
    ret = EnableAlpha(fd);
    if (ret < 0) {
        HDF_LOGE("%s: enable alpha failed", __func__);
        return DISPLAY_FAILURE;
    }

    return DISPLAY_SUCCESS;
}

static TD_BOOL CheckLayerInfo(uint32_t devId, uint32_t layerId, const LayerInfo *info)
{
    LayerPrivate *priv = GetLayerInstance(devId, layerId);
    if (info->width > priv->cap.maxWidth ||
        info->height > priv->cap.maxHeight ||
        info->pixFormat >= PIXEL_FMT_BUTT ||
        info->pixFormat < PIXEL_FMT_CLUT8 ||
        info->type >= LAYER_TYPE_BUTT) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

static int32_t SetLayerConfig(uint32_t devId, int32_t index, LayerPrivate *priv, const LayerInfo *info,
    uint32_t *layerId)
{
    int32_t ret;
    int32_t fd = -1;
    for (int times = 0; times < MAX_RETRY_TIMES; times++) {
        fd = open(priv->layerName, O_RDWR, 0);
        if (fd >= 0) {
            break;
        }
        usleep(RETRY_SLEEP_IN_US);
    }

    if (fd < 0) {
        HDF_LOGE("%s: cannot open framebuffer device: %s, reason:%s",
            __func__, priv->layerName, strerror(errno));
        return DISPLAY_FD_ERR;
    }
    ret = LayerConfig(fd, info);
    if (ret != DISPLAY_SUCCESS) {
        close(fd);
        HDF_LOGE("%s: layer config fail", __func__);
        return DISPLAY_FAILURE;
    }

    SavePrivLayerInfo(devId, index < 0 ? 0 : index, fd, info);
    *layerId = index < 0 ? 0 : (uint32_t)index;
    HDF_LOGI("%s: open graphic layer success, layerId = %u", __func__, *layerId);
    return DISPLAY_SUCCESS;
}

static int32_t CreateGraphicLayer(uint32_t devId, const LayerInfo *info, uint32_t *layerId)
{
    int32_t index;
    LayerPrivate *priv = NULL;

    for (index = GRA_LAYER_0; index < GRA_LAYER_MAX; index++) {
        priv = GetLayerInstance(devId, index);
        if (priv->fd == -1) {
            if (CheckLayerInfo(devId, index, info) == TD_FALSE) {
                HDF_LOGE("%s: invalid layer information", __func__);
                return DISPLAY_PARAM_ERR;
            }
            return SetLayerConfig(devId, index, priv, info, layerId);
        }
    }
    HDF_LOGE("%s: no layer to open", __func__);
    return DISPLAY_FAILURE;
}

static void ConvertToLayerBuffer(uint32_t devId, uint32_t layerId, LayerBuffer *buffer)
{
    LayerPrivate *priv = GetLayerInstance(devId, layerId);
    buffer->data.virAddr = priv->canvasVirAddress;
    buffer->data.phyAddr = priv->canvas.phys_addr;
    buffer->width = (int32_t)priv->canvas.width;
    buffer->height = (int32_t)priv->canvas.height;
    buffer->pitch = (int32_t)priv->canvas.pitch;
    buffer->pixFormat = Convert2PixFmt(priv->canvas.format);
}

static void SaveCanvasBuffer(uint32_t devId, uint32_t layerId, const HIFB_SURFACE_S *buffer)
{
    LayerPrivate *priv = GetLayerInstance(devId, layerId);
    priv->canvas.phys_addr = buffer->phys_addr;
    priv->canvas.width = buffer->width;
    priv->canvas.height = buffer->height;
    priv->canvas.format = buffer->format;
    priv->canvas.pitch = buffer->pitch;
}
#ifndef DISENABLE_DISP
static int32_t Convert2IntfSync(enum IntfSync intfSync, VO_INTF_SYNC_E *voIntfSync)
{
    int32_t i;
    int32_t len = sizeof(g_intfMapTable) / sizeof(g_intfMapTable[0]);
    for (i = 0; i < len; i++) {
        if (g_intfMapTable[i].intfSync == intfSync) {
            *voIntfSync = g_intfMapTable[i].voIntfSync;
            return DISPLAY_SUCCESS;
        }
    }
    HDF_LOGE("%s: not find intfSync, intfSync = %d", __func__, intfSync);
    return DISPLAY_FAILURE;
}

static void GetUserPubBaseAttr(uint32_t devId, VO_PUB_ATTR_S *pubAttr)
{
    struct DispInfo *info = GetDispInfo(devId);
    pubAttr->intf_type = info->intfType;
    if (Convert2IntfSync(info->intfSync, &pubAttr->intf_sync) != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: unsupport intfSync", __func__);
        return;
    }
    pubAttr->bg_color = COLOR_RGB_BLUE;
    pubAttr->sync_info.syncm = SYNM;
    pubAttr->sync_info.intfb = INTFB;
    pubAttr->sync_info.iop = IOP;
    pubAttr->sync_info.hmid = HMID;
    pubAttr->sync_info.idv = IDV;
    pubAttr->sync_info.ihs = IHS;
    pubAttr->sync_info.ivs = IVS;
    pubAttr->sync_info.bvact = BVACT;
    pubAttr->sync_info.bvbb = BVBB;
    pubAttr->sync_info.bvfb = BVFB;

    pubAttr->sync_info.hact = info->width;
    pubAttr->sync_info.hbb = info->hbp + info->hsw;
    pubAttr->sync_info.hfb = info->hfp;
    pubAttr->sync_info.hpw = info->hsw;
    pubAttr->sync_info.vact = info->height;
    pubAttr->sync_info.vbb = info->vbp + info->vsw;
    pubAttr->sync_info.vfb = info->vfp;
    pubAttr->sync_info.vpw = info->vsw;
}

static int32_t GetUserIntfSyncAttr(uint32_t devId, VO_USER_INTFSYNC_INFO_S *intfSyncAttr)
{
    VO_USER_INTFSYNC_PLL_S pllClk;
    struct DispInfo *info = GetDispInfo(devId);

    (void)memset_s(&pllClk, sizeof(VO_USER_INTFSYNC_PLL_S), 0, sizeof(VO_USER_INTFSYNC_PLL_S));
    intfSyncAttr->clk_reverse_en = HI_TRUE;
    intfSyncAttr->dev_div = 1;
    intfSyncAttr->pre_div = 1;
    intfSyncAttr->user_sync_attr.clk_src = VO_CLK_SOURCE_PLL;
    if (GetVoClkParameter(info, &pllClk) != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: GetVoClkParameter failure", __func__);
        return DISPLAY_FAILURE;
    }
    intfSyncAttr->user_sync_attr.vo_pll.fb_div = pllClk.fb_div;
    intfSyncAttr->user_sync_attr.vo_pll.frac = pllClk.frac;
    intfSyncAttr->user_sync_attr.vo_pll.ref_div = pllClk.ref_div;
    intfSyncAttr->user_sync_attr.vo_pll.post_div1 = pllClk.post_div1;
    intfSyncAttr->user_sync_attr.vo_pll.post_div2 = pllClk.post_div2;
    return DISPLAY_SUCCESS;
}

static int32_t EnableVoWithHdfDisp(uint32_t devId)
{
    int32_t ret;
    VO_PUB_ATTR_S voPubAttr = {0};
    VO_USER_INTFSYNC_INFO_S userInfo = {0};
    int32_t voDev = devId;

    GetUserPubBaseAttr(devId, &voPubAttr);
    ret = ss_mpi_vo_set_pub_attr(voDev, &voPubAttr);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: ss_mpi_vo_set_pub_attr failure, ret 0x%x", __func__, ret);
        return DISPLAY_FAILURE;
    }
    if (voPubAttr.intf_sync == VO_OUTPUT_USER) {
        if (GetUserIntfSyncAttr(devId, &userInfo)) {
            HDF_LOGE("%s: GetUserIntfSyncAttr failure", __func__);
            return DISPLAY_FAILURE;
        }
        ret = ss_mpi_vo_set_user_sync_info(voDev, &userInfo);
        if (ret != DISPLAY_SUCCESS) {
            HDF_LOGE("%s: ss_mpi_vo_set_user_sync_info failure, ret 0x%x", __func__, ret);
            return DISPLAY_FAILURE;
        }
    }
    VO_MOD_PARAM_S modParam = {0};
    ss_mpi_vo_get_mod_param(&modParam);
    modParam.exit_dev_en = HI_FALSE;
    ret = ss_mpi_vo_set_mod_param(&modParam);
    HDF_LOGI("%s: DISP ss_mpi_vo_set_mod_param, ret 0x%x", __func__, ret);
    ret = ss_mpi_vo_enable(voDev);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: DISP ss_mpi_vo_enable failure, ret 0x%x", __func__, ret);
        return DISPLAY_FAILURE;
    }
    HDF_LOGI("%s: lcd was initialized", __func__);
    return DISPLAY_SUCCESS;
}

static int32_t InitHdfDisp(uint32_t devId)
{
    struct DispInfo *info = GetDispInfo(devId);
    HalFuncs *halFunc = GetHalFuncs();
    if (halFunc == NULL) {
        HDF_LOGE("%s: halFunc is null", __func__);
        return DISPLAY_NULL_PTR;
    }

    int32_t ret = halFunc->GetInfo(devId, info);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: get disp info failed", __func__);
        return DISPLAY_FAILURE;
    }

    ret = halFunc->SetPowerStatus(devId, POWER_STATUS_ON);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: set power on failed", __func__);
        return DISPLAY_FAILURE;
    }

    ret = halFunc->SetBacklight(devId, info->defLevel);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: set default backlight failed, level = %d", __func__, info->defLevel);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}
#endif // DISENABLE_DISP

typedef enum {
    INIT_TYPE_MEDIA = 0,
    INIT_TYPE_GRAPHIC,
    INIT_TYPE_GRAPHIC_ONLY,
    INIT_TYPE_BUTT
} InitType;
static InitType g_initType = INIT_TYPE_BUTT;

static void SdkInit()
{
    td_s32 ret;
    ret = osal_init();
    if (ret != 0) {
        HDF_LOGE("%s: osal_init error:%d", __func__, ret);
    }

    ret = ot_base_mod_init();
    if (ret != 0) {
        HDF_LOGE("%s: BaseModInit error:%d", __func__, ret);
    }

    ret = ot_sys_mod_init();
    if (ret != 0) {
        HDF_LOGE("%s: SysModInit error:%d", __func__, ret);
    }
    ret = ot_rgn_mod_init();
    if (ret != 0) {
        HDF_LOGE("%s: ot_rgn_mod_init error:%d", __func__, ret);
    }
    ret = ot_vo_mod_init();
    if (ret != 0) {
        HDF_LOGE("%s: VoModInit error:%d", __func__, ret);
    }
}

static void SdkExit()
{
    ot_vo_mod_exit();
    ot_rgn_mod_exit();
    ot_sys_mod_exit();
    ot_base_mod_exit();
    osal_exit();
}

static int32_t DeinitDisplay(uint32_t devId)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);

    if (g_initType == INIT_TYPE_MEDIA || g_initType == INIT_TYPE_GRAPHIC_ONLY) {
        DisableVOWithHdmi(devId);
    }
    if (g_initType == INIT_TYPE_GRAPHIC_ONLY) {
        SdkExit();
    }
    return DISPLAY_SUCCESS;
}

static int32_t InitDisplay(uint32_t devId)
{
    uint32_t workDev = devId;
    if (workDev == 1) {
        g_initType = INIT_TYPE_MEDIA;
        workDev = 0;
    } else {
        FILE *file;
        const char *fileName = "/proc/umap/sys_user";
        for (int times = 0; times < MAX_RETRY_TIMES; times++) {
            file = fopen(fileName, "r");
            if (file != NULL) {
                break;
            }
            usleep(RETRY_SLEEP_IN_US);
        }
        if (file == NULL) {
            g_initType = INIT_TYPE_GRAPHIC_ONLY;
        } else {
            g_initType = INIT_TYPE_GRAPHIC;
            fclose(file);
        }
    }
    CHECK_DEVID_VALID(workDev, DISPLAY_FAILURE);
    if (g_initType == INIT_TYPE_GRAPHIC_ONLY) {
        SdkInit();
    }
    
    if (g_initType == INIT_TYPE_MEDIA || g_initType == INIT_TYPE_GRAPHIC_ONLY) {
        if (EnableVOWithHdmi(workDev) != DISPLAY_SUCCESS) {
            HDF_LOGE("%s: EnableVOWithHdmi fail", __func__);
            return DISPLAY_FAILURE;
        }
    }

    return DISPLAY_SUCCESS;
}

static int32_t GetDisplayInfo(uint32_t devId, DisplayInfo *dispInfo)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_NULLPOINTER_RETURN_VALUE(dispInfo, DISPLAY_NULL_PTR);
#ifdef __BT1120_SUPPORT__
    dispInfo->width = DEFAULT_WIDTH;
    dispInfo->height = DEFAULT_HEIGHT;
    dispInfo->rotAngle = ROTATE_NONE;
#endif

#ifdef __HDMI_SUPPORT__
    dispInfo->width = DEFAULT_WIDTH;
    dispInfo->height = DEFAULT_HEIGHT;
    dispInfo->rotAngle = ROTATE_NONE;
#endif

#ifndef DISENABLE_DISP
    struct DispInfo *info = GetDispInfo(devId);
#if VO_ROTATE_ENABLE
    dispInfo->width = info->height;
    dispInfo->height = info->width;
    dispInfo->rotAngle = ROTATE_90;
#else
    dispInfo->width = info->width;
    dispInfo->height = info->height;
    dispInfo->rotAngle = ROTATE_NONE;
#endif
#endif // DISENABLE_DISP
    return DISPLAY_SUCCESS;
}

static int32_t CreateLayer(uint32_t devId, const LayerInfo *layerInfo, uint32_t *layerId)
{
    int32_t ret;

    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_NULLPOINTER_RETURN_VALUE(layerInfo, DISPLAY_NULL_PTR);
    CHECK_NULLPOINTER_RETURN_VALUE(layerId, DISPLAY_NULL_PTR);
    PrintLayerInfo(layerInfo);
    if (layerInfo->type == LAYER_TYPE_GRAPHIC) {
        ret = CreateGraphicLayer(devId, layerInfo, layerId);
    } else if (layerInfo->type == LAYER_TYPE_OVERLAY) {
        if (g_vedioLayerIsInit == 0) {
            EnabelVideoLayer(devId);
        }
        g_vedioLayerIsInit++;
        ret = CreateOverlayLayer(devId, layerInfo, layerId);
    } else {
        HDF_LOGE("%s: not support layer type: %d", __func__, layerInfo->type);
        return DISPLAY_NOT_SUPPORT;
    }
    HDF_LOGE("%s: g_initType:%d, ret:%d", __func__, g_initType, ret);
    return ret;
}

static int32_t CloseLayer(uint32_t devId, uint32_t layerId)
{
    LayerPrivate *priv = NULL;

    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    if ((layerId & LAYER_ID_VO_LAYER_TYPE_MARK) != 0) {
        CloseOverlayLayer(devId, layerId);
        g_vedioLayerIsInit--;
        if (g_vedioLayerIsInit == 0) {
            DisableVideoLayer(devId);
        }
        return DISPLAY_SUCCESS;
    }

    CHECK_GRAPHIC_LAYERID_VALID(layerId, DISPLAY_FAILURE);

    priv = GetLayerInstance(devId, layerId);
    CHECK_LAYER_OPENED(priv->fd, DISPLAY_FAILURE);

    if (priv->fd >= 0) {
        close(priv->fd);
    }
    priv->fd = -1;
    if (priv->canvasVirAddress != 0) {
        ss_mpi_sys_munmap(priv->canvasVirAddress, priv->canvas.pitch * priv->canvas.height);
    }
    priv->canvasVirAddress = 0;
    return DISPLAY_SUCCESS;
}

static int32_t SetLayerVisible(uint32_t devId, uint32_t layerId, bool visibled)
{
    int32_t ret;
    TD_BOOL bShow;
    LayerPrivate *priv = NULL;

    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_GRAPHIC_LAYERID_VALID(layerId, DISPLAY_FAILURE);

    priv = GetLayerInstance(devId, layerId);
    CHECK_LAYER_OPENED(priv->fd, DISPLAY_FAILURE);

    if (visibled) {
        bShow = TD_TRUE;
    } else {
        bShow = TD_FALSE;
    }
    ret = ioctl(priv->fd, FBIOPUT_SHOW_HIFB, &bShow);
    if (ret < 0) {
        HDF_LOGE("%s: FBIOPUT_SHOW_HIFB failed", __func__);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

static int32_t SetLayerDirtyRegion(uint32_t devId, uint32_t layerId, IRect *region)
{
    LayerPrivate *priv = NULL;

    CHECK_NULLPOINTER_RETURN_VALUE(region, DISPLAY_NULL_PTR);
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_GRAPHIC_LAYERID_VALID(layerId, DISPLAY_FAILURE);

    priv = GetLayerInstance(devId, layerId);
    CHECK_LAYER_OPENED(priv->fd, DISPLAY_FAILURE);

    priv->flushRect.x = region->x;
    priv->flushRect.y = region->y;
    priv->flushRect.w = region->w;
    priv->flushRect.h = region->h;
    return DISPLAY_SUCCESS;
}

static int32_t Flush(uint32_t devId, uint32_t layerId, LayerBuffer *buffer)
{
    if ((layerId & LAYER_ID_VO_LAYER_TYPE_MARK) != 0) {
        return OverlayLayerFlush(devId, layerId, buffer);
    }
    int32_t ret;
    HIFB_BUFFER_S fbBuf;
    LayerPrivate *priv = NULL;

    CHECK_NULLPOINTER_RETURN_VALUE(buffer, DISPLAY_NULL_PTR);
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_GRAPHIC_LAYERID_VALID(layerId, DISPLAY_FAILURE);

    priv = GetLayerInstance(devId, layerId);
    CHECK_LAYER_OPENED(priv->fd, DISPLAY_FAILURE);

    (void)memset_s(&fbBuf, sizeof(HIFB_BUFFER_S), 0, sizeof(HIFB_BUFFER_S));
    fbBuf.update_rect.x = priv->flushRect.x;
    fbBuf.update_rect.y = priv->flushRect.y;
    fbBuf.update_rect.width = priv->flushRect.w;
    fbBuf.update_rect.height = priv->flushRect.h;
    fbBuf.canvas.format = Convert2FbPixFmt(buffer->pixFormat);
    fbBuf.canvas.phys_addr = buffer->data.phyAddr;
    fbBuf.canvas.width = (uint32_t)buffer->width;
    fbBuf.canvas.height = (uint32_t)buffer->height;
    fbBuf.canvas.pitch = (uint32_t)buffer->pitch;
    ret = ioctl(priv->fd, FBIO_REFRESH, &fbBuf);
    if (ret < 0) {
        HDF_LOGE("%s: FBIO_REFRESH failure", __func__);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

static int32_t GetLayerBuffer(uint32_t devId, uint32_t layerId, LayerBuffer *buffer)
{
    HIFB_BUFFER_S canvasBuf = {0};
    void *virAddr = NULL;
    int32_t layerSize = 0;
    uint32_t layerSizeUint32 = 0;
    int32_t ret = 0;
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_GRAPHIC_LAYERID_VALID(layerId, DISPLAY_FAILURE);
    CHECK_NULLPOINTER_RETURN_VALUE(buffer, DISPLAY_NULL_PTR);

    LayerPrivate *priv = GetLayerInstance(devId, layerId);
    CHECK_LAYER_OPENED(priv->fd, DISPLAY_FAILURE);

    if (priv->canvasVirAddress != 0) {
        ConvertToLayerBuffer(devId, layerId, buffer);
        return DISPLAY_SUCCESS;
    }
    ret = ioctl(priv->fd, FBIOGET_CANVAS_BUFFER, &canvasBuf);
    if (ret < 0) {
        HDF_LOGE("%s: FBIOGET_CANVAS_BUFFER failure", __func__);
        return DISPLAY_FAILURE;
    }

    layerSize = (int32_t)canvasBuf.canvas.pitch * (int32_t)canvasBuf.canvas.height;
    layerSizeUint32 = layerSize < 0 ? 0 : (uint32_t)layerSize;
    virAddr = ss_mpi_sys_mmap(canvasBuf.canvas.phys_addr, layerSizeUint32);
    if (virAddr == NULL) {
        HDF_LOGE("%s: ss_mpi_sys_mmap failure", __func__);
        return DISPLAY_FAILURE;
    }

    (void)memset_s(virAddr, layerSizeUint32, 0x0, layerSizeUint32);
    priv->canvasVirAddress = virAddr;
    SaveCanvasBuffer(devId, layerId, &canvasBuf.canvas);
    ConvertToLayerBuffer(devId, layerId, buffer);
    return DISPLAY_SUCCESS;
}

static int32_t WaitForVBlank(uint32_t devId, uint32_t layerId, int32_t timeOut)
{
    LayerPrivate *priv = NULL;
    (void)timeOut;
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_GRAPHIC_LAYERID_VALID(layerId, DISPLAY_FAILURE);

    priv = GetLayerInstance(devId, layerId);
    CHECK_LAYER_OPENED(priv->fd, DISPLAY_FAILURE);

    int32_t ret = ioctl(priv->fd, FBIOGET_VBLANK_HIFB, NULL);
    if (ret < 0) {
        HDF_LOGE("FBIOGET_VBLANK_HIFB failure\n");
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

static int32_t SetLayerSize(uint32_t devId, uint32_t layerId, IRect *rect)
{
    if ((layerId & LAYER_ID_VO_LAYER_TYPE_MARK) != 0) {
        return SetOverlayLayerSize(devId, layerId, rect);
    }
    return DISPLAY_SUCCESS;
}

static int32_t SetLayerPriority(uint32_t priority)
{
    return SetOverlayLayerPriority(priority);
}

int32_t LayerInitialize(LayerFuncs **funcs)
{
    CHECK_NULLPOINTER_RETURN_VALUE(funcs, DISPLAY_NULL_PTR);
    LayerFuncs *lFuncs = (LayerFuncs *)malloc(sizeof(LayerFuncs));
    if (lFuncs == NULL) {
        HDF_LOGE("%s: lFuncs is null", __func__);
        return DISPLAY_NULL_PTR;
    }
    errno_t eok = memset_s(lFuncs, sizeof(LayerFuncs), 0, sizeof(LayerFuncs));
    if (eok != EOK) {
        free(lFuncs);
        lFuncs = NULL;
        HDF_LOGE("%s: memset_s failed\n", __func__);
        return DISPLAY_FAILURE;
    }
    lFuncs->InitDisplay = InitDisplay;
    lFuncs->DeinitDisplay = DeinitDisplay;
    lFuncs->GetDisplayInfo = GetDisplayInfo;
    lFuncs->CreateLayer = CreateLayer;
    lFuncs->CloseLayer = CloseLayer;
    lFuncs->SetLayerVisible = SetLayerVisible;
    lFuncs->SetLayerDirtyRegion = SetLayerDirtyRegion;
    lFuncs->Flush = Flush;
    lFuncs->GetLayerBuffer = GetLayerBuffer;
    lFuncs->WaitForVBlank = WaitForVBlank;
    lFuncs->SetLayerSize = SetLayerSize;
    lFuncs->InvokeLayerCmd = InvokeOverlayLayerCmd;
    lFuncs->GetLayerDeviceId = GetOverlayLayerDeviceId;
    lFuncs->SetLayerPriority = SetLayerPriority;
    *funcs = lFuncs;
    HDF_LOGI("%s: layer initialize success", __func__);
    return DISPLAY_SUCCESS;
}

int32_t LayerUninitialize(LayerFuncs *funcs)
{
    CHECK_NULLPOINTER_RETURN_VALUE(funcs, DISPLAY_NULL_PTR);
    free(funcs);
    HDF_LOGI("%s: layer uninitialize success", __func__);
    return DISPLAY_SUCCESS;
}