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
#ifndef MKP_VO_DEV_H
#define MKP_VO_DEV_H

#include "ot_common_video.h"
#include "ot_common_vo.h"
#include "mkp_vo.h"
#include "mkp_vo_comm.h"
#include "ot_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define VO_LOADDETECT_MIN_CYCLE     10
#define VO_LOADDETECT_DEFAULT_CYCLE 30
#define VO_DEV_MAX_FRAME_RATE       240
#define VO_MIN_VTTH_WATERLINE       2
#define VO_MAX_VTTH_WATERLINE       8191
#define VO_DISP_FREQ_VGA  60

#define VO_DISP_1080_WIDTH 1920
#define VO_DISP_1080_HEIGHT 1080

typedef struct {
    ot_vo_intf_sync intf_sync;
    td_char *name;
    td_u32 width;
    td_u32 height;
    td_u32 frame_rate;
} vo_intf_sync_info;

/* 负载检测结构 */
typedef struct {
    td_bool load_detect_enable; /* CVBS负载检测静态开关 */
    td_u32 int_cur_count; /* 垂直时序中断当前计数 */
    td_u32 int_max_count; /* 垂直时序中断最大计数，即:检测周期 */
    td_u32 int_user_count;
    td_bool is_query_intf_status;
    td_bool is_detected;
    osal_wait_t vo_wait_query_intf_status;
} vou_load_detect_info;

typedef struct {
    td_u64 int_rate; /* 统计10s的平均中断率 */
    td_u64 int_count; /* 用于统计平均中断率的中断计数 */
    td_u64 last_rate_time; /* 记录上次统计平均中断率的时间 */

    td_u32 int_tmp; /* 中断计数，用于统计每分钟中断占用时长 */
    td_u32 int_time_per_min_tmp; /* 用于统计每分钟中断占用时长 */
    td_u32 int_time_per_min; /* 每分钟中断占用时长 */

    td_u32 int_time; /* 一个vo中断处理占用时间 */
    td_u32 max_int_time; /* 一个vo中断处理最大占用时间 */

    td_u64 last_int_time; /* 上一个中断时间点 */
    td_u32 int_gap_time; /* 相邻两个中断时间间隔 */
    td_u32 max_int_gap_time; /* 相邻两个中断最大时间间隔 */

    td_u64 last_time; /* 上一个统计阶段中断时间点 */
} vo_debug_info;

typedef struct {
    td_bool cas_config;     /* 级联配置标识 */
    td_bool cas_enable;     /* 级联使能标识 */
    td_bool cas_regup;      /* 级联注册标识 */
    td_u32  cas_pattern;    /* 级联pattern */
    ot_vo_cas_attr cas_attr; /* 设备级联属性 */
} vo_dev_cas_info;

typedef struct {
    td_bool vo_enable; /* 设备使能标志 */
    td_bool config; /* 设备配置标志 */

    ot_vo_wbc wbc; /* 绑定的WBC号 */
    ot_vo_pub_attr vou_attr; /* 设备公共属性 */
    ot_vo_dev_param dev_param; /* 设备参数 */

    td_u32 max_width; /* 设备分辨率最大宽度 */
    td_u32 max_height; /* 设备分辨率最大高度 */
    td_u32 full_frame_rate; /* 设备帧率 */

    td_u32 time_ref; /* timeref,虚拟设备发送编码时，如果不打timeref则不编码 */

    vou_load_detect_info load_detect_info; /* 负载检测 */
    vo_debug_info debug_info; /* 统计各种中断信息 */

    td_bool user_config; /* 设备用户时序信息配置标志 */
    td_bool vtth1_config; /* vtth1配置标志 */
    td_bool uboot_vo_enabled;   /* 设备是否在开机画面使能过 */
    ot_vo_user_sync_info vo_user_sync_info; /* 设备用户时序信息:时钟源，时钟信息和分配比 */
    ot_vo_vga_param vga_param;       /* VGA 调节参数 */
    td_bool vga_param_config;        /* VGA 调节参数配置标记 */
    ot_vo_hdmi_param hdmi_param;      /* HDMI 调节参数 */
    ot_vo_hdmi_param hdmi1_param;     /* HDMI1 调节参数 */
    ot_vo_rgb_param rgb_param;      /* RGB 调节参数 */
    ot_vo_bt_param bt_param;      /* BT.656/BT.1120 调节参数 */
    ot_vo_mipi_param mipi_param;      /* MIPI 调节参数 */
    ot_vo_less_buf_attr less_buf_attr; /* 省BUF属性 */
    ot_vo_user_notify_attr user_notify_attr; /* 用户通知属性 */
    vo_dev_cas_info dev_cas;           /* 级联设备属性 */
} vo_dev_info;

