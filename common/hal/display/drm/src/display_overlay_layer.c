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
#include "ot_math.h"
#include "ss_mpi_vb.h"
#include "ss_mpi_vgs.h"
#define ALIGN_DOWN OT_ALIGN_DOWN
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

#define DRM_DEVICE "/dev/dri/card0"

#define INVALID_VGS_HANDLE       (-1)
#define INVALID_FD               (-1)
#define VALID_FD(fd)             ((fd) > INVALID_FD)
#define YUV420_PLANE_RATIO       2
#define YUV420_SIZE_RATIO_NUM    3
#define YUV420_SIZE_RATIO_DEN    2
#define WIDTH_THRESHOLD_4K       4096
#define WIDTH_THRESHOLD_8K       8192
#define HEAD_STRIDE_4K           16
#define HEAD_STRIDE_8K           32
#define HEAD_STRIDE_MAX          64
#define INVALID_PHYS_ADDR        0
#define IOCTL_SUCCESS            0
#define PLANE_INDEX_Y            0
#define PLANE_INDEX_UV           1
#define ALIGN_VALUE              2

typedef struct {
    int drmFd;
    uint32_t width;
    uint32_t height;
    IRect displayRect;
} DrmOverlayDisplayT;

DrmOverlayDisplayT  g_overlayDisplay;

#define DRM_HI3403V100_OVERLAY_FLUSH      1
#define DRM_IOCTL_HI3403V100_OVERLAY_FLUSH \
    DRM_IOWR(DRM_COMMAND_BASE + DRM_HI3403V100_OVERLAY_FLUSH, ot_video_frame_info)

static bool CheckTypeIsOverlayLayer(uint32_t layerId)
{
    return (layerId & LAYER_ID_VO_LAYER_TYPE_MARK) != 0;
}

static int create_overlayer_drm(DrmOverlayDisplayT *display, const LayerInfo *layerInfo)
{
    errno_t eok = memset_s(&g_overlayDisplay, sizeof(g_overlayDisplay), 0, sizeof(g_overlayDisplay));
    if (eok != EOK) {
        HDF_LOGE("%s: memset_s failed\n", __func__);
        return DISPLAY_FAILURE;
    }
    display->drmFd = open(DRM_DEVICE, O_RDWR | O_CLOEXEC);
    if (display->drmFd == INVALID_FD) {
        HDF_LOGE("%s, Cannot open DRM device\n", __func__);
        return DISPLAY_FAILURE;
    }
    display->width = layerInfo->width;
    display->height = layerInfo->height;
    return DISPLAY_SUCCESS;
}

int32_t CreateOverlayLayer(uint32_t devId, const LayerInfo *layerInfo, uint32_t *layerId)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    create_overlayer_drm(&g_overlayDisplay, layerInfo);
    *layerId = LAYER_ID_VO_LAYER_TYPE_MARK;
    return DISPLAY_SUCCESS;
}

int32_t CloseOverlayLayer(uint32_t devId, uint32_t layerId)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    if (!CheckTypeIsOverlayLayer(layerId)) {
        HDF_LOGE("%s: layerId type is not overlay Layer", __func__);
        return DISPLAY_FAILURE;
    }
    if (VALID_FD(g_overlayDisplay.drmFd)) {
        close(g_overlayDisplay.drmFd);
        g_overlayDisplay.drmFd = INVALID_FD;
    }
    return DISPLAY_SUCCESS;
}

static int32_t OverlayLayerCopyFrame(ot_vgs_task_attr *vgsTaskAttr,
    ot_video_frame_info *videoFrameInfo, ot_vgs_handle *hHandle)
{
    int32_t ret = ss_mpi_vgs_begin_job(hHandle);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("mpi_vgs_begin_job failed, ret:0x%x", ret);
        return ret;
    }

    if (memcpy_s(&vgsTaskAttr->img_in, sizeof(ot_video_frame_info),
        videoFrameInfo, sizeof(ot_video_frame_info)) != EOK) {
        HDF_LOGE("memcpy_s img_in failed\n");
        ss_mpi_vgs_cancel_job(*hHandle);
        return DISPLAY_FAILURE;
    }

    if (memcpy_s(&vgsTaskAttr->img_out, sizeof(ot_video_frame_info),
        &vgsTaskAttr->img_in, sizeof(ot_video_frame_info)) != EOK) {
        HDF_LOGE("memcpy_s img_out failed\n");
        ss_mpi_vgs_cancel_job(*hHandle);
        return DISPLAY_FAILURE;
    }

    return DISPLAY_SUCCESS;
}

