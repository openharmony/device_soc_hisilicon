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

#ifndef SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART05_H_
#define SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART05_H_

static td_s32 drv_hdmi_vo_attr_get(const hdmi_device *hdmi_dev, hdmi_vo_attr *vo_attr)
{
    errno_t ret;

    hdmi_info("in\n");

    ret = memcpy_s(vo_attr, sizeof(*vo_attr), &hdmi_dev->attr.vo_attr, sizeof(hdmi_vo_attr));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    hdmi_info("out\n");

    return TD_SUCCESS;
}

static td_s32 drv_hdmi_infoframe_get(const hdmi_device *hdmi_dev,
    hdmi_infoframe_id infoframe_id, hdmi_infoframe *info_frame)
{
    errno_t ret;

    hdmi_info("in\n");

    switch (infoframe_id) {
        case HDMI_INFOFRAME_TYPE_VENDOR:
            ret = memcpy_s(&info_frame->vendor_infoframe, sizeof(info_frame->vendor_infoframe),
                &hdmi_dev->info_frame.vendor_infoframe, sizeof(hdmi_vendor_infoframe));
            if (ret != EOK) {
                hdmi_err("secure function error:%d\n", ret);
                return OT_ERR_HDMI_INVALID_PARA;
            }
            break;
        case HDMI_INFOFRAME_TYPE_AVI:
            ret = memcpy_s(&info_frame->avi_infoframe, sizeof(info_frame->avi_infoframe),
                &hdmi_dev->info_frame.avi_infoframe, sizeof(hdmi_avi_infoframe));
            if (ret != EOK) {
                hdmi_err("secure function error:%d\n", ret);
                return OT_ERR_HDMI_INVALID_PARA;
            }
            break;
        case HDMI_INFOFRAME_TYPE_AUDIO:
            ret = memcpy_s(&info_frame->audio_infoframe, sizeof(info_frame->audio_infoframe),
                &hdmi_dev->info_frame.audio_infoframe, sizeof(hdmi_audio_infoframe));
            if (ret != EOK) {
                hdmi_err("secure function error:%d\n", ret);
                return OT_ERR_HDMI_INVALID_PARA;
            }
            break;
        default:
            return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
    }
    hdmi_info("out\n");

    return TD_SUCCESS;
}

static td_void infoframe_set_vendor(hdmi_device *hdmi_dev, hdmi_infoframe *info_frame)
{
    errno_t ret;
    hdmi_user_vsif_content *src_vsif_content = TD_NULL;
    hdmi_user_vsif_content *dest_vsif_content = TD_NULL;

    src_vsif_content = &info_frame->vendor_infoframe.vsif_content.cea861_vsif;
    dest_vsif_content = &hdmi_dev->info_frame.vendor_infoframe.vsif_content.cea861_vsif;
    /*
     * save the user data to pstHdmiDev->puInfoFrame,
     * because it will get the data form it when user call get infoframe.
     */
    dest_vsif_content->len = src_vsif_content->len;
    ret = memcpy_s(dest_vsif_content->data, HDMI_VENDOR_USER_DATA_MAX_LEN,
        src_vsif_content->data, src_vsif_content->len);
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return;
    }
    /* first: save user data to pstHdmiDev->vendor_data */
    hdmi_dev->vendor_data.len = src_vsif_content->len;
    (td_void)memset_s(hdmi_dev->vendor_data.data, HDMI_VENDOR_USER_DATA_MAX_LEN, 0, HDMI_VENDOR_USER_DATA_MAX_LEN);
    ret = memcpy_s(hdmi_dev->vendor_data.data, HDMI_VENDOR_USER_DATA_MAX_LEN,
        src_vsif_content->data, src_vsif_content->len);
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return;
    }
    /* second: copy other information to puInfoFrame, except user data */
    src_vsif_content->ieee             = dest_vsif_content->ieee;
    src_vsif_content->format           = dest_vsif_content->format;
    src_vsif_content->vic              = dest_vsif_content->vic;
    src_vsif_content->_3d_meta_present = dest_vsif_content->_3d_meta_present;
    src_vsif_content->_3d_structure    = dest_vsif_content->_3d_structure;

    return;
}

