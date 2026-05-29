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

#ifndef __SS_MPI_DPU_MATCH_H__
#define __SS_MPI_DPU_MATCH_H__

#include "ot_common_dpu_match.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_dpu_match_get_assist_buf_size(td_u16 disparity_num, td_u32 dst_height, td_u32 *size);
td_s32 ss_mpi_dpu_match_create_grp(ot_dpu_match_grp match_grp, const ot_dpu_match_grp_attr *grp_attr);
td_s32 ss_mpi_dpu_match_destroy_grp(ot_dpu_match_grp match_grp);
td_s32 ss_mpi_dpu_match_set_grp_attr(ot_dpu_match_grp match_grp, const ot_dpu_match_grp_attr *grp_attr);
td_s32 ss_mpi_dpu_match_get_grp_attr(ot_dpu_match_grp match_grp, ot_dpu_match_grp_attr *grp_attr);
td_s32 ss_mpi_dpu_match_set_grp_cost_param(ot_dpu_match_grp match_grp, const ot_dpu_match_cost_param *cost_param);
td_s32 ss_mpi_dpu_match_get_grp_cost_param(ot_dpu_match_grp match_grp, ot_dpu_match_cost_param *cost_param);
td_s32 ss_mpi_dpu_match_start_grp(ot_dpu_match_grp match_grp);
td_s32 ss_mpi_dpu_match_stop_grp(ot_dpu_match_grp match_grp);

td_s32 ss_mpi_dpu_match_set_chn_attr(ot_dpu_match_grp match_grp, ot_dpu_match_chn match_chn,
    const ot_dpu_match_chn_attr *chn_attr);
td_s32 ss_mpi_dpu_match_get_chn_attr(ot_dpu_match_grp match_grp, ot_dpu_match_chn match_chn,
    ot_dpu_match_chn_attr *chn_attr);
td_s32 ss_mpi_dpu_match_enable_chn(ot_dpu_match_grp match_grp, ot_dpu_match_chn match_chn);
td_s32 ss_mpi_dpu_match_disable_chn(ot_dpu_match_grp match_grp, ot_dpu_match_chn match_chn);

td_s32 ss_mpi_dpu_match_get_frame(ot_dpu_match_grp match_grp, td_s32 milli_sec,
    ot_dpu_match_frame_info *src_frame_info, ot_video_frame_info *dst_frame);
td_s32 ss_mpi_dpu_match_release_frame(ot_dpu_match_grp match_grp, const ot_dpu_match_frame_info *src_frame_info,
    const ot_video_frame_info *dst_frame);
td_s32 ss_mpi_dpu_match_send_frame(ot_dpu_match_grp match_grp, const ot_dpu_match_frame_info *src_frame_info,
    td_s32 milli_sec);
td_s32 ss_mpi_dpu_match_set_grp_param(ot_dpu_match_grp match_grp, const ot_dpu_match_param *param);
td_s32 ss_mpi_dpu_match_get_grp_param(ot_dpu_match_grp match_grp, ot_dpu_match_param *param);
#ifdef __cplusplus
}
#endif

#endif /* __SS_MPI_DPU_MATCH_H__ */
