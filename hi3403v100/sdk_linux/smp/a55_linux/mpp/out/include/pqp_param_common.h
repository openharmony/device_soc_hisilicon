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

#ifndef  PQP_PARAM_COMMON_H
#define  PQP_PARAM_COMMON_H

#include "common_pqp.h"
#include "list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define PQP_PARAM_MAX_NAME_LEN       32
#define PQP_PARAM_MAX_SHAPE_NUM       3
#define PQP_MAX_SECT_NUM              1
#define PQP_MAX_IN_NUM               32
#define PQP_MAX_OUT_NUM              32
#define PQP_BYTE_MASK                0xff

#define PQP_PARAM_RSV_VAL 0
#define PQP_PARAM_DTYPE_DEFAULT 0xff
#define PQP_PARAM_ITEM_NUM_MAX 0xfffff

#define PQP_PARAM_ITEM_ID_LEN 2
#define PQP_PARAM_ITEM_INDEX_MASK 0x1fff

#define PQP_PARAM_DATA_BUF_SIZE_BYTE_1 1
#define PQP_PARAM_DATA_BUF_SIZE_BYTE_2 2
#define PQP_PARAM_DATA_BUF_SIZE_BYTE_4 4

#define PQP_PARAM_SHIFT_BITS_8 8
#define PQP_PARAM_SHIFT_BITS_16 16
#define PQP_PARAM_SHIFT_BITS_24 24

#define PQP_PARAM_ITEM_FLAG_MASK 0xe000
#define PQP_PARAM_ITEM_FLAG_1_BYTE 0x2000
#define PQP_PARAM_ITEM_FLAG_2_BYTE 0x4000
#define PQP_PARAM_ITEM_FLAG_4_BYTE 0x6000
#define PQP_PARAM_ITEM_FLAG_256_BYTE 0x8000
#define PQP_PARAM_ITEM_FLAG_64_KB 0xa000
#define PQP_PARAM_ITEM_FLAG_4_GB 0xc000

#define PQP_PARAM_CHN_IDX 0
#define PQP_PARAM_HEIGHT_IDX 1
#define PQP_PARAM_WIDTH_IDX 2

/*
 * magic4 + crc324 + paramSize4 + paramSize4
 * RSV    + RSV    + RSV        + RSV
 */
#define PQP_PARAM_HEADER_SIZE 32

#define PQP_PARAM_ITEM_SDK_ARCH 0x4001
#define PQP_PARAM_ITEM_SECT_NUM 0x2009
#define PQP_PARAM_ITEM_BATCH_NUM 0x600A
#define PQP_PARAM_ITEM_SECT 0xA040
#define PQP_PARAM_ITEM_SECT_ID 0x2041
#define PQP_PARAM_ITEM_SECT_TYPE 0x2042
#define PQP_PARAM_ITEM_INST_OFFSET 0x6043
#define PQP_PARAM_ITEM_INST_SIZE 0x6044
#define PQP_PARAM_ITEM_INST_NUM 0x6045
#define PQP_PARAM_ITEM_INPUT_NUM 0x2046
#define PQP_PARAM_ITEM_OUTPUT_NUM 0x2047
#define PQP_PARAM_ITEM_INPUTS 0xA050
#define PQP_PARAM_ITEM_INPUT_ID 0x4051
#define PQP_PARAM_ITEM_INPUT_HEIGHT 0x6052
#define PQP_PARAM_ITEM_INPUT_WIDTH 0x6053
#define PQP_PARAM_ITEM_INPUT_CHANNEL 0x6054
#define PQP_PARAM_ITEM_INPUT_DIM_TYPE 0x2055
#define PQP_PARAM_ITEM_INPUT_FORMAT 0x2056
#define PQP_PARAM_ITEM_RGB_ORDER 0x2057
#define PQP_PARAM_ITEM_INPUT_NAME 0x8058
#define PQP_PARAM_ITEM_INPUT_DATA_TYPE 0x2059
#define PQP_PARAM_ITEM_OUTPUTS 0x8060
#define PQP_PARAM_ITEM_OUTPUT_ID 0x4061
#define PQP_PARAM_ITEM_OUTPUT_HEIGHT 0x6062
#define PQP_PARAM_ITEM_OUTPUT_WIDTH 0x6063
#define PQP_PARAM_ITEM_OUTPUT_CHANNEL 0x6064
#define PQP_PARAM_ITEM_OUTPUT_DIM_TYPE 0x2065
#define PQP_PARAM_ITEM_OUTPUT_NAME 0x8066
#define PQP_PARAM_ITEM_OUTPUT_DATA_FORMAT 0x2067
#define PQP_PARAM_ITEM_OUTPUT_DATA_TYPE 0x2068

