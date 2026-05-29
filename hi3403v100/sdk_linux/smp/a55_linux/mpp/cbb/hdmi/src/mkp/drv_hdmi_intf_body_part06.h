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

#ifndef SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART06_H_
#define SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART06_H_

    hdmi_if_failure_return(ret, OT_ERR_HDMI_FEATURE_NO_SUPPORT);

    app_attr = &hdmi_dev->attr.app_attr;
    ret = hdmi_property_backup_attr(hdmi_dev, &tmp, &vo_attr, &ao_attr);
    hdmi_if_failure_return(ret, OT_ERR_HDMI_INVALID_PARA);

    hdmi_property_apply_app_attr(app_attr, prop);
    hdmi_property_vo_attr_adapt(&vo_attr, prop);
    ret = drv_hdmi_vo_attr_set(hdmi_dev, &vo_attr);
    if (ret != TD_SUCCESS) {
        goto err;
    }

    ao_attr.sample_fs    = prop->sample_rate;
    ao_attr.sample_depth = prop->bit_depth;
    ret = drv_hdmi_ao_attr_set(hdmi_dev, &ao_attr);
    if (ret != TD_SUCCESS) {
        goto err;
    }

    return TD_SUCCESS;

err:
    /* property set failed, recover the app_attr */
    err_code = memcpy_s(app_attr, sizeof(hdmi_dev->attr.app_attr), &tmp, sizeof(hdmi_app_attr));
    if (err_code != EOK) {
        hdmi_err("secure function error:%d\n", err_code);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    return ret;
}

static td_s32 drv_hdmi_property_get(hdmi_device_id hdmi_id, hdmi_property *prop)
{
    hdmi_attr *attr = TD_NULL;
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);

    attr = &hdmi_dev->attr;
    prop->enable_hdmi = attr->app_attr.enable_hdmi;
    prop->enable_video = attr->app_attr.enable_video;
    prop->enable_audio = attr->app_attr.enable_audio;
    prop->video_timing = attr->vo_attr.video_timing;
    prop->disp_fmt = attr->vo_attr.disp_fmt;
    prop->enable_avi_infoframe = attr->app_attr.enable_avi_infoframe;
    prop->enable_aud_infoframe = attr->app_attr.enable_aud_infoframe;
    prop->enable_vid_mode_adapt = attr->app_attr.enable_clr_space_adapt;
    prop->enable_deep_clr_adapt = attr->app_attr.enable_deep_clr_adapt;
    prop->auth_mode = attr->app_attr.auth_mode;
    prop->in_color_space = attr->vo_attr.in_color_space;
    prop->out_color_space = attr->app_attr.out_color_space;
    prop->deep_color_mode = attr->app_attr.deep_color_mode;
    prop->out_csc_quantization = attr->app_attr.out_csc_quantization;
    prop->hdmi_action = attr->app_attr.hdmi_action;
    prop->sample_rate = attr->ao_attr.sample_fs;
    prop->pix_clk = attr->vo_attr.clk_fs;
    prop->bit_depth = attr->ao_attr.sample_depth;

    return TD_SUCCESS;
}

