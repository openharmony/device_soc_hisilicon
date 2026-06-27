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
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <drm/drm_fourcc.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

static const int MAX_RETRY_TIMES = 10;
static const int RETRY_SLEEP_IN_US = 1000000;
#define DRM_DEVICE "/dev/dri/card0"
#define REFRESH_RATE 60
#define BPP 32

typedef struct {
    int drmFd;
    uint32_t crtcId;
    drmModeConnector *connector;
    drmModeModeInfo mode;
    uint32_t fbId;
    uint32_t fbHandle;
    uint32_t fbSize;
    uint32_t pitch;
    uint8_t *fbMap;
    bool setMode;
} DrmDisplayT;

DrmDisplayT g_primaryDisplay;


#ifndef DISENABLE_DISP
static struct DispInfo *GetDispInfo(uint32_t devId)
{
    static struct DispInfo dispInfo[DEV_ID_NUM];
    return &dispInfo[devId];
}
#endif // DISENABLE_DISP

static void PrintLayerInfo(const LayerInfo *layerInfo)
{
    HDF_LOGI("%s: [Print Layer Info] type = %d", __func__, layerInfo->type);
    HDF_LOGI("%s: width = %d, height = %d", __func__, layerInfo->width, layerInfo->height);
    HDF_LOGI("%s: bpp = %d, pixFormat = %d", __func__, layerInfo->bpp, layerInfo->pixFormat);
}

static int32_t CreateGraphicLayer(uint32_t devId, const LayerInfo *info, uint32_t *layerId)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    (void)info;
    *layerId = 0;
    return DISPLAY_SUCCESS;
}

#ifndef DISENABLE_DISP
static int32_t EnableVoWithHdfDisp(uint32_t devId)
{
    int32_t ret;
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
}

static void SdkExit()
{
    ot_rgn_mod_exit();
    ot_sys_mod_exit();
    ot_base_mod_exit();
    osal_exit();
}

static int32_t DeinitDisplay(uint32_t devId)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    if (g_initType == INIT_TYPE_GRAPHIC_ONLY) {
        SdkExit();
    }

    return DISPLAY_SUCCESS;
}

static int OpenDrm(DrmDisplayT *display)
{
    uint64_t hasDumb;
    int ret = 0;
    CHECK_NULLPOINTER_RETURN_VALUE(display, DISPLAY_NULL_PTR);
    display->drmFd = open(DRM_DEVICE, O_RDWR | O_CLOEXEC);
    if (display->drmFd < 0) {
        HDF_LOGE("Cannot open DRM device\n");
        return DISPLAY_FAILURE;
    }
    if (drmGetCap(display->drmFd, DRM_CAP_DUMB_BUFFER, &hasDumb) < 0) {
        HDF_LOGE("%s, %d failed to get DRM capabilities\n", __func__, __LINE__);
        return DISPLAY_FAILURE;
    }

    if (!hasDumb) {
        HDF_LOGE("%s, %d DRM device does not support dumb buffers\n", __func__, __LINE__);
        return DISPLAY_FAILURE;
    }
    ret = drmSetMaster(display->drmFd);
    if (ret < 0) {
        HDF_LOGE("Cannot  drmSetMaster\n");
    }
    return DISPLAY_SUCCESS;
}

static int InitDrmConnector(DrmDisplayT *display)
{
    drmModeRes *resources = drmModeGetResources(display->drmFd);
    if (!resources) {
        HDF_LOGE("Cannot get DRM resources\n");
        return DISPLAY_FAILURE;
    }

    // Find connected connector
    for (int i = 0; i < resources->count_connectors; i++) {
        display->connector = drmModeGetConnector(display->drmFd, resources->connectors[i]);
        if (display->connector && display->connector->connection == DRM_MODE_CONNECTED) {
            break;
        }
        drmModeFreeConnector(display->connector);
        display->connector = NULL;
    }

    if (!display->connector) {
        HDF_LOGE("No connected connector found\n");
        drmModeFreeResources(resources);
        return DISPLAY_FAILURE;
    }

    display->crtcId = resources->crtcs[0];
    drmModeFreeResources(resources);
    return DISPLAY_SUCCESS;
}

