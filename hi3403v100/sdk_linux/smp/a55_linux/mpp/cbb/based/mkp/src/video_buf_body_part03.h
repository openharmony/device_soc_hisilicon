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

#ifndef SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART03_H_
#define SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART03_H_

#ifdef CONFIG_OT_VB_ASYNC_SUPPORT
static int vb_async_destroy_thread(void *data)
{
    unsigned long flags;
    td_s32 ret;
    vb_pool_attr *pool = TD_NULL;
    vb_pool_attr *next_pool = TD_NULL;

    ot_unused(data);

    do {
        ret = osal_wait_event_interruptible(&g_vb_async_destroy_wait, vb_async_destroy_wait_condition, TD_NULL);
        if (ret == -ERESTARTSYS) {
            ot_trace_vb(OT_DBG_INFO, "interrupted by a signal!\n");
        } else if (ret != 0) {
            ot_trace_vb(OT_DBG_ERR, "wait interruptible failed!\n");
        } else {
            ot_trace_vb(OT_DBG_DEBUG, "wait success.\n");
        }

        vb_spin_lock(flags);
        osal_list_for_each_entry_safe(pool, next_pool, &g_zombie_state_pools, list) {
            if (pool->blk_cnt != pool->free_blk_cnt) {
                continue;
            }
            osal_list_del(&pool->list);
            pool->async_flag = TD_FALSE;
            pool->state = VB_POOL_DEAD;
            vb_reset_single_pools_and_blks(pool->pool_id);
            vb_spin_unlock(flags);
            ot_trace_vb(OT_DBG_DEBUG, "vb pool(%u) was asynchronously destroyed!\n", pool->pool_id);
            vb_release_pool_resource(pool);
            vb_spin_lock(flags);
        }
        vb_spin_unlock(flags);
    } while (!osal_kthread_should_stop());

    return TD_SUCCESS;
}

