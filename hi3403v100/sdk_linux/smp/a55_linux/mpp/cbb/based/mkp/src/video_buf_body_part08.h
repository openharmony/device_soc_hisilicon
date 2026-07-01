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

#ifndef SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART08_H_
#define SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART08_H_


static td_s32 base_ioctl_handle(unsigned int cmd, unsigned long arg, td_bool *handled)
{
    *handled = TD_TRUE;
    switch (cmd) {
        case VB_PHYS2H_CTRL: {
            td_phys_addr_t *phy_addr = (td_phys_addr_t *)(td_uintptr_t)arg;
            vb_check_null_ptr_return(phy_addr);
            osal_down(&g_sema);
            *phy_addr = vb_phy_to_handle(*phy_addr);
            osal_up(&g_sema);
            return TD_SUCCESS;
        }
        case VB_H2PHYS_CTRL: {
            ot_vb_blk handle;
            vb_check_null_ptr_return(arg);
            handle = *(ot_vb_blk *)(td_uintptr_t)arg;
            return vb_do_handle_to_phys_addr(handle, (td_phys_addr_t *)(td_uintptr_t)arg);
        }
        case VB_H2POOL_CTRL: {
            ot_vb_blk handle;
            vb_check_null_ptr_return(arg);
            handle = *(td_u32 *)(td_uintptr_t)arg;
            return vb_do_handle_to_pool_id(handle, (ot_vb_pool *)(td_uintptr_t)arg);
        }
        case VB_H2USERCNT_CTRL: {
            vb_ioc_user_cnt *vb_user_cnt = TD_NULL;
            vb_check_null_ptr_return(arg);
            vb_user_cnt = (vb_ioc_user_cnt *)(td_uintptr_t)arg;
            return vb_do_inquire_user_cnt(vb_user_cnt->vb_blk, &vb_user_cnt->user_cnt);
        }
        default:
            *handled = TD_FALSE;
            return TD_FAILURE;
    }
}

static td_s32 base_ioctl_config(unsigned int cmd, unsigned long arg, td_bool *handled)
{
    *handled = TD_TRUE;
    switch (cmd) {
        case VB_GETSUPPLEMENTADDR_CTRL: {
            video_supplement_info *supplement_info = (video_supplement_info *)(td_uintptr_t)arg;
            vb_check_null_ptr_return(supplement_info);
            return vb_get_supplement_addr(supplement_info);
        }
        case VB_GET_CONF_CTRL:
            return vb_get_config((ot_vb_cfg *)(td_uintptr_t)arg);
        case VB_SET_CONF_CTRL:
            return vb_set_config((ot_vb_cfg *)(td_uintptr_t)arg);
        case VB_GET_MOD_CONF_CTRL:
            return vb_get_mod_pool_config((vb_ioc_cfg_arg *)(td_uintptr_t)arg);
        case VB_SET_MOD_CONF_CTRL:
            return vb_set_mod_pool_config((vb_ioc_cfg_arg *)(td_uintptr_t)arg);
        default:
            *handled = TD_FALSE;
            return TD_FAILURE;
    }
}

static td_s32 base_ioctl_get_info(unsigned long arg)
{
    td_s32 ret;
    vb_pool_info *info = (vb_pool_info *)(td_uintptr_t)arg;

    vb_check_null_ptr_return(info);
    osal_down(&g_sema);
    ret = vb_get_pool_info(info);
    osal_up(&g_sema);
    return ret;
}

