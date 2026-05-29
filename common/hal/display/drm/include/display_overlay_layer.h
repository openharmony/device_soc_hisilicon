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

#ifndef DISPLAY_OVERLAY_LAYER_H
#define DISPLAY_OVERLAY_LAYER_H
#include "display_layer_internal.h"

int32_t EnabelVideoLayer(uint32_t devId);
int32_t DisableVideoLayer(uint32_t devId);
int32_t CreateOverlayLayer(uint32_t devId, const LayerInfo *layerInfo, uint32_t *layerId);
int32_t SetOverlayLayerSize(uint32_t devId, uint32_t layerId, const IRect *rect);
int32_t InvokeOverlayLayerCmd(uint32_t devId, uint32_t layerId, uint32_t cmd, ...);
int32_t CloseOverlayLayer(uint32_t devId, uint32_t layerId);
int32_t OverlayLayerFlush(uint32_t devId, uint32_t layerId, LayerBuffer *buffer);
int32_t GetOverlayLayerDeviceId(uint32_t devId, uint32_t layerId, uint32_t *deviceId);


#define OT_VGS_TASK_PRIVATE_DATA_LEN   4
#define OT_VGS_QUAD_POINT_NUM          4
#define OT_VGS_CLUT_LEN                16
#define OT_VGS_STITCH_NUM              4

typedef td_s32 ot_vgs_handle;

typedef struct {
    ot_video_frame_info        img_in;               /* RW; Input picture. */
    ot_video_frame_info        img_out;              /* RW; Output picture. */
    td_u64                     private_data[OT_VGS_TASK_PRIVATE_DATA_LEN];      /* RW; private data of task. */
    td_u32                     reserved;             /* RW; Debug information,state of current picture. */
} ot_vgs_task_attr;

typedef enum {
    OT_VGS_SCALE_COEF_NORM = 0,                            /* Normal scale coefficient. */
    OT_VGS_SCALE_COEF_TAP2,                                /* Scale coefficient of 2 tap. */
    OT_VGS_SCALE_COEF_TAP4,                                /* Scale coefficient of 4 tap. */
    OT_VGS_SCALE_COEF_TAP6,                                /* Scale coefficient of 6 tap. */
    OT_VGS_SCALE_COEF_TAP8,                                /* Scale coefficient of 8 tap. */
    OT_VGS_SCALE_COEF_BUTT
} ot_vgs_scale_coef_mode;

#define ALIGN_DOWN_NUM 2
#include "ot_math.h"
#endif // DISPLAY_OVERLAY_LAYER_H