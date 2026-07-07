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
    int drm_fd;
    uint32_t width;
    uint32_t height;
    IRect displayRect;
} drm_overlay_display_t;

drm_overlay_display_t  overlay_display;

#define DRM_HI3403V100_OVERLAY_FLUSH      1
#define DRM_IOCTL_HI3403V100_OVERLAY_FLUSH \
    DRM_IOWR(DRM_COMMAND_BASE + DRM_HI3403V100_OVERLAY_FLUSH, ot_video_frame_info)

static bool CheckTypeIsOverlayLayer(uint32_t layerId)
{
    return (layerId & LAYER_ID_VO_LAYER_TYPE_MARK) != 0;
}

static int create_overlayer_drm(drm_overlay_display_t *display, const LayerInfo *layerInfo) {
    memset(&overlay_display, 0, sizeof(overlay_display));
    display->drm_fd = open(DRM_DEVICE, O_RDWR | O_CLOEXEC);
    if (display->drm_fd == INVALID_FD) {
        HDF_LOGE("%s,Cannot open DRM device\n",__func__);
        return DISPLAY_FAILURE;
    }
    display->width = layerInfo->width;
    display->height = layerInfo->height;
    return DISPLAY_SUCCESS;
}

int32_t CreateOverlayLayer(uint32_t devId, const LayerInfo *layerInfo, uint32_t *layerId)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    create_overlayer_drm(&overlay_display, layerInfo);
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
    if (VALID_FD(overlay_display.drm_fd)) {
        close(overlay_display.drm_fd);
        overlay_display.drm_fd = INVALID_FD;
    }
    return DISPLAY_SUCCESS;
}

