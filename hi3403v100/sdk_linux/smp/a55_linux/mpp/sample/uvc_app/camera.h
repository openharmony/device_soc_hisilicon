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

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <signal.h>

typedef struct ot_camera {
    int (*init)();
    int (*open)();
    int (*close)();
    int (*run)();
} ot_camera;

ot_camera *get_ot_camera();
void release_ot_camera(ot_camera *camera);

void sample_venc_config(void);
void sample_audio_config(void);

volatile sig_atomic_t sample_uvc_get_quit_flag(void);

unsigned int get_g_uac_val();
#endif // __CAMERA_H__
