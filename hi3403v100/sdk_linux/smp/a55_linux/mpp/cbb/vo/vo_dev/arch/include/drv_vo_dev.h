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

#ifndef DRV_VO_DEV_H
#define DRV_VO_DEV_H

#include "valg_plat.h"
#ifdef CONFIG_OT_HDMI_SUPPORT
#include "hdmi_ext.h"
#endif
#include "sys_drv.h"

#include "inner_vo.h"
#include "drv_vo_comm.h"
#include "drv_vo_dev_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("UBOOT_VO")

typedef enum {
    VO_CLK_DIV1 = 0,
    VO_CLK_DIV2 = 1,
    VO_CLK_DIV_BUTT
} vo_clk_div;

typedef enum {
    SSC_VDP_DIV_340_TO_600 = 0,  /* 340MHz~600MHz, 1div */
    SSC_VDP_DIV_200_TO_340 = 1,  /* 200MHz~340MHz, 2div */
    SSC_VDP_DIV_100_TO_200 = 3,  /* 100MHz~200MHz, 4div */
    SSC_VDP_DIV_50_TO_100  = 7,  /* 50MHz~100MHz, 8div */
    SSC_VDP_DIV_25_TO_50   = 15, /* 25MHz~50MHz, 16div */

    SSC_VDP_DIV_BUTT
} vo_hdmi_ssc_vdp_div_mode;

typedef struct {
    ot_vo_intf_sync index;
    ot_vo_pll pll;
    vo_hdmi_ssc_vdp_div_mode div;
} vo_pll_param;

typedef struct {
    ot_vo_intf_sync index;
    ot_vo_clk_src clk_src;
    union {
        ot_vo_fixed_clk fixed_clk;
        td_u32 lcd_mclk_div;
    };
} vo_fix_lcd_param;

typedef enum {
    VO_CLK_PLL_SRC_FOUTPOSTDIV = 0, /* FOUTVCO / (postdiv1 * postdiv2) */
    VO_CLK_PLL_SRC_FOUT4 = 1, /* FOUTVCO / (postdiv1 * postdiv2 * 8) */

    VO_CLK_PLL_SRC_BUTT
} vo_clk_pll_src;

#if vo_desc("dev")
td_void vo_drv_set_dev_user_intf_sync_attr(ot_vo_dev dev, const ot_vo_user_sync_info *sync_info);
td_void vo_drv_set_dev_div(ot_vo_dev dev, td_u32 dev_div);
td_void vo_drv_set_hdmi_div(ot_vo_dev dev, td_u32 pre_div);
td_s32 vo_drv_check_other_dev_exclution_intf(ot_vo_dev other_dev, ot_vo_intf_type intf_type);
td_s32 vo_drv_check_dev_clk_src(ot_vo_dev dev, ot_vo_clk_src clk_src);
td_s32 vo_drv_check_intf_user_dev_div(ot_vo_dev dev, td_u32 dev_div);
td_s32 vo_drv_check_dev_clkvalue(ot_vo_dev dev, const ot_vo_user_sync_info *sync_info);
#endif

#if vo_desc("dev intf")
td_s32 vou_drv_check_hdmi_sync(ot_vo_dev dev, ot_vo_intf_type intf_type, ot_vo_intf_sync intf_sync);
td_s32 vou_drv_check_bt_sync(ot_vo_dev dev, ot_vo_intf_type intf_type, ot_vo_intf_sync intf_sync);
td_s32 vou_drv_check_vga_sync(ot_vo_dev dev, ot_vo_intf_type intf_type, ot_vo_intf_sync intf_sync);
td_s32 vou_drv_check_rgb_sync(ot_vo_dev dev, ot_vo_intf_type intf_type, ot_vo_intf_sync intf_sync);
td_s32 vou_drv_check_mipi_sync(ot_vo_dev dev, ot_vo_intf_type intf_type, ot_vo_intf_sync intf_sync);
td_s32 vou_drv_check_dev_intf(ot_vo_dev dev, ot_vo_intf_type intf_type);
td_u32 vo_drv_get_vga_gain_def(td_void);
td_u32 vo_drv_get_intf_vga_dac_gc(ot_vo_intf_sync intf_sync);
td_void vo_drv_set_hdmi1_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi_param);
#endif

#if vo_desc("dev")
td_void vo_drv_set_clk_reverse(ot_vo_dev dev, td_bool reverse);
td_void vo_drv_open(ot_vo_dev dev);
td_void vo_drv_close(ot_vo_dev dev);

