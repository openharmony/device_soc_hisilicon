/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
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

#ifndef D_CAMERA_UTILS_H
#define D_CAMERA_UTILS_H

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "distributed_hardware_log.h"

using namespace OHOS::DistributedHardware;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* ******************************************************
    macro define
****************************************************** */
#define HAL_SUCCESS (0)
#define HAL_FAILURE (-1)

#define STREAM_ABILITY_NUM 3
#define MAX_FPS 60
#define MIN_FPS 1
#define FRAME_TIME_OUT 1000
#define SUPPORT_MAX_CAMERA_NUM 2
#define CAEMRA_MAX_STREAM_NUM 2

#define MODE_NAME_MAX_LEN 64
#define MODE_NAME_MAX_NUM 16
#define ENLARGE_VALUE 2
#define DISPLAY_DEVID 0

#define CAMERA_RGB "CAMERA_RGB"

/* default layer width */
#define DEFAULT_WIDTH_FOR_HDMI 1920
#define DEFAULT_WIDTH_FOR_LCD 480
/* default layer height */
#define DEFAULT_HEIGHT_FOR_HDMI 1080
#define DEFAULT_HEIGHT_FOR_LCD 960
#define DEFAULT_DISPLAY_FPS 60
#define MIN_WIDTH 16
#define MIN_HEIGHT 16
#define MAX_FRAME_WIDTH 3840
#define MAX_FRAME_HEIGHT 2160

#define RESOLUTION_2592X1944_W 2592
#define RESOLUTION_2592X1944_H 1944
#define RESOLUTION_1080P_W 1920
#define RESOLUTION_1080P_H 1080
#define RESOLUTION_720P_W 1280
#define RESOLUTION_720P_H 720
#define RESOLUTION_D1_NTSC_W 720
#define RESOLUTION_D1_NTSC_H 480
#define RESOLUTION_360P_W 640
#define RESOLUTION_360P_H 360

#define MOD_ID(deviceid) ((unsigned int)(((deviceid) >> 16) & 0xFF))

#define DEVICE_ID(deviceid) ((unsigned int)(((deviceid) >> 8) & 0xFF))

#define CHAN_ID(deviceid) ((unsigned int)((deviceid)&0xFF))

#define NUMBER_OF_BITS 8

#define HAL_ENTER() DHLOGI("enter")
#define HAL_EXIT() DHLOGI("exit")

#define LOG_CHK_RETURN(val) \
    do {                       \
        if ((val)) {           \
            DHLOGI("return"); \
            return;            \
        };                     \
    } while (0)

#define LOG_CHK_RETURN_ERR(val, ret) \
    do {                                \
        if ((val)) {                    \
            DHLOGE("error = 0x%x", ret); \
            return ret;                 \
        }                               \
    } while (0)

#define HAL_LOG_DOFUNC(func) \
    do { \
        int32_t ret = func; \
        if (ret != 0) { \
            DHLOGE("error = 0x%x", ret); \
        } \
    } while (0)

#define HAL_LOG_DOFUNC_RETURN(func) \
    do { \
        int32_t ret = func; \
        if (ret != 0) { \
            DHLOGE("error = 0x%x", ret); \
            return ret; \
        } \
    } while (0)

#define HAL_CHECK_NULL_PTR(ptr) do { \
        if ((ptr) == NULL) { \
            DHLOGE("NULL pointer"); \
            return HAL_FAILURE;                                                 \
        } \
    } while (0)

#define CHECK_SNPRINTF_PROC(ret) do { \
    if ((ret) < 0) { \
        DHLOGE("snprintf_s failed"); \
    } \
} while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef D_CAMERA_UTILS_H */
