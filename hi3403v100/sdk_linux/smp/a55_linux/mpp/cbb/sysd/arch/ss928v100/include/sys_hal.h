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

#ifndef SYS_HAL_H
#define SYS_HAL_H

#include "ot_type.h"
#include "ot_common.h"
#include "ot_common_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define VI_INPUT_MODE_BIT  16
#define SYS_MULTI_V        1048576
#define SYS_MULTI_H        1048576

#define SYS_MULTI_V_VI     1048576

#define VI_RESET_BIT       0
#define VI_PPC_CLK_BIT     4
#define VI_AXI_CLK_BIT     5
#define VI_APB_CLK_BIT     6
#define VI_PORT_CLK_BIT    4
#define VI_CMOS_CLK_BIT    4
#define VI_CMOS_DOUBLE_BIT 12
#define VI_CLK_OFFSET      0x20

#define VI_DLL0_STATUS            0x700
#define VI_DLL0_READY_AND_LOCK    0x300
#define VI_DLL0_CKEN              0
#define VI_DLL0_SRST              1
#define VI_DLL0_MODE              16
#define VI_DLL0_BYPASS            17
#define VI_DLL0_STOP              18
#define VI_DLL0_SLAVE_EN          19
#define VI_DLL0_CKSEL             20

typedef enum {
    SYS_DRVFUNCID_DDRRST,
    SYS_DRVFUNCID_VIURST,
    SYS_DRVFUNCID_VOURST,
    SYS_DRVFUNCID_BUTT,
} sys_drvfuncid;

td_s32 sys_hal_vi_set_pt_input_sel(td_s32 dev_id, td_u32 input_sel);
td_s32 sys_hal_vi_set_thermo_work_mode(td_u32 work_mode);
td_s32 sys_hal_vi_get_ir_clk(td_u32 *clk_sel);
td_s32 sys_hal_vi_bus_clk_en(td_bool clk_en);
td_s32 sys_hal_vi_bus_reset(td_bool reset);
td_s32 sys_hal_vi_ppc_clk_en(td_bool clk_en);
td_s32 sys_hal_vi_get_ppc_clk(td_u32 *clk_sel);
td_s32 sys_hal_vi_ppc_reset(td_bool reset);
td_s32 sys_hal_vi_ppc_pt_clk_en(td_s32 dev_id, td_bool clk_en);
td_s32 sys_hal_vi_ppc_chn_clk_en(td_s32 dev_id, td_bool clk_en);
td_s32 sys_hal_vi_isp_clk_en(td_s32 dev_id, td_bool clk_en);
td_s32 sys_hal_vi_isp_core_reset(td_s32 dev_id, td_bool reset);
td_s32 sys_hal_vi_isp_cfg_reset(td_s32 dev_id, td_bool reset);
td_s32 sys_hal_vi_pt_clk_en(td_s32 dev_id, td_bool clk_en);
td_s32 sys_hal_vi_pt_reset(td_s32 dev_id, td_bool reset);
td_s32 sys_hal_vi_get_viproc_clk(td_s32 viproc_id, td_u32 *clk_sel);
td_s32 sys_hal_vi_proc_pre_clk_en(td_bool clk_en);
td_s32 sys_hal_vi_proc_pre_reset(td_bool reset);
td_s32 sys_hal_vi_set_viproc_post_clk_sel(td_u32 clk_sel);
td_s32 sys_hal_vi_proc_post_clk_en(td_bool clk_en);
td_s32 sys_hal_vi_proc_post_reset(td_bool clk_en);
td_s32 sys_hal_vi_cap_proc_pre_online_sel(td_bool online_en);
td_s32 sys_hal_vi_proc_pre_post_online_sel(td_bool online_en);
td_s32 sys_hal_vi_proc_post_vpss_online_sel(td_bool online_en);
td_s32 sys_hal_vi_get_sys_cnt(td_s32 dev_id, td_u64 *sys_cnt);
td_u64 sys_hal_get_local_sys_cnt(td_void);
td_s32 sys_hal_check_sys(td_void);

