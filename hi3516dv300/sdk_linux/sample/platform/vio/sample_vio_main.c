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
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "hi_common.h"
#include "sample_vio.h"
#include "mpi_sys.h"
#include "sdk.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

static volatile sig_atomic_t g_sample_vio_exit = 0;
hi_void SAMPLE_VIO_GetChar(hi_void)
{
    if (g_sample_vio_exit == 1) {
        return;
    }
    PAUSE();
}

static void SAMPLE_VIO_HandleSig(HI_S32 signo)
{
    if (g_sample_vio_exit == 1) {
        return;
    }

    if (signo == SIGINT || signo == SIGTERM) {
        g_sample_vio_exit = 1;
    }
}

#if (!defined(__HuaweiLite__)) || defined(__OHOS__)
static hi_void SAMPLE_VIO_SIGNAL(void (*func)(int))
{
    struct sigaction sa = { 0 };

    sa.sa_handler = func;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, HI_NULL);
    sigaction(SIGTERM, &sa, HI_NULL);
}
#endif

static void SAMPLE_VIO_Usage(const char *sPrgNm)
{
    printf("Usage : %s <index>\n", sPrgNm);
    printf("index:\n");
    printf("\t 0)VI (Online) - VPSS(Offline) - VO.\n");
    printf("\t 1)WDR(Offline)- VPSS(Offline) - VO. LDC+DIS+SPREAD.\n");
    printf("\t 2)GDC - VPSS LowDelay.\n");
    printf("\t 3)Double WDR Pipe.\n");
    printf("\t 4)FPN Calibrate & Correction.\n");
    printf("\t 5)WDR Switch.\n");
    printf("\t 6)90/180/270/0/free Rotate.\n");
    printf("\t 7)VI-VPSS-VO(MIPI_TX).\n\n");

    printf("\t Hi3516DV300/Hi3559V200/Hi3556V200) vo HDMI output.\n");
    printf("\t Hi3516CV500) vo BT1120 output.\n");
    printf("\t If you have any questions, please look at readme.txt!\n");
    return;
}


#if defined(__HuaweiLite__) && (!defined(__OHOS__))
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 s32Index;
    HI_U32 u32VoIntfType = 0;
    HI_U32 u32ChipId = 0xffffffff;
    sdk_init();
    if (argc != 2) { /* 2:arg num */
        SAMPLE_VIO_Usage(argv[0]);
        return HI_FAILURE;
    }

    if (strncmp(argv[1], "-h", 2) == 0) { /* 2:arg num */
        SAMPLE_VIO_Usage(argv[0]);
        return HI_FAILURE;
    }

    if (strlen(argv[1]) != 1) {
        SAMPLE_VIO_Usage(argv[0]);
        return HI_FAILURE;
    }

    if (argv[1][0] < '0' || argv[1][0] > '9') {
        SAMPLE_VIO_Usage(argv[0]);
        return HI_FAILURE;
    }

#if (!defined(__HuaweiLite__)) || defined(__OHOS__)
    SAMPLE_VIO_SIGNAL(SAMPLE_VIO_HandleSig);
#endif

    HI_MPI_SYS_GetChipId(&u32ChipId);
    if (u32ChipId == HI3516C_V500) {
        u32VoIntfType = 1;
    } else {
        u32VoIntfType = 0;
    }

    s32Index = atoi(argv[1]);
    switch (s32Index) {
        case 0:
            s32Ret = SAMPLE_VIO_ViOnlineVpssOfflineRoute(u32VoIntfType);
            break;

        case 1: /* 1: value of arg[1] */
            s32Ret = SAMPLE_VIO_WDR_LDC_DIS_SPREAD(u32VoIntfType);
            break;

        case 2: /* 2: value of arg[1] */
            s32Ret = SAMPLE_VIO_ViVpssLowDelay(u32VoIntfType);
            break;

        case 3: /* 3: value of arg[1] */
            s32Ret = SAMPLE_VIO_ViDoubleWdrPipe(u32VoIntfType);
            break;

        case 4: /* 4: value of arg[1] */
            s32Ret = SAMPLE_VIO_FPN(u32VoIntfType);
            break;

        case 5: /* 5: value of arg[1] */
            s32Ret = SAMPLE_VIO_ViWdrSwitch(u32VoIntfType);
            break;

        case 6: /* 6: value of arg[1] */
            s32Ret = SAMPLE_VIO_Rotate(u32VoIntfType);
            break;

        case 7: /* 7: value of arg[1] */
            s32Ret = SAMPLE_VIO_VPSS_VO_MIPI_TX();
            break;

        default:
            SAMPLE_PRT("the index %d is invalid!\n", s32Index);
            SAMPLE_VIO_Usage(argv[0]);
            return HI_FAILURE;
    }

    if ((s32Ret == HI_SUCCESS) && (g_sample_vio_exit == 0)) {
        SAMPLE_PRT("\033[0;32mprogram exit normally!\033[0;39m\n");
    } else {
        SAMPLE_PRT("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }
    sdk_exit();
    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
