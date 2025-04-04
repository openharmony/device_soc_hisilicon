/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description: ws63 rf reg interface and struct adaption
 * Date: 2022-11-12
*/

#ifndef __FE_HAL_RF_PLL_REG_IF_ROM_H__
#define __FE_HAL_RF_PLL_REG_IF_ROM_H__
#include "osal_types.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
osal_void hal_rf_set_pll1_en_aac_ct(osal_u16 en_aac_ct);
osal_void hal_rf_set_pll1_en_aac(osal_u16 en_aac);
osal_void hal_rf_set_pll1_en_vco_ct(osal_u16 en_vco_ct);
osal_void hal_rf_set_pll1_en_pll_ld(osal_u16 en_pll_ld);
osal_void hal_rf_set_pll1_en_phase_slip(osal_u16 en_phase_slip);
osal_void hal_rf_set_pll1_pll_dig_ckg_en(osal_u16 pll_dig_ckg_en);
osal_void hal_rf_set_pll1_clk_force_on(osal_u16 clk_force_on);
osal_void hal_rf_set_pll1_en_dig_clk(osal_u16 en_dig_clk);
osal_void hal_rf_set_pll1_en_dig_clk_div2(osal_u16 en_dig_clk_div2);
osal_void hal_rf_set_pll1_en_dig_clk_div4(osal_u16 en_dig_clk_div4);
osal_void hal_rf_set_pll1_en_dig_clk_reserve(osal_u16 en_dig_clk_reserve);
osal_void hal_rf_set_pll1_pll_ct_en_digclk_div2(osal_u16 pll_ct_en_digclk_div2);
osal_void hal_rf_set_pll1_pll_dig_ld_ck_en(osal_u16 pll_dig_ld_ck_en);
osal_void hal_rf_set_pll1_pll_mode_sel(osal_u16 pll_mode_sel);
osal_void hal_rf_set_pll1_pll_rst_n(osal_u16 pll_rst_n);
osal_void hal_rf_set_pll1_en_pll(osal_u16 en_pll);
osal_void hal_rf_set_pll2_frac_n_msb(osal_u16 frac_n_msb);
osal_void hal_rf_set_pll3_frac_n_lsb(osal_u16 frac_n_lsb);
osal_void hal_rf_set_pll11_d_pfd_en(osal_u16 d_pfd_en);
osal_void hal_rf_set_pll11_d_pfd_en_up(osal_u16 d_pfd_en_up);
osal_void hal_rf_set_pll11_d_pfd_en_dn(osal_u16 d_pfd_en_dn);
osal_void hal_rf_set_pll11_d_pfd_force_up(osal_u16 d_pfd_force_up);
osal_void hal_rf_set_pll11_d_pfd_force_dn(osal_u16 d_pfd_force_dn);
osal_void hal_rf_set_pll11_d_cp_en(osal_u16 d_cp_en);
osal_void hal_rf_set_pll11_d_cp_icp(osal_u16 d_cp_icp);
osal_void hal_rf_set_pll12_d_lpf_en_fll(osal_u16 d_lpf_en_fll);
osal_void hal_rf_set_pll12_d_lpf_en_swfll(osal_u16 d_lpf_en_swfll);
osal_void hal_rf_set_pll12_d_lpf_en_amp(osal_u16 d_lpf_en_amp);
osal_void hal_rf_set_pll12_d_lpf_amp_itrim(osal_u16 d_lpf_amp_itrim);
osal_void hal_rf_set_pll12_d_lpf_dac_vref_sel_ptat(osal_u16 d_lpf_dac_vref_sel_ptat);
osal_void hal_rf_set_pll12_d_lpf_dac_vset(osal_u16 d_lpf_dac_vset);
osal_void hal_rf_set_pll13_d_lpf_c0(osal_u16 d_lpf_c0);
osal_void hal_rf_set_pll13_d_lpf_c1(osal_u16 d_lpf_c1);
osal_void hal_rf_set_pll13_d_lpf_c3(osal_u16 d_lpf_c3);
osal_void hal_rf_set_pll13_d_lpf_c4(osal_u16 d_lpf_c4);
osal_void hal_rf_set_pll13_d_lpf_r1(osal_u16 d_lpf_r1);
osal_void hal_rf_set_pll13_d_lpf_r3(osal_u16 d_lpf_r3);
osal_void hal_rf_set_pll16_d_vco_ibias_faston(osal_u16 d_vco_ibias_faston);
osal_void hal_rf_set_pll16_d_vco_ibias_rc_trim(osal_u16 d_vco_ibias_rc_trim);
osal_void hal_rf_set_pll16_d_vco_ibias_ibg_sel(osal_u16 d_vco_ibias_ibg_sel);
osal_void hal_rf_set_pll16_d_vco_ibias_iptat_sel(osal_u16 d_vco_ibias_iptat_sel);
osal_void hal_rf_set_pll16_d_vco_tail_ind_ctrim(osal_u16 d_vco_tail_ind_ctrim);
osal_void hal_rf_set_pll17_d_vco_buf_pll_trim(osal_u16 d_vco_buf_pll_trim);
osal_void hal_rf_set_pll17_d_vco_buf_lo_trim(osal_u16 d_vco_buf_lo_trim);
osal_void hal_rf_set_pll17_d_vco_gm_couple_bypass(osal_u16 d_vco_gm_couple_bypass);
osal_void hal_rf_set_pll17_d_vco_gm_rtrim(osal_u16 d_vco_gm_rtrim);
osal_void hal_rf_set_pll17_d_vco_ibias_en(osal_u16 d_vco_ibias_en);
osal_void hal_rf_set_pll17_d_vco_ibias_bypass_en(osal_u16 d_vco_ibias_bypass_en);
osal_void hal_rf_set_pll17_d_vco_var_bias_en(osal_u16 d_vco_var_bias_en);
osal_void hal_rf_set_pll17_d_vco_ct_bias_en(osal_u16 d_vco_ct_bias_en);
osal_void hal_rf_set_pll17_d_vco_ct_bias_sel(osal_u16 d_vco_ct_bias_sel);
osal_void hal_rf_set_pll18_d_vco_var_bias_faston(osal_u16 d_vco_var_bias_faston);
osal_void hal_rf_set_pll18_d_vco_var_bias_sel(osal_u16 d_vco_var_bias_sel);
osal_void hal_rf_set_pll18_d_vco_buf_dty_cyl_adj(osal_u16 d_vco_buf_dty_cyl_adj);
osal_void hal_rf_set_pll18_d_vco_var_kp_trim(osal_u16 d_vco_var_kp_trim);
osal_void hal_rf_set_pll18_d_vco_var_ki_trim(osal_u16 d_vco_var_ki_trim);
osal_void hal_rf_set_pll19_d_vco_atemp_faston(osal_u16 d_vco_atemp_faston);
osal_void hal_rf_set_pll19_d_vco_atemp_rc_trim(osal_u16 d_vco_atemp_rc_trim);
osal_void hal_rf_set_pll19_d_vco_atemp_gnd_en(osal_u16 d_vco_atemp_gnd_en);
osal_void hal_rf_set_pll19_d_vco_atemp_ptat_mode(osal_u16 d_vco_atemp_ptat_mode);
osal_void hal_rf_set_pll19_d_vco_atemp_ptat_slope(osal_u16 d_vco_atemp_ptat_slope);
osal_void hal_rf_set_pll19_d_vco_atemp_preload_mode_en(osal_u16 d_vco_atemp_preload_mode_en);
osal_void hal_rf_set_pll19_d_vco_atemp_dac_vset(osal_u16 d_vco_atemp_dac_vset);
osal_void hal_rf_set_pll19_d_vco_bias_always_on(osal_u16 d_vco_bias_always_on);
osal_void hal_rf_set_pll19_d_vco_reserve(osal_u16 d_vco_reserve);
osal_void hal_rf_set_pll29_pll_ct_manual_vco1(osal_u16 pll_ct_manual_vco1);
osal_void hal_rf_set_pll29_pll_ct_manual_band_vco1(osal_u16 pll_ct_manual_band_vco1);
osal_void hal_rf_set_pll30_pll_ct_manual_vco2(osal_u16 pll_ct_manual_vco2);
osal_void hal_rf_set_pll30_pll_ct_manual_band_vco2(osal_u16 pll_ct_manual_band_vco2);
osal_void hal_rf_set_pll31_pll_ctc_bit_sel(osal_u16 pll_ctc_bit_sel);
osal_u16 hal_rf_get_pll31_pll_ctc_bit_sel(osal_void);
osal_void hal_rf_set_pll31_pll_ct_en_find_band0(osal_u16 pll_ct_en_find_band0);
osal_u16 hal_rf_get_pll31_pll_ct_en_find_band0(osal_void);
osal_void hal_rf_set_pll31_ct_pol(osal_u16 ct_pol);
osal_u16 hal_rf_get_pll31_ct_pol(osal_void);
osal_void hal_rf_set_pll31_ct_band_offset(osal_u16 ct_band_offset);
osal_u16 hal_rf_get_pll31_ct_band_offset(osal_void);
osal_void hal_rf_set_pll31_pll_ct_coarse_avg_timer(osal_u16 pll_ct_coarse_avg_timer);
osal_u16 hal_rf_get_pll31_pll_ct_coarse_avg_timer(osal_void);
osal_void hal_rf_set_pll31_pll_ct_fine_avg_timer(osal_u16 pll_ct_fine_avg_timer);
osal_u16 hal_rf_get_pll31_pll_ct_fine_avg_timer(osal_void);
osal_u16 hal_rf_get_pll52_ld_dig_done(osal_void);
osal_u16 hal_rf_get_pll52_ld_lock_cnt(osal_void);
osal_u16 hal_rf_get_pll52_ct_band_rb(osal_void);
osal_u16 hal_rf_get_pll53_pll_lock_indicator(osal_void);
osal_u16 hal_rf_get_pll53_ctc_done_flag(osal_void);
osal_u16 hal_rf_get_pll53_ctf_done_flag(osal_void);
osal_u16 hal_rf_get_pll53_ct_end_flag(osal_void);
osal_u16 hal_rf_get_pll53_warm_ready_flag(osal_void);
osal_u16 hal_rf_get_pll53_aac_ct_end_flag(osal_void);
osal_u16 hal_rf_get_pll53_vco_ct_end_flag(osal_void);
osal_u16 hal_rf_get_pll53_phase_ready_flag(osal_void);
osal_u16 hal_rf_get_pll53_close_ready_fll_flag(osal_void);
osal_u16 hal_rf_get_pll53_close_ready_smp_flag(osal_void);
osal_u16 hal_rf_get_pll53_bg_cal_ready_flag(osal_void);
osal_u16 hal_rf_get_pll53_fsm_state_final(osal_void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* end of __FE_HAL_RF_PLL_REG_IF_ROM_H__ */
