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

#ifndef  SVP_NPU_MODEL_COMMON_H
#define  SVP_NPU_MODEL_COMMON_H

#include <pthread.h>
#include "ot_common.h"
#include "mkp_npu.h"
#include "list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define SVP_NPU_MODEL_EXTRA_BUF_NUM  0x2
#define SVP_NPU_MODEL_TASK_BUF_IDX  0x0
#define SVP_NPU_MODEL_WORK_BUF_IDX  0x1
#define SVP_NPU_MODEL_COORDI_NUM 0x4
#define SVP_NPU_MODEL_EACH_BBOX_INPUT_NUM 0x2
#define SVP_NPU_MODEL_CORE_RAM_DEPTH 0x400
#define SVP_NPU_MODEL_IMG_THREE_CHN 0x3
#define SVP_NPU_MODEL_IMG_FOUR_CHN 0x4
#define SVP_NPU_MODEL_TWO_DIM_NUM 0x2
#define SVP_NPU_MODEL_THREE_DIM_NUM 0x3
#define SVP_NPU_MODEL_FOUR_DIM_NUM 0x4
#define SVP_NPU_MODEL_MAX_DATA_BUF_NUM 0x100
#define SVP_NPU_MODEL_MAGIC_NORMAL 0x4F434950
#define SVP_NPU_MODEL_MAGIC_MINI   0x494e494d
#define SVP_NPU_MODEL_MAGIC_SIZE 0x4
#define SVP_NPU_MODEL_NAME_LEN  256
#define SVP_NPU_MODEL_DYN_RES_MIN_SHAPE_NUM 2
#define SVP_NPU_MODEL_MAX_HW_NUM SVP_ACL_MAX_HW_NUM

typedef enum {
    SVP_NPU_MODEL_DATA_NONE = 0x0,
    SVP_NPU_MODEL_DATA_IMG_BGR_PLANAR = 0x1,
    SVP_NPU_MODEL_DATA_IMG_YUV420SP = 0x2,
    SVP_NPU_MODEL_DATA_IMG_YVU420SP = 0x3,
    SVP_NPU_MODEL_DATA_IMG_YUV422SP = 0x4,
    SVP_NPU_MODEL_DATA_IMG_YVU422SP = 0x5,
    SVP_NPU_MODEL_DATA_IMG_YUV400 = 0x6,
    SVP_NPU_MODEL_DATA_IMG_RGB_PLANAR = 0x7,
    SVP_NPU_MODEL_DATA_IMG_RGB_PACKAGE = 0x8,
    SVP_NPU_MODEL_DATA_IMG_BGR_PACKAGE = 0x9,
    SVP_NPU_MODEL_DATA_IMG_XRGB_PLANAR = 0xA,
    SVP_NPU_MODEL_DATA_IMG_XBGR_PLANAR = 0xB,
    SVP_NPU_MODEL_DATA_IMG_RGBX_PLANAR = 0xC,
    SVP_NPU_MODEL_DATA_IMG_BGRX_PLANAR = 0xD,
    SVP_NPU_MODEL_DATA_IMG_XRGB_PACKAGE = 0xE,
    SVP_NPU_MODEL_DATA_IMG_XBGR_PACKAGE = 0xF,
    SVP_NPU_MODEL_DATA_IMG_RGBX_PACKAGE = 0x10,
    SVP_NPU_MODEL_DATA_IMG_BGRX_PACKAGE = 0x11,
    SVP_NPU_MODEL_DATA_IMG_RAW_RGGB = 0x12,
    SVP_NPU_MODEL_DATA_IMG_RAW_GRBG = 0x13,
    SVP_NPU_MODEL_DATA_IMG_RAW_GBRG = 0x14,
    SVP_NPU_MODEL_DATA_IMG_RAW_BGGR = 0x15,
    SVP_NPU_MODEL_DATA_INPUT_DECODEBBOX_ANCHORS = 0x20,
    SVP_NPU_MODEL_DATA_INPUT_DECODEBBOX_SCORE = 0x21,
    SVP_NPU_MODEL_DATA_INPUT_DECODEBBOX_ANCHORS_SCORE = 0x22,
    SVP_NPU_MODEL_DATA_INPUT_FILTER_SORT_NMS_ANCHORS_SCORE_ID = 0x23,
    SVP_NPU_MODEL_DATA_INPUT_FILTER_SORT_NMS_ANCHORS_SCORE = 0x24,
    SVP_NPU_MODEL_DATA_OUTPUT_NORMAL = 0x30,
    SVP_NPU_MODEL_DATA_OUTPUT_RECURRENT_T = 0x31,
    SVP_NPU_MODEL_DATA_OUTPUT_DETECTION_NUM = 0x32,
    SVP_NPU_MODEL_DATA_OUTPUT_DETECTION_BOX = 0x33,
    SVP_NPU_MODEL_DATA_FORMAT_BUTT
} svp_npu_model_data_format;

