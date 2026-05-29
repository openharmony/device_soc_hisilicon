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

#ifndef SAMPLE_IVE_QUEUE_H
#define SAMPLE_IVE_QUEUE_H

#include "ot_type.h"
#include "ot_common_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct tag_ot_sample_ive_node {
    ot_video_frame_info frame_info;
    struct tag_ot_sample_ive_node *next;
} ot_sample_ive_node;

typedef struct {
    ot_sample_ive_node *front, *rear;
} ot_sample_ive_queue;

#define QUEUE_CORE_ERROR_BASE           1
#define QUEUE_CORE_FRAMEWORK_ERROR_BASE (QUEUE_CORE_ERROR_BASE + 10000)

#define QUEUE_NULL_POINTER  (QUEUE_CORE_FRAMEWORK_ERROR_BASE + 1)
#define QUEUE_ILLEGAL_STATE (QUEUE_CORE_FRAMEWORK_ERROR_BASE + 2)
#define QUEUE_OUT_OF_MEMORY (QUEUE_CORE_FRAMEWORK_ERROR_BASE + 3)

ot_sample_ive_queue *sample_ive_create_queue(td_s32 len);
td_void sample_ive_destory_queue(ot_sample_ive_queue **queue_head);
td_void sample_ive_clear_queue(ot_sample_ive_queue *queue_head);
td_bool sample_ive_is_queue_empty(ot_sample_ive_queue *queue_head);
td_s32 sample_ive_queue_size(ot_sample_ive_queue *queue_head);
td_s32 sample_ive_add_queue_node(ot_sample_ive_queue *queue_head, ot_video_frame_info *added_frm_info);
ot_sample_ive_node *sample_ive_get_queue_head_node(const ot_sample_ive_queue *queue_head);
ot_sample_ive_node *sample_ive_get_queue_node(ot_sample_ive_queue *queue_head);
td_void sample_ive_free_queue_node(ot_sample_ive_node **free_node);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* SAMPLE_IVE_QUEUE_H */

