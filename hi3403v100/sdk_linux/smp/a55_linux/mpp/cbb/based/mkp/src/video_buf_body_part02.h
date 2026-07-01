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

#ifndef SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART02_H_
#define SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART02_H_

    pool->pool_size = pool_size_set->align_blk_size * info->blk_cnt;
    pool->free_blk_cnt = info->blk_cnt;
    pool->min_free_blk_cnt = info->blk_cnt;
    pool->vb_remap_mode = info->vb_remap_mode;
    (td_void)memset_s(pool->user_cnt_sum, sizeof(pool->user_cnt_sum), 0, sizeof(pool->user_cnt_sum));

    OSAL_INIT_LIST_HEAD(&pool->busy);
    OSAL_INIT_LIST_HEAD(&pool->free_list);

    pool->user          = (info->owner == OT_POOL_OWNER_USER);
    pool->is_comm_pool  = (info->owner == OT_POOL_OWNER_COMMON || info->owner == OT_POOL_OWNER_MODULE);
    pool->pool_owner = info->owner;
    pool->uid = info->uid;
}

static td_s32 vb_init_pool_attr(const vb_pool_common_info *info, vb_pool_attr *pool, vb_blk_info *blk,
    const td_char *pc_mmz_name, const vb_create_pool_size_set *pool_size_set)
{
    td_u32 j;
    vb_blk_info *blk_tmp = TD_NULL;

    /* initialize this pool structure */
    vb_init_pool_common_attr(info, pool, blk, pool_size_set);

    /* initialize all buffer block structure */
    blk_tmp = blk;

    for (j = 0; j < info->blk_cnt; j++, blk_tmp++) {
        osal_list_add_tail(&blk_tmp->list, &pool->free_list);

        blk_tmp->blk_id = j;
        blk_tmp->phys_addr = (td_phys_addr_t)(pool->pool_phy_addr + j * pool_size_set->align_blk_size);
        if (info->vb_remap_mode == OT_VB_REMAP_MODE_NONE) {
            blk_tmp->vir_addr = TD_NULL;
        } else {
            blk_tmp->vir_addr = (td_void *)((td_u8 *)pool->pool_vir_addr + j * pool_size_set->align_blk_size);
        }
        blk_tmp->sum_cnt = 0;
        (td_void)memset_s(blk_tmp->user_cnt, sizeof(blk_tmp->user_cnt), 0, sizeof(blk_tmp->user_cnt));
        blk_tmp->blk_size = pool_size_set->align_blk_size;

        (td_void)memset_s(&blk_tmp->supplement, sizeof(ot_video_supplement), 0, sizeof(ot_video_supplement));

        if (pool_size_set->align_sup_no_cache_size) {
            vb_value_supplement_no_cache_addr((td_phys_addr_t)(pool->supplement_no_cache_phy + j * (td_u64)
                pool_size_set->align_sup_no_cache_blk_size), (td_void *)((td_u8 *)pool->supplement_no_cache_vir +
                j * (td_u64)pool_size_set->align_sup_no_cache_blk_size), &blk_tmp->supplement, &g_supplement_conf);
        }

        if (pool_size_set->align_sup_cached_size) {
            vb_value_supplement_cached_addr((td_phys_addr_t)(pool->supplement_cached_phy + j *
                (td_u64)pool_size_set->align_sup_cached_blk_size), (td_void *)((td_u8 *)pool->supplement_cached_vir +
                j * (td_u64)pool_size_set->align_sup_cached_blk_size), &blk_tmp->supplement, &g_supplement_conf);
        }
    }

    if (pc_mmz_name != TD_NULL) {
        if (strncpy_s(pool->ac_pool_name, sizeof(pool->ac_pool_name), pc_mmz_name,
            sizeof(pool->ac_pool_name) - 1) != EOK) {
            ot_trace_vb(OT_DBG_ERR, "copy err!\n");
            return OT_ERR_VB_NO_MEM;
        }
    }

    pool->state = VB_POOL_IDLE;
    pool->async_flag = TD_FALSE;
    return TD_SUCCESS;
}

