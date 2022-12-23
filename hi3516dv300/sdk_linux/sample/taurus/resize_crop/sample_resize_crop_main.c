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
 * 该文件为预处理示例主函数文件，其分别介绍了对一帧图像的裁剪和缩放，
 * 适用于AI推理之前对图片进行预处理。./可执行文件+index方式可以运行预处理API，
 * index 0表示缩放，index 1表示裁剪。最后转换成功的结果保存在文件中。
 *
 * This file is the main function file of the preprocessing example,
 * which respectively introduces the crop and resize of a frame of image,
 * and is suitable for preprocessing the image before AI inference.
 * ./executable file + index method can run the preprocessing API,
 * index 0 means resize, and index 1 means crop.
 * The result of successful conversion is finally saved in the file.
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

/*
 * 函数：显示用法
 * function: show usage
 */
void SampleSvpUsage(const char* pchPrgName)
{
    printf("Usage : %s <index> \n", pchPrgName);
    printf("index:\n");
    printf("\t 0) resize.\n");
    printf("\t 1) crop.\n");
}

/*
 * 函数：缩放和裁剪示例
 * function : resize and crop sample 
 */
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
