/*
 * Copyright (C) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#define DRVNAME "gpu-regulator-driver"
#define REGNAME "Hisilicon GPU Regulator"
#define pr_fmt(fmt) DRVNAME ": " fmt

#include <asm/delay.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/of_regulator.h>

#define REGULATOR_ERROR_INFO() \
    printk("error: func = %s, line = %d\n", __FUNCTION__, __LINE__)

#define REGULATOR_IGNORE(x) (x) = (x)

#define REGULATOR_DEBUG 0

struct hisi_gpu_regulator {
    struct regulator_desc desc;
    struct regulator_dev *regdev;
    void __iomem *base;
    int min_uV;
    int max_uV;
};

static int hisi_gpu_regulator_get_voltage(struct regulator_dev *regdev)
{
    REGULATOR_IGNORE(regdev);

    return 0;
}

static int hisi_gpu_regulator_set_voltage(struct regulator_dev *regdev, int min_uV, int max_uV, unsigned *selector)
{
    REGULATOR_IGNORE(regdev);
    REGULATOR_IGNORE(min_uV);
    REGULATOR_IGNORE(max_uV);
    REGULATOR_IGNORE(selector);

    return 0;
}

static struct regulator_ops hisi_gpu_regulator_ops = {
    .get_voltage = hisi_gpu_regulator_get_voltage,
    .set_voltage = hisi_gpu_regulator_set_voltage,
};

static int hisi_regulator_probe(struct platform_device *pdev)
{
    struct hisi_gpu_regulator *regulator;
    struct regulator_init_data *init_data;
    struct regulator_config config = {0};
    struct resource *res;
    char *pname = NULL;

    regulator = devm_kzalloc(&pdev->dev, sizeof(*regulator), GFP_KERNEL);
    if (!regulator) {
        REGULATOR_ERROR_INFO();
        return -ENOMEM;
    }

    pname = REGNAME;

    res = platform_get_resource_byname(pdev, IORESOURCE_MEM, pname);
    if (!res) {
        REGULATOR_ERROR_INFO();
        return -ENODEV;
    }

    regulator->base = devm_ioremap_nocache(&pdev->dev, res->start, resource_size(res));
    if (!regulator->base) {
        devm_kfree(&pdev->dev, regulator);
        REGULATOR_ERROR_INFO();
        return -ENOMEM;
    }

    regulator->desc.name = dev_name(&pdev->dev);
    regulator->desc.type = REGULATOR_VOLTAGE;
    regulator->desc.owner = THIS_MODULE;
    regulator->desc.continuous_voltage_range = true;

    init_data = of_get_regulator_init_data(&pdev->dev, pdev->dev.of_node, &regulator->desc);
    if (!init_data) {
        devm_iounmap(&pdev->dev, regulator->base);
        devm_kfree(&pdev->dev, regulator);
        REGULATOR_ERROR_INFO();
        return -EINVAL;
    }

    init_data->constraints.apply_uV = 0;
    regulator->min_uV = init_data->constraints.min_uV;
    regulator->max_uV = init_data->constraints.max_uV;

    if (regulator->min_uV >= regulator->max_uV) {
        devm_iounmap(&pdev->dev, regulator->base);
        devm_kfree(&pdev->dev, regulator);
        REGULATOR_ERROR_INFO();
        return -EINVAL;
    }

    regulator->desc.ops = &hisi_gpu_regulator_ops;

    config.dev = &pdev->dev;
    config.init_data = init_data;
    config.driver_data = regulator;
    config.of_node = pdev->dev.of_node;

    regulator->regdev = devm_regulator_register(&pdev->dev, &regulator->desc, &config);
    if (IS_ERR(regulator->regdev)) {
        devm_iounmap(&pdev->dev, regulator->base);
        devm_kfree(&pdev->dev, regulator);
        REGULATOR_ERROR_INFO();
        return PTR_ERR(regulator->regdev);
    }

    platform_set_drvdata(pdev, regulator);

    if (!!REGULATOR_DEBUG) {
        printk("hisi_regulator_probe@\n");
    }

    return 0;
}

static const struct of_device_id hisi_regulator_of_match[] = {
    { .compatible = "hisilicon,hi3751v350-volt", },
    { }
};

static struct platform_driver hisi_regulator_driver = {
    .probe = hisi_regulator_probe,
    .driver = {
        .name = DRVNAME,
        .owner = THIS_MODULE,
        .of_match_table = hisi_regulator_of_match,
    },
};

module_platform_driver(hisi_regulator_driver);

MODULE_AUTHOR("Hisilicon GPU Team");
MODULE_DESCRIPTION("Hisilicon GPU Regulator");
MODULE_LICENSE("GPLv2");
MODULE_ALIAS("Hisilicon GPU Regulator");
