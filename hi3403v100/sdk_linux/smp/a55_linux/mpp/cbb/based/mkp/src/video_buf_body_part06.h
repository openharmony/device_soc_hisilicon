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

#ifndef SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART06_H_
#define SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART06_H_

        return OT_ERR_VB_ILLEGAL_PARAM;
    }
    g_vb_mod_conf[uid].max_pool_cnt = OT_VB_MAX_POOLS; /* fix to OT_VB_MAX_POOLS */
    g_is_mod_conf[uid] = TD_TRUE;
    osal_up(&g_sema);

    return TD_SUCCESS;
}
static td_s32 vb_do_mod_vb_init_check(td_u32 uid, td_bool *need_return)
{
    *need_return = TD_TRUE;
    if (uid >= OT_VB_MAX_USER) {
        ot_trace_vb(OT_DBG_ERR, "uid should between [0, %d]! \n", OT_VB_MAX_USER);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }
    if (g_is_conf == TD_FALSE) {
        ot_trace_vb(OT_DBG_ERR, "please configurate common VB first! \n");
        return OT_ERR_VB_NOT_READY;
    }
    /* we should make sure common pool has initialized */
    if (g_pools == TD_NULL) {
        ot_trace_vb(OT_DBG_INFO, "please init common pools first!\n");
        return TD_SUCCESS;
    }
    if (g_is_mod_conf[uid] == TD_FALSE) {
        ot_trace_vb(OT_DBG_ERR, "please configurate mod common VB first! \n");
        return OT_ERR_VB_NOT_READY;
    }
    if (g_is_mod_pool_init[uid] == TD_TRUE) {
        ot_trace_vb(OT_DBG_INFO, "module %d common pool init again!\n", uid);
        return TD_SUCCESS;
    }
    *need_return = TD_FALSE;
    return TD_SUCCESS;
}
static td_s32 vb_do_mod_vb_init(td_u32 uid)
{
    td_s32 ret;
    td_u32 i;
    td_u32 pool_id;
    td_bool need_return = TD_FALSE;
    vb_pool_common_info common_info;

    if (osal_down(&g_sema)) {
        ot_trace_vb(OT_DBG_ERR, "osal_down error !\n");
        return -ERESTARTSYS;
    }
    ret = vb_do_mod_vb_init_check(uid, &need_return);
    if (need_return == TD_TRUE) {
        osal_up(&g_sema);
        return ret;
    }
    /* create module common pools */
    for (i = 0; i < OT_VB_MAX_MOD_COMMON_POOLS; i++) {
        if (!g_vb_mod_conf[uid].common_pool[i].blk_cnt || !g_vb_mod_conf[uid].common_pool[i].blk_size) {
            continue;
        }
        common_info.owner           = OT_POOL_OWNER_MODULE;
        common_info.uid             = uid;
        common_info.blk_cnt = g_vb_mod_conf[uid].common_pool[i].blk_cnt;
        common_info.blk_size = g_vb_mod_conf[uid].common_pool[i].blk_size;
        common_info.vb_remap_mode = g_vb_mod_conf[uid].common_pool[i].remap_mode;
        (td_void)strncpy_s(common_info.buf_name, OT_MAX_MMZ_NAME_LEN, "mod_vb", OT_MAX_MMZ_NAME_LEN - 1);
        ret = create_pool(&common_info, &pool_id, g_vb_mod_conf[uid].common_pool[i].mmz_name);
        if (ret != TD_SUCCESS) {
            goto create_fail;
        }
    }
    g_is_mod_pool_init[uid] = TD_TRUE;

    ot_trace_vb(OT_DBG_DEBUG, "vb init ok!\n");
    osal_up(&g_sema);
    return TD_SUCCESS;
create_fail:
    for (i = 0; i < g_vb_mod_conf[uid].max_pool_cnt; i++) {
        if (g_pools[i]) {
            vb_pool_attr *pool = g_pools[i];
            if (uid == pool->uid) {
                destroy_pool(g_pools[i]);
            }
        }
    }
    ot_trace_vb(OT_DBG_ERR, "init module(%d) common pool failed!\n", uid);
    osal_up(&g_sema);
    return OT_ERR_VB_NO_MEM;
}
static td_s32 vb_do_mod_vb_exit(td_u32 uid)
{
    td_u32 i;
    td_s32 ret;
    vb_pool_attr *pool = TD_NULL;
    td_u32 cnt;

    if (uid >= OT_VB_MAX_USER) {
        ot_trace_vb(OT_DBG_ERR, "invalid uid(%d) >= OT_VB_MAX_USER(%d)!\n", uid, OT_VB_MAX_USER);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }
    ret = osal_down(&g_sema);
    if (ret) {
        ot_trace_vb(OT_DBG_ERR, "osal_down error !\n");
        return -ERESTARTSYS;
    }
    if (g_pools == TD_NULL) {
        osal_up(&g_sema);
        ot_trace_vb(OT_DBG_INFO, "vb already exited!\n");
        return TD_SUCCESS;
    }
    for (i = 0; i < g_vb_mod_conf[uid].max_pool_cnt; i++) {
        if (g_pools[i] == TD_NULL) {
            continue;
        }
        pool = g_pools[i];
        if (uid != pool->uid) {
            continue;
        }
        if (vb_inquire_pool_busy_blk_cnt(pool->pool_id, &cnt) != TD_SUCCESS) {
            osal_up(&g_sema);
            return OT_ERR_VB_NOT_PERM;
        }
        if ((cnt != 0) && (!g_vb_force_exit)) {
            osal_up(&g_sema);
            ot_trace_vb(OT_DBG_ERR, "someone is using vb now, please make sure to release vb block first!\n");
            return OT_ERR_VB_NOT_PERM;
        }
        ret = destroy_pool(g_pools[i]);
    }
    g_is_mod_pool_init[uid] = TD_FALSE;
    g_is_mod_conf[uid] = TD_FALSE;

    osal_up(&g_sema);
    ot_trace_vb(OT_DBG_DEBUG, "common module(%d) vb exited!\n", uid);
    return ret;
}
#ifdef CONFIG_OT_VB_SUPPLEMENT_MASK_SUPPORT
td_s32 vb_set_supplement_conf(const ot_vb_supplement_cfg *supplement_conf)
{
    td_s32 ret;

    if (supplement_conf == TD_NULL) {
        ot_trace_vb(OT_DBG_ERR, "virt addr is null.\n");
        return OT_ERR_VB_NULL_PTR;
    }
    ret = vb_check_supplement(supplement_conf);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if (osal_down(&g_sema)) {
        return -ERESTARTSYS;
    }
    if (g_pools != TD_NULL) {
        osal_up(&g_sema);
        ot_trace_vb(OT_DBG_ERR, "VB is initialized!\n");
        return OT_ERR_VB_BUSY;
    }
    if (memcpy_s(&g_supplement_conf, sizeof(ot_vb_supplement_cfg), supplement_conf,
        sizeof(ot_vb_supplement_cfg)) != EOK) {
        osal_up(&g_sema);
        ot_trace_vb(OT_DBG_ERR, "memcpy_s fail!\n");
        return OT_ERR_VB_NOT_PERM;
    }
    osal_up(&g_sema);

    return TD_SUCCESS;
}
td_s32 vb_get_supplement_conf(ot_vb_supplement_cfg *supplement_conf)
{
    if (supplement_conf == TD_NULL) {
        ot_trace_vb(OT_DBG_ERR, "virt addr is null.\n");
        return OT_ERR_VB_NULL_PTR;
    }
    if (memcpy_s(supplement_conf, sizeof(ot_vb_supplement_cfg), &g_supplement_conf,
        sizeof(ot_vb_supplement_cfg)) != EOK) {
        ot_trace_vb(OT_DBG_ERR, "memcpy_s fail!\n");
        return OT_ERR_VB_NOT_PERM;
    }
    return TD_SUCCESS;
}
#endif

