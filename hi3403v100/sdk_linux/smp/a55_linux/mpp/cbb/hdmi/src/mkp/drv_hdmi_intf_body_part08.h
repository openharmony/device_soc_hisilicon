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

#ifndef SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART08_H_
#define SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART08_H_

    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_check_open_return((td_u32)hdmi_dev->run_state);

    app_attr = &hdmi_dev->attr.app_attr;
    audio_enable = (hdmi_dev->attr.app_attr.enable_audio && app_attr->enable_hdmi);
#if defined(HDMI_PRODUCT_SS626V100)
    drv_hdmi_low_power_set(hdmi_dev->hdmi_dev_id, TD_FALSE);
#else
    drv_hdmi_low_power_set(TD_FALSE);
#endif

    if (hdmi_mode_strategy(hdmi_dev) != TD_SUCCESS) {
        hdmi_err(" hdmi_mode_strategy fail\n");
    }

    /*
     * 20160415, fix SAMSUNG TV(UA55JU6400JXXZ) doesn't show at 10bit/12bit.
     * this TV clear mute must after phy enable when the deepcolor is 10bit and 12bit.
     */
    hal_call_void(hdmi_dev, hal_hdmi_avmute_set, TD_FALSE);

    hdmi_info("TMDS phy config\n");
    hdmi_phy_output_enable(hdmi_dev, TD_TRUE);
    drv_hdmi_avmute_set(hdmi_dev, TD_FALSE);
    drv_hdmi_audio_path_enable(hdmi_dev, audio_enable);

    if (hdmi_dev->attr.app_attr.enable_video == TD_FALSE) {
        hdmi_info("video was disable by user, so send blackframe.\n");
        drv_hdmi_black_data_set(hdmi_dev, TD_TRUE);
    } else {
        drv_hdmi_black_data_set(hdmi_dev, TD_FALSE);
    }
    hdmi_dev->run_state = (td_u32)hdmi_dev->run_state & (~(HDMI_RUN_STATE_STOP));
    hdmi_dev->run_state = (td_u32)hdmi_dev->run_state | (HDMI_RUN_STATE_START);

    return TD_SUCCESS;
}

td_s32 drv_hdmi_stop(hdmi_device *hdmi_dev)
{
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);

    hdmi_debug_delay(hdmi_dev, "drv_hdmi_stop debug start");
    if (!((td_u32)hdmi_dev->run_state & HDMI_RUN_STATE_START) || ((td_u32)hdmi_dev->run_state & HDMI_RUN_STATE_STOP)) {
        return TD_SUCCESS;
    }
    hdmi_debug_delay(hdmi_dev, "drv_hdmi_stop audio path disable");
    drv_hdmi_audio_path_enable(hdmi_dev, TD_FALSE);
    hdmi_debug_delay(hdmi_dev, "drv_hdmi_stop black data enable");
    drv_hdmi_black_data_set(hdmi_dev, TD_TRUE);
    hdmi_debug_delay(hdmi_dev, "drv_hdmi_stop avmute enable");
    drv_hdmi_avmute_set(hdmi_dev, TD_TRUE);
    hdmi_debug_delay(hdmi_dev, "drv_hdmi_stop phy disable");
    hdmi_phy_output_enable(hdmi_dev, TD_FALSE);
    /* disable clk for low power */
#if defined(HDMI_PRODUCT_SS626V100)
    drv_hdmi_low_power_set(hdmi_dev->hdmi_dev_id, TD_TRUE);
#else
    drv_hdmi_low_power_set(TD_TRUE);
#endif
    hdmi_dev->run_state = (td_u32)hdmi_dev->run_state  & (~(HDMI_RUN_STATE_START));
    hdmi_dev->run_state = (td_u32)hdmi_dev->run_state | HDMI_RUN_STATE_STOP;
    drv_hdmi_compat_stop_delay(hdmi_dev->hdmi_dev_id);
    hdmi_debug_delay(hdmi_dev, "stop delay");

    return TD_SUCCESS;
}

