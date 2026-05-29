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

#ifndef CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART04_H_
#define CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART04_H_

#ifndef PHY_SPEC_DEBUG
static td_u32 phy_transfer_cur2hex(td_u32 cur)
{
    td_u32 tmp;

    if (cur < PHY_HWSPEC_I_16) {
        tmp = cur;
    } else if (cur < PHY_HWSPEC_I_28) {
        tmp = (cur + 0x04);
    } else if (cur < PHY_HWSPEC_I_32) {
        tmp = (cur + 0x10);
    } else if (cur <= PHY_HWSPEC_I_MAX) {
        tmp = (cur + 0x14);
    } else {
        hdmi_err("%u is exceed max value %u.\n", cur, PHY_HWSPEC_I_MAX);
        tmp = 0;
    }

    return tmp;
}

#ifdef PHY_CRAFT_S28
static td_s32 phy_hw_enhance_spec_set(const phy_hwspec_enhance_cfg *hw_spec_cfg)
{
    errno_t ret;
    td_u32 tmp;

    hdmi_info("PHY HWSpec DeIMain     clk:%u, d0:%u, d1:%u, d2:%u\n",
              hw_spec_cfg->i_demain_clk, hw_spec_cfg->i_demain_d0, hw_spec_cfg->i_demain_d1, hw_spec_cfg->i_demain_d2);
    hdmi_info("PHY HWSpec iMain       clk:%u, d0:%u, d1:%u, d2:%u\n",
              hw_spec_cfg->i_main_clk, hw_spec_cfg->i_main_d0, hw_spec_cfg->i_main_d1, hw_spec_cfg->i_main_d2);
    hdmi_info("PHY HWSpec bFeedThruen clk:%u, d0:%u, d1:%u, d2:%u\n",
              hw_spec_cfg->feedthru_cap_clk_en, hw_spec_cfg->feedthru_cap_d0_en,
              hw_spec_cfg->feedthru_cap_d1_en, hw_spec_cfg->feedthru_cap_d2_en);
    hdmi_info("PHY HWSpec enFeedThru  clk:%u, d0:%u, d1:%u, d2:%u\n",
              hw_spec_cfg->feedthru_cap_delay_clk, hw_spec_cfg->feedthru_cap_delay_d0,
              hw_spec_cfg->feedthru_cap_delay_d1, hw_spec_cfg->feedthru_cap_delay_d2);
    hdmi_info("PHY HWSpec RTermMode   clk:%u, d0:%u, d1:%u, d2:%u\n",
              hw_spec_cfg->rterm_mode_clk, hw_spec_cfg->rterm_mode_d0,
              hw_spec_cfg->rterm_mode_d1, hw_spec_cfg->rterm_mode_d2);
    hdmi_info("PHY HWSpec RTerm       clk:0x%x, d0:0x%x, d1:0x%x, d2:0x%x\n",
              hw_spec_cfg->rterm_clk, hw_spec_cfg->rterm_d0, hw_spec_cfg->rterm_d1, hw_spec_cfg->rterm_d2);

    /* de iMain */
    hdmi_reg_isel_main_de_clk_set(phy_transfer_cur2hex(hw_spec_cfg->i_demain_clk));
    hdmi_reg_isel_main_de_d0_set(phy_transfer_cur2hex(hw_spec_cfg->i_demain_d0));
    hdmi_reg_isel_main_de_d1_set(phy_transfer_cur2hex(hw_spec_cfg->i_demain_d1));
    hdmi_reg_isel_main_de_d2_set(phy_transfer_cur2hex(hw_spec_cfg->i_demain_d2));
    /* iMain */
    hdmi_reg_isel_main_clk_set(phy_transfer_cur2hex(hw_spec_cfg->i_main_clk));
    hdmi_reg_isel_main_d0_set(phy_transfer_cur2hex(hw_spec_cfg->i_main_d0));
    hdmi_reg_isel_main_d1_set(phy_transfer_cur2hex(hw_spec_cfg->i_main_d1));
    hdmi_reg_isel_main_d2_set(phy_transfer_cur2hex(hw_spec_cfg->i_main_d2));
    /* Feed-Through Cap Delay control */
    tmp = hw_spec_cfg->feedthru_cap_delay_clk;
    tmp = hw_spec_cfg->feedthru_cap_clk_en ? (tmp | PHY_FEEDTHRU_EN_MASK) : (tmp & (~PHY_FEEDTHRU_EN_MASK));
    hdmi_reg_rsel_pre_clk_set(tmp);
    tmp = hw_spec_cfg->feedthru_cap_delay_d0;
    tmp = hw_spec_cfg->feedthru_cap_d0_en ? (tmp | PHY_FEEDTHRU_EN_MASK) : (tmp & (~PHY_FEEDTHRU_EN_MASK));
    hdmi_reg_rsel_pre_d0_set(tmp);
    tmp = hw_spec_cfg->feedthru_cap_delay_d1;
    tmp = hw_spec_cfg->feedthru_cap_d1_en ? (tmp | PHY_FEEDTHRU_EN_MASK) : (tmp & (~PHY_FEEDTHRU_EN_MASK));
    hdmi_reg_rsel_pre_d1_set(tmp);
    tmp = hw_spec_cfg->feedthru_cap_delay_d2;
    tmp = hw_spec_cfg->feedthru_cap_d2_en ? (tmp | PHY_FEEDTHRU_EN_MASK) : (tmp & (~PHY_FEEDTHRU_EN_MASK));
    hdmi_reg_rsel_pre_d2_set(tmp);
    /* R Term */
    hdmi_reg_rt_clk_set(hw_spec_cfg->rterm_clk);
    hdmi_reg_rt_d0_set(hw_spec_cfg->rterm_d0);
    hdmi_reg_rt_d1_set(hw_spec_cfg->rterm_d1);
    hdmi_reg_rt_d2_set(hw_spec_cfg->rterm_d2);
    /* VCM sel */
    hdmi_reg_test_set(hw_spec_cfg->vcm_sel);

    /* status info set */
    ret = memcpy_s(&g_hdmi_phy_info.spec_enhance_cfg, sizeof(g_hdmi_phy_info.spec_enhance_cfg),
        hw_spec_cfg, sizeof(phy_hwspec_enhance_cfg));
    hdmi_unequal_eok_return(ret, OT_ERR_HDMI_INVALID_PARA);

    return TD_SUCCESS;
}

