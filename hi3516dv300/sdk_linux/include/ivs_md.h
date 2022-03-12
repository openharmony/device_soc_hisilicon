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

#ifndef _HI_IVS_MD_H_
#define _HI_IVS_MD_H_

#include "hi_md.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 HI_IVS_MD_Init(HI_VOID);

HI_S32 HI_IVS_MD_Exit(HI_VOID);

HI_S32 HI_IVS_MD_CreateChn(MD_CHN MdChn, MD_ATTR_S *pstMdAttr);

HI_S32 HI_IVS_MD_DestroyChn(MD_CHN MdChn);

HI_S32 HI_IVS_MD_SetChnAttr(MD_CHN MdChn, MD_ATTR_S *pstMdAttr);

HI_S32 HI_IVS_MD_GetChnAttr(MD_CHN MdChn, MD_ATTR_S *pstMdAttr);

HI_S32 HI_IVS_MD_GetBg(MD_CHN MdChn, IVE_DST_IMAGE_S *pstBg);

HI_S32 HI_IVS_MD_Process(MD_CHN MdChn, IVE_SRC_IMAGE_S *pstCur, IVE_SRC_IMAGE_S *pstRef,
    IVE_DST_IMAGE_S *pstSad, IVE_DST_MEM_INFO_S *pstBlob);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* _HI_IVS_MD_H_ */