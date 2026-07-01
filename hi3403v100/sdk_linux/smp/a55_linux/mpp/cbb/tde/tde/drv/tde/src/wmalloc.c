/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free_unit software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#include "wmalloc.h"
#include "securec.h"
#include "tde_define.h"
#include "tde_adp.h"

typedef struct {
    td_u32 n_size;
    td_u16 n_free;
    td_u16 n_first;
    td_u16 n_unit_size;
#if OT_TDE_MEMCOUNT_SUPPORT
    td_u16 n_max_used; /* max used unit number */
    td_u16 n_max_num;  /* max unit number */
#endif
    td_void *start_addr;
    struct _memory_block *next;
} memory_block;

typedef enum {
    UNIT_SIZE_CMD = 0,
    UNIT_SIZE_JOB,
    UNIT_SIZE_NODE,
    UNIT_SIZE_FILTER,
    UNIT_SIZE_BUTT
} unit_size;

static memory_block g_stru_mem_block[UNIT_SIZE_BUTT];

static osal_spinlock_t g_mem_lock;

#define print_meminfo() \
    do { \
        tde_info("-----------------------------------------------------\n"); \
        tde_info("\tmem_block info\ttotal\tfree\n"); \
        tde_info("\t%d\t%lld\t%d\n", CMD_SIZE, OT_TDE_CMD_NUM, g_stru_mem_block[UNIT_SIZE_CMD].n_free); \
        tde_info("\t%d\t%lld\t%d\n", JOB_SIZE, OT_TDE_JOB_NUM, g_stru_mem_block[UNIT_SIZE_JOB].n_free); \
        tde_info("\t%ld\t%lld\t%d\n", (td_ulong)NODE_SIZE, OT_TDE_NODE_NUM, g_stru_mem_block[UNIT_SIZE_NODE].n_free); \
        tde_info("\t%d\t%d\t%d\n", FILTER_SIZE, OT_TDE_FILTER_NUM, g_stru_mem_block[UNIT_SIZE_FILTER].n_free); \
        tde_info("------------------------------------------------------\n"); \
    } while (0)

static td_s32 memory_block_init(unit_size size, td_u16 n_unit_num, td_void *addr)
{
    td_u16 i;

    td_u8 *data = (td_u8 *)addr;

    if ((size >= UNIT_SIZE_BUTT) || (addr == TD_NULL) || (n_unit_num == 0)) {
        return TD_FAILURE;
    }
    if (size == UNIT_SIZE_CMD) {
        g_stru_mem_block[size].n_unit_size = CMD_SIZE;
    } else if (size == UNIT_SIZE_JOB) {
        g_stru_mem_block[size].n_unit_size = JOB_SIZE;
    } else if (size == UNIT_SIZE_NODE) {
        g_stru_mem_block[size].n_unit_size = NODE_SIZE;
    } else if (size == UNIT_SIZE_FILTER) {
        g_stru_mem_block[size].n_unit_size = FILTER_SIZE;
    }
    for (i = 1; i < (td_u16)n_unit_num; i++) {
        /* Don't flag for last unit,for last unit is ready for assigned, which is say no next unit can be assigned */
        *(td_u16 *)data = i;

        data += g_stru_mem_block[size].n_unit_size;
    }

    g_stru_mem_block[size].n_first = 0;
    g_stru_mem_block[size].n_free = n_unit_num;
    g_stru_mem_block[size].n_size = n_unit_num * g_stru_mem_block[size].n_unit_size;
    g_stru_mem_block[size].next = TD_NULL;
    g_stru_mem_block[size].start_addr = addr;

#if OT_TDE_MEMCOUNT_SUPPORT
    g_stru_mem_block[size].n_max_num = n_unit_num;
    g_stru_mem_block[size].n_max_used = 0;
#endif

    return TD_SUCCESS;
}

static td_void *malloc_unit(unit_size size)
{
    unsigned long lockflags;

    memory_block *block = TD_NULL;
    td_u8 *free_unit = TD_NULL;

    tde_spin_lock(&g_mem_lock, lockflags);
    block = &g_stru_mem_block[size];
    tde_info("eUnitSize %d, malloc units:%d, first malloc unit:%d...\n", block->n_unit_size, block->n_free,
             block->n_first);
    if (!block->n_free) {
        tde_spin_unlock(&g_mem_lock, lockflags);
        return TD_NULL;
    }

    free_unit = (td_u8 *)block->start_addr + block->n_first * block->n_unit_size;

    block->n_first = *(td_u16 *)free_unit;
    block->n_free--;
#if OT_TDE_MEMCOUNT_SUPPORT
    if ((g_stru_mem_block[size].n_max_num - block->n_free) > g_stru_mem_block[size].n_max_used) {
        g_stru_mem_block[size].n_max_used = g_stru_mem_block[size].n_max_num - block->n_free;
    }
#endif
    tde_spin_unlock(&g_mem_lock, lockflags);
    (td_void)memset_s(free_unit, block->n_unit_size, 0, block->n_unit_size);
    return free_unit;
}

