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

#ifndef CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART06_H_
#define CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART06_H_

    } else {
        hdmi_warn("phy power configure is abnormal\n");
        *enable = TD_FALSE;
    }

    return TD_SUCCESS;
}

td_void hal_hdmi_phy_power_set(td_bool enable)
{
    hdmi_if_fpga_return_void();
    hdmi_if_false_return_void(g_hdmi_phy_info.init);

    if (enable) {
        hdmi_reg_gc_txpll_pd_set(PHY_POWER_ON);
        hdmi_reg_gc_pd_rxsense_set(PHY_POWER_ON);
        hdmi_reg_gc_pd_ldo_set(PHY_POWER_ON);
    } else {
        hdmi_reg_gc_txpll_pd_set(PHY_POWER_OFF);
        hdmi_reg_gc_pd_rxsense_set(PHY_POWER_OFF);
        hdmi_reg_gc_pd_ldo_set(PHY_POWER_OFF);
    }
    hdmi_reg_gc_pd_bist_set(PHY_POWER_OFF);
    g_hdmi_phy_info.power_enable = enable;

    return;
}

td_void hal_hdmi_phy_oe_set(td_bool enable)
{
    td_u32 oe;
    td_u32 de_main_power = PHY_POWER_OFF;

    hdmi_if_fpga_return_void();
    hdmi_if_false_return_void(g_hdmi_phy_info.init);
    oe = enable ? PHY_OE_ON : PHY_OE_OFF;
    /* de-emphasis must be power down. */
    if (enable) {
        if (hdmi_reg_isel_main_de_clk_get() != 0) {
            de_main_power &= PHY_POWER_ON_MASK_CLK;
        }
        if (hdmi_reg_isel_main_de_d0_get() != 0) {
            de_main_power &= PHY_POWER_ON_MASK_D0;
        }
        if (hdmi_reg_isel_main_de_d1_get() != 0) {
            de_main_power &= PHY_POWER_ON_MASK_D1;
        }
        if (hdmi_reg_isel_main_de_d2_get() != 0) {
            de_main_power &= PHY_POWER_ON_MASK_D2;
        }
    }
    hdmi_reg_gc_pd_de_set(de_main_power);
    phy_rterm_set(enable);
    hdmi_reg_gc_pd_set(oe);
    g_hdmi_phy_info.oe_enable = enable;

    return;
}

td_s32 hal_hdmi_phy_oe_get(td_bool *enable)
{
    td_bool phy_reset = TD_FALSE;

    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_false_return(g_hdmi_phy_info.init, TD_FAILURE);
    hdmi_if_null_return(enable, OT_ERR_HDMI_NULL_PTR);

    drv_hdmi_prod_crg_phy_reset_get(&phy_reset);
    if (phy_reset == TD_FALSE) {
        *enable = (hdmi_reg_gc_pd_get() == PHY_OE_ON) ? TD_TRUE : TD_FALSE;
    } else {
        *enable = TD_FALSE;
    }

    return TD_SUCCESS;
}

