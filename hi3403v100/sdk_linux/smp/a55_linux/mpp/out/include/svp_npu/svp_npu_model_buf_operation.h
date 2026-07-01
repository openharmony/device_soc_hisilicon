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

#ifndef  SVP_NPU_MODEL_BUF_OPERATION_H
#define  SVP_NPU_MODEL_BUF_OPERATION_H

#include "svp_npu_common.h"
#include "svp_npu_model_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 svp_npu_model_calc_stride(td_u32 align_dim, svp_npu_model_data_format format,
    svp_npu_model_data_type data_type, td_u32 ddr_align, td_u32 *stride);

td_s32 svp_npu_model_calc_input_size(const svp_npu_model_input_info *input_info,
    const svp_npu_model_resource_info *info, td_u32 ddr_align, td_u32 *mem_size, td_u32 *stride);

td_s32 svp_npu_model_calc_output_size(const svp_npu_model_output_info *output_info,
    const svp_npu_model_resource_info *info, td_u32 ddr_align, td_u32 *mem_size, td_u32 *stride);

td_s32 svp_npu_model_calc_model_info_size(const td_u8 *model_data, td_u32 model_buf_size, td_u32 *model_info_size,
    svp_npu_model_dynamic_info *dynamic_info, td_u32 *ctrl_info_size);

td_s32 svp_npu_model_fill_model_info(const td_u8 *model_data, td_u32 model_buf_size,
    svp_npu_model_dynamic_info *dynamic_info, svp_npu_model_info *model_info);

td_s32 svp_npu_model_fill_task_buf(const svp_npu_model_dataset *input, const svp_npu_model_dataset *output,
    const svp_npu_model *model, svp_npu_mem_info *mem_info, td_s32 sub_stream_id);

td_u32 svp_npu_model_calc_prof_aicore_buf_cnt(td_u32 in_num, td_u32 batch_num, td_u32 layer_num,
    td_u32 batch_layer_num);

td_s32 svp_npu_model_set_prof_addr_and_stride(const svp_npu_model_info *model_info);
#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_MODEL_BUF_OPERATION_H */
