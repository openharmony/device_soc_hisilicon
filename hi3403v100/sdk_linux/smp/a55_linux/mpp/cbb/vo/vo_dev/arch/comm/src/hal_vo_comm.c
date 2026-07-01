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

#include "hal_vo_comm.h"
#include "drv_vo.h"
#include "hal_vo.h"
#include "ot_board.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")
#if vo_desc("hal pub")

static td_bool g_vo_init_by_dts = TD_TRUE;

td_void hal_vo_init(td_void)
{
    volatile reg_vdp_regs *reg = vo_hal_get_reg();
    if (reg == TD_NULL) {
        reg = (volatile reg_vdp_regs *)osal_ioremap(VOU_REGS_ADDR, (td_u32)VOU_REGS_SIZE);
        if (reg == TD_NULL) {
            OT_PRINT("ioremap vo reg failed\n");
            return;
        }
        vo_hal_set_reg(reg);
        g_vo_init_by_dts = TD_FALSE;
    }
}

/* no ddr reg */
td_void hal_write_reg(td_u32 *address, td_u32 value)
{
    *(volatile td_u32 *)address = value;
}

td_u32 hal_read_reg(const td_u32 *address)
{
    return *(volatile td_u32 *)(address);
}

#endif
#if vo_desc("hal check")

td_bool vo_hal_is_phy_dev(ot_vo_dev dev)
{
    if ((dev >= DEV_PHY_START) && (dev <= DEV_PHY_END)) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

td_bool vo_hal_is_video_layer(hal_disp_layer layer)
{
    if ((layer >= LAYER_VID_START) && (layer <= LAYER_VID_END)) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

#ifdef VO_HAL_CHECK_DEBUG
td_bool vo_hal_is_gfx_layer(hal_disp_layer layer)
{
    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        return TD_TRUE;
    }

    return TD_FALSE;
}
#endif

#endif

#ifdef VO_HAL_CHECK_DEBUG

td_bool vo_hal_is_video_gfx_layer(hal_disp_layer layer)
{
    if (vo_hal_is_video_layer(layer) || vo_hal_is_gfx_layer(layer)) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

#endif

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

td_void hal_vo_deinit(td_void)
{
    volatile reg_vdp_regs *reg = vo_hal_get_reg();
    if (reg != TD_NULL) {
        if (g_vo_init_by_dts == TD_FALSE) {
            osal_iounmap((void *)reg, (td_u32)VOU_REGS_SIZE);
            g_vo_init_by_dts = TD_TRUE;
        }
        vo_hal_set_reg(TD_NULL);
    }
}

#ifdef VO_HAL_CHECK_DEBUG

/* 支持多区域的视频层 */
td_bool vo_hal_is_mrg_video_layer(hal_disp_layer layer)
{
    if ((layer >= LAYER_MRG_START) && (layer <= LAYER_MRG_END)) {
        return TD_TRUE;
    }

    return TD_FALSE;
}

#endif
#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
