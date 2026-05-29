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
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#include <unistd.h>
#include <signal.h>
#include "securec.h"
#include "sample_dpu_proc.h"
#include "sample_common_dpu.h"
#include "sdk_module_init.h"

#define SAMPLE_SVP_DPU_ARG_MAX_NUM 2

static char **g_ch_cmd_argv = TD_NULL;
/* function : to process abnormal case */
#ifndef __LITEOS__
static td_void sample_svp_dpu_handle_sig(td_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        switch (*g_ch_cmd_argv[1]) {
            case '0': {
                sample_svp_dpu_vi_vpss_rect_match_handle_sig();
                break;
                }
            case '1': {
                sample_svp_dpu_file_rect_match_handle_sig();
                break;
                }
            default:
                break;
        }
    }
}
#endif

/* function : show usage */
static td_void sample_svp_dpu_usage(const char *prg_name)
{
    printf("Usage : %s <index> \n", prg_name);
    printf("index:\n");
    printf("\t 0) VI->VPSS->RECT->MATCH.\n");
    printf("\t 1) FILE->RECT->MATCH.\n");
}

static td_s32 sample_svp_dpu_case(td_char idx)
{
    td_s32 ret;
    switch (idx) {
        case '0': {
            ret = sample_svp_dpu_vi_vpss_rect_match();
            sample_svp_dpu_check_exps_return(1, ret, "Not Support!\n");
            break;
            }
        case '1': {
            ret = sample_svp_dpu_file_rect_match();
            break;
            }
        default: {
            ret = TD_FAILURE;
            break;
            }
    }
    return ret;
}
/* function : dpu sample */
#ifdef __LITEOS__
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    td_s32 ret;
    td_s32 idx_len;
#ifndef __LITEOS__
    struct sigaction sa;
#endif
    if (argc != SAMPLE_SVP_DPU_ARG_MAX_NUM) {
        sample_svp_dpu_usage(argv[0]);
        return TD_FAILURE;
    }
    if (!strncmp(argv[1], "-h", SAMPLE_SVP_DPU_ARG_MAX_NUM)) {
        sample_svp_dpu_usage(argv[0]);
        return TD_SUCCESS;
    }
    g_ch_cmd_argv = argv;
#ifndef __LITEOS__
    (td_void)memset_s(&sa, sizeof(struct sigaction), 0, sizeof(struct sigaction));
    sa.sa_handler = sample_svp_dpu_handle_sig;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
#endif
    idx_len = strlen(argv[1]);
    if (idx_len != 1) {
        sample_svp_dpu_usage(argv[0]);
        return TD_FAILURE;
    }
#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif
    ret = sample_svp_dpu_case(*argv[1]);
#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif
    if (ret != TD_SUCCESS) {
        sample_svp_dpu_usage(argv[0]);
    }
    return 0;
}