td_void drv_hdmi_avmute_set(const hdmi_device *hdmi_dev, td_bool avmute_en)
{
    hdmi_delay delay = {0};

    hdmi_if_null_return_void(hdmi_dev);

    hdmi_info("avmute: %u\n", avmute_en);

    delay.fmt_delay  = hdmi_dev->delay.fmt_delay;
    delay.mute_delay = hdmi_dev->delay.mute_delay;
    drv_hdmi_compat_delay_get(hdmi_dev->hdmi_dev_id, &delay);

    if (hdmi_dev->attr.app_attr.auth_mode != TD_TRUE) {
        if (avmute_en == TD_FALSE) {
            hdmi_info("format_delay %u ms \n", delay.fmt_delay);
            osal_msleep(delay.fmt_delay);
        }
    }
    hal_call_void(hdmi_dev, hal_hdmi_avmute_set, avmute_en);

    if (hdmi_dev->attr.app_attr.auth_mode != TD_TRUE) {
        if (avmute_en == TD_TRUE) {
            hdmi_info("mute_delay %u ms \n", delay.mute_delay);
            osal_msleep(delay.mute_delay);
        }
    }

    return;
}

td_s32 hdmi_thread_state_set(hdmi_device *hdmi_dev, hdmi_thread_state state)
{
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);

    hdmi_info("state: %u\n", state);
    hdmi_dev->thread_info.thread_timer_sate = state;

    return TD_SUCCESS;
}

td_s32 drv_hdmi_vo_attr_set(hdmi_device *hdmi_dev, const hdmi_vo_attr *vo_attr)
{
    errno_t ret;
    hdmi_attr hw_attr = {0};
    hdmi_vo_attr *video_attr = TD_NULL;
    hdmi_app_attr *app_attr = TD_NULL;
    hdmi_video_timing timing_bak;

    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(vo_attr, OT_ERR_HDMI_NULL_PTR);

    if (check_video_attr(vo_attr) != TD_SUCCESS) {
        return OT_ERR_HDMI_INVALID_PARA;
    }
    video_attr = &hdmi_dev->attr.vo_attr;
    app_attr   = &hdmi_dev->attr.app_attr;
    timing_bak = video_attr->video_timing;
    ret = memcpy_s(video_attr, sizeof(hdmi_dev->attr.vo_attr), vo_attr, sizeof(hdmi_vo_attr));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    hdmi_vo_attr_info(hdmi_dev, video_attr);
    if (hdmi_format_is_change(hdmi_dev, video_attr, vo_attr) != TD_SUCCESS) {
        return OT_ERR_HDMI_STRATEGY_FAILED;
    }

    hdmi_attr_construct(hdmi_dev, &hw_attr);
    hw_attr.vo_attr.video_timing = timing_bak;
    if (vo_attr_is_changed(&hw_attr.vo_attr, video_attr) == TD_FALSE &&
        app_attr_is_changed(&hw_attr.app_attr, app_attr) == TD_FALSE) {
        hdmi_info("the video and app attr is not changed\n");
        return TD_SUCCESS;
    }
    if (app_attr->enable_video == TD_TRUE) {
        hdmi_video_path_set(hdmi_dev, video_attr);
    }
    /*
     * HDMI is not support dither on h7.
     * set output bit to VO. VO decide to whether enable dither or not.
     */
    drv_hdmi_avi_infoframe_send(&hdmi_dev->info_frame, (app_attr->enable_hdmi && app_attr->enable_avi_infoframe));
    drv_hdmi_vendor_infoframe_send(&hdmi_dev->info_frame, app_attr->enable_hdmi);

    return TD_SUCCESS;
}

td_s32 drv_hdmi_attr_get(const hdmi_device *hdmi_dev, hdmi_attr *attr)
{
    errno_t ret;

    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(attr, OT_ERR_HDMI_NULL_PTR);

    ret = memcpy_s(attr, sizeof(*attr), &hdmi_dev->attr, sizeof(hdmi_attr));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    return TD_SUCCESS;
}

