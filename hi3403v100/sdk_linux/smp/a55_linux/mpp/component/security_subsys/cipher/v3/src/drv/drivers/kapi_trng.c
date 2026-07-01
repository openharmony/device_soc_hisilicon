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

#include "drv_osal_lib.h"
#include "cryp_trng.h"

td_s32 kapi_trng_init(void)
{
    td_s32 ret;

    func_enter();

    ret = cryp_trng_init();
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_trng_init, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 kapi_trng_deinit(void)
{
    td_s32 ret;

    func_enter();

    ret = cryp_trng_deinit();
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_trng_deinit, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 kapi_trng_get_random(td_u32 *randnum, td_u32 timeout)
{
    td_s32 ret;

    func_enter();

    chk_ptr_err_return(randnum);

    ret = cryp_trng_get_random(randnum, timeout);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}
