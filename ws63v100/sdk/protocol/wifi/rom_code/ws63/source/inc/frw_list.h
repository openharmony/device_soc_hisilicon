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
 * Description: wifi dmac os adapt layer
 * Create: 2021-08-05
 */

#ifndef __FRW_LIST_H__
#define __FRW_LIST_H__

#include "frw_list_rom.h"

/* 从双向链表头部删除一个节点,不判断链表是否为空，不负责释放内存 请注意 */
static inline struct osal_list_head *osal_dlist_delete_head(const struct osal_list_head *head)
{
    struct osal_list_head *node;

    node = head->next;
    if (node == OSAL_NULL || node == head) {
        return NULL;
    }

    osal_dlist_delete_entry(node);

    return node;
}

/* 从双向链表头部删除一个节点,不判断链表是否为空，不负责释放内存 请注意 */
static inline struct osal_list_head *osal_dlist_delete_tail(const struct osal_list_head *head)
{
    struct osal_list_head *node;

    node = head->prev;
    if (node == OSAL_NULL || node == head) {
        return NULL;
    }

    osal_dlist_delete_entry(node);

    return node;
}

#endif // endif __FRW_LIST_ROM_H__

