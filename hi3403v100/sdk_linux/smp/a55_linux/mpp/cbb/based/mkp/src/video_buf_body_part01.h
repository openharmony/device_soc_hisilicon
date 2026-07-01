/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
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

#ifndef SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART01_H_
#define SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART01_H_


#line 1 "video_buf.c"
/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
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

#include "ot_osal.h"
#include "ot_common.h"
#include "securec.h"
#include "ot_debug.h"
#include "ot_common_vb.h"
#include "mkp_vb.h"
#include "proc_ext.h"
#include "dev_ext.h"
#include "vb_ext.h"
#include "mod_ext.h"
#include "mm_ext.h"
#include "vb_drv.h"
#include "vb_supplement.h"
#include "ot_inner_video.h"
#include "ot_inner_sys_ipc.h"

#define vb_mk_handle(pool_id, blk_id) ((((pool_id) & 0xffff) << 16) | ((blk_id) & 0xffff))
#define vb_hdle_to_pool_id(handle) ((handle) >> 16)
#define vb_hdle_to_blk_id(handle) ((handle) & 0xffff)

#define VB_MAX_BLK_CNT   10240
#define VB_MAX_POOL_SIZE 4294967295ULL /* 4GB */
#define CHN_INFO_NONE 9999

#define vb_check_initialization_status_return(ret)                    \
    do {                                                              \
        if (g_pools == TD_NULL) {                                 \
            ot_trace_vb(OT_DBG_ERR, "VB not initialized!\n"); \
            return ret;                                               \
        }                                                             \
    } while (0)

#define vb_check_pool_id_return(pool_id, ret)                       \
    do {                                                              \
        if ((pool_id) >= g_vb_conf.max_pool_cnt) {                      \
            ot_trace_vb(OT_DBG_ERR, "pool ID [%u] is great than max pool id [%u]!\n", \
                        pool_id, g_vb_conf.max_pool_cnt);             \
            return ret;                                               \
        }                                                             \
    } while (0)

#define vb_check_user_id_return(uid, ret)                           \
    do {                                                              \
        if ((uid) >= OT_VB_MAX_USER) {                                     \
            ot_trace_vb(OT_DBG_ERR, "uid should between [0, %d)! \n", OT_VB_MAX_USER); \
            return ret;                                               \
        }                                                             \
    } while (0)

#define vb_check_null_ptr_return(ptr) \
    do { \
        if (((td_void *)(td_uintptr_t)(ptr)) == TD_NULL) { \
            ot_trace_vb(OT_DBG_ERR, "illegal parameter is null!\n"); \
            return OT_ERR_VB_NULL_PTR; \
        } \
    } while (0)

typedef struct {
    td_u32 blk_cnt;
    td_u64 blk_size;
    td_s32 owner;
    ot_vb_uid uid;
    ot_vb_remap_mode vb_remap_mode;
    td_char buf_name[OT_MAX_MMZ_NAME_LEN];
} vb_pool_common_info;

typedef struct {
    td_u64 align_blk_size;
    td_u32 align_sup_cached_size;
    td_u32 align_sup_no_cache_size;
    td_u32 align_sup_cached_blk_size;
    td_u32 align_sup_no_cache_blk_size;
} vb_create_pool_size_set;

typedef struct {
    struct osal_list_head list;
    td_u32 blk_id;

    td_phys_addr_t phys_addr;
    td_void *vir_addr;

    td_s32 sum_cnt; /* sum of total user count */
    td_s32 user_cnt[OT_VB_MAX_USER];

    td_u64 blk_size;

    ot_video_supplement supplement;
} vb_blk_info;

typedef struct {
    td_u32 pool_id;
    vb_blk_info *blk;

    td_bool is_comm_pool;
    ot_vb_remap_mode vb_remap_mode;

    td_u32 blk_cnt;
    td_u64 blk_size;
    td_u64 pool_size;

    td_phys_addr_t pool_phy_addr;
    td_void *pool_vir_addr;

    struct osal_list_head free_list;
    struct osal_list_head busy;

    /* being used to link common pools */
    struct osal_list_head list;

    /* count of free_list block. */
    td_u32 free_blk_cnt;
    td_u32 min_free_blk_cnt;
    td_s32 user_cnt_sum[OT_VB_MAX_USER];

#define VB_POOL_IDLE     0x10 /* none operating this pool */
#define VB_POOL_DEAD     0x12 /* destroying */
#define VB_POOL_ZOMBIE   0x14 /* async destroy */
    td_u32 state;
    td_bool async_flag;
    td_bool user;

    td_char ac_pool_name[OT_MAX_MMZ_NAME_LEN];

    /* pool owner (who create the pool) */
    td_s32 pool_owner;
    ot_vb_uid uid;

    td_u32 supplement_cached_size;
    td_u32 supplement_no_cached_size;
    td_phys_addr_t supplement_cached_phy;
    td_phys_addr_t supplement_no_cache_phy;
    td_void *supplement_cached_vir;
    td_void *supplement_no_cache_vir;
} vb_pool_attr;