td_s32 drv_hdmi_attr_set(hdmi_device *hdmi_dev, hdmi_attr *attr)
{
    errno_t ret;
    hdmi_app_attr *app_attr = TD_NULL;

    hdmi_if_null_return(attr, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);

    app_attr = &hdmi_dev->attr.app_attr;
    hdmi_info("enable_hdmi: %u\n", attr->app_attr.enable_hdmi);
    hdmi_info("enable_video: %u\n", attr->app_attr.enable_video);
    hdmi_info("enable_audio: %u\n", attr->app_attr.enable_audio);
    hdmi_info("out_color_space: %u\n", attr->app_attr.out_color_space);
    hdmi_info("deep_color_mode: %u\n", attr->app_attr.deep_color_mode);
    hdmi_info("bxv_ycc_mode: %u\n", attr->app_attr.xvycc_mode);
    hdmi_info("enable_avi_info_frame: %u\n", attr->app_attr.enable_avi_infoframe);
    hdmi_info("enable_spd_info_frame: %u\n", attr->app_attr.enable_spd_infoframe);
    hdmi_info("enable_mpeg_info_frame: %u\n", attr->app_attr.enable_mpeg_infoframe);
    hdmi_info("enable_aud_info_frame: %u\n", attr->app_attr.enable_aud_infoframe);
    hdmi_info("debug_flag: %u\n", attr->app_attr.debug_flag);
    hdmi_info("hdcp_mode: %u\n", attr->app_attr.hdcp_mode);
    hdmi_info("enable_clr_space_adapt: %u\n", attr->app_attr.enable_clr_space_adapt);
    hdmi_info("enable_deep_clr_adapt: %u\n", attr->app_attr.enable_deep_clr_adapt);
    hdmi_info("auth_mode: %u\n", attr->app_attr.auth_mode);
    hdmi_info("out_csc_quantization: %u\n", attr->app_attr.out_csc_quantization);

    if (check_app_attr(&attr->app_attr) != TD_SUCCESS) {
        hdmi_err("check hdmi attr fail\n");
        return OT_ERR_HDMI_INVALID_PARA;
    }
    if (hdmi_color_and_bit_strategy(hdmi_dev, &attr->app_attr, &attr->vo_attr) != TD_SUCCESS) {
        hdmi_err(" hdmi_color_and_bit_strategy fail\n");
        return OT_ERR_HDMI_STRATEGY_FAILED;
    }
    ret = memcpy_s(app_attr, sizeof(hdmi_dev->attr.app_attr), &attr->app_attr, sizeof(hdmi_app_attr));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    if (drv_hdmi_ao_attr_set(hdmi_dev, &attr->ao_attr) != TD_SUCCESS) {
        hdmi_err("drv_hdmi_ao_attr_set fail\n");
        return OT_ERR_HDMI_SET_ATTR_FAILED;
    }
    if (drv_hdmi_vo_attr_set(hdmi_dev, &attr->vo_attr) != TD_SUCCESS) {
        hdmi_err("drv_hdmi_vo_attr_set fail\n");
        return OT_ERR_HDMI_SET_ATTR_FAILED;
    }

    return TD_SUCCESS;
}

td_void get_hdmi_default_action_set(hdmi_device *hdmi_dev, hdmi_default_action action)
{
    hdmi_if_null_return_void(hdmi_dev);
    hdmi_dev->attr.app_attr.hdmi_action = action;

    return;
}

td_s32 drv_hdmi_cmd_process(unsigned int cmd, td_void *arg, td_bool user)
{
    td_u32 index;
    td_s32 ret = TD_FAILURE;

    hdmi_if_null_return(arg, OT_ERR_HDMI_NULL_PTR);

    for (index = 0; index < hdmi_array_size(g_hdmi_cmd_func_tab); index++) {
        if ((g_hdmi_cmd_func_tab[index].cmd == cmd) && (g_hdmi_cmd_func_tab[index].hdmi_ioctrl_func != TD_NULL)) {
            ret = g_hdmi_cmd_func_tab[index].hdmi_ioctrl_func(arg, user);
            break;
        }
    }

    if (index == hdmi_array_size(g_hdmi_cmd_func_tab)) {
        hdmi_err("unknown cmd:0x%x\n", cmd);
        return OT_ERR_HDMI_UNKNOWN_COMMAND;
    }

    return ret;
}

