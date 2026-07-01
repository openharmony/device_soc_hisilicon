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

#ifndef HAL_KLAD_H
#define HAL_KLAD_H

#include "drv_klad_sw_utils.h"

td_s32 hal_klad_init(td_void);

td_void hal_klad_deinit(td_void);

td_s32 hal_klad_common_route_startup(td_u32 slot_num, const klad_common_slot *common_slot);

td_s32 hal_klad_clear_route_startup(td_u32 slot_num, const klad_clear_slot *key);

#endif /* HAL_KLAD_H */

