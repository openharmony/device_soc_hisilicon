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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "camera.h"
#include "frame_cache.h"
#include "sdk_module_init.h"

unsigned int g_bulk = 0;

#ifndef __LITEOS__
volatile sig_atomic_t g_need_quit_flag = 0;
volatile sig_atomic_t sample_uvc_get_quit_flag(void)
{
    return g_need_quit_flag;
}
#endif

void sample_uvc_usage(char *s_prg_nm)
{
    printf("Usage : %s <param>\n", s_prg_nm);
    printf("param:\n");
    printf("\t -h        --for help.\n");
    printf("\t -bulkmode --use bulkmode.\n");
    return;
}

#ifndef __LITEOS__
void sample_uvc_handle_signal(int signo)
{
    (void)signal(SIGINT, SIG_IGN);
    (void)signal(SIGTERM, SIG_IGN);

    if (signo == SIGINT || signo == SIGTERM) {
        g_need_quit_flag = 1;
    }
}
#endif

int main(int argc, char *argv[])
{
    int i = argc;

    g_bulk = 0;
    SDK_init();
    while (i > 1) {
        if (strcmp(argv[i - 1], "-bulkmode") == 0) {
            g_bulk = 1;
        }

        if (strcmp(argv[i - 1], "-h") == 0) {
            sample_uvc_usage(argv[0]);
        }

        i--;
    }

#ifndef __LITEOS__
    (void)signal(SIGINT, sample_uvc_handle_signal);
    (void)signal(SIGTERM, sample_uvc_handle_signal);
#endif

    if (create_uvc_cache() != 0) {
        goto err;
    }

    if (get_g_uac_val() == 1) {
        if (create_uac_cache() != 0) {
            goto err;
        }
    }

    if (get_ot_camera()->init() != 0 || get_ot_camera()->open() != 0 || get_ot_camera()->run() != 0) {
        goto err;
    }
    SDK_exit();
    printf("UVC sample exit!\n");

err:
    get_ot_camera()->close();
    destroy_uvc_cache();
    if (get_g_uac_val() == 1) {
        destroy_uac_cache();
    }

    printf("exit app normally\n");
    return 0;
}
