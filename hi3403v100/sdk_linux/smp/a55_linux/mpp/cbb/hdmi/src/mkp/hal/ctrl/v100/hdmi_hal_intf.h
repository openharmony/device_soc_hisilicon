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
#ifndef HDMI_HAL_INTF_H
#define HDMI_HAL_INTF_H

#include "drv_hdmi_common.h"
#include "hdmi_hal_ctrl.h"
#include "hdmi_hal_ddc.h"
#include "hdmi_hal_scdc.h"
#include "drv_hdmi_compatibility.h"

typedef struct {
    td_bool init;
    td_u32  tmds_clk;
    hdmi_tx_capability_data tx_capability;
} hdmi_hal_cfg;

typedef struct {
    hdmi_callback event_callback;
    td_void      *event_handle;
} hdmi_hal_event;

#endif /* HDMI_HAL_INTF_H */

