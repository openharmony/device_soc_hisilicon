/*
 *   Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: OS Abstract Layer.
 */

/**
 * @defgroup osal_cache osal_cache
 */
#ifndef OSAL_CAHCE_H
#define OSAL_CAHCE_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @ingroup osal_cache
 * @brief flush DCache.
 *
 * @par Description:
 * This API is used to flush DCache.
 *
 * @attention
 * The base address will be aligned to CACHE_LINE_SIZE(32Bytes) if it's not aligned to CACHE_LINE_SIZE.
 * Make sure that phys_addr is valid when mmu does not exist, and user set its addr in kvirt.
 *
 * @param  kvirt [in] kvirt.
 * @param  phys_addr [in] The start address need flush.
 * @param  size [in] The size of flush memory.
 *
 * @par Support System:
 * linux liteos seliteos.
 */
void osal_dcache_region_wb(void *kvirt, unsigned long phys_addr, unsigned long size);

/**
 * @ingroup osal_cache
 * @brief invalid DCache.
 *
 * @par Description:
 * This API is used to invalid DCache.
 *
 * @attention
 * The base address will be aligned to CACHE_LINE_SIZE(32Bytes) if it's not aligned to CACHE_LINE_SIZE.
 *
 * @param  addr [in] The start address need flush.
 * @param  size [in] The size of flush memory.
 *
 * @par Support System:
 * linux liteos seliteos.
 */
void osal_dcache_region_inv(void *addr, unsigned long size);

/**
 * @ingroup osal_cache
 * @brief clean DCache.
 *
 * @par Description:
 * This API is used to clean DCache.
 *
 * @par Support System:
 * liteos.
 */
void osal_dcache_region_clean(void *addr, unsigned int size);

/**
 * @ingroup osal_cache
 * @brief flush DCache.
 *
 * @par Description:
 * This API is used to flush DCache.
 *
 * @par Support System:
 * seliteos.
 */
void osal_arch_dcache_flush_by_va(void *base_addr, unsigned int size);

/**
 * @ingroup osal_cache
 * @brief invalid DCache.
 *
 * @par Description:
 * This API is used to invalid DCache.
 *
 * @par Support System:
 * seliteos.
 */
void osal_arch_dcahce_inv_by_va(void *base_addr, unsigned int size);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* __OSAL_CAHCE_H__ */