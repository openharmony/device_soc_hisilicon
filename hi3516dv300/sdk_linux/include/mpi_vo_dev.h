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

#ifndef __MPI_VO_DEV_H__
#define __MPI_VO_DEV_H__

#include "hi_comm_vo_dev.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* Device Relative Settings */
HI_S32 HI_MPI_VO_SetPubAttr(VO_DEV VoDev, const VO_PUB_ATTR_S *pstPubAttr);
HI_S32 HI_MPI_VO_GetPubAttr(VO_DEV VoDev, VO_PUB_ATTR_S *pstPubAttr);

HI_S32 HI_MPI_VO_Enable(VO_DEV VoDev);
HI_S32 HI_MPI_VO_Disable(VO_DEV VoDev);

HI_S32 HI_MPI_VO_CloseFd(HI_VOID);
HI_S32 HI_MPI_VO_SetUserIntfSyncInfo(VO_DEV VoDev, VO_USER_INTFSYNC_INFO_S *pstUserInfo);

/* Graphic Relative Settings */
HI_S32 HI_MPI_VO_BindGraphicLayer(GRAPHIC_LAYER GraphicLayer, VO_DEV VoDev);
HI_S32 HI_MPI_VO_UnBindGraphicLayer(GRAPHIC_LAYER GraphicLayer, VO_DEV VoDev);

HI_S32 HI_MPI_VO_SetGraphicLayerCSC(GRAPHIC_LAYER GraphicLayer, const VO_CSC_S *pstCSC);
HI_S32 HI_MPI_VO_GetGraphicLayerCSC(GRAPHIC_LAYER GraphicLayer, VO_CSC_S *pstCSC);

HI_S32 HI_MPI_VO_SetDevFrameRate(VO_DEV VoDev, HI_U32 u32FrameRate);
HI_S32 HI_MPI_VO_GetDevFrameRate(VO_DEV VoDev, HI_U32 *pu32FrameRate);

/* Module Parameter Settings */
HI_S32 HI_MPI_VO_SetModParam(const VO_MOD_PARAM_S *pstModParam);
HI_S32 HI_MPI_VO_GetModParam(VO_MOD_PARAM_S *pstModParam);
HI_S32 HI_MPI_VO_SetVtth(VO_DEV VoDev, HI_U32 u32Vtth);
HI_S32 HI_MPI_VO_GetVtth(VO_DEV VoDev, HI_U32 *pu32Vtth);
HI_S32 HI_MPI_VO_SetVtth2(VO_DEV VoDev, HI_U32 u32Vtth);
HI_S32 HI_MPI_VO_GetVtth2(VO_DEV VoDev, HI_U32 *pu32Vtth);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_VO_DEV_H__ */
