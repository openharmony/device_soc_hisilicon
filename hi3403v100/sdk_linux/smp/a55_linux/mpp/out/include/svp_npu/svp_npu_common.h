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

#ifndef SVP_NPU_COMMON_H
#define SVP_NPU_COMMON_H

#include "svp_acl_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SVP_NPU_BIT_NUM_2       2
#define SVP_NPU_BIT_NUM_4       4
#define SVP_NPU_BIT_NUM_8       8
#define SVP_NPU_BIT_NUM_10      10
#define SVP_NPU_BIT_NUM_12      12
#define SVP_NPU_BIT_NUM_14      14
#define SVP_NPU_BIT_NUM_16      16
#define SVP_NPU_BIT_NUM_24      24
#define SVP_NPU_BIT_NUM_32      32
#define SVP_NPU_BIT_NUM_64      64

/* SVP_NPU core id */
typedef enum {
    SVP_NPU_DEVICE_ID_0 = 0x0,

    SVP_NPU_DEVICE_ID_BUTT
} svp_npu_device_id;

typedef enum {
    SVP_NPU_CALLBACK_BLOCK_TYPE_NO_BLOCK = 0x0,
    SVP_NPU_CALLBACK_BLOCK_TYPE_BLOCK = 0x1,
    SVP_NPU_CALLBACK_BLOCK_TYPE_BUTT
} svp_npu_callback_block_type;

typedef enum {
    SVP_NPU_RUN_MODE_DEVICE = 0x0,
    SVP_NPU_RUN_MODE_HOST = 0x1,

    SVP_NPU_RUN_MODE_BUTT
} svp_npu_run_mode;

typedef enum {
    SVP_NPU_RUNTIME_MALLOC_HUGE_FIRST = 0x0,
    SVP_NPU_RUNTIME_MALLOC_HUGE_ONLY = 0x1,
    SVP_NPU_RUNTIME_MALLOC_NORMAL_ONLY = 0x2,

    SVP_NPU_RUNTIME_MALLOC_BUTT
} svp_npu_runtime_malloc_policy;

typedef enum {
    SVP_NPU_MODEL_PRIORITY_INT32 = 0x0,
    SVP_NPU_MODEL_LOAD_TYPE_SIZET,
    SVP_NPU_MODEL_PATH_PTR, /* pointer to model load path with deep copy */
    SVP_NPU_MODEL_MEM_ADDR_PTR, /* pointer to model memory with shallow copy */
    SVP_NPU_MODEL_MEM_SIZET,
    SVP_NPU_MODEL_WEIGHT_ADDR_PTR, /* pointer to weight memory of model with shallow copy */
    SVP_NPU_MODEL_WEIGHT_SIZET,
    SVP_NPU_MODEL_WORKSPACE_ADDR_PTR, /* pointer to worksapce memory of model with shallow copy */
    SVP_NPU_MODEL_WORKSPACE_SIZET,
    SVP_NPU_MODEL_INPUTQ_NUM_SIZET,
    SVP_NPU_MODEL_INPUTQ_ADDR_PTR, /* pointer to inputQ with shallow copy */
    SVP_NPU_MODEL_OUTPUTQ_NUM_SIZET,
    SVP_NPU_MODEL_OUTPUTQ_ADDR_PTR, /* pointer to outputQ with shallow copy */

    SVP_NPU_MODEL_CONFIG_BUTT
} svp_npu_model_config_attr;

typedef struct {
    struct list_head head;
    const svp_npu_data_buffer *data_buffer;
    td_u16 id;
} svp_npu_data_buffer_node;

typedef struct {
    td_u16 device_id;
    td_u16 node_id;
} svp_npu_runtime_stream;

typedef struct {
    td_u16 device_id;
    td_u16 node_id;
} svp_npu_runtime_ctx;

typedef struct {
    td_char name[SVP_NPU_MAX_TENSOR_NAME_LEN]; /* tensor name */
    size_t dim_count;  /* dim array count */
    td_s64 dims[SVP_NPU_MAX_DIM_CNT]; /* dim data array */
} svp_npu_model_io_dims;

typedef td_void (*svp_npu_callback)(td_void *user_data);

typedef enum {
    SVP_NPU_DUMP_MODE_INPUT = 0x0,
    SVP_NPU_DUMP_MODE_OUTPUT = 0x1,
    SVP_NPU_DUMP_MODE_ALL = 0x2,

    SVP_NPU_DUMP_MODE_BUTT
} svp_npu_dump_mode;

typedef struct {
    td_char *name;
    td_u32 size;
} svp_npu_dump_string;

typedef struct {
    struct list_head head;
    svp_npu_dump_string layer_name;
} svp_npu_dump_layer_node;

typedef struct {
    struct list_head head;
    struct list_head dump_layer_list;
    svp_npu_dump_string model_name;
    td_bool is_dump_all;
    td_u32 dump_layer_num;
} svp_npu_dump_model_node;

typedef struct {
    struct list_head dump_model_list;
    svp_npu_dump_string dump_path;
    td_u32 dump_model_num;
    td_u32 match_model_num;
    svp_npu_dump_mode dump_mode;
} svp_npu_dump_info;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* SVP_NPU_COMMON_H */
