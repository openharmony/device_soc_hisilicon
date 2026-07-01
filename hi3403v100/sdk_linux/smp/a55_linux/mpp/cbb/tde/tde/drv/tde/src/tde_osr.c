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

#include "tde_osr.h"
#include "tde_osilist.h"
#include "ot_type.h"
#include "tde_ioctl.h"
#include "tde_handle.h"
#include "tde_osilist.h"
#include "ot_type.h"
#include "tde_hal.h"

#ifdef CONFIG_USE_SYS_CONFIG
#include "sys_ext.h"
#endif
#define mkstr(exp)        #exp
#define mkmarcotostr(exp) mkstr(exp)

td_s32 tde_drv_mod_init(td_void)
{
#ifndef OT_MCE_SUPPORT
    td_s32 ret = TD_SUCCESS;
#ifdef CONFIG_USE_SYS_CONFIG
    if (ckfn_sys_entry() == TD_FALSE) {
        tde_error("no sys ko!\n");
        return TD_FAILURE;
    }
#endif
    ret = tde_init_module_k();
#endif

    return ret;
}

td_void tde_drv_mod_exit(td_void)
{
#ifndef OT_MCE_SUPPORT
    tde_cleanup_module_k();
#endif
}

