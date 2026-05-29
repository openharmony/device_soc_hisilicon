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

#ifndef IVE_LIST_H
#define IVE_LIST_H
#include "ot_common_ive.h"

#define ive_list_head_init(name) { &(name), &(name) }

#define ive_list_head(name) \
    ot_ive_list_head name = ive_list_head_init(name)

static __inline void ive_init_list_head(ot_ive_list_head *list)
{
    list->next = list;
    list->prev = list;
}

/*
 * Insert a new entry between two known consecutive entries.
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline void ive_list_add_(ot_ive_list_head *new_node, ot_ive_list_head *prev_node,
    ot_ive_list_head *next_node)
{
    next_node->prev = new_node;
    new_node->next = next_node;
    new_node->prev = prev_node;
    prev_node->next = new_node;
}

/*
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static __inline void ive_list_add(ot_ive_list_head *new_node, ot_ive_list_head *head)
{
    ive_list_add_(new_node, head, head->next);
}

/*
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static __inline void ive_list_add_tail(ot_ive_list_head *new_node, ot_ive_list_head *head)
{
    ive_list_add_(new_node, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline void ive_list_del_(ot_ive_list_head *prev_node, ot_ive_list_head *next_node)
{
    next_node->prev = prev_node;
    prev_node->next = next_node;
}

/*
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static __inline void ive_list_del_entry_(ot_ive_list_head *entry)
{
    ive_list_del_(entry->prev, entry->next);
}

#define IVE_LIST_POISON1 ((void *)0x00100100)
#define IVE_LIST_POISON2 ((void *)0x00200200)

static __inline void ive_list_del(ot_ive_list_head *entry)
{
    ive_list_del_(entry->prev, entry->next);
    entry->next = (ot_ive_list_head *)IVE_LIST_POISON1;
    entry->prev = (ot_ive_list_head *)IVE_LIST_POISON2;
}

/*
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static __inline int ive_list_empty(const ot_ive_list_head *head)
{
    return head->next == head;
}

#define ive_offset_of(type, member) ((int)(unsigned long)&((type *)0)->member)

#define ive_container_of(ptr, type, member) (type *)((char *)(ptr) - ive_offset_of(type, member))
/*
 * list_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the list_struct within the struct.
 */
#define ive_list_entry(ptr, type, member) \
    ive_container_of(ptr, type, member)

/*
 * list_for_each    -    iterate over a list
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:    the head for your list.
 */
#define ive_list_for_each(pos, head) \
    for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)

/*
 * __list_for_each    -    iterate over a list
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:    the head for your list.
 *
 * This variant doesn't differ from list_for_each() any more.
 * We don't do prefetching in either case.
 */
#define ive__list_for_each(pos, head) \
    for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)

/*
 * list_for_each_prev    -    iterate over a list backwards
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:    the head for your list.
 */
#define ive_list_for_each_prev(pos, head) \
    for ((pos) = (head)->prev; (pos) != (head); (pos) = (pos)->prev)

/*
 * list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:    the &struct list_head to use as a loop cursor.
 * @n:        another &struct list_head to use as temporary storage
 * @head:    the head for your list.
 */
#define ive_list_for_each_safe(pos, n, head)                     \
    for ((pos) = (head)->next, (n) = (pos)->next; (pos) != (head); \
         (pos) = (n), (n) = (pos)->next)

#endif
