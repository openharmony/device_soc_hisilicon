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

#ifndef DRV_VO_DEV_COMM_H
#define DRV_VO_DEV_COMM_H

#include "ot_common_vo.h"
#include "drv_vo_comm.h"
#include "ot_comm_irq.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")
typedef enum {
    VO_HD_HW_DEV = 0,  /* HD dev */
    VO_SD_HW_DEV,  /* SD dev */
    VO_UHD_HW_DEV,  /* UHD dev */
    VO_CAS_DEV,  /* cascade dev */
    VO_VIRT_DEV,  /* virtual dev */
    VO_DEV_TYPE_BUTT,
} vo_dev_type;

#define VO_DEV_REG_UP_TIMEOUT 50000  /* 50000: 50ms */

#define VO_MIN_DIV_MODE      1
#define VO_MAX_DIV_MODE      4
#define VO_INTF_HDMI_DIV_MODE      1 /* vga/bt1120 is the same */
#define VO_INTF_CVBS_DIV_MODE      4
#define VO_INTF_BT656_DIV_MODE     2
#define VO_INTF_RGB_DIV_MODE_1     1
#define VO_INTF_RGB_DIV_MODE_3     3
#define VO_INTF_RGB_DIV_MODE_4     4

#define VO_MIN_PRE_DIV_MODE  1
#define VO_MAX_PRE_DIV_MODE  32
#define VO_INTF_NO_HDMI_PRE_DIV_MODE      1
#define VO_INTF_HDMI_PRE_DIV_MODE         1

#define VO_MIN_PLL_REF_DIV   1
#define VO_DEF_PLL_REF_DIV   1
#define VO_MAX_PLL_REF_DIV   4

#define VO_MAX_USER_SYNC_INTFB 255
#define VO_MAX_USER_SYNC_VACT  4096
#define VO_MIN_USER_SYNC_VACT  100
#define VO_MAX_USER_SYNC_VBB   256
#define VO_MIN_USER_SYNC_VBB   1
#define VO_MAX_USER_SYNC_VFB   256
#define VO_MIN_USER_SYNC_VFB   1
#define VO_MAX_USER_SYNC_HACT  4096
#define VO_MIN_USER_SYNC_HACT  1
#define VO_MAX_USER_SYNC_HBB   65535
#define VO_MIN_USER_SYNC_HBB   1
#define VO_MAX_USER_SYNC_HFB   65535
#define VO_MIN_USER_SYNC_HFB   1
#define VO_MAX_USER_SYNC_BVACT 4096
#define VO_MAX_USER_SYNC_BVBB  256
#define VO_MAX_USER_SYNC_BVFB  256
#define VO_MAX_USER_SYNC_HPW   65535
#define VO_MIN_USER_SYNC_HPW   1
#define VO_MAX_USER_SYNC_VPW   256
#define VO_MIN_USER_SYNC_VPW   1

#if vo_desc("dev drv api")
td_u32 vo_drv_get_dev_bg_color(ot_vo_dev dev);
ot_vo_intf_type vo_drv_get_dev_intf_type(ot_vo_dev dev);
ot_vo_intf_sync vo_drv_get_dev_intf_sync(ot_vo_dev dev);
td_void vou_drv_set_disp_max_size(ot_vo_dev dev, td_u32 max_width, td_u32 max_height);
#endif

#if vo_desc("dev")
td_void vo_drv_get_sync_info(ot_vo_dev dev, hal_disp_syncinfo *sync_info);
td_void vo_drv_set_sync_info(ot_vo_dev dev, const hal_disp_syncinfo *sync_info);
td_bool vo_drv_is_progressive(ot_vo_dev dev);
td_bool vou_drv_get_dev_enable(ot_vo_dev dev);
td_s32 vo_drv_check_dev_pll(ot_vo_dev dev, const ot_vo_user_sync_info *sync_info);
td_s32 vou_drv_check_dev_sync(ot_vo_dev dev, ot_vo_intf_type intf_type, ot_vo_intf_sync intf_sync);
td_s32 vou_drv_check_dev_pub_attr(ot_vo_dev dev, const ot_vo_pub_attr *pub_attr);
td_void vo_drv_get_dev_reg_up(ot_vo_dev dev);
#endif