typedef enum {
    PQP_PARAM_DATA_NONE = 0x0,
    PQP_PARAM_DATA_IMG_BGR_PLANAR = 0x1,
    PQP_PARAM_DATA_IMG_YUV420SP = 0x2,
    PQP_PARAM_DATA_IMG_YVU420SP = 0x3,
    PQP_PARAM_DATA_IMG_YUV422SP = 0x4,
    PQP_PARAM_DATA_IMG_YVU422SP = 0x5,
    PQP_PARAM_DATA_IMG_YUV400 = 0x6,
    PQP_PARAM_DATA_IMG_RGB_PLANAR = 0x7,
    PQP_PARAM_DATA_IMG_RGB_PACKAGE = 0x8,
    PQP_PARAM_DATA_IMG_BGR_PACKAGE = 0x9,
    PQP_PARAM_DATA_IMG_XRGB_PLANAR = 0xa,
    PQP_PARAM_DATA_IMG_XBGR_PLANAR = 0xb,
    PQP_PARAM_DATA_IMG_RGBX_PLANAR = 0xc,
    PQP_PARAM_DATA_IMG_BGRX_PLANAR = 0xd,
    PQP_PARAM_DATA_IMG_XRGB_PACKAGE = 0xe,
    PQP_PARAM_DATA_IMG_XBGR_PACKAGE = 0xf,
    PQP_PARAM_DATA_IMG_RGBX_PACKAGE = 0x10,
    PQP_PARAM_DATA_IMG_BGRX_PACKAGE = 0x11,
    PQP_PARAM_DATA_IMG_RAW_RGGB = 0x12,
    PQP_PARAM_DATA_IMG_RAW_GRBG = 0x13,
    PQP_PARAM_DATA_IMG_RAW_GBRG = 0x14,
    PQP_PARAM_DATA_IMG_RAW_BGGR = 0x15,
    PQP_PARAM_DATA_INPUT_DECODEBBOX_ANCHORS = 0x20,
    PQP_PARAM_DATA_INPUT_DECODEBBOX_SCORE = 0x21,
    PQP_PARAM_DATA_INPUT_DECODEBBOX_ANCHORS_SCORE = 0x22,
    PQP_PARAM_DATA_INPUT_FILTER_SORT_NMS_ANCHORS_SCORE_ID = 0x23,
    PQP_PARAM_DATA_INPUT_FILTER_SORT_NMS_ANCHORS_SCORE = 0x24,
    PQP_PARAM_DATA_OUTPUT_NORMAL = 0x30,
    PQP_PARAM_DATA_OUTPUT_RECURRENT_T = 0x31,
    PQP_PARAM_DATA_OUTPUT_DETECTION_OUT = 0x32,
    PQP_PARAM_DATA_FORMAT_BUTT
} pqp_param_data_format;

typedef enum {
    PQP_PARAM_DTYPE_U8 = 0x0,
    PQP_PARAM_DTYPE_U10 = 0x1,
    PQP_PARAM_DTYPE_U12 = 0x2,
    PQP_PARAM_DTYPE_U14 = 0x3,
    PQP_PARAM_DTYPE_U16 = 0x4,
    PQP_PARAM_DTYPE_U2I = 0x5,
    PQP_PARAM_DTYPE_S2 = 0x6,
    PQP_PARAM_DTYPE_U2 = 0x7,
    PQP_PARAM_DTYPE_U4I = 0x8,
    PQP_PARAM_DTYPE_S4 = 0x9,
    PQP_PARAM_DTYPE_U4 = 0xA,
    PQP_PARAM_DTYPE_S8 = 0xB,
    PQP_PARAM_DTYPE_S16 = 0xC,
    PQP_PARAM_DTYPE_F16 = 0xD,
    PQP_PARAM_DTYPE_S32 = 0xE,
    PQP_PARAM_DTYPE_U32 = 0xF,
    PQP_PARAM_DTYPE_F32 = 0x10,
    PQP_PARAM_DTYPE_S64 = 0x11,
    PQP_PARAM_DTYPE_U64 = 0x12,
    PQP_PARAM_DTYPE_BUTT
} pqp_param_data_type;