static int SelectDrmMode(DrmDisplayT *display)
{
    for (int i = 0; i < display->connector->count_modes; i++) {
        drmModeModeInfo *mode = &display->connector->modes[i];
        HDF_LOGE("%dx%d@%dHz\n", mode->hdisplay, mode->vdisplay, mode->vrefresh);
        if (mode->hdisplay == DEFAULT_WIDTH && mode->vdisplay == DEFAULT_HEIGHT && mode->vrefresh == REFRESH_RATE) {
            display->mode = *mode;
            break;
        }
    }
    if (display->mode.hdisplay != DEFAULT_WIDTH || display->mode.vdisplay != DEFAULT_HEIGHT ||
        display->mode.vrefresh != REFRESH_RATE) {
        HDF_LOGE("BAD mode:%dx%d@%dHz\n", display->mode.hdisplay, display->mode.vdisplay, display->mode.vrefresh);
        return DISPLAY_FAILURE;
    }
    display->setMode = true;
    return DISPLAY_SUCCESS;
}

static int CreateDrmFramebuffer(DrmDisplayT *display)
{
    struct drm_mode_create_dumb create = {0};
    struct drm_mode_map_dumb map = {0};
    int ret = 0;
    /* create a dumb-buffer, the pixel format is XRGB888 */
    uint32_t handles[4] = {0};
    uint32_t pitches[4] = {0};
    uint32_t offsets[4] = {0};

    create.width = DEFAULT_WIDTH;
    create.height = DEFAULT_HEIGHT;
    create.bpp = BPP;
    create.flags = 0;
    ret = drmIoctl(display->drmFd, DRM_IOCTL_MODE_CREATE_DUMB, &create);
    if (ret < 0) {
        HDF_LOGE("%s,DRM_IOCTL_MODE_CREATE_DUMB ret:%d\n", __func__, ret);
        return -1;
    }
    /* bind the dumb-buffer to an FB object */

    display->fbHandle = create.handle;
    display->fbSize = create.size;
    display->pitch = create.pitch;

    handles[0] = display->fbHandle;
    pitches[0] = display->pitch;
    offsets[0] = 0;
    ret = drmModeAddFB2(display->drmFd, DEFAULT_WIDTH, DEFAULT_HEIGHT, DRM_FORMAT_ABGR8888, handles, pitches,
        offsets, &display->fbId, 0);
    if (ret) {
        HDF_LOGE("%s,drmModeAddFB2 ret:%d\n", __func__, ret);
        return -1;
    }

    /* map the dumb-buffer to userspace */
    map.handle = display->fbHandle;
    ret = drmIoctl(display->drmFd, DRM_IOCTL_MODE_MAP_DUMB, &map);
    if (ret < 0) {
        HDF_LOGE("%s,DRM_IOCTL_MODE_MAP_DUMB ret:%d\n", __func__, ret);
        return DISPLAY_FAILURE;
    }

    display->fbMap = mmap(0, create.size, PROT_READ | PROT_WRITE, MAP_SHARED, display->drmFd, map.offset);
    if (display->fbMap == MAP_FAILED) {
        HDF_LOGE("Cannot mmap framebuffer\n");
        return DISPLAY_FAILURE;
    }

    return DISPLAY_SUCCESS;
}