#if vo_desc("dev layer prio")
td_void vou_drv_set_layer_priority(ot_vo_dev dev, ot_vo_layer layer, td_u32 priority);
#endif

#if vo_desc("dev intf")
td_void vou_drv_intf_csc_config(ot_vo_intf_type intf, const ot_vo_csc *csc);
td_void vo_drv_set_vga_param(ot_vo_dev dev, const ot_vo_vga_param *vga_param);
td_void vo_drv_set_hdmi_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi_param);
td_void vo_drv_set_rgb_param(ot_vo_dev dev, const ot_vo_rgb_param *rgb_param);
td_void vo_drv_set_bt_param(ot_vo_dev dev, const ot_vo_bt_param *bt_param);
td_void vo_drv_set_bt_param_clk_edge(ot_vo_dev dev, ot_vo_clk_edge clk_edge);
ot_vo_clk_edge vo_drv_get_bt_param_clk_edge(ot_vo_dev dev);
td_void vo_drv_set_mipi_param(ot_vo_dev dev, const ot_vo_mipi_param *mipi_param);
td_void vo_drv_set_dev_mpp_chn(ot_vo_dev dev, ot_mpp_chn *mpp_chn);
td_bool vo_drv_is_hdmi_intf(ot_vo_intf_type intf_type);
td_bool vo_drv_is_hdmi1_intf(ot_vo_intf_type intf_type);
td_bool vo_drv_is_rgb_intf(ot_vo_intf_type intf_type);
td_bool vo_drv_is_vga_intf(ot_vo_intf_type intf_type);
td_bool vo_drv_is_cvbs_intf(ot_vo_intf_type intf_type);
td_bool vo_drv_is_mipi_intf(ot_vo_intf_type intf_type);
td_bool vo_drv_is_bt_intf(ot_vo_intf_type intf_type);
td_bool vo_drv_is_bt1120_intf(ot_vo_intf_type intf_type);
td_bool vo_drv_is_bt656_intf(ot_vo_intf_type intf_type);
td_bool vo_drv_is_rgb_serial_intf(ot_vo_intf_type intf_type);
td_bool vo_drv_is_rgb_parallel_intf(ot_vo_intf_type intf_type);
td_s32 vo_drv_check_intf_share(ot_vo_dev dev, ot_vo_intf_type intf_type);
#ifdef CONFIG_OT_VO_BT1120_DUAL_EDGE
td_void vo_drv_set_out_clk_div(td_u32 dev, td_u32 vo_out_clk_div);
#endif
#endif

#if vo_desc("dev interrupt")
td_void vo_drv_int_set_mode(ot_vo_dev dev, vo_int_mode int_mode);
#endif

#if vo_desc("check id")
td_s32 vo_drv_check_dev_id_pub(ot_vo_dev dev);
td_s32 vo_drv_check_layer_id_pub(ot_vo_layer layer);
#endif

td_void vo_drv_set_pub_attr(ot_vo_dev dev, const ot_vo_pub_attr *vo_pub_attr);
td_void vo_drv_enable(ot_vo_dev dev);
td_void vo_drv_disable(ot_vo_dev dev);

td_bool vo_drv_is_phy_dev_uhd(ot_vo_dev dev);
td_bool vo_drv_is_phy_dev_hd(ot_vo_dev dev);
td_bool vo_drv_is_phy_dev_sd(ot_vo_dev dev);
#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

