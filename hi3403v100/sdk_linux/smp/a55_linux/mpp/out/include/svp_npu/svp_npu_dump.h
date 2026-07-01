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

#ifndef  SVP_NPU_DUMP_H
#define  SVP_NPU_DUMP_H

#include "svp_npu_model_common.h"
#include "svp_npu_json.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 svp_npu_dump_init(td_bool acl_init_flag);

td_s32 svp_npu_dump_set_cfg(const td_char *dump_cfg_path);

td_s32 svp_npu_dump_set_info(const svp_npu_json_node *root_node, td_bool acl_init_flag);

td_s32 svp_npu_dump_deinit(td_bool acl_deinit_flag);

td_s32 svp_npu_dump_match_model(svp_npu_model_info *model_info);

td_void svp_npu_dump_result_file(const svp_npu_model_info *model_info, svp_npu_dump_info_arg *dump_arg);

td_void svp_npu_dump_aicpu_result_file(const svp_npu_model_info *model_info,
    svp_npu_dump_info_arg *dump_arg, const td_void *dump_addr);

td_bool svp_npu_dump_flag(td_void);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_DUMP_H */
