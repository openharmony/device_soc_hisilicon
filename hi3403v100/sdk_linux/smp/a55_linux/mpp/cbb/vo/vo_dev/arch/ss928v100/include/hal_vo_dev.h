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

#ifndef HAL_VO_DEV_H
#define HAL_VO_DEV_H

#include "hal_vo_dev_comm.h"
#include "inner_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

td_void hal_sys_set_outstanding(td_void);

#if vo_desc("dev intf")
#define VO_CVBS_DATE_PAL 0x628412dc
#define VO_CVBS_DATE_NTSC 0x108412dc
#define VO_CVBS_DATE_GAIN 0x8080958a
#define VO_CVBS_DATE_PAL_GAIN VO_CVBS_DATE_GAIN
#define VO_CVBS_DATE_NTSC_GAIN VO_CVBS_DATE_GAIN
td_void vo_hal_intf_set_cvbs_dac_cfg(td_void);
td_void vo_hal_intf_set_date_cvbs_gain(td_u32 data);
td_void vo_hal_intf_set_date_cvbs_burst_start(td_void);
td_void hal_disp_set_hdmi_mode(ot_vo_dev dev, td_u32 color_space);
td_void vo_hal_intf_set_rgb_sync_inv(const hal_disp_syncinv *inv);
td_void vo_hal_intf_set_mipi_sync_inv(const hal_disp_syncinv *inv);
td_void vo_hal_intf_set_mux_sel(ot_vo_dev dev, ot_vo_intf_type intf);
td_void vo_hal_intf_set_csc_enable(ot_vo_intf_type intf, td_bool enable);
td_void vo_hal_intf_set_csc_cfg(ot_vo_intf_type intf, const csc_coef *csc_cfg);
td_void vo_hal_set_intf_rgb_component_order(td_bool component_inverse_en);
td_void vo_hal_set_intf_rgb_bit_inverse(td_bool bit_inverse_en);
td_void hal_disp_set_lcd_serial_perd(td_u32 serial_perd);
td_void vo_hal_set_intf_ctrl(ot_vo_intf_type intf, const td_u32 *ctrl_info);
td_void vo_hal_set_dev_precharge_threshold(ot_vo_dev dev, td_bool te_enable);
td_void vo_hal_intf_set_lcd_dither(const vdp_dither_cfg *cfg);
#endif

#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

td_void hal_disp_set_int_mask1(td_u32 mask_en);
td_void hal_disp_clr_int_mask1(td_u32 mask_en);
td_void vo_hal_intf_set_dac_det_cable_en(ot_vo_dev dev, ot_vo_intf_type intf_type, td_bool enable);
td_void vo_hal_intf_get_dec_det_high(ot_vo_intf_type intf_type, td_bool enable,
    td_u32 *det_high);

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of HAL_VO_DEV_H */
