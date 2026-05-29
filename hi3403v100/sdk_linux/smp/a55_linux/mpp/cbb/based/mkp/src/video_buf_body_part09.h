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

#ifndef SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART09_H_
#define SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART09_H_

    .pfn_vb_get_blk_by_size_and_module = vb_get_blk_by_size_and_module,
    .pfn_vb_get_blk_by_size_and_pool_id = vb_get_blk_by_size_and_pool_id,
    .pfn_vb_get_pool_id_by_size_and_module = vb_get_pool_id_by_size_and_module,
    .pfn_vb_get_pool_id = vb_get_pool_id,
    .pfn_vb_get_pool_info = vb_get_pool_info,
    .pfn_vb_put_blk = vb_put_blk,
    .pfn_vb_phy_to_handle = vb_phy_to_handle,
    .pfn_vb_handle_to_pool_id = vb_handle_to_pool_id,
    .pfn_vb_handle_to_blk_id = vb_handle_to_blk_id,
    .pfn_vb_handle_to_kern = vb_handle_to_kern,
    .pfn_vb_handle_to_phys = vb_handle_to_phys,
    .pfn_vb_handle_to_blk_size = vb_handle_to_blk_size,
    .pfn_vb_user_add = vb_user_add,
    .pfn_vb_user_sub = vb_user_sub,
    .pfn_vb_inquire_user_cnt = vb_inquire_user_cnt,
    .pfn_vb_inquire_one_user_cnt = vb_inquire_one_user_cnt,
    .pfn_vb_inquire_total_user_cnt = vb_inquire_total_user_cnt,
    .pfn_vb_inquire_blk_cnt = vb_inquire_blk_cnt,
    .pfn_vb_is_pool_id_valid = vb_is_pool_id_valid,
    .pfn_vb_is_blk_valid = vb_is_blk_valid,
    .pfn_vb_inquire_pool = vb_inquire_pool,
    .pfn_vb_inquire_pool_user_cnt = vb_inquire_pool_user_cnt,
    .pfn_vb_handle_to_supplement = vb_handle_to_supplement,
    .pfn_vb_is_supplement_support = vb_is_supplement_support,
    .pfn_vb_copy_supplement = vb_copy_supplement,
    .pfn_vb_get_mod_pool_vb_config = vb_get_mod_pool_vb_config,
#ifdef CONFIG_OT_VB_LOG_SUPPORT
    .pfn_vb_get_blk_by_size_ex = vb_get_blk_by_size_ex,
    .pfn_vb_get_blk_by_size_and_module_ex = vb_get_blk_by_size_and_module_ex,
    .pfn_vb_get_blk_by_size_and_pool_id_ex = vb_get_blk_by_size_and_pool_id_ex,
#endif
#ifdef CONFIG_OT_VB_ASYNC_SUPPORT
    .pfn_vb_async_destroy_pool = vb_async_destroy_pool,
#endif
};

static umap_module g_vb_module = {
    .mod_id = OT_ID_VB,
    .mod_name = OT_MPP_MOD_VB,

    .pfn_init = vb_mod_init,
    .pfn_exit = vb_mod_exit,
    .pfn_ver_checker = TD_NULL,

    .export_funcs = &g_export_funcs,
    .data = TD_NULL,
};

static td_s32 vb_default_init(td_void)
{
    (td_void)memset_s(&g_vb_conf, sizeof(g_vb_conf), 0, sizeof(g_vb_conf));
    g_is_conf = TD_FALSE;
    g_pools = TD_NULL;
    g_blks = TD_NULL;

    if (osal_sema_init(&g_sema, 1) < 0) {
        ot_trace_vb(OT_DBG_EMERG, "sema init failed!\n");
        return TD_FAILURE;
    }

    if (osal_spin_lock_init(&g_vb_spin_lock) < 0) {
        osal_sema_destroy(&g_sema);
        ot_trace_vb(OT_DBG_EMERG, "spinlock init failed!\n");
        return TD_FAILURE;
    }

#ifdef CONFIG_OT_VB_ASYNC_SUPPORT
    if (vb_async_destroy_ctx_init() != TD_SUCCESS) {
        osal_spin_lock_destroy(&g_vb_spin_lock);
        osal_sema_destroy(&g_sema);
        return TD_FAILURE;
    }
#endif

    return TD_SUCCESS;
}

td_s32 vb_init(td_void)
{
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    osal_proc_entry_t *proc = TD_NULL;
#endif
    g_device = osal_createdev(UMAP_DEVNAME_VB_BASE);
    if (g_device == NULL) {
        ot_trace_vb(OT_DBG_EMERG, "VB createdev failed!\n");
        goto fail0;
    }
    g_device->fops = &g_file_op;
    g_device->minor = UMAP_VB_MINOR_BASE;
    if (osal_registerdevice(g_device)) {
        ot_trace_vb(OT_DBG_EMERG, "VB register device failed!\n");
        goto fail1;
    }
    if (cmpi_register_module(&g_vb_module)) {
        ot_trace_vb(OT_DBG_EMERG, "vb register module failed!\n");
        goto fail2;
    }

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    /* create proc interface */
    proc = osal_create_proc_entry(PROC_ENTRY_VB, NULL);
    if (proc == TD_NULL) {
        ot_trace_vb(OT_DBG_EMERG, "vb register module failed!\n");
        goto fail3;
    }

    proc->read = vb_proc_show;
#endif
    if (vb_default_init() != TD_SUCCESS) {
        goto fail4;
    }

    ot_trace_vb(OT_DBG_INFO, "VB init OK!\n");
    return TD_SUCCESS;

fail4:
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    osal_remove_proc_entry(PROC_ENTRY_VB, NULL);
fail3:
#endif
    cmpi_unregister_module(OT_ID_VB);
fail2:
    osal_deregisterdevice(g_device);
fail1:
    osal_destroydev(g_device);
    g_device = TD_NULL;
fail0:
    osal_printk("load base.ko ...fail!\n");
    return TD_FAILURE;
}

td_s32 vb_exit(td_void)
{
    cmpi_unregister_module(OT_ID_VB);
    osal_remove_proc_entry(PROC_ENTRY_VB, NULL);
    osal_deregisterdevice(g_device);
    osal_destroydev(g_device);

    if (vb_do_vb_exit() != TD_SUCCESS) {
        ot_trace_vb(OT_DBG_ERR, "VB exit FAILED!\n");
    } else {
        ot_trace_vb(OT_DBG_INFO, "VB exit OK!\n");
    }

#ifdef CONFIG_OT_VB_ASYNC_SUPPORT
    vb_async_destroy_ctx_deinit();
#endif

    osal_sema_destroy(&g_sema);
    osal_spin_lock_destroy(&g_vb_spin_lock);

    return TD_SUCCESS;
}


#endif /* SMP_A55_LINUX_MPP_CBB_BASED_MKP_SRC_VIDEO_BUF_BODY_PART09_H_ */
