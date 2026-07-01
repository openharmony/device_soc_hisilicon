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

#include <pthread.h>
#include <unistd.h>
#include "log.h"
#include "ot_uvc.h"
#include "uvc.h"
#include "camera.h"

static pthread_t g_stuvc_send_data_pid;

static int ot_uvc_init_impl()
{
    return 0;
}

static int ot_uvc_open_impl()
{
    const char *devpath = "/dev/video0";

    return open_uvc_device(devpath);
}

static int ot_uvc_close_impl()
{
    return close_uvc_device();
}

void *uvc_send_data_thread(void *p)
{
    int status = 0;
    const int running = 1;

    while (running) {
        if (sample_uvc_get_quit_flag() != 0) {
            return NULL;
        }
        status = run_uvc_data();
    }

    rlog("uvc_send_data_thread exit, status: %d.\n", status);

    return NULL;
}

static int ot_uvc_run_impl()
{
    int status;
    const int running = 1;

    pthread_create(&g_stuvc_send_data_pid, 0, uvc_send_data_thread, NULL);

    while (running) {
        if (sample_uvc_get_quit_flag() != 0) {
            break;
        }

        status = run_uvc_device();
        if (status < 0) {
            break;
        }
    }

    pthread_join(g_stuvc_send_data_pid, NULL);

    return 0;
}

/* ---------------------------------------------------------------------- */

static ot_uvc g_ot_uvc = {
    .init = ot_uvc_init_impl,
    .open = ot_uvc_open_impl,
    .close = ot_uvc_close_impl,
    .run = ot_uvc_run_impl,
};

ot_uvc *get_ot_uvc()
{
    return &g_ot_uvc;
}

void release_ot_uvc(ot_uvc *uvc) {}