/* for video path */
td_s32 hal_hdmi_phy_tmds_set(const hdmi_phy_tmds_cfg *hdmi_tmds_cfg)
{
    errno_t ret;
    phy_div_cfg         *div_cfg = TD_NULL;
    hdmi_deep_color      dp_color;
    hdmi_crg_cfg         crg_cfg = {0};
    hdmi_phy_ssc_cfg     hdmi_ssc_cfg = {0};
    hdmi_phy_hw_spec_cfg hdmi_hw_spec_cfg = {0};

    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_false_return(g_hdmi_phy_info.init, TD_FAILURE);
    hdmi_if_null_return(hdmi_tmds_cfg, OT_ERR_HDMI_NULL_PTR);

    dp_color = hdmi_tmds_cfg->deep_color;
    if (dp_color == HDMI_DEEP_COLOR_OFF) {
        dp_color = HDMI_DEEP_COLOR_24BIT;
    }

    /* get config para */
    div_cfg = phy_div_cfg_get(hdmi_tmds_cfg->pixel_clk, dp_color);
    hdmi_if_null_return(div_cfg, OT_ERR_HDMI_NULL_PTR);

    hal_hdmi_phy_power_set(TD_TRUE);
    /* set phy div mode */
    phy_div_mode_set(div_cfg->div_mode);
    /* set crg div */
    crg_cfg.ssc_bypass_div = div_cfg->ssc_byp_div;
    crg_cfg.tmds_clk_div = div_cfg->tmds_nxdiv;
    drv_hdmi_prod_crg_div_set(&crg_cfg);
    /* must delay 500us after CRG register config finished. */
    osal_msleep(1);
    /* set clk ratio */
    phy_clk_ratio_set(div_cfg->clk_ratio);
    /* set loopdiv */
    phy_loop_div_set(div_cfg->loop_div);
    /* spread spectrum ctrl(ssc) */
    hdmi_ssc_cfg.pix_clk    = hdmi_tmds_cfg->pixel_clk;
    hdmi_ssc_cfg.tmds_clk   = hdmi_tmds_cfg->tmds_clk;
    hdmi_ssc_cfg.deep_color = dp_color;
    hdmi_ssc_cfg.phy_ssc.ssc_enable   = hdmi_tmds_cfg->emi_enable;
    hdmi_ssc_cfg.phy_ssc.ssc_debug_en = TD_FALSE;
    hal_hdmi_phy_ssc_set(&hdmi_ssc_cfg);

    /* set hard_ware specification */
    hdmi_hw_spec_cfg.tmds_clk = hdmi_tmds_cfg->tmds_clk;
    hdmi_hw_spec_cfg.trace_len = hdmi_tmds_cfg->trace_len;
    hdmi_hw_spec_cfg.hdmi_phy_spec.hw_spec_debug_en = TD_FALSE;
    ret = hal_hdmi_phy_spec_set(&hdmi_hw_spec_cfg);
    if (ret != TD_SUCCESS) {
        hdmi_err("spec set fail.\n");
        return ret;
    }
    /* status info set */
    ret = memcpy_s(&g_hdmi_phy_info.tmds_cfg, sizeof(g_hdmi_phy_info.tmds_cfg),
        hdmi_tmds_cfg, sizeof(hdmi_phy_tmds_cfg));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_ssc_get(td_bool *enable)
{
    td_u32 span_fb;
    td_u32 step_fb;

    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_null_return(enable, OT_ERR_HDMI_NULL_PTR);

    span_fb = hdmi_reg_step_fb_get();
    step_fb = hdmi_reg_span_fb_get();
    *enable = ((span_fb == 0) && (step_fb == 0)) ? TD_FALSE : TD_TRUE;

    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_info_get(hdmi_phy_info *phy_status)
{
    errno_t ret;

    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_null_return(phy_status, OT_ERR_HDMI_NULL_PTR);

    ret = memcpy_s(phy_status, sizeof(*phy_status), &g_hdmi_phy_info, sizeof(hdmi_phy_info));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    return TD_SUCCESS;
}

td_s32 hal_hdmi_phy_params_set(td_u32 tmds_clk, const hdmi_hw_spec *hw_spec)
{
    errno_t ret;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_phy_hw_spec_cfg hw_spec_cfg = {0};

    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_false_return(g_hdmi_phy_info.init, OT_ERR_HDMI_INVALID_PARA);
    hdmi_if_null_return(hw_spec, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(HDMI_DEVICE_ID0);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);

    ret = memcpy_s(&g_hdmi_phy_info.spec_user.hw_param[0], sizeof(g_hdmi_phy_info.spec_user.hw_param),
        &hw_spec->hw_param[0], sizeof(hdmi_hw_spec));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    ret = memcpy_s(&g_hdmi_phy_info.hw_spec.hw_param[0], sizeof(g_hdmi_phy_info.hw_spec.hw_param),
        &hw_spec->hw_param[0], sizeof(hdmi_hw_spec));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    hw_spec_cfg.tmds_clk = tmds_clk;
    hw_spec_cfg.hdmi_phy_spec.hw_spec_debug_en = TD_FALSE;
    hw_spec_cfg.trace_len = hdmi_dev->mode_param.trace_len;
    ret = hal_hdmi_phy_spec_set(&hw_spec_cfg);

    return ret;
}

td_s32 hal_hdmi_phy_params_get(hdmi_phy_hw_param *hw_param)
{
    errno_t ret;

    hdmi_if_fpga_return(TD_SUCCESS);
    hdmi_if_false_return(g_hdmi_phy_info.init, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(hw_param, OT_ERR_HDMI_NULL_PTR);

    phy_def_hw_param_get(&hw_param->hw_spec_def);

    ret = memcpy_s(&hw_param->spec_drv_use, sizeof(hw_param->spec_drv_use),
        &g_hdmi_phy_info.hw_spec, sizeof(g_hdmi_phy_info.hw_spec));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    ret = memcpy_s(&hw_param->spec_user_set, sizeof(hw_param->spec_user_set),
        &g_hdmi_phy_info.spec_user, sizeof(g_hdmi_phy_info.spec_user));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    phy_cur_hw_param_get(&hw_param->hw_param_cur);

    return TD_SUCCESS;
}


#endif /* CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART06_H_ */