#if vo_desc("dev")
td_void vo_drv_set_dev_param(ot_vo_dev dev, const ot_vo_dev_param *dev_param);
td_u32 vo_drv_get_dev_vtth(ot_vo_dev dev);
td_u32 vo_drv_get_dev_vtth2(ot_vo_dev dev);
td_void vo_drv_int_reg_up_mode(ot_vo_layer layer, vo_int_mode int_mode);
td_u32 vo_drv_get_sync_field_vert_all(hal_disp_syncinfo *sync_info);
td_s32 vo_drv_check_dev_vtth(ot_vo_dev dev);
td_u32 vo_drv_cal_vtth1_by_sync(ot_vo_dev dev, td_u32 full_frame_rate);
td_void vo_drv_set_dev_default_vtth(ot_vo_dev dev);
td_void vo_drv_set_dev_vtth(ot_vo_dev dev, td_u32 vtth);
td_void vo_drv_set_dev_vtth2(ot_vo_dev dev, td_u32 vtth2);
td_void vo_drv_set_vtth(ot_vo_dev dev);
td_bool vo_drv_get_dev_dfir_en(ot_vo_dev dev);
td_void vo_drv_refresh_dev_vtth2(ot_vo_dev dev);
td_bool vo_drv_get_vdac_status(ot_vo_dev dev);
td_s32 vo_drv_enable_load_detect(ot_vo_dev dev);
td_s32 vo_drv_disable_load_detect(ot_vo_dev dev);
td_bool vo_drv_is_dev_support_wbc(ot_vo_dev dev);
td_void vo_drv_set_dev_less_buf_enable(ot_vo_dev dev, td_bool enable);
td_bool vo_drv_get_dev_less_buf_enable(ot_vo_dev dev);
td_void vo_drv_set_dev_user_notify_enable(ot_vo_dev dev, td_bool enable);
td_bool vo_drv_get_dev_user_notify_enable(ot_vo_dev dev);
td_void vo_drv_set_dev_default_vtth2(ot_vo_dev dev);
td_void vou_drv_get_dev_disp_mode(ot_vo_dev dev, td_bool *interface);
td_void vou_drv_get_dev_int_state(ot_vo_dev dev, td_bool *bottom);
#ifdef CONFIG_OT_VO_CASCADE
td_void vo_drv_get_dev_int_state_vblank(ot_vo_dev dev, td_bool *vblank);
#endif
td_void vou_drv_get_dev_int_state_vcnt(ot_vo_dev dev, td_u32 *vcnt);
#ifdef CONFIG_OT_VO_CASCADE
td_void vo_drv_set_dev_reg_up(ot_vo_dev dev);
#endif
td_bool vou_drv_is_virt_dev(ot_vo_dev dev);
td_u32 vo_drv_get_low_bandwidth_cnt(ot_vo_dev dev);
td_u32 vo_drv_get_low_delay_err_cnt(ot_vo_dev dev);
#endif

#if vo_desc("dev intf")
td_bool vo_drv_check_phy_dev_type(ot_vo_dev dev, vo_dev_type dev_type);
td_bool vo_drv_is_virt_dev(ot_vo_dev dev);
td_bool vo_drv_is_cas_dev(ot_vo_dev dev);
td_s32 vo_drv_check_vga_csc_matrix(ot_vo_csc_matrix csc_matrix);
td_s32 vo_drv_check_hdmi_csc_matrix(ot_vo_csc_matrix csc_matrix);
td_s32 vo_drv_check_rgb_csc_matrix(ot_vo_csc_matrix csc_matrix);
td_s32 vo_drv_check_mipi_csc_matrix(ot_vo_csc_matrix csc_matrix);
#endif

#if vo_desc("dev interrupt")
td_void vo_drv_do_int_vtth_config(ot_vo_dev dev, td_bool int_enable);
td_void vou_drv_int_clear(td_u32 int_clear);
td_void vou_drv_int_disable_all(td_void);
#endif

#if vo_desc("dev src clk")
td_void vo_drv_get_dev_div_pre_div(ot_vo_dev dev, td_u32 *pre_div, td_u32 *dev_div);
td_void vo_drv_get_sync_attr_info(ot_vo_dev dev, ot_vo_user_sync_attr *user_sync_attr);
#endif

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of DRV_VO_DEV_COMM_H */