static void OverlayLayerCalcStrideSize(td_u32 *headStride, td_u32 *mainStride,
    td_u32 *headYSize, td_u32 *headSize, td_u32 *vbSize)
{
    td_u32 alignHeight = OT_ALIGN_UP(g_overlayDisplay.height, ALIGN_VALUE);

    if (g_overlayDisplay.width <= WIDTH_THRESHOLD_4K) {
        *headStride = HEAD_STRIDE_4K;
    } else if (g_overlayDisplay.width <= WIDTH_THRESHOLD_8K) {
        *headStride = HEAD_STRIDE_8K;
    } else {
        *headStride = HEAD_STRIDE_MAX;
    }

    *mainStride = OT_ALIGN_UP(g_overlayDisplay.width, ALIGN_VALUE);
    *headYSize = *headStride * alignHeight;
    *headSize = *headStride * alignHeight * YUV420_SIZE_RATIO_NUM / YUV420_SIZE_RATIO_DEN;

    size_t sizeY = g_overlayDisplay.width * g_overlayDisplay.height;
    size_t sizeUv = g_overlayDisplay.width * g_overlayDisplay.height / YUV420_PLANE_RATIO;
    td_u32 mainSize = sizeY + sizeUv;
    *vbSize = *headSize + mainSize;
}

static int32_t OverlayLayerAllocVbBuffer(td_u32 vbSize, ot_vb_blk *vbHandle,
    td_phys_addr_t *physAddr, td_void **virtAddr)
{
    *vbHandle = ss_mpi_vb_get_blk(OT_VB_INVALID_POOL_ID, vbSize, TD_NULL);
    if (*vbHandle == OT_VB_INVALID_HANDLE) {
        HDF_LOGE("mpi_vb_get_block failed!\n");
        return DISPLAY_FAILURE;
    }

    *physAddr = ss_mpi_vb_handle_to_phys_addr(*vbHandle);
    if (*physAddr == INVALID_PHYS_ADDR) {
        HDF_LOGE("mpi_vb_handle2_phys_addr failed!\n");
        ss_mpi_vb_release_blk(*vbHandle);
        return DISPLAY_FAILURE;
    }

    *virtAddr = (td_u8*)ss_mpi_sys_mmap(*physAddr, vbSize);
    if (*virtAddr == TD_NULL) {
        HDF_LOGE("mpi_sys_mmap failed!\n");
        ss_mpi_vb_release_blk(*vbHandle);
        return DISPLAY_FAILURE;
    }

    return DISPLAY_SUCCESS;
}

static void OverlayLayerSetFrameBasicAttr(ot_vgs_task_attr *vgsTaskAttr, ot_vb_blk vbHandle)
{
    vgsTaskAttr->img_out.mod_id  = OT_ID_VGS;
    vgsTaskAttr->img_out.pool_id = ss_mpi_vb_handle_to_pool_id(vbHandle);
    vgsTaskAttr->img_out.video_frame.width = g_overlayDisplay.displayRect.w;
    vgsTaskAttr->img_out.video_frame.height = g_overlayDisplay.displayRect.h;
    vgsTaskAttr->img_out.video_frame.field        = OT_VIDEO_FIELD_FRAME;
    vgsTaskAttr->img_out.video_frame.video_format = OT_VIDEO_FORMAT_LINEAR;
    vgsTaskAttr->img_out.video_frame.compress_mode = OT_COMPRESS_MODE_NONE;
    vgsTaskAttr->img_out.video_frame.dynamic_range = OT_DYNAMIC_RANGE_SDR8;
    vgsTaskAttr->img_out.video_frame.color_gamut   = OT_COLOR_GAMUT_BT601;
}

static void OverlayLayerSetFrameHeaderAttr(ot_vgs_task_attr *vgsTaskAttr,
    td_phys_addr_t physAddr, td_void *virtAddr, td_u32 headStride, td_u32 headYSize)
{
    vgsTaskAttr->img_out.video_frame.header_stride[PLANE_INDEX_Y]  = headStride;
    vgsTaskAttr->img_out.video_frame.header_stride[PLANE_INDEX_UV]  = headStride;
    vgsTaskAttr->img_out.video_frame.header_phys_addr[PLANE_INDEX_Y] = physAddr;
    vgsTaskAttr->img_out.video_frame.header_phys_addr[PLANE_INDEX_UV] =
        vgsTaskAttr->img_out.video_frame.header_phys_addr[PLANE_INDEX_Y] + headYSize;
    vgsTaskAttr->img_out.video_frame.header_virt_addr[PLANE_INDEX_Y] = virtAddr;
    vgsTaskAttr->img_out.video_frame.header_virt_addr[PLANE_INDEX_UV] =
        vgsTaskAttr->img_out.video_frame.header_virt_addr[PLANE_INDEX_Y] + headYSize;
}

static void OverlayLayerSetFrameMainAttr(ot_vgs_task_attr *vgsTaskAttr,
    td_u32 mainStride, td_u32 headSize, size_t sizeY)
{
    vgsTaskAttr->img_out.video_frame.stride[PLANE_INDEX_Y]  = mainStride;
    vgsTaskAttr->img_out.video_frame.stride[PLANE_INDEX_UV]  = mainStride;
    vgsTaskAttr->img_out.video_frame.phys_addr[PLANE_INDEX_Y] =
        vgsTaskAttr->img_out.video_frame.header_phys_addr[PLANE_INDEX_Y] + headSize;
    vgsTaskAttr->img_out.video_frame.phys_addr[PLANE_INDEX_UV] =
        vgsTaskAttr->img_out.video_frame.phys_addr[PLANE_INDEX_Y] + sizeY;
    vgsTaskAttr->img_out.video_frame.virt_addr[PLANE_INDEX_Y] =
        vgsTaskAttr->img_out.video_frame.header_virt_addr[PLANE_INDEX_Y] + headSize;
    vgsTaskAttr->img_out.video_frame.virt_addr[PLANE_INDEX_UV] =
        vgsTaskAttr->img_out.video_frame.virt_addr[PLANE_INDEX_Y] + sizeY;
}

