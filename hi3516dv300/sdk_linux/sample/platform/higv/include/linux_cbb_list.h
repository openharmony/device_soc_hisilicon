/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef LINUX_ CBB_LIST_H
#define LINUX_ CBB_LIST_H

#include <stdint.h>

#ifndef HPT_INLINE
#define HPT_INLINE __inline
#endif

typedef uintptr_t HPT_UPTR;

struct cbb_list_head {
    struct cbb_list_head *next, *prev;
};

#ifndef LIST_HEAD_INIT
#define LIST_HEAD_INIT(name) { &(name), &(name) }
#endif

#ifndef LIST_HEAD
#define LIST_HEAD(name) \
    (struct list_head name = LIST_HEAD_INIT(name))
#endif

#ifndef INIT_LIST_HEAD
#define INIT_LIST_HEAD(ptr) do { (ptr)->next = (ptr); (ptr)->prev = (ptr); } while (0)
#endif

static HPT_INLINE void inline_cbb_list_add(struct cbb_list_head *_new,
    struct cbb_list_head *prev, struct cbb_list_head *next)
{
    if (prev == NULL || _new == NULL || next == NULL) {
        return;
    }

    next->prev = _new;
    _new->next = next;
    _new->prev = prev;
    prev->next = _new;
}

static HPT_INLINE void cbb_list_add(struct cbb_list_head *_new, struct cbb_list_head *head)
{
    if (_new == NULL || head == NULL) {
        return;
    }

    inline_cbb_list_add(_new, head, head->next);
}

static HPT_INLINE void cbb_list_add_tail(struct cbb_list_head *_new, struct cbb_list_head *head)
{
    if (_new == NULL || head == NULL) {
        return;
    }

    inline_cbb_list_add(_new, head->prev, head);
}

static HPT_INLINE void inline_cbb_list_del(struct cbb_list_head *prev, struct cbb_list_head *next)
{
    if (prev == NULL || next == NULL) {
        return;
    }

    next->prev = prev;
    prev->next = next;
}

static HPT_INLINE void cbb_list_del(struct cbb_list_head *entry)
{
    if (entry == NULL) {
        return;
    }

    inline_cbb_list_del(entry->prev, entry->next);
}

static HPT_INLINE void cbb_list_del_init(struct cbb_list_head *entry)
{
    inline_cbb_list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}

static HPT_INLINE int cbb_list_empty(const struct cbb_list_head *head)
{
    return head->next == head;
}

static HPT_INLINE void inline_cbb_list_splice(struct cbb_list_head *list,
    struct cbb_list_head *head)
{
    struct cbb_list_head *first = list->next;
    struct cbb_list_head *last = list->prev;
    struct cbb_list_head *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

static HPT_INLINE void cbb_list_splice(struct cbb_list_head *list, struct cbb_list_head *head)
{
    if (!(cbb_list_empty(list) == true)) {
        inline_cbb_list_splice(list, head);
    }
}

static HPT_INLINE void cbb_list_splice_init(struct cbb_list_head *list, struct cbb_list_head *head)
{
    if (!(cbb_list_empty(list) == true)) {
        inline_cbb_list_splice(list, head);
        INIT_LIST_HEAD(list);
    }
}

#define cbb_list_entry(ptr, type, member) \
    ((type *)((uintptr_t)(ptr)-((uintptr_t)(&((type *)0)->member))))

#define cbb_list_for_each(pos, head) \
    for ((pos) = (head)->next; (((pos) != (head)) && ((pos) != NULL)); \
        (pos) = (pos)->next)

#define cbb_list_for_each_safe(pos, n, head) \
    for ((pos) = (head)->next, (n) = (pos)->next; (pos) != (head); \
        (pos) = (n), (n) = (pos)->next)

#define cbb_get_first_item(attached, type, member) \
    ((type *)((char *)((attached)->next)-(HPT_UPTR)(&((type *)0)->member)))

#define cbb_list_for_each_entry_safe(pos, n, head, member) \
    do {                                                                    \
        for ((pos) = cbb_list_entry((head)->next, typeof(*(pos)), member),    \
            (n) = cbb_list_entry((pos)->member.next, typeof(*(pos)), member); \
            &(pos)->member != (head);                                       \
            (pos) = (n), (n) = cbb_list_entry((n)->member.next, typeof(*(n)), member)) \
    } while (0)

#endif