static td_void infoframe_trace_avi(const hdmi_avi_infoframe *avi_info)
{
    hdmi_info("type: %u\n", avi_info->type);
    hdmi_info("version: %u\n", avi_info->version);
    hdmi_info("length: %u\n", avi_info->length);
    hdmi_info("colorspace: %u\n", avi_info->colorspace);
    hdmi_info("active_info_valid: %u\n", avi_info->active_info_valid);
    hdmi_info("horizontal_bar_valid: %u\n", avi_info->horizontal_bar_valid);
    hdmi_info("vertical_bar_valid: %u\n", avi_info->vertical_bar_valid);
    hdmi_info("scan_mode: %u\n", avi_info->scan_mode);
    hdmi_info("colorimetry: %u\n", avi_info->colorimetry);
    hdmi_info("picture_aspect: %u\n", avi_info->picture_aspect);
    hdmi_info("active_aspect: %u\n", avi_info->active_aspect);
    hdmi_info("extended_colorimetry: %u\n", avi_info->extended_colorimetry);
    hdmi_info("quantization_range: %u\n", avi_info->quantization_range);
    hdmi_info("video_timing: %u\n", avi_info->video_timing);
    hdmi_info("disp_fmt: %u\n", avi_info->disp_fmt);
    hdmi_info("ycc_quantization_range: %u\n", avi_info->ycc_quantization_range);
    hdmi_info("content_type: %u\n", avi_info->content_type);
    hdmi_info("pixel_repeat: %u\n", avi_info->pixel_repeat);
    hdmi_info("top_bar: %u\n", avi_info->top_bar);
    hdmi_info("bottom_bar: %u\n", avi_info->bottom_bar);
    hdmi_info("left_bar: %u\n", avi_info->left_bar);
    hdmi_info("right_bar: %u\n", avi_info->right_bar);
}

static td_void infoframe_sync_avi_vo_attr(hdmi_device *hdmi_dev, const hdmi_avi_infoframe *dest_avi_info)
{
    hdmi_vo_attr *vo_attr = &hdmi_dev->attr.vo_attr;

    if (hdmi_dev->attr.app_attr.auth_mode != TD_TRUE) {
        return;
    }

    vo_attr->picture_aspect       = dest_avi_info->picture_aspect;
    vo_attr->active_aspect        = dest_avi_info->active_aspect;
    vo_attr->colorimetry          = dest_avi_info->colorimetry;
    vo_attr->extended_colorimetry = dest_avi_info->extended_colorimetry;
    vo_attr->rgb_quantization     = dest_avi_info->quantization_range;
    vo_attr->ycc_quantization     = dest_avi_info->ycc_quantization_range;
    (td_void)drv_hdmi_vo_attr_set(hdmi_dev, vo_attr);
}

static td_void infoframe_set_avi(hdmi_device *hdmi_dev, const hdmi_infoframe *info_frame)
{
    errno_t ret;
    const hdmi_avi_infoframe *src_avi_info = &info_frame->avi_infoframe;
    hdmi_avi_infoframe *dest_avi_info = &hdmi_dev->info_frame.avi_infoframe;

    infoframe_trace_avi(src_avi_info);

    if ((src_avi_info->type != dest_avi_info->type) || (src_avi_info->colorspace != dest_avi_info->colorspace) ||
        (src_avi_info->video_timing != dest_avi_info->video_timing) ||
        (src_avi_info->disp_fmt != dest_avi_info->disp_fmt) ||
        (src_avi_info->quantization_range != dest_avi_info->quantization_range) ||
        (src_avi_info->ycc_quantization_range != dest_avi_info->ycc_quantization_range) ||
        (src_avi_info->pixel_repeat != dest_avi_info->pixel_repeat)) {
        hdmi_warn("The current infoframe does not match the original infoframe!\n");
    }
    ret = memcpy_s(dest_avi_info, sizeof(hdmi_dev->info_frame.avi_infoframe),
        src_avi_info, sizeof(hdmi_avi_infoframe));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return;
    }

    infoframe_sync_avi_vo_attr(hdmi_dev, dest_avi_info);

    return;
}