static td_void trace_printf(hdmi_trace_len trace_len)
{
    switch (trace_len) {
        case HDMI_TRACE_LEN_0:
            hdmi_info("1.0 inch\n");
            break;
        case HDMI_TRACE_LEN_1:
            hdmi_info("1.5 inch\n");
            break;
        case HDMI_TRACE_LEN_2:
            hdmi_info("2.0 inch\n");
            break;
        case HDMI_TRACE_LEN_3:
            hdmi_info("2.5 inch\n");
            break;
        case HDMI_TRACE_LEN_4:
            hdmi_info("3.0 inch\n");
            break;
        case HDMI_TRACE_LEN_5:
            hdmi_info("3.5 inch\n");
            break;
        case HDMI_TRACE_LEN_6:
            hdmi_info("4.0 inch\n");
            break;
        case HDMI_TRACE_LEN_7:
            hdmi_info("4.5 inch\n");
            break;
        case HDMI_TRACE_LEN_8:
            hdmi_info("5.0 inch\n");
            break;
        default:
            hdmi_info("default inch\n");
            break;
    }
}

static td_u32 phy_hw_enhance_spec_default(phy_hwspec_enhance **hwspec_enhance, hdmi_trace_len trace_len)
{
    td_u32 size;
    phy_hwspec_enhance *hwspec = TD_NULL;

    /* size is the same frome 1p0 to 5p0 */
    size = hdmi_array_size(g_phy_hw_spec_enhance);
    switch (trace_len) {
        case HDMI_TRACE_LEN_0:
            hwspec = &g_phy_hw_spec_enhance_1p0[0];
            break;
        case HDMI_TRACE_LEN_1:
            hwspec = &g_phy_hw_spec_enhance_1p5[0];
            break;
        case HDMI_TRACE_LEN_2:
            hwspec = &g_phy_hw_spec_enhance_2p0[0];
            break;
        case HDMI_TRACE_LEN_3:
            hwspec = &g_phy_hw_spec_enhance_2p5[0];
            break;
        case HDMI_TRACE_LEN_4:
            hwspec = &g_phy_hw_spec_enhance_3p0[0];
            break;
        case HDMI_TRACE_LEN_5:
            hwspec = &g_phy_hw_spec_enhance_3p5[0];
            break;
        case HDMI_TRACE_LEN_6:
            hwspec = &g_phy_hw_spec_enhance_4p0[0];
            break;
        case HDMI_TRACE_LEN_7:
            hwspec = &g_phy_hw_spec_enhance_4p5[0];
            break;
        case HDMI_TRACE_LEN_8:
            hwspec = &g_phy_hw_spec_enhance_5p0[0];
            break;
        case HDMI_TRACE_DEFAULT:
        default:
            hwspec = &g_phy_hw_spec_enhance[0];
            break;
    }

    *hwspec_enhance = hwspec;

    return size;
}