td_s32 vo_init_dev_ctx(ot_vo_dev dev);

#define vo_init_dev_ctx_return(dev)               \
    do {                                          \
        vo_down_sem_return();                     \
        if (vo_init_dev_ctx(dev) != TD_SUCCESS) { \
            vo_up_sem();                          \
            return OT_ERR_VO_NO_MEM;              \
        }                                         \
        vo_up_sem();                              \
    } while (0)

td_void vo_init_dev_info(td_void);
td_void vo_deinit_dev_info(td_void);
vo_dev_info *vo_get_dev_ctx(ot_vo_dev vo_dev);
td_s32 vo_check_ioctl_dev(const vo_ioctl_dev_param *ioctl_dev);
td_void vo_reset_dev_vars(ot_vo_dev dev);

td_bool vo_is_dev_config_cas(ot_vo_dev dev);
td_bool vo_is_dev_enabled(ot_vo_dev dev);
td_u32 vo_get_dev_bg_color(ot_vo_dev dev);
td_bool vo_is_dev_interlaced_typical_intf_sync(ot_vo_dev dev);
td_bool vo_is_dev_interlaced_intf_sync(ot_vo_dev dev);
td_void vo_get_dev_max_size(ot_vo_dev dev, ot_size *dev_size);
td_u32 vo_get_dev_full_frame_rate(ot_vo_dev dev);
td_u32 vo_get_dev_less_buf_enable(ot_vo_dev dev);
td_u32 vo_get_dev_user_notify_enable(ot_vo_dev dev);
td_void vo_disable_all_dev_soft(td_void);
td_void vo_statistics_interrupt_time(ot_vo_dev dev, td_u64 time1);
td_char *vo_get_intf_sync_name(ot_vo_intf_sync intf_sync);

td_s32 vo_ioctl_enable_disable(const vo_ioctl_dev_param *ioctl_dev, vo_ioctl_type ioctl_type);
td_s32 vo_disable(const vo_ioctl_dev_param *ioctl_dev);
td_void vo_disable_clk(td_void);

td_s32 vo_ioctl_pub_attr(vo_ioctl_pub_attr_param *ioctl_pub_attr, vo_ioctl_type ioctl_type);

td_void vo_init_dev_param(ot_vo_dev_param *dev_param);
td_s32 vo_ioctl_oper_dev_param(vo_ioctl_dev_param_param *ioctl_dev_param, vo_ioctl_type ioctl_type);

td_s32 vo_ioctl_dev_frame_rate(vo_ioctl_dev_frame_rate_param *ioctl_frame_rate, vo_ioctl_type ioctl_type);

td_s32 vo_check_dev_user_div(ot_vo_dev dev, td_u32 dev_div, td_u32 pre_div);
td_s32 vo_get_dev_src_pixel_clk(ot_vo_dev dev, td_u32 *src_clk, td_u32 *pixel_clk);
td_s32 vo_ioctl_user_sync_info(vo_ioctl_user_sync_info_param *ioctl_sync_info, vo_ioctl_type ioctl_type);

ot_vo_mod_param *vo_get_vo_mod_param(td_void);
td_bool vo_get_mod_param_exit_dev_en(td_void);
td_bool vo_get_mod_param_dev_clk_ext_en(td_void);
td_s32 vo_ioctl_mod_param(ot_vo_mod_param *mod_param, vo_ioctl_type ioctl_type);

td_s32 vo_ioctl_vtth(vo_ioctl_vtth_param *ioctl_vtth, vo_ioctl_type ioctl_type);
td_s32 vo_ioctl_less_buf_attr(vo_ioctl_less_buf_attr_param *ioctl_less_buf, vo_ioctl_type ioctl_type);
td_s32 vo_ioctl_user_notify_attr(vo_ioctl_user_notify_attr_param *ioctl_user_notify, vo_ioctl_type ioctl_type);

td_void vo_reset_load_detect(ot_vo_dev dev);
td_void vo_init_load_detect(ot_vo_dev dev);
td_void vo_deinit_load_detect(ot_vo_dev dev);
td_void vo_load_detect(ot_vo_dev dev);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef MKP_VO_DEV_H */
