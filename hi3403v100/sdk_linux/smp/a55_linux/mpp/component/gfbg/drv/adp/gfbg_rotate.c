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

#include "gfbg_rotate.h"

static int_callback g_tde_rotate_call_back;
static vgs_callback g_vgs_rotate_call_back;

gfbg_rotate_ops g_gfbg_rotate_ops = {0};

td_void gfbg_rotation_register(td_void)
{
    g_gfbg_rotate_ops.gfbg_drv_rotate = gfbg_drv_rotate_process;
    g_gfbg_rotate_ops.is_support = TD_TRUE;
    return;
}

td_bool gfbg_get_rotation_support(td_void)
{
    return g_gfbg_rotate_ops.is_support;
}

gfbg_rotate gfbg_get_rotation(td_void)
{
    return g_gfbg_rotate_ops.gfbg_drv_rotate;
}

td_s32 gfbg_drv_set_tde_rotate_callback(int_callback tde_rot_callback)
{
    g_tde_rotate_call_back = tde_rot_callback;
    return TD_SUCCESS;
}

td_s32 gfbg_drv_set_vgs_rotate_callback(vgs_callback vgs_rot_callback)
{
    g_vgs_rotate_call_back = vgs_rot_callback;
    return TD_SUCCESS;
}

int_callback gfbg_drv_get_tde_rotate_callback(td_void)
{
    return g_tde_rotate_call_back;
}

vgs_callback gfbg_drv_get_vgs_rotate_callback(td_void)
{
    return g_vgs_rotate_call_back;
}
