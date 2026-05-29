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

#ifndef SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART04_H_
#define SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART04_H_


            vb_spin_unlock(flags);

            return vb_mk_handle(pool_id, blk_id);
        }
    }

    vb_spin_unlock(flags);

    return OT_VB_INVALID_HANDLE;
}

td_u32 vb_handle_to_pool_id(vb_blk_handle handle)
{
    vb_check_initialization_status_return(OT_VB_INVALID_POOL_ID);
    vb_check_pool_id_return(vb_hdle_to_pool_id(handle), OT_VB_INVALID_POOL_ID);

    return vb_hdle_to_pool_id(handle);
}

td_u32 vb_handle_to_blk_id(vb_blk_handle handle)
{
    vb_check_initialization_status_return(OT_VB_INVALID_POOL_ID);
    vb_check_pool_id_return(vb_hdle_to_pool_id(handle), OT_VB_INVALID_POOL_ID);

    return vb_hdle_to_blk_id(handle);
}

td_phys_addr_t vb_handle_to_phys(vb_blk_handle handle)
{
    td_s32 ret;
    unsigned long flags;
    td_phys_addr_t phys_addr;
    td_u32 pool_id = vb_hdle_to_pool_id(handle);
    td_u32 blk_id = vb_hdle_to_blk_id(handle);
    vb_check_initialization_status_return(0);
    vb_check_pool_id_return(pool_id, 0);

    vb_spin_lock(flags);
    if (g_pools[pool_id] == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool %u is already destroyed !\n", pool_id);
        vb_spin_unlock(flags);
        return 0;
    }

    if ((g_blks[pool_id] + blk_id) == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool [%u] doesn't have blocks! \n", pool_id);
        vb_spin_unlock(flags);
        return 0;
    }

    ret = vb_check_block_id(pool_id, blk_id);
    if (ret != TD_SUCCESS) {
        vb_spin_unlock(flags);
        return 0;
    }

    phys_addr = (td_phys_addr_t)(td_uintptr_t)((g_blks[pool_id] + blk_id)->phys_addr);
    vb_spin_unlock(flags);
    return phys_addr;
}

td_phys_addr_t vb_handle_to_kern(vb_blk_handle handle)
{
    td_s32 ret;
    unsigned long flags;
    td_phys_addr_t vir_addr;
    td_u32 pool_id = vb_hdle_to_pool_id(handle);
    td_u32 blk_id = vb_hdle_to_blk_id(handle);
    vb_check_initialization_status_return(0);
    vb_check_pool_id_return(pool_id, 0);

    vb_spin_lock(flags);
    if (g_pools[pool_id] == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool %u is already destroyed !\n", pool_id);
        vb_spin_unlock(flags);
        return 0;
    }

    if ((g_blks[pool_id] + blk_id) == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool [%u] doesn't have blocks! \n", pool_id);
        vb_spin_unlock(flags);
        return 0;
    }

    ret = vb_check_block_id(pool_id, blk_id);
    if (ret != TD_SUCCESS) {
        vb_spin_unlock(flags);
        return 0;
    }

    vir_addr = (td_ulong)(td_uintptr_t)((g_blks[pool_id] + blk_id)->vir_addr);
    vb_spin_unlock(flags);
    return vir_addr;
}

td_u64 vb_handle_to_blk_size(vb_blk_handle handle)
{
    td_s32 ret;
    unsigned long flags;
    td_u64 blk_size;
    td_u32 pool_id = vb_hdle_to_pool_id(handle);
    td_u32 blk_id = vb_hdle_to_blk_id(handle);
    vb_check_initialization_status_return(0);
    vb_check_pool_id_return(pool_id, 0);

    vb_spin_lock(flags);
    if (g_pools[pool_id] == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool %u is already destroyed !\n", pool_id);
        vb_spin_unlock(flags);
        return 0;
    }

    if ((g_blks[pool_id] + blk_id) == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool [%u] doesn't have blocks! \n", pool_id);
        vb_spin_unlock(flags);
        return 0;
    }

    ret = vb_check_block_id(pool_id, blk_id);
    if (ret != TD_SUCCESS) {
        vb_spin_unlock(flags);
        return 0;
    }

    blk_size = (td_ulong)(td_uintptr_t)((g_blks[pool_id] + blk_id)->blk_size);
    vb_spin_unlock(flags);
    return blk_size;
}

