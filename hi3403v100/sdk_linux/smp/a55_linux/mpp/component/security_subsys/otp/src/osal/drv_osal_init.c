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

#include <linux/module.h>
#include <linux/of_platform.h>

#include "drv_ioctl_otp.h"
#include "drv_otp_intf.h"
#include "ot_debug_otp.h"
#include "ot_osal.h"
#include "drv_lib.h"

static osal_dev_t *g_otp_device = TD_NULL;

static td_s32 ot_otp_open(td_void *private_data)
{
    ot_unused(private_data);
    return TD_SUCCESS;
}

static td_s32 ot_otp_release(td_void *private_data)
{
    ot_unused(private_data);
    return TD_SUCCESS;
}

static long ot_otp_ioctl(unsigned int cmd, unsigned long arg, td_void *private_data)
{
    td_s32 ret;

    ot_otp_enter();

    ot_unused(private_data);

    ret = intf_otp_ioctl(cmd, (td_void *)(td_uintptr_t)arg);
    ot_otp_func_fail_return(intf_otp_ioctl, ret != TD_SUCCESS, ret);

    ot_otp_exit();

    return ret;
}

static struct osal_fileops g_otp_file_ops = {
    .open             = ot_otp_open,
    .release          = ot_otp_release,
    .unlocked_ioctl   = ot_otp_ioctl,
    .poll = TD_NULL,
#ifdef CONFIG_COMPAT
    .compat_ioctl     = ot_otp_ioctl,
#endif
};

static int ot_otp_probe(struct platform_device *pdev)
{
    td_s32 ret;

    ot_unused(pdev);

    ret = otp_get_cpu_secure_sta();
    ot_otp_func_fail_return(otp_get_cpu_secure_sta, ret != TD_SUCCESS, ret);

    ret = otp_entry();
    ot_otp_func_fail_return(otp_entry, ret != TD_SUCCESS, ret);

    g_otp_device = osal_createdev(UMAP_DEVNAME_OTP_BASE);
    if (g_otp_device == TD_NULL) {
        ot_otp_error("create dev failed\n");
        otp_exit();
        return OT_ERR_OTP_NULL_PTR;
    }
    g_otp_device->minor = UMAP_OTP_MINOR_BASE;
    g_otp_device->fops = &g_otp_file_ops;

    if (osal_registerdevice(g_otp_device) < 0) {
        ot_otp_error("otp register failed\n");
        osal_destroydev(g_otp_device);
        g_otp_device = TD_NULL;
        otp_exit();
        return OT_ERR_OTP_FAILED_INIT;
    }

    OT_PRINT("load ot_otp.ko ....OK!\n");
    return TD_SUCCESS;
}

static int ot_otp_remove(struct platform_device *pdev)
{
    ot_unused(pdev);

    osal_deregisterdevice(g_otp_device);
    osal_destroydev(g_otp_device);
    g_otp_device = TD_NULL;

    otp_exit();

    OT_PRINT("unload ot_otp.ko ....OK!\n");
    return TD_SUCCESS;
}

static const struct of_device_id g_ot_otp_match[] = {
    { .compatible = "vendor,otp" },
    { },
};
MODULE_DEVICE_TABLE(of, g_ot_otp_match);

static struct platform_driver g_ot_otp_driver = {
    .probe          = ot_otp_probe,
    .remove         = ot_otp_remove,
    .driver         = {
        .name   = "ot_otp",
        .of_match_table = g_ot_otp_match,
    },
};

module_platform_driver(g_ot_otp_driver);
MODULE_LICENSE("GPL");

