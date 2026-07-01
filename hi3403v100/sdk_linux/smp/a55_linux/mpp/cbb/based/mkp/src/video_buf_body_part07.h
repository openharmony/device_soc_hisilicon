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

#ifndef SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART07_H_
#define SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART07_H_

    if (ret) {
        ot_trace_vb(OT_DBG_ERR, "osal_down error !\n");
        return -ERESTARTSYS;
    }

    if (g_pools == TD_NULL) {
        osal_up(&g_sema);
        ot_trace_vb(OT_DBG_INFO, "vb already exited!\n");
        return TD_SUCCESS;
    }

    for (i = 0; i < OT_VB_MAX_USER; i++) {
        if (g_is_mod_pool_init[i] != TD_FALSE) {
            osal_up(&g_sema);
            ot_trace_vb(OT_DBG_ERR, "mod[%d] vb should be exit first!\n", i);
            return OT_ERR_VB_NOT_PERM;
        }
    }

    if (g_vb_init) {
        osal_up(&g_sema);
        ot_trace_vb(OT_DBG_ERR, "please exit sys first!\n");
        return OT_ERR_VB_NOT_PERM;
    }

    for (i = 0; i < g_vb_conf.max_pool_cnt; i++) {
        if (g_pools[i] != TD_NULL) {
            g_pools[i]->async_flag = TD_FALSE;
            ret = destroy_pool(g_pools[i]);
            if (ret != TD_SUCCESS) {
                osal_up(&g_sema);
                return ret;
            }
        }
    }

    ret = osal_list_empty(&g_comm_pools);

    ot_assert(ret == TD_TRUE);

    osal_kfree(g_pools);
    osal_kfree(g_blks);
    g_blks = TD_NULL;
    g_pools = TD_NULL;
    g_is_conf = TD_FALSE;
    (td_void)memset_s(&g_vb_conf, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg));
    osal_up(&g_sema);
    ot_trace_vb(OT_DBG_DEBUG, "vb exited!\n");
    return TD_SUCCESS;
}

