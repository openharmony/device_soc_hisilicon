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

#ifndef OT_COMMON_AIV_H
#define OT_COMMON_AIV_H

#include "ot_common_video.h"
#include "ot_common_vb.h"
#include "ot_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* GENERAL STRUCTURES */
#define OT_AIV_MAX_MODEL_NUM       16
#define OT_AIV_MAX_DEPTH           64

#define OT_ERR_AIV_INVALID_PIPE_ID OT_DEFINE_ERR(OT_ID_AIV, OT_ERR_LEVEL_ERROR, OT_ERR_INVALID_PIPE_ID)
#define OT_ERR_AIV_ILLEGAL_PARAM   OT_DEFINE_ERR(OT_ID_AIV, OT_ERR_LEVEL_ERROR, OT_ERR_ILLEGAL_PARAM)
#define OT_ERR_AIV_NULL_PTR        OT_DEFINE_ERR(OT_ID_AIV, OT_ERR_LEVEL_ERROR, OT_ERR_NULL_PTR)
#define OT_ERR_AIV_NOT_SUPPORT     OT_DEFINE_ERR(OT_ID_AIV, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_SUPPORT)
#define OT_ERR_AIV_NOT_PERM        OT_DEFINE_ERR(OT_ID_AIV, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_PERM)
#define OT_ERR_AIV_NO_MEM          OT_DEFINE_ERR(OT_ID_AIV, OT_ERR_LEVEL_ERROR, OT_ERR_NO_MEM)
#define OT_ERR_AIV_NOT_READY       OT_DEFINE_ERR(OT_ID_AIV, OT_ERR_LEVEL_ERROR, OT_ERR_NOT_READY)

typedef enum {
    OT_AIV_TYPE_AIDEHAZE = 1,
    OT_AIV_TYPE_AIVDEC,
    OT_AIV_TYPE_AISR,
    OT_AIV_TYPE_BUTT
} ot_aiv_type;

typedef struct {
    td_phys_addr_t phys_addr; /* RW; The physical address of the memory */
    td_void *virt_addr;       /* RW; The virtual address of the memory */
    td_u32 size;              /* RW; The size of memory */
} ot_aiv_mem_info;

typedef struct {
    ot_aiv_mem_info mem_info;
    ot_size image_size;
} ot_aiv_model;

typedef struct {
    td_u32 cpu_id; /* RW; pthread of aiisp bind to cpu core, Range:[0, 1]; Format:1.0 */
} ot_aiv_thread_attr;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