/* array of pointer to the pool */
static vb_pool_attr **g_pools = TD_NULL;

/* array of pointer to the first block in every pool */
static vb_blk_info **g_blks = TD_NULL;

/* video buffer configuration */
static ot_vb_cfg g_vb_conf;

static td_bool g_is_conf = TD_FALSE;

/* common pools list */
static struct osal_list_head g_comm_pools;

/* module video buffer configuration */
static ot_vb_cfg g_vb_mod_conf[OT_VB_MAX_USER];
static td_bool g_is_mod_conf[OT_VB_MAX_USER] = { [0 ...(OT_VB_MAX_USER - 1)] = TD_FALSE };
static td_bool g_is_mod_pool_init[OT_VB_MAX_USER] = { [0 ...(OT_VB_MAX_USER - 1)] = TD_FALSE };

/* vb supplement configuration */
static ot_vb_supplement_cfg g_supplement_conf = {0};

/* the semaphore is used to avoid race condition for allocation of pool ID and
 * list of common pools.
 */
static osal_semaphore_t g_sema;

/* vb init flag */
static int g_vb_init = TD_FALSE;

osal_spinlock_t g_vb_spin_lock;

td_u32 g_vb_force_exit = 0;

#ifdef CONFIG_OT_VB_ASYNC_SUPPORT
/* vb_pool async */
static osal_wait_t g_vb_async_destroy_wait = {0};
static osal_task_t *g_vb_async_destroy_td = TD_NULL;
static struct osal_list_head g_zombie_state_pools;
#endif

#define vb_spin_lock(flags) osal_spin_lock_irqsave(&g_vb_spin_lock, &(flags))

#define vb_spin_unlock(flags) osal_spin_unlock_irqrestore(&g_vb_spin_lock, &(flags))

#define VB_ALIGN_LEN     256
#define vb_get_aligned_size(len) (VB_ALIGN_LEN * (((len) + VB_ALIGN_LEN - 1) / VB_ALIGN_LEN))
#define vb_get_default_aligned_size(len) (OT_DEFAULT_ALIGN * (((len) + OT_DEFAULT_ALIGN - 1) / OT_DEFAULT_ALIGN))

static td_s32 vb_check_block_id(td_u32 pool_id, td_u32 blk_id)
{
    if (blk_id > g_pools[pool_id]->blk_cnt) {
        ot_trace_vb(OT_DBG_ERR, "Pool [%u] invalid block id [0x%x]! \n", pool_id, blk_id);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }
    return TD_SUCCESS;
}

void vb_set_force_exit(td_u32 value)
{
    g_vb_force_exit = value;
}

static td_s32 vb_check_phy_addr(const vb_pool_attr *pool, td_phys_addr_t phys_addr)
{
    if ((phys_addr < pool->pool_phy_addr) ||
        (phys_addr >= pool->pool_phy_addr + pool->pool_size)) {
        ot_trace_vb(OT_DBG_ERR, "phy_addr = %lx error! pool[%d]: phy_addr = %lx, size = %llu\n",
                    (td_ulong)phys_addr, pool->pool_id, (td_ulong)pool->pool_phy_addr, pool->pool_size);
            return OT_ERR_VB_ILLEGAL_PARAM;
        }

    return TD_SUCCESS;
}