static td_void *wmalloc(unsigned long size)
{
    unit_size i;
    td_void *malloc_unit_ptr = TD_NULL;

    if ((size > FILTER_SIZE) || (size == 0)) {
        return TD_NULL;
    }
    if (size <= CMD_SIZE) {
        for (i = UNIT_SIZE_CMD; i < UNIT_SIZE_BUTT; i++) {
            malloc_unit_ptr = malloc_unit(i);
            if (malloc_unit_ptr != TD_NULL) {
                return malloc_unit_ptr;
            }
        }
        return TD_NULL;
    } else if (size <= JOB_SIZE) {
        for (i = UNIT_SIZE_JOB; i < UNIT_SIZE_BUTT; i++) {
            malloc_unit_ptr = malloc_unit(i);
            if (malloc_unit_ptr != TD_NULL) {
                return malloc_unit_ptr;
            }
        }
        return TD_NULL;
    } else if (size <= NODE_SIZE) {
        for (i = UNIT_SIZE_NODE; i < UNIT_SIZE_BUTT; i++) {
            malloc_unit_ptr = malloc_unit(i);
            if (malloc_unit_ptr != TD_NULL) {
                return malloc_unit_ptr;
            }
        }
        return TD_NULL;
    } else {
        return malloc_unit(UNIT_SIZE_FILTER);
    }
}

static td_s32 free_unit(unit_size size, td_void *ptr)
{
    unsigned long lockflags;
    memory_block *block = TD_NULL;
    tde_spin_lock(&g_mem_lock, lockflags);
    block = &g_stru_mem_block[size];

    if (((td_u8 *)ptr < (td_u8 *)block->start_addr) ||
        ((td_u8 *)ptr >= ((td_u8 *)block->start_addr + block->n_size))) {
        tde_spin_unlock(&g_mem_lock, lockflags);
        return TD_FAILURE;
    }

    block->n_free++;
    *(td_u16 *)ptr = block->n_first; /* point to next unit can be assigned */

    if (block->n_unit_size == 0) {
        tde_spin_unlock(&g_mem_lock, lockflags);
        return TD_FAILURE;
    }
    block->n_first = (td_u16)((ptr - (block->start_addr)) / block->n_unit_size);

    tde_info("eUnitSize:%hu,first free_unit unit:%hu, free_unit units:%hu\n", block->n_unit_size,
        block->n_first, block->n_free);
    tde_spin_unlock(&g_mem_lock, lockflags);
    return TD_SUCCESS;
}

static td_s32 wfree(td_void *ptr)
{
    unit_size i;
    for (i = UNIT_SIZE_CMD; i < UNIT_SIZE_BUTT; i++) {
        if (free_unit(i, ptr) == TD_SUCCESS) {
            return TD_SUCCESS;
        }
    }
    tde_error("Free mem failed!vir:0x%p, phy:%lx\n", ptr, (td_ulong)wgetphy(ptr));
    return TD_FAILURE;
}

td_void *tde_malloc(td_u32 size)
{
    td_void *ptr = TD_NULL;
    ptr = (td_void *)wmalloc(size);
    return ptr;
}

td_void tde_free(td_void *ptr)
{
    if (ptr == TD_NULL) {
        return;
    }
    if (wfree(ptr) != TD_SUCCESS) {
        tde_error("Free mem failed\n");
    }
    return;
}

#define TDE_MAX_NODE_NUM 500
#define TDE_MIN_NODE_NUM 2

static td_phys_addr_t g_mem_poolphy_addr;
static td_void *g_mem_pool_vrt_addr = TD_NULL;
static td_u64 g_max_node_num = 0;

#define OT_TDE_CMD_NUM  g_max_node_num
#define OT_TDE_JOB_NUM  OT_TDE_CMD_NUM
#define OT_TDE_NODE_NUM OT_TDE_CMD_NUM

