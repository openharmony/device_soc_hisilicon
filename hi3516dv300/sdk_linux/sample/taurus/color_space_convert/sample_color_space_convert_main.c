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

/*
 * 该文件为色彩空间转换示例主函数文件，其提供典型的色彩空间转换API接口，
 * ./可执行文件+index方式可以运行相关色彩空间转换API。总共五种典型的色彩空间转换场景。
 *
 * This file is the main function file of the color space conversion example,
 * which provides a typical color space conversion API interface,
 * and the ./executable file + index method can run the relevant color space conversion API.
 * A total of five typical color space conversion scenarios.
 */

#include "hi_debug.h"
#include "smp_color_space_convert.h"
#include "sdk.h"

/*
 * 函数：显示用法
 * function: show usage
 */
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

/*
 * 函数：色彩空间转换sample
 * function : color space convert sample
 */
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
