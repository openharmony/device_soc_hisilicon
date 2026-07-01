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

#ifndef SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART05_H_
#define SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART05_H_

    if (pool_status == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "null point!\n");
        return OT_ERR_VB_NULL_PTR;
    }

    vb_spin_lock(flags);
    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool id [%u] does not existed! \n", pool_id);
        vb_spin_unlock(flags);
        return OT_ERR_VB_UNEXIST;
    }
    pool_status->is_common_pool = pool->is_comm_pool;
    pool_status->blk_cnt = pool->blk_cnt;
    pool_status->free_blk_cnt = pool->free_blk_cnt;
    vb_spin_unlock(flags);
    return TD_SUCCESS;
}

td_s32 vb_inquire_pool_user_cnt(td_u32 pool_id, td_u32 uid, td_u32 *cnt)
{
    td_u32 i;
    td_u32 count = 0;
    vb_blk_info *blk = TD_NULL;
    vb_pool_attr *pool = TD_NULL;
    unsigned long flags;

    vb_check_initialization_status_return(OT_ERR_VB_NOT_READY);
    vb_check_user_id_return(uid, OT_ERR_VB_ILLEGAL_PARAM);
    vb_check_pool_id_return(pool_id, OT_ERR_VB_ILLEGAL_PARAM);

    vb_spin_lock(flags);
    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool id [%u] does not existed! \n", pool_id);
        vb_spin_unlock(flags);
        return OT_ERR_VB_UNEXIST;
    }

    blk = g_blks[pool_id];
    if (blk == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool [%u] doesn't have blocks! \n", pool_id);
        vb_spin_unlock(flags);
        return OT_ERR_VB_UNEXIST;
    }

    for (i = 0; i < pool->blk_cnt; i++, blk++) {
        count += blk->user_cnt[uid];
    }

    *cnt = count;
    vb_spin_unlock(flags);

    return TD_SUCCESS;
}

td_s32 vb_inquire_pool_busy_blk_cnt(td_u32 pool_id, td_u32 *cnt)
{
    vb_pool_attr *pool = TD_NULL;

    vb_check_initialization_status_return(OT_ERR_VB_NOT_READY);
    vb_check_pool_id_return(pool_id, OT_ERR_VB_ILLEGAL_PARAM);

    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool id [%u] does not existed! \n",
                    pool_id);
        return OT_ERR_VB_UNEXIST;
    }

    *cnt = pool->blk_cnt - pool->free_blk_cnt;
    ot_assert(*cnt <= pool->blk_cnt);

    return TD_SUCCESS;
}

td_bool vb_is_blk_valid(td_u32 pool_id, td_phys_addr_t phys_addr)
{
    unsigned long flags;
    vb_pool_attr *pool = TD_NULL;

    vb_check_initialization_status_return(TD_FALSE);
    vb_check_pool_id_return(pool_id, TD_FALSE);

    vb_spin_lock(flags);
    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool id [%u] does not existed! \n", pool_id);
        vb_spin_unlock(flags);
        return TD_FALSE;
    }

    if ((phys_addr < pool->pool_phy_addr) || (phys_addr >= pool->pool_phy_addr + pool->pool_size)) {
        ot_trace_vb(OT_DBG_WARN, "id%d, phys_addr = %lx, pool_phys_addr = 0x%lx, pool_size = %llu\n",
                    pool_id, (td_ulong)phys_addr, (td_ulong)pool->pool_phy_addr, pool->pool_size);
        vb_spin_unlock(flags);
        return TD_FALSE;
    }
    vb_spin_unlock(flags);
    return TD_TRUE;
}

td_bool vb_is_pool_id_valid(td_u32 pool_id)
{
    unsigned long flags;
    vb_pool_attr *pool = TD_NULL;

    vb_check_initialization_status_return(TD_FALSE);
    vb_check_pool_id_return(pool_id, TD_FALSE);

    vb_spin_lock(flags);
    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool id [%u] does not existed! \n",
                    pool_id);
        vb_spin_unlock(flags);
        return TD_FALSE;
    }
    vb_spin_unlock(flags);

    return TD_TRUE;
}