static td_u32 phy_hw_enhance_spec_otp(phy_hwspec_enhance **hwspec_enhance, hdmi_trace_len trace_len, td_u32 otp)
{
    td_u32 size = 0;
#if defined(CHIP_TYPE_ss528v100) || defined(CHIP_TYPE_ss625v100)
    phy_hwspec_enhance *hwspec = TD_NULL;
    td_u32 index = otp - 1;

    /* size is the same frome 1p0 to 5p0 */
    size = hdmi_array_size(g_phy_hw_spec_enhance_1p0_otp[0]);
    switch (trace_len) {
        case HDMI_TRACE_LEN_0:
            hwspec = &g_phy_hw_spec_enhance_1p0_otp[index][0];
            break;
        case HDMI_TRACE_LEN_1:
            hwspec = &g_phy_hw_spec_enhance_1p5_otp[index][0];
            break;
        case HDMI_TRACE_LEN_2:
            hwspec = &g_phy_hw_spec_enhance_2p0_otp[index][0];
            break;
        case HDMI_TRACE_LEN_3:
            hwspec = &g_phy_hw_spec_enhance_2p5_otp[index][0];
            break;
        case HDMI_TRACE_LEN_4:
            hwspec = &g_phy_hw_spec_enhance_3p0_otp[index][0];
            break;
        case HDMI_TRACE_LEN_5:
            hwspec = &g_phy_hw_spec_enhance_3p5_otp[index][0];
            break;
        case HDMI_TRACE_LEN_6:
            hwspec = &g_phy_hw_spec_enhance_4p0_otp[index][0];
            break;
        case HDMI_TRACE_LEN_7:
            hwspec = &g_phy_hw_spec_enhance_4p5_otp[index][0];
            break;
        case HDMI_TRACE_LEN_8:
            hwspec = &g_phy_hw_spec_enhance_5p0_otp[index][0];
            break;
        case HDMI_TRACE_DEFAULT:
        default:
#if defined(CHIP_TYPE_ss528v100)
            hwspec = &g_phy_hw_spec_enhance_otp[index][0];
#endif
            break;
    }
    *hwspec_enhance = hwspec;
#endif

    return size;
}

static td_u32 phy_get_otp_value(td_void)
{
    td_u32 val = 0;

#if defined(CHIP_TYPE_ss528v100)
    val = hdmi_reg_read_u32(HDMI_OPT_FLAG_ADDR) & 0x3;
#endif

    return val;
}

static td_u32 phy_hw_enhance_spec_debug_data_get(phy_hwspec_enhance **hwspec_enhance, hdmi_trace_len trace_len)
{
    td_u32 size = 0;
    td_u32 otp = 0;

    trace_printf(trace_len);

#if defined(CHIP_TYPE_ss528v100)
    /* otp: 0 = default, 1 - opt_1 phy config, 2 - opt_2 phy config */
    otp = phy_get_otp_value();
#endif
    if (otp == 0) { /* 0 - normal phy config */
        size = phy_hw_enhance_spec_default(hwspec_enhance, trace_len);
    } else if (otp == 1 || otp == 2) { /* 1 - opt_1 phy config, 2 - opt_2 phy config */
        size = phy_hw_enhance_spec_otp(hwspec_enhance, trace_len, otp);
    }  else {
        hdmi_err("otp %u is invalid, valid range [0 , 2].\n", otp);
    }

    return size;
}