typedef enum {
    SVP_NPU_MODEL_ORDER_IGNORE = 0x0,
    SVP_NPU_MODEL_ORDER_RGB = 0x1,
    SVP_NPU_MODEL_ORDER_BGR = 0x2,
    SVP_NPU_MODEL_ORDER_XRGB = 0x3,
    SVP_NPU_MODEL_ORDER_XBGR = 0x4,
    SVP_NPU_MODEL_ORDER_RGBX = 0x5,
    SVP_NPU_MODEL_ORDER_BGRX = 0x6,
    SVP_NPU_MODEL_ORDER_RGGB = 0x7,
    SVP_NPU_MODEL_ORDER_GRBG = 0x8,
    SVP_NPU_MODEL_ORDER_GBRG = 0x9,
    SVP_NPU_MODEL_ORDER_BGGR = 0xa,
    SVP_NPU_MODEL_ORDER_BUTT
} svp_npu_model_order;

typedef enum {
    SVP_NPU_MODEL_DIM_SCALAR = 0x0,
    SVP_NPU_MODEL_DIM_VECTOR = 0x1,
    SVP_NPU_MODEL_DIM_MATRIX = 0x2,
    SVP_NPU_MODEL_DIM_TENSOR = 0x3,
    SVP_NPU_MODEL_DIM_ND_TENSOR = 0x4,
    SVP_NPU_MODEL_DIM_BUTT
} svp_npu_model_dim_type;

typedef enum {
    SVP_NPU_MODEL_RUN_MODE_CHIP = 0x0,     /* on SOC chip running */
    SVP_NPU_MODEL_RUN_MODE_FUNC_SIM = 0x1, /* functional simulation */

    SVP_NPU_MODEL_RUN_MODE_BUTT
} svp_npu_model_run_mode;

typedef enum {
    SVP_NPU_MODEL_IO_BUF_INPUT = 0x0,
    SVP_NPU_MODEL_IO_BUF_OUTPUT = 0x1,
    SVP_NPU_MODEL_IO_BUF_AICPU = 0x2,

    SVP_NPU_MODEL_IO_BUF_BUTT
} svp_npu_model_io_buf_type;

typedef enum {
    SVP_NPU_MODEL_DATA_TYPE_U8 = 0x0,
    SVP_NPU_MODEL_DATA_TYPE_U10 = 0x1,
    SVP_NPU_MODEL_DATA_TYPE_U12 = 0x2,
    SVP_NPU_MODEL_DATA_TYPE_U14 = 0x3,
    SVP_NPU_MODEL_DATA_TYPE_U16 = 0x4,
    SVP_NPU_MODEL_DATA_TYPE_U2I = 0x5,
    SVP_NPU_MODEL_DATA_TYPE_S2 = 0x6,
    SVP_NPU_MODEL_DATA_TYPE_U2 = 0x7,
    SVP_NPU_MODEL_DATA_TYPE_U4I = 0x8,
    SVP_NPU_MODEL_DATA_TYPE_S4 = 0x9,
    SVP_NPU_MODEL_DATA_TYPE_U4 = 0xA,
    SVP_NPU_MODEL_DATA_TYPE_S8 = 0xB,
    SVP_NPU_MODEL_DATA_TYPE_S16 = 0xC,
    SVP_NPU_MODEL_DATA_TYPE_F16 = 0xD,
    SVP_NPU_MODEL_DATA_TYPE_S32 = 0xE,
    SVP_NPU_MODEL_DATA_TYPE_U32 = 0xF,
    SVP_NPU_MODEL_DATA_TYPE_F32 = 0x10,
    SVP_NPU_MODEL_DATA_TYPE_S64 = 0x11,
    SVP_NPU_MODEL_DATA_TYPE_U64 = 0x12,
    SVP_NPU_MODEL_DATA_TYPE_BUTT
} svp_npu_model_data_type;

