/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: hcc list completion.
 * Author:
 * Create: 2021-05-13
 */

#ifndef __HCC_LIST_H__
#define __HCC_LIST_H__

#include "td_type.h"
#include "soc_osal.h"
#include "osal_list.h"
#include "hcc_queue.h"
#include "hcc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_bool hcc_is_list_empty(hcc_data_queue *hcc_queue);
td_void hcc_list_head_deinit(hcc_data_queue *hcc_queue);
td_u32 hcc_list_head_init(hcc_data_queue *hcc_queue);
td_u32 hcc_list_len(TD_CONST hcc_data_queue *hcc_queue);
td_void hcc_list_add_tail(hcc_data_queue *hcc_queue, hcc_unc_struc *unc_buf);
td_void hcc_list_unlock_add_tail(hcc_data_queue *hcc_queue, hcc_unc_struc *unc_buf);
td_void hcc_list_add_head(hcc_data_queue* hcc_queue, hcc_unc_struc *unc_buf);
struct osal_list_head *hcc_list_peek(TD_CONST struct osal_list_head *head);
struct osal_list_head *hcc_list_delete_head(hcc_data_queue *TD_CONST hcc_queue);
hcc_unc_struc *hcc_list_dequeue(hcc_data_queue *TD_CONST hcc_queue);
hcc_unc_struc *hcc_list_unlock_dequeue(hcc_data_queue *TD_CONST hcc_queue);
td_void hcc_list_free(hcc_handler *hcc, hcc_data_queue *hcc_queue);
td_void hcc_list_restore(hcc_data_queue *dst_queue, hcc_data_queue *src_queue);
td_void _hcc_list_splice(TD_CONST struct osal_list_head *list, struct osal_list_head *prev,
    struct osal_list_head *next);
td_void hcc_list_splice_init(hcc_data_queue *hcc_queue, hcc_data_queue *head);
hcc_unc_struc *hcc_list_dequeue_tail(hcc_data_queue *TD_CONST hcc_queue);
struct osal_list_head *hcc_list_delete_tail(hcc_data_queue *TD_CONST hcc_queue);
td_void hcc_list_unlink(struct osal_list_head *node);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __HCC_LIST_H__ */
