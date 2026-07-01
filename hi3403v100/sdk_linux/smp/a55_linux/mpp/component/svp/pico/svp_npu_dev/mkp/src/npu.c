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

#include "npu.h"
#include "valg_plat.h"
#include "sys_ext.h"
#include "osal_mmz.h"
#include "ot_svp_npu_mod_init.h"
#include "mm_ext.h"
#include "dev_ext.h"

#define SVP_NPU_SMMU_BASE_ADDR       0x15400000
#define SVP_NPU_SMMU_BASE_SIZE       0x50000
#define SVP_NPU_MAGIC                20221125

/* CNcomment: 复位撤销复位等待时间，单位:US */
#define SVP_NPU_RESET_WAIT_TIME_US  0x400

/* SVP_NPU state */
static svp_npu_status g_svp_npu_mod_status = SVP_NPU_STATUS_FREE;
/* dev ptr */
static osal_dev_t *g_svp_npu_device = TD_NULL;

td_s32 svm_smmu_device_reset(const td_char *device_name);
td_void svm_smmu_device_reset_lock(td_void);
td_void svm_smmu_device_reset_unlock(td_void);
td_s32 svm_smmu_power_on(void *base, unsigned int tcu_offset, unsigned int tbu_offset);

static td_void drv_svp_npu_enable_reset(td_u32 device_id, td_bool reset_flag)
{
    ot_mpp_chn mpp_chn;
    td_u32 func_id[] = {SYS_SVP_NPU0_RESET_SEL};

    mpp_chn.mod_id = OT_ID_SVP_NPU;
    mpp_chn.dev_id = 0;
    mpp_chn.chn_id = 0;
    call_sys_drv_ioctrl(&mpp_chn, func_id[device_id], &reset_flag);
}

static td_s32 svp_npu_smmu_reset_ctrl(svm_smmu_reset_arg *arg)
{
    td_s32 ret = SVP_NPU_SUCCESS;
    svm_smmu_device_reset_lock();
    drv_svp_npu_enable_reset(arg->device_id, TD_TRUE);

    osal_udelay(SVP_NPU_RESET_WAIT_TIME_US);

    drv_svp_npu_enable_reset(arg->device_id, TD_FALSE);
    (td_void)svm_smmu_power_on((void *)osal_ioremap(SVP_NPU_SMMU_BASE_ADDR, SVP_NPU_SMMU_BASE_SIZE),
        arg->tcu_offset, arg->tbu_offset);
    (td_void)svm_smmu_device_reset("svm_pqp");
    svm_smmu_device_reset_unlock();
    return ret;
}

static td_s32 svp_npu_dev_ioctl_normal(unsigned int cmd, unsigned long arg, svp_npu_process_info *process_info)
{
    td_s32 ret;
    switch (cmd) {
        case SVP_NPU_SMMU_RESET_CTRL:
            ret = svp_npu_smmu_reset_ctrl(svp_npu_convert_addr_to_ptr(svm_smmu_reset_arg, arg));
            break;
        default:
            ret = SVP_NPU_ERROR_INVALID_PARAM;
            svp_npu_err_trace("Error, input npu cmd(%u) is illegal\n", cmd);
            break;
    }
    return ret;
}

static long svp_npu_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    td_s32 ret = SVP_NPU_ERROR_UNINITIALIZE;
    ret = svp_npu_dev_ioctl_normal(cmd, arg, *((svp_npu_process_info **)(private_data)));
    return ret;
}

td_s32 svp_npu_init(td_void *arg)
{
    td_s32 ret = 0;
    /* If NPU module has already been started, return SVP_NPU_SUCCESS */
    if (g_svp_npu_mod_status == SVP_NPU_STATUS_BUSY) {
        return SVP_NPU_SUCCESS;
    }

    /* If NPU module is being stopped, return OT_ERR_ANR_BUSY */
    if (g_svp_npu_mod_status == SVP_NPU_STATUS_STOPPING) {
        svp_npu_err_trace("Error, NPU is stopping!\n");
        return TD_FAILURE;
    }

    /* Check whether sys module has been registered */
    if ((!ckfn_sys_entry()) || (!ckfn_sys_get_mmz_name())) {
        svp_npu_err_trace("Error, sys is not ready!\n");
        return TD_FAILURE;
    }

    if (ret != SVP_NPU_SUCCESS) {
        svp_npu_err_trace("Error, svp_npu_init_param failed!\n");
        return TD_FAILURE;
    }
    g_svp_npu_mod_status = SVP_NPU_STATUS_BUSY;
    return ret;
}

td_void svp_npu_deinit(td_void)
{
    if (g_svp_npu_mod_status == SVP_NPU_STATUS_FREE) {
        return;
    }
    g_svp_npu_mod_status = SVP_NPU_STATUS_FREE;
}


static td_s32 svp_npu_open(void *private_data)
{
    return SVP_NPU_SUCCESS;
}

