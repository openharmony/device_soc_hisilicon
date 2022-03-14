/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
#ifndef __MPI_GDC_H__
#define __MPI_GDC_H__

#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_gdc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*
 * Description : Begin a gdc job,then add task into the job,gdc will finish all the task in the job.
 * Input       : GDC_HANDLE *phHandle
 * Output      : None
 */
HI_S32 HI_MPI_GDC_BeginJob(GDC_HANDLE *phHandle);

/*
 * Description : End a job,all tasks in the job will be submmitted to gdc
 * Input       : GDC_HANDLE  hHandle
 * Output      : None
 */
HI_S32 HI_MPI_GDC_EndJob(GDC_HANDLE hHandle);

/*
 * Description : Cancel a job ,then all tasks in the job will not be submmitted to gdc
 * Input       : GDC_HANDLE  hHandle
 * Output      : None
 */
HI_S32 HI_MPI_GDC_CancelJob(GDC_HANDLE hHandle);

/*
 * Description : Add a task to a gdc job
 * Input       : GDC_HANDLE  hHandle
 *               const GDC_TASK_ATTR_S *pstTask
 *               const FISHEYE_ATTR_S *pstFisheyeAttr
 * Output      : None
 */
HI_S32 HI_MPI_GDC_AddCorrectionTask(GDC_HANDLE hHandle, const GDC_TASK_ATTR_S *pstTask,
                                    const FISHEYE_ATTR_S *pstFisheyeAttr);

/*
 * Description : Add a task to a gdc job
 * Input       : GDC_HANDLE  hHandle
 *               const GDC_TASK_ATTR_S *pstTask
 *               const FISHEYE_ATTR_EX_S *pstFishEyeAttrEx
 *               HI_BOOL bCheckMode
 * Output      : None
 */
HI_S32 HI_MPI_GDC_AddCorrectionExTask(GDC_HANDLE hHandle, const GDC_TASK_ATTR_S *pstTask,
                                      const FISHEYE_ATTR_EX_S *pstFishEyeAttrEx, HI_BOOL bCheckMode);

/*
 * Description : Set Config
 * Input       : GDC_HANDLE  hHandle
 *               const FISHEYE_JOB_CONFIG_S *stJobConfig
 * Output       : None
 */
HI_S32 HI_MPI_GDC_SetConfig(GDC_HANDLE hHandle, const FISHEYE_JOB_CONFIG_S *pstJobConfig);

/*
 * Description : Set Config
 * Input       : GDC_HANDLE  hHandle
 *               const GDC_TASK_ATTR_S* pstTask
 *               const GDC_PMF_ATTR_S* pstGdcPmfAttr
 * Output      : None
 */
HI_S32 HI_MPI_GDC_AddPMFTask(GDC_HANDLE hHandle, const GDC_TASK_ATTR_S *pstTask, const GDC_PMF_ATTR_S *pstGdcPmfAttr);

/*
 * Description : Query source point from correction point
 * Input       : const GDC_FISHEYE_POINT_QUERY_ATTR_S *pstFisheyePointQueryAttr
 *               const VIDEO_FRAME_INFO_S *pstVideoInfo
 *               const POINT_S *pstDstPoint
 *               POINT_S *pstSrcPoint
 * Output      : None
 */
HI_S32 HI_MPI_GDC_FisheyePosQueryDst2Src(const GDC_FISHEYE_POINT_QUERY_ATTR_S *pstFisheyePointQueryAttr,
    const VIDEO_FRAME_INFO_S *pstVideoInfo, const POINT_S *pstDstPoint, POINT_S *pstSrcPoint);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_GDC_H__ */

