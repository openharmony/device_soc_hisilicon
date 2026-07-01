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
#ifndef IVE_QUEUE_H
#define IVE_QUEUE_H

#include "ot_common_ive.h"
#include "ive_list.h"

td_s32 ive_create_obj_list(const ot_svp_mem_info *mem, ot_ive_kcf_obj_list *obj_list, td_u32 max_num);
td_void ive_destroy_obj_list(ot_ive_kcf_obj_list *obj_list);
ot_ive_kcf_obj_node *ive_obj_list_get_free(ot_ive_kcf_obj_list *obj_list);
td_void ive_obj_list_put_free(ot_ive_kcf_obj_list *obj_list, ot_ive_kcf_obj_node *obj_node);
td_u32 ive_obj_list_get_free_num(const ot_ive_kcf_obj_list *obj_list);
td_u32 ive_obj_list_get_train_num(const ot_ive_kcf_obj_list *obj_list);
td_u32 ive_obj_list_get_track_num(const ot_ive_kcf_obj_list *obj_list);
td_void ive_obj_list_put_track(ot_ive_kcf_obj_list *obj_list, ot_ive_kcf_obj_node *obj_node);
td_void ive_obj_list_put_train(ot_ive_kcf_obj_list *obj_list, ot_ive_kcf_obj_node *obj_node);
ot_ive_kcf_obj_node *ive_obj_list_get_train(ot_ive_kcf_obj_list *obj_list);
ot_ive_kcf_obj_node *ive_obj_list_query_train(const ot_ive_kcf_obj_list *obj_list, const ot_ive_list_head *next);
ot_ive_kcf_obj_node *ive_obj_list_get_track(ot_ive_kcf_obj_list *obj_list);
ot_ive_kcf_obj_node *ive_obj_list_query_track(const ot_ive_kcf_obj_list *obj_list, const ot_ive_list_head *next);
td_s32 ive_obj_free_track_node_by_node(ot_ive_kcf_obj_list *obj_list, const ot_ive_kcf_obj_node *obj_node);

#endif
