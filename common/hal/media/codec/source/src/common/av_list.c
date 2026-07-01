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

#include "av_list.h"
#include "codec_common.h"
#include "media_hal_common.h"
#include "securec.h"

#define MODULE_NAME "AVLIST"

typedef struct {
    OtListHead listPtr;
    void *priv;
} AvListNode;

td_s32 AvListCreate(AvListInstance **listHdr, td_u32 maxListNode)
{
    CHK_NULL_RETURN(listHdr, TD_FAILURE, "listHdr null");
    if (maxListNode == 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "AvListCreate maxListNode 0 error");
        return TD_FAILURE;
    }
    AvListInstance *listInstance = malloc(sizeof(AvListInstance));
    if (listInstance == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc  AvListInstance failed");
        return TD_FAILURE;
    }
    if (memset_s(listInstance, sizeof(AvListInstance), 0x00, sizeof(AvListInstance)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "set listInstance failed");
        free(listInstance);
        return TD_FAILURE;
    }
    OT_LIST_INIT_HEAD_PTR(&(listInstance->header));
    (void)pthread_mutex_init(&listInstance->listLock, NULL);
    listInstance->maxNodeNum = maxListNode;
    *listHdr = listInstance;
    return TD_SUCCESS;
}

static void AvListDelAllNode(AvListInstance *listInstance)
{
    CHK_NULL_RETURN_NONE(listInstance, "listInstance null");

    OtListHead *pos = NULL;
    OtListHead *n = NULL;
    AvListNode *listNode = NULL;

    MEDIA_HAL_LOCK(listInstance->listLock);

    OT_LIST_FOR_EACH_SAFE(pos, n, &listInstance->header)
    {
        listNode = OT_LIST_ENTRY(pos, AvListNode, listPtr);
        OT_LIST_DEL(&listNode->listPtr);
        if (memset_s(listNode, sizeof(AvListNode), 0x00, sizeof(AvListNode)) != EOK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "set listNode failed");
        }
        free(listNode);
        listNode = NULL;
    }
    MEDIA_HAL_UNLOCK(listInstance->listLock);
}

td_s32 AvListDestroy(AvListInstance *listInstance)
{
    CHK_NULL_RETURN(listInstance, TD_FAILURE, "listInstance null");

    AvListDelAllNode(listInstance);
    pthread_mutex_destroy(&listInstance->listLock);

    free(listInstance);
    return TD_SUCCESS;
}

td_s32 AvListCount(AvListInstance *listInstance, td_u32 *count)
{
    CHK_NULL_RETURN(listInstance, TD_FAILURE, "listInstance null");
    CHK_NULL_RETURN(count, TD_FAILURE, "count null");

    OtListHead *pos = NULL;
    td_u32 num = 0;

    MEDIA_HAL_LOCK(listInstance->listLock);

    OT_LIST_FOR_EACH(pos, &listInstance->header)
    {
        num++;
    }

    MEDIA_HAL_UNLOCK(listInstance->listLock);
    *count = num;
    return TD_SUCCESS;
}

