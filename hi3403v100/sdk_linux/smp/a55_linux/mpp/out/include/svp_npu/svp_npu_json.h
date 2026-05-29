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

#ifndef SVP_NPU_JSON_H
#define SVP_NPU_JSON_H

#include "svp_acl_mdl.h"
#include "ot_common.h"
#include "ot_errno.h"
#include "list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    SVP_JSON_TYPE_ROOT = 0,
    SVP_JSON_TYPE_OBJ,
    SVP_JSON_TYPE_ARRAY,
    SVP_JSON_TYPE_STRING,
    SVP_JSON_TYPE_ARRAY_OBJ,
    SVP_JSON_TYPE_ARRAY_STRING,
    SVP_JSON_TYPE_BUTT
} svp_npu_json_type;

typedef struct {
    svp_npu_json_type type;

    td_char *key;
    td_char *str_val;
    td_s32 num_val;

    td_void *father;

    struct list_head head;
    struct list_head list;
} svp_npu_json_node;

typedef struct {
    td_char *file_data;
    td_u32 file_size;

    svp_npu_json_node *node;
} svp_npu_json_context;

td_s32 svp_npu_parse_json_init(const td_char *file_name, svp_npu_json_context *ctx);
td_void svp_npu_parse_json_exit(svp_npu_json_context *ctx);

td_char *svp_npu_json_get_val(const svp_npu_json_node *obj_node, const td_char *key);
svp_npu_json_node *svp_npu_json_get_obj_node(const svp_npu_json_node *root_node, const td_char *key);

#define MY_TRACE    svp_npu_err_trace("TRACE~~~~~~~~~~~~~~~~~~~~~~\n")

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* SVP_NPU_JSON_H */