td_void vo_drv_dev_info_init(td_void);
ot_vo_layer vo_drv_get_layer(ot_vo_dev dev);
td_void vo_drv_default_setting(td_void);
td_s32 vo_drv_check_dev_id(ot_vo_dev dev);
td_s32 vo_drv_check_layer_id(ot_vo_layer layer);
td_void vo_drv_set_dev_clk(ot_vo_dev dev);
td_void vo_drv_set_dev_clk_en(ot_vo_dev dev, td_bool clk_en);
td_void vo_drv_set_dev_clk_sel(ot_vo_dev dev, td_u32 clk_sel);
td_void vo_drv_dev_get_clk_pll_src(ot_vo_dev dev, td_u32 *pll_src);
td_void vo_drv_set_intf_dither(ot_vo_intf_type intf_type, td_u32 in_bit_width);
td_u32 vo_drv_dev_get_max_clk(ot_vo_dev dev);
#endif

td_void vo_drv_dev_get_pll_cfg(ot_vo_dev dev, ot_vo_pll *pll);
td_void vo_drv_get_pll_cfg_no_div(ot_vo_intf_sync intf_sync, ot_vo_pll *pll);
td_bool vo_drv_is_support_hdmi_ssc_vdp_div(td_void);
td_void vo_drv_get_hdmi_ssc_vdp_div(ot_vo_intf_sync intf_sync, td_u32 *hdmi_ssc_vdp_div);
td_void vo_drv_dev_set_pll_cfg(ot_vo_dev dev, ot_vo_pll *pll);
td_s32 vo_drv_check_dev_pll_ref_div(ot_vo_dev dev, td_u32 ref_div);

#if vo_desc("dev")
td_void vo_drv_set_all_crg_clk(td_bool clk_en);
td_void vo_lpw_bus_reset(td_bool reset);
#endif

td_bool vo_drv_is_dev_support_rgb(ot_vo_dev dev);
#endif /* #if vo_desc("UBOOT_VO") */

#if vo_desc("KERNEL_VO")

#if vo_desc("dev")
td_void vo_drv_get_div_mod(ot_vo_dev dev, td_u32 *div_mode);
td_bool vo_drv_is_vtth_int(ot_vo_dev dev, td_u32 int_status);
td_bool vo_drv_is_vtth2_int(ot_vo_dev dev, td_u32 int_status);
td_bool vo_drv_is_vtth3_int(ot_vo_dev dev, td_u32 int_status);
#endif

#if vo_desc("dev")
td_void vo_drv_dev_info_part_init(td_void);
#endif

#if vo_desc("dev")
td_bool vo_drv_is_dev_support_load_detect(ot_vo_dev dev);
td_void vo_drv_load_detect_reset(ot_vo_dev dev);
td_void vo_drv_load_detect_init(ot_vo_dev dev, td_bool vga_detect_en);
td_void vo_drv_load_detect_de_init(ot_vo_dev dev);
td_void vou_drv_load_detect_int_process(ot_vo_dev dev, td_u32 int_status);
td_bool vou_drv_get_dev_intf_detect_status(ot_vo_dev dev);
td_u32 vo_drv_get_lbw_int_mask(ot_vo_dev dev);
td_s32 vo_drv_get_dev_src_clk_value(ot_vo_user_sync_attr *user_sync_attr, td_u32 *src_clk);
td_s32 vo_drv_get_dev_clk_src_pll(const ot_vo_pll *pll, td_u32 *src_clk);
#endif

td_bool vo_drv_is_dev_support_vga(ot_vo_dev dev);
td_bool vo_drv_is_dev_support_hdmi(ot_vo_dev dev);
td_bool vo_drv_is_dev_support_hdmi1(ot_vo_dev dev);
td_bool vo_drv_is_dev_support_bt(ot_vo_dev dev);
td_s32 vo_drv_check_chn_id(ot_vo_chn chn);
td_bool vo_drv_is_dev_support_mipi(ot_vo_dev dev);
td_void vou_drv_low_bandwidth(ot_vo_dev dev, td_u32 int_status);
td_void vo_drv_low_delay_err(ot_vo_dev dev, td_u32 int_status);
td_u32 vo_drv_get_vtth2_int_type(ot_vo_dev dev);
td_u32 vo_drv_get_vtth1_int_type(ot_vo_dev dev);
td_void vo_drv_dev_int_enable(ot_vo_dev dev, td_bool enable);
td_void vo_drv_dev_int_clear(ot_vo_dev dev);
td_bool vo_drv_is_support_low_power(td_void);
td_s32 vo_drv_check_dev_vtth2(ot_vo_dev dev);
td_u16 vo_drv_get_vtth2_line(ot_vo_dev dev, td_u16 vtth_line, td_u16 vert_all);
td_s32 vo_drv_check_less_buf(ot_vo_dev dev, const ot_vo_less_buf_attr *less_buf_attr);
td_s32 vo_drv_check_user_notify(ot_vo_dev dev, const ot_vo_user_notify_attr *user_notify_attr);

#if vo_desc("low delay")

td_bool vo_drv_is_vgs_support_low_delay(td_void);

#endif

#endif /* #if vo_desc("KERNEL_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of DRV_VO_DEV_H */
