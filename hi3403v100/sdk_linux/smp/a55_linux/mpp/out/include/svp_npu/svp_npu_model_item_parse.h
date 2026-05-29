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

#ifndef  SVP_NPU_MODEL_ITEM_PARSE_H
#define  SVP_NPU_MODEL_ITEM_PARSE_H

#include "svp_npu_model_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

td_u32 svp_npu_model_read_header(svp_npu_model_dynamic_info *dynamic_info, svp_npu_model_info *model_info);

td_u32 svp_npu_model_read_item_by_id(svp_npu_model_dynamic_info *dynamic_info,
    svp_npu_model_info *model_info, td_u16 id);

td_u32 svp_npu_model_read_item(svp_npu_model_dynamic_info *dynamic_info,
    td_u32 buf_size, svp_npu_model_info *model_info);

td_u32 svp_npu_model_read_item_value(svp_npu_model_dynamic_info *dynamic_info, td_u16 id, td_u32 *value);

td_u32 svp_npu_model_read_item_value_size(svp_npu_model_dynamic_info *dynamic_info, td_u16 id, td_u32 *value_size);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_MODEL_ITEM_PARSE_H */
