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

#ifndef  SVP_NPU_DUMP_LIST_H
#define  SVP_NPU_DUMP_LIST_H

#include "svp_npu_common.h"
#include "svp_npu_model_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

td_void svp_npu_dump_init_dump_info_list(svp_npu_dump_info *dump_info);

td_void svp_npu_dump_deinit_dump_info_list(svp_npu_dump_info *dump_info);

svp_npu_dump_model_node *svp_npu_dump_get_last_model_node(svp_npu_dump_info *dump_info);

td_s32 svp_npu_dump_add_model_node_to_list(svp_npu_dump_info *dump_info, const td_char *model_name);

td_s32 svp_npu_dump_add_layer_node_to_list(svp_npu_dump_info *dump_info, const td_char *layer_name);

td_s32 svp_npu_dump_search_dump_info(const svp_npu_dump_info *dump_info, svp_npu_model_info *model_info);

td_void svp_npu_dump_free_dump_info(svp_npu_model_info *model_info);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_DUMP_LIST_H */
