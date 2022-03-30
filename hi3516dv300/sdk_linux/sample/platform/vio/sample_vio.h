/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SAMPLE_VIO_H
#define SAMPLE_VIO_H

#include "hi_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef SAMPLE_PRT
#define SAMPLE_PRT(fmt...)                           \
    do {                                             \
        printf("[%s]-%d: ", __FUNCTION__, __LINE__); \
        printf(fmt);                                 \
    } while (0)
#endif

#ifndef PAUSE
#define PAUSE()                                                             \
    do {                                                                    \
        printf("---------------press Enter key to exit!---------------\n"); \
        getchar();                                                          \
    } while (0)
#endif

hi_void SAMPLE_VIO_GetChar(hi_void);
HI_S32 SAMPLE_VIO_ViOnlineVpssOfflineRoute(HI_U32 u32VoIntfType);
HI_S32 SAMPLE_VIO_WDR_LDC_DIS_SPREAD(HI_U32 u32VoIntfType);
HI_S32 SAMPLE_VIO_ViDoublePipeRoute(HI_U32 u32VoIntfType);
HI_S32 SAMPLE_VIO_ViWdrSwitch(HI_U32 u32VoIntfType);
HI_S32 SAMPLE_VIO_ViVpssLowDelay(HI_U32 u32VoIntfType);
HI_S32 SAMPLE_VIO_Rotate(HI_U32 u32VoIntfType);
HI_S32 SAMPLE_VIO_FPN(HI_U32 u32VoIntfType);
HI_S32 SAMPLE_VIO_ViDoubleWdrPipe(HI_U32 u32VoIntfType);
HI_S32 SAMPLE_VIO_VPSS_VO_MIPI_TX(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef SAMPLE_VIO_H */
