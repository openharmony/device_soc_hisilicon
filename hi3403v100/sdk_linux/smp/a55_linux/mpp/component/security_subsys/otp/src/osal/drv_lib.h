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

#ifndef DRV_LIB_H
#define DRV_LIB_H

#include "ot_type.h"

td_void ot_otp_print_arr_u8(const td_char *name, const td_u8 *msg, td_u32 msg_len);

td_void ot_otp_print_arr_u32(const td_char *name, const td_u32 *msg, td_u32 msg_len);

td_s32 ot_otp_word_big_endian(const td_u32 word_val, td_u8 *byte_buf, td_u32 len);

td_u16 ot_otp_crc16_modbus(const td_u8 *msg, td_u32 msg_len);

td_s32 otp_copy_from_user(td_void *to, unsigned long to_len, const td_void *from, unsigned long from_len);

td_s32 otp_copy_to_user(td_void *to, unsigned long to_len, const td_void *from, unsigned long from_len);

extern int check_otp_cmd_mode(void);

td_s32 otp_get_cpu_secure_sta(td_void);

td_bool otp_is_secure_cpu(td_void);

#endif /* DRV_LIB_H */
