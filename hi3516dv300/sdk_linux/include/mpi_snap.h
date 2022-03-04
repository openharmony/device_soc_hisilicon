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

#ifndef MPI_SNAP_H
#define MPI_SNAP_H

#include "hi_comm_video.h"
#include "hi_comm_snap.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

HI_S32 HI_MPI_SNAP_SetPipeAttr(VI_PIPE ViPipe, const SNAP_ATTR_S *pstSnapAttr);
HI_S32 HI_MPI_SNAP_GetPipeAttr(VI_PIPE ViPipe, SNAP_ATTR_S *pstSnapAttr);
HI_S32 HI_MPI_SNAP_EnablePipe(VI_PIPE ViPipe);
HI_S32 HI_MPI_SNAP_DisablePipe(VI_PIPE ViPipe);
HI_S32 HI_MPI_SNAP_TriggerPipe(VI_PIPE ViPipe);

HI_S32 HI_MPI_SNAP_MultiTrigger(VI_STITCH_GRP StitchGrp);

/* HDR */
HI_S32 HI_MPI_SNAP_SetProSharpenParam(VI_PIPE ViPipe, const ISP_PRO_SHARPEN_PARAM_S *pstIspShpParam);
HI_S32 HI_MPI_SNAP_GetProSharpenParam(VI_PIPE ViPipe, ISP_PRO_SHARPEN_PARAM_S *pstIspShpParam);
HI_S32 HI_MPI_SNAP_SetProBNRParam(VI_PIPE ViPipe, const ISP_PRO_BNR_PARAM_S *pstNrParam);
HI_S32 HI_MPI_SNAP_GetProBNRParam(VI_PIPE ViPipe, ISP_PRO_BNR_PARAM_S *pstNrParam);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* MPI_SNAP_H */
