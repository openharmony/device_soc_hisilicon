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
#include <linux/of_platform.h>

#include "ot_aiv_mod_init.h"

td_s32 aiv_init(td_void)
{
    return aiv_drv_mod_init();
}

td_void aiv_exit(td_void)
{
    aiv_drv_mod_exit();
}

module_init(aiv_init);
module_exit(aiv_exit);

MODULE_DESCRIPTION("aiv driver");
MODULE_LICENSE("Proprietary");
