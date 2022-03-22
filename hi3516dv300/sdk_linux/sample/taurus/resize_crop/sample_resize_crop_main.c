/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
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
#include "hi_debug.h"
#include "sdk.h"
#include "smp_resize_crop.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* function : show usage */
void SampleSvpUsage(const char* pchPrgName)
{
    printf("Usage : %s <index> \n", pchPrgName);
    printf("index:\n");
    printf("\t 0) resize.\n");
    printf("\t 1) crop.\n");
}

/* function : resize and crop sample */
int main(int argc, char *argv[])
{
    int s32Ret = HI_SUCCESS;

    if (argc < 2 || argc > 2) { // 2: argc indicates the number of parameters
        SampleSvpUsage(argv[0]);
        return HI_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { // 2: used to compare the first 2 characters
        SampleSvpUsage(argv[0]);
        return HI_SUCCESS;
    }

    sdk_init();

    switch (*argv[1]) {
        case '0':
            SampleIveImgResize();
            break;
        case '1':
            SampleIveImgCrop();
            break;
        default:
            SampleSvpUsage(argv[0]);
            break;
    }

    sdk_exit();
    printf("\nsdk exit success\n");

    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