static td_s32 drv_hdmi_infoframe_set(hdmi_device *hdmi_dev, hdmi_infoframe_id infoframe_id, hdmi_infoframe *info_frame)
{
    errno_t ret;

    hdmi_info("in, infoframe_id: %u\n", infoframe_id);

    switch (infoframe_id) {
        case HDMI_INFOFRAME_TYPE_VENDOR:
            infoframe_set_vendor(hdmi_dev, info_frame);
            break;
        case HDMI_INFOFRAME_TYPE_AVI:
            infoframe_set_avi(hdmi_dev, info_frame);
            break;
        case HDMI_INFOFRAME_TYPE_AUDIO: {
            hdmi_audio_infoframe *src_audio_info = &info_frame->audio_infoframe;
            hdmi_audio_infoframe *dest_audio_info = &hdmi_dev->info_frame.audio_infoframe;
            ret = memcpy_s(dest_audio_info, sizeof(hdmi_dev->info_frame.audio_infoframe),
                src_audio_info, sizeof(hdmi_audio_infoframe));
            if (ret != EOK) {
                hdmi_err("secure function error:%d\n", ret);
                return OT_ERR_HDMI_INVALID_PARA;
            }

            hdmi_info("type: %u\n", dest_audio_info->type);
            hdmi_info("version: %u\n", dest_audio_info->version);
            hdmi_info("length: %u\n", dest_audio_info->length);
            hdmi_info("channels: %u\n", dest_audio_info->channels);
            hdmi_info("coding_type: %u\n", dest_audio_info->coding_type);
            hdmi_info("sample_size: %u\n", dest_audio_info->sample_size);
            hdmi_info("sample_frequency: %u\n", dest_audio_info->sample_frequency);
            hdmi_info("coding_type_ext: %u\n", dest_audio_info->coding_type_ext);
            hdmi_info("channel_allocation: %u\n", dest_audio_info->channel_allocation);
            hdmi_info("lfe_playback_level: %u\n", dest_audio_info->lfe_playback_level);
            hdmi_info("level_shift_value: %u\n", dest_audio_info->level_shift_value);
            hdmi_info("downmix_inhibit: %u\n", dest_audio_info->downmix_inhibit);
            hdmi_info("level_shift_value: %u\n", dest_audio_info->level_shift_value);
            break;
        }
        default:
            return OT_ERR_HDMI_INVALID_PARA;
    }
    if ((infoframe_id != HDMI_INFOFRAME_TYPE_AVI) || (hdmi_dev->attr.app_attr.auth_mode == TD_FALSE)) {
        drv_hdmi_infoframe_send(&hdmi_dev->info_frame, infoframe_id, info_frame);
    }
    hdmi_info("out\n");

    return TD_SUCCESS;
}

static td_s32 drv_hdmi_status_get(const hdmi_device *hdmi_dev, hdmi_status *status)
{
    hdmi_hardware_status *hw_status = TD_NULL;

    hw_status = (hdmi_hardware_status *)osal_vmalloc(sizeof(hdmi_hardware_status));
    if (hw_status == TD_NULL) {
        hdmi_err("alloc hdmi_hardware_status struct memory fail\n");
        return TD_FAILURE;
    }
    (td_void)memset_s(hw_status, sizeof(hdmi_hardware_status), 0, sizeof(hdmi_hardware_status));

    hal_call_void(hdmi_dev, hal_hdmi_hardware_status_get, hw_status);

    /* some TV sometimes has no hot_plug but has rsen */
    status->connected = (hw_status->common_status.hotplug || hw_status->common_status.rsen);
    status->sink_power_on = hw_status->common_status.rsen;

    osal_vfree(hw_status);
    hw_status = TD_NULL;

    return TD_SUCCESS;
}

static td_s32 drv_hdmi_mod_param_set(hdmi_device *hdmi_dev, const drv_hdmi_mod_param *mode_param)
{
    hdmi_mode_param *param = TD_NULL;

    param = &hdmi_dev->mode_param;
    hdmi_info("set trace length: %u->%u, emi: %u->%u\n",
        param->trace_len, mode_param->trace_len, param->emi_en, mode_param->emi_en);
    param->emi_en = mode_param->emi_en;
    param->trace_len = mode_param->trace_len;

    return TD_SUCCESS;
}

static td_s32 drv_hdmi_mod_param_get(const hdmi_device *hdmi_dev, drv_hdmi_mod_param *mode_param)
{
    const hdmi_mode_param *param = TD_NULL;

    param = &hdmi_dev->mode_param;
    mode_param->emi_en = param->emi_en;
    mode_param->trace_len = param->trace_len;

    return TD_SUCCESS;
}

static td_s32 drv_hdmi_hw_spec_get(const hdmi_device *hdmi_dev, hdmi_hw_spec *hw_spec)
{
    td_s32 ret;

    hal_call_ret(hdmi_dev, ret, hal_hdmi_phy_hw_spec_get, hw_spec);

    return ret;
}

static td_s32 drv_hdmi_hw_spec_set(const hdmi_device *hdmi_dev, const hdmi_hw_spec *hw_spec)
{
    td_u8 i;
    td_u32 tmds_clk;
    td_s32 ret;

    tmds_clk = hdmi_dev->attr.vo_attr.hdmi_adapt_pix_clk;
    hdmi_info("tmds_clk : %u\n", tmds_clk);
    for (i = 0; i < HW_PARAM_ARRAY_COUNT; i++) {
        hdmi_info("stage[%u]: i_de_main_clk:%u i_de_main_data:%u\
            i_main_clk:%u i_main_data:%u ft_cap_clk:%u ft_cap_data:%u\n", i,
            hw_spec->hw_param[i].i_de_main_clk, hw_spec->hw_param[i].i_de_main_data,
            hw_spec->hw_param[i].i_main_clk, hw_spec->hw_param[i].i_main_data,
            hw_spec->hw_param[i].ft_cap_clk, hw_spec->hw_param[i].ft_cap_data);
    }
    hal_call_ret(hdmi_dev, ret, hal_hdmi_phy_hw_spec_set, tmds_clk, hw_spec);

    return ret;
}

static td_s32 hdmi21_caps_check(const hdmi_device *hdmi_dev, const hdmi_property *attr)
{
    td_bool is_4k;

    ot_unused(hdmi_dev);
    is_4k = (attr->disp_fmt == HDMI_VIDEO_FORMAT_3840X2160P_50) ||
            (attr->disp_fmt == HDMI_VIDEO_FORMAT_3840X2160P_60) ||
            (attr->disp_fmt == HDMI_VIDEO_FORMAT_4096X2160P_50) ||
            (attr->disp_fmt == HDMI_VIDEO_FORMAT_4096X2160P_60) ||
            (attr->disp_fmt == HDMI_VIDEO_FORMAT_7680X4320P_30);

    if (is_4k == TD_TRUE &&
        (attr->out_color_space == HDMI_COLORSPACE_RGB || attr->out_color_space == HDMI_COLORSPACE_YCBCR444) &&
        (attr->deep_color_mode == HDMI_DEEP_COLOR_30BIT || attr->deep_color_mode == HDMI_DEEP_COLOR_36BIT)) {
        hdmi_err("param is invalid, fmt = %u, colorspace = %u, deepcolor = %u\n",
            attr->disp_fmt, attr->out_color_space, attr->deep_color_mode);
        return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
    }

    return TD_SUCCESS;
}

static td_s32 hdmi_tmds_check(const hdmi_device *hdmi_dev, const hdmi_property *attr)
{
    td_u32 attr_clk = attr->pix_clk;

    switch (attr->deep_color_mode) {
        case HDMI_DEEP_COLOR_24BIT:
            if (attr_clk < HDMI_TMDS_CLK_MIN) {
                hdmi_err("hdmi tmds clk: %d < 25000hz, not support!\n", attr->pix_clk);
                return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
            }
            break;
        case HDMI_DEEP_COLOR_30BIT:
            hdmi_multiple_1p25(attr_clk);
            if (attr_clk < HDMI_TMDS_CLK_MIN) {
                hdmi_err("hdmi tmds clk: %d < 25000hz, not support!\n", attr_clk);
                return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
            }
            break;
        case HDMI_DEEP_COLOR_36BIT:
            hdmi_multiple_1p5(attr_clk);
            if (attr_clk < HDMI_TMDS_CLK_MIN) {
                hdmi_err("hdmi tmds clk: %d < 25000hz, not support!\n", attr_clk);
                return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
            }
            break;
        default:
            break;
    }
    return TD_SUCCESS;
}

