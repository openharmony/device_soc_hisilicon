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

#include "ot_mpi_vo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "ot_errno.h"
#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_vo.h"
#include "mkp_vo.h"
#include "ot_defines.h"
#include "inner_vo.h"
#include "ot_osal_user.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if vo_desc("pthread lock")

static pthread_mutex_t g_vo_mutex = PTHREAD_MUTEX_INITIALIZER;

#define vo_mpi_mutex_lock()   ((void)pthread_mutex_lock(&g_vo_mutex))
#define vo_mpi_mutex_unlock() ((void)pthread_mutex_unlock(&g_vo_mutex))

#endif

#if vo_desc("File Operations")

static td_s32 g_vo_fd = OT_INVALID_VALUE;

static td_s32 vo_mpi_open_fd(td_void)
{
    g_vo_fd = open("/dev/vo", O_RDONLY);
    if (g_vo_fd < 0) {
        perror("open vo device error!\n");
        return OT_ERR_VO_NOT_READY;
    }

    return TD_SUCCESS;
}

static td_s32 vo_mpi_check_open(td_void)
{
    td_s32 ret = TD_SUCCESS;

    vo_mpi_mutex_lock();
    if (g_vo_fd < 0) {
        ret = vo_mpi_open_fd();
    }
    vo_mpi_mutex_unlock();
    return ret;
}

static td_s32 vo_mpi_close_vo_fd(td_void)
{
    if (g_vo_fd < 0) {
        return TD_SUCCESS;
    }
    if (close(g_vo_fd) != TD_SUCCESS) {
        perror("close vo fd fail");
        return OT_ERR_VO_BUSY;
    }
    g_vo_fd = OT_INVALID_VALUE;
    return TD_SUCCESS;
}

#define vo_mpi_check_open_return()               \
    do {                                         \
        if (vo_mpi_check_open() != TD_SUCCESS) { \
            return OT_ERR_VO_NOT_READY;          \
        }                                        \
    } while (0)

#endif

#if vo_desc("parameter check")

static td_s32 vo_mpi_check_not_support(td_void)
{
    vo_err_trace("The chip can not support this operation.\n");
    return OT_ERR_VO_NOT_SUPPORT;
}

static td_s32 vo_mpi_check_null_ptr(const td_void *ptr)
{
    if (ptr == TD_NULL) {
        vo_err_trace("null pointer!\r\n");
        return OT_ERR_VO_NULL_PTR;
    }

    return TD_SUCCESS;
}

static td_s32 vo_mpi_check_dev_id(ot_vo_dev dev)
{
    if ((dev < 0) || (dev >= OT_VO_MAX_DEV_NUM)) {
        vo_err_trace("dev(%d) is invalid\r\n", dev);
        return OT_ERR_VO_INVALID_DEV_ID;
    }

    return TD_SUCCESS;
}

static td_s32 vo_mpi_check_chn_id(ot_vo_chn chn)
{
    if ((chn < 0) || (chn >= OT_VO_MAX_CHN_NUM)) {
        vo_err_trace("chn(%d) is invalid\r\n", chn);
        return OT_ERR_VO_INVALID_CHN_ID;
    }

    return TD_SUCCESS;
}

static td_s32 vo_mpi_check_layer_id(ot_vo_layer layer)
{
    if ((layer < 0) || (layer >= OT_VO_MAX_LAYER_NUM)) {
        vo_err_trace("layer(%d) is invalid\r\n", layer);
        return OT_ERR_VO_INVALID_LAYER_ID;
    }

    return TD_SUCCESS;
}

#define vo_mpi_check_null_ptr_return(ptr)                            \
    do {                                                             \
        if (vo_mpi_check_null_ptr((td_void *)(ptr)) != TD_SUCCESS) { \
            return OT_ERR_VO_NULL_PTR;                               \
        }                                                            \
    } while (0)

