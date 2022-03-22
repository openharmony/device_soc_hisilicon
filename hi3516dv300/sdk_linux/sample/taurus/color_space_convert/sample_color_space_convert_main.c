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

#include "hi_debug.h"
#include "smp_color_space_convert.h"
#include "sdk.h"

/* function : show usage */
void SAMPLE_SVP_Usage(const char* pchPrgName)
{
    printf("Usage : %s <index> \n", pchPrgName);
    printf("index:\n");
    printf("\t 0) ive image to video frame.\n");
    printf("\t 1) video frame to ive image.\n");
    printf("\t 2) video YUV frame to ive image (U8C1).\n");
    printf("\t 3) YUV video frame to RGB ive image - ive image RGB to YUV.\n");
    printf("\t 4) YUV video frame to RGB ive image - ive image RGB to BGR.\n");
}

/* function : color space convert sample */
int main(int argc, char *argv[])
{
    int s32Ret = HI_SUCCESS;

    if (argc < 2 || argc > 2) { // 2: argc param
        SAMPLE_SVP_Usage(argv[0]);
        return HI_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { // 2: Compare the first 2 bytes
        SAMPLE_SVP_Usage(argv[0]);
        return HI_SUCCESS;
    }

    sdk_init();

    switch (*argv[1]) {
        case '0':
            SampleIveOrigImgToFrm();
            break;
        case '1':
            SampleIveFrmToOrigImg();
            break;
        case '2':
            SampleIveFrmToU8c1Img();
            break;
        case '3':
            SampleIveFrmToRgbImgToYuv();
            break;
        case '4':
            SampleIveFrmToRgbImgToBgr();
            break;
        default:
            SAMPLE_SVP_Usage(argv[0]);
            break;
    }

    sdk_exit();
    printf("\nsdk exit success\n");

    return s32Ret;
}
