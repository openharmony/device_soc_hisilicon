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

#ifndef  SVP_NPU_MODEL_SECONDARY_ITEM_PARSE_H
#define  SVP_NPU_MODEL_SECONDARY_ITEM_PARSE_H

#include "svp_npu_model_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

td_u32 svp_npu_model_read_layer_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_info(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_id(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_name(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_output_shape(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_input_idx(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_output_idx(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_data_type(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_batch_flag(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_output_lut_4_to_8(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_input_img_bit_width(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_img_is_raw_type(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_input_name(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_debug_info_num(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_last_merge_layer_flag(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_dump_en(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_type_name(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_dump_offset(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_trap_instr_pc(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_infos_prof_layer_id(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_layer_seg_id(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_MODEL_SECONDARY_ITEM_PARSE_H */
