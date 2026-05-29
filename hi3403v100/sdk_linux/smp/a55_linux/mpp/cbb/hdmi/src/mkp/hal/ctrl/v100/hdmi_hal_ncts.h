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
#ifndef HDMI_HAL_NCTS_H
#define HDMI_HAL_NCTS_H

#include "ot_type.h"

typedef struct {
    td_u32 audio_smp_rate;
    td_u32 tmds_clk;
    td_u32 n_value;
    td_u32 cts_value;
} hdmi_audio_cts_n;

td_u32 hal_hdmi_n_value_get(td_u32 sample_rate, td_u32 tmds_clk);

td_u32 hal_hdmi_cts_value_get(td_u32 sample_rate, td_u32 tmds_clk);

#endif /* HDMI_HAL_NCTS_H */

