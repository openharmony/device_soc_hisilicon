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

#ifndef HAL_OTP_COMM_H
#define HAL_OTP_COMM_H

#include "ot_type.h"

td_s32 hal_otp_init(td_void);

td_void hal_otp_deinit(td_void);

td_s32 hal_otp_read_word(td_u32 offset, td_u32 *value);

td_s32 hal_otp_write_word(td_u32 offset, td_u32 value);

#endif /* HAL_OTP_COMM_H */
