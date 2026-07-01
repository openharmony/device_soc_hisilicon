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

#ifndef HAL_VO_DEV_COMM_H
#define HAL_VO_DEV_COMM_H

#include "hal_vo_def.h"
#include "drv_vo_coef_comm.h"
#include "inner_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")
#if vo_desc("intf")
td_void hal_disp_set_intf_enable(ot_vo_dev dev, td_bool intf);
td_void hal_disp_get_intf_enable(ot_vo_dev dev, td_bool *intf_en);
td_void hal_disp_set_hdmi_mode(ot_vo_dev dev, td_u32 color_space);
td_void hal_disp_set_dev_multi_chn_en(ot_vo_dev dev, hal_multichn_en multi_chn_en);
#endif

#if vo_desc("disp intf")
td_void hal_disp_clear_int_status(td_u32 int_msk);
td_void hal_intf_bt_set_dfir_en(td_u32 dfir_en);
td_void hal_intf_bt_set_data_width(td_u32 data_width);
td_void hal_disp_set_reg_up(ot_vo_dev dev);
td_u32 hal_disp_get_reg_up(ot_vo_dev dev);
#endif

#if vo_desc("intf")
td_void vo_hal_intf_set_bt_sync_inv(const hal_disp_syncinv *inv);
td_void vo_hal_intf_set_cvbs_sync_inv(const hal_disp_syncinv *inv);
td_void vo_hal_intf_set_vga_sync_inv(const hal_disp_syncinv *inv);
td_void vo_hal_intf_set_hdmi_sync_inv(const hal_disp_syncinv *inv);
td_void vo_hal_intf_set_hdmi_csc_dc_coef(const vdp_csc_dc_coef *csc_dc_coef);
td_void vo_hal_intf_set_vga_csc_dc_coef(const vdp_csc_dc_coef *csc_dc_coef);
td_void vo_hal_intf_set_hdmi_csc_coef(const vdp_csc_coef *coef);
td_void vo_hal_intf_set_vga_csc_coef(const vdp_csc_coef *coef);
td_void vo_hal_intf_set_dac_sel(ot_vo_intf_type intf_type);
td_void vo_hal_intf_set_vga_and_cvbs_dac_enable(td_bool enable);
td_void vo_hal_intf_set_dac_chn_enable(ot_vo_intf_type intf_type, td_bool enable);
td_void vo_hal_intf_set_dac_gc(ot_vo_intf_type intf_type, td_u32 dac_gc);
td_void vo_hal_intf_set_dac_cablectr(ot_vo_intf_type intf_type, td_u32 cablectr);
td_void vo_hal_intf_set_dac_det(ot_vo_dev dev, ot_vo_intf_type intf_type, td_bool enable);
td_void vo_hal_intf_set_vga_hsp_cfg(td_bool enable, td_u32 strength);
td_void vo_hal_intf_set_date_cvbs_color_burst(td_void);
td_u32 vo_hal_intf_get_date(ot_vo_intf_sync intf_sync, td_u32 vact);
td_void vo_hal_intf_set_date_cfg(ot_vo_intf_sync intf_sync, td_u32 date, td_u32 pal_cvbs_gain, td_u32 ntsc_cvbs_gain);
td_void vo_hal_intf_set_clip_cfg(ot_vo_intf_type intf_type, td_bool clip, const hal_disp_clip *clip_data);
td_void vo_hal_intf_set_sync_info(ot_vo_dev dev, const hal_disp_syncinfo *sync_info);
td_void vo_hal_set_intf_bt_component_order(td_bool component_inverse_en);
td_void vo_hal_set_intf_bt_bit_inverse(td_bool bit_inverse_en);
#endif
#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")
#if vo_desc("intf")
td_void hal_disp_set_vtthd_mode(ot_vo_dev dev, td_u32 field_mode);
td_void hal_disp_set_vtthd(ot_vo_dev dev, td_u32 vtthd, td_u32 vtthd2);
td_void hal_disp_get_int_state(ot_vo_dev dev, td_bool *bottom);
td_void hal_disp_get_vtthd_mode(ot_vo_dev dev, td_bool *field_mode);
#ifdef CONFIG_OT_VO_CASCADE
td_void hal_disp_get_int_state_vblank(ot_vo_dev dev, td_bool *vblank);
#endif
td_void hal_disp_get_int_state_vcnt(ot_vo_dev dev, td_u32 *vcnt);
td_void hal_disp_get_disp_iop(ot_vo_dev dev, td_bool *iop);
#endif

#if vo_desc("disp intf")
td_void hal_disp_set_int_mask(td_u32 mask_en);
td_void hal_disp_clr_int_mask(td_u32 mask_en);
#endif

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of HAL_VO_DEV_COMM_H */