static td_s32 vb_async_destroy_ctx_init(td_void)
{
    if (osal_wait_init(&g_vb_async_destroy_wait) < 0) {
        ot_trace_vb(OT_DBG_ERR, "vb wait init failed!\n");
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    OSAL_INIT_LIST_HEAD(&g_zombie_state_pools);

    g_vb_async_destroy_td = osal_kthread_create(vb_async_destroy_thread, TD_NULL, "vb_thread");
    if (g_vb_async_destroy_td == TD_NULL) {
        osal_wait_destroy(&g_vb_async_destroy_wait);
        ot_trace_vb(OT_DBG_ERR, "create vb thread failed!\n");
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_void vb_async_destroy_ctx_deinit(td_void)
{
    osal_kthread_destroy(g_vb_async_destroy_td, 1);

    osal_wait_destroy(&g_vb_async_destroy_wait);
}

td_s32 vb_async_destroy_pool(td_u32 pool_id)
{
    td_s32 ret;
    vb_pool_attr *pool = TD_NULL;
    unsigned long flags;

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

    vb_spin_lock(flags);
    if (pool->state == VB_POOL_IDLE && pool->pool_owner == OT_POOL_OWNER_PRIVATE) {
        pool->async_flag = TD_TRUE;
        ot_trace_vb(OT_DBG_DEBUG, "pool(%u) set async destroy success.\n", pool_id);
    } else {
        vb_spin_unlock(flags);
        osal_up(&g_sema);
        ot_trace_vb(OT_DBG_ERR, "async destroy pool %u must be init\n", pool_id);
        return TD_FAILURE;
    }
    vb_spin_unlock(flags);

    ret = destroy_pool(pool);
    osal_up(&g_sema);

    return ret;
}
#endif

static td_void vb_init_low_delay_info(vb_blk_info *blk)
{
    low_delay_info *info = NULL;

    if (blk->supplement.low_delay_virt_addr != TD_NULL) {
        info = (low_delay_info *)(blk->supplement.low_delay_virt_addr);
        info->tunl_frame = TD_FALSE;
        info->y_height = 0;
        info->c_height = 0;
        info->frame_finish = TD_FALSE;
        info->frame_ok = TD_FALSE;
        info->miss_vedu = 0;
        info->miss_jpege = 0;
        info->buf_line = 0;
        info->all_online = 0;
        info->csize = 0;
        info->ysize = 0;
    }

    return;
}

static td_void vb_init_mot_info(vb_blk_info *blk)
{
    if (blk->supplement.bnr_mot_virt_addr != TD_NULL) {
        (td_void)memset_s(blk->supplement.bnr_mot_virt_addr, sizeof(bnr_mot_info), 0, sizeof(bnr_mot_info));
    }
}

static td_void vb_init_misc_info(const vb_blk_info *blk)
{
    if (blk->supplement.misc_info_virt_addr != TD_NULL) {
        ot_video_supplement_misc *misc_info = (ot_video_supplement_misc *)blk->supplement.misc_info_virt_addr;
        (td_void)memset_s(misc_info, sizeof(ot_video_supplement_misc), 0, sizeof(ot_video_supplement_misc));
    }
}

static vb_blk_handle vb_get_blk_handle_by_pool_id(td_u32 pool_id, td_u32 uid, td_u32 chn)
{
    vb_blk_info *blk = TD_NULL;
    vb_pool_attr *pool = TD_NULL;

    vb_check_pool_id_return(pool_id, OT_VB_INVALID_HANDLE);

    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        ot_trace_vb(OT_DBG_ERR, "pool %u is already destroyed !\n", pool_id);
        return OT_VB_INVALID_HANDLE;
    }
    if (pool->state == VB_POOL_DEAD) {
        ot_trace_vb(OT_DBG_ERR, "pool %u is dead !\n", pool_id);
        return OT_VB_INVALID_HANDLE;
    }

    if (osal_list_empty(&pool->free_list)) {
        ot_trace_vb(OT_DBG_WARN, "no free_list buffer in pool %u!\n", pool_id);
        return OT_VB_INVALID_HANDLE;
    }
    ot_assert(pool->free_blk_cnt != 0);
    pool->free_blk_cnt--;
    if (pool->free_blk_cnt < pool->min_free_blk_cnt) {
        pool->min_free_blk_cnt = pool->free_blk_cnt;
    }

    blk = osal_list_entry(pool->free_list.next, vb_blk_info, list);
    osal_list_del(pool->free_list.next);
    osal_list_add_tail(&blk->list, &pool->busy);

    vb_init_low_delay_info(blk);
    vb_init_mot_info(blk);
    vb_init_misc_info(blk);
    blk->user_cnt[uid]++;
    blk->sum_cnt++;
#ifdef CONFIG_OT_VB_LOG_SUPPORT
    vb_log_record(blk, pool, uid, 1, chn);
#else
    ot_unused(chn);
#endif

    ot_trace_vb(OT_DBG_DEBUG, "Uid: [%u] blk [%u] in pool[%u]:is allocated!\n",
                uid, blk->blk_id, pool_id);

    return vb_mk_handle(pool_id, blk->blk_id);
}

vb_blk_handle vb_get_blk_by_pool_id(td_u32 pool_id, td_u32 uid)
{
    unsigned long flags;
    vb_blk_handle handle;

    vb_check_initialization_status_return(OT_VB_INVALID_HANDLE);
    vb_check_user_id_return(uid, OT_VB_INVALID_HANDLE);
    vb_check_pool_id_return(pool_id, OT_VB_INVALID_HANDLE);

    /* when called in process context, we do this */
    vb_spin_lock(flags);
    handle = vb_get_blk_handle_by_pool_id(pool_id, uid, CHN_INFO_NONE);
    vb_spin_unlock(flags);
    return handle;
}

static td_s32 vb_search_comm_pool_by_size(td_u32 *pool_id, td_u64 blk_size, const td_char *pc_mmz_name,
    td_s32 owner, ot_vb_uid uid)
{
    td_u64 min_size = -1ULL;
    const td_char *pc_ret_name = TD_NULL;
    vb_pool_attr *n = TD_NULL;
    vb_pool_attr *pool = TD_NULL;
    struct osal_list_head *pos = TD_NULL;
    td_bool blk_size_enough = TD_FALSE;

    vb_check_initialization_status_return(OT_ERR_VB_NOT_READY);
    if (pc_mmz_name == TD_NULL) {
        pc_ret_name = "\0";
    } else {
        pc_ret_name = pc_mmz_name;
    }

    osal_list_for_each(pos, &g_comm_pools) {
        n = osal_list_entry(pos, vb_pool_attr, list);
        ot_assert(n->is_comm_pool == TD_TRUE);

        if (n->pool_owner != owner || n->uid != uid) {
            continue;
        }

        if ((n->blk_size < blk_size) || (n->blk_size > min_size) ||
            ((pc_mmz_name != TD_NULL) && (osal_strcmp(n->ac_pool_name, pc_ret_name) != 0))) {
            continue;
        }
        blk_size_enough = TD_TRUE;

        if (n->free_blk_cnt == 0) {
            continue;
        }

        min_size = n->blk_size;
        pool = n;
        if (blk_size == min_size) {
            break;
        }
    }

    if (blk_size_enough == TD_FALSE) {
        *pool_id = OT_VB_INVALID_POOL_ID;
        ot_trace_vb(OT_DBG_ERR, "all of vb pool(owner %d) is smaller than the actual size %llu !\n", owner, blk_size);
        return OT_ERR_VB_SIZE_NOT_ENOUGH;
    }

    if (pool != TD_NULL) {
        *pool_id = pool->pool_id;
        return TD_SUCCESS;
    } else {
        ot_trace_vb(OT_DBG_DEBUG, "no fit common pool!\n");
        *pool_id = OT_VB_INVALID_POOL_ID;
        return OT_ERR_VB_NO_BUF;
    }
}

td_s32 vb_get_pool_id_by_size_and_module(td_u64 blk_size, td_u32 *pool_id, const td_char *pc_mmz_name, ot_vb_uid uid)
{
    vb_check_initialization_status_return(OT_ERR_VB_NOT_READY);
    vb_check_user_id_return(uid, OT_ERR_VB_ILLEGAL_PARAM);
    ot_unused(pc_mmz_name);

    if (pool_id == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "null point !\n");
        return OT_ERR_VB_NULL_PTR;
    }

    return vb_search_comm_pool_by_size(pool_id, blk_size, TD_NULL, OT_POOL_OWNER_MODULE, uid);
}

td_s32 vb_get_pool_id(td_u64 blk_size, td_u32 *pool_id, const td_char *pc_mmz_name, ot_vb_uid uid)
{
    vb_check_initialization_status_return(OT_ERR_VB_NOT_READY);
    vb_check_user_id_return(uid, OT_ERR_VB_ILLEGAL_PARAM);

    if (pool_id == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "null point !\n");
        return OT_ERR_VB_NULL_PTR;
    }

    if (uid != OT_VB_UID_USER) {
        return vb_search_comm_pool_by_size(pool_id, blk_size, TD_NULL, OT_POOL_OWNER_COMMON, OT_VB_UID_COMMON);
    } else {
        return vb_search_comm_pool_by_size(pool_id, blk_size, pc_mmz_name, OT_POOL_OWNER_COMMON, OT_VB_UID_COMMON);
    }
}

