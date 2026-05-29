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

#ifndef  SVP_NPU_RUNTIME_MEM_H
#define  SVP_NPU_RUNTIME_MEM_H

#include "ot_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

td_s32 svp_npu_runtime_mem_get_virt_mem_info(const td_void *virt_addr, td_phys_addr_t *phys_addr, td_bool *is_cached);

td_s32 svp_npu_runtime_mem_malloc(td_void **dev_ptr, td_ulong size, td_bool is_cached);

td_s32 svp_npu_runtime_mem_malloc_inner(td_phys_addr_t *phys_addr, td_void **dev_ptr, td_ulong size, td_bool is_cached);

td_s32 svp_npu_runtime_mem_flush_cache(const td_void *dev_ptr, td_ulong size);

td_s32 svp_npu_runtime_mem_flush_cache_with_phys_addr(td_void *dev_ptr, td_phys_addr_t phys_addr, td_ulong size);

td_s32 svp_npu_runtime_mem_malloc_host(td_void **host_ptr, td_ulong size);

td_s32 svp_npu_runtime_mem_free(const td_void *dev_ptr);

td_s32 svp_npu_runtime_mem_free_host(const td_void *host_ptr);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* SVP_NPU_RUNTIME_MEM_H */