static td_void hdmi_property_vo_attr_adapt(hdmi_vo_attr *vo_attr, const hdmi_property *prop)
{
    td_bool aspect_is43;

    vo_attr->disp_fmt = prop->disp_fmt;
    vo_attr->video_timing = prop->video_timing;
    vo_attr->clk_fs = prop->pix_clk;

    aspect_is43 = ((vo_attr->video_timing == HDMI_VIDEO_TIMING_640X480P_60000) ||
                   (vo_attr->video_timing == HDMI_VIDEO_TIMING_720X480P_60000) ||
                   (vo_attr->video_timing == HDMI_VIDEO_TIMING_720X576P_50000) ||
                   (vo_attr->video_timing == HDMI_VIDEO_TIMING_1440X240P_60000) ||
                   (vo_attr->video_timing == HDMI_VIDEO_TIMING_1440X288P_50000) ||
                   (vo_attr->video_timing == HDMI_VIDEO_TIMING_1440X480I_60000) ||
                   (vo_attr->video_timing == HDMI_VIDEO_TIMING_1440X576I_50000) ||
                   (vo_attr->video_timing == HDMI_VIDEO_TIMING_1440X576P_50000) ||
                   (vo_attr->video_timing == HDMI_VIDEO_TIMING_1440X576I_60000));
    if (aspect_is43 == TD_TRUE) {
        vo_attr->picture_aspect = HDMI_PICTURE_ASPECT_4_3;
    } else if (vo_attr->video_timing <= HDMI_VIDEO_TIMING_4096X2160P_120000 &&
               vo_attr->video_timing >= HDMI_VIDEO_TIMING_4096X2160P_24000) {
        vo_attr->picture_aspect = HDMI_PICTURE_ASPECT_256_135;
    } else {
        vo_attr->picture_aspect = HDMI_PICTURE_ASPECT_16_9;
    }

    return;
}

static td_s32 hdmi_property_backup_attr(const hdmi_device *hdmi_dev, hdmi_app_attr *tmp,
    hdmi_vo_attr *vo_attr, hdmi_ao_attr *ao_attr)
{
    td_s32 err_code;

    err_code = memcpy_s(tmp, sizeof(*tmp), &hdmi_dev->attr.app_attr, sizeof(hdmi_app_attr));
    if (err_code != EOK) {
        hdmi_err("secure function error:%d\n", err_code);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    err_code = memcpy_s(vo_attr, sizeof(*vo_attr), &hdmi_dev->attr.vo_attr, sizeof(hdmi_vo_attr));
    if (err_code != EOK) {
        hdmi_err("secure function error:%d\n", err_code);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    err_code = memcpy_s(ao_attr, sizeof(*ao_attr), &hdmi_dev->attr.ao_attr, sizeof(hdmi_ao_attr));
    if (err_code != EOK) {
        hdmi_err("secure function error:%d\n", err_code);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    return TD_SUCCESS;
}

static td_void hdmi_property_apply_app_attr(hdmi_app_attr *app_attr, const hdmi_property *prop)
{
    app_attr->enable_hdmi = prop->enable_hdmi;
    app_attr->enable_video = prop->enable_video;
    app_attr->enable_audio = prop->enable_audio;
    app_attr->auth_mode = prop->auth_mode;
    app_attr->hdmi_action = prop->hdmi_action;
    app_attr->deep_color_mode = prop->deep_color_mode;
    app_attr->enable_avi_infoframe = prop->enable_avi_infoframe;
    app_attr->enable_aud_infoframe = prop->enable_aud_infoframe;
    app_attr->enable_deep_clr_adapt = prop->enable_deep_clr_adapt;
}

static td_s32 drv_hdmi_property_set(hdmi_device_id hdmi_id, const hdmi_property *prop)
{
    td_s32 ret;
    td_s32 err_code;
    hdmi_app_attr tmp = {0};
    hdmi_ao_attr ao_attr = {0};
    hdmi_vo_attr vo_attr = {0};
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_app_attr *app_attr = TD_NULL;

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    ret = hdmi21_caps_check(hdmi_dev, prop);
    hdmi_if_failure_return(ret, OT_ERR_HDMI_FEATURE_NO_SUPPORT);
    /* hdmi does not support < 25000hz tmds clk */
    ret = hdmi_tmds_check(hdmi_dev, prop);


#endif /* SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART05_H_ */
