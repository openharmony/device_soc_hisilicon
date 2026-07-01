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

#ifndef SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART03_H_
#define SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART03_H_

    while (!osal_kthread_should_stop()) {
        if (hdmi_dev->thread_info.thread_timer_sate == HDMI_THREAD_STATE_STOP) {
            osal_msleep(HDMI_THREAD_STATE_WAIT_TIME);
            continue;
        }
        if (hdmi_dev->hal != TD_NULL) {
            if (hdmi_dev->hal->hal_hdmi_sequencer_handler_process != TD_NULL &&
                (((td_u32)hdmi_dev->run_state & HDMI_RUN_STATE_OPEN) ||
                ((td_u32)hdmi_dev->run_state & HDMI_RUN_STATE_START) ||
                ((td_u32)hdmi_dev->run_state & HDMI_RUN_STATE_STOP))) {
                hdmi_dev->hal->hal_hdmi_sequencer_handler_process((struct hdmi_hal_ *)hdmi_dev->hal);
            }
        } else {
            hdmi_info("input param is NULL\n");
        }
        osal_msleep(hdmi_dev->debug.intf_status.event_thread_cycle_time);
    }

    return TD_SUCCESS;
}

static td_void hdmi_vo_attr_init(hdmi_vo_attr *video_attr)
{
    td_bool colorimetry_is601;

    colorimetry_is601 = ((video_attr->video_timing == HDMI_VIDEO_TIMING_720X480P_60000) ||
                         (video_attr->video_timing == HDMI_VIDEO_TIMING_720X576P_50000) ||
                         (video_attr->video_timing == HDMI_VIDEO_TIMING_1440X240P_60000) ||
                         (video_attr->video_timing == HDMI_VIDEO_TIMING_1440X480I_60000) ||
                         (video_attr->video_timing == HDMI_VIDEO_TIMING_1440X576I_50000) ||
                         (video_attr->video_timing == HDMI_VIDEO_TIMING_1440X576P_50000) ||
                         (video_attr->video_timing == HDMI_VIDEO_TIMING_1440X576I_60000));

    if (video_attr->video_timing == HDMI_VIDEO_TIMING_640X480P_60000) {
        video_attr->colorimetry = HDMI_COLORIMETRY_ITU_709;
        video_attr->picture_aspect = HDMI_PICTURE_ASPECT_4_3;
    } else if (colorimetry_is601 == TD_TRUE) {
        video_attr->colorimetry = HDMI_COLORIMETRY_ITU_601;
        video_attr->picture_aspect = HDMI_PICTURE_ASPECT_4_3;
    } else if (video_attr->video_timing <= HDMI_VIDEO_TIMING_4096X2160P_120000 &&
               video_attr->video_timing >= HDMI_VIDEO_TIMING_4096X2160P_24000) {
        video_attr->colorimetry = HDMI_COLORIMETRY_ITU_709;
        video_attr->picture_aspect = HDMI_PICTURE_ASPECT_256_135;
    } else {
        video_attr->colorimetry = HDMI_COLORIMETRY_ITU_709;
        video_attr->picture_aspect = HDMI_PICTURE_ASPECT_16_9;
    }

    video_attr->pixel_repeat = 1;
    if (video_attr->video_timing == HDMI_VIDEO_TIMING_1440X480I_60000 ||
        video_attr->video_timing == HDMI_VIDEO_TIMING_1440X576I_50000) {
        video_attr->pixel_repeat = 2; /* 2, pixel repeat count */
    }

    if (video_attr->video_timing <= HDMI_VIDEO_TIMING_640X480P_60000) {
        // cts1.4 test_id 7-24 required rgb_quantization is default or limited when the timing is 640x480p60
        video_attr->rgb_quantization = HDMI_QUANTIZATION_RANGE_DEFAULT;
    } else {
        video_attr->ycc_quantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
    }

    if (video_attr->video_timing != HDMI_VIDEO_TIMING_UNKNOWN &&
        video_attr->video_timing != HDMI_VIDEO_TIMING_640X480P_60000) {
        video_attr->in_color_space = HDMI_COLORSPACE_YCBCR444;
    } else {
        video_attr->in_color_space = HDMI_COLORSPACE_RGB;
    }

    video_attr->stereo_mode   = HDMI_3D_BUTT;
    video_attr->in_bit_depth  = HDMI_VIDEO_BITDEPTH_10;
    video_attr->active_aspect = HDMI_ACTIVE_ASPECT_PICTURE;
}

