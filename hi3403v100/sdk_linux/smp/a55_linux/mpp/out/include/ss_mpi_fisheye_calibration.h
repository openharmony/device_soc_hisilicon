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
#ifndef __SS_MPI_FISHEYE_CALIBRATION_H__
#define __SS_MPI_FISHEYE_CALIBRATION_H__

#include "ot_common_video.h"
#include "ot_common_fisheye_calibration.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_fisheye_calibration_compute(const ot_video_frame *frame_in, ot_fisheye_calibration_level level,
    ot_fisheye_calibration_output *calibration_out);

td_s32 ss_mpi_fisheye_calibration_mark(const ot_video_frame *frame_in, const ot_video_frame *frame_out,
    const ot_fisheye_calibration_result *calibration_result);

#ifdef __cplusplus
}
#endif

#endif /* __SS_MPI_FISHEYE_CALIBRATION_H__ */
