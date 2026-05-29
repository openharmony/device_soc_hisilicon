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
#ifndef __SS_MPI_GDC_H__
#define __SS_MPI_GDC_H__

#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_gdc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Description: Begin a gdc job,then add task into the job,gdc will finish all the task in the job. */
td_s32 ss_mpi_gdc_begin_job(ot_gdc_handle *handle);

/* Description: End a job,all tasks in the job will be submmitted to gdc */
td_s32 ss_mpi_gdc_end_job(ot_gdc_handle handle);

/* Description: Cancel a job ,then all tasks in the job will not be submmitted to gdc */
td_s32 ss_mpi_gdc_cancel_job(ot_gdc_handle handle);

/* Description: Add a task to a gdc job */
td_s32 ss_mpi_gdc_add_correction_task(ot_gdc_handle handle, const ot_gdc_task_attr *task,
    const ot_fisheye_attr *fisheye_attr);

/* Description: Add a task to a gdc job */
td_s32 ss_mpi_gdc_add_correction_ex_task(ot_gdc_handle h_handle, const ot_gdc_task_attr *task,
    const ot_fisheye_attr_ex *fisheye_attrr_ex, td_bool check_mode);

/* Description: Set Config */
td_s32 ss_mpi_gdc_set_cfg(ot_gdc_handle handle, const ot_gdc_fisheye_job_cfg *job_cfg);

/* Description: Set Config */
td_s32 ss_mpi_gdc_add_pmf_task(ot_gdc_handle handle, const ot_gdc_task_attr *task,
    const ot_gdc_pmf_attr *gdc_pmf_attr);

td_s32 ss_mpi_gdc_fisheye_pos_query_dst_to_src(const ot_gdc_fisheye_point_query_attr *fisheye_point_query_attr,
    const ot_video_frame_info *video_info, const ot_point *dst_point, ot_point *src_point);

td_s32 ss_mpi_gdc_fisheye_multi_pos_query_dst_to_src(const ot_gdc_fisheye_point_query_attr *fisheye_point_query_attr,
    const ot_video_frame_info *video_info, td_u32 num, const ot_point *dst, ot_point *src);

td_s32 ss_mpi_gdc_fisheye_multi_pos_query_src_to_dst(const ot_gdc_fisheye_point_query_attr *fisheye_point_query_attr,
    const ot_video_frame_info *video_info, td_u32 num, const ot_point *src, ot_point *dst);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MPI_GDC_ADAPT_H__ */