td_s32 vb_do_create_pool(vb_ioc_arg *ioc_arg)
{
    td_s32 ret;
    vb_pool_common_info common_info;

    if (ioc_arg->blk_size == 0 || ioc_arg->blk_cnt == 0) {
        ot_trace_vb(OT_DBG_ERR, "blk_size(%llu) or blk_cnt(%u) illegal, should not be 0!\n",
            ioc_arg->blk_size, ioc_arg->blk_cnt);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if (ioc_arg->remap_mode >= OT_VB_REMAP_MODE_BUTT) {
        ot_trace_vb(OT_DBG_ERR, "illegal parameter remap_mode %u!\n", ioc_arg->remap_mode);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if (osal_down(&g_sema)) {
        return -ERESTARTSYS;
    }

    if (g_pools == TD_NULL) {
        OT_TRACE(OT_DBG_ERR, OT_ID_VB, "VB not initialized!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_READY;
    }

    common_info.blk_cnt = ioc_arg->blk_cnt;
    common_info.blk_size = ioc_arg->blk_size;
    common_info.owner           = OT_POOL_OWNER_USER;
    common_info.uid             = OT_VB_UID_USER;
    common_info.vb_remap_mode = ioc_arg->remap_mode;
    (td_void)strncpy_s(common_info.buf_name, OT_MAX_MMZ_NAME_LEN, "user_pool", OT_MAX_MMZ_NAME_LEN - 1);
    ret = create_pool(&common_info, &ioc_arg->pool_id, ioc_arg->mmz_name);
    osal_up(&g_sema);
    return ret;
}


td_s32 vb_do_destroy_pool(td_u32 pool_id)
{
    vb_pool_attr *pool = TD_NULL;
    td_u32 cnt = 0;
    td_s32 ret;

    if (osal_down(&g_sema)) {
        return -ERESTARTSYS;
    }

    if (g_pools == TD_NULL) {
        OT_TRACE(OT_DBG_WARN, OT_ID_VB, "VB not initialized!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_READY;
    }

    if (pool_id >= g_vb_conf.max_pool_cnt) {
        ot_trace_vb(OT_DBG_ERR, "invalid pool ID [%u]!\n", pool_id);
        osal_up(&g_sema);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    pool = g_pools[pool_id];
    if (pool == TD_NULL) {
        ot_trace_vb(OT_DBG_ERR, "try to destroy a NOT existed pool!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_UNEXIST;
    }
    if (pool->user == TD_FALSE) {
        ot_trace_vb(OT_DBG_ERR, "try to destroy a kernel pool!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_PERM;
    }

    if (vb_inquire_pool_busy_blk_cnt(pool_id, &cnt) != TD_SUCCESS) {
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_PERM;
    } else {
        if ((cnt != 0) && (!g_vb_force_exit)) {
            osal_up(&g_sema);
            ot_trace_vb(OT_DBG_ERR, "someone is using vb now, please make sure to release vb block first!\n");
            return OT_ERR_VB_NOT_PERM;
        }
    }

    ret = destroy_pool(pool);
    osal_up(&g_sema);

    return ret;
}

td_s32 vb_check_blk_cfg(const vb_ioc_arg *ioc_arg)
{
    /* check pool ID */
    if (ioc_arg->pool_id >= g_vb_conf.max_pool_cnt) {
        ot_trace_vb(OT_DBG_ERR, "invalid pool id[%u]!\n", ioc_arg->pool_id);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if (!g_pools[ioc_arg->pool_id]) {
        ot_trace_vb(OT_DBG_ERR, "pool id[%u] is not existed!\n",  ioc_arg->pool_id);
        return OT_ERR_VB_UNEXIST;
    }

    if (ioc_arg->blk_size > g_pools[ioc_arg->pool_id]->blk_size) {
        ot_trace_vb(OT_DBG_ERR, "the size(%llu) is larger than the pool block size(%llu)!\n",
                    ioc_arg->blk_size, g_pools[ioc_arg->pool_id]->blk_size);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

td_s32 vb_do_get_blk(vb_ioc_arg *ioc_arg)
{
    td_s32 ret;

    if (osal_down(&g_sema)) {
        return -ERESTARTSYS;
    }

    if (g_pools == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "VB not initialized!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_READY;
    }

    /* get block from common pools */
    if (ioc_arg->pool_id == OT_VB_INVALID_POOL_ID) {
        ioc_arg->blk_handle = (td_u32)vb_get_blk_by_size(ioc_arg->blk_size, OT_VB_UID_USER, ioc_arg->mmz_name);
        if (ioc_arg->blk_handle == OT_VB_INVALID_HANDLE) {
            ot_trace_vb(OT_DBG_WARN, "no buffer block!\n");
            osal_up(&g_sema);
            return OT_ERR_VB_NO_BUF;
        }
        osal_up(&g_sema);
        return TD_SUCCESS;
    }

    ret = vb_check_blk_cfg(ioc_arg);
    if (ret != TD_SUCCESS) {
        ioc_arg->blk_handle = OT_VB_INVALID_HANDLE;
        osal_up(&g_sema);
        return ret;
    }

    /* get block from specific pool */
    ioc_arg->blk_handle = (td_u32)vb_get_blk_by_pool_id(ioc_arg->pool_id, OT_VB_UID_USER);
    if (ioc_arg->blk_handle == OT_VB_INVALID_HANDLE) {
        ot_trace_vb(OT_DBG_ERR, "no buffer block!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_NO_BUF;
    }
    osal_up(&g_sema);

    return TD_SUCCESS;
}

static td_s32 vb_do_release_blk(vb_blk_handle handle, td_s32 milli_sec, td_bool is_delete_buf)
{
    td_s32 ret;
    td_u32 pool_id = vb_hdle_to_pool_id(handle);
    td_u32 blk_id = vb_hdle_to_blk_id(handle);

    if (milli_sec < -1) {
        ot_trace_vb(OT_DBG_ERR, "invalid milli_sec %d!\n", milli_sec);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if (osal_down(&g_sema)) {
        return -ERESTARTSYS;
    }

    if (g_pools == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "VB not initialized!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_READY;
    }
    if (pool_id >= g_vb_conf.max_pool_cnt) {
        ot_trace_vb(OT_DBG_ERR, "invalid handle %u!\n", handle);
        osal_up(&g_sema);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }
    if (g_pools[pool_id] == TD_NULL) {
        ot_trace_vb(OT_DBG_ERR, "pool id[%u] is not existed!\n", pool_id);
        osal_up(&g_sema);
        return OT_ERR_VB_UNEXIST;
    }
    if (blk_id >= g_pools[pool_id]->blk_cnt) {
        ot_trace_vb(OT_DBG_ERR, "invalid handle!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    ot_unused(is_delete_buf);
    ret = vb_do_user_sub(pool_id, (g_blks[pool_id] + blk_id)->phys_addr, OT_VB_UID_USER, milli_sec);
    osal_up(&g_sema);
    return ret;
}

td_s32 vb_do_handle_to_phys_addr(ot_vb_blk handle, td_phys_addr_t *phys_addr)
{
    if (osal_down(&g_sema)) {
        return -ERESTARTSYS;
    }

    if (g_pools == TD_NULL) {
        *phys_addr = TD_NULL;
        ot_trace_vb(OT_DBG_WARN, "VB not initialized!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_READY;
    }

    if (vb_hdle_to_pool_id(handle) >= g_vb_conf.max_pool_cnt) {
        *phys_addr = TD_NULL;
        ot_trace_vb(OT_DBG_ERR, "bad argument!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if ((g_pools[vb_hdle_to_pool_id(handle)] == TD_NULL) ||
        (vb_hdle_to_blk_id(handle) >= g_pools[vb_hdle_to_pool_id(handle)]->blk_cnt)) {
        *phys_addr = TD_NULL;
        ot_trace_vb(OT_DBG_ERR, "bad argument!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    *phys_addr = vb_handle_to_phys(handle);

    osal_up(&g_sema);

    return TD_SUCCESS;
}

td_s32 vb_do_handle_to_pool_id(ot_vb_blk handle, ot_vb_pool *pool_id)
{
    if (osal_down(&g_sema)) {
        return -ERESTARTSYS;
    }

    if (g_pools == TD_NULL) {
        *pool_id = OT_VB_INVALID_POOL_ID;
        ot_trace_vb(OT_DBG_WARN, "VB not initialized!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_READY;
    }

    if (vb_hdle_to_pool_id(handle) >= g_vb_conf.max_pool_cnt) {
        *pool_id = OT_VB_INVALID_POOL_ID;
        ot_trace_vb(OT_DBG_ERR, "bad argument!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if ((g_pools[vb_hdle_to_pool_id(handle)] == TD_NULL) ||
        (vb_hdle_to_blk_id(handle) >= g_pools[vb_hdle_to_pool_id(handle)]->blk_cnt)) {
        *pool_id = OT_VB_INVALID_POOL_ID;
        ot_trace_vb(OT_DBG_ERR, "bad argument!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    *pool_id = vb_handle_to_pool_id(handle);

    osal_up(&g_sema);

    return TD_SUCCESS;
}

td_s32 vb_get_supplement_addr(video_supplement_info *supplement_info)
{
    ot_video_supplement *supplement_tmp = TD_NULL;
    ot_vb_blk handle = supplement_info->block;

    if (osal_down(&g_sema)) {
        return -ERESTARTSYS;
    }

    if (g_pools == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "VB not initialized!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_READY;
    }

    if (vb_hdle_to_pool_id(handle) >= g_vb_conf.max_pool_cnt) {
        ot_trace_vb(OT_DBG_ERR, "bad argument!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if ((g_pools[vb_hdle_to_pool_id(handle)] == TD_NULL) ||
        (vb_hdle_to_blk_id(handle) >= g_pools[vb_hdle_to_pool_id(handle)]->blk_cnt)) {
        ot_trace_vb(OT_DBG_ERR, "bad argument!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_UNEXIST;
    }

    supplement_tmp = vb_handle_to_supplement(handle);

    if (write_user_linear_space_valid((td_u8 *)(supplement_info->supplement),
        sizeof(ot_video_supplement)) == TD_FALSE) {
        ot_trace_vb(OT_DBG_ERR, "supplement address is invalid!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    if (supplement_tmp == NULL || osal_copy_to_user(supplement_info->supplement,
        supplement_tmp, sizeof(ot_video_supplement))) {
        osal_printk("user copy supplement_tmp failed!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    osal_up(&g_sema);

    return TD_SUCCESS;
}

td_s32 vb_do_inquire_user_cnt(ot_vb_blk handle, td_s32 *user_cnt)
{
    if (osal_down(&g_sema)) {
        return -ERESTARTSYS;
    }

    if (g_pools == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "VB not initialized!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_READY;
    }

    if (vb_hdle_to_pool_id(handle) >= g_vb_conf.max_pool_cnt) {
        *user_cnt = OT_VB_INVALID_HANDLE;
        ot_trace_vb(OT_DBG_ERR, "bad argument!\n");
        osal_up(&g_sema);
        return OT_VB_INVALID_HANDLE;
    }

    if ((g_pools[vb_hdle_to_pool_id(handle)] == TD_NULL) ||
        (vb_hdle_to_blk_id(handle) >= g_pools[vb_hdle_to_pool_id(handle)]->blk_cnt)) {
        *user_cnt = OT_VB_INVALID_HANDLE;
        ot_trace_vb(OT_DBG_ERR, "bad argument!\n");
        osal_up(&g_sema);
        return OT_VB_INVALID_HANDLE;
    }

    *user_cnt = vb_inquire_user_cnt(handle);
    osal_up(&g_sema);

    return TD_SUCCESS;
}

static td_s32 base_ioctl_pool(unsigned int cmd, unsigned long arg, td_bool *handled)
{
    *handled = TD_TRUE;
    switch (cmd) {
        case VB_CREATE_CTRL: {
            vb_ioc_arg *ioc_arg = (vb_ioc_arg *)(td_uintptr_t)arg;
            vb_check_null_ptr_return(ioc_arg);
            return vb_do_create_pool(ioc_arg);
        }
        case VB_DESTROY_CTRL: {
            td_u32 *p_id = (td_u32 *)(td_uintptr_t)arg;
            vb_check_null_ptr_return(p_id);
            return vb_do_destroy_pool(*p_id);
        }
        case VB_GETBLK_CTRL: {
            vb_ioc_arg *ioc_arg = (vb_ioc_arg *)(td_uintptr_t)arg;
            vb_check_null_ptr_return(ioc_arg);
            return vb_do_get_blk(ioc_arg);
        }
        case VB_RELBLK_CTRL: {
            vb_blk_handle *handle = (vb_blk_handle *)(td_uintptr_t)arg;
            vb_check_null_ptr_return(handle);
            return vb_do_release_blk(*handle, 0, TD_FALSE);
        }
        default:
            *handled = TD_FALSE;
            return TD_FAILURE;
    }
}


#endif /* SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART07_H_ */
