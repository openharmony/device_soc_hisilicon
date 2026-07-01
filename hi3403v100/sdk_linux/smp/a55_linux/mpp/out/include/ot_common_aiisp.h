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

#ifndef OT_COMMON_AIISP_H
#define OT_COMMON_AIISP_H

#include "ot_common_aiv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef ot_aiv_type                  ot_aiisp_type;
typedef ot_aiv_mem_info              ot_aiisp_mem_info;
typedef ot_aiv_model                 ot_aiisp_model;

#define OT_AIISP_TYPE_AIDEHAZE       OT_AIV_TYPE_AIDEHAZE
#define OT_AIISP_TYPE_BUTT           OT_AIV_TYPE_BUTT

#define OT_AIISP_MAX_MODEL_NUM       OT_AIV_MAX_MODEL_NUM
#define OT_AIISP_MAX_DEPTH           OT_AIV_MAX_DEPTH

#define OT_ERR_AIISP_INVALID_PIPE_ID OT_ERR_AIV_INVALID_PIPE_ID
#define OT_ERR_AIISP_ILLEGAL_PARAM   OT_ERR_AIV_ILLEGAL_PARAM
#define OT_ERR_AIISP_NULL_PTR        OT_ERR_AIV_NULL_PTR
#define OT_ERR_AIISP_NOT_SUPPORT     OT_ERR_AIV_NOT_SUPPORT
#define OT_ERR_AIISP_NOT_PERM        OT_ERR_AIV_NOT_PERM
#define OT_ERR_AIISP_NO_MEM          OT_ERR_AIV_NO_MEM
#define OT_ERR_AIISP_NOT_READY       OT_ERR_AIV_NOT_READY

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
