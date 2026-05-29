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
#ifndef HPT_LIST_H
#define HPT_LIST_H

#include <stdint.h>

#ifndef HPT_INLINE
#define HPT_INLINE __inline
#endif

typedef uintptr_t HPT_UPTR;

struct CbbListHead {
    struct CbbListHead *next, *prev;
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

static HPT_INLINE void CbbListAddHelper(struct CbbListHead *newHead,
    struct CbbListHead *prev, struct CbbListHead *next)
{
    if (prev == NULL || newHead == NULL || next == NULL) {
        return;
    }

    next->prev = newHead;
    newHead->next = next;
    newHead->prev = prev;
    prev->next = newHead;
}

static HPT_INLINE void CbbListAdd(struct CbbListHead *newHead, struct CbbListHead *head)
{
    if (newHead == NULL || head == NULL) {
        return;
    }

    CbbListAddHelper(newHead, head, head->next);
}

static HPT_INLINE void CbbListAddTail(struct CbbListHead *newHead, struct CbbListHead *head)
{
    if (newHead == NULL || head == NULL) {
        return;
    }

    CbbListAddHelper(newHead, head->prev, head);
}

static HPT_INLINE void CbbListDelHelper(struct CbbListHead *prev, struct CbbListHead *next)
{
    if (prev == NULL || next == NULL) {
        return;
    }

    next->prev = prev;
    prev->next = next;
}

static HPT_INLINE void CbbListDel(struct CbbListHead *entry)
{
    if (entry == NULL) {
        return;
    }

    CbbListDelHelper(entry->prev, entry->next);
}

static HPT_INLINE void CbbListDelInit(struct CbbListHead *entry)
{
    CbbListDelHelper(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}

static HPT_INLINE int CbbListEmpty(struct CbbListHead *head)
{
    return head->next == head;
}

static HPT_INLINE void CbbListSpliceHelper(struct CbbListHead *list,
    struct CbbListHead *head)
{
    struct CbbListHead *first = list->next;
    struct CbbListHead *last = list->prev;
    struct CbbListHead *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

static HPT_INLINE void CbbListSplice(struct CbbListHead *list, struct CbbListHead *head)
{
    if (CbbListEmpty(list) == 0) {
        CbbListSpliceHelper(list, head);
    }
}

static HPT_INLINE void CbbListSpliceInit(struct CbbListHead *list, struct CbbListHead *head)
{
    if (CbbListEmpty(list) == 0) {
        CbbListSpliceHelper(list, head);
        INIT_LIST_HEAD(list);
    }
}

#define CBB_LIST_ENTRY(ptr, type, member) \
    ((type *)((uintptr_t)(ptr) - ((uintptr_t)(&((type *)0)->member))))

#define CBB_LIST_FOR_EACH(pos, head) \
    for ((pos) = (head)->next; (((pos) != (head)) && ((pos) != NULL)); \
        (pos) = (pos)->next)

#define CBB_LIST_FOR_EACH_SAFE(pos, n, head) \
    for ((pos) = (head)->next, (n) = (pos)->next; (pos) != (head); \
        (pos) = (n), (n) = (pos)->next)

#define CBB_GET_FIRST_ITEM(attached, type, member) \
    ((type *)((char *)((attached)->next)-(HPT_UPTR)(&((type *)0)->member)))

#define CBB_LIST_FOR_EACH_ENTRY_SAFE(pos, n, head, member) \
    for ((pos) = CBB_LIST_ENTRY(((head)->next), typeof(*(pos)), (member)), \
        (n) = CBB_LIST_ENTRY(((pos)->member.next), typeof(*(pos)), (member)); \
        &(pos)->member != (head); \
        (pos) = (n), (n) = CBB_LIST_ENTRY(((n)->member.next), typeof(*(n)), (member)))


#endif