td_s32 vb_user_add(td_u32 pool_id, td_phys_addr_t phys_addr, td_u32 uid)
{
    unsigned long flags;
    vb_blk_info *blk = TD_NULL;
    const vb_pool_attr *pool = TD_NULL;

    vb_check_initialization_status_return(OT_ERR_VB_NOT_READY);
    vb_check_user_id_return(uid, OT_ERR_VB_ILLEGAL_PARAM);
    vb_check_pool_id_return(pool_id, OT_ERR_VB_ILLEGAL_PARAM);

    vb_spin_lock(flags);
    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool %u is already destroyed !\n", pool_id);
        vb_spin_unlock(flags);
        return OT_ERR_VB_NULL_PTR;
    }
    if (pool->state == VB_POOL_DEAD) {
        ot_trace_vb(OT_DBG_WARN, "pool %u is dead !\n", pool_id);
        vb_spin_unlock(flags);
        return OT_ERR_VB_UNEXIST;
    }

    if (vb_check_phy_addr(pool, phys_addr)) {
        vb_spin_unlock(flags);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    blk = pool->blk + osal_div64_u64(phys_addr - pool->pool_phy_addr, pool->blk_size);
    if ((blk->sum_cnt <= 0)) {
        vb_spin_unlock(flags);
        ot_trace_vb(OT_DBG_WARN, "try to increase counter for a free_list buffer!\n");
        return OT_ERR_VB_NOT_PERM;
    }
    blk->user_cnt[uid]++;
    blk->sum_cnt++;

#ifdef CONFIG_OT_VB_LOG_SUPPORT
    vb_log_record(blk, pool, uid, 1, CHN_INFO_NONE);
#endif
    vb_spin_unlock(flags);
    return TD_SUCCESS;
}

static td_s32 vb_user_sub_process(td_u32 pool_id, vb_pool_attr *pool,
    td_phys_addr_t phys_addr, td_u32 uid)
{
    vb_blk_info *blk = TD_NULL;

    if (vb_check_phy_addr(pool, phys_addr)) {
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    blk = pool->blk + osal_div64_u64(phys_addr - pool->pool_phy_addr, pool->blk_size);
    if (((blk->sum_cnt <= 0) || (blk->user_cnt[uid] == 0))) {
        ot_trace_vb(OT_DBG_WARN, "try to sub user for a free_list buffer!\n");
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    ot_trace_vb(OT_DBG_DEBUG, "blk [%u] in pool[%u]:is subed!\n", blk->blk_id, pool_id);
    blk->user_cnt[uid]--;
    blk->sum_cnt--;

#ifdef CONFIG_OT_VB_LOG_SUPPORT
    vb_log_record(blk, pool, uid, 0, CHN_INFO_NONE);
#endif
    if (blk->sum_cnt) {
        return TD_SUCCESS;
    }

#ifdef CONFIG_OT_VB_LOG_SUPPORT
    vb_save_seg_log(blk, pool->pool_owner, pool_id);
#endif
    /* now this buffer is free_list. */
    osal_list_del(&blk->list);
    osal_list_add_tail(&blk->list, &pool->free_list);
    pool->free_blk_cnt++;
#ifdef CONFIG_OT_VB_ASYNC_SUPPORT
    if (pool->state == VB_POOL_ZOMBIE && pool->blk_cnt == pool->free_blk_cnt) {
        vb_async_destroy_thread_wakeup();
    }
#endif

#ifdef OT_DEBUG
    {
        td_u32 i;
        for (i = 0; i < OT_VB_MAX_USER; i++) {
            ot_assert(blk->user_cnt[i] == 0);
        }
    }
#endif /* OT_DEBUG */
    return TD_SUCCESS;
}

static td_s32 vb_do_user_sub(td_u32 pool_id, td_phys_addr_t phys_addr, td_u32 uid, td_s32 milli_sec)
{
    unsigned long flags;
    vb_pool_attr *pool = TD_NULL;
    td_s32 ret;

    vb_check_initialization_status_return(OT_ERR_VB_NOT_READY);
    vb_check_user_id_return(uid, OT_ERR_VB_ILLEGAL_PARAM);
    vb_check_pool_id_return(pool_id, OT_ERR_VB_ILLEGAL_PARAM);

    vb_spin_lock(flags);
    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool %u is already destroyed !\n", pool_id);
        vb_spin_unlock(flags);
        return OT_ERR_VB_NULL_PTR;
    }
    if (pool->state == VB_POOL_DEAD) {
        ot_trace_vb(OT_DBG_WARN, "pool %u is dead !\n", pool_id);
        vb_spin_unlock(flags);
        return OT_ERR_VB_UNEXIST;
    }

    ot_unused(milli_sec);

    ret = vb_user_sub_process(pool_id, pool, phys_addr, uid);
    vb_spin_unlock(flags);
    return ret;
}

td_s32 vb_user_sub(td_u32 pool_id, td_phys_addr_t phys_addr, td_u32 uid)
{
    return vb_do_user_sub(pool_id, phys_addr, uid, 0);
}

td_u32 vb_inquire_user_cnt(vb_blk_handle handle)
{
    td_s32 ret;
    td_u32 i;
    td_u32 cnt;
    td_u32 pool_id = vb_hdle_to_pool_id(handle);
    vb_blk_info *blk = TD_NULL;
    unsigned long flags;
    vb_pool_attr *pool = TD_NULL;

    vb_check_initialization_status_return(0);
    vb_check_pool_id_return(pool_id, 0);

    vb_spin_lock(flags);
    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool %u is already destroyed !\n", pool_id);
        vb_spin_unlock(flags);
        return 0;
    }

    ret = vb_check_block_id(pool_id, vb_hdle_to_blk_id(handle));
    if (ret != TD_SUCCESS) {
        vb_spin_unlock(flags);
        return 0;
    }

    blk = g_blks[pool_id] + vb_hdle_to_blk_id(handle);
    if (blk == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool [%u] doesn't have blocks! \n", pool_id);
        vb_spin_unlock(flags);
        return 0;
    }

    for (i = 0, cnt = 0; i < OT_VB_MAX_USER; i++) {
        if (blk->user_cnt[i]) {
            cnt++;
        }
    }
    vb_spin_unlock(flags);
    return cnt;
}

