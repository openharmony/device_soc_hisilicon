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

#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

void ListInit(List *list, const DupFreeFuncPair *dataFunc)
{
    if (list == NULL) {
        return;
    }
    list->count = 0;
    list->head.next = &list->head;
    list->head.prev = &list->head;
    if (dataFunc != NULL) {
        list->dataFunc.freeFunc = dataFunc->freeFunc;
        list->dataFunc.dupFunc = dataFunc->dupFunc;
    } else {
        list->dataFunc.freeFunc = NULL;
        list->dataFunc.dupFunc = NULL;
    }
}

bool ListEmpty(const List *list)
{
    if (list == NULL) {
        return false;
    }
    return list->head.next == &list->head;
}

size_t ListSize(const List *list)
{
    if (list == NULL) {
        return 0;
    }
    return list->count;
}

static inline uint32_t DupUserData(const DupFunc dupFunc, ListNode *entry, const uintptr_t userData)
{
    if (dupFunc == NULL) {
        entry->userdata = userData;
    } else {
        entry->userdata = (uintptr_t)(dupFunc(userData));
    }

    return LIST_OK;
}

static inline void RawListInsert(List *list, RawListNode *curNode, RawListNode *newNode)
{
    newNode->prev = curNode->prev;
    newNode->next = curNode;
    newNode->prev->next = newNode;
    newNode->next->prev = newNode;
    list->count++;
}

static ListNode* GetListEntry(DupFunc dupFunc, uintptr_t userData)
{
    ListNode *entry = (ListNode*)malloc(sizeof(ListNode));
    if (entry == NULL) {
        return NULL;
    }

    if (DupUserData(dupFunc, entry, userData) == LIST_ERROR) {
        free(entry);
        entry = NULL;
        return NULL;
    }
    return entry;
}

uint32_t ListPushFront(List *list, uintptr_t userData)
{
    if (list == NULL) {
        return LIST_ERROR;
    }

    ListNode *entry = GetListEntry(list->dataFunc.dupFunc, userData);
    if (entry == NULL) {
        return LIST_ERROR;
    }

    RawListInsert(list, list->head.next, &entry->rawNode);

    return LIST_OK;
}

uint32_t ListPushBack(List *list, uintptr_t userData)
{
    if (list == NULL) {
        return LIST_ERROR;
    }

    ListNode *entry = GetListEntry(list->dataFunc.dupFunc, userData);
    if (entry == NULL) {
        return LIST_ERROR;
    }

    RawListInsert(list, &list->head, &entry->rawNode);

    return LIST_OK;
}

uintptr_t ListFront(List *list)
{
    if (list == NULL || ListEmpty(list)) {
        return 0; // 无法区分是值为零，还是链表为空
    }
    ListNode *node = (ListNode*)list->head.next;

    return node->userdata;
}

uintptr_t ListBack(List *list)
{
    if (list == NULL || ListEmpty(list)) {
        return 0; // 无法区分是值为零，还是链表为空
    }
    ListNode *node = (ListNode*)list->head.prev;

    return node->userdata;
}

uintptr_t ListIterData(ListIterator it)
{
    if (it == NULL) {
        return 0;
    }
    return ((ListNode*)it)->userdata;
}

ListIterator ListIterErase(List *list, ListIterator it)
{
    if (it == NULL) {
        return NULL;
    }
    uintptr_t data = ListIterData(it);
    if (list->dataFunc.freeFunc != NULL) {
        list->dataFunc.freeFunc(data);
    }

    ListIterator tmp = it->next;
    it->prev->next = it->next;
    it->next->prev = it->prev;
    list->count--;
    free(it);

    return tmp;
}

void ListPopFront(List *list)
{
    if (list == NULL || ListEmpty(list)) {
        return;
    }

    (void)ListIterErase(list, list->head.next);
}

void ListPopBack(List *list)
{
    if (list == NULL || ListEmpty(list)) {
        return;
    }

    (void)ListIterErase(list, list->head.prev);
}

ListIterator ListIterBegin(List *list)
{
    if (list == NULL) {
        return NULL;
    }
    if (ListEmpty(list)) {
        return &list->head;
    }
    return list->head.next;
}

ListIterator ListIterEnd(List *list)
{
    if (list == NULL) {
        return NULL;
    }
    return &list->head;
}

ListIterator ListIterNext(const List *list, ListIterator it)
{
    if (list == NULL || it == NULL) {
        return NULL;
    }
    if (it == &list->head) {
        return it;
    }
    return it->next;
}

uint32_t ListInsert(List *list, ListIterator it, uintptr_t userData)
{
    if (list == NULL) {
        return LIST_ERROR;
    }

    ListNode *entry = GetListEntry(list->dataFunc.dupFunc, userData);
    if (entry == NULL) {
        return LIST_ERROR;
    }

    RawListInsert(list, it, &entry->rawNode);

    return LIST_OK;
}

void ListDeinit(List *list)
{
    if (list == NULL || ListEmpty(list)) {
        return;
    }

    for (ListIterator it = ListIterBegin(list); it != ListIterEnd(list); it = ListIterErase(list, it)) {}

    list->dataFunc.dupFunc = NULL;
    list->dataFunc.freeFunc = NULL;
}

#ifdef __cplusplus
}
#endif