#define TDE_CMD_OFFSET    0
#define TDE_JOB_OFFSET    ((OT_TDE_CMD_NUM)*CMD_SIZE)
#define TDE_NODE_OFFSET   (TDE_JOB_OFFSET + ((OT_TDE_JOB_NUM)*JOB_SIZE))
#define TDE_FILTER_OFFSET (TDE_NODE_OFFSET + ((OT_TDE_NODE_NUM)*NODE_SIZE))
#define TDE_MEMPOOL_SIZE                                                                   \
    ((OT_TDE_CMD_NUM)*CMD_SIZE + (OT_TDE_JOB_NUM)*JOB_SIZE + (OT_TDE_NODE_NUM)*NODE_SIZE + \
     (OT_TDE_FILTER_NUM)*FILTER_SIZE)

td_void tde_set_max_node_num(td_u32 node_num)
{
    g_max_node_num = node_num;
}

td_s32 wmeminit(void)
{
    td_s32 ret;

    if (g_max_node_num > TDE_MAX_NODE_NUM) {
        g_max_node_num = TDE_MAX_NODE_NUM;
    } else if (g_max_node_num < TDE_MIN_NODE_NUM) {
        g_max_node_num = TDE_MIN_NODE_NUM;
    }

    tde_get_phyaddr_mmb("tde_mem_pool_mmb", TDE_MEMPOOL_SIZE, g_mem_poolphy_addr);
    if (g_mem_poolphy_addr == 0) {
        tde_error("malloc_unit_ptr mempool buffer failed!\n");
        return TD_FAILURE;
    }
#ifdef TDE_CACH_STRATEGY
    g_mem_pool_vrt_addr = (td_void *)CMPI_Remap_Cached(g_mem_poolphy_addr, TDE_MEMPOOL_SIZE);
    if (g_mem_pool_vrt_addr == 0) {
        tde_free_mmb(g_mem_poolphy_addr);
        return -1;
    }

#else
    g_mem_pool_vrt_addr = (td_void *)cmpi_remap_nocache(g_mem_poolphy_addr, TDE_MEMPOOL_SIZE);
    if (g_mem_pool_vrt_addr == 0) {
        tde_free_mmb(g_mem_poolphy_addr);
        return -1;
    }

#endif

    if (g_mem_pool_vrt_addr == TD_NULL) {
        tde_free_mmb(g_mem_poolphy_addr);
        g_mem_poolphy_addr = 0;
        return TD_FAILURE;
    }
    ret = memory_block_init(UNIT_SIZE_CMD, OT_TDE_CMD_NUM, (g_mem_pool_vrt_addr));
    if (ret != TD_SUCCESS) {
    }
    ret = memory_block_init(UNIT_SIZE_JOB, OT_TDE_JOB_NUM, ((td_u8 *)g_mem_pool_vrt_addr + TDE_JOB_OFFSET));
    if (ret != TD_SUCCESS) {
    }
    ret = memory_block_init(UNIT_SIZE_NODE, OT_TDE_NODE_NUM, ((td_u8 *)g_mem_pool_vrt_addr + TDE_NODE_OFFSET));
    if (ret != TD_SUCCESS) {
    }
    ret = memory_block_init(UNIT_SIZE_FILTER, OT_TDE_FILTER_NUM, ((td_u8 *)g_mem_pool_vrt_addr + TDE_FILTER_OFFSET));
    if (ret != TD_SUCCESS) {
    }
    osal_spin_lock_init(&g_mem_lock);
    print_meminfo();

    return TD_SUCCESS;
}

td_void wmemterm(void)
{
    print_meminfo();

    tde_unmap_mmb(g_mem_pool_vrt_addr);
    tde_free_mmb(g_mem_poolphy_addr);
    g_mem_poolphy_addr = 0;
    g_mem_pool_vrt_addr = TD_NULL;
    osal_spin_lock_destroy(&g_mem_lock);

    return;
}

td_phys_addr_t wgetphy(const td_void *ptr)
{
    if (ptr == TD_NULL) {
        return 0;
    }

    if ((ptr < g_mem_pool_vrt_addr) || ((td_u8 *)ptr >= ((td_u8 *)g_mem_pool_vrt_addr + TDE_MEMPOOL_SIZE))) {
        return 0;
    }

    return (g_mem_poolphy_addr + (ptr - g_mem_pool_vrt_addr));
}

td_void *wgetvrt(td_phys_addr_t phyaddr)
{
    if ((phyaddr < g_mem_poolphy_addr) || (phyaddr >= (g_mem_poolphy_addr + TDE_MEMPOOL_SIZE))) {
        return TD_NULL;
    }

    return (td_void *)(g_mem_pool_vrt_addr + (phyaddr - g_mem_poolphy_addr));
}

