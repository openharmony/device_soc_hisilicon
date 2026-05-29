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
#include "ot_osal.h"
#include "klad_cfg.h"
#include "ot_common_klad.h"
#include "ot_debug_klad.h"
#include "drv_klad_intf.h"
#include "drv_ioctl_klad.h"
#include "kapi_klad.h"
#include "drv_lib.h"
#include "drv_klad_sw_utils.h"

static osal_dev_t *g_klad_device = TD_NULL;
static td_s32 g_klad_irq_num;
static td_s32 g_rkp_irq_num;

static td_s32 ot_klad_open(td_void *private_data)
{
    ot_unused(private_data);
    return TD_SUCCESS;
}

static td_s32 ot_klad_release(td_void *private_data)
{
    ot_unused(private_data);
    kapi_klad_release();
    return TD_SUCCESS;
}

static long ot_klad_ioctl(unsigned int cmd, unsigned long arg, td_void *private_data)
{
    td_s32 ret;

    ot_unused(private_data);

    ot_klad_enter();

    ret = intf_klad_ioctl(cmd, (td_void *)(td_uintptr_t)arg);
    ot_klad_func_fail_return(intf_klad_ioctl, ret != TD_SUCCESS, ret);

    ot_klad_exit();

    return ret;
}

static struct osal_fileops g_klad_file_ops = {
    .open = ot_klad_open,
    .release = ot_klad_release,
    .unlocked_ioctl = ot_klad_ioctl,
    .poll = TD_NULL,
#ifdef CONFIG_COMPAT
    .compat_ioctl = ot_klad_ioctl,
#endif
};

static td_s32 ot_klad_get_irq(struct platform_device *pdev)
{
#ifdef KLAD_INT_SUPPORT
    g_klad_irq_num = platform_get_irq_byname(pdev, KLAD_INT_NAME);
    if (g_klad_irq_num <= 0) {
        dev_err(&pdev->dev, "cannot find klad IRQ number %d.\n", g_klad_irq_num);
        return OT_ERR_KLAD_FAILED_INIT;
    }
#endif

#ifdef RKP_INT_SUPPORT
    g_rkp_irq_num = platform_get_irq_byname(pdev, RKP_INT_NAME);
    if (g_rkp_irq_num <= 0) {
        dev_err(&pdev->dev, "cannot find rkp IRQ number %d.\n", g_rkp_irq_num);
        return OT_ERR_KLAD_FAILED_INIT;
    }
#endif

    return TD_SUCCESS;
}

td_u32 ot_klad_get_klad_irq(td_void)
{
    return (td_u32)g_klad_irq_num;
}

td_u32 ot_klad_get_rkp_irq(td_void)
{
    return (td_u32)g_rkp_irq_num;
}

static int ot_klad_probe(struct platform_device *pdev)
{
    td_s32 ret;

    ot_unused(pdev);

    ret = klad_get_cpu_secure_sta();
    ot_klad_func_fail_return(klad_get_cpu_secure_sta, ret != TD_SUCCESS, ret);

    ret = ot_klad_get_irq(pdev);
    ot_klad_func_fail_return(ot_klad_get_irq, ret != TD_SUCCESS, ret);

    ret = klad_entry();
    ot_klad_func_fail_return(klad_entry, ret != TD_SUCCESS, ret);

    g_klad_device = osal_createdev(UMAP_DEVNAME_KLAD_BASE);
    if (g_klad_device == TD_NULL) {
        ot_klad_error("create dev failed\n");
        klad_exit();
        return OT_ERR_KLAD_NULL_PTR;
    }
    g_klad_device->minor = UMAP_KLAD_MINOR_BASE;
    g_klad_device->fops = &g_klad_file_ops;

    if (osal_registerdevice(g_klad_device) < 0) {
        ot_klad_error("klad register failed\n");
        osal_destroydev(g_klad_device);
        g_klad_device = TD_NULL;
        klad_exit();
        return OT_ERR_KLAD_FAILED_INIT;
    }

    OT_PRINT("load ot_klad.ko ....OK!\n");

    return TD_SUCCESS;
}

static int ot_klad_remove(struct platform_device *pdev)
{
    ot_unused(pdev);

    osal_deregisterdevice(g_klad_device);
    osal_destroydev(g_klad_device);
    g_klad_device = TD_NULL;

    klad_exit();

    OT_PRINT("unload ot_klad.ko ....OK!\n");

    return TD_SUCCESS;
}

static const struct of_device_id g_ot_klad_match[] = {
    { .compatible = "vendor,klad" },
    { },
};
MODULE_DEVICE_TABLE(of, g_ot_klad_match);

static struct platform_driver g_ot_klad_driver = {
    .probe          = ot_klad_probe,
    .remove         = ot_klad_remove,
    .driver         = {
        .name   = "ot_klad",
        .of_match_table = g_ot_klad_match,
    },
};

module_platform_driver(g_ot_klad_driver);
MODULE_LICENSE("GPL");
