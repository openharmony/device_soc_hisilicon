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

#ifndef AV_LIST_H
#define AV_LIST_H

#include <pthread.h>
#include "ot_mw_type.h"
#include "ot_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define AV_COMPARE_EQUAL     0
#define AV_COMPARE_NO_EQUAL  1

typedef struct {
    OtListHead header;
    pthread_mutex_t listLock;
    td_u32 maxNodeNum;
} AvListInstance;

typedef td_s32 (*AvListCompareCallback)(const void *priv, const void *condition);

td_s32 AvListCreate(AvListInstance **ppListHdr, td_u32 maxListNode);

td_s32 AvListDestroy(AvListInstance *listInst);

td_s32 AvListAddNode(AvListInstance *listInst, void *priv);

td_s32 AvListDelNode(AvListInstance *listInstance, const void *priv);

td_bool AvListCheckExist(AvListInstance *listInst, const void *priv);

void *AvListFindNodeWithCompare(AvListInstance *listInst,
    AvListCompareCallback compareCbk, const void *condition);

void *AvListGetNext(AvListInstance *listInstance, const void *priv);

td_s32 AvListCount(AvListInstance *listInstance, td_u32 *count);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* AV_LIST_H */
