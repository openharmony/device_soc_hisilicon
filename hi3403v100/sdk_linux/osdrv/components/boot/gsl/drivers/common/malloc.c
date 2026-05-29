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
#include <platform.h>
#include <sizes.h>
#include "securecutil.h"
#include "malloc_sizes.h"

struct cache_sizes {
    size_t cs_size;

    int nr;
    unsigned char *cache;
    unsigned long map;
};

struct cache_sizes_ro {
    uint16_t cs_size;
    uint16_t nr;
};

/*
notes: all writable global variables should be placed in bss section,
should not have default value except zero
 */
static struct cache_sizes malloc_sizes[] = {
#define cache(size, n) {0, 0, 0, 0},
MALLOC_SIZE_TABLE(cache)
#undef cache
    {0, 0, 0, 0}
};

static const struct cache_sizes_ro malloc_sizes_ro[] = {
#define cache(size, n) {size, n},
MALLOC_SIZE_TABLE(cache)
#undef cache
    {0, 0}
};

/*
 * init the memory array for malloc operation
 */
int malloc_init(void)
{
    struct cache_sizes *p = NULL;
    struct cache_sizes *tmp_p = NULL;
    struct cache_sizes *min_p = NULL;
    struct cache_sizes tmp;
    struct cache_sizes_ro const *p_ro = NULL;
    unsigned char *buf = (unsigned char *)MALLOC_START;
    errno_t err;

    err = memset_s(malloc_sizes, sizeof(malloc_sizes), 0, sizeof(malloc_sizes));
    if (err != EOK) {
        return -1;
    }

    for (p = malloc_sizes, p_ro = malloc_sizes_ro; p_ro->nr; ++p, ++p_ro) {
        p->cs_size = p_ro->cs_size;
        p->nr = p_ro->nr;
        p->cache = buf;
        buf += p->cs_size * p->nr;
    }
    p->cs_size = SZ_32M;

    for (p = malloc_sizes; p->nr; p++) {
        min_p = p;
        for (tmp_p = p + 1; tmp_p->nr; tmp_p++) {
            if (tmp_p->cs_size < min_p->cs_size) {
                min_p = tmp_p;
            }
        }

        if (min_p != p) {
            tmp.cache = p->cache;
            tmp.nr = p->nr;
            tmp.cs_size = p->cs_size;

            p->cache = min_p->cache;
            p->nr = min_p->nr;
            p->cs_size = min_p->cs_size;

            min_p->cache = tmp.cache;
            min_p->nr = tmp.nr;
            min_p->cs_size = tmp.cs_size;
        }
    }

    return 0;
}

void *gsl_malloc(size_t size)
{
    unsigned int i;
    unsigned long map;
    struct cache_sizes *p = malloc_sizes;

    for (; size > p->cs_size; ++p) {
    }

    while (p->nr != 0) {
        map = p->map;
        for (i = 0; i < p->nr; ++i) {
            if (!(map & 0x1)) {
                void *ptr = p->cache + p->cs_size * i;
                p->map |= 1 << i;
                return ptr;
            }
            map = map >> 1;
        }
        p++;
    }

    return NULL;
}

void gsl_free(void *mem_ptr)
{
    unsigned int i;
    struct cache_sizes *p = NULL;

    for (p = malloc_sizes;
         ((uintptr_t)mem_ptr - (uintptr_t)p->cache) >= (p->cs_size * p->nr);
         ++p);

    unsigned long size = (uintptr_t)mem_ptr - (uintptr_t)p->cache;
    for (i = 0; size > (p->cs_size * i); ++i) {
    }

    p->map &= ~(1 << i);
}

