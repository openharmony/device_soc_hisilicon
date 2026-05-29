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

#include "hal_vo_reg.h"
#include "hal_vo_def.h"
#include "inner_vo.h"
#include "sys_cmp.h"
#include "hal_vo_comm.h"
#include "mkp_vo.h"
#include "ot_comm_irq.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

static volatile reg_vdp_regs *g_vo_reg = TD_NULL;

#if vo_desc("pub")

volatile reg_vdp_regs *vo_hal_get_reg(td_void)
{
    return g_vo_reg;
}

td_void vo_hal_set_reg(volatile reg_vdp_regs *reg)
{
    g_vo_reg = reg;
}

#endif

#if vo_desc("get abs addr")

td_ulong vou_get_abs_addr(hal_disp_layer layer, td_ulong reg)
{
    volatile td_ulong reg_abs_addr;

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
            reg_abs_addr = reg + (layer - HAL_DISP_LAYER_VHD0) * VHD_REGS_LEN;
            break;

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
            reg_abs_addr = reg + (layer - HAL_DISP_LAYER_GFX0) * GFX_REGS_LEN;
            break;

        case HAL_DISP_LAYER_GFX3:
            reg_abs_addr = reg + 3 * GFX_REGS_LEN; /* 3 lens */
            break;

        default:
            vo_err_trace("invalid layer %d!\n", layer);
            reg_abs_addr = reg;
            break;
    }

    return reg_abs_addr;
}

td_ulong vou_get_chn_abs_addr(ot_vo_dev dev, td_ulong reg)
{
    volatile td_ulong reg_abs_addr;

    switch (dev) {
        case VO_DEV_DHD0:
        case VO_DEV_DHD1:
            reg_abs_addr = reg + (dev - VO_DEV_DHD0) * DHD_REGS_LEN;
            break;

        default:
            vo_err_trace("invalid dev %d!\n", dev);
            reg_abs_addr = reg;
            break;
    }

    return reg_abs_addr;
}

td_ulong vou_get_vid_abs_addr(hal_disp_layer layer, td_ulong reg)
{
    volatile td_ulong reg_abs_addr;

    switch (layer) {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
            reg_abs_addr = reg + (layer - HAL_DISP_LAYER_VHD0) * VID_REGS_LEN;
            break;

        default:
            vo_err_trace("invalid layer %d!\n", layer);
            reg_abs_addr = reg;
            break;
    }

    return reg_abs_addr;
}

td_ulong vou_get_gfx_abs_addr(hal_disp_layer layer, td_ulong reg)
{
    volatile td_ulong reg_abs_addr;

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
            reg_abs_addr = reg + (layer - HAL_DISP_LAYER_GFX0) * GRF_REGS_LEN;
            break;

        case HAL_DISP_LAYER_GFX3:
            reg_abs_addr = reg + 3 * GRF_REGS_LEN; /* 3 lens */
            break;

        default:
            vo_err_trace("invalid layer %d!\n", layer);
            reg_abs_addr = reg;
            break;
    }

    return reg_abs_addr;
}
#endif

#endif /* #if vo_desc("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