/* another kind of common pool which is created by a module. */
vb_blk_handle vb_get_blk_by_size_and_module_ex(td_u64 blk_size, ot_vb_uid uid, const td_char *pc_mmz_name, td_u32 chn)
{
    td_u32 pool_id;
    td_s32 ret;
    unsigned long flags;
    vb_blk_handle handle;

    ot_unused(pc_mmz_name);
    vb_check_initialization_status_return(OT_VB_INVALID_HANDLE);
    vb_check_user_id_return(uid, OT_VB_INVALID_HANDLE);

    vb_spin_lock(flags);
    ret = vb_search_comm_pool_by_size(&pool_id, blk_size, TD_NULL, OT_POOL_OWNER_MODULE, uid);
    if (ret != TD_SUCCESS) {
        vb_spin_unlock(flags);
        return OT_VB_INVALID_HANDLE;
    }
    handle = vb_get_blk_handle_by_pool_id(pool_id, uid, chn);
    vb_spin_unlock(flags);
    return handle;
}

vb_blk_handle vb_get_blk_by_size_and_module(td_u64 blk_size, ot_vb_uid uid, const td_char *pc_mmz_name)
{
    return vb_get_blk_by_size_and_module_ex(blk_size, uid, pc_mmz_name, CHN_INFO_NONE);
}

