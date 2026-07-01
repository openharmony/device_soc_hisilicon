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
#ifndef DRV_HDMI_IOCTL_H
#define DRV_HDMI_IOCTL_H

#include "ot_type.h"
#include "autoconf.h"
#include "drv_hdmi_infoframe.h"
#include "drv_hdmi_edid.h"
#include "drv_hdmi_common.h"
#include "ot_common_hdmi.h"
#include "mkp_ioctl.h"
#include "osal_ioctl.h"

typedef ot_hdmi_colorimetry     drv_hdmi_colorimetry;
typedef ot_hdmi_rgb_quant_range drv_hdmi_rgb_quant_range;
typedef ot_hdmi_ycc_quant_range drv_hdmi_ycc_quant_range;
typedef ot_hdmi_pic_scaline     drv_hdmi_pic_scaline;

typedef struct {
    ot_hdmi_video_format     video_fmt;
    td_u32                   clk_fs;
    td_u32                   pixel_repeat;
    hdmi_3d_mode             stereo_mode;
    ot_hdmi_video_mode       in_color_space;
    hdmi_hvsync_polarity     hv_sync_pol;
    drv_hdmi_colorimetry     colorimetry;
    drv_hdmi_rgb_quant_range rgb_quantization;
    drv_hdmi_ycc_quant_range ycc_quantization;
    ot_pic_aspect_ratio      picture_aspect;
    hdmi_active_aspect       active_aspect;
    drv_hdmi_pic_scaline     picture_scaling;
    ot_hdmi_deep_color       in_bit_depth;
} hdmi_video_attr;

typedef struct {
    hdmi_device_id      hdmi_id;
    hdmi_default_action default_mode;
} hdmi_open;

typedef struct {
    hdmi_device_id hdmi_id;
    hdmi_event     event;
} drv_hdmi_event;

typedef struct {
    hdmi_device_id      hdmi_id;
    hdmi_default_action default_action;
} drv_hdmi_default_action;

typedef struct {
    hdmi_device_id       hdmi_id;
    hdmi_sink_capability cap;
} drv_hdmi_sink_capability;

typedef struct {
    hdmi_device_id hdmi_id;
    hdmi_property  prop;
} drv_hdmi_property;

typedef struct {
    hdmi_device_id hdmi_id;
    hdmi_vo_attr   vo_attr;
} drv_hdmi_vo_attr;

typedef struct {
    hdmi_device_id    hdmi_id;
    hdmi_infoframe_id infoframe_id;
    hdmi_infoframe    infoframe;
} drv_hdmi_infoframe;

typedef struct {
    hdmi_device_id hdmi_id;
    td_bool        avmute;
} drv_hdmi_avmute;

typedef struct {
    hdmi_device_id     hdmi_id;
    hdmi_edid_raw_data edid_raw;
} drv_hdmi_edid_raw_data;

typedef struct {
    hdmi_device_id hdmi_id;
    hdmi_status    status;
} drv_hdmi_status;

typedef struct {
    hdmi_device_id hdmi_id;
    hdmi_delay     delay;
} drv_hdmi_delay;

typedef struct {
    hdmi_device_id hdmi_id;
    td_bool emi_en;
    hdmi_trace_len trace_len;
} drv_hdmi_mod_param;

typedef struct {
    hdmi_device_id hdmi_id;
    hdmi_hw_spec   hw_spec;
} drv_hdmi_hw_spec;

enum ioctl_hdmi {
    IOCTL_HDMI_INIT = 0x01,
    IOCTL_HDMI_DEINIT,
    IOCTL_HDMI_OPEN,
    IOCTL_HDMI_CLOSE,
    IOCTL_HDMI_START,
    IOCTL_HDMI_STOP,
    IOCTL_HDMI_GET_SINK_CAPABILITY,
    IOCTL_HDMI_POLL_EVENT,
    IOCTL_HDMI_GET_ATTR,
    IOCTL_HDMI_SET_ATTR,
    IOCTL_HDMI_GET_VO_ATTR,
    IOCTL_HDMI_SET_VO_ATTR,
    IOCTL_HDMI_GET_INFOFRAME,
    IOCTL_HDMI_SET_INFOFRAME,
    IOCTL_HDMI_AVMUTE,
    IOCTL_HDMI_UPDATE_EDID,
    IOCTL_HDMI_REG_CALLBACK_FUNC,
    IOCTL_HDMI_GET_STATUS,
    IOCTL_HDMI_REGISTER_CALLBACK,
    IOCTL_HDMI_UNREGISTER_CALLBACK,
    IOCTL_HDMI_SET_MOD_PARAM,
    IOCTL_HDMI_GET_MOD_PARAM,
    IOCTL_HDMI_SET_HW_SPEC,
    IOCTL_HDMI_GET_HW_SPEC,
    IOCTL_HDMI_MAX
};

#define CMD_HDMI_INIT                 OSAL_IO(IOC_TYPE_HDMI, IOCTL_HDMI_INIT)
#define CMD_HDMI_DEINIT               OSAL_IO(IOC_TYPE_HDMI, IOCTL_HDMI_DEINIT)
#define CMD_HDMI_OPEN                 OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_OPEN, drv_hdmi_default_action)
#define CMD_HDMI_CLOSE                OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_CLOSE, hdmi_device_id)
#define CMD_HDMI_START                OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_START, hdmi_device_id)
#define CMD_HDMI_STOP                 OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_STOP, hdmi_device_id)
#define CMD_HDMI_GET_SINK_CAPABILITY  OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_GET_SINK_CAPABILITY, drv_hdmi_sink_capability)
#define CMD_HDMI_POLL_EVENT           OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_POLL_EVENT, drv_hdmi_event)
#define CMD_HDMI_GET_INFOFRAME        OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_GET_INFOFRAME, drv_hdmi_infoframe)
#define CMD_HDMI_SET_INFOFRAME        OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_SET_INFOFRAME, drv_hdmi_infoframe)
#define CMD_HDMI_SET_AVMUTE           OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_AVMUTE, drv_hdmi_avmute)
#define CMD_HDMI_UPDATE_EDID          OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_UPDATE_EDID, drv_hdmi_edid_raw_data)
#define CMD_HDMI_GET_STATUS           OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_GET_STATUS, drv_hdmi_status)
#define CMD_HDMI_REGISTER_CALLBACK    OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_REGISTER_CALLBACK, hdmi_device_id)
#define CMD_HDMI_UNREGISTER_CALLBACK  OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_UNREGISTER_CALLBACK, hdmi_device_id)
#define CMD_HDMI_SET_MOD_PARAM        OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_SET_MOD_PARAM, drv_hdmi_mod_param)
#define CMD_HDMI_GET_MOD_PARAM        OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_GET_MOD_PARAM, drv_hdmi_mod_param)
#define CMD_HDMI_SET_ATTR             OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_SET_ATTR, drv_hdmi_property)
#define CMD_HDMI_GET_ATTR             OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_GET_ATTR, drv_hdmi_property)
#define CMD_HDMI_SET_VO_ATTR          OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_SET_VO_ATTR, drv_hdmi_vo_attr)
#define CMD_HDMI_GET_VO_ATTR          OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_GET_VO_ATTR, drv_hdmi_vo_attr)
#define CMD_HDMI_GET_HW_SPEC          OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_GET_HW_SPEC, drv_hdmi_hw_spec)
#define CMD_HDMI_SET_HW_SPEC          OSAL_IOWR(IOC_TYPE_HDMI, IOCTL_HDMI_SET_HW_SPEC, drv_hdmi_hw_spec)
#endif