td_u32 vb_get_one_user_cnt(vb_blk_handle handle, td_u32 uid)
{
    td_u32 pool_id = vb_hdle_to_pool_id(handle);
    vb_blk_info *blk = TD_NULL;

    blk = g_blks[pool_id] + vb_hdle_to_blk_id(handle);
    if (blk == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool [%u] doesn't have blocks! \n", pool_id);
        return 0;
    }

    return blk->user_cnt[uid];
}

td_u32 vb_inquire_one_user_cnt(vb_blk_handle handle, td_u32 uid)
{
    td_s32 ret;
    td_u32 user_cnt;
    unsigned long flags;
    vb_pool_attr *pool = TD_NULL;
    td_u32 pool_id = vb_hdle_to_pool_id(handle);

    vb_check_initialization_status_return(0);
    vb_check_pool_id_return(pool_id, 0);
    vb_check_user_id_return(uid, 0);

    vb_spin_lock(flags);
    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool %u is already destroyed !\n", pool_id);
        vb_spin_unlock(flags);
        return 0;
    }
    ret = vb_check_block_id(pool_id, vb_hdle_to_blk_id(handle));
    if (ret != TD_SUCCESS) {
        vb_spin_unlock(flags);
        return 0;
    }

    user_cnt = vb_get_one_user_cnt(handle, uid);
    vb_spin_unlock(flags);

    return user_cnt;
}

td_u32 vb_inquire_total_user_cnt(vb_blk_handle handle)
{
    td_s32 ret;
    td_u32 user_id;
    td_u32 total_user_cnt = 0;
    vb_pool_attr *pool = TD_NULL;
    unsigned long flags;

    td_u32 pool_id = vb_hdle_to_pool_id(handle);

    vb_check_initialization_status_return(0);
    vb_check_pool_id_return(pool_id, 0);

    vb_spin_lock(flags);
    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool %u is already destroyed !\n", pool_id);
        vb_spin_unlock(flags);
        return 0;
    }

    ret = vb_check_block_id(pool_id, vb_hdle_to_blk_id(handle));
    if (ret != TD_SUCCESS) {
        vb_spin_unlock(flags);
        return 0;
    }

    for (user_id = 0; user_id < OT_VB_MAX_USER; user_id++) {
        total_user_cnt += vb_get_one_user_cnt(handle, user_id);
    }
    vb_spin_unlock(flags);

    return total_user_cnt;
}

td_u32 vb_inquire_blk_cnt(td_u32 uid, td_bool is_comm_pool)
{
    td_u32 i;
    td_u32 j;
    td_u32 cnt_in_comm = 0;
    td_u32 cnt_in_all = 0;
    vb_blk_info *blk = TD_NULL;
    unsigned long flags;

    vb_check_initialization_status_return(0);
    vb_check_user_id_return(uid, 0);

    vb_spin_lock(flags);
    /* travel all pools */
    for (i = 0; i < g_vb_conf.max_pool_cnt; i++) {
        blk = g_blks[i];
        if (g_pools[i] == TD_NULL ||  blk == TD_NULL) {
            continue;
        }

        /* travel all blocks */
        for (j = 0; j < g_pools[i]->blk_cnt; j++, blk++) {
            if (!blk->user_cnt[uid]) {
                continue;
            }
            if (g_pools[i]->is_comm_pool != TD_FALSE) {
                cnt_in_comm++;
            }
            cnt_in_all++;
        }
    }
    vb_spin_unlock(flags);

    if (is_comm_pool != TD_FALSE) {
        return cnt_in_comm;
    }
    return cnt_in_all;
}

td_s32 vb_inquire_pool(td_u32 pool_id, ot_vb_pool_status *pool_status)
{
    vb_pool_attr *pool = TD_NULL;
    unsigned long flags;

    vb_check_initialization_status_return(OT_ERR_VB_NOT_READY);
    vb_check_pool_id_return(pool_id, OT_ERR_VB_ILLEGAL_PARAM);


#endif /* SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART04_H_ */
