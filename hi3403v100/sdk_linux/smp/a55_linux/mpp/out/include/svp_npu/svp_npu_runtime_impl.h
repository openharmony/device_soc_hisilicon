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

#ifndef  SVP_NPU_RUNTIME_IMPL_H
#define  SVP_NPU_RUNTIME_IMPL_H

#include "svp_npu_runtime_common.h"
#include "svp_npu_model_common.h"
#include "svp_npu_prof.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 svp_npu_runtime_impl_check_input_ctx(const svp_npu_runtime_ctx *ctx);

td_s32 svp_npu_runtime_impl_check_input_stream(const svp_npu_runtime_stream *stream);

td_s32 svp_npu_runtime_impl_destroy_func_enter(td_void);

td_s32 svp_npu_runtime_impl_func_enter(td_void);

td_void svp_npu_runtime_impl_func_exit(td_void);

td_s32 svp_npu_runtime_impl_subscribe_report(td_u64 thread_id, const svp_npu_runtime_stream *stream);

td_s32 svp_npu_runtime_impl_launch_callback(svp_npu_callback *fn, td_void *user_data,
    svp_npu_callback_block_type block_type, svp_npu_runtime_stream *stream);

td_s32 svp_npu_runtime_impl_process_report(td_s32 timeout);

td_s32 svp_npu_runtime_impl_unsubscribe_report(td_u64 thread_id, const svp_npu_runtime_stream *stream);

td_s32 svp_npu_runtime_impl_create_ctx(svp_npu_runtime_ctx **ctx, td_s32 device_id);

td_s32 svp_npu_runtime_impl_destroy_ctx(const svp_npu_runtime_ctx *ctx);

td_s32 svp_npu_runtime_impl_set_current_ctx(const svp_npu_runtime_ctx *ctx);

td_s32 svp_npu_runtime_impl_get_current_ctx(svp_npu_runtime_ctx **ctx);

td_s32 svp_npu_runtime_impl_create_stream(svp_npu_runtime_stream **stream);

td_s32 svp_npu_runtime_impl_destroy_stream(const svp_npu_runtime_stream *stream);

td_s32 svp_npu_runtime_impl_sync_stream(const svp_npu_runtime_stream *stream);

td_s32 svp_npu_runtime_impl_set_device(td_s32 device_id);

td_s32 svp_npu_runtime_impl_reset_device(td_s32 device_id);

td_s32 svp_npu_runtime_impl_get_device(td_s32 *device_id);

td_s32 svp_npu_runtime_impl_get_device_count(td_u32 *count);

td_s32 svp_npu_runtime_impl_get_run_mode(svp_npu_run_mode *run_mode);

td_s32 svp_npu_runtime_impl_sync_device(td_void);

td_s32 svp_npu_runtime_impl_get_model_id(td_u32 *model_id, const svp_npu_model_info *model_info);

td_s32 svp_npu_runtime_impl_free_model_id(td_u32 model_id, const svp_npu_model_info *model_info);

td_s32 svp_npu_runtime_impl_update_model_prof(const svp_npu_model *model, const svp_npu_prof_model_update_info *info);

td_s32 svp_npu_runtime_impl_execute_model(const svp_npu_model *model, const svp_npu_mem_info *task_info_mem);

td_s32 svp_npu_runtime_impl_execute_model_async(const svp_npu_model *model,
    const svp_npu_mem_info *task_info_mem, const svp_npu_runtime_stream *stream);

td_s32 svp_npu_runtime_impl_get_model_resource(svp_npu_get_model_resource_arg *arg);

td_s32 svp_npu_runtime_impl_sub_stream_id(td_s32 *sub_stream_id);

td_s32 svp_npu_runtime_impl_dump_ctrl(const svp_npu_dump_ctrl_arg *ctrl_arg);

td_s32 svp_npu_runtime_impl_process_aicpu_task(td_s32 timeout);

td_s32 svp_npu_runtime_impl_get_aicpu_task_async(td_s32 timeout, svp_npu_process_aicpu_task_arg *arg,
    svp_npu_runtime_check_info_node *check_info_node);

td_s32 svp_npu_runtime_impl_get_aicpu_task_sync(td_s32 timeout, td_u16 task_id,
    svp_npu_process_aicpu_task_arg *arg, svp_npu_runtime_check_info_node *check_info_node);

td_s32 svp_npu_runtime_impl_process_aicpu_task_sync(td_s32 timeout, td_u16 task_id,
    svp_npu_task_next_execute_type *next_task_type);

td_s32 svp_npu_runtime_impl_handle_aicpu_task_sync(svp_npu_process_aicpu_task_arg *arg,
    svp_npu_runtime_check_info_node *check_info_node);

td_s32 svp_npu_runtime_impl_finish_aicpu_task_sync(svp_npu_process_aicpu_task_arg *arg);

td_s32 svp_npu_runtime_impl_init_resource(td_void);

td_s32 svp_npu_runtime_impl_deinit_resource(td_void);

td_s32 svp_npu_runtime_impl_get_fd(td_void);

td_s32 svp_npu_runtime_impl_set_op_wait_timeout(td_u32 timeout);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_RUNTIME_IMPL_H */
