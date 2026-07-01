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

#ifndef DISPALY_LAYER_INTERNAL_H
#define DISPALY_LAYER_INTERNAL_H

#include <sys/types.h>
#include "display_adapt_fb.h"
#include "display_adapt_vo.h"
#include "display_adapt_sys.h"
#include "display_adapt_vpss.h"
#ifdef __HDMI_SUPPORT__
#include "display_adapt_hdmi.h"
#endif
#include "disp_common.h"
#include "display_type.h"
#include "ot_math.h"

#define CHECK_DEVID_VALID(devId, ret) do { \
    if ((devId) >= DEV_ID_NUM) { \
        HDF_LOGE("devid is invalid"); \
        return (ret); \
    } \
} while (0)

#define CHECK_GRAPHIC_LAYERID_VALID(layerId, ret) do { \
    if ((layerId) >= GRA_LAYER_MAX) { \
        HDF_LOGE("graphic layer id is invalid"); \
        return (ret); \
    } \
} while (0)

#define CHECK_LAYER_OPENED(fd, ret) do { \
    if ((fd) == -1) { \
        HDF_LOGE("layer is not opened"); \
        return (ret); \
    } \
} while (0)

#define CHECK_ID_VALID_RET(targetId, idMax) do { \
    if ((targetId) >= (idMax)) { \
        HDF_LOGE(""#targetId" is invalid, "#targetId"=%d", targetId); \
        return DISPLAY_PARAM_ERR; \
    } \
} while (0)

#define CHECK_CALL_MPI_RET(mpiRet, func) do { \
    if ((mpiRet) != TD_SUCCESS) { \
        HDF_LOGE("call "#func" failed, "#mpiRet"=0x%x\n", (mpiRet)); \
        return DISPLAY_FAILURE; \
    } \
} while (0)

#ifdef DISPLAY_RESOLUTION_480P
#define DEFAULT_WIDTH            720
#define DEFAULT_HEIGHT           480
#else
/* default layer width */
#define DEFAULT_WIDTH            1920
/* default layer height */
#define DEFAULT_HEIGHT           1080
#endif
/* Maximum number of display devices supported */
#define DEV_ID_NUM               1
/* Backgroud color */
#define COLOR_RGB_BLUE           0x0000FF
#define BACKGROUD_COLOR          0x00
/* Hdmi frame rate */
#define FRAME_RATE               60
/* Byte align number */
#define BYTE_ALIGN_NUM           2
/* Video layer buffer len */
#define VIDEO_LAYER_BUFF_LEN     3
/* Rotate enable */
#define VO_ROTATE_ENABLE         0

#ifndef ENABLE_VO_BUSYDROP
#define ENABLE_VO_BUSYDROP       0
#endif

#ifndef ENABLE_VO_TIME_TASK
#define ENABLE_VO_TIME_TASK      1
#endif

#define LAYER_ID_VO_LAYER_TYPE_MARK    0x80000000
#define LAYER_ID_VO_LAYER_ID_MASK    0xff00
#define LAYER_ID_VO_CHN_MASK      0xff

/* SyncInfo symbol */
#define SYNM                0
#define INTFB               0
#define IOP                 1
#define HMID                1
#define IDV                 0
#define IHS                 0
#define IVS                 0
#define BVACT               1
#define BVBB                1
#define BVFB                1

#ifndef __LITEOS__
typedef struct {
    struct fb_bitfield red;
    struct fb_bitfield green;
    struct fb_bitfield blue;
    struct fb_bitfield transp;
} Bitfield;
#endif

typedef struct {
    /* premultiply enable */
    bool preMul;
    /* max width */
    int32_t maxWidth;
    /* max height */
    int32_t maxHeight;
   /* vo scale enable */
    bool voScale;
} LayerCap;

typedef struct {
    /* file handler */
    int32_t fd;
    /* layer type */
    LayerType type;
    /* layer name */
    char *layerName;
    /* layer pixel format */
    PixelFormat pixFmt;
    /* flush region */
    IRect flushRect;
    /* layer width */
    int32_t width;
    /* layer height */
    int32_t height;
    /* layer canvas */
    HIFB_SURFACE_S canvas;
    /* layer canvas virtual address */
    void *canvasVirAddress;
    /* layer capability */
    LayerCap cap;
} LayerPrivate;

struct IntfSyncConvert {
    enum IntfSync intfSync;
    VO_INTF_SYNC_E voIntfSync;
};

TD_BOOL IsHdmiInitialized(void);

#ifdef __HDMI_SUPPORT__
TD_BOOL CheckHdmiConnect(void);
int32_t EnableVOWithHdmi(uint32_t devId);
int32_t DisableVOWithHdmi(uint32_t devId);
#endif

#ifdef __BT1120_SUPPORT__
int32_t EnableVOWithBT1120(ot_vo_dev voDev);
int32_t DisableVOWithBT1120(ot_vo_dev voDev);
#endif
#endif /* DISPALY_LAYER_INTERNAL_H */
