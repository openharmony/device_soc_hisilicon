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

#ifndef  SVP_ACL_COMMON_H
#define  SVP_ACL_COMMON_H

#include <stdint.h>
#include "svp_acl_mdl.h"
#include "ot_type.h"
#include "list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define SVP_NPU_DYNAMIC_TENSOR_NAME SVP_ACL_DYNAMIC_TENSOR_NAME
#define SVP_NPU_MAX_DIM_CNT         SVP_ACL_MAX_DIM_CNT
#define SVP_NPU_MAX_TENSOR_NAME_LEN SVP_ACL_MAX_TENSOR_NAME_LEN
#define SVP_NPU_MAX_BATCH_NUM       SVP_ACL_MAX_BATCH_NUM

#define SVP_NPU_MODEL_LOAD_FROM_FILE            SVP_ACL_MDL_LOAD_FROM_FILE
#define SVP_NPU_MODEL_LOAD_FROM_FILE_WITH_MEM   SVP_ACL_MDL_LOAD_FROM_FILE_WITH_MEM
#define SVP_NPU_MODEL_LOAD_FROM_MEM             SVP_ACL_MDL_LOAD_FROM_MEM
#define SVP_NPU_MODEL_LOAD_FROM_MEM_WITH_MEM    SVP_ACL_MDL_LOAD_FROM_MEM_WITH_MEM
#define SVP_NPU_MODEL_LOAD_FROM_FILE_WITH_Q     SVP_ACL_MDL_LOAD_FROM_FILE_WITH_Q
#define SVP_NPU_MODEL_LOAD_FROM_MEM_WITH_Q      SVP_ACL_MDL_LOAD_FROM_MEM_WITH_Q

#define SVP_NPU_NET_TYPE_CNN            SVP_ACL_NET_TYPE_CNN
#define SVP_NPU_NET_TYPE_RECURRENT      SVP_ACL_NET_TYPE_RECURRENT

typedef struct {
    td_void *data;
    td_u32 size;
    td_u32 stride;
} svp_npu_data_buffer;

typedef struct {
    td_u16 model_id;
} svp_npu_model_desc;

typedef struct {
    struct list_head data_buffer_list;
    td_u32 dynamic_batch_size;
    td_u32 total_t;
    td_u32 num;
    td_u32 total_node_size;
    td_u32 dynamic_resolution_height;
    td_u32 dynamic_resolution_width;
    td_u32 dynamic_resolution_index;
} svp_npu_model_dataset;

typedef struct {
    td_void *model_mem;
    td_ulong size;
    td_u32 priority;
    td_u32 load_type;
} svp_npu_model_config_handle;

struct svp_acl_mdl_desc {
    svp_npu_model_desc model_desc;
};

struct svp_acl_mdl_dataset {
    svp_npu_model_dataset dataset;
};

struct svp_acl_data_buffer {
    svp_npu_data_buffer data_buffer;
};

struct svp_acl_mdl_config_handle {
    svp_npu_model_config_handle config_handle;
};

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_ACL_COMMON_H */
