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

#ifndef __SS_MPI_DPU_RECT_H__
#define __SS_MPI_DPU_RECT_H__

#include "ot_common_dpu_rect.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_dpu_rect_load_lut(const ot_dpu_rect_mem_info *lut_mem, ot_dpu_rect_lut_id *rect_lut_id);
td_s32 ss_mpi_dpu_rect_unload_lut(ot_dpu_rect_lut_id rect_lut_id);

td_s32 ss_mpi_dpu_rect_create_grp(ot_dpu_rect_grp rect_grp, const ot_dpu_rect_grp_attr *grp_attr);
td_s32 ss_mpi_dpu_rect_destroy_grp(ot_dpu_rect_grp rect_grp);

td_s32 ss_mpi_dpu_rect_set_grp_attr(ot_dpu_rect_grp rect_grp, const ot_dpu_rect_grp_attr *grp_attr);
td_s32 ss_mpi_dpu_rect_get_grp_attr(ot_dpu_rect_grp rect_grp, ot_dpu_rect_grp_attr *grp_attr);

td_s32 ss_mpi_dpu_rect_set_chn_attr(ot_dpu_rect_grp rect_grp, ot_dpu_rect_chn rect_chn,
    const ot_dpu_rect_chn_attr *chn_attr);
td_s32 ss_mpi_dpu_rect_get_chn_attr(ot_dpu_rect_grp rect_grp, ot_dpu_rect_chn rect_chn,
    ot_dpu_rect_chn_attr *chn_attr);

td_s32 ss_mpi_dpu_rect_enable_chn(ot_dpu_rect_grp rect_grp, ot_dpu_rect_chn rect_chn);
td_s32 ss_mpi_dpu_rect_disable_chn(ot_dpu_rect_grp rect_grp, ot_dpu_rect_chn rect_chn);

td_s32 ss_mpi_dpu_rect_start_grp(ot_dpu_rect_grp rect_grp);
td_s32 ss_mpi_dpu_rect_stop_grp(ot_dpu_rect_grp rect_grp);

td_s32 ss_mpi_dpu_rect_get_frame(ot_dpu_rect_grp rect_grp, td_s32 milli_sec, ot_dpu_rect_frame_info *rect_frame_info);

td_s32 ss_mpi_dpu_rect_release_frame(ot_dpu_rect_grp rect_grp, const ot_dpu_rect_frame_info *rect_frame_info);

td_s32 ss_mpi_dpu_rect_send_frame(ot_dpu_rect_grp rect_grp, const ot_video_frame_info *left_frame,
    const ot_video_frame_info *right_frame, td_s32 milli_sec);

#ifdef __cplusplus
}
#endif

#endif /* __SS_MPI_DPU_RECT_H__ */
