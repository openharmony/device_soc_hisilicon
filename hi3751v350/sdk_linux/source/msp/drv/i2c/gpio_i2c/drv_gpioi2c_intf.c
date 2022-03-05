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

#include <linux/module.h>

#include "hi_log.h"
#include "hi_common.h"
#include "hi_drv_gpioi2c.h"
#include "hi_osal.h"

int g_i2c_mode;
module_param(g_i2c_mode, int, S_IRUGO);

int *get_var_i2c_mode(void)
{
    return &g_i2c_mode;
}

hi_s32 gpioi2c_drv_mod_init(hi_void)
{
    hi_s32 ret;

#ifndef HI_MCE_SUPPORT
    ret = hi_drv_gpioi2c_init();
#endif

#ifdef MODULE
    HI_PRINT("Load hi_gpioi2c.ko success.\t(%s)\n", VERSION_STRING);
#endif
    return HI_SUCCESS;
}

hi_void gpioi2c_drv_mod_exit(hi_void)
{
#ifndef HI_MCE_SUPPORT
    hi_drv_gpioi2c_de_init();
#endif

    return;
}

#ifdef MODULE
module_init(gpioi2c_drv_mod_init);
module_exit(gpioi2c_drv_mod_exit);
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HISILICON");
