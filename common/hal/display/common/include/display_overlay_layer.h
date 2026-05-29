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
int32_t VpssSendFrame(ot_vpss_grp grp, ot_vpss_grp_pipe pipe, const ot_video_frame_info *frameInfo, uint32_t milliSec);
#endif // DISPLAY_OVERLAY_LAYER_H