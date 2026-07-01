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
#include "mkp_vo_init.h"
#include "mkp_vo_proc.h"
#include "mkp_vo_dev.h"
#include "dev_ext.h"
#include "ot_board.h"
#include "proc_ext.h"
#include "drv_vo_comm.h"
#include "mkp_vo_ioctl.h"
#include "ot_comm_irq.h"
#include "drv_vo_dev_comm.h"
#include "drv_vo_dev.h"
#include "vou.h"
#include "drv_vo.h"
#include "vou_ext.h"
#include "proc_ext.h"

#if vo_desc("spin lock")

static osal_spinlock_t g_vo_lock;

osal_spinlock_t *vo_get_dev_lock(td_void)
{
    return &g_vo_lock;
}

static td_s32 vo_init_spin_lock(td_void)
{
    td_s32 ret;
    osal_spinlock_t *vo_lock = vo_get_dev_lock();

    ret = osal_spin_lock_init(vo_lock);
    if (ret != TD_SUCCESS) {
        vo_alert_trace("spin_lock_init fail = %d\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

static td_void vo_destory_spin_lock(td_void)
{
    osal_spinlock_t *vo_lock = vo_get_dev_lock();

    osal_spin_lock_destroy(vo_lock);
}

#endif

td_void vo_set_init_vo_irq(unsigned int vo_irq)
{
    ot_unused(vo_irq);
}

#if vo_desc("semaphore")

static osal_semaphore_t g_dev_sema;

osal_semaphore_t *vo_get_dev_sema(td_void)
{
    return &g_dev_sema;
}

static td_s32 vo_init_sema(td_void)
{
    td_s32 ret;
    osal_semaphore_t *vo_dev_sema = vo_get_dev_sema();

    ret = osal_sema_init(vo_dev_sema, VO_SEMAPHORE_VAL);
    if (ret != TD_SUCCESS) {
        vo_alert_trace("sema_init fail = %d\n", ret);
        return ret;
    }

    return TD_SUCCESS;
}

static td_void vo_destory_sema(td_void)
{
    osal_semaphore_t *vo_dev_sema = vo_get_dev_sema();

    osal_sema_destroy(vo_dev_sema);
}

#endif

#if vo_desc("init sync res")

static td_s32 vo_init_sync_res(td_void)
{
    td_s32 ret;

    ret = vo_init_spin_lock();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = vo_init_sema();
    if (ret != TD_SUCCESS) {
        vo_destory_spin_lock();
        return ret;
    }

    return TD_SUCCESS;
}

static td_void vo_deinit_sync_res(td_void)
{
    vo_destory_sema();
    vo_destory_spin_lock();
}

#endif

#if vo_desc("atomic operation")

static osal_atomic_t g_vo_user_ref = OSAL_ATOMIC_INIT(0);

static osal_atomic_t *vo_get_user_ref(td_void)
{
    return &g_vo_user_ref;
}

static td_s32 vo_init_atomic(td_void)
{
    td_s32 ret;
    osal_atomic_t *vo_user_ref = vo_get_user_ref();

    ret = osal_atomic_init(vo_user_ref);
    if (ret < 0) {
        vo_alert_trace("vo atomic_init failed = %d\n", ret);
        return ret;
    }

    osal_atomic_set(vo_user_ref, 0);
    return TD_SUCCESS;
}

static td_void vo_deinit_atomic(td_void)
{
    osal_atomic_t *vo_user_ref = vo_get_user_ref();
    osal_atomic_destroy(vo_user_ref);
}

static td_s32 vo_atomic_read(td_void)
{
    osal_atomic_t *vo_user_ref = vo_get_user_ref();
    return osal_atomic_read(vo_user_ref);
}

static td_void vo_atomic_inc_return(td_void)
{
    td_s32 ret;
    osal_atomic_t *vo_user_ref = vo_get_user_ref();

    ret = osal_atomic_inc_return(vo_user_ref);
    if (ret < 0) {
        vo_alert_trace("vo atomic_inc_return failed = %d\n", ret);
    }
}

static td_void vo_atomic_dec_return(td_void)
{
    td_s32 ret;
    osal_atomic_t *vo_user_ref = vo_get_user_ref();

    ret = osal_atomic_dec_return(vo_user_ref);
    if (ret < 0) {
        vo_alert_trace("vo atomic_dec_return failed = %d\n", ret);
    }
}

#endif

#if vo_desc("init proc")

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
static td_s32 vo_init_proc(td_void)
{
    osal_proc_entry_t *proc = TD_NULL;

    proc = osal_create_proc_entry(PROC_ENTRY_VO_DEV, TD_NULL);
    if (proc == TD_NULL) {
        vo_alert_trace("vo create proc fail!\n");
        return OT_ERR_VO_NULL_PTR;
    }

    proc->read = vo_proc_show;
    return TD_SUCCESS;
}

static td_void vo_deinit_proc(td_void)
{
    osal_remove_proc_entry(PROC_ENTRY_VO_DEV, TD_NULL);
}

#else

static td_void vo_deinit_proc(td_void)
{
    /* 不支持PROC时异常释放为空函数，使用空函数避免宏乱用 */
}
#endif


#endif

#if vo_desc("export symbol")
#ifdef CONFIG_OT_VO_EXPORT_FUNCTION
static ot_vo_export_symbol g_vo_export_symbol = {0};
static ot_vo_export_callback g_vo_export_callback = {0};

ot_vo_export_symbol *ot_vo_get_export_symbol(td_void)
{
    return &g_vo_export_symbol;
}

ot_vo_export_callback *vo_get_export_callback(td_void)
{
    return &g_vo_export_callback;
}

static td_void vo_init_export_info(td_void)
{
    ot_vo_export_symbol *vo_export_symbol = ot_vo_get_export_symbol();
    ot_vo_export_callback *vo_export_callback = vo_get_export_callback();

    (td_void)memset_s(vo_export_symbol, sizeof(ot_vo_export_symbol), 0, sizeof(ot_vo_export_symbol));
    (td_void)memset_s(vo_export_callback, sizeof(ot_vo_export_callback), 0, sizeof(ot_vo_export_callback));
}

static td_s32 vo_register_export_callback(ot_vo_export_callback *export_callback)
{
    ot_vo_export_callback *vo_export_callback = vo_get_export_callback();

    if (export_callback == TD_NULL) {
        vo_err_trace("vo register export callback fail!\n");
        return OT_ERR_VO_NULL_PTR;
    }

    vo_export_callback->vo_notify = export_callback->vo_notify;
    vo_export_callback->vo_user_notify = export_callback->vo_user_notify;
    return TD_SUCCESS;
}

static td_void vo_init_export(td_void)
{
    ot_vo_export_symbol *vo_export_symbol = ot_vo_get_export_symbol();

    vo_init_export_info();
    vo_export_symbol->vo_register_export_callback = vo_register_export_callback;
}

static td_void vo_deinit_export(td_void)
{
    vo_init_export_info();
}

#else

ot_vo_export_symbol *ot_vo_get_export_symbol(td_void)
{
    vo_err_trace("current chip does not support export function!\n");
    return TD_NULL;
}
#endif /* end of CONFIG_OT_VO_EXPORT_FUNCTION */
#endif /* end of vo_desc("export symbol") */

#if vo_desc("vo reg")

td_void vo_set_init_vo_reg(void *reg)
{
    vo_drv_set_reg((volatile reg_vdp_regs *)reg);
}

#endif

#if vo_desc("file operation")

static long vo_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    int ret;
    vo_ioctl_para ioctl_para = {0};

    if (vo_is_vo_started() != TD_TRUE) {
        vo_err_trace("vo not init!\n");
        return OT_ERR_VO_NOT_READY;
    }

    vo_atomic_inc_return();
    ioctl_para.cmd = cmd;
    ioctl_para.arg = (td_uintptr_t)arg;
    ioctl_para.private_data = private_data;
    ret = vo_device_ioctl(&ioctl_para);
    vo_atomic_dec_return();

    return ret;
}

#ifdef CONFIG_COMPAT
static long vo_compat_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    int ret;
    vo_ioctl_para ioctl_para = {0};

    if (vo_is_vo_started() != TD_TRUE) {
        vo_err_trace("vo not init!\n");
        return OT_ERR_VO_NOT_READY;
    }

    vo_atomic_inc_return();
    ioctl_para.cmd = cmd;
    ioctl_para.arg = (td_uintptr_t)arg;
    ioctl_para.private_data = private_data;
    ret = vo_device_compat_ioctl(&ioctl_para);
    vo_atomic_dec_return();

    return ret;
}
#endif

static td_s32 vo_open(void *private_data)
{
    ot_unused(private_data);
    return TD_SUCCESS;
}

static td_s32 vo_close(void *private_data)
{
    ot_unused(private_data);
    return TD_SUCCESS;
}

static unsigned int vo_poll(osal_poll_t *osal_poll, void *private_data)
{
    ot_unused(osal_poll);
    ot_unused(private_data);
    return TD_SUCCESS;
}

static struct osal_fileops g_vo_file_op = {
    .open = vo_open,
    .unlocked_ioctl = vo_ioctl,
    .release = vo_close,
    .poll = vo_poll,
#ifdef CONFIG_COMPAT
    .compat_ioctl = vo_compat_ioctl,
#endif
};

#endif

#if vo_desc("init device")

static osal_dev_t *g_vo_device = TD_NULL;

static osal_dev_t *vo_get_vo_device(td_void)
{
    return g_vo_device;
}

static td_void vo_set_vo_device(osal_dev_t *vo_device)
{
    g_vo_device = vo_device;
}

static td_s32 vo_create_device(td_void)
{
    osal_dev_t *vo_device = vo_get_vo_device();

    vo_device = osal_createdev(UMAP_DEVNAME_VO_DEV_BASE);
    if (vo_device == TD_NULL) {
        vo_alert_trace("vo create device failed\n");
        return OT_ERR_VO_NULL_PTR;
    }

    vo_set_vo_device(vo_device);

    vo_device->fops = &g_vo_file_op;
    vo_device->minor = UMAP_VO_DEV_MINOR_BASE;

    return TD_SUCCESS;
}

static td_void vo_destroy_device(td_void)
{
    td_s32 ret;
    osal_dev_t *vo_device = vo_get_vo_device();

    ret = osal_destroydev(vo_device);
    if (ret < 0) {
        vo_alert_trace("vo destroy device failed\n");
    }

    vo_set_vo_device(TD_NULL);
}

static td_s32 vo_register_device(td_void)
{
    td_s32 ret;
    osal_dev_t *vo_device = vo_get_vo_device();

    ret = osal_registerdevice(vo_device);
    if (ret < 0) {
        vo_alert_trace("vo register device failed\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void vo_unregister_device(td_void)
{
    osal_dev_t *vo_device = vo_get_vo_device();

    osal_deregisterdevice(vo_device);
}

static td_s32 vo_init_device(td_void)
{
    td_s32 ret;

    ret = vo_create_device();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = vo_register_device();
    if (ret != TD_SUCCESS) {
        vo_destroy_device();
        return ret;
    }

    return TD_SUCCESS;
}

static td_void vo_deinit_device(td_void)
{
    vo_unregister_device();
    vo_destroy_device();
}

#endif

static td_void vo_set_drv_dev_int_enable(td_void)
{
    ot_vo_dev dev;

    if (vo_get_mod_param_exit_dev_en() == TD_TRUE) {
        return;
    }

    for (dev = 0; dev < OT_VO_MAX_PHYS_DEV_NUM; dev++) {
        vo_drv_dev_int_enable(dev, TD_TRUE);
    }
}

#if vo_desc("clk init")

static td_void vo_init_set_sys_clk(td_void)
{
    /* 撤销复位 */
    vo_lpw_bus_reset(TD_FALSE);
}

static td_void vo_init_crg_clk(td_void)
{
    td_ulong flags = 0;

    vo_dev_spin_lock(&flags);
    vo_drv_set_all_crg_clk(TD_TRUE);
    vo_dev_spin_unlock(&flags);
}

static td_void vo_init_default_setting(td_void)
{
    td_ulong flags = 0;

    vo_dev_spin_lock(&flags);
    vo_drv_default_setting();
    vo_dev_spin_unlock(&flags);
}

static td_void vo_exit_set_clk(td_void)
{
    td_ulong flags = 0;

    /* 如果是双核vo&fb分离模式，这里复位会导致图形层也被复位了。暂定这么解决，后续要求逻辑支持 */
    if (vo_get_mod_param_exit_dev_en() == TD_FALSE) {
        return;
    }

    /* 复位 */
    vo_lpw_bus_reset(TD_TRUE);

    vo_dev_spin_lock(&flags);
    /* 关闭时钟 */
    vo_drv_set_all_crg_clk(TD_FALSE);
    vo_dev_spin_unlock(&flags);
}

#endif

#if vo_desc("vo state flag")

static td_u32 g_vo_state = VOU_STATE_STOPPED;

static td_u32 vo_get_vo_state(td_void)
{
    return g_vo_state;
}

static td_void vo_set_vo_state(td_u32 vo_state)
{
    g_vo_state = vo_state;
}

td_bool vo_is_vo_started(td_void)
{
    td_u32 vo_state = vo_get_vo_state();
    return (vo_state == VOU_STATE_STARTED);
}

td_bool vo_is_vo_stopped(td_void)
{
    td_u32 vo_state = vo_get_vo_state();
    return (vo_state == VOU_STATE_STOPPED);
}

#endif

#if vo_desc("vo init flag")

/* 低功耗策略，加载ko时先打开时钟，初始化完成后关闭时钟init时撤销复位exit时复位 */
static td_bool g_is_initialized = TD_FALSE;

td_bool vo_get_vo_init_flag(td_void)
{
    return g_is_initialized;
}

static td_void vo_set_vo_init_flag(td_bool flag)
{
    g_is_initialized = flag;
}

#endif

#if vo_desc("init context")

static td_void vo_init_context(td_void)
{
    vo_init_dev_info();
}

static td_void vo_deinit_context(td_void)
{
    osal_semaphore_t *vo_dev_sema = vo_get_dev_sema();

    if (osal_down_interruptible(vo_dev_sema) != TD_SUCCESS) {
        vo_emerg_trace("down dev sem failed.\n");
        return;
    }

    vo_deinit_dev_info();

    vo_disable_all_dev_soft();
    osal_up(vo_dev_sema);
}

#endif

static td_void vo_set_low_power_cfg(td_void)
{
    td_ulong flags = 0;

    /* 如果是双核vo&fb分离模式，这里复位会导致图形层也被复位了。暂定这么解决，后续要求逻辑支持 */
    if (vo_get_mod_param_exit_dev_en() == TD_FALSE) {
        return;
    }

    /* drv层查询支持情况 */
    if (vo_drv_is_support_low_power() == TD_TRUE) {
        vo_dev_spin_lock(&flags);
        vo_disable_clk();
        vo_dev_spin_unlock(&flags);
    }
}

#if vo_desc("module operation")

td_s32 vo_init(td_void *arg)
{
    ot_unused(arg);

    vo_init_set_sys_clk();

    /* 如果已启动，返回成功  */
    if (vo_is_vo_started()) {
        return TD_SUCCESS;
    }

    /* 使能时钟 */
    vo_init_crg_clk();

    vo_init_context();

    /* 默认硬件配置 */
    vo_init_default_setting();

    /* 标识模块已经启动 */
    vo_set_vo_state(VOU_STATE_STARTED);

    vo_set_vo_init_flag(TD_TRUE);

    vo_set_drv_dev_int_enable();

    /* 低功耗策略 */
    vo_set_low_power_cfg();

    return TD_SUCCESS;
}

td_void vo_exit(td_void)
{
    td_ulong flags = 0;

    if (vo_is_vo_stopped()) {
        return;
    }

    /*
     * 1 系统上来就初始化，信号量没有初始化
     * 1 系统去初始化时也是可以响应中断的
     */
    if (vo_get_mod_param_exit_dev_en() == TD_TRUE) {
        vo_dev_spin_lock(&flags);
        /* 打开时钟 */
        vo_drv_set_all_crg_clk(TD_TRUE);
        vo_dev_spin_unlock(&flags);
    }

    vo_exit_dev();

    /* 清除中断掩码，使不再上报中断 */
    vou_drv_int_disable_all();
    /* 清除中断状态 */
    vou_drv_int_clear(VOU_INTCLEAR_ALL);

    vo_deinit_context();

    vo_exit_set_clk();

    vo_set_vo_state(VOU_STATE_STOPPED);
    vo_debug_trace("vo exit!\n");
}

static td_void vo_notify(mod_notice_id notice)
{
    osal_semaphore_t *vo_dev_sema = vo_get_dev_sema();

    if (notice != MOD_NOTICE_STOP) {
        return;
    }

    vo_set_vo_state(VOU_STATE_STOPPING);

    /* 获取设备信号量 */
    if (osal_down_interruptible(vo_dev_sema) != TD_SUCCESS) {
        return;
    }

    /* 释放设备信号量 */
    osal_up(vo_dev_sema);
}

static td_void vo_query_state(mod_state *state)
{
    if (vo_atomic_read() == 0) {
        *state = MOD_STATE_FREE;
    } else {
        *state = MOD_STATE_BUSY;
    }
}

static td_u32 vo_get_ver_magic(td_void)
{
    return VERSION_MAGIC;
}

#endif

#if vo_desc("module register")

static umap_module g_vo_module = {
    .mod_id = OT_ID_VO,
    .mod_name = "vo",

    .pfn_init = vo_init,
    .pfn_exit = vo_exit,
    .pfn_query_state = vo_query_state,
    .pfn_notify = vo_notify,
    .pfn_ver_checker = vo_get_ver_magic,

    .export_funcs = TD_NULL,
    .data = TD_NULL,
};

static td_s32 vo_register_module(td_void)
{
    if (cmpi_register_module(&g_vo_module)) {
        vo_alert_trace("vo mod register failed\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void vo_unregister_module(td_void)
{
    cmpi_unregister_module(OT_ID_VO);
}

#endif

#if vo_desc("module init")

static int vo_do_mod_init(void)
{
    td_s32 ret;

    if (ckfn_sys_entry() == TD_FALSE) {
        vo_err_trace("no sys ko!\n");
        return OT_ERR_VO_NOT_READY;
    }

    ret = vo_init_sync_res();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = vo_init_device();
    if (ret != TD_SUCCESS) {
        vo_deinit_sync_res();
        return ret;
    }

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    ret = vo_init_proc();
    if (ret != TD_SUCCESS) {
        vo_deinit_device();
        vo_deinit_sync_res();
        return ret;
    }
#endif

    vo_drv_board_init();

    ret = vo_register_module();
    if (ret != TD_SUCCESS) {
        vo_drv_board_deinit();
        vo_deinit_proc();
        vo_deinit_device();
        vo_deinit_sync_res();
        return ret;
    }

    ret = vo_init_atomic();
    if (ret != TD_SUCCESS) {
        vo_unregister_module();
        vo_drv_board_deinit();
        vo_deinit_proc();
        vo_deinit_device();
        vo_deinit_sync_res();
        return ret;
    }
#ifdef CONFIG_OT_VO_EXPORT_FUNCTION
    vo_init_export();
#endif
    return TD_SUCCESS;
}

td_s32 vo_mod_init(td_void)
{
    td_s32 ret;

    ret = vo_do_mod_init();
    if (ret != TD_SUCCESS) {
        OT_PRINT("load vo.ko ....FAILURE!\n");
        return TD_FAILURE;
    }

    OT_PRINT("load vo.ko ....OK!\n");
    return TD_SUCCESS;
}

td_void vo_mod_exit(td_void)
{
#ifdef CONFIG_OT_VO_EXPORT_FUNCTION
    vo_deinit_export();
#endif
    vo_deinit_atomic();
    vo_unregister_module();
    vo_drv_board_deinit();
    vo_deinit_proc();
    vo_deinit_device();

    /* 用户可能不sys_exit直接卸载模块 */
    vo_drv_set_all_crg_clk(TD_TRUE);
    vo_lpw_bus_reset(TD_TRUE);
    vo_drv_set_all_crg_clk(TD_FALSE);

    vo_deinit_sync_res();

    OT_PRINT("unload vo.ko ....OK!\n");
}

#endif
