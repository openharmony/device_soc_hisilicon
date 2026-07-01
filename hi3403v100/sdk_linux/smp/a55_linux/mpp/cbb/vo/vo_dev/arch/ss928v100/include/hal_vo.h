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

#ifndef HAL_VO_H
#define HAL_VO_H

#include "hal_vo_reg.h"
#include "hal_vo_def.h"
#include "inner_vo.h"
#include "sys_cmp.h"
#include "hal_vo_comm.h"
#include "hal_vo_dev.h"
#include "hal_vo_video.h"
#include "hal_vo_layer_comm.h"
#include "hal_vo_gfx_comm.h"
#include "mkp_vo.h"
#include "ot_comm_irq.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

#if vo_desc("hal pub")
volatile reg_vdp_regs *vo_hal_get_reg(td_void);
td_void vo_hal_set_reg(volatile reg_vdp_regs *reg);
#endif

#if vo_desc("get abs addr")
td_ulong vou_get_abs_addr(hal_disp_layer layer, td_ulong reg);
td_ulong vou_get_chn_abs_addr(ot_vo_dev dev, td_ulong reg);
td_ulong vou_get_vid_abs_addr(hal_disp_layer layer, td_ulong reg);
td_ulong vou_get_gfx_abs_addr(hal_disp_layer layer, td_ulong reg);
#endif

#endif /* #if vo_desc("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of HAL_VO_H */
