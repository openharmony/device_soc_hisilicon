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
 * Description: Malloc function.
 */

#include <stddef.h>
#ifdef _DEBUG_HEAP_MEM_MGR
#include "dbg_heap_memory.h"
#endif
#include "malloc.h"
#include "malloc_porting.h"

static void *malloc_port(uint32_t size, uint32_t caller)
{
    unused(caller);
    return malloc(size);
}

void malloc_port_init(void)
{
    malloc_funcs malloc_funcs = { 0 };
    malloc_funcs.init = malloc_init;
    malloc_funcs.free = free;
    malloc_funcs.malloc = malloc_port;
    malloc_register_funcs(&malloc_funcs);
    malloc_init((uintptr_t)(&g_intheap_begin), (uintptr_t)(&g_intheap_begin) + (uintptr_t)(&g_intheap_size));

#ifdef _DEBUG_HEAP_MEM_MGR
    heap_mem_tab_init();
    malloc_funcs.free = heap_mem_free;
    malloc_funcs.malloc = heap_mem_malloc;
    malloc_register_funcs(&malloc_funcs);
#endif
}