static phy_hwspec_enhance_cfg *phy_hw_enhance_spec_data_get(td_u32 tmds_clk, hdmi_trace_len trace_len)
{
    td_u32 i;
    td_u32 array_size;
    phy_hwspec_enhance *phy_hw_enhance_spec = TD_NULL;

    array_size = phy_hw_enhance_spec_debug_data_get(&phy_hw_enhance_spec, trace_len);

    /* updata g_hdmi_phy_info just in change trace len. */
    if (phy_hw_enhance_spec != TD_NULL  && trace_len != g_hdmi_phy_info.old_trace_len) {
        g_hdmi_phy_info.old_trace_len = trace_len;
        for (i = 0; i < HDMI_HW_PARAM_NUM; i++) {
            g_hdmi_phy_info.hw_spec.hw_param[i].i_de_main_clk  = phy_hw_enhance_spec[i].cfg.i_demain_clk;
            g_hdmi_phy_info.hw_spec.hw_param[i].i_de_main_data = phy_hw_enhance_spec[i].cfg.i_demain_d0;
            g_hdmi_phy_info.hw_spec.hw_param[i].i_main_clk     = phy_hw_enhance_spec[i].cfg.i_main_clk;
            g_hdmi_phy_info.hw_spec.hw_param[i].i_main_data    = phy_hw_enhance_spec[i].cfg.i_main_d0;
            g_hdmi_phy_info.hw_spec.hw_param[i].ft_cap_clk     = phy_hw_enhance_spec[i].cfg.feedthru_cap_clk_en ?
                (phy_hw_enhance_spec[i].cfg.feedthru_cap_delay_clk + 1) : 0;
            g_hdmi_phy_info.hw_spec.hw_param[i].ft_cap_data    = phy_hw_enhance_spec[i].cfg.feedthru_cap_d0_en ?
                (phy_hw_enhance_spec[i].cfg.feedthru_cap_delay_d0 + 1) : 0;

            hdmi_info("stage[%u]: i_de_main_clk:%u i_de_main_data:%u\
                i_main_clk:%u i_main_data:%u ft_cap_clk:%u ft_cap_data:%u\n", i,
                g_hdmi_phy_info.hw_spec.hw_param[i].i_de_main_clk, g_hdmi_phy_info.hw_spec.hw_param[i].i_de_main_data,
                g_hdmi_phy_info.hw_spec.hw_param[i].i_main_clk, g_hdmi_phy_info.hw_spec.hw_param[i].i_main_data,
                g_hdmi_phy_info.hw_spec.hw_param[i].ft_cap_clk, g_hdmi_phy_info.hw_spec.hw_param[i].ft_cap_data);
        }
        /* clean user cfg */
        (td_void)memset_s(g_hdmi_phy_info.spec_user.hw_param,
            sizeof(g_hdmi_phy_info.spec_user.hw_param), 0, sizeof(g_hdmi_phy_info.spec_user.hw_param));
    }

    for (i = 0; ((phy_hw_enhance_spec != TD_NULL) && (i < array_size)); phy_hw_enhance_spec++, i++) {
        if ((tmds_clk >= phy_hw_enhance_spec->range.clk_min) && (tmds_clk <= phy_hw_enhance_spec->range.clk_max)) {
            return (&phy_hw_enhance_spec->cfg);
        }
    }

    return TD_NULL;
}

