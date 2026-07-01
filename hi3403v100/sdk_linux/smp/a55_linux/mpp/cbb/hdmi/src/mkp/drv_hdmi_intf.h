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
#ifndef DRV_HDMI_INTF_H
#define DRV_HDMI_INTF_H

#include "ot_type.h"
#include "hdmi_hal.h"
#include "drv_hdmi_common.h"
#include "drv_hdmi_ioctl.h"
#include "drv_hdmi_edid.h"
#include "drv_hdmi_event.h"

typedef struct {
    hdmi_thread_state thread_timer_sate;
    osal_task_t      *thread_timer;
    td_bool           thread_run;
    td_bool           thread_exit;
    osal_wait_t       wait_destroy;
} hdmi_thread_info;

typedef struct {
    td_bool         attr_busy;
    td_bool         attr_set;
    osal_spinlock_t attr_spin;
} hdmi_hdr_mutex;

typedef struct {
    td_u32                 hdmi_dev_id;
    td_u32                 kernel_cnt;
    td_u32                 user_cnt;
    td_u32                 user_callback_cnt;
    td_bool                hpd_detected;
    td_bool                hpd_notifyed;
    hdmi_run_state         run_state;
    hdmi_transition_state  transition_state;
    hdmi_tmds_mode         tmds_mode;
    hdmi_callback          k_callback;
    hdmi_thread_info       thread_info;
    hdmi_edid_info         edid_info;
    hdmi_infoframe_type    info_frame;
    hdmi_attr              attr;
    hdmi_delay             delay;
    hdmi_hal              *hal;
    hdmi_debug             debug;
    osal_semaphore_t       mutex_thread;
    osal_semaphore_t       mutex_proc;
    hdmi_mode_param        mode_param;
    hdmi_csc_attr          csc_param;
    hdmi_user_vsif_content vendor_data;
} hdmi_device;

td_s32 drv_hdmi_start(hdmi_device *hdmi_dev);

td_s32 drv_hdmi_stop(hdmi_device *hdmi_dev);

td_void drv_hdmi_avmute_set(const hdmi_device *hdmi_dev, td_bool av_mute);

td_s32 drv_hdmi_vo_attr_set(hdmi_device *hdmi_dev, const hdmi_vo_attr *vo_attr);

td_s32 drv_hdmi_attr_get(const hdmi_device *hdmi_dev, hdmi_attr *attr);

td_s32 drv_hdmi_attr_set(hdmi_device *hdmi_dev, hdmi_attr *attr);

td_s32 drv_hdmi_cmd_process(unsigned int cmd, td_void *arg, td_bool user);

hdmi_device *get_hdmi_device(hdmi_device_id hdmi_id);

td_void get_hdmi_default_action_set(hdmi_device *hdmi_dev, hdmi_default_action action);

td_s32 hdmi_thread_state_set(hdmi_device *hdmi_dev, hdmi_thread_state state);

td_s32 hdmi_mode_strategy(hdmi_device *hdmi_dev);

#endif /* DRV_HDMI_INTF_H */

