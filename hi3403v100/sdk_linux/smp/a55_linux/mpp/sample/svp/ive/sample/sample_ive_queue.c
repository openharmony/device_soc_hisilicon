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
#include "sample_ive_queue.h"
#include <malloc.h>
#include "sample_common_ive.h"

static td_s32 g_max_queue_len = 0;
static td_s32 g_cur_queue_len = 0;

ot_sample_ive_queue *sample_ive_create_queue(td_s32 len)
{
    ot_sample_ive_queue *queue_head = TD_NULL;

    if ((len < -1) || (len == 0)) {
        return TD_NULL;
    }
    g_cur_queue_len = 0;
    queue_head = (ot_sample_ive_queue *)malloc(sizeof(ot_sample_ive_queue));
    sample_svp_check_exps_return(queue_head == TD_NULL, TD_NULL, SAMPLE_SVP_ERR_LEVEL_ERROR, "malloc failed!\n");

    queue_head->front = TD_NULL;
    queue_head->rear = TD_NULL;
    g_max_queue_len = len;

    return queue_head;
}

td_void sample_ive_destory_queue(ot_sample_ive_queue **queue_head)
{
    ot_sample_ive_node *queue_node_tmp = TD_NULL;
    if (queue_head == TD_NULL || (*queue_head) == TD_NULL) {
        return;
    }

    queue_node_tmp = (*queue_head)->front;
    while (queue_node_tmp != TD_NULL) {
        (*queue_head)->front = queue_node_tmp->next;
        free(queue_node_tmp);
        queue_node_tmp = (*queue_head)->front;
    }
    (*queue_head)->rear = (*queue_head)->front;
    g_cur_queue_len = 0;
    free(*queue_head);
    (*queue_head) = TD_NULL;
}


td_void sample_ive_clear_queue(ot_sample_ive_queue *queue_head)
{
    ot_sample_ive_node *queue_node_tmp = TD_NULL;

    if (queue_head == TD_NULL) {
        return;
    }

    queue_node_tmp = queue_head->front;
    while (queue_node_tmp != TD_NULL) {
        queue_head->front = queue_node_tmp->next;
        free(queue_node_tmp);
        queue_node_tmp = queue_head->front;
    }
    queue_head->rear = queue_head->front;
    g_cur_queue_len = 0;

    return;
}

td_bool sample_ive_is_queue_empty(ot_sample_ive_queue *queue_head)
{
    if (queue_head == TD_NULL) {
        return TD_TRUE;
    }

    if (queue_head->front != TD_NULL) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

td_s32 sample_ive_queue_size(ot_sample_ive_queue *queue_head)
{
    if (queue_head == TD_NULL) {
        return 0;
    }

    return g_cur_queue_len;
}

td_s32 sample_ive_add_queue_node(ot_sample_ive_queue *queue_head, ot_video_frame_info *added_frm_info)
{
    ot_sample_ive_node *queue_node = TD_NULL;
    td_s32 ret;

    if ((queue_head == TD_NULL) || (added_frm_info == TD_NULL)) {
        return QUEUE_NULL_POINTER;
    }

    if ((g_max_queue_len != -1) && (g_cur_queue_len >= g_max_queue_len)) {
        return QUEUE_ILLEGAL_STATE;
    }

    queue_node = (ot_sample_ive_node *)malloc(sizeof(ot_sample_ive_node));
    sample_svp_check_exps_return(queue_node == TD_NULL, QUEUE_OUT_OF_MEMORY,
        SAMPLE_SVP_ERR_LEVEL_ERROR, "malloc failed, out of memory!\n");

    ret = memcpy_s(&queue_node->frame_info, sizeof(ot_video_frame_info),
        added_frm_info, sizeof(ot_video_frame_info));
    if (ret != EOK) {
        free(queue_node);
        sample_svp_check_exps_return(1, OT_ERR_IVE_ILLEGAL_PARAM,
            SAMPLE_SVP_ERR_LEVEL_ERROR, "memcpy_s node failed!\n");
    }
    queue_node->next = TD_NULL;
    if (sample_ive_is_queue_empty(queue_head) != 0) {
        queue_head->front = queue_node;
        queue_head->rear = queue_node;
    } else {
        queue_head->rear->next = queue_node;
        queue_head->rear = queue_node;
    }

    g_cur_queue_len++;

    return TD_SUCCESS;
}

ot_sample_ive_node *sample_ive_get_queue_head_node(const ot_sample_ive_queue *queue_node)
{
    if ((queue_node == TD_NULL) || (queue_node->front == TD_NULL)) {
        return TD_NULL;
    }

    return queue_node->front;
}

ot_sample_ive_node *sample_ive_get_queue_node(ot_sample_ive_queue *queue_head)
{
    ot_sample_ive_node *queue_node = TD_NULL;

    if ((queue_head == TD_NULL) || (queue_head->front == TD_NULL)) {
        return TD_NULL;
    }

    queue_node = queue_head->front;
    queue_head->front = queue_node->next;
    if (queue_head->front == TD_NULL) {
        queue_head->rear = queue_head->front;
    }
    g_cur_queue_len--;

    return queue_node;
}

td_void sample_ive_free_queue_node(ot_sample_ive_node **free_node)
{
    if (free_node != TD_NULL && (*free_node) != TD_NULL) {
        free(*free_node);
        *free_node = TD_NULL;
    }

    return;
}