static td_void hdmi_user_attr_init(hdmi_device *hdmi_dev)
{
    hdmi_app_attr *app_attr = TD_NULL;

    app_attr = &hdmi_dev->attr.app_attr;
    app_attr->enable_hdmi = TD_TRUE;
    app_attr->enable_video = TD_TRUE;
    app_attr->enable_audio = TD_TRUE;
    app_attr->enable_aud_infoframe = TD_TRUE;
    app_attr->enable_avi_infoframe = TD_TRUE;
    app_attr->deep_color_mode = HDMI_DEEP_COLOR_24BIT;
    app_attr->out_color_space = HDMI_COLORSPACE_YCBCR444;
    app_attr->enable_clr_space_adapt = TD_TRUE;
    app_attr->enable_deep_clr_adapt = TD_TRUE;
    app_attr->out_csc_quantization = HDMI_QUANTIZATION_RANGE_LIMITED;

    return;
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wenum-conversion"
#endif
static td_s32 hdmi_device_init(hdmi_device *hdmi_dev)
{
    hdmi_ao_attr *audio_attr = TD_NULL;
    hdmi_vo_attr *video_attr = TD_NULL;

    audio_attr = &hdmi_dev->attr.ao_attr;
    video_attr = &hdmi_dev->attr.vo_attr;
    get_hdmi_default_action_set(hdmi_dev, HDMI_DEFAULT_ACTION_HDMI);

    hdmi_dev->mode_param.emi_en = TD_FALSE;
    hdmi_dev->debug.stop_delay = 0;
    hdmi_dev->delay.fmt_delay  = HDMI_DEV_FMT_DELAY;
    hdmi_dev->delay.mute_delay = HDMI_DEV_MUTE_DELAY;
    hdmi_user_attr_init(hdmi_dev);
    video_attr->video_timing = HDMI_VIDEO_TIMING_1280X720P_50000;
    video_attr->clk_fs       = HDMI_VIDEO_ATTR_CLK_FS;
    video_attr->h_sync_pol   = TD_FALSE;
    video_attr->v_sync_pol   = TD_FALSE;
    video_attr->de_pol       = TD_FALSE;
    (td_void)memset_s(&(hdmi_dev->debug.intf_status), sizeof(hdmi_dev->debug.intf_status), 0, sizeof(hdmi_intf_status));
    hdmi_vo_attr_init(video_attr);
    hdmi_dev->debug.intf_status.event_thread_cycle_time = HDMI_THREAD_DELAY;
    hdmi_dev->attr.app_attr.out_csc_quantization = hdmi_dev->csc_param.quantization;
    hdmi_dev->attr.app_attr.out_color_space = hdmi_dev->csc_param.pixel_encoding;
    video_attr->in_color_space = hdmi_dev->csc_param.pixel_encoding;
    video_attr->colorimetry = hdmi_dev->csc_param.colorimetry;
    audio_attr->sound_intf   = HDMI_AUDIO_INTF_I2S;
    audio_attr->sample_fs    = HDMI_SAMPLE_RATE_48K;
    audio_attr->sample_depth = HDMI_AUDIO_BIT_DEPTH_16;
    audio_attr->channels     = HDMI_AUDIO_FORMAT_2CH;
#if defined(PHY_CRAFT_S28) || defined(HDMI_PRODUCT_SS928V100) || defined(HDMI_PRODUCT_SS626V100)
    hdmi_dev->mode_param.trace_len = HDMI_TRACE_DEFAULT;
#else
    hdmi_dev->mode_param.trace_len = HDMI_TRACE_LEN_1;
#endif

    compatibility_info_default_set(hdmi_dev->hdmi_dev_id);
    hdmi_info("video_timing:%u\n", video_attr->video_timing);

    return TD_SUCCESS;
}
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif

static td_void hdmi_dev_deinit(hdmi_device *hdmi_dev)
{
    hdmi_dev->mode_param.emi_en = TD_FALSE;
    hdmi_dev->hpd_detected      = TD_FALSE;
    hdmi_dev->hpd_notifyed      = TD_FALSE;
    hdmi_dev->run_state         = HDMI_RUN_STATE_NONE;
    hdmi_dev->tmds_mode         = HDMI_TMDS_MODE_NONE;
    hdmi_dev->transition_state  = HDMI_TRANSITION_NONE;
    hdmi_dev->k_callback        = TD_NULL;
    hdmi_dev->hal               = TD_NULL;
    hdmi_dev->kernel_cnt        = 0;
    hdmi_dev->user_callback_cnt = 0;
    hdmi_dev->user_cnt          = 0;

    (td_void)memset_s(&hdmi_dev->attr, sizeof(hdmi_dev->attr), 0, sizeof(hdmi_dev->attr));
    (td_void)memset_s(&hdmi_dev->debug, sizeof(hdmi_dev->debug), 0, sizeof(hdmi_dev->debug));
    (td_void)memset_s(&hdmi_dev->delay, sizeof(hdmi_dev->delay), 0, sizeof(hdmi_dev->delay));
    (td_void)memset_s(&hdmi_dev->edid_info, sizeof(hdmi_dev->edid_info), 0, sizeof(hdmi_dev->edid_info));
    (td_void)memset_s(&hdmi_dev->info_frame, sizeof(hdmi_dev->info_frame), 0, sizeof(hdmi_dev->info_frame));
    (td_void)memset_s(&hdmi_dev->thread_info, sizeof(hdmi_dev->thread_info), 0, sizeof(hdmi_dev->thread_info));

    return;
}

static td_s32 create_task(hdmi_device *hdmi_dev, hdmi_hal_init *hal_cfg)
{
    td_s32 err_num;
    td_s32 ret = TD_SUCCESS;
    td_char thread_name[THREAD_NAME_MAX] = "hdmi0_kthread";

    if (hdmi_dev->thread_info.thread_timer == TD_NULL) {
        hal_cfg->base_addr      = g_hdmi_reg[hdmi_dev->hdmi_dev_id];
        hal_cfg->phy_addr       = g_hdmi_phy[hdmi_dev->hdmi_dev_id];
        hal_cfg->event_callback = hdmi_event_callback;
        hal_cfg->event_data     = hdmi_dev;
        hal_cfg->hdmi_dev_id    = hdmi_dev->hdmi_dev_id;

        ret = hal_hdmi_open(hal_cfg, &hdmi_dev->hal);
        if (hdmi_dev->hal == TD_NULL || ret != TD_SUCCESS) {
            hdmi_err("hal_hdmi_open fail!\n");
            return TD_FAILURE;
        }
        hdmi_dev->hal->hal_ctx.hdmi_dev = hdmi_dev;
        hdmi_dev->hal->hal_ctx.hdmi_id = hdmi_dev->hdmi_dev_id;

        err_num = snprintf_s(thread_name, THREAD_NAME_MAX,
            THREAD_NAME_MAX - 1, "hdmi%u_kthread", hdmi_dev->hdmi_dev_id);
        if (err_num < 0) {
            hdmi_err("snprintf_s err\n");
            return TD_FAILURE;
        }
        hdmi_dev->thread_info.thread_timer = osal_kthread_create(hdmi_kthread_timer, hdmi_dev, thread_name);
        if (hdmi_dev->thread_info.thread_timer == TD_NULL) {
            hdmi_err("create hdmi%u timer thread fail!\n", hdmi_dev->hdmi_dev_id);
            return TD_FAILURE;
        }
        hdmi_info("create %s success.\n", thread_name);
        hdmi_thread_state_set(hdmi_dev, HDMI_THREAD_STATE_RUN);
        hdmi_device_init(hdmi_dev);
    }

    return ret;
}

static td_s32 hdmi_oe_transition_state_set(hdmi_device *hdmi_dev, td_bool user, td_bool hdmi_on)
{
    td_s32 ret = TD_SUCCESS;

#if !defined(HDMI_FPGA_SUPPORT) || defined(HDMI_PRODUCT_SS626V100)
    if (hdmi_on == TD_FALSE) {
        hdmi_info("hdmi not on, do hardware init\n");
        hdmi_dev->transition_state = HDMI_TRANSITION_NONE;
        hal_call_void(hdmi_dev, hal_hdmi_hardware_init);
    } else {
        hdmi_info("hdmi already on, do not hardware init.\n");
        if (user) {
            hdmi_dev->transition_state = (hdmi_dev->kernel_cnt > 0) ?
                HDMI_TRANSITION_MCE_APP : HDMI_TRANSITION_BOOT_APP;
        } else {
            hdmi_dev->transition_state = HDMI_TRANSITION_BOOT_MCE;
        }
    }
#else
    ot_unused(hdmi_on);
    if (user) {
        hdmi_dev->transition_state = (hdmi_dev->kernel_cnt > 0) ?
            HDMI_TRANSITION_MCE_APP : HDMI_TRANSITION_BOOT_APP;
    } else {
        hdmi_dev->transition_state = HDMI_TRANSITION_BOOT_MCE;
    }
#endif

    return ret;
}

static td_void drv_hdmi_hpd_status_delay_get(const hdmi_device *hdmi_dev, td_bool *hotplug)
{
    td_u32 i;

    for (i = 0; i < HDMI_READ_HPD_STATUS_DELAY; i++) {
        hal_call_void(hdmi_dev, hal_hdmi_hot_plug_status_get, hotplug);
        if ((*hotplug) == TD_TRUE) {
            break;
        }
        osal_msleep(HDMI_READ_HPD_STATUS_DELAY_TIME);
    }
    hdmi_info("delay %d times, hot plug status is 0x%x\n", i, (*hotplug ? HDMI_EVENT_HOTPLUG : HDMI_EVENT_HOTUNPLUG));

    return;
}

static td_void notify_event_in_open(hdmi_device *hdmi_dev, td_bool hdmi_on, td_bool hotplug)
{
    if (hdmi_on == TD_TRUE) { /* need to updata event in open, when there is boot logo. */
        hdmi_event_callback(hdmi_dev, hotplug ? HDMI_EVENT_HOTPLUG : HDMI_EVENT_HOTUNPLUG);
    } else {
        if (hotplug == TD_TRUE) {
            hpd_event(hdmi_dev);
        } else {
            hdmi_dev->hpd_detected = TD_FALSE;
        }
    }

    return;
}

static td_s32 drv_hdmi_open(hdmi_device *hdmi_dev, td_bool user)
{
    td_s32 ret;
    hdmi_hal_init hal_cfg = {0};
    td_u32 event_pool_id = 0;
    td_bool hotplug = TD_FALSE;
    td_bool hdmi_on = TD_FALSE;

    /* create hdmi task, every hdmi device only create once */
    if (create_task(hdmi_dev, &hal_cfg) != TD_SUCCESS) {
        return OT_ERR_HDMI_CREATE_TESK_FAILED;
    }

    drv_hdmi_event_init(hdmi_dev->hdmi_dev_id);

    if (user == TD_TRUE) {
        /* must create event queue first */
        ret = drv_hdmi_event_pool_malloc(hdmi_dev->hdmi_dev_id, &event_pool_id);
        if (ret != TD_SUCCESS) {
            if (ret != HDMI_EVENT_ID_EXIST) {
                hdmi_err("drv_hdmi_event_pool_malloc fail\n");
                return OT_ERR_HDMI_MALLOC_FAILED;
            }
        } else {
            hdmi_dev->user_cnt++;
        }
        hdmi_info("create event queue for process:%u \n", hdmi_current_id_get(hdmi_dev));
    } else {
        hdmi_dev->kernel_cnt++;
    }

    if (hdmi_dev->user_callback_cnt == 0) {
        hdmi_mutex_unlock(g_hdmi_mutex);
        hdmi_mutex_lock(hdmi_dev->mutex_thread);
        hdmi_dev->k_callback = drv_hdmi_kernel_event_callback;
        hdmi_mutex_unlock(hdmi_dev->mutex_thread);
        hdmi_mutex_lock(g_hdmi_mutex);
    }
    hdmi_info("user_cnt: %u, kernel_cnt: %u, user_call_back_cnt: %u\n",
              hdmi_dev->user_cnt, hdmi_dev->kernel_cnt, hdmi_dev->user_callback_cnt);

    hal_call_void(hdmi_dev, hal_hdmi_phy_output_enable_get, &hdmi_on);
    ret = hdmi_oe_transition_state_set(hdmi_dev, user, hdmi_on);
    hdmi_if_failure_return(ret, TD_FAILURE);
    /* insure that get sink caps success immediately after open HDMI when no boot or no output. */
    drv_hdmi_hpd_status_delay_get(hdmi_dev, &hotplug);
    notify_event_in_open(hdmi_dev, hdmi_on, hotplug);
    hdmi_info("transition_state: %u\n", hdmi_dev->transition_state);
    hdmi_dev->run_state = HDMI_RUN_STATE_OPEN;

    return TD_SUCCESS;
}

static td_s32 hdmi_release(hdmi_device *hdmi_dev)
{
    hdmi_info("in\n");

    if ((td_u32)hdmi_dev->run_state & HDMI_RUN_STATE_START) {
        drv_hdmi_stop(hdmi_dev);
    }

    if (hdmi_dev->user_cnt == 0) {
        if (hdmi_dev->kernel_cnt == 0) {
            if (hdmi_dev->thread_info.thread_timer != TD_NULL) {
                hdmi_info("stop hdmi kernel thread timer\n");
                /*
                 * note: in linux,
                 * when call the kthread_stop function,
                 * the thread function cannot be finished, otherwise it will oops.
                 */
                osal_kthread_destroy(hdmi_dev->thread_info.thread_timer, TD_TRUE);
                hdmi_dev->thread_info.thread_timer = TD_NULL;
                hdmi_thread_state_set(hdmi_dev, HDMI_THREAD_STATE_STOP);
            }
            drv_hdmi_event_deinit(hdmi_dev->hdmi_dev_id);
            hdmi_mutex_lock(hdmi_dev->mutex_proc);
            if (hdmi_dev->hal != TD_NULL) {
                hal_hdmi_close(hdmi_dev->hal);
                hdmi_dev->hal = TD_NULL;
            }
            hdmi_dev_deinit(hdmi_dev);
            hdmi_mutex_unlock(hdmi_dev->mutex_proc);
        } else {
            drv_hdmi_event_deinit(hdmi_dev->hdmi_dev_id);
            hdmi_mutex_unlock(g_hdmi_mutex);
            hdmi_mutex_lock(hdmi_dev->mutex_thread);
            hdmi_dev->k_callback = drv_hdmi_kernel_event_callback;
            hdmi_mutex_unlock(hdmi_dev->mutex_thread);
            hdmi_mutex_lock(g_hdmi_mutex);
        }
    }
    hdmi_info("out\n");

    return TD_SUCCESS;
}

static td_s32 drv_hdmi_close(hdmi_device *hdmi_dev, td_bool user)
{
    hdmi_info("in\n");
    hdmi_check_open_return((td_u32)hdmi_dev->run_state);

    hdmi_info("user: %u\n", user);
    hdmi_info("user_cnt: %u, kernel_cnt: %u\n", hdmi_dev->user_cnt, hdmi_dev->kernel_cnt);

    hdmi_dev->hpd_notifyed = TD_FALSE;

    if (user == TD_TRUE && hdmi_dev->user_cnt > 0) {
        hdmi_info("delete event queue for process:%u \n", hdmi_current_id_get(hdmi_dev));
        if (drv_hdmi_event_pool_free(hdmi_dev->hdmi_dev_id, hdmi_current_id_get(hdmi_dev)) != TD_SUCCESS) {
            return OT_ERR_HDMI_FREE_FAILED;
        }
        hdmi_dev->user_cnt--;
        if (hdmi_dev->user_cnt == 0 && hdmi_dev->kernel_cnt > 0) {
            hdmi_mutex_unlock(g_hdmi_mutex);
            hdmi_mutex_lock(hdmi_dev->mutex_thread);
            hdmi_dev->k_callback = drv_hdmi_kernel_event_callback;
            hdmi_mutex_unlock(hdmi_dev->mutex_thread);
            hdmi_mutex_lock(g_hdmi_mutex);
        } else {
            hdmi_mutex_unlock(g_hdmi_mutex);
            hdmi_mutex_lock(hdmi_dev->mutex_thread);
            hdmi_dev->k_callback = TD_NULL;
            hdmi_mutex_unlock(hdmi_dev->mutex_thread);
            hdmi_mutex_lock(g_hdmi_mutex);
        }
    } else if (user == TD_FALSE && hdmi_dev->kernel_cnt > 0) {
        hdmi_dev->kernel_cnt--;
    }

    if (hdmi_dev->user_cnt == 0 && hdmi_dev->kernel_cnt == 0) {
        hdmi_release(hdmi_dev);
        hdmi_dev->run_state = HDMI_RUN_STATE_CLOSE;
    }

    hdmi_info("out\n");

    return TD_SUCCESS;
}

static td_void drv_hdmi_audio_path_enable(const hdmi_device *hdmi_dev, td_bool enable)
{
    hal_call_void(hdmi_dev, hal_hdmi_audio_path_enable_set, enable);
    return;
}

static td_void hdmi_debug_delay(const hdmi_device *hdmi_dev, const td_char *info)
{
    osal_msleep(hdmi_dev->debug.stop_delay);

    if (hdmi_dev->debug.stop_delay > 0) {
        hdmi_info("%s, delay %ums\n", info, hdmi_dev->debug.stop_delay);
    }
}

static td_s32 check_audio_attr(const hdmi_ao_attr *ao_attr)
{
#endif /* SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART03_H_ */
