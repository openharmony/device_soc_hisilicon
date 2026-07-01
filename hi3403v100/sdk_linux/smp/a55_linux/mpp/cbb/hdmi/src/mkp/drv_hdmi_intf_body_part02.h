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

#ifndef SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART02_H_
#define SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART02_H_

        } else if (user_app->enable_deep_clr_adapt == TD_TRUE) {
            hdmi_warn("fail, deepclr(%u) adapt to 8bit!\n", user_app->deep_color_mode);
            user_app->deep_color_mode = HDMI_DEEP_COLOR_24BIT;
        } else {
            hdmi_err("fail, adapt deepclr fail!\n");
            return TD_FAILURE;
        }

        return TD_SUCCESS;
    }
}

static td_s32 adapt_to_420(const hdmi_vo_attr *vo_attr, hdmi_app_attr *user_app, td_u32 max_tmds_clk)
{
    if (vo_attr->clk_fs <= max_tmds_clk) {
        return TD_SUCCESS;
    }

    hdmi_warn("pix_clk(%u) > max_tmds_clk(%u)\n", vo_attr->clk_fs, max_tmds_clk);
    /* in h8, hdmi can adapt to yuv420 just in yuv444 */
    if (user_app->out_color_space == HDMI_COLORSPACE_YCBCR444) {
        if (user_app->enable_clr_space_adapt == TD_TRUE) {
            hdmi_warn("out_color_space(%u) adapt to Y420.\n", user_app->out_color_space);
            user_app->out_color_space = HDMI_COLORSPACE_YCBCR420;
        } else {
            hdmi_err("adapt Y420 fail! enable_clr_space_adapt is false.\n");
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static td_s32 adapt_to_444(const hdmi_vo_attr *vo_attr, hdmi_app_attr *user_app)
{
    if (vo_attr->clk_fs > HDMI_14_MAX_TMDS_CLK) {
        return TD_SUCCESS;
    }

    if (user_app->out_color_space == HDMI_COLORSPACE_YCBCR420) {
        hdmi_warn("pix_clk(%u) <= HDMI_14_MAX_TMDS_CLK(%u)\n", vo_attr->clk_fs, HDMI_14_MAX_TMDS_CLK);
        if (user_app->enable_clr_space_adapt == TD_TRUE) {
            hdmi_warn("out_color_space(%u) adapt to Y444.\n", user_app->out_color_space);
            user_app->out_color_space = HDMI_COLORSPACE_YCBCR444;
        } else {
            hdmi_err("adapt Y444 fail! enable_clr_space_adapt is false.\n");
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static td_void edid_invalid_or_auth_mode(hdmi_device *hdmi_dev, hdmi_app_attr *user_app)
{
    hdmi_vo_attr *vo_attr = TD_NULL;

    vo_attr = &hdmi_dev->attr.vo_attr;
    vo_attr->hdmi_adapt_pix_clk =
        (user_app->out_color_space == HDMI_COLORSPACE_YCBCR420) ? (vo_attr->clk_fs >> 1) : vo_attr->clk_fs;

    if (user_app->out_color_space != HDMI_COLORSPACE_YCBCR422) {
        switch (user_app->deep_color_mode) {
            case HDMI_DEEP_COLOR_48BIT:
                hdmi_multiple_2p0(vo_attr->hdmi_adapt_pix_clk);
                break;
            case HDMI_DEEP_COLOR_36BIT:
                hdmi_multiple_1p5(vo_attr->hdmi_adapt_pix_clk);
                break;
            case HDMI_DEEP_COLOR_30BIT:
                hdmi_multiple_1p25(vo_attr->hdmi_adapt_pix_clk);
                break;
            default:
                break;
        }
    } else {
        if ((user_app->deep_color_mode != HDMI_DEEP_COLOR_OFF) &&
            (user_app->deep_color_mode != HDMI_DEEP_COLOR_24BIT)) {
            hdmi_info("Y422 foce deepcolor 8bit");
            user_app->deep_color_mode = HDMI_DEEP_COLOR_24BIT;
        }
        if (hdmi_dev->attr.vo_attr.video_timing == HDMI_VIDEO_TIMING_1440X480I_60000 ||
            hdmi_dev->attr.vo_attr.video_timing == HDMI_VIDEO_TIMING_1440X576I_50000) {
            hdmi_warn("Y422 is not support at pal and ntsc, force adapt to rgb!\n");
            user_app->out_color_space = HDMI_COLORSPACE_RGB;
        }
    }

    return;
}

static td_s32 dvi_color_and_bit_strategy(hdmi_device *hdmi_dev, hdmi_app_attr *user_app)
{
    hdmi_vo_attr *vo_attr = TD_NULL;

    vo_attr = &hdmi_dev->attr.vo_attr;
    if (user_app->out_color_space != HDMI_COLORSPACE_RGB) {
        hdmi_err("DVI mode, but the color space is not RGB!\n");
        return TD_FAILURE;
    }
    user_app->deep_color_mode = HDMI_DEEP_COLOR_OFF;
    vo_attr->hdmi_adapt_pix_clk = vo_attr->clk_fs;

    return TD_SUCCESS;
}

static td_s32 hdmi_deep_color_adapt(const hdmi_device *hdmi_dev, hdmi_app_attr *user_app)
{
    /* Y422 default 12bit output, deep_color force adapt to 8bit(24bit). */
    if (user_app->out_color_space == HDMI_COLORSPACE_YCBCR422) {
        if (hdmi_dev->attr.vo_attr.video_timing == HDMI_VIDEO_TIMING_1440X480I_60000 ||
            hdmi_dev->attr.vo_attr.video_timing == HDMI_VIDEO_TIMING_1440X576I_50000) {
            hdmi_err("Y422 is not support at pal and ntsc!\n");
            return TD_FAILURE;
        }
        if ((user_app->deep_color_mode != HDMI_DEEP_COLOR_24BIT) &&
            (user_app->deep_color_mode != HDMI_DEEP_COLOR_OFF)) {
            user_app->deep_color_mode = HDMI_DEEP_COLOR_OFF;
            hdmi_warn("when Y422, deep_color not support 10/12bit!\n");
        }
    }

    return TD_SUCCESS;
}

static td_s32 hdmi_color_and_bit_strategy(hdmi_device *hdmi_dev, hdmi_app_attr *user_app,
    const hdmi_vo_attr *vo_timing_attr)
{
    td_s32 ret;
    hdmi_edid_data edid_ret;
    td_u32 max_tmds_clk = 0;
    hdmi_vo_attr *vo_attr = TD_NULL;
    hdmi_sink_capability *sink_cap = TD_NULL;
    hdmi_tx_capability_data tx_cap = {0};
    ot_unused(vo_timing_attr);

    vo_attr = &hdmi_dev->attr.vo_attr;
    /* DVI mode, must set RGB & deep_color off */
    if (user_app->enable_hdmi == TD_FALSE) {
        return dvi_color_and_bit_strategy(hdmi_dev, user_app);
    }
    hal_call_void(hdmi_dev, hal_hdmi_tx_capability_get, &tx_cap);
    edid_ret = drv_hdmi_edid_capability_get(&hdmi_dev->edid_info, &sink_cap);
#if defined(HDMI_PRODUCT_SS626V100)
    if (edid_ret == HDMI_EDID_DATA_INVALID || (user_app->auth_mode == TD_TRUE)
        || (vo_timing_attr->disp_fmt == HDMI_VIDEO_FORMAT_7680X4320P_30))
#else
    if (edid_ret == HDMI_EDID_DATA_INVALID || user_app->auth_mode == TD_TRUE)
#endif
    {
        edid_invalid_or_auth_mode(hdmi_dev, user_app);
        /* get edid_capability fail means cannot do strategy, according to appatrr */
        hdmi_warn("get sink capability fail or auth_mode = %u, adapt hdmi_adapt_pix_clk = %u\n",
                  hdmi_dev->attr.app_attr.auth_mode, vo_attr->hdmi_adapt_pix_clk);
        return TD_SUCCESS;
    } else {
        /* get max_tmds_clk, in khz. */
        if (sink_cap->max_tmds_clock == 0) {
            /*
             * when the sink's max_tmds_clock is 0 Mhz, app can not set attr success.
             * then set max_tmds_clk = 300Mhz default.
             */
            max_tmds_clk = 300; /* 300, default clk */
        } else {
            max_tmds_clk =
                (sink_cap->max_tmds_clock < tx_cap.tx_max_tmds_clk) ? sink_cap->max_tmds_clock : tx_cap.tx_max_tmds_clk;
        }
        max_tmds_clk *= 1000; /* 1000, khz */
    }
    /* whether adapt to ycbcr420 */
    ret = adapt_to_420(vo_attr, user_app, max_tmds_clk);
    hdmi_if_failure_return(ret, TD_FAILURE);
    /* whether adapt to ycbcr444 */
    ret = adapt_to_444(vo_attr, user_app);
    hdmi_if_failure_return(ret, TD_FAILURE);
    /* color_space check & adapt */
    ret = hdmi_color_space_check(hdmi_dev, user_app);
    hdmi_if_failure_return(ret, TD_FAILURE);
    /* deep_color check & adapt */
    ret = hdmi_deep_color_check(hdmi_dev, user_app, max_tmds_clk);
    hdmi_if_failure_return(ret, TD_FAILURE);
    /* deep color adapt */
    ret = hdmi_deep_color_adapt(hdmi_dev, user_app);
    hdmi_if_failure_return(ret, TD_FAILURE);
    hdmi_info("clr_space_adapt=%u, deep_clr_adapt=%u, adptclrspace=%u, adptbit=%u, adptclk=%u, max_clk=%u\n",
              user_app->enable_clr_space_adapt, user_app->enable_deep_clr_adapt, user_app->out_color_space,
              user_app->deep_color_mode, vo_attr->hdmi_adapt_pix_clk, max_tmds_clk);

    return TD_SUCCESS;
}

static hdmi_deep_color depth_convert_to_deep_color(hdmi_video_bit_depth bit_depth)
{
    hdmi_deep_color deep_color;

    switch (bit_depth) {
        case HDMI_VIDEO_BITDEPTH_8:
            deep_color = HDMI_DEEP_COLOR_24BIT;
            break;
        case HDMI_VIDEO_BITDEPTH_10:
            deep_color = HDMI_DEEP_COLOR_30BIT;
            break;
        case HDMI_VIDEO_BITDEPTH_12:
            deep_color = HDMI_DEEP_COLOR_36BIT;
            break;
        case HDMI_VIDEO_BITDEPTH_16:
            deep_color = HDMI_DEEP_COLOR_48BIT;
            break;
        case HDMI_VIDEO_BITDEPTH_OFF:
            deep_color = HDMI_DEEP_COLOR_OFF;
            break;
        default:
            deep_color = HDMI_DEEP_COLOR_BUTT;
            break;
    }

    return deep_color;
}

static td_void hdmi_user_attr_construct(hdmi_device *hdmi_dev, hdmi_attr *attr, const hdmi_hardware_status *hw_status)
{
    hdmi_app_attr *app_attr = TD_NULL;

    app_attr = &attr->app_attr;
    hdmi_dev->tmds_mode = hw_status->common_status.tmds_mode;
    switch (hdmi_dev->tmds_mode) {
        case HDMI_TMDS_MODE_HDMI_1_4:
        case HDMI_TMDS_MODE_HDMI_2_0:
            app_attr->enable_hdmi = TD_TRUE;
            app_attr->enable_video = TD_TRUE;
            break;
        case HDMI_TMDS_MODE_DVI:
            app_attr->enable_hdmi = TD_FALSE;
            app_attr->enable_video = TD_TRUE;
            break;
        default:
            app_attr->enable_hdmi = TD_FALSE;
            app_attr->enable_video = TD_FALSE;
            break;
    }

    app_attr->enable_audio = (hw_status->audio_status.audio_enable && hw_status->info_frame_status.audio_enable);
    app_attr->out_color_space = hw_status->video_status.out_color_space;
    app_attr->deep_color_mode = depth_convert_to_deep_color(hw_status->video_status.out_bit_depth);
    app_attr->enable_avi_infoframe = hw_status->info_frame_status.avi_enable;
    app_attr->enable_aud_infoframe = hw_status->info_frame_status.audio_enable;
    app_attr->xvycc_mode = hw_status->info_frame_status.gbd_enable;

    return;
}

static td_void hdmi_video_attr_construct(hdmi_attr *attr, const hdmi_hardware_status *hw_status)
{
    hdmi_app_attr *app_attr = TD_NULL;
    hdmi_vo_attr *video_attr = TD_NULL;
    const hdmi_infoframe_status *status = TD_NULL;
    td_bool in_clr_is_rgb;
    td_bool aspect_is256;

    app_attr = &attr->app_attr;
    video_attr = &attr->vo_attr;
    status = &hw_status->info_frame_status;
    video_attr->in_color_space = app_attr->out_color_space;
    video_attr->v_sync_pol = hw_status->video_status.v_sync_pol;
    video_attr->h_sync_pol = hw_status->video_status.h_sync_pol;
    video_attr->de_pol = hw_status->video_status.de_pol;

    in_clr_is_rgb = ((hw_status->video_status.rgb2ycbcr) || ((hw_status->video_status.ycbcr2rgb == TD_FALSE) &&
                     (hw_status->video_status.out_color_space == HDMI_COLORSPACE_RGB)));
    video_attr->in_color_space = (in_clr_is_rgb == TD_TRUE) ? HDMI_COLORSPACE_RGB : HDMI_COLORSPACE_YCBCR444;
    if (status->avi_enable == TD_TRUE) {
        hdmi_video_code_vic video_code = status->avi[AVI_OFFSET_VIC];
        /*
         * when the timing is 4096*2160, the aspect ratio in AVI infoframe is 0
         * (but the real aspect ratio is 256:135<0x04>, the video_code is 0)
         */
        aspect_is256 = (((video_code == 0) &&
                        (status->vsif[VENDOR_OFFSET_VIC] == HDMI_VIC_4K24)) ||
                        ((video_code >= HDMI_4096X2160P25_256_135) && (video_code <= HDMI_4096X2160P60_256_135)));

        video_attr->picture_aspect = (aspect_is256 == TD_TRUE) ? HDMI_PICTURE_ASPECT_256_135 :
            ((status->avi[AVI_OFFSET_PB2] >> 4) & AVI_FRAME_PIC_ASPECT_MASK); /* 4, offset of byte. */
        video_attr->active_aspect = status->avi[AVI_OFFSET_PB2] & AVI_FRAME_ACTIVE_ASPECT_MASK;
        /* 6, offset of byte. */
        video_attr->colorimetry = (status->avi[AVI_OFFSET_PB2] >> 6) & AVI_FRAME_COLORIMETRY_MASK;
        /* 2, offset of byte. */
        video_attr->rgb_quantization = (status->avi[AVI_OFFSET_PB3] >> 2) & AVI_FRAME_QUANT_RANGE_MASK;
        /* 6, offset of byte. */
        video_attr->ycc_quantization = (status->avi[AVI_OFFSET_PB5] >> 6) & AVI_FRAME_YCC_QUANT_RANGE_MASK;
        video_attr->pixel_repeat = (status->avi[AVI_OFFSET_PB5] & AVI_FRAME_PIXEL_REPET_MASK) + 1;
        /* 4, offset of byte. */
        video_attr->extended_colorimetry = (status->avi[AVI_OFFSET_PB3] >> 4) & AVI_FRAME_EXT_COLORIMETRY_MASK;
        video_attr->video_timing = drv_hdmi_video_timing_lookup(video_code, video_attr->picture_aspect);
        hdmi_info("video_timing: %u, video_code: %u, picture_aspect: %u\n",
            video_attr->video_timing, video_code, video_attr->picture_aspect);
        if ((status->vsif_enable == TD_FALSE) && (!video_code)) {
            video_attr->video_timing = HDMI_VIDEO_TIMING_UNKNOWN;
        }
        app_attr->out_csc_quantization = (app_attr->out_color_space == HDMI_COLORSPACE_RGB) ?
            video_attr->rgb_quantization : (video_attr->ycc_quantization + 1);
    }
    video_attr->stereo_mode = HDMI_3D_BUTT;
    if (status->vsif_enable == TD_TRUE) {
        /* 5, offset of byte. */
        enum hdmi_video_format format = (status->vsif[VENDOR_OFFSET_VIDEO_FMT] >> 5) & VENDOR_FARAME_VIDEO_FMT_MASK;
        if (format == HDMI_VIDEO_FORMAT_4K) {
            hdmi_vsif_vic video_code = status->vsif[VENDOR_OFFSET_VIC];
            video_attr->video_timing = drv_hdmi_vsif_video_timing_lookup(video_code);
        } else if (format == HDMI_VIDEO_FORMAT_3D) {
            /* 4, offset of byte. */
            video_attr->stereo_mode = (status->vsif[VENDOR_OFFSET_VIC] >> 4) & VENDOR_3D_STRUCT_MASK;
        }
    }

    return;
}

static td_void hdmi_attr_construct(hdmi_device *hdmi_dev, hdmi_attr *attr)
{
    hdmi_ao_attr *audio_attr = TD_NULL;
    hdmi_hardware_status *status = TD_NULL;

    status = (hdmi_hardware_status *)osal_vmalloc(sizeof(hdmi_hardware_status));
    if (status == TD_NULL) {
        hdmi_err("alloc hdmi_hardware_status struct memory fail\n");
        return;
    }
    (td_void)memset_s(status, sizeof(hdmi_hardware_status), 0, sizeof(hdmi_hardware_status));

    audio_attr = &attr->ao_attr;
    hal_call_void(hdmi_dev, hal_hdmi_hardware_status_get, status);

    hdmi_user_attr_construct(hdmi_dev, attr, status);
    hdmi_video_attr_construct(attr, status);
    audio_attr->down_sample  = status->audio_status.down_sample;
    audio_attr->channels     = status->audio_status.layout;
    audio_attr->sample_depth = status->audio_status.sample_depth;
    audio_attr->sample_fs    = status->audio_status.sample_fs;
    audio_attr->sound_intf   = status->audio_status.sound_intf;
    if (status->info_frame_status.audio_enable) {
        /* 4, offset of byte. */
        audio_attr->audio_code = (status->info_frame_status.audio[AUDIO_OFFSET_PB1] >> 4) & AUDIO_FRAME_CODE_TYPE_MASK;
    }

    osal_vfree(status);
    status = TD_NULL;

    return;
}

static td_s32 hpd_event(hdmi_device *hdmi_dev)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 timeout_cnt = EDID_UPDATA_CNT_MAX;

    /* update edid from sink */
    if (hdmi_dev->hpd_detected == TD_FALSE) {
        drv_hdmi_edid_reset(&hdmi_dev->edid_info);
        do {
            /*
             * 1.for some TV, EDID need to read more times;
             * 2.for some repeater, EDID is readable after HPD assert 250ms.
             */
            ret = drv_hdmi_edid_update(&hdmi_dev->edid_info, HDMI_EDID_UPDATE_SINK);
            if (ret == TD_SUCCESS) {
                hdmi_dev->hpd_detected = TD_TRUE;
                break;
            }
            osal_msleep(DEV_HPD_ASSERT_WAIT_TIME);
        } while (timeout_cnt--);

        if (ret != TD_SUCCESS) {
            hdmi_warn("update EDID fail, timeout_cnt = %u\n", timeout_cnt);
        }
    }
    compatibility_info_update(hdmi_dev->hdmi_dev_id);

    return ret;
}

static td_void event_notify(hdmi_device *hdmi_dev, hdmi_event event)
{
    if (event <= HDMI_EVENT_HOTUNPLUG) {
        hdmi_mutex_unlock(g_hdmi_mutex);
        hdmi_mutex_lock(hdmi_dev->mutex_thread);
        if (hdmi_dev->k_callback == TD_NULL) { /* notify to user */
            hdmi_info("hdmi_id:%d, notify event(0x%x) to user\n", hdmi_dev->hdmi_dev_id, event);
            drv_hdmi_event_pool_write(hdmi_dev->hdmi_dev_id, event);
        } else { /* notify to kernel */
            hdmi_info("hdmi_id:%d, notify event(0x%x) to kernel\n", hdmi_dev->hdmi_dev_id, event);
            if (hdmi_dev->k_callback != TD_NULL) {
                hdmi_dev->k_callback((td_void *)&(hdmi_dev->hdmi_dev_id), event);
            }
            if (event == HDMI_EVENT_HOTPLUG) {
                hdmi_dev->hpd_notifyed = TD_TRUE;
            }
        }
        hdmi_mutex_unlock(hdmi_dev->mutex_thread);
        hdmi_mutex_lock(g_hdmi_mutex);
    }

    return;
}

static td_s32 hdmi_event_callback(td_void *data, hdmi_event event)
{
    td_s32 ret = TD_SUCCESS;
    hdmi_device *hdmi_dev = (hdmi_device *)data;

    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);

    switch (event) {
        case HDMI_EVENT_HOTPLUG:
            hpd_event(hdmi_dev);
            break;
        case HDMI_EVENT_HOTUNPLUG:
            hdmi_dev->hpd_detected = TD_FALSE;
            break;
        default:
            break;
    }
    event_notify(hdmi_dev, event);

    return ret;
}

static td_s32 hdmi_kthread_timer(void *data)
{
    hdmi_device *hdmi_dev = (hdmi_device *)data;
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);


#endif /* SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART02_H_ */
