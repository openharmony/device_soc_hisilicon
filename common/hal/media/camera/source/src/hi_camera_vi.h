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

#ifndef OHOS_CAMERA_VI_H
#define OHOS_CAMERA_VI_H

#include "hi_camera_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define PATH_MAX_LEN 128
int32_t HpCameraSetFps(td_float pf32Framerate);
int32_t CameraStartVi(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt);
void HpCameraAllIspStop(void);
int32_t CameraStopVi(const CAMERA_VI_CONFIG_S *pstViConfig, uint32_t cameraCnt);
int32_t HpCameraGetFps(void);
uint8_t GetParamCamera(const char path[PATH_MAX_LEN], InitParam *initParam, uint8_t cameraId);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef OHOS_CAMERA_VI_H */