#ifdef CONFIG_OT_VB_LOG_SUPPORT
static td_void vb_log_record(const vb_blk_info *blk, const vb_pool_attr *pool_attr, td_u32 uid, td_bool opt, td_u32 chn)
{
    vb_analysisy_export_func *pfn_vb_analysisy_exp_func = TD_NULL;
    if (pool_attr->pool_owner == OT_POOL_OWNER_PRIVATE) {
        return;
    }
    pfn_vb_analysisy_exp_func = func_entry(vb_analysisy_export_func, OT_ID_VB_LOG);
    if (pfn_vb_analysisy_exp_func != TD_NULL && ckfn_vb_analysisy_vb_log_record()) {
        call_vb_analysisy_vb_log_record(pool_attr->pool_id, blk->blk_id, uid, opt, chn);
    }
}

static td_void vb_save_seg_log(const vb_blk_info *blk, td_s32 pool_owner, td_u32 pool_id)
{
    vb_analysisy_export_func *pfn_vb_analysisy_exp_func = TD_NULL;
    if (pool_owner == OT_POOL_OWNER_PRIVATE) {
        return;
    }
    pfn_vb_analysisy_exp_func = func_entry(vb_analysisy_export_func, OT_ID_VB_LOG);
    if (pfn_vb_analysisy_exp_func != TD_NULL && ckfn_vb_analysisy_vb_save_seg()) {
        call_vb_analysisy_vb_save_seg(pool_id, blk->blk_id);
    }
}

static td_void vb_create_pool_log(const vb_pool_info *pool_info)
{
    vb_analysisy_export_func *pfn_vb_analysisy_exp_func = func_entry(vb_analysisy_export_func, OT_ID_VB_LOG);
    if (pfn_vb_analysisy_exp_func != TD_NULL && ckfn_vb_analysisy_create_pool_log()) {
        call_vb_analysisy_create_pool_log(pool_info);
    }
}

static td_void vb_destroy_pool_log(td_u32 pool_id)
{
    vb_analysisy_export_func *pfn_vb_analysisy_exp_func = func_entry(vb_analysisy_export_func, OT_ID_VB_LOG);
    if (pfn_vb_analysisy_exp_func != TD_NULL && ckfn_vb_analysisy_destroy_pool_log()) {
        call_vb_analysisy_destroy_pool_log(pool_id);
    }
}
#endif

static td_s32 vb_search_and_set_pool(const vb_pool_common_info *info, vb_pool_attr *pool,
    vb_blk_info *blk, td_u32 *pool_id)
{
    td_u32 i;
    unsigned long flags;

    /* search a free_list pool */
    for (i = 0; i < g_vb_conf.max_pool_cnt; i++) {
        if (g_pools[i] == TD_NULL) {
            break;
        }
    }

    if (i >= g_vb_conf.max_pool_cnt) {
        ot_trace_vb(OT_DBG_ERR, "too many pools!\n");
        return OT_ERR_VB_NO_MEM;
    }

    if (info->owner == OT_POOL_OWNER_COMMON || info->owner == OT_POOL_OWNER_MODULE) {
        osal_list_add_tail(&pool->list, &g_comm_pools);
    }

    *pool_id = i;
    pool->pool_id = i;

    /* finally set g_pools and g_blks */
    vb_spin_lock(flags);
    g_pools[i] = pool;
    g_blks[i] = blk;
    vb_spin_unlock(flags);
    ot_trace_vb(OT_DBG_DEBUG, "created pool %u!\n", i);
    return TD_SUCCESS;
}

static td_s32 vb_creat_pool_process(const vb_pool_common_info *info, const td_char *mmz_name,
    vb_pool_attr *pool, vb_blk_info *blk)
{
    td_s32 ret;
    const td_char *pc_mmz_name = "\0";
    vb_create_pool_size_set pool_size_set = {0};

    /* alloc memory from MMZ */
    if (mmz_name == NULL) {
        pc_mmz_name = NULL;
    } else {
        if (osal_strcmp(pc_mmz_name, mmz_name) != 0) {
            pc_mmz_name = mmz_name;
        } else {
            pc_mmz_name = NULL;
        }
    }
    ret = vb_creat_common_vb_pool(info, pool, pc_mmz_name, &pool_size_set);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = vb_create_supplement_vb_pool(info, pool, pc_mmz_name, &pool_size_set);
    if (ret != TD_SUCCESS) {
        goto mmz_failed_1;
    }

    ret = vb_init_pool_attr(info, pool, blk, pc_mmz_name, &pool_size_set);
    if (ret != TD_SUCCESS) {
        goto mmz_failed_2;
    }

    return TD_SUCCESS;

mmz_failed_2:
    cmpi_mmz_free(pool->supplement_cached_phy, pool->supplement_cached_vir);
    cmpi_mmz_free(pool->supplement_no_cache_phy, pool->supplement_no_cache_vir);
mmz_failed_1:
    cmpi_mmz_free(pool->pool_phy_addr, pool->pool_vir_addr);

    return ret;
}