#define vo_mpi_check_dev_id_return(dev)               \
    do {                                              \
        if (vo_mpi_check_dev_id(dev) != TD_SUCCESS) { \
            return OT_ERR_VO_INVALID_DEV_ID;          \
        }                                             \
    } while (0)

#define vo_mpi_check_layer_id_return(layer)               \
    do {                                                  \
        if (vo_mpi_check_layer_id(layer) != TD_SUCCESS) { \
            return OT_ERR_VO_INVALID_LAYER_ID;            \
        }                                                 \
    } while (0)

#define vo_mpi_check_chn_id_return(chn)               \
    do {                                              \
        if (vo_mpi_check_chn_id(chn) != TD_SUCCESS) { \
            return OT_ERR_VO_INVALID_CHN_ID;          \
        }                                             \
    } while (0)

#endif

#if vo_desc("wbc parameter check")

#ifdef CONFIG_OT_VO_WBC

static td_s32 vo_mpi_check_wbc_id(ot_vo_wbc wbc)
{
    if ((wbc < 0) || (wbc >= OT_VO_MAX_WBC_NUM)) {
        vo_err_trace("wbc(%d) is invalid\r\n", wbc);
        return OT_ERR_VO_INVALID_DEV_ID;
    }

    return TD_SUCCESS;
}

#define vo_mpi_check_wbc_id_return(wbc)               \
    do {                                              \
        if (vo_mpi_check_wbc_id(wbc) != TD_SUCCESS) { \
            return OT_ERR_VO_INVALID_DEV_ID;          \
        }                                             \
    } while (0)

#endif

#endif

#if vo_desc("device mpi")

td_s32 ot_mpi_vo_enable(ot_vo_dev dev)
{
    vo_ioctl_dev_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_open_return();

    param.dev = dev;
    return ioctl(g_vo_fd, VO_OPEN_CTRL, &param);
}

td_s32 ot_mpi_vo_disable(ot_vo_dev dev)
{
    vo_ioctl_dev_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_open_return();

    param.dev = dev;
    return ioctl(g_vo_fd, VO_CLOSE_CTRL, &param);
}

