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

#include "display_gralloc_internal.h"

int32_t ShareMemory(uint64_t phyAddr, BufferHandle *buffer)
{
    ot_sys_mem_info mem_info;
    int32_t ret = ss_mpi_sys_get_mem_info_by_phys(phyAddr, &mem_info);
    if (ret != TD_SUCCESS) {
        (void)ss_mpi_sys_mmz_free(buffer->phyAddr, buffer->virAddr);
        return DISPLAY_FAILURE;
    }
    
    ret = ss_mpi_sys_mem_share_all(mem_info.mem_handle);
    if (ret != TD_SUCCESS) {
        (void)ss_mpi_sys_mmz_free(buffer->phyAddr, buffer->virAddr);
        return DISPLAY_FAILURE;
    }

    return DISPLAY_SUCCESS;
}

int32_t UnShareMemory(BufferHandle *buffer)
{
    ot_sys_mem_info mem_info;
    int32_t ret = ss_mpi_sys_get_mem_info_by_phys(buffer->phyAddr, &mem_info);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("%s: ss_mpi_sys_get_mem_info_by_phys failure, ret 0x%x", __func__, ret);
        return DISPLAY_FAILURE;
    }
    (void)ss_mpi_sys_mem_unshare_all(mem_info.mem_handle);
    return DISPLAY_SUCCESS;
}
