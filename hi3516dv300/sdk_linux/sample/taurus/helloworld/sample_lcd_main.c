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
 * 该文件为helloworld主函数文件，打通了从视频输入->视频处理子系统->视频输出->显示屏整个媒体通路，
 * ./可执行文件即可运行helloworld媒体通路。运行成功即可在mipi屏上查看实时视频流。
 *
 * This file is the main function file of helloworld, which opens up the entire media channel from
 * VI->VPSS->VO->MIPI，./Executable file can run the helloworld media channel.
 * After running successfully, you can view the real-time video stream on the mipi screen.
 */

#include "mpi_sys.h"
#include "sample_lcd.h"
#include "sdk.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/*
 * 函数    : main()
 * function: main()
 */
int main(void)
{
    HI_S32 s32Ret;
    sdk_init();
    /*
     * MIPI为GPIO55，开启液晶屏背光
     * MIPI is GPIO55, Turn on the backlight of the LCD screen
     */
    system("cd /sys/class/gpio/;echo 55 > export;echo out > gpio55/direction;echo 1 > gpio55/value");
    s32Ret = SampleVioVpssVoMipi();
    sdk_exit();
    SAMPLE_PRT("\nsdk exit success\n");
    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */