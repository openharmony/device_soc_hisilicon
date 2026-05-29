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

#ifndef CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART05_H_
#define CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART05_H_

            break;
        default:
            hdmi_err("clk_ratio %u is invalid\n", clk_ratio);
            return;
    }
    hdmi_reg_aphy_data_clk_height_set(phy_fifo_ck_h);
    hdmi_reg_aphy_data_clk_low_set(phy_fifo_ck_l);

    return;
}

static td_u32 phy_transfer_hex2cur(td_u32 reg_hex)
{
    td_u32 cur;

    if (reg_hex < 0x10) {
        cur = reg_hex;
    } else if ((reg_hex >= 0x10) && (reg_hex < 0x20)) {
        cur = (reg_hex - 0x04);
    } else if ((reg_hex >= 0x20) && (reg_hex < 0x30)) {
        cur = (reg_hex - 0x10);
    } else if ((reg_hex >= 0x30) && (reg_hex < 0x40)) {
        cur = (reg_hex - 0x14);
    } else {
        hdmi_err("get current(%u) error.\n", reg_hex);
        cur = reg_hex;
    }

    return cur;
}

static td_void phy_def_hw_param_get(hdmi_hw_spec *hw_param)
{
    td_s32 i;
    phy_hwspec_enhance *def_hw_spec = TD_NULL;

    def_hw_spec = g_phy_hw_spec_enhance;
    for (i = 0; i < HDMI_HW_PARAM_LEN; i++) {
        hw_param->hw_param[i].ft_cap_clk = def_hw_spec[i].cfg.feedthru_cap_delay_clk;
        hw_param->hw_param[i].ft_cap_data = def_hw_spec[i].cfg.feedthru_cap_delay_d0;
        hw_param->hw_param[i].i_de_main_clk = def_hw_spec[i].cfg.i_demain_clk;
        hw_param->hw_param[i].i_de_main_data = def_hw_spec[i].cfg.i_demain_d0;
        hw_param->hw_param[i].i_main_clk = def_hw_spec[i].cfg.i_main_clk;
        hw_param->hw_param[i].i_main_data = def_hw_spec[i].cfg.i_main_d0;
    }

    return;
}

static td_void phy_cur_hw_param_get(hdmi_hw_param *hw_param)
{
    td_u32 tmp;

    tmp = hdmi_reg_isel_main_de_clk_get();
    hw_param->i_de_main_clk = phy_transfer_hex2cur(tmp);
    tmp = hdmi_reg_isel_main_de_d0_get();
    hw_param->i_de_main_data = phy_transfer_hex2cur(tmp);
    tmp = hdmi_reg_isel_main_clk_get();
    hw_param->i_main_clk = phy_transfer_hex2cur(tmp);
    tmp = hdmi_reg_isel_main_d0_get();
    hw_param->i_main_data = phy_transfer_hex2cur(tmp);

    tmp = hdmi_reg_rsel_pre_clk_get();
    /* if enable */
    if (tmp & PHY_FEEDTHRU_EN_MASK) {
        tmp &= PHY_FEEDTHRU_DATA_MASK; /* get data */
        tmp++; /* reg val add 1 */
        hw_param->ft_cap_clk = phy_transfer_hex2cur(tmp);
    } else {
        hw_param->ft_cap_clk = 0;
    }

    tmp = hdmi_reg_rsel_pre_d0_get();
    /* if enable */
    if (tmp & PHY_FEEDTHRU_EN_MASK) {
        tmp &= PHY_FEEDTHRU_DATA_MASK; /* get data */
        tmp++; /* reg val add 1 */
        hw_param->ft_cap_data = phy_transfer_hex2cur(tmp);
    } else {
        hw_param->ft_cap_data = 0;
    }

    return;
}

static td_void phy_default_spec_set(td_void)
{
    td_u8 i;
    hdmi_hw_spec *spec = TD_NULL;
    td_u32 otp = 0;

    /* otp: 0 = default, 1 - opt_1 phy config, 2 - opt_2 phy config */
    otp = phy_get_otp_value();
    g_hdmi_phy_info.old_trace_len = HDMI_TRACE_DEFAULT;
    spec = &g_hdmi_phy_info.hw_spec;
    for (i = 0; i < HDMI_HW_PARAM_NUM; i++) {
#ifdef PHY_CRAFT_S28
        if (otp == 0) { /* 0 - normal phy config */
            spec->hw_param[i].i_de_main_clk  = g_phy_hw_spec_enhance[i].cfg.i_demain_clk;
            spec->hw_param[i].i_de_main_data = g_phy_hw_spec_enhance[i].cfg.i_demain_d0;
            spec->hw_param[i].i_main_clk     = g_phy_hw_spec_enhance[i].cfg.i_main_clk;
            spec->hw_param[i].i_main_data    = g_phy_hw_spec_enhance[i].cfg.i_main_d0;
            spec->hw_param[i].ft_cap_clk     = g_phy_hw_spec_enhance[i].cfg.feedthru_cap_clk_en ?
                (g_phy_hw_spec_enhance[i].cfg.feedthru_cap_delay_clk + 1) : 0;
            spec->hw_param[i].ft_cap_data    = g_phy_hw_spec_enhance[i].cfg.feedthru_cap_d0_en ?
                (g_phy_hw_spec_enhance[i].cfg.feedthru_cap_delay_d0 + 1) : 0;
        } else if (otp == 1 || otp == 2) { /* 1 - opt_1 phy config, 2 - opt_2 phy config */
#if defined(CHIP_TYPE_ss528v100)
            td_u32 index = otp - 1;
            spec->hw_param[i].i_de_main_clk  = g_phy_hw_spec_enhance_otp[index][i].cfg.i_demain_clk;
            spec->hw_param[i].i_de_main_data = g_phy_hw_spec_enhance_otp[index][i].cfg.i_demain_d0;
            spec->hw_param[i].i_main_clk     = g_phy_hw_spec_enhance_otp[index][i].cfg.i_main_clk;
            spec->hw_param[i].i_main_data    = g_phy_hw_spec_enhance_otp[index][i].cfg.i_main_d0;
            spec->hw_param[i].ft_cap_clk     = g_phy_hw_spec_enhance_otp[index][i].cfg.feedthru_cap_clk_en ?
                (g_phy_hw_spec_enhance_otp[index][i].cfg.feedthru_cap_delay_clk + 1) : 0;
            spec->hw_param[i].ft_cap_data    = g_phy_hw_spec_enhance_otp[index][i].cfg.feedthru_cap_d0_en ?
                (g_phy_hw_spec_enhance_otp[index][i].cfg.feedthru_cap_delay_d0 + 1) : 0;
#endif
        } else {
            hdmi_err("otp_flag %u is invalid, valid range [0 , 2].\n", otp);
        }
#endif
    }

    return;
}

static td_void hal_hdmi_phy_ssc_on(td_u32 span_fb, td_u32 step_fb)
{
    /* step 1 */
    hdmi_reg_step_fb_set(step_fb);
    hdmi_reg_span_fb_set(span_fb);
    /* step 2 */
    hdmi_reg_ssc_mode_fb_set(TD_TRUE);
    /* step 3 */
    hdmi_reg_load_fb_set(TD_FALSE);
    hdmi_reg_load_fb_set(TD_TRUE);
    hdmi_reg_load_fb_set(TD_FALSE);
    /* step 4 */
    hdmi_reg_gc_txpll_test_set(PHY_TXPLL_TEST_ON);
    /* step 5 */
    hdmi_reg_gc_txpll_en_sscdiv_set(TD_TRUE);

    return;
}

static td_void hal_hdmi_phy_ssc_off(hdmi_deep_color deep_color)
{
    hdmi_reg_step_fb_set(0);
    hdmi_reg_span_fb_set(0);
    /* 12bit need to enter decimal mode */
    if (deep_color == HDMI_DEEP_COLOR_36BIT) {
        /* step 1 */
        hdmi_reg_ssc_mode_fb_set(TD_TRUE);
        /* step 2 */
        hdmi_reg_load_fb_set(TD_FALSE);
        hdmi_reg_load_fb_set(TD_TRUE);
        hdmi_reg_load_fb_set(TD_FALSE);
        /* step 3 */
        hdmi_reg_gc_txpll_test_set(PHY_TXPLL_TEST_ON);
        /* step 4 */
        hdmi_reg_gc_txpll_en_sscdiv_set(TD_TRUE);
    } else {
        hdmi_reg_gc_txpll_en_sscdiv_set(TD_FALSE);
        hdmi_reg_gc_txpll_test_set(PHY_TXPLL_TEST_OFF);
        hdmi_reg_ssc_mode_fb_set(TD_FALSE);
    }

    return;
}

static phy_div_cfg *phy_div_cfg_get(td_u32 pix_clk, hdmi_deep_color deep_clr)
{
    td_u32 i;
    phy_div_cfg *div_cfg = TD_NULL;

    for (i = 0, div_cfg = &g_phy_div_cfg[0]; (div_cfg && (i < hdmi_array_size(g_phy_div_cfg))); div_cfg++, i++) {
        if ((pix_clk >= div_cfg->pix_clk_range.clk_min) &&
            (pix_clk <= div_cfg->pix_clk_range.clk_max) &&
            (deep_clr == div_cfg->deep_color)) {
            return div_cfg;
        }
    }
    hdmi_err("can't find param, tmds_clk:%u, deep_clr=%u.\n", pix_clk, deep_clr);

    return &g_phy_div_cfg[1];
}

static td_void phy_rterm_init(td_void)
{
    td_u32 i;
    td_u32 tmp_regval;
    phy_rterm_mode_data rterm_mode = {0};
    phy_rterm_mode mode_clk = PHY_RTERM_MODE_SINGLE;
    phy_rterm_mode mode_d0  = PHY_RTERM_MODE_SINGLE;
    phy_rterm_mode mode_d1  = PHY_RTERM_MODE_SINGLE;
    phy_rterm_mode mode_d2  = PHY_RTERM_MODE_SINGLE;

    tmp_regval = hdmi_reg_gc_pd_rterm_get();
    rterm_mode.mode_data_clk = tmp_regval & 0x03;
    rterm_mode.mode_data_d0  = tmp_regval & 0x0C;
    rterm_mode.mode_data_d1  = tmp_regval & 0x30;
    rterm_mode.mode_data_d2  = tmp_regval & 0xC0;

    for (i = 0; i < hdmi_array_size(g_phy_rterm_mode_deta); i++) {
        if (rterm_mode.mode_data_clk == g_phy_rterm_mode_deta[i].mode_data_clk) {
            mode_clk = g_phy_rterm_mode_deta[i].r_term_mode;
        }
        if (rterm_mode.mode_data_d0 == g_phy_rterm_mode_deta[i].mode_data_d0) {
            mode_d0 = g_phy_rterm_mode_deta[i].r_term_mode;
        }
        if (rterm_mode.mode_data_d1 == g_phy_rterm_mode_deta[i].mode_data_d1) {
            mode_d1 = g_phy_rterm_mode_deta[i].r_term_mode;
        }
        if (rterm_mode.mode_data_d2 == g_phy_rterm_mode_deta[i].mode_data_d2) {
            mode_d2 = g_phy_rterm_mode_deta[i].r_term_mode;
        }
    }

#ifdef PHY_CRAFT_S28
    g_hdmi_phy_info.spec_enhance_cfg.rterm_mode_clk = mode_clk;
    g_hdmi_phy_info.spec_enhance_cfg.rterm_mode_d0  = mode_d0;
    g_hdmi_phy_info.spec_enhance_cfg.rterm_mode_d1  = mode_d1;
    g_hdmi_phy_info.spec_enhance_cfg.rterm_mode_d2  = mode_d2;
#endif

    hdmi_info("clk:%d, d0:%d, d1:%d, d2:%d, RTerm:0x%x\n",
              mode_clk, mode_d0, mode_d1, mode_d2, tmp_regval);

    return;
}

/* hisense LED32H130, this TV can't resume when RTerm is opend */
static td_void phy_rterm_set(td_bool enable)
{
#ifdef PHY_CRAFT_S28
    td_u32 tmp;

    if (enable == TD_TRUE) {
        tmp = phy_rterm_mode_data_get(g_hdmi_phy_info.spec_enhance_cfg.rterm_mode_clk)->mode_data_clk;
        tmp |= phy_rterm_mode_data_get(g_hdmi_phy_info.spec_enhance_cfg.rterm_mode_d0)->mode_data_d0;
        tmp |= phy_rterm_mode_data_get(g_hdmi_phy_info.spec_enhance_cfg.rterm_mode_d1)->mode_data_d1;
        tmp |= phy_rterm_mode_data_get(g_hdmi_phy_info.spec_enhance_cfg.rterm_mode_d2)->mode_data_d2;
    } else {
        tmp = 0x1FF;
    }
    hdmi_info("RTerm:0x%x\n", tmp);
    hdmi_reg_gc_pd_rterm_set(tmp);
#endif

    return;
}

