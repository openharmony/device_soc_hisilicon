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

#ifndef  SVP_NPU_MODEL_PARAM_CHECK_H
#define  SVP_NPU_MODEL_PARAM_CHECK_H

#define SVP_NPU_MODEL_MAX_INNER_BATCH_NUM 16

#define SVP_NPU_MODEL_MAX_IMG_BATCH_NUM 256
#define SVP_NPU_MODEL_MAX_NONE_IMG_BATCH_NUM 5000
#define SVP_NPU_MODEL_MAX_PROF_BATCH_NUM 1024

#include "svp_npu_common.h"
#include "svp_npu_model_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

td_bool svp_npu_model_is_aicpu_type(svp_npu_model_net_type type);

td_s32 svp_npu_model_check_aicpu_lib_version(td_u32 lib_version);

td_s32 svp_npu_model_check_model_info(const svp_npu_model_info *model_info);

td_s32 svp_npu_model_check_dynamic_batch_size(const svp_npu_model_info *model_info, td_u64 dynamic_batch_size);

td_s32 svp_npu_model_check_total_t(const svp_npu_model_info *model_info, td_u64 total_t, td_u32 recurrent_t_max);

td_s32 svp_npu_model_check_execute_param(const svp_npu_model_dataset *input,
    const svp_npu_model_dataset *output, const svp_npu_model_info *model_info);

td_s32 svp_npu_model_check_model_desc(const svp_npu_model_desc *model_desc, td_u32 max_model_num);

td_s32 svp_npu_model_check_inout_idx(const svp_npu_model_info *model_info, td_u32 model_id,
    td_ulong index, td_bool is_input);

td_s32 svp_npu_model_check_set_config_opt_param(const svp_npu_model_config_handle *handle,
    svp_npu_model_config_attr attr, const td_void *attr_value, td_ulong value_size);

td_s32 svp_npu_model_check_load_with_config_param(const svp_npu_model_config_handle *handle, td_u32 *model_id);

td_s32 svp_npu_model_check_dynamic_hw_size(const svp_npu_model_info *model_info, td_u64 height, td_u64 width);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_MODEL_PARAM_CHECK_H */
