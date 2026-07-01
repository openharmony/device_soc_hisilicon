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

#include "display_overlay_layer.h"

int32_t VpssSendFrame(ot_vpss_grp grp, ot_vpss_grp_pipe pipe, const ot_video_frame_info *frameInfo, uint32_t milliSec)
{
    (void)(pipe);
    return ss_mpi_vpss_send_frame(grp, frameInfo, milliSec);
}