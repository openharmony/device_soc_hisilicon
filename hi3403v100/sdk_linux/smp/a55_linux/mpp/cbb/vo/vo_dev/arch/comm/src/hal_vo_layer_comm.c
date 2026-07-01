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

#include "hal_vo_layer_comm.h"
#include "drv_vo.h"
#include "hal_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")
#if vo_desc("layer")
td_void hal_video_set_layer_alpha(hal_disp_layer layer, td_u32 alpha)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_v0_alpha v0_alpha;
    volatile reg_g0_alpha g0_alpha;
    volatile td_ulong addr_reg;

    vo_hal_check_video_gfx_layer_return(layer);

    if (vo_hal_is_video_layer(layer)) {
        addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->v0_alpha.u32));
        v0_alpha.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
        v0_alpha.bits.vbk_alpha = alpha;
        hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_alpha.u32);
        return;
    }

    addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->g0_alpha.u32));
    g0_alpha.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    g0_alpha.bits.vbk_alpha = alpha;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, g0_alpha.u32);
}

td_void hal_layer_csc_set_enable_ck_gt_en(hal_disp_layer layer, td_bool csc_en, td_bool ck_gt_en)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_v0_ot_pp_csc_ctrl v0_ot_pp_csc_ctrl;
    volatile td_ulong addr_reg;

    vo_hal_check_video_gfx_layer_return(layer);

    if (vo_hal_is_video_layer(layer)) {
        addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->v0_ot_pp_csc_ctrl.u32));
    } else {
        addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->g0_ot_pp_csc_ctrl.u32));
    }

    v0_ot_pp_csc_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    v0_ot_pp_csc_ctrl.bits.ot_pp_csc_en = csc_en;
    v0_ot_pp_csc_ctrl.bits.ot_pp_csc_ck_gt_en = ck_gt_en;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_ot_pp_csc_ctrl.u32);
}

td_void hal_layer_set_layer_global_alpha(hal_disp_layer layer, td_u8 alpha0)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_v0_ctrl v0_ctrl;
    volatile reg_g0_ctrl g0_ctrl;
    volatile td_ulong addr_reg;

    vo_hal_check_video_gfx_layer_return(layer);

    if (vo_hal_is_video_layer(layer)) {
        addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->v0_ctrl.u32));
        v0_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
        v0_ctrl.bits.galpha = alpha0;
        hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, v0_ctrl.u32);
        return;
    }

    addr_reg = vou_get_abs_addr(layer, (td_uintptr_t)&(vo_reg->g0_ctrl.u32));
    g0_ctrl.u32 = hal_read_reg((td_u32 *)(td_uintptr_t)addr_reg);
    g0_ctrl.bits.galpha = alpha0;
    hal_write_reg((td_u32 *)(td_uintptr_t)addr_reg, g0_ctrl.u32);
}
#endif
#endif /* #if vo_desc("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