static td_s32 base_ioctl_qry_user(unsigned long arg)
{
    td_s32 ret;
    td_u32 pool_id;

    vb_check_null_ptr_return(arg);
    osal_down(&g_sema);
    if (g_pools == TD_NULL) {
        OT_TRACE(OT_DBG_WARN, OT_ID_VB, "VB not initialized!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_NOT_READY;
    }

    pool_id = *(td_u32 *)(td_uintptr_t)arg;
    if ((pool_id >= g_vb_conf.max_pool_cnt) || !g_pools[pool_id]) {
        *(td_u32 *)(td_uintptr_t)arg = TD_NULL;
        ot_trace_vb(OT_DBG_ERR, "bad argument!\n");
        osal_up(&g_sema);
        return OT_ERR_VB_ILLEGAL_PARAM;
    }

    ret = vb_inquire_pool_user_cnt(pool_id, OT_VB_UID_USER, (td_u32 *)(td_uintptr_t)arg);
    osal_up(&g_sema);
    return ret;
}

static td_s32 base_ioctl_mod_pool(unsigned int cmd, unsigned long arg, td_bool *handled)
{
    *handled = TD_TRUE;
    switch (cmd) {
        case VB_INIT_POOL_CTRL:
            return vb_do_vb_init();
        case VB_EXIT_POOL_CTRL:
            return vb_do_vb_exit();
        case VB_INIT_MOD_POOL_CTRL: {
            ot_vb_uid uid;
            vb_check_null_ptr_return(arg);
            uid = *(ot_vb_uid *)(td_uintptr_t)arg;
            return vb_do_mod_vb_init(uid);
        }
        case VB_EXIT_MOD_POOL_CTRL: {
            td_u32 uid;
            vb_check_null_ptr_return(arg);
            uid = *(td_u32 *)(td_uintptr_t)arg;
            return vb_do_mod_vb_exit(uid);
        }
        default:
            *handled = TD_FALSE;
            return TD_FAILURE;
    }
}

static long base_ioctl(unsigned int cmd, unsigned long arg, td_void *private_data)
{
    td_s32 ret;
    td_bool handled;

    ot_unused(private_data);
    ret = base_ioctl_pool(cmd, arg, &handled);
    if (handled == TD_TRUE) {
        return ret;
    }
    ret = base_ioctl_handle(cmd, arg, &handled);
    if (handled == TD_TRUE) {
        return ret;
    }
    ret = base_ioctl_config(cmd, arg, &handled);
    if (handled == TD_TRUE) {
        return ret;
    }
    ret = base_ioctl_mod_pool(cmd, arg, &handled);
    if (handled == TD_TRUE) {
        return ret;
    }

    switch (cmd) {
        case VB_GET_INFO_CTRL:
            return base_ioctl_get_info(arg);
        case VB_QRY_USER_CTRL:
            return base_ioctl_qry_user(arg);
#ifdef CONFIG_OT_VB_SUPPLEMENT_MASK_SUPPORT
        case VB_SET_SUPPLEMENT_CONF_CTRL:
            return vb_set_supplement_conf((ot_vb_supplement_cfg *)(td_uintptr_t)arg);

        case VB_GET_SUPPLEMENT_CONF_CTRL:
            return vb_get_supplement_conf((ot_vb_supplement_cfg *)(td_uintptr_t)arg);
#endif

        default: {
            ot_trace_vb(OT_DBG_ERR, "ioctl cmd does NOT exist!\n");
            ret = TD_FAILURE;
        }
    }
    return ret;
}

#ifdef CONFIG_COMPAT
static long base_compat_ioctl(unsigned int cmd, unsigned long arg, td_void *private_data)
{
    switch (cmd) {
        case VB_GETSUPPLEMENTADDR_CTRL: {
            video_supplement_info *supplement_info = (video_supplement_info *)(td_uintptr_t)arg;

            OT_COMPAT_POINTER(supplement_info->supplement, ot_video_supplement *);
            break;
        }

        default: {
            break;
        }
    }

    return base_ioctl(cmd, arg, private_data);
}
#endif

static td_s32 open(td_void *data)
{
    ot_unused(data);
    return 0;
}

static td_s32 close(td_void *data)
{
    ot_unused(data);
    return 0;
}

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT

static void vb_proc_show_common_pool(struct osal_proc_dir_entry *s)
{
    td_u32 i;

    osal_seq_printf(s, "----------------------------------------common pool config"
        "--------------------------------------------------------------\n");
    osal_seq_printf(s, "pool_id");
    for (i = 0; i < OT_VB_MAX_COMMON_POOLS; i++) {
        osal_seq_printf(s, "%10u", i);
    }
    osal_seq_printf(s, "\nsize   ");
    for (i = 0; i < OT_VB_MAX_COMMON_POOLS; i++) {
        osal_seq_printf(s, "%10llu", vb_get_aligned_size(g_vb_conf.common_pool[i].blk_size));
    }
    osal_seq_printf(s, "\ncount  ");
    for (i = 0; i < OT_VB_MAX_COMMON_POOLS; i++) {
        osal_seq_printf(s, "%10u", g_vb_conf.common_pool[i].blk_cnt);
    }
    osal_seq_printf(s, "\n");
}

static void vb_proc_module_pool_cfg(struct osal_proc_dir_entry *s)
{
    td_u32 i;
    td_u32 m;

    for (m = 0; m < OT_VB_MAX_USER; m++) {
        if (g_is_mod_conf[m] == TD_FALSE) {
            continue;
        }
        if (g_is_mod_pool_init[m] == TD_FALSE) {
            continue;
        }

        osal_seq_printf(s, "----------------------------------------module common pool "
            "config of vb_uid <%d>-----------------------------------------\n", m);

        osal_seq_printf(s, "pool_id");
        for (i = 0; i < OT_VB_MAX_MOD_COMMON_POOLS; i++) {
            osal_seq_printf(s, "%10u", i);
        }
        osal_seq_printf(s, "\nsize   ");
        for (i = 0; i < OT_VB_MAX_MOD_COMMON_POOLS; i++) {
            osal_seq_printf(s, "%10llu", g_vb_mod_conf[m].common_pool[i].blk_size);
        }
        osal_seq_printf(s, "\ncount  ");
        for (i = 0; i < OT_VB_MAX_MOD_COMMON_POOLS; i++) {
            osal_seq_printf(s, "%10u", g_vb_mod_conf[m].common_pool[i].blk_cnt);
        }
        osal_seq_printf(s, "\n");
    }
}
static void vb_show_blk_by_module_pool(struct osal_proc_dir_entry *s, vb_pool_attr *pool)
{
    td_u32 k;
    vb_blk_info *blk = TD_NULL;
    struct osal_list_head *pos = TD_NULL;

    /* OT_VB_UID_COMMON not print! */
    td_char *apc_user_name[OT_VB_MAX_USER] = {
        "vi    ", "vo    ", "vgs   ", "venc  ", "vdec  ", "h265e ", "h264e ", "jpege ",
        "jpegd ", "vpss  ", "dis   ", "user  ", "pciv  ", "ai    ", "aenc  ", "rc    ",
        "vfmw  ", "gdc   ", "avs   ", "rect  ", "match ", "mcf   ", "vda   ", "vpp   ",
        TD_NULL, "uvc   ", "vdec_adapt   "
    };

    osal_seq_printf(s, "blk   ");
    for (k = 0; k < OT_VB_MAX_USER; k++) {
        if (!apc_user_name[k]) {
            continue;
        }
        osal_seq_printf(s, "%6s", apc_user_name[k]);
    }
    osal_seq_printf(s, "\n");

    osal_list_for_each(pos, &pool->busy) {
        blk = osal_list_entry(pos, vb_blk_info, list);
        osal_seq_printf(s, "%-6u", blk->blk_id);

        for (k = 0; k < OT_VB_MAX_USER; k++) {
            if (k == OT_VB_UID_COMMON) {
                continue;
            }
            osal_seq_printf(s, "%-6u", blk->user_cnt[k]);
            if (blk->user_cnt[k] > 0) {
                pool->user_cnt_sum[k]++;
            }
        }
        osal_seq_printf(s, "\n");
    }

    osal_seq_printf(s, "sum   ");
    for (k = 0; k < OT_VB_MAX_USER; k++) {
        if (k == OT_VB_UID_COMMON) {
            continue;
        }
        osal_seq_printf(s, "%-6u", pool->user_cnt_sum[k]);
        pool->user_cnt_sum[k] = 0;
    }
    osal_seq_printf(s, "\n");
}

static void vb_proc_show_module_pool(struct osal_proc_dir_entry *s)
{
    td_u32 i;
    td_u32 j;
    unsigned long flags;
    vb_pool_attr *pool = TD_NULL;
    struct osal_list_head *pos = TD_NULL;

    /* module common pool configure proc show */
    vb_proc_module_pool_cfg(s);

    for (i = 0, j = 0; i < g_vb_conf.max_pool_cnt; i++) {
        vb_spin_lock(flags);
        if (g_pools[i] == TD_NULL) {
            vb_spin_unlock(flags);
            continue;
        }

        pool = g_pools[i];
        j = 0;
        osal_list_for_each(pos, &pool->free_list) {
            j++;
        }
        osal_seq_printf(s, "----------------------------------------%s----------------"
            "----------------------------------------------------------------\n", pool->ac_pool_name);
        osal_seq_printf(s, "%-8s" "%-20s" "%-20s" "%-8s" "%-7s" "%-15s" "%-8s" "%-8s" "%-8s" "\n",
            "pool_id", "phys_addr", "virt_addr", "is_comm", "owner", "blk_sz", "blk_cnt", "free_list", "min_free");
        osal_seq_printf(s, "%-8u" "0x%-18lx" "0x%-18lx" "%-8d" "%-7d" "%-15llu" "%-8d" "%-8u" "%-8u" "\n",
            pool->pool_id, (td_ulong)pool->pool_phy_addr, (td_ulong)(td_uintptr_t)pool->pool_vir_addr,
            pool->is_comm_pool, pool->pool_owner, pool->blk_size, pool->blk_cnt,
            pool->free_blk_cnt, pool->min_free_blk_cnt);

        if (j == pool->blk_cnt) {
            vb_spin_unlock(flags);
            continue;
        }

        vb_show_blk_by_module_pool(s, pool);

        vb_spin_unlock(flags);
    }
}

static td_s32 vb_proc_show(struct osal_proc_dir_entry *s)
{
    td_u32 i;
    unsigned long flags;
    vb_pool_attr *pool = TD_NULL;
    td_u32 cnt = 0;

    osal_seq_printf(s, "\n[VB] Version: [" OT_MPP_VERSION "], Build Time["__DATE__", "__TIME__"]\n");
    osal_seq_printf(s, "\n");

    osal_seq_printf(s, "----------------------------------------vb pub config---------------------"
        "----------------------------------------------\n");
    osal_seq_printf(s, "max_pool_cnt\n");
    osal_seq_printf(s, "%10d\n", g_vb_conf.max_pool_cnt);

    osal_seq_printf(s, "----------------------------------------vb supplement attr---------------"
        "-----------------------------------------------\n");
    osal_seq_printf(s, "%8s" "%8s" "%9s" "\n", "config", "size", "vb_cnt");

    osal_seq_printf(s, "%8d%8d", g_supplement_conf.supplement_cfg,
                    vb_get_aligned_size(vb_get_supplement_size(&g_supplement_conf, TD_TRUE)));

    if (osal_down(&g_sema)) {
        return -ERESTARTSYS;
    }

    if (g_pools == TD_NULL) {
        osal_up(&g_sema);
        osal_seq_printf(s, "%9d\n", cnt);
        return 0;
    }

    vb_spin_lock(flags);
    for (i = 0; i < g_vb_conf.max_pool_cnt; i++) {
        if (g_pools[i] == TD_NULL) {
            continue;
        }
        pool = g_pools[i];
        cnt += pool->blk_cnt;
    }
    vb_spin_unlock(flags);
    osal_seq_printf(s, "%9d\n", cnt);

    vb_proc_show_common_pool(s);
    vb_proc_show_module_pool(s);

    osal_up(&g_sema);
    osal_seq_printf(s, "\n");

    return 0;
}
#endif

static struct osal_fileops g_file_op = {
    .open = open,
    .unlocked_ioctl = base_ioctl,
    .release = close,
#ifdef CONFIG_COMPAT
    .compat_ioctl = base_compat_ioctl,
#endif
};

static osal_dev_t *g_device;

td_s32 vb_mod_init(td_void *p)
{
    ot_unused(p);
    g_vb_init = TD_TRUE;
    return TD_SUCCESS;
}

td_void vb_mod_exit(td_void)
{
    g_vb_init = TD_FALSE;
    (td_void)memset_s(&g_supplement_conf, sizeof(g_supplement_conf), 0, sizeof(g_supplement_conf));
    return;
}

static vb_export_func g_export_funcs = {
    .pfn_vb_create_pool = vb_create_pool,
    .pfn_vb_destroy_pool = vb_destroy_pool,
    .pfn_vb_get_blk_by_pool_id = vb_get_blk_by_pool_id,
    .pfn_vb_get_blk_by_size = vb_get_blk_by_size,


#endif /* SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART08_H_ */