typedef enum {
    PQP_PARAM_ORDER_IGNORE = 0x0,
    PQP_PARAM_ORDER_RGB = 0x1,
    PQP_PARAM_ORDER_BGR = 0x2,
    PQP_PARAM_ORDER_XRGB = 0x3,
    PQP_PARAM_ORDER_XBGR = 0x4,
    PQP_PARAM_ORDER_RGBX = 0x5,
    PQP_PARAM_ORDER_BGRX = 0x6,
    PQP_PARAM_ORDER_RGGB = 0x7,
    PQP_PARAM_ORDER_GRBG = 0x8,
    PQP_PARAM_ORDER_GBRG = 0x9,
    PQP_PARAM_ORDER_BGGR = 0xA,
    PQP_PARAM_ORDER_BUTT
} pqp_param_order;

typedef enum {
    PQP_PARAM_DIM_SCALAR = 0x0,
    PQP_PARAM_DIM_VECTOR,
    PQP_PARAM_DIM_MATRIX,
    PQP_PARAM_DIM_TENSOR,
    PQP_PARAM_DIM_BUTT
} pqp_param_dim_type;

typedef struct {
    td_u32 magic;
    td_u32 crc;
    td_u32 total_size;
    td_u32 param_size;
    td_u32 param_offset;
    td_u32 tmp_buf_size;
    td_u32 rsv1;
    td_u32 rsv2;
} pqp_param_header;

typedef struct {
    const td_u8 *buf;
    td_phys_addr_t phys_addr;
    td_ulong size;
} pqp_param_data_buffer;

typedef struct {
    td_u32 id;
    td_u32 shape[PQP_PARAM_MAX_SHAPE_NUM];
    pqp_param_dim_type dim_type;
    pqp_param_order rgb_order;
    pqp_param_data_type data_type;
    pqp_param_data_format format;
    td_char layer_name[PQP_PARAM_MAX_NAME_LEN];
} pqp_param_inout_info;

typedef struct {
    td_u16 id;
    td_u32 size;
    td_u32 value;
    pqp_param_data_buffer *buf;
} pqp_param_item;

typedef struct {
    td_u32 id;
    td_u32 type;
    td_u32 param_offset;
    td_u32 param_size;
    td_u32 param_num;
    td_u32 input_num;
    td_u32 output_num;
    pqp_param_inout_info *input_info;
    pqp_param_inout_info *output_info;
} pqp_param_sect_info;

typedef struct {
    td_u32 sdk_arch;
    td_u32 total_size;
    pqp_param_data_buffer param;
    pqp_param_sect_info *sect_info;
    td_u32 batch_num;
    td_u32 sect_num;
    td_u32 tmp_buf_size;
} pqp_param_info;

typedef struct {
    const td_u8 *param_data_ptr;
    td_phys_addr_t param_data_phys_addr;
    td_u32 param_buf_size;
    td_u32 offset;

    pqp_param_sect_info *sect_info_ptr;
    pqp_param_inout_info *input_info_ptr;
    pqp_param_inout_info *output_info_ptr;

    td_bool is_get_param_info;

    td_u32 sect_info_num;
    td_u32 input_info_num;
    td_u32 output_info_num;

    td_u32 sect_info_idx;
    td_u32 input_info_idx;
    td_u32 output_info_idx;
} pqp_param_dynamic_info;

typedef td_u32 (*pqp_read_param_info_func)(pqp_param_dynamic_info *dynamic_info,
    pqp_param_info *param_info, td_u16 id);

typedef struct {
    td_u16 id;
    pqp_read_param_info_func func;
} pqp_param_data_handle;

typedef struct {
    struct list_head head;
    pqp_mem_info buf;
    td_u32 node_id;
    td_u32 ref_num;
} pqp_param_info_node;

typedef struct {
    td_phys_addr_t phys_addr;
    td_void *virt_addr;
    struct list_head free_param_info_list;
    struct list_head busy_param_info_list;
    td_u32 free_param_info_num;
    td_u32 busy_param_info_num;
    td_u32 total_param_info_num;
    td_u32 ref_num;
} pqp_param_info_list;

typedef struct {
    struct list_head head;
    td_void *virt_addr;
    td_ulong node_id;
} pqp_check_info_node;
typedef struct {
    td_phys_addr_t phys_addr;
    td_void *virt_addr;
    struct list_head free_check_info_list;
    td_u32 free_check_info_num;
    td_u32 total_check_info_num;
} pqp_check_info_list;

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* PQP_PARAM_COMMON_H */
