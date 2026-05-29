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
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "securec.h"
#include "sample_mau_dist.h"
#include "sample_mau_matrix_mul.h"
#include "sdk_module_init.h"

#define SAMPLE_SVP_MAU_ARG_MAX_NUM  3
#define SAMPLE_SVP_MAU_IDX_TWO  2
#define SAMPLE_SVP_MAU_CMP_STR_NUM 2

static char **g_ch_cmd_argv = TD_NULL;

/*
 * function : to process abnormal case
 */
#ifndef __LITEOS__
static td_void sample_svp_mau_handle_sig(td_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        switch (*g_ch_cmd_argv[1]) {
            case '0': {
                sample_svp_mau_matrix_mul_handle_sig();
                break;
                }
            case '1': {
                sample_svp_mau_cos_dist_handle_sig();
                break;
                }
            case '2': {
                sample_svp_mau_euclid_dist_handle_sig();
                break;
                }
            default:
                break;
        }
    }
}
#endif
/*
 * function : show usage
 */
static td_void sample_svp_mau_usage(const td_char *prg_name)
{
    printf("Usage : %s <index> [idx_flag] [output_type]\n", prg_name);
    printf("index:\n");
    printf("\t 0)mau_matrix_mul.\n");
    printf("\t 1)mau_cos_dist,<idx_flag>:0,left_idx right_norm;1,left_norm right_idx;2,left_idx right_idx.\n");
    printf("\t 2)mau_euclid_dist,<output_type>:0:,op_result;1,output_top_n;2,output_both.\n");
}

static td_s32 sample_svp_mau_case(int argc, char *argv[])
{
    td_s32 ret = TD_SUCCESS;
    td_s32 len;

    switch (*argv[1]) {
        case '0': {
            if (argc != SAMPLE_SVP_MAU_ARG_MAX_NUM - 1) {
                return TD_FAILURE;
            }
            sample_svp_mau_matrix_mul();
            break;
            }
        case '1': {
            if (argc < SAMPLE_SVP_MAU_ARG_MAX_NUM) {
                return TD_FAILURE;
            } else {
                len = strlen(argv[SAMPLE_SVP_MAU_ARG_MAX_NUM - 1]);
                if ((len != 1) || ((*argv[SAMPLE_SVP_MAU_ARG_MAX_NUM - 1] != '0') &&
                    (*argv[SAMPLE_SVP_MAU_ARG_MAX_NUM - 1] != '1') &&
                    (*argv[SAMPLE_SVP_MAU_ARG_MAX_NUM - 1] != '2'))) {
                    return TD_FAILURE;
                }
            }
            sample_svp_mau_cos_dist(*argv[SAMPLE_SVP_MAU_ARG_MAX_NUM - 1]);
            break;
            }
        case '2': {
            if (argc < SAMPLE_SVP_MAU_ARG_MAX_NUM) {
                return TD_FAILURE;
            } else {
                len = strlen(argv[SAMPLE_SVP_MAU_ARG_MAX_NUM - 1]);
                if ((len != 1) || ((*argv[SAMPLE_SVP_MAU_ARG_MAX_NUM - 1] != '0') &&
                    (*argv[SAMPLE_SVP_MAU_ARG_MAX_NUM - 1] != '1') &&
                    (*argv[SAMPLE_SVP_MAU_ARG_MAX_NUM - 1] != '2'))) {
                    return TD_FAILURE;
                }
            }
            sample_svp_mau_euclid_dist(*argv[SAMPLE_SVP_MAU_ARG_MAX_NUM - 1]);
            break;
            }
        default: {
            ret = TD_FAILURE;
            break;
            }
    }
    return ret;
}
/*
 * function : svp mau sample
 */
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

    if (argc < SAMPLE_SVP_MAU_ARG_MAX_NUM - 1 || argc > SAMPLE_SVP_MAU_ARG_MAX_NUM) {
        sample_svp_mau_usage(argv[0]);
        return TD_FAILURE;
    }
    if (!strncmp(argv[1], "-h", SAMPLE_SVP_MAU_CMP_STR_NUM)) {
        sample_svp_mau_usage(argv[0]);
        return TD_SUCCESS;
    }
    g_ch_cmd_argv = argv;
#ifndef __LITEOS__
    (td_void)memset_s(&sa, sizeof(struct sigaction), 0, sizeof(struct sigaction));
    sa.sa_handler = sample_svp_mau_handle_sig;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
#endif
    idx_len = strlen(argv[1]);
    if (idx_len != 1) {
        sample_svp_mau_usage(argv[0]);
        return TD_FAILURE;
    }
    
#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif
    ret = sample_svp_mau_case(argc, argv);
#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif
    if (ret != TD_SUCCESS) {
        sample_svp_mau_usage(argv[0]);
        return TD_FAILURE;
    }
    return 0;
}