td_s32 sys_hal_vo_bus_reset_sel(td_bool reset);
td_s32 sys_hal_vo_cfg_clk_en(td_bool clk_en);
td_s32 sys_hal_vo_apb_clk_en(td_bool clk_en);
td_s32 sys_hal_vo_bus_clk_en(td_bool clk_en);
td_s32 sys_hal_vo_out_hd_phase_ctrl(td_s32 vo_dev, td_bool reverse);
td_s32 sys_hal_vo_ppc_cfg_clk_sel(td_s32 dev, td_u32 clk_sel);
td_s32 sys_hal_vo_ppc_clk_sel(td_s32 dev, td_u32 clk_sel);
td_s32 sys_hal_vo_ppc_sc_clk_sel(td_s32 dev, td_u32 clk_sel);
td_s32 sys_hal_vo_hd_clk_sel(td_s32 dev, td_u32 clk_sel);
td_s32 sys_hal_vo_core_clk_en(td_s32 dev, td_bool clk_en);
td_s32 sys_hal_vo_dev_clk_en(td_s32 vo_dev, td_bool clk_en);
td_s32 sys_hal_vo_dev_div_mode(td_s32 vo_dev, td_u32 div_mod);
td_s32 sys_hal_vo_sd_date_clk_en(td_s32 vo_dev, td_bool clk_en);
td_s32 sys_hal_vo_lcd_clk_en(td_s32 vo_dev, td_bool clk_en);
td_s32 sys_hal_vo_lcd_mclk_div(td_s32 vo_dev, td_u32 mclk_div);
td_s32 sys_hal_vo_bt_clk_en(td_s32 vo_dev, td_bool bt_clk_en);
td_s32 sys_hal_vo_bt_clk_sel(td_s32 vo_dev, td_u32 bt_clk_ch_sel);
td_s32 sys_hal_vo_vdac_clk_en(td_s32 vo_dev, td_bool clk_en);
td_s32 sys_hal_vo_hdmi_clk_en(td_s32 vo_dev, td_bool hdmi_clk_en);
td_s32 sys_hal_vo_hdmi_clk_sel(td_s32 vo_dev, td_u32 clk_ch_sel);
td_s32 sys_hal_vo_mipi_clk_en(td_s32 vo_dev, td_bool mipi_clk_en);
td_s32 sys_hal_vo_mipi_clk_sel(td_s32 vo_dev, td_u32 clk_ch_sel);
td_s32 sys_hal_vo_set_pll_postdiv2(td_s32 pll, td_u32 bits_set);
td_s32 sys_hal_vo_set_pll_postdiv1(td_s32 pll, td_u32 bits_set);
td_s32 sys_hal_vo_set_pll_frac(td_s32 pll, td_u32 bits_set);
td_s32 sys_hal_vo_set_pll_refdiv(td_s32 pll, td_u32 bits_set);
td_s32 sys_hal_vo_set_pll_fbdiv(td_s32 pll, td_u32 bits_set);
td_s32 sys_hal_vo_set_pll_power_ctrl(td_s32 pll, td_bool power_on);

td_s32 sys_hal_vedu_reset_sel(td_s32 vedu, td_bool reset);
td_s32 sys_hal_vedu_clk_en(td_s32 vedu, td_bool clk_en);
td_s32 sys_hal_vedu_clk_sel(td_s32 vedu, td_u32 clk_sel);
td_s32 sys_hal_vedu_get_apll_clk(td_s32 vedu, td_u32 *clk_sel);

td_s32 sys_hal_vpss_get_proc_clk(td_u32 *clk_sel);
td_s32 sys_hal_vpss_clk_sel(td_s32 vpss, td_u32 clk_sel);
td_s32 sys_hal_vpss_reset_sel(td_s32 vpss, td_bool reset);
td_s32 sys_hal_vpss_clk_en(td_s32 vpss, td_bool clk_en);
td_s32 sys_hal_avs_reset_sel(td_bool reset);
td_s32 sys_hal_avs_clk_en(td_bool clk_en);
td_s32 sys_hal_check_apll_clk(ot_mod_id mod_id);

td_s32 sys_hal_tde_reset_sel(td_bool reset);
td_s32 sys_hal_tde_clk_en(td_bool clk_en);

