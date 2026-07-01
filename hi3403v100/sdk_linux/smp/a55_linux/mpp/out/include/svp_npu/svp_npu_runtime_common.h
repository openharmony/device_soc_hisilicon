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

#ifndef SVP_NPU_RUNTIME_COMMON_H
#define SVP_NPU_RUNTIME_COMMON_H

#include <pthread.h>
#include "svp_npu_common.h"
#include "list.h"
#include "mkp_npu.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    struct list_head head;
    struct list_head busy_stream_list;
    svp_npu_runtime_ctx *runtime_ctx;
    svp_npu_status status;
    td_u16 default_stream_node_id;
    td_u16 busy_stream_num;
    td_u16 device_id;
    td_u16 node_id;
} svp_npu_runtime_ctx_node;

typedef struct {
    svp_npu_runtime_ctx_node *ctx_node;
    struct list_head busy_ctx_list;
    struct list_head free_ctx_list;
    td_u16 free_ctx_num;
    td_u16 busy_ctx_num;
} svp_npu_runtime_ctx_list;

typedef struct {
    struct list_head head;
    svp_npu_runtime_stream *runtime_stream;
    svp_npu_status status;
    td_bool is_default;
    td_bool has_subscribed;
    td_u64 thread_id;
    td_u16 ctx_node_id;
    td_u16 stream_id; /* stream_id */
    td_u16 node_id;
    td_u8 model_id[SVP_NPU_MAX_MODEL_NUM];
    td_u8 model_num;
} svp_npu_runtime_stream_node;

typedef struct {
    svp_npu_runtime_stream_node *stream_node;
    struct list_head free_stream_list;
    td_u16 free_stream_num;
} svp_npu_runtime_stream_list;

typedef struct {
    struct list_head head;
    td_u64 thread_id;
    svp_npu_status status;
    td_u16 report_id;
    td_u16 stream_num;
} svp_npu_runtime_report_node;

typedef struct {
    svp_npu_runtime_report_node *report_node;
    struct list_head busy_report_list;
    struct list_head free_report_list;
    td_u16 free_report_num;
    td_u16 busy_report_num;
} svp_npu_runtime_report_list;

typedef struct {
    struct list_head head;
    td_bool *is_used;
    td_u8 *check_info;
    td_u16 check_info_id;
} svp_npu_runtime_check_info_node;

typedef struct {
    svp_npu_runtime_check_info_node *check_info_node;
    struct list_head free_check_info_list;
    td_u16 free_check_info_num;
} svp_npu_runtime_check_info_list;

typedef struct {
    pid_t pid;
    td_u64 ttbr;
    td_u64 tcr;
    td_s32 pasid;
    td_u32 flags;
} svp_npu_runtime_svm_bind;

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_RUNTIME_COMMON_H */
