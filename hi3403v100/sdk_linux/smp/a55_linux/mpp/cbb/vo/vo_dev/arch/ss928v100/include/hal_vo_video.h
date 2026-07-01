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

#ifndef HAL_VO_VIDEO_H
#define HAL_VO_VIDEO_H

#include "hal_vo_video_comm.h"
#include "ot_inner_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")
td_void hal_cbm_set_cbm_attr(hal_disp_layer layer, ot_vo_dev dev);
td_void hal_cbm_set_cbm_bkg(hal_cbmmix mixer, const hal_disp_bkcolor *bkg);
td_void hal_cbm_set_cbm_mixer_prio(ot_vo_layer layer, td_u8 prio, td_u8 mixer_id);
#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")
td_void hal_write_mrg_reg(td_u32 *address, td_u32 value);
td_u32 hal_read_mrg_reg(const td_u32 *address);
td_u32 hal_layer_get_layer_max_area_num(hal_disp_layer layer);
td_void hal_para_set_para_addr_vhd_chn08(td_phys_addr_t para_addr_vhd_chn08);
#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of HAL_VO_VIDEO_H */
