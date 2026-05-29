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

#ifndef SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART04_H_
#define SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART04_H_

    if (ao_attr->sound_intf != HDMI_AUDIO_INTF_I2S &&
        ao_attr->sound_intf != HDMI_AUDIO_INTF_SPDIF &&
        ao_attr->sound_intf != HDMI_AUDIO_INTF_HBRA) {
        hdmi_err("the audio interface(%u) is invalid\n", ao_attr->sound_intf);
        return TD_FAILURE;
    }

    if (ao_attr->channels < HDMI_AUDIO_FORMAT_2CH || ao_attr->channels > HDMI_AUDIO_FORMAT_8CH) {
        hdmi_err("the audio channel number(%u) is invalid\n", ao_attr->channels);
        return TD_FAILURE;
    }

    if (ao_attr->sample_fs < HDMI_SAMPLE_RATE_32K || ao_attr->sample_fs > HDMI_SAMPLE_RATE_768K) {
        hdmi_err("the input audio frequency(%u) is invalid\n", ao_attr->sample_fs);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_bool hdmi_chip_id(hdmi_device_id hdmi_id)
{
    td_bool ret = TD_TRUE;
#if defined(HDMI_PRODUCT_SS626V100)
    /* The ss623v100 chip supports only hdmi0 port output.  */
    if ((chip_is_ss623v100() == TD_TRUE) && (hdmi_id != 0)) {
        hdmi_err("lite : hdmi%d is not supported!\n", hdmi_id);
        ret = TD_FALSE;
    }
#endif
    return ret;
}

static td_s32 drv_hdmi_ao_attr_set(hdmi_device *hdmi_dev, const hdmi_ao_attr *ao_attr)
{
    errno_t err_code;
    td_s32 ret = TD_SUCCESS;
    hdmi_attr hw_attr = {0};
    hdmi_audio_config audio_cfg  = {0};
    hdmi_app_attr *app_attr = TD_NULL;
    hdmi_ao_attr *audio_attr = TD_NULL;
    hdmi_vo_attr *video_attr = TD_NULL;

    hdmi_info("in\n");

    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);
    if (check_audio_attr(ao_attr) != TD_SUCCESS) {
        hdmi_err("check_audio_attr fail\n");
        return OT_ERR_HDMI_INVALID_PARA;
    }
    app_attr   = &hdmi_dev->attr.app_attr;
    video_attr = &hdmi_dev->attr.vo_attr;
    audio_attr = &hdmi_dev->attr.ao_attr;

    err_code = memcpy_s(audio_attr, sizeof(hdmi_dev->attr.ao_attr), ao_attr, sizeof(hdmi_ao_attr));
    if (err_code != EOK) {
        hdmi_err("secure function error:%d\n", err_code);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    hdmi_attr_construct(hdmi_dev, &hw_attr);

    hdmi_info("down_sample: %u\n", audio_attr->down_sample);
    hdmi_info("sample_fs: %u\n", audio_attr->sample_fs);
    hdmi_info("channels: %u\n", audio_attr->channels);
    hdmi_info("sound_intf: %u\n", audio_attr->sound_intf);
    hdmi_info("sample_depth: %u\n", audio_attr->sample_depth);
    hdmi_info("audio_code: %u\n", audio_attr->audio_code);

    audio_cfg.down_sample  = ao_attr->down_sample;
    audio_cfg.layout       = ao_attr->channels;
    audio_cfg.sample_depth = ao_attr->sample_depth;
    audio_cfg.sample_fs    = ao_attr->sample_fs;
    audio_cfg.sound_intf   = ao_attr->sound_intf;
    audio_cfg.enable_audio = (hdmi_dev->attr.app_attr.enable_audio) && (app_attr->enable_hdmi);
    audio_cfg.tmds_clk     = video_attr->hdmi_adapt_pix_clk;
    audio_cfg.pixel_repeat = video_attr->pixel_repeat;
    hal_call_void(hdmi_dev, hal_hdmi_audio_path_enable_set, TD_FALSE);
    hal_call_ret(hdmi_dev, ret, hal_hdmi_audio_path_set, &audio_cfg);
    drv_hdmi_audio_infoframe_send(&hdmi_dev->info_frame, (app_attr->enable_audio && app_attr->enable_aud_infoframe));
    hal_call_void(hdmi_dev, hal_hdmi_audio_path_enable_set, audio_cfg.enable_audio);
    hdmi_info("out\n");

    return ret;
}

static td_s32 check_video_attr(const hdmi_vo_attr *vo_attr)
{
    if (vo_attr->video_timing >= HDMI_VIDEO_TIMING_BUTT) {
        hdmi_err("video timing(%u) is wrong\n", vo_attr->video_timing);
        return TD_FAILURE;
    }

    if (vo_attr->in_color_space >= HDMI_COLORSPACE_BUTT) {
        hdmi_err("video in_color_space(%u) is wrong\n", vo_attr->in_color_space);
        return TD_FAILURE;
    }

    if (vo_attr->colorimetry != HDMI_COLORIMETRY_ITU_601 &&
        vo_attr->colorimetry != HDMI_COLORIMETRY_ITU_709 &&
        vo_attr->colorimetry != HDMI_COLORIMETRY_EXTEND) {
        hdmi_err("video colorimetry(%u) is wrong\n", vo_attr->colorimetry);
        return TD_FAILURE;
    }

    if (vo_attr->rgb_quantization > HDMI_QUANTIZATION_RANGE_FULL) {
        hdmi_err("video rgb_quantization(%u) is wrong\n", vo_attr->rgb_quantization);
        return TD_FAILURE;
    }

    if (vo_attr->picture_aspect != HDMI_PICTURE_ASPECT_4_3 &&
        vo_attr->picture_aspect != HDMI_PICTURE_ASPECT_16_9 &&
        vo_attr->picture_aspect != HDMI_PICTURE_ASPECT_64_27 &&
        vo_attr->picture_aspect != HDMI_PICTURE_ASPECT_256_135) {
        hdmi_err("video picture_aspect(%u) is wrong\n", vo_attr->picture_aspect);
        return TD_FAILURE;
    }

    if (vo_attr->in_bit_depth != HDMI_VIDEO_BITDEPTH_8 &&
        vo_attr->in_bit_depth != HDMI_VIDEO_BITDEPTH_10 &&
        vo_attr->in_bit_depth != HDMI_VIDEO_BITDEPTH_12) {
        hdmi_err("video in_bit_depth(%u) is wrong\n", vo_attr->in_bit_depth);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void hdmi_vo_colorimetry_cfg(const hdmi_vo_attr *vo_attr, hdmi_video_config *video_cfg)
{
    switch (vo_attr->colorimetry) {
        case HDMI_COLORIMETRY_ITU_601:
            video_cfg->conv_std = HDMI_CONV_STD_BT_601;
            break;
        case HDMI_COLORIMETRY_ITU_709:
            video_cfg->conv_std = HDMI_CONV_STD_BT_709;
            break;
        case HDMI_COLORIMETRY_EXTEND:
            if (vo_attr->extended_colorimetry == HDMI_EXTENDED_COLORIMETRY_2020_NON_CONST_LUMINOUS) {
                video_cfg->conv_std = HDMI_CONV_STD_BT_2020_NON_CONST_LUMINOUS;
            } else if (vo_attr->extended_colorimetry == HDMI_EXTENDED_COLORIMETRY_2020_CONST_LUMINOUS) {
                video_cfg->conv_std = HDMI_CONV_STD_BT_2020_CONST_LUMINOUS;
            }
            break;
        default:
            video_cfg->conv_std = HDMI_CONV_STD_BT_709;
            break;
    }

    return;
}

static td_void hdmi_attr_to_vid_cfg(hdmi_vo_attr *vo_attr, const hdmi_app_attr *app_attr, hdmi_video_config *video_cfg)
{
    video_cfg->pixel_clk =
        (app_attr->out_color_space == HDMI_COLORSPACE_YCBCR420) ? (vo_attr->clk_fs >> 1) : vo_attr->clk_fs;
    video_cfg->tmds_clk        = vo_attr->hdmi_adapt_pix_clk;
    video_cfg->in_bit_depth    = vo_attr->in_bit_depth;
    video_cfg->quantization    = vo_attr->rgb_quantization;
    video_cfg->in_color_space  = vo_attr->in_color_space;
    video_cfg->deep_color      = app_attr->deep_color_mode;
    video_cfg->out_color_space = app_attr->out_color_space;
    video_cfg->v_sync_pol      = vo_attr->v_sync_pol;
    video_cfg->h_sync_pol      = vo_attr->h_sync_pol;
    video_cfg->de_pol          = vo_attr->de_pol;

    if (video_cfg->out_color_space == HDMI_COLORSPACE_RGB) {
        vo_attr->rgb_quantization = app_attr->out_csc_quantization;
        vo_attr->ycc_quantization = (vo_attr->rgb_quantization == HDMI_QUANTIZATION_RANGE_FULL) ?
            HDMI_YCC_QUANTIZATION_RANGE_FULL : HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
    } else {
        vo_attr->ycc_quantization = (app_attr->out_csc_quantization == HDMI_QUANTIZATION_RANGE_FULL) ?
            HDMI_YCC_QUANTIZATION_RANGE_FULL : HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
        if (vo_attr->ycc_quantization == HDMI_YCC_QUANTIZATION_RANGE_FULL) {
            vo_attr->rgb_quantization = HDMI_QUANTIZATION_RANGE_FULL;
        } else {
            vo_attr->rgb_quantization = HDMI_QUANTIZATION_RANGE_DEFAULT;
        }
    }

    if (app_attr->out_color_space == HDMI_COLORSPACE_RGB) {
        video_cfg->out_csc_quantization = vo_attr->rgb_quantization;
    } else {
        video_cfg->out_csc_quantization = (vo_attr->ycc_quantization == HDMI_YCC_QUANTIZATION_RANGE_FULL) ?
            HDMI_QUANTIZATION_RANGE_FULL : HDMI_QUANTIZATION_RANGE_LIMITED;
    }
    hdmi_vo_colorimetry_cfg(vo_attr, video_cfg);

    return;
}

static td_void hdmi_phy_set(const hdmi_device *hdmi_dev, const hdmi_vo_attr *vo_attr)
{
    hdmi_phy_cfg phy_cfg  = {0};
    const hdmi_app_attr *app_attr = TD_NULL;

    hdmi_if_fpga_return_void();

    app_attr = &hdmi_dev->attr.app_attr;

    phy_cfg.pixel_clk  =
        (app_attr->out_color_space == HDMI_COLORSPACE_YCBCR420) ? (vo_attr->clk_fs >> 1) : vo_attr->clk_fs;
    phy_cfg.emi_enable = hdmi_dev->mode_param.emi_en;
    phy_cfg.tmds_clk   = vo_attr->hdmi_adapt_pix_clk;
    phy_cfg.deep_color = app_attr->deep_color_mode;
    phy_cfg.trace_len  = hdmi_dev->mode_param.trace_len;
    hal_call_void(hdmi_dev, hal_hdmi_phy_set, &phy_cfg);

    return;
}

static td_void hdmi_csc_set(const hdmi_device *hdmi_dev, hdmi_vo_attr *vo_attr)
{
    hdmi_video_config video_cfg = {0};
    const hdmi_app_attr *app_attr = TD_NULL;

    app_attr = &hdmi_dev->attr.app_attr;
    hdmi_attr_to_vid_cfg(vo_attr, app_attr, &video_cfg);
    hal_call_void(hdmi_dev, hal_hdmi_csc_param_set, &video_cfg);

    return;
}

static td_s32 hdmi_video_path_set(const hdmi_device *hdmi_dev, hdmi_vo_attr *vo_attr)
{
    td_s32 ret;
    hdmi_video_config video_cfg = {0};
    const hdmi_app_attr *app_attr = TD_NULL;

    app_attr = &hdmi_dev->attr.app_attr;
    video_cfg.emi_enable = hdmi_dev->mode_param.emi_en;

    hdmi_attr_to_vid_cfg(vo_attr, app_attr, &video_cfg);
    hal_call_ret(hdmi_dev, ret, hal_hdmi_video_path_set, &video_cfg);
    hdmi_csc_set(hdmi_dev, vo_attr);
    hdmi_phy_set(hdmi_dev, vo_attr);

    return ret;
}

static td_bool app_attr_is_changed(const hdmi_app_attr *hw_app_attr, const hdmi_app_attr *sw_app_attr)
{
    if (hw_app_attr->enable_hdmi != sw_app_attr->enable_hdmi) {
        hdmi_info("hdmi enable change, old(%u)->new(%u)\n", hw_app_attr->enable_hdmi, sw_app_attr->enable_hdmi);
        return TD_TRUE;
    }

    if (hw_app_attr->out_color_space != sw_app_attr->out_color_space) {
        hdmi_info("out color_space change,old(%u)->new(%u)\n",
            hw_app_attr->out_color_space, sw_app_attr->out_color_space);
        return TD_TRUE;
    }

    if (((sw_app_attr->deep_color_mode == HDMI_DEEP_COLOR_OFF) ||
        (sw_app_attr->deep_color_mode == HDMI_DEEP_COLOR_24BIT)) &&
        ((hw_app_attr->deep_color_mode == HDMI_DEEP_COLOR_OFF) ||
        (hw_app_attr->deep_color_mode == HDMI_DEEP_COLOR_24BIT))) {
        hdmi_info("deepcolor mode not change: %u\n", hw_app_attr->deep_color_mode);
    } else if (hw_app_attr->deep_color_mode != sw_app_attr->deep_color_mode) {
        hdmi_info("deepcolor mode change,old(%u)->new(%u)\n",
            hw_app_attr->deep_color_mode, sw_app_attr->deep_color_mode);
        return TD_TRUE;
    }

    if (hw_app_attr->xvycc_mode != sw_app_attr->xvycc_mode) {
        hdmi_info("xvycc_mode mode change,old(%u)->new(%u)\n",
            hw_app_attr->xvycc_mode, sw_app_attr->xvycc_mode);
        return TD_TRUE;
    }

    if (hw_app_attr->enable_avi_infoframe != sw_app_attr->enable_avi_infoframe) {
        hdmi_info("avi infoframe enable change, old(%u)->new(%u)\n",
            hw_app_attr->enable_avi_infoframe, sw_app_attr->enable_avi_infoframe);
        return TD_TRUE;
    }

    if (hw_app_attr->out_csc_quantization != sw_app_attr->out_csc_quantization) {
        hdmi_info("out_csc_quantization change, old(%u)->new(%u)\n",
            hw_app_attr->out_csc_quantization, sw_app_attr->out_csc_quantization);
        return TD_TRUE;
    }

    return TD_FALSE;
}

static td_bool vo_attr_is_changed(const hdmi_vo_attr *hw_vo_attr, hdmi_vo_attr *sw_vo_attr)
{
    if (hw_vo_attr->video_timing != sw_vo_attr->video_timing) {
        hdmi_info("video timing change,old(%u)->new(%u)\n", hw_vo_attr->video_timing, sw_vo_attr->video_timing);
        return TD_TRUE;
    } else {
        /*
         * if timing is change, the sync param need according to user config, or not,
         * should not change the sync param.
         */
        sw_vo_attr->v_sync_pol = hw_vo_attr->v_sync_pol;
        sw_vo_attr->h_sync_pol = hw_vo_attr->h_sync_pol;
        sw_vo_attr->de_pol     = hw_vo_attr->de_pol;
    }

    if (hw_vo_attr->in_color_space != sw_vo_attr->in_color_space) {
        hdmi_info("input colorspace change, old(%u)->new(%u)\n",
            hw_vo_attr->in_color_space, sw_vo_attr->in_color_space);
        return TD_TRUE;
    }
    if (hw_vo_attr->stereo_mode != sw_vo_attr->stereo_mode) {
        hdmi_info("3d mode change, old(%u)->new(%u)\n", hw_vo_attr->stereo_mode, sw_vo_attr->stereo_mode);
        return TD_TRUE;
    }
    if (hw_vo_attr->pixel_repeat != sw_vo_attr->pixel_repeat) {
        hdmi_info("pixel repeation change, old(%u)->new(%u)\n", hw_vo_attr->pixel_repeat, sw_vo_attr->pixel_repeat);
        return TD_TRUE;
    }
    if (hw_vo_attr->colorimetry != sw_vo_attr->colorimetry) {
        hdmi_info("colorimetry change, old(%u)->new(%u)\n", hw_vo_attr->colorimetry, sw_vo_attr->colorimetry);
        return TD_TRUE;
    }
    if (hw_vo_attr->extended_colorimetry != sw_vo_attr->extended_colorimetry) {
        hdmi_info("extended_colorimetry change, old(%u)->new(%u)\n",
            hw_vo_attr->extended_colorimetry, sw_vo_attr->extended_colorimetry);
        return TD_TRUE;
    }
    if (hw_vo_attr->rgb_quantization != sw_vo_attr->rgb_quantization) {
        hdmi_info("RGB quantization change, old(%u)->new(%u)\n",
            hw_vo_attr->rgb_quantization, sw_vo_attr->rgb_quantization);
        return TD_TRUE;
    }
    if (hw_vo_attr->ycc_quantization != sw_vo_attr->ycc_quantization) {
        hdmi_info("YCC quantization change, old(%u)->new(%u)\n",
            hw_vo_attr->ycc_quantization, sw_vo_attr->ycc_quantization);
        return TD_TRUE;
    }
    if (hw_vo_attr->picture_aspect != sw_vo_attr->picture_aspect) {
        hdmi_info("picture aspect change, old(%u)->new(%u)\n", hw_vo_attr->picture_aspect, sw_vo_attr->picture_aspect);
        return TD_TRUE;
    }

    return TD_FALSE;
}

static td_s32 check_app_attr(const hdmi_app_attr *app_attr)
{
    td_s32 ret = TD_SUCCESS;

    if (app_attr->out_color_space != HDMI_COLORSPACE_RGB &&
        app_attr->out_color_space != HDMI_COLORSPACE_YCBCR422 &&
        app_attr->out_color_space != HDMI_COLORSPACE_YCBCR444 &&
        app_attr->out_color_space != HDMI_COLORSPACE_YCBCR420) {
        hdmi_err("out_color_space=%u is wrong\n", app_attr->out_color_space);
        ret = TD_FAILURE;
    }

    if (app_attr->deep_color_mode != HDMI_DEEP_COLOR_24BIT &&
        app_attr->deep_color_mode != HDMI_DEEP_COLOR_30BIT &&
        app_attr->deep_color_mode != HDMI_DEEP_COLOR_36BIT &&
        app_attr->deep_color_mode != HDMI_DEEP_COLOR_48BIT &&
        app_attr->deep_color_mode != HDMI_DEEP_COLOR_OFF) {
        hdmi_err("deep_clr_mode=%u is wrong\n", app_attr->deep_color_mode);
        ret = TD_FAILURE;
    }

    if (app_attr->hdcp_mode != HDMI_HDCP_MODE_1_4 &&
        app_attr->hdcp_mode != HDMI_HDCP_MODE_2_2 &&
        app_attr->hdcp_mode != HDMI_HDCP_MODE_AUTO) {
        hdmi_err("hdcp_mode=%u is wrong\n", app_attr->hdcp_mode);
        ret = TD_FAILURE;
    }

    return ret;
}

static td_void hdmi_vo_attr_info(const hdmi_device *hdmi_dev, const hdmi_vo_attr *video_attr)
{
    ot_unused(hdmi_dev);

    hdmi_info("clk_fs: %u\n", video_attr->clk_fs);
    hdmi_info("pixel_repeat: %u\n", video_attr->pixel_repeat);
    hdmi_info("video_timing: %u\n", video_attr->video_timing);
    hdmi_info("stereo_mode: %u\n", video_attr->stereo_mode);
    hdmi_info("in_color_space: %u\n", video_attr->in_color_space);
    hdmi_info("colorimetry: %u\n", video_attr->colorimetry);
    hdmi_info("extended_colorimetry: %u\n", video_attr->extended_colorimetry);
    hdmi_info("rgb_quantization: %u\n", video_attr->rgb_quantization);
    hdmi_info("ycc_quantization: %u\n", video_attr->ycc_quantization);
    hdmi_info("picture_aspect: %u\n", video_attr->picture_aspect);
    hdmi_info("active_aspect: %u\n", video_attr->active_aspect);
    hdmi_info("picture_scaling: %u\n", video_attr->picture_scaling);
    hdmi_info("in_bit_depth: %u\n", video_attr->in_bit_depth);
    hdmi_info("disp_fmt: %u\n", video_attr->disp_fmt);
    hdmi_info("v_sync_pol: %u\n", video_attr->v_sync_pol);
    hdmi_info("h_sync_pol: %u\n", video_attr->h_sync_pol);
    hdmi_info("de_pol: %u\n", video_attr->de_pol);

    return;
}

static td_s32 hdmi_format_is_change(hdmi_device *hdmi_dev, const hdmi_vo_attr *video_attr,
    const hdmi_vo_attr *vo_attr)
{
    errno_t ret;
    hdmi_app_attr tmp_app = {0};
    hdmi_app_attr *app_attr = TD_NULL;
    td_bool enable_clr_space_adapt_back;
    td_bool enable_deep_clr_adapt_back;
    static hdmi_disp_format disp_fmt = HDMI_VIDEO_FORMAT_BUTT;

    app_attr = &hdmi_dev->attr.app_attr;
    enable_clr_space_adapt_back = app_attr->enable_clr_space_adapt;
    enable_deep_clr_adapt_back = app_attr->enable_deep_clr_adapt;
    ret = memcpy_s(&tmp_app, sizeof(tmp_app), app_attr, sizeof(*app_attr));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    /* when format is changed, force to enable  hdmi_color_and_bit_strategy */
    if (disp_fmt != video_attr->disp_fmt) {
        tmp_app.enable_clr_space_adapt = TD_TRUE;
        tmp_app.enable_deep_clr_adapt = TD_TRUE;
        disp_fmt = video_attr->disp_fmt;
    }
    if (hdmi_color_and_bit_strategy(hdmi_dev, &tmp_app, vo_attr) != TD_SUCCESS) {
        hdmi_err(" hdmi_color_and_bit_strategy fail\n");
        return OT_ERR_HDMI_STRATEGY_FAILED;
    }

    ret = memcpy_s(app_attr, sizeof(hdmi_dev->attr.app_attr), &tmp_app, sizeof(tmp_app));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    app_attr->enable_clr_space_adapt = enable_clr_space_adapt_back;
    app_attr->enable_deep_clr_adapt  = enable_deep_clr_adapt_back;

    return TD_SUCCESS;
}

#endif /* SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART04_H_ */