static td_void vb_common_copy_supplement_info(td_void *dst_virt_addr, td_u32 dst_size,
    const td_void *src_virt_addr, td_u32 src_size)
{
    if ((src_virt_addr == TD_NULL) || (dst_virt_addr == TD_NULL)) {
        ot_trace_vb(OT_DBG_WARN, "virt addr is null.\n");
        return;
    }
    if (memcpy_s(dst_virt_addr, dst_size, src_virt_addr, src_size) != EOK) {
        ot_trace_vb(OT_DBG_ERR, "memcpy_s fail!\n");
        return;
    }
}
td_void vb_copy_supplement_info(const ot_video_supplement *src_supp, const ot_video_supplement *dst_supp)
{
    td_void *dst_virt_addr = TD_NULL;
    td_void *src_virt_addr = TD_NULL;

    if (vb_is_supplement_support(OT_VB_SUPPLEMENT_JPEG_MASK)) {
        dst_virt_addr = dst_supp->jpeg_dcf_virt_addr;
        src_virt_addr = src_supp->jpeg_dcf_virt_addr;
        vb_common_copy_supplement_info(dst_virt_addr, sizeof(ot_jpeg_dcf), src_virt_addr, sizeof(ot_jpeg_dcf));
    }
    if (vb_drv_is_isp_frame_info_supported()) {
        dst_virt_addr = dst_supp->isp_info_virt_addr;
        src_virt_addr = src_supp->isp_info_virt_addr;
        vb_common_copy_supplement_info(dst_virt_addr, sizeof(ot_isp_frame_info),
            src_virt_addr, sizeof(ot_isp_frame_info));
    }
    if (vb_is_supplement_support(OT_VB_SUPPLEMENT_DNG_MASK)) {
        dst_virt_addr = dst_supp->frame_dng_virt_addr;
        src_virt_addr = src_supp->frame_dng_virt_addr;
        vb_common_copy_supplement_info(dst_virt_addr, sizeof(ot_dng_image_dynamic_info),
            src_virt_addr, sizeof(ot_dng_image_dynamic_info));
    }
    if (vb_drv_is_misc_supported(OT_VB_SUPPLEMENT_MISC_MASK) == TD_TRUE) {
        dst_virt_addr = dst_supp->misc_info_virt_addr;
        src_virt_addr = src_supp->misc_info_virt_addr;
        vb_common_copy_supplement_info(dst_virt_addr, sizeof(ot_video_supplement_misc),
            src_virt_addr, sizeof(ot_video_supplement_misc));
    }
    if (vb_is_supplement_support(OT_VB_SUPPLEMENT_BNR_MOT_MASK)) {
        dst_virt_addr = dst_supp->bnr_mot_virt_addr;
        src_virt_addr = src_supp->bnr_mot_virt_addr;
        vb_common_copy_supplement_info(dst_virt_addr, sizeof(bnr_mot_info), src_virt_addr, sizeof(bnr_mot_info));
    }
    if (vb_is_supplement_support(OT_VB_SUPPLEMENT_MOTION_DATA_MASK)) {
        dst_virt_addr = dst_supp->motion_data_virt_addr;
        src_virt_addr = src_supp->motion_data_virt_addr;
        vb_common_copy_supplement_info(dst_virt_addr, sizeof(dis_inner_motion_data_info),
            src_virt_addr, sizeof(dis_inner_motion_data_info));
    }
}
td_void vb_copy_supplement(ot_video_frame *dst_v_frame, const ot_video_frame *src_v_frame)
{
    vb_blk_handle handle;
    ot_video_supplement *src_supp = TD_NULL;
    ot_video_supplement *dst_supp = TD_NULL;

    handle = vb_phy_to_handle(dst_v_frame->phys_addr[0]);
    if (handle != OT_VB_INVALID_HANDLE) {
        dst_supp = vb_handle_to_supplement(handle);
        if (dst_supp == TD_NULL) {
            ot_trace_vb(OT_DBG_ERR, "dst_supp is null!\n");
            return;
        }
    } else {
        ot_trace_vb(OT_DBG_INFO, "dst VB handle is %d,phy_addr 0x%lx!!!\n", OT_VB_INVALID_HANDLE,
            (td_ulong)dst_v_frame->phys_addr[0]);
        return;
    }
    handle = vb_phy_to_handle(src_v_frame->phys_addr[0]);
    if (handle != OT_VB_INVALID_HANDLE) {
        src_supp = vb_handle_to_supplement(handle);
        if (src_supp == TD_NULL) {
            ot_trace_vb(OT_DBG_ERR, "src_supp is null!\n");
            return;
        }
    } else {
        dst_supp->jpeg_dcf_virt_addr = TD_NULL;
        dst_supp->jpeg_dcf_phys_addr = 0;
        ot_trace_vb(OT_DBG_INFO, "src VB handle is %d,phy_addr 0x%lx!!!\n", OT_VB_INVALID_HANDLE,
            (td_ulong)src_v_frame->phys_addr[0]);
        return;
    }
    vb_copy_supplement_info(src_supp, dst_supp);
    return;
}
ot_video_supplement *vb_handle_to_supplement(vb_blk_handle handle)
{
    td_s32 ret;
    unsigned long flags;
    ot_video_supplement *supplement = TD_NULL;
    td_u32 pool_id = vb_hdle_to_pool_id(handle);
    td_u32 blk_id = vb_hdle_to_blk_id(handle);

    vb_check_initialization_status_return(TD_NULL);
    vb_check_pool_id_return(pool_id, TD_NULL);

    vb_spin_lock(flags);
    if (g_pools[pool_id] == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool %u is already destroyed !\n", pool_id);
        vb_spin_unlock(flags);
        return TD_NULL;
    }
    ret = vb_check_block_id(pool_id, blk_id);
    if (ret != TD_SUCCESS) {
        vb_spin_unlock(flags);
        return TD_NULL;
    }
    if ((g_blks[pool_id] + blk_id) == TD_NULL) {
        ot_trace_vb(OT_DBG_WARN, "pool [%u] doesn't have blocks! \n", pool_id);
        vb_spin_unlock(flags);
        return TD_NULL;
    }
    supplement = &((g_blks[pool_id] + blk_id)->supplement);
    vb_spin_unlock(flags);
    return supplement;
}
td_bool vb_is_supplement_support(td_u32 mask)
{
    return (g_supplement_conf.supplement_cfg & mask);
}
static td_void vb_free_pools_and_blks_buf(td_void)
{
    if (g_blks != TD_NULL) {
        osal_kfree(g_blks);
    }
    if (g_pools != TD_NULL) {
        osal_kfree(g_pools);
    }
    g_blks = TD_NULL;
    g_pools = TD_NULL;
}
static td_s32 vb_init_pool_cfg(td_void)
{
    td_s32 ret;
    td_u32 i;
    td_u32 pool_id;
    vb_pool_common_info common_info;
    td_u32 buf_len = g_vb_conf.max_pool_cnt * sizeof(td_void *);

    OSAL_INIT_LIST_HEAD(&g_comm_pools);
    g_pools = (vb_pool_attr **)osal_kmalloc(buf_len, osal_gfp_kernel);
    g_blks = (vb_blk_info **)osal_kmalloc(buf_len, osal_gfp_kernel);
    if ((g_pools == TD_NULL) || (g_blks == TD_NULL)) {
        ot_trace_vb(OT_DBG_EMERG, "no memory!\n");
        goto no_memory;
    }
    (td_void)memset_s(g_pools, buf_len, 0, buf_len);
    (td_void)memset_s(g_blks, buf_len, 0, buf_len);

    /* create common pools */
    for (i = 0; i < OT_VB_MAX_COMMON_POOLS; i++) {
        if (!g_vb_conf.common_pool[i].blk_cnt || !g_vb_conf.common_pool[i].blk_size) {
            continue;
        }
        common_info.blk_cnt = g_vb_conf.common_pool[i].blk_cnt;
        common_info.blk_size = g_vb_conf.common_pool[i].blk_size;
        common_info.owner = OT_POOL_OWNER_COMMON;
        common_info.uid         = OT_VB_UID_COMMON;
        common_info.vb_remap_mode = g_vb_conf.common_pool[i].remap_mode;
        (td_void)strncpy_s(common_info.buf_name, OT_MAX_MMZ_NAME_LEN, "vb_pool", OT_MAX_MMZ_NAME_LEN - 1);

        ret = create_pool(&common_info, &pool_id, g_vb_conf.common_pool[i].mmz_name);
        if (ret) {
            goto create_fail;
        }
    }
    return TD_SUCCESS;

create_fail:
    for (i = 0; i < g_vb_conf.max_pool_cnt; i++) {
        if (g_pools[i] != TD_NULL) {
            destroy_pool(g_pools[i]);
        }
    }
no_memory:
    vb_free_pools_and_blks_buf();
    return TD_FAILURE;
}
static td_s32 vb_do_vb_init(td_void)
{
    /* just for supplement configure */
    if (osal_down(&g_sema)) {
        return -ERESTARTSYS;
    }
    if (g_is_conf == TD_FALSE) {
        ot_trace_vb(OT_DBG_ERR, "please configurate VB first! \n");
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_READY;
    }
    if (g_pools != TD_NULL) {
        ot_trace_vb(OT_DBG_INFO, "vb_init again!\n");
        osal_up(&g_sema);
        return TD_SUCCESS;
    }
    if (vb_init_pool_cfg() != TD_SUCCESS) {
        osal_up(&g_sema);
        return OT_ERR_VB_NO_MEM;
    }
    ot_trace_vb(OT_DBG_DEBUG, "vb init ok!\n");

    osal_up(&g_sema);

    return TD_SUCCESS;
}
static td_s32 vb_do_vb_exit(td_void)
{
    td_u32 i;
    td_s32 ret;
    ret = osal_down(&g_sema);
#endif /* SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART06_H_ */
