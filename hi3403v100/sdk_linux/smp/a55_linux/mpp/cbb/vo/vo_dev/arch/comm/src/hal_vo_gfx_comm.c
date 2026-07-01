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

#include "hal_vo_gfx_comm.h"
#include "hal_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")
#if vo_desc("gfx")
td_void hal_gfx_set_pixel_alpha_range(hal_disp_layer layer, td_u32 alpha_range)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_gfx_out_ctrl gfx_out_ctrl;
    volatile td_ulong addr_reg;

    vo_hal_check_gfx_layer_return(layer);
    addr_reg = vou_get_gfx_abs_addr(layer, (td_uintptr_t)&(vo_reg->gfx_out_ctrl.u32));
    gfx_out_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    gfx_out_ctrl.bits.palpha_range = alpha_range;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, gfx_out_ctrl.u32);
}
#endif
#endif /* #if vo_desc("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