static td_s32 vb_check_create_pool_param(const vb_pool_common_info *info, const td_u32 *pool_id)
{
    if ((info->owner < OT_POOL_OWNER_USER) && (info->owner > OT_POOL_OWNER_COMMON)) {
        ot_trace_vb(OT_DBG_EMERG, "vb owner:%d is illegal!\n", info->owner);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if (pool_id == TD_NULL) {
        ot_trace_vb(OT_DBG_EMERG, "poolid NULL!\n");
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if (info->vb_remap_mode >= OT_VB_REMAP_MODE_BUTT) {
        ot_trace_vb(OT_DBG_EMERG, "vb_remap_mode:%d is illegal!\n", info->vb_remap_mode);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if (osal_strnlen(info->buf_name, OT_MAX_MMZ_NAME_LEN) <= 0 ||
        osal_strnlen(info->buf_name, OT_MAX_MMZ_NAME_LEN) >= OT_MAX_MMZ_NAME_LEN) {
        ot_trace_vb(OT_DBG_EMERG, "buf_name ERR!\n");
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if (info->uid >= OT_VB_UID_BUTT) {
        ot_trace_vb(OT_DBG_EMERG, "vb_uid:%d ERR!\n", info->uid);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if (info->blk_cnt > VB_MAX_BLK_CNT) {
        ot_trace_vb(OT_DBG_EMERG, "illegal blk_cnt(%d), [1, %d]!\n", info->blk_cnt, VB_MAX_BLK_CNT);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }
    return TD_SUCCESS;
}

static td_s32 vb_creat_common_vb_pool(const vb_pool_common_info *info, vb_pool_attr *pool,
    const td_char *pc_mmz_name, vb_create_pool_size_set *pool_size)
{
    td_s32 ret;
    const td_u64 align_blk_size = vb_get_aligned_size(info->blk_size);
    /*
     * kernel_only标志，对于公共VB、模块VB和用户VB默认是FALSE；
     * 对于私有VB，VO的display buffer、解码的输出buffer和编码的参考帧buffer也支持用户接口获取，设置为false也没有影响
     */
    const td_bool kernel_only = TD_FALSE;
    cmpi_mmz_malloc_param malloc_param;

    pool_size->align_blk_size = align_blk_size;

    /* 此处增加VB池大小的判断限制在4G，MMZ限制1G，防止32位系统VB池大小处于[4G, 5G]之间出现越界反而申请成功的情况 */
    if ((align_blk_size > VB_MAX_POOL_SIZE) || (align_blk_size * info->blk_cnt > VB_MAX_POOL_SIZE)) {
        ot_trace_vb(OT_DBG_EMERG, "[size = %llu, cnt = %u]vb mmz alloc:%s total size is larger than 4GB!\n",
            align_blk_size, info->blk_cnt, pc_mmz_name);
        return OT_ERR_VB_NO_MEM;
    }

    malloc_param.mmz_name = pc_mmz_name;
    malloc_param.buf_name = info->buf_name;
    malloc_param.phys_addr = &pool->pool_phy_addr;
    malloc_param.virt_addr = (td_void **)&pool->pool_vir_addr;
    malloc_param.len = (td_ulong)(align_blk_size * info->blk_cnt);
    malloc_param.kernel_only = kernel_only;

    if (info->vb_remap_mode == OT_VB_REMAP_MODE_NONE) {
        pool->pool_phy_addr = cmpi_mmz_malloc(pc_mmz_name, info->buf_name,
            (td_ulong)(align_blk_size * info->blk_cnt), kernel_only);
        if (pool->pool_phy_addr == 0) {
            ret = OT_ERR_VB_NO_MEM;
        } else {
            ret = TD_SUCCESS;
        }
        pool->pool_vir_addr = TD_NULL;
    } else if (info->vb_remap_mode == OT_VB_REMAP_MODE_NOCACHE) {
        ret = cmpi_mmz_malloc_nocache(&malloc_param);
    } else {
        ret = cmpi_mmz_malloc_cached(&malloc_param);
    }

    if (ret != TD_SUCCESS) {
        ot_trace_vb(OT_DBG_EMERG, "[size = %llu, cnt = %u]vb mmz alloc:%s failed!\n",
            align_blk_size, info->blk_cnt, pc_mmz_name);
        return OT_ERR_VB_NO_MEM;
    }

    return TD_SUCCESS;
}


static td_void vb_pool_size_init(vb_create_pool_size_set *pool_size, const vb_pool_common_info *info)
{
    td_u32 cached_blk_size   = vb_get_supplement_size(&g_supplement_conf, TD_TRUE);
    td_u32 cached_size       = cached_blk_size * info->blk_cnt;
    td_u32 no_cache_blk_size = vb_get_default_aligned_size(vb_get_supplement_size(&g_supplement_conf, TD_FALSE));
    td_u32 no_cache_size     = vb_get_default_aligned_size(no_cache_blk_size * info->blk_cnt);

    pool_size->align_sup_cached_blk_size   = cached_blk_size;
    pool_size->align_sup_cached_size       = cached_size;
    pool_size->align_sup_no_cache_blk_size = no_cache_blk_size;
    pool_size->align_sup_no_cache_size     = no_cache_size;
}

static td_s32 vb_create_supplement_no_cache(const vb_pool_common_info *info, vb_pool_attr *pool,
    const td_char *mmz_name, const vb_create_pool_size_set *pool_size)
{
    td_s32 ret;
    td_char supp_name[OT_MAX_MMZ_NAME_LEN] = {0};
    cmpi_mmz_malloc_param malloc_param = {
        .mmz_name = mmz_name,
        .buf_name = supp_name,
        .phys_addr = &pool->supplement_no_cache_phy,
        .virt_addr = (td_void **)&pool->supplement_no_cache_vir,
        .len = pool_size->align_sup_no_cache_size,
        .kernel_only = TD_FALSE,
    };

    if (pool_size->align_sup_no_cache_size == 0) {
        pool->supplement_no_cache_phy = 0;
        pool->supplement_no_cache_vir = TD_NULL;
        return TD_SUCCESS;
    }

    if (snprintf_s(supp_name, OT_MAX_MMZ_NAME_LEN, OT_MAX_MMZ_NAME_LEN - 1, "%s_%s",
        info->buf_name, "sup_nc") < 0) {
        ot_trace_vb(OT_DBG_ERR, "the buf len of name(%s) too long ,copy failed!\n", info->buf_name);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    ret = cmpi_mmz_malloc_nocache(&malloc_param);
    if (ret != TD_SUCCESS) {
        ot_trace_vb(OT_DBG_ERR, "supplement nocache malloc(size:%u) from mmz:%s err!\n",
            pool_size->align_sup_no_cache_size, mmz_name);
        return OT_ERR_VB_NO_MEM;
    }
    return TD_SUCCESS;
}

static td_s32 vb_create_supplement_cached(const vb_pool_common_info *info, vb_pool_attr *pool,
    const td_char *mmz_name, const vb_create_pool_size_set *pool_size)
{
    td_s32 ret;
    td_char supp_name[OT_MAX_MMZ_NAME_LEN] = {0};
    cmpi_mmz_malloc_param malloc_param = {
        .mmz_name = mmz_name,
        .buf_name = supp_name,
        .phys_addr = &pool->supplement_cached_phy,
        .virt_addr = (td_void **)&pool->supplement_cached_vir,
        .len = pool_size->align_sup_cached_size,
        .kernel_only = TD_FALSE,
    };

    if (pool_size->align_sup_cached_size == 0) {
        pool->supplement_cached_phy = 0;
        pool->supplement_cached_vir = TD_NULL;
        return TD_SUCCESS;
    }

    if (snprintf_s(supp_name, OT_MAX_MMZ_NAME_LEN, OT_MAX_MMZ_NAME_LEN - 1, "%s_%s",
        info->buf_name, "sup_c") < 0) {
        ot_trace_vb(OT_DBG_ERR, "the buf len of name(%s) too long ,copy failed!\n", info->buf_name);
        cmpi_mmz_free(pool->supplement_no_cache_phy, pool->supplement_no_cache_vir);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    ret = cmpi_mmz_malloc_cached(&malloc_param);
    if (ret != TD_SUCCESS) {
        ot_trace_vb(OT_DBG_EMERG, "supplement cached malloc(size:%u) from mmz:%s err!\n",
            pool_size->align_sup_cached_size, mmz_name);
        cmpi_mmz_free(pool->supplement_no_cache_phy, pool->supplement_no_cache_vir);
        return OT_ERR_VB_NO_MEM;
    }
    return TD_SUCCESS;
}

static td_s32 vb_create_supplement_vb_pool(const vb_pool_common_info *info, vb_pool_attr *pool,
    const td_char *mmz_name, vb_create_pool_size_set *pool_size)
{
    td_s32 ret;

    if (info->owner == OT_POOL_OWNER_PRIVATE && info->uid != OT_VB_UID_VDEC) {
        return TD_SUCCESS;
    }
    vb_pool_size_init(pool_size, info);

    ret = vb_create_supplement_no_cache(info, pool, mmz_name, pool_size);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return vb_create_supplement_cached(info, pool, mmz_name, pool_size);
}

static td_void vb_init_pool_common_attr(const vb_pool_common_info *info, vb_pool_attr *pool,
    vb_blk_info *blk, const vb_create_pool_size_set *pool_size_set)
{
    /* initialize this pool structure */
    pool->blk = blk;
    pool->blk_cnt = info->blk_cnt;
    pool->blk_size = pool_size_set->align_blk_size;
    pool->supplement_cached_size = pool_size_set->align_sup_cached_size;
    pool->supplement_no_cached_size = pool_size_set->align_sup_no_cache_size;


#endif /* SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART01_H_ */
