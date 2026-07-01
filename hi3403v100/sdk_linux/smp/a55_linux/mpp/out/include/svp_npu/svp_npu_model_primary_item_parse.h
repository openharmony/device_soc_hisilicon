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

#ifndef  SVP_NPU_MODEL_PRIMARY_ITEM_PARSE_H
#define  SVP_NPU_MODEL_PRIMARY_ITEM_PARSE_H

#include "svp_npu_model_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

td_u32 svp_npu_model_read_sdk_arch(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_mapper_arch(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_mapper_version(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_beta_version(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_patch_version(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_build_info(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_tmp_buf_size(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_run_mode(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_net_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_batch_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_model_type(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_dump_buf_size(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_model_name(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_ddr_align(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_aicpu_buf_size(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_image_size_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_net_info(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_net_id(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_net_type(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_instr_offset(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_instr_size(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_instr_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_input_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_output_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_recurrent_t_max(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_recurrent_seq_max(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_is_high_detection_accuracy(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_is_recurrent_cont_enable(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_is_decode_bbox_enable(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_is_nms_enable(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_is_filter_bbox_enable(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_is_sort_enable(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_prior_box_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_variance_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_class_len_info_input_flag(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_info_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_aicpu_op_name(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_aicpu_inner_buf_offset(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_aicpu_inner_buf_size(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_aicpu_attr_param(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_input_info(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_input_id(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_input_dim_type(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_input_format(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_input_order(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_input_name(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_input_data_type(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_input_shape(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_input_type(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_input_id_or_aicpu_buf_offset(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_input_batch_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_input_recurrent_id(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_output_info(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_output_id(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_output_dim_type(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_output_name(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_output_format(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_output_data_type(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_output_shape(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_output_type(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_output_id_or_aicpu_buf_offset(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_output_batch_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_nets_prof_layer_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);
td_u32 svp_npu_model_read_nets_batch_layer_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);
td_u32 svp_npu_model_read_nets_prof_max_loop_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);
td_u32 svp_npu_model_read_nets_roi_layer_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_aicpu_prof_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_MODEL_PRIMARY_ITEM_PARSE_H */
