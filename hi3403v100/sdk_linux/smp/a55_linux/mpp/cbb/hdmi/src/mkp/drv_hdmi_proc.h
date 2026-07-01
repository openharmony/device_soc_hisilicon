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
#ifndef DRV_HDMI_PROC_H
#define DRV_HDMI_PROC_H

#include "ot_type.h"
#include "hdmi_product_define.h"

td_s32 drv_hdmi_proc_show(hdmi_proc_entry *file);

td_s32 drv_hdmi_vo_proc_show(hdmi_proc_entry *file);

td_s32 drv_hdmi_ao_proc_show(hdmi_proc_entry *file);

td_s32 drv_hdmi_sink_proc_show(hdmi_proc_entry *file);

#endif /* DRV_HDMI_PROC_H */