td_u32 wgetfreenum(td_void)
{
    unit_size size = 0;
    td_u32 free_unit_num = g_stru_mem_block[size].n_free;

    for (size = UNIT_SIZE_CMD; size < UNIT_SIZE_FILTER; size++) {
        free_unit_num = (free_unit_num > g_stru_mem_block[size].n_free) ? g_stru_mem_block[size].n_free :
                         free_unit_num;
    }

    return free_unit_num;
}

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
osal_proc_entry_t *wprintinfo(osal_proc_entry_t *page)
{
#if OT_TDE_MEMCOUNT_SUPPORT
    td_u32 max_used_cmd;
    td_u32 max_used_job;
    td_u32 max_used_node;
    td_u32 max_used_filter;
#else
    td_u32 free_cmd;
    td_u32 free_job;
    td_u32 free_node;
    td_u32 free_filter;
#endif
    if (page == TD_NULL) {
        return 0;
    }

#if OT_TDE_MEMCOUNT_SUPPORT
    max_used_cmd = g_stru_mem_block[UNIT_SIZE_CMD].n_max_used;
    max_used_job = g_stru_mem_block[UNIT_SIZE_JOB].n_max_used;
    max_used_node = g_stru_mem_block[UNIT_SIZE_NODE].n_max_used;
    max_used_filter = g_stru_mem_block[UNIT_SIZE_FILTER].n_max_used;
#else
    free_cmd = g_stru_mem_block[UNIT_SIZE_CMD].n_free;
    free_job = g_stru_mem_block[UNIT_SIZE_JOB].n_free;
    free_node = g_stru_mem_block[UNIT_SIZE_NODE].n_free;
    free_filter = g_stru_mem_block[UNIT_SIZE_FILTER].n_free;
#endif
    osal_seq_printf(page, "\n[TDE] Version: ["OT_MPP_VERSION"], Build Time["__DATE__", "__TIME__"]\n\n");
    osal_seq_printf(page, "--------- tde memory pool info ---------\n");
#if OT_TDE_MEMCOUNT_SUPPORT
    osal_seq_printf(page, "type            total    max_used\n");
    osal_seq_printf(page, "[unit %d]   %8u  %8u\n", CMD_SIZE, OT_TDE_CMD_NUM, max_used_cmd);
    osal_seq_printf(page, "[unit %d]   %8u  %8u\n", JOB_SIZE, OT_TDE_JOB_NUM, max_used_job);
    osal_seq_printf(page, "[unit %d]   %8u  %8u\n", NODE_SIZE, OT_TDE_NODE_NUM, max_used_job);
    osal_seq_printf(page, "[unit %d]   %8u  %8u\n", FILTER_SIZE, OT_TDE_FILTER_NUM, max_used_filter);
    osal_seq_printf(page, "[total]   %8uK %8uK\n", TDE_MEMPOOL_SIZE / 1024, /* 1024 Demotion byte */
                    (CMD_SIZE * max_used_cmd + JOB_SIZE * max_used_job + NODE_SIZE * max_used_node + FILTER_SIZE *
                     max_used_filter) / 1024); /* 1024 Demotion byte */
#else
    osal_seq_printf(page, "type            total    used\n");
    osal_seq_printf(page, "[unit %d] \t %lld \t %lld\n", CMD_SIZE, OT_TDE_CMD_NUM, OT_TDE_CMD_NUM - free_cmd);
    osal_seq_printf(page, "[unit %d] \t %lld \t %lld\n", JOB_SIZE, OT_TDE_JOB_NUM, OT_TDE_JOB_NUM - free_job);
    osal_seq_printf(page, "[unit %ld] \t %lld \t %lld\n", (unsigned long)NODE_SIZE, OT_TDE_NODE_NUM,
                    OT_TDE_NODE_NUM - free_node);
    osal_seq_printf(page, "[unit %d] \t %d \t %d\n", FILTER_SIZE, OT_TDE_FILTER_NUM, OT_TDE_FILTER_NUM - free_filter);
    osal_seq_printf(page, "[total]          %lldK \t %lldK\n", TDE_MEMPOOL_SIZE / 1024, /* 1024 Demotion byte */
                    (TDE_MEMPOOL_SIZE - (CMD_SIZE * free_cmd + JOB_SIZE * free_job + NODE_SIZE * free_node +
                    FILTER_SIZE * free_filter)) / 1024); /* 1024 Demotion byte */

#endif
    return page;
}
#endif