typedef struct {
    td_u32 magic;
    td_u32 crc;
    td_u32 model_size;
    td_u32 param_size;
    td_u32 param_offset;
    td_u32 tmp_buf_size;
    td_u32 rsv1;
    td_u32 rsv2;
} svp_npu_model_header;

typedef struct {
    const td_u8 *buf;
    td_ulong size;
} svp_npu_model_data_buffer;

/* The dynamic resolution om mdl info item */
typedef struct {
    td_u32 work_buf_size;
    td_u32 dump_buf_size;
    td_u32 aicpu_buf_size;
} svp_npu_model_dyn_res_mdl_info;

/* The dynamic resolution om net info item */
typedef struct {
    td_u32 instr_offset;
    td_u32 instr_size;
    td_u32 instr_num;
    td_u32 aicpu_buf_offset;
    td_u32 aicpu_buf_size;
    svp_npu_model_data_buffer aicpu_attr_param;
} svp_npu_model_dyn_res_net_info;

/* The dynamic resolution om input info item */
typedef struct {
    td_u32 idx_or_offset;
    td_u32 default_buf_size;
    td_u32 default_stride;
    svp_npu_model_data_buffer shape;
} svp_npu_model_dyn_res_input_info;

/* The dynamic resolution om output info item */
typedef struct {
    td_u32 idx_or_offset;
    td_u32 default_buf_size;
    td_u32 default_stride;
    svp_npu_model_data_buffer shape;
} svp_npu_model_dyn_res_output_info;

/* The dynamic resolution om layer info item */
typedef struct {
    svp_npu_model_data_buffer output_shape;
    td_u32 dump_offset;
    td_bool dump_en;
    td_u32 prof_layer_id;
    td_u32 last_pc;
} svp_npu_model_dyn_res_layer_info;

typedef struct {
    td_u32 id;
    td_u32 shape_num;
    svp_npu_model_dim_type dim_type;
    svp_npu_model_data_format format;
    svp_npu_model_order rgb_order;
    svp_npu_model_data_type data_type;
    svp_npu_model_data_buffer layer_name;
    svp_npu_model_io_buf_type io_buf_type;
    td_u32 dim_n;
    td_u32 recurrent_id;
    svp_npu_model_dyn_res_input_info *dyn_res_input_info;
} svp_npu_model_input_info;

typedef struct {
    td_u32 id;
    td_u32 shape_num;
    svp_npu_model_dim_type dim_type;
    svp_npu_model_data_format format;
    svp_npu_model_data_type data_type;
    svp_npu_model_data_buffer layer_name;
    svp_npu_model_io_buf_type io_buf_type;
    td_u32 dim_n;
    svp_npu_model_dyn_res_output_info *dyn_res_output_info;
} svp_npu_model_output_info;

typedef enum {
    SVP_NPU_MODEL_NET_TYPE_CNN = 0x0,       /* Non-ROI input cnn net */
    SVP_NPU_MODEL_NET_TYPE_ROI = 0x1,       /* With ROI input cnn net */
    SVP_NPU_MODEL_NET_TYPE_RECURRENT = 0x2, /* RNN or LSTM net */
    SVP_NPU_MODEL_NET_TYPE_RECURRENT_BIDIRECTIONAL = 0x3,
    SVP_NPU_MODEL_NET_TYPE_DNN = 0x4,
    SVP_NPU_MODEL_NET_TYPE_CPU = 0x5,
    SVP_NPU_MODEL_NET_TYPE_BUTT
} svp_npu_model_net_type;