vb_blk_handle vb_get_blk_by_size_ex(td_u64 blk_size, ot_vb_uid uid, const td_char *pc_mmz_name, td_u32 chn)
{
    td_u32 pool_id;
    td_s32 ret;
    unsigned long flags;
    vb_blk_handle handle;

    vb_check_initialization_status_return(OT_VB_INVALID_HANDLE);
    vb_check_user_id_return(uid, OT_VB_INVALID_HANDLE);

    vb_spin_lock(flags);
    if (uid != OT_VB_UID_USER) {
        ret = vb_search_comm_pool_by_size(&pool_id, blk_size, TD_NULL, OT_POOL_OWNER_COMMON, OT_VB_UID_COMMON);
    } else {
        ret = vb_search_comm_pool_by_size(&pool_id, blk_size, pc_mmz_name, OT_POOL_OWNER_COMMON, OT_VB_UID_COMMON);
    }
    if (ret != TD_SUCCESS) {
        vb_spin_unlock(flags);
        return OT_VB_INVALID_HANDLE;
    }

    handle = vb_get_blk_handle_by_pool_id(pool_id, uid, chn);
    vb_spin_unlock(flags);
    return handle;
}

vb_blk_handle vb_get_blk_by_size(td_u64 blk_size, ot_vb_uid uid, const td_char *pc_mmz_name)
{
    return vb_get_blk_by_size_ex(blk_size, uid, pc_mmz_name, CHN_INFO_NONE);
}