static int InitDrm(DrmDisplayT *display)
{
    int ret;

    CHECK_NULLPOINTER_RETURN_VALUE(display, DISPLAY_NULL_PTR);

    ret = InitDrmConnector(display);
    if (ret != DISPLAY_SUCCESS) {
        return ret;
    }

    ret = SelectDrmMode(display);
    if (ret != DISPLAY_SUCCESS) {
        return ret;
    }

    return CreateDrmFramebuffer(display);
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

    if (devId == 0) {
        errno_t eok = memset_s(&g_primaryDisplay, sizeof(g_primaryDisplay), 0, sizeof(g_primaryDisplay));
        if (eok != EOK) {
            HDF_LOGE("%s: memset_s failed\n", __func__);
            return DISPLAY_FAILURE;
        }
        g_primaryDisplay.setMode = false;
        if (OpenDrm(&g_primaryDisplay) < 0) {
            HDF_LOGE("Failed to initialize DRM\n");
            return DISPLAY_FAILURE;
        }
        InitDrm(&g_primaryDisplay);

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
        ret = CreateOverlayLayer(devId, layerInfo, layerId);
    } else {
        HDF_LOGE("%s: not support layer type: %d", __func__, layerInfo->type);
        return DISPLAY_NOT_SUPPORT;
    }

    return ret;
}

static int32_t CloseLayer(uint32_t devId, uint32_t layerId)
{


    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);

    if ((layerId & LAYER_ID_VO_LAYER_TYPE_MARK) != 0) {
        CloseOverlayLayer(devId, layerId);
        return DISPLAY_SUCCESS;
    }

    return DISPLAY_SUCCESS;
}

static int32_t SetLayerVisible(uint32_t devId, uint32_t layerId, bool visibled)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_GRAPHIC_LAYERID_VALID(layerId, DISPLAY_FAILURE);
    (void)visibled;
    return DISPLAY_SUCCESS;
}

static int32_t SetLayerDirtyRegion(uint32_t devId, uint32_t layerId, IRect *region)
{
    CHECK_NULLPOINTER_RETURN_VALUE(region, DISPLAY_NULL_PTR);
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_GRAPHIC_LAYERID_VALID(layerId, DISPLAY_FAILURE);

    return DISPLAY_SUCCESS;
}

static int32_t Flush(uint32_t devId, uint32_t layerId, LayerBuffer *buffer)
{
    int ret = 0;

    if ((layerId & LAYER_ID_VO_LAYER_TYPE_MARK) != 0) {
        return OverlayLayerFlush(devId, layerId, buffer);
    }

    CHECK_NULLPOINTER_RETURN_VALUE(buffer, DISPLAY_NULL_PTR);
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_GRAPHIC_LAYERID_VALID(layerId, DISPLAY_FAILURE);

    ret = drmModeSetCrtc(g_primaryDisplay.drmFd, g_primaryDisplay.crtcId, g_primaryDisplay.fbId, 0, 0,
        &g_primaryDisplay.connector->connector_id, 1, &g_primaryDisplay.mode);
    if (ret < 0) {
        HDF_LOGE("%s: Failed to drmModeSetCrtc. ret: %d\n", __func__, ret);
    }
    return DISPLAY_SUCCESS;
}

static int32_t GetLayerBuffer(uint32_t devId, uint32_t layerId, LayerBuffer *buffer)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_GRAPHIC_LAYERID_VALID(layerId, DISPLAY_FAILURE);
    CHECK_NULLPOINTER_RETURN_VALUE(buffer, DISPLAY_NULL_PTR);

    if (!g_primaryDisplay.setMode) {
        InitDrm(&g_primaryDisplay);
    }

    buffer->data.virAddr = g_primaryDisplay.fbMap;
    buffer->data.phyAddr = 0;
    buffer->width = DEFAULT_WIDTH;
    buffer->height = DEFAULT_HEIGHT;
    buffer->pitch = g_primaryDisplay.pitch;
    buffer->pixFormat = PIXEL_FMT_RGBA_8888;
    return DISPLAY_SUCCESS;
}

static int32_t WaitForVBlank(uint32_t devId, uint32_t layerId, int32_t timeOut)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_GRAPHIC_LAYERID_VALID(layerId, DISPLAY_FAILURE);
    (void)timeOut;
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