typedef struct {
    td_u32 id;
    svp_npu_model_net_type type;
    td_u32 input_num;
    td_u32 output_num;
    td_u32 roi_pool_num;
    svp_npu_model_input_info *input_info;
    svp_npu_model_output_info *output_info;
    svp_npu_model_aicpu_buffer *dump_info;
    svp_npu_model_aicpu_func aicpu_func;
    svp_npu_model_data_buffer aicpu_op_name;
    td_u32 recurrent_t_max;
    td_u32 recurrent_seq_max;
    td_bool is_recurrent_cont_enable;
    td_bool is_high_detection_accuracy;
    td_bool is_decode_bbox_enable;
    td_bool is_nms_enable;
    td_bool is_filter_bbox_enable;
    td_bool is_sort_enable;
    td_bool need_class_length_info_input;
    td_u32 prior_box_num;
    td_u32 variance_num;
    td_u32 layer_info_num; /* layer info num of current net seg */
    td_u32 aicpu_param_size;

    td_u32 roi_layer_num;
    td_u32 prof_layer_num;
    td_u32 batch_layer_num;
    td_u32 prof_max_loop_num;
    td_u32 prof_buf_size;
    svp_npu_model_dyn_res_net_info *dyn_res_net_info;
} svp_npu_model_net_info;

typedef struct {
    svp_npu_model_data_buffer layer_name;
    svp_npu_model_data_buffer layer_type_name;
    svp_npu_model_data_buffer input_idx;
    svp_npu_model_data_type data_type;
    td_u32 layer_id;
    td_u32 seg_id;
    td_u32 output_idx;
    td_bool is_raw_type;
    td_bool batch_flag;
    td_u32 img_bit_width;
    td_bool last_merge_flag;
    svp_npu_model_dyn_res_layer_info *dyn_res_layer_info;
} svp_npu_model_layer_info;

typedef enum {
    SVP_NPU_MODEL_RELEASE = 0x0,
    SVP_NPU_MODEL_WITH_LAYER_INFO = 0x1,
    SVP_NPU_MODEL_PROFILE = 0x2,
    SVP_NPU_MODEL_DUMP = 0x4,

    SVP_NPU_MODEL_TYPE_BUTT
} svp_npu_model_type;

typedef struct {
    td_u32 layer_idx;
} svp_npu_model_dump_layer;

typedef struct {
    svp_npu_model_input_info *input_info;
    svp_npu_model_output_info *output_info;
    td_u32 *input_idx;
    td_u32 *output_idx;
    td_u32 input_num;
    td_u32 output_num;
} svp_npu_model_ext_inout_info;

typedef struct {
    td_u32 inout_addr_stride_size;
    td_u32 task_header_size;
    td_u32 task_info_size;
} svp_npu_model_task_buf_size_info;

typedef struct {
    td_u32 head;
    td_u32 tail;
    td_u64 begin_time;
    td_u64 end_time;
    td_u64 total_len;
    td_s32 pid;
    td_s32 tid;
    td_void *virt_addr;
} svp_npu_model_prof_acl_api_info;

typedef struct {
    td_bool is_prof_support;
    td_bool prof_en;
    td_u32 in_pic_num;
    td_u32 prof_buf_size[SVP_NPU_PROF_TYPE_BUTT];
    td_u32 total_size;
    svp_npu_model_prof_acl_api_info acl_api_info;
    td_phys_addr_t prof_phys_addr;
    td_void *prof_virt_addr;
    td_u64 in_pic_cnt;
} svp_npu_model_add_prof_info;

