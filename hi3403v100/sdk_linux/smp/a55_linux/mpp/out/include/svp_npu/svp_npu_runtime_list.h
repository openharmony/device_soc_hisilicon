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

#ifndef  SVP_NPU_RUNTIME_LIST_H
#define  SVP_NPU_RUNTIME_LIST_H

#include "svp_npu_runtime_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/* stream */
td_void svp_npu_runtime_init_stream_list(svp_npu_runtime_stream_list *list, td_u32 max_arr_len,
    td_u8 *mem_addr, td_u32 dev_id);

svp_npu_runtime_stream_node *svp_npu_runtime_get_stream_node_from_free_list(svp_npu_runtime_stream_list *list);

svp_npu_runtime_stream_node *svp_npu_runtime_delete_stream_node_from_busy_list(svp_npu_runtime_ctx_node *ctx_node,
    td_u32 node_id);

td_void svp_npu_runtime_put_stream_node_to_free_list(svp_npu_runtime_stream_list *list,
    svp_npu_runtime_stream_node *node);

td_void svp_npu_runtime_put_stream_node_to_busy_list(svp_npu_runtime_ctx_node *ctx_node,
    svp_npu_runtime_stream_node *node);

/* ctx */
td_void svp_npu_runtime_init_ctx_list(svp_npu_runtime_ctx_list *list, td_u32 max_arr_len,
    td_u8 *mem_addr, td_u32 dev_id);

svp_npu_runtime_ctx_node *svp_npu_runtime_get_ctx_node_from_free_list(svp_npu_runtime_ctx_list *list);

svp_npu_runtime_ctx_node *svp_npu_runtime_delete_ctx_node_from_busy_list(svp_npu_runtime_ctx_list *list,
    td_u32 node_id);

td_void svp_npu_runtime_put_ctx_node_to_free_list(svp_npu_runtime_ctx_list *list, svp_npu_runtime_ctx_node *node);

td_void svp_npu_runtime_put_ctx_node_to_busy_list(svp_npu_runtime_ctx_list *list, svp_npu_runtime_ctx_node *node);

/* report */
td_void svp_npu_runtime_init_report_list(svp_npu_runtime_report_list *list, td_u32 max_arr_len,
    td_u8 *mem_addr, td_u32 dev_id);

svp_npu_runtime_report_node *svp_npu_runtime_get_report_node_from_free_list(svp_npu_runtime_report_list *list);

svp_npu_runtime_report_node *svp_npu_runtime_delete_report_node_from_busy_list(svp_npu_runtime_report_list *list,
    td_u64 thread_id);

td_void svp_npu_runtime_put_report_node_to_free_list(svp_npu_runtime_report_list *list,
    svp_npu_runtime_report_node *node);

td_void svp_npu_runtime_put_report_node_to_busy_list(svp_npu_runtime_report_list *list,
    svp_npu_runtime_report_node *node);

svp_npu_runtime_report_node *svp_npu_runtime_find_report_node_from_busy_list(svp_npu_runtime_report_list *list,
    td_u64 thread_id);

/* check info */
td_void svp_npu_runtime_init_check_info_list(svp_npu_runtime_check_info_list *list, td_u32 max_arr_len, td_u8 *mem_addr,
    td_u8 *info, td_u32 check_info_size);

svp_npu_runtime_check_info_node *svp_npu_runtime_get_check_info_node_from_free_list(
    svp_npu_runtime_check_info_list *list);

td_void svp_npu_runtime_put_check_info_node_to_free_list(svp_npu_runtime_check_info_list *list,
    svp_npu_runtime_check_info_node *node);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_RUNTIME_LIST_H */