static td_s32 vb_check_size_and_pool_id(td_u32 pool_id, td_u64 blk_size, const vb_pool_attr *pool)
{
    if (pool == TD_NULL) {
        ot_trace_vb(OT_DBG_ERR, "pool %u is already destroyed !\n", pool_id);
        return TD_FAILURE;
    }
    if (pool->state == VB_POOL_DEAD) {
        ot_trace_vb(OT_DBG_ERR, "pool %u is dead !\n", pool_id);
        return TD_FAILURE;
    }
    if (pool->blk_size < blk_size) {
        ot_trace_vb(OT_DBG_ERR, "input blksize:%llu is larger than pool blk size:%llu !\n",
                    blk_size, pool->blk_size);
        return TD_FAILURE;
    }

    if (osal_list_empty(&pool->free_list)) {
        ot_trace_vb(OT_DBG_WARN, "no free_list buffer in pool %u!\n", pool_id);
        return TD_FAILURE;
    }

    if (pool->free_blk_cnt == 0) {
        ot_trace_vb(OT_DBG_ERR, "free_list blk cnt is 0, in pool %u!\n", pool_id);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/*
 * input: poolid, u32blksize, u32uid;
 * output: vb_handle;
 * description: search a free_list vb blk for poolid, and check the blksize must larger than the input
 *              blksize;
 *
 */
vb_blk_handle vb_get_blk_by_size_and_pool_id_ex(td_u32 pool_id, td_u64 blk_size, td_u32 uid, td_u32 chn)
{
    unsigned long flags;
    vb_blk_info *blk = TD_NULL;
    vb_pool_attr *pool = TD_NULL;

    vb_check_initialization_status_return(OT_VB_INVALID_HANDLE);
    vb_check_user_id_return(uid, OT_VB_INVALID_HANDLE);
    vb_check_pool_id_return(pool_id, OT_VB_INVALID_HANDLE);

    /* when called in process context, we do this */
    vb_spin_lock(flags);
    pool = g_pools[pool_id];

    if (vb_check_size_and_pool_id(pool_id, blk_size, pool) != TD_SUCCESS) {
        vb_spin_unlock(flags);
        return OT_VB_INVALID_HANDLE;
    }

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

    ot_trace_vb(OT_DBG_DEBUG, "blk [%u] in pool[%u]:is allocated!\n", blk->blk_id, pool_id);

    vb_spin_unlock(flags);
    return vb_mk_handle(pool_id, blk->blk_id);
}

vb_blk_handle vb_get_blk_by_size_and_pool_id(td_u32 pool_id, td_u64 blk_size, td_u32 uid)
{
    return vb_get_blk_by_size_and_pool_id_ex(pool_id, blk_size, uid, CHN_INFO_NONE);
}

td_s32 vb_get_config(ot_vb_cfg *vb_config)
{
    if (vb_config == TD_NULL) {
        return OT_ERR_VB_NULL_PTR;
    }

    osal_down(&g_sema);
    if (g_is_conf == TD_FALSE) {
        ot_trace_vb(OT_DBG_ERR, "please configurate common VB first!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_READY;
    }
    if (memcpy_s(vb_config, sizeof(*vb_config), &g_vb_conf, sizeof(g_vb_conf)) != EOK) {
        osal_up(&g_sema);
        ot_trace_vb(OT_DBG_ERR, "memcpy_s fail!\n");
        return OT_ERR_VB_ILLEGAL_PARAM;
    }
    osal_up(&g_sema);

    return TD_SUCCESS;
}

td_s32 vb_check_config(const ot_vb_pool_cfg *pool_cfg)
{
    td_u64 align_blk_size;

    if (pool_cfg->blk_cnt > VB_MAX_BLK_CNT) {
        ot_trace_vb(OT_DBG_ERR, "illegal blk_cnt(%d), [1, %d]!\n", pool_cfg->blk_cnt, VB_MAX_BLK_CNT);
        return TD_FAILURE;
    }

    align_blk_size = vb_get_aligned_size(pool_cfg->blk_size);
    if ((align_blk_size > VB_MAX_POOL_SIZE) || (align_blk_size * pool_cfg->blk_cnt > VB_MAX_POOL_SIZE)) {
        ot_trace_vb(OT_DBG_ERR, "[size = %llu, cnt = %u]vb mmz alloc total size is larger than 4GB!\n",
            align_blk_size, pool_cfg->blk_cnt);
        return TD_FAILURE;
    }

    if (osal_strnlen(pool_cfg->mmz_name, OT_MAX_MMZ_NAME_LEN) >= OT_MAX_MMZ_NAME_LEN) {
        ot_trace_vb(OT_DBG_ERR, "mmz name len it's too long\n");
        return TD_FAILURE;
    }

    if (pool_cfg->remap_mode >= OT_VB_REMAP_MODE_BUTT) {
        ot_trace_vb(OT_DBG_ERR, "remap_mode %d is illegal!\n", pool_cfg->remap_mode);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 vb_set_config(const ot_vb_cfg *vb_config)
{
    td_s32 i;

    if (vb_config == TD_NULL) {
        return OT_ERR_VB_NULL_PTR;
    }

    for (i = 0; i < OT_VB_MAX_COMMON_POOLS; i++) {
        if (!vb_config->common_pool[i].blk_cnt || !vb_config->common_pool[i].blk_size) {
            continue;
        }

        if (vb_check_config(&vb_config->common_pool[i]) != TD_SUCCESS) {
            return OT_ERR_VB_ILLEGAL_PARAM;
        }
    }

    osal_down(&g_sema);

    if (g_pools != TD_NULL) {
        ot_trace_vb(OT_DBG_ERR, "VB is initialized!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_BUSY;
    }

    if (memcpy_s(&g_vb_conf, sizeof(g_vb_conf), vb_config, sizeof(*vb_config)) != EOK) {
        osal_up(&g_sema);
        ot_trace_vb(OT_DBG_ERR, "memcpy_s fail!\n");
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    g_vb_conf.max_pool_cnt = OT_VB_MAX_POOLS; /* fix to OT_VB_MAX_POOLS */

    g_is_conf = TD_TRUE;
    osal_up(&g_sema);
    return TD_SUCCESS;
}

td_s32 vb_get_mod_pool_config(vb_ioc_cfg_arg *vb_ioc_cfg)
{
    td_u32 uid;

    if (vb_ioc_cfg == TD_NULL) {
        return OT_ERR_VB_NULL_PTR;
    }

    uid = vb_ioc_cfg->vb_uid;
    if (uid >= OT_VB_MAX_USER) {
        ot_trace_vb(OT_DBG_ERR, "invalid uid(%d) >= OT_VB_MAX_USER(%d)!\n", uid, OT_VB_MAX_USER);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    osal_down(&g_sema);
    if (g_is_mod_conf[uid] == TD_FALSE) {
        ot_trace_vb(OT_DBG_ERR, "invalid user id %d !\n", uid);
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_READY;
    }
    if (memcpy_s(&vb_ioc_cfg->vb_conf, sizeof(ot_vb_cfg), &g_vb_mod_conf[uid], sizeof(ot_vb_cfg)) != EOK) {
        osal_up(&g_sema);
        ot_trace_vb(OT_DBG_ERR, "memcpy_s fail!\n");
        return OT_ERR_VB_ILLEGAL_PARAM;
    }
    osal_up(&g_sema);
    return TD_SUCCESS;
}

td_s32 vb_get_mod_pool_vb_config(ot_vb_uid uid, ot_vb_cfg *vb_cfg)
{
    if (vb_cfg == TD_NULL) {
        return OT_ERR_VB_NULL_PTR;
    }

    if (uid >= OT_VB_MAX_USER) {
        ot_trace_vb(OT_DBG_ERR, "invalid uid(%d) >= OT_VB_MAX_USER(%d)!\n", uid, OT_VB_MAX_USER);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    osal_down(&g_sema);
    if (g_is_mod_conf[uid] == TD_FALSE) {
        ot_trace_vb(OT_DBG_ERR, "invalid user id %d !\n", uid);
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_READY;
    }
    if (memcpy_s(vb_cfg, sizeof(ot_vb_cfg), &g_vb_mod_conf[uid], sizeof(g_vb_mod_conf[uid])) != EOK) {
        osal_up(&g_sema);
        ot_trace_vb(OT_DBG_ERR, "memcpy_s fail!\n");
        return OT_ERR_VB_ILLEGAL_PARAM;
    }
    osal_up(&g_sema);
    return TD_SUCCESS;
}

td_s32 vb_set_mod_pool_config(const vb_ioc_cfg_arg *vb_ioc_cfg)
{
    td_u32 i;
    td_u32 uid;

    if (vb_ioc_cfg == TD_NULL) {
        return OT_ERR_VB_NULL_PTR;
    }

    uid = vb_ioc_cfg->vb_uid;
    if (uid >= OT_VB_MAX_USER) {
        ot_trace_vb(OT_DBG_ERR, "invalid uid(%d) >= OT_VB_MAX_USER(%d)!\n", uid, OT_VB_MAX_USER);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    for (i = 0; i < OT_VB_MAX_MOD_COMMON_POOLS; i++) {
        if (!vb_ioc_cfg->vb_conf.common_pool[i].blk_cnt ||
            !vb_ioc_cfg->vb_conf.common_pool[i].blk_size) {
                continue;
        }

        if (osal_strnlen(vb_ioc_cfg->vb_conf.common_pool[i].mmz_name, OT_MAX_MMZ_NAME_LEN) >= OT_MAX_MMZ_NAME_LEN) {
            ot_trace_vb(OT_DBG_ERR, "mmz name len it's too long\n");
            return OT_ERR_VB_ILLEGAL_PARAM;
        }

        if (vb_ioc_cfg->vb_conf.common_pool[i].remap_mode >= OT_VB_REMAP_MODE_BUTT) {
            ot_trace_vb(OT_DBG_ERR, "remap_mode %d is illegal!\n", vb_ioc_cfg->vb_conf.common_pool[i].remap_mode);
            return OT_ERR_VB_ILLEGAL_PARAM;
        }
    }

    osal_down(&g_sema);
    if (g_pools == TD_NULL) {
        ot_trace_vb(OT_DBG_ERR, "VB is not initialized!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_BUSY;
    }

    if (g_is_mod_pool_init[uid] == TD_TRUE) {
        ot_trace_vb(OT_DBG_ERR, "Mod VB is initialized!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_BUSY;
    }

    if (memcpy_s(&g_vb_mod_conf[uid], sizeof(ot_vb_cfg), &vb_ioc_cfg->vb_conf, sizeof(ot_vb_cfg)) != EOK) {
        osal_up(&g_sema);
        ot_trace_vb(OT_DBG_ERR, "memcpy_s fail!\n");


#endif /* SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART05_H_ */
