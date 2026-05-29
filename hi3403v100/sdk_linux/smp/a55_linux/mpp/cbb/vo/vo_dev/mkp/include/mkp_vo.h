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

#ifndef MKP_VO_H
#define MKP_VO_H

#include "mkp_ioctl.h"
#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_vo.h"
#include "inner_vo.h"
#include "securec.h"
#include "osal_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define VO_CHN_FRAME_STATISTICS_REFRESH_TIME 10000000

#define VO_LAYER_BUTT OT_VO_MAX_PHYS_LAYER_NUM

#define vo_emerg_trace(fmt, ...) \
        OT_EMERG_TRACE(OT_ID_VO, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define vo_err_trace(fmt, ...)  \
        OT_ERR_TRACE(OT_ID_VO, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define vo_warn_trace(fmt, ...)  \
        OT_WARN_TRACE(OT_ID_VO, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define vo_info_trace(fmt, ...)  \
        OT_INFO_TRACE(OT_ID_VO, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define vo_alert_trace(fmt, ...)  \
        OT_ALERT_TRACE(OT_ID_VO, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define vo_debug_trace(fmt, ...)  \
        OT_DEBUG_TRACE(OT_ID_VO, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

typedef enum {
    VO_IOCTL_TYPE_SET = 0,
    VO_IOCTL_TYPE_GET,
    VO_IOCTL_TYPE_ENABLE,
    VO_IOCTL_TYPE_DISABLE,
    VO_IOCTL_TYPE_BIND,
    VO_IOCTL_TYPE_UNBIND,
    VO_IOCTL_TYPE_BEGIN,
    VO_IOCTL_TYPE_END,
    VO_IOCTL_TYPE_BUTT
} vo_ioctl_type;

/* pub mpi frame info */
typedef struct {
    ot_video_frame_info frame_info;
    td_s32 milli_sec;
} vo_ioctl_vframe_timeout;

/* dev mpi param */
typedef struct {
    ot_vo_dev dev;
}vo_ioctl_dev_param;

typedef struct {
    vo_ioctl_dev_param ioctl_dev;
    ot_vo_pub_attr pub_attr;
}vo_ioctl_pub_attr_param;

typedef struct {
    vo_ioctl_dev_param ioctl_dev;
    ot_vo_dev_param dev_param;
}vo_ioctl_dev_param_param;

typedef struct {
    vo_ioctl_dev_param ioctl_dev;
    td_u32 frame_rate;
}vo_ioctl_dev_frame_rate_param;

typedef struct {
    vo_ioctl_dev_param ioctl_dev;
    ot_vo_user_sync_info sync_info;
}vo_ioctl_user_sync_info_param;

typedef struct {
    vo_ioctl_dev_param ioctl_dev;
    td_u32 vtth;
}vo_ioctl_vtth_param;

typedef struct {
    vo_ioctl_dev_param ioctl_dev;
    ot_vo_less_buf_attr less_buf_attr;
}vo_ioctl_less_buf_attr_param;

typedef struct {
    vo_ioctl_dev_param ioctl_dev;
    ot_vo_user_notify_attr user_notify_attr;
}vo_ioctl_user_notify_attr_param;

/* dev intf mpi param */
typedef struct {
    ot_vo_intf_type intf_type;
    ot_vo_intf_status intf_status;
}vo_intf_status_info;

typedef struct {
    vo_ioctl_dev_param ioctl_dev;
    ot_vo_vga_param vga_param;
}vo_ioctl_vga_param_param;

typedef struct {
    vo_ioctl_dev_param ioctl_dev;
    ot_vo_hdmi_param hdmi_param;
}vo_ioctl_hdmi_param_param;

typedef struct {
    vo_ioctl_dev_param ioctl_dev;
    ot_vo_rgb_param rgb_param;
}vo_ioctl_rgb_param_param;

typedef struct {
    vo_ioctl_dev_param ioctl_dev;
    ot_vo_bt_param bt_param;
}vo_ioctl_bt_param_param;

typedef struct {
    vo_ioctl_dev_param ioctl_dev;
    ot_vo_mipi_param mipi_param;
}vo_ioctl_mipi_param_param;

/* cascade mpi param */
typedef struct {
    vo_ioctl_dev_param cas_dev;
    td_u32 pattern;
}vo_ioctl_cas_pattern_param;

typedef enum {
    /* device operation */
    IOC_NR_VO_OPEN = 0,
    IOC_NR_VO_CLOSE,
    IOC_NR_VO_PUB_ATTR_SET,
    IOC_NR_VO_PUB_ATTR_GET,
    IOC_NR_VO_DEV_PARAM_SET,
    IOC_NR_VO_DEV_PARAM_GET,
    IOC_NR_VO_FPS_SET,
    IOC_NR_VO_FPS_GET,
    IOC_NR_VO_USER_SYNC_SET,
    IOC_NR_VO_USER_SYNC_GET,
    IOC_NR_VO_MOD_PARAM_SET,
    IOC_NR_VO_MOD_PARAM_GET,
    IOC_NR_VO_VTTH_SET,
    IOC_NR_VO_VTTH_GET,
    IOC_NR_VO_LESS_BUF_SET,
    IOC_NR_VO_LESS_BUF_GET,
    IOC_NR_VO_USER_NOTIFY_SET,
    IOC_NR_VO_USER_NOTIFY_GET,

    /* device interface operation */
    IOC_NR_VO_QUERY_INTF_STATUS,
    IOC_NR_VO_VGA_PARAM_SET,
    IOC_NR_VO_VGA_PARAM_GET,
    IOC_NR_VO_HDMI_PARAM_SET,
    IOC_NR_VO_HDMI_PARAM_GET,
    IOC_NR_VO_HDMI1_PARAM_SET,
    IOC_NR_VO_HDMI1_PARAM_GET,
    IOC_NR_VO_RGB_PARAM_SET,
    IOC_NR_VO_RGB_PARAM_GET,
    IOC_NR_VO_BT_PARAM_SET,
    IOC_NR_VO_BT_PARAM_GET,
    IOC_NR_VO_MIPI_PARAM_SET,
    IOC_NR_VO_MIPI_PARAM_GET,

    /* video layer operation */

    /* video&graphic pub operation */

    /* channel operation */

    /* wbc operation */

    /* cascade */
    IOC_NR_VO_CAS_ATTR_SET,
    IOC_NR_VO_CAS_ATTR_GET,
    IOC_NR_VO_CAS_ENABLE_DEV,
    IOC_NR_VO_CAS_DISABLE_DEV,
    IOC_NR_VO_CAS_PATTERN_SET,
    IOC_NR_VO_CAS_PATTERN_GET,
    IOC_NR_VO_CAS_ENABLE,
    IOC_NR_VO_CAS_DISABLE,

    IOC_NR_VO_BUTT
} ioc_nr_vou;

/* 设备操作ioctl */
#define VO_OPEN_CTRL                OSAL_IOW(IOC_TYPE_VOU,  IOC_NR_VO_OPEN, vo_ioctl_dev_param)
#define VO_CLOSE_CTRL               OSAL_IOW(IOC_TYPE_VOU,  IOC_NR_VO_CLOSE, vo_ioctl_dev_param)
#define VO_PUB_ATTR_SET_CTRL        OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_PUB_ATTR_SET, vo_ioctl_pub_attr_param)
#define VO_PUB_ATTR_GET_CTRL        OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_PUB_ATTR_GET, vo_ioctl_pub_attr_param)
#define VO_DEV_PARAM_SET_CTRL       OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_DEV_PARAM_SET, vo_ioctl_dev_param_param)
#define VO_DEV_PARAM_GET_CTRL       OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_DEV_PARAM_GET, vo_ioctl_dev_param_param)
#define VO_DEV_FPS_SET_CTRL         OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_FPS_SET, vo_ioctl_dev_frame_rate_param)
#define VO_DEV_FPS_GET_CTRL         OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_FPS_GET, vo_ioctl_dev_frame_rate_param)
#define VO_USER_SYNC_INFO_SET_CTRL  OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_USER_SYNC_SET, vo_ioctl_user_sync_info_param)
#define VO_USER_SYNC_INFO_GET_CTRL  OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_USER_SYNC_GET, vo_ioctl_user_sync_info_param)
#define VO_MOD_PARAM_SET_CTRL       OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_MOD_PARAM_SET, ot_vo_mod_param)
#define VO_MOD_PARAM_GET_CTRL       OSAL_IOR(IOC_TYPE_VOU, IOC_NR_VO_MOD_PARAM_GET, ot_vo_mod_param)
#define VO_DEV_VTTH_SET_CTRL        OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_VTTH_SET, vo_ioctl_vtth_param)
#define VO_DEV_VTTH_GET_CTRL        OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_VTTH_GET, vo_ioctl_vtth_param)
#define VO_DEV_LESS_BUF_SET_CTRL    OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_LESS_BUF_SET, vo_ioctl_less_buf_attr_param)
#define VO_DEV_LESS_BUF_GET_CTRL    OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_LESS_BUF_GET, vo_ioctl_less_buf_attr_param)
#define VO_DEV_USER_NOTIFY_SET_CTRL OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_USER_NOTIFY_SET, vo_ioctl_user_notify_attr_param)
#define VO_DEV_USER_NOTIFY_GET_CTRL OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_USER_NOTIFY_GET, vo_ioctl_user_notify_attr_param)

/* 设备接口操作ioctl */
#define VO_QUERY_INTF_STATUS        OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_QUERY_INTF_STATUS, vo_intf_status_info)
#define VO_VGA_PARAM_SET_CTRL       OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_VGA_PARAM_SET, vo_ioctl_vga_param_param)
#define VO_VGA_PARAM_GET_CTRL       OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_VGA_PARAM_GET, vo_ioctl_vga_param_param)
#define VO_HDMI_PARAM_SET_CTRL      OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_HDMI_PARAM_SET, vo_ioctl_hdmi_param_param)
#define VO_HDMI_PARAM_GET_CTRL      OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_HDMI_PARAM_GET, vo_ioctl_hdmi_param_param)
#define VO_HDMI1_PARAM_SET_CTRL     OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_HDMI1_PARAM_SET, vo_ioctl_hdmi_param_param)
#define VO_HDMI1_PARAM_GET_CTRL     OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_HDMI1_PARAM_GET, vo_ioctl_hdmi_param_param)
#define VO_RGB_PARAM_SET_CTRL       OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_RGB_PARAM_SET, vo_ioctl_rgb_param_param)
#define VO_RGB_PARAM_GET_CTRL       OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_RGB_PARAM_GET, vo_ioctl_rgb_param_param)
#define VO_BT_PARAM_SET_CTRL        OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_BT_PARAM_SET, vo_ioctl_bt_param_param)
#define VO_BT_PARAM_GET_CTRL        OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_BT_PARAM_GET, vo_ioctl_bt_param_param)
#define VO_MIPI_PARAM_SET_CTRL      OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_MIPI_PARAM_SET, vo_ioctl_mipi_param_param)
#define VO_MIPI_PARAM_GET_CTRL      OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_MIPI_PARAM_GET, vo_ioctl_mipi_param_param)

/* 层操作ioctl */

/* 视频层和图形层公共操作ioctl */

/* 通道操作ioctl */

/* 回写操作 */

/* 级联操作 */
#define VO_CAS_ATTR_SET_CTRL        OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_CAS_ATTR_SET, ot_vo_cas_attr)
#define VO_CAS_ATTR_GET_CTRL        OSAL_IOR(IOC_TYPE_VOU, IOC_NR_VO_CAS_ATTR_GET, ot_vo_cas_attr)
#define VO_CAS_ENABLE_DEV_CTRL      OSAL_IOW(IOC_TYPE_VOU,  IOC_NR_VO_CAS_ENABLE_DEV, vo_ioctl_dev_param)
#define VO_CAS_DISABLE_DEV_CTRL     OSAL_IOW(IOC_TYPE_VOU,  IOC_NR_VO_CAS_DISABLE_DEV, vo_ioctl_dev_param)
#define VO_CAS_PATTERN_SET_CTRL     OSAL_IOW(IOC_TYPE_VOU, IOC_NR_VO_CAS_PATTERN_SET, vo_ioctl_cas_pattern_param)
#define VO_CAS_PATTERN_GET_CTRL     OSAL_IOWR(IOC_TYPE_VOU, IOC_NR_VO_CAS_PATTERN_GET, vo_ioctl_cas_pattern_param)
#define VO_CAS_ENABLE_CTRL          OSAL_IO(IOC_TYPE_VOU,   IOC_NR_VO_CAS_ENABLE)
#define VO_CAS_DISABLE_CTRL         OSAL_IO(IOC_TYPE_VOU,   IOC_NR_VO_CAS_DISABLE)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef MKP_VO_H */
