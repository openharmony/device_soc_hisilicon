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
#include "sample_svp_npu_process.h"
#include "sdk_module_init.h"

#define SAMPLE_SVP_NPU_ARG_NUM_TWO 2
#define SAMPLE_SVP_NPU_ARG_NUM_THREE 3
#define SAMPLE_SVP_NPU_ARG_IDX_TWO 2
#define SAMPLE_SVP_NPU_CMP_STR_NUM 2
#define SAMPLE_SVP_NPU_ARG_STR_LEN 2

static char **g_npu_cmd_argv = TD_NULL;

/*
 * function : to process abnormal case
 */
#ifndef __LITEOS__
static td_void sample_svp_npu_handle_sig(td_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        sample_svp_npu_acl_handle_sig();
    }
}
#endif

/*
 * function : show usage
 */
static td_void sample_svp_npu_usage(const td_char *prg_name)
{
    printf("Usage : %s <index>\n", prg_name);
    printf("index:\n");
    printf("\t 0) svp_acl_resnet50.\n");
    printf("\t 1) svp_acl_resnet50_multi_thread.\n");
    printf("\t 2) svp_acl_resnet50_dynamic_batch_with_mem_cached.\n");
    printf("\t 3) svp_acl_lstm.\n");
    printf("\t 4) svp_acl_rfcn.(FILE->VDEC->VPSS->SVP_NPU->VGS->VO_HDMI).\n");
    printf("\t 5) {5, 7, 8} yolov5 yolov7 and yolov8.(RPN processed by npu)(VI->VPSS->SVP_NPU->VGS->VO)\n");
    printf("\t 6) {5, 7, 8} yolov5 yolov7 and yolov8.(RPN processed by npu)(FILE->VDEC->VPSS->SVP_NPU->VGS->VO)\n");
    printf("\t 7) {5} yolov5.(RPN processed by cpu)(VI->VPSS->SVP_NPU->VGS->VO)\n");
    printf("\t 8) {5} yolov5.(RPN processed by cpu)(FILE->VDEC->VPSS->SVP_NPU->VGS->VO)\n");
}

static td_s32 sample_svp_npu_case_with_one_arg(td_char idx)
{
    td_s32 ret = TD_SUCCESS;

    switch (idx) {
        case '0':
            sample_svp_npu_acl_resnet50();
            break;
        case '1':
            sample_svp_npu_acl_resnet50_multi_thread();
            break;
        case '2':
            sample_svp_npu_acl_resnet50_dynamic_batch();
            break;
        case '3':
            sample_svp_npu_acl_lstm();
            break;
        case '4':
            sample_svp_npu_acl_rfcn();
            break;
        default:
            ret = TD_FAILURE;
            break;
    }
    return ret;
}

static td_s32 sample_svp_npu_case_with_two_args(td_char idx1, td_char idx2)
{
    td_s32 ret = TD_SUCCESS;

    if (idx1 == '5' || idx1 == '6') {
        if (!(idx2 == '5' || idx2 == '7' || idx2 == '8')) {
            return TD_FAILURE;
        }
    } else {
        if (idx2 != '5') {
            return TD_FAILURE;
        }
    }

    switch (idx1) {
        case '5':
            sample_svp_npu_acl_e2e_yolo_npu(idx2 - '0');
            break;
        case '6':
            sample_svp_npu_acl_yolo_npu(idx2 - '0');
            break;
        case '7':
            sample_svp_npu_acl_e2e_yolo_cpu(idx2 - '0');
            break;
        case '8':
            sample_svp_npu_acl_yolo_cpu(idx2 - '0');
            break;
        default:
            ret = TD_FAILURE;
            break;
    }
    return ret;
}

static td_s32 sample_svp_npu_check_args(int argc, char *argv[])
{
    size_t argv1_len;
    size_t argv2_len;

    if (argc < SAMPLE_SVP_NPU_ARG_NUM_TWO || argc > SAMPLE_SVP_NPU_ARG_NUM_THREE) {
        sample_svp_npu_usage(argv[0]);
        return TD_FAILURE;
    }
    if (!strncmp(argv[1], "-h", SAMPLE_SVP_NPU_CMP_STR_NUM)) {
        sample_svp_npu_usage(argv[0]);
        return TD_SUCCESS;
    }

    argv1_len = strlen(argv[1]);
    if (argv1_len != 1) {
        sample_svp_npu_usage(argv[0]);
        return TD_FAILURE;
    }
    if (argc == SAMPLE_SVP_NPU_ARG_NUM_THREE) {
        argv2_len = strlen(argv[SAMPLE_SVP_NPU_ARG_IDX_TWO]);
        if (argv2_len != 1) {
            sample_svp_npu_usage(argv[0]);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 sample_svp_npu_run_case(int argc, char *argv[])
{
    if (argc == SAMPLE_SVP_NPU_ARG_NUM_TWO) {
        return sample_svp_npu_case_with_one_arg(*argv[1]);
    }
    return sample_svp_npu_case_with_two_args(*argv[1], *argv[SAMPLE_SVP_NPU_ARG_IDX_TWO]);
}

/*
 * function : svp npu sample
 */
#ifdef __LITEOS__
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    td_s32 ret;
#ifndef __LITEOS__
    struct sigaction sa;
#endif
    ret = sample_svp_npu_check_args(argc, argv);
    if (ret != TD_SUCCESS)
        return ret;

    g_npu_cmd_argv = argv;
#ifndef __LITEOS__
    (td_void)memset_s(&sa, sizeof(struct sigaction), 0, sizeof(struct sigaction));
    sa.sa_handler = sample_svp_npu_handle_sig;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
#endif

#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif
    ret = sample_svp_npu_run_case(argc, argv);
#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif

    if (ret != TD_SUCCESS) {
        sample_svp_npu_usage(argv[0]);
        return TD_FAILURE;
    }
    return 0;
}
