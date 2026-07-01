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
#ifndef HDMI_MOD_INIT_H
#define HDMI_MOD_INIT_H

#include "ot_defines.h"
#include "ot_type.h"
#include "ot_osal.h"

td_s32 hdmi_set_reg(td_u32 id, td_char *reg);
td_s32 hdmi_set_phy(td_u32 id, char *phy);
int hdmi_drv_mod_init(void);
void hdmi_drv_mod_exit(void);

#endif /* HDMI_MOD_INIT_H */

