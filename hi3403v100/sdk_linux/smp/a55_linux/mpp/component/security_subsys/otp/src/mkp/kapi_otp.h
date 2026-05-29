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

#ifndef KAPI_OTP_H
#define KAPI_OTP_H

#include "ot_common_otp.h"

td_s32 kapi_otp_init(td_void);

td_s32 kapi_otp_deinit(td_void);

td_s32 kapi_otp_set_user_data(const td_char *field_name,
    td_u32 offset, const td_u8 *value, td_u32 value_len);

td_s32 kapi_otp_get_user_data(const td_char *field_name,
    td_u32 offset, td_u8 *value, td_u32 value_len);

td_s32 kapi_otp_set_user_data_lock(const td_char *field_name,
    td_u32 offset, td_u32 value_len);

td_s32 kapi_otp_get_user_data_lock(const td_char *field_name,
    td_u32 offset, td_u32 value_len, ot_otp_lock_status *lock);

td_s32 kapi_otp_burn_product_pv(const ot_otp_burn_pv_item *pv, td_u32 num);

td_s32 kapi_otp_read_product_pv(ot_otp_burn_pv_item *pv, td_u32 num);

td_s32 kapi_otp_get_key_verify_status(const td_char *key_name, td_bool *status);

td_s32 kapi_otp_load_key_to_klad(const td_char *key_name);

#endif /* KAPI_OTP_H */
