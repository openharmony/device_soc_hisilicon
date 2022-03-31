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
#ifndef SAMPLE_UTILS_H
#define SAMPLE_UTILS_H

#include <stdlib.h>
#include <string.h>
#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#ifdef __HuaweiLite__
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#else
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320
#endif

#ifdef __HuaweiLite__
#define RES_PATH  "/home/test/auto_sample/res/"
#else
#define RES_PATH  "./res/"
#endif

#define SBFONT_FILE  RES_PATH"font/ttf/simhei.ttf"
#define MBFONT_FILE  RES_PATH"font/ttf/simhei.ttf"

#define INVALID_HANDLE 0x0

#define HIGV_CHECK(Function, ret)  \
    do { \
        if ((ret) != HI_SUCCESS) { \
            printf("[Func: %s, Line: %d] %s fail ret(x):%x, ret(d): %d\n", \
            __FUNCTION__, __LINE__, Function, ret, ret); \
            } \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
