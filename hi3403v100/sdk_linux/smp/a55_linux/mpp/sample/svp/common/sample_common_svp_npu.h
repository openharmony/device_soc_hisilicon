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

#ifndef SAMPLE_COMMON_SVP_NPU_H
#define SAMPLE_COMMON_SVP_NPU_H
#include "ot_type.h"
#include "svp_acl.h"
#include "svp_acl_mdl.h"

#define SAMPLE_SVP_NPU_MAX_THREAD_NUM    16
#define SAMPLE_SVP_NPU_MAX_TASK_NUM      16
#define SAMPLE_SVP_NPU_MAX_MODEL_NUM     1
#define SAMPLE_SVP_NPU_EXTRA_INPUT_NUM   2
#define SAMPLE_SVP_NPU_BYTE_BIT_NUM      8
#define SAMPLE_SVP_NPU_SHOW_TOP_NUM      5
#define SAMPLE_SVP_NPU_MAX_NAME_LEN      32
#define SAMPLE_SVP_NPU_MAX_MEM_SIZE      0xFFFFFFFF
#define SAMPLE_SVP_NPU_RECT_LEFT_TOP     0
#define SAMPLE_SVP_NPU_RECT_RIGHT_TOP    1
#define SAMPLE_SVP_NPU_RECT_RIGHT_BOTTOM 2
#define SAMPLE_SVP_NPU_RECT_LEFT_BOTTOM  3
#define SAMPLE_SVP_NPU_THRESHOLD_NUM     4

#define SAMPLE_SVP_NPU_RFCN_THRESHOLD_NUM      2
#define SAMPLE_SVP_NPU_AICPU_WAIT_TIME         1000
#define SAMPLE_SVP_NPU_RECT_COLOR              0x0000FF00
#define SAMPLE_SVP_NPU_MILLIC_SEC              20000
#define SAMPLE_SVP_NPU_IMG_THREE_CHN           3
#define SAMPLE_SVP_NPU_DOUBLE                  2
#define SAMPLE_SVP_NPU_YOLO_CLASS_NUM 80
#define SAMPLE_SVP_NPU_YOLO_CPU_OUT_NUM 3
#define SAMPLE_SVP_NPU_YOLO_SCALE_SIZE 3
#define SAMPLE_SVP_NPU_YOLO_OUT_PARM_NUM 85
#define SAMPLE_SVP_NPU_YOLO_CLASS_MEMBER_NUM 6
#define SAMPLE_SVP_NPU_YOLO_SCORE_THRESHOLE 0.5

typedef struct {
    td_u32 model_id;
    td_bool is_load_flag;
    td_ulong model_mem_size;
    td_void *model_mem_ptr;
    svp_acl_mdl_desc *model_desc;
    size_t input_num;
    size_t output_num;
    size_t dynamic_batch_idx;
} sample_svp_npu_model_info;

typedef struct {
    td_u32 max_batch_num;
    td_u32 dynamic_batch_num;
    td_u32 total_t;
    td_bool is_cached;
    td_u32 model_idx;
} sample_svp_npu_task_cfg;

typedef struct {
    sample_svp_npu_task_cfg cfg;
    svp_acl_mdl_dataset *input_dataset;
    svp_acl_mdl_dataset *output_dataset;
    td_void *task_buf_ptr;
    size_t task_buf_size;
    size_t task_buf_stride;
    td_void *work_buf_ptr;
    size_t work_buf_size;
    size_t work_buf_stride;
} sample_svp_npu_task_info;

typedef struct {
    td_void *work_buf_ptr;
    size_t work_buf_size;
    size_t work_buf_stride;
} sample_svp_npu_shared_work_buf;

typedef struct {
    td_float score;
    td_u32 class_id;
} sample_svp_npu_top_n_result;

typedef struct {
    td_char *num_name;
    td_char *roi_name;
    td_bool has_background;
    td_u32 roi_offset;
    td_bool is_cpu_rpn;
    td_u32 idx;
} sample_svp_npu_detection_info;

typedef enum {
    SAMPLE_SVP_NPU_RFCN = 0,
    SAMPLE_SVP_NPU_YOLO = 1,
    SAMPLE_SVP_NPU_HRNET = 2,
    SAMPLE_SVP_NPU_MODEL_NAME_BUTT,
} svp_npu_model_name;

typedef struct {
    svp_npu_model_name model_name;
    sample_svp_npu_detection_info *detection_info;
} sample_svp_npu_thread_args;

typedef struct {
    td_char roi_num_name[SAMPLE_SVP_NPU_MAX_NAME_LEN];
    td_char roi_class_name[SAMPLE_SVP_NPU_MAX_NAME_LEN];
} sample_svp_npu_roi_info;

typedef struct {
    td_float nms_threshold;
    td_float score_threshold;
    td_float min_height;
    td_float min_width;
    td_char *name;
} sample_svp_npu_threshold;

typedef struct {
    td_float score;
    td_float xcenter;
    td_float ycenter;
    td_float w;
    td_float h;
    td_float class_id;
} sample_svp_npu_valid_box;

typedef struct {
    td_float lx;
    td_float ly;
    td_float rx;
    td_float ry;
    td_float score;
    td_float class_id;
    td_float area;
} sample_svp_npu_roi_box;

typedef struct {
    td_float *out_data;
    size_t idx;
    size_t w_stride_offset;
    td_float score_thr;
    td_u32 out_width;
    td_u32 chn_step;
    td_u32 out_height_idx;
    td_u32 obj_score_offset;
} sample_svp_npu_detection_inner_param;

/* acl init */
td_s32 sample_common_svp_npu_acl_init(const td_char *acl_config_path, td_s32 dev_id);
/* acl deinit */
td_void sample_common_svp_npu_acl_deinit(td_s32 dev_id);

#endif