static td_void phy_feedthru_en(phy_hwspec_enhance_cfg *phy_spec_cfg, const hdmi_hw_param *hw_param)
{
    if (hw_param->ft_cap_clk != 0) {
        phy_spec_cfg->feedthru_cap_clk_en = TD_TRUE;
    } else {
        phy_spec_cfg->feedthru_cap_clk_en = TD_FALSE;
    }

    if (hw_param->ft_cap_data != 0) {
        phy_spec_cfg->feedthru_cap_d0_en = TD_TRUE;
        phy_spec_cfg->feedthru_cap_d1_en = TD_TRUE;
        phy_spec_cfg->feedthru_cap_d2_en = TD_TRUE;
    } else {
        phy_spec_cfg->feedthru_cap_d0_en = TD_FALSE;
        phy_spec_cfg->feedthru_cap_d1_en = TD_FALSE;
        phy_spec_cfg->feedthru_cap_d2_en = TD_FALSE;
    }

    return;
}

static td_void phy_user_enhance_param_set(td_u32 tmds_clk, phy_hwspec_enhance_cfg *phy_spec_cfg)
{
    td_u8 i;
    phy_hwspec_enhance *hw_spec = TD_NULL;

    for (i = 0, hw_spec = &g_phy_hw_spec_enhance[0];
         (hw_spec != TD_NULL && (i < hdmi_array_size(g_phy_hw_spec_enhance)));
         hw_spec++, i++) {
        if ((tmds_clk >= hw_spec->range.clk_min) && (tmds_clk <= hw_spec->range.clk_max)) {
            phy_spec_cfg->i_demain_clk = g_hdmi_phy_info.hw_spec.hw_param[i].i_de_main_clk;
            if (phy_spec_cfg->i_demain_d0 != g_hdmi_phy_info.hw_spec.hw_param[i].i_de_main_data) {
                phy_spec_cfg->i_demain_d0  = g_hdmi_phy_info.hw_spec.hw_param[i].i_de_main_data;
                phy_spec_cfg->i_demain_d1  = g_hdmi_phy_info.hw_spec.hw_param[i].i_de_main_data;
                phy_spec_cfg->i_demain_d2  = g_hdmi_phy_info.hw_spec.hw_param[i].i_de_main_data;
            }
            phy_spec_cfg->i_main_clk = g_hdmi_phy_info.hw_spec.hw_param[i].i_main_clk;
            if (phy_spec_cfg->i_main_d0 != g_hdmi_phy_info.hw_spec.hw_param[i].i_main_data) {
                phy_spec_cfg->i_main_d0  = g_hdmi_phy_info.hw_spec.hw_param[i].i_main_data;
                phy_spec_cfg->i_main_d1  = g_hdmi_phy_info.hw_spec.hw_param[i].i_main_data;
                phy_spec_cfg->i_main_d2  = g_hdmi_phy_info.hw_spec.hw_param[i].i_main_data;
            }
            if (g_hdmi_phy_info.hw_spec.hw_param[i].ft_cap_clk > 0) {
                phy_spec_cfg->feedthru_cap_delay_clk = g_hdmi_phy_info.hw_spec.hw_param[i].ft_cap_clk - 1;
            }
            if ((g_hdmi_phy_info.hw_spec.hw_param[i].ft_cap_data > 0) &&
                (phy_spec_cfg->feedthru_cap_delay_d0 != (g_hdmi_phy_info.hw_spec.hw_param[i].ft_cap_data - 1))) {
                phy_spec_cfg->feedthru_cap_delay_d0  = g_hdmi_phy_info.hw_spec.hw_param[i].ft_cap_data - 1;
                phy_spec_cfg->feedthru_cap_delay_d1  = g_hdmi_phy_info.hw_spec.hw_param[i].ft_cap_data - 1;
                phy_spec_cfg->feedthru_cap_delay_d2  = g_hdmi_phy_info.hw_spec.hw_param[i].ft_cap_data - 1;
            }
            phy_feedthru_en(phy_spec_cfg, &g_hdmi_phy_info.hw_spec.hw_param[i]);

            break;
        }
    }

    return;
}
#endif

