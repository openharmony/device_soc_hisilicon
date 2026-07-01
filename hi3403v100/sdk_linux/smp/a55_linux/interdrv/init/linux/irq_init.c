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
#include "ot_irq.h"
#include "ot_osal.h"

#define OSDRV_MODULE_VERSION_STRING "HISI_irq @HiMPP"

static int ot_irq_probe(struct platform_device *pdev)
{
    printk("enter ot_irq_probe \n");
    return ot_irq_init(pdev);
}

static int ot_irq_remove(struct platform_device *pdev)
{
    ot_irq_exit();
    return 0;
}

static const struct of_device_id g_ot_irq_match[] = {
    { .compatible = "vendor,ot_irq" },
    {},
};

static struct platform_driver g_ot_irq_driver = {
    .probe  = ot_irq_probe,
    .remove = ot_irq_remove,
    .driver =  {
        .name = OT_IRQ_DEVICE_NAME, /* ot_irq */
        .of_match_table = g_ot_irq_match,
    },
};

osal_module_platform_driver(g_ot_irq_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hisilicon");
MODULE_DESCRIPTION("Hisilicon Infrared remoter(HIIR11) Device Driver");
MODULE_VERSION("OT_VERSION=" OSDRV_MODULE_VERSION_STRING);