int32_t OverlayLayerFlush(uint32_t devId, uint32_t layerId, LayerBuffer *buffer)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    int32_t ret = DISPLAY_FAILURE;
    ot_vgs_handle hHandle = INVALID_VGS_HANDLE;
    ot_vgs_task_attr vgsTaskAttr = {0};
    ot_video_frame_info *videoFrameInfo;
    ot_vgs_scale_coef_mode vgsSclCoefMode = OT_VGS_SCALE_COEF_NORM;
    td_u32 headStride, mainStride, headYSize, headSize, vbSize;
    ot_vb_blk vbHandle;
    td_phys_addr_t physAddr;
    td_void *virtAddr;

    if (!CheckTypeIsOverlayLayer(layerId)) {
        HDF_LOGE("%s: layerId type is not overlay Layer", __func__);
        return ret;
    }

    if (!buffer || !buffer->data.virAddr) {
        HDF_LOGE("%s: buffer no data", __func__);
        return ret;
    }

    videoFrameInfo = (ot_video_frame_info *)buffer->data.virAddr;

    ret = OverlayLayerCopyFrame(&vgsTaskAttr, videoFrameInfo, &hHandle);
    if (ret != DISPLAY_SUCCESS) {
        return ret;
    }

    OverlayLayerCalcStrideSize(&headStride, &mainStride, &headYSize, &headSize, &vbSize);

    ret = OverlayLayerAllocVbBuffer(vbSize, &vbHandle, &physAddr, &virtAddr);
    if (ret != DISPLAY_SUCCESS) {
        ss_mpi_vgs_cancel_job(hHandle);
        return ret;
    }

    OverlayLayerSetFrameBasicAttr(vgsTaskAttr, vbHandle);
    OverlayLayerSetFrameHeaderAttr(vgsTaskAttr, physAddr, virtAddr, headStride, headYSize);
    OverlayLayerSetFrameMainAttr(vgsTaskAttr, mainStride, headSize, sizeY);

    ret = ss_mpi_vgs_add_scale_task(hHandle, vgsTaskAttr, vgsSclCoefMode);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("mpi_vgs_add_scale_task failed, ret:0x%x", ret);
        goto release_munmap;
    }

    ret = ss_mpi_vgs_end_job(hHandle);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("mpi_vgs_end_job failed, ret:0x%x", ret);
        goto release_munmap;
    }

    ret = ioctl(g_overlayDisplay.drmFd, DRM_IOCTL_HI3403V100_OVERLAY_FLUSH,
        &(vgsTaskAttr.img_out.video_frame));
    if (ret < IOCTL_SUCCESS) {
        HDF_LOGE("%s: Failed to overlay_flush:%d\n", __func__, ret);
    }

release_munmap:
    ss_mpi_sys_munmap(virtAddr, vbSize);
release_vb_handle:
    ss_mpi_vb_release_blk(vbHandle);
    return ret;
}

int32_t SetOverlayLayerSize(uint32_t devId, uint32_t layerId, const IRect *rect)
{

    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    CHECK_NULLPOINTER_RETURN_VALUE(rect, DISPLAY_NULL_PTR);
    if (!CheckTypeIsOverlayLayer(layerId)) {
        HDF_LOGE("%s: layerId type is not overlay Layer", __func__);
        return DISPLAY_FAILURE;
    }

    g_overlayDisplay.displayRect.x = ALIGN_DOWN(rect->x, ALIGN_DOWN_NUM);
    g_overlayDisplay.displayRect.y = ALIGN_DOWN(rect->y, ALIGN_DOWN_NUM);
    g_overlayDisplay.displayRect.w = ALIGN_DOWN(rect->w, ALIGN_DOWN_NUM);
    g_overlayDisplay.displayRect.h = ALIGN_DOWN(rect->h, ALIGN_DOWN_NUM);
    return DISPLAY_SUCCESS;
}

int32_t InvokeOverlayLayerCmd(uint32_t devId, uint32_t layerId, uint32_t cmd, ...)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    (void)layerId;
    (void)cmd;
    return DISPLAY_SUCCESS;
}

int32_t GetOverlayLayerDeviceId(uint32_t devId, uint32_t layerId, uint32_t *deviceId)
{
    if (deviceId == NULL) {
        return DISPLAY_FAILURE;
    }
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    if (!CheckTypeIsOverlayLayer(layerId)) {
        HDF_LOGE("%s: layerId type is not overlay Layer", __func__);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

int32_t SetOverlayLayerPriority(uint32_t priority)
{
    (void)priority;
    return DISPLAY_SUCCESS;
}