td_s32 AvListAddNode(AvListInstance *listInstance, void *priv)
{
    CHK_NULL_RETURN(listInstance, TD_FAILURE, "listInstance null");
    td_u32 nodeCnt = 0;
    if (AvListCount(listInstance, &nodeCnt) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if (nodeCnt >= listInstance->maxNodeNum) {
        MEDIA_HAL_LOGE(MODULE_NAME, "list node num reach max: %u failed", listInstance->maxNodeNum);
        return TD_FAILURE;
    }

    AvListNode *node = malloc(sizeof(AvListNode));
    CHK_NULL_RETURN(node, TD_FAILURE, "malloc AvListNode failed");

    if (memset_s(node, sizeof(AvListNode), 0x00, sizeof(AvListNode)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "set AvListNode failed");
        free(node);
        return TD_FAILURE;
    }
    node->priv = priv;

    MEDIA_HAL_LOCK(listInstance->listLock);
    OT_LIST_ADD_TAIL(&(node->listPtr), &(listInstance->header));
    MEDIA_HAL_UNLOCK(listInstance->listLock);
    return TD_SUCCESS;
}

td_s32 AvListDelNode(AvListInstance *listInstance, const void *priv)
{
    CHK_NULL_RETURN(listInstance, TD_FAILURE, "listInstance null");
    OtListHead *pos = NULL;
    AvListNode *listNode = NULL;
    td_bool isFound = TD_FALSE;

    MEDIA_HAL_LOCK(listInstance->listLock);

    OT_LIST_FOR_EACH(pos, &listInstance->header)
    {
        listNode = OT_LIST_ENTRY(pos, AvListNode, listPtr);
        if (listNode->priv == priv) {
            OT_LIST_DEL(&listNode->listPtr);
            free(listNode);
            isFound = TD_TRUE;
            break;
        }
    }

    if (isFound == TD_FALSE) {
        MEDIA_HAL_LOGE(MODULE_NAME, "input list node not exist");
        MEDIA_HAL_UNLOCK(listInstance->listLock);
        return TD_FAILURE;
    }

    MEDIA_HAL_UNLOCK(listInstance->listLock);
    return TD_SUCCESS;
}

td_bool AvListCheckExist(AvListInstance *listInstance, const void *priv)
{
    CHK_NULL_RETURN(listInstance, TD_FAILURE, "listInstance null");
    OtListHead *pos = NULL;
    AvListNode *listNode = NULL;

    MEDIA_HAL_LOCK(listInstance->listLock);

    OT_LIST_FOR_EACH(pos, &listInstance->header)
    {
        listNode = OT_LIST_ENTRY(pos, AvListNode, listPtr);
        if (listNode->priv == priv) {
            MEDIA_HAL_UNLOCK(listInstance->listLock);
            return TD_TRUE;
        }
    }
    MEDIA_HAL_UNLOCK(listInstance->listLock);
    return TD_FALSE;
}

void *AvListFindNodeWithCompare(AvListInstance *listInstance,
    AvListCompareCallback compareCbk, const void *condition)
{
    CHK_NULL_RETURN(listInstance, NULL, "listInstance null");
    CHK_NULL_RETURN(compareCbk, NULL, "compareCbk null");

    OtListHead *pos = NULL;
    AvListNode *listNode = NULL;

    MEDIA_HAL_LOCK(listInstance->listLock);

    OT_LIST_FOR_EACH(pos, &listInstance->header)
    {
        listNode = OT_LIST_ENTRY(pos, AvListNode, listPtr);
        if (compareCbk(listNode->priv, condition) == AV_COMPARE_EQUAL) {
            MEDIA_HAL_UNLOCK(listInstance->listLock);
            return listNode->priv;
        }
    }

    MEDIA_HAL_UNLOCK(listInstance->listLock);
    return NULL;
}

void *AvListGetNext(AvListInstance *listInstance, const void *priv)
{
    CHK_NULL_RETURN(listInstance, NULL, "listInstance null");

    OtListHead *pos = NULL;
    AvListNode *listNode = NULL;
    td_bool isFound = TD_FALSE;

    MEDIA_HAL_LOCK(listInstance->listLock);

    OT_LIST_FOR_EACH(pos, &listInstance->header)
    {
        listNode = OT_LIST_ENTRY(pos, AvListNode, listPtr);
        if (priv == NULL) {
            MEDIA_HAL_UNLOCK(listInstance->listLock);
            return listNode->priv;
        }

        if (isFound == TD_TRUE) {
            // return priv after one loop of found
            MEDIA_HAL_UNLOCK(listInstance->listLock);
            return listNode->priv;
        }

        if (listNode->priv == priv) {
            isFound = TD_TRUE;
        }
    }

    MEDIA_HAL_UNLOCK(listInstance->listLock);
    return TD_NULL;
}
