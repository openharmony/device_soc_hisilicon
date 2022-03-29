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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include "sample_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 SAMPLE_COMM_VPSS_Start(VPSS_GRP VpssGrp, HI_BOOL *pabChnEnable, VPSS_GRP_ATTR_S *pstVpssGrpAttr,
    VPSS_CHN_ATTR_S *pastVpssChnAttr)
{
    VPSS_CHN VpssChn;
    HI_S32 s32Ret;
    HI_S32 j;

    if ((pabChnEnable == NULL) || (pstVpssGrpAttr == NULL) || (pastVpssChnAttr == NULL)) {
        SAMPLE_PRT("null pointer!\n");
        return HI_FAILURE;
    }
    s32Ret = HI_MPI_VPSS_CreateGrp(VpssGrp, pstVpssGrpAttr);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VPSS_CreateGrp(grp:%d) failed with %#x!\n", VpssGrp, s32Ret);
        return HI_FAILURE;
    }

    for (j = 0; j < VPSS_MAX_PHY_CHN_NUM; j++) {
        if (pabChnEnable[j] == HI_TRUE) {
            VpssChn = j;
            s32Ret = HI_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn, &pastVpssChnAttr[VpssChn]);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("HI_MPI_VPSS_SetChnAttr failed with %#x\n", s32Ret);
                return HI_FAILURE;
            }

            s32Ret = HI_MPI_VPSS_EnableChn(VpssGrp, VpssChn);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("HI_MPI_VPSS_EnableChn failed with %#x\n", s32Ret);
                return HI_FAILURE;
            }
        }
    }

    s32Ret = HI_MPI_VPSS_StartGrp(VpssGrp);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("HI_MPI_VPSS_StartGrp failed with %#x\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VPSS_Stop(VPSS_GRP VpssGrp, HI_BOOL *pabChnEnable)
{
    HI_S32 j;
    HI_S32 s32Ret = HI_SUCCESS;
    VPSS_CHN VpssChn;

    if (pabChnEnable == NULL) {
        SAMPLE_PRT("null pointer!\n");
        return HI_FAILURE;
    }
    for (j = 0; j < VPSS_MAX_PHY_CHN_NUM; j++) {
        if (pabChnEnable[j] == HI_TRUE) {
            VpssChn = j;
            s32Ret = HI_MPI_VPSS_DisableChn(VpssGrp, VpssChn);
            if (s32Ret != HI_SUCCESS) {
                SAMPLE_PRT("failed with %#x!\n", s32Ret);
                return HI_FAILURE;
            }
        }
    }

    s32Ret = HI_MPI_VPSS_StopGrp(VpssGrp);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VPSS_DestroyGrp(VpssGrp);
    if (s32Ret != HI_SUCCESS) {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
