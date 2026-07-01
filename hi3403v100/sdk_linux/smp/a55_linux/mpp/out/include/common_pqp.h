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

#ifndef COMMON_PQP_H
#define COMMON_PQP_H

#include "ot_common.h"
#include "ot_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef td_s32 pqp_param_handle;
typedef td_s32 pqp_task_handle;

typedef enum {
    PQP_ERR_SYS_TIMEOUT = 0x40,   /* PQP process timeout */
    PQP_ERR_QUERY_TIMEOUT = 0x41, /* PQP query timeout */
    PQP_ERR_RUNNING_ERR = 0x42,   /* PQP running error */

    PQP_ERR_BUTT
} pqp_err_code;

#define ERR_PQP_INVALID_DEV_ID     OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_INVALID_DEV_ID)
#define ERR_PQP_INVALID_PIPE_ID    OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_INVALID_PIPE_ID)
#define ERR_PQP_INVALID_CHN_ID     OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_INVALID_CHN_ID)
#define ERR_PQP_INVALID_LAYER_ID   OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_INVALID_LAYER_ID)
#define ERR_PQP_ILLEGAL_PARAM      OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_ILLEGAL_PARAM)
#define ERR_PQP_EXIST              OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_EXIST)
#define ERR_PQP_UNEXIST            OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_UNEXIST)
#define ERR_PQP_NULL_PTR           OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_NULL_PTR)
#define ERR_PQP_NOT_CFG            OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_CFG)
#define ERR_PQP_NOT_SUPPORT        OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_SUPPORT)
#define ERR_PQP_NOT_PERM           OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_PERM)

#define ERR_PQP_NOT_ENABLE         OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_ENABLE)
#define ERR_PQP_NOT_DISABLE        OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_DISABLE)
#define ERR_PQP_NOT_START          OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_START)
#define ERR_PQP_NOT_STOP           OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_STOP)
#define ERR_PQP_NO_MEM             OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_NO_MEM)
#define ERR_PQP_NO_BUF             OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_NO_BUF)
#define ERR_PQP_BUF_EMPTY          OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_BUF_EMPTY)
#define ERR_PQP_BUF_FULL           OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_BUF_FULL)
#define ERR_PQP_NOT_READY          OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_READY)

#define ERR_PQP_TIMEOUT            OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_TIMEOUT)
#define ERR_PQP_BAD_ADDR           OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_BAD_ADDR)
#define ERR_PQP_BUSY               OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_BUSY)
#define ERR_PQP_SIZE_NOT_ENOUGH    OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_SIZE_NOT_ENOUGH)
#define ERR_PQP_NOT_BINDED         OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_BINDED)
#define ERR_PQP_BINDED             OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, OT_ERR_BINDED)
#define ERR_PQP_SYS_TIMEOUT        OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, PQP_ERR_SYS_TIMEOUT)
#define ERR_PQP_QUERY_TIMEOUT      OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, PQP_ERR_QUERY_TIMEOUT)
#define ERR_PQP_RUNNING_ERR        OT_DEFINE_ERR(OT_ID_PQP, OT_ERR_LEVEL_ERROR, PQP_ERR_RUNNING_ERR)

#define PQP_MAX_DIM_NUM     4
#define PQP_MAX_INPUT_NUM   16
#define PQP_MAX_OUTPUT_NUM  16

typedef enum {
    PQP_DATA_U2 = 0x0,
    PQP_DATA_S2 = 0x1,
    PQP_DATA_U4 = 0x2,
    PQP_DATA_S4 = 0x3,
    PQP_DATA_U8 = 0x4,
    PQP_DATA_S8 = 0x5,
    PQP_DATA_U10 = 0x6,
    PQP_DATA_S10 = 0x7,
    PQP_DATA_U12 = 0x8,
    PQP_DATA_S12 = 0x9,
    PQP_DATA_U14 = 0xA,
    PQP_DATA_S14 = 0xB,
    PQP_DATA_U16 = 0xC,
    PQP_DATA_S16 = 0xD,
    PQP_DATA_FP16 = 0xE,
    PQP_DATA_U32 = 0xF,
    PQP_DATA_S32 = 0x10,
    PQP_DATA_FP32 = 0x11,
    PQP_DATA_U64 = 0x12,
    PQP_DATA_S64 = 0x13,
    PQP_DATA_BUTT
} pqp_data_type;

typedef enum {
    PQP_LAYOUT_NORMAL = 0x0,
    PQP_LAYOUT_PLANAR = 0x1,
    PQP_LAYOUT_YVU420SP = 0x2,
    PQP_LAYOUT_YVU422SP = 0x3,
    PQP_LAYOUT_YUV400 = 0x4,
    PQP_LAYOUT_RAW = 0x5,
    PQP_LAYOUT_BUTT
} pqp_layout_type;

typedef enum {
    PQP_DIM_SCALAR = 0x0,
    PQP_DIM_VECTOR = 0x1,
    PQP_DIM_MATRIX = 0x2,
    PQP_DIM_TENSOR = 0x3,
    PQP_DIM_BUTT
} pqp_dim_type;

typedef struct {
    pqp_data_type data_type;
    pqp_layout_type layout_type;
    pqp_dim_type dim_type;
    td_u32 dim[PQP_MAX_DIM_NUM];
} pqp_data_info;

typedef struct {
    td_phys_addr_t phys_addr;
    td_void *virt_addr;
    td_u32 size;
} pqp_mem_info;

typedef struct {
    pqp_mem_info mem_info;
    td_u32 stride;
} pqp_task_data;

typedef struct {
    pqp_task_data input_data[PQP_MAX_INPUT_NUM];
    pqp_task_data output_data[PQP_MAX_OUTPUT_NUM];
    td_u32 input_num;
    td_u32 output_num;
} pqp_inout_data;

typedef enum {
    PQP_CORE_ID_0 = 0x0,
    PQP_CORE_ID_BUTT
} pqp_core_id;

typedef struct {
    pqp_mem_info ping_pong_buf;
    pqp_mem_info task_buf;
    td_bool is_secure; /* Whether the param memory is secure */
    td_bool is_instant; /* Whether to report an interrupt immediately after a task is complete */
    td_u32 batch_num;  /* batch num */
    td_u32 sect_idx;  /* section idx */
    pqp_core_id core_id; /* Executed core */
    td_bool bypass_en; /* whether a task is bypassed. */
} pqp_task_ctrl;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* COMMON_PQP_H */
