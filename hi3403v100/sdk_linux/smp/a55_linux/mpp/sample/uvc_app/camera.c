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

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <pthread.h>
#include "ot_uvc.h"
#include "ot_uac.h"
#include "ot_stream.h"
#include "ot_audio.h"
#include "camera.h"
#include "ot_type.h"
/* -------------------------------------------------------------------------- */

unsigned int g_uac = 1;
static pthread_t g_st_uvc_pid;
static pthread_t g_st_uac_pid;

static int camera_init_impl()
{
    sample_venc_config();

    if (get_ot_uvc()->init() != 0 || ot_stream_init() != 0) {
        return -1;
    }

    if (g_uac) {
#ifdef OT_UAC_COMPILE
        sample_audio_config();
#endif

        if (get_ot_uac()->init() != 0 || ot_audio_init() != 0) {
            return -1;
        }
    }

    return 0;
}

static int camera_open_impl()
{
    if (get_ot_uvc()->open() != 0) {
        return -1;
    }

    if (g_uac) {
        if (get_ot_uac()->open() != 0) {
            return -1;
        }
    }

    return 0;
}

static int camera_close_impl()
{
    get_ot_uvc()->close();
    ot_stream_shutdown();
    if (g_uac) {
        get_ot_uac()->close();
        ot_audio_shutdown();
    }
    ot_stream_deinit();
    return 0;
}

td_void *uvc_thread(td_void *p)
{
    get_ot_uvc()->run();
    return NULL;
}

td_void *uac_thread(td_void *p)
{
    get_ot_uac()->run();
    return NULL;
}

static int camera_run_impl()
{
    pthread_create(&g_st_uvc_pid, 0, uvc_thread, NULL);
    pthread_create(&g_st_uac_pid, 0, uac_thread, NULL);
    pthread_join(g_st_uvc_pid, NULL);
    pthread_join(g_st_uac_pid, NULL);
    return 0;
}

/* -------------------------------------------------------------------------- */

static ot_camera g_ot_camera = {
    .init = camera_init_impl,
    .open = camera_open_impl,
    .close = camera_close_impl,
    .run = camera_run_impl,
};

ot_camera *get_ot_camera()
{
    return &g_ot_camera;
}

void release_ot_camera(ot_camera *camera) {}

unsigned int get_g_uac_val()
{
    return g_uac;
}