static td_void hal_hdmi_phy_reset(td_void)
{
    hdmi_if_false_return_void(g_hdmi_phy_info.init);

    hal_hdmi_phy_oe_set(TD_FALSE);
    hal_hdmi_phy_power_set(TD_FALSE);
    drv_hdmi_prod_crg_phy_reset_set(TD_TRUE);

    return;
}

/* spread spectrum ctrl(ssc) */
static td_s32 hdmi_phy_spec_set(const hdmi_phy_hw_spec_cfg *hdmi_spec_cfg)
{
#ifdef PHY_CRAFT_S28
    errno_t err_code;
    phy_hwspec_enhance_cfg  enhance_spec = {0};
    const phy_hwspec_enhance_cfg *phy_hw_enhance_spec = TD_NULL;

    phy_hw_enhance_spec =
        hdmi_spec_cfg->hdmi_phy_spec.hw_spec_debug_en ? &hdmi_spec_cfg->hdmi_phy_spec.enhance_cfg :
        phy_hw_enhance_spec_data_get(hdmi_spec_cfg->tmds_clk, hdmi_spec_cfg->trace_len);
    hdmi_if_null_return(phy_hw_enhance_spec, OT_ERR_HDMI_NULL_PTR);
    err_code = memcpy_s(&enhance_spec, sizeof(enhance_spec), phy_hw_enhance_spec, sizeof(phy_hwspec_enhance_cfg));
    hdmi_unequal_eok_return(err_code, OT_ERR_HDMI_INVALID_PARA);
    phy_user_enhance_param_set(hdmi_spec_cfg->tmds_clk, &enhance_spec);

    return phy_hw_enhance_spec_set(&enhance_spec);
#else
    return TD_SUCCESS;
#endif
}

/* for video path & debug */
static td_s32 hal_hdmi_phy_spec_set(const hdmi_phy_hw_spec_cfg *hdmi_spec_cfg)
{
#ifndef PHY_SPEC_DEBUG
    return hdmi_phy_spec_set(hdmi_spec_cfg);
#else
    hdmi_if_false_return(g_hdmi_phy_info.init, TD_FAILURE);
    hal_hdmi_phy_spec_regs_set(hdmi_spec_cfg);
    return TD_SUCCESS;
#endif
}

/* for video path & debug */
static td_void hal_hdmi_phy_ssc_set(const hdmi_phy_ssc_cfg *hdmi_ssc_cfg)
{
    errno_t ret;
    td_u32 phy_ref_clk = 0;
    td_u32 span_fb;
    td_u32 step_fb;
    td_u32 tmp;
    hdmi_deep_color dp_color;
    const phy_ssc_cfg *phy_ssc_cfg_tmp = TD_NULL;
    phy_div_cfg *div_cfg = TD_NULL;

    hdmi_if_false_return_void(g_hdmi_phy_info.init);

    dp_color = hdmi_ssc_cfg->deep_color;
    if (dp_color == HDMI_DEEP_COLOR_OFF) {
        dp_color = HDMI_DEEP_COLOR_24BIT;
    }

    /* get config para */
    div_cfg = phy_div_cfg_get(hdmi_ssc_cfg->pix_clk, dp_color);
    hdmi_if_null_return_void(div_cfg);

    /* spread spectrum ctrl(ssc) */
    if (hdmi_ssc_cfg->phy_ssc.ssc_enable) {
        phy_ssc_cfg_tmp = hdmi_ssc_cfg->phy_ssc.ssc_debug_en ? &hdmi_ssc_cfg->phy_ssc.ssc_cfg :
            phy_ssc_data_get(hdmi_ssc_cfg->tmds_clk);
        hdmi_if_null_return_void(phy_ssc_cfg_tmp);
        if ((div_cfg->ssc_byp_div == 0) || (phy_ssc_cfg_tmp->ssc_freq == 0)) {
            span_fb = 0;
            step_fb = 0;
        } else {
            /*
             *  ref_clk calculation
             *  reference clock = (pixelclk(hz) * pixclkdiv) / ssc_bypass_div
             */
            phy_ref_clk = hdmi_ssc_cfg->pix_clk * div_cfg->pixel_clk_div * HDMI_THOUSAND;
            phy_ref_clk /= div_cfg->ssc_byp_div;
            /*
             * span fb calculation
             * span = ref clock/(ssc_freqence * 4)
             */
            span_fb = (phy_ref_clk / phy_ssc_cfg_tmp->ssc_freq) / 4; /* 4, according to protocol calculation */
            /*
             * step fb calculation
             * step = (((ssc_amptd * loop_div) / 10000) * ssc_freqcence) / (ref clock / 4)
             */
            tmp = (div_cfg->loop_div / HDMI_HUNDRED) * phy_ssc_cfg_tmp->ssc_amptd;
            tmp = tmp * (phy_ssc_cfg_tmp->ssc_freq / HDMI_THOUSAND);
            step_fb = phy_ref_clk ? (tmp / (phy_ref_clk / 4)) : 0; /* 4, (ref clock / 4) is protocol requirements */
        }

        hdmi_info("ssc info || loop_div:0x%x, ref_clk:%u.\n", div_cfg->loop_div, phy_ref_clk);
        hdmi_info("         || amptd:%u, freq:%u, span:0x%x, step:0x%x.\n",
            phy_ssc_cfg_tmp->ssc_amptd, phy_ssc_cfg_tmp->ssc_freq, span_fb, step_fb);

        hal_hdmi_phy_ssc_on(span_fb, step_fb);
    } else {
        hal_hdmi_phy_ssc_off(div_cfg->deep_color);
    }

    /* status info set */
    ret = memcpy_s(&g_hdmi_phy_info.ssc_cfg, sizeof(g_hdmi_phy_info.ssc_cfg),
        &hdmi_ssc_cfg->phy_ssc, sizeof(hdmi_phy_ssc));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return;
    }

    return;
}