td_s32 hdmi_set_reg(td_u32 id, td_char *reg)
{
    if (id >= HDMI_ID_MAX) {
        return TD_FAILURE;
    }
    g_hdmi_reg[id] = reg;

    return TD_SUCCESS;
}

td_s32 hdmi_set_phy(td_u32 id, td_char *phy)
{
    if (id >= HDMI_ID_MAX) {
        return TD_FAILURE;
    }
    g_hdmi_phy[id] = phy;

    return TD_SUCCESS;
}

hdmi_device *get_hdmi_device(hdmi_device_id hdmi_id)
{
    if ((td_u32)hdmi_id < HDMI_ID_MAX) {
        g_hdmi_ctrl[hdmi_id].hdmi_dev_id = hdmi_id;
        return &g_hdmi_ctrl[hdmi_id];
    }
    return TD_NULL;
}

td_s32 hdmi_drv_mod_init(td_void)
{
    td_s32 i;
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;

    /* init csc param. */
    for (i = HDMI_DEVICE_ID0; i < HDMI_ID_MAX; i++) {
        hdmi_dev = get_hdmi_device(i);
        if (hdmi_dev != TD_NULL) {
            hdmi_dev->csc_param.colorimetry = HDMI_COLORIMETRY_ITU709;
            hdmi_dev->csc_param.pixel_encoding = HDMI_EXT_COLORSPACE_YCBCR444;
            hdmi_dev->csc_param.quantization = HDMI_QUANT_RANGE_LIMITED;
            osal_sema_init(&hdmi_dev->mutex_proc, 1);
            osal_sema_init(&hdmi_dev->mutex_thread, 1);
        }
    }

    osal_sema_init(&g_hdmi_mutex, 1);
    osal_atomic_init(&g_hdmi_count);
    osal_atomic_set(&g_hdmi_count, 0);

    ret = hdmi_dev_register();
    if (ret == TD_FAILURE) {
        goto exit_dev;
    }
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    hdmi_proc_register();
#endif
    OT_PRINT("load hdmi.ko OK!\n");

    return TD_SUCCESS;

exit_dev:
    osal_destroydev(g_hdmi_device);
    cmpi_unregister_module(OT_ID_HDMI);

    return TD_FAILURE;
}

td_void hdmi_drv_mod_exit(td_void)
{
    td_u32 hdmi_id;
    hdmi_device *hdmi_dev = TD_NULL;

    for (hdmi_id = 0; hdmi_id < HDMI_ID_MAX; hdmi_id++) {
        hdmi_dev = get_hdmi_device(hdmi_id);
        if (hdmi_dev != TD_NULL) {
            if ((td_u32)hdmi_dev->run_state & HDMI_RUN_STATE_OPEN) {
                hdmi_dev->user_cnt = 0;
                hdmi_dev->kernel_cnt = 0;
                hdmi_release(hdmi_dev);
            }
            osal_sema_destroy(&hdmi_dev->mutex_proc);
            osal_sema_destroy(&hdmi_dev->mutex_thread);
        }
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
        drv_hdmi_proc_un_register(hdmi_id);
#endif
        drv_hdmi_hardware_reset(hdmi_id);
    }

    cmpi_unregister_module(OT_ID_HDMI);
    osal_deregisterdevice(g_hdmi_device);
    osal_destroydev(g_hdmi_device);
    osal_atomic_destroy(&g_hdmi_count);
    osal_sema_destroy(&g_hdmi_mutex);

    OT_PRINT("unload hdmi.ko OK!\n");

    return;
}


#endif /* SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART08_H_ */
