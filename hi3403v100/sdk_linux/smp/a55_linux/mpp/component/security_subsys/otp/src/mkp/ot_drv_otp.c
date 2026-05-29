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

#include <linux/kernel.h>
#include "ot_drv_otp.h"
#include "kapi_otp.h"

#ifdef OT_OTP_V100
td_s32 ot_drv_otp_load_key_to_klad(const td_char *key_name)
{
    return kapi_otp_load_key_to_klad(key_name);
}
EXPORT_SYMBOL(ot_drv_otp_load_key_to_klad);
#endif
