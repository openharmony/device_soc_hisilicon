/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdbool.h>
#include "hal_camera.h"

#define DEFAULT_WIDTH 480
#define DEFAULT_HEIGHT 480
#define DEFAULT_SET_FPS 60
#define DELAY_5S (5000 * 1000)
#define SAMPLE_CAMERA_WAIT_COUNT 0xFFFFFFFFU

int main(void)
{
    /* init para */
    HalCameraInit();
    uint8_t camUse = 0x1;
    uint32_t cameraList[0x2] = { 0 };
    uint8_t camNum = 0;
    uint32_t streamId = 0;
    HalCameraGetDeviceNum(&camNum);
    printf("num = %d\n", camNum);
    HalCameraGetDeviceList(cameraList, 0x2);

    for (uint8_t i = 0; i < camNum; i++) {
        camUse = i;
        StreamAttr streamAttr;
        streamAttr.crop.x = 0;
        streamAttr.crop.y = 0;
        streamAttr.crop.h = 0;
        streamAttr.crop.w = 0;
        streamAttr.fps = DEFAULT_SET_FPS;
        streamAttr.width = DEFAULT_WIDTH;
        streamAttr.height = DEFAULT_HEIGHT;
        streamAttr.type = STREAM_PREVIEW;
        streamAttr.format = FORMAT_YVU420;
        streamAttr.invertMode = 0;

        HalCameraDeviceOpen(cameraList[camUse]);
        HalCameraStreamCreate(cameraList[camUse], &streamAttr, &streamId);
        uint32_t deviceId = 0;
        HalCameraGetDeviceId(cameraList[camUse], streamId, &deviceId);
        StreamInfo info;
        info.type = STREAM_INFO_POS;
        info.u.pos.x = 0;
        info.u.pos.y = 0;
        HalCameraStreamSetInfo(cameraList[camUse], streamId, &info);
        printf(" device id = 0x%x\n", deviceId);
        HalCameraStreamOn(cameraList[camUse], streamId);
        HalBuffer buffer = { 0 };
        usleep(DELAY_5S);
    }
    for (uint32_t wait = 0; wait < SAMPLE_CAMERA_WAIT_COUNT; wait++) {
        usleep(DELAY_5S);
    }
    return 0;
}
