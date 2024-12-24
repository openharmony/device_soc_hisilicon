/**
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
 *
 * Description: dgb heap memory header file\n
 *
 * History: \n
 * 2022-09-16， Create file. \n
 */
#ifndef __DBG_HEAP_MEMORY_H__
#define __DBG_HEAP_MEMORY_H__
#ifdef _DEBUG_HEAP_MEM_MGR

#include "dbg_hash_tab.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

uint32_t heap_mem_tab_init(void);
uint32_t heap_mem_dump(void);
void *heap_mem_malloc(uint32_t size, uint32_t master);
void heap_mem_free(void *ptr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
#endif