td_s32 hal_hdmi_phy_init(const hdmi_hal_init *hal_init)
{
    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_null_return(hal_init, OT_ERR_HDMI_NULL_PTR);

    if (!g_hdmi_phy_info.init) {
        if (hal_init->phy_addr == TD_NULL && hal_init->base_addr != TD_NULL) {
            hdmi_reg_tx_phy_init((hal_init->base_addr + HDMI_TX_BASE_ADDR_DPHY));
        } else if (hal_init->phy_addr != TD_NULL) {
            hdmi_reg_tx_phy_init(hal_init->phy_addr);
        } else {
            hdmi_err("null pointer!\n");
            return TD_FAILURE;
        }

        /* init */
        hdmi_reg_txpll_icp_ictrl_set(PHY_I_CHARGE_PUMP);
        hdmi_reg_txpll_divsel_loop_set(PHY_PLL_LOOP_DEVIDER);
        hdmi_reg_sscin_bypass_en_set(TD_TRUE);
        hdmi_reg_pllfbmash111_en_set(TD_TRUE);
        phy_default_spec_set();
        phy_rterm_init();
        g_hdmi_phy_info.init = TD_TRUE;
    }

    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_deinit(td_void)
{
    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_false_return(g_hdmi_phy_info.init, TD_FAILURE);

    hal_hdmi_phy_reset();
    hdmi_reg_tx_phy_deinit();
    g_hdmi_phy_info.init = TD_FALSE;

    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_power_get(td_bool *enable)
{
    phy_power_cfg power_cfg = {0};

    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_false_return(g_hdmi_phy_info.init, TD_FAILURE);
    hdmi_if_null_return(enable, OT_ERR_HDMI_NULL_PTR);

    power_cfg.phy_gc_txpll_pd   = hdmi_reg_gc_txpll_pd_get();
    power_cfg.phy_gc_rxsense_pd = hdmi_reg_gc_pd_rxsense_get();
    power_cfg.phy_gc_ldo_pd     = hdmi_reg_gc_pd_ldo_get();
    power_cfg.phy_gc_de_pd      = hdmi_reg_gc_pd_de_get();
    power_cfg.phy_gc_bist_pd    = hdmi_reg_gc_pd_bist_get();

    if ((power_cfg.phy_gc_txpll_pd == PHY_POWER_ON) && (power_cfg.phy_gc_rxsense_pd == PHY_POWER_ON) &&
        (power_cfg.phy_gc_ldo_pd == PHY_POWER_ON) && power_cfg.phy_gc_bist_pd) {
        *enable = TD_TRUE;
    } else if (power_cfg.phy_gc_txpll_pd && power_cfg.phy_gc_rxsense_pd &&
        (PHY_POWER_LDO_OFF & power_cfg.phy_gc_ldo_pd) && (PHY_POWER_OFF & (td_u32)power_cfg.phy_gc_bist_pd)) {
        *enable = TD_FALSE;


#endif /* CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART05_H_ */
