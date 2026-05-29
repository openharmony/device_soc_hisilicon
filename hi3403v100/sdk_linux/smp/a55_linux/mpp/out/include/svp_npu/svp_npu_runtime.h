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

#ifndef  SVP_NPU_RUNTIME_H
#define  SVP_NPU_RUNTIME_H

#include "svp_npu_common.h"
#include "svp_npu_model_common.h"
#include "svp_npu_prof.h"
#include "mkp_npu.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 svp_npu_runtime_create_ctx(svp_npu_runtime_ctx **ctx, td_s32 device_id);

td_s32 svp_npu_runtime_destroy_ctx(const svp_npu_runtime_ctx *ctx);

td_s32 svp_npu_runtime_set_current_ctx(const svp_npu_runtime_ctx *ctx);

td_s32 svp_npu_runtime_get_current_ctx(svp_npu_runtime_ctx **ctx);

td_s32 svp_npu_runtime_set_device(td_s32 device_id);

td_s32 svp_npu_runtime_reset_device(td_s32 device_id);

td_s32 svp_npu_runtime_get_device(td_s32 *device_id);

td_s32 svp_npu_runtime_get_run_mode(svp_npu_run_mode *run_mode);

td_s32 svp_npu_runtime_synchronize_device(td_void);

td_s32 svp_npu_runtime_get_device_count(td_u32 *count);

td_s32 svp_npu_runtime_malloc(td_void **dev_ptr, td_ulong size, svp_npu_runtime_malloc_policy policy);

td_s32 svp_npu_runtime_malloc_inner(td_phys_addr_t *phys_addr, td_void **dev_ptr, td_ulong size, td_bool is_cached);

td_s32 svp_npu_runtime_malloc_cached(td_void **dev_ptr, td_ulong size, svp_npu_runtime_malloc_policy policy);

td_s32 svp_npu_runtime_mem_flush(const td_void *dev_ptr, td_ulong size);

td_s32 svp_npu_runtime_mem_invalid(const td_void *dev_ptr, td_ulong size);

td_s32 svp_npu_runtime_free(const td_void *dev_ptr);

td_s32 svp_npu_runtime_malloc_host(td_void **host_ptr, td_ulong size);

td_s32 svp_npu_runtime_free_host(const td_void *host_ptr);

td_s32 svp_npu_runtime_create_stream(svp_npu_runtime_stream **stream);

td_s32 svp_npu_runtime_destroy_stream(const svp_npu_runtime_stream *stream);

td_s32 svp_npu_runtime_sync_stream(const svp_npu_runtime_stream *stream);

td_s32 svp_npu_runtime_subscribe_report(td_u64 thread_id, const svp_npu_runtime_stream *stream);

td_s32 svp_npu_runtime_launch_callback(svp_npu_callback *fn, td_void *user_data,
    svp_npu_callback_block_type block_type, svp_npu_runtime_stream *stream);

td_s32 svp_npu_runtime_process_report(td_s32 timeout);

td_s32 svp_npu_runtime_unsubscribe_report(td_u64 thread_id, const svp_npu_runtime_stream *stream);

td_s32 svp_npu_runtime_get_model_id(td_u32 *model_id, const svp_npu_model_info *model_info);

td_s32 svp_npu_runtime_free_model_id(td_u32 model_id, const svp_npu_model_info *model_info);

td_s32 svp_npu_runtime_update_model_prof(const svp_npu_model *model, const svp_npu_prof_model_update_info *info);

td_s32 svp_npu_runtime_execute_model(const svp_npu_model *model, const svp_npu_mem_info *task_info_mem);

td_s32 svp_npu_runtime_execute_model_async(const svp_npu_model *model, const svp_npu_mem_info *task_info_mem,
    const svp_npu_runtime_stream *stream);

td_s32 svp_npu_runtime_get_model_resource(svp_npu_get_model_resource_arg *arg);

td_s32 svp_npu_runtime_get_sub_stream_id(td_s32 *sub_stream_id);

td_s32 svp_npu_runtime_dump_ctrl(const svp_npu_dump_ctrl_arg *ctrl_arg);

td_s32 svp_npu_runtime_process_aicpu_task(td_s32 timeout);

td_s32 svp_npu_runtime_init_resource(td_void);

td_s32 svp_npu_runtime_deinit_resource(td_void);

td_s32 svp_npu_runtime_set_op_wait_timeout(td_u32 timeout);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_RUNTIME_H */
