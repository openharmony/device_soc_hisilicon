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

#ifndef  PQP_MODEL_ITEM_PARSE_H
#define  PQP_MODEL_ITEM_PARSE_H

#include "pqp_param_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

td_u32 pqp_read_param_header(pqp_param_dynamic_info *dynamic_info, pqp_param_info *param_info);

td_u32 pqp_read_param_data(pqp_param_dynamic_info *dynamic_info, td_u32 buf_size, pqp_param_info *param_info);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* PQP_MODEL_H */
