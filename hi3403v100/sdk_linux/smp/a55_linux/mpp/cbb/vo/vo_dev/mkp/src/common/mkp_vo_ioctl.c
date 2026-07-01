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
#include "mkp_vo_ioctl.h"
#include "mkp_vo_intf.h"
#include "mkp_vo_intf_detect.h"
#include "mkp_vo_cascade.h"
#include "osal_ioctl.h"

#if vo_desc("dev ioctl")

static td_s32 vo_ioctl_cmd_dev_pub(const vo_ioctl_para *ioctl_para, td_bool *find_flag)
{
    td_uintptr_t arg = ioctl_para->arg;

    *find_flag = TD_TRUE;

    switch (ioctl_para->cmd) {
        case VO_OPEN_CTRL:
            return vo_ioctl_enable_disable((vo_ioctl_dev_param *)arg, VO_IOCTL_TYPE_ENABLE);

        case VO_CLOSE_CTRL:
            return vo_ioctl_enable_disable((vo_ioctl_dev_param *)arg, VO_IOCTL_TYPE_DISABLE);

        case VO_PUB_ATTR_SET_CTRL:
            return vo_ioctl_pub_attr((vo_ioctl_pub_attr_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_PUB_ATTR_GET_CTRL:
            return vo_ioctl_pub_attr((vo_ioctl_pub_attr_param *)arg, VO_IOCTL_TYPE_GET);

        case VO_DEV_FPS_SET_CTRL:
            return vo_ioctl_dev_frame_rate((vo_ioctl_dev_frame_rate_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_DEV_FPS_GET_CTRL:
            return vo_ioctl_dev_frame_rate((vo_ioctl_dev_frame_rate_param *)arg, VO_IOCTL_TYPE_GET);

        case VO_USER_SYNC_INFO_SET_CTRL:
            return vo_ioctl_user_sync_info((vo_ioctl_user_sync_info_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_USER_SYNC_INFO_GET_CTRL:
            return vo_ioctl_user_sync_info((vo_ioctl_user_sync_info_param *)arg, VO_IOCTL_TYPE_GET);

        default:
            *find_flag = TD_FALSE;
            break;
    }

    return OT_ERR_VO_ILLEGAL_PARAM;
}

static td_s32 vo_ioctl_cmd_dev_cfg(const vo_ioctl_para *ioctl_para, td_bool *find_flag)
{
    td_uintptr_t arg = ioctl_para->arg;

    *find_flag = TD_TRUE;

    switch (ioctl_para->cmd) {
#ifdef CONFIG_OT_VO_DEV_BYPASS
        case VO_DEV_PARAM_SET_CTRL:
            return vo_ioctl_oper_dev_param((vo_ioctl_dev_param_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_DEV_PARAM_GET_CTRL:
            return vo_ioctl_oper_dev_param((vo_ioctl_dev_param_param *)arg, VO_IOCTL_TYPE_GET);
#endif

        case VO_MOD_PARAM_SET_CTRL:
            return vo_ioctl_mod_param((ot_vo_mod_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_MOD_PARAM_GET_CTRL:
            return vo_ioctl_mod_param((ot_vo_mod_param *)arg, VO_IOCTL_TYPE_GET);

        case VO_DEV_VTTH_SET_CTRL:
            return vo_ioctl_vtth((vo_ioctl_vtth_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_DEV_VTTH_GET_CTRL:
            return vo_ioctl_vtth((vo_ioctl_vtth_param *)arg, VO_IOCTL_TYPE_GET);

#ifdef CONFIG_OT_VO_LESS_BUF
        case VO_DEV_LESS_BUF_SET_CTRL:
            return vo_ioctl_less_buf_attr((vo_ioctl_less_buf_attr_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_DEV_LESS_BUF_GET_CTRL:
            return vo_ioctl_less_buf_attr((vo_ioctl_less_buf_attr_param *)arg, VO_IOCTL_TYPE_GET);
#endif
#ifdef CONFIG_OT_VO_USER_NOTIFY
        case VO_DEV_USER_NOTIFY_SET_CTRL:
            return vo_ioctl_user_notify_attr((vo_ioctl_user_notify_attr_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_DEV_USER_NOTIFY_GET_CTRL:
            return vo_ioctl_user_notify_attr((vo_ioctl_user_notify_attr_param *)arg, VO_IOCTL_TYPE_GET);
#endif
        default:
            *find_flag = TD_FALSE;
            break;
    }

    return OT_ERR_VO_ILLEGAL_PARAM;
}

static td_s32 vo_ioctl_cmd_dev_intf(const vo_ioctl_para *ioctl_para, td_bool *find_flag)
{
    td_uintptr_t arg = ioctl_para->arg;

    *find_flag = TD_TRUE;

    switch (ioctl_para->cmd) {
#if defined(CONFIG_OT_VO_VGA) || defined(CONFIG_OT_VO_CVBS)
        case VO_QUERY_INTF_STATUS:
            return vo_query_intf_status((vo_intf_status_info *)arg);
#endif

#ifdef CONFIG_OT_VO_VGA
        case VO_VGA_PARAM_SET_CTRL:
            return vo_ioctl_vga_param((vo_ioctl_vga_param_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_VGA_PARAM_GET_CTRL:
            return vo_ioctl_vga_param((vo_ioctl_vga_param_param *)arg, VO_IOCTL_TYPE_GET);
#endif

#ifdef CONFIG_OT_VO_HDMI
        case VO_HDMI_PARAM_SET_CTRL:
            return vo_ioctl_hdmi_param((vo_ioctl_hdmi_param_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_HDMI_PARAM_GET_CTRL:
            return vo_ioctl_hdmi_param((vo_ioctl_hdmi_param_param *)arg, VO_IOCTL_TYPE_GET);
#endif

#ifdef CONFIG_OT_VO_HDMI1
        case VO_HDMI1_PARAM_SET_CTRL:
            return vo_ioctl_hdmi1_param((vo_ioctl_hdmi_param_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_HDMI1_PARAM_GET_CTRL:
            return vo_ioctl_hdmi1_param((vo_ioctl_hdmi_param_param *)arg, VO_IOCTL_TYPE_GET);
#endif

#ifdef CONFIG_OT_VO_RGB
        case VO_RGB_PARAM_SET_CTRL:
            return vo_ioctl_rgb_param((vo_ioctl_rgb_param_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_RGB_PARAM_GET_CTRL:
            return vo_ioctl_rgb_param((vo_ioctl_rgb_param_param *)arg, VO_IOCTL_TYPE_GET);
#endif

#ifdef CONFIG_OT_VO_BT1120
        case VO_BT_PARAM_SET_CTRL:
            return vo_ioctl_bt_param((vo_ioctl_bt_param_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_BT_PARAM_GET_CTRL:
            return vo_ioctl_bt_param((vo_ioctl_bt_param_param *)arg, VO_IOCTL_TYPE_GET);
#endif

#ifdef CONFIG_OT_VO_MIPI
        case VO_MIPI_PARAM_SET_CTRL:
            return vo_ioctl_mipi_param((vo_ioctl_mipi_param_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_MIPI_PARAM_GET_CTRL:
            return vo_ioctl_mipi_param((vo_ioctl_mipi_param_param *)arg, VO_IOCTL_TYPE_GET);
#endif

        default:
            *find_flag = TD_FALSE;
            break;
    }

    return OT_ERR_VO_ILLEGAL_PARAM;
}

static td_s32 vo_ioctl_cmd_dev(const vo_ioctl_para *ioctl_para, td_bool *find_flag)
{
    td_s32 ret;

    ret = vo_ioctl_cmd_dev_pub(ioctl_para, find_flag);
    if (*find_flag == TD_TRUE) {
        return ret;
    }

    ret = vo_ioctl_cmd_dev_cfg(ioctl_para, find_flag);
    if (*find_flag == TD_TRUE) {
        return ret;
    }

    ret = vo_ioctl_cmd_dev_intf(ioctl_para, find_flag);
    if (*find_flag == TD_TRUE) {
        return ret;
    }

    return OT_ERR_VO_ILLEGAL_PARAM;
}

#endif

#if vo_desc("cascade ioctl")

#ifdef CONFIG_OT_VO_CASCADE

static td_s32 vo_ioctl_cmd_cas_cfg(vo_ioctl_para *ioctl_para, td_bool *find_flag)
{
    td_uintptr_t arg = ioctl_para->arg;

    *find_flag = TD_TRUE;

    switch (ioctl_para->cmd) {
        case VO_CAS_ATTR_SET_CTRL:
            return vo_ioctl_cas_attr((ot_vo_cas_attr *)arg, VO_IOCTL_TYPE_SET);

        case VO_CAS_ATTR_GET_CTRL:
            return vo_ioctl_cas_attr((ot_vo_cas_attr *)arg, VO_IOCTL_TYPE_GET);

        case VO_CAS_PATTERN_SET_CTRL:
            return vo_ioctl_cas_pattern((vo_ioctl_cas_pattern_param *)arg, VO_IOCTL_TYPE_SET);

        case VO_CAS_PATTERN_GET_CTRL:
            return vo_ioctl_cas_pattern((vo_ioctl_cas_pattern_param *)arg, VO_IOCTL_TYPE_GET);

        default:
            *find_flag = TD_FALSE;
            break;
    }

    return OT_ERR_VO_ILLEGAL_PARAM;
}

static td_s32 vo_ioctl_cmd_cas_oper(vo_ioctl_para *ioctl_para, td_bool *find_flag)
{
    td_uintptr_t arg = ioctl_para->arg;

    *find_flag = TD_TRUE;

    switch (ioctl_para->cmd) {
        case VO_CAS_ENABLE_DEV_CTRL:
            return vo_ioctl_enable_disable_cas_dev((vo_ioctl_dev_param *)arg, VO_IOCTL_TYPE_ENABLE);

        case VO_CAS_DISABLE_DEV_CTRL:
            return vo_ioctl_enable_disable_cas_dev((vo_ioctl_dev_param *)arg, VO_IOCTL_TYPE_DISABLE);

        case VO_CAS_ENABLE_CTRL:
            return vo_ioctl_enable_disable_cas(VO_IOCTL_TYPE_ENABLE);

        case VO_CAS_DISABLE_CTRL:
            return vo_ioctl_enable_disable_cas(VO_IOCTL_TYPE_DISABLE);

        default:
            *find_flag = TD_FALSE;
            break;
    }

    return OT_ERR_VO_ILLEGAL_PARAM;
}

static td_s32 vo_ioctl_cmd_cas(vo_ioctl_para *ioctl_para, td_bool *find_flag)
{
    td_s32 ret;

    ret = vo_ioctl_cmd_cas_cfg(ioctl_para, find_flag);
    if (*find_flag == TD_TRUE) {
        return ret;
    }

    ret = vo_ioctl_cmd_cas_oper(ioctl_para, find_flag);
    if (*find_flag == TD_TRUE) {
        return ret;
    }

    return OT_ERR_VO_ILLEGAL_PARAM;
}

#endif

#endif

#if vo_desc("vo ioctl")

#ifdef OT_DEBUG

static vo_ioctl_cmd g_vo_ioctl_cmd[] = {
    { VO_OPEN_CTRL,                     "enable_dev" },
    { VO_CLOSE_CTRL,                    "disable_dev" },
    { VO_PUB_ATTR_SET_CTRL,             "set_pub_attr" },
    { VO_PUB_ATTR_GET_CTRL,             "get_pub_attr" },
    { VO_DEV_PARAM_SET_CTRL,            "set_dev_param" },
    { VO_DEV_PARAM_GET_CTRL,            "get_dev_param" },
    { VO_DEV_FPS_SET_CTRL,              "set_dev_frame_rate" },
    { VO_DEV_FPS_GET_CTRL,              "get_dev_frame_rate" },
    { VO_USER_SYNC_INFO_SET_CTRL,       "set_user_sync_info" },
    { VO_USER_SYNC_INFO_GET_CTRL,       "get_user_sync_info" },
    { VO_MOD_PARAM_SET_CTRL,            "set_mod_param" },
    { VO_MOD_PARAM_GET_CTRL,            "get_mod_param" },
    { VO_DEV_VTTH_SET_CTRL,             "set_vtth" },
    { VO_DEV_VTTH_GET_CTRL,             "get_vtth" },
    { VO_DEV_LESS_BUF_SET_CTRL,         "set_less_buf" },
    { VO_DEV_LESS_BUF_GET_CTRL,         "get_less_buf" },
    { VO_DEV_USER_NOTIFY_SET_CTRL,      "set_user_notify" },
    { VO_DEV_USER_NOTIFY_GET_CTRL,      "get_user_notify" },

    { VO_QUERY_INTF_STATUS,             "query_intf_status" },
    { VO_VGA_PARAM_SET_CTRL,            "set_vga_param" },
    { VO_VGA_PARAM_GET_CTRL,            "get_vga_param" },
    { VO_HDMI_PARAM_SET_CTRL,           "set_hdmi_param" },
    { VO_HDMI_PARAM_GET_CTRL,           "get_hdmi_param" },
    { VO_HDMI1_PARAM_SET_CTRL,          "set_hdmi1_param" },
    { VO_HDMI1_PARAM_GET_CTRL,          "get_hdmi1_param" },
    { VO_RGB_PARAM_SET_CTRL,            "set_rgb_param" },
    { VO_RGB_PARAM_GET_CTRL,            "get_rgb_param" },
    { VO_BT_PARAM_SET_CTRL,             "set_bt_param" },
    { VO_BT_PARAM_GET_CTRL,             "get_bt_param" },
    { VO_MIPI_PARAM_SET_CTRL,           "set_mipi_param" },
    { VO_MIPI_PARAM_GET_CTRL,           "get_mipi_param" },

    { VO_VIDEO_ENABLE_CTRL,             "enable_video_layer" },
    { VO_VIDEO_DISABLE_CTRL,            "disable_video_layer" },
    { VO_VIDEO_ATTR_SET_CTRL,           "set_video_layer_attr" },
    { VO_VIDEO_ATTR_GET_CTRL,           "get_video_layer_attr" },
    { VO_VIDEO_PARAM_SET_CTRL,          "set_video_layer_param" },
    { VO_VIDEO_PARAM_GET_CTRL,          "get_video_layer_param" },
    { VO_VIDEO_CROP_SET_CTRL,           "set_video_layer_crop" },
    { VO_VIDEO_CROP_GET_CTRL,           "get_video_layer_crop" },
    { VO_VIDEO_EARLY_SET_CTRL,          "set_video_layer_early_display" },
    { VO_VIDEO_EARLY_GET_CTRL,          "get_video_layer_early_display" },
    { VO_VIDEO_TOL_SET_CTRL,            "set_play_toleration" },
    { VO_VIDEO_TOL_GET_CTRL,            "get_play_toleration" },
    { VO_ATTR_BEGIN_SET_CTRL,           "batch_begin" },
    { VO_ATTR_END_SET_CTRL,             "batch_end" },
    { VO_VIDEO_GET_FRAME_CTRL,          "get_screen_frame" },
    { VO_VIDEO_RLS_FRAME_CTRL,          "release_screen_frame" },

    { VO_LAYER_PRIORITY_SET_CTRL,       "set_layer_priority" },
    { VO_LAYER_PRIORITY_GET_CTRL,       "get_layer_priority" },
    { VO_LAYER_CSC_SET_CTRL,            "set_layer_csc" },
    { VO_LAYER_CSC_GET_CTRL,            "get_layer_csc" },
    { VO_LAYER_BIND_CTRL,               "bind_layer" },
    { VO_LAYER_UNBIND_CTRL,             "unbind_layer" },

    { VO_CHN_ENABLE_CTRL,               "enable_chn" },
    { VO_CHN_DISABLE_CTRL,              "disable_chn" },
    { VO_CHN_ATTR_SET_CTRL,             "set_chn_attr" },
    { VO_CHN_ATTR_GET_CTRL,             "get_chn_attr" },
    { VO_CHN_PARAM_SET_CTRL,            "set_chn_param" },
    { VO_CHN_PARAM_GET_CTRL,            "get_chn_param" },
    { VO_CHN_DISP_POS_SET_CTRL,         "set_chn_display_pos" },
    { VO_CHN_DISP_POS_GET_CTRL,         "get_chn_display_pos" },
    { VO_CHN_GET_FRAME_CTRL,            "get_chn_frame" },
    { VO_CHN_RLS_FRAME_CTRL,            "release_chn_frame" },
    { VO_CHN_SEND_FRAME_CTRL,           "send_frame" },
    { VO_CHN_FRAMERATE_GET_CTRL,        "get_chn_frame_rate" },
    { VO_CHN_FRAMERATE_SET_CTRL,        "set_chn_frame_rate" },
    { VO_CHN_PAUSE_SET_CTRL,            "pause_chn" },
    { VO_CHN_RESUME_SET_CTRL,           "resume_chn" },
    { VO_CHN_STEP_SET_CTRL,             "step_chn" },
    { VO_CHN_REFRESH_SET_CTRL,          "refresh_chn" },
    { VO_CHN_SHOW_CTRL,                 "show_chn" },
    { VO_CHN_HIDE_CTRL,                 "hide_chn" },
    { VO_CHN_ZOOM_SET_CTRL,             "set_zoom_in_window" },
    { VO_CHN_ZOOM_GET_CTRL,             "get_zoom_in_window" },
    { VO_CHN_PTS_GET_CTRL,              "get_chn_pts" },
    { VO_CHN_STATUS_CTRL,               "query_chn_status" },
    { VO_CHN_CLRBUF_CTRL,               "clear_chn_buf" },
    { VO_CHN_BORDER_SET_CTRL,           "set_chn_border" },
    { VO_CHN_BORDER_GET_CTRL,           "get_chn_border" },
    { VO_CHN_THRESHOLD_SET_CTRL,        "set_chn_recv_threshold" },
    { VO_CHN_THRESHOLD_GET_CTRL,        "get_chn_recv_threshold" },
    { VO_CHN_ROTATION_SET_CTRL,         "set_chn_rotation" },
    { VO_CHN_ROTATION_GET_CTRL,         "get_chn_rotation" },
    { VO_CHN_MIRROR_SET_CTRL,           "set_chn_mirror" },
    { VO_CHN_MIRROR_GET_CTRL,           "get_chn_mirror" },
    { VO_CHN_GET_REGION_LUMA,           "get_chn_rgn_luma" },

    { VO_WBC_SOURCE_SET_CTRL,           "set_wbc_src" },
    { VO_WBC_SOURCE_GET_CTRL,           "get_wbc_src" },
    { VO_WBC_ENABLE_CTRL,               "enable_wbc" },
    { VO_WBC_DISABLE_CTRL,              "disable_wbc" },
    { VO_WBC_ATTR_SET_CTRL,             "set_wbc_attr" },
    { VO_WBC_ATTR_GET_CTRL,             "get_wbc_attr" },
    { VO_WBC_DEPTH_SET_CTRL,            "set_wbc_depth" },
    { VO_WBC_DEPTH_GET_CTRL,            "get_wbc_depth" },
    { VO_WBC_MODE_SET_CTRL,             "set_wbc_mode" },
    { VO_WBC_MODE_GET_CTRL,             "get_wbc_mode" },
    { VO_WBC_GET_FRAME_CTRL,            "get_wbc_frame" },
    { VO_WBC_RLS_FRAME_CTRL,            "release_wbc_frame" },

    { VO_CAS_ATTR_SET_CTRL,             "set_cas_attr" },
    { VO_CAS_ATTR_GET_CTRL,             "get_cas_attr" },
    { VO_CAS_ENABLE_DEV_CTRL,           "enable_cas_dev" },
    { VO_CAS_DISABLE_DEV_CTRL,          "disable_cas_dev" },
    { VO_CAS_PATTERN_SET_CTRL,          "set_cas_pattern" },
    { VO_CAS_PATTERN_GET_CTRL,          "get_cas_pattern" },
    { VO_CAS_BINDCHN_CTRL,              "bind_cas_chn" },
    { VO_CAS_UNBINDCHN_CTRL,            "unbind_cas_chn" },
    { VO_CAS_ENABLE_CTRL,               "enable_cas" },
    { VO_CAS_DISABLE_CTRL,              "disable_cas" }
};

static td_char *vo_get_ioctl_cmd_name(unsigned int cmd)
{
    td_u32 loop;
    td_u32 len = sizeof(g_vo_ioctl_cmd) / sizeof(vo_ioctl_cmd);

    for (loop = 0; loop < len; loop++) {
        if (cmd == g_vo_ioctl_cmd[loop].cmd) {
            return g_vo_ioctl_cmd[loop].cmd_name;
        }
    }

    vo_err_trace("not support cmd: %#x\n", cmd);
    return "-";
}

static td_void vo_print_ioctl_cmd(unsigned int cmd)
{
    vo_debug_trace("cmd: %#x cmd_name: %s\n", cmd, vo_get_ioctl_cmd_name(cmd));
}

#else

static td_void vo_print_ioctl_cmd(unsigned int cmd)
{
    vo_debug_trace("cmd: %#x\n", cmd);
}

#endif

td_s32 vo_device_ioctl(vo_ioctl_para *ioctl_para)
{
    td_s32 ret;
    td_bool find_flag = TD_FALSE;

    vo_print_ioctl_cmd(ioctl_para->cmd);

    /* vo只有级联的两个接口没有参数，其他均传入参数，统一校验参数有效性 */
    if ((OSAL_IOC_DIR(ioctl_para->cmd) != OSAL_IOC_NONE) &&
        (((char *)(td_uintptr_t)ioctl_para->arg) == TD_NULL)) {
        vo_err_trace("cmd: %#x arg null ptr!\n", ioctl_para->cmd);
        return OT_ERR_VO_NULL_PTR;
    }

    ret = vo_ioctl_cmd_dev(ioctl_para, &find_flag);
    if (find_flag == TD_TRUE) {
        return ret;
    }

#ifdef CONFIG_OT_VO_CASCADE
    ret = vo_ioctl_cmd_cas(ioctl_para, &find_flag);
    if (find_flag == TD_TRUE) {
        return ret;
    }
#endif

    vo_err_trace("ERR IOCTL CMD %#x!\n", ioctl_para->cmd);
    return OT_ERR_VO_NOT_SUPPORT;
}

#ifdef CONFIG_COMPAT
td_s32 vo_device_compat_ioctl(vo_ioctl_para *ioctl_para)
{
    vo_region_luma *vo_get_region_luma = TD_NULL;

    if (ioctl_para->cmd == VO_CHN_GET_REGION_LUMA) {
        vo_get_region_luma = (vo_region_luma *)ioctl_para->arg;
        OT_COMPAT_POINTER(vo_get_region_luma->luma_data, td_u64 *);
        OT_COMPAT_POINTER(vo_get_region_luma->rgn, ot_rect *);
    }

    return vo_device_ioctl(ioctl_para);
}
#endif

#endif