int32_t OverlayLayerFlush(uint32_t devId, uint32_t layerId, LayerBuffer *buffer)
{
    CHECK_DEVID_VALID(devId, DISPLAY_FAILURE);
    int32_t ret = DISPLAY_FAILURE;
    ot_vgs_handle h_handle = INVALID_VGS_HANDLE;
    ot_vgs_task_attr vgs_task_attr = {0};
    ot_video_frame_info *video_frame_info;
    ot_vgs_scale_coef_mode vgs_scl_coef_mode = OT_VGS_SCALE_COEF_NORM;
    size_t size_y = overlay_display.width *  overlay_display.height;
    size_t size_uv =  overlay_display.width * overlay_display.height / YUV420_PLANE_RATIO;
    td_u32 align_height;
    td_u32 vb_size;
    td_u32 head_stride;
    td_u32 head_size;
    td_u32 head_y_size;
    td_u32 main_stride;
    td_u32 main_size;
    ot_vb_blk vb_handle;
    td_phys_addr_t phys_addr;
    td_void *virt_addr;

    if (!CheckTypeIsOverlayLayer(layerId)) {
        HDF_LOGE("%s: layerId type is not overlay Layer", __func__);
        return ret;
    }

    if (!buffer || !buffer->data.virAddr) {
        HDF_LOGE("%s: buffer no data", __func__);
        return ret;
    }

    video_frame_info = (ot_video_frame_info *)buffer->data.virAddr;

    /* step3: create VGS job */
    ret = ss_mpi_vgs_begin_job(&h_handle);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("mpi_vgs_begin_job failed, ret:0x%x", ret);
        goto exit_and_release;
    }

    /* step4: add VGS task */
    if (memcpy_s(&vgs_task_attr.img_in, sizeof(ot_video_frame_info),
           video_frame_info, sizeof(ot_video_frame_info)) != EOK) {
        HDF_LOGE("memcpy_s img_in failed\n");
        goto release_cancel_job;
    }

    if (memcpy_s(&vgs_task_attr.img_out, sizeof(ot_video_frame_info),
        &vgs_task_attr.img_in, sizeof(ot_video_frame_info)) != EOK) {
        HDF_LOGE("memcpy_s img_out failed\n");
        goto release_cancel_job;
    }

    align_height = OT_ALIGN_UP(overlay_display.height, ALIGN_VALUE);

    if (overlay_display.width <= WIDTH_THRESHOLD_4K) {
        head_stride = HEAD_STRIDE_4K;
    } else if (overlay_display.width <= WIDTH_THRESHOLD_8K) {
        head_stride = HEAD_STRIDE_8K;
    } else {
        head_stride = HEAD_STRIDE_MAX;
    }
    main_stride  = OT_ALIGN_UP(overlay_display.width, ALIGN_VALUE);
    head_y_size =  head_stride * align_height;
    head_size  = head_stride * align_height * YUV420_SIZE_RATIO_NUM / YUV420_SIZE_RATIO_DEN;
    main_size = size_y + size_uv;
    vb_size = head_size + main_size;

    vb_handle= ss_mpi_vb_get_blk(OT_VB_INVALID_POOL_ID, vb_size, TD_NULL);
    if (vb_handle== OT_VB_INVALID_HANDLE) {
        HDF_LOGE("mpi_vb_get_block failed!\n");
        goto release_cancel_job;
    }
    phys_addr = ss_mpi_vb_handle_to_phys_addr(vb_handle);
    if (phys_addr == INVALID_PHYS_ADDR) {
        HDF_LOGE("mpi_vb_handle2_phys_addr failed!\n");
        goto release_vb_handle;
    }

    virt_addr = (td_u8*)ss_mpi_sys_mmap(phys_addr, vb_size);
    if (virt_addr == TD_NULL) {
        HDF_LOGE("mpi_sys_mmap failed!\n");
        goto release_vb_handle;
    }

    vgs_task_attr.img_out.mod_id  = OT_ID_VGS;
    vgs_task_attr.img_out.pool_id = ss_mpi_vb_handle_to_pool_id(vb_handle);
    vgs_task_attr.img_out.video_frame.width = overlay_display.displayRect.w;
    vgs_task_attr.img_out.video_frame.height = overlay_display.displayRect.h;
    vgs_task_attr.img_out.video_frame.field        = OT_VIDEO_FIELD_FRAME;
    vgs_task_attr.img_out.video_frame.video_format = OT_VIDEO_FORMAT_LINEAR;
    vgs_task_attr.img_out.video_frame.compress_mode = OT_COMPRESS_MODE_NONE;
    vgs_task_attr.img_out.video_frame.dynamic_range = OT_DYNAMIC_RANGE_SDR8;
    vgs_task_attr.img_out.video_frame.color_gamut   = OT_COLOR_GAMUT_BT601;
    vgs_task_attr.img_out.video_frame.header_stride[PLANE_INDEX_Y]  = head_stride;
    vgs_task_attr.img_out.video_frame.header_stride[PLANE_INDEX_UV]  = head_stride;
    vgs_task_attr.img_out.video_frame.header_phys_addr[PLANE_INDEX_Y] = phys_addr;
    vgs_task_attr.img_out.video_frame.header_phys_addr[PLANE_INDEX_UV] = vgs_task_attr.img_out.video_frame.header_phys_addr[PLANE_INDEX_Y] +
                                                   head_y_size;
    vgs_task_attr.img_out.video_frame.header_virt_addr[PLANE_INDEX_Y] = virt_addr;
    vgs_task_attr.img_out.video_frame.header_virt_addr[PLANE_INDEX_UV] = vgs_task_attr.img_out.video_frame.header_virt_addr[PLANE_INDEX_Y] +
                                                   head_y_size;

    vgs_task_attr.img_out.video_frame.stride[PLANE_INDEX_Y]  = main_stride;
    vgs_task_attr.img_out.video_frame.stride[PLANE_INDEX_UV]  = main_stride;
    vgs_task_attr.img_out.video_frame.phys_addr[PLANE_INDEX_Y] = vgs_task_attr.img_out.video_frame.header_phys_addr[PLANE_INDEX_Y] + head_size;
    vgs_task_attr.img_out.video_frame.phys_addr[PLANE_INDEX_UV] = vgs_task_attr.img_out.video_frame.phys_addr[PLANE_INDEX_Y] + size_y;
    vgs_task_attr.img_out.video_frame.virt_addr[PLANE_INDEX_Y] = vgs_task_attr.img_out.video_frame.header_virt_addr[PLANE_INDEX_Y] + head_size;
    vgs_task_attr.img_out.video_frame.virt_addr[PLANE_INDEX_UV] = vgs_task_attr.img_out.video_frame.virt_addr[PLANE_INDEX_Y] + size_y;

    ret = ss_mpi_vgs_add_scale_task(h_handle, &vgs_task_attr, vgs_scl_coef_mode);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("mpi_vgs_add_scale_task failed, ret:0x%x", ret);
        goto release_munmap;
    }

    /* step5: start VGS work */
    ret = ss_mpi_vgs_end_job(h_handle);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("mpi_vgs_end_job failed, ret:0x%x", ret);
        goto release_munmap;
    }

    ret = ioctl(overlay_display.drm_fd, DRM_IOCTL_HI3403V100_OVERLAY_FLUSH, &(vgs_task_attr.img_out.video_frame));
    if (ret < IOCTL_SUCCESS) {
        HDF_LOGE("%s: Failed to overlay_flush:%d\n",__func__,ret);
    }
release_munmap:
    ss_mpi_sys_munmap(virt_addr, vb_size);
release_vb_handle:
    ss_mpi_vb_release_blk(vb_handle);
    return ret;
release_cancel_job:
    ss_mpi_vgs_cancel_job(h_handle);
exit_and_release:
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

    overlay_display.displayRect.x = ALIGN_DOWN(rect->x, ALIGN_DOWN_NUM);
    overlay_display.displayRect.y = ALIGN_DOWN(rect->y, ALIGN_DOWN_NUM);
    overlay_display.displayRect.w = ALIGN_DOWN(rect->w, ALIGN_DOWN_NUM);
    overlay_display.displayRect.h = ALIGN_DOWN(rect->h, ALIGN_DOWN_NUM);
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