static td_s32 create_pool(const vb_pool_common_info *info, td_u32 *pool_id, const td_char *mmz_name)
{
    td_s32 ret;
    vb_blk_info *blk = TD_NULL;
    vb_pool_attr *pool = TD_NULL;

    ret = vb_check_create_pool_param(info, pool_id);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /* first alloc resources for pool, then search a free_list pool */
    pool = (vb_pool_attr *)osal_kmalloc(sizeof(*pool), osal_gfp_kernel);
    blk = (vb_blk_info *)osal_kmalloc(sizeof(*blk) * info->blk_cnt, osal_gfp_kernel);
    if ((pool == TD_NULL) || (blk == TD_NULL)) {
        ot_trace_vb(OT_DBG_EMERG, "failed to malloc memory(pool=%p,blk=%p,info->blk_cnt=%u,size=%llu)\n",
                    pool, blk, info->blk_cnt, info->blk_size);
        ret = OT_ERR_VB_NO_MEM;
        goto malloc_failed;
    }

    (td_void)memset_s(pool, sizeof(*pool), 0, sizeof(*pool));
    (td_void)memset_s(blk, sizeof(*blk) * info->blk_cnt, 0, sizeof(*blk) * info->blk_cnt);

    /* alloc memory from MMZ */
    ret = vb_creat_pool_process(info, mmz_name, pool, blk);
    if (ret != TD_SUCCESS) {
        goto malloc_failed;
    }

    ret = vb_search_and_set_pool(info, pool, blk, pool_id);
    if (ret != TD_SUCCESS) {
        goto mmz_failed;
    }

#ifdef CONFIG_OT_VB_LOG_SUPPORT
    {
        vb_pool_info pool_info = {0};
        pool_info.pool_id = pool->pool_id;
        pool_info.pool_info.blk_cnt = pool->blk_cnt;
        pool_info.pool_info.blk_size = pool->blk_size;
        pool_info.pool_info.pool_phy_addr = pool->pool_phy_addr;
        vb_create_pool_log(&pool_info);
    }
#endif
    return TD_SUCCESS;

mmz_failed:
    cmpi_mmz_free(pool->supplement_cached_phy, pool->supplement_cached_vir);
    cmpi_mmz_free(pool->supplement_no_cache_phy, pool->supplement_no_cache_vir);
    cmpi_mmz_free(pool->pool_phy_addr, pool->pool_vir_addr);
malloc_failed:
    if (pool != TD_NULL) {
        osal_kfree(pool);
    }
    if (blk != TD_NULL) {
        osal_kfree(blk);
    }

    return ret;
}

