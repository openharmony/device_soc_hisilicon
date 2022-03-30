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
#include <malloc.h>
#include "sample_comm_ive.h"
#include "sample_ive_queue.h"

static HI_S32 s_s32MaxQueuelen = 0;
static HI_S32 s_s32CurQueueLen = 0;

SAMPLE_IVE_QUEUE_S *SAMPLE_IVE_QueueCreate(HI_S32 s32Len)
{
    SAMPLE_IVE_QUEUE_S *pstQueueHead = NULL;

    if (s32Len < -1 || s32Len == 0) {
        return NULL;
    }
    s_s32CurQueueLen = 0;
    pstQueueHead = (SAMPLE_IVE_QUEUE_S *)malloc(sizeof(SAMPLE_IVE_QUEUE_S));
    if (pstQueueHead == NULL) {
        return NULL;
    }
    pstQueueHead->front = NULL;
    pstQueueHead->rear = NULL;
    s_s32MaxQueuelen = s32Len;

    return pstQueueHead;
}

HI_VOID SAMPLE_IVE_QueueDestory(SAMPLE_IVE_QUEUE_S *pstQueueHead)
{
    SAMPLE_IVE_NODE_S *pstQueueTmp = NULL;

    if (pstQueueHead == NULL) {
        return;
    }

    pstQueueTmp = pstQueueHead->front;
    while (pstQueueTmp != NULL) {
        pstQueueHead->front = pstQueueTmp->next;
        free(pstQueueTmp);
        pstQueueTmp = pstQueueHead->front;
    }
    pstQueueHead->rear = pstQueueHead->front;
    s_s32CurQueueLen = 0;
    free(pstQueueHead);
    pstQueueHead = NULL;

    return;
}

HI_VOID SAMPLE_IVE_QueueClear(SAMPLE_IVE_QUEUE_S *pstQueueHead)
{
    SAMPLE_IVE_NODE_S *pstQueueTmp = NULL;

    if (pstQueueHead == NULL) {
        return;
    }

    pstQueueTmp = pstQueueHead->front;
    while (pstQueueTmp != NULL) {
        pstQueueHead->front = pstQueueTmp->next;
        free(pstQueueTmp);
        pstQueueTmp = pstQueueHead->front;
    }
    pstQueueHead->rear = pstQueueHead->front;
    s_s32CurQueueLen = 0;

    return;
}

HI_BOOL SAMPLE_IVE_QueueIsEmpty(SAMPLE_IVE_QUEUE_S *pstQueueHead)
{
    if (pstQueueHead == NULL) {
        return HI_TRUE;
    }

    if (pstQueueHead->front != NULL) {
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_S32 SAMPLE_IVE_QueueSize(SAMPLE_IVE_QUEUE_S *pstQueueHead)
{
    if (pstQueueHead == NULL) {
        return 0;
    }

    return s_s32CurQueueLen;
}

HI_S32 SAMPLE_IVE_QueueAddNode(SAMPLE_IVE_QUEUE_S *pstQueueHead, VIDEO_FRAME_INFO_S *pstFrameInfo)
{
    SAMPLE_IVE_NODE_S *pstQueueNode = NULL;

    if ((pstQueueHead == NULL) || (pstFrameInfo == NULL)) {
        return QUEUE_NULL_POINTER;
    }

    if ((s_s32MaxQueuelen != -1) && (s_s32CurQueueLen >= s_s32MaxQueuelen)) {
        return QUEUE_ILLEGAL_STATE;
    }

    pstQueueNode = (SAMPLE_IVE_NODE_S *)malloc(sizeof(SAMPLE_IVE_NODE_S));
    if (pstQueueNode == NULL) {
        return QUEUE_OUT_OF_MEMORY;
    }

    (HI_VOID)memcpy_s(&pstQueueNode->stFrameInfo, sizeof(VIDEO_FRAME_INFO_S), pstFrameInfo, sizeof(VIDEO_FRAME_INFO_S));
    pstQueueNode->next = NULL;
    if (SAMPLE_IVE_QueueIsEmpty(pstQueueHead)) {
        pstQueueHead->front = pstQueueNode;
        pstQueueHead->rear = pstQueueNode;
    } else {
        pstQueueHead->rear->next = pstQueueNode;
        pstQueueHead->rear = pstQueueNode;
    }

    s_s32CurQueueLen++;

    return HI_SUCCESS;
}

SAMPLE_IVE_NODE_S *SAMPLE_IVE_QueueGetHeadNode(SAMPLE_IVE_QUEUE_S *pstQueueHead)
{
    if ((pstQueueHead == NULL) || (pstQueueHead->front == NULL)) {
        return NULL;
    }

    return pstQueueHead->front;
}

SAMPLE_IVE_NODE_S *SAMPLE_IVE_QueueGetNode(SAMPLE_IVE_QUEUE_S *pstQueueHead)
{
    SAMPLE_IVE_NODE_S *pstQueueTmp = NULL;

    if ((pstQueueHead == NULL) || (pstQueueHead->front == NULL)) {
        return NULL;
    }

    pstQueueTmp = pstQueueHead->front;
    pstQueueHead->front = pstQueueTmp->next;
    if (pstQueueHead->front == NULL) {
        pstQueueHead->rear = pstQueueHead->front;
    }
    s_s32CurQueueLen--;

    return pstQueueTmp;
}

HI_VOID SAMPLE_IVE_QueueFreeNode(SAMPLE_IVE_NODE_S *pstNode)
{
    if (pstNode != NULL) {
        free(pstNode);
        pstNode = NULL;
    }

    return;
}