td_s32 ot_mpi_vo_set_pub_attr(ot_vo_dev dev, const ot_vo_pub_attr *pub_attr)
{
    vo_ioctl_pub_attr_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(pub_attr);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    (td_void)memcpy_s(&param.pub_attr, sizeof(ot_vo_pub_attr), pub_attr, sizeof(ot_vo_pub_attr));
    return ioctl(g_vo_fd, VO_PUB_ATTR_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_pub_attr(ot_vo_dev dev, ot_vo_pub_attr *pub_attr)
{
    td_s32 ret;
    vo_ioctl_pub_attr_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(pub_attr);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    ret = ioctl(g_vo_fd, VO_PUB_ATTR_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        (td_void)memcpy_s(pub_attr, sizeof(ot_vo_pub_attr), &param.pub_attr, sizeof(ot_vo_pub_attr));
    }
    return ret;
}

#ifdef CONFIG_OT_VO_DEV_BYPASS

td_s32 ot_mpi_vo_set_dev_param(ot_vo_dev dev, const ot_vo_dev_param *dev_param)
{
    vo_ioctl_dev_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(dev_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    (td_void)memcpy_s(&param.dev_param, sizeof(ot_vo_dev_param), dev_param, sizeof(ot_vo_dev_param));
    return ioctl(g_vo_fd, VO_DEV_PARAM_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_dev_param(ot_vo_dev dev, ot_vo_dev_param *dev_param)
{
    td_s32 ret;
    vo_ioctl_dev_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(dev_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    ret = ioctl(g_vo_fd, VO_DEV_PARAM_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        (td_void)memcpy_s(dev_param, sizeof(ot_vo_dev_param), &param.dev_param, sizeof(ot_vo_dev_param));
    }
    return ret;
}

#else

td_s32 ot_mpi_vo_set_dev_param(ot_vo_dev dev, const ot_vo_dev_param *dev_param)
{
    ot_unused(dev);
    ot_unused(dev_param);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_get_dev_param(ot_vo_dev dev, ot_vo_dev_param *dev_param)
{
    ot_unused(dev);
    ot_unused(dev_param);
    return vo_mpi_check_not_support();
}

#endif

td_s32 ot_mpi_vo_close_fd(td_void)
{
    td_s32 ret;

    vo_mpi_mutex_lock();
    ret = vo_mpi_close_vo_fd();
    vo_mpi_mutex_unlock();
    if (ret != TD_SUCCESS) {
        return OT_ERR_VO_BUSY;
    }
    return TD_SUCCESS;
}

td_s32 ot_mpi_vo_set_dev_frame_rate(ot_vo_dev dev, td_u32 frame_rate)
{
    vo_ioctl_dev_frame_rate_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    param.frame_rate = frame_rate;
    return ioctl(g_vo_fd, VO_DEV_FPS_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_dev_frame_rate(ot_vo_dev dev, td_u32 *frame_rate)
{
    td_s32 ret;
    vo_ioctl_dev_frame_rate_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(frame_rate);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    ret = ioctl(g_vo_fd, VO_DEV_FPS_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        *frame_rate = param.frame_rate;
    }
    return ret;
}

td_s32 ot_mpi_vo_set_user_sync_info(ot_vo_dev dev, const ot_vo_user_sync_info *sync_info)
{
    vo_ioctl_user_sync_info_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(sync_info);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    (td_void)memcpy_s(&param.sync_info, sizeof(ot_vo_user_sync_info), sync_info, sizeof(ot_vo_user_sync_info));
    return ioctl(g_vo_fd, VO_USER_SYNC_INFO_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_user_sync_info(ot_vo_dev dev, ot_vo_user_sync_info *sync_info)
{
    td_s32 ret;
    vo_ioctl_user_sync_info_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(sync_info);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    ret = ioctl(g_vo_fd, VO_USER_SYNC_INFO_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        (td_void)memcpy_s(sync_info, sizeof(ot_vo_user_sync_info), &param.sync_info, sizeof(ot_vo_user_sync_info));
    }
    return ret;
}

td_s32 ot_mpi_vo_set_mod_param(const ot_vo_mod_param *mod_param)
{
    vo_mpi_check_null_ptr_return(mod_param);
    vo_mpi_check_open_return();

    return ioctl(g_vo_fd, VO_MOD_PARAM_SET_CTRL, mod_param);
}

td_s32 ot_mpi_vo_get_mod_param(ot_vo_mod_param *mod_param)
{
    vo_mpi_check_null_ptr_return(mod_param);
    vo_mpi_check_open_return();

    return ioctl(g_vo_fd, VO_MOD_PARAM_GET_CTRL, mod_param);
}

td_s32 ot_mpi_vo_set_vtth(ot_vo_dev dev, td_u32 vtth)
{
    vo_ioctl_vtth_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    param.vtth = vtth;
    return ioctl(g_vo_fd, VO_DEV_VTTH_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_vtth(ot_vo_dev dev, td_u32 *vtth)
{
    td_s32 ret;
    vo_ioctl_vtth_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(vtth);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    ret = ioctl(g_vo_fd, VO_DEV_VTTH_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        *vtth = param.vtth;
    }
    return ret;
}

#ifdef CONFIG_OT_VO_LESS_BUF

td_s32 ot_mpi_vo_set_less_buf_attr(ot_vo_dev dev, const ot_vo_less_buf_attr *less_buf_attr)
{
    vo_ioctl_less_buf_attr_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(less_buf_attr);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    (td_void)memcpy_s(&param.less_buf_attr, sizeof(ot_vo_less_buf_attr), less_buf_attr, sizeof(ot_vo_less_buf_attr));
    return ioctl(g_vo_fd, VO_DEV_LESS_BUF_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_less_buf_attr(ot_vo_dev dev, ot_vo_less_buf_attr *less_buf_attr)
{
    td_s32 ret;
    vo_ioctl_less_buf_attr_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(less_buf_attr);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    ret = ioctl(g_vo_fd, VO_DEV_LESS_BUF_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        (td_void)memcpy_s(less_buf_attr, sizeof(ot_vo_less_buf_attr), &param.less_buf_attr,
            sizeof(ot_vo_less_buf_attr));
    }
    return ret;
}

#else

td_s32 ot_mpi_vo_set_less_buf_attr(ot_vo_dev dev, const ot_vo_less_buf_attr *less_buf_attr)
{
    ot_unused(dev);
    ot_unused(less_buf_attr);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_get_less_buf_attr(ot_vo_dev dev, ot_vo_less_buf_attr *less_buf_attr)
{
    ot_unused(dev);
    ot_unused(less_buf_attr);
    return vo_mpi_check_not_support();
}

#endif

#ifdef CONFIG_OT_VO_USER_NOTIFY
td_s32 ot_mpi_vo_set_user_notify_attr(ot_vo_dev dev, const ot_vo_user_notify_attr *user_notify_attr)
{
    vo_ioctl_user_notify_attr_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(user_notify_attr);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    (td_void)memcpy_s(&param.user_notify_attr, sizeof(ot_vo_user_notify_attr),
        user_notify_attr, sizeof(ot_vo_user_notify_attr));
    return ioctl(g_vo_fd, VO_DEV_USER_NOTIFY_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_user_notify_attr(ot_vo_dev dev, ot_vo_user_notify_attr *user_notify_attr)
{
    td_s32 ret;
    vo_ioctl_user_notify_attr_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(user_notify_attr);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    ret = ioctl(g_vo_fd, VO_DEV_USER_NOTIFY_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        (td_void)memcpy_s(user_notify_attr, sizeof(ot_vo_user_notify_attr), &param.user_notify_attr,
            sizeof(ot_vo_user_notify_attr));
    }
    return ret;
}

#else

td_s32 ot_mpi_vo_set_user_notify_attr(ot_vo_dev dev, const ot_vo_user_notify_attr *user_notify_attr)
{
    ot_unused(dev);
    ot_unused(user_notify_attr);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_get_user_notify_attr(ot_vo_dev dev, ot_vo_user_notify_attr *user_notify_attr)
{
    ot_unused(dev);
    ot_unused(user_notify_attr);
    return vo_mpi_check_not_support();
}

#endif

#endif

#if vo_desc("device interface mpi")

#if defined(CONFIG_OT_VO_VGA) || defined(CONFIG_OT_VO_CVBS)

td_s32 ot_mpi_vo_query_intf_status(ot_vo_intf_type intf_type, ot_vo_intf_status *status)
{
    td_s32 ret;
    vo_intf_status_info intf_info;

    vo_mpi_check_null_ptr_return(status);
    vo_mpi_check_open_return();

    intf_info.intf_type = intf_type;
    intf_info.intf_status.plug_status = OT_VO_INTF_STATUS_BUTT;
    ret = ioctl(g_vo_fd, VO_QUERY_INTF_STATUS, &intf_info);
    if (ret == TD_SUCCESS) {
        (td_void)memcpy_s(status, sizeof(ot_vo_intf_status), &intf_info.intf_status, sizeof(ot_vo_intf_status));
    }
    return ret;
}

#else

td_s32 ot_mpi_vo_query_intf_status(ot_vo_intf_type intf_type, ot_vo_intf_status *status)
{
    ot_unused(intf_type);
    ot_unused(status);
    return vo_mpi_check_not_support();
}

#endif

#ifdef CONFIG_OT_VO_VGA

td_s32 ot_mpi_vo_set_vga_param(ot_vo_dev dev, const ot_vo_vga_param *vga_param)
{
    vo_ioctl_vga_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(vga_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    (td_void)memcpy_s(&param.vga_param, sizeof(ot_vo_vga_param), vga_param, sizeof(ot_vo_vga_param));
    return ioctl(g_vo_fd, VO_VGA_PARAM_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_vga_param(ot_vo_dev dev, ot_vo_vga_param *vga_param)
{
    td_s32 ret;
    vo_ioctl_vga_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(vga_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    ret = ioctl(g_vo_fd, VO_VGA_PARAM_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        (td_void)memcpy_s(vga_param, sizeof(ot_vo_vga_param), &param.vga_param, sizeof(ot_vo_vga_param));
    }
    return ret;
}

#else

td_s32 ot_mpi_vo_set_vga_param(ot_vo_dev dev, const ot_vo_vga_param *vga_param)
{
    ot_unused(dev);
    ot_unused(vga_param);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_get_vga_param(ot_vo_dev dev, ot_vo_vga_param *vga_param)
{
    ot_unused(dev);
    ot_unused(vga_param);
    return vo_mpi_check_not_support();
}

#endif

#ifdef CONFIG_OT_VO_HDMI
td_s32 ot_mpi_vo_set_hdmi_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi_param)
{
    vo_ioctl_hdmi_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(hdmi_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    (td_void)memcpy_s(&param.hdmi_param, sizeof(ot_vo_hdmi_param), hdmi_param, sizeof(ot_vo_hdmi_param));
    return ioctl(g_vo_fd, VO_HDMI_PARAM_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_hdmi_param(ot_vo_dev dev, ot_vo_hdmi_param *hdmi_param)
{
    td_s32 ret;
    vo_ioctl_hdmi_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(hdmi_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    ret = ioctl(g_vo_fd, VO_HDMI_PARAM_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        (td_void)memcpy_s(hdmi_param, sizeof(ot_vo_hdmi_param), &param.hdmi_param, sizeof(ot_vo_hdmi_param));
    }
    return ret;
}

#else

td_s32 ot_mpi_vo_set_hdmi_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi_param)
{
    ot_unused(dev);
    ot_unused(hdmi_param);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_get_hdmi_param(ot_vo_dev dev, ot_vo_hdmi_param *hdmi_param)
{
    ot_unused(dev);
    ot_unused(hdmi_param);
    return vo_mpi_check_not_support();
}

#endif

#ifdef CONFIG_OT_VO_HDMI1

td_s32 ot_mpi_vo_set_hdmi1_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi1_param)
{
    vo_ioctl_hdmi_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(hdmi1_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    (td_void)memcpy_s(&param.hdmi_param, sizeof(ot_vo_hdmi_param), hdmi1_param, sizeof(ot_vo_hdmi_param));
    return ioctl(g_vo_fd, VO_HDMI1_PARAM_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_hdmi1_param(ot_vo_dev dev, ot_vo_hdmi_param *hdmi1_param)
{
    td_s32 ret;
    vo_ioctl_hdmi_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(hdmi1_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    ret = ioctl(g_vo_fd, VO_HDMI1_PARAM_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        (td_void)memcpy_s(hdmi1_param, sizeof(ot_vo_hdmi_param), &param.hdmi_param, sizeof(ot_vo_hdmi_param));
    }
    return ret;
}

#else

td_s32 ot_mpi_vo_set_hdmi1_param(ot_vo_dev dev, const ot_vo_hdmi_param *hdmi1_param)
{
    ot_unused(dev);
    ot_unused(hdmi1_param);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_get_hdmi1_param(ot_vo_dev dev, ot_vo_hdmi_param *hdmi1_param)
{
    ot_unused(dev);
    ot_unused(hdmi1_param);
    return vo_mpi_check_not_support();
}

#endif

#ifdef CONFIG_OT_VO_RGB

td_s32 ot_mpi_vo_set_rgb_param(ot_vo_dev dev, const ot_vo_rgb_param *rgb_param)
{
    vo_ioctl_rgb_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(rgb_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    (td_void)memcpy_s(&param.rgb_param, sizeof(ot_vo_rgb_param), rgb_param, sizeof(ot_vo_rgb_param));
    return ioctl(g_vo_fd, VO_RGB_PARAM_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_rgb_param(ot_vo_dev dev, ot_vo_rgb_param *rgb_param)
{
    td_s32 ret;
    vo_ioctl_rgb_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(rgb_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    ret = ioctl(g_vo_fd, VO_RGB_PARAM_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        (td_void)memcpy_s(rgb_param, sizeof(ot_vo_rgb_param), &param.rgb_param, sizeof(ot_vo_rgb_param));
    }
    return ret;
}

#else

td_s32 ot_mpi_vo_set_rgb_param(ot_vo_dev dev, const ot_vo_rgb_param *rgb_param)
{
    ot_unused(dev);
    ot_unused(rgb_param);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_get_rgb_param(ot_vo_dev dev, ot_vo_rgb_param *rgb_param)
{
    ot_unused(dev);
    ot_unused(rgb_param);
    return vo_mpi_check_not_support();
}

#endif

#ifdef CONFIG_OT_VO_BT1120

td_s32 ot_mpi_vo_set_bt_param(ot_vo_dev dev, const ot_vo_bt_param *bt_param)
{
    vo_ioctl_bt_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(bt_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    (td_void)memcpy_s(&param.bt_param, sizeof(ot_vo_bt_param), bt_param, sizeof(ot_vo_bt_param));
    return ioctl(g_vo_fd, VO_BT_PARAM_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_bt_param(ot_vo_dev dev, ot_vo_bt_param *bt_param)
{
    td_s32 ret;
    vo_ioctl_bt_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(bt_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    ret = ioctl(g_vo_fd, VO_BT_PARAM_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        (td_void)memcpy_s(bt_param, sizeof(ot_vo_bt_param), &param.bt_param, sizeof(ot_vo_bt_param));
    }
    return ret;
}

#else

td_s32 ot_mpi_vo_set_bt_param(ot_vo_dev dev, const ot_vo_bt_param *bt_param)
{
    ot_unused(dev);
    ot_unused(bt_param);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_get_bt_param(ot_vo_dev dev, ot_vo_bt_param *bt_param)
{
    ot_unused(dev);
    ot_unused(bt_param);
    return vo_mpi_check_not_support();
}

#endif

#ifdef CONFIG_OT_VO_MIPI

td_s32 ot_mpi_vo_set_mipi_param(ot_vo_dev dev, const ot_vo_mipi_param *mipi_param)
{
    vo_ioctl_mipi_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(mipi_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    (td_void)memcpy_s(&param.mipi_param, sizeof(ot_vo_mipi_param), mipi_param, sizeof(ot_vo_mipi_param));
    return ioctl(g_vo_fd, VO_MIPI_PARAM_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_mipi_param(ot_vo_dev dev, ot_vo_mipi_param *mipi_param)
{
    td_s32 ret;
    vo_ioctl_mipi_param_param param = {0};

    vo_mpi_check_dev_id_return(dev);
    vo_mpi_check_null_ptr_return(mipi_param);
    vo_mpi_check_open_return();

    param.ioctl_dev.dev = dev;
    ret = ioctl(g_vo_fd, VO_MIPI_PARAM_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        (td_void)memcpy_s(mipi_param, sizeof(ot_vo_mipi_param), &param.mipi_param, sizeof(ot_vo_mipi_param));
    }
    return ret;
}

#else

td_s32 ot_mpi_vo_set_mipi_param(ot_vo_dev dev, const ot_vo_mipi_param *mipi_param)
{
    ot_unused(dev);
    ot_unused(mipi_param);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_get_mipi_param(ot_vo_dev dev, ot_vo_mipi_param *mipi_param)
{
    ot_unused(dev);
    ot_unused(mipi_param);
    return vo_mpi_check_not_support();
}

#endif

#endif

#if vo_desc("cascade mpi")

#ifdef CONFIG_OT_VO_CASCADE

td_s32 ot_mpi_vo_set_cas_attr(const ot_vo_cas_attr *cas_attr)
{
    vo_mpi_check_null_ptr_return(cas_attr);
    vo_mpi_check_open_return();

    return ioctl(g_vo_fd, VO_CAS_ATTR_SET_CTRL, cas_attr);
}

td_s32 ot_mpi_vo_get_cas_attr(ot_vo_cas_attr *cas_attr)
{
    vo_mpi_check_null_ptr_return(cas_attr);
    vo_mpi_check_open_return();

    return ioctl(g_vo_fd, VO_CAS_ATTR_GET_CTRL, cas_attr);
}

td_s32 ot_mpi_vo_enable_cas_dev(ot_vo_dev cas_dev)
{
    vo_ioctl_dev_param param = {0};

    vo_mpi_check_dev_id_return(cas_dev);
    vo_mpi_check_open_return();

    param.dev = cas_dev;
    return ioctl(g_vo_fd, VO_CAS_ENABLE_DEV_CTRL, &param);
}

td_s32 ot_mpi_vo_disable_cas_dev(ot_vo_dev cas_dev)
{
    vo_ioctl_dev_param param = {0};

    vo_mpi_check_dev_id_return(cas_dev);
    vo_mpi_check_open_return();

    param.dev = cas_dev;
    return ioctl(g_vo_fd, VO_CAS_DISABLE_DEV_CTRL, &param);
}

td_s32 ot_mpi_vo_set_cas_pattern(ot_vo_dev cas_dev, td_u32 pattern)
{
    vo_ioctl_cas_pattern_param param = {0};

    vo_mpi_check_dev_id_return(cas_dev);
    vo_mpi_check_open_return();

    param.cas_dev.dev = cas_dev;
    param.pattern = pattern;
    return ioctl(g_vo_fd, VO_CAS_PATTERN_SET_CTRL, &param);
}

td_s32 ot_mpi_vo_get_cas_pattern(ot_vo_dev cas_dev, td_u32 *pattern)
{
    td_s32 ret;
    vo_ioctl_cas_pattern_param param = {0};

    vo_mpi_check_dev_id_return(cas_dev);
    vo_mpi_check_null_ptr_return(pattern);
    vo_mpi_check_open_return();

    param.cas_dev.dev = cas_dev;
    ret = ioctl(g_vo_fd, VO_CAS_PATTERN_GET_CTRL, &param);
    if (ret == TD_SUCCESS) {
        *pattern = param.pattern;
    }
    return ret;
}

td_s32 ot_mpi_vo_enable_cas(td_void)
{
    vo_mpi_check_open_return();

    return ioctl(g_vo_fd, VO_CAS_ENABLE_CTRL, TD_NULL);
}

td_s32 ot_mpi_vo_disable_cas(td_void)
{
    vo_mpi_check_open_return();

    return ioctl(g_vo_fd, VO_CAS_DISABLE_CTRL, TD_NULL);
}

#else

td_s32 ot_mpi_vo_set_cas_attr(const ot_vo_cas_attr *cas_attr)
{
    ot_unused(cas_attr);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_get_cas_attr(ot_vo_cas_attr *cas_attr)
{
    ot_unused(cas_attr);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_enable_cas_dev(ot_vo_dev cas_dev)
{
    ot_unused(cas_dev);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_disable_cas_dev(ot_vo_dev cas_dev)
{
    ot_unused(cas_dev);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_set_cas_pattern(ot_vo_dev cas_dev, td_u32 pattern)
{
    ot_unused(cas_dev);
    ot_unused(pattern);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_get_cas_pattern(ot_vo_dev cas_dev, td_u32 *pattern)
{
    ot_unused(cas_dev);
    ot_unused(pattern);
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_enable_cas(td_void)
{
    return vo_mpi_check_not_support();
}

td_s32 ot_mpi_vo_disable_cas(td_void)
{
    return vo_mpi_check_not_support();
}

#endif

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