typedef struct {
    svp_npu_model_dump_layer *dump_layer;
    svp_npu_model_ext_inout_info ext_inout_info;
    svp_npu_model_task_buf_size_info task_buf_size_info;
    td_u32 extra_buf_size[SVP_NPU_MODEL_EXTRA_BUF_NUM];
    svp_npu_mem_info ctrl_info_mem;
    td_u32 dump_layer_num;
    td_char *dump_time;
    td_u32 scalar_shape;
    td_u32 aicpu_net_num;
    td_u32 recurrent_t_max;
    td_char model_format_name[SVP_NPU_MODEL_NAME_LEN];
    svp_npu_model_add_prof_info prof_info;
} svp_npu_model_additional_info;

typedef struct {
    svp_npu_model_data_buffer model_name;
    svp_npu_model_data_buffer model_format_name;
    svp_npu_model_data_buffer param;
    td_phys_addr_t param_phys_addr;
    td_u32 model_type;
    svp_npu_model_run_mode run_mode;
    td_u32 model_size;
    td_u32 work_buf_size;
    td_u32 sdk_arch;
    td_u32 mapper_arch;
    td_u32 mapper_version;
    td_u32 ddr_align;
    svp_npu_model_net_info *net_info;
    svp_npu_model_layer_info *layer_info;
    svp_npu_model_additional_info *add_info;
    svp_npu_model_dyn_res_mdl_info *dyn_res_mdl_info;
    td_u32 priority;
    td_u32 net_num;
    td_u32 layer_info_num;
    td_u32 batch_num;
    td_u32 prof_aicpu_cnt;
    td_u32 image_size_num; /* dynamic resolution num */
    td_u32 dyn_res_cur_idx; /* dynamic resolution index set by user */
} svp_npu_model_info;

typedef struct {
    const td_u8 *model_data_ptr;
    td_u32 model_buf_size;
    td_u32 model_buf_offset;
    td_u32 layer_start_offset;
    td_u32 layer_end_offset;
    td_u32 net_start_offset;
    td_u32 net_end_offset;

    svp_npu_model_net_info *net_info_ptr;
    svp_npu_model_input_info *input_info_ptr;
    svp_npu_model_output_info *output_info_ptr;
    svp_npu_model_layer_info *layer_info_ptr;
    svp_npu_model_dyn_res_mdl_info *dyn_res_mdl_info;
    svp_npu_model_dyn_res_net_info *dyn_res_net_info;
    svp_npu_model_dyn_res_input_info *dyn_res_input_info;
    svp_npu_model_dyn_res_output_info *dyn_res_output_info;
    svp_npu_model_dyn_res_layer_info *dyn_res_layer_info;

    td_bool is_get_model_info;
    td_u32 net_info_num;
    td_u32 input_info_num;
    td_u32 output_info_num;
    td_u32 layer_info_num;
    td_u32 input_data_num;
    td_u32 output_data_num;

    td_u32 net_info_idx;
    td_u32 input_info_idx;
    td_u32 output_info_idx;
    td_u32 layer_info_idx;
    td_u32 image_size_num; /* dynamic resolution num */
    td_u32 dyn_res_idx; /* dynamic resolution tmp idx */
    td_u32 dyn_res_buf_size; /* dynamic resolution buf size */
} svp_npu_model_dynamic_info;

typedef struct {
    svp_npu_model_info *model_info;
    td_u16 ref_num;
    td_u16 model_id;
} svp_npu_model;

typedef struct {
    svp_npu_dump_ctrl_arg ctrl_arg;
    td_u32 layer_id;
    td_u32 stride;
    td_u16 output_id;
    td_u16 seg_id;
    td_bool is_aicpu;
} svp_npu_dump_info_arg;

typedef struct {
    td_u32 input_num;
    td_u32 output_num;
    td_u32 net_idx;
    td_u32 dyn_res_cur_idx;
    td_u32 image_size_num;
    td_u32 batch_num;
    td_u32 dyn_res_idx;
} svp_npu_model_resource_info;

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_MODEL_COMMON_H */
