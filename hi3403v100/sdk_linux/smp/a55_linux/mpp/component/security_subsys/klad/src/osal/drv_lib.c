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

#include "drv_lib.h"
#ifdef KLAD_SWITCH_CPU
#include "sys_ext.h"
#include "ot_debug_klad.h"
#include "ot_common_klad.h"

static td_bool s_klad_secure_cpu = TD_FALSE;
#endif

td_s32 klad_get_cpu_secure_sta(td_void)
{
#ifdef KLAD_SWITCH_CPU
    if (!ckfn_sys_entry()) {
        ot_klad_error("no sys ko\n");
        return OT_ERR_KLAD_FAILED_INIT;
    }

    if (ckfn_sys_is_tee_enable_otp() && !call_sys_is_tee_enable_otp()) {
        s_klad_secure_cpu = TD_TRUE;
    } else {
        s_klad_secure_cpu = TD_FALSE;
    }
#endif
    return TD_SUCCESS;
}

/* KLAD_SECURE_CPU force to tee cpu
 * KLAD_SWITCH_CPU can switch to ree or tee cpu
 * else default ree cpu
 */
td_bool klad_is_secure_cpu(td_void)
{
#if defined(KLAD_SECURE_CPU)
    return TD_TRUE;
#elif defined(KLAD_SWITCH_CPU)
    return TD_TRUE;
#else
    return TD_FALSE;
#endif
}