td_s32 vb_create_pool(td_u32 *pool_id, const td_char *pc_mmz_name, const vb_info *info)
{
    td_s32 ret;
    vb_pool_common_info common_info;

    vb_check_initialization_status_return(OT_ERR_VB_NOT_READY);
    if (info == TD_NULL) {
        ot_trace_vb(OT_DBG_EMERG, "vb info NULL!\n");
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if (osal_down(&g_sema)) {
        return -ERESTARTSYS;
    }
    common_info.blk_cnt         = info->blk_cnt;
    common_info.blk_size        = info->blk_size;
    common_info.uid             = info->uid;
    common_info.vb_remap_mode   = info->vb_remap_mode;
    common_info.owner = OT_POOL_OWNER_PRIVATE;
    if (strncpy_s(common_info.buf_name, OT_MAX_MMZ_NAME_LEN, info->buf_name, OT_MAX_MMZ_NAME_LEN - 1) != EOK) {
        ot_trace_vb(OT_DBG_ERR, "memcpy_s fail!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }
    ret = create_pool(&common_info, pool_id, pc_mmz_name);
    osal_up(&g_sema);

    return ret;
}

#ifdef CONFIG_OT_VB_ASYNC_SUPPORT
static td_void vb_zombie_pool_list_add_tail(struct osal_list_head *list)
{
    osal_list_add_tail(list, &g_zombie_state_pools);
}

static td_void vb_async_destroy_thread_wakeup(td_void)
{
    osal_wakeup(&g_vb_async_destroy_wait);
}

static td_bool vb_is_zombie_pools_need_destroy(td_void)
{
    unsigned long flags;
    vb_pool_attr *pool = TD_NULL;

    vb_spin_lock(flags);
    osal_list_for_each_entry(pool, &g_zombie_state_pools, list) {
        if (pool->blk_cnt == pool->free_blk_cnt) {
            vb_spin_unlock(flags);
            return TD_TRUE;
        }
    }
    vb_spin_unlock(flags);
    return TD_FALSE;
}

static int vb_async_destroy_wait_condition(const void *param)
{
    ot_unused(param);
    return (osal_kthread_should_stop() || vb_is_zombie_pools_need_destroy() == TD_TRUE);
}
#endif

static td_void vb_reset_single_pools_and_blks(td_u32 pool_id)
{
    g_blks[pool_id] = TD_NULL;
    g_pools[pool_id] = TD_NULL;
}

static td_void vb_release_pool_resource(vb_pool_attr *pool)
{
    if (pool->is_comm_pool != TD_FALSE) {
        osal_list_del(&pool->list);
    }

    if (pool->supplement_cached_vir != TD_NULL) {
        osal_flush_dcache_area(pool->supplement_cached_vir, (td_ulong)pool->supplement_cached_phy,
            pool->supplement_cached_size);
    }
    cmpi_mmz_free(pool->supplement_cached_phy, pool->supplement_cached_vir);
    cmpi_mmz_free(pool->supplement_no_cache_phy, pool->supplement_no_cache_vir);
    cmpi_mmz_free(pool->pool_phy_addr, pool->pool_vir_addr);

    osal_kfree(pool->blk);
    osal_kfree(pool);

#ifdef CONFIG_OT_VB_LOG_SUPPORT
    vb_destroy_pool_log(pool_id);
#endif
}

static td_s32 destroy_pool(vb_pool_attr *pool)
{
    unsigned long flags;
    td_u32 pool_id = pool->pool_id;

    vb_spin_lock(flags);
    /* avoid destroying pool while someone using VB which belong to this pool */
    if (pool->blk_cnt != pool->free_blk_cnt) {
#ifdef CONFIG_OT_VB_ASYNC_SUPPORT
        if (pool->async_flag && pool->state == VB_POOL_IDLE) {
            pool->state = VB_POOL_ZOMBIE;
            vb_zombie_pool_list_add_tail(&pool->list);
            ot_trace_vb(OT_DBG_DEBUG, "pool(%u) trigger vb async destroy.\n", pool_id);
            vb_spin_unlock(flags);
            return TD_SUCCESS;
        }
#endif

        ot_trace_vb(OT_DBG_ERR, "blk in this pool(%d) occupied by someone, please release first!\n", pool_id);

        if (!g_vb_force_exit) {
            vb_spin_unlock(flags);
            return OT_ERR_VB_BUSY;
        }
    }
    pool->async_flag = TD_FALSE;

    if (pool->state == VB_POOL_IDLE || pool->state == VB_POOL_ZOMBIE) {
        pool->state = VB_POOL_DEAD;
    } else if (pool->state == VB_POOL_DEAD) {
        /* avoid destroying the same pool in different process */
        vb_spin_unlock(flags);
        return TD_FAILURE;
    }
    vb_reset_single_pools_and_blks(pool_id);
    vb_spin_unlock(flags);

    vb_release_pool_resource(pool);
    ot_trace_vb(OT_DBG_DEBUG, "destroyed pool %u!\n", pool_id);
    return TD_SUCCESS;
}

td_s32 vb_destroy_pool(td_u32 pool_id)
{
    td_s32 ret;
    vb_pool_attr *pool = TD_NULL;

    vb_check_initialization_status_return(OT_ERR_VB_NOT_READY);
    vb_check_pool_id_return(pool_id, OT_ERR_VB_ILLEGAL_PARAM);

    ret = osal_down(&g_sema);
    if (ret) {
        return -ERESTARTSYS;
    }

    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        osal_up(&g_sema);
        ot_trace_vb(OT_DBG_ERR, "null ptr! pool %u\n", pool_id);
        return TD_FAILURE;
    }

    ret = destroy_pool(pool);
    osal_up(&g_sema);

    return ret;
}


#endif /* SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART02_H_ */
