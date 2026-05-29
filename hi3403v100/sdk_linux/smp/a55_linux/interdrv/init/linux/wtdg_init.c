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
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "ot_type.h"
#include "ot_wtdg.h"

#define OSDRV_MODULE_VERSION_STRING "OT_WTDG @OT_MPP"

static int default_margin = OT_DOG_TIMER_MARGIN;
static int nodeamon = 0;

module_param(default_margin, int, 0);
MODULE_PARM_DESC(default_margin,
    "Watchdog default_margin in seconds. (0<default_margin<80, default=" __MODULE_STRING(OT_DOG_TIMER_MARGIN) ")");

module_param(nodeamon, int, 0);
MODULE_PARM_DESC(nodeamon,
    "By default, a kernel daemon feed watchdog when idle, set 'nodeamon=1' to disable this. (default=0)");

static int ot_wdg_probe(struct platform_device *pdev)
{
    struct resource *mem = NULL;
    volatile void **ot_wtdg_reg_base = NULL;

    wtdg_set_module_param(default_margin, nodeamon);
    ot_wtdg_reg_base = get_wtdg_reg_base();
    mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    *ot_wtdg_reg_base = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR((void*)*ot_wtdg_reg_base)) {
        return PTR_ERR((void*)*ot_wtdg_reg_base);
    }

    return watchdog_init();
}

static int ot_wdg_remove(struct platform_device *pdev)
{
    wtdg_unused(pdev);
    watchdog_exit();
    return 0;
}

static const struct of_device_id g_ot_wdg_match[] = {
    { .compatible = "vendor,ot_wdg" },
    { .compatible = "vendor,ot-wdg" },
    { },
};

static struct platform_driver g_ot_wdg_driver = {
    .probe  = ot_wdg_probe,
    .remove = ot_wdg_remove,
    .driver =  { .name = "ot_wdg",
                 .of_match_table = g_ot_wdg_match,
               },
};

module_platform_driver(g_ot_wdg_driver);
MODULE_LICENSE("GPL");

/*
 * Export symbol
 */
MODULE_AUTHOR("Digital Media Team ,Hisilicon crop ");
MODULE_DESCRIPTION("ot_wdg Driver");
MODULE_VERSION("OT_VERSION=" OSDRV_MODULE_VERSION_STRING);
