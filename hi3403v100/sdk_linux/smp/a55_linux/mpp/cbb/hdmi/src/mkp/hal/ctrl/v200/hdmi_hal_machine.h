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
#ifndef HDMI_HAL_MACHINE_H
#define HDMI_HAL_MACHINE_H

#include "hdmi_product_define.h"

#define HDMI_MACH_MAX_STAMPE_NUM   6
#define HDMI_MACH_MAX_NAME_SIZE    15
#define HDMI_MACH_DEFAULT_INTERVAL 10

typedef td_void (*mach_callback)(td_void *data);

typedef struct {
    td_char *name;
    td_u64   interval;
    td_void *data;
    mach_callback callback;
} hdmi_mach_ctrl;

typedef struct {
    td_bool valid_id;
    td_bool enable;
    td_u64  enable_time;
    td_u64  disable_time;
    td_u64  last_time;
    td_u32  run_cnt;
    td_u32  stamp_idx;
    td_u64  timestamp[HDMI_MACH_MAX_STAMPE_NUM];
    td_char name[HDMI_MACH_MAX_NAME_SIZE];
} hdmi_mach_run;

typedef struct {
    hdmi_mach_run  mach_run;
    hdmi_mach_ctrl mach_ctrl;
} hdmi_mach_elem;

typedef struct {
    hdmi_mach_run  mach_run;
    hdmi_mach_ctrl mach_ctrl;
} hdmi_mach_elem_status;

typedef struct {
    td_bool init;
    td_u32  total;
} hdmi_mach_status;

td_s32 hal_hdmi_mach_init(hdmi_device_id hdmi);

td_s32 hal_hdmi_mach_deinit(hdmi_device_id hdmi);

td_s32 hal_hdmi_mach_invoke(hdmi_device_id hdmi);

td_s32 hal_hdmi_mach_register(hdmi_device_id hdmi, td_u32 *mach_id, const hdmi_mach_ctrl *mach_ctrl);

td_s32 hal_hdmi_mach_unregister(hdmi_device_id hdmi, td_u32 mach_id);

td_s32 hal_hdmi_mach_start(hdmi_device_id hdmi, td_u32 mach_id);

td_s32 hal_hdmi_mach_stop(hdmi_device_id hdmi, td_u32 mach_id);

td_u64 hal_hdmi_mach_ms_get(td_void);

#endif /* HDMI_HAL_MACHINE_H */

