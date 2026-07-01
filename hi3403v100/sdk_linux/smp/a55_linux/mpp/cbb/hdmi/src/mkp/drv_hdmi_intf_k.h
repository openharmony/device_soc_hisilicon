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
#ifndef DRV_HDMI_INTF_K_H
#define DRV_HDMI_INTF_K_H

#include "drv_hdmi_common.h"

td_s32 drv_hdmi_kernel_event_callback(td_void *data, hdmi_event event);

td_s32 drv_hdmi_csc_param_set_export(hdmi_dev_id hdmi, const hdmi_csc_param *csc_param);

td_s32 drv_hdmi_video_param_set_export(hdmi_dev_id hdmi, const hdmi_video_param *video_param);

td_s32 drv_hdmi_stop_export(hdmi_dev_id hdmi);
#endif /* DRV_HDMI_INTF_K_H */