#else
static td_void hal_hdmi_phy_spec_regs_set(const hdmi_phy_hw_spec_cfg *hdmi_spec_cfg)
{
    td_u32 tmp;

    if ((hdmi_spec_cfg->tmds_clk >= TMDS_CLOCK_25M) && (hdmi_spec_cfg->tmds_clk <= TMDS_CLOCK_100M)) {
        hdmi_reg_write_u32(0x11400068, 0x00861865);
        hdmi_reg_write_u32(0x1140006c, 0x18180000);
        hdmi_reg_write_u32(0x11400070, 0x00000618);
        hdmi_reg_write_u32(0x11400074, 0x0001B6C0);
        hdmi_reg_write_u32(0x11400078, 0x20202000);
        tmp = hdmi_reg_read_u32(0x11400064);
        tmp |= 0xfd << 11; /* 11'b, BIT[19:12] */
        hdmi_reg_write_u32(0x11400064, tmp);
    } else if ((hdmi_spec_cfg->tmds_clk > TMDS_CLOCK_100M) && (hdmi_spec_cfg->tmds_clk <= TMDS_CLOCK_165M)) {
        hdmi_reg_write_u32(0x11400068, 0x00C71C65);
        hdmi_reg_write_u32(0x1140006c, 0x18600000);
        hdmi_reg_write_u32(0x11400070, 0x00000618);
        hdmi_reg_write_u32(0x11400074, 0x0001B780);
        hdmi_reg_write_u32(0x11400078, 0x20202000);
        tmp = hdmi_reg_read_u32(0x11400064);
        tmp |= 0xfd << 11; /* 11'b, BIT[19:12] */
        hdmi_reg_write_u32(0x11400064, tmp);
    } else if ((hdmi_spec_cfg->tmds_clk > TMDS_CLOCK_165M) && (hdmi_spec_cfg->tmds_clk <= TMDS_CLOCK_340M)) {
        hdmi_reg_write_u32(0x11400068, 0x00C71C65);
        hdmi_reg_write_u32(0x1140006c, 0x18600000);
        hdmi_reg_write_u32(0x11400070, 0x00000618);
        hdmi_reg_write_u32(0x11400074, 0x0001B6C0);
        hdmi_reg_write_u32(0x11400078, 0x60606000);
        tmp = hdmi_reg_read_u32(0x11400064);
        tmp |= 0x55 << 11; /* 11'b, BIT[19:12] */
        hdmi_reg_write_u32(0x11400064, tmp);
    }

    return;
}
#endif

static phy_ssc_cfg *phy_ssc_data_get(td_u32 tmds_clk)
{
    td_u32 i;
    phy_ssc *ssc_cfg = TD_NULL;

    for (i = 0, ssc_cfg = &g_phy_ssc_cfg[0]; (ssc_cfg && (i < hdmi_array_size(g_phy_ssc_cfg))); ssc_cfg++, i++) {
        if ((tmds_clk >= ssc_cfg->phy_tmds_clk_range.clk_min) && (tmds_clk <= ssc_cfg->phy_tmds_clk_range.clk_max)) {
            return (&ssc_cfg->ssc_cfg);
        }
    }

    return TD_NULL;
}

static td_void phy_div_mode_set(phy_div_mode div_mode)
{
    hdmi_reg_divsel_set(div_mode);
    return;
}

static td_void phy_loop_div_set(td_u32 loop_div)
{
    hdmi_reg_fb_set(loop_div);
    return;
}

static td_void phy_clk_ratio_set(phy_clk_ratio clk_ratio)
{
    td_u32 phy_fifo_ck_l = 0;  // 30bit
    td_u32 phy_fifo_ck_h = 0;  // 10bit

    switch (clk_ratio) {
        case PHY_CLK_RATIO_1_10:
            phy_fifo_ck_l = PHY_RATIO_1_10_MASK;
            phy_fifo_ck_l <<= 10; /* 10'b, BIT[15:11] */
            phy_fifo_ck_l |= PHY_RATIO_1_10_MASK;
            phy_fifo_ck_l <<= 10; /* 10'b, BIT[25:11] */
            phy_fifo_ck_l |= PHY_RATIO_1_10_MASK;
            phy_fifo_ck_h = PHY_RATIO_1_10_MASK;
            break;
        case PHY_CLK_RATIO_1_40:
            phy_fifo_ck_l = PHY_RATIO_1_40_MASK;
            phy_fifo_ck_h = 0;


#endif /* CBB_HDMI_SRC_MKP_HAL_PHY_V100_HDMI_HAL_PHY_BODY_PART04_H_ */
