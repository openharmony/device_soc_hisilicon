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
#include <asm/io.h>
#include "ot_common.h"
#include "ot_osal.h"
#include "mod_ext.h"
#include "securec.h"
#include "ot_svp_npu_mod_init.h"

#define SVP_NPU_DEV_NAME_LENGTH 8
#define SVP_NPU_DEFAULT_TASK_NODE_NUM 512
#define SVP_NPU_DEV_NUM 1

static td_u8 g_power_save_en = 1;
static td_u16 g_max_task_node_num = SVP_NPU_DEFAULT_TASK_NODE_NUM;

module_param_named(svp_npu_save_power, g_power_save_en, byte, S_IRUGO);
module_param_named(svp_npu_max_task_node_num, g_max_task_node_num, ushort, S_IRUGO);

static int svp_npu_probe(struct platform_device *pf_dev)
{
    td_s32 ret;
    td_char svp_npu_dev_name[SVP_NPU_DEV_NAME_LENGTH] = { '\0' };
    struct resource *mem = TD_NULL;
    td_void *reg = TD_NULL;
    ret = snprintf_s(svp_npu_dev_name, SVP_NPU_DEV_NAME_LENGTH, SVP_NPU_DEV_NAME_LENGTH - 1, "svp_npu");
    if ((ret < 0) || (ret > (SVP_NPU_DEV_NAME_LENGTH - 1))) {
        printk("snprintf_s npu name failed\n");
        return TD_FAILURE;
    }
    mem = osal_platform_get_resource_byname(pf_dev, IORESOURCE_MEM, "svp_npu");
    reg = devm_ioremap_resource(&pf_dev->dev, mem);
    if (IS_ERR(reg)) {
        printk("devm_ioremap_resource failed\n");
        return PTR_ERR(reg);
    }
    return svp_npu_mod_init();
}

static int svp_npu_remove(struct platform_device *pf_dev)
{
    ot_unused(pf_dev);
    svp_npu_mod_exit();
    return 0;
}

static const struct of_device_id g_svp_npu_match[] = {
    { .compatible = "vendor,svp_npu" },
    {},
};
MODULE_DEVICE_TABLE(of, g_svp_npu_match);

static struct platform_driver g_svp_npu_driver = {
    .probe          = svp_npu_probe,
    .remove         = svp_npu_remove,
    .driver         = {
        .name   = "svp_npu",
        .of_match_table = g_svp_npu_match,
    },
};

static int __init svp_npu_driver_init(void)
{
    if (cmpi_get_module_name(OT_ID_PQP) != TD_NULL) {
        printk("Attention: svp_npu cannot run because the pqp.ko has been loaded!\n");
        return TD_FAILURE;
    }
    return osal_platform_driver_register(&g_svp_npu_driver);
}
module_init(svp_npu_driver_init);

static void __exit svp_npu_driver_exit(void)
{
    return osal_platform_driver_unregister(&g_svp_npu_driver);
}
module_exit(svp_npu_driver_exit);

MODULE_LICENSE("Proprietary");
