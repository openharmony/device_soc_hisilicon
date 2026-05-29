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

#ifndef SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART07_H_
#define SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART07_H_

        if (hdmi_dev->hpd_notifyed == TD_TRUE && intr_status == TD_FALSE) {
            hal_call_void(hdmi_dev, hal_hdmi_hot_plug_status_get, &hpd);
            if (hpd == TD_TRUE) {
                drv_hdmi_event_pool_write(hdmi_dev->hdmi_dev_id, HDMI_EVENT_HOTPLUG);
            }
        }
        hdmi_mutex_unlock(hdmi_dev->mutex_thread);
        hdmi_mutex_lock(g_hdmi_mutex);
    }
    hdmi_mutex_unlock(g_hdmi_mutex);

    return TD_SUCCESS;
}

static td_s32 hdmi_ioctrl_ungister_callback(td_void *arg, td_bool user)
{
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_if_null_return(arg, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(*(hdmi_device_id *)arg);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_UNREGISTER_CALLBACK user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    if (hdmi_dev->user_callback_cnt > 0) {
        hdmi_dev->user_callback_cnt--;
    }
    if (hdmi_dev->user_callback_cnt == 0) {
        hdmi_mutex_unlock(g_hdmi_mutex);
        hdmi_mutex_lock(hdmi_dev->mutex_thread);
        hdmi_dev->k_callback = drv_hdmi_kernel_event_callback;
        hdmi_mutex_unlock(hdmi_dev->mutex_thread);
        hdmi_mutex_lock(g_hdmi_mutex);
    }
    hdmi_mutex_unlock(g_hdmi_mutex);

    return TD_SUCCESS;
}

static td_s32 hdmi_ioctrl_set_mod_param(td_void *arg, td_bool user)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;
    drv_hdmi_mod_param *mod_param = TD_NULL;

    mod_param = (drv_hdmi_mod_param *)arg;
    hdmi_if_null_return(mod_param, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(mod_param->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_SET_MOD_PARAM user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_mod_param_set(hdmi_dev, mod_param);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_get_mod_param(td_void *arg, td_bool user)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;
    drv_hdmi_mod_param *mod_param = TD_NULL;

    mod_param = (drv_hdmi_mod_param *)arg;
    hdmi_if_null_return(mod_param, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(mod_param->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_GET_MOD_PARAM user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_mod_param_get(hdmi_dev, mod_param);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_get_hw_spec(td_void *arg, td_bool user)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;
    drv_hdmi_hw_spec *spec = TD_NULL;

    spec = (drv_hdmi_hw_spec *)arg;
    hdmi_if_null_return(spec, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(spec->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_GET_HW_SPEC user=%u\n", hdmi_dev->hdmi_dev_id, user);

    (td_void)memset_s(&spec->hw_spec, sizeof(spec->hw_spec), 0, sizeof(hdmi_hw_spec));
    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_hw_spec_get(hdmi_dev, &spec->hw_spec);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

static td_s32 hdmi_ioctrl_set_hw_spec(td_void *arg, td_bool user)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;
    drv_hdmi_hw_spec *spec = TD_NULL;

    spec = (drv_hdmi_hw_spec *)arg;
    hdmi_if_null_return(spec, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(spec->hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(hdmi_chip_id(hdmi_dev->hdmi_dev_id), OT_ERR_HDMI_INVALID_PARA);

    hdmi_info("hdmi_id:%d, CMD_HDMI_SET_HW_SPEC user=%u\n", hdmi_dev->hdmi_dev_id, user);

    hdmi_mutex_lock(g_hdmi_mutex);
    ret = drv_hdmi_hw_spec_set(hdmi_dev, &spec->hw_spec);
    hdmi_mutex_unlock(g_hdmi_mutex);

    return ret;
}

#ifndef CONFIG_COMPAT
static long hdmi_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    ot_unused(private_data);
    return drv_hdmi_cmd_process(cmd, (td_void *)(uintptr_t)arg, TD_TRUE);
}
#else
static td_s32 drv_hdmi_compat_cmd_process(unsigned int cmd, td_void *arg, td_bool user)
{
    td_void __user *argp = (td_void __user *)arg;
    return drv_hdmi_cmd_process(cmd, argp, user);
}

static long hdmi_compact_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    ot_unused(private_data);
    return drv_hdmi_compat_cmd_process(cmd, (td_void *)(uintptr_t)arg, TD_TRUE);
}
#endif

static td_s32 hdmi_file_open(void *private_data)
{
    ot_unused(private_data);
    osal_atomic_inc_return(&g_hdmi_count);

    return TD_SUCCESS;
}

static td_s32 hdmi_file_close(void *private_data)
{
    td_u32 hdmi_id = 0;
    hdmi_device *hdmi_dev = TD_NULL;

    ot_unused(private_data);
    if (osal_atomic_read(&g_hdmi_count)) {
        osal_atomic_dec_return(&g_hdmi_count);
        for (; hdmi_id < HDMI_ID_MAX; hdmi_id++) {
            hdmi_dev = get_hdmi_device(hdmi_id);
            hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
            if ((td_u32)hdmi_dev->run_state & HDMI_RUN_STATE_OPEN) {
                hdmi_mutex_lock(g_hdmi_mutex);
                drv_hdmi_close(hdmi_dev, TD_TRUE);
                hdmi_mutex_unlock(g_hdmi_mutex);
            }
        }
    }

    return TD_SUCCESS;
}

static long hdmi_file_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
#ifndef CONFIG_COMPAT
    return (long)hdmi_ioctl(cmd, arg, private_data);
#else
    return (long)hdmi_compact_ioctl(cmd, arg, private_data);
#endif
}

static td_s32 hdmi_init(td_void *args)
{
    ot_unused(args);
    return 0;
}

static td_void hdmi_exit(td_void)
{
    return;
}

static td_void hdmi_notify(mod_notice_id notice)
{
    ot_unused(notice);
    return;
}

static td_void hdmi_query_state(mod_state *state)
{
    *state = MOD_STATE_FREE;
    return;
}

static td_u32 hdmi_get_ver_magic(td_void)
{
    return VERSION_MAGIC;
}

static td_s32 hdmi_dev_register(td_void)
{
    td_char ch_devfs_name[DEVFS_NAME_LEN] = {0};

    if (cmpi_register_module(&g_module)) {
        hdmi_warn("cmpi_register_module hdmi fail \n");
        return TD_FAILURE;
    }

    /* register hdmi device */
    if (snprintf_s(ch_devfs_name, DEVFS_NAME_LEN, DEVFS_NAME_LEN - 1, "%s", UMAP_DEVNAME_HDMI_BASE) < 0) {
        hdmi_err("snprintf_s err\n");
        return TD_FAILURE;
    }
    g_hdmi_device = osal_createdev(ch_devfs_name);
    hdmi_if_null_return(g_hdmi_device, OT_ERR_HDMI_NULL_PTR);
    g_hdmi_device->fops  = &g_hdmi_file_ops;
    g_hdmi_device->minor = UMAP_HDMI_MINOR_BASE;
    if (osal_registerdevice(g_hdmi_device) < 0) {
        hdmi_warn("g_hdmi_device register failed\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
static td_void drv_hdmi_proc_add_module(const td_char *entry_name, const hdmi_proc_item *proc_item)
{
    osal_proc_entry_t *proc = NULL;

    proc = osal_create_proc_entry(entry_name, NULL);
    hdmi_if_null_return_void(proc);
    proc->private = (td_void *)proc_item->private;
    proc->read = (td_void *)proc_item->read;
    proc->write = (td_void *)proc_item->write;

    return;
}

static td_void drv_hdmi_proc_remove_module(const td_char *entry_name, td_u32 arry_len)
{
    ot_unused(arry_len);
    osal_remove_proc_entry(entry_name, NULL);
    return;
}

static td_void drv_hdmi_proc_register(td_u32 hdmi_id)
{
    hdmi_device *hdmi_dev = TD_NULL;
    td_char proc_name[PROC_NAME_MAX] = {0};

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_return_void(hdmi_dev);

    if (snprintf_s(proc_name, PROC_NAME_MAX, PROC_NAME_MAX - 1, "%s%u", "hdmi", hdmi_id) < 0) {
        hdmi_err("snprintf_s err.\n");
        return;
    }

    g_hdmi_proc_item.private = (td_void *)hdmi_dev;
    drv_hdmi_proc_add_module(proc_name, &g_hdmi_proc_item);
    (td_void)memset_s(proc_name, PROC_NAME_MAX, 0, sizeof(proc_name));
    if (snprintf_s(proc_name, PROC_NAME_MAX, PROC_NAME_MAX - 1, "%s%u%s", "hdmi", hdmi_id, "_vo") < 0) {
        hdmi_err("snprintf_s err.\n");
        return;
    }
    g_vo_proc_item.private = (td_void *)hdmi_dev;
    drv_hdmi_proc_add_module(proc_name, &g_vo_proc_item);
    (td_void)memset_s(proc_name, PROC_NAME_MAX, 0, sizeof(proc_name));
    if (snprintf_s(proc_name, PROC_NAME_MAX, PROC_NAME_MAX - 1, "%s%u%s", "hdmi", hdmi_id, "_ao") < 0) {
        hdmi_err("snprintf_s err.\n");
        return;
    }
    g_ao_proc_item.private = (td_void *)hdmi_dev;
    drv_hdmi_proc_add_module(proc_name, &g_ao_proc_item);
    (td_void)memset_s(proc_name, PROC_NAME_MAX, 0, sizeof(proc_name));
    if (snprintf_s(proc_name, PROC_NAME_MAX, PROC_NAME_MAX - 1, "%s%u%s", "hdmi", hdmi_id, "_sink") < 0) {
        hdmi_err("snprintf_s err.\n");
        return;
    }
    g_sink_proc_item.private = (td_void *)hdmi_dev;
    drv_hdmi_proc_add_module(proc_name, &g_sink_proc_item);

    return;
}

static td_void hdmi_proc_register(td_void)
{
    td_u32 hdmi_id;
    hdmi_device *hdmi_dev = TD_NULL;

    for (hdmi_id = 0; hdmi_id < HDMI_ID_MAX; hdmi_id++) {
        hdmi_dev = get_hdmi_device(hdmi_id);
        if (hdmi_dev != TD_NULL) {
            drv_hdmi_proc_register(hdmi_id);
        }
    }

    return;
}

static td_void drv_hdmi_proc_un_register(td_u32 hdmi_id)
{
    td_char proc_name[PROC_NAME_MAX] = {0};

    if (snprintf_s(proc_name, PROC_NAME_MAX, PROC_NAME_MAX - 1, "%s%u", "hdmi", hdmi_id) < 0) {
        hdmi_err("snprintf_s err.\n");
        return;
    }
    drv_hdmi_proc_remove_module(proc_name, PROC_NAME_MAX);
    (td_void)memset_s(proc_name, PROC_NAME_MAX, 0, sizeof(proc_name));
    if (snprintf_s(proc_name, PROC_NAME_MAX, PROC_NAME_MAX - 1, "%s%u%s", "hdmi", hdmi_id, "_vo") < 0) {
        hdmi_err("snprintf_s err.\n");
        return;
    }
    drv_hdmi_proc_remove_module(proc_name, PROC_NAME_MAX);
    (td_void)memset_s(proc_name, PROC_NAME_MAX, 0, sizeof(proc_name));
    if (snprintf_s(proc_name, PROC_NAME_MAX, PROC_NAME_MAX - 1, "%s%u%s", "hdmi", hdmi_id, "_ao") < 0) {
        hdmi_err("snprintf_s err.\n");
        return;
    }
    drv_hdmi_proc_remove_module(proc_name, PROC_NAME_MAX);
    (td_void)memset_s(proc_name, PROC_NAME_MAX, 0, sizeof(proc_name));
    if (snprintf_s(proc_name, PROC_NAME_MAX, PROC_NAME_MAX - 1, "%s%u%s", "hdmi", hdmi_id, "_sink") < 0) {
        hdmi_err("snprintf_s err.\n");
        return;
    }
    drv_hdmi_proc_remove_module(proc_name, PROC_NAME_MAX);

    return;
}
#endif

static const hdmi_ioctrl_func g_hdmi_cmd_func_tab[] = {
    { CMD_HDMI_OPEN,                hdmi_ioctrl_open },
    { CMD_HDMI_CLOSE,               hdmi_ioctrl_close },
    { CMD_HDMI_START,               hdmi_ioctrl_start },
    { CMD_HDMI_STOP,                hdmi_ioctrl_stop },
    { CMD_HDMI_GET_SINK_CAPABILITY, hdmi_ioctrl_get_sink_capability },
    { CMD_HDMI_POLL_EVENT,          hdmi_ioctrl_poll_event },
    { CMD_HDMI_SET_ATTR,            hdmi_ioctrl_set_attr },
    { CMD_HDMI_GET_ATTR,            hdmi_ioctrl_get_attr },
    { CMD_HDMI_GET_VO_ATTR,         hdmi_ioctrl_get_vo_attr },
    { CMD_HDMI_SET_VO_ATTR,         hdmi_ioctrl_set_vo_attr },
    { CMD_HDMI_GET_INFOFRAME,       hdmi_ioctrl_get_infoframe },
    { CMD_HDMI_SET_INFOFRAME,       hdmi_ioctrl_set_infoframe },
    { CMD_HDMI_SET_AVMUTE,          hdmi_ioctrl_avmute },
    { CMD_HDMI_UPDATE_EDID,         hdmi_ioctrl_update_edid },
    { CMD_HDMI_GET_STATUS,          hdmi_ioctrl_get_status },
    { CMD_HDMI_REGISTER_CALLBACK,   hdmi_ioctrl_register_callback },
    { CMD_HDMI_UNREGISTER_CALLBACK, hdmi_ioctrl_ungister_callback },
    { CMD_HDMI_SET_MOD_PARAM,       hdmi_ioctrl_set_mod_param },
    { CMD_HDMI_GET_MOD_PARAM,       hdmi_ioctrl_get_mod_param },
    { CMD_HDMI_GET_HW_SPEC,         hdmi_ioctrl_get_hw_spec },
    { CMD_HDMI_SET_HW_SPEC,         hdmi_ioctrl_set_hw_spec }
};

td_s32 hdmi_mode_strategy(hdmi_device *hdmi_dev)
{
    hdmi_tmds_mode tmds_mode;
    hdmi_app_attr *app_attr = TD_NULL;
#ifdef HDMI_SCDC_SUPPORT
    hdmi_edid_data edid_ret;
    hdmi_scdc_status scdc_get = {0};
    hdmi_scdc_status scdc_status = {0};
    hdmi_vo_attr *vo_attr = TD_NULL;
    hdmi_sink_capability *sink_cap = TD_NULL;
#endif

    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    app_attr = &hdmi_dev->attr.app_attr;

#ifdef HDMI_SCDC_SUPPORT
    vo_attr = &hdmi_dev->attr.vo_attr;
    edid_ret = drv_hdmi_edid_capability_get(&hdmi_dev->edid_info, &sink_cap);
    if (edid_ret == HDMI_EDID_DATA_INVALID) {
        hdmi_warn("invalid edid_capability!\n");
    }
    if (hdmi_scdc_status_get(app_attr, vo_attr, &scdc_status, &tmds_mode) != TD_SUCCESS) {
        return TD_FAILURE;
    }
#else
    tmds_mode = (app_attr->enable_hdmi == TD_TRUE) ? HDMI_TMDS_MODE_HDMI_1_4 : HDMI_TMDS_MODE_DVI;
#endif

    hdmi_info("tmds mode %u->%u.\n", hdmi_dev->tmds_mode, tmds_mode);
    hdmi_dev->tmds_mode = tmds_mode;
    hal_call_void(hdmi_dev, hal_hdmi_tmds_mode_set, hdmi_dev->tmds_mode);
    /* reset controller when the controller and phy configuration is completed. */
    hal_call_void(hdmi_dev, hal_hdmi_ctrl_reset);

#ifdef HDMI_SCDC_SUPPORT
    if (sink_cap->support_scdc == TD_TRUE || hdmi_dev->attr.app_attr.auth_mode == TD_TRUE) {
        hal_call_void(hdmi_dev, hal_hdmi_scdc_status_get, &scdc_get);
        if ((scdc_get.sink_scramble_on != scdc_status.sink_scramble_on ||
            scdc_get.source_scramble_on != scdc_status.source_scramble_on ||
            scdc_get.tmds_bit_clk_ratio != scdc_status.tmds_bit_clk_ratio) ||
            (hdmi_dev->attr.app_attr.auth_mode == TD_TRUE)) {
            scdc_status.scramble_interval = SCDC_SCRAMBLE_INTERVAL_RESET;
            scdc_status.scramble_timeout = SCDC_SCRAMBLE_TIMEOUT_RESET;
            scdc_status.sink_read_quest = TD_FALSE;
            hal_call_void(hdmi_dev, hal_hdmi_scdc_status_set, &scdc_status);
        }
    } else {
        hdmi_info("sink not support SCDC\n");
    }
#endif

    return TD_SUCCESS;
}

td_s32 drv_hdmi_start(hdmi_device *hdmi_dev)
{
    td_bool audio_enable;
    hdmi_app_attr *app_attr = TD_NULL;


#endif /* SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_INTF_BODY_PART07_H_ */
