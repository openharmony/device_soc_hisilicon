/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ot_tde_mod_init.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include "ot_type.h"
#include "ot_common.h"
#include "ot_osal.h"

td_u32 g_tde_tmp_buf = 1658880; /* 1658880 buffer size */
bool g_is_resize_filter = true;
td_u32 g_max_node_num = 200; /* 200 nums */
td_u32 g_rgb_truncation_mode = 1; /* 1 for rgb truncation mode */

module_param(g_tde_tmp_buf, uint, S_IRUGO);
module_param(g_is_resize_filter, bool, S_IRUGO);
module_param(g_max_node_num, uint, S_IRUGO);
module_param(g_rgb_truncation_mode, uint, S_IRUGO);

static int ot_tde_probe(struct platform_device *pdev)
{
    struct resource *mem = TD_NULL;
    td_s32 tde_irq;
    td_u32 *base_vir_addr = TD_NULL;

    mem = osal_platform_get_resource_byname(pdev, IORESOURCE_MEM, "tde");
    base_vir_addr = devm_ioremap_resource(&pdev->dev, mem);
    tde_hal_set_base_vir_addr(base_vir_addr);
    if (IS_ERR(base_vir_addr)) {
        return PTR_ERR(base_vir_addr);
    }

    tde_irq = osal_platform_get_irq_byname(pdev, "tde_osr_isr");
    if (tde_irq <= 0) {
        dev_err(&pdev->dev, "cannot find tde IRQ\n");
        return -1;
    }
    set_tde_irq((td_u32)tde_irq);
    if (g_max_node_num > 0) {
        tde_set_max_node_num(g_max_node_num);
    }
    tde_set_resize_filter(g_is_resize_filter);
    if ((g_rgb_truncation_mode == 0) || (g_rgb_truncation_mode == 1)) { /* 0 1 for rgb truncation mode */
        tde_init_set_rgb_truncation_mode(g_rgb_truncation_mode);
    }
    if (g_tde_tmp_buf > 0) {
        tde_set_tde_tmp_buffer(g_tde_tmp_buf);
    }
    if (tde_drv_mod_init() != TD_SUCCESS) {
        osal_printk("load tde.ko ....FAILED!\n");
        return -1;
    }
    osal_printk("load tde.ko ....OK!\n");
    return 0;
}

static int ot_tde_remove(struct platform_device *pdev)
{
    ot_unused(pdev);
    tde_drv_mod_exit();
    osal_printk("unload tde.ko ....OK!\n");
    return 0;
}

static const struct of_device_id g_ot_tde_match[] = {
    { .compatible = "vendor,tde" },
    {},
};
MODULE_DEVICE_TABLE(of, g_ot_tde_match);

static struct platform_driver g_ot_tde_driver = {
    .probe = ot_tde_probe,
    .remove = ot_tde_remove,
    .driver = {
        .name = "ot_tde",
        .of_match_table = g_ot_tde_match,
    },
};

osal_module_platform_driver(g_ot_tde_driver);

MODULE_LICENSE("Proprietary");

