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
#include "sdk.h"
#include "sample_comm_ive.h"
#include "sample_ive_main.h"

static char **s_ppChCmdArgv = NULL;

#define SAMPLE_IVE_CANNY_ARG_MAX_NUM    3
#define SAMPLE_IVE_ARG_MAX_NUM          3
#define SAMPLE_IVE_ARG_MIN_NUM          2
#define SAMPLE_IVE_IDX_0                0
#define SAMPLE_IVE_IDX_1                1
#define SAMPLE_IVE_IDX_2                2
#define SAMPLE_IVE_HELP_SIZE            2

/* function : to process abnormal case */
#if (!defined(__HuaweiLite__)) || defined(__OHOS__)
static HI_VOID SAMPLE_IVE_HandleSig(HI_S32 s32Signo)
{
    if (s32Signo == SIGINT || s32Signo == SIGTERM) {
        switch (*s_ppChCmdArgv[1]) {
            case '0':
                SAMPLE_IVE_Od_HandleSig();
                break;
            case '1':
                SAMPLE_IVE_Md_HandleSig();
                break;
            case '2':
                SAMPLE_IVE_Canny_HandleSig();
                break;
            case '3':
                SAMPLE_IVE_Gmm2_HandleSig();
                break;
            case '4':
                SAMPLE_IVE_TestMemory_HandleSig();
                break;
            case '5':
                SAMPLE_IVE_Sobel_HandleSig();
                break;
            case '6':
                SAMPLE_IVE_St_Lk_HandleSig();
                break;
            case '7':
                SAMPLE_IVE_Kcf_HandleSig();
                break;
            case '8':
                SAMPLE_IVE_PerspTrans_HandleSig();
                break;
            default:
                break;
        }
    }
}
#endif

/* function : show usage */
static HI_VOID SAMPLE_IVE_Usage(HI_CHAR *pchPrgName)
{
    printf("Usage : %s <index> [complete] \n", pchPrgName);
    printf("index:\n");
    printf("\t 0)Occlusion detected.(VI->VPSS->IVE->VO_HDMI).\n");
    printf("\t 1)Motion detected.(VI->VPSS->IVE->VGS->VO_HDMI).\n");
    printf("\t 2)Canny,<complete>:0, part canny;1,complete canny.(FILE->IVE->FILE).\n");
    printf("\t 3)Gmm2.(FILE->IVE->FILE).\n");
    printf("\t 4)MemoryTest.(FILE->IVE->FILE).\n");
    printf("\t 5)Sobel.(FILE->IVE->FILE).\n");
    printf("\t 6)St Lk.(FILE->IVE->FILE).\n");
    printf("\t 7)Kcf track.(VI->VPSS->IVE->VO_HDMI).\n");
    printf("\t 8)PerspTrans.(FILE->IVE->FILE).\n");
}

/* function : ive sample */
#if defined(__HuaweiLite__) && (!defined(__OHOS__))
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    int ret = HI_SUCCESS;
    sdk_init();

    if (argc < SAMPLE_IVE_ARG_MIN_NUM || argc > SAMPLE_IVE_ARG_MAX_NUM) { /* 2, 3: arg count */
        SAMPLE_IVE_Usage(argv[0]);
        ret = HI_FAILURE;
        goto end;
    }
    s_ppChCmdArgv = argv;
#if (!defined(__HuaweiLite__)) || defined(__OHOS__)
    struct sigaction sa;
    (hi_void)memset_s(&sa, sizeof(struct sigaction), 0, sizeof(struct sigaction));
    sa.sa_handler = SAMPLE_IVE_HandleSig;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
#endif

    if (strncmp(argv[1], "-h", SAMPLE_IVE_HELP_SIZE) == 0) { /* 2: maximum number of characters to compare */
        SAMPLE_IVE_Usage(argv[0]);
        ret = HI_FAILURE;
        goto end;
    }
    switch (*argv[1]) {
        case '0':
            SAMPLE_IVE_Od();
            break;
        case '1':
            SAMPLE_IVE_Md();
            break;
        case '2':
            if ((argc < SAMPLE_IVE_CANNY_ARG_MAX_NUM) ||
                ((*argv[SAMPLE_IVE_IDX_2] != '0') && (*argv[SAMPLE_IVE_IDX_2] != '1'))) {
                SAMPLE_IVE_Usage(argv[0]);
                ret = HI_FAILURE;
                goto end;
            }
            SAMPLE_IVE_Canny(*argv[SAMPLE_IVE_IDX_2]); /* 2: arg index */
            break;
        case '3':
            SAMPLE_IVE_Gmm2();
            break;
        case '4':
            SAMPLE_IVE_TestMemory();
            break;
        case '5':
            SAMPLE_IVE_Sobel();
            break;
        case '6':
            SAMPLE_IVE_St_Lk();
            break;
        case '7':
            SAMPLE_IVE_Kcf();
            break;
        case '8':
            SAMPLE_IVE_PerspTrans();
            break;
        default:
            SAMPLE_IVE_Usage(argv[0]);
            break;
    }

end:
    sdk_exit();
    return ret;
}
