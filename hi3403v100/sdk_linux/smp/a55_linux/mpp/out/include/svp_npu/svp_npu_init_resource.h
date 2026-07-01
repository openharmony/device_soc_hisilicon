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

#ifndef  SVP_NPU_INIT_RESOURCE_H
#define  SVP_NPU_INIT_RESOURCE_H

#include "ot_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

td_void svp_npu_get_process_init_time(td_char *dump_time, td_u32 size);

td_s32 svp_npu_resource_init(const td_char *config_path);

td_s32 svp_npu_resource_finalize();

td_bool svp_npu_get_acl_init_flag(td_void);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_INIT_RESOURCE_H */
