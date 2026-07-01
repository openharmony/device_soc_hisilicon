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
#include "ot_osal.h"
#include "osal.h"

static int __init osal_init(void)
{
    if (mem_check_module_param() == -1) {
        return -1;
    }

    osal_device_init();
    osal_proc_init();
    if (ot_media_init() != 0) {
        goto failed;
    }
    if (media_mem_init() != 0) {
        ot_media_exit();
        goto failed;
    }
    osal_trace("load ot_osal %s ....OK!\n", OT_OSAL_VERSION);
    return 0;

failed:
    osal_proc_exit();
    osal_trace("load ot_osal %s failed!\n", OT_OSAL_VERSION);
    return -1;
}

static void __exit osal_exit(void)
{
    media_mem_exit();
    ot_media_exit();
    osal_proc_exit();
    osal_device_exit();
    osal_trace("unload ot_osal %s ....OK!\n", OT_OSAL_VERSION);
}

module_init(osal_init);
module_exit(osal_exit);

MODULE_AUTHOR("Hisilicon");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
