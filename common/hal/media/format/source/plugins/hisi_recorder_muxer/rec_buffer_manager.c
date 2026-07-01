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
 *
 * Description: muxer buffer manager
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ss_recorder_pro.h"
#include "media_hal_common.h"
#include "securec.h"
#define MODULE_NAME "HisiMuxerBufMng"
#define MAX_BUFF_SIZE (64 * 1024 * 1024)

td_s32 RecAllocBuf(void *pBufArg, td_u32 bufSize, void **ppVmAddr)
{
    MEDIA_HAL_UNUSED(pBufArg);
    MEDIA_HAL_LOGI(MODULE_NAME, "buffersize %d", bufSize);
    if ((ppVmAddr == NULL) || (bufSize == 0) || (bufSize > MAX_BUFF_SIZE)) {
        MEDIA_HAL_LOGE(MODULE_NAME, "invalid input param!");
        return TD_FAILURE;
    }
    /* free at RecFreeBuf */
    *ppVmAddr = malloc(bufSize);
    if (*ppVmAddr == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc mbuf memory failed");
        return TD_FAILURE;
    }
    if (memset_s(*ppVmAddr, bufSize, 0, bufSize) != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }
    return TD_SUCCESS;
}

td_s32 RecFreeBuf(void *pBufArg, td_u32 bufSize, void *pVmAddr)
{
    MEDIA_HAL_UNUSED(pBufArg);
    MEDIA_HAL_LOGI(MODULE_NAME, "buffersize %d", bufSize);
    if (pVmAddr == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "invalid input param!");
        return TD_FAILURE;
    }
    free(pVmAddr);
    return TD_SUCCESS;
}