td_s32 sys_hal_jpge_reset_sel(td_bool reset);
td_s32 sys_hal_jpge_clk_en(td_bool clk_en);

td_s32 sys_hal_jpgd_clk_en(td_bool clk_en);
td_s32 sys_hal_jpgd_reset_sel(td_bool reset);

td_s32 sys_hal_pqp_reset_sel(td_bool reset);
td_s32 sys_hal_pqp_clk_en(td_bool clk_en);
td_s32 sys_hal_pqp_clk_set(td_u32 clk_value);
td_void sys_hal_pqp_clk_get(td_u32 *clk_value);

#ifdef CONFIG_OT_SVP_NPU_V2R1
td_s32 sys_hal_svp_npu0_reset_sel(td_bool reset);
td_s32 sys_hal_svp_npu0_clk_en(td_bool clk_en);
td_s32 sys_hal_svp_npu0_clk_set(td_u32 clk_value);
td_s32 sys_hal_svp_npu0_clk_get(td_u32 *clk_value);
#endif

td_s32 sys_hal_ive_reset_sel(td_bool reset);
td_s32 sys_hal_ive_clk_en(td_bool clk_en);
td_s32 sys_hal_ive_clk_set(td_u32 clk_value);
td_s32 sys_hal_mau_clk_set(td_u32 clk_value);
td_s32 sys_hal_mau_reset_sel(td_bool reset);
td_s32 sys_hal_mau_clk_en(td_bool clk_en);
td_s32 sys_hal_mau_clk_get(td_u32 *clk_value);
td_s32 sys_hal_dsp_clk_set(td_u32 clk_value);
td_s32 sys_hal_dsp_core_reset_sel(td_u32 core_id, td_bool reset);
td_s32 sys_hal_dsp_cfg_reset_sel(td_u32 core_id, td_bool reset);
td_s32 sys_hal_dsp_dbg_reset_sel(td_u32 core_id, td_bool reset);
td_s32 sys_hal_dsp_jtag_reset_sel(td_u32 core_id, td_bool reset);
td_s32 sys_hal_dsp_clk_en(td_u32 core_id, td_bool clk_en);

td_s32 sys_hal_npu_clk_en(td_bool clk_en);
td_s32 sys_hal_npu_clk_sel(td_u32 clk_value);
td_s32 sys_hal_npu_clk_get(td_u32 *clk_value);

td_s32 sys_hal_npu_srst_reset_en(td_bool rst_en);
td_s32 sys_hal_npu_div_srst_reset_en(td_bool rst_en);
td_s32 sys_hal_npu_sw_bypass_cksel(td_u32 clk_value);
td_s32 sys_hal_npu_clk_sdiv_sel(td_u32 clk_value);
td_s32 sys_hal_npu_cpm_cken(td_bool clk_en);

td_s32 sys_hal_vgs_reset_sel(td_s32 vgs, td_bool reset);
td_s32 sys_hal_vgs_clk_en(td_s32 vgs, td_bool clk_en);

td_s32 sys_hal_gdc_reset_sel(td_bool reset);
td_s32 sys_hal_gdc_clk_en(td_bool clk_en);

td_s32 sys_hal_get_custom_code(td_u32 *custom_code);

td_s32 sys_hal_init(td_void);
td_void sys_hal_exit(td_void);

td_s32 sys_hal_aio_clk_sel(td_u32 clk_sel);
td_s32 sys_hal_aio_reset_sel(td_bool reset);
td_s32 sys_hal_aio_clk_en(td_bool clk_en);
td_s32 sys_hal_acodec_reset_sel(td_bool reset);

td_u32 sys_hal_get_chip_id(td_void);
td_phys_addr_t sys_hal_get_sys_base_addr(td_void);

td_void sys_hal_get_sys(td_u8 *read_crg_58);
td_void sys_hal_get_sct(td_u64 *sct);

td_void sys_hal_get_unique_id(ot_unique_id *unique_id);
td_void sys_hal_get_otp_user_lockable(td_u32 *tee_enable);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef SYS_HAL_H */