static td_s32 hdmi_ioctrl_open(td_void *arg, td_bool user)
{
    td_s32 ret;
    hdmi_open *open = TD_NULL;
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_if_null_return(arg, OT_ERR_HDMI_NULL_PTR);

    open = (hdmi_open *)arg;
    hdmi_if_null_return(open, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(open->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_OPEN user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    get_hdmi_default_action_set(hdmi_dev, open->default_mode);
    ret = drv_hdmi_open(hdmi_dev, user);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_close(td_void *arg, td_bool user)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_if_null_return(arg, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(*(hdmi_device_id *)arg);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_CLOSE user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_close(hdmi_dev, user);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_start(td_void *arg, td_bool user)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_if_null_return(arg, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(*(hdmi_device_id *)arg);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_START user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_start(hdmi_dev);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_stop(td_void *arg, td_bool user)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_if_null_return(arg, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(*(hdmi_device_id *)arg);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_STOP user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_stop(hdmi_dev);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_get_sink_capability(td_void *arg, td_bool user)
{
    td_s32 ret;
    errno_t err_code;
    hdmi_tx_capability_data tx_cap = {0};
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_sink_capability *sink_cap = TD_NULL;
    drv_hdmi_sink_capability *drv_sink_cap = TD_NULL;

    ot_unused(user);

    drv_sink_cap = (drv_hdmi_sink_capability *)arg;
    hdmi_if_null_return(drv_sink_cap, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(drv_sink_cap->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_GET_SINK_CAPABILITY user=%u\n", hdmi_dev->hdmi_dev_id, user);

    (td_void)memset_s(&drv_sink_cap->cap, sizeof(drv_sink_cap->cap), 0, sizeof(hdmi_sink_capability));
    hdmi_mutex_lock(g_hdmi_mutex);
    if (hdmi_dev->hal == TD_NULL || hdmi_dev->hal->hal_hdmi_tx_capability_get == TD_NULL) {
        hdmi_err("null pointer!\n");
        hdmi_mutex_unlock(g_hdmi_mutex);
        return OT_ERR_HDMI_NULL_PTR;
    }
    hdmi_dev->hal->hal_hdmi_tx_capability_get(hdmi_dev->hal, &tx_cap);
    if (drv_hdmi_edid_capability_get(&hdmi_dev->edid_info, &sink_cap) != HDMI_EDID_DATA_INVALID) {
        err_code = memcpy_s(&drv_sink_cap->cap, sizeof(drv_sink_cap->cap), sink_cap, sizeof(hdmi_sink_capability));
        hdmi_unlock_unequal_eok_return(err_code, g_hdmi_mutex, OT_ERR_HDMI_INVALID_PARA);
        ret = hdmi_capability_inter_section(&drv_sink_cap->cap, &tx_cap, hdmi_dev->attr.app_attr.auth_mode);
    } else {
        ret = OT_ERR_HDMI_READ_SINK_FAILED;
        hdmi_warn("no HPD, get sink capabity fail\n");
    }
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_poll_event(td_void *arg, td_bool user)
{
    drv_hdmi_event *event = TD_NULL;
    hdmi_device *hdmi_dev = TD_NULL;

    ot_unused(user);
    event = (drv_hdmi_event *)arg;
    hdmi_if_null_return(event, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(event->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    return drv_hdmi_event_pool_read(event->hdmi_id, hdmi_current_id_get(hdmi_dev), &event->event);
}

static td_s32 hdmi_ioctrl_set_attr(td_void *arg, td_bool user)
{
    td_s32 ret;
    drv_hdmi_property *attr = TD_NULL;
    hdmi_device *hdmi_dev = TD_NULL;

    attr = (drv_hdmi_property *)arg;
    hdmi_if_null_return(attr, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(attr->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_SET_ATTR user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_property_set(attr->hdmi_id, &attr->prop);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_get_attr(td_void *arg, td_bool user)
{
    td_s32 ret;
    drv_hdmi_property *attr = TD_NULL;

    attr = (drv_hdmi_property *)arg;
    hdmi_if_null_return(attr, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(attr->hdmi_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_GET_ATTR user=%u\n", attr->hdmi_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_property_get(attr->hdmi_id, &attr->prop);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_get_vo_attr(td_void *arg, td_bool user)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;
    drv_hdmi_vo_attr *attr = TD_NULL;

    attr = (drv_hdmi_vo_attr *)arg;
    hdmi_if_null_return(attr, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(attr->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);
    (td_void)memset_s(&attr->vo_attr, sizeof(attr->vo_attr), 0, sizeof(hdmi_vo_attr));

    hdmi_info("hdmi_id:%d, CMD_HDMI_GET_VO_ATTR user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_vo_attr_get(hdmi_dev, &attr->vo_attr);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_set_vo_attr(td_void *arg, td_bool user)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;
    drv_hdmi_vo_attr *attr = TD_NULL;

    attr = (drv_hdmi_vo_attr *)arg;
    hdmi_if_null_return(attr, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(attr->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_SET_VO_ATTR user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_vo_attr_set(hdmi_dev, &attr->vo_attr);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_get_infoframe(td_void *arg, td_bool user)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;
    drv_hdmi_infoframe *tmp = TD_NULL;

    tmp = (drv_hdmi_infoframe *)arg;
    hdmi_if_null_return(tmp, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(tmp->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_GET_INFOFRAME user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_infoframe_get(hdmi_dev, tmp->infoframe_id, &tmp->infoframe);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_set_infoframe(td_void *arg, td_bool user)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;
    drv_hdmi_infoframe *infoframe = TD_NULL;

    infoframe = (drv_hdmi_infoframe *)arg;
    hdmi_if_null_return(infoframe, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(infoframe->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_SET_INFOFRAME user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_infoframe_set(hdmi_dev, infoframe->infoframe_id, &infoframe->infoframe);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_avmute(td_void *arg, td_bool user)
{
    td_bool audio_enable;
    hdmi_device *hdmi_dev = TD_NULL;
    drv_hdmi_avmute *drv_avmute = TD_NULL;

    drv_avmute = (drv_hdmi_avmute *)arg;
    hdmi_if_null_return(drv_avmute, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(drv_avmute->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_SET_AVMUTE user=%u\n", hdmi_dev->hdmi_dev_id, user);

    audio_enable = (hdmi_dev->attr.app_attr.enable_hdmi) && (hdmi_dev->attr.app_attr.enable_audio);

    hdmi_mutex_lock(g_hdmi_mutex);
    drv_hdmi_avmute_set(hdmi_dev, drv_avmute->avmute);
    if (drv_avmute->avmute) {
        drv_hdmi_audio_path_enable(hdmi_dev, !drv_avmute->avmute);
    } else {
        drv_hdmi_audio_path_enable(hdmi_dev, audio_enable);
    }
    hdmi_mutex_unlock(g_hdmi_mutex);

    return TD_SUCCESS;
}

static td_s32 hdmi_ioctrl_update_edid(td_void *arg, td_bool user)
{
    td_s32 ret;
    td_s32 edid_len;
    hdmi_device *hdmi_dev = TD_NULL;
    drv_hdmi_edid_raw_data *drv_edid_raw = TD_NULL;

    drv_edid_raw = (drv_hdmi_edid_raw_data *)arg;
    hdmi_if_null_return(drv_edid_raw, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(drv_edid_raw->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_UPDATE_EDID user=%u\n", hdmi_dev->hdmi_dev_id, user);

    (td_void)memset_s(&drv_edid_raw->edid_raw, sizeof(drv_edid_raw->edid_raw), 0, sizeof(hdmi_edid_raw_data));
    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_edid_update(&hdmi_dev->edid_info, HDMI_EDID_UPDATE_SINK);
    if (ret == TD_SUCCESS) {
        edid_len = drv_hdmi_edid_raw_get(&hdmi_dev->edid_info, drv_edid_raw->edid_raw.edid, HDMI_EDID_SIZE);
        if (edid_len != OT_ERR_HDMI_NULL_PTR && edid_len != OT_ERR_HDMI_INVALID_PARA && edid_len != TD_FAILURE) {
            drv_edid_raw->edid_raw.edid_len = (td_u32)edid_len;
            drv_edid_raw->edid_raw.edid_valid = 1;
        }
    }
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_get_status(td_void *arg, td_bool user)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;
    drv_hdmi_status *hdmi_state = TD_NULL;

    hdmi_state = (drv_hdmi_status *)arg;
    hdmi_if_null_return(hdmi_state, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(hdmi_state->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_GET_STATUS user=%u\n", hdmi_dev->hdmi_dev_id, user);

    (td_void)memset_s(&hdmi_state->status, sizeof(hdmi_state->status), 0, sizeof(hdmi_status));
    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_status_get(hdmi_dev, &hdmi_state->status);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_register_callback(td_void *arg, td_bool user)
{
    td_bool hpd = TD_FALSE;
    td_bool intr_status = TD_FALSE;
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_if_null_return(arg, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(*(hdmi_device_id *)arg);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_REGISTER_CALLBACK user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    if ((hdmi_dev->user_cnt != 0) || (hdmi_dev->kernel_cnt != 0)) {
        hdmi_dev->user_callback_cnt++;
        hdmi_mutex_unlock(g_hdmi_mutex);
        hdmi_mutex_lock(hdmi_dev->mutex_thread);
        hdmi_dev->k_callback = TD_NULL;
        /*
         * HPD needs to be reported when the following conditions are met:
         * 1. the driver has reported HPD before registering the callback.
         * 2. there is currently no pending hpd interrupt.
         * 3. hotplug pin level is high.
         */
        hal_call_void(hdmi_dev, hal_hdmi_hdp_intr_status_get, &intr_status);


#endif /* SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART06_H_ */
