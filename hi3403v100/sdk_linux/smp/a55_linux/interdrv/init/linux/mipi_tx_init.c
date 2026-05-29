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
#include "ot_mipi_tx_mod_init.h"
#include "ot_osal.h"

static int g_smooth = 0;

int mipi_tx_get_smooth(void)
{
    return g_smooth;
}

static int mipi_tx_probe(struct platform_device *pdev)
{
    struct resource *mem = NULL;
    int irq_num;
    void *regs = NULL;

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "mipi_tx");
    regs = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(regs)) {
        dev_err(&pdev->dev, "mipi_tx remap mem error.\n");
        return PTR_ERR(regs);
    }
    mipi_tx_set_regs(regs);

    irq_num = osal_platform_get_irq_byname(pdev, "mipi_tx");
    if (irq_num <= 0) {
        dev_err(&pdev->dev, "can not find mipi_tx IRQ\n");
    }
    mipi_tx_set_irq_num((unsigned int)irq_num);

    return mipi_tx_module_init(g_smooth);
}

static int mipi_tx_remove(struct platform_device *pdev)
{
    (void)pdev;

    mipi_tx_module_exit();
    mipi_tx_set_regs(NULL);

    return 0;
}

static const struct of_device_id mipi_tx_match[] = {
    { .compatible = "vendor,mipi_tx" },
    {},
};
MODULE_DEVICE_TABLE(of, mipi_tx_match);

static struct platform_driver mipi_tx_driver = {
    .probe  = mipi_tx_probe,
    .remove = mipi_tx_remove,
    .driver = {
        .name   = "mipi_tx",
        .of_match_table = mipi_tx_match,
    },
};

module_param(g_smooth, int, S_IRUGO);
module_platform_driver(mipi_tx_driver);

MODULE_DESCRIPTION("mipi_tx driver");
MODULE_VERSION("mipi_tx");
MODULE_LICENSE("GPL");
