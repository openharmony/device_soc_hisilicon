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

#ifndef  SVP_NPU_MODEL_H
#define  SVP_NPU_MODEL_H

#include "svp_npu_common.h"
#include "svp_npu_model_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct {
    td_u32 layer_start;
    td_u32 layer_end;
    td_u32 cur_batch;
    td_u32 cur_cycle;

    td_u32 last_layer_idx;
    td_u32 last_prof_layer_id;
    td_u32 last_prof_idx;
    td_u32 last_is_batch;

    td_u32 layer_index; /* net layer offset */
    td_u32 net_layer_num;

    td_u32 batch_num;
    td_u32 cycle;
    td_u32 in_num;
} prof_subscribe_calc_info;

svp_npu_model_desc *svp_npu_model_create_desc();

td_s32 svp_npu_model_destroy_desc(svp_npu_model_desc *model_desc);

td_s32 svp_npu_model_get_desc(svp_npu_model_desc *model_desc, td_u32 model_id);

td_u32 svp_npu_model_get_inout_num(const svp_npu_model_desc *model_desc, td_bool is_input);

td_ulong svp_npu_model_get_inout_size_by_idx(const svp_npu_model_desc *model_desc, td_ulong index, td_bool is_input);

svp_npu_model_dataset *svp_npu_model_create_dataset();

td_s32 svp_npu_model_destroy_dataset(const svp_npu_model_dataset *dataset);

td_s32 svp_npu_model_add_dataset_buffer(svp_npu_model_dataset *dataset, const svp_npu_data_buffer *data_buffer);

td_u32 svp_npu_model_get_dataset_buffer_num(const svp_npu_model_dataset *dataset);

svp_npu_data_buffer *svp_npu_model_get_dataset_buffer(const svp_npu_model_dataset *dataset, td_ulong index);

td_s32 svp_npu_model_load_from_mem(const td_u8 *model, td_ulong model_size, td_u32 priority, td_u32 *model_id);

td_s32 svp_npu_model_execute(td_u32 model_id, const svp_npu_model_dataset *input, const svp_npu_model_dataset *output);

td_s32 svp_npu_model_execute_async(td_u32 model_id, const svp_npu_model_dataset *input,
    const svp_npu_model_dataset *output, const svp_npu_runtime_stream *stream);

td_s32 svp_npu_model_unload(td_u32 model_id);

td_s32 svp_npu_model_set_dynamic_batch_size(td_u32 model_id, svp_npu_model_dataset *dataset, td_ulong index,
    td_u64 batch_size);

td_s32 svp_npu_model_set_total_t(td_u32 model_id, svp_npu_model_dataset *dataset, td_u64 total_t);

td_s32 svp_npu_model_get_inout_dims(const svp_npu_model_desc *model_desc, td_ulong index,
    svp_npu_model_io_dims *dims, td_bool is_input);

const td_char *svp_npu_model_get_inout_name_by_idx(const svp_npu_model_desc *model_desc,
    td_ulong index, td_bool is_input);

td_s32 svp_npu_model_get_inout_format(const svp_npu_model_desc *model_desc, td_ulong index, td_bool is_input);

td_s32 svp_npu_model_get_inout_data_type(const svp_npu_model_desc *model_desc, td_ulong index, td_bool is_input);

td_s32 svp_npu_model_get_inout_idx_by_name(const svp_npu_model_desc *model_desc, const td_char *name,
    td_ulong *index, td_bool is_input);

td_u32 svp_npu_model_get_inout_default_stride(const svp_npu_model_desc *model_desc, td_ulong index, td_bool is_input);

td_u32 svp_npu_model_get_output_default_stride(const svp_npu_model_desc *model_desc, td_ulong index);

td_s32 svp_npu_model_init_dump(td_void);

td_s32 svp_npu_model_set_dump(const td_char *dump_cfg_path);

td_s32 svp_npu_model_finalize_dump(td_void);

td_void svp_npu_model_dump_proc(td_u32 device_id, td_bool *is_trap);

td_void svp_npu_model_aicpu_dump_proc(const td_void *arg);

td_s32 svp_npu_model_load_with_config(const svp_npu_model_config_handle *handle, td_u32 *model_id);

svp_npu_model_config_handle *svp_npu_model_create_config_handle();

td_s32 svp_npu_model_destory_config_handle(svp_npu_model_config_handle *handle);

td_s32 svp_npu_model_set_config_opt(svp_npu_model_config_handle *handle, svp_npu_model_config_attr attr,
    const td_void *attr_value, td_ulong value_size);

td_s32 svp_npu_model_init_resource(td_void);

td_s32 svp_npu_model_deinit_resource(td_void);

td_s32 svp_npu_model_get_model_prof_info(td_u32 model_id, svp_npu_prof_data_type channel,
    td_void **data, td_u32 *max_len, td_char **name);

td_void svp_npu_model_get_net_start_layer_idx(td_u32 model_id, td_u32 net_idx,
    prof_subscribe_calc_info *sub_info);
td_s32 svp_npu_model_update_prof_op_info(td_u32 model_id, prof_subscribe_calc_info *sub_info,
    svp_npu_prof_op_data *op_data);

td_void svp_npu_model_get_model_prof_acl_api_info(FILE *file_fd, td_u32 model_id, td_bool is_head);

td_s32 svp_npu_model_pre_init_sub_prof_info(td_u32 model_id);
td_void svp_npu_model_pre_exit_sub_prof_info(td_u32 model_id);

td_u64 svp_npu_model_prof_acl_api_begin(td_void);
td_void svp_npu_model_prof_acl_api_end(td_u32 model_id, svp_npu_prof_time_item_type type, td_u64 start);
td_void svp_npu_model_prof_acl_api_end_with_lock(td_u32 model_id, svp_npu_prof_time_item_type type,
    td_u64 start);

td_s32 svp_npu_model_get_aicpu_task_num(td_u32 model_id, td_u32 *num);

td_s32 svp_npu_model_check_aicpu_func_and_param_info(const svp_npu_task *task);

td_s32 svp_npu_model_get_first_aipp_info(td_u32 model_id, td_ulong index, svp_acl_aipp_info *aipp_info);

td_s32 svp_npu_model_get_dynamic_batch(const svp_npu_model_desc *model_desc, svp_acl_mdl_batch *batch);

td_s32 svp_npu_model_get_net_type(td_u32 model_id, td_u32 *net_type);

td_s32 svp_npu_model_get_dynamic_hw(const svp_npu_model_desc *model_desc, td_ulong index, svp_acl_mdl_hw *hw);

td_s32 svp_npu_model_set_dynamic_hw_size(td_u32 model_id, svp_npu_model_dataset *dataset, td_ulong index,
    td_u64 height, td_u64 width);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_MODEL_H */