static td_s32 svp_npu_close(void *private_data)
{
    return SVP_NPU_SUCCESS;
}

static td_u32 svp_npu_poll(osal_poll_t *osal_poll, void *private_data)
{
    return 0;
}

static struct osal_fileops g_svp_npu_fops = {
    .open = svp_npu_open,
    .unlocked_ioctl = svp_npu_ioctl,
    .release = svp_npu_close,
    .poll = svp_npu_poll,
};

static td_void svp_npu_notify(mod_notice_id notice)
{
    return;
}

static td_void svp_npu_query_state(mod_state *state)
{
    *state = MOD_STATE_FREE;
    return;
}

static td_u32 svp_npu_get_ver_magic(td_void)
{
    return SVP_NPU_MAGIC;
}

static umap_module g_svp_npu_module = {
    .mod_id = OT_ID_SVP_NPU,
    .mod_name = "svp_npu",

    .pfn_init = svp_npu_init,
    .pfn_exit = svp_npu_deinit,
    .pfn_query_state = svp_npu_query_state,
    .pfn_notify = svp_npu_notify,
    .pfn_ver_checker = svp_npu_get_ver_magic,

    .export_funcs = TD_NULL,
    .data = TD_NULL,
};

static td_s32 svp_npu_mod_register(td_void)
{
    td_char buf[UMAP_NAME_MAXLEN];
    td_s32 ret;
    /* create device */
    ret = snprintf_s(buf, UMAP_NAME_MAXLEN, UMAP_NAME_MAXLEN - 1, "%s", UMAP_DEVNAME_SVP_NPU_BASE);
    if (ret < 0) {
        svp_npu_err_trace("Error, snprintf_s npu dev name failed!\n");
        return SVP_NPU_ERROR_INVALID_PARAM;
    }
    g_svp_npu_device = osal_createdev(buf);
    if (g_svp_npu_device == TD_NULL) {
        svp_npu_err_trace("Error, osal create dev failed!\n");
        return SVP_NPU_ERROR_BAD_ALLOC;
    }
    g_svp_npu_device->fops = &g_svp_npu_fops;
    g_svp_npu_device->minor = UMAP_SVP_NPU_MINOR_BASE;

    /* register device */
    ret = SVP_NPU_ERROR_BAD_ALLOC;
    if (osal_registerdevice(g_svp_npu_device) < 0) {
        svp_npu_err_trace("Error, osal register dev failed!\n");
        goto fail_1;
    }
    /* register module */
    if (cmpi_register_module(&g_svp_npu_module)) {
        svp_npu_err_trace("Error, osal register module failed!\n");
        goto fail_2;
    }
    return SVP_NPU_SUCCESS;

fail_2:
    osal_deregisterdevice(g_svp_npu_device);
fail_1:
    osal_destroydev(g_svp_npu_device);
    g_svp_npu_device = TD_NULL;
    return ret;
}

#ifdef CONFIG_LINUX_OS
td_s32 svp_npu_set_init_reg(td_u32 *reg[], td_u32 num)
{
    td_s32 i;

    if (num < SVP_NPU_DEVICE_BUTT) {
        svp_npu_err_trace("Error, num(%u) can't be less than %d!\n", num, SVP_NPU_DEVICE_BUTT);
        return SVP_NPU_ERROR_INVALID_PARAM;
    }
    if (reg == TD_NULL) {
        svp_npu_err_trace("Error, reg is NULL\n");
        return SVP_NPU_ERROR_INVALID_PARAM;
    }
    for (i = 0; i < SVP_NPU_DEVICE_BUTT; i++) {
        if (reg[i] == TD_NULL) {
            svp_npu_err_trace("Error, reg[%u] is TD_NULL!\n", i);
            return SVP_NPU_ERROR_INVALID_PARAM;
        }
    }
    return SVP_NPU_SUCCESS;
}
#endif

td_s32 svp_npu_mod_init(td_void)
{
    td_s32 ret;
    if (!ckfn_sys_entry()) {
        OT_PRINT("no sys ko,load svp_npu.ko ....fail!\n");
        return TD_FAILURE;
    }

    /* register mode */
    ret = svp_npu_mod_register();
    if (ret != SVP_NPU_SUCCESS) {
        osal_printk("Error, svp_npu_mod_register failed!\n");
        return ret;
    }
    osal_printk("load svp_npu.ko ....OK!\n");
    return SVP_NPU_SUCCESS;
}

td_void svp_npu_mod_exit(td_void)
{
    cmpi_unregister_module(OT_ID_SVP_NPU);

    if (g_svp_npu_device != TD_NULL) {
        osal_deregisterdevice(g_svp_npu_device);
        osal_destroydev(g_svp_npu_device);
        g_svp_npu_device = TD_NULL;
    }

    osal_printk("unload svp_npu.ko ....OK!\n");
}

MODULE_LICENSE("Proprietary");
