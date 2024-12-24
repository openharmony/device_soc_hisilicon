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
 * Description: dfx mem
 * This file should be changed only infrequently and with great care.
 */

#include "dfx_mem.h"
#include "errcode.h"
#include "los_memory.h"

errcode_t dfx_mem_get_sys_pool_info(mdm_mem_info_t *info)
{
    LOS_MEM_POOL_STATUS pool_info;

    if (LOS_MemInfoGet(m_aucSysMem0, &pool_info) != 0) {
        return ERRCODE_FAIL;
    }

    info->total = pool_info.uwTotalUsedSize + pool_info.uwTotalFreeSize;
    info->used = pool_info.uwTotalUsedSize;
    info->free = pool_info.uwTotalFreeSize;
    info->max_free_node_size = pool_info.uwMaxFreeNodeSize;
    info->used_node_num = pool_info.uwUsedNodeNum;
    info->free_node_num = pool_info.uwFreeNodeNum;
#ifdef LOSCFG_MEM_TASK_STAT
    info->peek_size = pool_info.uwUsageWaterLine;
#endif

    return ERRCODE_SUCC;
}
