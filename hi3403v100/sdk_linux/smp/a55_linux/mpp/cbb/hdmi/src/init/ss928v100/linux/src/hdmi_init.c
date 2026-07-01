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
#include "ot_hdmi_mod_init.h"
#include <linux/module.h>
#include <linux/of_platform.h>
#include "ot_common.h"

#define HDMI_DEV_NAME_LENGTH 16

static int hdmi_probe(struct platform_device *pdev)
{
    td_s32 ret;
    struct resource *mem = NULL;
    td_char *hdmi_reg = NULL;
    td_char *hdmi_phy = NULL;
    td_char hdmi_dev_name[HDMI_DEV_NAME_LENGTH] = "hdmi0";
    td_char hdmi_phy_name[HDMI_DEV_NAME_LENGTH] = "phy";

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, hdmi_dev_name);
    hdmi_reg = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(hdmi_reg)) {
        return PTR_ERR(hdmi_reg);
    }
    ret = hdmi_set_reg(0, hdmi_reg);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, hdmi_phy_name);
    hdmi_phy = devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(hdmi_phy)) {
        return PTR_ERR(hdmi_phy);
    }
    ret = hdmi_set_phy(0, hdmi_phy);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return hdmi_drv_mod_init();
}

static int hdmi_remove(struct platform_device *pdev)
{
    ot_unused(pdev);
    hdmi_drv_mod_exit();
    hdmi_set_reg(0, NULL);

    return TD_SUCCESS;
}

static const struct of_device_id g_hdmi_match[] = {
    { .compatible = "vendor,hdmi" },
    {}
};

MODULE_DEVICE_TABLE(of, g_hdmi_match);

static struct platform_driver g_hdmi_driver = {
    .probe  = hdmi_probe,
    .remove = hdmi_remove,
    .driver = {
        .name = "hdmi",
        .of_match_table = g_hdmi_match,
    },
};

osal_module_platform_driver(g_hdmi_driver);

MODULE_LICENSE("Proprietary");

