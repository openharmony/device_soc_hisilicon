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
#ifndef DRV_HDMI_COMPATIBILITY_H
#define DRV_HDMI_COMPATIBILITY_H

#include "drv_hdmi_common.h"
#include "drv_hdmi_intf.h"

typedef struct {
    td_bool setmute_send; /* whether send AVMute */
    td_bool avi_use_gen5;
    td_bool avi_use_gen5_debug;
    td_u8   frl_compress_mode;
} compatibility_info;

td_s32 drv_hdmi_compat_avmute_get(hdmi_device_id hdmi_id, td_bool avmute, hdmi_avmute_cfg *avmute_cfg);

td_s32 drv_hdmi_compat_delay_get(hdmi_device_id hdmi_id, hdmi_delay *delay);

td_s32 drv_hdmi_compat_stop_delay(hdmi_device_id hdmi_id);

compatibility_info *compat_info_get(hdmi_device_id hdmi_id);

td_s32 compatibility_info_update(hdmi_device_id hdmi_id);

td_void compatibility_info_default_set(hdmi_device_id hdmi_id);

#endif  /* DRV_HDMI_COMPATIBILITY_H */

