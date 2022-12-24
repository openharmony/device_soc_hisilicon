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
 * 该文件为存储示例主函数文件，该示例将sensor获得的视频流通过编码成.h264文件并保存下来
 *
 * This file is the main function file of the storage example,
 * which encodes the video stream obtained by the sensor into a .h264 file and saves it
 */

#include "sample_store.h"
#include "sdk.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/*
 * 函数：main()
 * function: main()
 */
int main(void)
{
    int ret;
    sdk_init();
    ret = SAMPLE_VENC_H265_H264();
    sdk_exit();
    SAMPLE_PRT("\nsdk exit success\n");
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