td_s32 vb_get_pool_info(vb_pool_info *info)
{
    td_u32 pool_id;
    unsigned long flags;
    vb_pool_attr *pool = TD_NULL;

    if (info == TD_NULL) {
        ot_trace_vb(OT_DBG_ERR, "info is null!\n");
        return OT_ERR_VB_NULL_PTR;
    }

    vb_check_initialization_status_return(OT_ERR_VB_NOT_READY);
    vb_check_pool_id_return(info->pool_id, OT_VB_INVALID_POOL_ID);

    pool_id = info->pool_id;

    vb_spin_lock(flags);
    pool = g_pools[pool_id];
    if (pool == NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool %u is already destroyed !\n", pool_id);
        vb_spin_unlock(flags);
        return OT_VB_INVALID_POOL_ID;
    }

    info->pool_info.pool_size = pool->pool_size;
    info->pool_info.pool_phy_addr = pool->pool_phy_addr;
    info->pool_info.blk_cnt = pool->blk_cnt;
    info->pool_info.blk_size = pool->blk_size;
    info->pool_info.remap_mode = pool->vb_remap_mode;
    info->is_user_vb = pool->user;
    vb_spin_unlock(flags);
    return TD_SUCCESS;
}

td_s32 vb_put_blk(td_u32 pool_id, td_phys_addr_t phys_addr)
{
    unsigned long flags;
    vb_blk_info *blk = TD_NULL;
    vb_pool_attr *pool = TD_NULL;

    vb_check_initialization_status_return(OT_ERR_VB_NOT_READY);
    vb_check_pool_id_return(pool_id, OT_ERR_VB_ILLEGAL_PARAM);

    vb_spin_lock(flags);
    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        vb_spin_unlock(flags);
        return TD_FAILURE;
    }
    if (pool->state == VB_POOL_DEAD) {
        vb_spin_unlock(flags);
        return TD_FAILURE;
    }

    if (vb_check_phy_addr(pool, phys_addr)) {
        vb_spin_unlock(flags);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    blk = pool->blk + osal_div64_u64(phys_addr - pool->pool_phy_addr, pool->blk_size);
    if (!blk->sum_cnt) {
        vb_spin_unlock(flags);
        ot_trace_vb(OT_DBG_WARN, "try to put a free_list buffer block!\n");
        return OT_ERR_VB_ILLEGAL_PARAM;
    }
    (td_void)memset_s(blk->user_cnt, sizeof(blk->user_cnt), 0, sizeof(blk->user_cnt));
    blk->sum_cnt = 0;
    pool->free_blk_cnt++;

#ifdef CONFIG_OT_VB_LOG_SUPPORT
    vb_save_seg_log(blk, OT_POOL_OWNER_COMMON, pool_id);
#endif

    osal_list_del(&blk->list);
    osal_list_add_tail(&blk->list, &pool->free_list);

    vb_spin_unlock(flags);
    return TD_SUCCESS;
}

vb_blk_handle vb_phy_to_handle(td_phys_addr_t phy_addr)
{
    td_u32 pool_id;
    td_u32 blk_id;
    vb_pool_attr *pool = TD_NULL;
    unsigned long flags;

    vb_check_initialization_status_return(OT_VB_INVALID_HANDLE);

    vb_spin_lock(flags);

    /* search the pool_id */
    for (pool_id = 0; pool_id < g_vb_conf.max_pool_cnt; pool_id++) {
        pool = g_pools[pool_id];
        if (pool == TD_NULL) {
            continue;
        }

        if ((pool->pool_phy_addr <= phy_addr) &&
            ((pool->pool_phy_addr + pool->pool_size) > phy_addr)) {
                /* caculte the blk_id */
            blk_id = (td_u32)osal_div64_u64(phy_addr - pool->pool_phy_addr, pool->blk_size);
            ot_assert(blk_id < pool->blk_cnt);


#endif /* SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART03_H_ */
